# -*- coding: utf-8 -*-
"""生成 X3-C 备选成品：A–E 逐组广播（broadcast_per_group=True）+ 核心双发补丁。

主成品（ALL 组）由 devices/x3c.py 集成生成：
  out/X3_C_v1.38_app_keyD798.bin
备选成品（逐组 10..14）：
  out/X3_C_v1.38_app_keyD798_pergroup.bin

用法：
  tools\\python\\python.exe keypatch\\dev\\lead\\make_pergroup_final.py
"""
from __future__ import annotations

import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
KEYPATCH = os.path.join(ROOT, "keypatch")
ROM = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
UI_DIR = os.path.join(KEYPATCH, "dev", "x3c_ui")

for p in (KEYPATCH, UI_DIR):
    if p not in sys.path:
        sys.path.insert(0, p)

import common  # noqa: E402
import devices.x3c as x3c  # noqa: E402
import ui_patch  # noqa: E402

KEY = 0xD798
DST = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_pergroup.bin")


def main() -> int:
    orig = common.read_file(ROM)
    opts = {
        "key": KEY,
        "key_flag": x3c.KEY_ON_FLAG_RAM,
        "enable_menu_patch": True,
        "broadcast_per_group": True,
        "allid_step": 1,
    }
    ui_img, ui_log = ui_patch.apply_ui(orig, opts)
    print("UI 层（逐组广播）生成: 差异 %d 字节"
          % sum(1 for a, b in zip(orig, ui_img) if a != b))

    # 叠加核心双发补丁
    buf = bytearray(ui_img)
    for addr, before, after, desc in x3c._patched_regions(KEY):
        off = addr - x3c.BASE
        got = bytes(buf[off:off + len(before)])
        if got != before:
            print("核心补丁点 0x%08X 原字节不匹配: %s != %s"
                  % (addr, got.hex(" "), before.hex(" ")))
            return 1
        buf[off:off + len(after)] = after
    final = bytes(buf)
    common.write_file(DST, final)

    diffs = sum(1 for a, b in zip(orig, final) if a != b)
    print("成品:", DST)
    print("长度:", len(final), "差异字节:", diffs)
    print("sha256:", hashlib.sha256(final).hexdigest())

    ok = True
    for addr, before, after, desc in x3c._patched_regions(KEY):
        off = addr - x3c.BASE
        good = final[off:off + len(after)] == after
        ok &= good
        print("  [%s] 0x%08X %s" % ("PASS" if good else "FAIL", addr, desc))
    inj_code, _ = x3c.build_injection(KEY)
    off = x3c.INJ_BASE - x3c.BASE
    good = final[off:off + len(inj_code)] == inj_code
    ok &= good
    print("  [%s] 核心注入区 0x%08X" % ("PASS" if good else "FAIL", x3c.INJ_BASE))
    # UI 层校验（在"原厂+UI"上验证）
    for ok2, msg in ui_patch.verify_ui(ui_img, opts):
        print("  [%s] [UI] %s" % ("PASS" if ok2 else "FAIL", msg))
        ok &= ok2
    print("结论:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
