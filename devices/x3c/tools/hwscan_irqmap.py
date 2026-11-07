#!/usr/bin/env python
"""hwscan_irqmap.py - classify each IRQ handler using literal pool MMIO addresses."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_funcs import build_cached, classify, APP_BASE, b, N
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
import pickle

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def dis(start, n=200):
    out = []
    code = b[start-APP_BASE: start-APP_BASE+n*4]
    for ins in md.disasm(code, start):
        out.append((ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
    return out

def read_word(va):
    o = va - APP_BASE
    if 0 <= o <= N-4:
        return struct.unpack_from('<I', b, o)[0]
    return None

def lits_of(insns):
    res = []
    for addr, by, mn, ops in insns:
        if '[pc' in ops and mn.startswith('ldr'):
            try:
                disp = int(ops.split('#')[1].split(']')[0], 0)
            except Exception:
                disp = 0
            tgt = (addr + 4) & ~3
            tgt += disp
            w = read_word(tgt)
            if w is not None:
                res.append((addr, tgt, w))
    return res

PERIPH_NAMES = {
    0x40023800: 'RCC', 0x40023804: 'RCC_PLLCFGR', 0x40023808: 'RCC_CFGR', 0x4002380C: 'RCC_CIR',
    0x40023830: 'RCC_AHB1ENR', 0x40023840: 'RCC_APB1ENR', 0x40023844: 'RCC_APB2ENR',
    0x40023870: 'RCC_BDCR', 0x40023874: 'RCC_CSR', 0x400238A0: 'RCC_unusual_A0', 0x400238A4: 'RCC_unusual_A4',
    0x40023C00: 'FLASH', 0x40023C60: 'FLASH_unusual_60',
    0x40002800: 'RTC', 0x40002804: 'RTC_?', 0x40002808: 'RTC_?', 0x4000280C: 'RTC_?', 0x40002810: 'RTC_?',
    0x40007000: 'PWR', 0x40003000: 'IWDG', 0x40013C00: 'EXTI', 0x40013C14: 'EXTI_PR',
    0x40013000: 'SPI1', 0x40003800: 'SPI2', 0x40003C00: 'SPI3',
    0x40020000: 'GPIOA', 0x40020400: 'GPIOB', 0x40020800: 'GPIOC', 0x40020C00: 'GPIOD', 0x40021000: 'GPIOE',
    0x40021400: 'GPIOF', 0x40021800: 'GPIOG', 0x40012000: 'ADC1', 0x40012004: 'ADC_?', 0x40012008: 'ADC_CR',
    0x40023000: 'CRC', 0x40026400: 'DMA2', 0x40026600: 'DMA2_unusual_200',
    0x40026000: 'DMA1', 0x40005C00: 'I2C1', 0x40005800: 'I2C2', 0x40005400: 'I2C3',
    0x40004400: 'USART2', 0x40011000: 'USART1', 0x40004800: 'USART3',
    0x40000800: 'TIM3', 0x40000C00: 'TIM5', 0x40001000: 'TIM6', 0x40001400: 'TIM7',
    0x40010000: 'TIM1', 0x40010400: 'TIM8', 0x40012800: 'TIM?',
    0x40015C00: 'SAI1?', 0x50000000: 'USB_OTG_FS', 0x50050000: 'DCMI', 0x40012C00: 'SDIO',
    0xA0000000: 'FMC', 0x40023C00+0: 'FLASH',
    0x40023800: 'RCC',
}
# add register offsets heuristics
def annotate(w):
    if w in PERIPH_NAMES:
        return PERIPH_NAMES[w]
    base = w & ~0x3FF
    if 0x40020000 <= w < 0x40022000:
        gp = {0x40020000: 'A', 0x40020400: 'B', 0x40020800: 'C', 0x40020C00: 'D', 0x40021000: 'E', 0x40021400: 'F', 0x40021800: 'G'}.get(base, '?')
        return f'GPIO{gp}+{w-base:#x}'
    if 0x40023800 <= w < 0x40023C00:
        return f'RCC+{w-0x40023800:#x}'
    if 0x40023C00 <= w < 0x40024000:
        return f'FLASH+{w-0x40023C00:#x}'
    if 0x40002800 <= w < 0x40002C00:
        return f'RTC+{w-0x40002800:#x}'
    if 0x40013000 <= w < 0x40013400:
        return f'SPI1+{w-0x40013000:#x}'
    if 0x40012000 <= w < 0x40012400:
        return f'ADC+{w-0x40012000:#x}'
    if 0x50000000 <= w < 0x50040000:
        return f'USB_OTG_FS+{w-0x50000000:#x}'
    if 0x50050000 <= w < 0x50050400:
        return f'DCMI+{w-0x50050000:#x}'
    if 0x40026400 <= w < 0x40026800:
        return f'DMA2+{w-0x40026400:#x}'
    return None

if __name__ == '__main__':
    sp, vecs, vec_targets, funcs = build_cached()
    # IRQn table guess: F4/F7-style
    print(f"{'IRQ':>3} {'vector':>10} {'vaddr':>10} {'file':>8}  literals(MMIO)   calls")
    for i, v in vec_targets:
        irq = i - 16
        if irq < 0:
            name = {1:'Reset',2:'NMI',3:'HardFault',4:'MemManage',5:'BusFault',6:'UsageFault',11:'SVC',12:'DebugMon',14:'PendSV',15:'SysTick'}.get(i, f'Exc{i}')
        else:
            name = f'IRQ{irq}'
        insns, calls, lits, end = funcs.get(v, ([], [], [], v))
        mm = []
        for at, addr, w in lits:
            k, x = classify(w)
            if k == 'MMIO':
                a = annotate(w) or f'{w:#010x}'
                mm.append(a)
        callfs = [t for at, t in calls if APP_BASE <= (t & ~1) < APP_BASE + N]
        print(f"{name:>8} vec{i+1:3d} {v:#010x} {v-APP_BASE:#08x}  {', '.join(mm)}   -> {[hex(c) for c in callfs[:6]]}")
