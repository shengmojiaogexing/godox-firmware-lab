# -*- coding: utf-8 -*-
"""X3-S 私有密钥补丁 —— 构建脚本（keybtn 默认版 + fixed v1 历史版）。

用法（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\dev\\x3s\\keybtn_build.py
    # 输出同时写入 keypatch/dev/x3s/keybtn_build.log

成品：
    keypatch/out/X3_S_v1.25_app_keyD798.bin            <- keybtn（默认：双发 + 长按拨轮/M/C 切换）
    keypatch/out/X3_S_v1.25_app_keyD798_fixed.bin      <- v1 固定 KEY（无开关）
    keypatch/out/X3S_v1.25_app_keyD798_container.bin   <- keybtn 的 G3 容器备用件
"""
from __future__ import annotations

import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

import common  # noqa: E402
import container  # noqa: E402
import devices.x3s as x3s  # noqa: E402

KEY = 0xD798
SRC = os.path.join(ROOT, "devices", "x3s", "firmware", "X3_S_v1.25_app.bin")
OUT = os.path.join(ROOT, "keypatch", "out")
LOG = os.path.join(HERE, "keybtn_build.log")

lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def build(mode: str, fname: str, verify_fn) -> int:
    orig = common.read_file(SRC)
    patched, log = x3s.apply(orig, KEY, mode)
    dst = os.path.join(OUT, fname)
    common.write_file(dst, patched)
    out("=" * 78)
    out("模式 %-7s -> %s (%d B)" % (mode, os.path.relpath(dst, ROOT), len(patched)))
    out("=" * 78)
    for i, e in enumerate(log, 1):
        out("  [%d] 0x%08X  %s -> %s  %s"
            % (i, e["addr"], e["before"], e["after"], e["desc"]))
    diffs = [i for i in range(len(orig)) if orig[i] != patched[i]]
    out("  sha256 : %s" % hashlib.sha256(patched).hexdigest())
    out("  差异   : %d 字节" % len(diffs))
    ok_all = True
    for ok, msg in verify_fn(patched, KEY):
        out("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
        ok_all &= bool(ok)
    out("  结论: %s" % ("ALL PASS" if ok_all else "FAIL"))
    return 0 if ok_all else 1


def main() -> int:
    orig = common.read_file(SRC)
    ok_sha = hashlib.sha256(orig).hexdigest() == x3s.DEVICE["sha256"]
    out("Godox X3-S V1.25 私有密钥补丁构建（KEY=0x%04X）" % KEY)
    out("原厂镜像 : %s (%d B)" % (os.path.relpath(SRC, ROOT), len(orig)))
    out("原厂 sha256 校验: %s" % ("PASS" if ok_sha else "FAIL"))
    if not ok_sha:
        return 1
    os.makedirs(OUT, exist_ok=True)

    rc = 0
    rc |= build("keybtn", "X3_S_v1.25_app_keyD798.bin", x3s.verify_keybtn)
    # 容器备用件（G3 若走 V2.0 容器路径时使用；内嵌文件名必须含设备 ID "X3S"）
    keybtn = common.read_file(os.path.join(OUT, "X3_S_v1.25_app_keyD798.bin"))
    cname = "X3S_v1.25_app_keyD798_container.bin"
    common.write_file(os.path.join(OUT, cname), container.build(keybtn, cname))
    out("\n容器备用件: %s (%d B)" % (cname, os.path.getsize(os.path.join(OUT, cname))))
    rc |= build("fixed", "X3_S_v1.25_app_keyD798_fixed.bin", x3s.verify_fixed)

    out("\n结论: %s" % ("ALL PASS" if rc == 0 else "FAIL"))
    with open(LOG, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % os.path.relpath(LOG, ROOT))
    return rc


if __name__ == "__main__":
    sys.exit(main())
