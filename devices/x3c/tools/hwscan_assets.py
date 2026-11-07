#!/usr/bin/env python
"""hwscan_assets.py - dump candidate image/asset headers"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
b = load()
for base in (0xA0890, 0xA0A00, 0xA2880, 0xA5700, 0xA5A00):
    print(f"=== file {base:#08x} (va {APP_BASE+base:#010x})")
    for o in range(base, base + 0x50, 16):
        ch = b[o:o+16]
        asc = ''.join(chr(c) if 0x20 <= c < 0x7f else '.' for c in ch)
        print(f"  {o:08x}  {ch.hex(' '):<47}  {asc}")
