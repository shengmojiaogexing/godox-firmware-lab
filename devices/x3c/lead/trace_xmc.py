"""Trace XMC (0x60000000-0x70000000) writes and hot code regions during boot."""
import os
import struct
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")

from machine import Machine  # noqa: E402

from boot6 import patch_option_bytes, patch_adc, patch_delay, map_xmc  # noqa: E402


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 30000000
    m = Machine(image=IMAGE, log_fn=lambda s: None, trace=False)
    patch_option_bytes(m)
    patch_adc(m)
    patch_delay(m)
    map_xmc(m)

    xmc_writes = Counter()      # (pc, addr, size) -> count
    xmc_first = []
    code_pcs = Counter()

    from unicorn import UC_HOOK_MEM_WRITE, UC_HOOK_CODE

    def mem_hook(uc, access, address, size, value, user):
        if 0x60000000 <= address < 0x70000000:
            pc = uc.reg_read(15)
            key = (pc & ~1, address, size)
            xmc_writes[key] += 1
            if len(xmc_first) < 200:
                xmc_first.append((pc & ~1, address, size, value))

    def code_hook(uc, address, size, user):
        code_pcs[address & ~1] += 1

    m.uc.hook_add(UC_HOOK_MEM_WRITE, mem_hook)
    m.uc.hook_add(UC_HOOK_CODE, code_hook, begin=0x08004800, end=0x08100000)
    m.boot()
    m.run(max_insns)
    print("insns:", m.insn_count, "PC=0x%08x" % m.reg("PC"))
    print("\n=== first XMC writes (pc, addr, size, value) ===")
    for row in xmc_first[:80]:
        print("  pc=0x%08x addr=0x%08x size=%d val=0x%x" % row)
    print("\n=== XMC write summary (top 30) ===")
    for (pc, addr, size), n in xmc_writes.most_common(30):
        print("  pc=0x%08x addr=0x%08x size=%d x%d" % (pc, addr, size, n))
    total_xmc = sum(xmc_writes.values())
    print("total XMC writes:", total_xmc, "distinct:", len(xmc_writes))
    print("\n=== hot code regions (top 25) ===")
    regs = Counter()
    for pc, n in code_pcs.items():
        regs[pc & ~0xFFF] += n
    for base, n in regs.most_common(25):
        print("  0x%08x: %d" % (base, n))
    print("\nhot mmio:\n" + m.hot_mmio_report(20))
    print("\nunknown mmio:\n" + m.unknown_mmio_report(20))


if __name__ == "__main__":
    main()
