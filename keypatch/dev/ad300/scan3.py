# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 调用点/辅助函数引用扫描（BL 解码）。"""
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
SYNC_FN = 0x0800B548


def decode_bl(hw1, hw2):
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0xD000:
        return None
    s = (hw1 >> 10) & 1
    i1 = (hw2 >> 13) & 1
    i2 = (hw2 >> 11) & 1
    j1 = (i1 if s else (1 - i1))
    j2 = (i2 if s else (1 - i2))
    imm10 = hw1 & 0x3FF
    imm11 = hw2 & 0x7FF
    off = (s << 24) | (j1 << 23) | (j2 << 22) | (imm10 << 12) | (imm11 << 1)
    if s:
        off -= (1 << 25)
    return off


def decode_bw(hw1, hw2):
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0x9000:
        return None
    s = (hw1 >> 10) & 1
    i1 = (hw2 >> 13) & 1
    i2 = (hw2 >> 11) & 1
    j1 = (i1 if s else (1 - i1))
    j2 = (i2 if s else (1 - i2))
    imm10 = hw1 & 0x3FF
    imm11 = hw2 & 0x7FF
    off = (s << 24) | (j1 << 23) | (j2 << 22) | (imm10 << 12) | (imm11 << 1)
    if s:
        off -= (1 << 25)
    return off


calls = []
for i in range(0, len(data) - 3, 2):
    hw1, hw2 = struct.unpack_from("<HH", data, i)
    off = decode_bl(hw1, hw2)
    if off is not None:
        target = BASE + i + 4 + off
        calls.append((BASE + i, target))

print("total BL candidates:", len(calls))
for addr, tgt in calls:
    if tgt == SYNC_FN:
        print("CALL sync @0x%08X" % addr)

# 谁调用 WriteReg(0x0800C47A)？谁调用 strobe/delay？
for name, t in (("WriteReg", 0x0800C47A), ("strobe", 0x0800C53C),
                ("delay_us", 0x0800C200)):
    sites = [a for a, tgt in calls if tgt == t]
    print("\n%s 0x%08X: %d call sites" % (name, t, len(sites)))
    print(" ".join("0x%08X" % s for s in sites))
