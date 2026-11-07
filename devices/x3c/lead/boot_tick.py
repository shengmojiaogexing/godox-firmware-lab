"""Boot UI attempt v2: advance the firmware's 1 ms tick counter (RAM 0x20000310) manually.

The firmware's 1 ms time base comes from TMR7 interrupt (IRQ55 -> 0x08016D50 -> tick
updater 0x080334EC, which adds to RAM[0x20000310]).  Our emulator does not tick TMR7 yet,
so all scheduler timers stall and the UI stays on the boot/battery screen.  Here we
increment the tick counter ourselves every chunk of instructions and capture LCD frames.
"""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(HERE, "..", "emu"))

IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")

from machine import Machine  # noqa: E402
from boot6 import patch_option_bytes, patch_delay, map_xmc  # noqa: E402
from boot_ui import AdcBlock2  # noqa: E402
from lcd_capture import LcdBus  # noqa: E402

from unicorn import UC_HOOK_MEM_WRITE  # noqa: E402


def main():
    total = int(sys.argv[1]) if len(sys.argv) > 1 else 20000000
    chunk = int(sys.argv[2]) if len(sys.argv) > 2 else 20000
    raw = int(sys.argv[3]) if len(sys.argv) > 3 else 2600
    m = Machine(image=IMAGE, log_fn=lambda s: None, trace=True)
    patch_option_bytes(m)
    p = m.find_peripheral(0x40012000)
    dev = AdcBlock2(raw)
    if p is None:
        m.add_peripheral(dev)
    else:
        p.read = dev.read
        p.write = dev.write
        p.name = "ADC2"
    patch_delay(m)
    map_xmc(m)
    bus = LcdBus()
    out = os.path.join(HERE, "lcd_out")
    os.makedirs(out, exist_ok=True)

    def wr(uc, access, address, size, value, user):
        if address == 0x6C000000:
            bus.write_addr0(value & 0xFF)
        elif address == 0x6C000001:
            bus.write_addr1(value & 0xFF)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=0x6C000000, end=0x6C000002)
    m.boot()
    tick = 0
    done = 0
    last = 0
    while done < total:
        st = m.run(chunk)
        done += chunk
        tick += 1                               # 1 chunk == 1 virtual ms
        try:
            m.uc.mem_write(0x20000310, struct.pack("<I", tick & 0xFFFFFFFF))
            m.uc.mem_write(0x2000030C, b"\x00")
        except Exception:
            pass
        if bus.n_pixels != last:
            print("[%9d] tick=%d pixels=%d cmds=%d pc=0x%08x" % (
                done, tick, bus.n_pixels, len(bus.cmds), m.reg("PC")))
            print("   snapshot:", bus.snapshot("%09d" % done))
            last = bus.n_pixels
        if st not in (None, "max_insn"):
            print("stop:", st)
            break
    print("\ncommands:", " ".join("%02x" % c for c in bus.cmds[:160]))
    print("final:", bus.snapshot("tick_final"))


if __name__ == "__main__":
    main()
