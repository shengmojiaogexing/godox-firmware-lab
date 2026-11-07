#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Find all flash pointer constants that fall OUTSIDE the app image (0x08003000-0x0800F448)."""
import os, struct, collections

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # devices/ad100pro
IMG_BASE, IMG_SIZE = 0x08003000, 50248
IMG_END = IMG_BASE + IMG_SIZE
d = open(os.path.join(ROOT, 'firmware', 'AD100pro_V1.2.bin'), 'rb').read()
vals = [struct.unpack_from('<I', d, i)[0] for i in range(0, len(d) - 3, 4)]
outside = collections.Counter()
inside060c = collections.Counter()
for v in vals:
    if 0x08000000 <= v < 0x08003000:
        outside[v] += 1
    elif IMG_END <= v < 0x08040000:
        outside[v] += 1
    elif IMG_BASE <= v < IMG_END:
        inside060c[v] += 1
print(f'flash-valued words total: {sum(inside060c.values()) + sum(outside.values())}')
print(f'inside image: {sum(inside060c.values())} (unique {len(inside060c)})')
print(f'outside image: {sum(outside.values())} (unique {len(outside)})')
print('\nout-of-image flash constants:')
for v, c in sorted(outside.items()):
    kind = 'bootloader area' if v < 0x08003000 else 'high flash'
    print(f'  0x{v:08X} x{c:<3} {kind}')
