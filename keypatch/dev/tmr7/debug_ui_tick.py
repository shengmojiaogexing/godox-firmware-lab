# -*- coding: utf-8 -*-
"""Check whether the 100 ms UI tick callback (FUN_08014B28) runs in the ON loop."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn.arm_const import UC_ARM_REG_PC, UC_ARM_REG_LR

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
for item in ("PA1=1", "PA8=1"):
    n, v = item.split("=")
    m.gpio["GPIO" + n[1].upper()].set_input(int(n[2:]), int(v), driven=True)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

BPS = {0x08014B28: "ui_tick_14b28", 0x080335BC: "scheduler_335bc",
       0x080334D8: "get_tick", 0x08025A04: "power_25a04"}
counts = {n: 0 for n in BPS.values()}
first = {}
for a, n in BPS.items():
    m.breakpoints[a] = n

while m.insn_count < 8_000_000:
    m.run(200_000)
    if m.stop_reason and m.stop_reason.startswith("breakpoint"):
        pc = m.uc.reg_read(UC_ARM_REG_PC) & ~1
        name = m.breakpoints.get(pc)
        if name:
            counts[name] += 1
            first.setdefault(name, (m.insn_count, m.vtime,
                                    m.uc.reg_read(UC_ARM_REG_LR) & ~1))
        m.stop_reason = None
        m._bp_skip = pc
        continue
print("insn=%d tick=%r page=%d req=%d"
      % (m.insn_count, bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)).hex(),
         m.uc.mem_read(0x200003E7, 1)[0], m.uc.mem_read(0x200004C4, 1)[0]))
for n in BPS.values():
    print("  %-18s hits=%d first=%r" % (n, counts[n], first.get(n)))
