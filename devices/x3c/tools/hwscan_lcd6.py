#!/usr/bin/env python
"""hwscan_lcd6.py - callers of LCD init / screen object, find framebuffer"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

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

def show(va, n=50, label=''):
    print(f'===== {label} {va:#010x} (file {va-APP_BASE:#08x})')
    off = va - APP_BASE
    for insn in md.disasm(b[off:off+n*4], va):
        ann = ''
        if insn.mnemonic.startswith('ldr') and '[pc' in insn.op_str:
            try:
                disp = int(insn.op_str.split('#')[1].split(']')[0], 0)
            except Exception:
                disp = 0
            tgt = (insn.address + 4) & ~3
            tgt += disp
            tf = tgt - APP_BASE
            w = struct.unpack_from('<I', b, tf)[0] if 0 <= tf < N-3 else None
            ann = f'   ; [{tgt:#010x}] = {w:#010x}'
        print(f'  {insn.address:010x}: {insn.bytes.hex():<10} {insn.mnemonic:<8} {insn.op_str}{ann}')

def find_start(va, maxback=0x400):
    off = va - APP_BASE
    for back in range(2, maxback, 2):
        o = off - back
        if o < 0: break
        hw = struct.unpack_from('<H', b, o)[0]
        if (hw & 0xFF00) == 0xB500 or hw == 0xE92D:
            return APP_BASE + o
    return None

if __name__ == '__main__':
    for tgt, lab in [(0x08035A64, 'LCD panel init'), (0x08006134, '448x368 func')]:
        cs = CALLS.get(tgt, [])
        print(f"##### callers of {lab} {tgt:#010x}: {len(cs)}")
        for va in cs[:6]:
            st = find_start(va)
            print(f"  call @{va:#010x} funcstart={hex(st) if st else None}")
            if st:
                show(st, min(70, (va-st)//2 + 12), 'caller')
