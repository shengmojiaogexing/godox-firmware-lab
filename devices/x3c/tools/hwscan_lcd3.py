#!/usr/bin/env python
"""hwscan_lcd3.py - LCD low-level writers, callers, and framebuffer hunt."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from collections import Counter

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

def callers(target, ctx=10):
    cs = CALLS.get(target, [])
    print(f"=== callers of {target:#010x}: {len(cs)}")
    for va in cs[:20]:
        print(f"--- call @{va:#010x}")
        off = va - APP_BASE
        start = max(0, off - ctx*2)
        # disassemble a window ending at call
        for ins in md.disasm(b[start:off+4], APP_BASE+start):
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
            print(f"  {ins.address:010x}: {ins.bytes.hex():<10} {ins.mnemonic:<8} {ins.op_str}{ann}")

print("### literals 0x6C000000..0x6C000002 anywhere:")
for off in range(0, N-3, 4):
    w = struct.unpack_from('<I', b, off)[0]
    if 0x6C000000 <= w <= 0x6C000002:
        print(f"  word {w:#010x} @file {off:#08x} (va {APP_BASE+off:#010x})")
print()
show(0x08025E94, 20, 'lcd write A?')
print()
show(0x08025E9C, 20, 'lcd write B?')
print()
show(0x08028210, 20, 'lcd tail?')
print()
callers(0x0801C2C0)
