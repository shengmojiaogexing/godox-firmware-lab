"""Boot with a plausible battery ADC value; capture LCD frames to PNG."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")

from machine import Machine  # noqa: E402
from boot6 import patch_option_bytes, patch_delay, map_xmc  # noqa: E402
from lcd_capture import LcdBus  # noqa: E402

from unicorn import UC_HOOK_MEM_WRITE, UC_HOOK_MEM_READ  # noqa: E402


class AdcBlock2:
    """AT32 ADC with a configured-conversion model.

    status @0x00: ADRDY/CCEOC-ish bits used by the driver
    ctrl @0x04, @0x08: software start bits
    data @0x4C (RDATA): battery voltage sample (raw, set by run script)
    """

    def __init__(self, raw=2600):
        self.regs = {}
        self.raw = raw
        self.reads = {}

    def read(self, offset, size):
        off = offset & ~3
        self.reads[off] = self.reads.get(off, 0) + 1
        if off == 0x00:
            return 0x40 | 0x02
        if off == 0x08:
            return self.regs.get(off, 0) & ~0x0C
        if off == 0x4C:
            return self.raw
        if off in (0x3C, 0x40, 0x44, 0x48):
            return self.raw
        if off == 0x10:
            return self.raw
        return self.regs.get(off, 0)

    def write(self, offset, size, value):
        self.regs[offset & ~3] = value


def patch_adc2(m, raw):
    p = m.find_peripheral(0x40012000)
    dev = AdcBlock2(raw)
    if p is None:
        m.add_peripheral(dev)
    else:
        p.read = dev.read
        p.write = dev.write
        p.name = "ADC2"
    return dev


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 10000000
    chunk = int(sys.argv[2]) if len(sys.argv) > 2 else 2000000
    raw = int(sys.argv[3]) if len(sys.argv) > 3 else 2600
    m = Machine(image=IMAGE, log_fn=lambda s: None, trace=True)
    patch_option_bytes(m)
    dev = patch_adc2(m, raw)
    patch_delay(m)
    map_xmc(m)
    bus = LcdBus()
    os.makedirs(os.path.join(HERE, "lcd_out"), exist_ok=True)
    adc_reads = {}

    def wr(uc, access, address, size, value, user):
        if address == 0x6C000000:
            bus.write_addr0(value & 0xFF)
        elif address == 0x6C000001:
            bus.write_addr1(value & 0xFF)

    def rd(uc, access, address, size, value, user):
        if 0x40012000 <= address < 0x40012100:
            o = address - 0x40012000
            adc_reads[o] = adc_reads.get(o, 0) + 1

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=0x6C000000, end=0x6C000002)
    m.uc.hook_add(UC_HOOK_MEM_READ, rd, begin=0x40012000, end=0x40012100)
    m.boot()
    for i in range(0, max_insns, chunk):
        st = m.run(chunk)
        print("[%10d] %-9s PC=0x%08x pixels=%d cmds=%d win=(%d,%d)-(%d,%d)" % (
            m.insn_count, st, m.reg("PC"), bus.n_pixels, len(bus.cmds),
            bus.xs, bus.xe, bus.ys, bus.ye))
        if bus.n_pixels:
            print("   snapshot:", bus.snapshot("%09d" % m.insn_count))
        if st not in (None, "max_insn"):
            break
    print("\ncommands:", " ".join("%02x" % c for c in bus.cmds[:120]))
    print("ADC register reads (offset->count):", {hex(k): v for k, v in sorted(adc_reads.items())})
    print("final:", bus.snapshot("final_raw%d" % raw))


if __name__ == "__main__":
    main()
