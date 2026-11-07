#!/usr/bin/env python
"""hwscan_ram.py - RAM literal xrefs (validated) to find framebuffers/buffers, and find big memset/copy calls."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE, build_cached
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from collections import defaultdict, Counter

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def main():
    sp, vecs, vec_targets, funcs = build_cached()
    ram = defaultdict(list)
    for off in range(0, N-3, 4):
        w = struct.unpack_from('<I', b, off)[0]
        if not (0x20000000 <= w < 0x20050000):
            continue
        va_storage = APP_BASE + off
        sites = []
        for back in range(2, 0x200, 2):
            so = off - back
            if so < 0:
                break
            va = APP_BASE + so
            ins = next(md.disasm(b[so:so+4], va), None)
            if ins is None:
                continue
            if ins.mnemonic.startswith('ldr') and '[pc' in ins.op_str:
                try:
                    disp = int(ins.op_str.split('#')[1].split(']')[0], 0)
                except Exception:
                    continue
                tgt = (va + 4) & ~3
                tgt += disp
                if tgt == va_storage:
                    sites.append(va)
        if sites:
            ram[w].append((off, sites))
    print(f"=== {len(ram)} distinct RAM addresses with validated literals")
    # sort by address
    for w in sorted(ram):
        sites = [s for off, ss in ram[w] for s in ss]
        print(f"  {w:#010x} stores={[(hex(o)) for o, ss in ram[w]][:5]} sites={[hex(s) for s in sites[:6]]}")
    # look for size constants that look like framebuffer sizes
    print()
    print("=== immediates 0x1000-0x40000 in movw/movt (possible buffer sizes)")
    sizes = Counter()
    for off in range(0, N-4, 2):
        hw = struct.unpack_from('<H', b, off)[0]
        if (hw & 0xFBF0) == 0xF240:  # movw
            imm16 = ((hw & 0xF) << 12) | (((hw >> 10) & 1) << 11) | (((hw >> 9) & 1) << 10) | ((hw >> 8) & 1) << 9
            # crude skip
    # simpler: scan for 32-bit words 0x00004000..0x00040000 that look like aligned sizes
    for off in range(0, N-3, 4):
        w = struct.unpack_from('<I', b, off)[0]
        if 0x4000 <= w <= 0x40000 and w % 16 == 0 and w in (0x1C200, 0x25800, 0x1E000, 0x12C00, 0x9600, 0xF000, 0x14000, 0x20000, 0x10000, 0x2D000, 0x16800, 0x25800):
            print(f"  size word {w:#x} ({w}) @file {off:#08x} (va {APP_BASE+off:#010x})")

if __name__ == '__main__':
    main()
