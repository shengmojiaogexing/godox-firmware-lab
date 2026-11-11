# -*- coding: utf-8 -*-
"""X2T-S 侦察 12：搜 cmp #99 / #31 等菜单上限立即数，定位菜单项处理代码。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)


def find(pat, label, max_hits=8):
    print("=== %s ===" % label)
    i = 0
    n = 0
    while n < max_hits:
        i = DATA.find(pat, i)
        if i < 0:
            break
        a = BASE + i
        print("  @0x%08X" % a)
        start = a - 10
        for ins in md.disasm(DATA[start - BASE:a - BASE + 18], start):
            mark = " <<<" if ins.address == a else ""
            print("     0x%08X %-14s %s %s%s"
                  % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str, mark))
        i += 1
        n += 1
    print()


for rn in range(8):
    find(bytes([0x63, 0x28 | rn]), "cmp r%d, #99" % rn, 4)
for rn in range(8):
    find(bytes([0x1F, 0x28 | rn]), "cmp r%d, #31" % rn, 3)
