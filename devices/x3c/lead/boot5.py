"""Lead boot harness v5: option bytes, ADC-like block, and software-delay fast-forward.

Boots the X3 firmware and reports where it stops. All patches are documented emulator
accommodations (see devices/x3c/emu/FINDINGS.md).
"""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")
BASE = 0x08004800

from machine import Machine  # noqa: E402


def patch_delay(m):
    """Replace the calibrated nop-sled delay functions with `bx lr` (0x4770).

    0x0801C118 = delay_us(n): 17x nop sled loop inside a count loop
    0x0801C0FE = delay_ms(n): calls delay_us(1000) n times
    """
    for addr in (0x0801C118, 0x0801C0FE):
        m.uc.mem_write(addr, b"\x70\x47")


def patch_option_bytes(m):
    """STM32 option-byte area at 0x1FFFC000: emulate an already-provisioned device so the
    one-time provisioning path in 0x08020A20 does not trigger SYSRESETREQ."""
    try:
        m.uc.mem_map(0x1FFFC000, 0x1000)
    except Exception:
        pass
    m.uc.mem_write(0x1FFFC000, b"\x00" * 0x1000)
    m.uc.mem_write(0x1FFFC010, struct.pack("<H", 2))
    m.uc.mem_write(0x1FFFC002, struct.pack("<H", 0x000F))


class AdcBlock:
    """Stand-in for the ADC-like block at 0x40012000.

    Firmware: +0x00 status polled with mask 0x40; +0x04/+0x08 control RMW with busy
    bits 2/3 in +0x08; +0x80 config RMW (bit 10)."""

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


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 30000000
    chunk = int(sys.argv[2]) if len(sys.argv) > 2 else 2000000
    quiet = "--quiet" in sys.argv
    logs = []

    def log(s):
        logs.append(str(s))

    m = Machine(image=IMAGE, log_fn=log, trace=True, trace_max=100000)
    patch_option_bytes(m)
    patch_adc(m)
    patch_delay(m)
    m.boot()
    print("boot SP=0x%08x PC=0x%08x" % (m.reg("SP"), m.reg("PC")))
    prevpc = None
    for i in range(0, max_insns, chunk):
        stop = m.run(chunk)
        pc = m.reg("PC")
        if not quiet:
            print("[%10d] %-10s PC=0x%08x SP=0x%08x LR=0x%08x" % (
                m.insn_count, stop, pc, m.reg("SP"), m.reg("LR")))
        if stop not in (None, "max_insn"):
            break
        if prevpc is not None and abs(pc - prevpc) < 0x40:
            pass
        prevpc = pc
    print("\nlast PCs:", " ".join("0x%08x" % a for a in m.pc_history(24)))
    print("\nhot mmio:\n" + m.hot_mmio_report(30))
    print("\nunknown mmio:\n" + m.unknown_mmio_report(30))
    print("\nregs: PC=0x%08x LR=0x%08x SP=0x%08x R0=0x%x R1=0x%x R2=0x%x R3=0x%x" % (
        m.reg("PC"), m.reg("LR"), m.reg("SP"), m.reg("R0"), m.reg("R1"), m.reg("R2"), m.reg("R3")))
    print("\nlog:", logs[-20:])
    return m


if __name__ == "__main__":
    main()
