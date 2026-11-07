# -*- coding: utf-8 -*-
"""生成 AD100 Pro 临时可切换版（ID==0 -> 密钥模式）成品 bin。

输出：keypatch/out/AD100pro_V1.2_key{key:04X}_id0.bin（不覆盖 v4 的 _key{key}.bin）
语义：灯 ID 菜单选 0 -> SYNC=KEY；ID=1..99 -> 原厂 50024/(ID+1)。
      （零 UI 改动，用现有 ID 菜单当开关；v4 的 C.Fn 密钥开关版继续独立维护）

运行：
  tools\\python\\python.exe keypatch\\dev\\ad100\\make_id0.py [key] [out.log]
"""
from __future__ import annotations

import hashlib
import os
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS  # noqa: E402
from devices import ad100  # noqa: E402

OFFICIAL = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
KEY = 0xD798


def parse_key(text: str) -> int:
    return int(text, 0) if text.lower().startswith("0x") else int(text, 16)


def main():
    key = parse_key(sys.argv[1]) if len(sys.argv) > 1 else KEY
    out_path = os.path.join(ROOT, "keypatch", "out",
                            "AD100pro_V1.2_key%04X_id0.bin" % key)
    data = open(OFFICIAL, "rb").read()
    print("== 生成临时可切换版（ID==0 -> KEY，零 UI 改动） ==")
    print("原厂固件: %s  %d 字节  sha256 %s"
          % (os.path.relpath(OFFICIAL, ROOT), len(data), hashlib.sha256(data).hexdigest()))
    patched, log = ad100.apply_id0(data, key)
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, "wb") as f:
        f.write(patched)
    for e in log:
        print("补丁: 0x%08X off+0x%05X  %s" % (e["addr"], e["offset"], e["desc"]))
    print("输出: %s  %d 字节  sha256 %s"
          % (os.path.relpath(out_path, ROOT), len(patched), hashlib.sha256(patched).hexdigest()))
    assert len(patched) == len(data) == ad100.IMAGE_SIZE

    # 补丁后函数反汇编（证据）
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    off = ad100.SYNC_FN - ad100.BASE
    print("\n---- 补丁后 0x%08X（id0 版, KEY=0x%04X） ----" % (ad100.SYNC_FN, key))
    for ins in md.disasm(patched[off:off + ad100.SYNC_FN_SIZE], ad100.SYNC_FN):
        print("  0x%08X  %-11s %-8s %s"
              % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))

    print("\n== 离线验证（Unicorn：ID=0 -> KEY；ID=1/5/37/99 -> 原厂公式；原厂自证） ==")
    ok = True
    for good, msg in ad100.verify_id0(patched, key):
        print("  [%s] %s" % ("PASS" if good else "FAIL", msg))
        ok = ok and good
    print("\n== 结论: %s ==" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    if len(sys.argv) > 2:
        import contextlib
        with open(sys.argv[2], "w", encoding="utf-8", newline="\n") as f, \
                contextlib.redirect_stdout(f):
            rc = main()
        sys.exit(rc)
    sys.exit(main())
