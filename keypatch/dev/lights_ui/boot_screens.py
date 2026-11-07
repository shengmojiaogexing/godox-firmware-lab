# -*- coding: utf-8 -*-
"""从复位真实运行 AD100 Pro，在多个指令节点渲染 FB（正确 bit7=顶），观察屏幕状态。

python boot_screens.py [max_insn] [step]
"""
from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices/ad100pro", "emu"))
sys.path.insert(0, HERE)

import core                      # noqa: E402
from render_fb import fb_to_pixels, rot180, save_png, ascii_art  # noqa: E402

FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
OUT = os.path.join(HERE, "out")
os.makedirs(OUT, exist_ok=True)


def render(e, tag, scale=6):
    fb = bytes(e.screen_bits())
    pix = fb_to_pixels(fb, top_bit=7)
    save_png(pix, os.path.join(OUT, tag + ".png"), scale=scale)
    nonz = sum(1 for b in fb if b)
    lit = sum(sum(r) for r in pix)
    print("%-14s insn=%-9d pc=0x%08X fb_nonzero=%d lit=%d frames=%d faults=%d"
          % (tag, e.insn_count, e.reg("PC"), nonz, lit, e.lcd.frames, len(e.faults)))
    return pix


def main():
    max_insn = int(sys.argv[1]) if len(sys.argv) > 1 else 60_000_000
    step = int(sys.argv[2]) if len(sys.argv) > 2 else 5_000_000
    e = core.Emulator(FW, FUNCS, trace=False)
    e.reset()
    n = 0
    while e.insn_count < max_insn:
        e.run(step)
        n += 1
        pix = render(e, "boot_%02d" % n)
        if n <= 3:
            print(ascii_art(pix, xstep=1, ystep=1))
    print("done. irq=%s faults=%s" % (dict(e.irq_counts), len(e.faults)))


if __name__ == "__main__":
    main()
