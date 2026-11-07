"""debug.py -- interactive / scriptable debugger for the X3-C firmware emulator.

Two ways to use it:

1. Interactive REPL::

       tools/python/python.exe devices/x3c/emu/debug.py

   Commands: help, boot, run [n], c/continue, s [n], n (step over), u [addr]
   [count], x addr [len], r, b name addr, bl, bd name, watch lo hi [r|w],
   rs, bt, mmio [n], hot [n], unknown, map, fb [base w h fmt], scan, png base w
   h fmt file, symbols [file], savecsv file, reset, state, q/quit.

2. Scripted / non-interactive::

       python debug.py --run 200000 --scan-fb --dump-fb 0x20025000 240 240 rgb565

   or ``--script commands.dbg`` for a file of the same commands (lines starting
   with '#' are comments).

Python API::

    from debug import Debugger
    d = Debugger(image=..., base=0x08004800)
    d.boot(); d.run(1000000)
    d.breakpoint("main", 0x0800....); d.run()
    print(d.registers()); d.save_png(0x20025000, 240, 240, "rgb565", "fb.png")
"""
from __future__ import annotations

import argparse
import cmd
import os
import shlex
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

from machine import Machine, DEFAULT_IMAGE, APP_BASE, load_symbol_file  # noqa
import display as display_mod                                           # noqa

try:
    from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
    _MD = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
except Exception:                                                       # pragma: no cover
    _MD = None


def _hex(v):
    return int(v, 0)


class Debugger:
    def __init__(self, image=DEFAULT_IMAGE, base=APP_BASE, log=print):
        self.log = log
        self.m = Machine(image=os.path.abspath(image), image_base=base,
                         log_fn=log)
        self.m.symbols.update(load_symbol_file(os.path.join(HERE, "symbols.json")))
        self.m.call_trace_enabled = True
        self.breakpoints = {}
        self.watchpoints = []
        self.last_stop = None
        self.step_over_pc = None

    # -- symbols -------------------------------------------------------
    def add_symbol(self, name, addr):
        self.m.symbols[name] = addr

    def resolve(self, expr):
        """Resolve a symbol name or numeric address expression."""
        if isinstance(expr, int):
            return expr
        expr = expr.strip()
        if expr in self.m.symbols:
            return self.m.symbols[expr]
        if expr.startswith("+") or expr.startswith("-"):
            return self.resolve(expr[1:]) + (1 if expr.startswith("+") else -1)
        try:
            return int(expr, 0)
        except ValueError:
            pass
        # allow "symbol+0x10"
        for op in ("+", "-"):
            if op in expr:
                a, b = expr.split(op, 1)
                try:
                    return self.resolve(a) + (self.resolve(b) if op == "+"
                                              else -self.resolve(b))
                except Exception:
                    continue
        raise KeyError("cannot resolve %r" % (expr,))

    def symbolize(self, addr):
        for name, a in self.m.symbols.items():
            if a == addr or a == (addr | 1):
                return name
        return None

    # -- execution control ---------------------------------------------
    def boot(self):
        sp, pc = self.m.boot()
        self.log("boot: SP=%#x PC=%#x (vector table @ %#x)"
                 % (sp, pc, self.m.vtor))

    def run(self, max_insns=2000000, chunk=2000):
        reason = self.m.run(max_insns=max_insns, chunk=chunk)
        self.last_stop = reason
        return reason

    def cont(self, max_insns=2000000):
        return self.run(max_insns)

    def step(self, n=1):
        return self.m.step(n)

    def step_over(self):
        return self.m.step_over()

    def until(self, addr, max_insns=5000000):
        return self.m.run_until(self.resolve(addr), max_insns=max_insns)

    def reset(self):
        self.m.soft_reset()

    # -- breakpoints ----------------------------------------------------
    def breakpoint(self, name, addr):
        a = self.resolve(addr)
        self.m.breakpoints[a] = name
        self.breakpoints[a] = name
        return a

    def delete_breakpoint(self, name_or_addr):
        try:
            a = self.resolve(name_or_addr)
        except Exception:
            a = None
        if a in self.m.breakpoints:
            del self.m.breakpoints[a]
            self.breakpoints.pop(a, None)
            return True
        for a, n in list(self.m.breakpoints.items()):
            if n == name_or_addr:
                del self.m.breakpoints[a]
                self.breakpoints.pop(a, None)
                return True
        return False

    def watchpoint(self, lo, hi, kind="w"):
        lo, hi = self.resolve(lo), self.resolve(hi)

        def cb(uc, access, address, size, value, user_data):
            self.log("[watchpoint] %s @ %#x size=%d value=%#x pc=%#x"
                     % ("W" if access == 2 else "R", address, size, value,
                        uc.reg_read(15)))
        self.m.add_mem_hook(lo, hi, kind, cb)
        self.watchpoints.append((lo, hi, kind))
        return lo, hi

    # -- inspection -----------------------------------------------------
    def registers(self):
        return self.m.register_string()

    def read_mem(self, addr, length):
        return self.m.read_bytes(self.resolve(addr), length)

    def hexdump(self, addr, length=64):
        a = self.resolve(addr)
        data = self.read_mem(a, length)
        lines = []
        for off in range(0, len(data), 16):
            chunk = data[off:off + 16]
            hx = " ".join("%02x" % b for b in chunk)
            asc = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
            lines.append("%08x  %-47s  %s" % (a + off, hx, asc))
        return "\n".join(lines)

    def disasm(self, addr=None, count=16):
        if _MD is None:
            return "(capstone not available)"
        if addr is None:
            addr = self.m.reg("PC")
        a = self.resolve(addr) & ~1
        try:
            code = bytes(self.m.uc.mem_read(a, count * 4))
        except Exception as e:
            return "cannot read %#x: %r" % (a, e)
        lines = []
        for insn in _MD.disasm(code, a):
            sym = self.symbolize(insn.address)
            lines.append("%08x  %-8s %-24s %s"
                         % (insn.address, insn.bytes.hex(), insn.mnemonic,
                            insn.op_str + ("   ; " + sym if sym else "")))
            if len(lines) >= count:
                break
        return "\n".join(lines)

    def map_info(self):
        lines = ["memory map:"]
        lines.append("  flash    %#010x + %#x  (image @ %#010x, %d bytes)"
                     % (self.m.flash_base, self.m.flash_size,
                        self.m.image_base, self.m.image_len))
        lines.append("  RAM      %#010x + %#x" % (self.m.ram_base,
                                                  self.m.ram_size))
        lines.append("  vector   VTOR=%#010x SP=%#010x RESET=%#010x"
                     % (self.m.vtor, self.m.peek_u32(self.m.vtor),
                        self.m.peek_u32(self.m.vtor + 4) & ~1))
        lines.append("  peripherals (%d):" % len(self.m.periphs))
        for p in self.m.periphs:
            lines.append("    %-14s %#010x + %#x" % (p.name, p.base, p.size))
        return "\n".join(lines)

    def mmio_report(self, n=20):
        return self.m.hot_mmio_report(n)

    def unknown_report(self):
        return self.m.unknown_mmio_report(40) or "  (none)"

    def call_summary(self, n=20):
        lines = ["call targets (BL/BLX), %d calls, %d distinct:"
                 % (sum(self.m.call_targets.values()),
                    len(self.m.call_targets))]
        for target, cnt in self.m.call_targets.most_common(n):
            sym = self.symbolize(target)
            lines.append("  %8d  %#010x %s" % (cnt, target, sym or ""))
        return "\n".join(lines)

    # -- display -------------------------------------------------------
    def save_png(self, base, w, h, fmt, path, brighten=True):
        fb = display_mod.FramebufferModel(self.resolve(base), w, h, fmt)
        return fb.save(self.m, path, brighten=brighten)

    def scan_fb(self, outdir=None, ram_base=0x20000000, ram_size=0x80000):
        outdir = outdir or os.path.join(HERE, "logs", "fb_candidates")
        return display_mod.dump_candidates(self.m, outdir, ram_base, ram_size,
                                           log=self.log)

    def export_mmio_csv(self, path):
        self.m.access.csv(path)
        return path

    def state(self):
        return self.m.summary()

    def trace_csv(self, path):
        return self.export_mmio_csv(path)


# ==========================================================================
# CLI
# ==========================================================================
class DebugShell(cmd.Cmd):
    intro = ("X3-C firmware debugger. Type 'help' for commands, 'boot' then "
             "'run 200000'.")
    prompt = "(emu) "

    def __init__(self, dbg):
        super().__init__()
        self.d = dbg

    def _p(self, s):
        if s:
            print(s)

    # -- execution --
    def do_boot(self, arg):
        """boot -- reset from vector table (SP/PC from image base)"""
        self.d.boot()

    def do_run(self, arg):
        """run [n] -- run up to n instructions (default 2,000,000)"""
        n = int(arg, 0) if arg.strip() else 2000000
        reason = self.d.run(n)
        print("stopped: %s" % reason)
        self._p(self.d.registers())

    def do_c(self, arg):
        """c [n] -- continue"""
        self.do_run(arg or "2000000")

    do_continue = do_c

    def do_s(self, arg):
        """s [n] -- single step n instructions"""
        n = int(arg, 0) if arg.strip() else 1
        self.d.step(n)
        self._p(self.d.registers())
        self._p(self.d.disasm(None, 3))

    do_si = do_s

    def do_n(self, arg):
        """n -- step over the next instruction (call)"""
        self.d.step_over()
        self._p(self.d.registers())

    def do_until(self, arg):
        """until addr|symbol -- run until address"""
        print("stopped: %s" % self.d.until(arg))

    def do_reset(self, arg):
        """reset -- soft reset (PC/SP from vector table)"""
        self.d.reset()
        self._p(self.d.registers())

    # -- breakpoints --
    def do_b(self, arg):
        """b name addr -- set breakpoint (addr may be symbol or number)"""
        parts = arg.split()
        if len(parts) < 2:
            print("usage: b name addr")
            return
        a = self.d.breakpoint(parts[0], parts[1])
        print("breakpoint %s @ %#x" % (parts[0], a))

    def do_bl(self, arg):
        """bl -- list breakpoints"""
        for a, n in self.d.m.breakpoints.items():
            print("  %-20s %#010x" % (n, a))

    def do_bd(self, arg):
        """bd name -- delete breakpoint"""
        print("deleted" if self.d.delete_breakpoint(arg.strip()) else "not found")

    def do_watch(self, arg):
        """watch lo hi [r|w] -- watchpoint on a memory range"""
        parts = arg.split()
        if len(parts) < 2:
            print("usage: watch lo hi [r|w]")
            return
        kind = parts[2] if len(parts) > 2 else "w"
        print("watchpoint %s on [%s, %s]" % (kind, parts[0], parts[1]))
        self.d.watchpoint(parts[0], parts[1], kind)

    # -- inspection --
    def do_r(self, arg):
        """r -- registers"""
        self._p(self.d.registers())

    do_regs = do_r

    def do_x(self, arg):
        """x addr [len] -- hex dump"""
        parts = arg.split()
        if not parts:
            print("usage: x addr [len]")
            return
        n = int(parts[1], 0) if len(parts) > 1 else 64
        self._p(self.d.hexdump(parts[0], n))

    def do_u(self, arg):
        """u [addr] [count] -- disassemble"""
        parts = arg.split()
        addr = parts[0] if parts else None
        n = int(parts[1], 0) if len(parts) > 1 else 16
        self._p(self.d.disasm(addr, n))

    def do_sym(self, arg):
        """sym name addr -- add a symbol"""
        parts = arg.split()
        if len(parts) >= 2:
            self.d.add_symbol(parts[0], self.d.resolve(parts[1]))
            print("ok")

    def do_symbols(self, arg):
        """symbols [file] -- load symbol file (json/txt)"""
        path = arg.strip() or os.path.join(HERE, "symbols.json")
        syms = load_symbol_file(path)
        self.d.m.symbols.update(syms)
        print("loaded %d symbols from %s" % (len(syms), path))

    def do_bt(self, arg):
        """bt -- call-trace summary (BL/BLX targets)"""
        self._p(self.d.call_summary(30))

    def do_mmio(self, arg):
        """mmio [n] -- hot MMIO summary"""
        self._p(self.d.mmio_report(int(arg) if arg.strip() else 20))

    def do_hot(self, arg):
        """hot [n] -- hot MMIO addresses"""
        self._p(self.d.mmio_report(int(arg) if arg.strip() else 20))

    def do_unknown(self, arg):
        """unknown -- unmodelled MMIO accesses"""
        self._p(self.d.unknown_report())

    def do_map(self, arg):
        """map -- memory map / peripherals"""
        self._p(self.d.map_info())

    def do_state(self, arg):
        """state -- machine summary"""
        self._p(self.d.state())

    def do_fb(self, arg):
        """fb -- scan SRAM for framebuffer candidates and save PNGs"""
        for p in self.d.scan_fb():
            print(p)

    def do_scan(self, arg):
        """scan -- alias for fb"""
        self.do_fb(arg)

    def do_png(self, arg):
        """png base w h fmt file -- dump a framebuffer region to PNG"""
        parts = arg.split()
        if len(parts) < 5:
            print("usage: png base w h fmt file")
            return
        try:
            p = self.d.save_png(parts[0], int(parts[1]), int(parts[2]),
                                parts[3], parts[4])
            print("wrote %s" % p)
        except Exception as e:
            print("error: %r" % (e,))

    def do_csv(self, arg):
        """csv file -- export MMIO access CSV"""
        print("wrote %s" % self.d.export_mmio_csv(arg.strip()))

    def do_quit(self, arg):
        """q -- quit"""
        return True

    do_q = do_quit
    do_exit = do_quit

    def emptyline(self):
        return False

    def default(self, line):
        print("unknown command %r (try 'help')" % (line,))


def run_script(dbg, lines):
    sh = DebugShell(dbg)
    for line in lines:
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        print(">> %s" % line)
        sh.onecmd(line)
        if dbg.m.stop_reason:
            print("(stop reason: %s)" % dbg.m.stop_reason)


def main(argv=None):
    ap = argparse.ArgumentParser(description="X3-C firmware debugger")
    ap.add_argument("--img", default=DEFAULT_IMAGE)
    ap.add_argument("--base", type=_hex, default=APP_BASE)
    ap.add_argument("--script", help="command script file")
    ap.add_argument("--export", help="command script to create and run")
    ap.add_argument("--run", type=int, default=0,
                    help="run N instructions then act")
    ap.add_argument("--break-at", type=_hex, default=None)
    ap.add_argument("--scan-fb", action="store_true",
                    help="scan SRAM for framebuffers and dump PNGs")
    ap.add_argument("--dump-fb", nargs=4,
                    metavar=("BASE", "W", "H", "FMT"),
                    help="dump a framebuffer to PNG")
    ap.add_argument("--out", default=None, help="output png/script path")
    args = ap.parse_args(argv)

    dbg = Debugger(image=args.img, base=args.base)
    dbg.boot()
    if args.run:
        reason = dbg.run(args.run)
        print("run stopped: %s" % reason)
        print(dbg.registers())
        print(dbg.m.summary())
    if args.break_at:
        dbg.breakpoint("bp", args.break_at)
    if args.scan_fb:
        paths = dbg.scan_fb()
        for p in paths:
            print("fb candidate PNG: %s" % p)
    if args.dump_fb:
        base, w, h, fmt = args.dump_fb
        out = args.out or os.path.join(HERE, "logs", "fb.png")
        print("wrote %s" % dbg.save_png(base, int(w, 0), int(h, 0), fmt, out))
    if args.script:
        with open(args.script, "r", encoding="utf-8") as f:
            run_script(dbg, f.readlines())
    if not (args.run or args.script or args.scan_fb or args.dump_fb):
        try:
            DebugShell(dbg).cmdloop()
        except KeyboardInterrupt:
            print("\nbye")
    return 0


if __name__ == "__main__":
    sys.exit(main())
