# -*- coding: utf-8 -*-
"""Trace FUN_08017f5e calls and dump state at the first HardFault."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_LR,
                               UC_ARM_REG_SP, UC_ARM_REG_R4)

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

recent = []
m.breakpoints[0x08017F5E] = "next"
while m.insn_count < 8_000_000:
    m.run(200_000)
    pc = m.uc.reg_read(11) & ~1
    if pc == 0x0800C14C:
        print("HARDFAULT at insn=%d" % m.insn_count)
        break
    if m.stop_reason and m.stop_reason.startswith("breakpoint"):
        r0 = m.uc.reg_read(UC_ARM_REG_R0)
        r1 = m.uc.reg_read(UC_ARM_REG_R1)
        lr = m.uc.reg_read(UC_ARM_REG_LR) & ~1
        recent.append((m.insn_count, m.vtime, r0, r1, lr,
                       struct.unpack("<I", bytes(m.uc.mem_read(r0, 4)))[0]
                       if r0 else None))
        if len(recent) > 8:
            del recent[0]
        m.stop_reason = None
        m._bp_skip = 0x08017F5E
        continue

print("last FUN_08017f5e calls (insn, vtime, list, arg, lr, *list):")
for r in recent:
    print("  ", r)
for addr in (0x080336A8, 0x080336A0, 0x080336A4):
    try:
        v = struct.unpack("<I", bytes(m.uc.mem_read(addr, 4)))[0]
    except Exception:
        v = None
    print("literal %#x -> %r" % (addr, hex(v) if isinstance(v, int) else v))
# list struct content
try:
    a = struct.unpack("<I", bytes(m.uc.mem_read(0x080336A8, 4)))[0]
    print("list head @%#x content:" % a, bytes(m.uc.mem_read(a, 64)).hex())
except Exception as e:
    print("list read err", e)
print("PC=%#x SP=%#x" % (m.uc.reg_read(11) & ~1, m.uc.reg_read(UC_ARM_REG_SP)))
print("last log lines:")
for ln in m.log_lines[-15:]:
    print(" ", ln)
