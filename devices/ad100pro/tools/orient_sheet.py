#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Build a contact sheet of all 8 framebuffer orientations to pick the panel view."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import lcd_decode as ld          # noqa: E402
from emu.core import Emulator    # noqa: E402

FB, W, H = 0x200004CC, 128, 64


def main():
    e = Emulator()
    e.reset()
    e.run(int(sys.argv[1], 0) if len(sys.argv) > 1 else 6_000_000)
    fb = bytes(e.read_mem(FB, 1024))
    pix = [[(fb[(y // 8) * W + x] >> (y % 8)) & 1 for x in range(W)] for y in range(H)]
    views = [
        ('id', pix),
        ('r90', [[pix[H - 1 - y][x] for y in range(H)] for x in range(W)]),
        ('r180', [[pix[H - 1 - y][W - 1 - x] for x in range(W)] for y in range(H)]),
        ('r270', [[pix[y][W - 1 - x] for y in range(H)] for x in range(W)]),
        ('flipx', [[pix[y][W - 1 - x] for x in range(W)] for y in range(H)]),
        ('flipy', [[pix[H - 1 - y][x] for x in range(W)] for y in range(H)]),
        ('transp', [[pix[x][y] for x in range(H)] for y in range(W)]),
        ('transp_flip', [[pix[x][H - 1 - y] for x in range(H)] for y in range(W)]),
    ]
    sc = 2
    cw, chh = W * sc, H * sc
    sheet = [[(28, 28, 32)] * (4 * cw + 5 * 10) for _ in range(2 * (chh + 12) + 16)]
    for k, (name, m) in enumerate(views):
        col, row = k % 4, k // 4
        ox, oy = 10 + col * (cw + 10), 8 + row * (chh + 12)
        for y in range(len(m)):
            for x in range(len(m[0])):
                c = (240, 244, 250) if m[y][x] else (14, 18, 24)
                for dy in range(sc):
                    for dx in range(sc):
                        yy, xx = oy + y * sc + dy, ox + x * sc + dx
                        if 0 <= yy < len(sheet) and 0 <= xx < len(sheet[0]):
                            sheet[yy][xx] = c
    ld.write_png(os.path.join(ROOT, 'out', 'orient_sheet.png'), sheet)
    print('order:', [v[0] for v in views])


if __name__ == '__main__':
    main()
