#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Boot test: load AD100pro app image at 0x08003000 and try to execute from reset.

This is a minimal proof-of-concept used to validate the load-base discovery
(0x08003000) and to enumerate the hardware the firmware touches at boot.

Usage:
  python tools/boottest.py [--steps N] [--trace out/boottest_trace.log]
"""
import os, sys, struct, collections

import unicorn
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE, \
    UC_HOOK_MEM_FETCH, UC_HOOK_INTR, UC_HOOK_CODE, UcError
from unicorn.arm_const import *
from unicorn import arm_const
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import lcd_decode as _lcdmod

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FW = os.path.join(ROOT, 'firmware', 'AD100pro_V1.2.bin')

IMG_BASE = 0x08003000
IMG_END = IMG_BASE + 50248
FLASH_BASE, FLASH_SIZE = 0x08000000, 0x20000
RAM_BASE, RAM_SIZE = 0x20000000, 0x2000
MMIO = [(0x40000000, 0x24000), (0x48000000, 0x8000), (0xE0000000, 0x10000)]

PERIPH = {
    0x40000000: 'TIM2', 0x40000400: 'TIM3', 0x40001000: 'TIM6', 0x40001400: 'TIM7',
    0x40002000: 'TIM14', 0x40002800: 'RTC', 0x40002C00: 'WWDG', 0x40003000: 'IWDG',
    0x40003800: 'SPI2', 0x40004400: 'USART2', 0x40005400: 'I2C1', 0x40005800: 'I2C2',
    0x40007000: 'PWR', 0x40010000: 'SYSCFG', 0x40010400: 'EXTI', 0x40012400: 'ADC',
    0x40012C00: 'TIM1', 0x40013000: 'SPI1', 0x40013800: 'USART1', 0x40014000: 'TIM15',
    0x40014400: 'TIM16', 0x40014800: 'TIM17', 0x40020000: 'DMA1', 0x40021000: 'RCC',
    0x40022000: 'FLASH', 0x40023000: 'CRC',
    0x48000000: 'GPIOA', 0x48000400: 'GPIOB', 0x48000800: 'GPIOC', 0x48000C00: 'GPIOD',
    0x48001000: 'GPIOE', 0x48001400: 'GPIOF',
    0xE000E000: 'SYS', 0xE000ED00: 'SCB',
}
REGS = {
    'GPIO': {0x00: 'MODER', 0x04: 'OTYPER', 0x08: 'OSPEEDR', 0x0C: 'PUPDR', 0x10: 'IDR',
             0x14: 'ODR', 0x18: 'BSRR', 0x1C: 'LCKR', 0x20: 'AFRL', 0x24: 'AFRH', 0x28: 'BRR'},
    'RCC': {0x00: 'CR', 0x04: 'CFGR', 0x08: 'CIR', 0x0C: 'APB2RSTR', 0x10: 'APB1RSTR',
            0x14: 'AHBENR', 0x18: 'APB2ENR', 0x1C: 'APB1ENR', 0x20: 'BDCR', 0x24: 'CSR',
            0x28: 'AHBRSTR', 0x2C: 'CFGR2', 0x30: 'CFGR3', 0x34: 'CR2'},
    'FLASH': {0x00: 'ACR', 0x04: 'KEYR', 0x08: 'OPTKEYR', 0x0C: 'SR', 0x10: 'CR',
              0x14: 'AR', 0x1C: 'OBR', 0x20: 'WRPR'},
    'EXTI': {0x00: 'IMR', 0x04: 'EMR', 0x08: 'RTSR', 0x0C: 'FTSR', 0x10: 'SWIER', 0x14: 'PR'},
    'SYSCFG': {0x00: 'CFGR1', 0x04: 'CFGR2'},
    'TIM': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'SMCR', 0x0C: 'DIER', 0x10: 'SR', 0x14: 'EGR',
            0x18: 'CCMR1', 0x1C: 'CCMR2', 0x20: 'CCER', 0x24: 'CNT', 0x28: 'PSC',
            0x2C: 'ARR', 0x34: 'CCR1', 0x38: 'CCR2', 0x3C: 'CCR3', 0x40: 'CCR4',
            0x44: 'BDTR', 0x48: 'DCR', 0x4C: 'DMAR'},
    'ADC': {0x00: 'ISR', 0x04: 'IER', 0x08: 'CR', 0x0C: 'CFGR1', 0x10: 'CFGR2',
            0x14: 'SMPR', 0x40: 'DR'},
    'DMA1': {0x00: 'ISR', 0x04: 'IFCR'},
    'PWR': {0x00: 'CR', 0x04: 'CSR'},
    'IWDG': {0x00: 'KR', 0x04: 'PR', 0x08: 'RLR', 0x0C: 'SR'},
}
GROUP = {}
for b, n in PERIPH.items():
    if n.startswith('GPIO'):
        GROUP[b] = 'GPIO'
    elif n.startswith('TIM'):
        GROUP[b] = 'TIM'
    elif n.startswith('FLASH'):
        GROUP[b] = 'FLASH'
    else:
        GROUP[b] = n


def regname(addr):
    best = None
    for b in PERIPH:
        if b <= addr < b + 0x400:
            if best is None or b > best:
                best = b
    if best is None:
        return None
    pname = PERIPH[best]
    off = addr - best
    g = GROUP[best]
    if g == 'DMA1':
        return f'{pname}.{ {0:"ISR",4:"IFCR"}.get(off, "CH%d+%X" % (off//0x1C+1, off%0x1C)) }'
    nm = REGS.get(g, {}).get(off)
    return f'{pname}.{nm}' if nm else f'{pname}.+0x{off:X}'


class Emu:
    def __init__(self, trace_path=None):
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
        data = open(FW, 'rb').read()
        self.image = data
        self.uc.mem_map(FLASH_BASE, FLASH_SIZE)
        # calibration area at 0x0801F000 is factory data, not part of the update file
        UNPROG = b'\xff'
        self.uc.mem_write(IMG_END, UNPROG * (0x0801F000 - IMG_END))
        calib = bytearray(UNPROG * (FLASH_BASE + FLASH_SIZE - 0x0801F000))
        if os.environ.get('CALIB', 'blank') == 'synthetic':
            for rec in range(0x1000 // 100):
                off = rec * 100
                calib[off:off + 100] = bytes([0x5C] + [0] * 99)
        self.uc.mem_write(0x0801F000, bytes(calib))
        # manual-ISR return trampoline (after the 0xFF fill so it is not overwritten)
        self.uc.mem_write(0x0800F900, b'\xfe\xe7')
        self.uc.mem_write(0x0800F902, b'\x00\x00')
        # app vector table also mirrored at 0x08000000 so exceptions resolve
        self.uc.mem_write(FLASH_BASE, data[:0xC0])
        self.uc.mem_write(IMG_BASE, data)
        self.uc.mem_map(RAM_BASE, RAM_SIZE)
        for b, s in MMIO:
            self.uc.mem_map(b, s)
        self.mmio = collections.defaultdict(int)
        self.gpio_odr = collections.defaultdict(int)
        # --- bit-banged LCD decoding (SCL=PB6, A0=PB7, CS=PB9, SDA=PA10) ---
        self.lcd = _lcdmod.Lcd()
        self.lcd_bits = 0
        self.lcd_nb = 0
        self.lcd_a0 = 0
        self.lcd_cs = 1
        self.lcd_prev_scl = 0
        self.lcd_stream = []
        self.lcd_png = os.environ.get('LCD_PNG')
        # --- virtual key matrix: rows PB3/PB4/PB5, cols PC10/PC11/PC12/PC7 and PD4 ---
        # keycode = row*5 + col  (0..14); pressing key k pulls its column low while
        # the scanner drives the matching row low.
        self.vkeys = set()
        kc = os.environ.get('KEYCODES', '').strip()
        if kc:
            for tok in kc.replace(';', ',').split(','):
                tok = tok.strip()
                if tok:
                    self.vkeys.add(int(tok, 0))
        self.key_window = os.environ.get('KEY_WINDOW', '')   # e.g. 8000000:12000000
        # --- manual EXTI/IRQ injection (Unicorn MCLASS has no NVIC) ---
        self.TRAMP = 0x0800F900
        self.isr_pending = None
        self.tramp_hit = False
        self.saved_ctx = None
        self.power_pressed = False
        self.power_at = int(os.environ.get('POWER_AT', '0') or 0)
        self.power_off_at = int(os.environ.get('POWER_OFF_AT', '0') or 0)
        self.power_injected = False
        self.power_handler = 0x08005A78   # IRQ5 handler (EXTI0_1)
        self.systick_handler = 0x0800B8E4  # SysTick vector[15] = 0x0800B8E5
        self.systick_period = int(os.environ.get('SYSTICK_PERIOD', '0') or 0)
        self.systick_next = self.systick_period
        self.systick_count = 0
        self.keys_ever_pressed = 0
        self.log = []
        self.trace_path = trace_path
        self.trace = open(trace_path, 'w', encoding='utf-8') if trace_path else None
        self.insn = 0
        self.last_pc = 0
        self.rcc_cr = 0x03020003       # HSI/HSE/PLL ready
        self.rcc_cfgr = 0
        self.flash_sr = 0
        self.systick_load = 0
        self.uc.hook_add(UC_HOOK_MEM_READ, self.on_read)
        self.uc.hook_add(UC_HOOK_MEM_WRITE, self.on_write)
        self.uc.hook_add(UC_HOOK_INTR, self.on_intr)
        self.uc.hook_add(UC_HOOK_CODE, self.on_code)

    # ---------------------------------------------------------------- hooks
    def in_mmio(self, a):
        for b, s in MMIO:
            if b <= a < b + s:
                return b
        return None

    def on_code(self, uc, address, size, user):
        if address == self.TRAMP:
            self.tramp_hit = True
            uc.emu_stop()
            return
        self.insn += 1
        self.last_pc = address

    def _enter_isr(self, handler):
        uc = self.uc
        self.saved_ctx = {
            'r': [uc.reg_read(getattr(arm_const, 'UC_ARM_REG_R%d' % i)) for i in range(13)],
            'lr': uc.reg_read(UC_ARM_REG_LR),
            'pc': uc.reg_read(UC_ARM_REG_PC),
            'xpsr': uc.reg_read(UC_ARM_REG_XPSR),
        }
        uc.reg_write(UC_ARM_REG_LR, self.TRAMP | 1)

    def _leave_isr(self):
        uc = self.uc
        c = self.saved_ctx
        for i in range(13):
            uc.reg_write(getattr(arm_const, 'UC_ARM_REG_R%d' % i), c['r'][i])
        uc.reg_write(UC_ARM_REG_LR, c['lr'])
        uc.reg_write(UC_ARM_REG_PC, c['pc'])
        uc.reg_write(UC_ARM_REG_XPSR, c['xpsr'])
        self.saved_ctx = None

    def trigger_irq(self, handler, at_insn=None):
        """Manually invoke an exception handler (no NVIC in Unicorn MCLASS)."""
        self.isr_pending = (handler, self.insn if at_insn is None else at_insn)

    def on_read(self, uc, access, address, size, value, user):
        if self.in_mmio(address) is None:
            return
        rn = regname(address) or f'0x{address:08X}'
        base = address & ~0x3FF if (address & 0xFFFC0000) == 0x48000000 else address
        val = self.mmio[address]
        # GPIO input data register: apply virtual key matrix (pull-ups = 1)
        if 0x48000000 <= address < 0x48002000:
            port = 0x48000000 + ((address - 0x48000000) & ~0x3FF)
            if address - port == 0x10:
                val = self.gpio_idr(port)
                self.uc.mem_write(address, struct.pack('<I', val & 0xFFFFFFFF))
                self.log.append(('R', address, rn, val, self.last_pc, self.insn))
                if self.trace and len(self.log) < 400000:
                    self.trace.write(f'{self.insn:9d} {self.last_pc:08X}  R {rn:20s} = 0x{val:08X}\n')
                return
        # RCC special
        if 0x40021000 <= address < 0x40021400:
            off = address - 0x40021000
            if off == 0x00:
                val = self.rcc_cr
            elif off == 0x04:
                sw = self.rcc_cfgr & 3
                val = (self.rcc_cfgr & ~0xC) | (sw << 2)
            elif off == 0x0C:
                val = 0
            elif off == 0x24:
                val = (self.mmio[address] | 0x2)
            elif off == 0x20:
                val = (self.mmio[address] | 0x2)
        elif 0x40022000 <= address < 0x40022400:
            off = address - 0x40022000
            if off == 0x0C:
                val = self.flash_sr
            elif off == 0x00:
                val = self.mmio[address]
        elif 0x40002800 <= address < 0x40002C00:
            off = address - 0x40002800
            if off == 0x0C:
                val = 0x0000007F
            else:
                val = self.mmio[address]
        elif 0xE000E010 <= address < 0xE000E020:
            off = address - 0xE000E010
            if off == 0x08:
                val = (self.insn // 3) & 0xFFFFFF
        self.uc.mem_write(address, struct.pack('<I', val & 0xFFFFFFFF))
        self.log.append(('R', address, rn, val, self.last_pc, self.insn))
        if self.trace and len(self.log) < 400000:
            self.trace.write(f'{self.insn:9d} {self.last_pc:08X}  R {rn:20s} = 0x{val:08X}\n')

    def on_write(self, uc, access, address, size, value, user):
        if self.in_mmio(address) is None:
            return
        rn = regname(address) or f'0x{address:08X}'
        self.mmio[address] = value
        # side effects
        if 0x40021000 <= address < 0x40021400:
            off = address - 0x40021000
            if off == 0x00:
                self.rcc_cr = value | 0x03020003
            elif off == 0x04:
                self.rcc_cfgr = value
            elif off == 0x24:
                self.mmio[address] = value | 0x2
            elif off == 0x20:
                self.mmio[address] = value | 0x2
        elif 0x40022000 <= address < 0x40022400:
            off = address - 0x40022000
            if off == 0x0C:
                self.flash_sr = value & ~0x07
            elif off == 0x00:
                self.flash_sr = 0
        elif 0x48000000 <= address < 0x48002000:
            # GPIO: BSRR/BRR affect ODR
            port = 0x48000000 + ((address - 0x48000000) & ~0x3FF)
            off = address - port
            if off == 0x18:
                odr = self.mmio[port + 0x14]
                odr |= value & 0xFFFF
                odr &= ~((value >> 16) & 0xFFFF)
                self.mmio[port + 0x14] = odr
                self.uc.mem_write(port + 0x14, struct.pack('<I', odr))
            elif off == 0x28:
                odr = self.mmio[port + 0x14] & ~(value & 0xFFFF)
                self.mmio[port + 0x14] = odr
                self.uc.mem_write(port + 0x14, struct.pack('<I', odr))
            self.gpio_odr[port] = self.mmio[port + 0x14]
            self._lcd_step()
        self.log.append(('W', address, rn, value, self.last_pc, self.insn))
        if self.trace and len(self.log) < 400000:
            self.trace.write(f'{self.insn:9d} {self.last_pc:08X}  W {rn:20s} = 0x{value:08X}\n')

    def gpio_idr(self, port):
        """Virtual key matrix: rows PB3/PB4/PB5 driven low by the scanner, columns
        PC10/PC11/PC12/PC7 (GPIOC) and PD4 (GPIOD) sampled high (pull-up)."""
        val = 0xFFFF
        if port == 0x48000400 and self.power_pressed:
            val &= ~0x0001        # power key on PB0 (EXTI0, active low)
        kset = self.vkeys
        w = self.key_window
        if w and ':' in w:
            lo, hi = w.split(':')
            if not (int(lo, 0) <= self.insn < int(hi, 0)):
                kset = set()
        if kset:
            self.keys_ever_pressed = len(kset)
            gpiob = self.gpio_odr.get(0x48000400, 0)
            rows = (3, 4, 5)
            cols = ((0x48000800, 10), (0x48000800, 11), (0x48000800, 12),
                    (0x48000800, 7), (0x48000C00, 4))
            for k in kset:
                r, c = divmod(k, 5)
                if r >= len(rows) or c >= len(cols):
                    continue
                if (gpiob & (1 << rows[r])) == 0:      # scanner drives this row low
                    pbase, bit = cols[c]
                    if pbase == port:
                        val &= ~(1 << bit)
        return val

    def _lcd_step(self):
        """Sample the bit-banged SH1106/SSD1306 serial stream."""
        gpioa = self.gpio_odr.get(0x48000000, 0)
        gpiob = self.gpio_odr.get(0x48000400, 0)
        scl = 1 if gpiob & 0x40 else 0
        cs_new = 0 if not (gpiob & 0x200) else 1
        if cs_new == 0 and self.lcd_cs != 0:
            self.lcd_a0 = 1 if gpiob & 0x80 else 0
        self.lcd_cs = cs_new
        if scl == 1 and self.lcd_prev_scl == 0 and self.lcd_cs == 0:
            bit = 1 if gpioa & 0x400 else 0
            self.lcd_bits = (self.lcd_bits << 1) | bit
            self.lcd_nb += 1
            if self.lcd_nb == 8:
                self.lcd.write(self.lcd_a0, self.lcd_bits)
                self.lcd_stream.append((self.lcd_a0, self.lcd_bits))
                self.lcd_bits = 0
                self.lcd_nb = 0
        self.lcd_prev_scl = scl

    def save_screen(self, path=None, rotate=90):
        path = path or self.lcd_png
        if not path:
            return
        W, H = _lcdmod.W, _lcdmod.H
        pix = [[0] * W for _ in range(H)]
        for y in range(H):
            page, bit = y // 8, y % 8
            for x in range(W):
                pix[y][x] = (self.lcd.ram[page][x] >> bit) & 1
        if rotate == 90:
            pix = [[pix[H - 1 - y][x] for y in range(H)] for x in range(W)]
        elif rotate == 180:
            pix = [[pix[H - 1 - y][W - 1 - x] for x in range(W)] for y in range(H)]
        elif rotate == 270:
            pix = [[pix[y][W - 1 - x] for y in range(H)] for x in range(W)]
        scale = 4
        rgb = []
        for row in pix:
            line = []
            for v in row:
                c = (245, 247, 250) if v else (16, 20, 26)
                line.extend([c] * scale)
            for _ in range(scale):
                rgb.append(line)
        _lcdmod.write_png(path, rgb)
        print(f'LCD: bytes={len(self.lcd_stream)} data_writes={self.lcd.data_writes} '
              f'-> {path}')

    def on_intr(self, uc, intno, user):
        self.log.append(('I', intno, '', 0, self.last_pc, self.insn))

    # ---------------------------------------------------------------- run
    def run(self, steps=2_000_000):
        uc = self.uc
        sp = struct.unpack_from('<I', self.image, 0)[0]
        reset = struct.unpack_from('<I', self.image, 4)[0]
        uc.reg_write(UC_ARM_REG_SP, sp)
        uc.reg_write(UC_ARM_REG_PC, reset)  # keep thumb bit
        print(f'SP=0x{sp:08X}  reset=0x{reset:08X}')
        pc = reset
        done = 0
        try:
            while done < steps:
                if self.saved_ctx is None and self.systick_period and self.insn >= self.systick_next:
                    self.systick_next = self.insn + self.systick_period
                    self.systick_count += 1
                    if self.systick_count <= 5 or self.systick_count % 100 == 0:
                        print(f'  [inject] SysTick #{self.systick_count} at insn={self.insn}')
                    self.trigger_irq(self.systick_handler)
                if self.saved_ctx is None and self.power_at and not self.power_injected and self.insn >= self.power_at:
                    self.power_injected = True
                    self.power_pressed = True
                    self.mmio[0x40010414] |= 0x0001   # EXTI PR bit0 pending
                    print(f'  [inject] power key pressed at insn={self.insn}, '
                          f'calling EXTI0/IRQ5 handler 0x{self.power_handler:08X}')
                    self.trigger_irq(self.power_handler)
                if self.power_off_at and self.power_pressed and self.insn >= self.power_off_at:
                    self.power_pressed = False
                    print(f'  [inject] power key released at insn={self.insn}')
                if self.saved_ctx is None and self.isr_pending and self.insn >= self.isr_pending[1]:
                    handler = self.isr_pending[0]
                    self.isr_pending = None
                    self._enter_isr(handler)
                    pc = handler | 1
                n = min(200000, steps - done)
                uc.emu_start(pc | 1, 0, count=n)
                done += n
                if self.tramp_hit:
                    self._leave_isr()
                    self.tramp_hit = False
                pc = uc.reg_read(UC_ARM_REG_PC) | 1
                if done % 1000000 == 0:
                    print(f'  {done/1e6:.1f}M insn  pc=0x{pc:08X} sp=0x{uc.reg_read(UC_ARM_REG_SP):08X} '
                          f'lr=0x{uc.reg_read(UC_ARM_REG_LR):08X}')
        except UcError as e:
            print(f'STOP at pc=0x{self.last_pc:08X}: {e}')
            print(f'  insn={self.insn} sp=0x{uc.reg_read(UC_ARM_REG_SP):08X} lr=0x{uc.reg_read(UC_ARM_REG_LR):08X}')
            print(f'  r0=0x{uc.reg_read(UC_ARM_REG_R0):08X} r1=0x{uc.reg_read(UC_ARM_REG_R1):08X} '
                  f'r2=0x{uc.reg_read(UC_ARM_REG_R2):08X} r3=0x{uc.reg_read(UC_ARM_REG_R3):08X} '
                  f'r4=0x{uc.reg_read(UC_ARM_REG_R4):08X} r5=0x{uc.reg_read(UC_ARM_REG_R5):08X} '
                  f'r6=0x{uc.reg_read(UC_ARM_REG_R6):08X} r7=0x{uc.reg_read(UC_ARM_REG_R7):08X}')
        print(f'executed ~{self.insn} instructions, final pc=0x{uc.reg_read(UC_ARM_REG_PC):08X}')
        if self.trace:
            self.trace.close()
        self.save_screen()
        return self.log


def main():
    steps = 2_000_000
    trace = os.path.join(ROOT, 'out', 'boottest_trace.log')
    args = sys.argv[1:]
    for i, a in enumerate(args):
        if a == '--steps':
            steps = int(args[i + 1], 0)
        elif a == '--trace':
            trace = args[i + 1]
    os.makedirs(os.path.dirname(trace), exist_ok=True)
    e = Emu(trace)
    log = e.run(steps)
    # summary
    reads = collections.Counter(rn for k, a, rn, v, pc, i in log if k == 'R')
    writes = collections.Counter(rn for k, a, rn, v, pc, i in log if k == 'W')
    print(f'\nMMIO accesses: {len(log)}  (reads {sum(reads.values())}, writes {sum(writes.values())})')
    print('\ntop registers read:')
    for rn, c in reads.most_common(25):
        print(f'  {rn:22s} {c}')
    print('\ntop registers written:')
    for rn, c in writes.most_common(25):
        print(f'  {rn:22s} {c}')
    print(f'\ntrace: {trace}')


if __name__ == '__main__':
    main()
