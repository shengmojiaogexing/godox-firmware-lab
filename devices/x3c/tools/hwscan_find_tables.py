#!/usr/bin/env python
"""hwscan_find_tables.py - scan image for embedded Cortex-M vector tables (RAM SP + code vectors)."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load, BASE

b = load()
n = len(b)

def word(o):
    return struct.unpack_from('<I', b, o)[0]

def is_ram(w):
    return 0x20000000 <= w < 0x20080000

def is_code(w):
    return 0x08000000 <= w < BASE + n

def starts_prologue(va):
    """look at first 2 bytes at va for common ARMCC/Thumb2 prologue"""
    o = va - BASE
    if not (0 <= o < n - 4):
        return 0
    hw = struct.unpack_from('<H', b, o)[0]
    if (hw & 0xFF00) in (0xB500, 0xB580, 0xB510, 0xB5F0, 0xB5F8, 0xB570, 0xB538, 0xB508, 0xB5):
        return 2
    if hw == 0xE92D:
        return 2
    if (hw & 0xFF80) == 0x4600:  # mov rX, r0 (some leaves)
        return 1
    if (hw & 0xFFF0) == 0xB080:  # sub sp
        return 1
    if hw == 0x4FF0:  # mov.w fp? rare
        return 1
    return 0

cands = []
for o in range(0, n - 16, 4):
    w0 = word(o)
    if not is_ram(w0):
        continue
    w1 = word(o+4)
    if not is_code(w1):
        continue
    # count following code/zero words
    good = 0
    pro = 0
    k = 0
    while True:
        oo = o + 8 + 4*k
        if oo + 4 > n:
            break
        wk = word(oo)
        if wk == 0:
            k += 1
            if k > 200:
                break
            continue
        if not is_code(wk):
            break
        good += 1
        pro += starts_prologue(wk & ~1)
        k += 1
        if k > 250:
            break
    if good >= 20:
        cands.append((good, pro, o, w0, w1))

cands.sort(reverse=True)
for good, pro, o, w0, w1 in cands[:25]:
    print(f"table @file {o:#08x} (va {BASE+o:#010x}): sp={w0:#010x} vec1={w1:#010x} code_words={good} prologues={pro}")
