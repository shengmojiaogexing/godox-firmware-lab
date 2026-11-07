#!/usr/bin/env python3
"""explore.py -- fast-forward then detailed trace, for the X3-C emulator.

Boots the firmware, runs N instructions in fast burst mode (no per-instruction
hook, ~20-50x faster), then switches to fine mode and traces/breaks so the
"steady state" (main loop) can be analysed without paying the hook cost for the
whole boot.

Examples
--------
    # fast-forward 150M instructions, then trace 20000 instructions with a PC
    # histogram, call-target histogram and MMIO dump
    python explore.py --fast 150000000 --trace 20000

    # fast-forward, then stop at the first IWDG write (0x0803BA52) and dump state
    python explore.py --fast 140000000 --break 0x0803BA52

    # fast-forward, then watch RAM writes around 0x2004xxxx for 5M instructions
    python explore.py --fast 150000000 --watch 0x20040000 0x20050000 --trace 200000
"""
from __future__ import annotations

import argparse
import os
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

from machine import Machine, DEFAULT_IMAGE, APP_BASE, load_symbol_file


def parse_hex(v):
    return int(v, 0)


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=DEFAULT_IMAGE)
    ap.add_argument("--base", type=parse_hex, default=APP_BASE)
    ap.add_argument("--fast", type=int, default=150_000_000,
                    help="fast-forward instruction count")
    ap.add_argument("--trace", type=int, default=20000,
                    help="fine-mode instructions to trace after fast-forward")
    ap.add_argument("--break", dest="break_at", type=parse_hex, default=None,
                    help="stop at this PC after fast-forward")
    ap.add_argument("--watch", nargs=2, type=parse_hex, default=None,
                    metavar=("LO", "HI"), help="watchpoint range")
    ap.add_argument("--symbols", default=None)
    ap.add_argument("--chunk", type=int, default=200000)
    args = ap.parse_args(argv)

    m = Machine(image=os.path.abspath(args.img), image_base=args.base,
                log_fn=lambda s: None)
    m.symbols.update(load_symbol_file(args.symbols or
                                      os.path.join(HERE, "symbols.json")))
    m.boot()
    print("booted: SP=%#x PC=%#x" % (m.reg("SP"), m.reg("PC")))

    if args.fast:
        m.set_fine(False)
        r = m.run(max_insns=args.fast, chunk=args.chunk)
        print("fast-forwarded %d instructions (stop=%s) -> PC=%#x SP=%#x LR=%#x"
              % (m.insn_count, r, m.reg("PC"), m.reg("SP"), m.reg("LR")))
    else:
        print("warning: no fast-forward; tracing will be slow for long boots")

    m.set_fine(True)
    if args.watch:
        lo, hi = args.watch
        hits = Counter()

        def cb(uc, access, address, size, value, user_data):
            hits[address] += 1
            if sum(hits.values()) <= 40:
                print("  [watch] %s @ %#x size=%d value=%#x pc=%#x lr=%#x"
                      % ("W" if access == 2 else "R", address, size, value,
                         uc.reg_read(15), uc.reg_read(14)))
        m.add_mem_hook(lo, hi, "w", cb)

    # enable call tracing + PC histogram
    m.call_trace_enabled = True
    pcs = Counter()
    m.pc_counter = pcs

    if args.break_at is not None:
        m.breakpoints[args.break_at] = "explore"
        r = m.run(max_insns=args.trace)
        print("stopped: %s at PC=%#x LR=%#x SP=%#x"
              % (r, m.reg("PC"), m.reg("LR"), m.reg("SP")))
        print(m.register_string())
        # unwind a few frames by scanning the stack for return addresses
        print("stack scan (return-address candidates):")
        sp = m.reg("SP")
        for off in range(0, 0x100, 4):
            w = m.peek_u32(sp + off)
            if m.image_base <= w < m.image_base + m.image_len:
                print("  [sp+%#04x] %#010x" % (off, w))
    else:
        r = m.run(max_insns=args.trace)
        print("traced %d instructions (stop=%s), %d distinct PCs"
              % (args.trace, r, len(pcs)))

    print("\ntop PCs:")
    for pc, cnt in pcs.most_common(40):
        print("  %8d  %#010x" % (cnt, pc))
    print("\ncall targets (BL/BLX):")
    for target, cnt in m.call_targets.most_common(30):
        print("  %8d  %#010x" % (cnt, target))
    print("\nhot MMIO in trace:")
    print(m.hot_mmio_report(25))
    print("\nunknown MMIO:")
    print(m.unknown_mmio_report(25) or "  (none)")
    print("\nlast 40 PCs:", " ".join("%#x" % p for p in m.pc_history(40)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
