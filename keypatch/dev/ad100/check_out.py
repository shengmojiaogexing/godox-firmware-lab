# -*- coding: utf-8 -*-
"""独立核对 v4 补丁输出 bin：长度不变、差异字节只在补丁区、原厂固件未被改动。

运行：
  tools\\python\\python.exe keypatch\\dev\\ad100\\check_out.py [key]
  key 默认 0xD798（十进制/0x 前缀均可）
"""
import hashlib
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

from devices import ad100  # noqa: E402

OFFICIAL = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")


def parse_key(text: str) -> int:
    return int(text, 0) if text.lower().startswith("0x") else int(text, 16)


KEY = parse_key(sys.argv[1]) if len(sys.argv) > 1 else 0xD798
OUT = os.path.join(ROOT, "keypatch", "out", ad100.DEVICE["out_name"].format(key=KEY))


def main():
    a = open(OFFICIAL, "rb").read()
    b = open(OUT, "rb").read()
    print("原厂 : %s  %d 字节  sha256 %s" % (os.path.relpath(OFFICIAL, ROOT), len(a),
                                            hashlib.sha256(a).hexdigest()))
    print("补丁 : %s  %d 字节  sha256 %s" % (os.path.relpath(OUT, ROOT), len(b),
                                            hashlib.sha256(b).hexdigest()))
    ok = True
    ok &= len(a) == len(b) == ad100.IMAGE_SIZE
    print("[%s] 长度一致: %d == %d" % ("PASS" if len(a) == len(b) == ad100.IMAGE_SIZE else "FAIL",
                                       len(a), len(b)))

    diffs = [i for i in range(min(len(a), len(b))) if a[i] != b[i]]
    print("差异字节: %d 个，范围 0x%05X..0x%05X"
          % (len(diffs), diffs[0] if diffs else -1, diffs[-1] if diffs else -1))
    off = ad100.SYNC_FN - ad100.BASE
    in_region = all(off <= d < off + ad100.SYNC_FN_SIZE for d in diffs)
    ok &= in_region
    print("[%s] 全部差异落在补丁区 0x%05X..0x%05X（56 字节整函数改写）"
          % ("PASS" if in_region else "FAIL", off, off + ad100.SYNC_FN_SIZE - 1))

    # 补丁区应与模块生成的期望函数体逐字节一致
    want = ad100.build_sync_function(KEY)
    got = b[off:off + ad100.SYNC_FN_SIZE]
    ok &= got == want
    print("[%s] 补丁区与期望函数体一致（KEY=0x%04X）" % ("PASS" if got == want else "FAIL", KEY))
    print("     期望: %s" % want.hex(" "))
    print("     实际: %s" % got.hex(" "))

    # 下一函数起点应保持原厂
    nxt = ad100.SYNC_FN_END - ad100.BASE
    ok &= b[nxt:nxt + 8] == a[nxt:nxt + 8]
    print("[%s] 下一函数 0x%08X 起点字节未被改动: %s"
          % ("PASS" if b[nxt:nxt + 8] == a[nxt:nxt + 8] else "FAIL",
             ad100.SYNC_FN_END, b[nxt:nxt + 8].hex(" ")))

    # 原厂文件未被改动
    ok &= hashlib.sha256(a).hexdigest() == ad100.DEVICE["sha256"]
    print("[%s] 原厂固件 sha256 与 DEVICE 记录一致（未被改动）"
          % ("PASS" if hashlib.sha256(a).hexdigest() == ad100.DEVICE["sha256"] else "FAIL"))
    print("\n结论:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
