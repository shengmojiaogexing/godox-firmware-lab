# -*- coding: utf-8 -*-
"""X3-C v3 密钥开关双发补丁 —— **独立** Unicorn 调用级验证（不导入设备模块）。

本脚本刻意不复用 keypatch/devices/x3c.py 的任何代码：直接读原厂 bin 与
keypatch 输出的补丁 bin，自己解码入口跳转，用独立 Unicorn harness 断言 v3 语义：

  密钥模式 = (KEY_ON_FLAG != 0) 或 (当前ID==99 且 ID-OFF==0)
  * 密钥模式：先 WriteReg(4,KEY>>8),WriteReg(5,KEY&0xFF) -> 发送；
              再 WriteReg(4, sync>>8),WriteReg(5, sync&0xFF) -> 再发送
              （sync = floor(50024/(当前ID+1))，注入代码里现算）
  * 普通模式：只发一次，不写 SYNC 寄存器（原厂行为）
  * 原厂 bin 自证：同一 harness 下只发一次、不写 SYNC

KEY_ON_FLAG = 0x20001080（ui-x3c 最终定址；RAM 版，重启复位=关）。

运行（在  下）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\unicorn_toggle_test.py
    # 输出同时写入 keypatch/dev/x3c/unicorn_toggle_test.log
"""
from __future__ import annotations

import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))

from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                     UC_HOOK_CODE)  # noqa: E402
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,  # noqa: E402
                               UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_SP,
                               UC_ARM_REG_CPSR)

BASE = 0x08004800
ORIG = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798.bin")
KEY = 0xD798
KEY_HI, KEY_LO = (KEY >> 8) & 0xFF, KEY & 0xFF
SYNC_NUMERATOR = 50024

SEND_SHORT = 0x080118B0
SEND_PROP = 0x0801184C
WRITE_REG = 0x08023540
LOW_SHORT = 0x0801712C
LOW_PROP = 0x0801708C
DELAY = 0x080232B6
ID_RAM = 0x20000055
ID_OFF_FLAG = 0x20000143
KEY_ON_FLAG = 0x20001080      # ui-x3c 最终定址（无字面量引用；RAM 版，重启复位）
TX_GATE = 0x20000146
INJ_BASE = 0x080EE000
INJ_END = 0x080EE400

FLASH_ALIGN, FLASH_SIZE = 0x08004000, 0x100000
RAM, RAM_SIZE = 0x20000000, 0x80000
PERIPH, PERIPH_SIZE = 0x40000000, 0x100000
STACK = 0x20047000
SENTINEL = 0x20040000

lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def rel(p: str) -> str:
    return os.path.relpath(p, ROOT)


def decode_bw(addr: int, b: bytes) -> int:
    """解码 4 字节 B.W(T4)，返回目标地址。"""
    hw1, hw2 = struct.unpack("<HH", b)
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0x9000:
        raise ValueError("不是 B.W T4 编码: %s" % b.hex(" "))
    s = (hw1 >> 10) & 1
    imm10 = hw1 & 0x3FF
    j1 = (hw2 >> 13) & 1
    j2 = (hw2 >> 11) & 1
    i1 = 1 - (j1 ^ s)
    i2 = 1 - (j2 ^ s)
    imm = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | ((hw2 & 0x7FF) << 1)
    if imm & (1 << 24):
        imm -= 1 << 25
    return addr + 4 + imm


def run_case(image: bytes, entry: int, args: tuple, ident: int,
             id_off: int = 0, key_on: int = 0) -> dict:
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(FLASH_ALIGN, FLASH_SIZE)
    uc.mem_map(RAM, RAM_SIZE)
    uc.mem_map(PERIPH, PERIPH_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
    uc.mem_write(ID_RAM, bytes([ident & 0xFF]))
    uc.mem_write(ID_OFF_FLAG, bytes([id_off & 0xFF]))
    uc.mem_write(KEY_ON_FLAG, bytes([key_on & 0xFF]))
    uc.mem_write(TX_GATE, b"\x00")

    calls: list[tuple] = []
    err = {"msg": None}

    def hook(_uc, address, _size, _user):
        if address == SENTINEL:
            _uc.emu_stop()
            return
        if address == WRITE_REG:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0),
                          _uc.reg_read(UC_ARM_REG_R1)))
        elif address == LOW_SHORT:
            calls.append(("send_short", None, None))
        elif address == LOW_PROP:
            calls.append(("send_prop", None, None))
        elif address == DELAY:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))
        else:
            return
        _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))  # LR 原值

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, STACK)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    for i, v in enumerate(args):
        uc.reg_write((UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2)[i], v)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    try:
        uc.emu_start(entry | 1, 0, count=200000)
    except UcError as exc:
        err["msg"] = "%s (pc=0x%08X lr=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR))
    return {"calls": calls, "error": err["msg"]}


def fmt(calls) -> str:
    p = []
    for k, a, b in calls:
        if k == "wreg":
            p.append("WriteReg(%d,0x%02X)" % (a, b))
        elif k == "send_short":
            p.append("SEND_SHORT")
        elif k == "send_prop":
            p.append("SEND_PROP")
        else:
            p.append("delay(%d)" % a)
    return " -> ".join(p) or "(none)"


def want_calls(keymode: bool, ident: int, send_tag: str) -> list:
    if not keymode:
        return [(send_tag, None, None)]
    sync = SYNC_NUMERATOR // (ident + 1)
    return [("wreg", 4, KEY_HI), ("wreg", 5, KEY_LO), (send_tag, None, None),
            ("wreg", 4, (sync >> 8) & 0xFF), ("wreg", 5, sync & 0xFF),
            (send_tag, None, None)]


def main() -> int:
    orig = open(ORIG, "rb").read()
    patched = open(PATCHED, "rb").read()
    out("X3-C v3 密钥开关双发补丁 —— 独立 Unicorn 调用级验证")
    out("原厂镜像 : %s (%d B)" % (rel(ORIG), len(orig)))
    out("补丁镜像 : %s (%d B)" % (rel(PATCHED), len(patched)))
    out("KEY=0x%04X（SYNC1=0x%02X SYNC0=0x%02X）；第二帧 SYNC=floor(50024/(ID+1))"
        % (KEY, KEY_HI, KEY_LO))
    out("KEY_ON_FLAG = 0x%08X（ui-x3c 最终定址；RAM 版，重启复位）" % KEY_ON_FLAG)
    ok_all = True

    # ---- 1) 静态 ----
    out("\n--- 静态检查 ---")
    ok = len(orig) == len(patched)
    out("  [%s] 长度不变: %d B" % ("PASS" if ok else "FAIL", len(patched)))
    ok_all &= ok
    entry_s = patched[SEND_SHORT - BASE:SEND_SHORT - BASE + 4]
    entry_p = patched[SEND_PROP - BASE:SEND_PROP - BASE + 4]
    try:
        t_short = decode_bw(SEND_SHORT, entry_s)
        t_prop = decode_bw(SEND_PROP, entry_p)
        inj_ok = (INJ_BASE <= t_short < INJ_END and INJ_BASE <= t_prop < INJ_END)
        msg = ("send_short 入口 B.W -> 0x%08X，send_property 入口 B.W -> 0x%08X"
               "（都在注入区 0x%08X..0x%08X 内）" % (t_short, t_prop, INJ_BASE, INJ_END))
    except ValueError as e:
        inj_ok = False
        msg = "入口跳转解码失败: %s" % e
    out("  [%s] %s" % ("PASS" if inj_ok else "FAIL", msg))
    ok_all &= inj_ok

    diffs = [i for i in range(len(orig)) if orig[i] != patched[i]]
    regions = [(SEND_SHORT - BASE, SEND_SHORT - BASE + 4),
               (SEND_PROP - BASE, SEND_PROP - BASE + 4),
               (INJ_BASE - BASE, INJ_END - BASE)]
    outside = [d for d in diffs if not any(lo <= d < hi for lo, hi in regions)]
    out("  [%s] 与原厂差异 %d 字节，全部落在 3 个补丁区内（越界 %d 字节）"
        % ("PASS" if not outside else "FAIL", len(diffs), len(outside)))
    ok_all &= not outside

    # ---- 2) 补丁镜像 ----
    out("\n--- 补丁镜像（真实注入代码执行）---")
    short_cases = [
        # (说明, ID, KEY_ON, ID_OFF, 期望是否密钥模式)
        ("KEY_ON=1,ID=5  -> 双发(第二帧 0x2091)", 5, 1, 0, True),
        ("KEY_ON=0,ID=5  -> 单发", 5, 0, 0, False),
        ("KEY_ON=0,ID=99 -> 双发(第二帧 0x01F4)", 99, 0, 0, True),
        ("KEY_ON=1,ID=99 -> 双发(第二帧 0x01F4)", 99, 1, 0, True),
        ("KEY_ON=0,ID=99,ID_OFF=1 -> 单发", 99, 0, 1, False),
        ("KEY_ON=1,ID=5,ID_OFF=1  -> 双发(开关优先)", 5, 1, 1, True),
        ("KEY_ON=0,ID=0  -> 单发", 0, 0, 0, False),
    ]
    for name, ident, key_on, id_off, keymode in short_cases:
        r = run_case(patched, SEND_SHORT, (0x09,), ident, id_off, key_on)
        want = want_calls(keymode, ident, "send_short")
        ok = r["error"] is None and r["calls"] == want
        out("  [%s] send_short %-38s %s"
            % ("PASS" if ok else "FAIL", name, fmt(r["calls"])))
        if not ok:
            out("        error: %s\n        want: %s" % (r["error"], fmt(want)))
        ok_all &= ok

    prop_cases = [
        ("KEY_ON=1,ID=5  -> 双发", 5, 1, 0, True),
        ("KEY_ON=0,ID=5  -> 单发", 5, 0, 0, False),
        ("KEY_ON=0,ID=99 -> 双发", 99, 0, 0, True),
    ]
    for name, ident, key_on, id_off, keymode in prop_cases:
        r = run_case(patched, SEND_PROP, (0x50, 0xB1, 0x00), ident, id_off, key_on)
        want = want_calls(keymode, ident, "send_prop")
        ok = r["error"] is None and r["calls"] == want
        out("  [%s] send_property %-35s %s"
            % ("PASS" if ok else "FAIL", name, fmt(r["calls"])))
        if not ok:
            out("        error: %s\n        want: %s" % (r["error"], fmt(want)))
        ok_all &= ok

    # ---- 3) 原厂镜像自证 ----
    out("\n--- 原厂镜像（harness 自证：只发一次、不写 SYNC）---")
    for name, entry, args, tag in (
            ("send_short(ID=99)", SEND_SHORT, (0x09,), "send_short"),
            ("send_short(ID=5)", SEND_SHORT, (0x09,), "send_short"),
            ("send_property(ID=99)", SEND_PROP, (0x50, 0xB1, 0x00), "send_prop")):
        r = run_case(orig, entry, args, 99 if "99" in name else 5)
        ok = r["error"] is None and r["calls"] == [(tag, None, None)]
        out("  [%s] %-24s %s" % ("PASS" if ok else "FAIL", name, fmt(r["calls"])))
        ok_all &= ok

    out("\n结论: %s" % ("ALL PASS" if ok_all else "FAIL"))
    log = os.path.join(HERE, "unicorn_toggle_test.log")
    with open(log, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % rel(log))
    return 0 if ok_all else 1


if __name__ == "__main__":
    sys.exit(main())
