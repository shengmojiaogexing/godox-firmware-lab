# -*- coding: utf-8 -*-
"""Trace the FUN_0801BC88 ready-bit poll and its callers."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_LR

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

# log first N calls to FUN_0801bc88 with the raw address it polls
seen = []
pc_calls = 0
m.breakpoints[0x0801BC88] = "poll"
m.breakpoints[0x080203EC] = "init203ec"
n = 0
while m.insn_count < 15_000_000 and n < 60:
    m.run(200_000)
    if m.stop_reason and m.stop_reason.startswith("breakpoint"):
        pc = m.uc.reg_read(11) & ~1
        r0 = m.uc.reg_read(UC_ARM_REG_R0)
        lr = m.uc.reg_read(UC_ARM_REG_LR) & ~1
        arg = r0
        bit = arg & 0x1f
        off = arg >> 16
        base = 0x40000000
        addr = base + off + 0x800
        val = None
        try:
            val = struct.unpack("<I", bytes(m.uc.mem_read(addr, 4)))[0]
        except Exception:
            val = "err"
        n += 1
        if n <= 40:
            print("[%9d] %s pc=%#x arg=%#x -> bit%d addr=%#x val=%s lr=%#x"
                  % (m.insn_count, m.breakpoints[pc], pc, arg, bit, addr,
                     val, lr))
        m.stop_reason = None
        m._bp_skip = pc
        continue
print("done insn=%d PC=%#x" % (m.insn_count, m.uc.reg_read(11) & ~1))
