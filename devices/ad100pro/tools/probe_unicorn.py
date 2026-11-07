#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""probe unicorn MCLASS support, take 4."""
import unicorn
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_ARM, UC_MODE_MCLASS, UC_MODE_THUMB, UcError
from unicorn.arm_const import *

CODE_ADDR = 0x08003000
code = bytes.fromhex('0120' '0230' 'fee7')  # movs r0,#1; adds r0,#2; b .
print('XPSR reg id', UC_ARM_REG_XPSR, 'CPSR', UC_ARM_REG_CPSR)


def trial(mode, setup):
    uc = Uc(UC_ARCH_ARM, mode)
    uc.mem_map(CODE_ADDR, 0x1000)
    uc.mem_write(CODE_ADDR, code)
    uc.mem_map(0x20000000, 0x1000)
    uc.reg_write(UC_ARM_REG_SP, 0x20000800)
    setup(uc)
    uc.reg_write(UC_ARM_REG_PC, CODE_ADDR | 1)
    try:
        uc.emu_start(CODE_ADDR | 1, 0, count=2)
        ok = 'ok'
    except UcError as e:
        ok = f'err {e}'
    return ok, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_R0), uc.reg_read(UC_ARM_REG_XPSR)


cases = [
    ('MCLASS xpsr=0x01000000', UC_MODE_MCLASS, lambda uc: uc.reg_write(UC_ARM_REG_XPSR, 0x01000000)),
    ('MCLASS xpsr=0', UC_MODE_MCLASS, lambda uc: None),
    ('THUMB|ARM xpsr T', UC_MODE_ARM | UC_MODE_THUMB, lambda uc: uc.reg_write(UC_ARM_REG_XPSR, 0x01000000)),
    ('THUMB|ARM cpsr=0x01000013', UC_MODE_ARM | UC_MODE_THUMB, lambda uc: uc.reg_write(UC_ARM_REG_CPSR, 0x01000013)),
]
for name, mode, fn in cases:
    try:
        ok, pc, r0, xpsr = trial(mode, fn)
        print(f'{name:28s} -> {ok:22s} pc=0x{pc:08X} r0={r0} xpsr=0x{xpsr:08X}')
    except Exception as e:
        print(f'{name:28s} -> EXC {type(e).__name__}: {e}')
