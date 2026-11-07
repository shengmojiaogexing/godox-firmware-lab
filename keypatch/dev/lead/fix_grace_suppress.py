# -*- coding: utf-8 -*-
"""keybtn18：广播后 15 秒"关机宽限"——任何软件关机调用都被忽略。

背景：全 ID 广播结束后约 2-4 秒设备仍会关机（疑似广播导致电池/电源状态被判低）。
本补丁在两个关机入口（0x08035FD0 / 0x08010C00）加跳板：
  - 广播进行中（BTN7_ACTIVE != 0）→ 忽略关机；
  - 广播结束后 15 秒内（tick - BTN7_LAST_TICK < 15000 ms）→ 忽略关机；
  - 其它情况 → 原厂流程。

产物：keypatch/out/X3_C_v1.38_app_keyD798_keybtn18.bin
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
DST = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_keybtn18.bin")
TICK = 0x20000310
ACTIVE = 0x200010AA
LAST = 0x200010B0
GRACE_MS = 15000


def mov_imm(imm16: int, rd: int, top: bool) -> bytes:
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    hw1 = (0xF2C0 if top else 0xF240) | (i << 10) | imm4
    hw2 = (imm3 << 12) | (rd << 8) | imm8
    return struct.pack("<HH", hw1, hw2)


def addr(rd: int, va: int) -> bytes:
    return mov_imm(va & 0xFFFF, rd, False) + mov_imm(va >> 16, rd, True)


def bw(at: int, target: int) -> bytes:
    imm = (target - (at + 4)) & 0x1FFFFFF
    s = (imm >> 24) & 1
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    j1 = 0 if (i1 ^ s) else 1
    j2 = 0 if (i2 ^ s) else 1
    hw1 = 0xF000 | (s << 10) | ((imm >> 12) & 0x3FF)
    hw2 = 0x8000 | (j1 << 13) | (1 << 12) | (j2 << 11) | ((imm >> 1) & 0x7FF)
    return struct.pack("<HH", hw1, hw2)


def bcc16(off: int) -> bytes:
    """16 位 BCC（无符号小于跳转）"""
    return struct.pack("<H", 0xD300 | ((off >> 1) & 0xFF))


def bne16(off: int) -> bytes:
    return struct.pack("<H", 0xD100 | ((off >> 1) & 0xFF))


def make_tramp(t: int, body: int) -> bytes:
    c = bytearray()
    # 广播进行中 → 忽略
    c += addr(0, ACTIVE)
    c += bytes.fromhex("0078")                  # ldrb r0,[r0]
    c += bytes.fromhex("0028")                  # cmp r0,#0
    bne_at = len(c)
    c += b"\x1f\xe0"                            # 占位
    # r0 = tick, r1 = last, r2 = delta
    c += addr(0, TICK) + bytes.fromhex("0068")  # ldr r0,[r0]
    c += addr(1, LAST) + bytes.fromhex("0968")  # ldr r1,[r1]
    c += bytes.fromhex("421a")                  # subs r2,r0,r1
    # 与 GRACE_MS 比较
    c += mov_imm(GRACE_MS & 0xFFFF, 1, False)   # movw r1,#0x3a98
    c += bytes.fromhex("8a42")                  # cmp r2,r1
    bcc_at = len(c)
    c += b"\x1f\xe0"
    # 正常：复刻原函数序言并跳回函数体
    c += bytes.fromhex("70b5")                  # push {r4,r5,r6,lr}
    c += bw(t + len(c), body)                   # b.w body
    ignore_off = len(c)
    c += bytes.fromhex("7047")                  # bx lr
    # 回填（16 位条件跳转目标为 ignore 处）
    def fix(at: int, op: int) -> bytes:
        off = ignore_off - (at + 4)
        return struct.pack("<H", op | ((off >> 1) & 0xFF))
    c[bne_at:bne_at + 2] = fix(bne_at, 0xD100)
    c[bcc_at:bcc_at + 2] = fix(bcc_at, 0xD300)
    return bytes(c)


def main() -> int:
    data = bytearray(common.read_file(SRC))
    for entry, body, t in ((0x08035FD0, 0x08035FD2, 0x080EF980),
                           (0x08010C00, 0x08010C02, 0x080EFA00)):
        o = entry - BASE
        if bytes(data[o:o + 2]) != bytes.fromhex("70b5"):
            print("0x%08X 不是 push: %s" % (entry, bytes(data[o:o + 2]).hex(" ")))
            return 1
        to = t - BASE
        if bytes(data[to:to + 64]) != b"\xff" * 64:
            print("跳板区 0x%08X 非 0xFF" % t)
            return 1
        code = make_tramp(t, body)
        data[to:to + len(code)] = code
        data[o:o + 4] = bw(entry, t)
        print("跳板 0x%08X (%d B) -> 0x%08X" % (t, len(code), entry))
    # 关闭热靴状态触发的关机请求（0x080220D4 只被 TMR7 调用）
    off = 0x080220D4 - BASE
    if bytes(data[off:off + 4]) == bytes.fromhex("01480078"):
        data[off:off + 8] = bytes.fromhex("0020704700bf00bf")
        print("已关闭热靴状态关机请求 0x080220D4")
    out = bytes(data)
    common.write_file(DST, out)
    print("成品:", DST)
    print("sha256:", hashlib.sha256(out).hexdigest())
    return 0


if __name__ == "__main__":
    sys.exit(main())
