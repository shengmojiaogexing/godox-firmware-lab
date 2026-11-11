# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 调用点附近反汇编。"""
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)


def show(lo, n):
    print("\n=== 0x%08X .. 0x%08X ===" % (lo, lo + n))
    for ins in md.disasm(data[lo - BASE:lo - BASE + n], lo):
        print("0x%08X: %-14s %-8s %s" % (ins.address, ins.bytes.hex(),
                                         ins.mnemonic, ins.op_str))


for lo, n in ((0x08004550, 0x170), (0x080083C0, 0x120), (0x08004840, 0xC0),
              (0x080087D0, 0xE0)):
    show(lo, n)
