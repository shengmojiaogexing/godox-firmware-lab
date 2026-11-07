#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Debug the virtual CC2500 SPI decoder against real firmware activity."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'emu'))
import core  # noqa: E402

PIN_BITS = {('GPIOB', 0x1000): 'MOSI', ('GPIOB', 0x2000): 'SCK', ('GPIOC', 0x40): 'CS'}


def main():
    e = core.Emulator()
    e.reset()
    e.skip_to_main_loop(1000000)
    cc = e.cc2500
    events = []
    orig = cc.on_pins

    def hook(port, setbits, clrbits, insn=0):
        for (p, bit), name in PIN_BITS.items():
            if p != port:
                continue
            if setbits & bit:
                events.append((insn, name, 1))
            if clrbits & bit:
                events.append((insn, name, 0))
        return orig(port, setbits, clrbits, insn)

    cc.on_pins = hook
    e.run(400000)
    print('pin events captured:', len(events))
    # find a CS falling edge and print the following events
    start = None
    for i, ev in enumerate(events):
        if ev[1] == 'CS' and ev[2] == 0:
            start = i
            break
    print('first CS-low at event', start)
    for ev in events[start:start + 120]:
        print('  insn=%-9d %-5s %d' % ev)
    print('decoded writes:', [(hex(a), hex(b)) for a, b in cc.spi_writes[-20:]])
    print('decoded reads :', [(hex(a), hex(b)) for a, b in cc.spi_reads[-10:]])
    print('tx fifo', bytes(cc.tx_fifo).hex(), 'state', cc.state)


if __name__ == '__main__':
    main()
