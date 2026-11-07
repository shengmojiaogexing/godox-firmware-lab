#!/usr/bin/env python
"""hwscan_ad200.py - deep look at AD200pro small firmware layout"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
BASE = 0x08000000
_REPO = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                     '..', '..', '..'))
path = os.path.join(_REPO, 'devices', 'ad200pro', 'firmware', 'AD200pro_V1.60.bin')
b = open(path, 'rb').read()

def d(va, n=24, label=''):
    off = va - BASE
    print(f'--- {label} va {va:#010x} (file {off:#06x})')
    for ins in md.disasm(b[off:off+n*4], va):
        print(f'{ins.address:08x}: {ins.bytes.hex():<10} {ins.mnemonic} {ins.op_str}')

d(0x08000084, 12, 'stub1')
d(0x08000098, 8, 'reset?')
d(0x080030C0, 20, 'vec1-0x14')
d(0x080030D4, 20, 'vec1 target')
d(0x0800BD0C, 12, 'SystemInit?')
d(0x0800CDB0, 24, 'jump target from 0x84')
print()
# dump first 0x100 words with offset annotations
for o in range(0, 0xC0, 4):
    w = struct.unpack_from('<I', b, o)[0]
    print(f'{o:#06x}: {w:#010x}')
