# -*- coding: utf-8 -*-
"""动态找触摸链路：统计主循环稳态下 GPIO IDR 读 / ODR 写 / BSRR 写的 PC。"""
import collections
import struct
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import probe_ui as P  # noqa: E402
from unicorn import UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE  # noqa: E402

GPIO_IDR = [0x40020010, 0x40020410, 0x40020810, 0x40020C10, 0x40021010,
            0x40021410, 0x40021810]
GPIO_ODR_BSRR = [(0x40020014, 0x40020018), (0x40020414, 0x40020418),
                 (0x40020814, 0x40020818), (0x40020C14, 0x40020C18),
                 (0x40021014, 0x40021018)]

m = P.Machine(image=P.IMAGE, log_fn=lambda s: None, trace=False)
P.patch_option_bytes(m)
P.patch_adc(m, 2600)
P.patch_delays(m)
reads = collections.Counter()
writes = collections.Counter()
odr = collections.Counter()


def hook_r(uc, access, address, size, value, user):
    reads[(uc.reg_read(UC_ARM_REG_PC) & ~1, address)] += 1


def hook_w(uc, access, address, size, value, user):
    writes[(uc.reg_read(UC_ARM_REG_PC) & ~1, address, value & 0xFFFF)] += 1
    odr[(uc.reg_read(UC_ARM_REG_PC) & ~1, address)] += 1


for a in GPIO_IDR:
    m.uc.hook_add(UC_HOOK_MEM_READ, hook_r, begin=a, end=a + 3)
for lo, hi in GPIO_ODR_BSRR:
    m.uc.hook_add(UC_HOOK_MEM_WRITE, hook_w, begin=lo, end=hi + 3)

m.boot()
m.set_fine(False)
tick = 0
done = 0
while done < 30_000_000:
    m.run(200_000)
    done = m.insn_count
    tick += 1
    m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick))
    m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")

print("=== GPIO IDR reads (top 20) ===")
for (pc, addr), n in reads.most_common(20):
    f = P.__dict__.get("x")  # unused
    print("  pc=0x%08x addr=0x%08x x%d" % (pc, addr, n))
print("=== GPIO ODR/BSRR writes (top 20) ===")
for (pc, addr), n in odr.most_common(20):
    print("  pc=0x%08x addr=0x%08x x%d" % (pc, addr, n))
