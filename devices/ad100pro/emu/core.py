"""Cortex-M0 (STM32F0) firmware emulation core for the Godox AD100 Pro.

Builds on Unicorn 2.1.4.  Design notes / discovered facts:

* The vendor image ``firmware/AD100pro_V1.2.bin`` (50248 bytes) is the *application*
  image and is linked at **0x08003000** (flash 0x08000000..0x08010000 is 64 KiB;
  0x08000000..0x08002FFF is the (absent) bootloader).
* Application vector table lives at 0x08003000, Reset_Handler = 0x080030D5.
* Unicorn needs ``emu_start`` begin address to carry the Thumb bit (addr | 1);
  with an even begin address UC_MODE_MCLASS starts in ARM state and raises
  UC_ERR_INSN_INVALID.  We therefore always pass ``pc | 1``.
* MMIO is emulated by mapping the peripheral windows and using
  UC_HOOK_MEM_READ / UC_HOOK_MEM_WRITE over exactly those windows.  On read we
  write the peripheral value back into the mapped page *before* the CPU load
  completes, so the load sees the stubbed value (verified on unicorn 2.1.4).
* Instruction counting is exact: a UC_HOOK_BLOCK hook accumulates the length of
  the previously executed translation block (computed on demand with capstone,
  cached).  Batching stops are made at TB boundaries via emu_stop(), so the
  counter never drifts.
"""

import bisect
import json
import os
import re
import struct
import threading
import time

from unicorn import *
from unicorn import arm_const
from unicorn.arm_const import *

try:
    import cc2500 as _cc2500
except Exception:  # pragma: no cover - optional module
    _cc2500 = None

HERE = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(HERE)
DEFAULT_FIRMWARE = os.path.join(PROJECT_ROOT, 'firmware', 'AD100pro_V1.2.bin')
DEFAULT_FUNCTIONS = os.path.join(PROJECT_ROOT, 'out', 'functions.json')

# ---------------------------------------------------------------------------
# memory map
# ---------------------------------------------------------------------------
ROM_BASE = 0x08000000
ROM_SIZE = 0x20000          # 128 KiB flash
APP_BASE = 0x08003000       # application image link address
IMAGE_END = 0x0800F448      # 0x08003000 + 50248 (end of the update image)
CALIB_BASE = 0x0801F000     # factory calibration records (not in the update file)
RAM_BASE = 0x20000000
RAM_SIZE = 0x2000           # 8 KiB SRAM
VECTOR_BASE_ALIAS = 0x00000000   # CPU view of the vector table after remap
VECTOR_ALIAS_SIZE = 0x1000
TRAMP_ADDR = 0x0800F900          # manual-ISR return trampoline ('b .')
RAM_VECTOR_BASE = 0x20000000     # firmware copies its 48 vectors here in main()
DEFAULT_HANDLER = 0x080030E6     # Default_Handler: 'b .' infinite loop

MMIO_WINDOWS = (
    (0x40000000, 0x00025000),   # APB/AHB peripherals
    (0x48000000, 0x00002000),   # GPIOA..GPIOF
    (0xE0000000, 0x0000F000),   # NVIC / SCB / SysTick / ITM / DWT
)

# unicorn access-type -> trace direction ------------------------------------
_ACCESS_NAMES = {
    UC_MEM_READ: 'R', UC_MEM_WRITE: 'W',
    UC_MEM_FETCH: 'X',
    UC_MEM_READ_UNMAPPED: 'R?', UC_MEM_WRITE_UNMAPPED: 'W?',
    UC_MEM_FETCH_UNMAPPED: 'X?',
    UC_MEM_WRITE_PROT: 'Wp', UC_MEM_READ_PROT: 'Rp',
    UC_MEM_FETCH_PROT: 'Xp',
}


# ---------------------------------------------------------------------------
# generic helpers
# ---------------------------------------------------------------------------
def _u32(b):
    return struct.unpack('<I', b)[0]


class FaultRecord:
    __slots__ = ('kind', 'pc', 'lr', 'sp', 'addr', 'insn', 'detail')

    def __init__(self, kind, pc, lr, sp, addr, insn, detail=''):
        self.kind = kind
        self.pc = pc
        self.lr = lr
        self.sp = sp
        self.addr = addr
        self.insn = insn
        self.detail = detail

    def to_dict(self):
        return dict(kind=self.kind, pc=self.pc, lr=self.lr, sp=self.sp,
                    addr=self.addr, insn=self.insn, detail=self.detail)


class TraceEntry:
    """One MMIO access."""
    __slots__ = ('insn', 'pc', 'addr', 'dev', 'reg', 'rw', 'value', 'size', 'func')

    def __init__(self, insn, pc, addr, dev, reg, rw, value, size, func):
        self.insn = insn
        self.pc = pc
        self.addr = addr
        self.dev = dev
        self.reg = reg
        self.rw = rw
        self.value = value
        self.size = size
        self.func = func

    def line(self):
        fn = ('%s@%08X' % (self.func[1], self.func[0])) if self.func else '-'
        return ('[%9d] %08X  %-11s %-12s %s 0x%08X  sz=%d  %s'
                % (self.insn, self.pc, self.dev, self.reg, self.rw,
                   self.value & 0xFFFFFFFF, self.size, fn))

    def to_dict(self):
        return dict(insn=self.insn, pc=self.pc, addr=self.addr, dev=self.dev,
                    reg=self.reg, rw=self.rw, value=self.value & 0xFFFFFFFF,
                    size=self.size,
                    func=(dict(addr=self.func[0], name=self.func[1])
                          if self.func else None))


# ---------------------------------------------------------------------------
# peripherals
# ---------------------------------------------------------------------------
class Peripheral:
    """Base class: stored register file with hooks for special behaviour."""
    name = 'PERIPH'
    base = 0
    size = 0x400
    reg_names = {}
    reset_value = 0
    write_only = ()

    def __init__(self, emu):
        self.emu = emu
        self.regs = {}
        self.reset()

    # -- API used by the bus -------------------------------------------------
    def reset(self):
        self.regs = {}

    def read(self, off):
        return self.regs.get(off, self.reset_value)

    def write(self, off, val):
        self.regs[off] = val & 0xFFFFFFFF

    def reg_name(self, off):
        nm = self.reg_names.get(off)
        if nm:
            return nm
        return 'REG_%02X' % off

    def peek_raw(self, off):
        return self.regs.get(off, 0)

    def state(self):
        """JSON friendly register dump (named)."""
        out = {}
        for off, nm in sorted(self.reg_names.items()):
            if off in self.regs:
                out[nm] = self.regs[off]
            elif self.read(off):
                out[nm] = self.read(off)
        # special virtual registers
        for off, val in sorted(self.snapshot_extra().items()):
            out[off] = val
        return out

    def snapshot_extra(self):
        return {}


class GPIO(Peripheral):
    name = 'GPIO'
    size = 0x400
    reg_names = {
        0x00: 'MODER', 0x04: 'OTYPER', 0x08: 'OSPEEDR', 0x0C: 'PUPDR',
        0x10: 'IDR', 0x14: 'ODR', 0x18: 'BSRR', 0x1C: 'LCKR',
        0x20: 'AFRL', 0x24: 'AFRH', 0x28: 'BRR',
    }
    write_only = (0x18,)          # BSRR

    def __init__(self, emu, name):
        self.name = name
        self.base = GPIO_BASES[name]
        super().__init__(emu)

    def reset(self):
        super().reset()
        # input levels forced through inject(); bit value (0/1) per pin
        self.inject = {}

    def read(self, off):
        if off == 0x10:                     # IDR
            idr = self._input_levels()
            try:
                idr = self.emu.inputs.idr_for(self.name, idr, self.emu)
            except Exception:
                pass
            try:
                cc = getattr(self.emu, 'cc2500', None)
                if cc is not None:
                    for meth in ('adjust_idr', 'gpio_idr', 'apply_idr', 'idr'):
                        fn = getattr(cc, meth, None)
                        if callable(fn):
                            idr = fn(self.name, idr) & 0xFFFF
                            break
            except Exception:
                pass
            for bit, val in self.inject.items():
                if val:
                    idr |= (1 << bit)
                else:
                    idr &= ~(1 << bit) & 0xFFFF
            return idr & 0xFFFF
        if off in (0x18, 0x28):             # BSRR / BRR are write-only
            return 0
        return self.regs.get(off, 0)

    def _input_levels(self):
        """Hardware-like pin levels: outputs read back ODR, inputs read their
        pull-up/pull-down level (F0 GPIOs default to pull-up = 1 when a column
        is configured as input, which the key scan relies on)."""
        moder = self.regs.get(0x00, 0)
        pupdr = self.regs.get(0x0C, 0)
        odr = self.regs.get(0x14, 0)
        idr = 0
        for pin in range(16):
            mode = (moder >> (2 * pin)) & 3
            if mode == 0:                   # input
                pupd = (pupdr >> (2 * pin)) & 3
                if pupd == 1:               # pull-up
                    idr |= (1 << pin)
            else:                           # output / AF: read back the latch
                if odr & (1 << pin):
                    idr |= (1 << pin)
        return idr

    def write(self, off, val):
        if off == 0x18:                     # BSRR: low half set, high half reset
            odr = self.regs.get(0x14, 0)
            setbits = val & 0xFFFF
            clrbits = (val >> 16) & 0xFFFF
            odr = (odr | setbits) & ~clrbits & 0xFFFF
            self.regs[0x14] = odr
            self.emu.on_gpio_pins(self.name, setbits, clrbits)
            return
        if off == 0x28:                     # BRR: reset bits
            odr = self.regs.get(0x14, 0)
            clrbits = val & 0xFFFF
            odr &= ~clrbits & 0xFFFF
            self.regs[0x14] = odr
            self.emu.on_gpio_pins(self.name, 0, clrbits)
            return
        if off == 0x14:                     # ODR
            old = self.regs.get(0x14, 0)
            new = val & 0xFFFF
            self.regs[0x14] = new
            self.emu.on_gpio_pins(self.name, new & ~old, old & ~new)
            return
        self.regs[off] = val & 0xFFFFFFFF

    def snapshot_extra(self):
        return {'ODR': self.regs.get(0x14, 0) & 0xFFFF,
                'IDR': self.read(0x10)}


GPIO_BASES = {'GPIOA': 0x48000000, 'GPIOB': 0x48000400, 'GPIOC': 0x48000800,
              'GPIOD': 0x48000C00, 'GPIOE': 0x48001000, 'GPIOF': 0x48001400}


class VirtualInputs:
    """Thread-safe virtual user-input state (buttons / battery / ADC).

    Physical evidence (docs/04_ui_lcd.md, func_08008D00):
      * key matrix rows = GPIOB 3/4/5 (driven low one at a time, output),
        columns = GPIOC 10/11/12/7 and GPIOD 4 (input with pull-up).
      * a pressed matrix key pulls its column low while its row is driven low.
      * EXTI keys: power = PB0 / EXTI0, key2 = PA6 / EXTI6 (active low).
    Key ids accepted by the API:
      power, key2, mode, set, up, down, menu, group, ch,
      k0..k9 (raw matrix bits, see MATRIX) and r{row}c{col}.
    """

    # key_id -> (row port, row pin, column port, column pin)
    MATRIX = {
        'k0': ('GPIOB', 3, 'GPIOC', 10),
        'k1': ('GPIOB', 3, 'GPIOC', 11),
        'k2': ('GPIOB', 3, 'GPIOC', 12),
        'k3': ('GPIOB', 4, 'GPIOC', 10),
        'k4': ('GPIOB', 4, 'GPIOC', 11),
        'k5': ('GPIOB', 5, 'GPIOC', 10),
        'k6': ('GPIOB', 5, 'GPIOC', 11),
        'k7': ('GPIOB', 5, 'GPIOC', 12),
        'k8': ('GPIOB', 5, 'GPIOD', 4),
        'k9': ('GPIOB', 5, 'GPIOC', 7),
    }
    # provisional UI button names -> physical key
    ALIASES = {
        'power': 'power', 'pwr': 'power', 'pwrkey': 'power', 'key1': 'power',
        'key2': 'key2', 'aux': 'key2', 'spare': 'key2',
        'mode': 'k0', 'set': 'k1', 'up': 'k2', 'down': 'k3',
        'menu': 'k4', 'group': 'k5', 'channel': 'k6', 'ch': 'k6',
        'minus': 'k7', 'plus': 'k8', 'enter': 'k9',
    }
    EXTI_KEYS = {
        'power': ('GPIOB', 0, 0, 5),     # (port, pin, exTI line, NVIC IRQ)
        'key2': ('GPIOA', 6, 6, 7),      # EXTI4_15 -> IRQ7
    }
    # 1 emulated "ms" of key_press = this many executed instructions
    # (emulation runs far slower than the real 48 MHz CPU, so a pure 48k/ms
    #  would make an 80 ms pulse take ~1.4 s of wall time)
    MS_SCALE = 8000
    BATTERY_FULL_MV = 4200
    BATTERY_EMPTY_MV = 2600
    BATTERY_FULL_RAW = 950      # ADC raw at "full" per 0x0800A034 threshold
    DEFAULT_BATTERY_MV = 3700

    def __init__(self, emu):
        self.emu = emu
        self.lock = threading.RLock()
        self.keys = {}          # canonical key id -> bool
        self.release_at = {}    # canonical key id -> instruction deadline
        self.battery_mv = self.DEFAULT_BATTERY_MV
        self.adc_mv = {}        # channel -> millivolts
        self.last = None
        self.total_events = 0

    # -- reset --------------------------------------------------------------
    def reset(self):
        with self.lock:
            self.keys = {}
            self.release_at = {}
            self.battery_mv = self.DEFAULT_BATTERY_MV
            self.adc_mv = {}
            self.last = None
            self.total_events = 0

    # -- naming -------------------------------------------------------------
    def canonical(self, name):
        if name is None:
            return None
        n = str(name).strip().lower()
        if n in self.ALIASES:
            return self.ALIASES[n]
        if n in self.MATRIX or n in self.EXTI_KEYS:
            return n
        m = re.match(r'^r([0-2])c([0-4])$', n)
        if m:
            return 'k%d' % (int(m.group(1)) * 3 + int(m.group(2)))
        if n.startswith('key') and n[3:].isdigit():
            return 'k%s' % n[3:]
        return None

    def is_down(self, name):
        key = self.canonical(name)
        with self.lock:
            return bool(self.keys.get(key, False))

    def pressed_list(self):
        with self.lock:
            return [k for k, v in sorted(self.keys.items()) if v]

    # -- injection ----------------------------------------------------------
    def set_key(self, name, down, trigger_exti=True):
        key = self.canonical(name)
        if key is None:
            return False, 'unknown key %r' % name
        with self.lock:
            was = self.keys.get(key, False)
            self.keys[key] = bool(down)
            if not down:
                self.release_at.pop(key, None)
            self.total_events += 1
            self.last = dict(type='key_down' if down else 'key_up', key=key,
                             t=time.time())
        if down and not was and trigger_exti and key in self.EXTI_KEYS:
            self._trigger_exti(key)
        return True, key

    def press(self, name, ms=80):
        ok, key = self.set_key(name, True)
        if not ok:
            return ok, key
        with self.lock:
            self.release_at[key] = self.emu.insn_count + max(1, int(ms)) * self.MS_SCALE
            self.last = dict(type='key_press', key=key, ms=ms, t=time.time())
        return True, key

    def release_due(self, insn):
        with self.lock:
            if not self.release_at:
                return None
            due = [k for k, deadline in self.release_at.items() if insn >= deadline]
            for k in due:
                self.keys[k] = False
                del self.release_at[k]
                self.last = dict(type='key_up', key=k, t=time.time())
        return due[0] if due else None

    def _trigger_exti(self, key):
        port, pin, line, irq = self.EXTI_KEYS[key]
        self.trigger_exti_line(line, irq, source=key)

    def trigger_exti_line(self, line, irq, source='EXTI'):
        """Set EXTI pending + NVIC pending and request a real handler call."""
        try:
            self.emu.exti.pending |= (1 << line)
            self.emu.scb.pending |= (1 << irq)
            self.emu.note_irq('%s%d' % (source, line), 1)
            self.emu.note_event('EXTI%d' % line, 1)
            # hand the exception number (16 + IRQn) to the dispatcher
            self.emu.request_irq(16 + irq, source='%s%d' % (source, line))
        except Exception:
            pass

    def set_battery(self, mv):
        mv = int(mv)
        mv = max(self.BATTERY_EMPTY_MV, min(self.BATTERY_FULL_MV, mv))
        with self.lock:
            self.battery_mv = mv
            self.total_events += 1
            self.last = dict(type='battery', mv=mv, t=time.time())
        # ADC raw heuristic: full 4200 mV -> 950 (threshold used by 0x0800A034)
        raw = int(round(mv * self.BATTERY_FULL_RAW / float(self.BATTERY_FULL_MV)))
        raw = max(0, min(0xFFF, raw))
        emu = self.emu
        try:
            emu.adc.set_channel(10, raw)
            emu.adc.set_channel(11, raw)
            emu.adc.set_channel(12, raw)
            emu.adc.default = raw
            # firmware battery state machine polls these RAM words (DMA targets)
            emu.write_mem(0x2000020A, struct.pack('<H', raw))
            emu.write_mem(0x2000020C, struct.pack('<H', raw))
        except Exception:
            pass
        return raw

    def set_adc(self, channel, mv):
        channel = int(channel)
        mv = int(mv)
        with self.lock:
            self.adc_mv[channel] = mv
            self.total_events += 1
            self.last = dict(type='adc', ch=channel, mv=mv, t=time.time())
        raw = max(0, min(0xFFF, int(round(mv * 0xFFF / 3300.0))))
        try:
            self.emu.adc.set_channel(channel, raw)
            if channel == 10:
                self.emu.write_mem(0x2000020A, struct.pack('<H', raw))
        except Exception:
            pass
        return raw

    def clear(self):
        with self.lock:
            self.keys = {}
            self.release_at = {}
            self.adc_mv = {}
            self.battery_mv = self.DEFAULT_BATTERY_MV
            self.last = dict(type='clear_inputs', t=time.time())
        try:
            self.emu.exti.pending = 0
            self.emu.scb.pending = 0
        except Exception:
            pass
        self.set_battery(self.DEFAULT_BATTERY_MV)
        return True

    # -- GPIO semantics -----------------------------------------------------
    def idr_for(self, port, idr, emu):
        """Apply pressed keys to the GPIO input-data-register value."""
        with self.lock:
            pressed = {k for k, v in self.keys.items() if v}
        if not pressed:
            return idr
        for key in pressed:
            if key in self.MATRIX:
                row_port, row_pin, col_port, col_pin = self.MATRIX[key]
                if port != col_port:
                    continue
                row = emu.devices.get(row_port)
                if row is None:
                    continue
                moder = row.regs.get(0x00, 0)
                odr = row.regs.get(0x14, 0)
                mode = (moder >> (2 * row_pin)) & 3
                if mode == 1 and not (odr & (1 << row_pin)):
                    idr &= ~(1 << col_pin)      # pressed -> column pulled low
            elif key in self.EXTI_KEYS:
                kport, kpin = self.EXTI_KEYS[key][0], self.EXTI_KEYS[key][1]
                if port == kport:
                    idr &= ~(1 << kpin)         # button to GND, active low
        return idr & 0xFFFF

    # -- serialisation ------------------------------------------------------
    def battery_pct(self):
        mv = self.battery_mv
        span = float(self.BATTERY_FULL_MV - self.BATTERY_EMPTY_MV)
        return max(0, min(100, int(round((mv - self.BATTERY_EMPTY_MV) / span * 100))))

    def state(self):
        with self.lock:
            keys = dict(self.keys)
            last = dict(self.last) if self.last else None
            adc = {('ch%d' % ch): mv for ch, mv in sorted(self.adc_mv.items())}
            return dict(
                keys={'power': keys.get('power', False),
                      'key2': keys.get('key2', False),
                      'mode': keys.get('k0', False),
                      'set': keys.get('k1', False),
                      'up': keys.get('k2', False),
                      'down': keys.get('k3', False),
                      'matrix': {k: v for k, v in sorted(keys.items())
                                 if k in self.MATRIX},
                      'all': keys},
                pressed=sorted(k for k, v in keys.items() if v),
                battery_mv=self.battery_mv,
                battery_pct=self.battery_pct(),
                adc=adc,
                last=last,
                events=self.total_events,
                press_ms_scale=self.MS_SCALE,
            )


class LCDDecoder:
    """Decode the firmware's bit-banged SPI LCD bus into a frame buffer.

    Verified from func_0800A480 (byte-write helper):
        SCL  = GPIOB bit 6   (clock, data sampled on rising edge)
        RS   = GPIOB bit 7   (0 = command, 1 = data as driven by the caller)
        CS   = GPIOB bit 9   (low while a byte is transferred)
        MOSI = GPIOA bit 10  (data bit, MSB first)

    The controller speaks the ST7565/ST7567-style command set (0xB0|page,
    0x10|col_hi, 0x00|col_lo, data bytes are 8 vertical pixels per column),
    which yields a 128x64 1bpp frame buffer in page/column format.
    """
    SCL = 6
    RS = 7
    CS = 9
    MOSI_PORT = 'GPIOA'
    MOSI_BIT = 10

    def __init__(self, width=128, height=64):
        self.w = width
        self.h = height
        self.fb = bytearray((width * height) // 8)
        self.pb = 0
        self.pa = 0
        self.page = 0
        self.col = 0
        self.start_line = 0
        self._bits = 0
        self._val = 0
        self._rs = 0
        self._cs = 0
        self.bytes = []            # (insn, rs, value)
        self.commands = []         # (insn, value)
        self.max_bytes = 200000
        self.frames = 0
        self.dirty = True
        self._last_rs = 0

    def reset(self):
        self.fb = bytearray((self.w * self.h) // 8)
        self.pb = self.pa = 0
        self.page = self.col = self.start_line = 0
        self._bits = self._val = 0
        self._cs = 0
        self.bytes = []
        self.commands = []
        self.frames = 0

    # -- pin events ---------------------------------------------------------
    def on_pins(self, port, setbits, clrbits, insn):
        if port == 'GPIOB':
            old = self.pb
            self.pb = (self.pb | setbits) & ~clrbits & 0xFFFF
            # RS is asserted before CS goes low; capture it early
            self._rs = 1 if (self.pb & (1 << self.RS)) else 0
            cs_bit = 1 << self.CS
            # CS falling edge starts a new byte transfer
            if (old & cs_bit) and not (self.pb & cs_bit):
                self._cs = 1
                self._bits = 0
                self._val = 0
            elif not (old & cs_bit) and (self.pb & cs_bit):
                self._cs = 0
            # SCL rising edge -> sample MOSI (only while a transfer is active)
            scl_bit = 1 << self.SCL
            if self._cs and not (old & scl_bit) and (self.pb & scl_bit):
                self._clock(insn)
        elif port == self.MOSI_PORT:
            self.pa = (self.pa | setbits) & ~clrbits & 0xFFFF

    def _clock(self, insn):
        bit = 1 if (self.pa & (1 << self.MOSI_BIT)) else 0
        self._val = (self._val << 1) | bit
        self._bits += 1
        if self._bits >= 8:
            self._emit(insn, self._rs, self._val & 0xFF)
            self._bits = 0
            self._val = 0

    def _emit(self, insn, rs, value):
        if len(self.bytes) < self.max_bytes:
            self.bytes.append((insn, rs, value))
        if rs == 0:
            # command stream
            if len(self.commands) < 5000:
                self.commands.append((insn, value))
            if 0xB0 <= value <= 0xB7:
                self.page = value & 7
            elif 0x10 <= value <= 0x1F:
                self.col = (self.col & 0x0F) | ((value & 0x0F) << 4)
            elif 0x00 <= value <= 0x0F:
                self.col = (self.col & 0xF0) | (value & 0x0F)
            elif 0x40 <= value <= 0x7F:
                self.start_line = value & 0x3F
            elif value in (0xE2, 0xE3):     # reset / nop
                pass
        else:
            if self.page == 0 and self.col == 0:
                self.frames += 1
                self.dirty = True
            idx = (self.page % (self.h // 8)) * self.w + (self.col % self.w)
            if 0 <= idx < len(self.fb):
                if self.fb[idx] != value:
                    self.fb[idx] = value
                    self.dirty = True
            self.col = (self.col + 1) % self.w

    # -- output -------------------------------------------------------------
    def snapshot(self):
        return dict(w=self.w, h=self.h, format='1bpp', start_line=self.start_line,
                    page=self.page, col=self.col, bytes_written=len(self.bytes),
                    frames=self.frames, dirty=self.dirty)

    def frame_bits(self):
        """Unpack page-format 1bpp into a row-major, MSB-first bit list."""
        out = bytearray((self.w * self.h) // 8)
        for page in range(self.h // 8):
            for col in range(self.w):
                b = self.fb[page * self.w + col]
                for bit in range(8):
                    if b & (1 << bit):
                        row = page * 8 + bit
                        idx = row * (self.w // 8) + (col // 8)
                        out[idx] |= 1 << (7 - (col % 8))
        return bytes(out)



class RCC(Peripheral):
    name = 'RCC'
    base = 0x40021000
    reg_names = {
        0x00: 'CR', 0x04: 'CFGR', 0x08: 'CIR', 0x0C: 'APB2RSTR',
        0x10: 'APB1RSTR', 0x14: 'AHBENR', 0x18: 'APB2ENR', 0x1C: 'APB1ENR',
        0x20: 'BDCR', 0x24: 'CSR', 0x28: 'AHBRSTR', 0x2C: 'CFGR2',
        0x30: 'CFGR3', 0x34: 'CR2',
    }

    def reset(self):
        super().reset()
        self.regs.update({
            0x00: 0x00000083,   # HSION|HSIRDY|HSITRIM
            0x04: 0x00000000,
            0x20: 0x00000000,
            0x24: 0x00000000,
            0x2C: 0x00000000,
            0x30: 0x00000000,
        })

    def read(self, off):
        if off == 0x00:
            v = self.regs.get(0x00, 0)
            # HSI(bit1) / HSE(bit17) / PLL(bit25) ready always asserted,
            # PLLON/HSION always readable as set (lead verification: 0x03020003)
            return v | 0x03020003
        if off == 0x24:  # CSR: LSIRDY(bit1), LSION mirror
            return self.regs.get(0x24, 0) | (1 << 1) | (1 << 9)
        if off == 0x20:  # BDCR: LSE ready
            return self.regs.get(0x20, 0) | (1 << 1)
        if off == 0x04:  # CFGR: clock switch status follows the selected source
            v = self.regs.get(0x04, 0)
            sw = v & 0x3
            return (v & ~0xC) | (sw << 2)
        return self.regs.get(off, 0)

    def snapshot_extra(self):
        return {'APB2ENR': self.regs.get(0x18, 0), 'APB1ENR': self.regs.get(0x1C, 0),
                'AHBENR': self.regs.get(0x14, 0)}


class FLASH(Peripheral):
    name = 'FLASH'
    base = 0x40022000
    reg_names = {
        0x00: 'ACR', 0x04: 'KEYR', 0x08: 'OPTKEYR', 0x0C: 'SR',
        0x10: 'CR', 0x14: 'AR', 0x1C: 'OBR', 0x20: 'WRPR',
    }

    def reset(self):
        super().reset()
        self.unlocked = False
        self.key_stage = 0
        self.cr_writes = []

    def read(self, off):
        if off == 0x0C:      # SR: BSY(bit0)=0, EOP(bit5)=1 (write finished)
            return self.regs.get(0x0C, 0) | 0x20
        if off == 0x00:
            return self.regs.get(0x00, 0x00000030)  # ACR: PRFTBE|LATENCY=0
        if off == 0x04:
            return 0          # KEYR is write only
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x04:
            self.key_stage = 1 if val == 0x45670123 else (2 if val == 0xCDEF89AB and self.key_stage == 1 else 0)
            self.unlocked = self.key_stage == 2
            self.regs[0x04] = val
            return
        if off == 0x0C:      # writing 1 clears EOP, BSY stays 0
            self.regs[0x0C] = 0x20
            return
        if off == 0x10:
            self.cr_writes.append(val)
            self.regs[0x10] = val
            return
        self.regs[off] = val


class TIM(Peripheral):
    name = 'TIM'
    base = 0x40000000
    size = 0x400
    reg_names = {
        0x00: 'CR1', 0x04: 'CR2', 0x08: 'SMCR', 0x0C: 'DIER', 0x10: 'SR',
        0x14: 'EGR', 0x18: 'CCMR1', 0x1C: 'CCMR2', 0x20: 'CCER',
        0x24: 'CNT', 0x28: 'PSC', 0x2C: 'ARR', 0x30: 'RCR',
        0x34: 'CCR1', 0x38: 'CCR2', 0x3C: 'CCR3', 0x40: 'CCR4',
        0x44: 'BDTR', 0x48: 'DCR', 0x4C: 'DMAR',
    }

    def __init__(self, emu, name, base):
        self.name = name
        self.base = base
        super().__init__(emu)

    def reset(self):
        super().reset()
        self.regs.update({0x28: 0, 0x2C: 0xFFFF, 0x24: 0})
        self._t0 = 0
        self._phase = 0
        self._wrapped = False

    def _ticks(self):
        # number of timer ticks that should have elapsed by now
        psc = (self.regs.get(0x28, 0) & 0xFFFF) + 1
        return (self.emu.insn_count - self._t0) // psc

    def _maybe_wrap(self):
        ticks = self._ticks()
        arr = (self.regs.get(0x2C, 0xFFFF) & 0xFFFF) + 1
        phase = ticks % arr
        if ticks // arr > self._phase:
            self._wrapped = True
        self._phase = ticks // arr
        return phase

    def read(self, off):
        if off == 0x24:                    # CNT
            if self.regs.get(0x00, 0) & 1:  # CR1.CEN
                return self._maybe_wrap()
            return self.regs.get(0x24, 0) & 0xFFFF
        if off == 0x10:                    # SR
            v = self.regs.get(0x10, 0)
            if self.regs.get(0x00, 0) & 1:
                self._maybe_wrap()
            if self._wrapped:
                v |= 1                 # UIF
            return v
        if off == 0x00:
            return self.regs.get(0x00, 0)
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x24:                    # CNT write resets the counter base
            self.regs[0x24] = val & 0xFFFF
            self._t0 = self.emu.insn_count
            self._phase = 0
            self._wrapped = False
            return
        if off == 0x14:                    # EGR
            if val & 1:                    # UG
                self._t0 = self.emu.insn_count
                self._phase = 0
                self._wrapped = True       # generate update event
                self.regs[0x24] = 0
            self.regs[0x14] = 0            # EGR is write-only, reads as 0
            return
        if off == 0x10:                    # SR write 0 clears flags
            self.regs[0x10] = self.regs.get(0x10, 0) & val
            self._wrapped = False
            return
        self.regs[off] = val & 0xFFFFFFFF

    def snapshot_extra(self):
        return {'CNT': self.read(0x24)}


class SysTick(Peripheral):
    name = 'SYSTICK'
    base = 0xE000E010
    size = 0x10
    reg_names = {0x00: 'CTRL', 0x04: 'LOAD', 0x08: 'VAL', 0x0C: 'CALIB'}

    def reset(self):
        super().reset()
        self._t0 = 0
        self._last_wrap = 0
        self._countflag = False

    def _wraps(self):
        ctrl = self.regs.get(0x00, 0)
        load = self.regs.get(0x04, 0) & 0xFFFFFF
        if not (ctrl & 1) or load == 0:
            return 0
        return (self.emu.insn_count - self._t0) // (load + 1)

    def read(self, off):
        ctrl = self.regs.get(0x00, 0)
        load = self.regs.get(0x04, 0) & 0xFFFFFF
        if off == 0x00:
            w = self._wraps()
            if w > self._last_wrap:
                self._countflag = True
                self._last_wrap = w
            if self._countflag:
                ctrl |= (1 << 16)      # COUNTFLAG
            return ctrl
        if off == 0x08:
            if (ctrl & 1) and load:
                w = self._wraps()
                rem = (self.emu.insn_count - self._t0) % (load + 1)
                return (load - rem) & 0xFFFFFF
            # free-running counter: the firmware's ms-delay polls VAL and
            # expects it to change; lead verification -> (insn//3)&0xFFFFFF
            return (self.emu.insn_count // 3) & 0xFFFFFF
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x00:
            self.regs[0x00] = val & 0x7
            if val & 1:
                self._t0 = self.emu.insn_count
                self._last_wrap = 0
            self._countflag = False
            return
        if off == 0x04:
            self.regs[0x04] = val & 0xFFFFFF
            self._t0 = self.emu.insn_count
            self._last_wrap = 0
            return
        if off == 0x08:
            self.regs[0x08] = val & 0xFFFFFF
            self._countflag = False
            return
        self.regs[off] = val


class SCB_NVIC(Peripheral):
    name = 'SCB'
    base = 0xE000E000
    size = 0xF00
    reg_names = {
        0x100: 'NVIC_ISER', 0x104: 'NVIC_ISER1', 0x108: 'NVIC_ISER2',
        0x180: 'NVIC_ICER', 0x200: 'NVIC_ISPR', 0x280: 'NVIC_ICPR',
        0x300: 'NVIC_IABR', 0x400: 'NVIC_IPR0', 0x404: 'NVIC_IPR1',
        0x408: 'NVIC_IPR2', 0x40C: 'NVIC_IPR3', 0x410: 'NVIC_IPR4',
        0x414: 'NVIC_IPR5', 0x418: 'NVIC_IPR6', 0x41C: 'NVIC_IPR7',
        0xD00: 'CPUID', 0xD04: 'ICSR', 0xD08: 'VTOR', 0xD0C: 'AIRCR',
        0xD10: 'SCR', 0xD14: 'CCR', 0xD24: 'SHCSR', 0xD28: 'CFSR',
        0xD2C: 'HFSR', 0xD34: 'MMFAR', 0xD38: 'BFAR', 0xD3C: 'AFSR',
        0xD88: 'CPACR',
    }

    def reset(self):
        super().reset()
        self.regs[0xD00] = 0x410FC241     # CPUID: Cortex-M0 r0p1
        self.pending = 0
        self.enabled = 0

    _ISER = (0x100, 0x104, 0x108, 0x10C, 0x110, 0x114, 0x118, 0x11C)
    _ICER = (0x180, 0x184, 0x188, 0x18C)
    _ISPR = (0x200, 0x204, 0x208, 0x20C)
    _ICPR = (0x280, 0x284, 0x288, 0x28C)

    def read(self, off):
        if off == 0xD00:
            return self.regs[0xD00]
        if off == 0xD04:                  # ICSR
            return self.regs.get(0xD04, 0)
        if off == 0xD08:
            return self.regs.get(0xD08, 0x08000000)
        if off in self._ISER:
            idx = (off - 0x100) // 4
            return (self.enabled >> (8 * idx)) & 0xFF
        if off in self._ISPR:
            idx = (off - 0x200) // 4
            return (self.pending >> (8 * idx)) & 0xFF
        if 0x400 <= off <= 0x41F:
            return self.regs.get(off, 0)
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off in self._ISER:
            idx = (off - 0x100) // 4
            self.enabled |= (val & 0xFF) << (8 * idx)
            self.regs[off] = val
            return
        if off in self._ICER:
            idx = (off - 0x180) // 4
            self.enabled &= ~((val & 0xFF) << (8 * idx))
            self.regs[off] = val
            return
        if off in self._ISPR:
            idx = (off - 0x200) // 4
            self.pending |= (val & 0xFF) << (8 * idx)
            self.regs[off] = val
            return
        if off in self._ICPR:
            idx = (off - 0x280) // 4
            self.pending &= ~((val & 0xFF) << (8 * idx))
            self.regs[off] = val
            return
        # AIRCR PRIGROUP / SYSRESETREQ etc. are swallowed
        self.regs[off] = val & 0xFFFFFFFF

    def snapshot_extra(self):
        return {'NVIC_ENABLED': self.enabled, 'NVIC_PENDING': self.pending}


class EXTI(Peripheral):
    name = 'EXTI'
    base = 0x40010400
    reg_names = {0x00: 'IMR', 0x04: 'EMR', 0x08: 'RTSR', 0x0C: 'FTSR',
                 0x10: 'SWIER', 0x14: 'PR'}

    def reset(self):
        super().reset()
        self.pending = 0

    def read(self, off):
        if off == 0x14:
            return self.pending
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x14:          # PR: write 1 to clear
            self.pending &= ~val
            self.regs[0x14] = 0
            return
        if off == 0x10:          # SWIER: software interrupt
            self.pending |= val
            self.regs[0x10] = val
            self.emu.note_irq('EXTI', val)
            return
        self.regs[off] = val

    def inject(self, mask):
        self.pending |= mask
        self.emu.note_irq('EXTI', mask)

    def snapshot_extra(self):
        return {'PR': self.pending}


class ADC(Peripheral):
    name = 'ADC'
    base = 0x40012400
    size = 0x400
    reg_names = {
        0x00: 'ISR', 0x04: 'IER', 0x08: 'CR', 0x0C: 'CFGR1', 0x10: 'CFGR2',
        0x14: 'SMPR', 0x40: 'DR', 0x44: 'JDR1',
    }

    def reset(self):
        super().reset()
        self.values = {}         # channel -> mV (12 bit) ; empty = default
        self.default = 0x0C80    # ~3.2 V battery
        self.last_channel = 0

    def _selected_channel(self):
        cfgr1 = self.regs.get(0x0C, 0)
        chsel = cfgr1 & 0x7FFFF
        if chsel:
            return chsel.bit_length() - 1
        return self.last_channel

    def _value(self):
        ch = self._selected_channel()
        return self.values.get(ch, self.values.get(-1, self.default)) & 0xFFF

    def read(self, off):
        if off == 0x00:          # ISR
            v = self.regs.get(0x00, 0)
            # EOC(bit2) | EOS(bit3) always ready after a start
            return v | (1 << 2) | (1 << 3)
        if off == 0x40:          # DR
            return self._value()
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x0C:
            self.regs[0x0C] = val
            chsel = val & 0x7FFFF
            if chsel:
                self.last_channel = chsel.bit_length() - 1
            return
        if off == 0x00:          # clear flags
            self.regs[0x00] = self.regs.get(0x00, 0) & val
            return
        self.regs[off] = val

    def set_channel(self, channel, value):
        self.values[channel] = value

    def snapshot_extra(self):
        return {'DR': self._value(), 'CH': self._selected_channel()}


class USART(Peripheral):
    name = 'USART'
    reg_names = {0x00: 'SR', 0x04: 'DR', 0x08: 'BRR', 0x0C: 'CR1',
                 0x10: 'CR2', 0x14: 'CR3', 0x18: 'GTPR'}

    def __init__(self, emu, name, base):
        self.name = name
        self.base = base
        super().__init__(emu)

    def reset(self):
        super().reset()
        self.rx = []             # injected receive bytes

    def read(self, off):
        if off == 0x00:
            v = self.regs.get(0x00, 0)
            v |= (1 << 7) | (1 << 6)            # TXE | TC
            if self.rx:
                v |= (1 << 5)                   # RXNE
            return v
        if off == 0x04:
            if self.rx:
                return self.rx.pop(0)
            return 0
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x04:
            ch = val & 0xFF
            self.emu.uart_tx(ch)
            return
        self.regs[off] = val & 0xFFFFFFFF


class SPI(Peripheral):
    name = 'SPI'
    reg_names = {0x00: 'CR1', 0x04: 'CR2', 0x08: 'SR', 0x0C: 'DR',
                 0x10: 'CRCPR', 0x14: 'RXCRCR', 0x18: 'TXCRCR',
                 0x1C: 'I2SCFGR', 0x20: 'I2SPR'}

    def __init__(self, emu, name, base):
        self.name = name
        self.base = base
        super().__init__(emu)

    def reset(self):
        super().reset()
        self.miso = []           # queued MISO bytes, default 0xFF
        self.default_miso = 0xFF

    def read(self, off):
        if off == 0x08:          # SR: TXE(bit1)|RXNE(bit0)
            return self.regs.get(0x08, 0) | 0x3
        if off == 0x0C:          # DR: shift in next MISO byte
            if self.miso:
                return self.miso.pop(0)
            return self.default_miso
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x0C:
            self.emu.spi_tx(val & 0xFF)
            return
        self.regs[off] = val


class I2C(Peripheral):
    name = 'I2C'
    reg_names = {0x00: 'CR1', 0x04: 'CR2', 0x08: 'OAR1', 0x0C: 'OAR2',
                 0x10: 'DR', 0x14: 'SR1', 0x18: 'SR2', 0x1C: 'CCR',
                 0x20: 'TRISE', 0x24: 'FLTR'}

    def __init__(self, emu, name, base):
        self.name = name
        self.base = base
        super().__init__(emu)

    def reset(self):
        super().reset()
        self.rx = []
        self.rx_log = []

    def read(self, off):
        if off == 0x14:          # SR1
            v = self.regs.get(0x14, 0)
            v |= 0x84            # TXE(bit7) | BTF(bit2)
            if self.rx:
                v |= 0x40        # RXNE(bit6)
            cr2 = self.regs.get(0x04, 0)
            if cr2 & 0x0100:     # START just requested
                v |= 0x01        # SB
            if cr2 & 0x0200:     # STOP requested
                v |= 0x10        # STOPF
            return v & 0xFF
        if off == 0x18:          # SR2
            return self.regs.get(0x18, 0) | 0x01   # BUSY (bus always busy-ish)
        if off == 0x10:          # DR
            if self.rx:
                return self.rx.pop(0)
            return 0
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x10:
            self.rx_log.append(val & 0xFF)
            self.emu.note_irq('I2C', val & 0xFF)
            return
        self.regs[off] = val & 0xFFFFFFFF


class IWDG(Peripheral):
    name = 'IWDG'
    base = 0x40003000
    size = 0x400
    reg_names = {0x00: 'KR', 0x04: 'PR', 0x08: 'RLR', 0x0C: 'SR'}

    def reset(self):
        super().reset()
        self.kicks = 0
        self.started = False

    def read(self, off):
        if off == 0x0C:
            return self.regs.get(0x0C, 0)     # PVU/RVU cleared
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x00:
            if val == 0xAAAA:
                self.kicks += 1
            elif val == 0xCCCC:
                self.started = True
            self.regs[0x00] = val
            return
        self.regs[off] = val & 0xFFFFFFFF

    def snapshot_extra(self):
        return {'KICKS': self.kicks, 'STARTED': int(self.started)}


class DMA(Peripheral):
    name = 'DMA1'
    base = 0x40020000
    reg_names = {0x00: 'ISR', 0x04: 'IFCR'}
    for _i in range(7):
        _b = 0x08 + 0x14 * _i
        reg_names[_b] = 'CCR%d' % (_i + 1)
        reg_names[_b + 4] = 'CNDTR%d' % (_i + 1)
        reg_names[_b + 8] = 'CPAR%d' % (_i + 1)
        reg_names[_b + 12] = 'CMAR%d' % (_i + 1)
    del _i, _b


class CRC(Peripheral):
    name = 'CRC'
    base = 0x40023000
    reg_names = {0x00: 'DR', 0x04: 'IDR', 0x08: 'CR', 0x10: 'INIT', 0x14: 'POL'}


class SYSCFG(Peripheral):
    name = 'SYSCFG'
    base = 0x40010000
    reg_names = {0x00: 'CFGR1', 0x04: 'CFGR2', 0x08: 'EXTICR1',
                 0x0C: 'EXTICR2', 0x10: 'EXTICR3', 0x14: 'EXTICR4'}

    def write(self, off, val):
        self.regs[off] = val & 0xFFFFFFFF
        if off == 0x00:
            self.emu.note_event('SYSCFG.CFGR1', val)


class PWR(Peripheral):
    name = 'PWR'
    base = 0x40007000
    size = 0x400
    reg_names = {0x00: 'CR', 0x04: 'CSR'}

    def read(self, off):
        if off == 0x04:      # CSV: PVDO(1) | VOSF(2) | VREFINTRDYF(8) ready
            return self.regs.get(0x04, 0) | 0x0106
        return self.regs.get(off, 0)


class RTC(Peripheral):
    name = 'RTC'
    base = 0x40002800
    size = 0x400
    reg_names = {
        0x00: 'TR', 0x04: 'DR', 0x08: 'CR', 0x0C: 'ISR', 0x10: 'PRER',
        0x14: 'WUTR', 0x18: 'CALIBR', 0x1C: 'ALRMAR', 0x20: 'ALRMBR',
        0x24: 'WPR', 0x28: 'SSR', 0x2C: 'SHIFTR', 0x30: 'TSTR',
        0x34: 'TSDR', 0x38: 'TSSSR', 0x3C: 'CALR', 0x40: 'TAFCR',
        0x44: 'ALRMASSR', 0x48: 'ALRMBSSR',
    }

    def reset(self):
        super().reset()
        self.regs[0x24] = 0x0

    def read(self, off):
        if off == 0x0C:      # ISR: lead verification -> 0x7F (all ready bits)
            v = self.regs.get(0x0C, 0)
            if v & 0x80:
                v |= (1 << 6)
            return v | 0x7F
        if off == 0x24:      # WPR always reports write-protection off
            return 0
        return self.regs.get(off, 0)

    def write(self, off, val):
        if off == 0x0C:      # INIT bit is set by writing 1, flags clear on 0
            old = self.regs.get(0x0C, 0)
            new = (old & val) | (val & 0x80)
            self.regs[0x0C] = new
            return
        self.regs[off] = val & 0xFFFFFFFF

    def snapshot_extra(self):
        return {'ISR': self.read(0x0C)}


class GenericMMIO(Peripheral):
    """Fallback for unknown peripheral windows: store + trace as unknown."""
    name = 'UNKNOWN'
    reg_names = {}

    def __init__(self, emu, name, base, size):
        self.name = name
        self.base = base
        self.size = size
        super().__init__(emu)


# ---------------------------------------------------------------------------
# function symbol table
# ---------------------------------------------------------------------------
class FunctionTable:
    def __init__(self, path=None):
        self.starts = []
        self.entries = []
        self.by_start = {}
        if path and os.path.isfile(path):
            try:
                data = json.load(open(path, encoding='utf-8'))
            except Exception:
                data = []
            for e in data:
                try:
                    a = int(e['addr'], 16) if isinstance(e['addr'], str) else int(e['addr'])
                    b = int(e['end'], 16) if isinstance(e.get('end', ''), str) else int(e.get('end', a + 2))
                except Exception:
                    continue
                if b <= a:
                    b = a + 2
                self.entries.append((a, b, 'func_%08X' % a))
            self.entries.sort()
            self.starts = [e[0] for e in self.entries]
            self.by_start = {e[0]: e for e in self.entries}

    def __len__(self):
        return len(self.entries)

    def lookup(self, pc):
        if not self.starts:
            return None
        i = bisect.bisect_right(self.starts, pc) - 1
        if i < 0:
            return None
        a, b, name = self.entries[i]
        if a <= pc < b:
            return (a, name)
        return None

    def name_of(self, addr):
        e = self.by_start.get(addr)
        return e[2] if e else 'sub_%08X' % addr

    def is_start(self, addr):
        return addr in self.by_start


class _CC2500Stub:
    """Fallback used until emu/cc2500.py exists.  The real CC2500 model is
    owned by Lead; core.py only needs its GPIO hooks (MISO on GPIOB.14, GDO0
    on PA15 -> EXTI15 -> IRQ7)."""
    pending_gdo0 = False

    def __init__(self, emu):
        self.emu = emu

    def on_pins(self, port, setbits, clrbits, insn):
        pass

    def adjust_idr(self, port, idr):
        return idr

    def state(self):
        return dict(available=False, pending_gdo0=False)


# ---------------------------------------------------------------------------
# emulator
# ---------------------------------------------------------------------------
class Emulator:
    """STM32F0 / Cortex-M0 emulator with peripheral stubs."""

    BATCH_DEFAULT = 200000

    def __init__(self, firmware_path=None, functions_json=None, *,
                 trace=True, trace_filter=None, max_trace=500_000,
                 on_fault='stop', ram_size=RAM_SIZE, batch=None,
                 calib='blank', verbose=False, virtual_tick=True,
                 tick_period=48000, real_irq=True, isr_max_insns=300_000,
                 virtual_tim14=True, tim14_period=9600):
        # default to the project's firmware + analysis outputs
        if firmware_path is None and os.path.isfile(DEFAULT_FIRMWARE):
            firmware_path = DEFAULT_FIRMWARE
        if functions_json is None and os.path.isfile(DEFAULT_FUNCTIONS):
            functions_json = DEFAULT_FUNCTIONS
        self.firmware_path = firmware_path
        self.functions = FunctionTable(functions_json)
        self.trace_enabled = trace
        self.trace_filter = list(trace_filter or [])
        self.max_trace = max_trace
        self.on_fault = on_fault
        self.ram_size = ram_size
        self.batch = batch or self.BATCH_DEFAULT
        self.calib = calib
        self.calib_records = 0
        self.verbose = verbose
        # virtual SysTick: unicorn MCLASS does not run NVIC handlers, so we
        # emulate the 1 ms SysTick ISR *effects* (tick counter + main-loop
        # event flag) at fixed instruction intervals.
        self.virtual_tick = virtual_tick
        self.tick_period = tick_period
        self._next_tick = tick_period
        self.in_main_loop = False
        # real IRQ dispatch: Unicorn MCLASS has no NVIC exception model, so we
        # enter/leave handlers manually (trampoline scheme, verified by Lead in
        # tools/boottest.py).  The 1 ms SysTick and EXTI key edges drive this.
        self.real_irq = real_irq
        self.isr_max_insns = int(isr_max_insns)
        # The firmware starts TIM14 with its update interrupt (5 kHz) and uses
        # it as the UI/key scan time base; Unicorn cannot raise it, so we
        # dispatch IRQ19 (exception 35) periodically like the real timer.
        self.virtual_tim14 = virtual_tim14
        self.tim14_period = int(tim14_period)
        self._next_tim14 = int(tim14_period)
        self._in_isr = False
        self._isr_ctx = None
        self._isr_irq = None
        self._isr_stop_insn = None
        self._tramp_hit = False
        self._irq_queue = []
        self._irq_queue_lock = threading.Lock()
        self.irq_counts = {}
        self._irq_dispatch_log = []
        self.cc2500 = None
        self.cc2500_available = False
        self.rf_initialized = False
        self.last_function_call = None

        self.rom = b''
        if firmware_path:
            with open(firmware_path, 'rb') as f:
                self.rom = f.read()

        self.trace = []
        self.trace_truncated = False
        self.faults = []
        self.pin_log = [] if trace else None
        self.max_pin_log = 400000
        self.pin_toggles = {}
        self.lcd = LCDDecoder()
        self.inputs = VirtualInputs(self)
        self.fb_addr = 0x200004CC        # firmware LCD frame buffer (docs/04 搂2.3)
        self.fb_width, self.fb_height = 128, 64
        self.uart_out = []
        self.spi_out = []
        self.irq_log = []
        self.events = []
        self.call_log = []
        self.func_counts = {}
        self.call_log_enabled = True
        self.max_call_log = 6000
        self.breakpoints = set()
        self._bp_hooks = {}
        self.insn_count = 0
        self.run_time = 0.0
        self._last_batch_count = 0
        self._stop_reason = None
        self._prev_block = None
        self._block_len_cache = {}
        self._batch_target = None
        self._md = None
        self._filter_cache = None
        self._uc = None
        self._stop_requested = False

        self._setup()

    # -- construction --------------------------------------------------------
    def _setup(self):
        self._uc = self._new_uc()
        self._load_image()
        self._create_peripherals()
        self._setup_hooks()
        self._init_cc2500()
        self._reset_cpu()
        self._faults_since_reset = 0

    def _init_cc2500(self):
        """Attach Lead's virtual CC2500 model if emu/cc2500.py is present."""
        try:
            import cc2500 as _cc          # noqa: F401  (owned by Lead)
            self.cc2500 = _cc.CC2500(self)
            self.cc2500_available = True
        except Exception:
            self.cc2500 = _CC2500Stub(self)
            self.cc2500_available = False

    def _new_uc(self):
        try:
            return Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
        except UcError:
            return Uc(UC_ARCH_ARM, UC_MODE_THUMB)

    def _load_image(self):
        uc = self._uc
        uc.mem_map(ROM_BASE, ROM_SIZE)
        # unprogrammed flash reads as 0xFF
        uc.mem_write(ROM_BASE, b'\xff' * ROM_SIZE)
        if self.rom:
            uc.mem_write(APP_BASE, self.rom)
            # emulate the bootloader: application vectors are also visible at
            # the beginning of flash (and at the CPU vector-table alias 0x0).
            vec = self.rom[:0xC0]
            uc.mem_write(ROM_BASE, vec)
            if self.calib == 'synthetic':
                self._write_synthetic_calib(uc, vec)
        # manual-ISR return trampoline: 'b .' at 0x0800F900 (inside the 0xFF
        # area, after the calibration block), so handler returns are caught.
        uc.mem_write(TRAMP_ADDR, b'\xfe\xe7\x00\x00')
        uc.mem_map(RAM_BASE, (self.ram_size + 0xFFF) & ~0xFFF)
        uc.mem_map(VECTOR_BASE_ALIAS, VECTOR_ALIAS_SIZE)
        if self.rom:
            uc.mem_write(VECTOR_BASE_ALIAS, vec)
        for base, size in MMIO_WINDOWS:
            uc.mem_map(base, (size + 0xFFF) & ~0xFFF)

    def _write_synthetic_calib(self, uc, vec):
        """Fill the 0x0801F000 factory-calibration area with plausible records.

        The application walks 0x0801F000 + (i-1)*100, reading 32 bytes per
        record and treating a first byte of 0x5C as a magic marker (up to 20
        records).  Real units ship with this area programmed; blank (0xFF)
        makes the firmware take its "no calibration" fallback path.
        """
        data = bytearray(b'\xff' * (ROM_BASE + ROM_SIZE - CALIB_BASE))
        for i in range(20):
            off = i * 100
            rec = bytearray(b'\xff' * 32)
            rec[0] = 0x5C
            rec[1] = 0x01
            rec[2] = (i + 1) & 0xFF
            # plausible-looking 16-bit calibration constants (battery, temp)
            struct.pack_into('<HHHH', rec, 4, 0x0F00 + i * 7, 0x0B00 + i * 5,
                             0x0800 + i * 3, 0x1A00 + i * 2)
            data[off:off + 32] = rec
        uc.mem_write(CALIB_BASE, bytes(data))
        self.calib_records = 20

    def _create_peripherals(self):
        self.devices = {}
        self.order = []          # (base, size, device) sorted by base
        for nm in ('GPIOA', 'GPIOB', 'GPIOC', 'GPIOD', 'GPIOE', 'GPIOF'):
            d = GPIO(self, nm)
            self.devices[nm] = d
            self.order.append((d.base, d.size, d))
        self.rcc = RCC(self); self.flash = FLASH(self)
        tims = (('TIM1', 0x40012C00), ('TIM2', 0x40000000), ('TIM3', 0x40000400),
                ('TIM6', 0x40001000), ('TIM7', 0x40001400), ('TIM14', 0x40002000),
                ('TIM15', 0x40014000), ('TIM16', 0x40014400), ('TIM17', 0x40014800))
        self.tims = {}
        devices = [self.rcc, self.flash]
        for nm, base in tims:
            t = TIM(self, nm, base)
            self.tims[nm] = t
            devices.append(t)
        self.systick = SysTick(self)
        self.scb = SCB_NVIC(self)
        self.exti = EXTI(self)
        self.adc = ADC(self)
        self.usart1 = USART(self, 'USART1', 0x40013800)
        self.usart2 = USART(self, 'USART2', 0x40004400)
        self.usarts = {'USART1': self.usart1, 'USART2': self.usart2}
        self.spi1 = SPI(self, 'SPI1', 0x40013000)
        self.spis = {'SPI1': self.spi1}
        self.i2c1 = I2C(self, 'I2C1', 0x40005400)
        self.iwdg = IWDG(self)
        self.dma = DMA(self)
        self.crc = CRC(self)
        self.syscfg = SYSCFG(self)
        self.pwr = PWR(self)
        self.rtc = RTC(self)
        self.cc2500 = _cc2500.CC2500(self) if _cc2500 else None
        devices += [self.exti, self.adc, self.usart1, self.usart2, self.spi1,
                    self.i2c1, self.iwdg, self.dma, self.crc, self.syscfg,
                    self.pwr, self.rtc, self.systick, self.scb]
        for d in devices:
            self.devices[d.name] = d
            self.order.append((d.base, d.size, d))
        # sort by base address for bisect lookup
        self.order.sort()
        self._order_starts = [o[0] for o in self.order]

    # -- hooks ---------------------------------------------------------------
    def _setup_hooks(self):
        uc = self._uc
        for base, size in MMIO_WINDOWS:
            end = base + size - 1
            uc.hook_add(UC_HOOK_MEM_READ, self._mem_read, None, base, end)
            uc.hook_add(UC_HOOK_MEM_WRITE, self._mem_write, None, base, end)
        uc.hook_add(UC_HOOK_MEM_UNMAPPED, self._mem_unmapped)
        uc.hook_add(UC_HOOK_INTR, self._hook_intr)
        uc.hook_add(UC_HOOK_BLOCK, self._hook_block)
        uc.hook_add(UC_HOOK_CODE, self._hook_tramp, None, TRAMP_ADDR, TRAMP_ADDR + 3)

    def _hook_intr(self, uc, intno, user_data):
        self.note_event('INTERRUPT', intno)

    # -- IRQ dispatch (manual Cortex-M exception entry / return) -----------
    def request_irq(self, irq, source=''):
        """Queue an IRQ for the run loop (thread safe; used by VirtualInputs
        and the CC2500 model)."""
        with self._irq_queue_lock:
            if irq not in self._irq_queue:
                self._irq_queue.append(irq)
        self.note_irq(source or ('IRQ%d' % irq), irq)

    def _ram_vector(self, exc):
        """Handler address from the firmware's RAM vector table (0x20000000).

        ``exc`` is the *exception number* (= vector index): 15 = SysTick,
        16+IRQn for peripheral interrupts (e.g. 21 = EXTI0_1/IRQ5).
        """
        try:
            data = bytes(self._uc.mem_read(RAM_VECTOR_BASE + 4 * exc, 4))
        except UcError:
            return None
        handler = _u32(data)
        if handler in (0, 0xFFFFFFFF):
            return None
        h = handler & ~1
        if h in (DEFAULT_HANDLER, DEFAULT_HANDLER | 1):
            return None               # Default_Handler ('b .') would hang
        if not (ROM_BASE <= h < ROM_BASE + ROM_SIZE):
            return None
        return h

    def _hook_tramp(self, uc, address, size, user_data):
        self._tramp_hit = True
        self._stop_reason = 'isr_return'
        uc.emu_stop()
        return False

    def _try_enter_irq(self, exc, source=''):
        if self._in_isr or not self.real_irq:
            return False
        handler = self._ram_vector(exc)
        if handler is None:
            return False
        uc = self._uc
        ctx = {
            'r': [uc.reg_read(getattr(arm_const, 'UC_ARM_REG_R%d' % i)) for i in range(13)],
            'lr': uc.reg_read(UC_ARM_REG_LR),
            'pc': uc.reg_read(UC_ARM_REG_PC),
            'xpsr': uc.reg_read(UC_ARM_REG_XPSR),
        }
        self._isr_ctx = ctx
        self._in_isr = True
        self._isr_irq = exc
        self._isr_stop_insn = self.insn_count + self.isr_max_insns
        self._tramp_hit = False
        uc.reg_write(UC_ARM_REG_LR, TRAMP_ADDR | 1)
        uc.reg_write(UC_ARM_REG_PC, handler)
        self.irq_counts[exc] = self.irq_counts.get(exc, 0) + 1
        if exc == 15:
            self.virtual_ticks = getattr(self, 'virtual_ticks', 0) + 1
        elif exc == 35:
            self._tim14_count = getattr(self, '_tim14_count', 0) + 1
        if len(self._irq_dispatch_log) < 2000:
            self._irq_dispatch_log.append(dict(insn=self.insn_count, irq=exc,
                                               handler=handler, source=source))
        self.note_event('IRQ_ENTER', exc)
        return True

    def _leave_isr(self, reason='return'):
        if not self._in_isr:
            return
        ctx = self._isr_ctx
        uc = self._uc
        if ctx is not None:
            for i in range(13):
                uc.reg_write(getattr(arm_const, 'UC_ARM_REG_R%d' % i), ctx['r'][i])
            uc.reg_write(UC_ARM_REG_LR, ctx['lr'])
            uc.reg_write(UC_ARM_REG_PC, ctx['pc'])
            try:
                uc.reg_write(UC_ARM_REG_XPSR, ctx['xpsr'])
            except UcError:
                pass
        self._isr_ctx = None
        self._in_isr = False
        self._isr_irq = None
        self._isr_stop_insn = None
        self._tramp_hit = False
        self._prev_block = None
        self.note_event('IRQ_RETURN', reason)

    def _run_isr(self):
        """Run the current handler until it returns through the trampoline."""
        uc = self._uc
        self._prev_block = None
        self._batch_target = None
        pc = uc.reg_read(UC_ARM_REG_PC)
        try:
            uc.emu_start(pc | 1, 0, 0, 0)
        except UcError as e:
            self._record_fault('ISR_UC:' + str(e).strip(), pc,
                               uc.reg_read(UC_ARM_REG_LR),
                               uc.reg_read(UC_ARM_REG_SP), None)
            self._leave_isr('fault')
            return 'fault'
        if self._tramp_hit:
            self._leave_isr('return')
            return 'return'
        if self._stop_reason == 'isr_timeout':
            self._leave_isr('timeout')
            return 'timeout'
        if self._stop_reason == 'breakpoint':
            return 'breakpoint'
        self._leave_isr('stopped')
        return 'stopped'

    def _poll_cc2500(self):
        cc = self.cc2500
        if cc is None:
            return
        try:
            if getattr(cc, 'pending_gdo0', False):
                cc.pending_gdo0 = False
                # GDO0 = PA15 -> EXTI line 15 -> EXTI4_15 ISR (IRQ7)
                self.exti.pending |= (1 << 15)
                self.scb.pending |= (1 << 7)
                self.request_irq(16 + 7, source='GDO0')
        except Exception:
            pass

    @staticmethod
    def _irq_for_exti_line(line):
        """EXTI line -> NVIC IRQn (STM32F0): 0/1->5, 2/3->6, 4..15->7."""
        if line <= 1:
            return 5
        if line <= 3:
            return 6
        return 7

    def _poll_exti(self):
        """Queue handlers for EXTI pending bits (keys, CC2500 GDO0, SWIER)."""
        try:
            pending = self.exti.pending & 0xFFFF
        except Exception:
            return
        new = pending & ~getattr(self, '_exti_req_mask', 0)
        self._exti_req_mask = pending
        for line in range(16):
            if new & (1 << line):
                irq = self._irq_for_exti_line(line)
                self.scb.pending |= (1 << irq)
                self.request_irq(16 + irq, source='EXTI%d' % line)

    def _fake_tick(self):
        """Fallback when no real SysTick handler is available: emulate the
        ISR effects by poking the HAL tick and the main-loop event flag."""
        try:
            uc = self._uc
            tick = int.from_bytes(bytes(uc.mem_read(0x20000104, 4)), 'little') + 1
            uc.mem_write(0x20000104, struct.pack('<I', tick & 0xFFFFFFFF))
            uc.mem_write(0x200001E1, b'\x01')
            self.virtual_ticks = getattr(self, 'virtual_ticks', 0) + 1
            self._fake_tick_count = getattr(self, '_fake_tick_count', 0) + 1
        except UcError:
            pass

    def _dispatch_due_irq(self):
        if self._in_isr or not self.real_irq:
            return False
        self._poll_cc2500()
        self._poll_exti()
        if self.virtual_tick and self.insn_count >= self._next_tick:
            self._next_tick = self.insn_count + self.tick_period
            if self._try_enter_irq(15, source='SysTick'):
                return True
            self._fake_tick()
            return False
        if self.virtual_tim14 and self.insn_count >= self._next_tim14:
            self._next_tim14 = self.insn_count + self.tim14_period
            if self._try_enter_irq(35, source='TIM14'):
                return True
        with self._irq_queue_lock:
            pending = list(self._irq_queue)
            self._irq_queue = []
        retry = getattr(self, '_irq_retry', None)
        if retry is None:
            retry = self._irq_retry = {}
        for exc in pending:
            if self._try_enter_irq(exc, source='pending'):
                retry.pop(exc, None)
                return True
            # handler slot not ready yet (e.g. main() has not copied the vector
            # table to RAM) -> keep it queued and retry later
            n = retry.get(exc, 0) + 1
            if n <= 100_000:
                retry[exc] = n
                with self._irq_queue_lock:
                    self._irq_queue.append(exc)
            else:
                retry.pop(exc, None)
        return False

    def _hook_block(self, uc, address, size, user_data):
        if self._prev_block is not None:
            self.insn_count += self._block_len(self._prev_block)
        self._prev_block = address
        if self.inputs is not None and self.inputs.release_at:
            self.inputs.release_due(self.insn_count)
        if self.virtual_tick and not self.real_irq and self.insn_count >= self._next_tick:
            self._fake_tick()
            self._next_tick = self.insn_count + self.tick_period
        if self._in_isr and self._isr_stop_insn is not None and \
                self.insn_count >= self._isr_stop_insn:
            self._stop_reason = 'isr_timeout'
            uc.emu_stop()
            return False
        if self.call_log_enabled and self.functions.starts:
            tgt = address
            if self.functions.is_start(tgt):
                self.func_counts[tgt] = self.func_counts.get(tgt, 0) + 1
                if len(self.call_log) < self.max_call_log:
                    self.call_log.append((self.insn_count, tgt,
                                          self.functions.name_of(tgt),
                                          self._func_depth()))
        if self._batch_target is not None and self.insn_count >= self._batch_target:
            self._stop_reason = 'batch'
            uc.emu_stop()
            return False
        return True

    def _func_depth(self):
        return 0

    def _block_len(self, addr):
        n = self._block_len_cache.get(addr)
        if n is not None:
            return n
        n = self._compute_block_len(addr)
        self._block_len_cache[addr] = n
        return n

    def _compute_block_len(self, addr):
        if self._md is None:
            import capstone
            self._md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
            self._md.detail = False
        try:
            data = self._uc.mem_read(addr, 64)
        except UcError:
            return 2
        data = bytes(data)
        n = 0
        for ins in self._md.disasm(data, addr):
            n += 1
            mn = ins.mnemonic
            if mn in ('b', 'bl', 'blx', 'bx', 'cbz', 'cbnz', 'pop', 'ldm',
                      'ldmia', 'tbb', 'tbh', 'svc', 'udf', 'bkpt') or ins.op_str.find('pc') >= 0 and mn in ('mov',):
                return max(n, 1)
        return max(n, 1)

    # -- MMIO ----------------------------------------------------------------
    def _find_device(self, addr):
        i = bisect.bisect_right(self._order_starts, addr) - 1
        if i >= 0:
            base, size, dev = self.order[i]
            if base <= addr < base + size:
                return dev, addr - base
        return None, 0

    def _mmio_read(self, addr, size):
        dev, off = self._find_device(addr)
        if dev is None:
            return 0, 'MMIO', 'UNKNOWN'
        aligned = off & ~3
        word = dev.read(aligned)
        shift = 8 * (off - aligned)
        val = (word >> shift) & ((1 << (8 * size)) - 1)
        return val, dev.name, dev.reg_name(aligned)

    def _mmio_write(self, addr, size, value):
        dev, off = self._find_device(addr)
        if dev is None:
            return 'MMIO', 'UNKNOWN'
        aligned = off & ~3
        if size >= 4:
            word = value & 0xFFFFFFFF
        else:
            old = dev.read(aligned)
            shift = 8 * (off - aligned)
            mask = ((1 << (8 * size)) - 1) << shift
            word = (old & ~mask) | ((value & ((1 << (8 * size)) - 1)) << shift)
        dev.write(aligned, word)
        return dev.name, dev.reg_name(aligned)

    def _mem_read(self, uc, access, address, size, value, user_data):
        val, dev, reg = self._mmio_read(address, size)
        try:
            uc.mem_write(address, (val & ((1 << (8 * size)) - 1)).to_bytes(size, 'little'))
        except UcError:
            pass
        if self.trace_enabled:
            self._record_trace(address, dev, reg, 'R', val, size)
        return True

    def _mem_write(self, uc, access, address, size, value, user_data):
        dev, reg = self._mmio_write(address, size, value)
        if self.trace_enabled:
            self._record_trace(address, dev, reg, 'W', value, size)
        return True

    def _mem_unmapped(self, uc, access, address, size, value, user_data):
        pc = uc.reg_read(UC_ARM_REG_PC)
        lr = uc.reg_read(UC_ARM_REG_LR)
        sp = uc.reg_read(UC_ARM_REG_SP)
        kind = _ACCESS_NAMES.get(access, str(access))
        if access == UC_MEM_FETCH_UNMAPPED:
            self._record_fault('FETCH_UNMAPPED', pc, lr, sp, address)
            return False
        self._record_fault('%s_UNMAPPED' % ('READ' if access == UC_MEM_READ_UNMAPPED else 'WRITE'),
                           pc, lr, sp, address)
        if self.on_fault == 'continue':
            try:
                uc.mem_map(address & ~0xFFF, 0x1000)
                if access == UC_MEM_READ_UNMAPPED:
                    uc.mem_write(address, (0).to_bytes(size, 'little'))
                return True
            except UcError:
                return False
        return False

    def _record_fault(self, kind, pc, lr, sp, addr):
        f = FaultRecord(kind, pc, lr, sp, addr, self.insn_count)
        self.faults.append(f)
        self._faults_since_reset += 1

    # -- tracing -------------------------------------------------------------
    def _filter_match(self, dev, addr, reg):
        if not self.trace_filter:
            return True
        for f in self.trace_filter:
            if f.startswith('0x') or f.startswith('0X'):
                try:
                    if '-' in f:
                        a, b = f.split('-', 1)
                        lo, hi = int(a, 16), int(b, 16)
                    else:
                        lo = int(f, 16)
                        hi = lo
                    if lo <= addr <= hi:
                        return True
                except ValueError:
                    pass
            elif dev.upper().startswith(f.upper()) or reg.upper().startswith(f.upper()):
                return True
        return False

    def _record_trace(self, addr, dev, reg, rw, value, size):
        if not self._filter_match(dev, addr, reg):
            return
        if len(self.trace) >= self.max_trace:
            self.trace_truncated = True
            return
        pc = self._uc.reg_read(UC_ARM_REG_PC)
        func = self.functions.lookup(pc) if self.functions.starts else None
        self.trace.append(TraceEntry(self.insn_count, pc, addr, dev, reg, rw,
                                     value, size, func))

    def note_event(self, what, value):
        if len(self.events) < 10000:
            self.events.append((self.insn_count, what, value))

    def on_gpio_pins(self, port, setbits, clrbits):
        if setbits or clrbits:
            key = port
            self.pin_toggles[key] = self.pin_toggles.get(key, 0) + 1
        if self.pin_log is not None and len(self.pin_log) < self.max_pin_log:
            self.pin_log.append((self.insn_count, port, setbits, clrbits))
        try:
            self.lcd.on_pins(port, setbits, clrbits, self.insn_count)
        except Exception:
            pass
        try:
            if self.cc2500 is not None:
                self.cc2500.on_pins(port, setbits, clrbits, self.insn_count)
                if getattr(self.cc2500, 'pending_gdo0', False):
                    self._poll_cc2500()
        except Exception:
            pass

    def note_irq(self, what, value):
        if len(self.irq_log) < 10000:
            self.irq_log.append((self.insn_count, what, value))

    def uart_tx(self, ch):
        self.uart_out.append(ch)
        if len(self.uart_out) > 100000:
            del self.uart_out[:50000]

    def spi_tx(self, ch):
        self.spi_out.append(ch)

    # -- CPU state -----------------------------------------------------------
    def _reset_cpu(self):
        uc = self._uc
        sp = _u32(self.rom[0:4]) if len(self.rom) >= 4 else 0x20001000
        reset = (_u32(self.rom[4:8]) if len(self.rom) >= 8 else APP_BASE) | 0
        uc.reg_write(UC_ARM_REG_SP, sp)
        uc.reg_write(UC_ARM_REG_LR, 0xFFFFFFFF)
        uc.reg_write(UC_ARM_REG_PC, reset & ~1)
        for r in (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3,
                  UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6, UC_ARM_REG_R7,
                  UC_ARM_REG_R8, UC_ARM_REG_R9, UC_ARM_REG_R10, UC_ARM_REG_R11,
                  UC_ARM_REG_R12):
            uc.reg_write(r, 0)
        try:
            uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)   # Thumb, no flags
        except UcError:
            pass
        self.reset_pc = reset & ~1
        self.reset_sp = sp
        self._prev_block = None
        self.insn_count = 0
        self._last_batch_count = 0
        self._stop_reason = None

    def reset(self):
        for d in set(self.devices.values()):
            d.reset()
        self.trace = []
        self.trace_truncated = False
        self.faults = []
        self.pin_log = [] if self.trace_enabled else None
        self.pin_toggles = {}
        self.lcd.reset()
        self.inputs.reset()
        self.uart_out = []
        self.spi_out = []
        self.irq_log = []
        self.events = []
        self.call_log = []
        self.func_counts = {}
        self._faults_since_reset = 0
        self._block_len_cache = {}
        self.virtual_ticks = 0
        self._next_tick = self.tick_period
        self._next_tim14 = self.tim14_period
        self._tim14_count = 0
        self.in_main_loop = False
        self._in_isr = False
        self._isr_ctx = None
        self._isr_irq = None
        self._isr_stop_insn = None
        self._tramp_hit = False
        self._irq_queue = []
        self._irq_retry = {}
        self.rf_initialized = False
        self.last_function_call = None
        self.irq_counts = {}
        self._irq_dispatch_log = []
        self._fake_tick_count = 0
        self._exti_req_mask = 0
        try:
            if self.cc2500 is not None and hasattr(self.cc2500, 'reset'):
                self.cc2500.reset()
        except Exception:
            pass
        self._reset_cpu()

    def reset_to_app(self):
        self._reset_cpu()

    # -- registers/memory ----------------------------------------------------
    def reg(self, name):
        return self._uc.reg_read(getattr(arm_const, 'UC_ARM_REG_' + name.upper()))

    def set_reg(self, name, value):
        self._uc.reg_write(getattr(arm_const, 'UC_ARM_REG_' + name.upper()), value)

    def regs_all(self):
        return {n: self._uc.reg_read(getattr(arm_const, 'UC_ARM_REG_' + n))
                for n in ('R0', 'R1', 'R2', 'R3', 'R4', 'R5', 'R6', 'R7', 'R8',
                          'R9', 'R10', 'R11', 'R12', 'SP', 'LR', 'PC')}

    def read_mem(self, addr, size):
        return bytes(self._uc.mem_read(addr, size))

    def write_mem(self, addr, data):
        self._uc.mem_write(addr, data)

    def read_u32(self, addr):
        return _u32(self.read_mem(addr, 4))

    # -- manual function calls / RF bring-up --------------------------------
    def call_function(self, addr, args=(), max_insns=3_000_000):
        """Synchronously call a firmware function and return its result.

        Saves r0-r12/lr/pc/xpsr, sets LR=trampoline, runs until the callee
        returns through TRAMP_ADDR (or the instruction budget expires), then
        restores the caller context.  This is how `rf_init()` drives the RF
        driver bring-up without unwinding the whole boot path.
        """
        uc = self._uc
        regs = [arm_const.UC_ARM_REG_R0, arm_const.UC_ARM_REG_R1,
                arm_const.UC_ARM_REG_R2, arm_const.UC_ARM_REG_R3,
                arm_const.UC_ARM_REG_R4, arm_const.UC_ARM_REG_R5,
                arm_const.UC_ARM_REG_R6, arm_const.UC_ARM_REG_R7,
                arm_const.UC_ARM_REG_R8, arm_const.UC_ARM_REG_R9,
                arm_const.UC_ARM_REG_R10, arm_const.UC_ARM_REG_R11,
                arm_const.UC_ARM_REG_R12]
        saved = {r: uc.reg_read(r) for r in regs}
        saved_lr = uc.reg_read(UC_ARM_REG_LR)
        saved_pc = uc.reg_read(UC_ARM_REG_PC)
        saved_xpsr = uc.reg_read(UC_ARM_REG_XPSR)
        for i, v in enumerate(list(args)[:4]):
            uc.reg_write(regs[i], int(v) & 0xFFFFFFFF)
        uc.reg_write(UC_ARM_REG_LR, TRAMP_ADDR | 1)
        uc.reg_write(UC_ARM_REG_PC, addr & ~1)
        self._tramp_hit = False
        start = self.insn_count
        returned = False
        guard = 0
        while not returned and (self.insn_count - start) < max_insns:
            guard += 1
            if guard > 10_000:
                break
            remaining = max_insns - (self.insn_count - start)
            self._batch_target = self.insn_count + max(1, min(remaining, 200_000))
            self._prev_block = None
            try:
                uc.emu_start(uc.reg_read(UC_ARM_REG_PC) | 1, 0, 0, 0)
            except UcError as e:
                self._record_fault('CALL_UC:' + str(e).strip(),
                                   self.insn_count, saved_lr, 0, None)
                break
            self._batch_target = None
            if self._tramp_hit:
                returned = True
                break
            if self._stop_reason == 'breakpoint':
                break
        self._batch_target = None
        self._tramp_hit = False
        # restore caller context
        for r, v in saved.items():
            try:
                uc.reg_write(r, v)
            except UcError:
                pass
        try:
            uc.reg_write(UC_ARM_REG_LR, saved_lr)
            uc.reg_write(UC_ARM_REG_PC, saved_pc)
            uc.reg_write(UC_ARM_REG_XPSR, saved_xpsr)
        except UcError:
            pass
        self._prev_block = None
        self.last_function_call = dict(addr=addr & ~1, returned=returned,
                                       insns=self.insn_count - start,
                                       args=list(args))
        self.note_event('CALL_FUNCTION', addr & ~1)
        return returned

    RF_INIT_FUNC = 0x08004AF0

    def rf_init(self, max_insns=3_000_000):
        """Bring up the virtual CC2500 by calling the firmware RF init routine."""
        if getattr(self, 'rf_initialized', False):
            return self.rf_status()
        ok = self.call_function(self.RF_INIT_FUNC, max_insns=max_insns)
        self.rf_initialized = bool(ok)
        try:
            self.run(200_000)          # let the driver settle / enter RX
        except Exception:
            pass
        return self.rf_status()

    def boot_interactive(self, warmup=1_000_000, with_rf=True, settle=200_000,
                         rf_max_insns=3_000_000, pre_settle=200_000):
        """One-shot interactive boot: reset -> 1 ms scheduler -> settle ->
        RF init -> settle.  (A short settle before the RF bring-up is needed
        because the display/scheduler state machine has to reach its steady
        state before 0x08004AF0 is called.)"""
        self.reset()
        self.skip_to_main_loop(warmup)
        if pre_settle:
            self.run(pre_settle)
        rf = None
        if with_rf:
            try:
                rf = self.rf_init(rf_max_insns)
            except Exception as e:
                rf = dict(present=False, error=str(e))
        if settle:
            self.run(settle)
        return dict(ok=True, pc=self.reg('PC'), insn=self.insn_count,
                    pcName=self.pc_name(), taskPhase=self.task_phase(),
                    rf=rf, rfInitialized=bool(getattr(self, 'rf_initialized', False)),
                    inputs=self.inputs.state())

    # -- virtual CC2500 ------------------------------------------------------
    def rf_status(self):
        if getattr(self, 'cc2500', None) is None:
            return {'present': False}
        d = self.cc2500.state_dict()
        d['present'] = True
        return d

    def rf_inject_rx(self, payload, rssi=0x28, lqi=0x10):
        """Inject a 2.4 GHz packet into the virtual CC2500 (GDO0 -> EXTI15)."""
        if getattr(self, 'cc2500', None) is None:
            raise RuntimeError('cc2500 module not available')
        if isinstance(payload, str):
            payload = bytes.fromhex(payload.replace(' ', ''))
        self.cc2500.inject_rx(payload, rssi=rssi, lqi=lqi)
        return self.rf_status()

    def rf_tx_packets(self):
        if getattr(self, 'cc2500', None) is None:
            return []
        return [p.hex() for p in self.cc2500.tx_packets]

    # -- breakpoints ---------------------------------------------------------
    def bp_add(self, addr):
        addr &= ~1
        self.breakpoints.add(addr)
        self._sync_bp_hooks()
        return True

    def bp_del(self, addr):
        self.breakpoints.discard(addr & ~1)
        self._sync_bp_hooks()
        return True

    def _sync_bp_hooks(self):
        uc = self._uc
        for addr, h in list(self._bp_hooks.items()):
            if addr not in self.breakpoints:
                try:
                    uc.hook_del(h)
                except UcError:
                    pass
                del self._bp_hooks[addr]
        for addr in self.breakpoints:
            if addr in self._bp_hooks:
                continue
            h = uc.hook_add(UC_HOOK_CODE, self._bp_hit, None, addr, addr + 3)
            self._bp_hooks[addr] = h

    def _bp_hit(self, uc, address, size, user_data):
        self._stop_reason = 'breakpoint'
        self._bp_pc = address
        uc.emu_stop()
        return False

    # -- execution -----------------------------------------------------------
    def run(self, count=None, *, reset_if_needed=False):
        """Execute up to ``count`` instructions, dispatching virtual IRQs.

        Unicorn MCLASS has no NVIC, so handlers are entered manually between
        emulation batches: save context -> LR = trampoline -> PC = RAM vector
        handler, run until the handler returns through the trampoline, then
        restore the interrupted context.
        """
        t0 = time.time()
        start_insn = self.insn_count
        target = None
        if count is not None:
            target = self.insn_count + int(count)
        self._stop_reason = None
        status = 'ok'
        guard = 0
        while True:
            guard += 1
            if guard > 2_000_000:
                status = 'loop_guard'
                break
            if not self._in_isr and self._dispatch_due_irq():
                res = self._run_isr()
                if res == 'fault':
                    status = 'fault'
                if res in ('fault', 'breakpoint'):
                    break
                if target is not None and self.insn_count >= target:
                    break
                continue
            stop = target
            if not self._in_isr and self.real_irq:
                if self.virtual_tick and (stop is None or self._next_tick < stop):
                    stop = self._next_tick
                if self.virtual_tim14 and (stop is None or self._next_tim14 < stop):
                    stop = self._next_tim14
            if self._in_isr and self._isr_stop_insn is not None:
                stop = self._isr_stop_insn if stop is None else min(stop, self._isr_stop_insn)
            self._batch_target = stop
            self._prev_block = None
            pc = self._uc.reg_read(UC_ARM_REG_PC)
            try:
                self._uc.emu_start(pc | 1, 0, 0, 0)
            except UcError as e:
                status = 'fault'
                self._record_fault('UC:' + str(e).strip(), pc,
                                   self._uc.reg_read(UC_ARM_REG_LR),
                                   self._uc.reg_read(UC_ARM_REG_SP), None)
                self._stop_reason = 'exception'
                self._prev_block = None
                self._batch_target = None
                break
            self._batch_target = None
            if self._tramp_hit:
                self._leave_isr('return')
                if target is not None and self.insn_count >= target:
                    break
                continue
            if self._in_isr:
                if self._stop_reason == 'isr_timeout':
                    self._leave_isr('timeout')
                    if target is not None and self.insn_count >= target:
                        break
                    continue
                if self._stop_reason == 'breakpoint':
                    break
                if target is not None and self.insn_count >= target:
                    break
                continue
            if self._stop_reason == 'breakpoint':
                break
            if target is not None and self.insn_count >= target:
                break
            if self._stop_reason == 'batch':
                continue
            break
        self.run_time += time.time() - t0
        done = self.insn_count - start_insn
        self._last_batch_count = done
        if status == 'ok':
            if self._stop_reason == 'breakpoint':
                status = 'breakpoint'
            elif target is not None and self.insn_count >= target:
                status = 'ok'
            else:
                status = 'stopped'
        return dict(status=status, pc=self._uc.reg_read(UC_ARM_REG_PC),
                    insn=self.insn_count, executed=done,
                    reason=self._stop_reason, faults=len(self.faults),
                    inIsr=self._in_isr, irqCounts=dict(self.irq_counts))

    def step(self, n=1):
        """Execute exactly n instructions (uses count=1 per instruction)."""
        start = self.insn_count
        status = 'ok'
        for _ in range(max(1, int(n))):
            pc = self._uc.reg_read(UC_ARM_REG_PC)
            self._stop_reason = None
            self._batch_target = None
            self._prev_block = None
            try:
                self._uc.emu_start(pc | 1, 0, 0, 1)
            except UcError as e:
                status = 'fault'
                self._record_fault('UC:' + str(e).strip(), pc,
                                   self._uc.reg_read(UC_ARM_REG_LR),
                                   self._uc.reg_read(UC_ARM_REG_SP), None)
                break
            # count=1 runs exactly one instruction; account it
            self.insn_count += 1
            self._prev_block = None
        return dict(status=status, pc=self._uc.reg_read(UC_ARM_REG_PC),
                    insn=self.insn_count, executed=self.insn_count - start,
                    faults=len(self.faults))

    def set_pc(self, addr):
        self._uc.reg_write(UC_ARM_REG_PC, addr & ~1)
        self._prev_block = None

    # -- fast boot -----------------------------------------------------------
    MAIN_LOOP_PC = 0x0800D008          # main scheduler preamble (r4/r5/flag init)

    def skip_to_main_loop(self, warmup=1_000_000):
        """Warm up the boot path, then jump straight into the main scheduler.

        The real boot path spends >50M instructions in display-refresh /
        power-on handshake loops before the 1 ms scheduler is reached (see
        README).  For interactive debugging we can enter the scheduler
        directly: 0x0800D008 sets r4=0, r5=&event flag, r0=&ui_flag, waits for
        the virtual SysTick flag, then calls the task list (key scan first).
        """
        if warmup:
            self.run(warmup)
        self.set_reg('R4', 0)
        self.set_reg('R5', 0x200001E1)
        self.set_reg('R0', 0x2000021B)
        self.set_pc(self.MAIN_LOOP_PC)
        self.in_main_loop = True
        self.note_event('skip_to_main_loop', self.insn_count)
        return dict(ok=True, pc=self.MAIN_LOOP_PC, insn=self.insn_count)

    def stop(self):
        self._stop_reason = 'requested'
        self._uc.emu_stop()

    # -- injection -----------------------------------------------------------
    def inject_uart(self, data):
        if isinstance(data, str):
            data = data.encode('latin-1', 'replace')
        self.usart1.rx.extend(data)
        self.usart2.rx.extend(data)

    def inject_key(self, port, pin, value):
        gpio = self.devices.get('GPIO' + port.upper().lstrip('P'))
        if gpio is None:
            gpio = self.devices.get(port.upper())
        if gpio is None:
            return False
        gpio.inject[pin] = 1 if value else 0
        return True

    def key_down(self, name):
        return self.inputs.set_key(name, True)

    def key_up(self, name):
        return self.inputs.set_key(name, False)

    def key_press(self, name, ms=80):
        return self.inputs.press(name, ms)

    def clear_inputs(self):
        return self.inputs.clear()

    def set_battery(self, mv):
        return self.inputs.set_battery(mv)

    def set_adc(self, channel, mv):
        return self.inputs.set_adc(channel, mv)

    def inject_adc(self, value, channel=None):
        if channel is None:
            self.adc.default = value & 0xFFF
        else:
            self.adc.set_channel(channel, value & 0xFFF)
        return True

    def inject_spi(self, data):
        if isinstance(data, str):
            data = data.encode('latin-1', 'replace')
        self.spi1.miso.extend(data)
        return True

    def inject_exti(self, mask):
        self.exti.inject(mask)
        return True

    # -- introspection -------------------------------------------------------
    def uart_text(self):
        return ''.join(chr(c) if 32 <= c < 127 or c in (9, 10, 13) else '.'
                       for c in self.uart_out)

    def func_at(self, pc):
        return self.functions.lookup(pc)

    # -- screen --------------------------------------------------------------
    def frame_buffer(self):
        """The firmware's own 128x64 1bpp frame buffer at RAM 0x200004CC.

        This is the ground truth for what the panel shows (docs/04_ui_lcd.md
        搂2.3): page(0..7)*128 + x(0..127), bit0 = top pixel of the page.
        """
        try:
            data = bytes(self.read_mem(self.fb_addr, (self.fb_width * self.fb_height) // 8))
        except Exception:
            data = b''
        if len(data) != (self.fb_width * self.fb_height) // 8:
            data = bytes(self.screen_bits_decoded())
        return data

    def screen_bits_decoded(self):
        return self.lcd.frame_bits()

    def screen_info(self):
        fb = self.frame_buffer()
        return dict(w=self.fb_width, h=self.fb_height, format='1bpp',
                    addr=self.fb_addr, frames=self.lcd.frames,
                    bytes_written=len(self.lcd.bytes),
                    nonzero=sum(1 for b in fb if b),
                    page=self.lcd.page, col=self.lcd.col,
                    lcd_bytes=len(self.lcd.bytes))

    def screen_bits(self):
        """Ground-truth frame buffer (RAM 0x200004CC), page/column format."""
        return self.frame_buffer()

    def screen_json(self):
        bits = self.screen_bits()
        return dict(w=self.fb_width, h=self.fb_height, addr=self.fb_addr,
                    format='1bpp', data=list(bits), hex=bits.hex())

    def state_dict(self):
        return dict(
            pc=self._uc.reg_read(UC_ARM_REG_PC),
            sp=self._uc.reg_read(UC_ARM_REG_SP),
            lr=self._uc.reg_read(UC_ARM_REG_LR),
            xpsr=self._uc.reg_read(UC_ARM_REG_CPSR),
            regs=[self._uc.reg_read(getattr(arm_const, 'UC_ARM_REG_R%d' % i))
                  for i in range(13)],
            insnCount=self.insn_count,
            breakpoints=sorted(self.breakpoints),
            faults=[f.to_dict() for f in self.faults[-20:]],
            running=False,
            func=self._func_dict(),
            uart=self.uart_text()[-4000:],
            events=[dict(insn=i, what=w, value=v) for i, w, v in self.events[-200:]],
            lcd=self.screen_info(),
            inputs=self.inputs.state(),
            irqCounts=dict(self.irq_counts),
            irqLog=list(self._irq_dispatch_log[-40:]),
            inIsr=bool(self._in_isr),
            realIrq=bool(self.real_irq),
            virtualTicks=getattr(self, 'virtual_ticks', 0),
            tim14Count=getattr(self, '_tim14_count', 0),
            rf=(self.rf_status() if hasattr(self, 'rf_status') else None),
        )

    def _func_dict(self):
        pc = self._uc.reg_read(UC_ARM_REG_PC)
        f = self.functions.lookup(pc)
        if f:
            return dict(addr=f[0], name=f[1])
        return None

    def pc_name(self):
        pc = self._uc.reg_read(UC_ARM_REG_PC)
        f = self.functions.lookup(pc)
        return f[1] if f else '0x%08X' % pc

    def task_phase(self):
        """Coarse runtime phase for the web UI ('boot' until the 1 ms scheduler
        is reached; 'main_loop' afterwards; 'fault' if the CPU crashed)."""
        if self.faults:
            return 'fault'
        if not getattr(self, 'in_main_loop', False):
            return 'boot'
        pc = self._uc.reg_read(UC_ARM_REG_PC)
        if 0x0800D008 <= pc < 0x0800D068:
            return 'scheduler'
        return 'main_loop'

    def periph_summary(self):
        """Aggregate MMIO trace per (device, register, direction)."""
        agg = {}
        for t in self.trace:
            key = (t.dev, t.reg, t.rw)
            e = agg.get(key)
            if e is None:
                agg[key] = dict(dev=t.dev, reg=t.reg, rw=t.rw, count=1,
                                first=t.value, last=t.value,
                                first_insn=t.insn, last_insn=t.insn)
            else:
                e['count'] += 1
                e['last'] = t.value
                e['last_insn'] = t.insn
        return sorted(agg.values(), key=lambda x: (x['dev'], x['reg'], x['rw']))
