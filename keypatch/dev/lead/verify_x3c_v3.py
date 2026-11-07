# -*- coding: utf-8 -*-
"""Lead 独立验证：X3-C v3 双发（KEY_ON_FLAG 控制 + 按当前 ID 发普通帧）。

不导入 devices/x3c.py，直接用 Unicorn 执行补丁后的 send_short / send_property。
注意：KEY_ON_FLAG 目前是 fw-x3c 的占位地址 0x20000145；ui-x3c 定最终地址后改这里。
"""
from __future__ import annotations

import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tx_proto import TxHarness, IMAGE, BASE, SEND_SHORT, SEND_PROPERTY  # noqa: E402

KEY = 0xD798  # 与 out/X3_C_v1.38_app_keyD798.bin 一致

ID_RAM = 0x20000055
ID_OFF_FLAG_RAM = 0x20000143
TX_GATE_RAM = 0x20000146
KEY_ON_FLAG_RAM = 0x20000145      # TODO: 等 ui-x3c 最终地址


def sync_of(syn):
    d = dict(syn)
    return (d.get(4, -1) << 8) | d.get(5, -1)


def check(h, func, args, ident, id_off, key_on, label):
    h.uc.mem_write(ID_RAM, bytes([ident & 0xFF]))
    h.uc.mem_write(ID_OFF_FLAG_RAM, bytes([id_off & 0xFF]))
    h.uc.mem_write(KEY_ON_FLAG_RAM, bytes([key_on & 0xFF]))
    h.uc.mem_write(TX_GATE_RAM, b"\x00")
    h.clear()
    h.call(func, args)
    frames = [f["data"] for f in h.frames]
    syn = [(r, v) for (r, v) in h.regs if r in (4, 5)]
    dual = bool(key_on) or (ident == 99 and id_off == 0)
    pub = 50024 // (ident + 1) if dual else None
    if dual:
        want_syn = [(4, KEY >> 8), (5, KEY & 0xFF), (4, pub >> 8), (5, pub & 0xFF)]
        want_frames = 2
    else:
        want_syn = []
        want_frames = 1
    ok = (syn == want_syn and len(frames) == want_frames)
    print("  [%s] %-38s ID=%-3d id_off=%d key_on=%d frames=%d syn=%s%s"
          % ("PASS" if ok else "FAIL", label, ident, id_off, key_on, len(frames),
             [(hex(a), hex(b)) for a, b in syn],
             ("  (pub=0x%04X)" % pub) if dual else ""))
    if not ok:
        print("        frames=%s" % [d.hex(" ") for d in frames])
    return ok


def main() -> int:
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                        "..", "..", "out", "X3_C_v1.38_app_keyD798.bin")
    patched = open(os.path.abspath(path), "rb").read()
    print("补丁镜像:", os.path.abspath(path), len(patched))
    h = TxHarness()
    h.uc.mem_write(BASE, patched)
    ok = True
    print("-" * 82)
    print("send_short(0x09)：")
    ok &= check(h, SEND_SHORT, (0x09,), 5, 0, 1, "KEY_ON=1, ID=5 -> KEY + 0x2091 双发")
    ok &= check(h, SEND_SHORT, (0x09,), 0, 0, 1, "KEY_ON=1, ID=0 -> KEY + 0xC368 双发")
    ok &= check(h, SEND_SHORT, (0x09,), 5, 0, 0, "KEY_ON=0, ID=5 -> 单发")
    ok &= check(h, SEND_SHORT, (0x09,), 99, 0, 0, "KEY_ON=0, ID=99 -> KEY + 0x01F4 双发")
    ok &= check(h, SEND_SHORT, (0x09,), 99, 1, 0, "KEY_ON=0, ID=99, id_off=1 -> 单发")
    print("send_property(0x50,0xB1,0x00)：")
    ok &= check(h, SEND_PROPERTY, (0x50, 0xB1, 0x00), 5, 0, 1, "KEY_ON=1, ID=5 -> 双发")
    ok &= check(h, SEND_PROPERTY, (0x50, 0xB1, 0x00), 5, 0, 0, "KEY_ON=0, ID=5 -> 单发")
    print("-" * 82)
    print("结论:", "ALL PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
