# -*- coding: utf-8 -*-
"""Collect machine fault/exception log lines during the TMR7 UI boot."""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model

lines = []
m = Machine(log_fn=lambda s: lines.append(s), trace=True)
tmr7_model.patch_real_device(m)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()
for i in range(6):
    m.run(1_000_000)
    print("[%8d] PC=%#x lines=%d pending=%r"
          % (m.insn_count, m.uc.reg_read(11) & ~1, len(lines),
             sorted(m.pending_irqs)[:6]))
print("\n--- log lines (first fault/exception related) ---")
for ln in lines:
    if "FAULT" in ln or "exc" in ln or "unmapped" in ln or "NULL" in ln:
        print(ln)
