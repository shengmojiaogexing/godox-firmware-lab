#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Search for a Cortex-M vector table anywhere in the image; report candidates."""
import sys, struct, capstone
from capstone import arm_const

BASE = 0x08000000
md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB + capstone.CS_MODE_MCLASS)
md.detail = True

data = open(sys.argv[1], 'rb').read()
N = len(data)


def w(off):
    return struct.unpack_from('<I', data, off)[0]


def decode1(addr):
    off = addr - BASE
    if off < 0 or off + 2 > N:
        return None
    for i in md.disasm(data[off:off + 4], addr):
        return i
    return None


def in_flash(v):
    return BASE <= v < BASE + 0x10000


def in_ram(v):
    return 0x20000000 <= v < 0x20002000


def look(off):
    if off + 4 * 20 > N:
        return None
    sp = w(off)
    rst = w(off + 4)
    score = 0
    notes = []
    if not in_ram(sp):
        return None
    score += 2
    if not (rst & 1 and in_flash(rst)):
        return None
    score += 2
    # reset handler should look like a function
    i = decode1(rst & ~1)
    if i is None:
        return None
    if i.mnemonic in ('push', 'ldr', 'sub', 'mov', 'bl', 'b', 'bx'):
        score += 2
        notes.append(f'rst={i.mnemonic} {i.op_str}')
    # subsequent vectors: 0 or odd flash pointers
    good = 0
    for k in range(2, 20):
        v = struct.unpack_from('<I', data, off + 4 * k)[0]
        if v == 0:
            good += 1
        elif v & 1 and in_flash(v):
            good += 1
        else:
            break
    score += good
    notes.append(f'vectors_ok={good}')
    return score, notes


best = []
for off in range(0, N - 4 * 20, 4):
    r = look(off)
    if r:
        best.append((r[0], off, r[1]))
best.sort(reverse=True)
print(f'candidates: {len(best)}')
for score, off, notes in best[:10]:
    print(f'off=0x{off:06x} addr=0x{BASE+off:08x} score={score} {notes}')

# also report what is at the official reset vector target
rv = w(4)
print(f'\nofficial reset vector = 0x{rv:08x} -> target 0x{rv & ~1:08x}')
print(f'  bytes: {data[(rv & ~1) - BASE:(rv & ~1) - BASE + 16].hex()}')
i = decode1(rv & ~1)
print(f'  decodes: {i.mnemonic} {i.op_str} size={i.size}' if i else '  no decode')
