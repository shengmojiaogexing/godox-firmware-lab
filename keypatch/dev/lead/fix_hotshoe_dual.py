# -*- coding: utf-8 -*-
"""给 keybtn5 增加"热靴引闪也走密钥双发"的修复。

问题：用户用手按 ⚡ 引闪（走 0x08017018 → send_short(0x09)，白名单双发）能引闪密钥灯；
但相机热靴引闪走的是 EXTI23 处理函数 0x080084AC → FUN_0800923C（热靴/单触点引闪流程）：
- 该函数在"普通热靴"分支里**直接写 CC2500 寄存器/FIFO 并触发**，完全绕开 send_short，
  用的是当前同步字（普通 ID 的同步字）→ 密钥模式的灯收不到；
- 另一分支用 send_short(0x19)（不在 keybtn5 的双发白名单）。

修复：把 FUN_0800923C 入口改成跳板：
  - KEY_ON == 1 时：直接调用 send_short(0x09)（走 core 双发 = 私钥帧 + 当前 ID 普通帧），
    然后按原函数约定返回（跳过原热靴流程）；
  - KEY_ON == 0 时：原样进入原函数体（0x08009240）。
这样热靴引闪在密钥模式下会双发，密钥灯和同 ID 普通灯都能收到。

产物：keypatch/out/X3_C_v1.38_app_keyD798_keybtn5_hs.bin
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
SRC = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_keybtn5.bin")
DST = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_keybtn5_hs.bin")

HS_FIRE = 0x0800923C
HS_BODY = 0x08009240
SEND_SHORT = 0x080118B0
TRAMP = 0x080EF800
KEY_ON = 0x20001080


def _hw_mov_imm(imm16: int, rd: int, top: bool) -> bytes:
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    hw1 = (0xF2C0 if top else 0xF240) | (i << 10) | imm4
    hw2 = (imm3 << 12) | (rd << 8) | imm8
    return struct.pack("<HH", hw1, hw2)


def encode_bl(addr: int, target: int) -> bytes:
    imm = (target - (addr + 4)) & 0x1FFFFFF
    s = (imm >> 24) & 1
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    j1 = 0 if (i1 ^ s) else 1
    j2 = 0 if (i2 ^ s) else 1
    hw1 = 0xF000 | (s << 10) | ((imm >> 12) & 0x3FF)
    hw2 = 0xC000 | (j1 << 13) | (1 << 12) | (j2 << 11) | ((imm >> 1) & 0x7FF)
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
    print("源:", SRC)

    # 原入口必须是 push.w {r4,r5,r6,r7,r8,lr} = 2d e9 f0 41
    o = HS_FIRE - BASE
    if bytes(data[o:o + 4]) != bytes.fromhex("2de9f041"):
        print("0x%08X 原字节不符: %s" % (HS_FIRE, bytes(data[o:o + 4]).hex(" ")))
        return 1

    t = TRAMP - BASE
    if bytes(data[t:t + 64]) != b"\xff" * 64:
        print("跳板区不是 0xFF")
        return 1

    code = bytearray()
    code += bytes.fromhex("2de9f041")                      # push.w {r4-r8,lr}（复刻原序言）
    code += _hw_mov_imm(KEY_ON & 0xFFFF, 0, False)         # movw r0,#0x1080
    code += _hw_mov_imm(KEY_ON >> 16, 0, True)             # movt r0,#0x2000
    code += bytes.fromhex("0078")                          # ldrb r0,[r0]
    code += bytes.fromhex("0028")                          # cmp r0,#0
    # beq -> 原函数体（位置在 pop 之后）
    beq_at = TRAMP + len(code)
    code += b"\x00\x00"                                    # 占位
    code += bytes.fromhex("0920")                          # movs r0,#9
    code += encode_bl(TRAMP + len(code), SEND_SHORT)       # bl send_short(0x09)
    code += bytes.fromhex("bde8f081")                      # pop.w {r4-r8,pc}
    body_branch_at = TRAMP + len(code)
    code += encode_bw(body_branch_at, HS_BODY)             # b.w 原函数体
    # 回填 beq（16 位条件跳转：0xD000 | cond<<8 | imm8，cond=0 → beq）
    off = body_branch_at - (beq_at + 4)
    imm8 = (off >> 1) & 0xFF
    hw = 0xD000 | imm8
    code[(beq_at - TRAMP):(beq_at - TRAMP) + 2] = struct.pack("<H", hw)

    data[t:t + len(code)] = code
    # 入口改跳板
    data[o:o + 4] = encode_bw(HS_FIRE, TRAMP)

    out = bytes(data)
    common.write_file(DST, out)
    print("跳板 @0x%08X (%d B): %s" % (TRAMP, len(code), code.hex(" ")))
    print("入口 0x%08X: %s" % (HS_FIRE, bytes(data[o:o + 4]).hex(" ")))
    print("成品:", DST)
    print("长度:", len(out), "sha256:", hashlib.sha256(out).hexdigest())
    return 0


if __name__ == "__main__":
    sys.exit(main())
