# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 最大值/回绕表 + 设置区读写点定位。"""
import struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)


def read(addr, n):
    return data[addr - BASE:addr - BASE + n]


for addr, n in ((0x0800DA64, 16), (0x0800DA54, 32), (0x0800DA80, 16)):
    print("0x%08X: %s" % (addr, read(addr, n).hex(" ")))

print("\n--- 0x0800D9C0..0x0800DAE0 ---")
for a in range(0x0800D9C0, 0x0800DAE0, 16):
    print("0x%08X: %s  |%s|" % (a, read(a, 16).hex(" "),
                                "".join(chr(b) if 32 <= b < 127 else "." for b in read(a, 16))))

# settings base 0x2000048C 的引用（ldr literal 指向该地址）
print("\n--- 内存常量 0x2000048C 在全镜像中的出现位置 ---")
needle = struct.pack("<I", 0x2000048C)
for i in range(0, len(data) - 3, 2):
    if data[i:i + 4] == needle:
        print("  0x%08X (0x%08X)" % (i, BASE + i))

# 找 save/load 设置函数：设置区大小？先看 0x2000048C 附近常量
for val in (0x2000048C, 0x2000048D, 0x2000048E, 0x20000490, 0x20000494):
    needle = struct.pack("<I", val)
    hits = [BASE + i for i in range(0, len(data) - 3, 2) if data[i:i + 4] == needle]
    print("\n0x%08X refs: %s" % (val, " ".join("0x%08X" % h for h in hits[:20])))
