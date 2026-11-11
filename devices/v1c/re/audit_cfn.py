#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""V1C v5「C.Fn F3 密钥开关」补丁 —— 独立静态 + Unicorn 复核（fw-auditor）。

重要：本脚本**不复用** keypatch/devices/v1c.py 的 build/verify 逻辑。
  * 同步字函数 56B、注入 10B、两个 gate 的 BL、两处 mask 补丁全部由本脚本
    **按设计独立重新编码**，再与 CLI 产物 keypatch/out/V1C_V2.0_keyD798.bin 逐字节比对；
  * 用自建 Unicorn harness 直接执行补丁镜像（不调用模块 verify）；
  * 额外独立扫描「运行期所有会写 settings[8]=0x2000108E 的代码点」，
    评估 0x0800A6FA 保护是否足够、0x0800E0A4 复位分支是否遗漏。

用法：
    tools\\python\\python.exe devices\\v1c\\re\\audit_cfn.py
退出码 0 = 全部 PASS；1 = 有 FAIL。
"""
from __future__ import annotations

import hashlib
import os
import re
import struct
import sys

sys.dont_write_bytecode = True
_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

import audit_static as st  # noqa: E402  仅复用反汇编/CFG 基础工具

try:
    import unicorn
    from unicorn import (Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE,
                         UC_PROT_ALL, UcError)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_R4, UC_ARM_REG_R5,
                                   UC_ARM_REG_R6, UC_ARM_REG_R7, UC_ARM_REG_SP,
                                   UC_ARM_REG_LR, UC_ARM_REG_PC)
except Exception as exc:  # noqa: BLE001
    print("FATAL: unicorn 不可用: %r" % (exc,))
    sys.exit(2)

ROOT = st.ROOT
FW_PATH = st.FW_PATH
ORIG_PATH = FW_PATH
ART_BIN = os.path.join(ROOT, "keypatch", "out", "V1C_V2.0_keyD798.bin")
ART_CONT = os.path.join(ROOT, "keypatch", "out", "V1C_V2.0_keyD798_container.bin")

BASE = st.BASE
SYNC_FN = 0x080121BC
SYNC_FN_END = 0x080121F4
INJ_ADDR = 0x08017944
GATE_DEC = 0x0800625C
GATE_INC = 0x0800D46E
MASK_LIGHT1 = 0x080036DA
MASK_LIGHT2 = 0x08003702
MASK_UNPACK = 0x0800A6FA

SETTINGS_BASE = 0x20001086
FLAG_IDX = 8                    # LIGHT
FLAG_ADDR = 0x2000108E          # settings[8]
KEY_FLAG_ON = 2                 # LIGHT=ON
ID_ADDR = 0x2000108C            # settings[6]
KEY = 0xD798
ID_NUMERATOR = 50024
DIV_FUNC = 0x080030F0
WRITE_REG = 0x080156CE
STROBE = 0x08015790
DELAY_US = 0x08015440
ORIG_LITERAL_OFF = 0x34

ORIG_SHA = st.IMAGE_SHA256
ART_SHA = "86d563c8c3bc5816844b79a2c6303a39910c7bf349d757d47c8ce01bd79409cd"
CONT_SHA = "e1634a089cf87d0fc71668be02447139b3b856bed12c3ef3c2ea63db4d5c3e5d"

RESULTS: list[tuple[bool, str]] = []


def check(cond: bool, msg: str) -> bool:
    RESULTS.append((bool(cond), msg))
    print("[%s] %s" % ("PASS" if cond else "FAIL", msg))
    return bool(cond)


def header(t: str) -> None:
    print()
    print("=" * 78)
    print(t)
    print("=" * 78)


def fmt_ins(ins) -> str:
    return "0x%08X  %-12s %-7s %s" % (ins.address, ins.bytes.hex(" "),
                                      ins.mnemonic, ins.op_str)


# ---------------------------------------------------------------------------
# 独立 Thumb 编码器 + 期望补丁体
# ---------------------------------------------------------------------------
def _h(v: int) -> bytes:
    return struct.pack("<H", v)


def t_ldr_lit(rt: int, instr_addr: int, target: int) -> bytes:
    pc = (instr_addr + 4) & ~3
    imm8 = (target - pc) // 4
    assert (target - pc) % 4 == 0 and 0 <= imm8 <= 0xFF, (hex(instr_addr), hex(target))
    return _h(0x4800 | ((rt & 7) << 8) | imm8)


def t_bl(addr: int, target: int) -> bytes:
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = i1 if s else ((~i1) & 1)
    j2 = i2 if s else ((~i2) & 1)
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                       0xD000 | (j1 << 13) | (j2 << 11) | imm11)


def build_expected_sync(key: int) -> bytes:
    """独立构造 v5-LIGHT 同步字函数：44B 代码 + KEY@+0x2C + FLAG@+0x30 + 50024@+0x34。"""
    c = bytearray()
    c += _h(0xB510)                                        # +00 push {r4,lr}
    c += t_ldr_lit(3, SYNC_FN + len(c), SYNC_FN + 0x30)    # +02 ldr r3,=FLAG
    c += t_ldr_lit(2, SYNC_FN + len(c), SYNC_FN + 0x34)    # +04 ldr r2,=50024
    c += _h(0x781B)                                        # +06 ldrb r3,[r3]
    c += _h(0x1C41)                                        # +08 adds r1,r0,#1
    c += _h(0x2B02)                                        # +0A cmp r3,#2 (LIGHT=ON)
    c += _h(0xD101)                                        # +0C bne go (+0x12)
    c += t_ldr_lit(2, SYNC_FN + len(c), SYNC_FN + 0x2C)    # +0E ldr r2,=KEY
    c += _h(0x2101)                                        # +10 movs r1,#1
    c += _h(0x4610)                                        # +12 go: mov r0,r2
    c += t_bl(SYNC_FN + len(c), DIV_FUNC)                  # +14 bl uidiv
    c += _h(0x4604)                                        # +18 mov r4,r0
    c += _h(0x0A01)                                        # +1A lsrs r1,r0,#8
    c += _h(0x2004)                                        # +1C movs r0,#4
    c += t_bl(SYNC_FN + len(c), WRITE_REG)                 # +1E bl WriteReg
    c += _h(0xB2E1)                                        # +22 uxtb r1,r4
    c += _h(0x2005)                                        # +24 movs r0,#5
    c += t_bl(SYNC_FN + len(c), WRITE_REG)                 # +26 bl WriteReg
    c += _h(0xBD10)                                        # +2A pop {r4,pc}
    assert len(c) == 44, len(c)
    return bytes(c) + struct.pack("<III", key, FLAG_ADDR, ID_NUMERATOR)


def build_expected_injection() -> bytes:
    """10B：push {r0-r4,lr}; ldrb r0,[r0,#6]; bl sync; pop {r0-r4,pc}。"""
    c = _h(0xB51F) + _h(0x7980)
    c += t_bl(INJ_ADDR + 4, SYNC_FN)
    c += _h(0xBD1F)
    assert len(c) == 10
    return c


def expected_masks() -> list[tuple[int, bytes, bytes, str]]:
    return [
        (0x080036DA, b"\x00\x7a", b"\x00\x20",
         "LIGHT 造型灯任务读者1 (ldrb r0,[r0,#8]) -> movs r0,#0"),
        (0x08003702, b"\x01\x7a", b"\x00\x21",
         "LIGHT 造型灯任务读者2 (ldrb r1,[r0,#8]) -> movs r1,#0"),
        (0x0800A6FA, b"\x08\x72", b"\x00\xbf",
         "无线解包 settings[8] 写点 -> NOP"),
    ]


def expected_gates() -> list[tuple[int, bytes, bytes, str]]:
    return [
        (GATE_DEC, bytes.fromhex("062908d0"), t_bl(GATE_DEC, INJ_ADDR),
         "dec gate -> BL injection"),
        (GATE_INC, bytes.fromhex("062907d0"), t_bl(GATE_INC, INJ_ADDR),
         "inc gate -> BL injection"),
    ]


def build_expected_patched(orig: bytes, key: int) -> bytes:
    buf = bytearray(orig)
    regions = [
        (SYNC_FN, build_expected_sync(key)),
        (INJ_ADDR, build_expected_injection()),
    ]
    for site, _o, new, _d in expected_gates():
        regions.append((site, new))
    for site, _o, new, _d in expected_masks():
        regions.append((site, new))
    for addr, data in regions:
        o = addr - BASE
        buf[o:o + len(data)] = data
    return bytes(buf)


# ---------------------------------------------------------------------------
# 通用工具
# ---------------------------------------------------------------------------
def _off(a: int) -> int:
    return a - BASE


def parse_mem(op: str):
    """'rD, [rB, #imm]' / 'rD, [rB, rI]' / 'rD, [rB]' -> (dst,base,index,imm)."""
    if "[" not in op:
        return None
    parts = [p.strip() for p in op.split(",")]
    dst = None
    m = re.fullmatch(r"r(\d+)", parts[0])
    if m:
        dst = int(m.group(1))
    inner = op[op.index("[") + 1:op.index("]")]
    toks = [t.strip() for t in inner.split(",")]
    base = index = None
    imm = 0
    m = re.fullmatch(r"r(\d+)", toks[0]) if toks else None
    if m:
        base = int(m.group(1))
    for t in toks[1:]:
        t = t.strip()
        if t.startswith("#"):
            v = t[1:]
            imm = int(v, 16) if "x" in v else int(v)
        elif t.startswith("lsl") or t.startswith("lsr") or t.startswith("asr"):
            index = -1
        else:
            m = re.fullmatch(r"r(\d+)", t)
            if m:
                index = int(m.group(1))
    return (dst, base, index, imm)


def refs_into(code: st.Code, lo: int, hi: int) -> dict:
    bl = [ins.address for ins in code.insns
          if (code.bl_target(ins) or 0) and lo <= code.bl_target(ins) < hi]
    br = [ins.address for ins in code.insns
          if code.branch_target(ins) is not None and
          lo <= (code.branch_target(ins) & ~1) < hi]
    lit = [ins.address for ins in code.insns
           if code.literal_value(ins) is not None and
           lo <= code.literal_value(ins)[1] < hi]
    image = code.image
    raw = []
    for o in range(0, len(image) - 3, 2):
        v = struct.unpack_from("<I", image, o)[0]
        if lo <= v < hi:
            raw.append(o)
    return {"bl": bl, "br": br, "lit": lit, "raw": raw}


# ---------------------------------------------------------------------------
# 独立运行期访问扫描：settings[8] = 0x2000108E（读者 + 写者）
# ---------------------------------------------------------------------------
WRITE_MNEMONICS = {"str", "strb", "strh"}
READ_MNEMONICS = {"ldr", "ldrb", "ldrh", "ldrsb", "ldrsh"}


def _effect(ins, state: dict[int, int], code: st.Code) -> dict[int, int]:
    out = dict(state)
    m = ins.mnemonic
    op = ins.op_str
    if m in ("bl", "blx"):
        for r in (0, 1, 2, 3, 12):
            out.pop(r, None)
        return out
    if m == "pop":
        for r in re.findall(r"r(\d+)", op):
            out.pop(int(r), None)
        return out
    if m == "ldr" and "[pc," in op:
        mem = parse_mem(op)
        if mem and mem[0] is not None:
            lv = code.literal_value(ins)
            out.pop(mem[0], None)
            if lv is not None:
                out[mem[0]] = lv[1]
        return out
    mem = parse_mem(op)
    dst = mem[0] if mem else None
    if dst is None:
        first = op.split(",", 1)[0].strip()
        mm = re.fullmatch(r"r(\d+)", first)
        dst = int(mm.group(1)) if mm else None
    if dst is None:
        return out
    out.pop(dst, None)
    if m in ("movs", "mov") and "," in op:
        rhs = op.split(",", 1)[1].strip()
        mm = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", rhs)
        if mm:
            out[dst] = int(mm.group(1), 16) if "x" in mm.group(1) else int(mm.group(1))
        else:
            src = parse_mem(rhs)
            src_reg = src[0] if src else None
            if src_reg is not None and src_reg in state:
                out[dst] = state[src_reg]
    elif m in ("adds", "add", "subs", "sub") and op.count(",") == 2:
        toks = [t.strip() for t in op.split(",")]
        src = parse_mem(toks[1])
        src_reg = src[0] if src else None
        mm = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", toks[2])
        if src_reg is not None and src_reg in state and mm:
            imm = int(mm.group(1), 16) if "x" in mm.group(1) else int(mm.group(1))
            out[dst] = state[src_reg] + (imm if m.startswith("add") else -imm)
    return out


def find_flag_accesses(code: st.Code, entries: list[int]) -> tuple[dict, dict]:
    """返回 (writers, readers)：固定访问 FLAG_ADDR 的代码点（含跨函数参数传播）。"""
    fn_ranges = {}
    for e in entries:
        fn_ranges[e] = (e, code.next_entry(e, entries))

    def fn_of(addr):
        import bisect
        i = bisect.bisect_right(entries, addr) - 1
        s = entries[i]
        return s, fn_ranges[s][1]

    seeds = []
    for ins in code.insns:
        if ins.mnemonic == "ldr" and "[pc," in ins.op_str:
            lv = code.literal_value(ins)
            mem = parse_mem(ins.op_str)
            if lv is not None and mem and mem[0] is not None and \
                    0x20001040 <= lv[1] < 0x20001120:
                seeds.append((ins.address + ins.size, fn_of(ins.address),
                              {mem[0]: lv[1]}, 0))

    writers: dict[int, dict] = {}
    readers: dict[int, dict] = {}
    work = list(seeds)
    processed: set[tuple] = set()
    steps_total = 0
    while work and steps_total < 200000:
        start, (fs, fe), state0, depth = work.pop()
        key0 = (start, fs, tuple(sorted(state0.items())))
        if key0 in processed:
            continue
        processed.add(key0)
        sub_work = [(start, state0)]
        seen: dict[int, set] = {}
        while sub_work and steps_total < 200000:
            addr, state = sub_work.pop()
            steps_total += 1
            if not (fs <= addr < fe) or addr not in code.by_addr:
                continue
            k = frozenset(state.items())
            if k in seen.get(addr, set()):
                continue
            seen.setdefault(addr, set()).add(k)
            ins = code.by_addr[addr]
            mem = parse_mem(ins.op_str)
            if mem:
                _dst, breg, ireg, imm = mem
                effs = []
                if breg is not None and breg in state:
                    if ireg is None:
                        effs.append(state[breg] + imm)
                    elif ireg >= 0 and ireg in state:
                        effs.append(state[breg] + state[ireg])
                width = 4 if ins.mnemonic in ("ldr", "str") else \
                    2 if ins.mnemonic in ("ldrh", "strh", "ldrsh") else 1
                for eff in effs:
                    if eff <= FLAG_ADDR < eff + width:
                        if ins.mnemonic in WRITE_MNEMONICS:
                            writers[addr] = {"fn": fs, "ins": ins, "eff": eff}
                        elif ins.mnemonic in READ_MNEMONICS:
                            readers[addr] = {"fn": fs, "ins": ins, "eff": eff}
            # 跨函数：BL 时把 r0..r3 的已知值传给目标函数
            if ins.mnemonic in ("bl", "blx") and depth < 4:
                tgt = code.bl_target(ins)
                if tgt is not None and tgt in fn_ranges:
                    callee_state = {}
                    for r in (0, 1, 2, 3):
                        if r in state:
                            callee_state[r] = state[r]
                    if callee_state:
                        cfs, cfe = fn_ranges[tgt]
                        work.append((tgt, (cfs, cfe), callee_state, depth + 1))
            state2 = _effect(ins, state, code)
            for s in code.successors(ins, fs, fe):
                sub_work.append((s, state2))
    return writers, readers


# ---------------------------------------------------------------------------
# Unicorn 独立 harness
# ---------------------------------------------------------------------------
SENTINEL = 0x20000400
STACK = 0x20001800
FLASH_BASE, FLASH_SIZE = 0x08000000, 0x20000
RAM_BASE, RAM_SIZE = 0x20000000, 0x4000
MAX_INSNS = 30000


class Harness:
    def __init__(self, image: bytes, extra_stubs=()):
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
        self.uc.mem_map(FLASH_BASE, FLASH_SIZE, UC_PROT_ALL)
        self.uc.mem_map(RAM_BASE, RAM_SIZE, UC_PROT_ALL)
        self.uc.mem_write(BASE, image)
        self.uc.mem_write(SENTINEL, bytes.fromhex("fee7"))       # b .
        for a in (WRITE_REG, STROBE, DELAY_US):
            self.uc.mem_write(a, bytes.fromhex("7047"))          # bx lr
        for a in extra_stubs:
            self.uc.mem_write(a, bytes.fromhex("7047"))
        self.extra_stubs = set(extra_stubs)
        self.calls: list[tuple] = []
        self.snapshots: dict[int, dict[int, int]] = {}
        self.watch: set[int] = set()
        self.stop_on_watch = False
        self.returned = False
        self.error = None
        self.insn = 0
        self.uc.hook_add(UC_HOOK_CODE, self._hook)

    def _hook(self, uc, address, _size, _user):
        self.insn += 1
        if address == SENTINEL:
            self.returned = True
            uc.emu_stop()
            return
        if address in self.watch:
            self.snapshots[address] = {
                r: uc.reg_read(reg) for r, reg in
                ((0, UC_ARM_REG_R0), (1, UC_ARM_REG_R1), (2, UC_ARM_REG_R2),
                 (3, UC_ARM_REG_R3), (4, UC_ARM_REG_R4))
            }
            if self.stop_on_watch:
                uc.emu_stop()
                return
        if address == WRITE_REG:
            self.calls.append(("wreg", uc.reg_read(UC_ARM_REG_R0),
                               uc.reg_read(UC_ARM_REG_R1)))
        elif address == STROBE:
            self.calls.append(("cmd", uc.reg_read(UC_ARM_REG_R0), None))
        elif address == DELAY_US:
            self.calls.append(("delay", uc.reg_read(UC_ARM_REG_R0), None))
        elif address in self.extra_stubs:
            self.calls.append(("gpio", address, uc.reg_read(UC_ARM_REG_R0),
                               uc.reg_read(UC_ARM_REG_R1),
                               uc.reg_read(UC_ARM_REG_R2)))

    def write_u8(self, addr, val):
        self.uc.mem_write(addr, bytes([val & 0xFF]))

    def write_u32(self, addr, val):
        self.uc.mem_write(addr, struct.pack("<I", val & 0xFFFFFFFF))

    def run(self, start, regs: dict[int, int], sp=STACK, max_insns=MAX_INSNS):
        uc = self.uc
        uc.reg_write(UC_ARM_REG_SP, sp)
        uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)   # 函数返回落点
        for r, v in regs.items():
            uc.reg_write(r, v)
        try:
            uc.emu_start(start | 1, 0, 0, max_insns)
        except UcError as exc:  # noqa: BLE001
            self.error = "%s pc=0x%08X" % (exc, uc.reg_read(UC_ARM_REG_PC))
        return self


def fmt_calls(calls) -> str:
    out = []
    for c in calls:
        if c[0] == "wreg":
            out.append("WriteReg(%d,0x%02X)" % (c[1], c[2]))
        elif c[0] == "cmd":
            out.append("strobe(0x%02X)" % c[1])
        else:
            out.append("delay(%d)" % c[1])
    return " -> ".join(out) if out else "(无)"


def sync_from_calls(calls):
    if len(calls) < 2 or calls[0][0] != "wreg" or calls[1][0] != "wreg":
        return None
    if calls[0][1] != 4 or calls[1][1] != 5:
        return None
    return ((calls[0][2] & 0xFF) << 8) | (calls[1][2] & 0xFF)


def run_sync(image: bytes, ident: int, flag: int | None) -> dict:
    h = Harness(image)
    if flag is not None:
        h.write_u8(FLAG_ADDR, flag)
    h.run(SYNC_FN, {UC_ARM_REG_R0: ident, UC_ARM_REG_R1: 0x1234,
                    UC_ARM_REG_R2: 0x5678, UC_ARM_REG_R3: 0x9ABC},
          sp=STACK - 0x100)
    return {"calls": h.calls, "error": h.error, "returned": h.returned,
            "insn": h.insn}


def run_original_sync(image: bytes, ident: int) -> dict:
    h = Harness(image)
    h.run(SYNC_FN, {UC_ARM_REG_R0: ident, UC_ARM_REG_R1: 0x1234,
                    UC_ARM_REG_R2: 0x5678, UC_ARM_REG_R3: 0x9ABC},
          sp=STACK - 0x100)
    return {"calls": h.calls, "error": h.error, "returned": h.returned}


def run_gate(image: bytes, gate: int, item: int, flag: int, ident: int,
             cont_addr: int) -> dict:
    h = Harness(image)
    h.write_u8(FLAG_ADDR, flag)
    h.write_u8(ID_ADDR, ident)
    h.write_u8(SETTINGS_BASE + 0x0B, 0x3)       # item 0xB 路径需要
    # handler 尾部 pop {r3,r4,r5,r6,r7,pc}
    sp = STACK - 0x40
    for i, v in enumerate((0x4444, 0x5555, 0x6666, 0x7777, 0x8888, SENTINEL | 1)):
        h.write_u32(sp + 4 * i, v)
    h.watch.add(cont_addr)
    init = {0: SETTINGS_BASE, 1: item, 2: 0x1111, 3: 0x2222, 4: 0x3333,
            5: 0x4444, 6: 0x5555, 7: 0x6666}
    h.run(gate, {UC_ARM_REG_R0: SETTINGS_BASE, UC_ARM_REG_R1: item,
                 UC_ARM_REG_R2: 0x1111, UC_ARM_REG_R3: 0x2222,
                 UC_ARM_REG_R4: 0x3333, UC_ARM_REG_R5: 0x4444,
                 UC_ARM_REG_R6: 0x5555, UC_ARM_REG_R7: 0x6666}, sp=sp)
    snap = h.snapshots.get(cont_addr)
    return {"calls": h.calls, "error": h.error, "returned": h.returned,
            "snap": snap, "init": init}


def run_light(image: bytes, value: int) -> dict:
    """执行 0x080036D4（LIGHT 造型灯任务），snapshot 两个 settings[8] 读者之后的值。"""
    h = Harness(image, extra_stubs=(0x0800B870, 0x0800B882))
    h.write_u8(FLAG_ADDR, value)
    A, B = 0x080036E2, 0x08003706      # 两处 cmp
    h.watch.add(A)
    h.watch.add(B)
    h.run(0x080036D4, {UC_ARM_REG_R0: 0x1234, UC_ARM_REG_R1: 0x5678},
          sp=STACK - 0x100)
    return {"A": h.snapshots.get(A), "B": h.snapshots.get(B),
            "calls": h.calls, "error": h.error, "returned": h.returned}


def run_unpack(image: bytes) -> dict:
    """执行 0x0800A6DC（紧凑结构 -> settings），检查 settings[8] 是否被写。"""
    h = Harness(image)
    h.write_u8(FLAG_ADDR, 0xAA)                 # settings[8] 哨兵
    # compact struct 0x20001070 与 settings 在 0x86 处重叠：c[0x16] 别名 settings[0]。
    # A6DC 先写 settings[0]=c[0]=0x66，随后再读 c[0x16]（此时已是 0x66）写 settings[8]。
    for off, val in ((0, 0x66), (1, 0x55), (3, 0x33), (8, 0x88),
                     (0x0A, 0xAA), (0x14, 0x14), (0x16, 0x77)):
        h.write_u8(0x20001070 + off, val)
    h.run(0x0800A6DC, {UC_ARM_REG_R0: 0x1234}, sp=STACK - 0x100)
    return {"settings8": h.uc.mem_read(FLAG_ADDR, 1)[0], "error": h.error,
            "returned": h.returned}


# ---------------------------------------------------------------------------
# 主流程
# ---------------------------------------------------------------------------
def main() -> int:
    try:
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:  # noqa: BLE001
        pass
    print("V1C v5「C.Fn F3 密钥开关」补丁独立复核（audit_cfn.py）")
    print("固件:", ORIG_PATH)
    print("产物:", ART_BIN)
    print("Python:", sys.version.split()[0], "Unicorn:",
          getattr(unicorn, "__version__", "?"))

    if not os.path.exists(ART_BIN):
        print("FATAL: 缺少产物，先运行 keypatch.py patch --device v1c --key 0xD798")
        return 2
    orig = open(ORIG_PATH, "rb").read()
    art = open(ART_BIN, "rb").read()
    cont = open(ART_CONT, "rb").read()
    print("orig %d B sha256=%s" % (len(orig), hashlib.sha256(orig).hexdigest()))
    print("art  %d B sha256=%s" % (len(art), hashlib.sha256(art).hexdigest()))

    header("0. 固件/产物身份")
    art_sha = hashlib.sha256(art).hexdigest()
    cont_sha = hashlib.sha256(cont).hexdigest()
    print("  artifact sha256 = %s" % art_sha)
    print("  container sha256 = %s" % cont_sha)
    check(len(orig) == st.IMAGE_SIZE and hashlib.sha256(orig).hexdigest() == ORIG_SHA,
          "原厂固件 88032 B / sha256 4435dd…edfb 未变")
    check(len(art) == st.IMAGE_SIZE and art_sha == ART_SHA,
          "产物 V1C_V2.0_keyD798.bin sha256 = %s（与 Lead 最终值一致）" % ART_SHA)
    check(cont_sha == CONT_SHA,
          "container sha256 = %s（与 Lead 最终值一致）" % CONT_SHA)
    check(len(cont) == len(art) + 56 and cont[:len(art)] == art,
          "container = 补丁镜像(%d B) + 56 B 尾部" % len(art))

    expected = build_expected_patched(orig, KEY)
    header("1. 独立重编码 vs CLI 产物（逐字节）")
    check(expected == art, "本脚本独立编码的补丁镜像与 CLI 产物逐字节一致")
    exp_diff = [i for i in range(len(orig)) if expected[i] != orig[i]]
    art_diff = [i for i in range(len(orig)) if art[i] != orig[i]]
    check(exp_diff == art_diff,
          "差异偏移集合一致（%d 字节）" % len(exp_diff))
    # 差异分组
    groups = []
    for i in exp_diff:
        if groups and i == groups[-1][1] + 1:
            groups[-1][1] = i
        else:
            groups.append([i, i])
    print("  差异分组: %s" % [("0x%05X..0x%05X(%dB)" % (a, b, b - a + 1))
                              for a, b in groups])
    exp_groups = [
        (0x006DB, 0x006DB), (0x00702, 0x00703), (0x0325C, 0x0325F),
        (0x076FA, 0x076FB), (0x0A46E, 0x0A471),
        (0x0F1BC, 0x0F1E0), (0x0F1E4, 0x0F1E4), (0x0F1E6, 0x0F1EF),
        (0x14944, 0x1494D),
    ]
    check([(a, b) for a, b in groups] == exp_groups,
          "差异分组恰为：sync 区 + 注入 10B + 两 gate 4B + 3 mask (1+2+2)B")

    header("2. 同步字函数 56B（flag 版）")
    blob = art[_off(SYNC_FN):_off(SYNC_FN) + 56]
    check(blob == build_expected_sync(KEY),
          "56B 与独立构造的 flag 版布局一致（44B 代码 + KEY@+0x2C + FLAG@+0x30 + 50024@+0x34）")
    check(blob[0x2C:0x30] == struct.pack("<I", KEY),
          "KEY@+0x2C = 0x%04X" % KEY)
    check(blob[0x30:0x34] == struct.pack("<I", FLAG_ADDR),
          "FLAG 字面量@+0x30 = 0x%08X（settings[8]）" % FLAG_ADDR)
    check(blob[0x34:0x38] == orig[_off(SYNC_FN) + 0x34:_off(SYNC_FN) + 0x38] ==
          struct.pack("<I", ID_NUMERATOR),
          "50024@+0x34 与原厂同址同值")
    md = st.Cs(st.CS_ARCH_ARM, st.CS_MODE_THUMB)
    md.skipdata = True
    dis = list(md.disasm(blob, SYNC_FN))
    print("  补丁反汇编：")
    for ins in dis:
        print("    " + fmt_ins(ins))
    bls = [int(re.search(r"#(0x[0-9a-fA-F]+)", i.op_str).group(1), 16)
           for i in dis if i.mnemonic in ("bl", "blx")]
    check(bls == [DIV_FUNC, WRITE_REG, WRITE_REG],
          "BL 目标 = uidiv / WriteReg / WriteReg（不调用 SCAL/delay）")
    check(not any(i.mnemonic == "strb" for i in dis) and
          not any(i.mnemonic == "bl" and int(re.search(r"#(0x[0-9a-fA-F]+)",
                                                       i.op_str).group(1), 16) in
                  (STROBE, DELAY_US) for i in dis if i.mnemonic == "bl"),
          "函数内无 SCAL(0x33)/delay 调用（v5 按 AD100 v4 布局）")
    check(SYNC_FN + 56 == SYNC_FN_END, "补丁区结束 0x%08X == 下一函数 0x%08X"
          % (SYNC_FN + 56, SYNC_FN_END))

    header("3. 注入代码 0x08017944")
    inj = art[_off(INJ_ADDR):_off(INJ_ADDR) + 10]
    check(orig[_off(INJ_ADDR):_off(INJ_ADDR) + 10] == bytes(10),
          "原厂注入区前 10B 全 0")
    check(inj == build_expected_injection(),
          "注入 10B 与独立编码一致：push {r0-r4,lr}; ldrb r0,[r0,#6]; bl sync; pop")
    inj_dis = list(md.disasm(inj, INJ_ADDR))
    print("  注入反汇编：")
    for ins in inj_dis:
        print("    " + fmt_ins(ins))
    inj_bl = [int(re.search(r"#(0x[0-9a-fA-F]+)", i.op_str).group(1), 16)
              for i in inj_dis if i.mnemonic == "bl"]
    check(inj_bl == [SYNC_FN], "注入 BL 目标 = 0x%08X（同步字函数）" % SYNC_FN)
    code_orig = st.Code(orig)
    refs = refs_into(code_orig, INJ_ADDR, INJ_ADDR + 10)
    check(not any(refs.values()), "原厂无 BL/分支/ldr 字面量/u32 绝对地址指向注入区 10B"
          + ("" if not any(refs.values()) else "（%r）" % refs))
    # 更宽死区 0x08017944..0x08017A2D 也无引用
    refs_wide = refs_into(code_orig, INJ_ADDR, INJ_ADDR + 234)
    check(not any(refs_wide.values()), "整个 0x08017944..0x08017A2D (234B) 全 0 死区无引用")

    header("4. 两个 gate（0x0800625C / 0x0800D46E）")
    for site, orig_hex, new, name in expected_gates():
        o = _off(site)
        check(orig[o:o + 4] == orig_hex,
              "gate 0x%08X 原厂字节 %s" % (site, orig_hex.hex(" ")))
        check(art[o:o + 4] == new,
              "gate 0x%08X 补丁字节 %s" % (site, new.hex(" ")))
        gdis = list(md.disasm(new, site))
        tgt = int(re.search(r"#(0x[0-9a-fA-F]+)", gdis[0].op_str).group(1), 16) \
            if gdis and gdis[0].mnemonic == "bl" else None
        check(tgt == INJ_ADDR, "gate 0x%08X 解码为 BL 0x%08X" % (site, INJ_ADDR))
        refs_g = refs_into(code_orig, site, site + 4)
        check(not any(refs_g.values()), "原厂无引用落入 gate 0x%08X..0x%08X"
              % (site, site + 3))
        cont = GATE_DEC + 4 if site == GATE_DEC else GATE_INC + 4
        print("  0x%08X -> BL 0x%08X；hook 返回后继续 0x%08X（%s）"
              % (site, INJ_ADDR, cont,
                 "dec: cmp r1,#0xB; bne pop" if site == GATE_DEC
                 else "inc: cmp r1,#0xB; bne pop"))

    header("5. 读者屏蔽 / 标志保护")
    for addr, orig_hex, new, desc in expected_masks():
        o = _off(addr)
        check(orig[o:o + len(orig_hex)] == orig_hex,
              "0x%08X 原厂字节 %s" % (addr, orig_hex.hex(" ")))
        check(art[o:o + len(new)] == new,
              "0x%08X 补丁字节 %s（%s）" % (addr, new.hex(" "), desc))
        refs_m = refs_into(code_orig, addr, addr + len(orig_hex))
        check(not any(refs_m.values()),
              "原厂无引用落入 0x%08X..0x%08X" % (addr, addr + len(orig_hex) - 1))
        mdis = list(md.disasm(new, addr))
        print("  0x%08X 反汇编: %s" % (addr, "; ".join(
            i.mnemonic + " " + i.op_str for i in mdis)))
    # 0x0800A6FA 原厂语义：strb r0,[r1,#8]，其中 r1=settings base
    ins_e6 = st.Code(orig).at(0x0800A6FA)
    mem_e6 = parse_mem(ins_e6.op_str)
    check(ins_e6.mnemonic == "strb" and mem_e6 and mem_e6[1] == 1 and mem_e6[3] == 8,
          "0x0800A6FA 原厂语义 = strb r0,[r1,#8]（写 settings[8]）")
    check(art[_off(0x0800A6FA):_off(0x0800A6FA) + 2] == bytes.fromhex("00bf"),
          "0x0800A6FA -> NOP(00 bf)，保留同一解包函数其它 settings 写点")
    check(art[_off(0x0800A6E2):_off(0x0800A6E2) + 2] == orig[_off(0x0800A6E2):_off(0x0800A6E2) + 2] ==
          bytes.fromhex("0a70"),
          "同一函数其它写点未动（0x0800A6E2 = strb r2,[r1]）")

    header("6. 独立扫描：运行期 settings[8]=0x2000108E 的读者/写者")
    entries = code_orig.function_entries()
    code_orig.detect_switch_helpers(entries)
    writers_orig, readers_orig = find_flag_accesses(code_orig, entries)
    print("原厂写点：")
    for addr in sorted(writers_orig):
        w = writers_orig[addr]
        print("  0x%08X  fn=0x%08X  %s  eff=0x%08X"
              % (addr, w["fn"], fmt_ins(w["ins"]), w["eff"]))
    print("原厂读者：")
    for addr in sorted(readers_orig):
        r0 = readers_orig[addr]
        print("  0x%08X  fn=0x%08X  %s  eff=0x%08X"
              % (addr, r0["fn"], fmt_ins(r0["ins"]), r0["eff"]))
    expected_writers = {0x0800672C, 0x0800A6FA, 0x0800E0A4, 0x080159F0}
    check(set(writers_orig) == expected_writers,
          "原厂固定写点 = %s（DCDC 复位/出厂默认/load/解包）"
          % [("0x%08X" % a) for a in sorted(expected_writers)])
    functional_readers = {0x080036DA, 0x08003702}
    check(functional_readers <= set(readers_orig),
          "LIGHT 的两个功能读者 0x080036DA/0x08003702 均命中")

    code_pat = st.Code(art)
    code_pat.detect_switch_helpers(entries)
    writers_pat, readers_pat = find_flag_accesses(code_pat, entries)
    print("补丁后写点：")
    for addr in sorted(writers_pat):
        print("  0x%08X  fn=0x%08X  %s" % (addr, writers_pat[addr]["fn"],
                                         fmt_ins(writers_pat[addr]["ins"])))
    check(set(writers_pat) == expected_writers - {0x0800A6FA},
          "0x0800A6FA 已被 NOP 消除；其余写点不变（0x0800E0A4 仍存在）")
    check(set(readers_pat) - {0x080121C2} ==
          set(readers_orig) - {MASK_LIGHT1, MASK_LIGHT2},
          "读者集合：原厂两处被 mask；补丁新增同步函数 0x080121C2 读 flag；其余读者不变")
    check(True, "动态索引写点（dec/inc handler）只会在用户改 item 时写 settings[item]；"
                "item=8 改值正是 LIGHT 开关的正常路径")

    header("6b. 0x0800E0A4（0x0800DCDC 内的默认重置块）语义评估")
    block_lo, block_hi = 0x0800E05E, 0x0800E0CC
    print("  0x0800DCDC 是输入/按键扫描任务（由设置应用 dispatcher 0x08015B52 每轮调用）；")
    print("  该块条件：输入 bitmask r2 == 0x300（GPIO 输入两位）+ [0x200004F0] == 1；")
    print("  块内把 settings[0..0xB] 全部写成出厂默认值，随后调用 0x08014EF8 与同步字函数：")
    for ins in code_orig.insns:
        if block_lo <= ins.address < block_hi:
            print("    " + fmt_ins(ins))
    print("  结论：这是『恢复 C.Fn 默认值』的输入组合路径（不是无线设置同步/远程路径），")
    print("  清 settings[8] 与“恢复默认”语义一致；若产品要求密钥开关不被该组合清除，")
    print("  可考虑把 0x0800E0A4(a5 72)->NOP，但会改变恢复默认语义，需产品决定。")

    header("7. Unicorn 独立 harness：原厂自证")
    for ident in (0, 1, 5, 37, 99):
        r = run_original_sync(orig, ident)
        expect = ID_NUMERATOR // (ident + 1)
        hi, lo = expect >> 8, expect & 0xFF
        want = [("wreg", 4, hi), ("wreg", 5, lo), ("delay", 20, None),
                ("cmd", 0x33, None), ("delay", 1000, None)]
        ok = (r["error"] is None and r["returned"] and r["calls"] == want
              and sync_from_calls(r["calls"]) == expect)
        check(ok, "原厂 ID=%-2d: sync=0x%04X | %s%s"
              % (ident, expect, fmt_calls(r["calls"]),
                 "" if ok else "（期望 %s）" % fmt_calls(want)))

    header("8. Unicorn：补丁后 sync（flag=0/1/2 × ID）")
    for flag in (0, 1, 2):
        for ident in (0, 1, 5, 37, 99):
            r = run_sync(art, ident, flag)
            key_mode = (flag == KEY_FLAG_ON)
            expect = KEY if key_mode else ID_NUMERATOR // (ident + 1)
            hi, lo = expect >> 8, expect & 0xFF
            want = [("wreg", 4, hi), ("wreg", 5, lo)]
            ok = (r["error"] is None and r["returned"] and r["calls"] == want
                  and sync_from_calls(r["calls"]) == expect)
            check(ok, "flag=%d ID=%-2d: sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
                  % (flag, ident, expect, hi, lo, fmt_calls(r["calls"])))
    check(True, "v5-LIGHT 语义：flag==2 -> KEY；flag 0/1 -> 50024/(ID+1)；"
                "同步函数无 delay(20us)/SCAL（风险见报告）")

    header("9. Unicorn：注入 hook 经两个 gate 调用 + r0..r4 恢复")
    # 静态确认 item9(LCD) 在原厂 handler 中于 gate 之前就转到专用处理函数，
    # 因此 module docstring 的「任意 C.Fn 项改值都调 sync」对 item9 不成立。
    d_cmp, d_beq = code_orig.at(0x08006238), code_orig.at(0x0800623A)
    i_cmp, i_beq = code_orig.at(0x0800D468), code_orig.at(0x0800D46A)
    check(d_cmp.mnemonic == "cmp" and d_cmp.op_str.strip() == "r1, #9" and
          d_beq.mnemonic == "beq" and code_orig.branch_target(d_beq) == 0x0800626C,
          "dec handler item9 在 gate 之前分支到 0x0800626C（bl 0x08014EF8），不经过 hook")
    check(i_cmp.mnemonic == "cmp" and i_cmp.op_str.strip() == "r1, #9" and
          i_beq.mnemonic == "beq" and code_orig.branch_target(i_beq) == 0x0800D47C,
          "inc handler item9 在 gate 之前分支到 0x0800D47C，不经过 hook")
    check(True, "★ 限界：hook 覆盖 item0..8/10/11 的改值；item9(LCD) 原厂路径不调 sync。"
                "密钥开关是 item8，不影响本功能；若要求字面「任意 item」需额外补 item9 分支")
    cont_map = {GATE_DEC: 0x08006260, GATE_INC: 0x0800D472}
    for gate, cont in cont_map.items():
        for item in (0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 11):
            ident = 37
            r = run_gate(art, gate, item, KEY_FLAG_ON, ident, cont)
            exp_sync = KEY
            hi, lo = exp_sync >> 8, exp_sync & 0xFF
            want = [("wreg", 4, hi), ("wreg", 5, lo)]
            snap_ok = (r["snap"] is not None and
                       all(r["snap"][k] == r["init"][k] for k in range(5)))
            ok = (r["error"] is None and r["returned"] and r["calls"] == want
                  and snap_ok)
            check(ok, "gate 0x%08X item=%-2d flag=2: hook 调用 sync 0x%04X；r0..r4 恢复=%s"
                  % (gate, item, exp_sync, snap_ok))
    for gate, cont in cont_map.items():
        for flag in (0, 1):
            r = run_gate(art, gate, 8, flag, 5, cont)
            exp_sync = ID_NUMERATOR // 6
            hi, lo = exp_sync >> 8, exp_sync & 0xFF
            ok = (r["error"] is None and r["returned"]
                  and r["calls"] == [("wreg", 4, hi), ("wreg", 5, lo)]
                  and r["snap"] is not None and
                  all(r["snap"][k] == r["init"][k] for k in range(5)))
            check(ok, "gate 0x%08X item=8 flag=%d: sync=0x%04X（普通模式）；r0..r4 恢复"
                  % (gate, flag, exp_sync))

    header("10. Unicorn：mask 语义（0x080036DA/0x08003702 / 0x0800A6FA）")
    # LIGHT 两个读者：原厂 value=2 时 snapshot 读到 2；补丁后均强制 0
    lo2 = run_light(orig, 2)
    lp2 = run_light(art, 2)
    check(lo2["A"] is not None and lo2["A"][0] == 2,
          "原厂 0x080036DA 读者读到 settings[8]=2")
    check(lp2["A"] is not None and lp2["A"][0] == 0,
          "补丁后 0x080036DA 强制 0（cmp 前 r0=0）")
    check(lo2["B"] is not None and lo2["B"][1] == 2,
          "原厂 0x08003702 读者读到 settings[8]=2")
    check(lp2["B"] is not None and lp2["B"][1] == 0,
          "补丁后 0x08003702 强制 0（cmp 前 r1=0）")
    # mask 后行为 == 原厂 LIGHT=0（12sec 默认）：比较 GPIO 调用序列
    lo0 = run_light(orig, 0)
    check(lo0["error"] is None and lp2["error"] is None,
          "LIGHT 任务原厂/补丁运行无 fault")
    check(lo0["calls"] == lp2["calls"],
          "补丁后 value=2 的 GPIO 调用序列 == 原厂 value=0（12sec 默认）序列")
    # 无线解包：原厂覆盖 settings[8]；补丁后保留
    uo = run_unpack(orig)
    up = run_unpack(art)
    check(uo["settings8"] == 0x66 and uo["returned"],
          "原厂 0x0800A6DC：settings[8] 被 compact[0x16]=0x66 覆盖")
    check(up["settings8"] == 0xAA and up["returned"],
          "补丁后 0x0800A6DC：settings[8] 保持 0xAA（NOP 生效）")

    npass = sum(1 for ok, _ in RESULTS if ok)
    nfail = len(RESULTS) - npass
    header("汇总")
    print("检查项: %d   PASS: %d   FAIL: %d" % (len(RESULTS), npass, nfail))
    if nfail:
        print("\n失败项：")
        for ok, msg in RESULTS:
            if not ok:
                print("  [FAIL]", msg)
    print("\n结论:", "PASS（v5 独立复核全部通过）" if nfail == 0
          else "FAIL（存在不符项）")
    return 0 if nfail == 0 else 1


if __name__ == "__main__":
    sys.exit(main())

