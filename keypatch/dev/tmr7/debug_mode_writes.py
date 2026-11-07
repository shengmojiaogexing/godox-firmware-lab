# -*- coding: utf-8 -*-
"""Who writes the UI mode byte RAM[0x200003E7]?  (and other UI state vars)"""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model
from unicorn import UC_HOOK_MEM_WRITE
from unicorn.arm_const import UC_ARM_REG_PC

GPIO = sys.argv[1] if len(sys.argv) > 1 else "PA1=1,PA8=1"

WATCH = {0x200003E7: "mode", 0x200004C4: "page_req", 0x20000470: "root_win",
         0x20000006: "ui_loop_flag", 0x20000069: "x69", 0x2000006A: "x6a"}

m = Machine(log_fn=lambda s: None, trace=True)
tmr7_model.patch_real_device(m)
if GPIO:
    for item in GPIO.split(","):
        n, v = item.split("=")
        m.gpio["GPIO" + n[1].upper()].set_input(int(n[2:]), int(v), driven=True)
model = tmr7_model.attach(m, scale_num=1, scale_den=1)
m.boot()

counts = {k: 0 for k in WATCH}
seen = []


def on_write(uc, access, address, size, value, user):
    for a, name in WATCH.items():
        if a <= address < a + 4:
            counts[a] += 1
            if len(seen) < 60:
                seen.append((m.vtime, name, address, value,
                             uc.reg_read(UC_ARM_REG_PC) & ~1))


m.uc.hook_add(UC_HOOK_MEM_WRITE, on_write, begin=0x200003E0, end=0x200004D0)
while m.insn_count < 12_000_000:
    m.run(1_000_000)
    print("[%9d] PC=%#x mode=%d req=%d root=%#x tick=%r"
          % (m.insn_count, m.uc.reg_read(UC_ARM_REG_PC) & ~1,
             m.uc.mem_read(0x200003E7, 1)[0],
             m.uc.mem_read(0x200004C4, 1)[0],
             struct.unpack("<I", bytes(m.uc.mem_read(0x20000470, 4)))[0],
             bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)).hex()))
print("\nwrite counts:", {WATCH[k]: v for k, v in counts.items()})
print("\nfirst writes (vtime, name, addr, value, pc):")
for s in seen:
    print("  t=%-9d %-12s %#x = %#x  pc=%#x" % (s[0], s[1], s[2], s[3], s[4]))
