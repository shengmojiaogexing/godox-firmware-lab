"""X3-C firmware simulator demo: boot the real firmware and reconstruct the LCD output.

What it does
------------
1. Loads `devices/x3c/firmware/X3_C_v1.38_app.bin` into a Unicorn Cortex-M4 machine
   (`devices/x3c/emu/machine.py`) at its real link base 0x08004800.
2. Applies the minimal "real device" accommodations discovered during RE:
     * option-byte page 0x1FFFC000 is already provisioned (skip one-time provisioning)
     * ADC block @0x40012000 answers with a valid status/data (battery sensor)
     * the calibrated nop-sled delay functions are short-circuited (fast-forward)
3. Maps the AT32 XMC external memory window 0x60000000-0x70000000 and decodes the
   LCD 8080 bus at 0x6C000000 (A0=0 command, A0=1 data) with its 0xF9 command preamble.
4. Reconstructs a 368x448 RGB565 panel image and writes PNG snapshots + a bus log.

Run:
    tools/python/python.exe devices/x3c/emu/demo_x3.py --max-insn 8000000
Outputs (in devices/x3c/emu/lcd_out/):
    lcd_<insn>.png   screen snapshots
    lcd_bus.log      decoded command/data stream
"""
import argparse
import os
import struct
import sys
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

from machine import Machine  # noqa: E402

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")
OUT = os.path.join(HERE, "lcd_out")


# --------------------------------------------------------------------------- panel
class LcdBus:
    """NV3007/ST77xx-like 8080 panel model fed by the firmware's byte stream."""

    def __init__(self, w=368, h=448):
        self.w, self.h = w, h
        self.fb = bytearray(w * h * 2)
        self.cmds = []
        self.cmd = None
        self.args = []
        self.colmod = 0x55
        self.madctl = 0
        self.xs = self.xe = self.ys = self.ye = 0
        self.px = self.py = 0
        self.ram = False
        self.pixbuf = bytearray()
        self.n_pixels = 0
        self.n_data = 0
        self.log = []

    def write_addr0(self, v):
        if v == 0xF9:                      # command preamble of the bus bridge
            return
        self.cmd = v
        self.args = []
        self.cmds.append(v)
        self.log.append("CMD %02X" % v)
        if v == 0x2C:
            self.ram = True
            self.px, self.py = self.xs, self.ys
        elif v in (0x28, 0x2A, 0x2B):
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
                    self.px, self.py = self.xs, self.py + 1
            return
        self.args.append(v)
        a = self.args
        if self.cmd == 0x2A and len(a) == 4:
            self.xs, self.xe = (a[0] << 8) | a[1], (a[2] << 8) | a[3]
            self.log.append("  window x=%d..%d" % (self.xs, self.xe))
        elif self.cmd == 0x2B and len(a) == 4:
            self.ys, self.ye = (a[0] << 8) | a[1], (a[2] << 8) | a[3]
            self.log.append("  window y=%d..%d" % (self.ys, self.ye))
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
        _png_write(path, self.w, self.h, rgb)
        return path


def _png_write(path, w, h, rgb):
    raw = bytearray()
    for y in range(h):
        raw.append(0)
        raw += rgb[y * w * 3:(y + 1) * w * 3]

    def chunk(tag, data):
        return (struct.pack(">I", len(data)) + tag + data
                + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF))

    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n")
        f.write(chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)))
        f.write(chunk(b"IDAT", zlib.compress(bytes(raw), 6)))
        f.write(chunk(b"IEND", b""))


# --------------------------------------------------------------------------- patches
def patch_option_bytes(m):
    """The real device is already provisioned: 0x1FFFC002=0x000F, 0x1FFFC010=0x0002.

    Without this the reset hook 0x08020A20 runs the one-time option-byte programming
    path and issues SYSRESETREQ (we emulate a previously-booted device).
    """
    try:
        m.uc.mem_map(0x1FFFC000, 0x1000)
    except Exception:
        pass
    m.uc.mem_write(0x1FFFC000, b"\x00" * 0x1000)
    m.uc.mem_write(0x1FFFC002, struct.pack("<H", 0x000F))
    m.uc.mem_write(0x1FFFC010, struct.pack("<H", 0x0002))


class AdcBlock:
    """AT32 ADC sensor block: ADRDY in STS, busy bits self-clear, data available."""

    def __init__(self, raw=2600):
        self.regs = {}
        self.raw = raw

    def read(self, offset, size):
        off = offset & ~3
        if off == 0x00:
            return 0x42
        if off == 0x08:
            return self.regs.get(off, 0) & ~0x0C
        if off in (0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C):
            return self.raw
        return self.regs.get(off, 0)

    def write(self, offset, size, value):
        self.regs[offset & ~3] = value


def patch_adc(m, raw):
    p = m.find_peripheral(0x40012000)
    dev = AdcBlock(raw)
    if p is None:
        m.add_peripheral(dev)
    else:
        p.read = dev.read
        p.write = dev.write
        p.name = "ADC"


def patch_delays(m):
    """Short-circuit the two calibrated nop-sled delays (0x0801C118 us, 0x0801C0FE ms)."""
    m.uc.mem_write(0x0801C118, b"\x70\x47")
    m.uc.mem_write(0x0801C0FE, b"\x70\x47")


def map_xmc(m):
    try:
        m.uc.mem_map(0x60000000, 0x1000000)
    except Exception:
        pass


# --------------------------------------------------------------------------- main
def main():
    ap = argparse.ArgumentParser(description="X3-C firmware boot + LCD reconstruction demo")
    ap.add_argument("--img", default=IMAGE)
    ap.add_argument("--max-insn", type=int, default=8000000)
    ap.add_argument("--chunk", type=int, default=1000000)
    ap.add_argument("--adc-raw", type=int, default=2600, help="battery ADC raw value")
    ap.add_argument("--snapshot-every", type=int, default=2000000)
    args = ap.parse_args()

    m = Machine(image=args.img, log_fn=lambda s: None, trace=True)
    patch_option_bytes(m)
    patch_adc(m, args.adc_raw)
    patch_delays(m)
    map_xmc(m)

    from unicorn import UC_HOOK_MEM_WRITE
    bus = LcdBus()
    os.makedirs(OUT, exist_ok=True)

    def wr(uc, access, address, size, value, user):
        if address == 0x6C000000:
            bus.write_addr0(value & 0xFF)
        elif address == 0x6C000001:
            bus.write_addr1(value & 0xFF)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=0x6C000000, end=0x6C000002)
    sp, pc = m.boot()
    print("reset: SP=0x%08x PC=0x%08x (app base 0x08004800)" % (sp, pc))

    done = 0
    last_pix = 0
    while done < args.max_insn:
        m.run(min(args.chunk, args.max_insn - done))
        done = m.insn_count
        if bus.n_pixels != last_pix:
            print("[%9d insn] PC=0x%08x  LCD pixels=%d  cmds=%d  window x=%d..%d y=%d..%d" % (
                done, m.reg("PC"), bus.n_pixels, len(bus.cmds),
                bus.xs, bus.xe, bus.ys, bus.ye))
            if done % args.snapshot_every < args.chunk or args.snapshot_every <= 0:
                print("   snapshot:", bus.snapshot("%09d" % done))
            last_pix = bus.n_pixels
    final = bus.snapshot("final")
    print("\nLCD commands:", " ".join("%02x" % c for c in bus.cmds[:80]))
    print("LCD data bytes:", bus.n_data, " pixels:", bus.n_pixels)
    print("final screenshot:", final)
    with open(os.path.join(OUT, "lcd_bus.log"), "w", encoding="utf-8") as f:
        f.write("\n".join(bus.log[:20000]))
    print("bus log:", os.path.join(OUT, "lcd_bus.log"))
    return bus


if __name__ == "__main__":
    main()
