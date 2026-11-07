# -*- coding: utf-8 -*-
"""X3-C v3 密钥开关双发补丁 —— 整机模拟器端到端测试（跑真实固件函数，无桩）。

流程
----
1. 用 devices/x3c/emu/machine.Machine 真实启动补丁后的固件，跑到 main(0x08033E20) 入口。
2. 在 main 入口"劫持"：设置 RAM 里的当前 ID（0x20000055）、KEY_ON 开关
   （0x20001080）、ID-OFF 标志（0x20000143）、发送门控（0x20000146），
   然后直接调用真实组帧函数 send_short(0x09) / send_property(0x50,0xB1,0)。
3. 代码钩子记录 CC2500 写寄存器（0x08023540）与低层发送（读 TX 缓冲 0x20000152）。
4. 断言 v3 语义：
   - KEY_ON=1, ID=5  -> 双发；第二帧 SYNC=50024/6=0x2091
   - KEY_ON=0, ID=5  -> 单发
   - KEY_ON=0, ID=99 -> 双发；第二帧 SYNC=50024/100=0x01F4
   - KEY_ON=0, ID=99, ID-OFF=1 -> 单发
   - 原厂镜像自证：只发一次、不写 SYNC。

运行（在  下）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\runtime_toggle_test.py
    # 输出同时写入 keypatch/dev/x3c/runtime_toggle_test.log
"""
from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
sys.path.insert(0, EMU)

from machine import Machine, APP_BASE  # noqa: E402
from unicorn import UC_HOOK_CODE  # noqa: E402
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,  # noqa: E402
                               UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_SP)

ORIG = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798.bin")
KEY = 0xD798
KEY_HI, KEY_LO = (KEY >> 8) & 0xFF, KEY & 0xFF
SYNC_NUMERATOR = 50024

MAIN = 0x08033E20
SEND_SHORT = 0x080118B0
SEND_PROP = 0x0801184C
WRITE_REG = 0x08023540
LOW_SHORT = 0x0801712C
LOW_PROP = 0x0801708C
ID_RAM = 0x20000055
ID_OFF_FLAG = 0x20000143
KEY_ON_FLAG = 0x20001080      # ui-x3c 最终定址（无字面量引用；RAM 版，重启复位）
TX_GATE = 0x20000146
TX_BUF = 0x20000152
SENTINEL = 0x20046000
STACK = 0x20047000

lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def rel(p: str) -> str:
    return os.path.relpath(p, ROOT)


def run_case(image: str, ident: int, func: int, args: tuple,
             key_on: int = 0, id_off: int = 0) -> dict:
    m = Machine(image=image, image_base=APP_BASE, trace=False,
                log_fn=lambda s: None, hardfault=True)
    m.uc.mem_write(0x0801C118, b"\x70\x47")   # 内存快进标定延时（真机适配）
    m.uc.mem_write(0x0801C0FE, b"\x70\x47")
    m.uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
    m.boot()
    m.interrupt_after_chunk = False

    wreg: list[tuple[int, int]] = []
    frames: list[tuple[int, bytes]] = []

    def on_code(uc, address, size, _user):
        if address == WRITE_REG:
            wreg.append((uc.reg_read(UC_ARM_REG_R0), uc.reg_read(UC_ARM_REG_R1)))
        elif address == LOW_SHORT:
            frames.append((2, bytes(uc.mem_read(TX_BUF, 2))))
        elif address == LOW_PROP:
            frames.append((4, bytes(uc.mem_read(TX_BUF, 4))))

    for a in (WRITE_REG, LOW_SHORT, LOW_PROP):
        m.uc.hook_add(UC_HOOK_CODE, on_code, None, a, a + 2)

    stop = m.run_until(MAIN, max_insns=2_000_000)
    if (m.reg("PC") & ~1) != MAIN:
        return {"ok": False, "error": "未停在 main（%s, pc=0x%08X）"
                % (stop, m.reg("PC") & ~1)}

    m.uc.mem_write(ID_RAM, bytes([ident & 0xFF]))
    m.uc.mem_write(ID_OFF_FLAG, bytes([id_off & 0xFF]))
    m.uc.mem_write(KEY_ON_FLAG, bytes([key_on & 0xFF]))
    m.uc.mem_write(TX_GATE, b"\x00")
    m.uc.reg_write(UC_ARM_REG_SP, STACK)
    m.uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    for i, v in enumerate(args):
        m.uc.reg_write((UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2)[i], v)
    m.uc.reg_write(UC_ARM_REG_PC, func | 1)
    m.stop_reason = None
    stop = m.run_until(SENTINEL, max_insns=3_000_000)
    pc = m.reg("PC") & ~1
    if pc != SENTINEL:
        return {"ok": False, "error": "函数未返回哨兵（%s, pc=0x%08X lr=0x%08X）"
                % (stop, pc, m.reg("LR")), "wreg": wreg, "frames": frames}
    return {"ok": True, "wreg": wreg, "frames": frames}


def fmt_wreg(w) -> str:
    return " -> ".join("WriteReg(%d,0x%02X)" % (a, b) for a, b in w) or "(none)"


def fmt_frames(f) -> str:
    return " | ".join("TX%d[%s]" % (n, data.hex(" ")) for n, data in f) or "(none)"


def want_wreg(keymode: bool, ident: int) -> list[tuple[int, int]]:
    if not keymode:
        return []
    sync = SYNC_NUMERATOR // (ident + 1)
    return [(4, KEY_HI), (5, KEY_LO),
            (4, (sync >> 8) & 0xFF), (5, sync & 0xFF)]


def check_patched() -> tuple[bool, list[str]]:
    msgs: list[str] = []
    ok_all = True
    frame_short = bytes.fromhex("d509")
    frame_prop = bytes.fromhex("a950b100")

    cases = [
        # (说明, ID, KEY_ON, ID_OFF, 是否密钥模式)
        ("KEY_ON=1,ID=5  双发(第二帧 0x2091)", 5, 1, 0, True),
        ("KEY_ON=0,ID=5  单发", 5, 0, 0, False),
        ("KEY_ON=0,ID=99 双发(第二帧 0x01F4)", 99, 0, 0, True),
        ("KEY_ON=1,ID=99 双发(第二帧 0x01F4)", 99, 1, 0, True),
        ("KEY_ON=0,ID=99,ID-OFF=1 单发", 99, 0, 1, False),
        ("KEY_ON=1,ID=5,ID-OFF=1  双发(开关优先)", 5, 1, 1, True),
    ]
    for name, ident, key_on, id_off, keymode in cases:
        r = run_case(PATCHED, ident, SEND_SHORT, (0x09,), key_on, id_off)
        if not r["ok"]:
            msgs.append("[补丁] send_short %s 执行失败: %s" % (name, r.get("error")))
            ok_all = False
            continue
        want = want_wreg(keymode, ident)
        want_frames = [2, 2] if keymode else [2]
        ok = (r["wreg"] == want and [n for n, _ in r["frames"]] == want_frames
              and all(d == frame_short for _, d in r["frames"]))
        msgs.append("[补丁] send_short %-34s 写SYNC=%-52s 帧=%s %s"
                    % (name, fmt_wreg(r["wreg"]), fmt_frames(r["frames"]),
                       "PASS" if ok else "FAIL"))
        ok_all &= ok

    for name, ident, key_on, id_off, keymode in (
            ("KEY_ON=1,ID=5  双发", 5, 1, 0, True),
            ("KEY_ON=0,ID=5  单发", 5, 0, 0, False),
            ("KEY_ON=0,ID=99 双发", 99, 0, 0, True)):
        r = run_case(PATCHED, ident, SEND_PROP, (0x50, 0xB1, 0x00), key_on, id_off)
        if not r["ok"]:
            msgs.append("[补丁] send_property %s 执行失败: %s" % (name, r.get("error")))
            ok_all = False
            continue
        want = want_wreg(keymode, ident)
        want_frames = [4, 4] if keymode else [4]
        ok = (r["wreg"] == want and [n for n, _ in r["frames"]] == want_frames
              and all(d == frame_prop for _, d in r["frames"]))
        msgs.append("[补丁] send_property %-30s 写SYNC=%-52s 帧=%s %s"
                    % (name, fmt_wreg(r["wreg"]), fmt_frames(r["frames"]),
                       "PASS" if ok else "FAIL"))
        ok_all &= ok
    return ok_all, msgs


def check_original() -> tuple[bool, list[str]]:
    msgs: list[str] = []
    ok_all = True
    for name, func, args, tag, ident in (
            ("send_short(ID=99)", SEND_SHORT, (0x09,), "d509", 99),
            ("send_short(ID=5)", SEND_SHORT, (0x09,), "d509", 5),
            ("send_property(ID=99)", SEND_PROP, (0x50, 0xB1, 0x00), "a950b100", 99)):
        r = run_case(ORIG, ident, func, args, 0, 0)
        if not r["ok"]:
            msgs.append("[原厂] %s 执行失败: %s" % (name, r.get("error")))
            ok_all = False
            continue
        ok = r["wreg"] == [] and len(r["frames"]) == 1 and \
            r["frames"][0][1] == bytes.fromhex(tag)
        msgs.append("[原厂] %-22s 写SYNC=%s 帧=%s %s"
                    % (name, fmt_wreg(r["wreg"]), fmt_frames(r["frames"]),
                       "PASS" if ok else "FAIL"))
        ok_all &= ok
    return ok_all, msgs


def main() -> int:
    out("X3-C v3 密钥开关双发补丁 —— 整机模拟器端到端测试（真实固件函数）")
    out("原厂镜像 : %s" % rel(ORIG))
    out("补丁镜像 : %s" % rel(PATCHED))
    out("KEY=0x%04X（SYNC 0x%02X/0x%02X）；第二帧 SYNC=floor(50024/(ID+1))"
        % (KEY, KEY_HI, KEY_LO))
    out("KEY_ON_FLAG=0x%08X（ui-x3c 最终定址；RAM 版，重启复位）" % KEY_ON_FLAG)
    out("")
    ok1, m1 = check_patched()
    ok2, m2 = check_original()
    for m in m1 + m2:
        out("  " + m)
    out("")
    out("结论: %s" % ("ALL PASS" if (ok1 and ok2) else "FAIL"))
    log = os.path.join(HERE, "runtime_toggle_test.log")
    with open(log, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % rel(log))
    return 0 if (ok1 and ok2) else 1


if __name__ == "__main__":
    sys.exit(main())
