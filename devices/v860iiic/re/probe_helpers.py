# -*- coding: utf-8 -*-
"""V860IIIC 辅助函数/调用点探针。"""
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

BASE = 0x08003000
data = open(r"devices/v860iiic/firmware/V860IIIC_V1.3.bin", "rb").read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.skipdata = True
insns = list(md.disasm(data, BASE))

for i in insns:
    if i.mnemonic in ("bl", "b") and i.op_str == "#0x80122e8":
        print("%s to sync at 0x%08X" % (i.mnemonic.upper(), i.address))

for a in (0x0801574E, 0x080157EE, 0x08015560, 0x080158B0, 0x08015590, 0x080066FC, 0x080122E8):
    print("--- 0x%08X ---" % a)
    n = 0
    for i in insns:
        if i.address >= a:
            print("  0x%08X %s %s" % (i.address, i.mnemonic, i.op_str))
            n += 1
            if n >= 12:
                break
