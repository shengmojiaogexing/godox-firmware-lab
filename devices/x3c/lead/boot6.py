"""Lead boot harness v6: full boot attempt with AT32F435 external memory (SDRAM/LCD at 0x6C000000).

Patches:
  * option bytes at 0x1FFFC000 (provisioned device)
  * ADC-like block at 0x40012000 (AT32 ADC: ADRDY poll, busy bits)
  * software delay functions (nop-sled) -> fast forward
  * maps external memory window 0x60000000-0x70000000 (XMC banks/SDRAM, 0x6C000000 used)
Runs in chunks; at the end dumps candidate framebuffers from SRAM and external RAM.
"""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")
BASE = 0x08004800

from machine import Machine  # noqa: E402


def patch_option_bytes(m):
    try:
        m.uc.mem_map(0x1FFFC000, 0x1000)
    except Exception:
        pass
    m.uc.mem_write(0x1FFFC000, b"\x00" * 0x1000)
    m.uc.mem_write(0x1FFFC010, struct.pack("<H", 2))
    m.uc.mem_write(0x1FFFC002, struct.pack("<H", 0x000F))


class AdcBlock:
    def __init__(self):
        self.regs = {}

    def read(self, offset, size):
        off = offset & ~3
        if off == 0x00:
            return 0x40
        if off == 0x08:
            return self.regs.get(off, 0) & ~0x0C
        return self.regs.get(off, 0)

    def write(self, offset, size, value):
        self.regs[offset & ~3] = value


def patch_adc(m):
    p = m.find_peripheral(0x40012000)
    dev = AdcBlock()
    if p is None:
        m.add_peripheral(dev)
    else:
        p.read = dev.read
        p.write = dev.write
        p.name = "ADC-LIKE"


def patch_delay(m):
    for addr in (0x0801C118, 0x0801C0FE):
        m.uc.mem_write(addr, b"\x70\x47")


def map_xmc(m):
    # AT32F435 XMC external memory banks (SDRAM/LCD). Firmware writes 0x6C000000.
    for base, size in ((0x60000000, 0x1000000),):
        try:
            m.uc.mem_map(base, size)
        except Exception as e:
            print("xmc map:", e)


def dump_candidates(m):
    """Look for plausible framebuffer content in SRAM and external RAM."""
    out = []
    regions = [(0x20000000, 0x80000, "SRAM")]
    try:
        m.uc.mem_read(0x60000000, 4)
        regions.append((0x60000000, 0x1000000, "XMC"))
    except Exception:
        pass
    for base, size, name in regions:
        try:
            data = bytes(m.uc.mem_read(base, size))
        except Exception as e:
            print("read %s failed: %s" % (name, e))
            continue
        # entropy per 32KB block to find graphics/font data
        for off in range(0, size - 0x8000, 0x8000):
            blk = data[off:off + 0x8000]
            nz = sum(1 for b in blk if b)
            if nz > 0x8000 * 0.10:
                uniq = len(set(blk))
                out.append((name, base + off, nz, uniq))
    return out


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 60000000
    chunk = int(sys.argv[2]) if len(sys.argv) > 2 else 5000000
    m = Machine(image=IMAGE, log_fn=lambda s: None, trace=True, trace_max=100000)
    patch_option_bytes(m)
    patch_adc(m)
    patch_delay(m)
    map_xmc(m)
    m.boot()
    print("boot SP=0x%08x PC=0x%08x" % (m.reg("SP"), m.reg("PC")))
    stops = []
    for i in range(0, max_insns, chunk):
        st = m.run(chunk)
        pc = m.reg("PC")
        print("[%10d] %-9s PC=0x%08x SP=0x%08x LR=0x%08x R0=0x%x" % (
            m.insn_count, st, pc, m.reg("SP"), m.reg("LR"), m.reg("R0")))
        stops.append(pc)
        if st not in (None, "max_insn"):
            break
    print("\nhot mmio:\n" + m.hot_mmio_report(25))
    print("\nunknown mmio:\n" + m.unknown_mmio_report(25))
    print("\nmemory candidates (region, addr, nonzero, unique):")
    for row in dump_candidates(m):
        print("   %s 0x%08x nz=%d uniq=%d" % row)
    # dump external writes log if any
    return m


if __name__ == "__main__":
    main()
