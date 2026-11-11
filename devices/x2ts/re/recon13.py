# -*- coding: utf-8 -*-
"""X2T-S 侦察 13：找出代码区里的「小数值表」（候选 C.Fn 最大值表/菜单表）。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08003000
SIZE = len(DATA)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True


def u32(addr):
    o = addr - BASE
    if 0 <= o <= SIZE - 4:
        return struct.unpack_from("<I", DATA, o)[0]
    return None


refs = []
for o in range(0, SIZE - 4, 2):
    for ins in md.disasm(DATA[o:o + 4], BASE + o):
        if ins.mnemonic in ("ldr", "ldr.w") and len(ins.operands) == 2:
            op = ins.operands[1]
            if op.type == 3 and op.mem.base in (0, 11):
                lit = ((ins.address + 4) & ~3) + op.mem.disp
                val = u32(lit)
                if val is not None:
                    refs.append((ins.address, val))
        break

print("=== 候选小数值表（目标在 0x08003000..0x0800C000，前 20 字节均 < 0x40）===")
seen = set()
for src, val in refs:
    if not (BASE <= val < 0x0800C000) or val in seen:
        continue
    o = val - BASE
    chunk = DATA[o:o + 20]
    if len(chunk) == 20 and all(b < 0x40 for b in chunk):
        seen.add(val)
        print("  表 @0x%08X  引用 ldr@0x%08X  bytes=%s" % (val, src, chunk.hex(" ")))

print()
print("=== 候选含 0xC3/0x63/0x1F 的表（前 32 字节）===")
seen = set()
for src, val in refs:
    if not (BASE <= val < 0x0800C000) or val in seen:
        continue
    o = val - BASE
    chunk = DATA[o:o + 32]
    if len(chunk) < 32:
        continue
    if 0x63 in chunk or 0xC3 in chunk:
        if all(b < 0x80 for b in chunk):
            seen.add(val)
            print("  表 @0x%08X  引用 ldr@0x%08X  bytes=%s" % (val, src, chunk.hex(" ")))
