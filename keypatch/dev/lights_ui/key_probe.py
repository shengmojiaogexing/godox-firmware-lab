# -*- coding: utf-8 -*-
"""交互式 UI 探索：boot_interactive 后按各种键，正确渲染 FB 并记录函数调用序列。

python key_probe.py [key ...]
"""
from __future__ import annotations

import collections
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices/ad100pro", "emu"))
sys.path.insert(0, HERE)

import core                      # noqa: E402
from render_fb import fb_to_pixels, save_png, ascii_art  # noqa: E402

FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
OUT = os.path.join(HERE, "out")
os.makedirs(OUT, exist_ok=True)

FN_TABLE = json.load(open(FUNCS, encoding="utf-8"))
FN_STARTS = {}
for f in FN_TABLE:
    a = int(f["addr"], 16)
    FN_STARTS[a] = f

UI = 0x20000164
SET = 0x20000480


def install_fn_tracer(e, limit=20000):
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import UC_ARM_REG_PC

    calls = []
    seen_starts = FN_STARTS

    def hook(uc, address, size, user):
        if len(calls) < limit and address in seen_starts:
            calls.append(address)
        return True

    # 只挂 ROM 范围（0x08003000..0x0800F448）
    h = e._uc.hook_add(UC_HOOK_CODE, hook, None, 0x08003000, 0x0800F448)
    return calls, h


def render(e, tag):
    fb = bytes(e.screen_bits())
    pix = fb_to_pixels(fb, top_bit=7)
    path = os.path.join(OUT, tag + ".png")
    save_png(pix, path, scale=6)
    print("[%s] pc=0x%08X insn=%d fb_nonzero=%d lit=%d frames=%d"
          % (tag, e.reg("PC"), e.insn_count, sum(1 for b in fb if b),
             sum(sum(r) for r in pix), e.lcd.frames))
    return pix


def dump_ui(e, tag):
    ui = e.read_mem(UI, 0x40)
    st = e.read_mem(SET, 0x40)
    print("  UI : %s" % ui.hex(" "))
    print("  SET: %s" % st.hex(" "))


def summarize_calls(calls, top=25):
    cnt = collections.Counter(calls)
    print("  calls=%d distinct=%d" % (len(calls), len(cnt)))
    for a, n in cnt.most_common(top):
        f = FN_STARTS.get(a)
        print("   0x%08X x%-3d size=%s" % (a, n, f["size"] if f else "?"))


def main():
    keys = sys.argv[1:] or ["set", "mode", "up", "down", "power"]
    e = core.Emulator(FW, FUNCS, trace=False)
    e.boot_interactive()
    calls, hook = install_fn_tracer(e)
    render(e, "probe_00_base")
    dump_ui(e, "base")
    print("--- base calls:", len(calls))
    summarize_calls(calls)
    calls.clear()

    for i, key in enumerate(keys, 1):
        print("=== key %s ===" % key)
        e.inputs.press(key, 120)
        e.run(1_200_000)
        render(e, "probe_%02d_%s_down" % (i, key))
        dump_ui(e, "down")
        e.inputs.set_key(key, False)
        e.run(1_200_000)
        render(e, "probe_%02d_%s_up" % (i, key))
        dump_ui(e, "up")
        summarize_calls(calls)
        calls.clear()

    print("faults:", [(f.kind, hex(f.pc)) for f in e.faults[:5]])


if __name__ == "__main__":
    main()
