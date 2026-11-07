#!/usr/bin/env python
"""hwscan_data.py - inspect data regions & pointer targets"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load

APP_BASE = 0x08004800
b = load()
n = len(b)

def hd(off, ln=256):
    print(f"hexdump file {off:#08x} (va {APP_BASE+off:#010x})")
    for o in range(off, min(off+ln, n), 16):
        chunk = b[o:o+16]
        asc = ''.join(chr(c) if 0x20 <= c < 0x7f else '.' for c in chunk)
        print(f"  {o:08x}  {chunk.hex(' '):<47}  {asc}")

if __name__ == '__main__':
    # pointer target histogram
    ptrs = {}
    for off in range(0, n-3, 4):
        w = struct.unpack_from('<I', b, off)[0]
        if APP_BASE <= w < APP_BASE+n:
            ptrs.setdefault(w-APP_BASE, []).append(off)
    print("flash-pointer targets in data region 0x0800-0x4000:")
    for t in sorted(ptrs):
        if 0x0800 <= t < 0x4000:
            print(f"  target file {t:#08x} {b[t:t+24]!r} <- stored at {[hex(x) for x in ptrs[t][:5]]}")
    print()
    hd(0x0e80, 0x180)
    print()
    hd(0x2a80, 0x1c0)
    print()
    hd(0x1f980, 0x80)
