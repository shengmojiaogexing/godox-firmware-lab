#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""chk_vec.py <bin> [base] -- vector table / reset handler sanity check for Cortex-M images."""
import sys, struct

BASE = int(sys.argv[2], 0) if len(sys.argv) > 2 else 0x08000000
data = open(sys.argv[1], 'rb').read()
size = len(data)
print(f'file={sys.argv[1]} size={size} (0x{size:x}) base=0x{BASE:08x}')


def flash(v):
    return BASE <= v < BASE + 0x100000


def off(v):
    return v - BASE


print('\nvector table:')
for i in range(48):
    v = struct.unpack_from('<I', data, i * 4)[0]
    if v == 0:
        continue
    t = v & ~1
    tag = ''
    if 0x20000000 <= v < 0x20010000:
        tag = 'SP/RAM'
    elif flash(t):
        tag = f'flash+0x{off(t):05x}'
        if off(t) + 2 > size:
            tag += '  *** BEYOND EOF ***'
    else:
        tag = '??'
    print(f'  [{i:2}] 0x{v:08x}  {tag}')

rv = struct.unpack_from('<I', data, 4)[0]
t = rv & ~1
print(f'\nreset target 0x{t:08x}:')
if flash(t) and off(t) + 32 <= size:
    o = off(t)
    print('  hex :', data[o:o + 32].hex())
    print('  u16 :', ' '.join(f'{struct.unpack_from("<H", data, o + 2*k)[0]:04x}' for k in range(8)))
    print('  u32 :', ' '.join(f'{struct.unpack_from("<I", data, o + 4*k)[0]:08x}' for k in range(8)))

print('\nword scan: occurrences of first 0x40 bytes elsewhere?')
head = data[:0x40]
found = []
i = 0
while True:
    j = data.find(head, i)
    if j < 0:
        break
    found.append(j)
    i = j + 1
print(' ', [hex(x) for x in found])

print('\nliterals around 0xC0..0x100:')
print('  ', data[0xC0:0x100].hex())
print('\nwords 0xC0..0x100:')
for k in range(0xC0, 0x100, 4):
    print(f'  {BASE+k:08x}: 0x{struct.unpack_from("<I", data, k)[0]:08x}')
