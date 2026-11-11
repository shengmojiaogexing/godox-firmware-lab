# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 首轮扫描：镜像头 / 50024 字面量 / 向量表候选。"""
import sys, struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
FW = ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin"
data = FW.read_bytes()
print("size =", len(data), hex(len(data)))

print("head 64B:", data[:64].hex(" "))
print("tail 32B:", data[-32:].hex(" "))

for base in (0x08003000, 0x08004800, 0x08000000):
    if base - 0x08000000 < len(data) + 0x100000:
        pass

base = 0x08003000
word0, word1 = struct.unpack_from("<II", data, 0)
print("word0 (initial SP?) = 0x%08X" % word0)
print("word1 (reset?)      = 0x%08X" % word1)
if 0x08000000 <= word1 < 0x09000000:
    print("reset handler file offset (base=0x08003000): 0x%X" % (word1 - base))
    print("reset handler file offset (base=0x08000000): 0x%X" % (word1 - 0x08000000))

# 50024 = 0xC368 little endian
needle = (50024).to_bytes(4, "little")
hits = [i for i in range(len(data) - 3) if data[i:i + 4] == needle]
print("\n0xC368 literal hits (offset, base=0x08003000 vaddr):")
for h in hits:
    print("  0x%05X  0x%08X" % (h, base + h))

# 也有可能是 16 位立即数构造 50024？先看看 C368 出现在哪些位置
needle16 = (50024 & 0xFFFF).to_bytes(2, "little")
hits16 = [i for i in range(len(data) - 1) if data[i:i + 2] == needle16]
print("\n0xC368 16-bit hits count:", len(hits16), [hex(x) for x in hits16[:20]])

# 非零区域/全 0 死区（找注入候选）
runs = []
start = None
for i, b in enumerate(data):
    if b == 0:
        if start is None:
            start = i
    else:
        if start is not None and i - start >= 32:
            runs.append((start, i - start))
        start = None
if start is not None and len(data) - start >= 32:
    runs.append((start, len(data) - start))
print("\n全 0 死区 >=32B:")
for off, n in runs:
    print("  0x%05X (0x%08X) len=%d" % (off, base + off, n))

# 0xFF 死区
runs = []
start = None
for i, b in enumerate(data):
    if b == 0xFF:
        if start is None:
            start = i
    else:
        if start is not None and i - start >= 32:
            runs.append((start, i - start))
        start = None
if start is not None and len(data) - start >= 32:
    runs.append((start, len(data) - start))
print("\n全 0xFF 死区 >=32B:")
for off, n in runs:
    print("  0x%05X (0x%08X) len=%d" % (off, base + off, n))
