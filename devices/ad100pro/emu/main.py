#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""CLI debugger for the Godox AD100 Pro (STM32F0 / Cortex-M0) firmware.

Examples
--------
  python emu/main.py --info
  python emu/main.py --run 5000000 --regs
  python emu/main.py --run 5000000 --screen out/lcd_emu.png
  python emu/main.py --run 3000000 --dump out/emu_mmio.log --trace RCC,GPIOB
  python emu/main.py --bp 0x0800CF74 --run 2000000 --regs
  python emu/main.py --step 20 --regs
  python emu/main.py --mem 0x20000000 64
  python emu/main.py --run 5000000 --bootlog
"""

import argparse
import os
import struct
import sys
import time
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(HERE)
if HERE not in sys.path:
    sys.path.insert(0, HERE)

from core import (Emulator, DEFAULT_FIRMWARE, DEFAULT_FUNCTIONS, APP_BASE,
                  IMAGE_END, CALIB_BASE, ROM_BASE, ROM_SIZE, RAM_BASE)

DEFAULT_BOOT_INSN = 5_000_000


# ---------------------------------------------------------------------------
# helpers
# ---------------------------------------------------------------------------
def parse_int(text):
    """Accept 0x.., decimal, and bare hex addresses like 080030D4."""
    text = text.strip()
    try:
        if text.lower().startswith('0x'):
            return int(text, 16)
        if text.startswith('0b'):
            return int(text, 2)
        if text.isdigit():
            return int(text, 10)
        return int(text, 16)
    except ValueError:
        raise argparse.ArgumentTypeError('bad number: %r' % text)


def parse_inject(spec):
    """Parse one --inject item: uart=48656c6c6f | key=PB0:0 | adc=1234 | ..."""
    kind, _, val = spec.partition('=')
    kind = kind.strip().lower()
    val = val.strip()
    if kind == 'uart':
        try:
            data = bytes.fromhex(val)
        except ValueError:
            data = val.encode('latin-1', 'replace')
        return ('uart', data)
    if kind == 'key':
        port, _, level = val.partition(':')
        port = port.strip().upper()
        # key=PB0:0 -> port=PB pin=0 value=0
        import re
        m = re.match(r'P?([A-Fa-f])(\d+)', port)
        if not m:
            raise SystemExit('bad key spec: %r (want e.g. PB0:0)' % spec)
        port_letter = m.group(1).upper()
        pin = int(m.group(2))
        value = int(level.strip(), 0) if level.strip() else 0
        return ('key', (port_letter, pin, value))
    if kind == 'adc':
        if ':' in val:
            ch, _, v = val.partition(':')
            return ('adc', (int(v, 0), int(ch, 0)))
        return ('adc', (int(val, 0), None))
    if kind == 'spi':
        try:
            data = bytes.fromhex(val)
        except ValueError:
            data = val.encode('latin-1', 'replace')
        return ('spi', data)
    if kind == 'exti':
        return ('exti', int(val, 0))
    raise SystemExit('unknown inject kind: %r' % kind)


def apply_injections(emu, injections):
    applied = []
    for kind, payload in injections:
        if kind == 'uart':
            emu.inject_uart(payload)
            applied.append('uart rx %d bytes' % len(payload))
        elif kind == 'key':
            port, pin, value = payload
            emu.inject_key(port, pin, value)
            applied.append('key P%s%d=%d' % (port, pin, value))
        elif kind == 'adc':
            value, channel = payload
            emu.inject_adc(value, channel)
            applied.append('adc ch%s=%d' % (channel, value))
        elif kind == 'spi':
            emu.inject_spi(payload)
            applied.append('spi miso %d bytes' % len(payload))
        elif kind == 'exti':
            emu.inject_exti(payload)
            applied.append('exti mask 0x%X' % payload)
    return applied


def make_png_1bpp(width, height, bits, scale=6, invert=True):
    """Encode a page-format 1bpp framebuffer (RAM 0x200004CC layout) as PNG.

    Page format: byte = bits[page*width + column], bit0 = top row of the page.
    Rendered as RGB (dark panel background / light pixels).
    """
    w, h = width * scale, height * scale
    fg = (240, 244, 250)
    bg = (14, 18, 24)
    raw = bytearray()
    for y in range(h):
        page = (y // scale) // 8
        bit = (y // scale) % 8
        raw.append(0)
        line = bytearray()
        for x in range(w):
            col = x // scale
            idx = page * width + col
            byte = bits[idx] if idx < len(bits) else 0
            v = (byte >> bit) & 1
            if invert:
                v ^= 1
            c = fg if v else bg
            line += bytes(c)
        raw += line

    def chunk(tag, data):
        c = struct.pack('>I', len(data)) + tag + data
        return c + struct.pack('>I', zlib.crc32(tag + data) & 0xFFFFFFFF)

    ihdr = struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)
    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', ihdr) +
            chunk(b'IDAT', zlib.compress(bytes(raw), 9)) + chunk(b'IEND', b''))


def _fb_pixels(bits, width, height):
    pix = [[0] * width for _ in range(height)]
    for y in range(height):
        page, bit = y // 8, y % 8
        for x in range(width):
            idx = page * width + x
            if idx < len(bits) and (bits[idx] >> bit) & 1:
                pix[y][x] = 1
    return pix


def _rotate_pixels(pix, rotate=90, mirror=True):
    h, w = len(pix), len(pix[0])
    if rotate == 90:      # panel is mounted rotated 90 deg vs controller RAM
        pix = [[pix[h - 1 - y][x] for y in range(h)] for x in range(w)]
    elif rotate == 180:
        pix = [[pix[h - 1 - y][w - 1 - x] for x in range(w)] for y in range(h)]
    elif rotate == 270:
        pix = [[pix[y][w - 1 - x] for y in range(h)] for x in range(w)]
    if mirror:
        pix = [list(reversed(row)) for row in pix]
    return pix


def make_png_pixels(pix, scale=6):
    """Encode a 0/1 pixel matrix as an RGB PNG."""
    h, w = len(pix), len(pix[0])
    W, H = w * scale, h * scale
    fg, bg = (240, 244, 250), (14, 18, 24)
    raw = bytearray()
    for y in range(H):
        raw.append(0)
        line = bytearray()
        row = pix[y // scale]
        for x in range(W):
            line += bytes(fg if row[x // scale] else bg)
        raw += line

    def chunk(tag, data):
        c = struct.pack('>I', len(data)) + tag + data
        return c + struct.pack('>I', zlib.crc32(tag + data) & 0xFFFFFFFF)

    ihdr = struct.pack('>IIBBBBB', W, H, 8, 2, 0, 0, 0)
    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', ihdr) +
            chunk(b'IDAT', zlib.compress(bytes(raw), 9)) + chunk(b'IEND', b''))


def write_screen_png(emu, path, scale=6, rotate=90, mirror=True):
    bits = emu.screen_bits()
    pix = _fb_pixels(bits, emu.fb_width, emu.fb_height)
    pix = _rotate_pixels(pix, rotate=rotate, mirror=mirror)
    png = make_png_pixels(pix, scale=scale)
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, 'wb') as f:
        f.write(png)
    lit = 0
    for b in bits:
        lit += bin(b).count('1')
    return path, len(png), lit


def fmt_trace_line(t):
    return t.line()


def write_trace_log(emu, path, limit=None):
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    lines = ['# AD100 Pro MMIO trace',
             '# firmware=%s' % emu.firmware_path,
             '# app_base=0x%08X reset=0x%08X sp=0x%08X' % (APP_BASE, emu.reset_pc, emu.reset_sp),
             '# instructions=%d  trace_entries=%d  truncated=%s'
             % (emu.insn_count, len(emu.trace), emu.trace_truncated),
             '# columns: [insn] pc  device  register  R/W value  size  function']
    n = 0
    for t in emu.trace:
        lines.append(t.line())
        n += 1
        if limit and n >= limit:
            lines.append('# ... truncated at %d entries' % limit)
            break
    with open(path, 'w', encoding='utf-8') as f:
        f.write('\n'.join(lines) + '\n')
    return path, n


def gpio_activity(emu):
    """Summarise GPIOB/GPIOA pin toggling (LCD bus + buttons)."""
    counts = {}
    for insn, port, setb, clrb in (emu.pin_log or []):
        counts.setdefault(port, 0)
        counts[port] += 1
    return counts


def lcd_summary(emu):
    lcd = emu.lcd
    cmds = [v for _, v in lcd.commands]
    init = cmds[:32]
    fb = emu.screen_bits()
    return dict(width=emu.fb_width, height=emu.fb_height, bytes_written=len(lcd.bytes),
                frames=lcd.frames, page=lcd.page, col=lcd.col,
                fb_addr=emu.fb_addr,
                fb_nonzero=sum(1 for b in fb if b),
                init_commands=['%02X' % v for v in init],
                data_bytes=sum(1 for _, r, v in lcd.bytes if r),
                cmd_bytes=sum(1 for _, r, v in lcd.bytes if not r))


def final_report(emu, elapsed, argv):
    regs = emu.regs_all()
    lines = []
    lines.append('=' * 78)
    lines.append('AD100 Pro emulator - final state')
    lines.append('=' * 78)
    lines.append('firmware        : %s' % emu.firmware_path)
    lines.append('flash/ram       : flash 0x%08X+0x%X, RAM 0x%08X+0x%X'
                 % (ROM_BASE, ROM_SIZE, RAM_BASE, emu.ram_size))
    lines.append('reset vector    : SP=0x%08X PC=0x%08X (thumb bit stripped)'
                 % (emu.reset_sp, emu.reset_pc))
    lines.append('instructions    : %d' % emu.insn_count)
    lines.append('wall time       : %.3fs  (%.2f Mips)' % (elapsed, emu.insn_count / max(elapsed, 1e-9) / 1e6))
    lines.append('final PC        : 0x%08X  SP=0x%08X  LR=0x%08X'
                 % (regs['PC'], regs['SP'], regs['LR']))
    f = emu.func_at(regs['PC'])
    lines.append('inside function : %s' % ('%s (0x%08X)' % (f[1], f[0]) if f else '-'))
    lines.append('crashed         : %s (%d faults)' % ('YES' if emu.faults else 'no', len(emu.faults)))
    for fault in emu.faults[-5:]:
        lines.append('  fault %-22s pc=0x%08X lr=0x%08X sp=0x%08X addr=%s insn=%d'
                     % (fault.kind, fault.pc, fault.lr, fault.sp,
                        ('0x%08X' % fault.addr) if fault.addr is not None else '-', fault.insn))
    if emu.uart_out:
        text = emu.uart_text()
        lines.append('UART TX         : %d bytes' % len(emu.uart_out))
        lines.append('  %r' % text[:400])
    else:
        lines.append('UART TX         : (none)')
    spi = ''.join('%02X' % b for b in emu.spi_out[:32])
    lines.append('SPI MOSI        : %d bytes %s%s' % (len(emu.spi_out), spi, '...' if len(emu.spi_out) > 32 else ''))
    lines.append('IWDG kicks      : %d (started=%s)' % (emu.iwdg.kicks, emu.iwdg.started))
    lines.append('real IRQ        : %s  counts=%s  virtualTicks=%d  tim14=%d'
                 % (emu.real_irq, dict(emu.irq_counts),
                    getattr(emu, 'virtual_ticks', 0), getattr(emu, '_tim14_count', 0)))
    for x in emu._irq_dispatch_log[-6:]:
        lines.append('  irq %-3d handler=0x%08X source=%s at insn=%d'
                     % (x['irq'], x['handler'], x['source'], x['insn']))
    lines.append('GPIO writes     : %s' % (gpio_activity(emu) or '-'))
    lcd = lcd_summary(emu)
    lines.append('LCD             : %dx%d 1bpp, FB@0x%08X nonzero=%d, %d bytes streamed (%d cmd / %d data), frames=%d'
                 % (lcd['width'], lcd['height'], lcd['fb_addr'], lcd['fb_nonzero'],
                    lcd['bytes_written'], lcd['cmd_bytes'], lcd['data_bytes'], lcd['frames']))
    lines.append('LCD init cmds   : %s' % ' '.join(lcd['init_commands']))
    if emu.events:
        lines.append('events          : %d (last: %s)' % (len(emu.events), emu.events[-3:]))
    lines.append('')
    lines.append('peripheral summary (device register R/W count first last):')
    for e in emu.periph_summary():
        lines.append('  %-8s %-12s %s %7d  first=0x%08X last=0x%08X  insn %d..%d'
                     % (e['dev'], e['reg'], e['rw'], e['count'], e['first'], e['last'],
                        e['first_insn'], e['last_insn']))
    lines.append('')
    lines.append('last 20 trace entries:')
    for t in emu.trace[-20:]:
        lines.append('  ' + t.line())
    lines.append('')
    lines.append('registers:')
    for name in ('R0', 'R1', 'R2', 'R3', 'R4', 'R5', 'R6', 'R7', 'R8', 'R9',
                 'R10', 'R11', 'R12', 'SP', 'LR', 'PC'):
        lines.append('  %-4s = 0x%08X' % (name, regs[name]))
    return '\n'.join(lines)


# ---------------------------------------------------------------------------
# boot log / main trace log
# ---------------------------------------------------------------------------
def write_boot_logs(emu, outdir, n_trace=2000, label=''):
    os.makedirs(outdir, exist_ok=True)
    boot_path = os.path.join(outdir, 'emu_boot.log')
    main_path = os.path.join(outdir, 'emu_main_trace.log')

    lcd = lcd_summary(emu)
    L = []
    L.append('AD100 Pro (Godox) firmware emulation -- boot log')
    L.append('generated: %s' % time.strftime('%Y-%m-%d %H:%M:%S'))
    L.append('firmware : %s' % emu.firmware_path)
    L.append('layout   : flash 0x%08X..0x%08X (128 KiB); application image at 0x%08X..0x%08X'
             % (ROM_BASE, ROM_BASE + ROM_SIZE, APP_BASE, IMAGE_END))
    L.append('           bootloader area 0x08000000..0x08002FFF emulated as application vectors (first 0xC0 bytes) + 0xFF')
    L.append('           factory calibration 0x%08X..0x%08X: %s'
             % (CALIB_BASE, ROM_BASE + ROM_SIZE,
                'synthetic (%d records)' % emu.calib_records if emu.calib == 'synthetic' else 'blank (0xFF)'))
    L.append('vectors  : SP=0x%08X  Reset_Handler=0x%08X' % (emu.reset_sp, emu.reset_pc))
    L.append('executed : %d instructions, %.2f Mips' %
             (emu.insn_count, emu.insn_count / max(emu.run_time, 1e-9) / 1e6))
    pc = emu.reg('PC')
    f = emu.func_at(pc)
    L.append('final PC : 0x%08X  SP=0x%08X  LR=0x%08X  (%s)'
             % (pc, emu.reg('SP'), emu.reg('LR'), f[1] if f else '-'))
    L.append('crashed  : %s, faults=%d' % ('YES' if emu.faults else 'no', len(emu.faults)))
    for fault in emu.faults[:10]:
        L.append('  fault %-24s pc=0x%08X lr=0x%08X sp=0x%08X addr=%s insn=%d'
                 % (fault.kind, fault.pc, fault.lr, fault.sp,
                    ('0x%08X' % fault.addr) if fault.addr is not None else '-', fault.insn))
    L.append('UART     : %d bytes %r' % (len(emu.uart_out), emu.uart_text()[:200]))
    L.append('IWDG     : %d kicks (started=%s)' % (emu.iwdg.kicks, emu.iwdg.started))
    L.append('LCD      : %dx%d 1bpp, FB@0x%08X nonzero=%d, frames=%d, bytes=%d (cmd=%d data=%d)'
             % (lcd['width'], lcd['height'], lcd['fb_addr'], lcd['fb_nonzero'],
                lcd['frames'], lcd['bytes_written'], lcd['cmd_bytes'], lcd['data_bytes']))
    L.append('LCD init : %s' % ' '.join(lcd['init_commands']))
    L.append('GPIO toggles by port: %s' % (gpio_activity(emu) or '-'))
    L.append('events   : %d' % len(emu.events))
    for insn, what, val in emu.events[:20]:
        L.append('  [%9d] %-18s 0x%08X' % (insn, what, val))
    L.append('')
    L.append('=' * 78)
    L.append('first %d MMIO accesses' % n_trace)
    L.append('=' * 78)
    L.append('# [insn] pc  device  register R/W value size function')
    for t in emu.trace[:n_trace]:
        L.append(t.line())
    L.append('')
    L.append('=' * 78)
    L.append('peripheral access summary (whole run, %d traced accesses%s)'
             % (len(emu.trace), ', TRUNCATED' if emu.trace_truncated else ''))
    L.append('=' * 78)
    for e in emu.periph_summary():
        L.append('  %-8s %-12s %s %7d  first=0x%08X last=0x%08X  insn %d..%d'
                 % (e['dev'], e['reg'], e['rw'], e['count'], e['first'], e['last'],
                    e['first_insn'], e['last_insn']))
    L.append('')
    L.append('=' * 78)
    L.append('final registers / memory')
    L.append('=' * 78)
    regs = emu.regs_all()
    for name in ('R0', 'R1', 'R2', 'R3', 'R4', 'R5', 'R6', 'R7', 'R8', 'R9',
                 'R10', 'R11', 'R12', 'SP', 'LR', 'PC'):
        L.append('  %-4s = 0x%08X' % (name, regs[name]))
    L.append('  RAM 0x20000000..0x20000040: %s'
             % emu.read_mem(0x20000000, 0x40).hex(' '))
    L.append('  RAM 0x20000100..0x20000120: %s'
             % emu.read_mem(0x20000100, 0x20).hex(' '))
    L.append('  calib[0:16] @0x%08X: %s' % (CALIB_BASE, emu.read_mem(CALIB_BASE, 16).hex(' ')))
    with open(boot_path, 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(L) + '\n')

    # ---- main trace log ---------------------------------------------------
    M = []
    M.append('AD100 Pro emulation -- main flow / MMIO summary')
    M.append('generated: %s' % time.strftime('%Y-%m-%d %H:%M:%S'))
    M.append('')
    M.append('main entry : 0x0800CF74 (func_0800CF74), called from 0x080030C0 startup stub')
    M.append('SystemInit : 0x0800B964 (func_0800B964)')
    M.append('init_array : 0x08003548 (func_08003548)')
    M.append('')
    M.append('== runtime function-entry sequence (start of run, first 150) ==')
    seen = []
    last = None
    main_insn = None
    for insn, addr, name, depth in emu.call_log:
        if addr == 0x0800CF74:
            main_insn = insn
        if name == last:
            continue
        last = name
        seen.append((insn, addr, name))
        if len(seen) >= 150:
            break
    if main_insn is not None:
        M.append('  main(0x0800CF74) first entered at insn %d' % main_insn)
    for insn, addr, name in seen:
        M.append('  [%9d] %-16s 0x%08X' % (insn, name, addr))
    M.append('')
    M.append('== most-invoked functions over the whole run (top 25) ==')
    counts = getattr(emu, 'func_counts', {})
    for addr, cnt in sorted(counts.items(), key=lambda kv: -kv[1])[:25]:
        M.append('  %-16s 0x%08X  entries=%d' % (emu.functions.name_of(addr), addr, cnt))
    M.append('')
    M.append('== MMIO summary by device/register ==')
    for e in emu.periph_summary():
        M.append('  %-8s %-12s %s %7d  first=0x%08X last=0x%08X  insn %d..%d'
                 % (e['dev'], e['reg'], e['rw'], e['count'], e['first'], e['last'],
                    e['first_insn'], e['last_insn']))
    M.append('')
    M.append('== peripheral notes ==')
    rcc = emu.rcc
    M.append('  RCC CR      = 0x%08X  CFGR = 0x%08X  APB2ENR = 0x%08X  APB1ENR = 0x%08X  AHBENR = 0x%08X'
             % (rcc.regs.get(0x00, 0), rcc.regs.get(0x04, 0), rcc.regs.get(0x18, 0),
                rcc.regs.get(0x1C, 0), rcc.regs.get(0x14, 0)))
    M.append('  SYSCFG CFGR1= 0x%08X (bit0/1 = memory remap: SRAM at 0x0)'
             % emu.syscfg.regs.get(0x00, 0))
    M.append('  FLASH ACR   = 0x%08X  CR writes=%s'
             % (emu.flash.regs.get(0x00, 0), ['0x%08X' % v for v in emu.flash.cr_writes[:8]]))
    M.append('  RTC  ISR    = 0x%08X  WUTR=0x%08X' % (emu.rtc.read(0x0C), emu.rtc.regs.get(0x14, 0)))
    M.append('  IWDG kicks  = %d' % emu.iwdg.kicks)
    M.append('  ADC  DR     = 0x%03X (ch %d)  values=%s'
             % (emu.adc._value(), emu.adc._selected_channel(), emu.adc.values))
    M.append('  SPI1  bytes = %d  first=%s'
             % (len(emu.spi_out), ''.join('%02X' % b for b in emu.spi_out[:16])))
    M.append('  I2C1  bytes = %d  first=%s'
             % (len(emu.i2c1.rx_log), ''.join('%02X' % b for b in emu.i2c1.rx_log[:16])))
    M.append('  UART  bytes = %d' % len(emu.uart_out))
    M.append('  GPIO writes = %s' % (gpio_activity(emu) or '-'))
    M.append('')
    M.append('== LCD ==')
    M.append('  controller     : SH1106/SSD1306 style 128x64 1bpp, page/column format')
    M.append('  pins           : SCL=GPIOB.6  RS/A0=GPIOB.7  CS=GPIOB.9 (active low)  SDA=GPIOA.10')
    M.append('  bytes written  : %d (%d command, %d data)'
             % (lcd['bytes_written'], lcd['cmd_bytes'], lcd['data_bytes']))
    M.append('  frames started : %d' % lcd['frames'])
    M.append('  init sequence  : %s' % ' '.join(lcd['init_commands']))
    M.append('  first 64 bytes : %s'
             % ' '.join('%02X%s' % (v, '' if r else "'") for _, r, v in emu.lcd.bytes[:64]))
    M.append('')
    M.append('== GPIO pin map (derived) ==')
    M.append('  GPIOA.10 = LCD SDA (bit-banged data)')
    M.append('  GPIOB.6  = LCD SCL')
    M.append('  GPIOB.7  = LCD RS/A0 (0=cmd, 1=data)')
    M.append('  GPIOB.9  = LCD CS (active low)')
    M.append('  GPIOC    = buttons/LCD status input (IDR polling)')
    with open(main_path, 'w', encoding='utf-8') as fh:
        fh.write('\n'.join(M) + '\n')
    return boot_path, main_path


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------
def build_arg_parser():
    p = argparse.ArgumentParser(description='AD100 Pro Cortex-M0 emulator / debugger')
    p.add_argument('--firmware', default=DEFAULT_FIRMWARE, help='firmware .bin path')
    p.add_argument('--functions', default=DEFAULT_FUNCTIONS, help='functions.json path')
    p.add_argument('--info', action='store_true', help='show firmware/vector/peripheral info')
    p.add_argument('--run', type=parse_int, default=None, metavar='N',
                   help='run N instructions from current PC')
    p.add_argument('--step', type=parse_int, default=None, metavar='N',
                   help='single-step N instructions')
    p.add_argument('--bp', action='append', default=[], type=parse_int, metavar='ADDR',
                   help='breakpoint address (repeatable)')
    p.add_argument('--regs', action='store_true', help='dump registers before/after run')
    p.add_argument('--mem', nargs='+', type=parse_int, metavar='ADDR',
                   help='dump memory: --mem ADDR [LEN]')
    p.add_argument('--trace', action='append', default=[], metavar='FILTER',
                   help='MMIO trace filter: comma list of device names or hex addrs/ranges')
    p.add_argument('--trace-reg', action='append', default=[], metavar='REG')
    p.add_argument('--trace-addr', action='append', default=[], metavar='ADDR[-ADDR]')
    p.add_argument('--no-trace', action='store_true', help='disable MMIO trace capture')
    p.add_argument('--dump', default=None, metavar='PATH', help='write MMIO trace log to PATH')
    p.add_argument('--screen', default=None, metavar='PATH', help='render decoded LCD frame to PNG')
    p.add_argument('--screen-scale', type=int, default=4, help='PNG scale factor (default 4)')
    p.add_argument('--jump-main', action='store_true',
                   help='fast-boot: warm up then jump into the 1 ms scheduler (interactive mode)')
    p.add_argument('--warmup', type=parse_int, default=1_000_000,
                   help='instructions to run before --jump-main (default 1000000)')
    p.add_argument('--battery', type=int, default=None, metavar='MV', help='set battery voltage (mV)')
    p.add_argument('--key', action='append', default=[], metavar='NAME',
                   help='hold a virtual key down (power/key2/mode/set/up/down/k0..k9)')
    p.add_argument('--key-press', action='append', default=[], metavar='NAME[:MS]',
                   help='virtual key pulse, e.g. up:80')
    p.add_argument('--bootlog', action='store_true',
                   help='write out/emu_boot.log + out/emu_main_trace.log after the run')
    p.add_argument('--outdir', default=os.path.join(PROJECT_ROOT, 'out'))
    p.add_argument('--inject', action='append', default=[], metavar='SPEC',
                   help='inject state: uart=HEX, key=PB0:0, adc=VALUE, spi=HEX, exti=MASK')
    p.add_argument('--calib', choices=['blank', 'synthetic'], default='blank',
                   help='factory calibration area 0x0801F000: blank=0xFF, synthetic=fake records')
    p.add_argument('--no-real-irq', dest='real_irq', action='store_false', default=True,
                   help='disable real handler dispatch (fall back to fake SysTick RAM poke)')
    p.add_argument('--no-tim14-irq', dest='virtual_tim14', action='store_false', default=True,
                   help='do not dispatch the periodic TIM14 update IRQ (IRQ19)')
    p.add_argument('--rf-rx', action='append', default=[], metavar='HEX',
                   help='inject a CC2500 RX packet (GDO0 -> EXTI15 -> IRQ7) before running')
    p.add_argument('--on-fault', choices=['stop', 'continue'], default='stop',
                   help='behaviour on unmapped data access')
    p.add_argument('--max-trace', type=int, default=500_000)
    p.add_argument('--quiet', action='store_true')
    return p


def main(argv=None):
    args = build_arg_parser().parse_args(argv)
    if args.info:
        print('firmware  : %s' % args.firmware)
        try:
            data = open(args.firmware, 'rb').read()
            import struct as _s
            sp, reset = _s.unpack_from('<II', data, 0)
            print('size      : %d bytes (0x%X)' % (len(data), len(data)))
            print('app base  : 0x%08X  image end 0x%08X' % (APP_BASE, APP_BASE + len(data)))
            print('vectors   : SP=0x%08X  Reset=0x%08X (entry 0x%08X)'
                  % (sp, reset, reset & ~1))
            vecs = [_s.unpack_from('<I', data, i * 4)[0] for i in range(48)]
            named = {0: 'initial SP', 1: 'Reset_Handler', 2: 'NMI', 3: 'HardFault',
                     4: 'MemManage', 5: 'BusFault', 6: 'UsageFault', 11: 'SVCall',
                     14: 'PendSV', 15: 'SysTick'}
            for i, v in enumerate(vecs):
                if v and (i < 16):
                    print('  vec[%2d] %-14s 0x%08X' % (i, named.get(i, 'IRQ%d' % (i - 16)), v))
            print('flash     : 0x%08X..0x%08X (%d KiB)'
                  % (ROM_BASE, ROM_BASE + ROM_SIZE, ROM_SIZE // 1024))
            print('RAM       : 0x%08X..0x%08X' % (RAM_BASE, RAM_BASE + 0x2000))
            print('calib     : 0x%08X..0x%08X (factory data, not in image)' % (CALIB_BASE, ROM_BASE + ROM_SIZE))
        except OSError as e:
            print('cannot read firmware: %s' % e)
        print('peripheral windows:')
        for base, size in ((0x40000000, 0x25000), (0x48000000, 0x2000), (0xE0000000, 0xF000)):
            print('  0x%08X..0x%08X' % (base, base + size))
        try:
            import json
            f = json.load(open(args.functions, encoding='utf-8'))
            print('functions : %d (from %s)' % (len(f), args.functions))
        except Exception as e:
            print('functions : unavailable (%s)' % e)
        if args.run is None and args.step is None:
            return 0

    injections = [parse_inject(s) for s in args.inject]
    filters = []
    for item in args.trace:
        filters.extend([x.strip() for x in item.split(',') if x.strip()])
    filters.extend(args.trace_reg)
    filters.extend(args.trace_addr)

    emu = Emulator(args.firmware, args.functions, trace=not args.no_trace,
                   trace_filter=filters or None, max_trace=args.max_trace,
                   on_fault=args.on_fault, calib=args.calib,
                   real_irq=args.real_irq, virtual_tim14=args.virtual_tim14)
    if args.no_trace:
        emu.pin_log = None
    for bp in args.bp:
        emu.bp_add(bp)
    applied = apply_injections(emu, injections)
    if args.battery is not None:
        applied.append('battery %d mV (raw %d)' % (args.battery, emu.set_battery(args.battery)))
    for name in args.key:
        ok, res = emu.key_down(name)
        applied.append('key %s down=%s' % (name, ok))
    for spec in args.key_press:
        name, _, ms = spec.partition(':')
        ok, res = emu.key_press(name, int(ms) if ms else 80)
        applied.append('key_press %s=%s' % (name, ok))
    for hx in args.rf_rx:
        try:
            emu.rf_inject_rx(bytes.fromhex(hx.replace(' ', '')))
            applied.append('rf_rx %s' % hx)
        except Exception as e:
            applied.append('rf_rx failed: %s' % e)
    if applied and not args.quiet:
        print('injected: %s' % ', '.join(applied))
    if args.jump_main:
        info = emu.skip_to_main_loop(args.warmup)
        if not args.quiet:
            print('fast-boot: jumped to main scheduler 0x%08X after %d instructions'
                  % (info['pc'], info['insn']))

    if args.mem:
        addr = args.mem[0]
        length = args.mem[1] if len(args.mem) > 1 else 64
        data = emu.read_mem(addr, length)
        print('memory 0x%08X..0x%08X:' % (addr, addr + length))
        for off in range(0, length, 16):
            chunk = data[off:off + 16]
            print('  %08X  %s  %s' % (addr + off, chunk.hex(' '),
                                      ''.join(chr(c) if 32 <= c < 127 else '.' for c in chunk)))
        if args.run is None and args.step is None:
            return 0

    if args.regs and not args.quiet:
        print('registers before:')
        r = emu.regs_all()
        print('  ' + '  '.join('%s=0x%08X' % (k, r[k]) for k in ('R0', 'R1', 'R2', 'R3',
                                                                  'R4', 'R5', 'R6', 'R7')))
        print('  ' + '  '.join('%s=0x%08X' % (k, r[k]) for k in ('R8', 'R9', 'R10', 'R11',
                                                                  'R12', 'SP', 'LR', 'PC')))

    t0 = time.time()
    status = None
    if args.step:
        status = emu.step(args.step)
    elif args.run:
        status = emu.run(args.run)
    elapsed = time.time() - t0

    if status is None:
        status = dict(status='idle', pc=emu.reg('PC'), insn=emu.insn_count, faults=0)

    if not args.quiet:
        if args.step:
            print('stepped %d instructions' % status['executed'])
        print('run status: %s, executed=%d, total=%d, pc=0x%08X, %.3fs (%.2f Mips)'
              % (status['status'], status.get('executed', 0), emu.insn_count,
                 status['pc'], elapsed, emu.insn_count / max(elapsed, 1e-9) / 1e6))
        if emu.breakpoints and status['status'] == 'breakpoint':
            print('hit breakpoint at 0x%08X' % status['pc'])
        if emu.faults:
            print('FAULTS (%d):' % len(emu.faults))
            for f in emu.faults[-5:]:
                print('  %-22s pc=0x%08X lr=0x%08X sp=0x%08X addr=%s insn=%d'
                      % (f.kind, f.pc, f.lr, f.sp,
                         ('0x%08X' % f.addr) if f.addr is not None else '-', f.insn))
        else:
            print('no faults')
        print(final_report(emu, elapsed, argv))

    if args.dump:
        path, n = write_trace_log(emu, args.dump)
        print('trace log written: %s (%d entries)' % (path, n))
    if args.screen:
        path, size, lit = write_screen_png(emu, args.screen, args.screen_scale)
        print('screen written: %s (%d bytes) frames=%d lit_pixels=%d'
              % (path, size, emu.lcd.frames, lit))
    if args.bootlog:
        boot, maintrace = write_boot_logs(emu, args.outdir)
        print('boot log written: %s' % boot)
        print('main trace written: %s' % maintrace)
    return 0


if __name__ == '__main__':
    sys.exit(main())
