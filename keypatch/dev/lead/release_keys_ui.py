# -*- coding: utf-8 -*-
"""Lead: try releasing all GPIO keys after boot to unstick the X3-C UI.

Hypothesis: the firmware sees a key (power/test/touch) as held (emulator GPIO
inputs default 0 = active-low pressed) and therefore never leaves the boot
splash.  After the firmware booted, force all GPIO input pins to 1 (released)
and keep running; watch page state RAM and LCD.
"""
from __future__ import annotations

import os
import struct
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
TMR = os.path.join(ROOT, "keypatch", "dev", "tmr7")
for p in (EMU, TMR):
    if p not in sys.path:
        sys.path.insert(0, p)

from machine import Machine          # noqa: E402
import tmr7_model                    # noqa: E402
from lcd_capture import LcdCapture   # noqa: E402

IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
OUT = os.path.join(HERE, "out_ui2")
os.makedirs(OUT, exist_ok=True)

PAGE_MODE = 0x200003E7
PAGE_REQ = 0x200004C4
ROOT_WIN = 0x20000470
TICK = 0x20000310


def u32(m, a):
    try:
        return struct.unpack("<I", bytes(m.uc.mem_read(a, 4)))[0]
    except Exception:
        return -1


def u8(m, a):
    try:
        return m.uc.mem_read(a, 1)[0]
    except Exception:
        return -1


def release_all_inputs(m):
    for name, gpio in m.gpio.items():
        for pin in range(16):
            try:
                gpio.set_input(pin, 1)
            except Exception:
                pass
        print("  released all inputs on", name)


def main() -> int:
    m = Machine(log_fn=lambda s: None)
    tmr7_model.patch_real_device(m)
    tmr7_model.attach(m, scale_num=1, scale_den=1)
    m.boot()
    cap = LcdCapture(brighten=False).install(m)
    m.run(3_000_000)
    print("phase1 insn=%d pc=%#x tick=%d page=%#x req=%#x root=%#x"
          % (m.insn_count, m.uc.reg_read(11) & ~1, u32(m, TICK),
             u8(m, PAGE_MODE), u32(m, PAGE_REQ), u32(m, ROOT_WIN)))
    release_all_inputs(m)
    for i in range(16):
        m.run(1_000_000)
        path = os.path.join(OUT, "rel_%02d_ins%d.png" % (i, m.insn_count))
        cap.snapshot(path)
        print("[%2d] insn=%d pc=%#x tick=%d page=%#x req=%#x root=%#x lcd_px=%d nonblack=%.3f"
              % (i, m.insn_count, m.uc.reg_read(11) & ~1, u32(m, TICK),
                 u8(m, PAGE_MODE), u32(m, PAGE_REQ), u32(m, ROOT_WIN),
                 cap.stats().get("pixels", 0), cap.nonblack_ratio()))
    return 0


if __name__ == "__main__":
    sys.exit(main())
