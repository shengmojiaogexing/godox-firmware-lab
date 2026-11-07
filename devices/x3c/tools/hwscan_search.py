#!/usr/bin/env python
"""hwscan_search.py - search for a 32-bit little-endian word value in the image."""
import sys, os, struct, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load, BASE

b = load()

def find_word(val):
    pat = struct.pack('<I', val)
    return [m.start() for m in re.finditer(re.escape(pat), b)]

def find_range(lo, hi, aligned=True):
    """all aligned words in [lo,hi]"""
    res = []
    for off in range(0, len(b)-3, 4):
        w = struct.unpack_from('<I', b, off)[0]
        if lo <= w <= hi:
            res.append((off, w))
    return res

if __name__ == '__main__':
    for s in sys.argv[1:]:
        if '-' in s and not s.startswith('0x08'):
            lo, hi = [int(x, 16) for x in s.split('-')]
            print(f"words in [{lo:#x},{hi:#x}]:")
            for off, w in find_range(lo, hi)[:40]:
                print(f"  file {off:#08x} (va {BASE+off:#010x}) = {w:#010x}")
        else:
            v = int(s, 16)
            print(f"occurrences of {v:#010x}: {[hex(x) for x in find_word(v)]}")
