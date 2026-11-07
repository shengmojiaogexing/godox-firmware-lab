# -*- coding: utf-8 -*-
"""修复 X3-C UI 补丁的页 1 钩子（这是实机重启的第二个、也是更致命的 bug）。

问题
----
ui_patch 把 0x0800FE78（页 1 处理器）的首指令改成 `b.w trampoline`，trampoline 是：
    push {r0-r3, lr}
    bl   poll
    pop  {r0,r1,r2,r3,pc}     <-- 这里 pop 出 pc = 进入时的 LR，等于直接返回调用者！
    push.w {...}              <-- 死代码
    b.w  0x0800FE7C           <-- 死代码
结果 0x0800FE78 变成"只跑 poll 就返回"的空函数：main 冷启动里 `bl 0x0800FE78`
（0x08034148）被跳过，页 1/UI 初始化没做 → 后面 NULL 指针 → 看门狗反复复位。

修复方案（不改 0x0800FE78 入口，改调用点）
------------------------------------------
1. 恢复 0x0800FE78 原始首 4 字节（`2d e9 fc 41`）。
2. 把 UI tick 里唯一那个 `bl 0x0800FE78`（0x08014C1E）改成 `bl wrapper`。
3. wrapper（放 0x080F1100）：
       push.w {r0,r1,r2,r3,r4,lr}
       bl     poll(0x080F1000)
       pop.w  {r0,r1,r2,r3,r4,lr}
       b.w    0x0800FE78          ; 尾部调用真正的页 1 处理器
   UI tick 的返回地址(0x08014C22)在 push/pop 里保存，处理器正常执行、正常返回。

产物：keypatch/out/X3_C_v1.38_app_keyD798_ui_fix3.bin
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
ROM = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
SRC = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798.bin")
DST = os.path.join(KEYPATCH, "out", "X3_C_v1.38_app_keyD798_ui_fix3.bin")

PAGE1 = 0x0800FE78
PAGE1_ORIG = bytes.fromhex("2de9fc41")
TICK_BL = 0x08014C1E
WRAPPER = 0x080F1100
POLL = 0x080F1000


def _sx(v: int, bits: int) -> int:
    m = 1 << (bits - 1)
    return (v ^ m) - m


def encode_bl(addr: int, target: int) -> bytes:
    off = target - (addr + 4)
    if off & 1:
        raise ValueError("BL target must be even")
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
    if off & 1:
        raise ValueError("B.W target must be even")
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
    orig = common.read_file(ROM)
    data = bytearray(common.read_file(SRC))

    # 1) 恢复页 1 处理器原始入口
    o = PAGE1 - BASE
    if bytes(data[o:o + 4]) == PAGE1_ORIG:
        print("0x%08X 已是原厂字节（无需恢复）" % PAGE1)
    else:
        data[o:o + 4] = PAGE1_ORIG
        print("0x%08X 恢复原厂首 4 字节: %s" % (PAGE1, PAGE1_ORIG.hex(" ")))

    # 2) wrapper
    w = bytearray()
    w += bytes.fromhex("2de91f40")                 # push.w {r0,r1,r2,r3,r4,lr}
    w += encode_bl(WRAPPER + len(w), POLL)         # bl poll
    w += bytes.fromhex("bde81f40")                 # pop.w  {r0,r1,r2,r3,r4,lr}
    w += encode_bw(WRAPPER + len(w), PAGE1)        # b.w 0x0800FE78
    wo = WRAPPER - BASE
    data[wo:wo + len(w)] = w
    print("wrapper @0x%08X (%d B): %s" % (WRAPPER, len(w), w.hex(" ")))

    # 3) 调用点 0x08014C1E: bl 0x0800FE78 -> bl wrapper
    to = TICK_BL - BASE
    old = bytes(data[to:to + 4])
    data[to:to + 4] = encode_bl(TICK_BL, WRAPPER)
    print("0x%08X: %s -> %s（bl 0x0800FE78 -> bl wrapper）"
          % (TICK_BL, old.hex(" "), bytes(data[to:to + 4]).hex(" ")))

    out = bytes(data)
    common.write_file(DST, out)
    diffs = sum(1 for a, b in zip(orig, out) if a != b)
    print("成品:", DST)
    print("长度:", len(out), "差异字节:", diffs)
    print("sha256:", hashlib.sha256(out).hexdigest())
    return 0


if __name__ == "__main__":
    sys.exit(main())
