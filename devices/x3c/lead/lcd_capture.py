"""Decode the X3 LCD bus (XMC 8080 @0x6C000000) into PNG images.

Bus protocol (recovered from firmware):
  write 0xF9 to 0x6C000000        -> 'command follows' strobe (bridge/PC4 control)
  write <cmd> to 0x6C000000       -> LCD command
  write <byte> to 0x6C000001      -> LCD parameter / pixel byte (MSB first for RGB565)
Panel: 368 x 448 portrait (448x368 landscape), RGB565 (COLMOD=0x55).
"""
import os
import struct
import sys
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")
OUT = os.path.join(HERE, "lcd_out")

from machine import Machine  # noqa: E402
from boot6 import patch_option_bytes, patch_adc, patch_delay, map_xmc  # noqa: E402

from unicorn import UC_HOOK_MEM_WRITE  # noqa: E402


def png_write(path, w, h, rgb):
    raw = bytearray()
    for y in range(h):
        raw.append(0)
        raw += rgb[y * w * 3:(y + 1) * w * 3]

    def chunk(tag, data):
        c = struct.pack(">I", len(data)) + tag + data
        return c + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)

    sig = b"\x89PNG\r\n\x1a\n"
    ihdr = struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)
    with open(path, "wb") as f:
        f.write(sig + chunk(b"IHDR", ihdr) + chunk(b"IDAT", zlib.compress(bytes(raw), 6)) + chunk(b"IEND", b""))


class LcdBus:
    def __init__(self, w=368, h=448):
        self.w, self.h = w, h
        self.fb = bytearray(w * h * 2)
        self.frames = []
        self.cmds = []
        self.cmd = None
        self.args = []
        self.colmod = 0x55
        self.madctl = 0
        self.win = [0, 0, 0, 0]
        self.xs = self.xe = self.ys = self.ye = 0
        self.px = self.py = 0
        self.ram = False
        self.pixbuf = bytearray()
        self.n_pixels = 0
        self.n_data = 0
        self.cmd_mode = False

    # ---- bus
    def write_addr0(self, v):
        if v == 0xF9:
            self.cmd_mode = True
            return
        self.cmd = v
        self.args = []
        self.cmds.append(v)
        self.cmd_mode = False
        if v == 0x2C:
            self.ram = True
            self.px, self.py = self.xs, self.ys
            self.pixbuf = bytearray()
        elif v in (0x28, 0x29, 0x11, 0x01):
            if v == 0x28:
                self.ram = False
        elif v in (0x2A, 0x2B):
            self.ram = False

    def write_addr1(self, v):
        self.n_data += 1
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
                    self.px = self.xs
                    self.py += 1
            return
        # command parameter
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
        # apply MADCTL rotation (only common cases)
        mx, my = self.madctl & 1, (self.madctl >> 5) & 1
        if mx:
            x = self.w - 1 - x
        if my:
            y = self.h - 1 - y
        if 0 <= x < self.w and 0 <= y < self.h:
            o = (y * self.w + x) * 2
            self.fb[o] = v & 0xFF
            self.fb[o + 1] = v >> 8

    def snapshot(self, tag):
        rgb = bytearray(self.w * self.h * 3)
        for i in range(0, len(self.fb), 2):
            v = self.fb[i] | (self.fb[i + 1] << 8)
            o = (i // 2) * 3
            rgb[o] = ((v >> 11) & 0x1F) << 3
            rgb[o + 1] = ((v >> 5) & 0x3F) << 2
            rgb[o + 2] = (v & 0x1F) << 3
        os.makedirs(OUT, exist_ok=True)
        path = os.path.join(OUT, "lcd_%s.png" % tag)
        png_write(path, self.w, self.h, rgb)
        return path


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 30000000
    chunk = int(sys.argv[2]) if len(sys.argv) > 2 else 5000000
    m = Machine(image=IMAGE, log_fn=lambda s: None, trace=True)
    patch_option_bytes(m)
    patch_adc(m)
    patch_delay(m)
    map_xmc(m)
    bus = LcdBus()
    os.makedirs(OUT, exist_ok=True)
    last_save = [0]

    def wr(uc, access, address, size, value, user):
        if address == 0x6C000000:
            bus.write_addr0(value & 0xFF)
        elif address == 0x6C000001:
            bus.write_addr1(value & 0xFF)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=0x6C000000, end=0x6C000002)
    m.boot()
    for i in range(0, max_insns, chunk):
        st = m.run(chunk)
        done = bus.n_pixels
        print("[%10d] %-9s PC=0x%08x pixels=%d data=%d cmds=%d window=(%d,%d)-(%d,%d)" % (
            m.insn_count, st, m.reg("PC"), done, bus.n_data, len(bus.cmds),
            bus.xs, bus.xe, bus.ys, bus.ye))
        if bus.n_pixels > last_save[0]:
            p = bus.snapshot("%09d" % m.insn_count)
            print("   snapshot ->", p)
            last_save[0] = bus.n_pixels
        if st not in (None, "max_insn"):
            break
    print("\ncommands:", " ".join("%02x" % c for c in bus.cmds[:200]))
    print("colmod=0x%02x madctl=0x%02x total pixels=%d" % (bus.colmod, bus.madctl, bus.n_pixels))
    p = bus.snapshot("final")
    print("final ->", p)
    return bus


if __name__ == "__main__":
    main()
