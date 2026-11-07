# -*- coding: utf-8 -*-
"""Check NVIC enable / pending / active state after the first IRQ55 dispatch."""
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

for i in range(12):
    m.run(1_000_000)
    print("[%9d] PC=%#x tick=%r sub=%d active=%r pending=%r "
          "en18=%d en55=%d pend55counts=%d"
          % (m.insn_count, m.uc.reg_read(11) & ~1,
             bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)).hex(),
             m.uc.mem_read(tmr7_model.MS_SUBCOUNTER, 1)[0],
             m.active_exception, sorted(m.pending_irqs),
             (m.ppb.nvic_enabled >> 18) & 1,
             (m.ppb.nvic_enabled >> 55) & 1,
             model.irq_pends))
    if model.irq_entry_pcs:
        print("    irq entries: %r" % (model.irq_entry_pcs[-5:],))
