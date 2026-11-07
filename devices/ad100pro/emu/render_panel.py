#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Render the LCD panel view (framebuffer + panel mounting rotation/mirror)."""
import os
import sys
import zlib
import struct

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, ROOT)
from emu.core import Emulator  # noqa: E402

FB_ADDR, W, H = 0x200004CC, 128, 64


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

    png = b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)) \
        + chunk(b'IDAT', zlib.compress(bytes(raw), 9)) + chunk(b'IEND', b'')
    open(path, 'wb').write(png)


def main():
    emu = Emulator()
    emu.reset()
    steps = int(sys.argv[1], 0) if len(sys.argv) > 1 else 6_000_000
    out = sys.argv[2] if len(sys.argv) > 2 else os.path.join(ROOT, 'out', 'screen_panel.png')
    emu.run(steps)
    fb = bytes(emu.read_mem(FB_ADDR, 1024))
    pix = [[(fb[(y // 8) * W + x] >> (y % 8)) & 1 for x in range(W)] for y in range(H)]
    # panel is mounted rotated 90 deg and mirrored relative to controller RAM
    q = [[pix[H - 1 - y][x] for y in range(H)] for x in range(W)]
    q = [list(reversed(row)) for row in q]
    scale = 6
    rgb = []
    for row in q:
        line = []
        for v in row:
            line.extend([(240, 244, 250) if v else (14, 18, 24)] * scale)
        for _ in range(scale):
            rgb.append(list(line))
    write_png(out, rgb)
    print(f'insn={emu.insn_count} pc=0x{emu.state_dict()["pc"]:08X} -> {out} ({len(rgb[0])}x{len(rgb)})')


if __name__ == '__main__':
    main()
