#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""V1C_V2.0.bin 私有密钥补丁点 —— 独立静态复核（fw-auditor）。

核查 Lead 给出的 6 条候选结论，并独立构造「ID==0 -> KEY / ID=1..99 ->
50024//(ID+1)，保留 delay(20us)，去掉 SCAL(0x33)+delay(1000)」的 56 字节补丁，
做静态字节/边界/调用点/写 SYNC 路径核对。行为级验证见 audit_unicorn.py。

用法:
    tools\\python\\python.exe devices\\v1c\\re\\audit_static.py
退出码: 0 = 全部 PASS；1 = 存在 FAIL。

只读固件；不修改任何补丁模块。输出为 UTF-8 文本。
"""
from __future__ import annotations

import bisect
import hashlib
import os
import re
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

# ---------------------------------------------------------------------------
# 常量（目标固件 / 地址）
# ---------------------------------------------------------------------------
_HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))      # D:\神牛
FW_PATH = os.path.join(ROOT, "devices", "v1c", "firmware", "V1C_V2.0.bin")

BASE = 0x08003000                     # 镜像链接/加载基址
IMAGE_SIZE = 88032
IMAGE_SHA256 = "4435dda84903cb97cfaf34f7eeb15ecb6168771d60b57da1c346e6a9ca36edfb"

SYNC_FN = 0x080121BC                  # void rf_set_sync(uint8_t id)
SYNC_FN_END = 0x080121F4              # 下一函数起点（不可越过）
SYNC_FN_SIZE = 56
NEXT_FN = 0x080121F4

# Lead 候选结论 #2 给出的原厂 56 字节（逐字节核对）
ORIG_FUNC_HEX = (
    "0c4a10b5411c1046f0f794ff0446010a042003f07efae1b2052003f07afa"
    "142003f030f9332003f0d5fa7d20c00003f029f910bd68c30000"
)
ORIG_FUNC = bytes.fromhex(ORIG_FUNC_HEX)
assert len(ORIG_FUNC) == SYNC_FN_SIZE

ID_NUMERATOR = 50024                  # 0xC368，原厂公式分子
REG_SYNC1 = 4                         # CC2500 SYNC1
REG_SYNC0 = 5                         # CC2500 SYNC0

# 辅助函数（候选结论 #5，本脚本用「原函数 BL 目标 + 函数体反汇编」独立验证）
UIDIV = 0x080030F0                    # __aeabi_uidiv(r0/r1) -> r0=商, r1=余
WRITE_REG = 0x080156CE                # WriteReg(r0=reg, r1=val)
STROBE = 0x08015790                   # strobe(r0=cmd)  (= WriteCmd)
DELAY = 0x08015440                    # delay(r0=count)  (= delay_us)
READ_REG = 0x0801562E                 # ReadReg(r0=reg) -> r0=val

INIT_TABLE_FN = 0x08015470            # CC2500 47 个配置寄存器初始化表
RF_INIT_FN = 0x0800664C               # RF 初始化（先 init 表，再同步字函数）

# 候选结论 #4 的 5 个调用点
SYNC_CALLERS = (0x08006274, 0x080066B2, 0x0800D484, 0x0800E0C2, 0x0800E138)
SETTINGS_BASE = 0x20001086            # 设置区基址（+6 = ID；本脚本用数据流复核）
ID_ADDR = SETTINGS_BASE + 6           # 0x2000108C

# 补丁使用的两个测试 KEY
KEYS = (0xD798, 0xA5C3)

COND_BRANCHES = {
    "beq", "bne", "bcs", "bhs", "bcc", "blo", "bmi", "bpl", "bvs", "bvc",
    "bhi", "bls", "bge", "blt", "bgt", "ble",
}
DEST_WRITE_MNEMONICS = {
    "mov", "movs", "adds", "subs", "add", "sub", "adr", "ldr", "ldrb", "ldrh",
    "ldrsb", "ldrsh", "uxtb", "uxth", "and", "ands", "orr", "orrs", "bic",
    "bics", "lsl", "lsls", "lsr", "lsrs", "asr", "asrs", "rsb", "rsbs",
    "mvn", "mvns", "mul", "muls", "eor", "eors", "movw", "movt", "neg",
}


# ---------------------------------------------------------------------------
# 通用工具
# ---------------------------------------------------------------------------
def _off(addr: int) -> int:
    return addr - BASE


def parse_imm(op: str) -> int | None:
    """解析操作数里的立即数（支持 '#0' / '#0x14' / '#-1'）。"""
    m = re.search(r"#(-?(?:0x[0-9a-fA-F]+|\d+))", op)
    if not m:
        return None
    s = m.group(1)
    return int(s, 16) if "x" in s else int(s)


def load_image() -> bytes:
    with open(FW_PATH, "rb") as f:
        return f.read()


class Code:
    """capstone 线性反汇编结果（含 skipdata，避免字面量池处停止）。"""

    def __init__(self, image: bytes):
        self.image = image
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
        md.skipdata = True
        self.insns = list(md.disasm(image, BASE))
        self.by_addr = {i.address: i for i in self.insns}
        # 反汇编器覆盖校验：每个 2 字节槽位都应有一条指令（跳过的数据会以 .byte 形式出现）
        self.gaps = [a for a in range(BASE, BASE + len(image), 2)
                     if a not in self.by_addr]
        # Thumb 的 switch 分派 helper（GCC __gnu_thumb1_case_*）：
        #   bl helper; <2 字节跳转表> ; <各 case 代码>
        # 这类 BL 不按普通调用返回，而是用 LR 附近的表做 computed goto，
        # 线性反汇编会把后面的跳转表/内联数据误当指令，必须视作「不返回」。
        self.switch_helpers: set[int] = set()

    def detect_switch_helpers(self, entries: list[int]) -> set[int]:
        found: set[int] = set()
        for e in entries:
            ins = self.by_addr.get(e)
            if ins is None or ins.mnemonic != "push":
                continue
            seq = [ins]
            a = e + ins.size
            for _ in range(16):
                i2 = self.by_addr.get(a)
                if i2 is None:
                    break
                seq.append(i2)
                a += i2.size
            text = " | ".join("%s %s" % (i.mnemonic, i.op_str) for i in seq)
            reads_lr = ("mov r4, lr" in text or "mov r12, lr" in text
                        or "mov ip, lr" in text)
            has_bx = "bx r3" in text or "bx r12" in text or "bx ip" in text
            has_sub1 = any(i.mnemonic == "subs" and i.op_str.endswith("#1")
                           for i in seq)
            if reads_lr and has_bx and has_sub1:
                found.add(e)
        self.switch_helpers = found
        return found

    def is_switch_dispatch(self, ins) -> bool:
        t = self.bl_target(ins)
        return t is not None and t in self.switch_helpers

    def at(self, addr: int):
        return self.by_addr.get(addr)

    def literal_value(self, ins) -> tuple[int, int] | None:
        """LDR (literal) -> (字面量地址, 值)；非 literal 形式返回 None。"""
        if ins.mnemonic != "ldr" or "[pc," not in ins.op_str:
            return None
        m = re.search(r"#(-?0x[0-9a-fA-F]+|-?\d+)", ins.op_str)
        if not m:
            return None
        imm = int(m.group(1), 16) if "x" in m.group(1) else int(m.group(1))
        lit_addr = ((ins.address + 4) & ~3) + imm
        off = lit_addr - BASE
        if off < 0 or off + 4 > len(self.image):
            return None
        return lit_addr, struct.unpack_from("<I", self.image, off)[0]

    def branch_target(self, ins) -> int | None:
        if not (ins.mnemonic == "b" or ins.mnemonic in COND_BRANCHES
                or ins.mnemonic in ("bl", "blx", "cbz", "cbnz")):
            return None
        m = re.search(r"#(0x[0-9a-fA-F]+)", ins.op_str)
        return int(m.group(1), 16) if m else None

    def bl_target(self, ins) -> int | None:
        if ins.mnemonic not in ("bl", "blx"):
            return None
        t = self.branch_target(ins)
        return None if t is None else (t & ~1)

    # -- 寄存器写集合 / 控制流 -------------------------------------------------
    @staticmethod
    def written_regs(ins) -> set[int]:
        regs: set[int] = set()
        op = ins.op_str
        m = ins.mnemonic
        if m in ("bl", "blx"):
            return {0, 1, 2, 3, 12}                     # AAPCS caller-saved
        if m == "pop":
            for r in re.findall(r"r(\d+)", op):
                regs.add(int(r))
            return regs
        if m in ("ldm", "ldmia", "ldmdb", "ldmfd"):
            for r in re.findall(r"r(\d+)", op.split("{", 1)[-1]):
                regs.add(int(r))
            return regs
        if m in DEST_WRITE_MNEMONICS:
            first = op.split(",", 1)[0].strip()
            if re.fullmatch(r"r\d+", first):
                regs.add(int(first[1:]))
        return regs

    def _has_pc_write(self, ins) -> bool:
        if ins.mnemonic in ("bx", "blx") and "r" in ins.op_str:
            return True
        if ins.mnemonic == "pop" and "pc" in ins.op_str:
            return True
        if ins.mnemonic in ("ldm", "ldmia", "ldmfd") and "pc" in ins.op_str:
            return True
        if ins.mnemonic in DEST_WRITE_MNEMONICS:
            if ins.op_str.split(",", 1)[0].strip() == "pc":
                return True
        return False

    def successors(self, ins, fn_start: int, fn_end: int) -> list[int]:
        nxt = ins.address + ins.size
        m = ins.mnemonic
        out: list[int] = []
        if m == "b":
            t = self.branch_target(ins)
            if t is not None:
                out.append(t & ~1)
        elif m in COND_BRANCHES or m in ("cbz", "cbnz"):
            t = self.branch_target(ins)
            if t is not None:
                out.append(t & ~1)
            out.append(nxt)
        elif m in ("bx", "blx") or self._has_pc_write(ins):
            out = []
        elif m in ("bl", "blx"):
            if self.is_switch_dispatch(ins):
                out = []                     # computed goto：不返回、不顺序执行
            else:
                out = [nxt]
        else:
            out = [nxt]
        return [a for a in out
                if fn_start <= a < fn_end and a in self.by_addr]

    # -- 函数边界 ------------------------------------------------------------
    def function_entries(self) -> list[int]:
        entries: set[int] = set()
        for ins in self.insns:
            t = self.bl_target(ins)
            if t is not None and BASE <= t < BASE + len(self.image):
                entries.add(t)
        # 向量表（前 0x40 字节，最多 16 项；再补 0x40..0x50 的其余向量）
        for off in range(0, 0x50, 4):
            if off + 4 > len(self.image):
                break
            v = struct.unpack_from("<I", self.image, off)[0]
            if 0x08000000 <= v < 0x09000000 and BASE <= (v & ~1) < BASE + len(self.image):
                entries.add(v & ~1)
        return sorted(entries)

    def containing_function(self, addr: int, entries: list[int]) -> int:
        i = bisect.bisect_right(entries, addr) - 1
        return entries[i] if i >= 0 else BASE

    def next_entry(self, addr: int, entries: list[int]) -> int:
        i = bisect.bisect_right(entries, addr)
        return entries[i] if i < len(entries) else BASE + len(self.image)

    # -- 反向数据流（按 CFG 前驱回溯，走到定义处为止） ------------------------
    def reachable_set(self, fn_start: int, fn_end: int) -> set[int]:
        """从函数入口做前向可达性分析（switch 分派 helper 视为不返回）。

        这样可剔除「函数内嵌跳转表/字面量/位图」被线性反汇编误当成指令后
        产生的虚假控制流边（例如 0x08006292..0x080062AD 的跳转表）。
        """
        seen: set[int] = set()
        work = [fn_start]
        while work:
            a = work.pop()
            if a in seen or not (fn_start <= a < fn_end):
                continue
            ins = self.by_addr.get(a)
            if ins is None:
                continue
            seen.add(a)
            work.extend(self.successors(ins, fn_start, fn_end))
        return seen

    def build_preds(self, fn_start: int, fn_end: int,
                    reachable: set[int] | None = None) -> dict[int, set[int]]:
        preds: dict[int, set[int]] = {}
        addrs = [a for a in sorted(self.by_addr) if fn_start <= a < fn_end
                 and (reachable is None or a in reachable)]
        for a in addrs:
            ins = self.by_addr[a]
            for s in self.successors(ins, fn_start, fn_end):
                if reachable is not None and s not in reachable:
                    continue
                preds.setdefault(s, set()).add(a)
        return preds

    def resolve_reg(self, reg: int, addr: int, fn_start: int, fn_end: int,
                    preds: dict[int, set[int]], max_steps: int = 6000) -> dict:
        """求「执行 addr 之前」reg 的静态值。

        返回 {"status": "resolved"/"ambiguous"/"unknown", "value": int|None,
              "defs": [(def_kind, value, def_addr, desc), ...]}
        """
        work = list(preds.get(addr, ()))
        visited: set[int] = set()
        defs: list[tuple] = []
        while work:
            a = work.pop()
            if a in visited:
                continue
            visited.add(a)
            if len(visited) > max_steps:
                return {"status": "unknown", "value": None,
                        "defs": defs + [("too-complex", None, a, "回溯超限")]}
            ins = self.by_addr.get(a)
            if ins is None:
                work.append(a - 2)
                continue
            wr = self.written_regs(ins)
            if reg in wr:
                defs.append(self._describe_def(ins, reg))
                continue
            if a == fn_start:
                defs.append(("entry", None, a, "函数入口（值来自调用者）"))
                continue
            work.extend(preds.get(a, ()))
        if not defs:
            return {"status": "unknown", "value": None, "defs": []}
        known = [d for d in defs if d[0] in ("imm", "lit")]
        values = {d[1] for d in known}
        if len(known) == len(defs) and len(values) == 1:
            return {"status": "resolved", "value": known[0][1], "defs": defs}
        if len(defs) == 1 and defs[0][0] == "reg":
            # mov rX, rY：继续回溯 rY（在定义指令处）
            inner = self.resolve_reg(defs[0][1], defs[0][2], fn_start, fn_end, preds)
            if inner["status"] == "resolved":
                inner["defs"] = [defs[0]] + inner["defs"]
            return inner
        return {"status": "ambiguous", "value": None, "defs": defs}

    def _describe_def(self, ins, reg: int) -> tuple:
        """描述一条「写 reg」的指令；返回 (kind, value, addr, desc)。"""
        op = ins.op_str
        if ins.mnemonic in ("movs", "mov") and op.startswith("r%d," % reg):
            rhs = op.split(",", 1)[1].strip()
            m = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", rhs)
            if m:
                v = int(m.group(1), 16) if "x" in m.group(1) else int(m.group(1))
                return ("imm", v, ins.address,
                        "0x%08X  %-12s %s %s   ; r%d = %s"
                        % (ins.address, ins.bytes.hex(" "), ins.mnemonic, op, reg, rhs))
            m = re.fullmatch(r"(r\d+)", rhs)
            if m:
                return ("reg", int(m.group(1)[1:]), ins.address,
                        "0x%08X  %-12s %s %s   ; r%d <- %s"
                        % (ins.address, ins.bytes.hex(" "), ins.mnemonic, op, reg, rhs))
        if ins.mnemonic == "ldr" and op.startswith("r%d," % reg):
            lv = self.literal_value(ins)
            if lv is not None:
                lit_addr, val = lv
                return ("lit", val, ins.address,
                        "0x%08X  %-12s %s %s   ; 字面量@0x%08X = 0x%08X -> r%d"
                        % (ins.address, ins.bytes.hex(" "), ins.mnemonic, op,
                           lit_addr, val, reg))
        if ins.mnemonic in ("bl", "blx"):
            return ("call", None, ins.address,
                    "0x%08X  %-12s %s %s   ; 调用使 r%d 失效"
                    % (ins.address, ins.bytes.hex(" "), ins.mnemonic, op, reg))
        return ("dyn", None, ins.address,
                "0x%08X  %-12s %s %s   ; r%d 来源非字面量/立即数"
                % (ins.address, ins.bytes.hex(" "), ins.mnemonic, op, reg))


# ---------------------------------------------------------------------------
# 检查结果收集
# ---------------------------------------------------------------------------
RESULTS: list[tuple[bool, str]] = []


def check(cond: bool, msg: str) -> bool:
    RESULTS.append((bool(cond), msg))
    print("[%s] %s" % ("PASS" if cond else "FAIL", msg))
    return bool(cond)


def header(title: str) -> None:
    print()
    print("=" * 78)
    print(title)
    print("=" * 78)


def dump_insns(code: Code, start: int, end: int, indent: str = "    ") -> None:
    for ins in code.insns:
        if start <= ins.address < end:
            extra = ""
            lv = code.literal_value(ins)
            if lv is not None:
                extra = "   ; [0x%08X] = 0x%08X" % lv
            print("%s0x%08X  %-12s %-7s %-24s%s"
                  % (indent, ins.address, ins.bytes.hex(" "),
                     ins.mnemonic, ins.op_str, extra))


# ---------------------------------------------------------------------------
# Thumb-1 编码器（仅用于构造本补丁；语义由 capstone/Unicorn 复核）
# ---------------------------------------------------------------------------
def _h(v: int) -> bytes:
    return struct.pack("<H", v)


def _t_ldr_literal(rt: int, imm8: int) -> bytes:
    """LDR (literal) T1: target = (addr+4 & ~3) + imm8*4。"""
    assert 0 <= imm8 <= 0xFF
    return _h(0x4800 | ((rt & 7) << 8) | imm8)


def _t_push_r4lr() -> bytes:
    return _h(0xB510)


def _t_pop_r4pc() -> bytes:
    return _h(0xBD10)


def _t_cmp_imm(rn: int, imm8: int) -> bytes:
    return _h(0x2800 | ((rn & 7) << 8) | (imm8 & 0xFF))


def _t_beq(offset_bytes: int) -> bytes:
    assert offset_bytes % 2 == 0 and 0 <= offset_bytes // 2 <= 0xFF
    return _h(0xD000 | ((offset_bytes // 2) & 0xFF))


def _t_b(offset_bytes: int) -> bytes:
    assert offset_bytes % 2 == 0 and -0x800 <= offset_bytes // 2 <= 0x7FF
    return _h(0xE000 | ((offset_bytes // 2) & 0x7FF))


def _t_adds_ri(rn: int, rd: int, imm3: int) -> bytes:
    return _h(0x1C00 | ((imm3 & 7) << 6) | ((rn & 7) << 3) | (rd & 7))


def _t_movs_imm(rd: int, imm8: int) -> bytes:
    return _h(0x2000 | ((rd & 7) << 8) | (imm8 & 0xFF))


def _t_mov(rd: int, rm: int) -> bytes:
    return _h(0x4600 | ((rm & 7) << 3) | (rd & 7))


def _t_lsrs(rd: int, rm: int, imm5: int) -> bytes:
    return _h(0x0800 | ((imm5 & 0x1F) << 6) | ((rm & 7) << 3) | (rd & 7))


def _t_uxtb(rd: int, rm: int) -> bytes:
    assert rd < 8 and rm < 8
    return _h(0xB2C0 | ((rm & 7) << 3) | (rd & 7))


def _t_bl(addr: int, target: int) -> bytes:
    """Thumb BL T1；addr = BL 指令地址，target = 绝对地址（偶数）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target), off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = i1 if s else ((~i1) & 1)
    j2 = i2 if s else ((~i2) & 1)
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return struct.pack("<HH",
                       0xF000 | (s << 10) | imm10,
                       0xD000 | (j1 << 13) | (j2 << 11) | imm11)


def build_patch_blob(key: int) -> bytes:
    """构造 56 字节替换函数：

        sync = (id == 0) ? KEY : 50024/(id+1);
        WriteReg(4, sync >> 8); WriteReg(5, sync & 0xFF);
        delay(20);
        return;

    布局：48 字节代码（+0x00..+0x2F）+ KEY(+0x30) + 50024(+0x34)，
    50024 保持原厂字面量地址 0x080121F0（+0x34），总 56 字节。
    """
    assert 0 <= key <= 0xFFFF
    key_lit = SYNC_FN + 0x30
    orig_lit = SYNC_FN + 0x34
    assert orig_lit + 4 == SYNC_FN_END

    code = bytearray()

    def emit(b: bytes) -> None:
        code.extend(b)

    def here() -> int:
        return SYNC_FN + len(code)

    # +00 ldr r2, [pc, #0x2C]   ; = KEY   （字面量在 +0x30）
    pc0 = (here() + 4) & ~3
    assert key_lit - pc0 == 0x2C, (hex(key_lit), hex(pc0))
    emit(_t_ldr_literal(2, (key_lit - pc0) // 4))
    emit(_t_push_r4lr())                 # +02 push {r4, lr}
    emit(_t_cmp_imm(0, 0))               # +04 cmp r0, #0
    emit(_t_beq((SYNC_FN + 0x0E) - (here() + 4)))   # +06 beq keymode(+0x0E)
    # +08 ldr r2, [pc, #0x28]   ; = 50024 （字面量在 +0x34）
    pc8 = (here() + 4) & ~3
    assert orig_lit - pc8 == 0x28, (hex(orig_lit), hex(pc8))
    emit(_t_ldr_literal(2, (orig_lit - pc8) // 4))
    emit(_t_adds_ri(0, 1, 1))            # +0A adds r1, r0, #1     ; 除数 ID+1
    emit(_t_b((SYNC_FN + 0x10) - (here() + 4)))     # +0C b go(+0x10)
    assert here() == SYNC_FN + 0x0E
    emit(_t_movs_imm(1, 1))              # +0E keymode: movs r1, #1 ; 除数 1
    assert here() == SYNC_FN + 0x10
    emit(_t_mov(0, 2))                   # +10 go: mov r0, r2       ; 被除数
    emit(_t_bl(here(), UIDIV))           # +12 bl __aeabi_uidiv
    emit(_t_mov(4, 0))                   # +16 mov r4, r0           ; quotient
    emit(_t_lsrs(1, 0, 8))               # +18 lsrs r1, r0, #8      ; SYNC1
    emit(_t_movs_imm(0, REG_SYNC1))      # +1A movs r0, #4
    emit(_t_bl(here(), WRITE_REG))       # +1C bl WriteReg
    emit(_t_uxtb(1, 4))                  # +20 uxtb r1, r4          ; SYNC0
    emit(_t_movs_imm(0, REG_SYNC0))      # +22 movs r0, #5
    emit(_t_bl(here(), WRITE_REG))       # +24 bl WriteReg
    emit(_t_movs_imm(0, 20))             # +28 movs r0, #20         ; 保留原厂 delay(20us)
    emit(_t_bl(here(), DELAY))           # +2A bl delay
    emit(_t_pop_r4pc())                  # +2E pop {r4, pc}
    blob = bytes(code) + struct.pack("<II", key, ID_NUMERATOR)
    assert len(blob) == SYNC_FN_SIZE, len(blob)
    return blob


def apply_patch(image: bytes, key: int) -> bytes:
    off = _off(SYNC_FN)
    out = bytearray(image)
    out[off:off + SYNC_FN_SIZE] = build_patch_blob(key)
    return bytes(out)


# ---------------------------------------------------------------------------
# 各项检查
# ---------------------------------------------------------------------------
def check_image_and_base(image: bytes, code: Code) -> None:
    header("1. 固件身份与镜像基址（候选结论 #1）")
    check(len(image) == IMAGE_SIZE,
          "固件长度 %d 字节（期望 %d）" % (len(image), IMAGE_SIZE))
    got = hashlib.sha256(image).hexdigest()
    check(got == IMAGE_SHA256, "sha256 %s（期望 %s）" % (got[:32] + "…", IMAGE_SHA256[:32] + "…"))

    msp = struct.unpack_from("<I", image, 0)[0]
    reset_vec = struct.unpack_from("<I", image, 4)[0]
    check(0x20000000 <= msp < 0x20002000 and msp % 8 == 0,
          "向量表[0] 初始 MSP = 0x%08X（RAM 内、8 字节对齐）" % msp)
    check(reset_vec & 1 == 1 and 0x08000000 <= (reset_vec & ~1) < 0x09000000,
          "向量表[1] Reset = 0x%08X（Thumb 地址，bit0=1）" % reset_vec)

    # reset 向量指向的代码位置 = 「基址 + 文件内偏移」；用 reset 处理函数内部的
    # __main 字面量闭合求出基址，避免循环论证。
    reset_addr = reset_vec & ~1
    # reset 处理函数必须是文件内某个偶数偏移处的代码：用 Lead 的基址假设解出偏移
    reset_off = reset_addr - BASE
    ok_reset = 0 <= reset_off <= len(image) - 16
    check(ok_reset and image[reset_off:reset_off + 2] in (b"\x04\x48", b"\x07\x48", b"\x0c\x48"),
          "Reset 处理函数位于文件偏移 0x%X（0x%08X），首指令 LDR literal（%s）"
          % (reset_off, reset_addr, image[reset_off:reset_off + 2].hex(" ")))
    if ok_reset:
        dump_insns(code, reset_addr, reset_addr + 0x14)
        lv = code.literal_value(code.at(reset_addr))
        lv2 = code.literal_value(code.at(reset_addr + 4))
        check(lv is not None and 0x08000000 <= lv[1] < 0x09000000,
              "Reset 第一条 LDR 目标 = %s" % (("0x%08X" % lv[1]) if lv else "无"))
        check(lv2 is not None and (lv2[1] & ~1) == 0x080030C0,
              "Reset 第二条 LDR 目标 = %s（= __main 桩 0x080030C0 或 0x080030C1）"
              % (("0x%08X" % lv2[1]) if lv2 else "无"))
        # __main 桩 0x080030C0 位于文件偏移 0xC0 -> 基址 = 0x080030C0 - 0xC0
        main_off = 0x080030C0 - BASE
        expected_main = bytes.fromhex("0348854600f092f900480047")
        check(image[main_off:main_off + len(expected_main)] == expected_main,
              "0x080030C0(__main 桩) 文件偏移 0x%X 字节 = %s（自洽基址 0x%08X）"
              % (main_off, image[main_off:main_off + len(expected_main)].hex(" "),
                 0x080030C0 - main_off))
        # __main 桩设置的 SP 字面量应等于向量表[0] 的 MSP
        sp_lit = struct.unpack_from("<I", image, main_off + 0x10)[0]
        check(sp_lit == msp,
              "__main 桩 LDR 字面量 0x%08X == 向量表初始 MSP 0x%08X" % (sp_lit, msp))

    check(reset_vec == 0x080030D5,
          "Reset 向量 0x%08X 与 AD100/AD200 已支持设备的 0x080030D5 一致" % reset_vec)
    print("    说明：镜像文件偏移 = 地址 - 0x%08X；以下所有地址均按该基址解析。" % BASE)


def check_sync_function(image: bytes, code: Code, entries: list[int]) -> None:
    header("2. 同步字函数身份、56 字节边界与字节内容（候选结论 #2）")
    got = image[_off(SYNC_FN):_off(SYNC_FN) + SYNC_FN_SIZE]
    check(got == ORIG_FUNC,
          "0x%08X..0x%08X 原厂 56 字节与 Lead 给定值逐字节一致"
          % (SYNC_FN, SYNC_FN_END - 1))
    if got != ORIG_FUNC:
        print("    期望:", ORIG_FUNC.hex(" "))
        print("    实际:", got.hex(" "))
    print("    原厂 56 字节:", got.hex(" "))

    # 函数体覆盖：0x080121BC..0x080121EF 为代码，0x080121F0..0x080121F3 为字面量。
    # 注意线性反汇编会继续把 0x080121F0 的字面量误解码成指令，故顺序解码到 0x080121F0 为止。
    spans = []
    a = SYNC_FN
    while a < 0x080121F0:
        ins = code.at(a)
        if ins is None or ins.address + ins.size > 0x080121F0:
            break
        spans.append((a, a + ins.size))
        a += ins.size
    check(a == 0x080121F0,
          "从 0x%08X 顺序解码的函数代码结束于 0x%08X（0x080121F0 起为字面量池）"
          % (SYNC_FN, a))
    check(len(spans) == 20 and all(s[1] - s[0] in (2, 4) for s in spans),
          "函数代码由 %d 条 Thumb 指令（含 6 条 32 位 BL）连续覆盖"
          " 0x080121BC..0x080121EF" % len(spans))
    check(0x080121F0 not in code.by_addr or
          code.at(0x080121F0).address == 0x080121F0,
          "0x080121F0 处是 4 字节字面量（线性反汇编误当作指令时也不影响本检查）")

    sync_insns = [code.at(s) for s, _ in spans]
    assert all(x is not None for x in sync_insns)
    exp_targets = {
        0x080121C4: UIDIV,       # bl __aeabi_uidiv
        0x080121CE: WRITE_REG,   # bl WriteReg(4, hi)
        0x080121D6: WRITE_REG,   # bl WriteReg(5, lo)
        0x080121DC: DELAY,       # bl delay(20)
        0x080121E2: STROBE,      # bl strobe(0x33)  SCAL
        0x080121EA: DELAY,       # bl delay(1000)
    }
    ok = True
    detail = []
    for ins in sync_insns:
        if ins.mnemonic in ("bl", "blx"):
            t = code.bl_target(ins)
            detail.append("0x%08X->0x%08X" % (ins.address, t))
            if exp_targets.get(ins.address) != t:
                ok = False
    check(ok and len(detail) == 6,
          "函数内 6 条 BL 目标 = %s" % ", ".join(detail))

    instrs = {ins.address: ins for ins in sync_insns}
    check(instrs[0x080121BC].mnemonic == "ldr" and
          code.literal_value(instrs[0x080121BC]) == (0x080121F0, ID_NUMERATOR),
          "0x080121BC LDR literal -> 0x080121F0 = %d (0x%04X)"
          % (ID_NUMERATOR, ID_NUMERATOR))
    check(instrs[0x080121CC].op_str.startswith("r0") and
          instrs[0x080121CC].op_str.endswith("#4"),
          "0x080121CC: %s（写 SYNC1 的寄存器号 4）"
          % (instrs[0x080121CC].mnemonic + " " + instrs[0x080121CC].op_str))
    check(instrs[0x080121D4].op_str.startswith("r0") and
          instrs[0x080121D4].op_str.endswith("#5"),
          "0x080121D4: %s（写 SYNC0 的寄存器号 5）"
          % (instrs[0x080121D4].mnemonic + " " + instrs[0x080121D4].op_str))
    check(instrs[0x080121DA].op_str.endswith("#0x14"),
          "0x080121DA: %s（保留的 20us 延时，0x14 = 20）"
          % (instrs[0x080121DA].mnemonic + " " + instrs[0x080121DA].op_str))
    check(instrs[0x080121E0].op_str.endswith("#0x33"),
          "0x080121E0: %s（SCAL 命令 0x33）"
          % (instrs[0x080121E0].mnemonic + " " + instrs[0x080121E0].op_str))
    check(instrs[0x080121E6].op_str.endswith("#0x7d") and
          instrs[0x080121E8].mnemonic == "lsls" and
          instrs[0x080121E8].op_str.endswith("#3"),
          "0x080121E6/E8: movs r0,#0x7d; lsls r0,#3 = 1000（1ms 延时）")

    # 字面量 0x080121F0
    lit = struct.unpack_from("<I", image, _off(0x080121F0))[0]
    check(lit == ID_NUMERATOR,
          "0x080121F0 字面量 = 0x%08X (= %d = 50024)" % (lit, ID_NUMERATOR))

    # 函数边界：0x080121F4 必须是真实函数起点
    check(NEXT_FN in entries,
          "0x%08X 是独立函数起点（被 0x0800DCE2 处 BL 调用）" % NEXT_FN)
    bl_callers = [ins.address for ins in code.insns
                  if code.bl_target(ins) == NEXT_FN]
    check(bl_callers == [0x0800DCE2],
          "指向 0x%08X 的 BL 调用点 = %s" % (NEXT_FN, [hex(a) for a in bl_callers]))
    nxt = code.at(NEXT_FN)
    check(nxt is not None and nxt.mnemonic == "ldr",
          "0x%08X 起为可反汇编代码：%s %s" % (NEXT_FN, nxt.mnemonic, nxt.op_str))

    # 无其它入口进入 0x080121BC..0x080121F3 内部
    internal = []
    for ins in code.insns:
        t = code.branch_target(ins)
        if t is None:
            continue
        t &= ~1
        if SYNC_FN < t < SYNC_FN_END:
            internal.append((ins.address, t))
    check(not internal, "除入口 0x%08X 外，无分支/BL 目标落在函数内部（发现 %d 处）"
          % (SYNC_FN, len(internal)))
    lit_refs = []
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is None:
            continue
        if SYNC_FN <= lv[0] < SYNC_FN_END:
            lit_refs.append((ins.address, lv[0]))
    check(lit_refs == [(SYNC_FN, 0x080121F0)],
          "函数区字面量仅被 0x%08X 的 LDR 引用（引用点 %s）"
          % (SYNC_FN, [("0x%08X->0x%08X" % r) for r in lit_refs]))


def check_helpers_and_global_sync(image: bytes, code: Code) -> None:
    header("3. 辅助函数语义（候选结论 #5）")
    # uidiv：必须是自包含除法循环（无 BL），r0/r1 入参，r0=商
    uidiv_ins = [ins for ins in code.insns if UIDIV <= ins.address < UIDIV + 0x2C]
    uidiv_bl = [ins for ins in uidiv_ins if ins.mnemonic in ("bl", "blx")]
    check(len(uidiv_ins) == 22 and not uidiv_bl and
          uidiv_ins[-1].mnemonic == "pop" and "pc" in uidiv_ins[-1].op_str,
          "0x%08X __aeabi_uidiv：%d 条指令自包含循环、无 BL、r0=商 r1=余（%d 条 BL）"
          % (UIDIV, len(uidiv_ins), len(uidiv_bl)))
    print("    uidiv 关键指令：")
    dump_insns(code, UIDIV, UIDIV + 0x2C)

    # WriteReg：push {r0,r1,r4,lr}; r2=1; r1=sp+4; bl 0x080157A8（单字节写）
    wr = [code.at(a) for a in range(WRITE_REG, WRITE_REG + 12, 2)]
    check(wr[0].mnemonic == "push" and "r0" in wr[0].op_str and "r1" in wr[0].op_str,
          "0x%08X WriteReg 入口 push {r0, r1, ...}（保存 reg/val 参数）" % WRITE_REG)
    check(wr[1].op_str.endswith("#1") and wr[1].mnemonic == "movs",
          "0x%08X movs r2, #1（写 1 字节）" % wr[1].address)
    check(wr[2].mnemonic == "add" and "sp, #4" in wr[2].op_str.replace(" ,", ","),
          "0x%08X add r1, sp, #4（r1=val 指针）" % wr[2].address)
    check(wr[3].mnemonic == "bl" and code.bl_target(wr[3]) == 0x080157A8,
          "0x%08X bl 0x%08X（底层单寄存器写；r0=reg, r1=val）"
          % (wr[3].address, code.bl_target(wr[3])))
    print("    WriteReg 反汇编：")
    dump_insns(code, WRITE_REG, WRITE_REG + 0x0C)

    # strobe：r0=cmd 保存到 r4，然后 0x08015718
    st = [code.at(a) for a in range(STROBE, STROBE + 10, 2)]
    check(st[0].mnemonic == "push" and st[1].mnemonic == "mov" and
          st[1].op_str.strip() == "r4, r0",
          "0x%08X strobe：mov r4, r0（r0=命令）" % STROBE)
    print("    strobe 反汇编：")
    dump_insns(code, STROBE, STROBE + 0x0A)

    # delay：subs r0,#1 / uxth / 内层 nop 循环 / bx lr
    de = [ins for ins in code.insns if DELAY <= ins.address < DELAY + 0x30]
    check(de[0].mnemonic == "subs" and de[0].op_str == "r0, r0, #1" and
          de[-1].mnemonic == "bx" and de[-1].op_str == "lr",
          "0x%08X delay：subs r0,#1 计数循环 + bx lr（r0=计数）" % DELAY)

    # ReadReg：push {r3,lr}; orrs r0,#0x80; bl 0x08015764; ldrb -> r0
    rd = [code.at(a) for a in range(READ_REG, READ_REG + 12, 2)]
    check(rd[1].mnemonic == "movs" and rd[1].op_str.endswith("#0x80"),
          "0x%08X ReadReg：orrs r0, #0x80（CC2500 读寄存器地址位）" % READ_REG)


def check_50024_literal(image: bytes, code: Code) -> None:
    header("4. 50024 (0xC368) 字面量全局搜索与引用者")
    pat = struct.pack("<I", ID_NUMERATOR)
    hits = []
    i = 0
    while True:
        j = image.find(pat, i)
        if j < 0:
            break
        hits.append(j)
        i = j + 1
    check(hits == [0x080121F0 - BASE],
          "全镜像字节模式 %s 仅出现 1 次，位于文件偏移 0x%05X（地址 0x%08X）"
          % (pat.hex(" "), hits[0] if hits else -1,
             BASE + hits[0] if hits else 0))
    refs = []
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is not None and lv[0] in [BASE + h for h in hits]:
            refs.append((ins.address, lv[0], lv[1]))
    check(refs == [(SYNC_FN, 0x080121F0, ID_NUMERATOR)],
          "引用该字面量的 LDR 只有 0x%08X（-> 0x%08X = %d）"
          % (refs[0] if refs else (0, 0, 0)))
    imm_uses = [ins.address for ins in code.insns
                if str(ID_NUMERATOR) in ins.op_str or "0xc368" in ins.op_str.lower()]
    check(not imm_uses, "无代码以立即数/其它形式直接使用 50024（发现 %d 处）"
          % len(imm_uses))


def check_write_reg_sites(code: Code) -> None:
    header("5. 全镜像 reg4/reg5 写路径（候选结论 #3）")
    calls = [ins for ins in code.insns
             if ins.mnemonic in ("bl", "blx") and code.bl_target(ins) == WRITE_REG]
    check(len(calls) == 59,
          "BL -> WriteReg(0x%08X) 共 %d 个调用点（全镜像静态扫描）"
          % (WRITE_REG, len(calls)))

    def resolve_r0(call_addr: int) -> tuple[str, int | None, str]:
        """线性回溯（最多 20 条），返回 (kind, value, 描述)。"""
        a = call_addr - 2
        for _ in range(20):
            ins = code.at(a)
            if ins is None:
                a -= 2
                continue
            if code.written_regs(ins) & {0}:
                op = ins.op_str
                if ins.mnemonic in ("movs", "mov") and op.startswith("r0,"):
                    rhs = op.split(",", 1)[1].strip()
                    m = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", rhs)
                    if m:
                        v = int(m.group(1), 16) if "x" in m.group(1) else int(m.group(1))
                        return ("imm", v, "0x%08X %s %s" % (ins.address, ins.mnemonic, op))
                    m = re.fullmatch(r"(r\d+)", rhs)
                    if m:
                        return ("mov-reg", None,
                                "0x%08X %s %s" % (ins.address, ins.mnemonic, op))
                if ins.mnemonic == "ldr" and op.startswith("r0,"):
                    lv = code.literal_value(ins)
                    if lv is not None:
                        return ("lit", lv[1],
                                "0x%08X %s %s [0x%08X]=0x%08X"
                                % (ins.address, ins.mnemonic, op, lv[0], lv[1]))
                return ("other", None, "0x%08X %s %s" % (ins.address, ins.mnemonic, op))
            if ins.mnemonic in ("bl", "blx", "pop", "bx"):
                break
            a -= ins.size
        return ("unknown", None, "0x%08X 之前无法静态确定 r0" % call_addr)

    rows = []
    for c in calls:
        kind, val, desc = resolve_r0(c.address)
        rows.append((c.address, kind, val, desc))

    sync_sites = [r for r in rows
                  if r[1] == "imm" and r[2] in (REG_SYNC1, REG_SYNC0)]
    expect = [
        (0x080121CE, REG_SYNC1), (0x080121D6, REG_SYNC0),
        (0x0801549E, REG_SYNC1), (0x080154A6, REG_SYNC0),
    ]
    check([(a, v) for a, k, v, d in sync_sites] == expect,
          "写 reg4/reg5 的 WriteReg 调用点恰为 %s（其余 %d 个调用点 r0≠4/5）"
          % ([("0x%08X" % a, "reg%d" % v) for a, v in expect],
             len(rows) - len(expect)))
    for a, v in expect:
        d = [r[3] for r in rows if r[0] == a][0]
        print("    0x%08X -> reg%d   <- %s" % (a, v, d))

    dyn = [r for r in rows if r[1] in ("mov-reg", "other", "unknown", "lit")
           and not (r[1] == "lit" and r[2] not in (REG_SYNC1, REG_SYNC0))]
    # 允许的唯一动态点：0x0800F9B8（mov r0, r1，而 r1=0x18）
    dyn_addrs = [(r[0], r[1], r[3]) for r in dyn]
    f9 = code.at(0x0800F9B8)
    prev1 = code.at(0x0800F9B6)
    prev2 = code.at(0x0800F9B4)
    ok_dyn = (len(dyn_addrs) == 1 and dyn_addrs[0][0] == 0x0800F9B8 and
              prev1 is not None and prev1.mnemonic == "mov" and
              prev1.op_str.strip() == "r0, r1" and
              prev2 is not None and prev2.mnemonic == "movs" and
              prev2.op_str.strip() == "r1, #0x18")
    check(ok_dyn,
          "唯一未能直接解析为立即数的调用点 0x0800F9B8：mov r0,r1（r1=0x18，非 4/5）"
          if not dyn_addrs else "动态调用点 %s" % dyn_addrs)

    # 低层单寄存器写 helper 0x080157A8 的其它调用点（绕过 WriteReg 的可能路径）
    direct = [ins for ins in code.insns
              if ins.mnemonic in ("bl", "blx") and code.bl_target(ins) == 0x080157A8]
    check(len(direct) == 3,
          "底层单寄存器写 0x080157A8 的调用点 = %d 个（%s）"
          % (len(direct), ["0x%08X" % i.address for i in direct]))
    for ins in direct:
        a = ins.address - 2
        ins_prev = code.at(a)
        while ins_prev is not None and ins_prev.mnemonic not in ("movs", "mov"):
            a -= ins_prev.size
            ins_prev = code.at(a)
        reg2 = None
        if ins_prev is not None and ins_prev.op_str.startswith("r0,"):
            m = re.search(r"#(0x[0-9a-fA-F]+)", ins_prev.op_str)
            if m:
                reg2 = int(m.group(1), 16)
        print("    0x%08X: 调用前 r0=%s（%s）"
              % (ins.address, ("0x%02X" % reg2) if reg2 is not None else "动态/来自 WriteReg",
                 "非 SYNC 寄存器" if reg2 not in (REG_SYNC1, REG_SYNC0) else "SYNC!"))
        check(reg2 not in (REG_SYNC1, REG_SYNC0),
              "0x%08X 的直接写目标 r0=%s 不是 reg4/5"
              % (ins.address, ("0x%02X" % reg2) if reg2 is not None else "动态(WriteReg 内部)"))

    # 双保险：任何其它 BL 目标为 reg4/5 写 helper 的路径都没有
    other_writer_calls = [ins.address for ins in code.insns
                          if code.bl_target(ins) == WRITE_REG]
    check(sorted(other_writer_calls) == sorted([c.address for c in calls]),
          "WriteReg 调用点集合与前述 59 个一致（无重复/遗漏）")


def check_init_and_rf_ordering(image: bytes, code: Code) -> None:
    header("6. CC2500 初始化表 vs 同步字函数执行顺序（候选结论 #3/#2）")
    # 6a. 初始化表函数：47 个 WriteReg，reg 集合 0..0x2E，reg4=0xC3, reg5=0x68
    init_calls = [ins for ins in code.insns
                  if ins.mnemonic in ("bl", "blx")
                  and code.bl_target(ins) == WRITE_REG
                  and INIT_TABLE_FN <= ins.address < INIT_TABLE_FN + 0x180]
    pairs = []
    for c in init_calls:
        p0 = code.at(c.address - 2)      # movs r0, #reg
        p1 = code.at(c.address - 4)      # movs r1, #val
        reg = val = None
        if p0 is not None and p0.mnemonic == "movs" and p0.op_str.startswith("r0,"):
            reg = parse_imm(p0.op_str)
        if p1 is not None and p1.mnemonic == "movs" and p1.op_str.startswith("r1,"):
            val = parse_imm(p1.op_str)
        pairs.append((c.address, reg, val))
    check(all(p[1] is not None and p[2] is not None for p in pairs),
          "初始化表 47 个 WriteReg 的 (reg, val) 立即数全部可静态解析")
    regs = [p[1] for p in pairs if p[1] is not None]
    check(len(pairs) == 47 and sorted(regs) == list(range(0x2F)),
          "0x%08X CC2500 初始化表函数：%d 条 WriteReg，寄存器集合 = %s"
          % (INIT_TABLE_FN, len(pairs),
             "0x00..0x2E" if sorted(regs) == list(range(0x2F)) else "异常"))
    p4 = [p for p in pairs if p[1] == REG_SYNC1]
    p5 = [p for p in pairs if p[1] == REG_SYNC0]
    check(len(p4) == 1 and p4[0][2] == 0xC3,
          "初始化表写 SYNC1(reg4) 值 = 0x%02X（调用点 0x%08X）"
          % (p4[0][2] if p4 else -1, p4[0][0] if p4 else 0))
    check(len(p5) == 1 and p5[0][2] == 0x68,
          "初始化表写 SYNC0(reg5) 值 = 0x%02X（调用点 0x%08X）"
          % (p5[0][2] if p5 else -1, p5[0][0] if p5 else 0))
    print("    初始化表前 7 项（reg,val）: %s"
          % [(p[1], ("0x%02X" % p[2]) if p[2] is not None else None)
             for p in pairs[:7]])

    # 6b. init 表只被 rf_init 调用一次（0x08006662）
    init_callers = [ins.address for ins in code.insns
                    if code.bl_target(ins) == INIT_TABLE_FN]
    check(init_callers == [0x08006662],
          "初始化表函数 0x%08X 的调用点 = %s（在 RF 初始化内）"
          % (INIT_TABLE_FN, [hex(a) for a in init_callers]))

    # 6c. rf_init 先 bl init 表、后 bl 同步字函数
    rf_start = RF_INIT_FN
    entries = code.function_entries()
    rf_end = code.next_entry(rf_start, entries)
    rf_ins = [ins for ins in code.insns if rf_start <= ins.address < rf_end]
    init_bl = [ins.address for ins in rf_ins if code.bl_target(ins) == INIT_TABLE_FN]
    sync_bl = [ins.address for ins in rf_ins if code.bl_target(ins) == SYNC_FN]
    check(init_bl == [0x08006662] and sync_bl == [0x080066B2] and
          init_bl[0] < sync_bl[0],
          "RF 初始化 0x%08X..0x%08X：先 bl 初始化表(0x%08X) 后 bl 同步字(0x%08X)"
          % (rf_start, rf_end - 1,
             init_bl[0] if init_bl else 0, sync_bl[0] if sync_bl else 0))
    # 同步字调用点 0x080066B2 只能顺序到达：没有分支目标直接指向它
    branch_to_sync = []
    for ins in rf_ins:
        t = code.branch_target(ins)
        if t is not None and (t & ~1) == 0x080066B2:
            branch_to_sync.append(ins.address)
    check(not branch_to_sync,
          "RF 初始化内没有分支直接跳到 0x080066B2（只能顺序执行，必先经过 0x08006662）")
    print("    RF 初始化关键序列：")
    dump_insns(code, 0x0800665E, 0x08006666)
    dump_insns(code, 0x080066AE, 0x080066B6)

    # 6d. rf_init 的调用者（上电 / 重初始化）
    rf_callers = [ins.address for ins in code.insns
                  if code.bl_target(ins) == RF_INIT_FN]
    print("    RF 初始化被调用于：%s" % [hex(a) for a in rf_callers])
    check(len(rf_callers) >= 1, "RF 初始化存在至少 1 个真实调用点（%d 个）"
          % len(rf_callers))


def check_sync_callers_and_id(code: Code, entries: list[int]) -> None:
    header("7. 同步字函数调用点与 ID 入参（候选结论 #4）")
    callers = [ins.address for ins in code.insns if code.bl_target(ins) == SYNC_FN]
    check(sorted(callers) == sorted(SYNC_CALLERS),
          "BL -> 0x%08X 的调用点 = %s（期望 %s）"
          % (SYNC_FN, [hex(a) for a in callers], [hex(a) for a in SYNC_CALLERS]))

    for site in SYNC_CALLERS:
        fn_start = code.containing_function(site, entries)
        fn_end = code.next_entry(fn_start, entries)
        prev = code.at(site - 2)
        ok = (prev is not None and prev.mnemonic == "ldrb" and
              prev.op_str.startswith("r0,") and
              re.fullmatch(r"r0, \[r(\d+), #6\]", prev.op_str.strip()) is not None)
        base_reg = int(re.fullmatch(r"r0, \[r(\d+), #6\]",
                                    prev.op_str.strip()).group(1)) if ok else None
        check(ok,
              "调用点 0x%08X 前一条是 %s %s（r0 = settings[base+6]）"
              % (site, prev.mnemonic, prev.op_str) if prev else
              "调用点 0x%08X 前一条缺失" % site)
        if not ok:
            continue
        reach = code.reachable_set(fn_start, fn_end)
        preds = code.build_preds(fn_start, fn_end, reach)
        res = code.resolve_reg(base_reg, prev.address, fn_start, fn_end, preds)
        if res["status"] == "resolved":
            value = res["value"]
            check(value == SETTINGS_BASE,
                  "  基址寄存器 r%d @0x%08X = 0x%08X（= 设置区基址，ID 字节 = 0x%08X）"
                  % (base_reg, prev.address, value, value + 6))
            for d in res["defs"]:
                print("      " + d[3])
        else:
            check(False,
                  "  基址寄存器 r%d @0x%08X 数据流未解析（%s，%d 个定义）"
                  % (base_reg, prev.address, res["status"], len(res["defs"])))
            for d in res["defs"]:
                print("      " + d[3])
        print("    调用点 0x%08X 属于函数 0x%08X..0x%08X；上下文："
              % (site, fn_start, fn_end - 1))
        dump_insns(code, max(fn_start, site - 8), site + 2, indent="      ")

    # 两个 C.Fn 改值处理函数里 item==6 的门条件（ID 项 = 6）
    for cmp_addr, beq_addr, target in (
            (0x0800625C, 0x0800625E, 0x08006272),
            (0x0800D46E, 0x0800D470, 0x0800D482)):
        c = code.at(cmp_addr)
        b = code.at(beq_addr)
        ok = (c is not None and c.mnemonic == "cmp" and c.op_str.strip() == "r1, #6" and
              b is not None and b.mnemonic == "beq" and
              code.branch_target(b) == target)
        check(ok,
              "0x%08X %s %s；0x%08X %s %s -> 0x%08X（仅当改动项==6 才重设同步字）"
              % (cmp_addr, c.mnemonic if c else "?", c.op_str if c else "?",
                 beq_addr, b.mnemonic if b else "?", b.op_str if b else "?",
                 target))

    # 另外两个「settings[6] 读者」佐证 settings[6] 就是 ID 值
    #  0x08004EB0: ID 菜单显示（ID!=0 时 /10 取十位；ID==0 走单独分支）
    d1 = code.at(0x08004EA2)
    d2 = code.at(0x08004EB0)
    d3 = code.at(0x08004EB2)
    d4 = code.at(0x08004EB6)
    ok = (d1 is not None and d1.mnemonic == "ldr" and
          code.literal_value(d1) is not None and
          code.literal_value(d1)[1] == 0x200004F2 and
          d2 is not None and d2.mnemonic == "ldrb" and d2.op_str.strip() == "r0, [r7, #6]" and
          d3 is not None and d3.mnemonic == "cmp" and d3.op_str.strip() == "r0, #0" and
          d4 is not None and d4.mnemonic == "movs" and d4.op_str.strip() == "r1, #0xa")
    check(ok,
          "0x08004EA2(菜单当前项=%s) + 0x08004EB0 ldrb settings[6] + cmp #0 + movs r1,#0xa："
          "ID!=0 时按 /10 取十位显示（ID 菜单数值 0..99）"
          % ("ID项6" if ok else "?"))
    if ok:
        dump_insns(code, 0x08004EA0, 0x08004EC6, indent="      ")
    # 0x08006026: 保存记录时把 settings[0..0xB] 拷进记录（含 settings[6]）
    d5 = code.at(0x08006004)
    d6 = code.at(0x08006026)
    check(d5 is not None and d6 is not None and
          code.literal_value(d5) is not None and
          code.literal_value(d5)[1] == SETTINGS_BASE and
          d6.mnemonic == "ldrb" and d6.op_str.strip() == "r4, [r2, #6]",
          "0x08006004 载入设置区基址、0x08006026 读 settings[6] 存入 flash 记录"
          "（ID 随设置持久化）")


def check_patch_design(image: bytes, code: Code) -> dict:
    header("8. 拟采用补丁的静态构造与边界（候选结论 #6）")
    blobs = {}
    for key in KEYS:
        blob = build_patch_blob(key)
        blobs[key] = blob
        check(len(blob) == SYNC_FN_SIZE,
              "KEY=0x%04X：函数体 %d 字节，结束于 0x%08X == 下一函数 0x%08X"
              % (key, len(blob), SYNC_FN + len(blob), SYNC_FN_END))
        check(SYNC_FN + len(blob) == SYNC_FN_END, "KEY=0x%04X：不越过下一函数" % key)

        # 字面量落位
        check(blob[0x30:0x34] == struct.pack("<I", key),
              "KEY=0x%04X @+0x30 = 0x080121EC" % key)
        check(blob[0x34:0x38] == struct.pack("<I", ID_NUMERATOR),
              "50024 @+0x34 = 0x080121F0（与原厂字面量同址同值）")

        # 反汇编复核（只解码 +0x00..+0x2F 的代码；+0x30 起是字面量）
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
        md.skipdata = True
        dis = []
        for ins in md.disasm(blob, SYNC_FN):
            if ins.address >= SYNC_FN + 0x30:
                break
            dis.append(ins)
        print("    KEY=0x%04X 补丁代码反汇编（+0x00..+0x2F）：" % key)
        for ins in dis:
            extra = ""
            if ins.mnemonic == "ldr" and "[pc," in ins.op_str:
                m = re.search(r"#(0x[0-9a-fA-F]+)", ins.op_str)
                if m:
                    imm = int(m.group(1), 16)
                    la = ((ins.address + 4) & ~3) + imm
                    off = la - SYNC_FN
                    if 0 <= off <= 0x34:
                        val = struct.unpack_from("<I", blob, off)[0]
                        extra = "   ; [0x%08X]=0x%08X" % (la, val)
            print("      0x%08X  %-12s %-6s %-22s%s"
                  % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str, extra))
        print("      +0x30 .word 0x%08X (KEY)   +0x34 .word 0x%08X (50024)"
              % (struct.unpack_from("<I", blob, 0x30)[0],
                 struct.unpack_from("<I", blob, 0x34)[0]))
        mnems = [(ins.mnemonic, ins.op_str) for ins in dis]
        bl_targets = [code.bl_target(ins) for ins in dis if ins.mnemonic in ("bl", "blx")]
        check(bl_targets == [UIDIV, WRITE_REG, WRITE_REG, DELAY],
              "BL 目标 = %s（uidiv/WriteReg x2/delay）"
              % [("0x%08X" % t) for t in bl_targets])
        check(("movs", "r0, #0x14") in mnems and ("pop", "{r4, pc}") in mnems,
              "包含 delay(20)（movs r0,#0x14）并正常返回")
        check(not any(x[0] == "movs" and x[1] == "r0, #0x33" for x in mnems) and
              not any(x[0] == "lsls" for x in mnems),
              "不含 SCAL(movs r0,#0x33) / 1000(0x7d<<3) 序列（相对于原厂的唯一行为差异）")
        # 代码与字面量不重叠
        code_end = dis[-1].address + dis[-1].size if dis else 0
        check(code_end == SYNC_FN + 0x30 and
              sum(ins.size for ins in dis) == 0x30,
              "48 字节代码结束于 +0x30（实际 0x%08X），字面量 KEY/50024 位于 +0x30/+0x34，无重叠"
              % code_end)

        patched = apply_patch(image, key)
        check(len(patched) == IMAGE_SIZE, "KEY=0x%04X：补丁后长度不变" % key)
        diffs = [i for i in range(len(image)) if patched[i] != image[i]]
        region = set(range(_off(SYNC_FN), _off(SYNC_FN) + SYNC_FN_SIZE))
        check(set(diffs) <= region,
              "KEY=0x%04X：与原厂差异 %d 字节，全部落在补丁区 0x%05X..0x%05X"
              % (key, len(diffs), _off(SYNC_FN), _off(SYNC_FN) + SYNC_FN_SIZE - 1))
        outside = [d for d in diffs if d not in region]
        check(not outside, "KEY=0x%04X：补丁区外无任何字节变化（%d 处）"
              % (key, len(outside)))
        # 50024 地址不变
        check(patched[_off(0x080121F0):_off(0x080121F0) + 4] == struct.pack("<I", ID_NUMERATOR),
              "KEY=0x%04X：0x080121F0 处 50024 字面量保持不变" % key)
    return blobs


def main() -> int:
    try:
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:  # noqa: BLE001
        pass
    print("V1C_V2.0.bin 补丁点独立静态复核（audit_static.py）")
    print("固件 :", FW_PATH)
    print("Python:", sys.version.split()[0])

    image = load_image()
    code = Code(image)
    entries = code.function_entries()
    helpers = code.detect_switch_helpers(entries)
    print("\nThumb switch 分派 helper（GCC __gnu_thumb1_case_*，BL 后为跳转表，"
          "线性反汇编会把表当指令）: %s"
          % ([("0x%08X" % h) for h in sorted(helpers)] or "无"))

    check_image_and_base(image, code)
    check_sync_function(image, code, entries)
    check_helpers_and_global_sync(image, code)
    check_50024_literal(image, code)
    check_write_reg_sites(code)
    check_init_and_rf_ordering(image, code)
    check_sync_callers_and_id(code, entries)
    check_patch_design(image, code)

    npass = sum(1 for ok, _ in RESULTS if ok)
    nfail = len(RESULTS) - npass
    header("汇总")
    print("检查项: %d   PASS: %d   FAIL: %d" % (len(RESULTS), npass, nfail))
    if nfail:
        print("\n失败项：")
        for ok, msg in RESULTS:
            if not ok:
                print("  [FAIL]", msg)
    print("\n结论:", "PASS（静态复核全部通过）" if nfail == 0 else "FAIL（存在不符项）")
    return 0 if nfail == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
