# -*- coding: utf-8 -*-
"""Why doesn't the ON loop call the scheduler?  Check the branch at 0x08010FE6."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn.arm_const import UC_ARM_REG_PC, UC_ARM_REG_R4

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
m.gpio["GPIOA"].set_input(1, 1, driven=True)
m.gpio["GPIOA"].set_input(8, 1, driven=True)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

BPS = {0x08010FBC: "loop_wait", 0x08010FE6: "branch", 0x08010FEA: "sched_call",
       0x080110E0: "else_target", 0x080335BC: "scheduler"}
counts = {n: 0 for n in BPS.values()}
r4s = []
for a, n in BPS.items():
    m.breakpoints[a] = n

while m.insn_count < 12_000_000:
    m.run(100_000)
    if m.stop_reason and m.stop_reason.startswith("breakpoint"):
        pc = m.uc.reg_read(UC_ARM_REG_PC) & ~1
        name = m.breakpoints.get(pc)
        if name:
            counts[name] += 1
            if name in ("branch", "sched_call", "else_target") and len(r4s) < 12:
                r4s.append((name, m.insn_count, hex(m.uc.reg_read(UC_ARM_REG_R4)),
                            m.uc.mem_read(0x20000006, 1)[0],
                            m.uc.mem_read(0x2000007F, 1)[0],
                            m.uc.mem_read(0x20000003, 1)[0]))
        m.stop_reason = None
        m._bp_skip = pc
        continue
print("insn=%d counts=%r" % (m.insn_count, counts))
print("samples:", r4s)
print("RAM[0x2000007f]=%d RAM[0x20000006]=%d RAM[0x20000003]=%d"
      % (m.uc.mem_read(0x2000007F, 1)[0], m.uc.mem_read(0x20000006, 1)[0],
         m.uc.mem_read(0x20000003, 1)[0]))
