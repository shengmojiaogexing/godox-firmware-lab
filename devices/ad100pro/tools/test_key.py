#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Test virtual key-matrix injection: compare two runs (with / without key press)."""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
import boottest as bt  # noqa: E402


def run(keys, window, steps):
    e = bt.Emu(None)
    e.vkeys = set(keys)
    e.key_window = window
    e.run(steps)
    ram = bytes(e.uc.mem_read(0x20000000, 0x2000))
    return e, ram


def main():
    steps = int(sys.argv[1], 0) if len(sys.argv) > 1 else 20_000_000
    keys = [int(x, 0) for x in (sys.argv[2].split(',') if len(sys.argv) > 2 else ['0'])]
    window = sys.argv[3] if len(sys.argv) > 3 else '8000000:12000000'
    base_e, base_ram = run([], '', steps)
    key_e, key_ram = run(keys, window, steps)
    print(f'baseline: insn={base_e.insn} pc=0x{base_e.last_pc:08X} lcd_bytes={len(base_e.lcd_stream)}')
    print(f'key {keys} {window}: insn={key_e.insn} pc=0x{key_e.last_pc:08X} '
          f'lcd_bytes={len(key_e.lcd_stream)} pressed={key_e.keys_ever_pressed}')
    diffs = [i for i in range(0x2000) if base_ram[i] != key_ram[i]]
    print(f'RAM differences: {len(diffs)}')
    if diffs:
        print('  first 40 diff offsets:', [hex(x) for x in diffs[:40]])
    lcd_diff = sum(1 for p in range(8) for x in range(128)
                   if base_e.lcd.ram[p][x] != key_e.lcd.ram[p][x])
    print(f'LCD framebuffer differing cells: {lcd_diff}')
    # save both screens for eyeballing
    os.makedirs(os.path.join(ROOT, 'out'), exist_ok=True)
    for name, e in (('out/key_base.png', base_e), ('out/key_pressed.png', key_e)):
        e.save_screen(name)


if __name__ == '__main__':
    main()
