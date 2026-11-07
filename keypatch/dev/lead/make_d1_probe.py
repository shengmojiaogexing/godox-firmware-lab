# -*- coding: utf-8 -*-
"""诊断变体 D1：在 A2-p0b 基础上放开两道限制，用来定位"长按无反应"。

A2-p0b 的 poll 有三道防线：
  1) 页面门：`*(u8*)0x200003E7 != 0` 就清状态返回；
  2) 控件树校验（0x080303B8）；
  3) 长按阈值 >=5 tick。
D1 = 去掉页面门 + 阈值改为 1 tick（保留树校验），这样用户一按就能看到动作，
用来判断"是不是页面门/阈值导致没反应"。

改动（A2-p0b 的 poll @0x080F1000）：
  0x080F100A: `0cd0` (beq 0x080F1026) -> `0ce0` (b 0x080F1026)   ; 绕过页面门
  0x080F1050: `052a` (cmp r2,#5)       -> `012a` (cmp r2,#1)      ; 锁：立即触发
  0x080F10A4: `052a` (cmp r2,#5)       -> `012a` (cmp r2,#1)      ; 设置页签：立即触发
"""
from __future__ import annotations

import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
KEYPATCH = os.path.join(ROOT, "keypatch")
if KEYPATCH not in sys.path:
    sys.path.insert(0, KEYPATCH)

import common  # noqa: E402

BASE = 0x08004800
SRC = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_A2p0b.bin")
DST = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_D1_probe.bin")

PATCHES = [
    (0x080F100A, "0cd0", "0ce0", "页面门 -> 无条件进入"),
    (0x080F1050, "052a", "012a", "锁长按阈值 5 -> 1"),
    (0x080F10A4, "052a", "012a", "设置页签长按阈值 5 -> 1"),
]


def main() -> int:
    data = bytearray(common.read_file(SRC))
    for va, old, new, desc in PATCHES:
        off = va - BASE
        got = bytes(data[off:off + 2]).hex()
        if got != old:
            print("0x%08X 期望 %s 实际 %s（%s）" % (va, old, got, desc))
            return 1
        data[off:off + 2] = bytes.fromhex(new)
        print("0x%08X: %s -> %s  %s" % (va, old, new, desc))
    out = bytes(data)
    common.write_file(DST, out)
    print("成品:", DST)
    print("长度:", len(out), "sha256:", hashlib.sha256(out).hexdigest())
    return 0


if __name__ == "__main__":
    sys.exit(main())
