# -*- coding: utf-8 -*-
"""Why does the UI boot path call the power-off routine (FUN_08010C00)?"""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                               UC_ARM_REG_R3, UC_ARM_REG_R4, UC_ARM_REG_R5,
                               UC_ARM_REG_R6, UC_ARM_REG_R7)

RG = {"r0": UC_ARM_REG_R0, "r1": UC_ARM_REG_R1, "r2": UC_ARM_REG_R2,
      "r3": UC_ARM_REG_R3, "r4": UC_ARM_REG_R4, "r5": UC_ARM_REG_R5,
      "r6": UC_ARM_REG_R6, "r7": UC_ARM_REG_R7}

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

BPS = {0x080112C0: "shutdown_call", 0x080112B2: "poweroff_ok",
       0x08010C8C: "boot_power"}
for a, n in BPS.items():
    m.breakpoints[a] = n

hits = 0
while m.insn_count < 20_000_000:
    m.run(1_000_000)
    r = m.stop_reason or ""
    if r.startswith("breakpoint"):
        hits += 1
        pc = m.uc.reg_read(11) & ~1
        regs = {n: m.uc.reg_read(RG[n]) for n in RG}
        tick = struct.unpack("<I", bytes(m.uc.mem_read(
            tmr7_model.TICK_COUNTER, 4)))[0]

        def u32(a):
            try:
                return struct.unpack("<I", bytes(m.uc.mem_read(a, 4)))[0]
            except Exception:
                return -1
        gpioa = m.gpio.get("GPIOA")
        idr = (gpioa.odr & ~gpioa.input_mask) | (gpioa.inputs & gpioa.input_mask)
        print("HIT %s pc=%#x insn=%d tick=%d"
              % (m.breakpoints[pc], pc, m.insn_count, tick))
        print("   r0=%#x r1=%#x r2=%#x r3=%#x r4=%#x r5=%#x r6=%#x r7=%#x"
              % tuple(regs[n] for n in ("r0", "r1", "r2", "r3", "r4", "r5",
                                        "r6", "r7")))
        print("   RAM 0x20000168=%#x 0x2000016c=%#x 0x20000070=%#x "
              "0x20000069=%#x 0x2000006a=%#x"
              % (u32(0x20000168), u32(0x2000016C), u32(0x20000070),
                 m.uc.mem_read(0x20000069, 1)[0],
                 m.uc.mem_read(0x2000006A, 1)[0]))
        print("   GPIOA MODER=%#x IDR=%#x ODR=%#x input_mask=%#x inputs=%#x"
              % (gpioa.moder, idr, gpioa.odr, gpioa.input_mask, gpioa.inputs))
        if hits >= 4:
            break
        m.stop_reason = None
        m._bp_skip = pc
        continue
    if m.insn_count % 2_000_000 < 1_000_000:
        print("[%10d] PC=%#x" % (m.insn_count, m.uc.reg_read(11) & ~1))
