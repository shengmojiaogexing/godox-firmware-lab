#!/usr/bin/env python
"""hwscan_sysinit.py - disassemble SystemInit & other startup with resolved literals + MMIO annotation"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

APP_BASE = 0x08004800
b = load()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

PERIPH = {
    0x40023800: 'RCC', 0x40023804: 'RCC_PLLCFGR', 0x40023808: 'RCC_CFGR', 0x4002380C: 'RCC_CIR',
    0x40023810: 'RCC_AHB1RSTR', 0x40023814: 'RCC_AHB2RSTR', 0x40023818: 'RCC_AHB3RSTR',
    0x40023820: 'RCC_APB1RSTR', 0x40023824: 'RCC_APB2RSTR',
    0x40023830: 'RCC_AHB1ENR', 0x40023834: 'RCC_AHB2ENR', 0x40023838: 'RCC_AHB3ENR',
    0x40023840: 'RCC_APB1ENR', 0x40023844: 'RCC_APB2ENR',
    0x40023850: 'RCC_AHB1LPENR', 0x40023854: 'RCC_AHB2LPENR', 0x40023858: 'RCC_AHB3LPENR',
    0x40023860: 'RCC_APB1LPENR', 0x40023864: 'RCC_APB2LPENR',
    0x40023870: 'RCC_BDCR', 0x40023874: 'RCC_CSR', 0x40023880: 'RCC_SSCGR',
    0x40023884: 'RCC_PLLI2SCFGR', 0x40023888: 'RCC_PLLSAICFGR', 0x4002388C: 'RCC_DCKCFGR',
    0x40023890: 'RCC_DCKCFGR2', 0x400238A0: 'RCC_?A0', 0x400238A4: 'RCC_?A4',
    0x40023C00: 'FLASH', 0x40023C60: 'FLASH_?60',
    0x40007000: 'PWR', 0x40002800: 'RTC', 0x40003000: 'IWDG', 0x40013C00: 'EXTI',
    0x40013000: 'SPI1', 0x40020000: 'GPIOA', 0x40020400: 'GPIOB', 0x40020800: 'GPIOC',
    0x40020C00: 'GPIOD', 0x40021000: 'GPIOE', 0x40012000: 'ADC1', 0x40023000: 'CRC',
    0x40026400: 'DMA2', 0x40026600: 'DMA2+0x200',
    0xE000ED08: 'SCB_VTOR', 0xE000ED0C: 'SCB_AIRCR', 0xE000E010: 'SysTick',
}

def word(va):
    o = va - APP_BASE
    if 0 <= o <= len(b)-4:
        return struct.unpack_from('<I', b, o)[0]
    return None

def show(start, n=120, label=''):
    print(f'===== {label} vaddr {start:#010x} (file {start-APP_BASE:#08x})')
    code = b[start-APP_BASE: start-APP_BASE + n*4]
    for ins in md.disasm(code, start):
        ann = ''
        if ins.mnemonic.startswith('ldr') and '[pc' in ins.op_str:
            try:
                disp = int(ins.op_str.split('#')[1].split(']')[0], 0)
            except Exception:
                disp = 0
            tgt = (ins.address + 4) & ~3
            tgt += disp
            w = word(tgt)
            ann = f'   ; [{tgt:#010x}] = {w:#010x}'
            if w in PERIPH:
                ann += f'  <{PERIPH[w]}>'
        print(f'{ins.address:010x}: {ins.bytes.hex():<10} {ins.mnemonic:<8} {ins.op_str}{ann}')

if __name__ == '__main__':
    show(0x0801666C, 70, 'SystemInit?')
    show(0x08004AF8, 20, 'Reset_Handler')
    show(0x08004A0C, 16, '__main?')
