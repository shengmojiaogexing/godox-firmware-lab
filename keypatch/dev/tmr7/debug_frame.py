# -*- coding: utf-8 -*-
"""Capture the faulting context (LR / exception frame) at the first HardFault."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn.arm_const import UC_ARM_REG_PC, UC_ARM_REG_SP

GPIO = sys.argv[1] if len(sys.argv) > 1 else ""

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
if GPIO:
    for item in GPIO.split(","):
        name, val = item.split("=")
        m.gpio["GPIO" + name[1].upper()].set_input(int(name[2:]), int(val),
                                                    driven=True)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()
while m.insn_count < 12000000:
    m.run(200_000)
    if (m.uc.reg_read(UC_ARM_REG_PC) & ~1) == 0x0800C14C:
        print("HARDFAULT at insn=%d vtime=%d tick=%r"
              % (m.insn_count, m.vtime,
                 bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)).hex()))
        sp = m.uc.reg_read(UC_ARM_REG_SP)
        frame = struct.unpack("<8I", bytes(m.uc.mem_read(sp, 0x20)))
        print("frame r0=%#x r1=%#x r2=%#x r3=%#x r12=%#x lr=%#x pc=%#x xpsr=%#x"
              % frame)
        break
print("last log lines:")
for ln in m.log_lines[-6:]:
    print(" ", ln)
