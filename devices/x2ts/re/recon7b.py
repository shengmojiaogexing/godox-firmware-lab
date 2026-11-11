# -*- coding: utf-8 -*-
"""X2T-S 侦察 7b：指令级 LDR 字面量 -> 值，找出指向 0x0800C000..0x0800E504 的指针。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000
SIZE = len(DATA)

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True


def read_u32(addr):
    off = addr - BASE
    if 0 <= off <= SIZE - 4:
        return struct.unpack_from("<I", DATA, off)[0]
    return None


print("=== LDR 字面量值指向 0x0800C000..0x0800E504 ===")
out = []
for off in range(0, SIZE - 4, 2):
    for ins in md.disasm(DATA[off:off + 4], BASE + off):
        if ins.mnemonic in ("ldr", "ldr.w") and len(ins.operands) == 2:
            op1 = ins.operands[1]
            if op1.type == 3:
                mem = op1.mem
                if mem.base == 0 and mem.index == 0:
                    lit_addr = mem.disp
                    val = read_u32(lit_addr)
                    if val is not None and 0x0800C000 <= val < BASE + SIZE:
                        out.append((val, lit_addr, ins.address))
        break

for val, lit, src in sorted(out):
    print("  V=0x%08X  (lit@0x%08X, ldr@0x%08X)" % (val, lit, src))
print("  total:", len(out))
