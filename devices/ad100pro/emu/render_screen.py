#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""render_screen.py -- run the firmware in the Unicorn emulator and dump the LCD.

The app keeps its 128x64 monochrome framebuffer at RAM 0x200004CC (8 pages x
128 columns, documented in docs/04_ui_lcd.md), which is the ground truth for
what the panel shows.  The refresh routine streams it to the SH1106 panel.

Usage:
  python emu/render_screen.py --run 6000000 --out out/screen.png
  python emu/render_screen.py --frames 5 --out out/screen_%d.png   (capture several)
"""
import argparse
import os
import sys
import zlib
import struct

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from emu.core import Emulator  # noqa: E402

FB_ADDR = 0x200004CC
FB_SIZE = 1024
W, H = 128, 64


def write_png(path, rgb):
    h, w = len(rgb), len(rgb[0])
    raw = bytearray()
    for row in rgb:
        raw.append(0)
        for (r, g, b) in row:
            raw += bytes((r, g, b))

    def chunk(tag, data):
        return struct.pack('>I', len(data)) + tag + data + \
            struct.pack('>I', zlib.crc32(tag + data) & 0xFFFFFFFF)

    png = b'\x89PNG\r\n\x1a\n'
    png += chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0))
    png += chunk(b'IDAT', zlib.compress(bytes(raw), 9))
    png += chunk(b'IEND', b'')
    open(path, 'wb').write(png)


def render(fb, rotate=90, scale=6, fg=(240, 244, 250), bg=(14, 18, 24)):
    pix = [[0] * W for _ in range(H)]
    for y in range(H):
        page, bit = y // 8, y % 8
        for x in range(W):
            if fb[page * W + x] & (1 << bit):
                pix[y][x] = 1
    if rotate == 90:      # panel is mounted rotated 90 deg relative to controller RAM
        pix = [[pix[H - 1 - y][x] for y in range(H)] for x in range(W)]
    elif rotate == 180:
        pix = [[pix[H - 1 - y][W - 1 - x] for x in range(W)] for y in range(H)]
    elif rotate == 270:
        pix = [[pix[y][W - 1 - x] for y in range(H)] for x in range(W)]
    rgb = []
    for row in pix:
        line = []
        for v in row:
            c = fg if v else bg
            line.extend([c] * scale)
        for _ in range(scale):
            rgb.append(line)
    return rgb


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--run', type=lambda s: int(s, 0), default=6_000_000)
    ap.add_argument('--out', default='out/screen.png')
    ap.add_argument('--rotate', type=int, default=90, choices=(0, 90, 180, 270))
    ap.add_argument('--scale', type=int, default=6)
    ap.add_argument('--calib', choices=('blank', 'synthetic'), default='blank')
    args = ap.parse_args()
    os.makedirs(os.path.dirname(os.path.abspath(args.out)), exist_ok=True)
    emu = Emulator(calib=args.calib)
    emu.reset()
    emu.run(args.run)
    fb = emu.read_mem(FB_ADDR, FB_SIZE)
    if isinstance(fb, (bytes, bytearray)):
        fb = bytes(fb)
    else:
        fb = bytes(fb)
    write_png(args.out, render(fb, args.rotate, args.scale))
    nz = sum(1 for b in fb if b)
    print(f'ran {emu.insn_count} instructions, pc=0x{emu.state_dict()["pc"]:08X}')
    print(f'framebuffer 0x{FB_ADDR:08X}: {nz} non-zero bytes / {FB_SIZE}')
    print(f'wrote {args.out}')


if __name__ == '__main__':
    main()
