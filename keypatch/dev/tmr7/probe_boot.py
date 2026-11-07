# -*- coding: utf-8 -*-
"""Boot the X3-C firmware with the stock emulator and dump every MMIO access
around the TMR7 / NVIC / RCC blocks, so the 1 ms tick timer configuration can
be reconstructed from real register writes.

Usage:
  python probe_boot.py [--max 8000000] [--fast] [--no-patch]
"""
from __future__ import annotations

import argparse
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
sys.path.insert(0, EMU)

from machine import Machine  # noqa: E402

IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")

WATCH = [
    (0x40001400, 0x400, "TMR7"),
    (0x40001000, 0x400, "TMR6"),
    (0x40000000, 0x400, "TMR2"),
    (0x40000C00, 0x400, "TMR5"),
    (0x40023800, 0x400, "RCC"),
    (0xE000E100, 0x400, "NVIC"),
    (0xE000ED00, 0x100, "SCB"),
    (0xE000E010, 0x10, "SysTick"),
]


def patch_option_bytes(m):
    try:
        m.uc.mem_write(0x1FFFC002, struct.pack("<H", 0x000F))
        m.uc.mem_write(0x1FFFC010, struct.pack("<H", 0x0002))
    except Exception:
        pass


class AdcBlock:
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


def patch_adc(m, raw=2600):
    p = m.find_peripheral(0x40012000)
    dev = AdcBlock(raw)
    if p is None:
        m.add_peripheral(dev)
    else:
        p.read = dev.read
        p.write = dev.write
        p.name = "ADC"


def patch_delays(m):
    m.uc.mem_write(0x0801C118, b"\x70\x47")
    m.uc.mem_write(0x0801C0FE, b"\x70\x47")


def watch_region(addr):
    for base, size, name in WATCH:
        if base <= addr < base + size:
            return name, addr - base
    return None, None


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=IMAGE)
    ap.add_argument("--max", type=int, default=8_000_000)
    ap.add_argument("--chunk", type=int, default=100_000)
    ap.add_argument("--fast", action="store_true")
    ap.add_argument("--no-patch", action="store_true")
    args = ap.parse_args()

    events = []
    m = Machine(image=args.img, log_fn=lambda s: None, trace=True,
                trace_max=400000)
    if not args.no_patch:
        patch_option_bytes(m)
        patch_adc(m)
        patch_delays(m)

    def pc_now():
        try:
            return m.uc.reg_read(11) & ~1   # UC_ARM_REG_PC == 11 in unicorn 2.x
        except Exception:
            return 0

    orig_rd = m._mmio_read
    orig_wr = m._mmio_write

    def rd(addr, size, offset=None):
        name, off = watch_region(addr)
        v = orig_rd(addr, size, offset)
        if name:
            events.append(("R", m.vtime, addr, size, v, pc_now(), name, off))
        return v

    def wr(addr, size, value, offset=None):
        name, off = watch_region(addr)
        if name:
            events.append(("W", m.vtime, addr, size, value, pc_now(), name, off))
        return orig_wr(addr, size, value, offset)

    m._mmio_read = rd
    m._mmio_write = wr
    m._mmio_read_cb = m._make_mmio_read()
    m._mmio_write_cb = m._make_mmio_write()
    for base, size in ((0x40000000, 0x1000000), (0x50000000, 0x100000),
                       (0xA0000000, 0x100000), (0xE0000000, 0x100000),
                       (0x60000000, 0x10000000)):
        try:
            m.uc.mmio_map(base, size, m._mmio_read_cb, base,
                          m._mmio_write_cb, base)
        except Exception:
            pass

    sp, pc = m.boot()
    print("reset SP=%#x PC=%#x" % (sp, pc))

    if args.fast:
        m.set_fine(False)
    done = 0
    while done < args.max:
        n = min(args.chunk, args.max - done)
        m.run(n)
        done = m.insn_count
        reason = m.stop_reason
        if reason and reason != "max_insn":
            print("stop:", reason, "at", done)
            break
        print("[%10d] PC=%#010x pending=%s" % (done, pc_now(),
                                              sorted(m.pending_irqs)[:8]))

    print("instructions:", m.insn_count, "vtime:", m.vtime)
    print("PC=%#010x SP=%#010x LR=%#010x"
          % (pc_now(), m.reg("SP"), m.reg("LR")))
    stats = {}
    for ev in events:
        key = (ev[0], ev[6], ev[7])
        stats[key] = stats.get(key, 0) + 1
    print("\n-- watched access summary (dir, region, offset) --")
    for (d, name, off), cnt in sorted(stats.items()):
        print("  %s %-8s +%#04x  x%d" % (d, name, off, cnt))

    print("\n-- first 60 watched W events --")
    for ev in [e for e in events if e[0] == "W"][:60]:
        print("  W t=%-10d addr=%#010x size=%d val=%#010x pc=%#010x %s+%#x"
              % (ev[1], ev[2], ev[3], ev[4], ev[5], ev[6], ev[7]))
    print("\n-- first 40 watched R events --")
    for ev in [e for e in events if e[0] == "R"][:40]:
        print("  R t=%-10d addr=%#010x size=%d val=%#010x pc=%#010x %s+%#x"
              % (ev[1], ev[2], ev[3], ev[4], ev[5], ev[6], ev[7]))

    print("\n-- ALL TMR7 / NVIC / SysTick events --")
    for ev in events:
        if ev[6] in ("TMR7", "NVIC", "SysTick"):
            print("  %s t=%-10d addr=%#010x size=%d val=%#010x pc=%#010x %s+%#x"
                  % (ev[0], ev[1], ev[2], ev[3], ev[4], ev[5], ev[6], ev[7]))

    if m.stop_reason:
        print("\nSTOP REASON:", m.stop_reason)


if __name__ == "__main__":
    main()
