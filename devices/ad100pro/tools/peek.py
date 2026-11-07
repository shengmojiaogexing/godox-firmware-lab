#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""peek.py <file> -- print head/tail and vector-ish info for any bin."""
import sys, struct

p = sys.argv[1]
d = open(p, 'rb').read()
print(f'file: {p}')
print(f'size: {len(d)} (0x{len(d):x})')
print('head 64:', d[:64].hex())
print('tail 64:', d[-64:].hex())
if len(d) >= 8:
    print('first 8 words:', [hex(struct.unpack_from('<I', d, 4 * i)[0]) for i in range(8)])
