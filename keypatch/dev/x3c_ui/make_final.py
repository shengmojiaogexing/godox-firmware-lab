# -*- coding: utf-8 -*-
"""生成两个成品 + sha256 + 验证摘要。

  1) keypatch/dev/x3c_ui/out/X3_C_v1.38_app_patch.bin            —— v1.2 主成品（ALL 组）
  2) keypatch/out/X3_C_v1.38_app_keyD798_pergroup.bin            —— 备选：A–E 逐组（KEY=0xD798）

用法：tools\\python\\python.exe make_final.py
"""
from __future__ import annotations

import hashlib
import io
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, HERE)

import ui_patch as U  # noqa: E402

OUT_LOCAL = os.path.join(HERE, "out")
OUT_KEYPATCH = os.path.join(ROOT, "keypatch", "out")

PERGROUP_KEY = 0xD798      # 与文件名 keyD798 对应


def build(opts: dict) -> tuple[bytes, list, list]:
    orig = open(U.IMAGE, "rb").read()
    patched, log = U.apply_ui(orig, opts)
    results = U.verify_ui(patched, opts)
    return patched, log, results


def summarize(name: str, path: str, patched: bytes, results: list) -> dict:
    sha = hashlib.sha256(patched).hexdigest()
    ok = all(o for o, _ in results)
    return {"name": name, "path": path, "sha256": sha, "len": len(patched), "ok": ok,
            "pass": sum(1 for o, _ in results if o), "total": len(results),
            "results": results}


def main() -> int:
    os.makedirs(OUT_LOCAL, exist_ok=True)
    os.makedirs(OUT_KEYPATCH, exist_ok=True)

    # --- 主成品（ALL 组，保持与原 v1.2 相同语义）
    opts_main = {"enable_menu_patch": True, "broadcast_per_group": False,
                 "three_tab_layout": False}
    p1, log1, r1 = build(opts_main)
    path1 = os.path.join(OUT_LOCAL, "X3_C_v1.38_app_patch.bin")
    with open(path1, "wb") as f:
        f.write(p1)
    s1 = summarize("v1.2 ALL-group", path1, p1, r1)

    # --- 备选成品（A–E 逐组，KEY=0xD798）
    opts_pg = {"enable_menu_patch": True, "broadcast_per_group": True,
               "three_tab_layout": False, "key": PERGROUP_KEY}
    p2, log2, r2 = build(opts_pg)
    path2 = os.path.join(OUT_KEYPATCH, "X3_C_v1.38_app_keyD798_pergroup.bin")
    with open(path2, "wb") as f:
        f.write(p2)
    s2 = summarize("v1.2 per-group (A-E), KEY=0xD798", path2, p2, r2)

    lines = ["X3-C v1.38 UI 原型补丁 —— 成品清单", "=" * 72]
    for s in (s1, s2):
        lines.append("")
        lines.append("名称   : %s" % s["name"])
        lines.append("路径   : %s" % s["path"])
        lines.append("长度   : %d（原厂 %d，长度不变=%s）"
                     % (s["len"], U.IMAGE_SIZE, s["len"] == U.IMAGE_SIZE))
        lines.append("sha256 : %s" % s["sha256"])
        lines.append("验证   : %s（%d/%d PASS）"
                     % ("PASS" if s["ok"] else "FAIL", s["pass"], s["total"]))
        for ok, msg in s["results"]:
            if ("broadcast" in msg) or ("全部" in msg) or ("per" in msg):
                lines.append("   [%s] %s" % ("PASS" if ok else "FAIL", msg))

    txt = os.path.join(OUT_LOCAL, "final_sha256.txt")
    io.open(txt, "w", encoding="utf-8").write("\n".join(lines) + "\n")
    print("\n".join(lines))
    print("\n清单:", txt)
    return 0 if (s1["ok"] and s2["ok"]) else 1


if __name__ == "__main__":
    sys.exit(main())
