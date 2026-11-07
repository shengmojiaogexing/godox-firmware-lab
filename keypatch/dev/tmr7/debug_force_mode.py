# -*- coding: utf-8 -*-
"""Do the UI page switch / home-page builder run once mode=2 is forced?"""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn.arm_const import UC_ARM_REG_PC, UC_ARM_REG_LR, UC_ARM_REG_R0

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
for item in ("PA1=1", "PA8=1"):
    n, v = item.split("=")
    m.gpio["GPIO" + n[1].upper()].set_input(int(n[2:]), int(v), driven=True)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

BPS = {0x08014B28: "ui_tick", 0x08006F0C: "page_switch", 0x080148A8: "home_builder",
       0x080335BC: "scheduler", 0x08010ACC: "create_window"}
counts = {n: 0 for n in BPS.values()}
first = {}
forced = False
for a, n in BPS.items():
    m.breakpoints[a] = n

while m.insn_count < 12_000_000:
    m.run(200_000)
    tick = struct.unpack("<I", bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)))[0]
    if not forced and tick >= 5:
        m.uc.mem_write(0x200003E7, b"\x02")
        m.uc.mem_write(0x200004C4, b"\x01")
        forced = True
        print("forced mode=2 req=1 at insn=%d" % m.insn_count)
    if m.stop_reason and m.stop_reason.startswith("breakpoint"):
        pc = m.uc.reg_read(UC_ARM_REG_PC) & ~1
        name = m.breakpoints.get(pc)
        if name:
            counts[name] += 1
            first.setdefault(name, (m.insn_count, m.vtime,
                                    m.uc.reg_read(UC_ARM_REG_R0),
                                    m.uc.reg_read(UC_ARM_REG_LR) & ~1))
        m.stop_reason = None
        m._bp_skip = pc
        continue
print("insn=%d tick=%r mode=%d req=%d"
      % (m.insn_count, bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)).hex(),
         m.uc.mem_read(0x200003E7, 1)[0], m.uc.mem_read(0x200004C4, 1)[0]))
for n in BPS.values():
    print("  %-14s hits=%d first=%r" % (n, counts[n], first.get(n)))
