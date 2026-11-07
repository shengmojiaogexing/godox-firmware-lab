#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Scan all 15 matrix keycodes + power key, report which ones change app state."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import boottest as bt  # noqa: E402

SETTLE = 8_000_000
STEP = 1_500_000


def snap(e):
    ram = bytes(e.uc.mem_read(0x20000000, 0x2000))
    lcd = bytes(e.lcd.ram[p][x] for p in range(8) for x in range(128))
    return ram, lcd


def main():
    e = bt.Emu(None)
    e.run(SETTLE)
    prev_ram, prev_lcd = snap(e)
    print(f'settled at insn={e.insn} pc=0x{e.last_pc:08X}')
    print(f'{"key":>4}  {"RAMdiff":>7}  {"LCDdiff":>7}  changed_offsets')
    for k in range(15):
        e.vkeys = {k}
        base = e.insn
        e.key_window = f'{base}:{base + 600000}'
        e.run(STEP)
        ram, lcd = snap(e)
        diffs = [i for i in range(0x2000) if ram[i] != prev_ram[i]]
        lcd_d = sum(1 for a, b in zip(lcd, prev_lcd) if a != b)
        extra = ''
        if diffs:
            extra = ', '.join(f'0x{0x20000000 + i:08X}' for i in diffs[:8])
        print(f'{k:>4}  {len(diffs):>7}  {lcd_d:>7}  {extra}')
        prev_ram, prev_lcd = ram, lcd
    # power key = PB0 (EXTI0); inject by pulling IDR bit0 low on GPIOB
    print('\npower key PB0 injection is EXTI-based (needs NVIC) - skipped here')


if __name__ == '__main__':
    main()
