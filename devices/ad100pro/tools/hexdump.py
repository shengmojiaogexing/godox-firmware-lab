#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""hexdump.py <file> <addr> <len>  -- addr is an absolute address (base 0x08000000)."""
import sys, os

BASE = 0x08000000

def main():
    path, addr, ln = sys.argv[1], int(sys.argv[2], 0), int(sys.argv[3], 0)
    data = open(path, 'rb').read()
    off = addr - BASE
    end = min(off + ln, len(data))
    off &= ~0xF
    while off < end:
        chunk = data[off:off + 16]
        hexs = ' '.join(f'{b:02x}' for b in chunk)
        asc = ''.join(chr(b) if 32 <= b < 127 else '.' for b in chunk)
        words = ' '.join(f'{int.from_bytes(chunk[i:i+4], "little"):08x}'
                         for i in range(0, len(chunk) - 3, 4))
        print(f'{BASE + off:08x}  {hexs:<47}  {asc:<16}  {words}')
        off += 16

if __name__ == '__main__':
    main()
