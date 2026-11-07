# -*- coding: utf-8 -*-
"""lcd_capture.py -- composite the X3-C XMC/8080 LCD byte stream into a PNG.

The firmware never keeps a framebuffer in SRAM for the panel: it streams
8080 commands/data through the AT32 XMC window at 0x6C000000 (A0=0 -> command,
A0=1 -> data, 0xF9 command preamble).  This class hooks every write in that
window, tracks CASET/RASET/COLMOD/MADCTL and composites the RAMWR pixel stream
into a 368x448 RGB565 framebuffer, so a snapshot always shows the real screen
content (including partial redraws, which a "last RAMWR buffer" model loses).

Adapted from devices/x3c/emu/demo_x3.py's LcdBus (same bus decoding), extended with
dirty tracking and snapshot bookkeeping for the TMR7 UI runs.
"""
from __future__ import annotations

import os
import struct
import zlib


def png_write(path, w, h, rgb):
    raw = bytearray()
    for y in range(h):
        raw.append(0)
        raw += rgb[y * w * 3:(y + 1) * w * 3]

    def chunk(tag, data):
        return (struct.pack(">I", len(data)) + tag + data
                + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF))

    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n")
        f.write(chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)))
        f.write(chunk(b"IDAT", zlib.compress(bytes(raw), 6)))
        f.write(chunk(b"IEND", b""))


class LcdCapture:
    """Full-screen compositor for the 368x448 RGB565 X3-C panel."""

    BASE = 0x6C000000

    def __init__(self, width=368, height=448, brighten=False):
        self.w = width
        self.h = height
        self.fb = bytearray(width * height * 2)
        self.brighten = brighten
        self.cmd = None
        self.args = []
        self.colmod = 0x55
        self.madctl = 0
        self.xs = self.xe = 0
        self.ys = self.ye = 0
        self.px = self.py = 0
        self.ram = False
        self.pixbuf = bytearray()
        self.n_pixels = 0
        self.n_data = 0
        self.n_cmds = 0
        self.cmds = []
        # dirty tracking
        self.dirty = False
        self.last_write_vtime = 0
        self.frames = []            # (vtime, n_pixels, n_cmds) on 2C start

    # -- bus side -----------------------------------------------------
    def write_cmd(self, value):
        v = value & 0xFF
        if v == 0xF9:               # command preamble of the bus bridge
            return
        self.cmd = v
        self.args = []
        self.n_cmds += 1
        if len(self.cmds) < 200000:
            self.cmds.append(v)
        if v == 0x2C or v == 0x3C:
            self.ram = True
            self.px, self.py = self.xs, self.ys
            self.frames.append((self.last_write_vtime, self.n_pixels,
                                self.n_cmds))
        elif v in (0x28, 0x2A, 0x2B):
            self.ram = False

    def write_data(self, value):
        v = value & 0xFF
        self.n_data += 1
        self.dirty = True
        if self.ram:
            self.pixbuf.append(v)
            if len(self.pixbuf) >= 2:
                hi, lo = self.pixbuf[0], self.pixbuf[1]
                del self.pixbuf[:2]
                if self.px <= self.xe and self.py <= self.ye:
                    self._put(self.px, self.py, (hi << 8) | lo)
                    self.n_pixels += 1
                self.px += 1
                if self.px > self.xe:
                    self.px, self.py = self.xs, self.py + 1
            return
        self.args.append(v)
        a = self.args
        if self.cmd == 0x2A and len(a) == 4:
            self.xs = (a[0] << 8) | a[1]
            self.xe = (a[2] << 8) | a[3]
        elif self.cmd == 0x2B and len(a) == 4:
            self.ys = (a[0] << 8) | a[1]
            self.ye = (a[2] << 8) | a[3]
        elif self.cmd == 0x3A and len(a) == 1:
            self.colmod = a[0]
        elif self.cmd == 0x36 and len(a) == 1:
            self.madctl = a[0]

    def _put(self, x, y, v):
        if self.madctl & 1:
            x = self.w - 1 - x
        if (self.madctl >> 5) & 1:
            y = self.h - 1 - y
        if 0 <= x < self.w and 0 <= y < self.h:
            o = (y * self.w + x) * 2
            self.fb[o] = v & 0xFF
            self.fb[o + 1] = v >> 8

    # -- machine hook -------------------------------------------------
    def install(self, machine):
        from unicorn import UC_HOOK_MEM_WRITE
        base = self.BASE

        def hook(uc, access, address, size, value, user):
            self.last_write_vtime = getattr(machine, "vtime", 0)
            if address == base:
                self.write_cmd(value)
            elif address == base + 1:
                self.write_data(value)
            elif address == base + 2:       # 16-bit data port variant
                self.write_data((value >> 8) & 0xFF)
                self.write_data(value & 0xFF)

        machine.uc.hook_add(UC_HOOK_MEM_WRITE, hook,
                            begin=base, end=base + 3)
        machine.lcd_capture = self
        return self

    # -- rendering ----------------------------------------------------
    def rgb_rows(self):
        rows = []
        w, h = self.w, self.h
        fb = self.fb
        for y in range(h):
            row = bytearray(w * 3)
            o = y * w * 2
            for x in range(w):
                v = fb[o] | (fb[o + 1] << 8)
                o += 2
                r = ((v >> 11) & 0x1F) << 3
                g = ((v >> 5) & 0x3F) << 2
                b = (v & 0x1F) << 3
                if self.brighten:
                    r = min(255, r + (r >> 2))
                    g = min(255, g + (g >> 2))
                    b = min(255, b + (b >> 2))
                row[3 * x] = r
                row[3 * x + 1] = g
                row[3 * x + 2] = b
            rows.append(bytes(row))
        return rows

    def snapshot(self, path, tag=None):
        png_write(path, self.w, self.h, b"".join(self.rgb_rows()))
        if tag is not None:
            self.frames.append((self.last_write_vtime, self.n_pixels, tag))
        self.dirty = False
        return path

    def nonblack_ratio(self):
        nz = 0
        fb = self.fb
        for i in range(0, len(fb), 2):
            if fb[i] or fb[i + 1]:
                nz += 1
        return nz / (self.w * self.h)

    def stats(self):
        return dict(pixels=self.n_pixels, data=self.n_data, cmds=self.n_cmds,
                    madctl=self.madctl, colmod=self.colmod,
                    window=(self.xs, self.xe, self.ys, self.ye),
                    nonblack=self.nonblack_ratio())
