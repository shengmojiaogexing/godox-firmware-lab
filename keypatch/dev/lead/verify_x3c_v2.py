# -*- coding: utf-8 -*-
"""Lead 独立验证：X3-C v2 双发补丁（不依赖 devices/x3c.py 的 verify）。

直接用 Unicorn 执行补丁后的 send_short / send_property，检查：
  * ID=99（ID-OFF 标志=0）时：先写 KEY 同步字发一次，再写 ID99 普通同步字再发一次；
  * ID=5/0 时：只发一次。
"""
from __future__ import annotations

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tx_proto import TxHarness, IMAGE, BASE, KEY, SEND_SHORT, SEND_PROPERTY  # noqa: E402

ID_RAM = 0x20000055
ID_OFF_FLAG_RAM = 0x20000143
TX_GATE_RAM = 0x20000146
ID99_SYNC = 50024 // 100


def check(h: TxHarness, func: int, args, ident: int, id_off: int, label: str):
    h.uc.mem_write(ID_RAM, bytes([ident & 0xFF]))
    h.uc.mem_write(ID_OFF_FLAG_RAM, bytes([id_off & 0xFF]))
    h.uc.mem_write(TX_GATE_RAM, b"\x00")
    h.clear()
    h.call(func, args)
    frames = [f["data"] for f in h.frames]
    syn = [(r, v) for (r, v) in h.regs if r in (4, 5)]
    key_mode = (ident == 99 and id_off == 0)
    if key_mode:
        want_syn = [(4, KEY >> 8), (5, KEY & 0xFF), (4, ID99_SYNC >> 8), (5, ID99_SYNC & 0xFF)]
        want_frames = 2
    else:
        want_syn = []
        want_frames = 1
    ok = (syn == want_syn and len(frames) == want_frames)
    print("  [%s] %-42s ID=%-3d id_off=%d frames=%d syn=%s"
          % ("PASS" if ok else "FAIL", label, ident, id_off, len(frames),
             [(hex(a), hex(b)) for a, b in syn]))
    if not ok:
        print("        frames=%s" % [d.hex(" ") for d in frames])
    return ok


def main() -> int:
    patched_path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                "..", "..", "out", "X3_C_v1.38_app_key6B21.bin")
    patched = open(os.path.abspath(patched_path), "rb").read()
    orig = open(IMAGE, "rb").read()
    print("补丁镜像:", os.path.abspath(patched_path), len(patched))
    print("长度一致:", len(patched) == len(orig))
    h = TxHarness()
    h.uc.mem_write(BASE, patched)
    ok = True
    print("-" * 78)
    print("send_short(0x09)：")
    ok &= check(h, SEND_SHORT, (0x09,), 99, 0, "密钥模式(ID=99, id_off=0)")
    ok &= check(h, SEND_SHORT, (0x09,), 5, 0, "普通模式(ID=5)")
    ok &= check(h, SEND_SHORT, (0x09,), 0, 1, "普通模式(ID=0, id_off=1)")
    print("send_property(0x50,0xB1,0x00)：")
    ok &= check(h, SEND_PROPERTY, (0x50, 0xB1, 0x00), 99, 0, "密钥模式(ID=99, id_off=0)")
    ok &= check(h, SEND_PROPERTY, (0x50, 0xB1, 0x00), 5, 0, "普通模式(ID=5)")
    print("-" * 78)
    print("结论:", "ALL PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
