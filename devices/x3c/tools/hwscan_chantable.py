#!/usr/bin/env python
"""hwscan_chantable.py - decode the Godox channel table at vaddr 0x08045390 (CC2500 FREQ2/1/0)"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE

b = load()
TBL_VA = 0x08045390
off = TBL_VA - APP_BASE
raw = b[off:off+99]
f2 = list(raw[0:33]); f1 = list(raw[33:66]); f0 = list(raw[66:99])
print(f"table @file {off:#08x} (va {TBL_VA:#010x})")
print("FREQ2:", ' '.join(f'{v:02x}' for v in f2))
print("FREQ1:", ' '.join(f'{v:02x}' for v in f1))
print("FREQ0:", ' '.join(f'{v:02x}' for v in f0))
s3_half = [4826, 4829, 4832, 4836, 4839, 4842, 4846, 4849, 4852, 4856, 4859, 4862, 4866,
           4869, 4872, 4876, 4879, 4882, 4886, 4889, 4892, 4896, 4899, 4902, 4906, 4909,
           4912, 4916, 4919, 4922, 4926, 4929]
xtal = 26_000_000.0
print(f"\n{'idx':>3} {'FREQ24':>10} {'f(MHz,26MHz xtal)':>18} {'round half-MHz':>14} {'S3 ch?':>10}")
for i in range(33):
    F = (f2[i] << 16) | (f1[i] << 8) | f0[i]
    mhz = F * xtal / 65536.0 / 1e6
    hm = round(mhz * 2)
    s3 = ''
    for k, v in enumerate(s3_half):
        if v == hm:
            s3 = f'ch{k+1}'
            break
    print(f'{i:3d} {F:10d} {mhz:18.4f} {hm:14d} {s3:>10}')
