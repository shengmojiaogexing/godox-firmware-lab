# -*- coding: utf-8 -*-
"""X2T-S 侦察 17：C.Fn[2](BEEP) 的读者/写者；0x0800B988 语义；apply 函数完整反汇编。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08003000
SIZE = len(DATA)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True


def off(a):
    return a - BASE


def u32(a):
    o = off(a)
    return struct.unpack_from("<I", DATA, o)[0] if 0 <= o <= SIZE - 4 else None


def dis(start, end):
    for ins in md.disasm(DATA[off(start):off(end)], start):
        print("      0x%08X %-18s %s %s"
              % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))


print("=== 0x0800B988 函数 ===")
dis(0x0800B988, 0x0800B9B4)
print()

print("=== 0x0800C790 apply 函数完整 ===")
dis(0x0800C790, 0x0800C81E)
print()

print("=== 找 ldrb/strb [reg,#2] 且 reg 在附近由 0x200203E0 载入 ===")
for o in range(0, SIZE - 4, 2):
    for ins in md.disasm(DATA[o:o + 4], BASE + o):
        if ins.mnemonic in ("ldr", "ldr.w") and len(ins.operands) == 2:
            op = ins.operands[1]
            if op.type == 3 and op.mem.base in (0, 11):
                lit = ((ins.address + 4) & ~3) + op.mem.disp
                if u32(lit) == 0x200203E0:
                    reg = ins.operands[0].reg
                    print("  base@0x%08X r%d" % (ins.address, reg))
                    # 往后扫 120 字节找 [reg,#2]
                    start = ins.address
                    for ins2 in md.disasm(DATA[off(start):off(start) + 120], start):
                        if len(ins2.operands) >= 2 and ins2.operands[1].type == 3:
                            m = ins2.operands[1].mem
                            if m.base == reg and m.disp in (2,):
                                print("     0x%08X %-18s %s %s"
                                      % (ins2.address, ins2.bytes.hex(" "),
                                         ins2.mnemonic, ins2.op_str))
        break
