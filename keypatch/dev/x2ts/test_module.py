# -*- coding: utf-8 -*-
"""X2T-S 模块自测：apply + verify 全量输出（Lead 验证）。"""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

import devices.x2ts as mod  # noqa: E402


def main():
    data = open(os.path.join(ROOT, mod.DEVICE["image"]), "rb").read()
    print("image size:", len(data), "sha256 matches:",
          __import__("hashlib").sha256(data).hexdigest() == mod.DEVICE["sha256"])
    key = 0xD798
    patched, log = mod.apply(data, key)
    print("patched size:", len(patched))
    for e in log:
        print("  patch @0x%08X (off 0x%05X): %s" % (e["addr"], e["offset"], e["desc"]))
    print()
    results = mod.verify(patched, key)
    bad = 0
    for ok, msg in results:
        print(("  PASS " if ok else "  FAIL ") + msg)
        if not ok:
            bad += 1
    print()
    print("总计 %d 项，失败 %d 项" % (len(results), bad))
    out = os.path.join(ROOT, "keypatch", "out")
    os.makedirs(out, exist_ok=True)
    name = mod.DEVICE["out_name"].format(key=key)
    open(os.path.join(out, name), "wb").write(patched)
    print("written:", os.path.join(out, name))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
