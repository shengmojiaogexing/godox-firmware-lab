# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— C.Fn 处理函数 + 字面量池解析。"""
import struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)


def word(addr):
    return struct.unpack_from("<I", data, addr - BASE)[0]


def show(lo, n):
    print("\n=== 0x%08X .. 0x%08X ===" % (lo, lo + n))
    for ins in md.disasm(data[lo - BASE:lo - BASE + n], lo):
        note = ""
        if ins.mnemonic == "ldr" and "[pc" in ins.op_str:
            imm = int(ins.op_str.split("#")[-1].rstrip("]"), 16)
            pc = (ins.address + 4) & ~3
            tgt = pc + imm
            note = "  ; -> 0x%08X = 0x%08X" % (tgt, word(tgt))
        print("0x%08X: %-14s %-8s %-22s%s" % (ins.address, ins.bytes.hex(),
                                              ins.mnemonic, ins.op_str, note))


show(0x080045E8, 0x1C0)
print("\n--- literal pool 0x080047C0..0x08004870 ---")
for a in range(0x080047C0, 0x08004870, 4):
    print("0x%08X: 0x%08X" % (a, word(a)))
