# -*- coding: utf-8 -*-
"""AD200 Pro II 蒙版切换版：命令行生成 + 离线验证。

用法：
    python tools/build_mask.py --key 0xD4EC --verify
    python tools/build_mask.py --key 0xD4EC --image devices/ad200proii/firmware/AD200ProII_V1.05.bin
    python tools/build_mask.py --key 0xD4EC --compare examples/AD200ProII_V1.05_keyD4EC_mask.bin
"""

import argparse
import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "keypatch", "devices"))

import ad200proii_mask as dev  # noqa: E402


def parse_key(text: str) -> int:
    t = text.strip()
    if t.lower().startswith("0x"):
        t = t[2:]
    value = int(t, 16)
    if not 0 <= value <= 0xFFFF:
        raise SystemExit("密钥必须是 16 位，例如 0xD4EC")
    public = [50024 // (i + 1) for i in range(100)]
    if value in public:
        raise SystemExit("0x%04X 属于 100 个公开同步字，换一个" % value)
    return value


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--key", required=True, help="4 位十六进制，例如 0xD4EC")
    ap.add_argument("--image", default=None, help="原厂固件路径")
    ap.add_argument("--out", default=None, help="输出文件路径")
    ap.add_argument("--verify", action="store_true", help="生成后跑离线验证")
    ap.add_argument("--compare", default=None, help="与已知成品逐字节比对")
    args = ap.parse_args()

    key = parse_key(args.key)
    image = args.image or os.path.join(
        ROOT, "devices", "ad200proii", "firmware", dev.IMAGE_NAME)
    if not os.path.exists(image):
        raise SystemExit("找不到原厂固件：%s（可用 --image 指定）" % image)

    raw = open(image, "rb").read()
    dev.set_image(image)
    patched, log = dev.apply(raw, key)

    out = args.out or os.path.join(ROOT, "out", dev.DEVICE["out_name"].format(key=key))
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "wb") as f:
        f.write(patched)

    print("原厂固件 : %s (%d 字节)" % (image, len(raw)))
    print("私有密钥 : 0x%04X  ->  SYNC1=0x%02X SYNC0=0x%02X"
          % (key, key >> 8, key & 0xFF))
    print("补丁点   :")
    for i, p in enumerate(log, 1):
        print("  [%d] 0x%08X  %s -> %s  %s"
              % (i, p["addr"], p["before"][:16], p["after"][:16], p["desc"]))
    print("输出     : %s (%d 字节, sha256 %s)"
          % (out, len(patched), hashlib.sha256(patched).hexdigest()[:16] + "…"))

    rc = 0
    if args.verify:
        results = dev.verify(patched, key)
        for ok, msg in results:
            print("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
            if not ok:
                rc = 1
        print("结论: %s（%d 项）" % ("PASS" if rc == 0 else "FAIL", len(results)))

    if args.compare:
        other = open(args.compare, "rb").read()
        same = other == patched
        print("比对 %s : %s" % (args.compare, "逐字节相同" if same else "不同"))
        if not same:
            diff = [i for i in range(min(len(other), len(patched))) if other[i] != patched[i]]
            print("  差异 %d 字节，首个 0x%X" % (len(diff), diff[0] if diff else -1))
            rc = 1
    return rc


if __name__ == "__main__":
    sys.exit(main())
