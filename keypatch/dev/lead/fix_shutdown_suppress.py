# -*- coding: utf-8 -*-
"""keybtn13：广播期间屏蔽关机（防止全 ID 广播导致意外关机）。

做法：在关机入口 FUN_08035FD0(0x08035FD0) 加跳板：
  若广播进行中（BTN7_ACTIVE = 0x200010AA != 0）→ 直接返回，忽略关机请求；
  否则按原厂流程执行（先复刻原函数序言 push {r4,r5,r6,lr}，再跳回 0x08035FD2）。

产物：keypatch/out/X3_C_v1.38_app_keyD798_keybtn13.bin
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
SRC = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_keybtn11.bin")
DST = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_keybtn14.bin")
SHUTDOWN = 0x08035FD0
SHUTDOWN_BODY = 0x08035FD2
SHUTDOWN2 = 0x08010C00
SHUTDOWN2_BODY = 0x08010C02
TRAMP2 = 0x080EF940
ACTIVE = 0x200010AA
TRAMP = 0x080EF900


def _hw_mov_imm(imm16: int, rd: int, top: bool) -> bytes:
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    hw1 = (0xF2C0 if top else 0xF240) | (i << 10) | imm4
    hw2 = (imm3 << 12) | (rd << 8) | imm8
    return struct.pack("<HH", hw1, hw2)


def encode_bw(addr: int, target: int) -> bytes:
    imm = (target - (addr + 4)) & 0x1FFFFFF
    s = (imm >> 24) & 1
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    j1 = 0 if (i1 ^ s) else 1
    j2 = 0 if (i2 ^ s) else 1
    hw1 = 0xF000 | (s << 10) | ((imm >> 12) & 0x3FF)
    hw2 = 0x8000 | (j1 << 13) | (1 << 12) | (j2 << 11) | ((imm >> 1) & 0x7FF)
    return struct.pack("<HH", hw1, hw2)


def main() -> int:
    data = bytearray(common.read_file(SRC))
    o = SHUTDOWN - BASE
    if bytes(data[o:o + 2]) != bytes.fromhex("70b5"):
        print("0x%08X 不是 push {r4-r6,lr}: %s" % (SHUTDOWN, bytes(data[o:o + 2]).hex(" ")))
        return 1
    t = TRAMP - BASE
    if bytes(data[t:t + 48]) != b"\xff" * 48:
        print("跳板区非 0xFF")
        return 1

    code = bytearray()
    code += _hw_mov_imm(ACTIVE & 0xFFFF, 0, False)   # movw r0,#0x10aa
    code += _hw_mov_imm(ACTIVE >> 16, 0, True)       # movt r0,#0x2000
    code += bytes.fromhex("0078")                    # ldrb r0,[r0]
    code += bytes.fromhex("0028")                    # cmp  r0,#0
    # bne -> ignore
    bne_at = TRAMP + len(code)
    code += b"\x00\x00"
    code += bytes.fromhex("70b5")                    # push {r4,r5,r6,lr}
    code += encode_bw(TRAMP + len(code), SHUTDOWN_BODY)
    ignore_at = TRAMP + len(code)
    code += bytes.fromhex("7047")                    # bx lr
    if TRAMP + len(code) - TRAMP > 0x20:
        print("tramp too long")
        return 1
    # 回填 bne（16 位条件跳转，cond=NE -> 0xD100 | imm8）
    off = ignore_at - (bne_at + 4)
    hw = 0xD100 | ((off >> 1) & 0xFF)
    code[(bne_at - TRAMP):(bne_at - TRAMP) + 2] = struct.pack("<H", hw)

    data[t:t + len(code)] = code
    data[o:o + 4] = encode_bw(SHUTDOWN, TRAMP)

    # 第二处：关机本体入口，同样在广播期间忽略
    o2 = SHUTDOWN2 - BASE
    if bytes(data[o2:o2 + 2]) != bytes.fromhex("70b5"):
        print("0x%08X 不是 push: %s" % (SHUTDOWN2, bytes(data[o2:o2 + 2]).hex(" ")))
        return 1
    t2 = TRAMP2 - BASE
    if bytes(data[t2:t2 + 32]) != b"\xff" * 32:
        print("第二跳板区非 0xFF")
        return 1
    code2 = bytearray()
    code2 += _hw_mov_imm(ACTIVE & 0xFFFF, 0, False)
    code2 += _hw_mov_imm(ACTIVE >> 16, 0, True)
    code2 += bytes.fromhex("0078")
    code2 += bytes.fromhex("0028")
    bne_at2 = TRAMP2 + len(code2)
    code2 += b"\x00\x00"
    code2 += bytes.fromhex("70b5")
    code2 += encode_bw(TRAMP2 + len(code2), SHUTDOWN2_BODY)
    ignore_at2 = TRAMP2 + len(code2)
    code2 += bytes.fromhex("7047")
    off2 = ignore_at2 - (bne_at2 + 4)
    hw2 = 0xD100 | ((off2 >> 1) & 0xFF)
    code2[(bne_at2 - TRAMP2):(bne_at2 - TRAMP2) + 2] = struct.pack("<H", hw2)
    data[t2:t2 + len(code2)] = code2
    data[o2:o2 + 4] = encode_bw(SHUTDOWN2, TRAMP2)

    out = bytes(data)
    common.write_file(DST, out)
    print("跳板 @0x%08X: %s" % (TRAMP, code.hex(" ")))
    print("关机入口 0x%08X: %s" % (SHUTDOWN, bytes(data[o:o + 4]).hex(" ")))
    print("成品:", DST)
    print("sha256:", hashlib.sha256(out).hexdigest())
    return 0


if __name__ == "__main__":
    sys.exit(main())
