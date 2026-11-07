"""Lead boot harness v2: patches the boot-mode register page at 0x1FFFC000 and runs."""
import os
import sys
import struct

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")

from machine import Machine  # noqa: E402


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 2000000
    chunk = int(sys.argv[2]) if len(sys.argv) > 2 else 200000
    logs = []

    def log(msg):
        logs.append(str(msg))

    m = Machine(image=IMAGE, log_fn=log, trace=True, trace_max=400000)
    # boot-mode/status page used by the very first reset stub: [0x1FFFC010]&7==2 and
    # (halfword at 0x1FFFC002 & 0x60)==0 are required to continue boot instead of reset.
    try:
        m.uc.mem_map(0x1FFFC000, 0x1000)
    except Exception:
        pass
    m.uc.mem_write(0x1FFFC000, b"\x00" * 0x1000)
    m.uc.mem_write(0x1FFFC010, struct.pack("<H", 2))

    sp, pc = m.boot()
    print("boot SP=0x%08x PC=0x%08x" % (sp, pc))
    done = 0
    stop = None
    for i in range(0, max_insns, chunk):
        stop = m.run(chunk)
        done = getattr(m, "insn_count", (i + chunk))
        print("[%7d] stop=%-14s PC=0x%08x SP=0x%08x LR=0x%08x" % (
            done, stop, m.reg("PC"), m.reg("SP"), m.reg("LR")))
        if stop not in (None, "max_insn"):
            break
        if m.detect_hang(64):
            print("HANG detected at PC=0x%08x" % m.reg("PC"))
            break
    print("\n--- last 40 PCs ---")
    for a in m.pc_history(40):
        print("   0x%08x" % a)
    print("\n--- log tail ---")
    for line in logs[-40:]:
        print("  ", line)
    print("\n--- hot mmio ---")
    print(m.hot_mmio_report(40))
    print("\n--- unknown mmio ---")
    print(m.unknown_mmio_report(40))
    return m


if __name__ == "__main__":
    main()
