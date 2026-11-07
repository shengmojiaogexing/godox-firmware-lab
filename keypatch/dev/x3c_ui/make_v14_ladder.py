# -*- coding: utf-8 -*-
"""生成 v1.4 三档安全阶梯 bin + sha256（给实机 A/B 测试用）。"""
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
    # (文件名, opts, 说明)
    ("X3_C_v1.38_app_ui_A_noTiles.bin",
     {"enable_menu_patch": True, "tile_mode": "none"},
     "A 诊断档：只装轮询+跳板，不建磁贴（验证是否安全）"),
    ("X3_C_v1.38_app_ui_B_minimal.bin",
     {"enable_menu_patch": True, "tile_mode": "minimal"},
     "B 最小档：只建磁贴+设文本（无属性序列/图标）"),
    ("X3_C_v1.38_app_ui_C_full.bin",
     {"enable_menu_patch": True, "tile_mode": "full"},
     "C 完整档：原厂完整属性序列（无图标子控件）"),
]


def sha(b):
    return hashlib.sha256(b).hexdigest()


def main() -> int:
    orig = open(U.IMAGE, "rb").read()
    lines = ["X3-C v1.38 UI 补丁 v1.4 —— 安全阶梯三档（磁贴创建挪到页 1 尾部）", "=" * 78]
    for name, opts, desc in CASES:
        patched, log = U.apply_ui(orig, opts)
        res = U.verify_ui(patched, opts)
        ok = all(o for o, _ in res)
        path = os.path.join(OUTL, name)
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
            if ("磁贴" in m) or ("tile_mode" in m) or ("桩" in m):
                lines.append("   [%s] %s" % ("PASS" if o else "FAIL", m))
    # 逐组备选（v1.4 full + per_group）也出一版
    opts_pg = {"enable_menu_patch": True, "tile_mode": "full",
               "broadcast_per_group": True, "key": 0xD798}
    patched, log = U.apply_ui(orig, opts_pg)
    res = U.verify_ui(patched, opts_pg)
    name = "X3_C_v1.38_app_keyD798_pergroup.bin"
    path = os.path.join(OUTK, name)
    with open(path, "wb") as f:
        f.write(patched)
    lines += ["", "文件   : %s" % path, "说明   : 逐组 A–E 广播备选（v1.4 full，KEY=0xD798）",
              "长度   : %d" % len(patched), "sha256 : %s" % sha(patched),
              "验证   : %s（%d/%d）" % ("PASS" if all(o for o, _ in res) else "FAIL",
                                        sum(1 for o, _ in res if o), len(res))]
    txt = os.path.join(OUTL, "v14_sha256.txt")
    io.open(txt, "w", encoding="utf-8").write("\n".join(lines) + "\n")
    print("\n".join(lines))
    print("\n清单:", txt)
    return 0


if __name__ == "__main__":
    sys.exit(main())
