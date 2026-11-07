# -*- coding: utf-8 -*-
"""动态定位触摸链路：fast 到主循环后切 fine，收集 GPIO 读写 / RAM u16 写入的 PC。"""
import collections
import struct
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import probe_ui as P  # noqa: E402
from unicorn import UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE  # noqa: E402

GPIO = [0x40020000, 0x40020400, 0x40020800, 0x40020C00, 0x40021000]
m = P.Machine(image=P.IMAGE, log_fn=lambda s: None, trace=False)
P.patch_option_bytes(m)
P.patch_adc(m, 2600)
P.patch_delays(m)

reads = collections.Counter()
writes = collections.Counter()


def hook_r(uc, access, address, size, value, user):
    reads[(uc.reg_read(UC_ARM_REG_PC) & ~1, address & ~3)] += 1


def hook_w(uc, access, address, size, value, user):
    writes[(uc.reg_read(UC_ARM_REG_PC) & ~1, address & ~3, value & 0xFFFF)] += 1


for base in GPIO:
    m.uc.hook_add(UC_HOOK_MEM_READ, hook_r, begin=base + 0x10, end=base + 0x13)   # IDR
    m.uc.hook_add(UC_HOOK_MEM_WRITE, hook_w, begin=base + 0x14, end=base + 0x1B)  # ODR/BSRR

m.boot()
m.set_fine(False)
tick = 0
done = 0
while done < 20_000_000:
    m.run(200_000)
    done = m.insn_count
    tick += 1
    m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick))
    m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
print("fast phase done insn=%d pc=0x%08x" % (done, m.reg("PC")))
m.set_fine(True)
m.run(200_000)
print("fine window done insn=%d pc=0x%08x" % (m.insn_count, m.reg("PC")))

print("=== GPIO IDR reads by PC (top 25) ===")
bypc = collections.Counter()
for (pc, addr), n in reads.items():
    bypc[pc] += n
for pc, n in bypc.most_common(25):
    print("  pc=0x%08x x%d" % (pc, n))
print("=== GPIO ODR/BSRR writes by PC (top 25) ===")
bypcw = collections.Counter()
for (pc, addr, v), n in writes.items():
    bypcw[pc] += n
for pc, n in bypcw.most_common(25):
    print("  pc=0x%08x x%d" % (pc, n))
