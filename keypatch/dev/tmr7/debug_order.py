# -*- coding: utf-8 -*-
"""Trace boot order: LCD init vs power-management vs TMR5 measurement."""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

BPS = {0x08035A40: "lcd_init", 0x08010C8C: "power_mgmt", 0x08025F30: "tmr5_measure",
       0x08035280: "adc_init", 0x080203EC: "lsi_init", 0x08006F0C: "mega_6f0c",
       0x0800830C: "mega_830c", 0x080353A0: "x353a0", 0x080358FC: "x358fc"}
for a, n in BPS.items():
    m.breakpoints[a] = n

hits = []
n = 0
while m.insn_count < 6_000_000 and n < 80:
    m.run(500_000)
    if m.stop_reason and m.stop_reason.startswith("breakpoint"):
        pc = m.uc.reg_read(11) & ~1
        n += 1
        hits.append((m.insn_count, pc, m.breakpoints[pc]))
        print("[%9d] %s @ %#x pending=%r tick=%r"
              % (m.insn_count, m.breakpoints[pc], pc,
                 sorted(m.pending_irqs)[:6],
                 bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)).hex()))
        m.stop_reason = None
        m._bp_skip = pc
        continue
print("end insn=%d PC=%#x" % (m.insn_count, m.uc.reg_read(11) & ~1))
