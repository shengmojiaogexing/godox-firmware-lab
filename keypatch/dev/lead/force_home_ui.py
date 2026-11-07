# -*- coding: utf-8 -*-
"""Lead: try to force the X3-C firmware UI to the home page and capture it.

Boots with the TMR7 tick model, runs long enough for the UI framework to be
initialised, then writes the "mode/page" byte (RAM 0x200003E7) to 1 (home) and
keeps running so the 100 ms UI tick rebuilds the page.  Captures LCD snapshots.
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
OUT = os.path.join(HERE, "out_ui")
os.makedirs(OUT, exist_ok=True)

PAGE_MODE = 0x200003E7        # 当前模式/页面
PAGE_REQ = 0x200004C4         # 请求页面


def main() -> int:
    m = Machine(log_fn=lambda s: None)
    tmr7_model.patch_real_device(m)
    tmr7_model.attach(m, scale_num=4, scale_den=1)   # 4 instr per TMR7 clock -> fast UI time
    m.boot()
    cap = LcdCapture(brighten=False).install(m)
    print("booted, running to UI-ready...")
    t0 = time.time()
    m.run(8_000_000)
    print("phase1 insns=%d pc=%#x tick=%d" %
          (m.insn_count, m.uc.reg_read(11) & ~1,
           struct.unpack("<I", bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)))[0]))
    print("before: page_mode=%#x page_req=%#x" %
          (m.uc.mem_read(PAGE_MODE, 1)[0], m.uc.mem_read(PAGE_REQ, 4)[0] & 0xFF))
    # force home page
    m.uc.mem_write(PAGE_MODE, b"\x01")
    m.uc.mem_write(PAGE_REQ, b"\x01\x00\x00\x00")
    for i in range(12):
        m.run(500_000)
        tick = struct.unpack("<I", bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)))[0]
        pm = m.uc.mem_read(PAGE_MODE, 1)[0]
        pr = m.uc.mem_read(PAGE_REQ, 4)[0]
        path = os.path.join(OUT, "home_%02d_ins%d_tick%d.png" % (i, m.insn_count, tick))
        cap.snapshot(path)
        st = cap.stats()
        print("[%2d] insn=%d tick=%d pc=%#x page_mode=%#x req=%#x lcd_px=%d cmds=%d nonblack=%.3f -> %s"
              % (i, m.insn_count, tick, m.uc.reg_read(11) & ~1, pm, pr,
                 st.get("pixels", 0), st.get("cmds", 0), cap.nonblack_ratio(), os.path.basename(path)))
    print("done in %.1fs" % (time.time() - t0))
    return 0


if __name__ == "__main__":
    sys.exit(main())
