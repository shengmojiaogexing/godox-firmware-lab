"""Chunked boot + XMC/LCD write capture (no code hook; safe)."""
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

from unicorn import UC_HOOK_MEM_WRITE, UC_HOOK_MEM_READ  # noqa: E402


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 60000000
    chunk = int(sys.argv[2]) if len(sys.argv) > 2 else 5000000
    m = Machine(image=IMAGE, log_fn=lambda s: None, trace=True)
    patch_option_bytes(m)
    patch_adc(m)
    patch_delay(m)
    map_xmc(m)

    lcd_cmd = []
    lcd_data = []
    xmc_other = Counter()
    pc_xmc = Counter()

    def wr(uc, access, address, size, value, user):
        if 0x6C000000 <= address < 0x6C100000:
            pc = uc.reg_read(15) & ~1
            if address == 0x6C000000:
                if len(lcd_cmd) < 500:
                    lcd_cmd.append((pc, value, size))
            elif address == 0x6C000001:
                if len(lcd_data) < 500:
                    lcd_data.append((pc, value, size))
            else:
                xmc_other[(pc, address)] += 1
            pc_xmc[pc] += 1
        elif 0x60000000 <= address < 0x70000000:
            xmc_other[(uc.reg_read(15) & ~1, address)] += 1

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=0x60000000, end=0x6FFFFFFF)
    m.boot()
    for i in range(0, max_insns, chunk):
        st = m.run(chunk)
        print("[%10d] %-9s PC=0x%08x" % (m.insn_count, st, m.reg("PC")))
        if st not in (None, "max_insn"):
            break
    print("\nLCD command writes (%d):" % len(lcd_cmd))
    for pc, v, sz in lcd_cmd[:120]:
        print("  pc=0x%08x cmd=0x%02x size=%d" % (pc, v & 0xFF, sz))
    print("\nLCD data writes (%d):" % len(lcd_data))
    for pc, v, sz in lcd_data[:80]:
        print("  pc=0x%08x data=0x%02x size=%d" % (pc, v & 0xFF, sz))
    print("\nother XMC writes (top 20):")
    for (pc, addr), n in xmc_other.most_common(20):
        print("  pc=0x%08x addr=0x%08x x%d" % (pc, addr, n))
    print("\nXMC write PCs (top 20):")
    for pc, n in pc_xmc.most_common(20):
        print("  0x%08x x%d" % (pc, n))
    print("\nhot mmio:\n" + m.hot_mmio_report(15))
    print("\nunknown mmio:\n" + m.unknown_mmio_report(15))


if __name__ == "__main__":
    main()
