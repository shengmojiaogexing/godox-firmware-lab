#!/usr/bin/env python
"""hwscan_lcd.py - dump LCD/XMC related functions."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def show(va, n=60, label=''):
    print(f'===== {label} {va:#010x} (file {va-APP_BASE:#08x})')
    off = va - APP_BASE
    if off < 0 or off >= N:
        print('  <out of range>'); return
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
            if w is not None and 0x40000000 <= w < 0x60000000: ann += ' (MMIO)'
            elif w is not None and 0x20000000 <= w < 0x20060000: ann += ' (RAM)'
            elif w is not None and 0x6C000000 <= w < 0x6D000000: ann += ' (XMC/LCD)'
        print(f'{ins.address:010x}: {ins.bytes.hex():<10} {ins.mnemonic:<8} {ins.op_str}{ann}')

RANGES = [
    (0x08035B98, 20, 'LCD init tail'),
    (0x08035BC4, 20, 'LCD cmd writer'),
    (0x08035BE8, 20, 'LCD data writer'),
    (0x08035C00, 30, 'LCD buffer push'),
    (0x0801C118, 40, 'delay/DMA func'),
    (0x08035A42, 15, 'pre-init XMC calls'),
]

if __name__ == '__main__':
    if len(sys.argv) > 1:
        for a in sys.argv[1:]:
            va, n = (int(x, 16) for x in a.split(':'))
            show(va, n)
    else:
        for va, n, lab in RANGES:
            show(va, n, lab)
