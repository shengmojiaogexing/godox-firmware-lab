#!/usr/bin/env python
"""hwscan_mmio.py - attribute MMIO literals to code sites, find big RAM buffers, dump peripheral map."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

APP_BASE = 0x08004800
b = load()
N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

PERIPH_BASE = {
    0x40020000: 'GPIOA', 0x40020400: 'GPIOB', 0x40020800: 'GPIOC', 0x40020C00: 'GPIOD',
    0x40021000: 'GPIOE', 0x40021400: 'GPIOF', 0x40021800: 'GPIOG', 0x40021C00: 'GPIOH',
    0x40023800: 'CRM/RCC', 0x40023C00: 'FLASH', 0x40002800: 'ERTC', 0x40003000: 'IWDG',
    0x40007000: 'PWR', 0x40013C00: 'EXINT/EXTI', 0x40013000: 'SPI1', 0x40003800: 'SPI2',
    0x40003C00: 'SPI3', 0x40012000: 'ADC1', 0x40023000: 'CRC', 0x40026000: 'DMA1',
    0x40026400: 'DMA2', 0x40005400: 'I2C1', 0x40005800: 'I2C2', 0x40005C00: 'I2C3',
    0x40011000: 'USART1', 0x40004400: 'USART2', 0x40004800: 'USART3',
    0x40010000: 'TMR1', 0x40000000: 'TMR2', 0x40000400: 'TMR3', 0x40000800: 'TMR4',
    0x40000C00: 'TMR5', 0x40001000: 'TMR6', 0x40001400: 'TMR7', 0x40010400: 'TMR8',
    0x40014000: 'TMR9', 0x40014400: 'TMR10', 0x40014800: 'TMR11',
    0x50000000: 'USB_OTG_FS', 0x50050000: 'DVP/DCMI', 0x40012C00: 'SDIO1',
    0xA0000000: 'XMC/FMC', 0xE000E000: 'SCS', 0xE000ED00: 'SCB',
    0x40015800: 'SAI1', 0x40015C00: 'SAI?', 0x40001000: 'TMR6',
    0x40013800: 'SYSCFG', 0x40023C00: 'FLASH_IF',
}
def annotate(w):
    if w in PERIPH_BASE:
        return PERIPH_BASE[w]
    # find best base match
    best = None
    for base, name in PERIPH_BASE.items():
        if base <= w < base + 0x400:
            if best is None or base > best[0]:
                best = (base, name)
    if best:
        return f'{best[1]}+{w-best[0]:#04x}'
    return None

def is_mmio(w):
    return (0x40000000 <= w < 0x40080000) or (0x50000000 <= w < 0x50080000) or \
           (0xA0000000 <= w < 0xA0100000) or (0xE0000000 <= w < 0xE0100000)

# 1) find all MMIO literals: stored aligned word + referenced by an ldr [pc,#imm] within 0x300 bytes before
mmios = {}
for off in range(0, N-3, 4):
    w = struct.unpack_from('<I', b, off)[0]
    if not is_mmio(w):
        continue
    # search backwards for ldr literal to this address
    sites = []
    va_storage = APP_BASE + off
    for back in range(2, 0x300, 2):
        so = off - back
        if so < 0:
            break
        va = APP_BASE + so
        ins = next(md.disasm(b[so:so+4], va), None)
        if ins is None:
            continue
        if ins.mnemonic.startswith('ldr') and '[pc' in ins.op_str:
            try:
                disp = int(ins.op_str.split('#')[1].split(']')[0], 0)
            except Exception:
                continue
            tgt = (va + 4) & ~3
            tgt += disp
            if tgt == va_storage:
                sites.append(va)
    mmios[off] = (w, sites)

print(f"=== {len(mmios)} MMIO literal words found")
from collections import defaultdict
byper = defaultdict(list)
for off, (w, sites) in mmios.items():
    ann = annotate(w) or f'{w:#010x}'
    base = ann.split('+')[0]
    byper[base].append((off, w, ann, sites))
for base in sorted(byper):
    rows = sorted(byper[base])
    print(f"\n--- {base}: {len(rows)} literals")
    for off, w, ann, sites in rows:
        # cluster sites into functions: nearest preceding push
        print(f"  {w:#010x} {ann:<14} stored@{off:#08x} used@{[hex(s) for s in sites[:4]]}")
