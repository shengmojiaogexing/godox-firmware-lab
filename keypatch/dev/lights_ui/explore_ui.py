# -*- coding: utf-8 -*-
"""AD100 Pro UI 探索脚本（只读分析，不改固件）。

用法:
  tools\\python\\python.exe keypatch\\dev\\lights_ui\\explore_ui.py

功能:
  1) boot_interactive 到 1ms 主调度器；
  2) 依次注入按键，截图 + 记录关键 RAM 状态；
  3) 输出到 keypatch/dev/lights_ui/out/。
"""
from __future__ import annotations

import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
HERE = os.path.dirname(os.path.abspath(__file__))
OUT = os.path.join(HERE, "out")
EMU_DIR = os.path.join(ROOT, "devices/ad100pro", "emu")
if EMU_DIR not in sys.path:
    sys.path.insert(0, EMU_DIR)

import core           # noqa: E402
import main as emu_main  # noqa: E402

FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")

# UI 状态区（docs/04_ui_lcd.md §3.2）
UI_STATE = 0x20000164
SETTINGS = 0x20000480
UI_FLAGS = 0x20000184

os.makedirs(OUT, exist_ok=True)


def snap(e, name, rotate=270, mirror=False, scale=4):
    path = os.path.join(OUT, name)
    p, size, lit = emu_main.write_screen_png(e, path, scale=scale, rotate=rotate, mirror=mirror)
    print("  screen -> %s (%d B, %d lit)" % (p, size, lit))
    return path


def dump_state(e, tag):
    ui = e.read_mem(UI_STATE, 0x40)
    st = e.read_mem(SETTINGS, 0x40)
    flags = e.read_mem(UI_FLAGS, 0x20)
    print("[%s] pc=0x%08X insn=%d" % (tag, e.reg('PC'), e.insn_count))
    print("    UI 0x%08X: %s" % (UI_STATE, ui.hex(" ")))
    print("    SET 0x%08X: %s" % (SETTINGS, st.hex(" ")))
    print("    FLG 0x%08X: %s" % (UI_FLAGS, flags.hex(" ")))


def press(e, key, hold_insn=200_000, settle=400_000):
    e.inputs.press(key, 200)
    e.run(hold_insn)
    e.inputs.set_key(key, False)
    e.run(settle)


def main():
    e = core.Emulator(FW, FUNCS, trace=False)
    print("boot_interactive ...")
    r = e.boot_interactive()
    print("  pc=0x%08X insn=%d phase=%s rf=%s faults=%d"
          % (r["pc"], r["insn"], r["taskPhase"], r["rf"].get("state"), len(e.faults)))
    dump_state(e, "baseline")
    snap(e, "00_baseline.png")

    for i, key in enumerate(("set", "up", "down", "mode", "key2", "menu", "enter"), 1):
        print("== press %s ==" % key)
        press(e, key)
        print("  pc=0x%08X" % e.reg("PC"))
        dump_state(e, "after_" + key)
        snap(e, "%02d_%s.png" % (i, key))

    print("faults:", [(f.kind, hex(f.pc), hex(f.addr) if f.addr else None) for f in e.faults[:5]])


if __name__ == "__main__":
    main()
