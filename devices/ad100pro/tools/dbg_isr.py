#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Debug manual ISR injection: step through the handler and print the trace."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import boottest as bt  # noqa: E402
import capstone  # noqa: E402
from unicorn.arm_const import *  # noqa: E402

md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB + capstone.CS_MODE_MCLASS)
md.detail = False


def dis1(e, addr):
    for i in md.disasm(bytes(e.uc.mem_read(addr, 4)), addr):
        return i
    return None


def main():
    handler = int(sys.argv[1], 0) if len(sys.argv) > 1 else 0x08005A78
    setup = int(sys.argv[2], 0) if len(sys.argv) > 2 else 300000
    e = bt.Emu(None)
    e.run(setup)
    print(f'--- before ISR pc=0x{e.last_pc:08X} insn={e.insn} sp=0x{e.uc.reg_read(UC_ARM_REG_SP):08X}')
    e._enter_isr(handler)
    print(f'    LR=0x{e.uc.reg_read(UC_ARM_REG_LR):08X}')
    pc = handler | 1
    for k in range(200):
        i = dis1(e, pc & ~1)
        print(f'{k:3d} pc=0x{pc & ~1:08X}  {i.mnemonic if i else "?"} {i.op_str if i else ""}  '
              f'r0=0x{e.uc.reg_read(UC_ARM_REG_R0):08X} r1=0x{e.uc.reg_read(UC_ARM_REG_R1):08X} '
              f'r2=0x{e.uc.reg_read(UC_ARM_REG_R2):08X} r3=0x{e.uc.reg_read(UC_ARM_REG_R3):08X}')
        try:
            e.uc.emu_start(pc, 0, count=1)
        except Exception as ex:
            print('EXC:', ex)
            break
        pc = e.uc.reg_read(UC_ARM_REG_PC) | 1
        if (pc & ~1) == e.TRAMP:
            print('*** reached trampoline -> return')
            break


if __name__ == '__main__':
    main()
