#!/usr/bin/env python3
"""Recover every gpio_write_pin/gpio_read_pin/exti_config call site with its
constant arguments, i.e. the raw evidence for docs/05-pinmap.md.

The helper functions are:
  0x08006E9E  gpio_write_pin(port_base, pin_mask, level)   -> BSRR/BRR
  0x08006E94  gpio_read_pin (port_base, pin_mask) -> 0/1    (IDR)
  0x08006D3C  exti_config  (port_base, cfg*)                (cfg built on stack)

Only a light linear constant propagation is used (movs / ldr [pc] / lsls / adds),
which is enough because all call sites build the mask immediately before the call.

Usage: python tools/an_pins.py [addr ...]
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

PORTS = {
    0x48000000: 'GPIOA', 0x48000400: 'GPIOB', 0x48000800: 'GPIOC',
    0x48000c00: 'GPIOD', 0x48001000: 'GPIOE', 0x48001400: 'GPIOF',
}

WRITE_FN = 0x08006E9E
READ_FN = 0x08006E94
EXTI_FN = 0x08006D3C


def port_name(v):
    return PORTS.get(v)


def lit_notes(fn):
    out = {}
    for a, raw, text, notes in U.DIS.get(fn, []):
        for n in notes:
            m = re.match(r'0x([0-9a-f]{8}) = 0x([0-9a-f]{8})', n)
            if m:
                out[a] = int(m.group(2), 16)
    return out


def insn_at(a, raw):
    off = a - U.BASE
    for i in MD.disasm(U.DATA[off:off + max(2, len(raw) // 2)], a):
        return i
    return None


def bits(mask):
    return [i for i in range(32) if mask >> i & 1]


def scan():
    sites = []
    for fn in sorted(U.FUNCS):
        regs = {}
        lits = lit_notes(fn)
        for a, raw, text, notes in U.DIS.get(fn, []):
            ins = insn_at(a, raw)
            if ins is None:
                continue
            mn, ops = ins.mnemonic, ins.operands
            if mn == 'ldr' and len(ops) == 2 and ops[0].type == CS_OP_REG and \
                    ops[1].type == CS_OP_MEM and ops[1].mem.base == ARM_REG_PC:
                regs[ops[0].reg] = lits.get(a)
            elif mn in ('movs', 'mov') and len(ops) == 2 and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_IMM:
                regs[ops[0].reg] = ops[1].imm
            elif mn in ('movs', 'mov') and len(ops) == 2 and ops[0].type == CS_OP_REG and \
                    ops[1].type == CS_OP_REG and regs.get(ops[1].reg) is not None:
                regs[ops[0].reg] = regs[ops[1].reg]
            elif mn in ('adds', 'add') and len(ops) == 3 and ops[1].type == CS_OP_REG and ops[2].type == CS_OP_IMM:
                if regs.get(ops[1].reg) is not None:
                    regs[ops[0].reg] = regs[ops[1].reg] + ops[2].imm
            elif mn in ('subs', 'sub') and len(ops) == 3 and ops[1].type == CS_OP_REG and ops[2].type == CS_OP_IMM:
                if regs.get(ops[1].reg) is not None:
                    regs[ops[0].reg] = regs[ops[1].reg] - ops[2].imm
            elif mn in ('lsls', 'lsrs') and len(ops) == 3 and ops[1].type == CS_OP_REG and ops[2].type == CS_OP_IMM:
                v = regs.get(ops[1].reg)
                if v is not None:
                    regs[ops[0].reg] = (v << ops[2].imm) if mn == 'lsls' else (v >> ops[2].imm)
            elif mn in ('bl', 'blx'):
                t = ops[0].imm & ~1 if ops and ops[0].type == CS_OP_IMM else None
                if t in (WRITE_FN, READ_FN):
                    r0 = regs.get(A.ARM_REG_R0)
                    r1 = regs.get(A.ARM_REG_R1)
                    r2 = regs.get(A.ARM_REG_R2)
                    port = port_name(r0)
                    sites.append({
                        'func': fn, 'insn': a, 'kind': 'write' if t == WRITE_FN else 'read',
                        'port': port, 'r0': r0, 'mask': r1, 'level': r2,
                    })
                elif t == EXTI_FN:
                    r0 = regs.get(A.ARM_REG_R0)
                    sites.append({'func': fn, 'insn': a, 'kind': 'exti',
                                  'port': port_name(r0), 'r0': r0})
                regs = {}
            # clobber heuristic: any other write to a tracked register invalidates it
            elif ops and ops[0].type == CS_OP_REG and ops[0].reg in regs:
                regs.pop(ops[0].reg, None)
    return sites


def main():
    sites = scan()
    bypin = defaultdict(list)
    for s in sites:
        p = s['port']
        if not p:
            continue
        if s['kind'] in ('write', 'read') and s['mask'] is not None:
            for b in bits(s['mask']):
                bypin[(p, b)].append(s)
        elif s['kind'] == 'exti':
            bypin[(p, None)].append(s)
    print('# gpio_write_pin / gpio_read_pin call sites: %d' % len(sites))
    for (p, b) in sorted(bypin, key=lambda x: (x[0], -1 if x[1] is None else x[1])):
        if b is None:
            print('%s EXTI_config call sites: %s' % (
                p, ', '.join('%08x(%s)' % (s['insn'], U.NAMES_HINT.get(s['func'], 'FUNC_%08x' % s['func']))
                             for s in bypin[(p, b)])))
            continue
        print('%s%d:' % (p, b))
        for s in bypin[(p, b)]:
            print('    %s %s at %08x  mask=0x%x level=%s  (%s)' % (
                s['kind'], U.NAMES_HINT.get(s['func'], 'FUNC_%08x' % s['func']),
                s['insn'], s['mask'], s['level'], ''))


if __name__ == '__main__':
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    try:
        import an_names
        U.NAMES_HINT = an_names.NAMES
    except Exception:
        U.NAMES_HINT = {}
    main()
