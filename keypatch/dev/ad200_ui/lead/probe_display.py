# -*- coding: utf-8 -*-
"""Lead: display_task 直调探针 —— 读取 C.Fn 段码位图，验证 F8 页与补丁无 UI 回归。

用法（必须用 devices/ad200pro 的 python）：
    devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\lead\\probe_display.py
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(
    os.path.dirname(os.path.abspath(__file__))))))
ADROOT = os.path.join(ROOT, "devices/ad200pro")
sys.path.insert(0, ADROOT)
from sim.machine import Machine  # noqa: E402

LOOP = 0x0800CE40
UI = 0x20000150
SET = 0x20000418
FB = 0x20000408
SENT = 0x20000600


def boot(path):
    m = Machine(path)
    m.reset()
    m.max_slice_rate = 1
    m.set_breakpoint(LOOP)
    r = m.run(max_insns=5_000_000)
    assert r == "breakpoint", (r, m.state)
    m.clear_breakpoint(LOOP)
    m.max_slice_rate = 4000
    return m


def call_display(m, item, flag, ident=5):
    s = bytearray(m.read(SET, 8))
    s[6] = ident
    s[7] = flag
    m.write(SET, bytes(s))
    m.write(UI + 0x12, b"\x01")   # C.Fn open
    m.write(UI + 0x13, b"\x00")
    m.write(UI + 0x14, bytes([item & 0xFF]))
    m.write(0x2000011C, b"\x09")  # frame counter：下一次 display_task 进入渲染
    m.write(0x2000017A, b"\x00\x00")
    m.write(0x2000016A, b"\x00")
    m.write(0x200001D0, b"\x01")
    m.write(0x200001D1, b"\x00")
    m.write(FB, bytes(16))
    m.write(SENT, bytes.fromhex("fee7"))
    m.set_reg("lr", SENT | 1)
    m.set_reg("pc", 0x08009228 | 1)
    m.set_reg("sp", 0x200008A0)
    m.run(max_insns=4000)   # display_task 自身 ~300 条；返回后停在 sentinel 的 b .
    return bytes(m.read(FB, 16))


def main():
    orig = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")
    patched = os.path.join(ROOT, "keypatch", "out", "AD200pro_V1.60_keyD798.bin")
    mo, mp = boot(orig), boot(patched)
    ok = True
    print("item  F#  ORIG bitmap                     PATCH bitmap                    same")
    for item in range(8):
        bo = call_display(mo, item, 0)
        bp = call_display(mp, item, 0)
        same = bo == bp
        ok &= same
        print("%-5d F%-2d %s  %s  %s"
              % (item, item + 1, bo.hex(" "), bp.hex(" "), same))
    # F8 ON/OFF 必须不同（值区段码变化），且补丁与原厂渲染一致
    for flag in (0, 1):
        bo = call_display(mo, 7, flag)
        bp = call_display(mp, 7, flag)
        same = bo == bp
        ok &= same
        print("F8 flag=%d ORIG %s  PATCH %s  same=%s"
              % (flag, bo.hex(" "), bp.hex(" "), same))
    f0 = call_display(mp, 7, 0)
    f1 = call_display(mp, 7, 1)
    diff = [i for i in range(16) if f0[i] != f1[i]]
    ok &= bool(diff)
    print("F8 ON/OFF 段码差异字节: %s（ON=%s / OFF=%s）"
          % (diff, f1.hex(" "), f0.hex(" ")))
    print("结论:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
