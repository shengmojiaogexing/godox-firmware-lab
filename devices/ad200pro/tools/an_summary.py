#!/usr/bin/env python3
"""Condensed per-function summary with *constant values* (for naming).

Prints, for every recovered function:
  calls   : named callees
  wr      : memory/MMIO writes with a constant value when known  addr=val
  rd      : memory/MMIO offsets read
  cmp     : interesting immediate constants used in compares
  str     : string literals referenced
This is a read-only helper; nothing is written.

Usage: python tools/an_summary.py [addr ...]
"""
import os
import re
import sys
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import an_util as U

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_OP_IMM, CS_OP_REG, CS_OP_MEM
from capstone.arm import ARM_REG_PC
import capstone.arm as A

MD = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_MCLASS)
MD.detail = True
MD.skipdata = True

PERIPH = None


def reg_name(a):
    names = [
        (0x40021000, 'RCC', {0: 'CR', 4: 'CFGR', 8: 'CIR', 0xc: 'APB2RSTR', 0x10: 'APB1RSTR',
                             0x14: 'AHBENR', 0x18: 'APB2ENR', 0x1c: 'APB1ENR', 0x20: 'BDCR',
                             0x24: 'CSR', 0x28: 'AHBRSTR', 0x2c: 'CFGR2', 0x30: 'CFGR3', 0x34: 'CR2'}),
        (0x48000000, 'GPIOA', {0: 'MODER', 4: 'OTYPER', 8: 'OSPEEDR', 0xc: 'PUPDR', 0x10: 'IDR',
                               0x14: 'ODR', 0x18: 'BSRR', 0x1c: 'LCKR', 0x28: 'BRR'}),
        (0x48000400, 'GPIOB', {0: 'MODER', 4: 'OTYPER', 8: 'OSPEEDR', 0xc: 'PUPDR', 0x10: 'IDR',
                               0x14: 'ODR', 0x18: 'BSRR', 0x1c: 'LCKR', 0x28: 'BRR'}),
        (0x48000800, 'GPIOC', {0: 'MODER', 4: 'OTYPER', 8: 'OSPEEDR', 0xc: 'PUPDR', 0x10: 'IDR',
                               0x14: 'ODR', 0x18: 'BSRR', 0x1c: 'LCKR', 0x28: 'BRR'}),
        (0x48000c00, 'GPIOD', {0: 'MODER', 4: 'OTYPER', 8: 'OSPEEDR', 0xc: 'PUPDR', 0x10: 'IDR',
                               0x14: 'ODR', 0x18: 'BSRR', 0x1c: 'LCKR', 0x28: 'BRR'}),
        (0x48001400, 'GPIOF', {0: 'MODER', 4: 'OTYPER', 8: 'OSPEEDR', 0xc: 'PUPDR', 0x10: 'IDR',
                               0x14: 'ODR', 0x18: 'BSRR', 0x1c: 'LCKR', 0x28: 'BRR'}),
        (0x40010400, 'EXTI', {0: 'IMR', 4: 'EMR', 8: 'RTSR', 0xc: 'FTSR', 0x10: 'SWIER', 0x14: 'PR'}),
        (0x40010000, 'SYSCFG', {0: 'CFGR1', 8: 'EXTICR1', 0xc: 'EXTICR2', 0x10: 'EXTICR3', 0x14: 'EXTICR4'}),
        (0x40022000, 'FLASH', {0: 'ACR', 4: 'KEYR', 0xc: 'SR', 0x10: 'CR', 0x14: 'AR'}),
        (0x40007000, 'PWR', {0: 'CR', 4: 'CSR'}),
        (0x40003000, 'IWDG', {0: 'KR', 4: 'PR', 8: 'RLR', 0xc: 'SR'}),
        (0x40002800, 'RTC', {0: 'TR', 4: 'DR', 8: 'CR', 0xc: 'ISR'}),
        (0x40020000, 'DMA1', {0: 'ISR', 4: 'IFCR'}),
        (0x40012400, 'ADC', {0: 'ISR', 4: 'IER', 8: 'CR', 0xc: 'CFGR1', 0x10: 'CFGR2', 0x14: 'SMPR',
                             0x28: 'TR', 0x30: 'CHSELR', 0x50: 'DR', 0x54: 'CCR'}),
        (0xe000e000, 'SCS', {0: 'ICTR', 4: 'ICSR', 8: 'VTOR', 0xc: 'AIRCR', 0x10: 'SCR', 0x14: 'CCR',
                             0x18: 'SHPR2', 0x1c: 'SHPR3', 0x100: 'ISER', 0x180: 'ICER',
                             0x200: 'ISPR', 0x280: 'ICPR', 0x400: 'IPR'}),
        (0xe000e010, 'SYSTICK', {0: 'CSR', 4: 'RVR', 8: 'CVR', 0xc: 'CALIB'}),
    ]
    for base, port, regs in names:
        if base <= a < base + 0x400:
            off = a - base
            if port.startswith('GPIO'):
                return '%s_%s' % (port, regs.get(off, '+0x%x' % off))
            return '%s_%s' % (port, regs.get(off, '+0x%x' % off))
    for base, nm in ((0x40000000, 'TIM2'), (0x40000400, 'TIM3'), (0x40002000, 'TIM14'),
                     (0x40014800, 'TIM17'), (0x40012c00, 'TIM1'), (0x40014000, 'TIM15'),
                     (0x40014400, 'TIM16'), (0x40001000, 'TIM6'), (0x40001400, 'TIM7')):
        if base <= a < base + 0x400:
            off = a - base
            rn = {0: 'CR1', 4: 'CR2', 0xc: 'DIER', 0x10: 'SR', 0x14: 'EGR', 0x24: 'CNT',
                  0x28: 'PSC', 0x2c: 'ARR', 0x34: 'CCR1'}.get(off, '+0x%x' % off)
            return '%s_%s' % (nm, rn)
    if 0x40020008 <= a < 0x40020200:
        ch = 1 + (a - 0x40020008) // 0x14
        r = (a - 0x40020008) % 0x14
        return 'DMA1_CH%d_%s' % (ch, {0: 'CCR', 4: 'CNDTR', 8: 'CPAR', 0xc: 'CMAR'}.get(r, '+0x%x' % r))
    return None


def lit_for(fn):
    out = {}
    for a, raw, text, notes in U.DIS.get(fn, []):
        for n in notes:
            m = re.match(r'0x([0-9a-f]{8}) = 0x([0-9a-f]{8})', n)
            if m:
                out[a] = int(m.group(2), 16)
    return out


def scan(fn):
    lits = lit_for(fn)
    regs = {}
    calls = []
    wr = []
    rd = []
    cmps = set()
    for a, raw, text, notes in U.DIS.get(fn, []):
        off = a - U.BASE
        ins = None
        for i in MD.disasm(U.DATA[off:off + max(2, len(raw) // 2)], a):
            ins = i
            break
        if ins is None:
            continue
        ops = ins.operands
        mn = ins.mnemonic
        # constants
        if mn in ('ldr',) and len(ops) == 2 and ops[1].type == CS_OP_MEM and ops[1].mem.base == ARM_REG_PC:
            regs[ops[0].reg] = ('c', lits.get(ins.address))
        elif mn in ('movs', 'mov') and len(ops) == 2 and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_IMM:
            regs[ops[0].reg] = ('c', ops[1].imm)
        elif mn in ('adds', 'add', 'subs', 'sub') and len(ops) == 3 and ops[1].type == CS_OP_REG:
            b = regs.get(ops[1].reg)
            if b and b[0] == 'c' and ops[2].type == CS_OP_IMM:
                regs[ops[0].reg] = ('c', b[1] + (ops[2].imm if mn.startswith('add') else -ops[2].imm))
        elif mn in ('lsls', 'lsrs') and len(ops) == 3 and ops[1].type == CS_OP_REG:
            b = regs.get(ops[1].reg)
            if b and b[0] == 'c' and ops[2].type == CS_OP_IMM:
                v = b[1] << ops[2].imm if mn == 'lsls' else b[1] >> ops[2].imm
                regs[ops[0].reg] = ('c', v & 0xFFFFFFFF)
        elif mn == 'mov' and len(ops) == 2 and ops[1].type == CS_OP_REG and ops[1].reg in regs:
            regs[ops[0].reg] = regs[ops[1].reg]
        # cmp immediates
        if mn.startswith('cmp') and len(ops) == 2 and ops[1].type == CS_OP_IMM:
            cmps.add(ops[1].imm)
        # calls
        if mn in ('bl', 'blx') and ops and ops[0].type == CS_OP_IMM:
            calls.append(ops[0].imm & ~1)
        # memory access
        if len(ops) >= 2 and ops[1].type == CS_OP_MEM and ops[1].mem.base != ARM_REG_PC:
            m = ops[1].mem
            b = regs.get(m.base)
            if b and b[0] == 'c' and b[1] is not None:
                tgt = b[1] + m.disp
                sz = 1 if ('ldrb' in mn or 'strb' in mn) else 2 if ('ldrh' in mn or 'strh' in mn) else 4
                if mn.startswith('str'):
                    val = None
                    if ops[0].type == CS_OP_REG and ops[0].reg in regs:
                        val = regs[ops[0].reg][1]
                    elif ops[0].type == CS_OP_IMM:
                        val = ops[0].imm
                    ang = regs.get(m.index) if m.index else None
                    wr.append((tgt, val, sz, a, ang[1] if ang else None))
                elif mn.startswith('ldr'):
                    ang = regs.get(m.index) if m.index else None
                    rd.append((tgt, sz, a, ang[1] if ang else None))
        # invalidate clobbered regs
        if ops and ops[0].type == CS_OP_REG and ops[0].reg in regs:
            if mn.startswith('ldr') or mn in ('lsls', 'lsrs', 'asrs', 'ands', 'orrs', 'eors', 'muls', 'uxth', 'uxtb', 'adds', 'subs', 'add', 'sub'):
                if not (mn in ('adds', 'add', 'subs', 'sub', 'lsls', 'lsrs') and len(ops) == 3):
                    regs.pop(ops[0].reg, None)
    return calls, wr, rd, cmps


NAMES = {}


def nm(a):
    return NAMES.get(a) or 'FUNC_%08x' % a


def main():
    try:
        import an_names
        NAMES.update(an_names.NAMES)
    except Exception:
        pass
    targets = [int(x, 0) for x in sys.argv[1:]] or sorted(U.FUNCS)
    for fn in targets:
        rec = U.FUNCS.get(fn)
        if not rec:
            print('0x%08x  (not a function)' % fn)
            continue
        calls, wr, rd, cmps = scan(fn)
        print('\n0x%08x %s  %d ins' % (fn, nm(fn), rec['insn_count']))
        if rec['strings']:
            print('   str: %r' % rec['strings'][:6])
        print('   calls: %s' % ' '.join(nm(c) for c in calls))
        wrs = []
        for tgt, val, sz, a, idx in wr:
            rn = reg_name(tgt)
            tag = rn or ('0x%08x' % tgt)
            if idx is not None:
                tag += '[idx=%d]' % idx
            if val is not None:
                wrs.append('%s=%s' % (tag, ('0x%x' % val) if val > 0xff else str(val)))
            else:
                wrs.append('%s=?' % tag)
        if wrs:
            print('   wr: %s' % ' '.join(wrs[:26]))
        rds = []
        for tgt, sz, a, idx in rd:
            rn = reg_name(tgt)
            tag = rn or ('0x%08x' % tgt)
            if idx is not None:
                tag += '[idx=%d]' % idx
            if tag not in rds:
                rds.append(tag)
        if rds:
            print('   rd: %s' % ' '.join(rds[:26]))
        if cmps:
            print('   cmp: %s' % ' '.join(hex(c) for c in sorted(cmps)[:20]))


if __name__ == '__main__':
    main()
