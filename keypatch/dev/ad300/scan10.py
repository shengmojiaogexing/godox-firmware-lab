# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 交叉校验：所有 ldrb/strb [rY,#7] 指令 + 关键函数。"""
import struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
SETTINGS = 0x2000048C


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
        print("0x%08X: %-8s %-8s %-22s%s" % (ins.address, ins.bytes.hex(),
                                             ins.mnemonic, ins.op_str, note))


print("--- 所有 ldrb rX,[rY,#7]（0x79C0 掩码）---")
for i in range(0, len(data) - 1, 2):
    hw = struct.unpack_from("<H", data, i)[0]
    if (hw & 0xFFC0) == 0x79C0:
        rn = (hw >> 3) & 7
        rd = hw & 7
        print("  0x%08X  ldrb r%d,[r%d,#7]" % (BASE + i, rd, rn))

print("\n--- 所有 strb rX,[rY,#7]（0x71C0 掩码）---")
for i in range(0, len(data) - 1, 2):
    hw = struct.unpack_from("<H", data, i)[0]
    if (hw & 0xFFC0) == 0x71C0:
        rn = (hw >> 3) & 7
        rd = hw & 7
        print("  0x%08X  strb r%d,[r%d,#7]" % (BASE + i, rd, rn))

# 谁调用 0x0800B670
print("\n--- BL 到 0x0800B670 的调用点 ---")


def decode_bl(hw1, hw2):
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0xD000:
        return None
    s = (hw1 >> 10) & 1
    i1 = (hw2 >> 13) & 1
    i2 = (hw2 >> 11) & 1
    j1 = i1 if s else 1 - i1
    j2 = i2 if s else 1 - i2
    off = (s << 24) | (j1 << 23) | (j2 << 22) | ((hw1 & 0x3FF) << 12) | (((hw2 & 0x7FF) << 1))
    if s:
        off -= 1 << 25
    return off


for i in range(0, len(data) - 3, 2):
    hw1, hw2 = struct.unpack_from("<HH", data, i)
    o = decode_bl(hw1, hw2)
    if o is not None:
        t = BASE + i + 4 + o
        if t in (0x0800B670, 0x0800C8AC, 0x080048EC, 0x08004410):
            print("  BL 0x%08X -> 0x%08X" % (BASE + i, t))

show(0x0800C890, 0x60)
show(0x08008388, 0xA0)
show(0x0800B5E0, 0x40)
show(0x0800B37C, 0x40)
