#!/usr/bin/env python
"""hwscan_adr.py - find ADR-based (PC-relative) references to strings/data, plus RAM usage histogram."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import build_cached, APP_BASE
from hwscan_strings import extract
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from collections import Counter, defaultdict

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def main():
    sp, vecs, vec_targets, funcs = build_cached()
    # 1) RAM literal histogram
    ram = Counter()
    ram_store = defaultdict(list)
    for off in range(0, N-3, 4):
        w = struct.unpack_from('<I', b, off)[0]
        if 0x20000000 <= w < 0x20048000:
            ram[w] += 1
            ram_store[w].append(off)
    print("=== RAM literals by 4KB page")
    pages = Counter()
    for w, c in ram.items():
        pages[w >> 12] += c
    for p in sorted(pages):
        print(f"  0x{p<<12:08x}-0x{(p+1)<<12-1:08x}: {pages[p]} literals")
    print()
    print("=== top RAM literals (likely buffers/globals)")
    for w, c in ram.most_common(40):
        print(f"  {w:#010x} x{c}")

    # 2) ADR-based references: scan whole file, disassemble 16-bit windows to find adr
    print()
    print("=== ADR references")
    strs = {off: s for off, kind, s in extract(b) if kind == 'ascii' and len(s) >= 3}
    hits = defaultdict(list)
    for off in range(0, N-2, 2):
        va = APP_BASE + off
        ins = next(md.disasm(b[off:off+4], va), None)
        if ins is None or ins.mnemonic != 'adr':
            continue
        try:
            tgt = int(ins.op_str.split('#')[1].split(',')[0].replace('#', ''), 0)
        except Exception:
            continue
        tfile = tgt - APP_BASE
        if tfile in strs:
            hits[tfile].append((va, ins.address, ins.bytes.hex(), ins.op_str))
    print(f"{len(hits)} strings reached by ADR")
    for tfile in sorted(hits):
        s = strs[tfile]
        print(f"  str@{tfile:#08x} (va {APP_BASE+tfile:#010x}) {s[:50]!r} <- adr sites {[hex(h[1]) for h in hits[tfile][:5]]}")

if __name__ == '__main__':
    main()
