"""xmc.py -- AT32 XMC external-memory-controller display model for the X3-C.

Hardware facts (from disassembly of the LCD driver + Lead's analysis):

* Display controller sits on an STM32/AT32 **XMC/FSMC NE4 8-bit window at
  0x6C000000**.
* Command write  (firmware 0x08035BC4): PC4 (GPIOC ODR bit4) low, then
  byte **0xF9** to 0x6C000000, short delay, then the **command byte** to
  0x6C000000.
* Data write     (firmware 0x08035BE8): byte to **0x6C000001**.
* The caller pulses PC4 high after each command/data pair (latch / strobe).
* Init sequence seen at 0x08035A64: FE 20, F4 5A, F5 59, FE 90, AA 00,
  4E 1F, FE 00, 3A 55 (COLMOD=16bpp), 35 00, 53 20, 51 FF, 63 FF, 11, 14 ...

This module models the window plus the most common parallel-LCD controller
registers so the pixel stream can be captured and rendered even before the
exact controller IC is identified:

* 0x2A CASET, 0x2B RASET : column/row address windows
* 0x36 MADCTL            : memory access control (orientation)
* 0x3A COLMOD            : pixel format (0x55 = 16bpp RGB565)
* 0x2C RAMWR / 0x3C RAMWRC: pixel write stream

Every command and its data bytes are logged (with virtual timestamps) to
``logs/lcd_<ts>.txt`` so the controller can be identified, and the pixel stream
is stored so a PNG can be rendered.
"""
from __future__ import annotations

import os

B = 0x6C000000
WINDOW = 0x40000


class XmcDisplay:
    """Duck-typed peripheral registered in Machine.periphs."""

    name = "XMC_LCD"
    base = B
    size = WINDOW

    def __init__(self, machine, base=B, size=WINDOW, width=240, height=240):
        self.m = machine
        self.base = base
        self.size = size
        self.width = width
        self.height = height
        # register state
        self.madctl = 0
        self.colmod = 0
        self.caset = (0, 0)
        self.raset = (0, 0)
        self.colrange = 0          # width of CASET window
        self.rowrange = 0          # height of RASET window
        # command log and byte counters
        self.cmds = []             # [vtime, cmd, args]
        self.cur_cmd = None
        self.cur_args = None
        self.byte_count = 0
        self.cmd_count = 0
        self.marker_0xF9 = 0
        self.data_bytes = 0
        # pixel stream
        self.pixels = bytearray()
        self.pixel_pos = 0
        self.in_ramwr = False
        self.ramwr_frames = 0
        self.ramwr_cmd = 0x2C
        self.latch_pulses = 0
        self._pending_marker = False
        self._cas_tmp = []
        self._ras_tmp = []
        self.m = machine

    # -- peripheral interface -----------------------------------------
    def contains(self, addr):
        return self.base <= addr < self.base + self.size

    def reset(self):
        self.madctl = 0
        self.colmod = 0
        self.caset = (0, 0)
        self.raset = (0, 0)
        self.in_ramwr = False
        self.pixel_pos = 0

    def read(self, offset, size):
        # Status port reads: unknown controller; return 0 (no busy flags set).
        return 0

    def write(self, offset, size, value):
        self.byte_count += 1
        if offset & 1:
            self.data_bytes += 1
            b = value & 0xFF
            if size >= 2:
                # 16-bit data write: feed both bytes, high byte first
                for sh in (8, 0):
                    self._data((value >> sh) & 0xFF)
            else:
                self._data(b)
            return
        # command port
        b = value & 0xFF
        if size >= 2:
            b = value & 0xFF
        if b == 0xF9:
            self.marker_0xF9 += 1
            self._pending_marker = True
            return
        self._cmd(b)

    # -- command decoding ---------------------------------------------
    def _cmd(self, cmd):
        self.cmd_count += 1
        self.cur_cmd = cmd
        self.cur_args = []
        self.cmds.append([self.m.vtime, cmd, self.cur_args])
        if len(self.cmds) > 50000:
            del self.cmds[:25000]
        self.in_ramwr = cmd in (0x2C, 0x3C)
        if cmd == 0x2A:
            self._cas_tmp = []
        elif cmd == 0x2B:
            self._ras_tmp = []
        elif cmd == 0x2C or cmd == 0x3C:
            self.pixels = bytearray()
            self.pixel_pos = 0
            self.ramwr_frames += 1

    def _data(self, b):
        if self.cur_args is not None and len(self.cur_args) < 512:
            self.cur_args.append(b)
        c = self.cur_cmd
        if c == 0x2A:
            self._cas_tmp.append(b)
            if len(self._cas_tmp) == 4:
                xs = (self._cas_tmp[0] << 8) | self._cas_tmp[1]
                xe = (self._cas_tmp[2] << 8) | self._cas_tmp[3]
                self.caset = (xs, xe)
                self.colrange = xe - xs + 1
                self._cas_tmp = []
        elif c == 0x2B:
            self._ras_tmp.append(b)
            if len(self._ras_tmp) == 4:
                ys = (self._ras_tmp[0] << 8) | self._ras_tmp[1]
                ye = (self._ras_tmp[2] << 8) | self._ras_tmp[3]
                self.raset = (ys, ye)
                self.rowrange = ye - ys + 1
                self._ras_tmp = []
        elif c == 0x36:
            self.madctl = b
        elif c == 0x3A:
            self.colmod = b
        elif self.in_ramwr:
            if self.pixel_pos < (16 * 1024 * 1024):
                self.pixels.append(b)
                self.pixel_pos += 1

    # -- GPIO latch tracking ------------------------------------------
    def attach_latch_gpio(self):
        """Count PC4 transitions (the driver pulses PC4 after each pair)."""
        gpio = self.m.gpio.get("GPIOC") if hasattr(self.m, "gpio") else None
        if gpio is None:
            return False

        def listener(pin, level):
            if pin == 4:
                self.latch_pulses += 1
        gpio.add_listener(listener)
        return True

    # -- reporting / rendering ----------------------------------------
    def guessed_geometry(self):
        if self.colrange and self.rowrange:
            return self.colrange, self.rowrange
        if self.caset != (0, 0) or self.raset != (0, 0):
            return max(self.colrange, self.width), max(self.rowrange, self.height)
        return self.width, self.height

    def command_summary(self, n=40):
        from collections import Counter
        cnt = Counter(c[1] for c in self.cmds)
        lines = ["XMC LCD: %d byte writes, %d commands, %d distinct, "
                 "0xF9 markers=%d, data=%d bytes, RAMWR frames=%d"
                 % (self.byte_count, self.cmd_count, len(cnt),
                    self.marker_0xF9, self.data_bytes, self.ramwr_frames)]
        lines.append("  MADCTL=%#04x COLMOD=%#04x CASET=%s RASET=%s "
                     "window=%dx%d" % (self.madctl, self.colmod,
                                       self.caset, self.raset,
                                       *self.guessed_geometry()))
        for cmd, ncmd in cnt.most_common(n):
            args = []
            for c in self.cmds:
                if c[1] == cmd and c[2]:
                    args = c[2][:16]
                    break
            lines.append("  cmd %02X x%-5d data: %s"
                         % (cmd, ncmd, " ".join("%02X" % a for a in args)))
        return "\n".join(lines)

    def last_commands(self, n=40):
        out = []
        for (t, cmd, args) in self.cmds[-n:]:
            out.append("  t=%-10d cmd %02X  %s"
                       % (t, cmd, " ".join("%02X" % a for a in args[:24])))
        return "\n".join(out)

    def save_log(self, path):
        os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
        with open(path, "w", encoding="utf-8") as f:
            f.write("# X3-C XMC LCD command/data log\n")
            f.write("# MADCTL=%02X COLMOD=%02X CASET=%s RASET=%s\n"
                    % (self.madctl, self.colmod, self.caset, self.raset))
            for (t, cmd, args) in self.cmds:
                f.write("%d\t%02X\t%s\n"
                        % (t, cmd, " ".join("%02X" % a for a in args[:64])))
        return path

    def render_png(self, path, width=None, height=None, big_endian=True):
        import display
        w, h = self.guessed_geometry()
        if width:
            w = width
        if height:
            h = height
        data = bytes(self.pixels)
        rows = []
        need = w * h * 2
        if len(data) < need:
            data = data + bytes(need - len(data))
        for y in range(h):
            row = bytearray(w * 3)
            for x in range(w):
                o = (y * w + x) * 2
                if big_endian:
                    v = (data[o] << 8) | data[o + 1]
                else:
                    v = data[o] | (data[o + 1] << 8)
                r, g, b = display.rgb565_to_rgb(v)
                r, g, b = display.enhance(r, g, b)
                row[3 * x:3 * x + 3] = bytes((r, g, b))
            rows.append(bytes(row))
        return display.write_png(path, w, h, rows)

    def pixel_histogram(self):
        from collections import Counter
        c = Counter()
        d = self.pixels
        for i in range(0, len(d) - 1, 2):
            c[(d[i] << 8) | d[i + 1]] += 1
        return c
