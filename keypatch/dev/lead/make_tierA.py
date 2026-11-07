# -*- coding: utf-8 -*-
"""构建诊断档 A：只保留"已修好的页 1 钩子 + 轮询"，**不建任何新控件**。

做法：从 _ui_fix3.bin（v1.3 + 修好的页 1 钩子）出发，把快捷面板构建函数尾部的
挂载点 0x08008236 恢复成原厂 `bl 0x08027FE0`（4 字节），这样注入的建磁贴函数
永远不会被调用；其余（0x0800FE78 原厂入口、UI tick 调用点 wrapper、poll、注入函数）
保持。

产物：keypatch/out/X3_C_v1.38_app_keyD798_tierA.bin
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
ROM = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
SRC = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_ui_fix3.bin")
DST = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_tierA.bin")

HOOK = 0x08008236
HOOK_ORIG = bytes.fromhex("1ff0d3fe")   # bl 0x08027FE0


def main() -> int:
    orig = common.read_file(ROM)
    data = bytearray(common.read_file(SRC))
    off = HOOK - BASE
    data[off:off + 4] = HOOK_ORIG
    print("0x%08X 恢复原厂: %s（建磁贴函数不再被调用）"
          % (HOOK, HOOK_ORIG.hex(" ")))
    out = bytes(data)
    common.write_file(DST, out)
    print("成品:", DST)
    print("长度:", len(out), "差异字节:", sum(1 for a, b in zip(orig, out) if a != b))
    print("sha256:", hashlib.sha256(out).hexdigest())
    return 0


if __name__ == "__main__":
    sys.exit(main())
