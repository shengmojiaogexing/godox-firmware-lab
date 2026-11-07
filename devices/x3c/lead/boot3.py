"""Lead boot harness v3: patch boot-mode page + the 0x40012000 ADC-like block, then run.

Logs where the firmware stops; designed for fast iteration on missing device models.
"""
import os
import sys
import struct

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")

from machine import Machine  # noqa: E402


class Device40012000:
    """Generic stand-in for the peripheral at 0x40012000 (ADC-like block).

    Observed firmware usage (file base 0x08004800):
      +0x00 : status polled with mask 0x40 -> must become 1
      +0x04 : control, read-modify-write
      +0x08 : control/status, bits 2/3 are 'busy' flags that must clear after being set
      +0x80 : config register, read-modify-write (set bit 10)
      others: storage
    """

    def __init__(self, base):
        self.base = base
        self.size = 0x100
        self.regs = {}

    def _get(self, off, default=0):
        return self.regs.get(off & ~3, default)

    def read(self, offset, size):
        off = offset & ~3
        if off == 0x00:
            return 0x40          # 'ready/done' bit the driver waits for
        if off == 0x08:
            return self._get(off) & ~0x0C   # busy bits 2/3 auto-clear
        if off == 0x04:
            return self._get(off)
        return self._get(off)

    def write(self, offset, size, value):
        off = offset & ~3
        if off == 0x08:
            v = self._get(off)
            v = (v & ~0xFFFFFFFF) | value
            self.regs[off] = v
            return
        self.regs[off] = value


def patch_devices(m):
    p = m.find_peripheral(0x40012000)
    print("peripheral @0x40012000 is", type(p).__name__ if p else None)
    dev = Device40012000(0x40012000)
    if p is not None:
        # replace delegate methods on the instance
        p.read = dev.read
        p.write = dev.write
        p.name = "DEV40012000"
    else:
        m.add_peripheral(dev)


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 5000000
    chunk = int(sys.argv[2]) if len(sys.argv) > 2 else 100000
    logs = []

    def log(msg):
        logs.append(str(msg))

    m = Machine(image=IMAGE, log_fn=log, trace=True, trace_max=400000)
    try:
        m.uc.mem_map(0x1FFFC000, 0x1000)
        m.uc.mem_write(0x1FFFC000, b"\x00" * 0x1000)
        m.uc.mem_write(0x1FFFC010, struct.pack("<H", 2))
    except Exception as e:
        print("boot page patch:", e)
    patch_devices(m)

    sp, pc = m.boot()
    print("boot SP=0x%08x PC=0x%08x" % (sp, pc))
    stop = None
    for i in range(0, max_insns, chunk):
        stop = m.run(chunk)
        pc = m.reg("PC")
        print("[%8d] stop=%-12s PC=0x%08x SP=0x%08x LR=0x%08x" % (
            m.insn_count, stop, pc, m.reg("SP"), m.reg("LR")))
        if stop not in (None, "max_insn"):
            break
        if m.detect_hang(128):
            print("HANG detected at PC=0x%08x" % pc)
            break
    print("\n--- last 30 PCs ---")
    for a in m.pc_history(30):
        print("   0x%08x" % a)
    print("\n--- log tail ---")
    for line in logs[-30:]:
        print("  ", line)
    print("\n--- hot mmio (top 25) ---")
    print(m.hot_mmio_report(25))
    print("\n--- unknown mmio ---")
    print(m.unknown_mmio_report(25))
    return m


if __name__ == "__main__":
    main()
