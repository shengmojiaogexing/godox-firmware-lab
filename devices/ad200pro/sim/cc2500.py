"""CC2500-compatible 2.4 GHz transceiver model (bit-banged 4-wire SPI).

Wiring (from the firmware):
    CSN = PC6 (active low)      SCK = PB13
    MOSI= PB12                  MISO= PB14
SPI mode 0, MSB first.
"""
DEFAULT_REGS = {
    0x00: 0x29, 0x01: 0x2E, 0x02: 0x06, 0x03: 0x07,
    0x04: 0xD3, 0x05: 0x91, 0x06: 0xFF, 0x07: 0x04, 0x08: 0x05,
    0x09: 0x00, 0x0A: 0x00, 0x0B: 0x0F, 0x0C: 0x00,
    0x0D: 0x5C, 0x0E: 0xCE, 0x0F: 0xC4,          # FREQ2/1/0 -> ~2413 MHz
    0x10: 0x6C, 0x11: 0x43, 0x12: 0x73, 0x13: 0x22, 0x14: 0xF8,
    0x15: 0x15, 0x16: 0x07, 0x17: 0x30, 0x18: 0x18, 0x19: 0x16,
    0x1A: 0x6C, 0x1B: 0x43, 0x1C: 0x40, 0x1D: 0x91,
    0x1E: 0x87, 0x1F: 0x6B, 0x20: 0xF8, 0x21: 0xB6, 0x22: 0x10,
    0x23: 0xEA, 0x24: 0x0A, 0x25: 0x00, 0x26: 0x11, 0x27: 0x41, 0x28: 0x00,
    0x2E: 0x09,
}
# read-only status registers
STATUS_REGS = {0x30: 0x00, 0x31: 0x00, 0x32: 0x00, 0x33: 0x00,
               0x34: 0x00, 0x35: 0x0D, 0x36: 0x00, 0x37: 0x00,
               0x38: 0x00, 0x39: 0x00, 0x3A: 0x00, 0x3B: 0x00,
               0x3C: 0x00, 0x3D: 0x00}
STROBES = {0x30: 'SRES', 0x31: 'SFSTXON', 0x32: 'SXOFF', 0x33: 'SCAL',
           0x34: 'SRX', 0x35: 'STX', 0x36: 'SIDLE', 0x37: 'SWOR',
           0x38: 'SPWD', 0x39: 'SFRX', 0x3A: 'SFTX', 0x3B: 'SWORRST',
           0x3C: 'SNOP', 0x3D: 'SNOP'}


class CC2500:
    def __init__(self, tracer=None):
        self.regs = dict(DEFAULT_REGS)
        self.tracer = tracer
        self.csn = 1
        self.sck = 0
        self.bits = 0
        self.shift = 0
        self.byte_index = 0
        self.reading = False
        self.burst = False
        self.addr = 0
        self.read_idx = 0
        self.out_bits = []
        self.tx_fifo = []
        self.rx_fifo = []
        self.log = []
        self.present = True

    # --------------------------------------------------------------- SPI
    def set_csn(self, level):
        if self.csn and not level:              # falling: new transaction
            self.bits = 0
            self.shift = 0
            self.byte_index = 0
            self.reading = False
            self.burst = False
            self.out_bits = []
        self.csn = level

    def clock(self, sck, mosi):
        """called on every SCK edge; return the MISO level to drive (None = keep)"""
        if self.csn:
            return None
        if not sck:
            return None
        out = self.out_bits.pop(0) if self.out_bits else 0
        self.shift = ((self.shift << 1) | (mosi & 1)) & 0xFF
        self.bits += 1
        if self.bits == 8:
            self.bits = 0
            self._byte(self.shift)
        return out

    def _byte(self, b):
        if self.byte_index == 0:
            self.byte_index = 1
            self.addr = b
            self.reading = bool(b & 0x80)
            self.burst = bool(b & 0x40)
            base = b & 0x7F
            if 0x30 <= base <= 0x3D and not self.reading:
                self._strobe(base)
                return
            if self.reading:
                self.read_idx = base
                self._queue_read_byte()
            return
        if self.reading:
            self.read_idx = (self.read_idx + 1) & 0x7F
            self._queue_read_byte()
            return
        # write data byte
        base = self.addr & 0x7F
        if base in (0x3E, 0x3F):
            self.tx_fifo.append(b)
            if self.tracer:
                self.tracer.add('rf_tx', name='CC2500 FIFO', data={'byte': b})
        else:
            self.regs[base & 0x7F] = b

    def _queue_read_byte(self):
        v = self.regs.get(self.read_idx, 0) if self.read_idx <= 0x2E else \
            STATUS_REGS.get(self.read_idx, 0)
        self.out_bits.extend([(v >> (7 - k)) & 1 for k in range(8)])

    def _strobe(self, b):
        name = STROBES.get(b, '?')
        self.log.append(name)
        if self.tracer:
            self.tracer.add('rf_tx', name='CC2500 %s' % name)
        if b == 0x39:      # SFRX
            self.rx_fifo = []
        elif b == 0x3A:    # SFTX
            self.tx_fifo = []
        elif b == 0x30:    # SRES
            self.regs = dict(DEFAULT_REGS)

    # ------------------------------------------------------------ packets
    def inject_rx(self, payload):
        self.rx_fifo.extend(payload)

    def pop_tx(self):
        out = bytes(self.tx_fifo)
        self.tx_fifo = []
        return out

    def state(self):
        return {'regs': {'0x%02x' % k: '0x%02x' % v for k, v in sorted(self.regs.items())},
                'tx_fifo': list(self.tx_fifo), 'rx_fifo': list(self.rx_fifo),
                'strobes': self.log[-20:]}
