#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""periph.py -- build a peripheral access map from the AD100pro app image.

Image load base is 0x08003000 (48-entry vector table at file offset 0).
Outputs:
  out/periph_map.json   [{func, at, reg, rw, insn}]
  out/periph_summary.md grouped by peripheral/register
  out/functions.meta.json  per-function instruction count, callees, peripherals
"""
import os, json, re, struct, collections

import capstone
from capstone import arm_const

IMG_BASE = 0x08003000
FLASH_BASE = 0x08000000
FLASH_SIZE = 0x10000
RAM_BASE, RAM_SIZE = 0x20000000, 0x2000
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FW = os.path.join(ROOT, 'firmware', 'AD100pro_V1.2.bin')
OUT = os.path.join(ROOT, 'out')

PERIPH = {
    0x40000000: ('TIM2', 'TIM'), 0x40000400: ('TIM3', 'TIM'), 0x40001000: ('TIM6', 'TIM'),
    0x40001400: ('TIM7', 'TIM'), 0x40002000: ('TIM14', 'TIM'), 0x40002800: ('RTC', 'RTC'),
    0x40002C00: ('WWDG', 'WWDG'), 0x40003000: ('IWDG', 'IWDG'), 0x40003800: ('SPI2', 'SPI'),
    0x40004400: ('USART2', 'USART'), 0x40005400: ('I2C1', 'I2C'), 0x40005800: ('I2C2', 'I2C'),
    0x40007000: ('PWR', 'PWR'), 0x40010000: ('SYSCFG', 'SYSCFG'), 0x40010400: ('EXTI', 'EXTI'),
    0x40012400: ('ADC', 'ADC'), 0x40012C00: ('TIM1', 'TIM'), 0x40013000: ('SPI1', 'SPI'),
    0x40013800: ('USART1', 'USART'), 0x40014000: ('TIM15', 'TIM'), 0x40014400: ('TIM16', 'TIM'),
    0x40014800: ('TIM17', 'TIM'), 0x40020000: ('DMA1', 'DMA1'), 0x40021000: ('RCC', 'RCC'),
    0x40022000: ('FLASH_IF', 'FLASH'), 0x40023000: ('CRC', 'CRC'),
    0x48000000: ('GPIOA', 'GPIO'), 0x48000400: ('GPIOB', 'GPIO'), 0x48000800: ('GPIOC', 'GPIO'),
    0x48000C00: ('GPIOD', 'GPIO'), 0x48001000: ('GPIOE', 'GPIO'), 0x48001400: ('GPIOF', 'GPIO'),
    0xE000E000: ('SYS', 'SYS'), 0xE000E010: ('SYSTICK', 'SYSTICK'),
    0xE000E100: ('NVIC', 'NVIC'), 0xE000ED00: ('SCB', 'SCB'),
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
    'SYSCFG': {0x00: 'CFGR1', 0x04: 'CFGR2', 0x08: 'EXTICR1', 0x0C: 'EXTICR2',
               0x10: 'EXTICR3', 0x14: 'EXTICR4', 0x18: 'CFGR3'},
    'TIM': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'SMCR', 0x0C: 'DIER', 0x10: 'SR', 0x14: 'EGR',
            0x18: 'CCMR1', 0x1C: 'CCMR2', 0x20: 'CCER', 0x24: 'CNT', 0x28: 'PSC',
            0x2C: 'ARR', 0x30: 'RCR', 0x34: 'CCR1', 0x38: 'CCR2', 0x3C: 'CCR3',
            0x40: 'CCR4', 0x44: 'BDTR', 0x48: 'DCR', 0x4C: 'DMAR'},
    'ADC': {0x00: 'ISR', 0x04: 'IER', 0x08: 'CR', 0x0C: 'CFGR1', 0x10: 'CFGR2',
            0x14: 'SMPR', 0x40: 'DR', 0x44: 'CCR'},
    'USART': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'CR3', 0x0C: 'BRR', 0x10: 'GTPR',
              0x14: 'RTOR', 0x18: 'RQR', 0x1C: 'ISR', 0x20: 'ICR', 0x24: 'RDR', 0x28: 'TDR'},
    'SPI': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'SR', 0x0C: 'DR', 0x10: 'CRCPR',
            0x14: 'RXCRCR', 0x18: 'TXCRCR', 0x1C: 'I2SCFGR', 0x20: 'I2SPR'},
    'I2C': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'OAR1', 0x0C: 'OAR2', 0x10: 'TIMINGR',
            0x14: 'TIMEOUTR', 0x18: 'ISR', 0x1C: 'ICR', 0x20: 'PECR', 0x24: 'RXDR', 0x28: 'TXDR'},
    'RTC': {0x00: 'TR', 0x04: 'DR', 0x08: 'CR', 0x0C: 'ISR', 0x10: 'PRER', 0x14: 'WUTR',
            0x18: 'CALR', 0x1C: 'ALRMAR', 0x20: 'ALRMBR', 0x24: 'WPR', 0x28: 'SSR',
            0x2C: 'SHIFTR', 0x30: 'TSTR', 0x34: 'TSDR', 0x38: 'TSSSR', 0x40: 'TAFCR'},
    'PWR': {0x00: 'CR', 0x04: 'CSR'},
    'IWDG': {0x00: 'KR', 0x04: 'PR', 0x08: 'RLR', 0x0C: 'SR', 0x10: 'WINR'},
    'CRC': {0x00: 'DR', 0x04: 'IDR', 0x08: 'CR', 0x0C: 'INIT', 0x10: 'POL'},
    'WWDG': {0x00: 'CR', 0x04: 'CFR', 0x08: 'SR'},
    'SYSTICK': {0x00: 'CTRL', 0x04: 'LOAD', 0x08: 'VAL', 0x0C: 'CALIB'},
    'SCB': {0x00: 'CPUID', 0x04: 'ICSR', 0x08: 'VTOR', 0x0C: 'AIRCR', 0x10: 'SCR',
            0x14: 'CCR', 0x18: 'SHPR1', 0x1C: 'SHPR2', 0x20: 'SHPR3', 0x24: 'SHCSR',
            0x28: 'CFSR', 0x2C: 'HFSR'},
}


def reg_name(addr):
    best = None
    for b in PERIPH:
        if b <= addr < b + 0x400:
            if best is None or b > best:
                best = b
    if best is None:
        return None
    pname, group = PERIPH[best]
    off = addr - best
    if group == 'DMA1':
        ch, r = off // 0x1C, off % 0x1C
        nm = {0x00: 'ISR', 0x04: 'IFCR', 0x08: 'CCR', 0x0C: 'CNDTR',
              0x10: 'CPAR', 0x14: 'CMAR'}.get(r)
        return f'{pname}.{"ISR" if off < 4 else ("IFCR" if off < 8 else "CH%d_%s" % (ch + 1, nm))}'
    nm = REGS.get(group, {}).get(off)
    if nm is None:
        return f'{pname}.+0x{off:X}'
    return f'{pname}.{nm}'


md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB + capstone.CS_MODE_MCLASS)
md.detail = True


def main():
    data = open(FW, 'rb').read()
    n = len(data)
    # functions from functions.json (regenerate if missing)
    fpath = os.path.join(OUT, 'functions.json')
    funcs = json.load(open(fpath))
    entries = sorted(int(f['addr'], 16) for f in funcs)
    # decode everything with owner mapping
    owner = {}
    insns = {}
    for f in funcs:
        # decode linearly the claimed blocks via disasm.txt is easier; here: decode whole image once
        pass
    # Build a simple "instruction at addr" map by decoding linearly with resync over the image
    addr = IMG_BASE
    while addr < IMG_BASE + n:
        off = addr - IMG_BASE
        got = None
        for insn in md.disasm(data[off:off + 4], addr):
            got = insn
            break
        if got is None:
            addr += 2
            continue
        insns[addr] = got
        addr += got.size
    # owner: nearest preceding function entry
    import bisect
    for a in insns:
        i = bisect.bisect_right(entries, a) - 1
        owner[a] = entries[i] if i >= 0 else 0

    hits = []
    meta = collections.defaultdict(lambda: {'periph': set(), 'callees': set()})
    regs_lit = {}
    for a in sorted(insns):
        insn = insns[a]
        mn, ops = insn.mnemonic, insn.op_str
        fentry = owner[a]
        # track ldr rX, =literal
        if mn == 'ldr' and '[pc' in ops and len(insn.operands) == 2:
            op = insn.operands[1]
            if op.type == capstone.CS_OP_MEM and op.mem.base == arm_const.ARM_REG_PC:
                lit = ((a + 4) & ~3) + op.mem.disp
                lo = lit - IMG_BASE
                if 0 <= lo + 4 <= n:
                    v = struct.unpack_from('<I', data, lo)[0]
                    regs_lit[insn.operands[0].reg] = v
            continue
        if mn in ('bl', 'blx') and insn.operands and insn.operands[0].type == capstone.CS_OP_IMM:
            meta[fentry]['callees'].add(insn.operands[0].imm & ~1)
            continue
        # memory access with base register
        if mn.startswith(('ldr', 'ldrb', 'ldrh', 'ldrsb', 'ldrsh', 'str', 'strb', 'strh', 'ldm', 'stm')):
            for op in insn.operands:
                if op.type == capstone.CS_OP_MEM and op.mem.base != arm_const.ARM_REG_PC:
                    base = regs_lit.get(op.mem.base)
                    if base is not None:
                        rn = reg_name(base + op.mem.disp)
                        if rn:
                            rw = 'r' if mn.startswith('ldr') or mn.startswith('ldm') else 'w'
                            hits.append({'func': f'0x{fentry:08X}', 'at': f'0x{a:08X}',
                                         'reg': rn, 'rw': rw, 'insn': f'{mn} {ops}'})
                            meta[fentry]['periph'].add(rn + '/' + rw)
                    break
            # direct literal-based access (ldr r0,=PERIPH; then str [r0]) handled above
            continue

    # also direct: ldr/str [pc] to a peripheral address
    for a in sorted(insns):
        insn = insns[a]
        if not insn.mnemonic.startswith(('ldr', 'str')):
            continue
        if '[pc' in insn.op_str and len(insn.operands) == 1:
            pass
    os.makedirs(OUT, exist_ok=True)
    with open(os.path.join(OUT, 'periph_map.json'), 'w', encoding='utf-8') as f:
        json.dump(hits, f, indent=1)
    # summary
    byreg = collections.defaultdict(set)
    for h in hits:
        byreg[h['reg']].add(h['func'])
    lines = ['# 外设寄存器访问映射（由 tools/periph.py 生成）', '']
    lines.append(f'总访问点: {len(hits)}，涉及寄存器: {len(byreg)}')
    lines.append('')
    lines.append('| 寄存器 | 访问点数 | 涉及函数数 | 访问函数（前8个） |')
    lines.append('|---|---|---|---|')
    cnt = collections.Counter(h['reg'] for h in hits)
    for reg, c in cnt.most_common():
        fs = sorted(byreg[reg])
        lines.append(f'| `{reg}` | {c} | {len(fs)} | ' +
                     ', '.join(fs[:8]) + (' …' if len(fs) > 8 else '') + ' |')
    # meta
    meta_out = []
    for e in entries:
        m = meta.get(e, {'periph': set(), 'callees': set()})
        meta_out.append({'addr': f'0x{e:08X}', 'insns': sum(1 for x in insns if owner[x] == e),
                         'periph': sorted(m['periph']), 'callees': sorted(f'0x{c:08X}' for c in m['callees'])})
    with open(os.path.join(OUT, 'functions.meta.json'), 'w', encoding='utf-8') as f:
        json.dump(meta_out, f, indent=1)
    with open(os.path.join(OUT, 'periph_summary.md'), 'w', encoding='utf-8') as f:
        f.write('\n'.join(lines))
    print(f'peripheral access points: {len(hits)}  registers: {len(byreg)}')
    for reg, c in cnt.most_common(20):
        print(f'  {reg:24s} {c}')


if __name__ == '__main__':
    main()
