# -*- coding: utf-8 -*-
"""Godox X3-S V1.25 私有密钥 v1 补丁 —— 独立的 Unicorn 调用级测试（交叉验证，不依赖设备模块）。

该脚本刻意 **不导入** keypatch/devices/x3s.py，独立实现补丁与 harness，用来交叉核对设备模块；
同一 harness 跑原厂镜像应得到原公式 50024/(ID+1)（自证 harness 本身正确）。

用法（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\dev\\x3s\\unicorn_sync_test.py
    # 输出同时写入 keypatch/dev/x3s/unicorn_sync_test.log

harness 要点
------------
* 镜像加载到 0x08004800；RAM 0x20000000 起 0x48000 字节，栈 0x20047000。
* 从 0x0801474D（Thumb）开始执行 0x0801474C 同步字设置函数，r0=ID。
* 代码钩子把辅助函数入口当作"记录桩"：
    0x08023428 WriteReg(reg=r0, val=r1)
    0x08023408 Strobe(cmd=r0)
    0x0802319E delay(n=r0)
  钩子里记录参数后把 PC 写成 LR 的 **原值**（bit0=1 表示 Thumb，不能清零）。
* 函数末尾 `pop.w {r4,lr}; mov.w r0,#0x2bc; b.w delay`，因此 delay 桩也必须拦截。
"""
from __future__ import annotations

import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))

from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,  # noqa: E402
                     UC_HOOK_CODE)
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_LR,  # noqa: E402
                               UC_ARM_REG_PC, UC_ARM_REG_SP, UC_ARM_REG_CPSR)

BASE = 0x08004800
IMAGE = os.path.join(ROOT, "devices", "x3s", "firmware", "X3_S_v1.25_app.bin")
KEY = 0xD798
IDS = (0, 1, 37, 99)
ID_NUMERATOR = 50024

SYNC_FN = 0x0801474C
PATCH_MOVW_ADDR = 0x0801474E
PATCH_DIV_ADDR = 0x08014752
WRITE_REG = 0x08023428
WRITE_CMD = 0x08023408
DELAY = 0x0802319E

FLASH_ALIGN = 0x08004000
FLASH_SIZE = 0x100000
RAM = 0x20000000
RAM_SIZE = 0x48000
STACK = 0x20047000
SENTINEL = 0x20000000

lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def movw(imm16: int, rd: int = 1) -> bytes:
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    hw1 = 0xF240 | (i << 10) | imm4
    hw2 = (imm3 << 12) | (rd << 8) | imm8
    return struct.pack("<HH", hw1, hw2)


def make_patched(orig: bytes) -> bytes:
    buf = bytearray(orig)
    o = PATCH_MOVW_ADDR - BASE
    assert bytes(buf[o:o + 4]) == bytes.fromhex("4cf26831"), "原厂 movw 不匹配"
    buf[o:o + 4] = movw(KEY, rd=1)
    o = PATCH_DIV_ADDR - BASE
    assert bytes(buf[o:o + 2]) == bytes.fromhex("401c"), "原厂 adds 不匹配"
    buf[o:o + 2] = bytes.fromhex("0120")
    return bytes(buf)


def run_sync_fn(image: bytes, ident: int) -> dict:
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(FLASH_ALIGN, FLASH_SIZE)
    uc.mem_map(RAM, RAM_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))   # b .

    calls: list[tuple] = []
    err = {"msg": None}

    def hook(_uc, address, _size, _user):
        if address == SENTINEL:
            _uc.emu_stop()
            return
        if address in (WRITE_REG, WRITE_CMD, DELAY):
            r0 = _uc.reg_read(UC_ARM_REG_R0)
            r1 = _uc.reg_read(UC_ARM_REG_R1)
            kind = {WRITE_REG: "wreg", WRITE_CMD: "cmd", DELAY: "delay"}[address]
            calls.append((kind, r0, r1))
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))  # LR 原值！
            return

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, STACK)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    try:
        uc.emu_start(SYNC_FN | 1, 0, count=50000)
    except UcError as exc:  # noqa: BLE001
        err["msg"] = "%s (pc=0x%08X lr=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR))
    return {"calls": calls, "error": err["msg"]}


def sync_of(calls):
    regs = {a: b for (k, a, b) in calls if k == "wreg"}
    if 4 in regs and 5 in regs:
        return (regs[4] << 8) | regs[5]
    return None


def seq_str(calls) -> str:
    parts = []
    for k, a, b in calls:
        if k == "wreg":
            parts.append("WriteReg(%d,0x%02X)" % (a, b))
        elif k == "cmd":
            parts.append("cmd(0x%02X)" % a)
        else:
            parts.append("delay(%d)" % a)
    return " -> ".join(parts)


def check_tail(calls) -> bool:
    tail = [(k, a) for (k, a, b) in calls[2:]]
    return tail == [("delay", 0x14), ("cmd", 0x33), ("delay", 0x3E8),
                    ("cmd", 0x36), ("delay", 0x2BC)]


def main() -> int:
    orig = open(IMAGE, "rb").read()
    patched = make_patched(orig)
    out("Godox X3-S V1.25 私有密钥 v1 补丁 —— 独立 Unicorn 调用级测试")
    out("镜像     : %s (%d B)" % (os.path.relpath(IMAGE, ROOT), len(orig)))
    out("补丁     : 0x%08X movw r1,#0x%04X (%s) ; 0x%08X movs r0,#1"
        % (PATCH_MOVW_ADDR, KEY, movw(KEY).hex(" "), PATCH_DIV_ADDR))
    out("KEY      : 0x%04X -> SYNC1=0x%02X SYNC0=0x%02X"
        % (KEY, KEY >> 8, KEY & 0xFF))

    ok_all = True
    out("\n--- 原厂镜像（harness 自证：应得 50024/(ID+1)）---")
    for ident in IDS:
        r = run_sync_fn(orig, ident)
        want = ID_NUMERATOR // (ident + 1)
        got = sync_of(r["calls"])
        ok = (r["error"] is None and got == want and check_tail(r["calls"]))
        ok_all &= ok
        out("  ID=%-2d 期望=0x%04X 实得=%s  %s"
            % (ident, want, ("0x%04X" % got) if got is not None else "NONE",
               "PASS" if ok else "FAIL"))
        if not ok:
            out("    seq=%s err=%s" % (seq_str(r["calls"]), r["error"]))

    out("\n--- 补丁镜像（应恒为 KEY=0x%04X）---" % KEY)
    for ident in IDS:
        r = run_sync_fn(patched, ident)
        got = sync_of(r["calls"])
        wregs = [(a, b) for (k, a, b) in r["calls"] if k == "wreg"]
        orig_sync = ID_NUMERATOR // (ident + 1)
        ok = (r["error"] is None and got == KEY
              and wregs == [(4, KEY >> 8), (5, KEY & 0xFF)]
              and check_tail(r["calls"])
              and got != orig_sync)
        ok_all &= ok
        out("  ID=%-2d 实得=%s（原公式=0x%04X）%s"
            % (ident, ("0x%04X" % got) if got is not None else "NONE",
               orig_sync, "PASS" if ok else "FAIL"))
        out("        %s" % seq_str(r["calls"]))
        if not ok:
            out("        err=%s" % r["error"])

    out("\n结论: %s" % ("ALL PASS" if ok_all else "FAIL"))
    log = os.path.join(HERE, "unicorn_sync_test.log")
    with open(log, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % os.path.relpath(log, ROOT))
    return 0 if ok_all else 1


if __name__ == "__main__":
    sys.exit(main())
