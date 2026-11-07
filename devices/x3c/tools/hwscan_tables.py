#!/usr/bin/env python
"""hwscan_tables.py - find channel/frequency tables and dump EXINT helper functions."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE, build_cached
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def show(va, n=60):
    print(f'===== {va:#010x} (file {va-APP_BASE:#08x})')
    off = va - APP_BASE
    for ins in md.disasm(b[off:off+n*4], va):
        ann = ''
        if ins.mnemonic.startswith('ldr') and '[pc' in ins.op_str:
            try:
                disp = int(ins.op_str.split('#')[1].split(']')[0], 0)
            except Exception:
                disp = 0
            tgt = (ins.address + 4) & ~3
            tgt += disp
            tf = tgt - APP_BASE
            w = struct.unpack_from('<I', b, tf)[0] if 0 <= tf < N-3 else None
            ann = f'   ; [{tgt:#010x}] = {w:#010x}'
        print(f'{ins.address:010x}: {ins.bytes.hex():<10} {ins.mnemonic:<8} {ins.op_str}{ann}')

def tables():
    print("### uint16 tables 2400..2500 or 4800..5000 (24+ entries)")
    for rng, lo, hi, step in (('MHz', 2400, 2500, 6), ('halfMHz', 4800, 5000, 6)):
        hits = 0
        for off in range(0, N-2*24, 2):
            vals = list(struct.unpack_from('<24H', b, off))
            if all(lo <= v <= hi for v in vals):
                diffs = [vals[i+1]-vals[i] for i in range(23)]
                if all(1 <= d <= step for d in diffs):
                    print(f"  [{rng}] @file {off:#08x} (va {APP_BASE+off:#010x}): {vals}")
                    hits += 1
                    if hits > 10:
                        break
    print("### uint8 tables 8..90 (24+ increasing entries)")
    for off in range(0, N-24):
        vals = list(b[off:off+24])
        if all(8 <= v <= 90 for v in vals):
            diffs = [vals[i+1]-vals[i] for i in range(23)]
            if all(0 <= d <= 4 for d in diffs) and sum(diffs) > 8:
                print(f"  @file {off:#08x} (va {APP_BASE+off:#010x}): {vals}")
    print("### uint32 tables of Hz (2.4e9..2.5e9)")
    for off in range(0, N-4*16, 4):
        vals = list(struct.unpack_from('<16I', b, off))
        if all(2400000000 <= v <= 2500000000 for v in vals):
            print(f"  @file {off:#08x}: {[hex(v) for v in vals]}")
    print("### uint32 tables of kHz/MHz (2300..2600, increasing)")
    for off in range(0, N-4*16, 4):
        vals = list(struct.unpack_from('<16I', b, off))
        if all(2300 <= v <= 2600 for v in vals) and all(vals[i+1] > vals[i] for i in range(15)):
            print(f"  @file {off:#08x}: {vals}")

if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] == 'tables':
        tables()
    else:
        show(0x08020978, 50)
        print()
        show(0x08020984, 50)
