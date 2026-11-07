# -*- coding: utf-8 -*-
"""把 touchprobe 的"动作"从 ui_key_toggle 改成"发一次 TEST 引闪帧"，
这样用户手里的**原厂灯**就能直接看到反馈（摸屏幕 → 灯闪一下）。

原理：
  touchprobe 的 poll（页面 0 的 wrapper，每 100ms 一次）在检测到触摸坐标变化时
  调用 0x080F0000（ui_key_toggle）。把这条 bl 改成 bl stub，stub 在 0x080F2000：
      movs r0, #9          ; send_short(9) = TEST
      b.w  0x080118B0      ; 原厂 send_short（单发，用当前信道的公共同步字）
  触摸时就会发一帧 TEST，旁边的原厂灯闪一下。

产物：keypatch/out/X3_C_v1.38_app_keyD798_touchprobe_test.bin
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
KEYPATCH = os.path.join(ROOT, "keypatch")
if KEYPATCH not in sys.path:
    sys.path.insert(0, KEYPATCH)

import common  # noqa: E402

BASE = 0x08004800
SRC = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_touchprobe.bin")
DST = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_touchprobe_test.bin")

CALL_SITE = 0x080F104E          # bl 0x080F0000（ui_key_toggle）
STUB = 0x080F2000
SEND_SHORT = 0x080118B0


def encode_bl(addr: int, target: int) -> bytes:
    off = target - (addr + 4)
    imm = off & 0x1FFFFFF
    s = (imm >> 24) & 1
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    j1 = 0 if (i1 ^ s) else 1
    j2 = 0 if (i2 ^ s) else 1
    imm10 = (imm >> 12) & 0x3FF
    imm11 = (imm >> 1) & 0x7FF
    hw1 = 0xF000 | (s << 10) | imm10
    hw2 = 0xC000 | (j1 << 13) | (1 << 12) | (j2 << 11) | imm11
    return struct.pack("<HH", hw1, hw2)


def encode_bw(addr: int, target: int) -> bytes:
    off = target - (addr + 4)
    imm = off & 0x1FFFFFF
    s = (imm >> 24) & 1
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    j1 = 0 if (i1 ^ s) else 1
    j2 = 0 if (i2 ^ s) else 1
    imm10 = (imm >> 12) & 0x3FF
    imm11 = (imm >> 1) & 0x7FF
    hw1 = 0xF000 | (s << 10) | imm10
    hw2 = 0x8000 | (j1 << 13) | (1 << 12) | (j2 << 11) | imm11
    return struct.pack("<HH", hw1, hw2)


def main() -> int:
    data = bytearray(common.read_file(SRC))
    print("源:", SRC)
    # 1) stub 区必须是 0xFF
    so = STUB - BASE
    if bytes(data[so:so + 8]) != b"\xff" * 8:
        print("stub 区不是 0xFF:", data[so:so + 8].hex(" "))
        return 1
    stub = bytearray()
    stub += bytes.fromhex("0920")                       # movs r0,#9
    stub += encode_bw(STUB + len(stub), SEND_SHORT)     # b.w send_short
    data[so:so + len(stub)] = stub
    print("stub @0x%08X: %s" % (STUB, stub.hex(" ")))

    # 2) 调用点：bl 0x080F0000 -> bl stub
    co = CALL_SITE - BASE
    old = bytes(data[co:co + 4]).hex(" ")
    data[co:co + 4] = encode_bl(CALL_SITE, STUB)
    print("0x%08X: %s -> %s" % (CALL_SITE, old, bytes(data[co:co + 4]).hex(" ")))

    out = bytes(data)
    common.write_file(DST, out)
    print("成品:", DST)
    print("长度:", len(out), "sha256:", hashlib.sha256(out).hexdigest())
    return 0


if __name__ == "__main__":
    sys.exit(main())
