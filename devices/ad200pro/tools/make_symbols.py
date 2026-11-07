#!/usr/bin/env python3
"""Build analysis/symbols.json (addr -> name) from functions.json + vector table."""
import json, os, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AN = os.path.join(ROOT, 'analysis')
BASE = 0x08003000

names = {
    0x080030C0: 'startup_c',
    0x080030C8: 'memclr_or_copy',
    0x080030D4: 'reset_handler',
    0x080030F0: 'udivmod_helper',
    0x0800BD0C: 'SystemInit',
    0x08003548: 'scatter_load',
    0x0800CDB0: 'main',
    0x08006E9E: 'GPIO_WritePin',
    0x08006D3C: 'EXTI_Config',
    0x0800356C: 'ADC_Init',
    0x08005544: 'GPIO_InitAll',
    0x080070C0: 'NVIC_DisableIRQ',
    0x080070D4: 'NVIC_EnableIRQ',
    0x080070E8: 'NVIC_SetPriority0',
    0x08009730: 'DMA1_Init',
    0x08009758: 'Board_InitInputs',
    0x0800B81C: 'SVCall_Handler',
    0x0800A90C: 'PendSV_Handler',
    0x0800BC8C: 'SysTick_Handler',
    0x0800A25C: 'NMI_Handler',
    0x08008200: 'HardFault_Handler',
}
# STM32F0 IRQ names in vector order
IRQ = ['WWDG','PVD','RTC','FLASH','RCC','EXTI0_1','EXTI2_3','EXTI4_15','TSC',
       'DMA1_Ch1','DMA1_Ch2_3','DMA1_Ch4_7','ADC','TIM1_BRK_UP_TRG_COM','TIM1_CC',
       'TIM2','TIM3','TIM6','TIM7','TIM14','TIM15','TIM16','TIM17','I2C1','I2C2',
       'SPI1','SPI2','USART1','USART2','USART3','CEC','USB']

data = open(os.path.join(ROOT, 'firmware', 'AD200pro_V1.60.bin'), 'rb').read()
vec = struct.unpack_from('<48I', data, 0)
vecs = []
for i, v in enumerate(vec):
    if i == 0:
        vecs.append({'idx': 0, 'name': 'initial_sp', 'value': v, 'target': None})
    elif i == 1:
        vecs.append({'idx': 1, 'name': 'Reset', 'value': v, 'target': v & ~1})
    elif i == 2:
        vecs.append({'idx': 2, 'name': 'NMI', 'value': v, 'target': v & ~1})
    elif i == 3:
        vecs.append({'idx': 3, 'name': 'HardFault', 'value': v, 'target': v & ~1})
    elif 4 <= i <= 10:
        vecs.append({'idx': i, 'name': 'reserved%d' % i, 'value': v, 'target': None})
    elif i == 11:
        vecs.append({'idx': i, 'name': 'SVCall', 'value': v, 'target': v & ~1})
    elif i in (12, 13):
        vecs.append({'idx': i, 'name': 'reserved%d' % i, 'value': v, 'target': None})
    elif i == 14:
        vecs.append({'idx': i, 'name': 'PendSV', 'value': v, 'target': v & ~1})
    elif i == 15:
        vecs.append({'idx': i, 'name': 'SysTick', 'value': v, 'target': v & ~1})
    else:
        irq = i - 16
        vecs.append({'idx': i, 'name': 'IRQ%d_%s' % (irq, IRQ[irq] if irq < len(IRQ) else '?'),
                     'value': v, 'target': (v & ~1) if v else None})

funcs = json.load(open(os.path.join(AN, 'functions.json'), encoding='utf-8'))
for f in funcs:
    a = f['start']
    if a in names:
        continue
    names[a] = 'FUNC_%08x' % a

out = {
    'image_base': BASE,
    'image_size': len(data),
    'reset': {'sp': vec[0], 'pc': vec[1] & ~1},
    'vectors': vecs,
    'symbols': {('%08x' % a): n for a, n in sorted(names.items())},
    'functions': funcs,
}
json.dump(out, open(os.path.join(AN, 'symbols.json'), 'w', encoding='utf-8'), indent=1)
print('wrote symbols.json:', len(names), 'symbols', len(vecs), 'vectors')
