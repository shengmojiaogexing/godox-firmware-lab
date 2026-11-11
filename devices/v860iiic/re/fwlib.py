# -*- coding: utf-8 -*-
"""V860IIIC_V1.3.bin 静态分析基础库（从 devices/v1c/re/audit_static.py 裁剪适配）。

只保留通用部分：常量（镜像/基址/同步字函数/辅助函数地址）+ load_image() + Code 反汇编
/函数边界/数据流辅助类。V1C 的补丁点静态复核主体已移除；V860III-C 的分析脚本见
devices/v860iiic/re/cfn.py（本目录），补丁结论见 keypatch/devices/v860iiic.py。

用法（在仓库根）::

    tools\\python\\python.exe devices\\v860iiic\\re\\cfn.py
"""
# ---------------------------------------------------------------------------
from __future__ import annotations

import bisect
import hashlib
import os
import re
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

_HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))      # D:\神牛
FW_PATH = os.path.join(ROOT, "devices", "v860iiic", "firmware", "V860IIIC_V1.3.bin")

BASE = 0x08003000                     # 镜像链接/加载基址
IMAGE_SIZE = 87612
IMAGE_SHA256 = "8530a7e85ce1098d64d72d7ea4a28f3fdfb489e3d39da49be1ffa166f6ba270d"

SYNC_FN = 0x080122E8                  # void rf_set_sync(uint8_t id)
SYNC_FN_END = 0x08012320              # 下一函数起点（不可越过）
SYNC_FN_SIZE = 56
NEXT_FN = 0x08012320

# Lead 候选结论 #2 给出的原厂 56 字节（逐字节核对）
ORIG_FUNC_HEX = (
    "0c4a10b5411c1046f0f7fefe0446010a042003f078fae1b2052003f074fa"
    "142003f02af9332003f0cffa7d20c00003f023f910bd68c30000"
)
ORIG_FUNC = bytes.fromhex(ORIG_FUNC_HEX)
assert len(ORIG_FUNC) == SYNC_FN_SIZE

ID_NUMERATOR = 50024                  # 0xC368，原厂公式分子
REG_SYNC1 = 4                         # CC2500 SYNC1
REG_SYNC0 = 5                         # CC2500 SYNC0

# 辅助函数（候选结论 #5，本脚本用「原函数 BL 目标 + 函数体反汇编」独立验证）
UIDIV = 0x080030F0                    # __aeabi_uidiv(r0/r1) -> r0=商, r1=余
WRITE_REG = 0x080157EE                # WriteReg(r0=reg, r1=val)
STROBE = 0x080158B0                   # strobe(r0=cmd)  (= WriteCmd)
DELAY = 0x08015560                    # delay(r0=count)  (= delay_us)
READ_REG = 0x0801574E                 # ReadReg(r0=reg) -> r0=val

INIT_TABLE_FN = 0x08015590            # CC2500 47 个配置寄存器初始化表
RF_INIT_FN = 0x080066FC               # RF 初始化（先 init 表，再同步字函数）

# 候选结论 #4 的 5 个调用点
SYNC_CALLERS = (0x08006324, 0x08006762, 0x0800D554, 0x0800E1A8, 0x0800E23C)
SETTINGS_BASE = 0x2000108A            # 设置区基址（+6 = ID；本脚本用数据流复核）
ID_ADDR = SETTINGS_BASE + 6           # 0x20001090

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
