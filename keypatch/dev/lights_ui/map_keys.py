# -*- coding: utf-8 -*-
"""把模拟器注入的按键映射到固件键值位图（读 UI 状态 0x20000164+0x2c），并观察屏幕变化。

python map_keys.py
"""
from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices/ad100pro", "emu"))
sys.path.insert(0, HERE)

import core  # noqa: E402
from render_fb import fb_to_pixels, save_png  # noqa: E402

FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
OUT = os.path.join(HERE, "out")
UI = 0x20000164


def fb_sig(e):
    fb = bytes(e.screen_bits())
    return sum(1 for b in fb if b), hash(fb)


def boot():
    e = core.Emulator(FW, FUNCS, trace=False)
    e.boot_interactive()
    return e


def main():
    keys = ["k0", "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k8", "k9", "power", "key2"]
    base = boot()
    print("base UI: %s" % base.read_mem(UI, 0x40).hex(" "))
    base_sig = fb_sig(base)
    print("base fb:", base_sig)
    save_png(fb_to_pixels(base.screen_bits()), os.path.join(OUT, "map_base.png"), 6)

    results = {}
    for key in keys:
        e = boot()
        e.inputs.press(key, 150)
        e.run(1_500_000)
        ui = e.read_mem(UI, 0x40)
        keyval = int.from_bytes(ui[0x2c:0x2e], "little")
        flags = {
            "ui[0x09]": ui[0x09], "ui[0x0b]": ui[0x0b], "ui[0x0c]": ui[0x0c],
            "ui[0x0d]": ui[0x0d], "ui[0x0f]": ui[0x0f], "ui[0x10]": ui[0x10],
            "ui[0x11]": ui[0x11], "ui[0x13]": ui[0x13], "ui[0x1c]": ui[0x1c],
        }
        # 长按：再跑 3M
        e.inputs.set_key(key, False)
        e.run(1_000_000)
        ui2 = e.read_mem(UI, 0x40)
        sig = fb_sig(e)
        save_png(fb_to_pixels(e.screen_bits()), os.path.join(OUT, "map_%s.png" % key), 6)
        print("%-6s keyval=0x%03X flags=%s | after release fb=%s nz=%d" %
              (key, keyval, flags, sig[1], sig[0]))
        results[key] = keyval

    print("\nsummary: %s" % {k: "0x%03X" % v for k, v in results.items()})


if __name__ == "__main__":
    main()
