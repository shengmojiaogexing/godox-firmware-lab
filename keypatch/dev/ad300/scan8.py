# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 符号化扫描 settings[0..7] 的读者（基于 ldr literal 引用点）。

思路：settings 基址 0x2000048C 只能通过字面量加载。先找所有引用该字面量的
LDR(literal) 指令，再以所在函数为范围做极简符号执行：跟踪哪些寄存器持有点阵
基址/基址+偏移，遇到内存操作就记录偏移与宽度。
"""
import struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from capstone.arm import ARM_OP_REG, ARM_OP_MEM, ARM_OP_IMM, ARM_REG_R0, ARM_REG_PC

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True
SETTINGS = 0x2000048C

OPSIZE = {"ldrb": 1, "ldrsb": 1, "strb": 1, "ldrh": 2, "ldrsh": 2, "strh": 2,
          "ldr": 4, "str": 4}


def word(addr):
    return struct.unpack_from("<I", data, addr - BASE)[0]


def decode_bl(hw1, hw2):
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0xD000:
        return None
    s = (hw1 >> 10) & 1
    i1 = (hw2 >> 13) & 1
    i2 = (hw2 >> 11) & 1
    j1 = i1 if s else 1 - i1
    j2 = i2 if s else 1 - i2
    off = (s << 24) | (j1 << 23) | (j2 << 22) | ((hw1 & 0x3FF) << 12) | ((hw2 & 0x7FF) << 1)
    if s:
        off -= 1 << 25
    return off


# 函数入口：所有 BL 目标 + 向量表里的 Thumb 指针
entries = set()
for i in range(0, len(data) - 3, 2):
    hw1, hw2 = struct.unpack_from("<HH", data, i)
    o = decode_bl(hw1, hw2)
    if o is not None:
        t = BASE + i + 4 + o
        if BASE <= t < BASE + len(data):
            entries.add(t)
for i in range(0, 0xC0, 4):
    v = word(BASE + i)
    if BASE <= (v & ~1) < BASE + len(data):
        entries.add(v & ~1)
entries = sorted(entries)


def func_range(addr):
    lo = None
    for e in entries:
        if e <= addr:
            lo = e
        else:
            return lo, e
    return lo, BASE + len(data)


# 找所有 settings 基址 LDR 引用
sites = []
for i in range(0, len(data) - 3, 2):
    if data[i:i + 4] == struct.pack("<I", SETTINGS):
        la = BASE + i
        for back in range(2, 0x400, 2):
            a = la - back
            if a < BASE:
                break
            hw = struct.unpack_from("<H", data, a - BASE)[0]
            if (hw & 0xF800) == 0x4800:
                pc = (a + 4) & ~3
                if pc + (hw & 0xFF) * 4 == la:
                    sites.append((a, (hw >> 8) & 7, la))
                    break

ACCESSES = {}

for ldr_addr, rt, la in sorted(sites):
    lo0, _hi0 = func_range(ldr_addr)
    # 从 ldr 引用点开始向后反汇编（capstone 会在解码失败处停止）
    start = ldr_addr
    end = min(len(data), ldr_addr - BASE + 0x800)
    regs = {md.reg_name(ARM_REG_R0 + rt): (0, ldr_addr)}
    for ins in md.disasm(data[start - BASE:end], start):
        for op in ins.operands:
            if op.type == ARM_OP_MEM and op.mem.base != 0:
                bname = ins.reg_name(op.mem.base)
                if bname and bname in regs:
                    off0, src = regs[bname]
                    total = off0 + op.mem.disp
                    if ins.mnemonic in OPSIZE:
                        sz = OPSIZE[ins.mnemonic]
                        if 0 <= total < 8 and total + sz <= 8:
                            ACCESSES.setdefault(total, []).append(
                                (ins.address, ins.mnemonic, ins.op_str, src))
        try:
            _, written = ins.regs_access()
        except Exception:
            written = []
        for r in written:
            n = ins.reg_name(r)
            if n:
                regs.pop(n, None)
        mo, mn = ins.mnemonic, ins.op_str
        if mo == "mov" and len(ins.operands) == 2 and ins.operands[1].type == ARM_OP_REG:
            src = ins.reg_name(ins.operands[1].reg)
            dst = ins.reg_name(ins.operands[0].reg)
            if src in regs and dst:
                regs[dst] = regs[src]
        elif mo == "ldr" and "[pc" in mn:
            dst = ins.reg_name(ins.operands[0].reg)
            for o in ins.operands:
                if o.type == ARM_OP_MEM and o.mem.base == ARM_REG_PC:
                    pc = (ins.address + 4) & ~3
                    tgt = pc + (o.mem.disp or 0)
                    if 0 <= tgt - BASE < len(data) - 3 and word(tgt) == SETTINGS and dst:
                        regs[dst] = (0, ins.address)
        if mo in ("pop", "bx") and "pc" in ins.op_str:
            break
        if mo in ("b", "b.w"):
            break

for off in sorted(ACCESSES):
    print("\n=== settings[%d]（RAM 0x%08X）访问点 ===" % (off, SETTINGS + off))
    for addr, m, ops, src in ACCESSES[off]:
        lo, hi = func_range(addr)
        print("  0x%08X  %-5s %-20s (基址 ldr @0x%08X，函数 0x%08X..0x%08X)"
              % (addr, m, ops, src, lo, hi))
