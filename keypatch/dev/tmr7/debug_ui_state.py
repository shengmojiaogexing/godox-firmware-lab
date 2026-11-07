# -*- coding: utf-8 -*-
"""Dump UI/power state RAM during the ON-state loop."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn.arm_const import UC_ARM_REG_PC

GPIO = sys.argv[1] if len(sys.argv) > 1 else "PA1=1,PA8=1"
MAX = int(sys.argv[2]) if len(sys.argv) > 2 else 16_000_000

WATCH = {
    "0x20000006 ui_loop_flag": (0x20000006, 1),
    "0x200003E7 page/mode": (0x200003E7, 1),
    "0x200004C4 page_req": (0x200004C4, 1),
    "0x20000470 root_window": (0x20000470, 4),
    "0x200000F4 tmr5_flag": (0x200000F4, 1),
    "0x2000030C tick_flag": (0x2000030C, 1),
    "0x20000310 tick": (0x20000310, 4),
    "0x20000000 ms_sub": (0x20000000, 1),
    "0x20000069": (0x20000069, 1),
    "0x2000006a": (0x2000006A, 1),
}

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
if GPIO:
    for item in GPIO.split(","):
        n, v = item.split("=")
        m.gpio["GPIO" + n[1].upper()].set_input(int(n[2:]), int(v), driven=True)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

for i in range(8):
    m.run(MAX // 8)
    vals = []
    for name, (addr, size) in WATCH.items():
        try:
            raw = bytes(m.uc.mem_read(addr, size))
            v = int.from_bytes(raw, "little")
        except Exception:
            v = None
        vals.append("%s=%s" % (name.split()[0], hex(v) if v is not None else "?"))
    print("[%9d] PC=%#x %s" % (m.insn_count, m.uc.reg_read(UC_ARM_REG_PC) & ~1,
                               " ".join(vals)))
