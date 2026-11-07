#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Verify the virtual CC2500 end to end: init, packet injection, RAM effects.

For each Godox test packet (see docs/05_cc2500_emu_spec.md §6) this script:
  * snapshots RAM + LCD frame buffer,
  * injects the packet into the virtual CC2500 (GDO0 -> EXTI15 -> IRQ7),
  * runs the firmware a bit,
  * reports which RAM words / LCD pixels changed.

Run:  python tools/rf_verify.py
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

PACKETS = [
    ('D5 80 07 00', '属性/命令帧：写 0x20000480[0]'),
    ('D5 09 00 00', '属性帧：0x200001B2 |= 0x0A, 0x200001C4=100'),
    ('D5 21 00 00', '命令帧 0x21'),
    ('A9 50 B4 02', '配对属性帧 A9 50 B4 02'),
]


def call_function(emu, addr, args=(), max_insns=2_000_000):
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
    return hit['v']


def snapshot(e):
    ram = bytes(e.read_mem(0x20000000, 0x2000))
    fb = bytes(e.read_mem(0x200004CC, 1024))
    return ram, fb


def diff_ram(a, b, limit=12):
    out = []
    for i in range(0x2000, 4):
        pass
    for i in range(0, 0x2000, 4):
        wa = int.from_bytes(a[i:i + 4], 'little')
        wb = int.from_bytes(b[i:i + 4], 'little')
        if wa != wb:
            out.append((0x20000000 + i, wa, wb))
            if len(out) >= limit:
                break
    return out


def main():
    e = core.Emulator()
    e.reset()
    e.run(3_000_000)
    print(f'boot: insn={e.insn_count} pc=0x{e.reg("PC"):08X}')
    print('rf_init:', end=' ')
    call_function(e, 0x08004AF0)
    e.run(100_000)
    st = e.rf_status()
    print('state=%s freq2=0x%02X writes=%d patable=%s'
          % (st['state'], st['freq2'], len(e.cc2500.spi_writes), st['patable'][:3]))
    for hexpkt, desc in PACKETS:
        ram0, fb0 = snapshot(e)
        irq0 = e.irq_counts.get(23, 0)
        e.rf_inject_rx(hexpkt)
        e.run(300_000)
        ram1, fb1 = snapshot(e)
        irq1 = e.irq_counts.get(23, 0)
        d = diff_ram(ram0, ram1)
        lcd_diff = sum(1 for a, b in zip(fb0, fb1) if a != b)
        print(f'\n[{hexpkt}] {desc}')
        print(f'  IRQ7: {irq0} -> {irq1}   LCD bytes changed: {lcd_diff}   RAM words changed: {len(d)}')
        for addr, a, b in d[:8]:
            print(f'    0x{addr:08X}: 0x{a:08X} -> 0x{b:08X}')
        e.rf_inject_rx(hexpkt)  # restore RX state for next round
        e.run(100_000)


if __name__ == '__main__':
    main()
