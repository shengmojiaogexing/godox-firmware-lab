# -*- coding: utf-8 -*-
"""X2T-S 侦察 14：skipdata 线性反汇编，收集 movw/movt/ADR 常量（真实 base 0x08003000）。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from capstone.arm import ARM_OP_IMM, ARM_OP_REG

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08003000
SIZE = len(DATA)

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True
md.skipdata = True

pend = {}
combos = []
for ins in md.disasm(DATA, BASE):
    if ins.mnemonic == "movw" and len(ins.operands) == 2 \
            and ins.operands[0].type == ARM_OP_REG and ins.operands[1].type == ARM_OP_IMM:
        pend[ins.operands[0].reg] = ins.operands[1].imm
    elif ins.mnemonic == "movt" and len(ins.operands) == 2 \
            and ins.operands[0].type == ARM_OP_REG and ins.operands[1].type == ARM_OP_IMM:
        rd = ins.operands[0].reg
        if rd in pend:
            combos.append((ins.address, rd, (ins.operands[1].imm << 16) | pend.pop(rd)))
    elif ins.mnemonic in ("adr", "add", "addw") and len(ins.operands) >= 2:
        ops = ins.operands
        if len(ops) == 2 and ops[1].type == ARM_OP_IMM and "pc" in ins.op_str:
            combos.append((ins.address, ops[0].reg, ((ins.address + 4) & ~3) + ops[1].imm))

print("movw/movt/adr combos:", len(combos))
print()
print("=== 值 >= 0x0800C000 ===")
for addr, rd, val in combos:
    if 0x0800C000 <= val < BASE + SIZE:
        print("  0x%08X r%d -> 0x%08X" % (addr, rd, val))
print()
print("=== 值 0x08003000..0x0800C000（代码区内的数据地址）===")
for addr, rd, val in combos:
    if BASE <= val < 0x0800C000:
        print("  0x%08X r%d -> 0x%08X" % (addr, rd, val))
print()
print("=== 值 0x20000000..0x20030000（RAM）前 80 条 ===")
n = 0
for addr, rd, val in combos:
    if 0x20000000 <= val < 0x20030000:
        print("  0x%08X r%d -> 0x%08X" % (addr, rd, val))
        n += 1
        if n >= 80:
            break
