#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Probe: call a firmware function manually (trampoline call) and verify the
virtual CC2500 sees the RF init sequence.

The firmware boots into a display-animation loop and never reaches the RF init
function (0x08004AF0) in our emulation, so we invoke it directly, exactly like
the manual-ISR trampoline trick.

Run:  python tools/rf_boot.py [func_addr] [boot_insns]
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'emu'))
import core  # noqa: E402
from unicorn import UC_HOOK_CODE, UcError  # noqa: E402
from unicorn.arm_const import *  # noqa: E402
from unicorn import arm_const  # noqa: E402

TRAMP = 0x0800F900
REGS = [getattr(arm_const, 'UC_ARM_REG_R%d' % i) for i in range(13)]


def call_function(emu, addr, args=(), max_insns=3_000_000):
    uc = emu._uc
    ctx = {r: uc.reg_read(r) for r in REGS + [UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_XPSR]}
    for i, v in enumerate(args[:4]):
        uc.reg_write(REGS[i], v & 0xFFFFFFFF)
    uc.reg_write(UC_ARM_REG_LR, TRAMP | 1)
    hit = {'v': False}

    def hook(u, a, s, user):
        if a == TRAMP:
            hit['v'] = True
            u.emu_stop()
    h = uc.hook_add(UC_HOOK_CODE, hook, None, TRAMP, TRAMP + 3)
    pc = addr | 1
    done = 0
    try:
        while done < max_insns and not hit['v']:
            uc.emu_start(pc, 0, count=50_000)
            done += 50_000
            pc = uc.reg_read(UC_ARM_REG_PC) | 1
    except UcError as e:
        print('  call error:', e)
    finally:
        uc.hook_del(h)
        for r, v in ctx.items():
            try:
                uc.reg_write(r, v)
            except UcError:
                pass
    print(f'  called 0x{addr:08X} ({done} insns) returned={hit["v"]}')
    return hit['v']


def main():
    func = int(sys.argv[1], 0) if len(sys.argv) > 1 else 0x08004AF0
    boot = int(sys.argv[2], 0) if len(sys.argv) > 2 else 3_000_000
    e = core.Emulator()
    e.reset()
    e.run(boot)
    print(f'boot: insn={e.insn_count} pc=0x{e.reg("PC"):08X}')
    print('cc2500 before:', e.rf_status()['state'], 'writes', len(e.cc2500.spi_writes))
    call_function(e, func)
    e.run(200_000)
    st = e.rf_status()
    print('cc2500 after: state=%s freq2=0x%02X pktlen=%d patable=%s tx=%d rx=%d'
          % (st['state'], st['freq2'], st['pktlen'], st['patable'][:4], st['tx_count'], st['rx_count']))
    print('spi writes:', len(e.cc2500.spi_writes),
          [(hex(a), hex(b)) for a, b in e.cc2500.spi_writes[:12]])
    print('strobes:', [m for _, m in e.cc2500.log[:12]])
    # now inject a packet and let the firmware ISR consume it
    before = e.irq_counts.get(23, 0)
    e.rf_inject_rx('A9 50 B4 02')
    e.run(400_000)
    print('after inject: irq7=%d (was %d) gdo0=%d rx_fifo=%s'
          % (e.irq_counts.get(23, 0), before, e.cc2500.gdo0, bytes(e.cc2500.rx_fifo).hex()))


if __name__ == '__main__':
    main()
