# -*- coding: utf-8 -*-
"""生成 page-0 版成品：A2-p0 / C-p0 / 主成品 / 逐组备选，并写 sha 清单。"""
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
    ("X3_C_v1.38_app_ui_A2p0_fallback.bin", os.path.join(OUTL, "X3_C_v1.38_app_ui_A2p0_fallback.bin"),
     {"enable_menu_patch": True, "entry_mode": "fallback_p0"},
     "A2-p0：page0 调用点 wrapper + 长按轮询（不建控件）"),
    ("X3_C_v1.38_app_patch.bin", os.path.join(OUTL, "X3_C_v1.38_app_patch.bin"),
     {"enable_menu_patch": True, "entry_mode": "tiles_p0"},
     "C-p0 / 主成品：A2-p0 + 磁贴（builder 后追加，父窗口=句柄表[0]）"),
    ("X3_C_v1.38_app_ui_Cp0_full.bin", os.path.join(OUTL, "X3_C_v1.38_app_ui_Cp0_full.bin"),
     {"enable_menu_patch": True, "entry_mode": "tiles_p0"},
     "C-p0（同上，另存名）"),
    ("X3_C_v1.38_app_keyD798_pergroup.bin", os.path.join(OUTK, "X3_C_v1.38_app_keyD798_pergroup.bin"),
     {"enable_menu_patch": True, "entry_mode": "tiles_p0",
      "broadcast_per_group": True, "key": 0xD798},
     "C-p0 + A–E 逐组广播（KEY=0xD798）"),
    ("X3_C_v1.38_app_keyD798_A2p0.bin", os.path.join(OUTK, "X3_C_v1.38_app_keyD798_A2p0.bin"),
     {"enable_menu_patch": True, "entry_mode": "fallback_p0",
      "broadcast_per_group": True, "key": 0xD798},
     "A2-p0 + 逐组广播（KEY=0xD798）"),
]


def sha(b):
    return hashlib.sha256(b).hexdigest()


def main() -> int:
    orig = open(U.IMAGE, "rb").read()
    lines = ["X3-C v1.38 UI 补丁 —— page 0（主界面）版", "=" * 78]
    for _n, path, opts, desc in CASES:
        patched, log = U.apply_ui(orig, opts)
        res = U.verify_ui(patched, opts)
        ok = all(o for o, _ in res)
        with open(path, "wb") as f:
            f.write(patched)
        diffs = sum(1 for a, b in zip(orig, patched) if a != b)
        lines += ["", "文件   : %s" % path, "说明   : %s" % desc,
                  "长度   : %d（原厂 1021468，不变=%s）"
                  % (len(patched), len(patched) == U.IMAGE_SIZE),
                  "差异   : %d 字节" % diffs,
                  "sha256 : %s" % sha(patched),
                  "验证   : %s（%d/%d）" % ("PASS" if ok else "FAIL",
                                            sum(1 for o, _ in res if o), len(res))]
        for o, m in res:
            if ("page0" in m) or ("wrapper" in m) or ("tiles0" in m) or ("fallback poll" in m):
                lines.append("   [%s] %s" % ("PASS" if o else "FAIL", m[:200]))
    txt = os.path.join(OUTL, "v16_p0_sha256.txt")
    io.open(txt, "w", encoding="utf-8").write("\n".join(lines) + "\n")
    print("\n".join(lines))
    print("\n清单:", txt)
    return 0


if __name__ == "__main__":
    sys.exit(main())
