#!/usr/bin/env python3
"""Write analysis/symbols.json from functions.json + tools/an_names.py.

Output schema is a superset of tools/make_symbols.py:
  image_base, image_size, reset{sp,pc}, vectors[], symbols{hexaddr:name},
  functions[] (from functions.json), comments{hexaddr:evidence}, ram_vars,
  mmio_regs
so sim/firmware.py can keep consuming it unchanged.

Usage: python tools/an_symbols.py
"""
import json
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AN = os.path.join(ROOT, 'analysis')
BASE = 0x08003000

import an_names  # noqa: E402

NAMES = dict(an_names.NAMES)

IRQ = ['WWDG', 'PVD', 'RTC', 'FLASH', 'RCC', 'EXTI0_1', 'EXTI2_3', 'EXTI4_15', 'TSC',
       'DMA1_Ch1', 'DMA1_Ch2_3', 'DMA1_Ch4_7', 'ADC', 'TIM1_BRK_UP_TRG_COM', 'TIM1_CC',
       'TIM2', 'TIM3', 'TIM6', 'TIM7', 'TIM14', 'TIM15', 'TIM16', 'TIM17', 'I2C1', 'I2C2',
       'SPI1', 'SPI2', 'USART1', 'USART2', 'USART3', 'CEC', 'USB']

# RAM variables recovered by tools/an_dossier.py + manual reading
RAM_VARS = {
    0x20000100: 'u32 systick_ms / get_tick() value (incremented in SysTick_Handler)',
    0x20000104: 'u32 systick_reload (from RCC table 0x0800D00C)',
    0x20000108: 'u8 flag: EXTI/clock irq latch',
    0x20000109: 'u8 flag: TIM2 irq pending flag',
    0x20000122: 'u8[4] key debounce/stability counters',
    0x20000150: 'app/UI state struct (base)',
    0x2000015D: 'u8 mode/state byte (bits1:0 = power state, 1=normal)',
    0x20000162: 'u8 charge/dump request flag',
    0x20000165: 'u8 RF subsystem enabled',
    0x2000016B: 'u8 rf busy flag',
    0x2000016C: 'u8 RF channel / config value pending',
    0x20000198: 'u8 RF rx state',
    0x20000199: 'u8 RF rx byte counter',
    0x2000019B: 'u8 RF rx active flag',
    0x200001A1: 'u8 RF rx done flag',
    0x200001A4: 'u16 measured ADC value (raw)',
    0x200001B2: 'u16 computed flash pulse / charge target',
    0x200001B8: 'u16 trigger/retrigger timer',
    0x200001BF: 'u8 boot reason code',
    0x200001C2: 'u8 ISR-ownership guard (ISRs skip app work when !=0)',
    0x200001CE: 'u8 state struct (charging)',
    0x200001CF: 'u8 main loop wake flag (set by SysTick)',
    0x20000200: 'u8[8] RF status / rx buffer',
    0x2000020D: 'u8 RTC interrupt flag',
    0x20000214: 'FLASH driver state struct {.., cur_addr@+8}',
    0x20000220: 'FLASH erase result (out param)',
    0x20000228: 'FLASH state struct {lock/busy@+0x18, ...}',
    0x20000248: 'u32 low-power / sleep counter',
    0x2000025C: 'ADC device struct {base=0x40012400, ...}',
    0x200002E4: 'TIM device struct (base 0x40002000 = TIM14)',
    0x20000318: 'TIM device struct (IRQ13)',
    0x20000354: 'TIM device struct (IRQ15/TIM2)',
    0x200003CC: 'TIM device struct (IRQ19/TIM14)',
    0x20000418: 'u8[8] user settings mirror (brightness/mode/...)',
    0x20000464: 'u32[8] parameter block (flash page 0x0801F000)',
    0x200004A4: 'FLASH erase descriptor {type,start,pages}',
    0x200008B0: 'initial SP',
}

# MMIO register -> semantic (key ones used by the simulator)
MMIO_INFO = {
    'SYSCFG_CFGR1': 'MEM_MODE remap (writes 3 = SRAM vector table)',
    'FLASH_ACR': 'latency/prefetch',
    'FLASH_KEYR': 'unlock keys 0x45670123/0xCDEF89AB',
    'FLASH_SR': 'BSY(0)/PGERR(2)/WRPRTERR(4)/EOP(5)',
    'FLASH_CR': 'PG(0)/PER(1)/MER(2)/STRT(6)/LOCK(7)',
    'FLASH_AR': 'erase address',
    'RCC_CR': 'HSION/HSIRDY/HSEON/PLLON...',
    'RCC_CFGR': 'SW/SWS/HPRE/PPRE/PLLSRC/PLLMUL',
    'RCC_AHBENR': 'DMA1EN(0), GPIOA..F',
    'RCC_APB2ENR': 'SYSCFG(0), ADC(9), TIM17(18), TIM16(17), TIM15(16), TIM14(15), TIM1(11)',
    'RCC_APB1ENR': 'TIM2(0), TIM3(1), TIM6(4), TIM7(5), PWR(28)',
    'RCC_BDCR': 'LSEON/LSERDY/RTCSEL/RTCEN/BDCR',
    'RCC_CSR': 'LSION/LSIRDY/reset flags (PINRSTF bit26)',
    'EXTI_IMR': 'interrupt mask', 'EXTI_EMR': 'event mask',
    'EXTI_RTSR': 'rising trigger', 'EXTI_FTSR': 'falling trigger',
    'EXTI_PR': 'pending (write 1 to clear)',
    'ADC_CR': 'ADEN/ADSTART/ADSTP/ADCAL',
    'ADC_CFGR1': 'DMAEN/CONT/EXTEN/ALIGN/SCAN', 'ADC_CHSELR': 'channel select',
    'ADC_DR': 'conversion result',
    'DMA1_ISR': 'channel flags', 'DMA1_IFCR': 'clear flags',
    'GPIOx_MODER': '2 bits/pin', 'GPIOx_PUPDR': '2 bits/pin',
    'GPIOx_BSRR': 'set (low half) / reset (high half)',
    'GPIOx_BRR': 'reset bits', 'GPIOx_ODR': 'output data', 'GPIOx_IDR': 'input data',
    'SCS_ISER': 'NVIC enable', 'SCS_ICER': 'NVIC disable',
    'SCS_SHPR2': 'SVCall/PendSV priority', 'SCB_SCR': 'SLEEPONEXIT/SLEEPDEEP/SEVONPEND',
    'SYSTICK_CSR': 'ENABLE/TICKINT/COUNTFLAG', 'SYSTICK_RVR': 'reload',
}


def main():
    data = open(os.path.join(ROOT, 'firmware', 'AD200pro_V1.60.bin'), 'rb').read()
    vec = struct.unpack_from('<48I', data, 0)
    vecs = []
    for i, v in enumerate(vec):
        if i == 0:
            rec = {'idx': 0, 'name': 'initial_sp', 'value': v, 'target': None}
        elif i == 1:
            rec = {'idx': 1, 'name': 'Reset', 'value': v, 'target': v & ~1}
        elif i == 2:
            rec = {'idx': 2, 'name': 'NMI', 'value': v, 'target': v & ~1}
        elif i == 3:
            rec = {'idx': 3, 'name': 'HardFault', 'value': v, 'target': v & ~1}
        elif i == 11:
            rec = {'idx': 11, 'name': 'SVCall', 'value': v, 'target': v & ~1}
        elif i == 14:
            rec = {'idx': 14, 'name': 'PendSV', 'value': v, 'target': v & ~1}
        elif i == 15:
            rec = {'idx': 15, 'name': 'SysTick', 'value': v, 'target': v & ~1}
        elif i >= 16:
            irq = i - 16
            nm = 'IRQ%d_%s' % (irq, IRQ[irq] if irq < len(IRQ) else '?')
            rec = {'idx': i, 'name': nm, 'value': v, 'target': (v & ~1) if v else None}
        else:
            rec = {'idx': i, 'name': 'reserved%d' % i, 'value': v, 'target': None}
        if rec['target'] and rec['target'] in NAMES:
            rec['handler'] = NAMES[rec['target']]
        vecs.append(rec)

    funcs = json.load(open(os.path.join(AN, 'functions.json'), encoding='utf-8'))
    for f in funcs:
        a = f['start']
        if a not in NAMES:
            NAMES[a] = 'FUNC_%08x' % a

    out = {
        'image_base': BASE,
        'image_size': len(data),
        'reset': {'sp': vec[0], 'pc': vec[1] & ~1},
        'vectors': vecs,
        'symbols': {('%08x' % a): n for a, n in sorted(NAMES.items())},
        'functions': funcs,
        'ram_vars': {('%08x' % a): d for a, d in sorted(RAM_VARS.items())},
        'mmio_info': MMIO_INFO,
        'notes': {
            'flash_param_page': '0x0801F000 (2 KiB page, struct {type,start,pages})',
            'flash_flag_page': '0x0801F800 magic word 0x0000100E',
            'scatter_table': '0x0800E3E4, 16-byte records {src,dst,len,fn} x2',
            'data_region': '0x0800D000..0x0800E52C tables/strings',
            'rf_chip': 'CC2500-compatible 2.4G transceiver (see docs/04-subsystems.md)',
        },
    }
    j = os.path.join(AN, 'symbols.json')
    json.dump(out, open(j, 'w', encoding='utf-8'), indent=1)
    named = sum(1 for n in NAMES.values() if not n.startswith('FUNC_'))
    print('wrote %s: %d symbols (%d named, %d unnamed), %d vectors' % (
        j, len(NAMES), named, len(NAMES) - named, len(vecs)))


if __name__ == '__main__':
    main()
