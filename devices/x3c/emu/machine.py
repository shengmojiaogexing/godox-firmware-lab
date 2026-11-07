"""machine.py -- Cortex-M firmware emulation machine for the Godox X3-C trigger.

Design goals
------------
* Load a raw flash image at an arbitrary base (X3-C app is linked at 0x08004800;
  a small fake bootloader region is provided at 0x08000000 because the app reset
  handler reads the stack pointer from word 0 of real flash).
* Cortex-M4/M7 peripheral space modelled through Unicorn ``mmio_map`` callbacks:
  PC-relative peripheral register models with ready bits (RCC, FLASH, GPIO,
  SPI, UART, I2C, RTC, IWDG, TIM, ADC, DMA, USB, FMC, CRC, RNG, EXTI...).
* Full PPB model: SCB (CPUID/ICSR/VTOR/AIRCR reset/SHCSR/CFSR/HFSR/CPACR/MPU),
  SysTick (CTRL/LOAD/VAL/CALIB with virtual tick counting and IRQ delivery),
  NVIC (ISER/ICER/ISPR/ICPR/IPR/STIR + pending IRQ dispatch into the vector
  table at instruction-chunk boundaries), DWT (CYCCNT), ITM/TPIU stubs.
* MMIO access log (ring buffer), per-address counters, CSV export, hot-MMIO
  summary and "unknown MMIO" bookkeeping with the PC that touched it.
* Access trace of every instruction PC (ring buffer) for hang analysis.
* Optional call trace (BL/BLX) with target histogram.
* Breakpoints / watchpoints / step / run-until, implemented as Unicorn hooks.
* Exception dispatch and return (EXC_RETURN handling) in the run loop.

Everything is deliberately plain-python and dependency-free (no PIL required;
PNG writing lives in display.py using zlib).

Usage from python::

    from machine import Machine
    m = Machine()                       # loads default X3-C image
    m.reset_from_vector()               # SP/PC from the vector table
    m.run(max_insns=200000)
    print(m.summary())
"""
from __future__ import annotations

import os
import struct
import zlib
import json
import time
from collections import deque, Counter, OrderedDict

from unicorn import (
    Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
    UC_HOOK_CODE, UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE,
    UC_HOOK_MEM_READ_UNMAPPED, UC_HOOK_MEM_WRITE_UNMAPPED,
    UC_HOOK_MEM_FETCH_UNMAPPED, UC_HOOK_MEM_INVALID,
    UC_MEM_READ_UNMAPPED, UC_MEM_WRITE_UNMAPPED, UC_MEM_FETCH_UNMAPPED,
    UC_MEM_FETCH_PROT, UC_MEM_READ_PROT, UC_MEM_WRITE_PROT,
    UC_PROT_ALL, UC_PROT_READ, UC_PROT_WRITE, UC_PROT_EXEC,
    UC_ERR_INSN_INVALID, UC_ERR_FETCH_UNMAPPED, UC_ERR_READ_UNMAPPED,
    UC_ERR_WRITE_UNMAPPED,
)
from unicorn.arm_const import (
    UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3,
    UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6, UC_ARM_REG_R7,
    UC_ARM_REG_R8, UC_ARM_REG_R9, UC_ARM_REG_R10, UC_ARM_REG_R11,
    UC_ARM_REG_R12, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC,
)

# --------------------------------------------------------------------------
# Default X3-C layout
# --------------------------------------------------------------------------
DEFAULT_IMAGE = os.path.join(os.path.dirname(os.path.abspath(__file__)),
                             "..", "firmware", "X3_C_v1.38_app.bin")
DEFAULT_IMAGE = os.path.normpath(DEFAULT_IMAGE)

APP_BASE = 0x08004800        # linker base of the main application
FLASH_BASE = 0x08000000
FLASH_SIZE = 0x00200000      # 2 MB (app image ends at 0x080FDE1C)
RAM_BASE = 0x20000000
RAM_SIZE = 0x00080000        # 512 KB
CCM_BASE = 0x10000000        # Cortex-M4 CCM (optional)
CCM_SIZE = 0x00010000
PERIPH_BASE = 0x40000000
PERIPH_SIZE = 0x00080000     # AHB1/AHB2/APB1/APB2
USB_BASE = 0x50000000
USB_SIZE = 0x00080000
FMC_BASE = 0xA0000000
FMC_SIZE = 0x00100000
PPB_BASE = 0xE0000000
PPB_SIZE = 0x00100000

EXC_RETURN_MASK = 0xFFFFFF00
UNTIL_MAGIC = 0xFFFFFFFE


def ror(v, n=1):
    return ((v >> n) | (v << (32 - n))) & 0xFFFFFFFF


class _Stop(Exception):
    """internal: abort the run loop with a reason."""


class MachineFault(Exception):
    def __init__(self, msg, pc=None):
        super().__init__(msg)
        self.pc = pc


# ==========================================================================
# Access trace
# ==========================================================================
class AccessTrace:
    """Ring buffer of MMIO accesses plus per-address aggregation."""

    def __init__(self, capacity=200000):
        self.capacity = capacity
        self.ring = deque(maxlen=capacity)
        self.count = 0
        self.per_addr = Counter()          # (dir, addr, size) -> count
        self.per_addr_pc = {}              # (dir, addr) -> [pc samples]
        self.per_periph = Counter()        # periph name -> count
        self.unknown = OrderedDict()       # addr -> [read_count, write_count, pcs]
        self.tx_bytes = 0                  # stat counters
        self.rx_bytes = 0

    def add(self, vtime, pc, is_write, addr, size, value, region=None):
        self.count += 1
        if self.ring.maxlen:
            self.ring.append((vtime, pc, 1 if is_write else 0, addr, size,
                              value, region))
        key = ("W" if is_write else "R", addr, size)
        self.per_addr[key] += 1
        pcs = self.per_addr_pc.setdefault(("W" if is_write else "R", addr), [])
        if len(pcs) < 4 and pc not in pcs:
            pcs.append(pc)
        if region:
            self.per_periph[region] += 1

    def note_unknown(self, addr, is_write, pc, size, value):
        ent = self.unknown.get(addr)
        if ent is None:
            if len(self.unknown) > 4096:
                return
            ent = [0, 0, []]
            self.unknown[addr] = ent
        ent[1 if is_write else 0] += 1
        if len(ent[2]) < 4 and pc not in ent[2]:
            ent[2].append((pc, "W" if is_write else "R", size, value))

    def csv(self, path):
        with open(path, "w", encoding="utf-8") as f:
            f.write("vtime,pc,dir,addr,size,value,region\n")
            for (vt, pc, w, addr, size, val, region) in self.ring:
                f.write("%d,0x%08X,%s,0x%08X,%d,0x%X,%s\n" % (
                    vt, pc, "W" if w else "R", addr, size, val, region or ""))

    def hot(self, n=40):
        rows = []
        for (d, addr, size), cnt in self.per_addr.most_common(n):
            rows.append((cnt, d, addr, size,
                         self.per_addr_pc.get((d, addr), [])))
        return rows


# ==========================================================================
# Peripheral framework
# ==========================================================================
class Peripheral:
    """Base class: a named register window with byte/word access."""

    def __init__(self, machines, name, base, size, desc=""):
        self.m = machines
        self.name = name
        self.base = base
        self.size = size
        self.desc = desc
        self.reset()

    def contains(self, addr):
        return self.base <= addr < self.base + self.size

    def reset(self):
        pass

    def read(self, offset, size):
        return 0

    def write(self, offset, size, value):
        pass

    def decode(self, offset):
        return ""

    def log(self, msg):
        self.m.log("[%s] %s" % (self.name, msg))


class RegFile(Peripheral):
    """Generic read/write register file (writes stored, reads return value)."""

    def __init__(self, machines, name, base, size, reset_values=None,
                 read_mask=None, desc=""):
        self.reset_values = dict(reset_values or {})
        self.read_mask = dict(read_mask or {})   # offset -> mask of forced bits
        self.values = {}
        super().__init__(machines, name, base, size, desc)

    def reset(self):
        self.values = dict(self.reset_values)

    def read(self, offset, size):
        v = self.values.get(offset, 0)
        if size == 1:
            v = (v >> (8 * (offset & 3))) & 0xFF
        elif size == 2:
            v = (v >> (8 * (offset & 2))) & 0xFFFF
        for m_off, mask in self.read_mask.items():
            if m_off == offset:
                v |= mask
        return v

    def write(self, offset, size, value):
        mask = {1: 0xFF, 2: 0xFFFF, 4: 0xFFFFFFFF}[size]
        if size == 1:
            shift = 8 * (offset & 3)
            cur = self.values.get(offset, 0)
            self.values[offset] = (cur & ~(mask << shift)) | ((value & mask) << shift)
        elif size == 2:
            shift = 8 * (offset & 2)
            cur = self.values.get(offset, 0)
            self.values[offset] = (cur & ~(mask << shift)) | ((value & mask) << shift)
        else:
            self.values[offset] = value & mask


class RCC(Peripheral):
    """Reset & clock control: auto-ready bits so firmware clock init completes."""
    def __init__(self, machines, base=0x40023800):
        self.cr_bits = 0
        self.cfgr = 0
        self.pllcfgr = 0
        self.bdcr = 0
        self.csr = 0
        self.ahb1 = self.ahb2 = self.ahb3 = 0
        self.apb1 = self.apb2 = 0
        self.other = {}
        super().__init__(machines, "RCC", base, 0x400)

    def reset(self):
        self.cr_bits = 0
        self.cfgr = 0
        self.pllcfgr = 0
        self.bdcr = 0
        self.csr = 0x0E000000        # pretend POR/PIN/BOR reset flags
        self.ahb1 = self.ahb2 = self.ahb3 = 0
        self.apb1 = self.apb2 = 0
        self.other = {}

    READY = {
        0: (0x00000001, 0x00000002),     # HSION -> HSIRDY
        16: (0x00010000, 0x00020000),    # HSEON -> HSERDY
        24: (0x01000000, 0x02000000),    # PLLON -> PLLRDY
        26: (0x04000000, 0x08000000),    # PLLI2SON -> PLLI2SRDY
        28: (0x10000000, 0x20000000),    # PLLSAION -> PLLSAIRDY
    }

    def read(self, offset, size):
        if offset == 0x00:               # CR
            v = self.cr_bits
            for en, rdy in self.READY.values():
                if v & en:
                    v |= rdy
            # HSICAL/HSITRIM default plausible
            return v
        if offset == 0x04:               # PLLCFGR
            return self.pllcfgr
        if offset == 0x08:               # CFGR
            sw = self.cfgr & 0x3
            v = self.cfgr | (sw << 2) & 0xF
            return v
        if offset == 0x0C:               # CIR
            return 0
        if offset == 0x10:               # AHB1RSTR
            return self.ahb1
        if offset == 0x14:
            return self.ahb2
        if offset == 0x18:
            return self.ahb3
        if offset == 0x20:
            return self.apb1
        if offset == 0x24:
            return self.apb2
        if offset == 0x28:
            return 0                     # APB2RSTR
        if offset == 0x74:               # CSR
            return self.csr
        if offset == 0x70:               # BDCR
            v = self.bdcr
            if v & 0x1:                 # LSEON -> LSERDY
                v |= 0x2 | 0x4          # LSERDY | LSEBYP-ish
            if v & 0x80:                # RTCSEL=1 (LSE)
                v |= 0x2
            return v
        return self.other.get(offset, 0)

    def write(self, offset, size, value):
        if offset == 0x00:
            self.cr_bits = (self.cr_bits & ~(0x010F0000 | 0x1)) | (value & (0x010F0000 | 0x1))
            # HSEBYP/CSSON kept too
            self.cr_bits |= value & 0x000C0000
        elif offset == 0x04:
            self.pllcfgr = value
        elif offset == 0x08:
            self.cfgr = (self.cfgr & ~0x3) | (value & 0x3) if (value & 0x3) else self.cfgr
            # keep MCO/pre bits roughly
        elif offset == 0x10:
            self.ahb1 = value
        elif offset == 0x14:
            self.ahb2 = value
        elif offset == 0x18:
            self.ahb3 = value
        elif offset == 0x20:
            self.apb1 = value
        elif offset == 0x24:
            self.apb2 = value
        elif offset == 0x70:
            self.bdcr = value
        elif offset == 0x74:
            self.csr = value & 0xFE000000
        else:
            self.other[offset] = value


class FlashIF(Peripheral):
    """FLASH interface (ACR/SR/CR/OPTCR): never busy, no errors."""
    def __init__(self, machines, base=0x40023C00):
        self.acr = 0x00000705
        super().__init__(machines, "FLASH", base, 0x100)

    def reset(self):
        self.acr = 0x00000705

    def read(self, offset, size):
        if offset == 0x00:
            return self.acr
        if offset == 0x04:              # SR: no busy, no errors
            return 0
        if offset == 0x08:              # CR
            return 0
        if offset == 0x10:              # OPTCR
            return 0x0FFFAAED
        if offset == 0x14:
            return 0x0FFFFFFF
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            self.acr = value
        # SR error-clear writes / CR writes: accepted silently


class GPIO(Peripheral):
    """GPIO port model with output state, input injection and MR-based decode."""
    REGS = {
        0x00: "MODER", 0x04: "OTYPER", 0x08: "OSPEEDR", 0x0C: "PUPDR",
        0x10: "IDR", 0x14: "ODR", 0x18: "BSRR", 0x1C: "LCKR",
        0x20: "AFRL", 0x24: "AFRH",
    }

    def __init__(self, machines, name, base, pins=16):
        self.odr = 0
        self.moder = 0
        self.otyper = 0
        self.ospeedr = 0
        self.pupdr = 0
        self.lckr = 0
        self.afr = [0, 0]
        self.inputs = 0            # pin levels forced by external world
        self.input_mask = 0        # which pins are externally driven
        self.listeners = []        # fn(pin, level) on ODR change
        self.pins = pins
        super().__init__(machines, name, base, 0x400)

    def reset(self):
        self.odr = 0
        self.moder = 0xFFFFFFFF   # reset value on F4: analog? actually 0xA8000000 per port
        self.otyper = 0
        self.ospeedr = 0
        self.pupdr = 0
        self.lckr = 0
        self.afr = [0, 0]
        self.inputs = 0
        self.input_mask = 0

    # ---- API used by input.py / rf.py / display.py --------------------
    def set_input(self, pin, level, driven=True):
        if level:
            self.inputs |= (1 << pin)
        else:
            self.inputs &= ~(1 << pin)
        if driven:
            self.input_mask |= (1 << pin)
        else:
            self.input_mask &= ~(1 << pin)

    def get_output(self, pin):
        return (self.odr >> pin) & 1

    def add_listener(self, fn):
        self.listeners.append(fn)

    def _notify(self, changed):
        for fn in self.listeners:
            for pin in range(self.pins):
                if changed & (1 << pin):
                    fn(pin, (self.odr >> pin) & 1)

    def read(self, offset, size):
        if offset == 0x10:            # IDR
            v = (self.odr & ~self.input_mask) | (self.inputs & self.input_mask)
            return v
        if offset == 0x14:
            return self.odr
        if offset == 0x00:
            return self.moder
        if offset == 0x04:
            return self.otyper
        if offset == 0x08:
            return self.ospeedr
        if offset == 0x0C:
            return self.pupdr
        if offset == 0x1C:
            return self.lckr
        if offset == 0x20:
            return self.afr[0]
        if offset == 0x24:
            return self.afr[1]
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            self.moder = value
        elif offset == 0x04:
            self.otyper = value
        elif offset == 0x08:
            self.ospeedr = value
        elif offset == 0x0C:
            self.pupdr = value
        elif offset == 0x14:
            old = self.odr
            self.odr = value & 0xFFFF
            self._notify(old ^ self.odr)
        elif offset == 0x18:          # BSRR: low half set, high half reset
            old = self.odr
            self.odr |= value & 0xFFFF
            self.odr &= ~((value >> 16) & 0xFFFF)
            self.odr &= 0xFFFF
            self._notify(old ^ self.odr)
        elif offset == 0x1C:
            self.lckr = value
        elif offset == 0x20:
            self.afr[0] = value
        elif offset == 0x24:
            self.afr[1] = value

    def decode(self, offset):
        return self.REGS.get(offset, "GPIO+0x%X" % offset)


class SPI(Peripheral):
    """SPI master: instant transfers, per-byte callbacks, RX queue."""
    SR_RXNE = 0x01
    SR_TXE = 0x02
    SR_OVR = 0x40
    SR_BSY = 0x80

    def __init__(self, machines, name, base):
        self.cr1 = 0
        self.cr2 = 0
        self.rx = deque()
        self.rx_data = 0
        self.tx_log = []
        self.last_tx = None
        self.tx_handler = None     # fn(byte) -> optional reply byte
        self.cs = 0
        super().__init__(machines, name, base, 0x400)

    def reset(self):
        self.cr1 = 0
        self.cr2 = 0
        self.rx.clear()
        self.rx_data = 0
        self.cs = 0

    def read(self, offset, size):
        if offset == 0x00:
            return self.cr1
        if offset == 0x04:
            return self.cr2
        if offset == 0x08:              # SR
            sr = self.SR_TXE
            if self.rx:
                sr |= self.SR_RXNE
            return sr
        if offset == 0x0C:              # DR
            if self.rx:
                v = self.rx.popleft()
            else:
                v = self.rx_data
                self.rx_data = 0
            return v
        if offset == 0x10:              # CRCPR
            return 0
        if offset == 0x14:              # RXCRCR
            return 0
        if offset == 0x18:              # TXCRCR
            return 0
        if offset == 0x1C:              # I2SCFGR
            return 0
        if offset == 0x20:
            return 0
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            self.cr1 = value
        elif offset == 0x04:
            self.cr2 = value
        elif offset == 0x0C:            # DR: byte transferred
            b = value & 0xFF if size == 1 else value & 0xFFFF
            self.tx_log.append(b)
            if len(self.tx_log) > 200000:
                self.tx_log = self.tx_log[-100000:]
            reply = None
            if self.tx_handler is not None:
                try:
                    reply = self.tx_handler(b)
                except Exception as e:      # never break emulation on device bugs
                    self.log("tx_handler error: %r" % (e,))
            self.last_tx = b
            if reply is not None:
                self.rx.append(reply & 0xFF)
            return
        elif offset == 0x10:
            pass

    def set_cs(self, level):
        self.cs = level
        if self.tx_handler is not None and hasattr(self.tx_handler, "on_cs"):
            self.tx_handler.on_cs(level)

    def feed_rx(self, data):
        for b in data:
            self.rx.append(b & 0xFF)


class UART(Peripheral):
    """USART/UART: TX bytes recorded, RX queue injectable; SR flag-compatible."""
    SR_TXE = 0x80
    SR_TC = 0x40
    SR_RXNE = 0x20
    SR_ORE = 0x08

    def __init__(self, machines, name, base):
        self.cr1 = 0
        self.cr2 = 0
        self.cr3 = 0
        self.brr = 0
        self.rx = deque()
        self.rx_data = 0
        self.tx_log = bytearray()
        self.tx_handler = None
        super().__init__(machines, name, base, 0x400)

    def reset(self):
        self.cr1 = self.cr2 = self.cr3 = 0
        self.brr = 0
        self.rx.clear()
        self.rx_data = 0
        self.tx_log = bytearray()

    def read(self, offset, size):
        if offset == 0x00:              # SR
            sr = self.SR_TXE | self.SR_TC
            if self.rx:
                sr |= self.SR_RXNE
            return sr
        if offset == 0x04:              # DR
            if self.rx:
                return self.rx.popleft()
            v = self.rx_data
            self.rx_data = 0
            return v
        if offset == 0x08:
            return self.brr
        if offset == 0x0C:
            return self.cr1
        if offset == 0x10:
            return self.cr2
        if offset == 0x14:
            return self.cr3
        if offset == 0x18:
            return 0
        return 0

    def write(self, offset, size, value):
        if offset == 0x04:
            b = value & 0xFF
            self.tx_log.append(b)
            if self.tx_handler is not None:
                try:
                    self.tx_handler(b)
                except Exception:
                    pass
        elif offset == 0x08:
            self.brr = value
        elif offset == 0x0C:
            self.cr1 = value
        elif offset == 0x10:
            self.cr2 = value
        elif offset == 0x14:
            self.cr3 = value

    def feed_rx(self, data):
        for b in data:
            self.rx.append(b & 0xFF)


class I2C(Peripheral):
    """I2C master stub: bus idle, transfer always completes, RX injectable."""
    SR1_SB = 0x0001
    SR1_ADDR = 0x0002
    SR1_BTF = 0x0004
    SR1_RXNE = 0x0040
    SR1_TXE = 0x0080

    def __init__(self, machines, name, base):
        self.cr1 = 0
        self.cr2 = 0
        self.oar1 = 0
        self.oar2 = 0
        self.ccr = 0
        self.trise = 0
        self.rx = deque()
        self.rx_data = 0
        self.tx_log = []
        self.devices = {}           # addr -> obj with read/write hooks
        self.sr1_forced = 0
        super().__init__(machines, name, base, 0x400)

    def reset(self):
        self.cr1 = self.cr2 = 0
        self.rx.clear()
        self.rx_data = 0

    def read(self, offset, size):
        if offset == 0x00:              # CR1
            return self.cr1
        if offset == 0x04:              # CR2
            return self.cr2
        if offset == 0x08:              # OAR1
            return self.oar1
        if offset == 0x0C:              # OAR2
            return self.oar2
        if offset == 0x10:              # DR
            if self.rx:
                return self.rx.popleft()
            return self.rx_data
        if offset == 0x14:              # SR1
            sr1 = self.SR1_TXE | self.SR1_BTF | self.sr1_forced
            if self.rx:
                sr1 |= self.SR1_RXNE
            return sr1
        if offset == 0x18:              # SR2
            return 0
        if offset == 0x1C:
            return self.ccr
        if offset == 0x20:
            return self.trise
        if offset == 0x24:
            return 0
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            self.cr1 = value
        elif offset == 0x04:
            self.cr2 = value
            # transfer "completes" instantly: keep SR1 ready (already forced)
        elif offset == 0x08:
            self.oar1 = value
        elif offset == 0x0C:
            self.oar2 = value
        elif offset == 0x10:
            self.tx_log.append(value & 0xFF)
            if self.tx_handler is not None:
                try:
                    self.tx_handler(value & 0xFF)
                except Exception:
                    pass
        elif offset == 0x1C:
            self.ccr = value
        elif offset == 0x20:
            self.trise = value

    tx_handler = None
    sr1_forced = 0

    def feed_rx(self, data):
        for b in data:
            self.rx.append(b & 0xFF)


class EXTI(Peripheral):
    def __init__(self, machines, base=0x40013C00):
        self.imr = self.emr = self.rtsr = self.ftsr = self.swier = 0
        self.pr = 0
        super().__init__(machines, "EXTI", base, 0x400)

    def reset(self):
        self.imr = self.emr = self.rtsr = self.ftsr = self.swier = 0
        self.pr = 0

    def read(self, offset, size):
        return {0x00: self.imr, 0x04: self.emr, 0x08: self.rtsr,
                0x0C: self.ftsr, 0x10: self.swier, 0x14: self.pr}.get(offset, 0)

    def write(self, offset, size, value):
        if offset == 0x00:
            self.imr = value
        elif offset == 0x04:
            self.emr = value
        elif offset == 0x08:
            self.rtsr = value
        elif offset == 0x0C:
            self.ftsr = value
        elif offset == 0x10:
            self.swier = value
            self.pr |= value
            self.m.note_exti(value)
        elif offset == 0x14:
            self.pr &= ~value


class Timer(Peripheral):
    """Generic STM32 timer (TIM2..TIM14 family) with a running counter."""
    def __init__(self, machines, name, base, bits=32):
        self.cr1 = 0
        self.cr2 = 0
        self.smcr = 0
        self.dier = 0
        self.sr = 0
        self.egr = 0
        self.ccmr1 = self.ccmr2 = 0
        self.ccer = 0
        self.cnt = 0
        self.psc = 0
        self.arr = 0xFFFFFFFF
        self.ccr = [0, 0, 0, 0]
        self.bdtr = 0
        self.dcr = 0
        self.dmar = 0
        self.or_reg = 0
        self.start_vtime = 0
        self.bits = bits
        super().__init__(machines, name, base, 0x400)

    def reset(self):
        self.cr1 = self.cr2 = self.smcr = self.dier = self.sr = 0
        self.egr = self.ccmr1 = self.ccmr2 = self.ccer = 0
        self.cnt = 0
        self.psc = 0
        self.arr = 0xFFFFFFFF
        self.ccr = [0, 0, 0, 0]
        self.start_vtime = 0

    def _tick(self):
        if not (self.cr1 & 1):
            return
        div = self.psc + 1
        now = self.m.vtime
        elapsed = now - self.start_vtime
        ticks = elapsed // div
        if ticks <= 0:
            return
        self.start_vtime += ticks * div
        period = (self.arr + 1) if self.arr != 0xFFFFFFFF else (1 << self.bits)
        total = self.cnt + ticks
        if total >= period:
            self.sr |= 1          # UIF
            total %= period
        self.cnt = total & 0xFFFFFFFF

    def read(self, offset, size):
        self._tick()
        if offset == 0x00:
            return self.cr1
        if offset == 0x04:
            return self.cr2
        if offset == 0x08:
            return self.smcr
        if offset == 0x0C:
            return self.dier
        if offset == 0x10:
            return self.sr
        if offset == 0x14:
            return self.egr
        if offset == 0x18:
            return self.ccmr1
        if offset == 0x1C:
            return self.ccmr2
        if offset == 0x20:
            return self.ccer
        if offset == 0x24:
            return self.cnt
        if offset == 0x28:
            return self.psc
        if offset == 0x2C:
            return self.arr
        if 0x34 <= offset <= 0x40:
            return self.ccr[(offset - 0x34) // 4]
        if offset == 0x44:
            return self.dcr
        if offset == 0x48:
            return self.dmar
        if offset == 0x4C:
            return self.or_reg
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            if value & 1 and not (self.cr1 & 1):
                self.start_vtime = self.m.vtime
            self.cr1 = value & ~0x8   # URQ self-clears
        elif offset == 0x04:
            self.cr2 = value
        elif offset == 0x08:
            self.smcr = value
        elif offset == 0x0C:
            self.dier = value
        elif offset == 0x10:
            self.sr &= ~value
        elif offset == 0x14:            # EGR
            if value & 1:               # UG
                self.cnt = 0
                self.sr |= 1
        elif offset == 0x18:
            self.ccmr1 = value
        elif offset == 0x1C:
            self.ccmr2 = value
        elif offset == 0x20:
            self.ccer = value
        elif offset == 0x24:
            self.cnt = value & 0xFFFFFFFF
            self.start_vtime = self.m.vtime
        elif offset == 0x28:
            self.psc = value
        elif offset == 0x2C:
            self.arr = value
        elif 0x34 <= offset <= 0x40:
            self.ccr[(offset - 0x34) // 4] = value
        elif offset == 0x44:
            self.dcr = value
        elif offset == 0x48:
            self.dmar = value
        elif offset == 0x4C:
            self.or_reg = value


class RTC(Peripheral):
    """RTC: register file with readiness bits, plausible calendar values."""
    def __init__(self, machines, base=0x40002800):
        self.vals = {0x00: 0x00000000,   # TR (00:00:00 BCD)
                     0x04: 0x00240101,   # DR 2024-01-01
                     0x08: 0,            # CR
                     0x0C: 0x77,         # ISR ready bits
                     0x10: 0x007F00FF,   # PRER
                     0x14: 0xFFFF,       # WUTR
                     0x18: 0x00000000,   # CALIBR
                     0x1C: 0, 0x20: 0, 0x24: 0,
                     0x28: 0, 0x2C: 0, 0x30: 0, 0x34: 0, 0x38: 0,
                     0x3C: 0, 0x40: 0}
        super().__init__(machines, "RTC", base, 0x400)

    def reset(self):
        pass

    READY = 0x77

    def read(self, offset, size):
        if offset == 0x0C:
            v = self.vals.get(0x0C, 0) | self.READY
            # INITF mirrors INIT (CR bit 7): config mode always ready
            if self.vals.get(0x08, 0) & 0x80:
                v |= 0x40
            return v
        return self.vals.get(offset, 0)

    def write(self, offset, size, value):
        if offset == 0x28:              # WPR: ignore
            return
        self.vals[offset] = value


class IWDG(Peripheral):
    """Independent watchdog: accepts reloads, never resets."""
    def __init__(self, machines, base=0x40003000):
        self.kr = 0
        self.pr = 0
        self.rlr = 0x0FFF
        self.sr = 0
        self.reloads = 0
        super().__init__(machines, "IWDG", base, 0x400)

    def reset(self):
        self.kr = 0
        self.pr = 0
        self.rlr = 0x0FFF
        self.sr = 0
        self.reloads = 0

    def read(self, offset, size):
        if offset == 0x00:
            return self.kr
        if offset == 0x04:
            return self.pr
        if offset == 0x08:
            return self.rlr
        if offset == 0x0C:
            return 0                    # SR: no PVU/RVU/WVU busy
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            self.kr = value
            if value == 0xAAAA:
                self.reloads += 1
        elif offset == 0x04:
            self.pr = value
        elif offset == 0x08:
            self.rlr = value


class ADC(Peripheral):
    """ADC model supporting both register layouts used by STM32 families:

    * F4/F7-style:  SR@0x00, CR1@0x04, CR2@0x08, SMPR1@0x0C, SMPR2@0x10,
      SQR1@0x2C, DR@0x4C.
    * F3/L4/H7-style: ISR@0x00, IER@0x04, CR@0x08, CFGR1@0x0C, CFGR2@0x10,
      SMPR1@0x14, SMPR2@0x18, SQR1@0x2C, DR@0x40.

    The X3-C firmware polls bit6 (ADRDY in the newer layout / JEOC in the
    older one) after enabling the ADC, and polls the self-clearing ADSTART /
    JADSTART bits (CR bits 2,3) after starting a conversion, so both are
    modelled: bit6 is always returned set and the start bits self-clear.
    """

    READY_BITS = 0x00000047      # ADRDY(0) EOSMP(1) EOC(2) JEOC(6)

    def __init__(self, machines, name, base):
        self.cr1 = self.cr2 = self.smpr1 = self.smpr2 = 0
        self.sqr1 = self.sqr2 = self.sqr3 = self.jsqr = 0
        self.jcr = self.jdr = 0
        self.ccr = 0
        self.cr = 0                  # newer-layout CR
        self.ier = 0
        self.cfgr1 = self.cfgr2 = 0
        self.extra = {}
        super().__init__(machines, name, base, 0x100)

    def reset(self):
        self.cr1 = self.cr2 = 0
        self.cr = self.ier = 0
        self.cfgr1 = self.cfgr2 = 0
        self.extra = {}

    def read(self, offset, size):
        if offset == 0x00:              # SR / ISR
            v = self.READY_BITS
            if self.cr & 1:             # ADEN -> ADRDY (bit0/bit6 both)
                v |= 0x41
            return v
        if offset == 0x04:              # CR1 / IER
            return self.cr1 if self.cr1 else self.ier
        if offset == 0x08:              # CR2 / CR
            return self.cr2 if self.cr2 else (self.cr & ~0x0C)
        if offset == 0x0C:
            return self.smpr1 or self.cfgr1
        if offset == 0x10:
            return self.smpr2 or self.cfgr2
        if offset == 0x14:
            return self.jcr
        if offset == 0x18:
            return self.extra.get(0x18, 0)
        if 0x2C <= offset <= 0x34:
            return self.extra.get(offset, 0)
        if offset == 0x38:
            return self.jsqr
        if offset == 0x40:              # DR (newer layout)
            return 0x800
        if offset == 0x4C:              # DR (older layout)
            return 0x800
        return self.extra.get(offset, 0)

    def write(self, offset, size, value):
        if offset == 0x00:              # ISR clear (rc_w1) / SR read-only
            return
        if offset == 0x04:
            self.cr1 = value
            self.ier = value
        elif offset == 0x08:
            # newer layout CR: ADEN(0), ADDIS(1) persist; ADSTART(2),
            # JADSTART(3) self-clear; older layout CR2 stored as-is
            self.cr = value & ~0x0C
            self.cr2 = value & ~0x0C
        elif offset == 0x0C:
            self.smpr1 = value
            self.cfgr1 = value
        elif offset == 0x10:
            self.smpr2 = value
            self.cfgr2 = value
        elif offset == 0x14:
            self.jcr = value
        elif offset == 0x18:
            self.extra[0x18] = value
        elif 0x2C <= offset <= 0x34:
            self.extra[offset] = value
            setattr(self, "sqr%d" % ((offset - 0x2C) // 4 + 1), value)
        elif offset == 0x38:
            self.jsqr = value
        elif offset == 0x40:
            self.jdr = value
        elif offset == 0x4C:
            pass
        else:
            self.extra[offset] = value


class DMA(Peripheral):
    """DMA controller (STM32F4/AT32 stream layout) with real transfers.

    Each stream window starts at 0x10 + 0x18*idx and contains:
        +0x00 SxCR, +0x04 SxNDTR, +0x08 SxPAR, +0x0C SxMAR, +0x10 SxFCR
    Writing SxCR with EN=1 performs the transfer immediately through the
    machine's unified memory dispatcher, so a transfer whose peripheral side is
    the XMC LCD window (0x6C000001) feeds the display model.  EN self-clears,
    NDTR becomes 0 and TCIF is raised (flags live in LISR/HISR, 6 bits/stream).
    """

    def __init__(self, machines, name, base, nstreams=8):
        self.nstreams = nstreams
        self.lisr = 0
        self.hisr = 0
        self.streams = [dict(regs={}, ndtr=0) for _ in range(nstreams)]
        self.transfers = []
        self.enabled = True
        super().__init__(machines, name, base, 0x400)

    def reset(self):
        self.lisr = 0
        self.hisr = 0
        for s in self.streams:
            s["regs"] = {}
            s["ndtr"] = 0

    def _stream_index(self, offset):
        if offset < 0x10:
            return None
        rel = offset - 0x10
        return rel // 0x18, rel % 0x18

    def _flag_base(self, idx):
        return idx * 6 if idx < 4 else (idx - 4) * 6

    def _set_flag(self, idx, bit):
        base = self._flag_base(idx)
        if idx < 4:
            self.lisr |= (1 << (base + bit))
        else:
            self.hisr |= (1 << (base + bit))

    # -- register access ----------------------------------------------
    def read(self, offset, size):
        if offset == 0x00:
            return self.lisr
        if offset == 0x04:
            return self.hisr
        if offset == 0x08 or offset == 0x0C:
            return 0
        si = self._stream_index(offset)
        if si is None:
            return 0
        idx, ro = si
        s = self.streams[idx]
        if ro == 0x00:
            return s["regs"].get(0x00, 0) & ~1     # EN reads 0
        if ro == 0x04:
            return s["ndtr"]
        return s["regs"].get(ro, 0)

    def write(self, offset, size, value):
        if offset == 0x08:               # LIFCR
            self.lisr &= ~(value & 0x0FFFFFFF)
            return
        if offset == 0x0C:               # HIFCR
            self.hisr &= ~(value & 0x0FFFFFFF)
            return
        if offset == 0x00 or offset == 0x04:
            return
        si = self._stream_index(offset)
        if si is None:
            return
        idx, ro = si
        s = self.streams[idx]
        if ro == 0x04:
            s["ndtr"] = value & 0xFFFF
        elif ro == 0x00:                 # SxCR
            s["regs"][0x00] = value & ~1
            if (value & 1) and self.enabled:
                self._run_transfer(idx)
        else:
            s["regs"][ro] = value

    # -- transfer engine ---------------------------------------------
    def _run_transfer(self, idx):
        s = self.streams[idx]
        cr = s["regs"].get(0x00, 0)
        ndtr = s["ndtr"] & 0xFFFF
        par = s["regs"].get(0x08, 0)
        mar = s["regs"].get(0x0C, 0)
        dir_ = (cr >> 6) & 3
        pinc = (cr >> 9) & 1
        minc = (cr >> 10) & 1
        psize = 1 << ((cr >> 11) & 3)
        msize = 1 << ((cr >> 13) & 3)
        if ndtr > 0x100000:
            self.log("transfer truncated NDTR=%d" % ndtr)
            ndtr = 0x100000
        if dir_ == 0:            # peripheral -> memory
            src, dst = par, mar
            src_inc, src_sz = pinc, psize
            dst_inc, dst_sz = minc, msize
        elif dir_ == 1:          # memory -> peripheral
            src, dst = mar, par
            src_inc, src_sz = minc, msize
            dst_inc, dst_sz = pinc, psize
        else:                    # memory -> memory (MAR source)
            src, dst = mar, par
            src_inc, src_sz = minc, msize
            dst_inc, dst_sz = pinc, psize
        ok = True
        try:
            for _ in range(ndtr):
                val = self.m.read_mem(src, src_sz)
                self.m.write_mem(dst, dst_sz, val)
                if src_inc:
                    src += src_sz
                if dst_inc:
                    dst += dst_sz
        except Exception as e:
            ok = False
            self.log("DMA transfer error: %r" % (e,))
        s["ndtr"] = 0
        self._set_flag(idx, 5)           # TCIF
        self.transfers.append(dict(vtime=self.m.vtime, stream=idx, dir=dir_,
                                   ndtr=ndtr, par=par, mar=mar,
                                   psize=psize, msize=msize, ok=ok))
        if len(self.transfers) > 2000:
            del self.transfers[:1000]

    def summary(self, n=20):
        lines = ["DMA transfers: %d" % len(self.transfers)]
        for t in self.transfers[-n:]:
            lines.append("  t=%-10d stream=%d dir=%d ndtr=%-6d PAR=%#010x "
                         "MAR=%#010x sz=%d/%d ok=%s"
                         % (t["vtime"], t["stream"], t["dir"], t["ndtr"],
                            t["par"], t["mar"], t["psize"], t["msize"],
                            t["ok"]))
        return "\n".join(lines)


class USB(Peripheral):
    """USB OTG FS/HS: idle core; reset/handshake bits self-clear."""
    def __init__(self, machines, name, base):
        self.vals = {}
        super().__init__(machines, name, base, 0x40000)

    def reset(self):
        self.vals = {}

    def read(self, offset, size):
        # GINTSTS/GOTGINT read as zero (nothing happened)
        if offset in (0x14, 0x18, 0x1C, 0x20):
            return 0
        if offset == 0x00:              # GOTGCTL
            return 0x00000800           # BSESVLD
        if offset == 0x0C:              # GUSBCFG
            return self.vals.get(offset, 0)
        if offset == 0x10:              # GRSTCTL
            return 0x80000000           # AHBIDL: core idle
        if offset == 0x40:              # GRXSTSR
            return 0
        if offset == 0x44:
            return 0
        if offset == 0x48:
            return 0
        if offset == 0x4C:
            return 0
        return self.vals.get(offset, 0)

    def write(self, offset, size, value):
        if offset == 0x10:
            return                      # reset bits self-clear
        self.vals[offset] = value


class CRC(Peripheral):
    """CRC unit with a real CRC-32 (STM32 style: poly 04C11DB7, init FFFFFFFF)."""
    def __init__(self, machines, base=0x40023000):
        self.dr = 0xFFFFFFFF
        self.cr = 0
        self.idr = 0x00000000
        super().__init__(machines, "CRC", base, 0x400)

    def reset(self):
        self.dr = 0xFFFFFFFF
        self.cr = 0
        self.idr = 0

    def _crc32_byte(self, crc, byte):
        crc ^= (byte << 24) & 0xFFFFFFFF
        for _ in range(8):
            if crc & 0x80000000:
                crc = ((crc << 1) ^ 0x04C11DB7) & 0xFFFFFFFF
            else:
                crc = (crc << 1) & 0xFFFFFFFF
        return crc

    def read(self, offset, size):
        if offset == 0x00:
            return self.dr
        if offset == 0x04:
            return self.idr
        if offset == 0x08:
            return self.cr
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            crc = self.dr
            for i in (0, 1, 2, 3):
                crc = self._crc32_byte(crc, (value >> (8 * i)) & 0xFF)
            self.dr = crc
        elif offset == 0x08:
            self.cr = value
            if value & 1:               # RESET -> init value
                self.dr = 0xFFFFFFFF


class RNG(Peripheral):
    def __init__(self, machines, base=0x50060800):
        self.state = 0x12345678
        super().__init__(machines, "RNG", base, 0x400)

    def reset(self):
        self.state = 0x12345678

    def read(self, offset, size):
        if offset == 0x00:              # CR
            return 0x4
        if offset == 0x04:              # SR: DRDY|SEIS? DRDY bit0, SECS bit1
            return 0x1
        if offset == 0x08:              # DR
            self.state ^= (self.state << 13) & 0xFFFFFFFF
            self.state ^= self.state >> 17
            self.state ^= (self.state << 5) & 0xFFFFFFFF
            return self.state & 0xFFFFFFFF
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            pass


class FMC(Peripheral):
    """FMC/FSMC exterior memory controller - registers only."""
    def __init__(self, machines, base=0xA0000000):
        self.vals = {}
        super().__init__(machines, "FMC", base, FMC_SIZE)

    def reset(self):
        self.vals = {}

    def read(self, offset, size):
        if offset < 0x100:              # registers
            return self.vals.get(offset, 0)
        return 0                        # external memory reads 0

    def write(self, offset, size, value):
        if offset < 0x100:
            self.vals[offset] = value


class PWR(Peripheral):
    def __init__(self, machines, base=0x40007000):
        self.cr = 0
        self.csr = 0
        super().__init__(machines, "PWR", base, 0x400)

    def reset(self):
        self.cr = 0
        self.csr = 0

    def read(self, offset, size):
        if offset == 0x00:
            return self.cr
        if offset == 0x04:
            return self.csr | 0x0000C000      # VOSRDY | (PVDO?)  -> supply ready
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            self.cr = value
        elif offset == 0x04:
            self.csr &= ~(value & 0x00004400)


class DBGMCU(Peripheral):
    def __init__(self, machines, base=0xE0042000):
        self.cr = 0
        super().__init__(machines, "DBGMCU", base, 0x400)

    def reset(self):
        self.cr = 0

    def read(self, offset, size):
        if offset == 0x04:
            return 0x10076413       # IDCODE (F4-ish)
        if offset == 0x00:
            return self.cr
        return 0

    def write(self, offset, size, value):
        if offset == 0x00:
            self.cr = value


class LcdFmc(Peripheral):
    """FMC-attached LCD controller window at 0x6C000000.

    Writes at offset 0 are commands, writes at any other offset are data
    (RS line wired to an FMC address line).  The actual decoding is done by
    ``display.ParallelLcd``; this peripheral just routes byte traffic to it.
    """

    def __init__(self, machines, base=0x6C000000, size=0x40000):
        self.lcd = None
        self.count = 0
        super().__init__(machines, "LCD_FMC", base, size)

    def read(self, offset, size):
        return 0

    def write(self, offset, size, value):
        self.count += 1
        if self.lcd is not None:
            self.lcd.feed(offset, value, size)

    def decode(self, offset):
        return "CMD" if offset == 0 else "DATA"


class LcdSpi(Peripheral):
    """SPI-attached LCD command/data window (placeholder; not yet identified)."""

    def __init__(self, machines, base=0x40000000, size=0x100):
        self.count = 0
        super().__init__(machines, "LCD_SPI_win", base, size)

    def read(self, offset, size):
        return 0

    def write(self, offset, size, value):
        self.count += 1


# ==========================================================================
# PPB: SCB / SysTick / NVIC / DWT / ITM
# ==========================================================================
class PPB(Peripheral):
    def __init__(self, machines):
        self.cpuid = 0x410FC241        # Cortex-M4
        self.icsr_forced = 0
        self.vtor = 0x08004800
        self.aircr = 0xFA050000
        self.scr = 0
        self.ccr = 0
        self.shpr = [0, 0, 0]
        self.shcsr = 0
        self.cfsr = 0
        self.hfsr = 0
        self.mmfar = 0
        self.bfar = 0
        self.afsr = 0
        self.cpacr = 0x00F00000
        self.demcr = 0
        self.stir = 0
        self.fpccr = 0x20000000
        self.mpu_type = 0x00000800
        self.mpu_ctrl = 0
        self.mpu_rnr = 0
        self.mpu_rbar = 0
        self.mpu_rasr = 0
        # SysTick
        self.st_ctrl = 0
        self.st_load = 0
        self.st_val = 0
        self.st_calib = 0x0000A000
        self.st_accum = 0
        self.st_last = 0
        self.st_countflag = 0
        # NVIC
        self.nvic_enabled = 0
        self.nvic_pending = 0
        self.nvic_active = 0
        self.nvic_prio = {}
        # DWT
        self.dwt_ctrl = 0
        self.dwt_cyccnt = 0
        super().__init__(machines, "PPB", PPB_BASE, PPB_SIZE)

    def reset(self):
        self.icsr_forced = 0
        self.vtor = self.m.vtor if hasattr(self.m, "vtor") else 0x08004800
        self.scr = 0
        self.shcsr = 0
        self.cfsr = 0
        self.hfsr = 0
        self.st_ctrl = 0
        self.st_load = 0
        self.st_val = 0
        self.st_accum = 0
        self.st_last = 0
        self.st_countflag = 0
        self.nvic_enabled = 0
        self.nvic_pending = 0
        self.dwt_ctrl = 0

    # -- SysTick tick accounting ---------------------------------------
    def systick_update(self):
        """Advance SysTick with virtual time; pend IRQ 15 on wrap."""
        if not (self.st_ctrl & 1):
            self.st_last = self.m.vtime
            return
        period = (self.st_load & 0xFFFFFF) + 1
        delta = self.m.vtime - self.st_last
        if delta <= 0:
            return
        self.st_last = self.m.vtime
        self.st_accum += delta
        if self.st_accum >= period:
            self.st_accum %= period
            self.st_countflag = 1
            if self.st_ctrl & 0x2:
                self.m.pend_irq(15)

    def systick_val(self):
        if not (self.st_ctrl & 1):
            return self.st_val & 0xFFFFFF
        period = (self.st_load & 0xFFFFFF) + 1
        delta = self.m.vtime - self.st_last
        return (period - (self.st_accum + delta) % period) % period

    # -- read/write ----------------------------------------------------
    def read(self, offset, size):
        # SCB
        if offset == 0xED00:
            return self.cpuid
        if offset == 0xED04:            # ICSR
            v = self.icsr_forced & ~0x1FF00000
            v |= (0x1FF & 0) << 0
            return v
        if offset == 0xED08:
            return self.vtor
        if offset == 0xED0C:
            return self.aircr
        if offset == 0xED10:
            return self.scr
        if offset == 0xED14:
            return self.ccr
        if offset == 0xED18:
            return self.shpr[0]
        if offset == 0xED1C:
            return self.shpr[1]
        if offset == 0xED20:
            return self.shpr[2]
        if offset == 0xED24:
            return self.shcsr
        if offset == 0xED28:
            return self.cfsr
        if offset == 0xED2C:
            return self.hfsr
        if offset == 0xED34:
            return self.mmfar
        if offset == 0xED38:
            return self.bfar
        if offset == 0xED3C:
            return self.afsr
        if offset == 0xED88:
            return self.cpacr
        if offset == 0xEDFC:            # DEMCR
            return self.demcr
        # MPU
        if offset == 0xED90:
            return self.mpu_type
        if offset == 0xED94:
            return self.mpu_ctrl
        if offset == 0xED98:
            return self.mpu_rnr
        if offset == 0xED9C:
            return self.mpu_rbar
        if offset == 0xEDA0:
            return self.mpu_rasr
        # SysTick
        if offset == 0xE010:
            v = self.st_ctrl & ~0x00010000
            if self.st_countflag:
                v |= 0x00010000
                self.st_countflag = 0
            return v
        if offset == 0xE014:
            return self.st_load & 0xFFFFFF
        if offset == 0xE018:
            return self.systick_val()
        if offset == 0xE01C:
            return self.st_calib
        # NVIC
        if 0xE100 <= offset < 0xE120:          # ISER
            return (self.nvic_enabled >> ((offset - 0xE100) * 8)) & 0xFFFFFFFF
        if 0xE180 <= offset < 0xE1A0:          # ICER
            return 0
        if 0xE200 <= offset < 0xE220:          # ISPR
            return (self.nvic_pending >> ((offset - 0xE200) * 8)) & 0xFFFFFFFF
        if 0xE280 <= offset < 0xE2A0:          # ICPR
            return 0
        if 0xE400 <= offset < 0xE500:          # IPR (byte regs)
            idx = offset - 0xE400
            return self.nvic_prio.get(idx // 4, [0, 0, 0, 0])[idx & 3]
        if offset == 0xEF00:                   # STIR
            return 0
        # DWT / ITM / TPIU
        if offset == 0x1000:                   # DWT_CTRL
            return self.dwt_ctrl
        if offset == 0x1004:                   # DWT_CYCCNT
            return self.m.vtime & 0xFFFFFFFF
        if offset == 0x101C:
            return 0
        if offset < 0x2000:
            return 0
        if offset == 0x4000:
            return 0
        if offset == 0x100000 - 0x4000:        # ROM table-ish
            return 0
        return 0

    def write(self, offset, size, value):
        # SCB
        if offset == 0xED04:            # ICSR
            if value & 0x10000000:      # PENDSVSET
                self.m.pend_irq(14)
            if value & 0x08000000:      # PENDSVCLR
                self.m.pending_irqs.discard(14)
            if value & 0x04000000:      # PENDSTSET
                self.m.pend_irq(15)
            if value & 0x02000000:      # PENDSTCLR
                self.m.pending_irqs.discard(15)
            if value & 0x00800000:      # ISRPREEMPT
                pass
            self.icsr_forced = value & 0x1FF00000
        elif offset == 0xED08:          # VTOR
            self.vtor = value & 0xFFFFFF80
            self.m.vtor = self.vtor
        elif offset == 0xED0C:          # AIRCR
            self.aircr = value
            if (value >> 16) == 0x05FA and (value & 0x00000004):
                self.log("AIRCR SYSRESETREQ (pc=%#x)"
                         % self.m.uc.reg_read(UC_ARM_REG_PC))
                self.m.request_reset()
        elif offset == 0xED10:
            self.scr = value
        elif offset == 0xED14:
            self.ccr = value
        elif offset in (0xED18, 0xED1C, 0xED20):
            self.shpr[(offset - 0xED18) // 4] = value
        elif offset == 0xED24:
            self.shcsr = value
        elif offset == 0xED28:
            self.cfsr &= ~value
        elif offset == 0xED2C:
            self.hfsr &= ~value
        elif offset == 0xED34:
            self.mmfar = value
        elif offset == 0xED38:
            self.bfar = value
        elif offset == 0xED88:
            self.cpacr = value
        elif offset == 0xEDFC:
            self.demcr = value
        elif offset == 0xED94:
            self.mpu_ctrl = value & 0x7
        elif offset == 0xED98:
            self.mpu_rnr = value & 0xFF
        elif offset == 0xED9C:
            self.mpu_rbar = value
        elif offset == 0xEDA0:
            self.mpu_rasr = value
        # SysTick
        elif offset == 0xE010:
            self.st_ctrl = value & 0x7
            self.st_countflag = 0
            self.st_accum = 0
            self.st_last = self.m.vtime
        elif offset == 0xE014:
            self.st_load = value & 0xFFFFFF
        elif offset == 0xE018:
            self.st_val = value & 0xFFFFFF
            self.st_accum = 0
            self.st_last = self.m.vtime
        # NVIC
        elif 0xE100 <= offset < 0xE120:          # ISER
            self.nvic_enabled |= value << ((offset - 0xE100) * 8)
        elif 0xE180 <= offset < 0xE1A0:          # ICER
            self.nvic_enabled &= ~(value << ((offset - 0xE180) * 8))
        elif 0xE200 <= offset < 0xE220:          # ISPR
            self.m.pending_irqs |= set(_bits(value, (offset - 0xE200) * 8))
            self.nvic_pending = self._pending_mask()
        elif 0xE280 <= offset < 0xE2A0:          # ICPR
            for b in _bits(value, (offset - 0xE280) * 8):
                self.m.pending_irqs.discard(b)
            self.nvic_pending = self._pending_mask()
        elif 0xE400 <= offset < 0xE500:          # IPR
            idx = offset - 0xE400
            self.nvic_prio.setdefault(idx // 4, [0, 0, 0, 0])[idx & 3] = value & 0xFF
        elif offset == 0xEF00:                   # STIR
            self.m.pend_irq(value & 0x1FF)
        # DWT
        elif offset == 0x1000:
            self.dwt_ctrl = value
        elif offset == 0x1004:
            self.dwt_cyccnt = value

    def _pending_mask(self):
        v = 0
        for irq in self.m.pending_irqs:
            if 0 <= irq < 256:
                v |= 1 << irq
        return v


def _bits(value, base):
    out = []
    v = value
    i = 0
    while v:
        if v & 1:
            out.append(base + i)
        v >>= 1
        i += 1
    return out


# ==========================================================================
# The machine
# ==========================================================================
class Machine:
    def __init__(self, image=DEFAULT_IMAGE, image_base=APP_BASE,
                 flash_base=FLASH_BASE, flash_size=FLASH_SIZE,
                 ram_base=RAM_BASE, ram_size=RAM_SIZE,
                 fake_bootloader=True,
                 trace=True, trace_max=200000,
                 unknown_policy="last", auto_map=True,
                 log_fn=print, hardfault=True):
        self.image = image
        self.image_base = image_base
        self.flash_base = flash_base
        self.flash_size = flash_size
        self.ram_base = ram_base
        self.ram_size = ram_size
        self.fake_bootloader = fake_bootloader
        self.unknown_policy = unknown_policy
        self.auto_map = auto_map
        self.hardfault = hardfault
        self.log_fn = log_fn
        self.log_lines = []
        self.trace_enabled = trace
        self.access = AccessTrace(trace_max)
        self.periphs = []
        self.unknown_values = {}
        self.auto_mapped = set()
        self.breakpoints = {}          # addr -> name
        self._bp_skip = None
        self.watchpoints = []          # (lo, hi, kind, name)
        self.symbols = {}
        self.call_targets = Counter()  # target -> count
        self.call_events = deque(maxlen=50000)
        self.call_trace_enabled = False
        self.insn_count = 0
        self.vtime = 0
        self.pc_ring = deque(maxlen=8192)
        self.last_pc = 0
        self.stop_reason = None
        self.fault = None
        self.active_exception = None
        self.pending_irqs = set()
        self.mmio_log_enabled = True
        self._auto_map_prot = UC_PROT_ALL
        self._in_reset = False
        self.vtor = image_base
        self.interrupt_after_chunk = True
        self.progress_fn = None
        self.progress_every = 100000
        self.pc_counter = None
        self._load()

    # ------------------------------------------------------------------
    # setup
    # ------------------------------------------------------------------
    def _load(self):
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
        # ---- flash
        self.uc.mem_map(self.flash_base, self.flash_size)
        data = b""
        if self.image:
            with open(self.image, "rb") as f:
                data = f.read()
        # fake bootloader area before the app image
        if self.fake_bootloader and self.image_base > self.flash_base:
            filler = bytearray(self.image_base - self.flash_base)
            sp = self._peek_image_word(data, 0)
            rst = self._peek_image_word(data, 4)
            struct.pack_into("<I", filler, 0, sp if sp else 0x200477C0)
            struct.pack_into("<I", filler, 4, rst if rst else self.image_base)
            self.uc.mem_write(self.flash_base, bytes(filler))
        if data:
            self.uc.mem_write(self.image_base, data)
        self.image_data = data
        self.image_len = len(data)
        # ---- RAM
        self.uc.mem_map(self.ram_base, self.ram_size)
        try:
            self.uc.mem_map(CCM_BASE, CCM_SIZE)
        except UcError:
            pass
        # ---- STM32 option-byte area (0x1FFFC000).
        # The app's reset stub calls a one-time "program option bytes" routine
        # (0x08020A20) which reads 0x1FFFC010 and 0x1FFFC002: if
        # (opt10&7)==2 && !(opt02&0x60) it returns (normal boot); otherwise it
        # unlocks flash, sets OPTSTRT and programs opt02=0x000F, opt10=0x0002,
        # then performs SYSRESETREQ.  A device that has already booted once
        # therefore reads back these programmed values.  Preset them so the
        # emulated device takes the normal-boot path.
        self.uc.mem_map(0x1FFFC000, 0x1000)
        opt = bytearray(b"\xff" * 0x1000)
        struct.pack_into("<H", opt, 0x00, 0xFFFF)
        struct.pack_into("<H", opt, 0x02, 0x000F)   # already-programmed flag
        struct.pack_into("<H", opt, 0x04, 0xFFFF)
        struct.pack_into("<H", opt, 0x10, 0x0002)   # (opt10 & 7) == 2
        struct.pack_into("<H", opt, 0x12, 0xFFFF)
        self.uc.mem_write(0x1FFFC000, bytes(opt))
        # ---- peripheral arena via mmio callbacks
        self._mmio_read_cb = self._make_mmio_read()
        self._mmio_write_cb = self._make_mmio_write()
        for base, size in ((PERIPH_BASE, PERIPH_SIZE), (USB_BASE, USB_SIZE),
                           (FMC_BASE, FMC_SIZE), (PPB_BASE, PPB_SIZE),
                           # FMC/FSMC external memory windows (LCD controller
                           # was observed at 0x6C000000)
                           (0x60000000, 0x10000000)):
            self.uc.mmio_map(base, size, self._mmio_read_cb, base,
                             self._mmio_write_cb, base)
        # ---- hooks
        self.fine = True
        self._code_hook = self.uc.hook_add(UC_HOOK_CODE, self._hook_code)
        self.uc.hook_add(UC_HOOK_MEM_READ_UNMAPPED | UC_HOOK_MEM_WRITE_UNMAPPED,
                         self._hook_unmapped)
        self.uc.hook_add(UC_HOOK_MEM_FETCH_UNMAPPED, self._hook_fetch_unmapped)
        self.uc.hook_add(UC_HOOK_MEM_INVALID, self._hook_invalid)
        # ---- peripherals
        self._build_peripherals()

    # ------------------------------------------------------------------
    # fine / fast execution mode
    # ------------------------------------------------------------------
    def set_fine(self, fine):
        """Enable/disable per-instruction code hook (fast burst mode).

        Fast mode removes the Python code hook, so it cannot service
        breakpoints or collect a per-instruction PC trace, but it runs roughly
        20-50x faster and is ideal for long software delays at boot.
        """
        fine = bool(fine)
        if fine == self.fine:
            return
        if fine:
            self._code_hook = self.uc.hook_add(UC_HOOK_CODE, self._hook_code)
        else:
            if self._code_hook is not None:
                self.uc.hook_del(self._code_hook)
                self._code_hook = None
        self.fine = fine

    def _do_reset(self):
        self._in_reset = False
        self.log("[reset] AIRCR requested system reset -> rebooting")
        self.ppb.reset()
        for p in self.periphs:
            if p is not self.ppb:
                p.reset()
        self.soft_reset()

    @staticmethod
    def _peek_image_word(data, off):
        if data and len(data) >= off + 4:
            return struct.unpack_from("<I", data, off)[0]
        return 0

    def _build_peripherals(self):
        self.ppb = PPB(self)
        add = self.add_peripheral
        add(self.ppb)
        add(RCC(self))
        add(FlashIF(self))
        # GPIOA..GPIOK, 0x400 per port from 0x40020000
        for i in range(11):
            name = "GPIO%s" % "ABCDEFGHIJK"[i]
            add(GPIO(self, name, 0x40020000 + 0x400 * i))
        # AHB1
        add(DMA(self, "DMA1", 0x40026000, 8))
        add(DMA(self, "DMA2", 0x40026400, 8))
        # APB1
        tnames = [("TIM2", 0x40000000), ("TIM3", 0x40000400), ("TIM4", 0x40000800),
                  ("TIM5", 0x40000C00), ("TIM6", 0x40001000), ("TIM7", 0x40001400),
                  ("TIM12", 0x40001800), ("TIM13", 0x40001C00), ("TIM14", 0x40002000)]
        for name, base in tnames:
            add(Timer(self, name, base))
        add(RegFile(self, "WWDG", 0x40002C00, 0x400))
        add(RTC(self))
        add(IWDG(self))
        add(RegFile(self, "SPI2", 0x40003800, 0x400))
        add(SPI(self, "SPI3", 0x40003C00))
        add(UART(self, "USART2", 0x40004400))
        add(UART(self, "USART3", 0x40004800))
        add(UART(self, "UART4", 0x40004C00))
        add(UART(self, "UART5", 0x40005000))
        add(I2C(self, "I2C1", 0x40005400))
        add(I2C(self, "I2C2", 0x40005800))
        add(I2C(self, "I2C3", 0x40005C00))
        add(RegFile(self, "CAN1", 0x40006400, 0x400))
        add(PWR(self))
        add(RegFile(self, "DAC", 0x40007400, 0x400))
        # APB2
        add(RegFile(self, "SYSCFG", 0x40013800, 0x400))
        add(EXTI(self))
        add(Timer(self, "TIM1", 0x40010000))
        add(SPI(self, "SPI1", 0x40013000))
        add(UART(self, "USART1", 0x40011000))
        add(UART(self, "USART6", 0x40011400))
        add(ADC(self, "ADC1", 0x40012000))
        add(ADC(self, "ADC2", 0x40012100))
        add(ADC(self, "ADC3", 0x40012200))
        add(RegFile(self, "ADC_Common", 0x40012300, 0x100))
        add(RegFile(self, "SDIO", 0x40012C00, 0x400))
        add(RegFile(self, "TIM8", 0x40010400, 0x400))
        add(RegFile(self, "TIM9", 0x40014000, 0x400))
        add(RegFile(self, "TIM10", 0x40014400, 0x400))
        add(RegFile(self, "TIM11", 0x40014800, 0x400))
        # AHB2/3
        add(RegFile(self, "GPIO_ADC_common", 0x40020000 - 0x100, 0x100))
        add(CRC(self))
        add(RegFile(self, "RCC_dummy", 0x40023C00 + 0x100, 0x100))
        # USB
        add(USB(self, "USB_OTG_FS", USB_BASE))
        add(RegFile(self, "USB_OTG_HS", 0x40040000, 0x40000))
        add(RNG(self))
        add(FMC(self))
        add(DBGMCU(self))
        # ---- AT32 XMC-attached display controller (writes at 0x6C000000)
        try:
            from xmc import XmcDisplay
            self.lcd_model = XmcDisplay(self, width=240, height=240)
            add(self.lcd_model)
        except Exception as _e:
            self.log("xmc display model unavailable: %r" % (_e,))
            self.lcd_model = None
        self.lcd = self.lcd_model
        # remove duplicates (keep first registered for a base)
        seen = {}
        uniq = []
        for p in self.periphs:
            if p.base in seen:
                continue
            seen[p.base] = p
            uniq.append(p)
        self.periphs = uniq
        self._periph_bases = sorted(p.base for p in self.periphs)
        self._periph_by_base = {p.base: p for p in self.periphs}
        # attach SPI/GPIO wiring that display/input/rf expect
        self._wire_devices()

    def _wire_devices(self):
        self.rf_devices = []
        self.display_devices = []
        self.touch_devices = []
        self.spi = {p.name: p for p in self.periphs if isinstance(p, SPI)}
        self.uart = {p.name: p for p in self.periphs if isinstance(p, UART)}
        self.i2c = {p.name: p for p in self.periphs if isinstance(p, I2C)}
        self.gpio = {p.name: p for p in self.periphs if isinstance(p, GPIO)}
        self.timers = {p.name: p for p in self.periphs if isinstance(p, Timer)}
        if getattr(self, "lcd_model", None) is not None:
            try:
                self.lcd_model.attach_latch_gpio()
            except Exception:
                pass

    # ------------------------------------------------------------------
    # peripheral registry
    # ------------------------------------------------------------------
    def add_peripheral(self, p):
        self.periphs.append(p)
        return p

    def find_peripheral(self, addr):
        bases = self._periph_bases
        lo, hi = 0, len(bases)
        while lo < hi:
            mid = (lo + hi) // 2
            if bases[mid] <= addr:
                lo = mid + 1
            else:
                hi = mid
        if lo == 0:
            return None
        p = self._periph_by_base[bases[lo - 1]]
        if addr < p.base + p.size:
            return p
        return None

    # ------------------------------------------------------------------
    # unified memory helpers
    # ------------------------------------------------------------------
    def read_mem(self, addr, size):
        try:
            return int.from_bytes(self.uc.mem_read(addr, size), "little")
        except UcError:
            # MMIO or unmapped
            if self.find_peripheral(addr):
                return self._mmio_read(addr, size, addr)
            return 0

    def write_mem(self, addr, size, value):
        try:
            self.uc.mem_write(addr, value.to_bytes(size, "little"))
        except UcError:
            if self.find_peripheral(addr):
                self._mmio_write(addr, size, value, addr)

    def read_bytes(self, addr, length):
        try:
            return bytes(self.uc.mem_read(addr, length))
        except UcError:
            out = bytearray()
            for i in range(length):
                out.append(self.read_mem(addr + i, 1))
            return bytes(out)

    def peek_u32(self, addr):
        return self.read_mem(addr, 4)

    def reg(self, name):
        return self.uc.reg_read(_REG_BY_NAME[name])

    def setreg(self, name, value):
        self.uc.reg_write(_REG_BY_NAME[name], value)

    # ------------------------------------------------------------------
    # MMIO dispatch
    # ------------------------------------------------------------------
    def _make_mmio_read(self):
        def cb(uc, offset, size, user_data):
            return self._mmio_read(user_data + offset, size, offset)
        return cb

    def _make_mmio_write(self):
        def cb(uc, offset, size, value, user_data):
            self._mmio_write(user_data + offset, size, value, offset)
        return cb

    def _mmio_read(self, addr, size, offset=None):
        pc = self.uc.reg_read(UC_ARM_REG_PC)
        p = self.find_peripheral(addr)
        if p is None:
            val = self._unknown_access(addr, False, size, pc)
            return val
        try:
            val = p.read(addr - p.base, size)
        except Exception as e:
            self.log("peripheral %s read error @%#x: %r" % (p.name, addr, e))
            val = 0
        if self.mmio_log_enabled and self.trace_enabled:
            self.access.add(self.vtime, pc, False, addr, size, val, p.name)
        return val & 0xFFFFFFFF

    def _mmio_write(self, addr, size, value, offset=None):
        pc = self.uc.reg_read(UC_ARM_REG_PC)
        p = self.find_peripheral(addr)
        if p is None:
            self._unknown_access(addr, True, size, pc, value)
            return
        try:
            p.write(addr - p.base, size, value)
        except Exception as e:
            self.log("peripheral %s write error @%#x: %r" % (p.name, addr, e))
        if self.mmio_log_enabled and self.trace_enabled:
            self.access.add(self.vtime, pc, True, addr, size, value, p.name)
        # peripheral side-effects: GPIO CS lines -> SPI
        if isinstance(p, GPIO) and p.name in ("GPIOA", "GPIOB", "GPIOC", "GPIOD",
                                              "GPIOE"):
            self._on_gpio_changed(p)

    def _on_gpio_changed(self, gpio):
        pass  # extended by display/rf wiring via listeners

    def _unknown_access(self, addr, is_write, size, pc, value=0):
        self.access.note_unknown(addr, is_write, pc, size, value)
        if is_write:
            if self.unknown_policy == "last":
                self.unknown_values[addr] = value
            return 0
        if self.unknown_policy == "ones":
            return 0xFFFFFFFF
        if self.unknown_policy == "last":
            return self.unknown_values.get(addr, 0)
        return 0

    # ------------------------------------------------------------------
    # Unicorn hooks
    # ------------------------------------------------------------------
    def _hook_code(self, uc, address, size, user_data):
        self.insn_count += 1
        self.vtime += 1
        self.last_pc = address
        self.pc_ring.append(address)
        if self.pc_counter is not None:
            self.pc_counter[address] += 1
        if self.breakpoints:
            if self._bp_skip == address:
                self._bp_skip = None
            elif address in self.breakpoints:
                self.stop_reason = "breakpoint:%s@%#x" % (
                    self.breakpoints[address], address)
                uc.emu_stop()
                return
        if self.call_trace_enabled:
            self._maybe_call(address)
        if address == UNTIL_MAGIC - 2:
            self.stop_reason = "until_pc"
            uc.emu_stop()

    def _maybe_call(self, pc):
        try:
            h = int.from_bytes(self.uc.mem_read(pc, 4), "little")
        except UcError:
            return
        h1 = h & 0xFFFF
        h2 = (h >> 16) & 0xFFFF
        target = None
        if (h1 & 0xF800) == 0xF000:
            if (h2 & 0xD000) == 0xD000 or (h2 & 0xD000) == 0xC000:
                # BL / BLX immediate
                s = (h1 >> 10) & 1
                j1 = (h2 >> 13) & 1
                j2 = (h2 >> 11) & 1
                i1 = 1 - (j1 ^ s)
                i2 = 1 - (j2 ^ s)
                imm = (s << 24) | (i1 << 23) | (i2 << 22) | ((h1 & 0x3FF) << 12) | ((h2 & 0x7FF) << 1)
                if imm & 0x01000000:
                    imm -= 0x02000000
                target = (pc + 4 + imm) & 0xFFFFFFFF
        elif (h1 & 0xFF87) == 0x4780:  # BLX Rn
            r = (h1 >> 3) & 0xF
            if r == 15:
                return
            target = self.uc.reg_read(_REG_BY_NAME["R%d" % r]) & ~1
        if target is not None:
            self.call_targets[target] += 1
            self.call_events.append((self.vtime, pc, target,
                                     self.uc.reg_read(UC_ARM_REG_LR)))
            if not self.access.tx_bytes:
                pass

    def _hook_unmapped(self, uc, access, address, size, value, user_data):
        pc = uc.reg_read(UC_ARM_REG_PC)
        is_w = access == UC_MEM_WRITE_UNMAPPED
        if self.auto_map and address > 0x1000:
            page = address & ~0xFFF
            if page not in self.auto_mapped:
                self.auto_mapped.add(page)
                try:
                    uc.mem_map(page, 0x1000, UC_PROT_ALL)
                    self.log("[unmapped] %s pc=%#010x addr=%#010x size=%d -> "
                             "auto-mapped zero page" % ("W" if is_w else "R",
                                                        pc, address, size))
                    if not is_w:
                        return True
                    return True
                except UcError:
                    pass
        self.log("[FAULT] unmapped %s pc=%#010x addr=%#010x size=%d value=%#x"
                 % ("W" if is_w else "R", pc, address, size, value))
        self.stop_reason = "fault:unmapped_%s@%#x(pc=%#x)" % (
            "w" if is_w else "r", address, pc)
        self.fault = self.stop_reason
        return False

    def _hook_fetch_unmapped(self, uc, access, address, size, value, user_data):
        pc = uc.reg_read(UC_ARM_REG_PC)
        # EXC_RETURN: exception return
        if (address & EXC_RETURN_MASK) == EXC_RETURN_MASK or \
           (pc & EXC_RETURN_MASK) == EXC_RETURN_MASK:
            self._exception_return(pc)
            return False
        self.log("[FAULT] fetch unmapped pc=%#010x addr=%#010x" % (pc, address))
        self.stop_reason = "fault:fetch@%#x" % address
        self.fault = self.stop_reason
        return False

    def _hook_invalid(self, uc, access, address, size, value, user_data):
        pc = uc.reg_read(UC_ARM_REG_PC)
        self.log("[FAULT] invalid access=%d pc=%#010x addr=%#010x size=%d"
                 % (access, pc, address, size))
        self.stop_reason = "fault:invalid@%#x(pc=%#x)" % (address, pc)
        self.fault = self.stop_reason
        return False

    def add_mem_hook(self, begin, end, kind, fn):
        """kind: 'r', 'w' -- watchpoint helper."""
        if kind == "r":
            flags = UC_HOOK_MEM_READ
        else:
            flags = UC_HOOK_MEM_WRITE
        self.uc.hook_add(flags, fn, None, begin, end)

    # ------------------------------------------------------------------
    # boot / reset / run
    # ------------------------------------------------------------------
    def reset_from_vector(self, sp=None, pc=None):
        if sp is None:
            sp = self.peek_u32(self.vtor)
        if pc is None:
            pc = self.peek_u32(self.vtor + 4) & ~1
        self.uc.reg_write(UC_ARM_REG_SP, sp)
        self.uc.reg_write(UC_ARM_REG_PC, pc | 1)
        self.active_exception = None
        return sp, pc

    def reset_from_entry(self, entry, sp=None):
        if sp is None:
            sp = self.image_base + 0x0 if False else 0x200477C0
        self.uc.reg_write(UC_ARM_REG_SP, sp)
        self.uc.reg_write(UC_ARM_REG_PC, entry | 1)
        self.active_exception = None

    def boot(self):
        """Full hardware-ish reset: SP/PC from vector table at image base."""
        return self.reset_from_vector()

    def soft_reset(self):
        self.uc.reg_write(UC_ARM_REG_SP, self.peek_u32(self.vtor))
        self.uc.reg_write(UC_ARM_REG_PC, self.peek_u32(self.vtor + 4) | 1)
        self.active_exception = None
        self.pending_irqs.clear()

    def request_reset(self):
        self._in_reset = True

    def pend_irq(self, irq):
        if 0 <= irq <= 240:
            self.pending_irqs.add(irq)
            if hasattr(self, "ppb"):
                self.ppb.nvic_pending = self.ppb._pending_mask()
            return True
        return False

    def note_exti(self, mask):
        """EXTI software interrupt: map pin -> IRQ is left to input.py wiring."""
        self.last_exti_swier = mask

    # --- exception machinery ------------------------------------------
    def take_exception(self, exc_num):
        uc = self.uc
        sp = uc.reg_read(UC_ARM_REG_SP)
        frame = [uc.reg_read(r) for r in (UC_ARM_REG_R0, UC_ARM_REG_R1,
                                          UC_ARM_REG_R2, UC_ARM_REG_R3,
                                          UC_ARM_REG_R12, UC_ARM_REG_LR)]
        ret = uc.reg_read(UC_ARM_REG_PC)
        xpsr = 0x01000000 | (exc_num & 0x1FF)
        frame += [ret, xpsr]
        sp -= 0x20
        self.uc.mem_write(sp, b"".join(struct.pack("<I", v & 0xFFFFFFFF)
                                       for v in frame))
        uc.reg_write(UC_ARM_REG_SP, sp)
        vec = self.peek_u32(self.vtor + 4 * exc_num) & ~1
        if vec == 0:
            self.log("exception %d has a NULL vector" % exc_num)
            return False
        uc.reg_write(UC_ARM_REG_LR, 0xFFFFFFF9)
        uc.reg_write(UC_ARM_REG_PC, vec | 1)
        self.active_exception = exc_num
        self.log("[exc] enter IRQ %d -> handler %#010x (sp=%#x, ret=%#010x)"
                 % (exc_num, vec, sp, ret))
        return True

    def _exception_return(self, lr_value):
        uc = self.uc
        exc = self.active_exception
        sp = uc.reg_read(UC_ARM_REG_SP)
        try:
            raw = bytes(uc.mem_read(sp, 0x20))
        except UcError:
            self.log("[exc] exception return with bad SP=%#x" % sp)
            self.stop_reason = "fault:exc_return_sp"
            self.fault = self.stop_reason
            return False
        vals = list(struct.unpack("<8I", raw))
        for reg, val in zip((UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                             UC_ARM_REG_R3, UC_ARM_REG_R12, UC_ARM_REG_LR),
                            vals[:6]):
            uc.reg_write(reg, val)
        pc = vals[6]
        uc.reg_write(UC_ARM_REG_SP, sp + 0x20)
        uc.reg_write(UC_ARM_REG_PC, pc | 1)
        self.active_exception = None
        self._bp_skip = None
        return True

    def _check_interrupts(self):
        if self.active_exception is not None:
            return False
        if not self.pending_irqs:
            return False
        primask = 0
        try:
            primask = self.uc.reg_read(UC_ARM_REG_PRIMASK)
        except (UcError, AttributeError):
            pass
        if primask:
            return False
        # pick lowest-numbered pending IRQ that is enabled in the NVIC
        enabled = self.ppb.nvic_enabled
        cands = [i for i in self.pending_irqs
                 if i >= 16 and ((enabled >> (i - 16)) & 1)]
        cands.sort()
        if not cands:
            return False
        irq = cands[0]
        self.pending_irqs.discard(irq)
        self.ppb.nvic_pending = self.ppb._pending_mask()
        return self.take_exception(irq)

    # --- main run loop ------------------------------------------------
    def run(self, max_insns=None, until_pc=None, chunk=2000):
        """Execute until stop reason / max instructions. Returns stop reason."""
        if (not self.fine) and until_pc is None and not self.breakpoints:
            return self._run_fast(max_insns)
        uc = self.uc
        self.stop_reason = None
        start_insn = self.insn_count
        budget = max_insns
        self._until_pc = until_pc
        if until_pc is not None:
            self.breakpoints.setdefault(until_pc, "until")
            self._until_bp = until_pc
        else:
            self._until_bp = None
        first = True
        while True:
            if budget is not None:
                remaining = budget - (self.insn_count - start_insn)
                if remaining <= 0:
                    self.stop_reason = self.stop_reason or "max_insn"
                    break
                n = min(chunk, remaining)
            else:
                n = chunk
            # pending system reset?
            while self._in_reset:
                self._do_reset()
            # interrupts at chunk boundary
            if self.interrupt_after_chunk and self._check_interrupts():
                pass
            # sys tick accounting
            self.ppb.systick_update()
            # prepare thumb state (unicorn wants odd PC to mean Thumb)
            pc = uc.reg_read(UC_ARM_REG_PC)
            if pc & 1:
                pc &= ~1
            try:
                uc.reg_write(UC_ARM_REG_PC, pc | 1)
                uc.emu_start(pc | 1, UNTIL_MAGIC, 0, n)
            except UcError as e:
                if self._handle_uc_error(e):
                    continue
                break
            finally:
                first = False
            if self.stop_reason:
                break
            if self._in_reset:
                continue
            if self.progress_fn is not None and self.progress_every:
                if self.insn_count - getattr(self, "_last_progress", 0) >= \
                        self.progress_every:
                    self._last_progress = self.insn_count
                    try:
                        self.progress_fn(self)
                    except Exception:
                        pass
            if budget is None and self._until_bp is None:
                # no explicit budget: single chunk per call (interactive stepping)
                break
        if until_pc is not None and self.breakpoints.get(until_pc) == "until":
            del self.breakpoints[until_pc]
            self._bp_skip = None
        return self.stop_reason

    def _run_fast(self, budget, chunk=200000):
        """Burst-mode run: no per-instruction hook, PC sampled per chunk."""
        uc = self.uc
        self.stop_reason = None
        if budget is None:
            budget = chunk
        start = self.insn_count
        while True:
            remaining = budget - (self.insn_count - start)
            if remaining <= 0:
                self.stop_reason = self.stop_reason or "max_insn"
                break
            n = min(chunk, remaining)
            while self._in_reset:
                self._do_reset()
            self._check_interrupts()
            self.ppb.systick_update()
            pc = uc.reg_read(UC_ARM_REG_PC) & ~1
            try:
                uc.reg_write(UC_ARM_REG_PC, pc | 1)
                uc.emu_start(pc | 1, UNTIL_MAGIC, 0, n)
                self.insn_count += n
                self.vtime += n
            except UcError as e:
                # unknown number executed; assume half the chunk
                self.insn_count += n // 2
                self.vtime += n // 2
                if self._handle_uc_error(e):
                    continue
                break
            end_pc = uc.reg_read(UC_ARM_REG_PC) & ~1
            self.last_pc = end_pc
            self.pc_ring.append(end_pc)
            if self.progress_fn is not None and self.progress_every:
                if self.insn_count - getattr(self, "_last_progress", 0) >= \
                        self.progress_every:
                    self._last_progress = self.insn_count
                    try:
                        self.progress_fn(self)
                    except Exception:
                        pass
            if self._in_reset:
                continue
        return self.stop_reason

    def _handle_uc_error(self, e):
        uc = self.uc
        pc = uc.reg_read(UC_ARM_REG_PC)
        # WFI/WFE may be unsupported in some unicorn builds: emulate as nop
        try:
            h = int.from_bytes(uc.mem_read(pc, 2), "little")
        except UcError:
            h = None
        if h in (0xBF30, 0xBF20, 0xBF10, 0xBF40):
            if h == 0xBF30 and (self.pending_irqs or self.ppb.st_ctrl & 2):
                # let time advance so SysTick can fire
                self.vtime += 1
                self.ppb.systick_update()
            uc.reg_write(UC_ARM_REG_PC, (pc + 2) | 1)
            self.stop_reason = None
            return True
        if str(e).startswith("Invalid instruction"):
            # a second chance: treat as UDF -> hardfault
            if self.hardfault:
                return self._enter_hardfault(pc, "invalid_insn")
            self.stop_reason = "fault:invalid_insn@%#x" % pc
            self.fault = self.stop_reason
            return False
        if self.hardfault:
            return self._enter_hardfault(pc, str(e))
        self.stop_reason = "fault:%s@%#x" % (e, pc)
        self.fault = self.stop_reason
        return False

    def _enter_hardfault(self, pc, why):
        # record CFSR-ish, then tail-chain into HardFault
        self.ppb.hfsr |= 0x40000000
        self.ppb.cfsr |= 0x00000001 | 0x00000002      # IACCVIOL, INVSTATE
        self.log("[FAULT] %s at pc=%#010x -> taking HardFault" % (why, pc))
        if self.take_exception(3):
            self.stop_reason = None
            self.fault = None
            return True
        self.stop_reason = "fault:%s@%#x" % (why, pc)
        self.fault = self.stop_reason
        return False

    # ------------------------------------------------------------------
    # stepping API
    # ------------------------------------------------------------------
    def step(self, n=1):
        return self.run(max_insns=n, chunk=1)

    def step_over(self, max_steps=200000):
        """Step over a BL/BLX call (naive: run to next PC after current insn)."""
        pc = self.uc.reg_read(UC_ARM_REG_PC) & ~1
        try:
            size = 4 if (int.from_bytes(self.uc.mem_read(pc, 2), "little") & 0xF800) == 0xF000 else 2
        except UcError:
            size = 2
        next_pc = pc + size
        return self.run_until(next_pc, max_insns=max_steps)

    def run_until(self, addr, max_insns=None):
        self.breakpoints[addr] = "until"
        try:
            reason = self.run(max_insns=max_insns)
        finally:
            if self.breakpoints.get(addr) == "until":
                del self.breakpoints[addr]
        self._bp_skip = addr if self.stop_reason and \
            self.stop_reason.startswith("breakpoint:until") else None
        return reason

    # ------------------------------------------------------------------
    # logging / summaries
    # ------------------------------------------------------------------
    def log(self, msg):
        line = "[t=%d] %s" % (self.vtime, msg)
        self.log_lines.append(line)
        if len(self.log_lines) > 20000:
            del self.log_lines[:10000]
        try:
            self.log_fn(line)
        except Exception:
            pass

    def unknown_mmio_report(self, limit=40):
        rows = []
        for addr, ent in self.access.unknown.items():
            rows.append((ent[0] + ent[1], addr, ent))
        rows.sort(reverse=True)
        out = []
        for _, addr, ent in rows[:limit]:
            pcs = ", ".join("%#x(%s)" % (p, d) for (p, d, s, v) in ent[2])
            out.append("  %#010x  R=%d W=%d  touched@ %s"
                       % (addr, ent[0], ent[1], pcs))
        return "\n".join(out)

    def hot_mmio_report(self, n=25):
        out = []
        for cnt, d, addr, size, pcs in self.access.hot(n):
            p = self.find_peripheral(addr)
            name = p.name if p else "UNKNOWN"
            off = (addr - p.base) if p else 0
            decode = p.decode(off) if (p and hasattr(p, "decode")) else ""
            out.append("  %8d %s %#010x %s+%#x size=%d %s  pcs=%s"
                       % (cnt, d, addr, name, off, size, decode,
                          ", ".join("%#x" % x for x in pcs[:3])))
        return "\n".join(out)

    def register_string(self):
        uc = self.uc
        names = ["R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9",
                 "R10", "R11", "R12", "SP", "LR", "PC"]
        vals = [(n, uc.reg_read(_REG_BY_NAME[n])) for n in names]
        parts = []
        for i in range(0, len(vals), 4):
            parts.append("  ".join("%-4s %08X" % kv for kv in vals[i:i + 4]))
        return "\n".join(parts)

    def pc_history(self, n=40):
        return list(self.pc_ring)[-n:]

    def detect_hang(self, window=64):
        """Return (is_hang, addr_range) using the PC ring buffer."""
        pcs = list(self.pc_ring)
        if len(pcs) < window * 2:
            return False, None
        tail = pcs[-window:]
        lo, hi = min(tail), max(tail)
        # tight loop: PC stays inside a small window
        if hi - lo <= 0x40:
            return True, (lo, hi)
        return False, None

    def summary(self, n_pcs=32):
        pc = self.uc.reg_read(UC_ARM_REG_PC) & ~1
        lines = []
        lines.append("instructions executed : %d (vtime=%d)" % (
            self.insn_count, self.vtime))
        lines.append("current PC            : %#010x  LR=%#010x  SP=%#010x"
                     % (pc, self.uc.reg_read(UC_ARM_REG_LR),
                        self.uc.reg_read(UC_ARM_REG_SP)))
        lines.append("stop reason           : %s" % (self.stop_reason,))
        if self.fault:
            lines.append("fault                 : %s" % self.fault)
        lines.append("last %d PCs           : %s" % (
            n_pcs, " ".join("%#x" % x for x in self.pc_history(n_pcs))))
        lines.append("MMIO accesses         : %d (events=%d)" % (
            sum(self.access.per_addr.values()), self.access.count))
        lines.append("unknown MMIO addrs    : %d" % len(self.access.unknown))
        lines.append("peripheral hit counts : %s" % dict(
            self.access.per_periph.most_common(12)))
        return "\n".join(lines)


_REG_BY_NAME = {
    "R0": UC_ARM_REG_R0, "R1": UC_ARM_REG_R1, "R2": UC_ARM_REG_R2,
    "R3": UC_ARM_REG_R3, "R4": UC_ARM_REG_R4, "R5": UC_ARM_REG_R5,
    "R6": UC_ARM_REG_R6, "R7": UC_ARM_REG_R7, "R8": UC_ARM_REG_R8,
    "R9": UC_ARM_REG_R9, "R10": UC_ARM_REG_R10, "R11": UC_ARM_REG_R11,
    "R12": UC_ARM_REG_R12, "SP": UC_ARM_REG_SP, "LR": UC_ARM_REG_LR,
    "PC": UC_ARM_REG_PC,
}


def load_symbol_file(path):
    """Load a symbol map: JSON {"name": addr} or text lines 'addr name'."""
    syms = {}
    if not path or not os.path.exists(path):
        return syms
    if path.endswith(".json"):
        with open(path, "r", encoding="utf-8") as f:
            raw = json.load(f)
        for k, v in raw.items():
            if isinstance(v, dict):
                v = v.get("addr", v.get("address"))
            if isinstance(v, (int, str)):
                syms[k] = int(v, 0) if isinstance(v, str) else v
        return syms
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split(None, 1)
            if len(parts) == 2:
                try:
                    syms[parts[1]] = int(parts[0], 16)
                except ValueError:
                    pass
            elif len(parts) == 1:
                try:
                    syms[parts[0]] = int(parts[0], 16)
                except ValueError:
                    pass
    return syms


def demangle(machine, addr):
    for name, a in machine.symbols.items():
        if a == addr:
            return name
    return None
