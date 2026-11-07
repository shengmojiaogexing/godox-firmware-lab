# -*- coding: utf-8 -*-
"""Find out where the UI run gets stuck in the RCC clock-switch poll and why."""
import os
import struct
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
rcc = m.find_peripheral(0x40023800)
print("RCC model at %#x" % rcc.base)

BP = 0x0801BDA0
m.breakpoints[BP] = "rcc_poll"
print("breakpoint at %#x" % BP)

sp, pc = m.boot()
hits = []
while m.insn_count < 20_000_000:
    m.run(1_000_000)
    if m.stop_reason and m.stop_reason.startswith("breakpoint"):
        lr = m.uc.reg_read(10) & ~1
        cfgr = rcc.cfgr
        rd = m.read_mem(0x40023808, 4)
        tick = struct.unpack("<I", bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)))[0]
        hits.append((m.insn_count, lr, cfgr, rd, tick))
        print("HIT %d: insn=%d LR=%#x cfgr=%#x read08=%#x tick=%d"
              % (len(hits), m.insn_count, lr, cfgr, rd, tick))
        if len(hits) >= 8:
            break
        m.stop_reason = None
        m._bp_skip = BP
        continue
    if m.insn_count % 2_000_000 < 1_000_000:
        print("[%10d] PC=%#010x cfgr=%#x read08=%#x" %
              (m.insn_count, m.uc.reg_read(11) & ~1, rcc.cfgr,
               m.read_mem(0x40023808, 4)))
print("done insn=%d PC=%#x" % (m.insn_count, m.uc.reg_read(11) & ~1))
