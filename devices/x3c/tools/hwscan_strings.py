#!/usr/bin/env python
"""hwscan_strings.py - dump ASCII/UTF-8 strings from X3_C with file offset + vaddr (base 0x08004800)."""
import sys, os, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load

APP_BASE = 0x08004800
IMG2_BASE = 0x08002000

b = load()

def extract(b, lo=0, hi=None):
    hi = hi if hi is not None else len(b)
    out = []
    i = lo
    while i < hi:
        c = b[i]
        # ASCII run
        if 0x20 <= c < 0x7f:
            j = i
            while j < hi and (0x20 <= b[j] < 0x7f):
                j += 1
            if j - i >= 4:
                out.append((i, 'ascii', b[i:j].decode('latin-1')))
            i = j
            continue
        # UTF-8 CJK run: try decode 3-byte seq
        if c >= 0xE0 and i + 2 < hi and b[i+1] >= 0x80 and b[i+2] >= 0x80:
            j = i
            s = ''
            ok = True
            while j + 2 < hi and b[j] >= 0xE0 and b[j+1] >= 0x80 and b[j+2] >= 0x80:
                try:
                    ch = b[j:j+3].decode('utf-8')
                except Exception:
                    ok = False
                    break
                s += ch
                j += 3
            if len(s) >= 2:
                out.append((i, 'utf8cjk', s))
            i = max(j, i+1)
            continue
        i += 1
    return out

if __name__ == '__main__':
    lo = int(sys.argv[1], 16) if len(sys.argv) > 1 else 0
    hi = int(sys.argv[2], 16) if len(sys.argv) > 2 else len(b)
    strs = extract(b, lo, hi)
    print(f"# {len(strs)} strings in file [{lo:#x},{hi:#x})")
    for off, kind, s in strs:
        if off < 0x0F3800:
            va = APP_BASE + off
        else:
            va = IMG2_BASE + (off - 0x0F3800)
        print(f"{off:#08x}\t{va:#010x}\t{kind}\t{s}")
