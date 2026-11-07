#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Virtual TI CC2500 transceiver for the AD100 Pro firmware.

The firmware talks to the chip with a bit-banged 4-wire SPI (docs/03_rf_protocol.md):

    CS   = PC6   (active low)
    SCLK = PB13
    MOSI = PB12
    MISO = PB14  (input, pull-up)
    GDO0 = PA15  -> EXTI15 -> IRQ7   ("packet received")
    GDO2 = PA6   -> EXTI6  / polled

SPI byte timing (``SPI_WriteByte`` @0x0800CD7C): 8 bits MSB first, each bit:
SCLK low, set MOSI, SCLK high, small delay, read MISO, SCLK low.

Every transfer starts with an address byte:
  bit7 = 1 read / 0 write, bit6 = burst, bits5..0 = address.
Registers 0x00..0x2E are configuration, 0x30..0x3D are command strobes,
0x3E is PATABLE, 0x3F is TX FIFO (write) / RX FIFO (read).

This module decodes those transitions and keeps a register file, TX/RX FIFO
and GDO0 level.  Packets can be injected with :meth:`inject_rx`.
"""
import time

# pin map
CS_PORT, CS_BIT = 'GPIOC', 6
SCK_PORT, SCK_BIT = 'GPIOB', 13
MOSI_PORT, MOSI_BIT = 'GPIOB', 12
MISO_PORT, MISO_BIT = 'GPIOB', 14
GDO0_PORT, GDO0_BIT = 'GPIOA', 15

# CC2500 registers with sane defaults (values the driver reads back)
REG_DEFAULTS = {
    0x00: 0x29, 0x01: 0x2E, 0x02: 0x06, 0x03: 0x01, 0x04: 0x00, 0x05: 0x00,
    0x06: 0x04, 0x07: 0x00, 0x08: 0x00, 0x09: 0x00, 0x0A: 0x00, 0x0B: 0x06,
    0x0C: 0x00, 0x0D: 0x5C, 0x0E: 0xCE, 0x0F: 0xC4, 0x10: 0x00, 0x11: 0x3B,
    0x12: 0x73, 0x13: 0x22, 0x14: 0xF8, 0x15: 0x07, 0x16: 0x00, 0x17: 0x30,
    0x18: 0x00, 0x19: 0x00, 0x1A: 0x00, 0x1B: 0x00, 0x1C: 0x00, 0x1D: 0x00,
    0x1E: 0x00, 0x1F: 0x00, 0x20: 0x00, 0x21: 0x00, 0x22: 0x00, 0x23: 0x00,
    0x24: 0x00, 0x25: 0x00, 0x26: 0x00, 0x27: 0x00, 0x28: 0x00, 0x29: 0x00,
    0x2A: 0x00, 0x2B: 0x00, 0x2C: 0x88, 0x2D: 0x31, 0x2E: 0x0B,
}
# strobes actually used by the firmware (docs/05_cc2500_emu_spec.md):
#   0x30 SRES  0x33 SCAL  0x34 SRX  0x35 STX  0x36 SIDLE
#   0x39 flush-RX-FIFO (driver calls it SFRX)  0x3A flush-TX-FIFO  0x3B SWORRST
ST_SRES, ST_SCAL = 0x30, 0x33
ST_SRX, ST_STX, ST_SIDLE = 0x34, 0x35, 0x36
ST_SFRX, ST_SFTX, ST_SWORRST = 0x39, 0x3A, 0x3B

MARC_IDLE, MARC_RX, MARC_TX = 0x01, 0x0D, 0x13


class CC2500:
    def __init__(self, emu=None):
        self.emu = emu
        self.reset()

    # ------------------------------------------------------------------ state
    def reset(self):
        self.regs = dict(REG_DEFAULTS)
        self.patable = [0x3E] * 8
        self.tx_fifo = bytearray()
        self.rx_fifo = bytearray()
        self.state = MARC_IDLE
        self.gdo0 = 0
        self.cs = 1
        self.sck = 0
        self.mosi = 0
        # SPI decoder
        self._bit = 0
        self._in_byte = 0
        self._addr = 0
        self._read = False
        self._burst = False
        self._byte_idx = 0
        self._out_byte = 0
        self._out_next = 0
        # statistics / injections
        self.tx_packets = []
        self.rx_packets = []
        self.spi_writes = []
        self.spi_reads = []
        self.rx_count = 0
        self.tx_count = 0
        self.last_rx = None
        self.log = []
        self._clear_miso()

    def _gpio(self, name):
        try:
            return self.emu.devices[name]
        except Exception:
            return None

    def _clear_miso(self):
        gb = self._gpio(MISO_PORT)
        if gb is not None:
            gb.inject.pop(MISO_BIT, None)

    def _set_miso(self, bit):
        gb = self._gpio(MISO_PORT)
        if gb is not None:
            gb.inject[MISO_BIT] = 1 if bit else 0

    def _set_gdo0(self, level):
        self.gdo0 = 1 if level else 0
        ga = self._gpio(GDO0_PORT)
        if ga is not None:
            ga.inject[GDO0_BIT] = self.gdo0
        if self.gdo0:
            self.note('GDO0 high')
            self._trigger_exti15()

    def _trigger_exti15(self):
        try:
            self.emu.exti.pending |= (1 << 15)
            self.emu.scb.pending |= (1 << 7)      # EXTI4_15 -> IRQ7
            self.emu.note_irq('EXTI15', 'GDO0')
            self.emu.note_event('EXTI15', 1)
        except Exception:
            pass
        # the emulator's real IRQ dispatcher consumes this queue (exception 16+7)
        try:
            self.emu.request_irq(16 + 7, source='GDO0')
        except Exception:
            pass
        self.pending_gdo0 = True

    def note(self, msg):
        self.log.append((getattr(self.emu, 'insn_count', 0), msg))
        if len(self.log) > 2000:
            del self.log[:1000]

    # ------------------------------------------------------------------- pins
    def on_pins(self, port, setbits, clrbits, insn=0):
        """Called by the emulator on every GPIO BSRR/BRR/ODR write.

        Note that SCLK and MOSI share GPIOB, so a single write can change both;
        the bits are applied in hardware order (CS first, then MOSI, then SCLK,
        then CS rising edge ends the transaction).
        """
        cs_new = self.cs
        sck_new = self.sck
        mosi_new = self.mosi
        if port == CS_PORT:
            if setbits & (1 << CS_BIT):
                cs_new = 1
            elif clrbits & (1 << CS_BIT):
                cs_new = 0
        if port == SCK_PORT:
            if setbits & (1 << SCK_BIT):
                sck_new = 1
            elif clrbits & (1 << SCK_BIT):
                sck_new = 0
        if port == MOSI_PORT:
            if setbits & (1 << MOSI_BIT):
                mosi_new = 1
            elif clrbits & (1 << MOSI_BIT):
                mosi_new = 0

        old_cs = self.cs
        if cs_new != self.cs:
            self.cs = cs_new
            if cs_new == 0:
                self._begin_transaction()
        self.mosi = mosi_new
        if sck_new != self.sck:
            self.sck = sck_new
            if sck_new == 1:
                self._rising_edge()
        if old_cs == 0 and self.cs == 1:
            self._end_transaction()

    def _begin_transaction(self):
        self._bit = 0
        self._in_byte = 0
        self._byte_idx = 0
        self._read = False
        self._burst = False
        self._addr = 0
        self._out_byte = self._status_byte()
        self._out_next = 0
        self._set_miso(0)          # MISO is pulled down when idle

    def _rising_edge(self):
        # sample MOSI (MSB first)
        self._in_byte = ((self._in_byte << 1) | (1 if self.mosi else 0)) & 0xFF
        # drive MISO for the bit the master is about to read
        out_bit = (self._out_byte >> (7 - self._bit)) & 1
        self._set_miso(out_bit)
        self._bit += 1
        if self._bit == 8:
            self._bit = 0
            self._byte_done(self._in_byte)
            self._in_byte = 0
            self._out_byte = self._out_next

    def _end_transaction(self):
        self._clear_miso()
        # command strobes act on CS rising edge
        if not self._read and self._byte_idx == 1:
            self._strobe(self._addr)

    # --------------------------------------------------------------- protocol
    def _status_byte(self):
        # bit7 = CHIP_RDYn (0 = ready), bits6:4 = MARC_STATE & 7, bits3:0 = FIFO bytes
        st = self.state & 0x07
        avail = len(self.rx_fifo) if self.rx_fifo else len(self.tx_fifo)
        return ((st << 4) | (avail & 0x0F)) & 0xFF

    def _byte_done(self, b):
        if self._byte_idx == 0:
            # address byte
            self._read = bool(b & 0x80)
            self._burst = bool(b & 0x40)
            self._addr = b & 0x3F
            self._byte_idx = 1
            self._out_next = self._read_data_first()
            if not self._read:
                self._out_next = self._status_byte()
            return
        # data bytes
        if self._read:
            self.spi_reads.append((self._addr, b))
            self._out_next = self._read_data_next()
        else:
            self._write_data(b)
            self._out_next = self._status_byte()
        self._byte_idx += 1

    def _read_data_first(self):
        if self._addr == 0x3F:
            return self._rx_fifo_pop()
        if self._addr == 0x3E:
            return self.patable[0]
        if self._addr == 0x35:          # MARCSTATE
            return self.state
        if self._addr == 0x3B:          # RXBYTES (approx.)
            return len(self.rx_fifo) & 0xFF
        if self._addr == 0x3A:          # TXBYTES
            return (0x80 if self.tx_fifo else 0)
        return self.regs.get(self._addr, 0)

    def _read_data_next(self):
        if self._addr == 0x3F:
            return self._rx_fifo_pop()
        if self._addr == 0x3E:
            return self.patable[min(self._byte_idx - 1, 7)]
        return self.regs.get(self._addr, 0)

    def _rx_fifo_pop(self):
        if not self.rx_fifo:
            return 0
        b = self.rx_fifo[0]
        del self.rx_fifo[0]
        if not self.rx_fifo:
            self._set_gdo0(0)
            self.state = MARC_RX if self.state == MARC_RX else MARC_IDLE
        return b

    def _write_data(self, b):
        self.spi_writes.append((self._addr, b))
        if self._addr == 0x3F:          # TX FIFO
            self.tx_fifo.append(b)
        elif self._addr == 0x3E:        # PATABLE
            if not self._burst:
                self.patable[0] = b
            else:
                idx = min(self._byte_idx - 1, 7)
                self.patable[idx] = b
        else:
            self.regs[self._addr] = b

    def _strobe(self, addr):
        self.note('strobe 0x%02X' % addr)
        if addr == ST_SRES:
            keep_emu = self.emu
            keep_log = self.log
            keep_tx, keep_rx = self.tx_count, self.rx_count
            keep_pkts = self.tx_packets
            self.reset()
            self.emu = keep_emu
            self.log = keep_log
            self.tx_count, self.rx_count = keep_tx, keep_rx
            self.tx_packets = keep_pkts
        elif addr == ST_SRX:
            self.state = MARC_RX
        elif addr == ST_STX:
            self.state = MARC_TX
            pkt = bytes(self.tx_fifo)
            self.tx_fifo.clear()
            self.tx_count += 1
            self.tx_packets.append(pkt)
            self.note('TX %s' % pkt.hex())
            try:
                self.emu.note_event('RF_TX', pkt.hex())
            except Exception:
                pass
            self.state = MARC_RX
        elif addr == ST_SIDLE:
            self.state = MARC_IDLE
        elif addr == ST_SFRX:
            self.rx_fifo.clear()
            self._set_gdo0(0)
        elif addr == ST_SFTX:
            self.tx_fifo.clear()
        elif addr == ST_SCAL:
            self.state = MARC_IDLE

    # ------------------------------------------------------------------- API
    def inject_rx(self, payload, rssi=0x28, lqi=0x10, append_status=False):
        """Deliver a packet to the firmware as if received over the air."""
        payload = bytes(payload)
        fifo = bytearray()
        if append_status:
            fifo += payload + bytes([rssi, lqi])
        else:
            fifo += payload
        self.rx_fifo = fifo
        self.rx_count += 1
        self.last_rx = payload
        self.state = MARC_RX
        self._set_gdo0(1)
        self.note('RX %s' % payload.hex())
        try:
            self.emu.note_event('RF_RX', payload.hex())
        except Exception:
            pass
        return payload

    def state_dict(self):
        return dict(
            state={MARC_IDLE: 'IDLE', MARC_RX: 'RX', MARC_TX: 'TX'}.get(self.state, hex(self.state)),
            gdo0=self.gdo0,
            rx_fifo=bytes(self.rx_fifo).hex(),
            tx_fifo=bytes(self.tx_fifo).hex(),
            rx_count=self.rx_count,
            tx_count=self.tx_count,
            last_rx=self.last_rx.hex() if self.last_rx else None,
            last_tx=self.tx_packets[-1].hex() if self.tx_packets else None,
            freq2=self.regs.get(0x0D, 0),
            pktlen=self.regs.get(0x06, 0),
            patable=[hex(x) for x in self.patable],
            recent=self.log[-20:],
        )
