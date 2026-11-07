"""Interactive debugger console for the AD200 Pro firmware emulator.

Usage:
    python -m sim.cli [firmware.bin]
Commands (type 'help'):
    run [n] | r          run until breakpoint / n instructions
    step [n] | s         single step
    pause
    reset
    regs                 show registers
    mem ADDR [LEN]       hex dump memory
    setmem ADDR HEX      write bytes
    dis ADDR [N]         disassemble
    bp [ADDR]            list / add / remove breakpoints (bp del ADDR)
    trace [N]            recent trace events
    mmio [N]             recent MMIO events
    irq                  NVIC / SysTick state
    dev                  virtual device state
    dev set KEY=VAL      set battery(V) cap_voltage temp auto_charge
    event btn NAME       press/release a button
    rf HEX               inject an RF packet
    symbols [FILTER]     list symbols
    info                 machine summary
    quit
"""
import sys
import cmd


def _hex(s):
    return int(s, 0)


class DebugShell(cmd.Cmd):
    intro = 'AD200 Pro firmware debugger. Type "help" or "run".'

    def __init__(self, machine):
        super().__init__()
        self.m = machine
        self.prompt = '(adbg) '

    # ------------------------------------------------------------- helpers
    def _short(self):
        st = self.m.stats()
        return 'pc=0x%08x %s | insns=%d %.0fms | %s' % (
            st['pc'], st['pc_name'] or '', st['insns'], st['uptime_ms'], st['state'])

    # ------------------------------------------------------------- commands
    def do_run(self, arg):
        n = int(arg) if arg.strip() else 500000
        reason = self.m.run(max_insns=n)
        print('stop:', reason, '|', self._short())

    do_r = do_run

    def do_step(self, arg):
        n = int(arg) if arg.strip() else 1
        try:
            self.m.step(n)
        except Exception as e:
            print('step error:', e)
        print(self._short())

    do_s = do_step

    def do_pause(self, arg):
        self.m.state = 'paused'
        print(self._short())

    def do_reset(self, arg):
        self.m.reset()
        print('reset |', self._short())

    def do_regs(self, arg):
        r = self.m.regs()
        keys = ['r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'r6', 'r7',
                'r8', 'r9', 'r10', 'r11', 'r12', 'sp', 'lr', 'pc', 'xpsr', 'primask']
        for i in range(0, len(keys), 4):
            print('  ' + '  '.join('%s=0x%08x' % (k, r.get(k, 0))
                                   for k in keys[i:i + 4]))

    def do_mem(self, arg):
        parts = arg.split()
        if not parts:
            print('usage: mem ADDR [LEN]')
            return
        addr = _hex(parts[0])
        n = int(parts[1], 0) if len(parts) > 1 else 64
        data = bytes(self.m.read(addr, n))
        for i in range(0, len(data), 16):
            chunk = data[i:i + 16]
            hexs = ' '.join('%02x' % b for b in chunk)
            txt = ''.join(chr(b) if 32 <= b < 127 else '.' for b in chunk)
            print('  %08x  %-47s  %s' % (addr + i, hexs, txt))

    def do_setmem(self, arg):
        parts = arg.split()
        addr = _hex(parts[0])
        data = bytes.fromhex(parts[1])
        self.m.write(addr, data)
        print('wrote %d bytes @ 0x%08x' % (len(data), addr))

    def do_dis(self, arg):
        parts = arg.split()
        addr = _hex(parts[0]) if parts else self.m.regs()['pc']
        n = int(parts[1], 0) if len(parts) > 1 else 12
        for ins in self.m.disasm(addr, n):
            mark = '>' if ins['addr'] == self.m.regs()['pc'] else ' '
            print(' %s %08x  %-10s %-30s %s' % (mark, ins['addr'], ins['bytes'],
                                                ins['text'], ins['symbol'] or ''))

    def do_bp(self, arg):
        parts = arg.split()
        if not parts:
            for a in sorted(self.m.breakpoints):
                print('  0x%08x %s' % (a, self.m.fw.name(a) or ''))
            return
        if parts[0] in ('del', 'clear', '-d'):
            self.m.clear_breakpoint(_hex(parts[1]))
            print('removed 0x%08x' % _hex(parts[1]))
        else:
            self.m.set_breakpoint(_hex(parts[0]))
            print('breakpoint @ 0x%08x %s' % (_hex(parts[0]), self.m.fw.name(_hex(parts[0])) or ''))

    def do_trace(self, arg):
        n = int(arg) if arg.strip() else 30
        for e in self.m.trace(0, n):
            extra = ''
            if e['addr'] is not None:
                extra = '0x%08x %s' % (e['addr'], e['name'] or '')
            print('  %6d %-11s pc=0x%08x %s' % (e['seq'], e['type'], e['pc'] or 0, extra))

    def do_mmio(self, arg):
        n = int(arg) if arg.strip() else 30
        ev = [e for e in self.m.trace(0, 5000) if e['type'].startswith('mmio')][-n:]
        for e in ev:
            print('  %-11s 0x%08x %-22s pc=0x%08x' % (e['type'], e['addr'] or 0,
                                                     e['name'] or '', e['pc'] or 0))

    def do_irq(self, arg):
        st = self.m.bus.peripheral_state()
        print('NVIC enabled :', st['irq_enabled'])
        print('pending      :', st['irq_pending'],
              'systick_pending:', self.m.systick_pending)
        print('SysTick      :', st['systick'], 'countflag:', st['systick_countflag'])

    def do_dev(self, arg):
        parts = arg.split()
        if len(parts) >= 2 and parts[0] == 'set':
            kw = {}
            for kv in parts[1:]:
                k, v = kv.split('=', 1)
                kw[k] = float(v)
            st = self.m.device_set(**kw)
            print({k: st[k] for k in ('battery_mv', 'cap_voltage', 'temp_c')})
            return
        st = self.m.device_state()
        print('battery: %.2f V   capacitor: %.1f V   temp: %.1f C' % (
            st['battery_mv'] / 1000.0, st['cap_voltage'], st['temp_c']))
        print('gpio out:', {k: hex(v) for k, v in st['gpio_outputs'].items()})
        print('gpio in :', st['gpio_inputs'])
        print('adc     :', st['adc'])

    def do_event(self, arg):
        parts = arg.split()
        if not parts:
            print('usage: event btn NAME | event rf HEX')
            return
        if parts[0] == 'btn' and len(parts) >= 2:
            self.m.device_event('button', {'name': parts[1].upper(), 'pressed': True})
            print('button pressed:', parts[1].upper())
        elif parts[0] == 'rf' and len(parts) >= 2:
            data = bytes.fromhex(''.join(parts[1:]).replace(' ', ''))
            self.m.device_event('rf_rx', {'channel': 0, 'bytes': list(data)})
            print('injected RF rx:', data.hex())

    def do_rf(self, arg):
        data = bytes.fromhex(arg.replace(' ', ''))
        self.m.device_event('rf_rx', {'channel': 0, 'bytes': list(data)})
        print('injected RF rx:', data.hex())

    def do_symbols(self, arg):
        flt = arg.strip().lower()
        for a, n in sorted(self.m.fw.symbols.items()):
            if not flt or flt in n.lower():
                print('  0x%08x %s' % (a, n))

    def do_info(self, arg):
        st = self.m.status()
        print('state:', st['state'], 'stop:', st['stop_reason'])
        print('pc   : 0x%08x %s' % (st['pc'], st['pc_name'] or ''))
        print('stats:', st['stats'])
        print('breakpoints:', [hex(a) for a in st['breakpoints']])
        print('firmware:', self.m.fw.path, 'base=0x%08x size=%d' % (self.m.fw.base, self.m.fw.size))
        print('delays patched:', getattr(self.m, 'delays_patched', 0), 'wfi patched:',
              getattr(self.m, 'wfi_patched', 0))

    def do_quit(self, arg):
        return True

    do_exit = do_quit
    do_q = do_quit

    def emptyline(self):
        self.do_step('1')

    def default(self, line):
        print('unknown command:', line)


def main(argv=None):
    from .machine import Machine
    argv = argv or sys.argv[1:]
    path = argv[0] if argv else None
    m = Machine(path)
    print('loaded %s (image base 0x%08x, %d bytes)' % (m.fw.path, m.fw.base, m.fw.size))
    print('reset: sp=0x%08x pc=0x%08x %s' % (m.fw.reset_sp, m.fw.reset_pc,
                                            m.fw.name(m.fw.reset_pc) or ''))
    DebugShell(m).cmdloop()


if __name__ == '__main__':
    main()
