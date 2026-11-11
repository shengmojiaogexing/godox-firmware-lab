# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 0x08003C00..0x08003D00 对齐复看。"""
import struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)


def word(a):
    return struct.unpack_from("<I", data, a - BASE)[0]


print("bytes 0x08003C54:", data[0x08003C54 - BASE:0x08003C54 - BASE + 16].hex(" "))
for start in (0x08003C54, 0x08003C20, 0x08003C30):
    print("\n=== try start 0x%08X ===" % start)
    n = 0
    for ins in md.disasm(data[start - BASE:start - BASE + 0xA0], start):
        note = ""
        if ins.mnemonic == "ldr" and "[pc" in ins.op_str:
            imm = int(ins.op_str.split("#")[-1].rstrip("]"), 16)
            pc = (ins.address + 4) & ~3
            tgt = pc + imm
            if 0 <= tgt - BASE < len(data) - 3:
                note = "  ; -> 0x%08X = 0x%08X" % (tgt, word(tgt))
        print("0x%08X: %-8s %-8s %-22s%s" % (ins.address, ins.bytes.hex(),
                                             ins.mnemonic, ins.op_str, note))
        n += 1
        if n > 60:
            break
