#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Unit tests for the virtual CC2500 SPI model (no firmware needed).

Run:  ..\..\tools\python\python.exe tools\test_cc2500.py
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'emu'))
import cc2500 as m  # noqa: E402


class FakeGPIO:
    def __init__(self):
        self.inject = {}


class FakeEXTI:
    pending = 0


class FakeSCB:
    pending = 0


class FakeEmu:
    def __init__(self):
        self.devices = {'GPIOA': FakeGPIO(), 'GPIOB': FakeGPIO(), 'GPIOC': FakeGPIO()}
        self.insn_count = 0
        self.exti = FakeEXTI()
        self.scb = FakeSCB()
        self.irqs = []
        self.events = []

    def note_irq(self, what, value):
        self.irqs.append((what, value))

    def note_event(self, what, value):
        self.events.append((what, value))


class Spi:
    """Drive the CC2500 model like the firmware bit-bang SPI does."""

    def __init__(self, emu, cc):
        self.emu = emu
        self.cc = cc

    def pin(self, port, setb=0, clrb=0):
        self.cc.on_pins(port, setb, clrb, 0)

    def begin(self):
        self.pin('GPIOC', clrb=0x40)

    def end(self):
        self.pin('GPIOC', setb=0x40)

    def write_byte(self, v):
        for i in range(8):
            bit = (v >> (7 - i)) & 1
            self.pin('GPIOB', clrb=0x2000)
            if bit:
                self.pin('GPIOB', setb=0x1000)
            else:
                self.pin('GPIOB', clrb=0x1000)
            self.pin('GPIOB', setb=0x2000)   # rising edge: slave samples MOSI
            self.pin('GPIOB', clrb=0x2000)

    def read_byte(self):
        out = 0
        for _ in range(8):
            self.pin('GPIOB', setb=0x2000)   # rising edge: slave drives MISO
            bit = self.emu.devices['GPIOB'].inject.get(14, 1)
            self.pin('GPIOB', clrb=0x2000)
            out = (out << 1) | (1 if bit else 0)
        return out

    def xfer(self, addr_byte, data=(), read_len=0):
        self.begin()
        self.write_byte(addr_byte)
        outs = []
        for b in data:
            self.write_byte(b)
        for _ in range(read_len):
            outs.append(self.read_byte())
        self.end()
        return outs


def test_register_write():
    emu = FakeEmu()
    cc = m.CC2500(emu)
    spi = Spi(emu, cc)
    spi.xfer(0x0D, [0x5E])              # FREQ2 = 0x5E
    assert cc.regs[0x0D] == 0x5E, hex(cc.regs[0x0D])
    spi.xfer(0x06, [0x04])              # PKTLEN = 4
    assert cc.regs[0x06] == 4


def test_register_read():
    emu = FakeEmu()
    cc = m.CC2500(emu)
    cc.regs[0x0D] = 0x5C
    spi = Spi(emu, cc)
    vals = spi.xfer(0x8D, read_len=1)   # read FREQ2
    assert vals == [0x5C], vals
    cc.state = m.MARC_RX
    vals = spi.xfer(0xB5, read_len=1)   # MARCSTATE
    assert vals == [m.MARC_RX], vals


def test_patable_burst():
    emu = FakeEmu()
    cc = m.CC2500(emu)
    spi = Spi(emu, cc)
    spi.xfer(0x7E, [0x3E, 0x2E, 0x1E])  # burst write PATABLE (0x3E|0x40)
    assert cc.patable[:3] == [0x3E, 0x2E, 0x1E], cc.patable


def test_tx_fifo_and_stx():
    emu = FakeEmu()
    cc = m.CC2500(emu)
    spi = Spi(emu, cc)
    spi.xfer(0x34)                       # SRX
    assert cc.state == m.MARC_RX
    spi.xfer(0x7F, [0xD5, 0x01, 0x02, 0x03])   # burst write TX FIFO
    assert bytes(cc.tx_fifo) == b'\xd5\x01\x02\x03', bytes(cc.tx_fifo)
    spi.xfer(0x35)                       # STX
    assert cc.tx_count == 1
    assert cc.tx_packets[0] == b'\xd5\x01\x02\x03'
    assert cc.state == m.MARC_RX


def test_rx_inject_and_read():
    emu = FakeEmu()
    cc = m.CC2500(emu)
    spi = Spi(emu, cc)
    cc.inject_rx(b'\xA9\x50\xB4\x02')
    assert cc.gdo0 == 1
    assert emu.devices['GPIOA'].inject.get(15) == 1
    assert emu.exti.pending & (1 << 15)
    assert emu.scb.pending & (1 << 7)
    vals = spi.xfer(0xFF, read_len=4)    # burst read RX FIFO
    assert vals == [0xA9, 0x50, 0xB4, 0x02], vals
    assert cc.gdo0 == 0, 'GDO0 must drop when the FIFO becomes empty'


def test_sfrx():
    emu = FakeEmu()
    cc = m.CC2500(emu)
    spi = Spi(emu, cc)
    cc.inject_rx(b'\xD5\x01\x02\x03')
    spi.xfer(0x39)                       # flush RX FIFO
    assert len(cc.rx_fifo) == 0
    assert cc.gdo0 == 0


def test_sres_resets_registers():
    emu = FakeEmu()
    cc = m.CC2500(emu)
    cc.regs[0x0D] = 0x11
    spi = Spi(emu, cc)
    spi.xfer(0x30)                       # SRES
    assert cc.regs[0x0D] == 0x5C, hex(cc.regs[0x0D])


def main():
    tests = [v for k, v in sorted(globals().items()) if k.startswith('test_')]
    failed = 0
    for fn in tests:
        try:
            fn()
            print(f'PASS  {fn.__name__}')
        except AssertionError as e:
            failed += 1
            print(f'FAIL  {fn.__name__}: {e}')
        except Exception as e:  # noqa: BLE001
            failed += 1
            print(f'ERROR {fn.__name__}: {type(e).__name__}: {e}')
    print(f'\n{len(tests) - failed}/{len(tests)} passed')
    return 1 if failed else 0


if __name__ == '__main__':
    sys.exit(main())
