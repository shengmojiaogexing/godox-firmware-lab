#!/usr/bin/env python
"""hwscan_adr2.py - fixed ADR/PC-relative string xrefs + scan for frequency/channel tables."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE, build_cached
from hwscan_strings import extract
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from collections import defaultdict

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def main():
    sp, vecs, vec_targets, funcs = build_cached()
    # ---- ADR scan: disassemble every even offset, only 'adr' (16-bit ADR) ----
    strs = {off: s for off, kind, s in extract(b) if kind in ('ascii', 'utf8cjk') and len(s) >= 3}
    hits = defaultdict(list)
    for off in range(0, N-4, 2):
        va = APP_BASE + off
        for ins in md.disasm(b[off:off+4], va):
            if ins.mnemonic == 'adr':
                op = ins.op_str
                try:
                    disp = int(op.split('#')[1].split(',')[0], 0)
                except Exception:
                    break
                tgt = ((ins.address + 4) & ~3) + disp
                tfile = tgt - APP_BASE
                if tfile in strs:
                    hits[tfile].append(ins.address)
            break
    print(f"=== {len(hits)} strings referenced by ADR")
    for tfile in sorted(hits)[:120]:
        print(f"  file {tfile:#08x} va {APP_BASE+tfile:#010x} {strs[tfile][:46]!r} <- {[hex(a) for a in hits[tfile][:4]]}")

    # ---- frequency / channel table scan ----
    print()
    print("=== candidate uint16 tables (monotonic, freq-like)")
    for off in range(0, N-4, 2):
        n = 0
        vals = []
        o = off
        while o + 2 <= N and n < 40:
            v = struct.unpack_from('<H', b, o)[0]
            vals.append(v)
            n += 1
            o += 2
        # check heading run
        pass
    # do smarter: find runs of 16+ uint16 that are monotonic-ish and in freq ranges
    for off in range(0, N-2*16, 2):
        vals = list(struct.unpack_from('<16H', b, off))
        def ok(vals):
            if not all(2400 <= v <= 2500 for v in vals): return False
            diffs = [vals[i+1]-vals[i] for i in range(15)]
            return all(1 <= d <= 5 for d in diffs)
        if ok(vals):
            print(f"  freq MHz table @file {off:#08x} (va {APP_BASE+off:#010x}): {vals}")
    for off in range(0, N-2*16, 2):
        vals = list(struct.unpack_from('<16H', b, off))
        if all(4800 <= v <= 5000 for v in vals):
            diffs = [vals[i+1]-vals[i] for i in range(15)]
            if all(1 <= d <= 6 for d in diffs):
                print(f"  freq half-MHz table @file {off:#08x}: {vals}")

if __name__ == '__main__':
    main()
