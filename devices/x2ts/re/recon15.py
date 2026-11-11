# -*- coding: utf-8 -*-
"""X2T-S 侦察 15：找菜单状态字节（state+0x24/0x25/0x26）的读写代码，定位菜单项列表。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08003000
SIZE = len(DATA)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)


def scan_off(off_val, label):
    mask = 0x7FC0
    want = 0x7800 | ((off_val & 0x1F) << 6) | (0x0800 if False else 0)
    # LDRB: 0111 1; STRB: 0111 0 -> 比较去掉 bit 11
    print("=== %s (byte off 0x%02X) ===" % (label, off_val))
    seen = set()
    for i in range(0, SIZE - 2, 2):
        hw = DATA[i] | (DATA[i + 1] << 8)
        if (hw & 0x7FC0) == (0x7800 | ((off_val & 0x1F) << 6)):
            a = BASE + i
            if a in seen:
                continue
            seen.add(a)
            start = a - 8
            print("  @0x%08X hw=0x%04X" % (a, hw))
            for ins in md.disasm(DATA[start - BASE:a - BASE + 10], start):
                mark = " <<<" if ins.address == a else ""
                print("      0x%08X %-14s %s %s%s"
                      % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str, mark))
    print()


for v, name in ((0x24, "state[0x24]"), (0x25, "state[0x25]"), (0x26, "state[0x26]")):
    scan_off(v, name)
