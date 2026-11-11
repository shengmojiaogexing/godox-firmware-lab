# -*- coding: utf-8 -*-
"""X2T-S 侦察 9：movw/movt 组合常量 + ADR/PC 相对 -> 定位高区数据表基址。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from capstone.arm import ARM_OP_IMM, ARM_OP_REG

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000
SIZE = len(DATA)

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True

# 线性扫描（含 skipdata），记录 movw 后跟 movt 的组合，以及 adr
pending = {}
combos = []
adrs = []


def scan(start, end):
    off = start - BASE
    while off < min(end - BASE, SIZE):
        got = False
        for ins in md.disasm(DATA[off:off + 4], BASE + off):
            got = True
            if ins.mnemonic == "movw" and len(ins.operands) == 2 \
                    and ins.operands[0].type == ARM_OP_REG and ins.operands[1].type == ARM_OP_IMM:
                pending[ins.operands[0].reg] = ins.operands[1].imm
            elif ins.mnemonic == "movt" and len(ins.operands) == 2 \
                    and ins.operands[0].type == ARM_OP_REG and ins.operands[1].type == ARM_OP_IMM:
                rd = ins.operands[0].reg
                if rd in pending:
                    val = (ins.operands[1].imm << 16) | pending.pop(rd)
                    combos.append((ins.address, rd, val))
            elif ins.mnemonic == "adr" and len(ins.operands) == 2:
                combos.append((ins.address, ins.operands[0].reg, ins.operands[1].imm))
            size = ins.size
            off += size
            break
        if not got:
            off += 2


scan(BASE, BASE + SIZE)
print("movw/movt + adr combos:", len(combos))
print()
print("=== 值落在 0x0800C000..0x0800E504 ===")
for addr, rd, val in combos:
    if 0x0800C000 <= val < BASE + SIZE:
        print("  0x%08X  r%d -> 0x%08X" % (addr, rd, val))
print()
print("=== 值落在 0x0800D800..0x0800E300 ===")
for addr, rd, val in combos:
    if 0x0800D800 <= val < 0x0800E300:
        print("  0x%08X  r%d -> 0x%08X" % (addr, rd, val))
print()
print("=== 全部组合（前 120）===")
for addr, rd, val in combos[:120]:
    print("  0x%08X  r%d -> 0x%08X" % (addr, rd, val))
