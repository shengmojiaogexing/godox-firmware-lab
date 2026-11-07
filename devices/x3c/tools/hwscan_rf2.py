#!/usr/bin/env python
"""hwscan_rf2.py - RF chip register/command call sites and TX packet builders"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from collections import Counter

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def decode_bl(off):
    if off + 4 > N: return None
    hw1 = struct.unpack_from('<H', b, off)[0]; hw2 = struct.unpack_from('<H', b, off+2)[0]
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0xD000: return None
    s = (hw1 >> 10) & 1; imm10 = hw1 & 0x3FF
    j1 = (hw2 >> 13) & 1; j2 = (hw2 >> 11) & 1
    i1 = (~(j1 ^ s)) & 1; i2 = (~(j2 ^ s)) & 1; imm11 = hw2 & 0x7FF
    imm = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | (imm11 << 1)
    if s: imm -= 0x2000000
    return APP_BASE + ((off + 4) & ~3) + imm

CALLS = {}
for off in range(0, N-4, 2):
    t = decode_bl(off)
    if t is not None:
        CALLS.setdefault(t, []).append(APP_BASE + off)

def ins(off):
    return next(md.disasm(b[off:off+4], APP_BASE+off), None)

def call_scan(target, name):
    cs = CALLS.get(target, [])
    print(f"===== {name} {target:#010x}: {len(cs)} call sites")
    consts = Counter()
    out = []
    for va in cs:
        off = va - APP_BASE
        vals = []
        for back in range(2, 20, 2):
            i2 = ins(off-back)
            if i2 is None: continue
            if i2.mnemonic.startswith('mov') and i2.op_str.startswith('r0, #'):
                try: vals.append(('r0', int(i2.op_str.split('#')[1], 0)))
                except Exception: pass
            if i2.mnemonic.startswith('mov') and i2.op_str.startswith('r1, #'):
                try: vals.append(('r1', int(i2.op_str.split('#')[1], 0)))
                except Exception: pass
        out.append((va, vals))
        if vals:
            consts[tuple(v for _, v in vals[:2])] += 1
    for k, v in consts.most_common(20):
        print(f"   args {[hex(x) for x in k]}: x{v}")
    print("   sites:")
    for va, vals in out[:60]:
        print(f"     {va:#010x} vals={[(a, hex(v)) for a, v in vals]}")

if __name__ == '__main__':
    call_scan(0x08023520, 'cmd byte write')
    print()
    call_scan(0x08023540, 'reg write')
    print()
    call_scan(0x08023A52, 'reg+payload write')
    print()
    call_scan(0x0802354C, 'read byte')
