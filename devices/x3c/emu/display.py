"""display.py -- virtual display model + framebuffer rendering for the X3-C emulator.

Two display paths are supported:

1. **RAM framebuffer**: the firmware owns a buffer in SRAM and the model knows
   its address, geometry and pixel format.  ``FramebufferModel`` renders it to
   a PNG (pure-python PNG writer: no Pillow needed in the embedded interpreter).
2. **SPI LCD controller stub**: for a display driven over SPI, ``SpiLcdModel``
   sniffs the SPI byte stream, tracks command/data bytes (and an optional
   D/C GPIO pin), and can build a pixel buffer from a memory-write (0x2C-ish)
   stream once the real controller protocol is known.

There is also a *framebuffer candidate scanner* used before the display
controller is identified: it scans SRAM for blocks that look like a
240x240/320x240 framebuffer and renders several interpretations to PNG so the
UI can be inspected.

No third-party dependencies are required; if tkinter is available a live
window can be opened from the debugger (offscreen render works regardless).
"""
from __future__ import annotations

import os
import struct
import zlib


# --------------------------------------------------------------------------
# Minimal PNG writer (RGB24)
# --------------------------------------------------------------------------
def write_png(path, width, height, rows):
    """rows: sequence of bytes, each of length width*3 (RGB)."""
    raw = bytearray()
    for row in rows:
        raw.append(0)                      # filter type 0
        raw += row

    def chunk(tag, data):
        return (struct.pack(">I", len(data)) + tag + data +
                struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF))

    png = b"\x89PNG\r\n\x1a\n"
    png += chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(bytes(raw), 6))
    png += chunk(b"IEND", b"")
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "wb") as f:
        f.write(png)
    return path


def write_png_gray(path, width, height, rows):
    """rows: bytes of length width (8-bit grayscale)."""
    raw = bytearray()
    for row in rows:
        raw.append(0)
        raw += row

    def chunk(tag, data):
        return (struct.pack(">I", len(data)) + tag + data +
                struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF))

    png = b"\x89PNG\r\n\x1a\n"
    png += chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 0, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(bytes(raw), 6))
    png += chunk(b"IEND", b"")
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, "wb") as f:
        f.write(png)
    return path


# --------------------------------------------------------------------------
# Pixel conversions
# --------------------------------------------------------------------------
def rgb565_to_rgb(v, swap=False):
    if swap:
        v = ((v & 0xFF) << 8) | (v >> 8)
    r = ((v >> 11) & 0x1F) << 3
    g = ((v >> 5) & 0x3F) << 2
    b = (v & 0x1F) << 3
    return r, g, b


def enhance(r, g, b):
    """Brighten the typical dark RGB565 rendering a little."""
    return (min(255, r + (r >> 2)),
            min(255, g + (g >> 2)),
            min(255, b + (b >> 2)))


# --------------------------------------------------------------------------
# RAM framebuffer model
# --------------------------------------------------------------------------
FORMATS = ("rgb565", "bgr565", "rgb565be", "8bpp", "1bpp", "4bpp")


class FramebufferModel:
    """A framebuffer living at ``base`` in the emulated address space."""

    def __init__(self, base, width, height, fmt="rgb565", stride=None,
                 name="fb", endian="little", invert=False, palette=None):
        self.base = base
        self.width = width
        self.height = height
        self.fmt = fmt
        self.stride = stride or self._default_stride()
        self.name = name
        self.endian = endian
        self.invert = invert
        self.palette = palette

    def _default_stride(self):
        if self.fmt in ("rgb565", "bgr565", "rgb565be"):
            return self.width * 2
        if self.fmt == "8bpp":
            return self.width
        if self.fmt == "4bpp":
            return (self.width + 1) // 2
        return (self.width + 7) // 8

    # -- rendering -----------------------------------------------------
    def render(self, machine, brighten=True):
        """Return list of RGB row bytes."""
        h, w, stride = self.height, self.width, self.stride
        data = machine.read_bytes(self.base, stride * h)
        rows = []
        if self.fmt in ("rgb565", "bgr565", "rgb565be"):
            for y in range(h):
                off = y * stride
                row = bytearray(w * 3)
                for x in range(w):
                    o = off + x * 2
                    if o + 1 >= len(data):
                        break
                    if self.fmt == "rgb565be":
                        v = (data[o] << 8) | data[o + 1]
                    else:
                        v = data[o] | (data[o + 1] << 8)
                    r, g, b = rgb565_to_rgb(v, self.fmt == "bgr565")
                    if brighten:
                        r, g, b = enhance(r, g, b)
                    row[3 * x] = r
                    row[3 * x + 1] = g
                    row[3 * x + 2] = b
                rows.append(bytes(row))
        elif self.fmt == "8bpp":
            for y in range(h):
                off = y * stride
                row = bytearray(w * 3)
                for x in range(w):
                    v = data[off + x] if off + x < len(data) else 0
                    if self.palette:
                        c = self.palette[v % len(self.palette)]
                    else:
                        c = (v, v, v)
                    row[3 * x:3 * x + 3] = bytes(c)
                rows.append(bytes(row))
        elif self.fmt == "4bpp":
            for y in range(h):
                off = y * stride
                row = bytearray(w * 3)
                for x in range(w):
                    byte = data[off + x // 2] if off + x // 2 < len(data) else 0
                    v = (byte >> 4) if (x & 1) == 0 else (byte & 0xF)
                    g = v * 17
                    row[3 * x:3 * x + 3] = bytes((g, g, g))
                rows.append(bytes(row))
        else:  # 1bpp
            for y in range(h):
                off = y * stride
                row = bytearray(w * 3)
                for x in range(w):
                    byte = data[off + x // 8] if off + x // 8 < len(data) else 0
                    bit = (byte >> (7 - (x & 7))) & 1
                    if self.invert:
                        bit ^= 1
                    g = 255 if bit else 0
                    row[3 * x:3 * x + 3] = bytes((g, g, g))
                rows.append(bytes(row))
        return rows

    def ascii_art(self, machine, max_w=64):
        """Coarse ASCII preview (useful in logs)."""
        rows = self.render(machine, brighten=False)
        out = []
        step = max(1, self.width // max_w)
        for y in range(0, self.height, step * 2):
            line = []
            for x in range(0, self.width, step):
                px = rows[y][3 * x:3 * x + 3] if 3 * x + 2 < len(rows[y]) else (0, 0, 0)
                lum = (px[0] * 299 + px[1] * 587 + px[2] * 114) // 1000
                line.append(" .:-=+*#%@"[min(10, lum * 11 // 256)])
            out.append("".join(line))
        return "\n".join(out)

    def save(self, machine, path, brighten=True):
        rows = self.render(machine, brighten=brighten)
        return write_png(path, self.width, self.height, rows)


# --------------------------------------------------------------------------
# SPI LCD controller sniffing model
# --------------------------------------------------------------------------
class SpiLcdModel:
    """Generic SPI display-controller stub.

    Records every command byte (and following data bytes) so the real command
    set can be identified from a boot log.  Tracks a "write memory" command
    stream to an internal pixel buffer for controllers whose RAMWR opcode is
    known (default: ILI9341 0x2C).

    Usage::

        lcd = SpiLcdModel(width=240, height=240, ctrl="ili9341")
        machine.spi["SPI1"].tx_handler = lcd.on_tx_byte
        lcd.attach_dc(gpio, pin)      # optional D/C line
        ...
        lcd.render_png(machine, "screen.png")
    """

    def __init__(self, width=240, height=240, ctrl="ili9341"):
        self.width = width
        self.height = height
        self.ctrl = ctrl
        self.cmds = []                  # (vtime, cmd, [data bytes])
        self.cur_cmd = None
        self.cur_args = None
        self.dc = 1                     # 1=data,0=command when no D/C wired
        self.pixels = bytearray(width * height * 2)
        self.pixel_pos = 0
        self.in_ramwr = False
        self.cs = 1
        self.byte_count = 0
        self.ramwr_cmd = 0x2C if ctrl == "ili9341" else 0x2C
        # ILI9341 registers we care about
        self.madctl = 0
        self.colmod = 0x05
        self.win = (0, 0, width - 1, height - 1)

    # -- SPI side ------------------------------------------------------
    def on_cs(self, level):
        self.cs = level

    def on_tx_byte(self, b):
        self.byte_count += 1
        if self.dc == 0:
            self.cur_cmd = b
            self.cur_args = []
            self.cmds.append([self.byte_count, b, []])
            if len(self.cmds) > 20000:
                del self.cmds[:10000]
            self._handle_cmd(b)
            return 0
        # data byte
        if self.cmds and self.cur_args is not None:
            if len(self.cur_args) < 256:
                self.cur_args.append(b)
                self.cmds[-1][2].append(b)
        self._handle_data(b)
        return 0

    def attach_dc(self, gpio, pin, active_high=True):
        """Wire a GPIO pin as the D/C line (listener on GPIO output)."""
        def listener(p, level):
            if p == pin:
                self.dc = (1 if level else 0) if active_high else (0 if level else 1)
        gpio.add_listener(listener)

    # -- controller command decoding ----------------------------------
    def _handle_cmd(self, cmd):
        self.in_ramwr = (cmd == self.ramwr_cmd)
        if cmd == 0x2A:            # column address set
            self._win_tmp = []
        elif cmd == 0x2B:          # page address set
            self._win_tmp = []
        elif cmd == 0x36:          # MADCTL
            self._mad_tmp = []
        elif cmd == 0x3A:          # COLMOD
            self._col_tmp = []
        if self.in_ramwr:
            self.pixel_pos = 0

    def _handle_data(self, b):
        if self.cur_cmd == 0x2A:
            self._win_tmp = getattr(self, "_win_tmp", []) + [b]
            if len(self._win_tmp) == 4:
                xs = (self._win_tmp[0] << 8) | self._win_tmp[1]
                xe = (self._win_tmp[2] << 8) | self._win_tmp[3]
                self.win = (xs, self.win[1], xe, self.win[3])
                self._win_tmp = []
        elif self.cur_cmd == 0x2B:
            self._win_tmp = getattr(self, "_win_tmp", []) + [b]
            if len(self._win_tmp) == 4:
                ys = (self._win_tmp[0] << 8) | self._win_tmp[1]
                ye = (self._win_tmp[2] << 8) | self._win_tmp[3]
                self.win = (self.win[0], ys, self.win[2], ye)
                self._win_tmp = []
        elif self.cur_cmd == 0x36:
            self.madctl = b
        elif self.cur_cmd == 0x3A:
            self.colmod = b
        elif self.in_ramwr:
            if self.pixel_pos + 1 < len(self.pixels):
                self.pixels[self.pixel_pos] = b
                self.pixel_pos += 1
            # in "high byte first" mode bytes pair up; keep raw stream

    # -- output --------------------------------------------------------
    def render_png(self, path, width=None, height=None):
        w = width or self.width
        h = height or self.height
        rows = []
        for y in range(h):
            row = bytearray(w * 3)
            for x in range(w):
                o = (y * w + x) * 2
                if o + 1 >= len(self.pixels):
                    break
                v = self.pixels[o] | (self.pixels[o + 1] << 8)
                r, g, b = rgb565_to_rgb(v)
                r, g, b = enhance(r, g, b)
                row[3 * x:3 * x + 3] = bytes((r, g, b))
            rows.append(bytes(row))
        return write_png(path, w, h, rows)

    def command_summary(self, n=40):
        from collections import Counter
        c = Counter(cmds[1] for cmds in self.cmds)
        lines = ["SPI LCD %s: %d bytes, %d commands, %d distinct"
                 % (self.ctrl, self.byte_count, len(self.cmds), len(c))]
        for cmd, cnt in c.most_common(n):
            args = []
            for cmds in self.cmds:
                if cmds[1] == cmd and cmds[2]:
                    args = cmds[2][:8]
                    break
            lines.append("  cmd %02X x%-5d data: %s"
                         % (cmd, cnt, " ".join("%02X" % a for a in args)))
        return "\n".join(lines)

    def last_commands(self, n=30):
        out = []
        for (t, cmd, args) in self.cmds[-n:]:
            out.append("  #%-6d cmd %02X  %s"
                       % (t, cmd, " ".join("%02X" % a for a in args[:16])))
        return "\n".join(out)


class ParallelLcd(SpiLcdModel):
    """LCD controller attached to the FMC/FSMC external memory bus.

    The X3-C firmware writes its LCD through FMC at 0x6C000000 (observed:
    unmapped write pc=0x08035BD8 addr=0x6C000000 at t=140M).  On such a bus the
    RS (command/data) line is usually wired to an address line, so a *command*
    is a write at the base address and *data* is a write at base+2 (16-bit bus)
    or base+1 (8-bit bus).  This class accepts those writes and reuses the
    generic SPI LCD command decoder.
    """

    def __init__(self, width=240, height=240, ctrl="unknown", base=0x6C000000):
        super().__init__(width=width, height=height, ctrl=ctrl)
        self.base = base
        self.alt_offsets = set()

    def feed(self, offset, value, size=2):
        """Called by the machine on a write to the FMC LCD window."""
        if offset == 0:
            self.dc = 0
            self.on_tx_byte(value & 0xFF)
        else:
            if offset not in (1, 2):
                self.alt_offsets.add(offset)
            self.dc = 1
            if size >= 2:
                # keep the 16-bit value as delivered; pixel buffer is built
                # from raw bytes (high byte first for 16-bit stores)
                self.on_tx_byte((value >> 8) & 0xFF)
                self.on_tx_byte(value & 0xFF)
            else:
                self.on_tx_byte(value & 0xFF)

    def render_png_be(self, path, width=None, height=None):
        w = width or self.width
        h = height or self.height
        rows = []
        for y in range(h):
            row = bytearray(w * 3)
            for x in range(w):
                o = (y * w + x) * 2
                if o + 1 >= len(self.pixels):
                    break
                v = (self.pixels[o] << 8) | self.pixels[o + 1]
                r, g, b = rgb565_to_rgb(v)
                r, g, b = enhance(r, g, b)
                row[3 * x:3 * x + 3] = bytes((r, g, b))
            rows.append(bytes(row))
        return write_png(path, w, h, rows)


# --------------------------------------------------------------------------
# Framebuffer candidate scanner
# --------------------------------------------------------------------------
CANDIDATE_GEOMETRIES = [
    (240, 240, "rgb565"),
    (320, 240, "rgb565"),
    (240, 320, "rgb565"),
    (128, 128, "rgb565"),
    (160, 128, "rgb565"),
    (240, 240, "rgb565be"),
    (320, 240, "rgb565be"),
    (240, 240, "1bpp"),
    (320, 240, "1bpp"),
    (128, 64, "1bpp"),
    (240, 240, "8bpp"),
    (256, 64, "1bpp"),
]


def _stats(data):
    if not data:
        return 0.0, 0.0
    nz = sum(1 for b in data if b)
    return nz / len(data), len(set(data))


def _smoothness_16(data, w):
    """Mean absolute difference between horizontally adjacent 16-bit pixels."""
    if len(data) < 4:
        return 999
    total = 0
    cnt = 0
    for i in range(0, min(len(data) - 4, 2 * w * 8), 2):
        a = data[i] | (data[i + 1] << 8)
        b = data[i + 2] | (data[i + 3] << 8)
        total += abs(((a >> 5) & 63) - ((b >> 5) & 63))
        cnt += 1
    return total / cnt if cnt else 999


def scan_candidates(machine, ram_base=0x20000000, ram_size=0x80000,
                    align=0x10, max_candidates=8):
    """Scan SRAM pages for plausible framebuffer blocks.

    Returns a list of dicts sorted by how "image-like" they look.
    """
    try:
        raw = bytes(machine.uc.mem_read(ram_base, ram_size))
    except Exception:
        return []
    found = []
    step = 0x400
    for off in range(0, ram_size, step):
        for (w, h, fmt) in CANDIDATE_GEOMETRIES:
            bpp = 2 if fmt.startswith("rgb565") else (1 if fmt == "8bpp" else 0.125)
            nbytes = int(w * h * bpp)
            if nbytes > ram_size - off:
                continue
            chunk = raw[off:off + nbytes]
            nz, distinct = _stats(chunk)
            if nz < 0.25 or distinct < 24:
                continue
            if fmt.startswith("rgb565"):
                smooth = _smoothness_16(chunk, w)
                score = nz * 2.0 + min(distinct, 200) / 200.0 - min(smooth, 40) / 20.0
            else:
                score = nz + min(distinct, 100) / 100.0
            found.append(dict(addr=ram_base + off, w=w, h=h, fmt=fmt,
                              nbytes=nbytes, nz=nz, distinct=distinct,
                              score=score))
    # dedupe near-identical addresses, keep best score
    found.sort(key=lambda d: -d["score"])
    out = []
    seen = set()
    for c in found:
        key = (c["addr"] >> 10, c["fmt"])
        if key in seen:
            continue
        seen.add(key)
        out.append(c)
        if len(out) >= max_candidates:
            break
    return out


def dump_candidates(machine, outdir, ram_base=0x20000000, ram_size=0x80000,
                    log=print):
    """Render plausible framebuffers from SRAM to PNGs, return paths."""
    os.makedirs(outdir, exist_ok=True)
    paths = []
    for c in scan_candidates(machine, ram_base, ram_size):
        fb = FramebufferModel(c["addr"], c["w"], c["h"], c["fmt"],
                              name="cand_%08x" % c["addr"])
        path = os.path.join(outdir, "fb_%08x_%dx%d_%s.png"
                            % (c["addr"], c["w"], c["h"], c["fmt"]))
        try:
            fb.save(machine, path)
        except Exception as e:
            log("candidate %#x render failed: %r" % (c["addr"], e))
            continue
        c["png"] = path
        paths.append(path)
        log("candidate fb @ %#010x %dx%d %s (nz=%.2f distinct=%d score=%.2f) "
            "-> %s" % (c["addr"], c["w"], c["h"], c["fmt"], c["nz"],
                       c["distinct"], c["score"], os.path.basename(path)))
    return paths


# --------------------------------------------------------------------------
# Optional live window (only if tkinter exists)
# --------------------------------------------------------------------------
def tk_available():
    try:
        import tkinter  # noqa: F401
        return True
    except Exception:
        return False


def show_window(fb, machine, title="X3-C display"):
    """Open a live-updating Tkinter window; returns viewer handle or None."""
    if not tk_available():
        return None
    import tkinter as tk

    root = tk.Tk()
    root.title(title)
    canvas = tk.Canvas(root, width=fb.width, height=fb.height)
    canvas.pack()

    def refresh():
        rows = fb.render(machine)
        for y, row in enumerate(rows):
            for x in range(0, fb.width):
                r, g, b = row[3 * x], row[3 * x + 1], row[3 * x + 2]
                canvas.create_rectangle(x, y, x + 1, y + 1,
                                        fill="#%02x%02x%02x" % (r, g, b),
                                        outline="")
        root.after(500, refresh)

    refresh()
    return root
