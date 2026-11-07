# -*- coding: utf-8 -*-
"""枚举 AD100 Pro C.Fn 菜单：逐项设置 UI+0x17 选择索引、设置值数组 0x20000480，渲染菜单。

python cfn_enum.py
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
UI = 0x20000164
SET = 0x20000480
DRAW = 0x08003F0C


def render(e, tag, art=False):
    fb = bytes(e.read_mem(FB, 1024))
    pix = fb_to_pixels(fb, top_bit=7)
    save_png(pix, os.path.join(OUT, tag + ".png"), scale=6)
    if art:
        print(ascii_art(pix))
    return sum(1 for b in fb if b)


def main():
    e = core.Emulator(FW, FUNCS, trace=False)
    e.boot_interactive()
    # 默认值数组（boot 后 0x20000480 = 01 00 02 00 01 00 00 00）
    default = bytes(e.read_mem(SET, 8))
    print("default C.Fn values:", default.hex(" "))
    # 确保进入 C.Fn 画面：UI+0x0x1a? 绘制直接调用即可
    e.write_mem(UI + 0x17, b"\x00")
    for sel in range(0, 9):
        e.write_mem(UI + 0x17, bytes([sel]))
        e.write_mem(FB, b"\x00" * 1024)
        e.call_function(DRAW, args=(), max_insns=2_000_000)
        nz = render(e, "cfn_sel%d" % sel, art=(sel in (0, 1, 2)))
        print("sel=%d fb_nonzero=%d" % (sel, nz))


if __name__ == "__main__":
    main()
