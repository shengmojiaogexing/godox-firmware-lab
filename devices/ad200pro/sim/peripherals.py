"""STM32F0-family peripheral models.

Design: the peripheral address windows are mapped as ordinary memory ("register
backing store").  Writes land there; a deferred post-write hook implements the
side effects (ready bits, erase, BSRR, ...).  Reads are served from the backing
store, with a pre-read hook refreshing dynamic values (GPIO IDR, ...).
"""
import struct

FLASH_BASE = 0x08000000
FLASH_SIZE = 0x20000

PERIPH_BASE = 0x40000000
PERIPH_SIZE = 0x40000
GPIO_BASE = 0x48000000
GPIO_SIZE = 0x2000
SCS_BASE = 0xE0000000
SCS_SIZE = 0x100000

REGION_FLASH = 'flash'
REGION_RAM = 'ram'
REGION_PERIPH = 'periph'
REGION_GPIO = 'gpio'
REGION_SCS = 'scs'

PERIPH_NAMES = {
    0x40021000: 'RCC', 0x40022000: 'FLASH', 0x40012400: 'ADC', 0x40012700: 'ADC_common',
    0x40010400: 'EXTI', 0x40010000: 'SYSCFG', 0x40020000: 'DMA1', 0x40007000: 'PWR',
    0x40003000: 'IWDG', 0x40002800: 'RTC', 0x40012C00: 'TIM1', 0x40000000: 'TIM2',
    0x40000400: 'TIM3', 0x40001000: 'TIM6', 0x40001400: 'TIM7', 0x40002000: 'TIM14',
    0x40014000: 'TIM15', 0x40014400: 'TIM16', 0x40014800: 'TIM17', 0x40013800: 'USART1',
    0x40004400: 'USART2', 0x40013000: 'SPI1', 0x40003800: 'SPI2', 0x40005400: 'I2C1',
}
GPIO_PORTS = {0x48000000: 'A', 0x48000400: 'B', 0x48000800: 'C',
              0x48000C00: 'D', 0x48001000: 'E', 0x48001400: 'F'}
TIM_IRQ = {'TIM1': 13, 'TIM2': 15, 'TIM3': 16, 'TIM6': 17, 'TIM7': 18,
           'TIM14': 19, 'TIM15': 20, 'TIM16': 21, 'TIM17': 22}
EXTI_LINE_IRQ = {0: 5, 1: 5, 2: 6, 3: 6, 4: 7, 5: 7, 6: 7, 7: 7, 8: 7, 9: 7, 10: 7,
                 11: 7, 12: 7, 13: 7, 14: 7, 15: 7}

GPIO_REG = {0x00: 'MODER', 0x04: 'OTYPER', 0x08: 'OSPEEDR', 0x0C: 'PUPDR',
            0x10: 'IDR', 0x14: 'ODR', 0x18: 'BSRR', 0x1C: 'LCKR',
            0x20: 'AFR0', 0x24: 'AFR1', 0x28: 'BRR'}
RCC_REG = {0x00: 'CR', 0x04: 'CFGR', 0x08: 'CIR', 0x0C: 'APB2RSTR', 0x10: 'APB1RSTR',
           0x14: 'AHBENR', 0x18: 'APB2ENR', 0x1C: 'APB1ENR', 0x20: 'BDCR', 0x24: 'CSR',
           0x28: 'AHBRSTR', 0x2C: 'CFGR2', 0x30: 'CFGR3', 0x34: 'CR2'}
FLASH_REG = {0x00: 'ACR', 0x04: 'KEYR', 0x08: 'OPTKEYR', 0x0C: 'SR', 0x10: 'CR',
             0x14: 'AR', 0x1C: 'OBR', 0x20: 'WRPR'}
ADC_REG = {0x00: 'ISR', 0x04: 'IER', 0x08: 'CR', 0x0C: 'CFGR1', 0x10: 'CFGR2',
           0x14: 'SMPR', 0x28: 'TR', 0x30: 'CHSELR', 0x50: 'DR'}
DMA_CH_REG = {0x00: 'CCR', 0x04: 'CNDTR', 0x08: 'CPAR', 0x0C: 'CMAR'}
TIM_REG = {0x00: 'CR1', 0x04: 'CR2', 0x08: 'SMCR', 0x0C: 'DIER', 0x10: 'SR',
           0x14: 'EGR', 0x18: 'CCMR1', 0x1C: 'CCMR2', 0x20: 'CCER', 0x24: 'CNT',
           0x28: 'PSC', 0x2C: 'ARR', 0x34: 'CCR1', 0x38: 'CCR2', 0x3C: 'CCR3',
           0x40: 'CCR4', 0x44: 'BDTR', 0x48: 'DCR', 0x4C: 'DMAR'}
SYSTICK_REG = {0x00: 'CSR', 0x04: 'RVR', 0x08: 'CVR', 0x0C: 'CALIB'}


def classify_region(addr):
    if FLASH_BASE <= addr < FLASH_BASE + FLASH_SIZE:
        return REGION_FLASH
    if 0x20000000 <= addr < 0x20002000:
        return REGION_RAM
    if PERIPH_BASE <= addr < PERIPH_BASE + PERIPH_SIZE:
        return REGION_PERIPH
    if GPIO_BASE <= addr < GPIO_BASE + GPIO_SIZE:
        return REGION_GPIO
    if SCS_BASE <= addr < SCS_BASE + SCS_SIZE:
        return REGION_SCS
    return None


def reg_name(addr):
    """human readable register name for a peripheral address"""
    for base, name in PERIPH_NAMES.items():
        if base <= addr < base + 0x400:
            off = addr - base
            if name == 'RCC':
                return 'RCC_' + RCC_REG.get(off, '+0x%02x' % off)
            if name == 'FLASH':
                return 'FLASH_' + FLASH_REG.get(off, '+0x%02x' % off)
            if name.startswith('ADC'):
                if off == 0x08 and name == 'ADC_common':
                    return 'ADC_CCR'
                return name + '_' + ADC_REG.get(off, '+0x%02x' % off)
            if name.startswith('TIM'):
                return name + '_' + TIM_REG.get(off, '+0x%02x' % off)
            if name == 'DMA1':
                if off < 8:
                    return 'DMA_ISR' if off == 0 else 'DMA_IFCR'
                ch = 1 + (off - 8) // 0x14
                r = (off - 8) % 0x14
                return 'DMA1_CH%d_%s' % (ch, DMA_CH_REG.get(r, '+0x%x' % r))
            return '%s+0x%02x' % (name, off)
    for base, port in GPIO_PORTS.items():
        if base <= addr < base + 0x400:
            return 'GPIO%s_%s' % (port, GPIO_REG.get(addr - base, '+0x%02x' % (addr - base)))
    if SCB_BASE <= addr < SCB_BASE + 0x100:
        pass
    if 0xE000E010 <= addr < 0xE000E020:
        return 'SysTick_' + SYSTICK_REG.get(addr - 0xE000E010, '+0x%02x' % (addr - 0xE000E010))
    if 0xE000E100 <= addr < 0xE000E120:
        return 'NVIC_ISER'
    if 0xE000E180 <= addr < 0xE000E1A0:
        return 'NVIC_ICER'
    if 0xE000E200 <= addr < 0xE000E220:
        return 'NVIC_ISPR'
    if 0xE000E280 <= addr < 0xE000E2A0:
        return 'NVIC_ICPR'
    if 0xE000E400 <= addr < 0xE000E420:
        return 'NVIC_IPR'
    return None


SCB_BASE = 0xE000ED00


class PeripheralBus:
    """All peripheral behaviour lives here; Machine provides memory access + NVIC."""

    def __init__(self, machine, mem, device, tracer):
        self.m = machine
        self.mem = mem            # callables: mem.read(addr,size), mem.write(addr,data)
        self.device = device
        self.trace = tracer
        self.pending = []         # ordered (addr, size, value) written since last flush
        self.flash_unlock = 0
        self.flash_pg = False
        self.irq_pending = [False] * 32
        self.nvic_enabled = 0          # write-1-to-set ISER shadow
        self.systick = {'enabled': False, 'tickint': False, 'reload': 0, 'count': 0}
        self.systick_countflag = False
        self.timers = {}
        self._last_charge_out = None
        self.unimplemented = set()

    # ------------------------------------------------------------- helpers
    def rd(self, addr, size=4):
        return self.mem.read(addr, size)

    def rd32(self, addr):
        try:
            return struct.unpack('<I', self.mem.read(addr, 4))[0]
        except Exception:
            return 0

    def wr32(self, addr, val):
        self.mem.write(addr, struct.pack('<I', val & 0xFFFFFFFF))

    # ------------------------------------------------------- write handling
    def on_write(self, addr, size, value):
        """apply post-write side effects; `value` is the value the store wrote"""
        region = classify_region(addr)
        if region == REGION_GPIO:
            self._gpio_write(addr, value)
        elif region == REGION_PERIPH:
            self._periph_write(addr, value)
        elif region == REGION_SCS:
            self._scs_write(addr)

    def flush(self):
        """apply deferred post-write side effects in program order"""
        pend, self.pending = self.pending, []
        for addr, size, value in pend:
            try:
                self.on_write(addr, size, value)
            except Exception:
                pass

    def queue(self, addr, size=4, value=None):
        self.pending.append((addr & ~3 if addr >= 0x40000000 else addr, size, value))

    # -------------------------------------------------- peripheral dispatch
    def _periph_write(self, addr, value):
        for base, name in PERIPH_NAMES.items():
            if base <= addr < base + 0x400:
                break
        else:
            self.unimplemented.add(addr)
            return
        if name == 'RCC':
            self._rcc_write(addr)
        elif name == 'FLASH':
            self._flash_write(addr)
        elif name == 'ADC':
            self._adc_write(addr)
        elif name == 'ADC_common':
            pass
        elif name == 'DMA1':
            self._dma_write(addr)
        elif name in TIM_IRQ:
            self._tim_write(name, addr)
        elif name == 'RTC':
            self._rtc_write(addr)
        elif name == 'IWDG':
            pass
        elif 0xE000E000 <= addr < 0xE000E100:
            self._scs_write(addr)

    def _scs_write(self, addr):
        if addr == 0xE000E010:            # SysTick CSR
            return
        if 0xE000E100 <= addr < 0xE000E120:        # ISER
            self.nvic_enabled |= self.rd32(addr)
        elif 0xE000E180 <= addr < 0xE000E1A0:      # ICER
            self.nvic_enabled &= ~self.rd32(addr)
        elif 0xE000E200 <= addr < 0xE000E220:      # ISPR
            v = self.rd32(addr)
            for i in range(32):
                if v & (1 << i):
                    self.irq_pending[i] = True
        elif 0xE000E280 <= addr < 0xE000E2A0:      # ICPR
            v = self.rd32(addr)
            for i in range(32):
                if v & (1 << i):
                    self.irq_pending[i] = False

    def _rcc_write(self, addr):
        off = addr - 0x40021000
        v = self.rd32(0x40021000 + (off & ~3))
        if off in (0, 4):
            cr = self.rd32(0x40021000)
            self.wr32(0x40021000, cr | (1 << 1) | (1 << 17) | (1 << 25) | 1)
            cfgr = self.rd32(0x40021004)
            self.wr32(0x40021004, (cfgr & ~0xC) | ((cfgr & 3) << 2))
        elif off == 0x24:      # CSR
            self.wr32(0x40021024, self.rd32(0x40021024) | 3)
        elif off == 0x20:      # BDCR
            self.wr32(0x40021020, self.rd32(0x40021020) | 3)
        if off in (0, 4) and not self.systick['enabled']:
            # firmware programs SysTick via SCB/ SysTick registers later; nothing here
            pass

    def _flash_write(self, addr):
        off = addr - 0x40022000
        if off == 0x04:        # KEYR
            v = self.rd32(addr)
            if v == 0x45670123:
                self.flash_unlock = 1
            elif v == 0xCDEF89AB and self.flash_unlock == 1:
                cr = self.rd32(0x40022010)
                self.wr32(0x40022010, cr & ~((1 << 31) | (1 << 7)))
                self.trace.add('flash', name='unlock')
        elif off == 0x10:      # CR
            cr = self.rd32(0x40022010)
            self.flash_pg = bool(cr & 1)
            if cr & (1 << 6):                     # STRT
                ar = self.rd32(0x40022014)
                if cr & 2:                        # PER: erase page
                    page = ar & 0xFFFFF800
                    if 0x08000000 <= page < 0x08020000:
                        self.mem.write(page, b'\xff' * 0x800)
                        self.trace.add('flash', name='erase page 0x%08x' % page)
                elif cr & (1 << 2):               # MER mass erase
                    self.mem.write(0x08000000, b'\xff' * 0x20000)
                    self.trace.add('flash', name='mass erase')
                self.wr32(0x40022010, cr & ~(1 << 6))
                sr = self.rd32(0x4002200C)
                self.wr32(0x4002200C, (sr & ~1) | 0x20)
                self.trace.add('flash', name='done')
        elif off == 0x0C:      # SR write clears flags
            sr = self.rd32(0x4002200C)
            self.wr32(0x4002200C, sr & 0xFFFFFE7F)

    def flash_data_write(self, addr):
        """a store to flash memory happened while PG might be set"""
        if self.flash_pg:
            sr = self.rd32(0x4002200C)
            self.wr32(0x4002200C, (sr & ~1) | 0x20)

    def _adc_write(self, addr):
        off = addr - 0x40012400
        if off == 0x08:        # CR
            cr = self.rd32(0x40012408)
            if cr & 1:         # ADEN
                isr = self.rd32(0x40012400)
                self.wr32(0x40012400, isr | 1)      # ADRDY
            if cr & 2:         # ADDIS: disable ADC command
                cr &= ~(1 | 2)
                self.wr32(0x40012408, cr)
            if cr & 0x10:      # ADSTP: stop conversion command
                cr &= ~(4 | 0x10)
                self.wr32(0x40012408, cr)
            if cr & 4:         # ADSTART - one shot
                self._adc_convert()
                cr2 = self.rd32(0x40012408)
                self.wr32(0x40012408, cr2 & ~4)

    ADC_DR = 0x40012440          # data register of this MCU
    ADC_CHSELR = (0x28, 0x30)    # be liberal: firmware/this chip differ from RM

    def _adc_channels(self):
        for off in self.ADC_CHSELR:
            v = self.rd32(0x40012400 + off)
            if v:
                return [i for i in range(16) if v & (1 << i)]
        return [3, 10, 11, 12]

    def _adc_convert(self):
        chans = self._adc_channels()
        val = 0
        for ch in chans:
            val = self.device.adc_value(ch)
            self.wr32(self.ADC_DR, val & 0xFFFF)
        isr = self.rd32(0x40012400)
        self.wr32(0x40012400, isr | 0x4 | 0x8)       # EOC | EOS
        self.trace.add('adc', addr=self.ADC_DR,
                       name='ch %s' % ','.join(str(c) for c in chans))
        self._dma_adc(chans)

    def _dma_write(self, addr):
        off = addr - 0x40020000
        if off == 0x04:        # IFCR: clear flags
            v = self.rd32(0x40020004)
            isr = self.rd32(0x40020000)
            self.wr32(0x40020000, isr & ~v)
            self.wr32(0x40020004, 0)
        elif off >= 8:
            ch = 1 + (off - 8) // 0x14
            r = (off - 8) % 0x14
            if r == 0:         # CCR written
                pass

    def _dma_adc(self, chans):
        """copy the ADC sample burst into every DMA channel fed by the ADC"""
        base = 0x40020000 + 8
        for ch in range(1, 8):
            b = base + (ch - 1) * 0x14
            ccr = self.rd32(b)
            cpar = self.rd32(b + 8)
            if not (ccr & 1):
                continue
            if not (0x40012440 <= cpar < 0x40012460):
                continue
            cmar = self.rd32(b + 0xC)
            ndtr = self.rd32(b + 4)
            if not ndtr:
                continue
            for i in range(ndtr):
                chsel = chans[i % len(chans)]
                v = self.device.adc_value(chsel)
                self.mem.write(cmar + 2 * i, struct.pack('<H', v & 0xFFFF))
            circ = bool(ccr & (1 << 5))
            self.wr32(b + 4, ndtr if circ else 0)
            self.wr32(b + 0xC, cmar if circ else cmar + 2 * ndtr)
            isr = self.rd32(0x40020000)
            self.wr32(0x40020000, isr | (1 << (4 * (ch - 1))) | (1 << (4 * (ch - 1) + 1)))
            self.trace.add('dma', addr=cmar, name='CH%d<-%d samples' % (ch, ndtr))

    def _tim_write(self, name, addr):
        off = addr - [b for b, n in PERIPH_NAMES.items() if n == name][0]
        if off == 0x14:        # EGR: UG
            egr = self.rd32(addr)
            if egr & 1:
                base = addr - 0x14
                self.wr32(base + 0x24, 0)
                sr = self.rd32(base + 0x10)
                self.wr32(base + 0x10, sr | 1)
                self.wr32(addr, 0)

    def _rtc_write(self, addr):
        off = addr - 0x40002800
        if off == 0x0C:        # ISR
            v = self.rd32(0x4000280C)
            self.wr32(0x4000280C, v | 0x77)   # ALRAWF|ALRBWF|WUTWF|INITS|RSF|INITF
        elif off == 0x24:      # WPR
            pass

    # ------------------------------------------------------- GPIO handling
    def _gpio_write(self, addr, value=None):
        base = addr & ~0x3FF
        off = addr - base
        port = GPIO_PORTS.get(base)
        if port is None:
            return
        if off == 0x18:        # BSRR
            v = value if value is not None else self.rd32(base + 0x18)
            odr = self.rd32(base + 0x14)
            odr = (odr | (v & 0xFFFF)) & ~((v >> 16) & 0xFFFF)
            self.wr32(base + 0x14, odr)
            self._gpio_output_changed(port, odr)
        elif off == 0x28:      # BRR
            v = value if value is not None else self.rd32(base + 0x28)
            odr = self.rd32(base + 0x14) & ~v
            self.wr32(base + 0x14, odr)
            self._gpio_output_changed(port, odr)
        elif off == 0x14:      # ODR direct
            odr = value if value is not None else self.rd32(base + 0x14)
            self._gpio_output_changed(port, odr)
        elif off == 0x00:      # MODER
            self.device.set_dir(port, self.rd32(base) & 0x3FFFFFFF)

    def _gpio_output_changed(self, port, odr):
        self.device.set_output(port, odr)
        try:
            self.m.on_gpio_output(port, odr)
        except Exception:
            pass
        self.trace.add('gpio_change', name='GPIO%s_ODR' % port, data={'odr': odr})

    # --------------------------------------------------------- pre-read
    def on_read_pre(self, addr):
        """called before the load returns: refresh dynamic register values"""
        region = classify_region(addr)
        if region == REGION_GPIO:
            base = addr & ~0x3FF
            off = addr - base
            port = GPIO_PORTS.get(base)
            if port and off == 0x10:      # IDR
                self._refresh_idr(port, base)
        if 0xE000E100 <= addr < 0xE000E120:
            self.wr32(0xE000E100, self.nvic_enabled)
            return
        if 0xE000E180 <= addr < 0xE000E1A0:
            self.wr32(0xE000E180, ~self.nvic_enabled & 0xFFFFFFFF)
            return
        if addr == 0xE000E010:
            csr = self.rd32(0xE000E010)
            if self.systick_countflag:
                self.systick_countflag = False
                self.wr32(0xE000E010, csr | 0x10000)
            else:
                self.wr32(0xE000E010, csr & ~0x10000)

    def _refresh_idr(self, port, base):
        moder = self.rd32(base)
        odr = self.rd32(base + 0x14)
        pupdr = self.rd32(base + 0x0C)
        idr = 0
        for pin in range(16):
            mode = (moder >> (2 * pin)) & 3
            if mode in (1, 2):           # output / alternate function
                level = (odr >> pin) & 1
            else:
                driven = self.device.gpio_inputs.get(port, {})
                if pin in driven:
                    level = driven[pin]          # externally driven level
                else:
                    pull = (pupdr >> (2 * pin)) & 3
                    level = 1 if pull == 1 else 0   # internal pull-up / pull-down
            idr |= (level & 1) << pin
        self.wr32(base + 0x10, idr)

    # ------------------------------------------------------------- time
    def advance(self, insns):
        """advance peripherals by an number of executed instructions"""
        self._acc = getattr(self, '_acc', 0) + insns
        per_ms = max(1, self.m.systick_insn_per_ms)
        while self._acc >= per_ms:
            self._acc -= per_ms
            csr = self.rd32(0xE000E010)
            if csr & 1:                                # SysTick enabled
                self.systick['enabled'] = True
                self.wr32(0xE000E010, csr | 0x10000)    # COUNTFLAG sticky
                self.systick_countflag = True
                if (csr & 2) and not self.m.exc_active:  # TICKINT
                    self.raise_irq(-1)
            self._timers_tick(1)
            self.device.tick(1.0)

    def _timers_tick(self, ms):
        """TIMx timebase approximated from the instruction budget.
        One timer clock is ~systick_insn_per_ms/48000 instructions (48 MHz)."""
        for name, irq in TIM_IRQ.items():
            base = [b for b, n in PERIPH_NAMES.items() if n == name][0]
            cr1 = self.rd32(base)
            if not (cr1 & 1):
                continue
            arr = self.rd32(base + 0x2C) & 0xFFFF
            psc = self.rd32(base + 0x28) & 0xFFFF
            if arr == 0:
                continue
            period = max(4, int((psc + 1) * (arr + 1) * self.m.systick_insn_per_ms / 48000.0))
            st = self.timers.setdefault(name, {'acc': 0, 'period': period})
            st['acc'] += max(1, self.m.systick_insn_per_ms // 1000) * ms * 1000
            st['period'] = period
            fired = 0
            while st['acc'] >= period and fired < 8:
                st['acc'] -= period
                fired += 1
                sr = self.rd32(base + 0x10)
                self.wr32(base + 0x10, sr | 1)
                dier = self.rd32(base + 0x0C)
                if dier & 1:
                    self.raise_irq(irq)
            self.wr32(base + 0x24, (self.rd32(base + 0x24) + fired) & 0xFFFF)

    # -------------------------------------------------------------- IRQ
    def raise_irq(self, exc):
        """exc: -1 = SysTick, 0..31 = IRQn"""
        if exc == -1:
            self.m.systick_pending = True
        else:
            self.irq_pending[exc] = True

    def irq_enabled(self, irq):
        return bool(self.nvic_enabled & (1 << irq)) if irq < 32 else False

    def gpio_input_changed(self, port, pin, level):
        """device pin toggled: run EXTI logic and update IDR"""
        base = [b for b, p in GPIO_PORTS.items() if p == port][0]
        self._refresh_idr(port, base)
        # EXTI line == pin number; check EXTICR select
        exti = 0x40010400
        line = pin
        cfg_idx = line // 4
        shift = (line % 4) * 4
        port_idx = {'A': 0, 'B': 1, 'C': 2, 'D': 3, 'E': 4, 'F': 5}[port]
        ext_sel = self.rd32(0x40010008 + cfg_idx * 4)
        if ((ext_sel >> shift) & 0xF) != port_idx:
            return
        imr = self.rd32(exti)
        rtsr = self.rd32(exti + 8)
        ftsr = self.rd32(exti + 0xC)
        trigger = (level == 1 and (rtsr & (1 << line))) or (level == 0 and (ftsr & (1 << line)))
        if not trigger:
            return
        self.wr32(exti + 0x14, 1 << line)          # PR
        if imr & (1 << line):
            self.raise_irq(EXTI_LINE_IRQ.get(line, 7))
            self.trace.add('exception', name='EXTI%d' % line, data={'level': level})

    def peripheral_state(self):
        return {
            'systick': dict(self.systick),
            'systick_countflag': self.systick_countflag,
            'irq_pending': [i for i, p in enumerate(self.irq_pending) if p],
            'irq_enabled': [i for i in range(32) if self.irq_enabled(i)],
            'unimplemented': sorted(self.unimplemented)[:32],
        }
