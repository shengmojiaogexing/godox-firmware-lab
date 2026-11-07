"""Lead's quick boot harness for the X3 emulator (uses devices/x3c/emu/machine.py)."""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")

from machine import Machine  # noqa: E402


def main():
    max_insns = int(sys.argv[1]) if len(sys.argv) > 1 else 200000
    logs = []

    def log(msg):
        logs.append(str(msg))

    m = Machine(image=IMAGE, log_fn=log, trace=True, trace_max=200000)
    sp, pc = m.boot()
    print("boot: SP=0x%08x PC=0x%08x" % (sp, pc))
    reason = m.run(max_insns)
    print("stop_reason:", reason)
    print("insn_count:", getattr(m, "insn_count", "?"))
    try:
        print("PC=0x%08x SP=0x%08x LR=0x%08x xPSR=0x%08x" % (
            m.reg("PC"), m.reg("SP"), m.reg("LR"), m.reg("XPSR")))
    except Exception as e:
        print("reg read failed:", e)
    print("\n--- last PCs ---")
    for a in m.pc_history(40):
        print("  0x%08x" % a)
    print("\n--- log (%d lines) ---" % len(logs))
    for line in logs[-80:]:
        print("  ", line)
    print("\n--- unknown mmio ---")
    try:
        print(m.unknown_mmio_report(40))
    except Exception as e:
        print("unknown_mmio_report failed:", e)
    print("\n--- hot mmio ---")
    try:
        print(m.hot_mmio_report(30))
    except Exception as e:
        print("hot_mmio_report failed:", e)
    return m


if __name__ == "__main__":
    main()
