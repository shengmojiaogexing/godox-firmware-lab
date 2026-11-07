#!/usr/bin/env python
"""hwscan_disp.py - dump functions around SPI1/GPIO to identify display & RF drivers."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE, build_cached
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def show(va, n=80, label=''):
    print(f'===== {label} {va:#010x} (file {va-APP_BASE:#08x})')
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
            if w is not None and 0x40000000 <= w < 0x60000000:
                ann += '  (MMIO)'
            elif w is not None and 0x20000000 <= w < 0x20060000:
                ann += '  (RAM)'
        print(f'{ins.address:010x}: {ins.bytes.hex():<10} {ins.mnemonic:<8} {ins.op_str}{ann}')

if __name__ == '__main__':
    which = sys.argv[1] if len(sys.argv) > 1 else 'spi'
    if which == 'spi':
        show(0x08013688, 90, 'SPI1+GPIO func')
        print()
        show(0x08012984, 60, 'IRQ35 handler (SPI1)')
    elif which == 'irq67':
        show(0x08010B18, 120, 'IRQ67 handler (OTGFS1?)')
    elif which == 'gpio':
        show(int(sys.argv[2], 16), int(sys.argv[3]) if len(sys.argv) > 3 else 80, 'func')
