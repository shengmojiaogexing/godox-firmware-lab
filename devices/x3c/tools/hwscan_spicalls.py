#!/usr/bin/env python
"""hwscan_spicalls.py - find SPI1 byte-write call sites and their constant arguments (LCD/RF init hunt)"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def decode_bl(off):
    if off + 4 > N:
        return None
    hw1 = struct.unpack_from('<H', b, off)[0]
    hw2 = struct.unpack_from('<H', b, off+2)[0]
    if (hw1 & 0xF800) != 0xF000:
        return None
    if (hw2 & 0xD000) == 0xD000:
        s = (hw1 >> 10) & 1
        imm10 = hw1 & 0x3FF
        j1 = (hw2 >> 13) & 1
        j2 = (hw2 >> 11) & 1
        i1 = (~(j1 ^ s)) & 1
        i2 = (~(j2 ^ s)) & 1
        imm11 = hw2 & 0x7FF
        imm = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | (imm11 << 1)
        if s:
            imm -= 0x2000000
        return ((off + 4) & ~3) + imm
    return None

# find calls to SPI driver helpers
TARGETS = {}
for off in range(0, N-4, 2):
    t = decode_bl(off)
    if t is not None:
        TARGETS.setdefault(t, []).append(off)

def ins_at(va):
    return next(md.disasm(b[va-APP_BASE: va-APP_BASE+4], va), None)

def prev_imm(va, depth=3):
    """walk backwards a few instructions collecting movs/movw immediates"""
    out = []
    for back in range(2, 20, 2):
        ins = ins_at(va - back)
        if ins is None:
            continue
        if ins.mnemonic.startswith('movs') and ins.op_str.startswith('r'):
            out.append((hex(va-back), ins.op_str))
            if len(out) >= depth:
                break
    return out

if __name__ == '__main__':
    # SPI1 low-level functions in driver cluster 0x08038000-0x08038350
    spifuncs = sorted(t for t in TARGETS if 0x08038000 <= t < 0x08038400)
    print("SPI driver cluster funcs (with callers count):")
    for t in spifuncs:
        cs = [APP_BASE + o for o in TARGETS[t]]
        print(f"  {t:#010x}: {len(cs)} callers, e.g. {[hex(c) for c in cs[:6]]}")
    print()
    # constants passed to 0x080381d6 (write byte?) and 0x08038224
    for target in [0x080381d6, 0x08038224, 0x08038216, 0x080382c8]:
        if target not in TARGETS:
            print(f"-- {target:#010x}: no callers")
            continue
        print(f"-- calls to {target:#010x}:")
        consts = []
        for o in TARGETS[target][:80]:
            va = APP_BASE + o
            ctx = prev_imm(va)
            consts.append((va, ctx))
        # summarize constants
        from collections import Counter
        c = Counter()
        for va, ctx in consts:
            m = [p for p in ctx if '#' in p[1]]
            if m:
                c[m[0][1]] += 1
        print("   arg constants:", dict(c.most_common(12)))
        shown = 0
        for va, ctx in consts:
            if ctx:
                print(f"   call @{va:#010x}: {ctx}")
                shown += 1
                if shown > 15:
                    break
