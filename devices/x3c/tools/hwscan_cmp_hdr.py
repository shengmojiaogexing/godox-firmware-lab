#!/usr/bin/env python
"""hwscan_cmp_hdr.py - compare starts of Godox firmware files"""
import os
import struct
_REPO = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                     '..', '..', '..'))
a = open(os.path.join(_REPO, 'devices', 'ad200pro', 'firmware',
                      'AD200pro_V1.60.bin'), 'rb').read()
c = open(os.path.join(_REPO, 'devices', 'ad100pro', 'firmware',
                      'AD100pro_V1.2.bin'), 'rb').read()
b = open(os.path.join(_REPO, 'devices', 'x3c', 'firmware',
                      'X3_C_v1.38_app.bin'), 'rb').read()
print('AD200 len', hex(len(a)), 'AD100 len', hex(len(c)))
for o in range(0, 0x80, 4):
    w1 = struct.unpack_from('<I', a, o)[0]
    w2 = struct.unpack_from('<I', c, o)[0]
    same = 'SAME' if w1 == w2 else ''
    print(f'{o:#04x}: AD200 {w1:#010x}   AD100 {w2:#010x}  {same}')
print()
print('AD200 first 0x100:', a[:0x100].hex(' '))
print('AD100 first 0x100:', c[:0x100].hex(' '))
print()
print('X3 first 0x60:', b[:0x60].hex(' '))
