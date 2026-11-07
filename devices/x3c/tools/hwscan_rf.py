#!/usr/bin/env python
"""hwscan_rf.py - hunt Godox RF framing constants / radio driver evidence in X3_C."""
import sys, os, struct, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE

b = load(); N = len(b)

def find_all(pat, limit=40):
    return [m.start() for m in re.finditer(re.escape(pat), b)][:limit]

def hd(off, ln=64, base=APP_BASE):
    for o in range(off, min(off+ln, N), 16):
        chunk = b[o:o+16]
        asc = ''.join(chr(c) if 0x20 <= c < 0x7f else '.' for c in chunk)
        print(f"  {o:08x} (va {base+o:#010x})  {chunk.hex(' '):<47}  {asc}")

print("=== pattern AA AA AA AA")
for off in find_all(b'\xaa\xaa\xaa\xaa'):
    if off < 0x0F3800 and 0x8000 <= off:
        print(f" @ {off:#08x} (va {APP_BASE+off:#010x})")
        hd(off-16, 64)
print()
print("=== pattern D5 (as byte) followed by 0x09/0x11/0x21")
for off in range(0, N-4):
    if b[off] == 0xD5 and b[off+1] in (0x09, 0x11, 0x21):
        print(f" @ {off:#08x}")
        hd(off-24, 48)
print()
print("=== movt patterns (movt rX,#imm16): scan 32-bit thumb movt")
hits = []
for off in range(0, N-4, 2):
    hw1 = struct.unpack_from('<H', b, off)[0]
    hw2 = struct.unpack_from('<H', b, off+2)[0]
    if (hw1 & 0xFBF0) == 0xF2C0:  # movt
        imm16 = ((hw1 & 0xF) << 12) | (((hw1 >> 10) & 1) << 11) | (((hw1 >> 9) & 1) << 10) | (((hw1 >> 8) & 1) << 9) | ((hw2 & 0x7F) << 2) | ((hw2 >> 12) & 1)
        rd = (hw2 >> 8) & 0xF
        if imm16 in (0x5000, 0xA000, 0x4002, 0x4001, 0x4000, 0x5802):
            hits.append((off, imm16, rd))
print(f"  {len(hits)} movt hits")
for off, imm, rd in hits[:40]:
    print(f"   movt r{rd}, #{imm:#06x} @file {off:#08x} (va {APP_BASE+off:#010x})")
