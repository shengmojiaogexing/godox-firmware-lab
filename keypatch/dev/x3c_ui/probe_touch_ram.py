# -*- coding: utf-8 -*-
"""动态：主循环稳态下，RAM 低区（0x20000000-0x20001000）的 2 字节写入的 PC/地址。"""
import collections
import struct
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import probe_ui as P  # noqa: E402
from unicorn import UC_HOOK_MEM_WRITE  # noqa: E402
from unicorn.arm_const import UC_ARM_REG_PC  # noqa: E402

m = P.Machine(image=P.IMAGE, log_fn=lambda s: None, trace=False)
P.patch_option_bytes(m)
P.patch_adc(m, 2600)
P.patch_delays(m)
m.boot()
m.set_fine(False)
tick = 0
done = 0
while done < 20_000_000:
    m.run(200_000)
    done = m.insn_count
    tick += 100
    m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
    m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")

hits = collections.Counter()
pcok = [0]


def hook(uc, access, address, size, value, user):
    pc = uc.reg_read(UC_ARM_REG_PC) & ~1
    if pcok[0] < 5 and pc == 0:
        pcok[0] += 1
    hits[(pc, address, size, value & 0xFFFF)] += 1


m.uc.hook_add(UC_HOOK_MEM_WRITE, hook, begin=0x20000000, end=0x20000FFF)
m.run(12_000_000)
print("insn=%d pc=0x%08x writes=%d" % (m.insn_count, m.reg("PC"), sum(hits.values())))
print("=== top write PCs ===")
bypc = collections.Counter()
for (pc, addr, size, val), n in hits.items():
    bypc[pc] += n
for pc, n in bypc.most_common(20):
    print("  pc=0x%08x x%d" % (pc, n))
print("=== u16 writes (size=2) top 30 by addr ===")
byaddr = collections.Counter()
for (pc, addr, size, val), n in hits.items():
    if size == 2:
        byaddr[(addr, pc)] += n
for (addr, pc), n in byaddr.most_common(30):
    print("  addr=0x%08x pc=0x%08x x%d" % (addr, pc, n))
