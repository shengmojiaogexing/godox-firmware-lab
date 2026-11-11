# -*- coding: utf-8 -*-
"""X2T-S 侦察 10（真实 base 0x08003000）：同步字函数调用点上下文 + C.Fn 改值处理线索。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08003000
SIZE = len(DATA)

SYNC_FN = 0x0800C8F8
CALLS = [0x08005F72, 0x0800ADC4, 0x0800C7F8]


def off(a):
    return a - BASE


def dis(addr, before=0x20, after=0x40):
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    start = addr - before
    for ins in md.disasm(DATA[off(start):off(addr) + after], start):
        mark = "   <<<< CALL" if ins.address == addr else ""
        print("    0x%08X  %-16s %s %s%s"
              % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str, mark))


print("=== 同步字函数调用点上下文（真实 base 0x08003000）===")
for c in CALLS:
    print("---- call @0x%08X ----" % c)
    dis(c, before=0x28, after=0x30)
    print()

print("=== 全镜像中 BL 到 0x%08X 的点 ===" % SYNC_FN)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True
for o in range(0, SIZE - 4, 2):
    for ins in md.disasm(DATA[o:o + 4], BASE + o):
        if ins.mnemonic in ("bl", "bl.w") and ins.operands and ins.operands[0].imm == SYNC_FN:
            print("  BL @0x%08X" % ins.address)
        break

print()
print("=== 尾部数据区对齐字（0x080114A0..0x08011504）===")
base = 0x080114A0
for a in range(base, BASE + SIZE, 4):
    v = struct.unpack_from("<I", DATA, off(a))[0]
    print("  0x%08X: 0x%08X" % (a, v))
