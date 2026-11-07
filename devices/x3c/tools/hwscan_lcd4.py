#!/usr/bin/env python
"""hwscan_lcd4.py - find window (0x2A/0x2B) setup: cmd constants + following writes = resolution"""
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
        CALLS.setdefault(t, []).append(off)

def ins(off):
    return next(md.disasm(b[off:off+4], APP_BASE+off), None)

def show_ctx(va, before=0, after=40):
    print(f'--- @{va:#010x} (file {va-APP_BASE:#08x})')
    off = va - APP_BASE
    for insn in md.disasm(b[off:off+after*4], va):
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
            ann = f'   ; = {w:#010x}'
        print(f'  {insn.address:010x}: {insn.mnemonic:<8} {insn.op_str}{ann}')

def scan_targets(targets):
    for t in targets:
        print(f"===== call sites of {t:#010x}: {len(CALLS.get(t,[]))}")
        for off in CALLS.get(t, [])[:40]:
            va = APP_BASE + off
            # find previous cmds
            prev = []
            for back in range(2, 14, 2):
                i2 = ins(off-back)
                if i2 is None: continue
                if i2.mnemonic.startswith('movs') and i2.op_str.startswith('r0'):
                    prev.append(i2.op_str)
            print(f"  {va:#010x} file {off:#08x}  r0defs={prev}")

if __name__ == '__main__':
    # low-level writers: cmd 0x8025E94, data 0x8025E9C ; wrappers 0x8035BC4 / 0x8035BE8
    scan_targets([0x08025E94, 0x08025E9C, 0x08035BC4, 0x08035BE8])
    print()
    # Find the 0x2A/0x2B window setup by scanning for cmd 0x2A in random-access code
    for off in range(0, N-4, 2):
        i = ins(off)
        if i is None: continue
        if i.mnemonic == 'movs' and i.op_str in ('r0, #0x2a', 'r0, #0x2b'):
            # next instruction should be a bl to a writer
            nxt = ins(off+2)
            if nxt is not None and nxt.mnemonic == 'bl':
                t = decode_bl(off+2)
                if t in (0x08025E94, 0x08035BC4):
                    print(f"### window cmd {i.op_str} at {APP_BASE+off:#010x}")
                    show_ctx(APP_BASE+off, after=30)
