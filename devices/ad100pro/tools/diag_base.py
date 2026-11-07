#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Check the load-base hypothesis: collect all flash-like pointer values."""
import os
import struct
import collections

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # devices/ad100pro
d = open(os.path.join(ROOT, 'firmware', 'AD100pro_V1.2.bin'), 'rb').read()
vals = [struct.unpack_from('<I', d, i)[0] for i in range(0, len(d) - 3, 4)]
fl = [v for v in vals if 0x08000000 <= v < 0x08010000]
print('flash-valued words:', len(fl))
print('min 0x%08x  max 0x%08x' % (min(fl), max(fl)))
lo = collections.Counter(v for v in fl if v < 0x08003000)
print('values below 0x08003000:', {hex(k): v for k, v in sorted(lo.items())})
hi = sorted(v for v in fl if v >= 0x0800f448)
print('values >= 0x0800f448:', [hex(v) for v in hi])
