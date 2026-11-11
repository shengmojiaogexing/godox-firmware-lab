# -*- coding: utf-8 -*-
"""X2T-S 侦察 7：指令级字面量引用（只认 LDR literal），定位 0x0800C000..0x0800E504 的数据表基准。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000
SIZE = len(DATA)

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True

print("=== LDR literal refs (instruction-level) with literal in 0x0800C000..0x0800E504 ===")
seen = {}
for off in range(0, SIZE - 4, 2):
    for ins in md.disasm(DATA[off:off + 4], BASE + off):
        if ins.mnemonic in ("ldr", "ldr.w") and len(ins.operands) == 2:
            op1 = ins.operands[1]
            if op1.type == 3:  # ARM_OP_MEM
                mem = op1.mem
                if mem.base == 0 and mem.index == 0:
                    lit = mem.disp
                    if 0x0800C000 <= lit < BASE + SIZE:
                        seen.setdefault(lit, []).append((ins.address, ins.bytes.hex(" ")))
        break

for lit in sorted(seen):
    off = lit - BASE
    val = struct.unpack_from("<I", DATA, off)[0]
    print("  lit_addr=0x%08X -> 0x%08X   refs=%s"
          % (lit, val, [(hex(a), b) for a, b in seen[lit][:8]]))
