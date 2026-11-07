# -*- coding: utf-8 -*-
"""v1.5 成品：主成品(ALL) + A/B/C 三档 + 逐组备选，全部用"调用点 wrapper"钩子。"""
import hashlib
import io
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, HERE)
import ui_patch as U  # noqa: E402

OUTL = os.path.join(HERE, "out")
OUTK = os.path.join(ROOT, "keypatch", "out")

CASES = [
    ("X3_C_v1.38_app_patch.bin", os.path.join(OUTL, "X3_C_v1.38_app_patch.bin"),
     {"enable_menu_patch": True, "tile_mode": "full"},
     "主成品（ALL 组，磁贴完整属性序列）"),
    ("X3_C_v1.38_app_ui_A_noTiles.bin", os.path.join(OUTL, "X3_C_v1.38_app_ui_A_noTiles.bin"),
     {"enable_menu_patch": True, "tile_mode": "none"},
     "A 诊断档：修好的钩子 + 轮询，不建控件"),
    ("X3_C_v1.38_app_ui_B_minimal.bin", os.path.join(OUTL, "X3_C_v1.38_app_ui_B_minimal.bin"),
     {"enable_menu_patch": True, "tile_mode": "minimal"},
     "B 最小档：建磁贴+文本"),
    ("X3_C_v1.38_app_ui_C_full.bin", os.path.join(OUTL, "X3_C_v1.38_app_ui_C_full.bin"),
     {"enable_menu_patch": True, "tile_mode": "full"},
     "C 完整档：磁贴+原厂完整属性序列（=主成品）"),
    ("X3_C_v1.38_app_keyD798_pergroup.bin", os.path.join(OUTK, "X3_C_v1.38_app_keyD798_pergroup.bin"),
     {"enable_menu_patch": True, "tile_mode": "full", "broadcast_per_group": True,
      "key": 0xD798},
     "逐组 A–E 广播备选（KEY=0xD798）"),
]


def sha(b):
    return hashlib.sha256(b).hexdigest()


def main() -> int:
    orig = open(U.IMAGE, "rb").read()
    lines = ["X3-C v1.38 UI 补丁 v1.5（页 1 钩子改为调用点 wrapper）", "=" * 78]
    for _n, path, opts, desc in CASES:
        patched, log = U.apply_ui(orig, opts)
        res = U.verify_ui(patched, opts)
        ok = all(o for o, _ in res)
        with open(path, "wb") as f:
            f.write(patched)
        diffs = sum(1 for a, b in zip(orig, patched) if a != b)
        lines += ["", "文件   : %s" % path, "说明   : %s" % desc,
                  "长度   : %d（原厂 1021468，不变=%s）" % (len(patched), len(patched) == U.IMAGE_SIZE),
                  "差异   : %d 字节" % diffs,
                  "sha256 : %s" % sha(patched),
                  "验证   : %s（%d/%d）" % ("PASS" if ok else "FAIL",
                                            sum(1 for o, _ in res if o), len(res))]
        for o, m in res:
            if ("wrapper" in m) or ("0x0800FE78" in m) or ("tile_mode" in m) or ("轮询桩" in m):
                lines.append("   [%s] %s" % ("PASS" if o else "FAIL", m))
    txt = os.path.join(OUTL, "v15_sha256.txt")
    io.open(txt, "w", encoding="utf-8").write("\n".join(lines) + "\n")
    print("\n".join(lines))
    print("\n清单:", txt)
    return 0


if __name__ == "__main__":
    sys.exit(main())
