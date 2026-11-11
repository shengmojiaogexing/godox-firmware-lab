# -*- coding: utf-8 -*-
"""X2T-S 侦察 8：正确的 LDR(literal) 解析（base=PC），扫描数据表指针/数据引用。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000
SIZE = len(DATA)


def u32(addr):
    off = addr - BASE
    if 0 <= off <= SIZE - 4:
        return struct.unpack_from("<I", DATA, off)[0]
    return None


def u16(addr):
    off = addr - BASE
    if 0 <= off <= SIZE - 2:
        return struct.unpack_from("<H", DATA, off)[0]
    return None


def literals():
    """返回 [(insn_addr, lit_addr, value)]：所有 LDR(literal)。"""
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True
    out = []
    for off in range(0, SIZE - 4, 2):
        for ins in md.disasm(DATA[off:off + 4], BASE + off):
            if ins.mnemonic in ("ldr", "ldr.w") and len(ins.operands) == 2:
                op1 = ins.operands[1]
                if op1.type == 3:
                    mem = op1.mem
                    if mem.base in (0, 11):   # capstone5: ARM_REG_PC == 11
                        lit = ((ins.address + 4) & ~3) + mem.disp
                        out.append((ins.address, lit, u32(lit)))
            break
    return out


def main():
    lits = literals()
    print("total LDR-literals:", len(lits))

    print()
    print("=== 指针值指向 0x0800C000..0x0800E504 的字面量 ===")
    n = 0
    for src, lit, val in lits:
        if val is not None and 0x0800C000 <= val < BASE + SIZE:
            print("  ldr@0x%08X lit@0x%08X -> 0x%08X" % (src, lit, val))
            n += 1
    print("  total:", n)

    print()
    print("=== 指针值指向 0x20000000..0x20030000 的字面量 ===")
    n = 0
    for src, lit, val in lits:
        if val is not None and 0x20000000 <= val < 0x20030000:
            print("  ldr@0x%08X lit@0x%08X -> 0x%08X" % (src, lit, val))
            n += 1
    print("  total:", n)

    print()
    print("=== 指针值指向 0x08000000..0x0800C000（低区数据表）的字面量 ===")
    n = 0
    for src, lit, val in lits:
        if val is not None and BASE <= val < 0x0800C000:
            print("  ldr@0x%08X lit@0x%08X -> 0x%08X" % (src, lit, val))
            n += 1
    print("  total:", n)


if __name__ == "__main__":
    main()
