#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""xrefs_beyond.py <bin> -- count flash pointers that point beyond EOF, and list top targets.

Also dump tail region as u16/u32 to look for a section/descriptor table.
"""
import sys, struct, collections

BASE = 0x08000000
p = sys.argv[1]
d = open(p, 'rb').read()
n = len(d)


def resolve(v):
    if 0x08000000 <= v < 0x08100000:
        off = v - BASE
        return ('flash', off, off >= n)
    if 0x20000000 <= v < 0x20002000:
        return ('ram', v - 0x20000000, False)
    return (None, v, False)


beyond = collections.Counter()
inside = collections.Counter()
for i in range(0, n - 3, 4):
    v = struct.unpack_from('<I', d, i)[0]
    kind, off, is_beyond = resolve(v)
    if kind == 'flash':
        if is_beyond:
            beyond[v] += 1
        else:
            inside[v] += 1

print(f'file={p} size={n} (0x{n:x})')
print(f'flash ptrs in range: {sum(inside.values())}, beyond EOF: {sum(beyond.values())} (unique {len(beyond)})')
mx = max(beyond) if beyond else 0
print(f'max beyond-EOF target: 0x{mx:08x} (EOF=0x{BASE+n:08x}, gap=0x{0 if not mx else mx-(BASE+n):x})')
print('\nbeyond-EOF targets (target: count):')
for v, c in sorted(beyond.items()):
    print(f'  0x{v:08x} x{c}')
print('\ntail words (last 0x120 bytes):')
start = max(0, n - 0x120)
start -= start % 4
for i in range(start, n, 16):
    chunk = d[i:i + 16]
    ws = ' '.join(f'{int.from_bytes(chunk[j:j+4], "little"):08x}' for j in range(0, len(chunk) - 3, 4))
    hs = ' '.join(f'{int.from_bytes(chunk[j:j+2], "little"):04x}' for j in range(0, len(chunk) - 1, 2))
    print(f'  0x{i:05x}: {ws}   |u16 {hs}')
