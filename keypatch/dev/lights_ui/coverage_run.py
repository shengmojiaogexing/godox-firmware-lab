# -*- coding: utf-8 -*-
"""运行时执行覆盖：记录 AD100 Pro 在 (boot + 主循环 + 按键 + RF init) 下执行过的代码地址，
找出从未执行的函数（可回收做代码洞的候选）。

python coverage_run.py
"""
from __future__ import annotations

import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices/ad100pro", "emu"))

import core  # noqa: E402
from unicorn import UC_HOOK_CODE  # noqa: E402

FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
BASE = 0x08003000
IMAGE_END = 0x0800F448


def main():
    e = core.Emulator(FW, FUNCS, trace=False)
    hot = bytearray(IMAGE_END - BASE)
    stats = {"insn": 0}

    def hook(uc, address, size, user):
        if BASE <= address < IMAGE_END:
            hot[address - BASE] = 1
            stats["insn"] += 1
        return True

    e._uc.hook_add(UC_HOOK_CODE, hook, None, BASE, IMAGE_END)

    e.reset()
    e.run(5_000_000)
    print("after boot 5M: pc=0x%08X faults=%d" % (e.reg("PC"), len(e.faults)))
    e.boot_interactive()   # 重置 + 跳到主循环 + RF init
    print("boot_interactive insn=%d faults=%d" % (e.insn_count, len(e.faults)))
    for key in ("k0", "k1", "k2", "k3", "k4", "k5", "k6", "k7", "k9", "power"):
        e.inputs.press(key, 120)
        e.run(800_000)
        e.inputs.set_key(key, False)
        e.run(800_000)
    # 长按
    for key in ("k0", "k1", "k2", "k3"):
        e.inputs.press(key, 400)
        e.run(3_000_000)
        e.inputs.set_key(key, False)
        e.run(600_000)
    print("after keys: insn=%d faults=%d" % (e.insn_count, len(e.faults)))

    FUNC_LIST = json.load(open(FUNCS, encoding="utf-8"))
    never = []
    for f in FUNC_LIST:
        s = int(f["addr"], 16)
        en = int(f["end"], 16)
        if en <= s:
            continue
        touched = any(hot[i] for i in range(s - BASE, min(en, IMAGE_END) - BASE))
        if not touched:
            never.append((int(f["size"]), s, en))
    never.sort(reverse=True)
    print("functions never executed: %d (of %d)" % (len(never), len(FUNC_LIST)))
    for size, s, en in never:
        if size >= 24:
            print("  0x%08X..0x%08X size=%-4d" % (s, en, size))

    # 未执行连续区间（>=64 字节）
    print("\nnever-executed merged ranges >=64B:")
    ranges = []
    i = 0
    while i < len(hot):
        if not hot[i]:
            j = i
            while j < len(hot) and not hot[j]:
                j += 1
            if j - i >= 64:
                ranges.append((BASE + i, BASE + j, j - i))
            i = j
        else:
            i += 1
    for s, en, n in ranges:
        print("  0x%08X..0x%08X %d bytes" % (s, en, n))


if __name__ == "__main__":
    main()
