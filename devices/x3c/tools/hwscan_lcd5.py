#!/usr/bin/env python
"""hwscan_lcd5.py - display geometry, set-window func, framebuffer hunt"""
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

def find_func_start(va, maxback=0x200):
    off = va - APP_BASE
    for back in range(2, maxback, 2):
        o = off - back
        if o < 0: break
        hw = struct.unpack_from('<H', b, o)[0]
        if (hw & 0xFF00) == 0xB500 or hw == 0xE92D:
            # check previous bytes look like a terminal (pop/bx/b)
            return APP_BASE + o
    return None

def search_vals(vals):
    print("### search words:", [hex(v) for v in vals])
    for off in range(0, N-3, 4):
        w = struct.unpack_from('<I', b, off)[0]
        if w in vals:
            print(f"  {w:#x} @file {off:#08x} (va {APP_BASE+off:#010x})")

if __name__ == '__main__':
    show(0x080060BE, 30, 'set-window caller (2A/2B)')
    print()
    st = find_func_start(0x0800CE2C)
    print("set-window func start:", hex(st) if st else None)
    if st:
        show(st, 40, 'set-window')
    print()
    show(0x08006134, 70, 'draw? 448x368 constants')
    print()
    search_vals([0x1C0, 0x170, 0x28400, 0x50800, 0xE0, 0x1C0 * 2, 0x170 * 2])
