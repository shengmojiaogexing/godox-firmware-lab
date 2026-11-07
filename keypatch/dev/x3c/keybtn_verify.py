# -*- coding: utf-8 -*-
"""X3-C 物理按键方案 —— 独立验证探针版 / 正式版成品 bin。

对以下两个成品分别做独立静态断言 + `verify_keys()`：
    keypatch/out/X3_C_v1.38_app_keyD798_keyprobe.bin
    keypatch/out/X3_C_v1.38_app_keyD798_keybtn2.bin

运行（在  下）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\keybtn_verify.py
    # 输出同时写入 keypatch/dev/x3c/keybtn_verify.log
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

import common  # noqa: E402
import devices.x3c as x3c  # noqa: E402

KEY = 0xD798
CASES = {
    "keyprobe": "X3_C_v1.38_app_keyD798_keyprobe.bin",
    "keybtn2": "X3_C_v1.38_app_keyD798_keybtn2.bin",
    "keybtn3": "X3_C_v1.38_app_keyD798_keybtn3.bin",
    "keybtn4": "X3_C_v1.38_app_keyD798_keybtn4.bin",
    "keybtn4_nobc_a": "X3_C_v1.38_app_keyD798_keybtn4_nobc_a.bin",
    "keybtn4_nobc_b": "X3_C_v1.38_app_keyD798_keybtn4_nobc_b.bin",
    "keybtn4_k1": "X3_C_v1.38_app_keyD798_keybtn4_k1.bin",
    "keybtn5": "X3_C_v1.38_app_keyD798_keybtn5.bin",
    "keybtn7": "X3_C_v1.38_app_keyD798_keybtn7.bin",
    "keybtn8": "X3_C_v1.38_app_keyD798_keybtn8.bin",
}
LOG = os.path.join(HERE, "keybtn_verify.log")

lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def decode_bw(addr: int, b: bytes) -> int:
    hw1, hw2 = struct.unpack("<HH", b)
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0x9000:
        raise ValueError("不是 B.W T4 编码: %s" % b.hex(" "))
    s = (hw1 >> 10) & 1
    imm10 = hw1 & 0x3FF
    j1 = (hw2 >> 13) & 1
    j2 = (hw2 >> 11) & 1
    i1 = 1 - (j1 ^ s)
    i2 = 1 - (j2 ^ s)
    imm = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | ((hw2 & 0x7FF) << 1)
    if imm & (1 << 24):
        imm -= 1 << 25
    return addr + 4 + imm


def check_one(mode: str, data: bytes, orig: bytes) -> bool:
    ok_all = True
    out("\n" + "=" * 78)
    out("模式 %s：%s" % (mode, CASES[mode]))
    out("sha256 : %s" % hashlib.sha256(data).hexdigest())
    out("=" * 78)

    ok = len(data) == len(orig) == x3c.IMAGE_SIZE
    out("  [%s] 长度不变: %d" % ("PASS" if ok else "FAIL", len(data)))
    ok_all &= ok
    try:
        t = decode_bw(x3c.KEYSCAN_FN,
                      data[x3c.KEYSCAN_FN - x3c.BASE:x3c.KEYSCAN_FN - x3c.BASE + 4])
        ok = t == x3c.BTN_INJ_BASE
        msg = "FUN_0800CC30 入口 B.W -> 0x%08X（应 0x%08X）" % (t, x3c.BTN_INJ_BASE)
    except ValueError as e:
        ok = False
        msg = "入口解码失败: %s" % e
    out("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
    ok_all &= ok

    # 阈值补丁按模式检查
    cfg = x3c._btn_cfg(mode)
    expect = {}
    if 1 in cfg["disable_bits"]:
        expect[x3c.KEYSCAN_LONG1_CMP] = x3c.KEYSCAN_CMP_DISABLED
    if 2 in cfg["disable_bits"]:
        expect[x3c.KEYSCAN_LONG2_CMP] = x3c.KEYSCAN_CMP_DISABLED
    for addr, want in expect.items():
        got = data[addr - x3c.BASE:addr - x3c.BASE + 2]
        ok = got == want
        out("  [%s] 0x%08X 原厂长按阈值 = %s（应 %s）"
            % ("PASS" if ok else "FAIL", addr, got.hex(" "), want.hex(" ")))
        ok_all &= ok
    # 未绑定的 bit1/keybtn2 阈值必须保持原厂
    if 1 not in cfg["disable_bits"]:
        got = data[x3c.KEYSCAN_LONG1_CMP - x3c.BASE:x3c.KEYSCAN_LONG1_CMP - x3c.BASE + 2]
        ok = got == x3c.KEYSCAN_CMP_BEFORE
        out("  [%s] 0x%08X bit1 原厂长按阈值保持原厂 %s"
            % ("PASS" if ok else "FAIL", x3c.KEYSCAN_LONG1_CMP, got.hex(" ")))
        ok_all &= ok

    full = x3c.verify_keys(data, KEY, mode)
    for ok2, msg in full:
        out("  [%s] %s" % ("PASS" if ok2 else "FAIL", msg))
        ok_all &= bool(ok2)
    return ok_all


def main() -> int:
    orig = common.read_file(os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin"))
    out("X3-C 物理按键方案 —— 独立验证（keyprobe / keybtn2）")
    ok_orig = hashlib.sha256(orig).hexdigest() == x3c.DEVICE["sha256"]
    out("原厂镜像未修改: %s (%s)" % (ok_orig, hashlib.sha256(orig).hexdigest()[:16] + "…"))
    ok_all = ok_orig
    for mode, name in CASES.items():
        path = os.path.join(ROOT, "keypatch", "out", name)
        if not os.path.isfile(path):
            out("\n[FAIL] 缺少成品 %s（先跑 keybtn_build.py --mode all）" % name)
            ok_all = False
            continue
        ok_all &= check_one(mode, common.read_file(path), orig)
    out("\n结论: %s" % ("ALL PASS" if ok_all else "FAIL"))
    with open(LOG, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % os.path.relpath(LOG, ROOT))
    return 0 if ok_all else 1


if __name__ == "__main__":
    sys.exit(main())
