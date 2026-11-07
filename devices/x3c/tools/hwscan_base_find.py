#!/usr/bin/env python
"""hwscan_base_find.py - find the true image base by testing literal->string alignment."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load

b = load()
n = len(b)

# collect all 32-bit aligned words that look like flash addresses in 0x08000000..0x08100000
words = {}
for off in range(0, n-3, 4):
    w = struct.unpack_from('<I', b, off)[0]
    if 0x08000000 <= w <= 0x08100000:
        words.setdefault(w, []).append(off)

def printable_run(off, minlen=6):
    if off < 0 or off >= n:
        return 0
    c = b[off]
    if not (0x20 <= c < 0x7f):
        return 0
    j = off
    while j < n and 0x20 <= b[j] < 0x7f:
        j += 1
    return j - off

import collections
best = []
for B in range(0x08000000, 0x08040001, 0x400):
    hits = 0
    strhits = 0
    for w in words:
        off = w - B
        if 0 <= off < n:
            r = printable_run(off)
            if r >= 6:
                strhits += 1
    best.append((strhits, B))
best.sort(reverse=True)
for s, B in best[:10]:
    print(f"base {B:#010x}: string-pointer hits = {s}")

print()
# For top base, dump sample strings referenced
top = best[0][1]
print(f"=== samples for base {top:#010x}")
cnt = 0
for w in sorted(words):
    off = w - top
    if 0 <= off < n and printable_run(off) >= 6:
        print(f"  literal {w:#010x} (stored at file {words[w][0]:#08x}) -> file {off:#08x} : {b[off:off+printable_run(off)][:60]!r}")
        cnt += 1
        if cnt > 25:
            break
