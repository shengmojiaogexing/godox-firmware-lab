#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""HTTP status/control service for the AD100 Pro emulator (stdlib only).

Implements the contract from docs/INTERFACES.md plus the optional extensions
requested by the web UI:

  GET  /api/state                emulator state JSON (regs, insnCount, uart, ...)
  GET  /api/disasm?addr=&n=      disassembly lines at addr
  GET  /api/mem?addr=&len=       memory hex dump
  POST /api/cmd                  {cmd: run|pause|step|step_over|reset|bp_add|bp_del|set_pc|set_reg|...}
  GET  /api/screen               decoded LCD frame buffer (1bpp)
  GET  /api/functions            function table [{addr,size,name}]
  GET  /api/periph               peripheral register snapshot + MMIO summary
  GET  /api/trace?n=             recent MMIO trace entries
  OPTIONS *                      CORS preflight (Access-Control-Allow-Origin: *)

Run:  python emu/server.py [--host 127.0.0.1] [--port 8765] [--calib blank]
"""

import argparse
import json
import os
import sys
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

HERE = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(HERE)
if HERE not in sys.path:
    sys.path.insert(0, HERE)

from core import (Emulator, DEFAULT_FIRMWARE, DEFAULT_FUNCTIONS, APP_BASE,
                  ROM_BASE, ROM_SIZE, RAM_BASE)

CHUNK = 100_000            # instructions per worker slice
MAX_LOG = 400
STATE_LOG_MAX = 200


def make_png_1bpp(width, height, bits, scale=6, invert=True):
    """Encode a page-format 1bpp framebuffer (RAM 0x200004CC) as RGB PNG."""
    import struct as _struct
    import zlib as _zlib
    w, h = width * scale, height * scale
    fg, bg = (240, 244, 250), (14, 18, 24)
    raw = bytearray()
    for y in range(h):
        page_row = (y // scale) // 8
        bit = (y // scale) % 8
        raw.append(0)
        line = bytearray()
        for x in range(w):
            col = x // scale
            idx = page_row * width + col
            byte = bits[idx] if idx < len(bits) else 0
            v = (byte >> bit) & 1
            if invert:
                v ^= 1
            line += bytes(fg if v else bg)
        raw += line

    def chunk(tag, data):
        return (_struct.pack('>I', len(data)) + tag + data +
                _struct.pack('>I', _zlib.crc32(tag + data) & 0xFFFFFFFF))

    ihdr = _struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0)
    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', ihdr) +
            chunk(b'IDAT', _zlib.compress(bytes(raw), 9)) + chunk(b'IEND', b''))


class EmuService:
    """Owns the single emulator instance and serialises access to it."""

    def __init__(self, firmware=DEFAULT_FIRMWARE, functions=DEFAULT_FUNCTIONS,
                 calib='blank', max_trace=200_000):
        self.lock = threading.RLock()
        self.emu = Emulator(firmware, functions, trace=True,
                            max_trace=max_trace, calib=calib)
        self.running = False
        self.stop_flag = False
        self.worker = None
        self.started_at = None
        self.log = []
        self._last_insn = 0

    # -- logging ------------------------------------------------------------
    def log_line(self, text):
        self.log.append('%s %s' % (time.strftime('%H:%M:%S'), text))
        if len(self.log) > MAX_LOG:
            del self.log[:len(self.log) - MAX_LOG]

    def _drain_events(self):
        """Turn new trace entries / faults into human-readable log lines."""
        try:
            new = self.emu.trace[-STATE_LOG_MAX:]
        except Exception:
            return
        for t in new:
            self.log.append(t.line())
        if len(self.log) > MAX_LOG:
            del self.log[:len(self.log) - MAX_LOG]

    # -- state --------------------------------------------------------------
    def state(self):
        with self.lock:
            emu = self.emu
            regs = emu.regs_all()
            order = ['R0', 'R1', 'R2', 'R3', 'R4', 'R5', 'R6', 'R7', 'R8', 'R9',
                     'R10', 'R11', 'R12', 'SP', 'LR', 'PC']
            callstack = self._callstack()
            return dict(
                pc=regs['PC'], sp=regs['SP'], lr=regs['LR'],
                xpsr=emu.reg('CPSR'),
                regs=[regs[k] for k in order],
                insnCount=emu.insn_count,
                running=self.running,
                breakpoints=sorted(emu.breakpoints),
                log=self.log[-STATE_LOG_MAX:],
                faults=[f.to_dict() for f in emu.faults[-20:]],
                uart=emu.uart_text()[-4000:],
                uartLen=len(emu.uart_out),
                func=(lambda f: dict(addr=f[0], name=f[1]) if f else None)(emu.func_at(regs['PC'])),
                callstack=callstack,
                calib=emu.calib,
                lcd=emu.screen_info(),
                inputs=emu.inputs.state(),
                pcName=emu.pc_name(),
                taskPhase=emu.task_phase(),
                inMainLoop=bool(getattr(emu, 'in_main_loop', False)),
                virtualTicks=getattr(emu, 'virtual_ticks', 0),
                tim14Count=getattr(emu, '_tim14_count', 0),
                irqCounts=dict(getattr(emu, 'irq_counts', {})),
                irq_counts=dict(getattr(emu, 'irq_counts', {})),
                rfInitialized=bool(getattr(emu, 'rf_initialized', False)),
                irqLog=list(getattr(emu, '_irq_dispatch_log', [])[-40:]),
                inIsr=bool(getattr(emu, '_in_isr', False)),
                realIrq=bool(getattr(emu, 'real_irq', False)),
                rf=(emu.rf_status() if hasattr(emu, 'rf_status') else None),
                lastScreenSave=getattr(self, 'last_screen_path', None),
                stats=dict(executed=emu.insn_count, runTime=round(emu.run_time, 3),
                           virtualTicks=getattr(emu, 'virtual_ticks', 0),
                           tim14=getattr(emu, '_tim14_count', 0),
                           irqTotal=sum(getattr(emu, 'irq_counts', {}).values())),
                peripherals=self._peripherals_brief(),
            )

    def _callstack(self):
        """Approximate call stack: current function + LR chain in RAM/ROM."""
        emu = self.emu
        stack = []
        pc = emu.reg('PC')
        f = emu.func_at(pc)
        if f:
            stack.append(dict(addr=f[0], name=f[1]))
        lr = emu.reg('LR')
        for _ in range(6):
            if lr in (0, 0xFFFFFFFF) or not (ROM_BASE <= lr < ROM_BASE + ROM_SIZE):
                break
            f = emu.func_at(lr & ~1)
            if f:
                stack.append(dict(addr=f[0], name=f[1], return_pc=lr))
            # follow the saved LR on the stack
            try:
                sp = emu.reg('SP')
                lr = emu.read_u32(sp) if sp else 0
            except Exception:
                break
        return stack

    def _peripherals_brief(self):
        emu = self.emu
        return {
            'RCC': emu.rcc.state(),
            'SYSCFG': emu.syscfg.state(),
            'FLASH': emu.flash.state(),
            'RTC': emu.rtc.state(),
            'IWDG': emu.iwdg.state(),
            'ADC': emu.adc.state(),
            'SysTick': emu.systick.state(),
            'GPIOA': emu.devices['GPIOA'].state(),
            'GPIOB': emu.devices['GPIOB'].state(),
            'GPIOC': emu.devices['GPIOC'].state(),
        }

    # -- commands -----------------------------------------------------------
    def cmd(self, payload):
        cmd = (payload.get('cmd') or '').strip().lower()
        emu = self.emu
        with self.lock:
            if cmd == 'run':
                count = payload.get('count') or payload.get('n') or 0
                return self.start_run(int(count) if count else None)
            if cmd == 'pause':
                return self.pause()
            if cmd == 'step':
                n = int(payload.get('n') or payload.get('count') or 1)
                st = emu.step(n)
                self._drain_events()
                self.log_line('step %d -> pc=0x%08X insn=%d' % (n, st['pc'], st['insn']))
                return dict(ok=True, **st)
            if cmd == 'step_over':
                return self.step_over()
            if cmd == 'reset':
                self.stop_flag = True
                self._join_worker()
                emu.reset()
                self.log_line('reset -> pc=0x%08X insn=0' % emu.reg('PC'))
                return dict(ok=True, pc=emu.reg('PC'), insn=0)
            if cmd in ('bp_add', 'bp_del', 'breakpoint_add', 'breakpoint_del'):
                addrs = payload.get('addrs') or payload.get('addr')
                if isinstance(addrs, (int, str)):
                    addrs = [addrs]
                elif addrs is None and payload.get('bp') is not None:
                    addrs = [payload['bp']]
                out = []
                for a in addrs or []:
                    addr = int(a, 0) if isinstance(a, str) else int(a)
                    if cmd in ('bp_add', 'breakpoint_add'):
                        emu.bp_add(addr)
                    else:
                        emu.bp_del(addr)
                    out.append(addr & ~1)
                self.log_line('%s %s' % (cmd, ['0x%08X' % a for a in out]))
                return dict(ok=True, breakpoints=sorted(emu.breakpoints), changed=out)
            if cmd == 'set_pc':
                addr = int(payload.get('addr') or payload.get('value'), 0) \
                    if isinstance(payload.get('addr') or payload.get('value'), str) \
                    else int(payload.get('addr') or payload.get('value'))
                emu.set_pc(addr)
                self.log_line('set_pc 0x%08X' % addr)
                return dict(ok=True, pc=emu.reg('PC'))
            if cmd == 'set_reg':
                name = payload.get('name') or payload.get('reg')
                value = payload.get('value')
                value = int(value, 0) if isinstance(value, str) else int(value)
                emu.set_reg(name, value)
                self.log_line('set_reg %s=0x%08X' % (name, value))
                return dict(ok=True, regs=emu.regs_all())
            if cmd == 'set_mem':
                addr = payload.get('addr')
                addr = int(addr, 0) if isinstance(addr, str) else int(addr)
                data = payload.get('hex') or payload.get('data') or ''
                raw = bytes.fromhex(data) if isinstance(data, str) else bytes(data)
                emu.write_mem(addr, raw)
                self.log_line('set_mem 0x%08X %d bytes' % (addr, len(raw)))
                return dict(ok=True, addr=addr, len=len(raw))
            if cmd == 'inject':
                kind = payload.get('kind')
                value = payload.get('value')
                if kind == 'uart':
                    emu.inject_uart(bytes.fromhex(value) if isinstance(value, str) else bytes(value))
                elif kind == 'adc':
                    emu.inject_adc(int(value) if not isinstance(value, str) else int(value, 0))
                elif kind == 'key':
                    emu.inject_key(payload.get('port', 'B'), int(payload.get('pin', 0)),
                                   int(payload.get('value', 0)))
                elif kind == 'spi':
                    emu.inject_spi(bytes.fromhex(value) if isinstance(value, str) else bytes(value))
                elif kind == 'exti':
                    emu.inject_exti(int(value) if not isinstance(value, str) else int(value, 0))
                self.log_line('inject %s %r' % (kind, value))
                return dict(ok=True)
            # ---- virtual inputs (interactive panel) -----------------------
            if cmd in ('key', 'key_down'):
                name = payload.get('key') or payload.get('name')
                down = payload.get('down', True)
                if isinstance(down, str):
                    down = down.strip().lower() not in ('0', 'false', 'no', 'up')
                ok, res = emu.key_down(name) if down else emu.key_up(name)
                self.log_line('key %s down=%s -> %s' % (name, down, res))
                if not ok:
                    return dict(ok=False, error=res)
                return dict(ok=True, key=res, down=bool(down),
                            inputs=emu.inputs.state())
            if cmd == 'key_up':
                name = payload.get('key') or payload.get('name')
                ok, res = emu.key_up(name)
                self.log_line('key_up %s -> %s' % (name, res))
                if not ok:
                    return dict(ok=False, error=res)
                return dict(ok=True, key=res, down=False,
                            inputs=emu.inputs.state())
            if cmd == 'key_press':
                name = payload.get('key') or payload.get('name')
                ms = int(payload.get('ms') or payload.get('duration') or 80)
                ok, res = emu.key_press(name, ms)
                self.log_line('key_press %s %dms -> %s' % (name, ms, res))
                if not ok:
                    return dict(ok=False, error=res)
                return dict(ok=True, key=res, ms=ms,
                            release_at=emu.inputs.release_at.get(res),
                            inputs=emu.inputs.state())
            if cmd in ('battery', 'set_battery'):
                mv = payload.get('mv')
                if mv is None:
                    mv = payload.get('value') or payload.get('battery_mv') or 3700
                raw = emu.set_battery(int(mv))
                self.log_line('battery %s mV (raw=%d)' % (mv, raw))
                return dict(ok=True, battery_mv=emu.inputs.battery_mv,
                            battery_pct=emu.inputs.battery_pct(), raw=raw,
                            inputs=emu.inputs.state())
            if cmd == 'adc':
                ch = payload.get('ch', payload.get('channel'))
                mv = payload.get('mv', payload.get('value'))
                if ch is None:
                    return dict(ok=False, error='adc needs ch')
                raw = emu.set_adc(int(ch), int(mv or 0))
                self.log_line('adc ch%s=%s mV (raw=%d)' % (ch, mv, raw))
                return dict(ok=True, ch=int(ch), mv=int(mv or 0), raw=raw,
                            inputs=emu.inputs.state())
            if cmd in ('clear_inputs', 'key_clear'):
                emu.clear_inputs()
                self.log_line('clear_inputs')
                return dict(ok=True, inputs=emu.inputs.state())
            if cmd in ('screen_save', 'save_screen', 'screenshot'):
                return self.screen_save(payload.get('path'))
            # ---- RF / IRQ (real handler dispatch) -------------------------
            if cmd in ('rf_rx', 'rf_inject_rx', 'inject_rf'):
                data = payload.get('hex') or payload.get('data') or payload.get('payload') or ''
                if isinstance(data, str):
                    raw = bytes.fromhex(data.replace(' ', ''))
                else:
                    raw = bytes(data)
                try:
                    rf = emu.rf_inject_rx(raw,
                                          rssi=int(payload.get('rssi') or 0x28),
                                          lqi=int(payload.get('lqi') or 0x10))
                except Exception as e:
                    return dict(ok=False, error=str(e))
                self.log_line('rf_rx %d bytes -> GDO0/EXTI15/IRQ7' % len(raw))
                return dict(ok=True, len=len(raw), rf=rf, irqCounts=dict(emu.irq_counts))
            if cmd in ('rf_init', 'init_rf'):
                try:
                    rf = emu.rf_init(int(payload.get('max_insns') or 3_000_000))
                except Exception as e:
                    return dict(ok=False, error=str(e))
                self.log_line('rf_init: state=%s freq2=0x%02X writes=%d'
                              % (rf.get('state'), rf.get('freq2', 0),
                                 len(getattr(emu.cc2500, 'spi_writes', []))))
                return dict(ok=True, rf=rf,
                            rfInitialized=bool(getattr(emu, 'rf_initialized', False)),
                            irqCounts=dict(emu.irq_counts),
                            pc=emu.reg('PC'), insn=emu.insn_count)
            if cmd in ('rf_status', 'rf'):
                return dict(ok=True, rf=emu.rf_status())
            if cmd in ('irq_status', 'irqs'):
                return dict(ok=True, irqCounts=dict(emu.irq_counts),
                            irqLog=list(emu._irq_dispatch_log[-40:]),
                            inIsr=bool(emu._in_isr), realIrq=bool(emu.real_irq))
            # ---- fast boot / interactive mode -----------------------------
            if cmd in ('jump_main', 'skip_boot', 'fastboot'):
                warmup = int(payload.get('warmup') or payload.get('n') or 1_000_000)
                try:
                    self.stop_flag = True
                    self._join_worker()
                    info = emu.skip_to_main_loop(warmup)
                    self._drain_events()
                    return dict(ok=True, pc=emu.reg('PC'),
                                insn=emu.insn_count,
                                pcName=emu.pc_name(),
                                taskPhase=emu.task_phase(),
                                inputs=emu.inputs.state())
                except Exception as e:
                    import traceback
                    traceback.print_exc()
                    self.log_line('jump_main failed: %s' % e)
                    return dict(ok=False, error=str(e), pc=emu.reg('PC'),
                                insn=emu.insn_count)
            if cmd in ('boot_interactive', 'boot', 'start'):
                warmup = int(payload.get('warmup') or payload.get('n') or 1_000_000)
                with_rf = payload.get('rf', payload.get('with_rf', True))
                if isinstance(with_rf, str):
                    with_rf = with_rf.strip().lower() not in ('0', 'false', 'no')
                try:
                    self.stop_flag = True
                    self._join_worker()
                    info = emu.boot_interactive(
                        warmup=warmup, with_rf=bool(with_rf),
                        settle=int(payload.get('settle') or 200_000),
                        pre_settle=int(payload.get('pre_settle') or 200_000))
                    self._drain_events()
                    self.log_line('boot_interactive: pc=0x%08X phase=%s rf=%s'
                                  % (emu.reg('PC'), emu.task_phase(),
                                     (info.get('rf') or {}).get('state')))
                    info.update(dict(pc=emu.reg('PC'), insn=emu.insn_count,
                                     pcName=emu.pc_name(),
                                     taskPhase=emu.task_phase(),
                                     inputs=emu.inputs.state(),
                                     lcd=emu.screen_info(),
                                     irqCounts=dict(emu.irq_counts),
                                     irq_counts=dict(emu.irq_counts)))
                    return info
                except Exception as e:
                    import traceback
                    traceback.print_exc()
                    self.log_line('boot_interactive failed: %s' % e)
                    return dict(ok=False, error=str(e), pc=emu.reg('PC'),
                                insn=emu.insn_count)
        return dict(ok=False, error='unknown cmd: %r' % (cmd or payload))

    # -- screen save --------------------------------------------------------
    def screen_save(self, path=None):
        emu = self.emu
        with self.lock:
            bits = emu.screen_bits()
            info = emu.screen_info()
        if not path:
            path = os.path.join(PROJECT_ROOT, 'out',
                                'screen_%s.png' % time.strftime('%H%M%S'))
        elif not os.path.isabs(path):
            path = os.path.join(PROJECT_ROOT, path)
        try:
            png = make_png_1bpp(emu.fb_width, emu.fb_height, bits, scale=6, invert=True)
            os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
            with open(path, 'wb') as f:
                f.write(png)
        except Exception as e:
            self.log_line('screen_save failed: %s' % e)
            return dict(ok=False, error=str(e))
        self.last_screen_path = path
        nonzero = sum(1 for b in bits if b)
        self.log_line('screen_save -> %s (%d bytes, %d nonzero FB bytes)'
                      % (path, len(png), nonzero))
        return dict(ok=True, path=path, file=path, saved=path,
                    bytes=len(png), frames=info.get('frames', 0),
                    insnCount=emu.insn_count, nonzero=nonzero,
                    w=emu.fb_width, h=emu.fb_height)


    # -- run worker ---------------------------------------------------------
    def start_run(self, count=None):
        self._join_worker()
        self.stop_flag = False
        self.running = True
        self.started_at = time.time()
        self.log_line('run start count=%s at pc=0x%08X' % (count, self.emu.reg('PC')))
        self.worker = threading.Thread(target=self._run_loop, args=(count,), daemon=True)
        self.worker.start()
        return dict(ok=True, running=True, count=count, pc=self.emu.reg('PC'))

    def pause(self):
        self.stop_flag = True
        self._join_worker()
        self.log_line('paused at pc=0x%08X insn=%d' % (self.emu.reg('PC'), self.emu.insn_count))
        return dict(ok=True, running=False, pc=self.emu.reg('PC'), insn=self.emu.insn_count)

    def _join_worker(self):
        w = self.worker
        if w is not None and w.is_alive():
            w.join(timeout=15)
        self.worker = None
        self.running = False

    def _run_loop(self, count):
        remaining = count
        try:
            while not self.stop_flag:
                n = CHUNK if remaining is None else min(CHUNK, remaining)
                with self.lock:
                    before = self.emu.insn_count
                    st = self.emu.run(n)
                    executed = self.emu.insn_count - before
                    self._drain_events()
                if executed <= 0:
                    self.log_line('run stopped (no progress, status=%s)' % st['status'])
                    break
                if remaining is not None:
                    remaining -= executed
                    if remaining <= 0:
                        break
                if st['status'] not in ('ok', 'breakpoint'):
                    self.log_line('run stopped: %s at pc=0x%08X' % (st['status'], st['pc']))
                    break
                if st['status'] == 'breakpoint':
                    self.log_line('breakpoint hit at pc=0x%08X' % st['pc'])
                    break
        finally:
            with self.lock:
                self.running = False
                self.emu.run_time = getattr(self.emu, 'run_time', 0.0)
            self.log_line('run end pc=0x%08X insn=%d' % (self.emu.reg('PC'), self.emu.insn_count))

    # -- disassembly / memory ----------------------------------------------
    def disasm(self, addr, n=16):
        emu = self.emu
        try:
            data = emu.read_mem(addr & ~1, max(4, n * 4))
        except Exception:
            return dict(addr=addr, lines=[])
        try:
            import capstone
            md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
            md.detail = False
            lines = []
            for ins in md.disasm(bytes(data), addr & ~1):
                lines.append(dict(addr=ins.address, bytes=ins.bytes.hex(),
                                  mnem=ins.mnemonic, op=ins.op_str))
                if len(lines) >= n:
                    break
            return dict(addr=addr & ~1, lines=lines)
        except Exception as e:
            return dict(addr=addr, error=str(e), lines=[])

    def mem(self, addr, length=64):
        length = max(1, min(int(length), 0x10000))
        try:
            data = bytes(self.emu.read_mem(addr, length))
        except Exception as e:
            return dict(addr=addr, hex='', error=str(e))
        return dict(addr=addr, hex=data.hex(), len=length,
                    ascii=''.join(chr(c) if 32 <= c < 127 else '.' for c in data))

    def screen(self):
        emu = self.emu
        bits = emu.screen_bits()
        info = emu.screen_info()
        return dict(w=emu.lcd.w, h=emu.lcd.h, format='1bpp',
                    addr=0, hex=bits.hex(), data=list(bits),
                    info=info)

    def functions(self):
        out = []
        for a, b, name in self.emu.functions.entries:
            out.append(dict(addr=a, end=b, size=b - a, name=name))
        return out

    def trace(self, n=100):
        with self.lock:
            entries = self.emu.trace[-int(n):]
            return [t.to_dict() for t in entries]

    def periph(self):
        with self.lock:
            return dict(state=self._peripherals_brief(),
                        summary=self.emu.periph_summary())


SERVICE = None


class Handler(BaseHTTPRequestHandler):
    server_version = 'AD100ProEmu/1.0'
    protocol_version = 'HTTP/1.1'

    # -- plumbing -----------------------------------------------------------
    def _cors(self):
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type, Authorization')
        self.send_header('Access-Control-Max-Age', '86400')

    def _json(self, obj, status=200):
        body = json.dumps(obj, ensure_ascii=False).encode('utf-8')
        self.send_response(status)
        self._cors()
        self.send_header('Content-Type', 'application/json; charset=utf-8')
        self.send_header('Content-Length', str(len(body)))
        self.end_headers()
        try:
            self.wfile.write(body)
        except (BrokenPipeError, ConnectionResetError):
            pass

    def _params(self):
        q = parse_qs(urlparse(self.path).query)
        return {k: (v[0] if len(v) == 1 else v) for k, v in q.items()}

    def log_message(self, fmt, *args):
        if os.environ.get('EMU_HTTP_LOG'):
            sys.stderr.write('[http] %s\n' % (fmt % args))

    # -- routes -------------------------------------------------------------
    def do_OPTIONS(self):
        self.send_response(204)
        self._cors()
        self.send_header('Content-Length', '0')
        self.end_headers()

    def do_GET(self):
        p = urlparse(self.path).path.rstrip('/') or '/'
        q = self._params()
        svc = SERVICE
        try:
            if p in ('/', '/api'):
                self._json(dict(service='AD100ProEmulator',
                                endpoints=['/api/state', '/api/disasm', '/api/mem',
                                           '/api/cmd', '/api/screen', '/api/functions',
                                           '/api/periph', '/api/trace'],
                                pc=svc.emu.reg('PC'), insnCount=svc.emu.insn_count))
                return
            if p == '/api/state':
                self._json(svc.state())
                return
            if p == '/api/disasm':
                addr = int(q.get('addr', '0'), 0)
                n = int(q.get('n', q.get('len', 16)))
                self._json(svc.disasm(addr, n))
                return
            if p == '/api/mem':
                addr = int(q.get('addr', '0'), 0)
                length = int(q.get('len', q.get('n', 64)))
                self._json(svc.mem(addr, length))
                return
            if p == '/api/screen':
                self._json(svc.screen())
                return
            if p == '/api/functions':
                self._json(svc.functions())
                return
            if p == '/api/periph':
                self._json(svc.periph())
                return
            if p == '/api/trace':
                self._json(dict(entries=svc.trace(int(q.get('n', 100)))))
                return
            if p == '/api/health':
                self._json(dict(ok=True, ts=time.time()))
                return
            self._json(dict(error='not found', path=p), 404)
        except Exception as e:
            import traceback
            traceback.print_exc()
            self._json(dict(error=str(e)), 500)

    def do_POST(self):
        p = urlparse(self.path).path.rstrip('/') or '/'
        try:
            length = int(self.headers.get('Content-Length') or 0)
            raw = self.rfile.read(length) if length else b'{}'
            try:
                payload = json.loads(raw.decode('utf-8') or '{}')
            except Exception:
                payload = {}
        except Exception as e:
            self._json(dict(ok=False, error='bad request: %s' % e), 400)
            return
        try:
            if p in ('/api/cmd', '/api/command'):
                self._json(SERVICE.cmd(payload))
                return
            self._json(dict(ok=False, error='not found', path=p), 404)
        except Exception as e:
            import traceback
            traceback.print_exc()
            self._json(dict(ok=False, error=str(e)), 500)


def write_screen_json(path):
    """INTERFACES.md item 4: emulator publishes emu/screen.json with the LCD layout."""
    svc = SERVICE
    info = svc.emu.screen_info()
    obj = dict(w=info['w'], h=info['h'], addr=0, format='1bpp',
               page=info['page'], col=info['col'], frames=info['frames'])
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, 'w', encoding='utf-8') as f:
        json.dump(obj, f, indent=1)


def main(argv=None):
    global SERVICE
    ap = argparse.ArgumentParser(description='AD100 Pro emulator HTTP service')
    ap.add_argument('--host', default='127.0.0.1')
    ap.add_argument('--port', type=int, default=8765)
    ap.add_argument('--firmware', default=DEFAULT_FIRMWARE)
    ap.add_argument('--functions', default=DEFAULT_FUNCTIONS)
    ap.add_argument('--calib', choices=['blank', 'synthetic'], default='blank')
    ap.add_argument('--max-trace', type=int, default=200_000)
    ap.add_argument('--auto-boot', dest='auto_boot', action='store_true', default=True,
                    help='boot into the interactive 1 ms scheduler at startup (default)')
    ap.add_argument('--no-auto-boot', dest='auto_boot', action='store_false',
                    help='start at the reset vector instead')
    ap.add_argument('--warmup', type=int, default=1_000_000,
                    help='instructions to warm up before jumping to the main scheduler')
    args = ap.parse_args(argv)

    SERVICE = EmuService(args.firmware, args.functions, args.calib, args.max_trace)
    if args.auto_boot:
        try:
            SERVICE.emu.skip_to_main_loop(args.warmup)
            SERVICE.emu.run(200_000)
            SERVICE.log_line('auto-boot: main loop pc=0x%08X insn=%d'
                             % (SERVICE.emu.reg('PC'), SERVICE.emu.insn_count))
        except Exception as e:
            SERVICE.log_line('auto-boot failed: %s' % e)
    SERVICE.log_line('service ready: reset pc=0x%08X sp=0x%08X'
                     % (SERVICE.emu.reg('PC'), SERVICE.emu.reg('SP')))
    try:
        write_screen_json(os.path.join(HERE, 'screen.json'))
    except Exception:
        pass

    httpd = ThreadingHTTPServer((args.host, args.port), Handler)
    httpd.daemon_threads = True
    url = 'http://%s:%d' % (args.host, args.port)
    print('AD100 Pro emulator HTTP service listening on %s' % url)
    print('  GET  %s/api/state' % url)
    print('  POST %s/api/cmd' % url)
    print('  CORS enabled (Access-Control-Allow-Origin: *)')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        print('\nstopping...')
    finally:
        httpd.shutdown()
    return 0


if __name__ == '__main__':
    sys.exit(main())
