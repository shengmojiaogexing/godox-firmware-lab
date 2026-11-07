# -*- coding: utf-8 -*-
"""X3-C 救砖/回退与安全测试用镜像。

生成：
1) X3_C_v1.38_app_keyD798_core.bin  —— 只有核心双发补丁、无任何 UI 补丁（安全测试用）
2) X3_C_v1.38_app_keyD798_ui_fix2.bin —— 修正了 UI 磁贴父窗口寄存器的候选 UI 版
   （ui_patch v1.2 的 quickpanel ext 里 parent 用了 r6=0x200004C7(RAM 字节指针)，
    导致建磁贴时把 RAM 当控件结构写 -> 崩溃重启；改为 r5=快捷面板根窗口，
    颜色暂存改到 r6。）

用法：
  tools\\python\\python.exe keypatch\\dev\\lead\\make_recovery_bins.py
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
import devices.x3c as x3c  # noqa: E402

KEY = 0xD798
ROM = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
MAIN_UI = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798.bin")
CORE_OUT = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_core.bin")
FIX_OUT = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_ui_fix2.bin")


def apply_core(data: bytes) -> bytes:
    buf = bytearray(data)
    for addr, before, after, desc in x3c._patched_regions(KEY):
        off = addr - x3c.BASE
        got = bytes(buf[off:off + len(before)])
        if got != before:
            raise ValueError("核心补丁点 0x%08X 原字节不匹配" % addr)
        buf[off:off + len(after)] = after
    return bytes(buf)


def main() -> int:
    orig = common.read_file(ROM)

    # 1) 核心-only
    core = apply_core(orig)
    common.write_file(CORE_OUT, core)
    print("1) 核心-only:", CORE_OUT)
    print("   长度 %d 差异 %d sha256 %s"
          % (len(core), sum(1 for a, b in zip(orig, core) if a != b),
             hashlib.sha256(core).hexdigest()))

    # 2) UI 修复版：在集成好的 UI bin 上改 4 个寄存器移动指令
    ui = bytearray(common.read_file(MAIN_UI))
    fixes = [
        # (va, old_hex, new_hex, 说明)
        (0x080F0806, "0546", "0646", "mov r5,r0 -> mov r6,r0（颜色暂存 r6）"),
        (0x080F0812, "3046", "2846", "mov r0,r6 -> mov r0,r5（parent=面板根窗口）"),
        (0x080F083A, "3046", "2846", "mov r0,r6 -> mov r0,r5（第二个磁贴 parent）"),
        (0x080F0858, "2846", "3046", "mov r0,r5 -> mov r0,r6（返回颜色）"),
    ]
    for va, old, new, desc in fixes:
        off = va - x3c.BASE
        got = bytes(ui[off:off + 2]).hex()
        if got != old:
            raise ValueError("0x%08X 期望 %s 实际 %s（%s）" % (va, old, got, desc))
        ui[off:off + 2] = bytes.fromhex(new)
        print("   修复 0x%08X: %s -> %s  %s" % (va, old, new, desc))
    fixed = bytes(ui)
    common.write_file(FIX_OUT, fixed)
    print("2) UI 修复候选:", FIX_OUT)
    print("   长度 %d 差异 %d sha256 %s"
          % (len(fixed), sum(1 for a, b in zip(orig, fixed) if a != b),
             hashlib.sha256(fixed).hexdigest()))
    return 0


if __name__ == "__main__":
    sys.exit(main())
