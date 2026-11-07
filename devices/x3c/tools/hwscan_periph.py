#!/usr/bin/env python
"""hwscan_periph.py - precise MMIO literal attribution (only literals with real ldr xrefs)."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import build_cached, APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from collections import defaultdict

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

NAMES = {
    0x40020000: 'GPIOA', 0x40020400: 'GPIOB', 0x40020800: 'GPIOC', 0x40020C00: 'GPIOD',
    0x40021000: 'GPIOE', 0x40021400: 'GPIOF', 0x40021800: 'GPIOG', 0x40021C00: 'GPIOH',
    0x40023800: 'CRM/RCC', 0x40023C00: 'FLASH', 0x40002800: 'ERTC/RTC', 0x40003000: 'IWDG',
    0x40007000: 'PWR', 0x40013C00: 'EXINT/EXTI', 0x40013000: 'SPI1', 0x40003800: 'SPI2',
    0x40003C00: 'SPI3', 0x40012000: 'ADC1', 0x40023000: 'CRC', 0x40026000: 'DMA1',
    0x40026400: 'DMA2', 0x40005400: 'I2C1', 0x40005800: 'I2C2', 0x40005C00: 'I2C3',
    0x40011000: 'USART1', 0x40004400: 'USART2', 0x40004800: 'USART3',
    0x40010000: 'TMR1', 0x40000000: 'TMR2', 0x40000400: 'TMR3', 0x40000800: 'TMR4',
    0x40000C00: 'TMR5', 0x40001000: 'TMR6', 0x40001400: 'TMR7', 0x40010400: 'TMR8',
    0x40014000: 'TMR9', 0x40014400: 'TMR10', 0x40014800: 'TMR11',
    0x50000000: 'USB_OTG_FS', 0x50050000: 'DVP', 0x40012C00: 'SDIO1',
    0xA0000000: 'XMC/FMC', 0xE000E000: 'SCS', 0xE000ED00: 'SCB',
    0x40015800: 'SAI1', 0x40013800: 'SYSCFG', 0x40017800: 'TMR20',
    0x40017400: 'TMR?', 0x40022000: 'FLASH_IF',
}
def annotate(w):
    best = None
    for base, name in NAMES.items():
        if base <= w < base + 0x400:
            if best is None or base > best[0]:
                best = (base, name)
    return best

def function_of(va, funcs_starts):
    import bisect
    i = bisect.bisect_right(funcs_starts, va) - 1
    return funcs_starts[i] if i >= 0 else None

def main():
    sp, vecs, vec_targets, funcs = build_cached()
    starts = sorted(funcs.keys())
    mmio = defaultdict(list)
    for off in range(0, N-3, 4):
        w = struct.unpack_from('<I', b, off)[0]
        if not (0x40000000 <= w < 0x40080000 or 0x50000000 <= w < 0x50080000 or
                0xA0000000 <= w < 0xA0100000 or 0xE0000000 <= w < 0xE0100000):
            continue
        sites = []
        va_storage = APP_BASE + off
        for back in range(2, 0x400, 2):
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
        if not sites:
            continue
        ann = annotate(w)
        key = ann[1] if ann else f'{w:#010x}'
        mmio[key].append((off, w, ann[1] if ann else '', sites))
    print(f"=== peripherals with genuinely referenced literals")
    for key in sorted(mmio, key=lambda k: -sum(len(x[3]) for x in mmio[k])):
        rows = mmio[key]
        sites_all = [s for r in rows for s in r[3]]
        fs = sorted(set(function_of(s, starts) for s in sites_all))
        print(f"{key:<14} literals={len(rows):3d} read_sites={len(sites_all):3d} funcs={[hex(f) for f in fs[:12]]}")
    print()
    for key in ['SPI1', 'USB_OTG_FS', 'XMC/FMC', 'I2C1', 'I2C2', 'I2C3', 'GPIOA', 'GPIOB', 'GPIOC', 'GPIOD', 'GPIOE', 'ADC1', 'DMA1', 'DMA2', 'SPI2', 'SPI3', 'USART1']:
        if key not in mmio:
            continue
        print(f"--- {key}")
        for off, w, name, sites in sorted(mmio[key]):
            print(f"    {w:#010x} {name:<10} stored@{off:#08x} sites={[hex(s) for s in sites[:5]]}")

if __name__ == '__main__':
    main()
