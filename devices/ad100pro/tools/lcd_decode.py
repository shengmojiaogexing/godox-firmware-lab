#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Decode the bit-banged LCD serial stream from a boottest trace and render a PNG.

Pins (from static analysis of 0x0800A480 / 0x0800A426 / 0x08007426):
  GPIOB bit6 (0x040) : SCL   (clock, active high pulse)
  GPIOB bit7 (0x080) : A0/RS (1 = data, 0 = command)
  GPIOB bit9 (0x200) : CS    (active low)
  GPIOA bit10(0x400) : SDA   (data, MSB first)

Usage: python tools/lcd_decode.py out/boottest_trace.log --out out/lcd.png
"""
import sys, os, re, struct, zlib, collections

TRACE_RE = re.compile(r'^\s*(\d+)\s+([0-9A-F]{8})\s+W (\S+)\s+= 0x([0-9A-F]{8})')

SCL, A0, CS = 0x040, 0x080, 0x200
SDA = 0x400

# --- controller model (ST7565/ST7567/UC1701 compatible, 128x64) ---
W, H = 128, 64


class Lcd:
    def __init__(self):
        self.ram = [[0] * W for _ in range(8)]     # 8 pages x 128 columns, bit0=top
        self.page = 0
        self.col = 0
        self.start_line = 0
        self.adc_rev = False
        self.com_rev = False
        self.inverse = False
        self.all_on = False
        self.contrast = 0
        self.cmd_data = []       # (a0, byte) log
        self.data_writes = 0

    def write(self, a0, b):
        if a0:                   # data
            if self.page < 8:
                self.ram[self.page][self.col % W] = b
            self.col = (self.col + 1) % W
            self.data_writes += 1
            return
        # command
        if 0xB0 <= b <= 0xB7:
            self.page = b & 7
        elif b <= 0x0F:
            self.col = (self.col & 0xF0) | b
        elif 0x10 <= b <= 0x1F:
            self.col = (self.col & 0x0F) | ((b & 0x0F) << 4)
        elif 0x40 <= b <= 0x7F:
            self.start_line = b & 0x3F
        elif b == 0xA0:
            self.adc_rev = False
        elif b == 0xA1:
            self.adc_rev = True
        elif b == 0xA6:
            self.inverse = False
        elif b == 0xA7:
            self.inverse = True
        elif b == 0xA4:
            self.all_on = False
        elif b == 0xA5:
            self.all_on = True
        elif b == 0xC0:
            self.com_rev = False
        elif b == 0xC8:
            self.com_rev = True
        elif b == 0x81:
            self._expect_contrast = True
        elif b == 0xE2:
            self.ram = [[0] * W for _ in range(8)]

    def render_rgb(self, scale=4):
        out = [[(0, 0, 0)] * (W * scale) for _ in range(H * scale)]
        for y in range(H):
            page, bit = y // 8, y % 8
            for x in range(W):
                v = (self.ram[page][x] >> bit) & 1
                if self.inverse:
                    v ^= 1
                if self.all_on:
                    v = 1
                # our memory mapping: bit set = pixel on (mono LCD)
                c = (255, 255, 255) if v else (10, 14, 20)
                for sy in range(scale):
                    row = out[y * scale + sy]
                    for sx in range(scale):
                        row[x * scale + sx] = c
        return out


def write_png(path, rgb):
    h, w = len(rgb), len(rgb[0])
    raw = bytearray()
    for row in rgb:
        raw.append(0)
        for (r, g, b) in row:
            raw += bytes((r, g, b))

    def chunk(tag, data):
        c = struct.pack('>I', len(data)) + tag + data
        return c + struct.pack('>I', zlib.crc32(tag + data) & 0xFFFFFFFF)

    png = b'\x89PNG\r\n\x1a\n'
    png += chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0))
    png += chunk(b'IDAT', zlib.compress(bytes(raw), 9))
    png += chunk(b'IEND', b'')
    open(path, 'wb').write(png)


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else 'out/boottest_trace.log'
    out = 'out/lcd.png'
    rotate = 0
    for i, a in enumerate(sys.argv):
        if a == '--out':
            out = sys.argv[i + 1]
        if a == '--rotate':
            rotate = int(sys.argv[i + 1])
    gpiob = 0
    gpioa = 0
    lcd = Lcd()
    bits = 0
    a0 = 0
    cs = 1
    prev_scl = 0
    nb = 0
    bytes_cmd = 0
    stream = []
    for line in open(path, encoding='utf-8'):
        m = TRACE_RE.match(line)
        if not m:
            continue
        insn, pc, reg, val = int(m.group(1)), int(m.group(2), 16), m.group(3), int(m.group(4), 16)
        if reg.startswith('GPIOB'):
            if reg.endswith('BSRR'):
                gpiob |= val & 0xFFFF
                gpiob &= ~((val >> 16) & 0xFFFF)
            elif reg.endswith('BRR'):
                gpiob &= ~(val & 0xFFFF)
            else:
                continue
        elif reg.startswith('GPIOA'):
            if reg.endswith('BSRR'):
                gpioa |= val & 0xFFFF
                gpioa &= ~((val >> 16) & 0xFFFF)
            elif reg.endswith('BRR'):
                gpioa &= ~(val & 0xFFFF)
            else:
                continue
        else:
            continue
        scl = 1 if gpiob & SCL else 0
        cs_new = 0 if not (gpiob & CS) else 1   # CS is active-low
        if cs_new == 0 and cs != 0:
            # CS asserted: start transaction, latch A0
            a0 = 1 if gpiob & A0 else 0
        cs = cs_new
        if scl == 1 and prev_scl == 0 and cs == 0:
            bit = 1 if gpioa & SDA else 0
            bits = (bits << 1) | bit
            nb += 1
            if nb == 8:
                lcd.write(a0, bits)
                stream.append((a0, bits))
                nb = 0
                bits = 0
        prev_scl = scl
    print(f'decoded bytes: {len(stream)}  data writes: {lcd.data_writes}  pages: {lcd.page} '
          f'col: {lcd.col}  inverse={lcd.inverse} all_on={lcd.all_on}')
    print('first 40 bytes:', ' '.join(('D:' if a else 'C:') + f'{b:02X}' for a, b in stream[:40]))
    # raw pixel matrix: rows = y (0..H-1), cols = x (0..W-1); 1 = lit
    pix = [[0] * W for _ in range(H)]
    for y in range(H):
        page, bit = y // 8, y % 8
        for x in range(W):
            v = (lcd.ram[page][x] >> bit) & 1
            if lcd.inverse:
                v ^= 1
            if lcd.all_on:
                v = 1
            pix[y][x] = v
    if rotate == 90:
        pix2 = [[pix[H - 1 - y][x] for y in range(H)] for x in range(W)]
    elif rotate == 180:
        pix2 = [[pix[H - 1 - y][W - 1 - x] for x in range(W)] for y in range(H)]
    elif rotate == 270:
        pix2 = [[pix[y][W - 1 - x] for y in range(H)] for x in range(W)]
    else:
        pix2 = pix
    scale = 4
    rgb = []
    for row in pix2:
        line = []
        for v in row:
            c = (245, 247, 250) if v else (16, 20, 26)
            line.extend([c] * scale)
        for _ in range(scale):
            rgb.append(line)
    write_png(out, rgb)
    print('wrote', out, f'{len(rgb[0])}x{len(rgb)}')
    # text dump (unrotated, controller RAM view)
    for y in range(0, H, 2):
        page, bit = y // 8, y % 8
        print(''.join('#' if ((lcd.ram[page][x] >> bit) & 1) else '.' for x in range(W)))


if __name__ == '__main__':
    main()
