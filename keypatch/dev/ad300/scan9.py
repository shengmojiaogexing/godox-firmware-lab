# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— F8(settings[7]) 读者/持久化函数反汇编。"""
import struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)


def word(a):
    return struct.unpack_from("<I", data, a - BASE)[0]


def show(lo, n):
    print("\n=== 0x%08X .. 0x%08X ===" % (lo, lo + n))
    for ins in md.disasm(data[lo - BASE:lo - BASE + n], lo):
        note = ""
        if ins.mnemonic == "ldr" and "[pc" in ins.op_str:
            imm = int(ins.op_str.split("#")[-1].rstrip("]"), 16)
            pc = (ins.address + 4) & ~3
            tgt = pc + imm
            if 0 <= tgt - BASE < len(data) - 3:
                note = "  ; -> 0x%08X = 0x%08X" % (tgt, word(tgt))
        print("0x%08X: %-8s %-8s %-24s%s" % (ins.address, ins.bytes.hex(),
                                             ins.mnemonic, ins.op_str, note))


for lo, n in ((0x08004410, 0x100), (0x08004AE4, 0x160), (0x08004D34, 0x110),
              (0x0800595C, 0x60), (0x08006044, 0x60), (0x0800ADAC, 0x50),
              (0x0800B670, 0x70), (0x0800C814, 0x40)):
    show(lo, n)
