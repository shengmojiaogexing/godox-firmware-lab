# -*- coding: utf-8 -*-
"""X3-C 物理按键方案 —— 生成探针版 / 正式版成品 bin。

    --mode keyprobe : core + 长按三个键分别闪 1/2/3 次（当前 ID 公共同步字）
    --mode keybtn2  : core + 长按拨轮切密钥 / 长按 ⚡ 32 频道广播（旧假设键位）
    --mode keybtn3  : core + 长按拨轮/M/C 切密钥 / 长按 ⚡ 广播（实机探针最终键位）
    --mode all      : 全部生成

输出（不覆盖任何已有 bin）：
    keypatch/out/X3_C_v1.38_app_keyD798_keyprobe.bin
    keypatch/out/X3_C_v1.38_app_keyD798_keybtn2.bin
    keypatch/out/X3_C_v1.38_app_keyD798_keybtn3.bin

运行（在  下）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\keybtn_build.py --mode all
"""
from __future__ import annotations

import argparse
import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

import common  # noqa: E402
import devices.x3c as x3c  # noqa: E402

SRC = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
OUT_DIR = os.path.join(ROOT, "keypatch", "out")
KEY = 0xD798
OUTS = {
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
    "keybtn11": "X3_C_v1.38_app_keyD798_keybtn11.bin",
    "keybtn12": "X3_C_v1.38_app_keyD798_keybtn12.bin",
}

lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def build_one(mode: str, orig: bytes) -> int:
    dst = os.path.join(OUT_DIR, OUTS[mode])
    start = len(lines)
    patched, log = x3c.apply_keys(orig, KEY, mode)
    if len(patched) != len(orig):
        out("[FAIL] %s 长度变化 %d -> %d" % (mode, len(orig), len(patched)))
        return 1
    common.write_file(dst, patched)
    out("=" * 78)
    out("模式 %s -> %s (%d B)" % (mode, os.path.relpath(dst, ROOT), len(patched)))
    out("=" * 78)
    for i, e in enumerate(log, 1):
        out("  [%d] 0x%08X  %s -> %s  %s"
            % (i, e["addr"], e["before"], e["after"], e["desc"]))
    diffs = [i for i in range(len(orig)) if orig[i] != patched[i]]
    out("  sha256 : %s" % hashlib.sha256(patched).hexdigest())
    out("  差异   : %d 字节" % len(diffs))
    out("")
    results = x3c.verify_keys(patched, KEY, mode)
    ok_all = True
    for ok, msg in results:
        out("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
        ok_all &= bool(ok)
    out("  结论: %s" % ("ALL PASS" if ok_all else "FAIL"))
    logpath = os.path.join(HERE, "keybtn_build_%s.log" % mode)
    with open(logpath, "w", encoding="utf-8") as f:
        f.write("\n".join(lines[start:]) + "\n")
    out("  [log] %s" % os.path.relpath(logpath, ROOT))
    return 0 if ok_all else 1


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--mode", default="all", choices=["keyprobe", "keybtn2", "keybtn3", "keybtn4", "keybtn4_nobc_a", "keybtn4_nobc_b", "keybtn4_k1", "keybtn5", "keybtn7", "keybtn8", "keybtn11", "keybtn12", "all"])
    args = ap.parse_args()
    modes = ["keyprobe", "keybtn2", "keybtn3", "keybtn4", "keybtn4_nobc_a", "keybtn4_nobc_b", "keybtn4_k1", "keybtn5", "keybtn7", "keybtn8"] if args.mode == "all" else [args.mode]
    out("X3-C 物理按键方案构建（KEY=0x%04X）" % KEY)
    orig = common.read_file(SRC)
    if hashlib.sha256(orig).hexdigest() != x3c.DEVICE["sha256"]:
        out("[FAIL] 原厂镜像 sha256 不匹配")
        return 1
    os.makedirs(OUT_DIR, exist_ok=True)
    rc = 0
    for mode in modes:
        rc |= build_one(mode, orig)
    return rc


if __name__ == "__main__":
    sys.exit(main())
