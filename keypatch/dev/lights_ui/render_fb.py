# -*- coding: utf-8 -*-
"""AD100 Pro 帧缓冲正确渲染（bit7 = 每页顶行；可旋转 180° 还原面板方向）。

  python render_fb.py              # 从原厂固件 + 模拟器启动后截图
  python render_fb.py --bin FILE   # 直接从二进制镜像（未运行，内容为初始 FB=空）
"""
from __future__ import annotations

import os
import sys


def fb_to_pixels(fb: bytes, w=128, h=64, top_bit=7):
    """page*128+x 布局；top_bit 指定每页内 bit 的顶行（本固件实测 bit7=顶）。"""
    pix = [[0] * w for _ in range(h)]
    for page in range(h // 8):
        for x in range(w):
            b = fb[page * w + x]
            for i in range(8):
                bit = (top_bit - i)  # top_bit=7 -> bit7 是页内顶行
                if (b >> bit) & 1:
                    pix[page * 8 + i][x] = 1
    return pix


def rot180(pix):
    return [list(reversed(row)) for row in reversed(pix)]


def ascii_art(pix, xstep=1, ystep=1):
    out = []
    for y in range(0, len(pix), ystep):
        out.append("".join("#" if pix[y][x] else "." for x in range(0, len(pix[0]), xstep)))
    return "\n".join(out)


def save_png(pix, path, scale=6):
    sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "..", "..", "devices/ad100pro", "emu"))
    try:
        import main as emu_main
    except Exception:
        emu_main = None
    if emu_main:
        png = emu_main.make_png_pixels(pix, scale=scale)
        with open(path, "wb") as f:
            f.write(png)
        return path
    raise RuntimeError("cannot import emu main")


def main():
    root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
    sys.path.insert(0, os.path.join(root, "devices/ad100pro", "emu"))
    import core
    fw = os.path.join(root, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
    fns = os.path.join(root, "devices/ad100pro", "out", "functions.json")
    e = core.Emulator(fw, fns, trace=False)
    e.boot_interactive()
    fb = bytes(e.screen_bits())
    out = os.path.join(os.path.dirname(__file__), "out")
    os.makedirs(out, exist_ok=True)
    pix = fb_to_pixels(fb, top_bit=7)
    save_png(pix, os.path.join(out, "screen_correct_raw.png"), scale=6)
    save_png(rot180(pix), os.path.join(out, "screen_correct_panel.png"), scale=6)
    print("nonzero bytes=", sum(1 for b in fb if b), "lit pixels=",
          sum(sum(row) for row in pix))
    print("==== raw (bit7 top) ====")
    print(ascii_art(pix, xstep=1, ystep=1))
    print("==== panel (rot180) ====")
    print(ascii_art(rot180(pix), xstep=1, ystep=1))


if __name__ == "__main__":
    main()
