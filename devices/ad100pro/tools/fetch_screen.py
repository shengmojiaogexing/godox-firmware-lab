#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Fetch /api/screen from a running simulator and render a panel-view PNG."""
import json
import os
import struct
import sys
import urllib.request
import zlib

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


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


def fetch(api):
    with urllib.request.urlopen(api + '/api/screen', timeout=30) as r:
        return json.load(r)


def render(scr, rotate=90, mirror=True, scale=6):
    W, H = scr['w'], scr['h']
    data = bytes.fromhex(scr['hex'])
    if len(data) < W * H // 8:
        raise SystemExit(f'bad screen data {len(data)}')
    pix = [[(data[p * W + x] >> bit) & 1 for x in range(W)] for p in range(H // 8) for bit in range(8)]
    # pix is page-major rows; convert to y,x with bit0 = top
    img = [[0] * W for _ in range(H)]
    for p in range(H // 8):
        for x in range(W):
            b = data[p * W + x]
            for bit in range(8):
                img[p * 8 + bit][x] = (b >> bit) & 1
    if rotate == 90:
        img = [[img[H - 1 - y][x] for y in range(H)] for x in range(W)]
    if mirror:
        img = [list(reversed(row)) for row in img]
    rgb = []
    for row in img:
        line = []
        for v in row:
            line.extend([(240, 244, 250) if v else (16, 20, 26)] * scale)
        for _ in range(scale):
            rgb.append(list(line))
    return rgb


def main():
    api = sys.argv[1] if len(sys.argv) > 1 else 'http://127.0.0.1:8775'
    out = sys.argv[2] if len(sys.argv) > 2 else os.path.join(ROOT, 'out', 'ui_screen.png')
    rotate = int(sys.argv[3]) if len(sys.argv) > 3 else 90
    mirror = (sys.argv[4] != '0') if len(sys.argv) > 4 else True
    scr = fetch(api)
    write_png(out, render(scr, rotate=rotate, mirror=mirror))
    nz = sum(1 for b in bytes.fromhex(scr['hex']) if b)
    print(f'screen {scr["w"]}x{scr["h"]} {scr["format"]} nonzero={nz} -> {out}')


if __name__ == '__main__':
    main()
