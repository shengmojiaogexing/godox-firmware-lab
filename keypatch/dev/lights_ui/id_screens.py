# -*- coding: utf-8 -*-
"""直接调用各"屏幕绘制"候选函数，清空 FB 后渲染，识别每个函数画的是哪个界面。

python id_screens.py
"""
from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices/ad100pro", "emu"))
sys.path.insert(0, HERE)

import core  # noqa: E402
from render_fb import fb_to_pixels, save_png, ascii_art  # noqa: E402

FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
OUT = os.path.join(HERE, "out")
FB = 0x200004CC

# 0x0800C2B8 里分派到的子绘制函数 + 其它候选
CANDIDATES = [
    0x08003F0C, 0x08005474, 0x080054E4, 0x080056D0, 0x080056E4,
    0x080058E0, 0x080059CC, 0x08005A18, 0x08005BC8, 0x080053E4,
    0x08005400, 0x08004C0C, 0x08004C50, 0x08004C5C, 0x08004CD4,
]


def main():
    e = core.Emulator(FW, FUNCS, trace=False)
    e.boot_interactive()
    print("boot ok insn=%d" % e.insn_count)
    for addr in CANDIDATES:
        e.write_mem(FB, b"\x00" * 1024)
        ok = e.call_function(addr, args=(), max_insns=2_000_000)
        fb = bytes(e.read_mem(FB, 1024))
        pix = fb_to_pixels(fb, top_bit=7)
        nz = sum(1 for b in fb if b)
        path = os.path.join(OUT, "screen_%08X.png" % addr)
        save_png(pix, path, scale=6)
        print("\n=== 0x%08X call_ok=%s fb_nonzero=%d ===" % (addr, ok, nz))
        print(ascii_art(pix, xstep=1, ystep=1))


if __name__ == "__main__":
    main()
