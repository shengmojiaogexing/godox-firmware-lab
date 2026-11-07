#!/usr/bin/env python
"""hwscan_base.py - shared basics for X3_C firmware scans.
Loads the image, maps file offset <-> virtual address (base 0x08000000), string table, etc.
"""
import struct, sys, os

_REPO = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                     '..', '..', '..'))
FW = os.path.join(_REPO, 'devices', 'x3c', 'firmware', 'X3_C_v1.38_app.bin')
BASE = 0x08000000

def load(path=FW):
    with open(path, 'rb') as f:
        return f.read()

def u32(b, off):
    return struct.unpack_from('<I', b, off)[0]

def u16(b, off):
    return struct.unpack_from('<H', b, off)[0]

def va_to_off(va):
    return va - BASE

def off_to_va(off):
    return off + BASE

def in_flash(va, size):
    return BASE <= va < BASE + size

def vectors(b):
    """Return (sp, entries) where entries[0..] = reset..SVC..SysTick..IRQn."""
    sp = u32(b, 0)
    n = 0
    entries = []
    for i in range(1, 131):
        v = u32(b, i*4)
        entries.append(v)
    return sp, entries

IRQ_NAMES_HINT = {}

def cstr(b, off, maxlen=4096):
    e = b.find(b'\0', off)
    if e < 0:
        e = min(len(b), off+maxlen)
    return b[off:e]

def find_ascii_strings(b, minlen=4):
    out = []
    i = 0
    n = len(b)
    while i < n:
        c = b[i]
        if 0x20 <= c < 0x7f:
            j = i
            while j < n and (0x20 <= b[j] < 0x7f or b[j] == 0x09):
                j += 1
            if j - i >= minlen:
                out.append((i, b[i:j].decode('latin-1')))
            i = j
        else:
            i += 1
    return out

if __name__ == '__main__':
    b = load()
    print("file size", hex(len(b)))
    sp, e = vectors(b)
    print("SP", hex(sp), "reset", hex(e[0]))
    for i, v in enumerate(e):
        if v and v != e[0]:
            print(f"vec {i+1:3d} (IRQ{i-1:3d}) = {v:08x}")
