#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""compare.py A.bin B.bin -- find matching/diverging ranges between two firmware images."""
import sys

a = open(sys.argv[1], 'rb').read()
b = open(sys.argv[2], 'rb').read()
n = min(len(a), len(b))
print(f'A={len(a)} B={len(b)} compare {n} bytes')

same = [a[i] == b[i] for i in range(n)]
print(f'identical: {sum(same)}/{n} ({100*sum(same)/n:.1f}%)')

# runs
runs = []
i = 0
while i < n:
    j = i
    while j < n and same[j] == same[i]:
        j += 1
    if j - i >= 16:
        runs.append(('SAME' if same[i] else 'DIFF', i, j))
    i = j
print('\nruns >= 16 bytes:')
for t, s, e in runs:
    print(f'  {t} 0x{s:05x}-0x{e:05x} len=0x{e-s:x}')

# where does b match a at shift?
print('\nsearch best shift for b inside a (b[0:0x2000]):')
needle = b[0x2000:0x3000]
pos = a.find(needle)
print('  b[0x2000:0x3000] at a offset', hex(pos) if pos >= 0 else 'not found')
for shift in range(-0x400, 0x401, 2):
    m = 0
    tot = 0
    for i in range(0x2000, 0x4000):
        j = i + shift
        if 0 <= j < len(a):
            tot += 1
            if a[j] == b[i]:
                m += 1
    if tot and m / tot > 0.98:
        print(f'  shift {shift:+x} match {m}/{tot}')
