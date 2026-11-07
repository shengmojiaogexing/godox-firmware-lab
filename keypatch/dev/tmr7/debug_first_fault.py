# -*- coding: utf-8 -*-
"""Stop at the first HardFault and dump the machine log around it."""
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

fault_at = None
while m.insn_count < 12000000:
    m.run(200_000)
    pc = m.uc.reg_read(11) & ~1
    if pc in (0x0800C14C,):
        fault_at = m.insn_count
        break
    if m.stop_reason and not m.stop_reason.startswith("max_insn"):
        fault_at = m.insn_count
        print("stop_reason", m.stop_reason)
        break
print("fault_at insn=%r PC=%#x" % (fault_at, m.uc.reg_read(11) & ~1))
print("\n--- last 40 log lines ---")
for ln in lines[-40:]:
    print(ln)
