# -*- coding: utf-8 -*-
"""临时反汇编辅助：python dis.py <bin> <base> <start_hex> <len_hex> [--walk]"""
import sys
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

path, base, start, length = sys.argv[1], int(sys.argv[2], 0), int(sys.argv[3], 0), int(sys.argv[4], 0)
walk = "--walk" in sys.argv
data = open(path, "rb").read()
off = start - base
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.skipdata = True
code = data[off:off + length]
n = 0
for ins in md.disasm(code, start):
    print("0x%08X: %-8s %s" % (ins.address, ins.mnemonic, ins.op_str))
    n += 1
    if n > 400:
        break
