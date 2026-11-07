#!/usr/bin/env python
"""hwscan_find_callers.py - find BL/B.W callsites targeting an address (thumb, 32-bit scan)."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load, BASE

b = load()
n = len(b)

def decode_bl(off):
    """decode 32-bit thumb BL/BLX at file offset off; return (target, kind) or None"""
    if off + 4 > n:
        return None
    hw1 = struct.unpack_from('<H', b, off)[0]
    hw2 = struct.unpack_from('<H', b, off+2)[0]
    if (hw1 & 0xF800) != 0xF000:
        return None
    # BL: 11110 S imm10  (hw1),  11 J1 1 J2 imm11  (hw2)
    if (hw2 & 0xD000) == 0xD000:  # BL/BLX
        s = (hw1 >> 10) & 1
        imm10 = hw1 & 0x3FF
        j1 = (hw2 >> 13) & 1
        j2 = (hw2 >> 11) & 1
        imm11 = hw2 & 0x7FF
        i1 = (~(j1 ^ s)) & 1
        i2 = (~(j2 ^ s)) & 1
        imm32 = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | (imm11 << 1)
        if s:
            imm32 -= 0x2000000
        pc = (off + 4) & ~3
        return (pc + imm32, 'blx' if (hw2 & 0x1000) == 0 else 'bl')
    return None

def find_callers(target_va):
    res = []
    for off in range(0, n-4):
        r = decode_bl(off)
        if r and r[0] == target_va:
            res.append((BASE+off, off, r[1]))
    return res

if __name__ == '__main__':
    targets = [int(x, 16) for x in sys.argv[1:]] or [0x08011E6C, 0x0800020C]
    for t in targets:
        cs = find_callers(t)
        print(f"callers of {t:#010x}: {len(cs)}")
        for va, off, kind in cs[:20]:
            print(f"  {kind} at file {off:#08x} (va {va:#010x})")
