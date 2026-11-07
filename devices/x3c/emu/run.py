#!/usr/bin/env python3
"""run.py -- headless boot driver / boot-log generator for the X3-C firmware.

Examples
--------
    python run.py                                # boot the default image
    python run.py --max-insn 200000              # limit instruction budget
    python run.py --trace                        # + access-trace CSV
    python run.py --base 0x08004800 --img path   # custom layout
    python run.py --entry 0x080002F8             # override entry point

Outputs a human-readable report to stdout and a full log to
devices/x3c/emu/logs/boot_<timestamp>.log.
"""
from __future__ import annotations

import argparse
import datetime
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

from machine import (Machine, DEFAULT_IMAGE, APP_BASE, FLASH_BASE,  # noqa: E402
                     load_symbol_file)


def parse_hex(v):
    return int(v, 0)


def analyze_hang(m, n=200):
    pcs = list(m.pc_ring)[-n:]
    if not pcs:
        return None
    tail = pcs[-min(64, len(pcs)):]
    lo, hi = min(tail), max(tail)
    if hi - lo <= 0x40:
        return "tight loop in [%#x, %#x] over last %d instructions" % (
            lo, hi, len(tail))
    return "no tight loop (recent PC range %#x..%#x)" % (lo, hi)


def main(argv=None):
    ap = argparse.ArgumentParser(description="X3-C firmware boot driver")
    ap.add_argument("--img", default=DEFAULT_IMAGE, help="firmware image")
    ap.add_argument("--base", type=parse_hex, default=APP_BASE,
                    help="image load base (default 0x08004800)")
    ap.add_argument("--max-insn", type=int, default=4000000,
                    help="instruction budget (default 4M)")
    ap.add_argument("--entry", type=parse_hex, default=None,
                    help="override entry PC (elf-style even/odd both ok)")
    ap.add_argument("--sp", type=parse_hex, default=None, help="override SP")
    ap.add_argument("--trace", action="store_true",
                    help="write MMIO CSV access trace next to the log")
    ap.add_argument("--no-hf", action="store_true",
                    help="stop on faults instead of taking HardFault")
    ap.add_argument("--quiet", action="store_true", help="only summary to stdout")
    ap.add_argument("--chunk", type=int, default=2000,
                    help="instructions per emulated chunk")
    ap.add_argument("--symbols", default=None, help="symbol file (json/txt)")
    ap.add_argument("--progress", type=int, default=500000,
                    help="log a progress line every N instructions (0=off)")
    ap.add_argument("--fine-insn", type=int, default=200000,
                    help="instructions to run with full per-instruction hook "
                         "before switching to fast burst mode (0=fast only)")
    ap.add_argument("--fine", action="store_true",
                    help="never switch to fast burst mode")
    ap.add_argument("--scan-fb", action="store_true",
                    help="after running, scan SRAM for framebuffer candidates")
    ap.add_argument("--fb", nargs=4, metavar=("BASE", "W", "H", "FMT"),
                    help="dump one framebuffer region: base w h fmt")
    ap.add_argument("--outdir", default=None,
                    help="directory for exported PNGs/CSV (default devices/x3c/emu/logs)")
    args = ap.parse_args(argv)

    ts = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")
    logdir = os.path.join(HERE, "logs")
    os.makedirs(logdir, exist_ok=True)
    logpath = os.path.join(logdir, "boot_%s.log" % ts)
    logf = open(logpath, "w", encoding="utf-8")
    loglines = []

    def log(msg):
        loglines.append(msg)
        if not args.quiet:
            print(msg)
        logf.write(msg + "\n")
        logf.flush()

    log("# X3-C firmware boot log  %s" % datetime.datetime.now().isoformat())
    log("# image=%s" % os.path.abspath(args.img))
    log("# base=%#010x entry=%s max_insn=%d" % (
        args.base, hex(args.entry) if args.entry else "auto", args.max_insn))

    m = Machine(image=os.path.abspath(args.img), image_base=args.base,
                trace=True, log_fn=log, hardfault=not args.no_hf)
    syms = load_symbol_file(args.symbols or os.path.join(HERE, "symbols.json"))
    m.symbols.update(syms)
    if syms:
        log("# loaded %d symbols" % len(syms))

    if args.entry is not None:
        sp = args.sp if args.sp is not None else m.peek_u32(args.base)
        m.reset_from_entry(args.entry, sp)
        log("# entry override: pc=%#x sp=%#x" % (args.entry, sp))
    else:
        sp, pc = m.boot()
        log("# vector table @ %#010x: SP=%#010x RESET=%#010x" % (
            m.vtor, sp, pc))

    log("# ---------- running ----------")
    if args.progress:
        def _progress(mm):
            log("# progress: %d insns  pc=%#010x sp=%#010x lr=%#010x" % (
                mm.insn_count, mm.reg("PC"), mm.reg("SP"), mm.reg("LR")))
        m.progress_fn = _progress
        m.progress_every = args.progress
    stop = None
    fine_budget = args.fine_insn if not args.fine else args.max_insn
    if fine_budget:
        m.set_fine(True)
        stop = m.run(max_insns=min(fine_budget, args.max_insn), chunk=args.chunk)
        if stop == "max_insn":
            stop = None
        if not stop and args.max_insn > m.insn_count:
            log("# switched to fast burst mode after %d instructions"
                % m.insn_count)
            m.set_fine(False)
            stop = m.run(max_insns=args.max_insn - m.insn_count)
    else:
        m.set_fine(False)
        stop = m.run(max_insns=args.max_insn)
    total = m.insn_count

    log("# ---------- report ----------")
    pc = m.reg("PC") & ~1
    sp = m.reg("SP")
    lr = m.reg("LR")
    log("instructions : %d" % m.insn_count)
    log("stop reason  : %s" % stop)
    log("final PC     : %#010x  (image offset %+#x)" % (
        pc, pc - args.base if pc >= args.base else 0))
    log("SP=%#010x LR=%#010x" % (sp, lr))
    log("reached main area (>0x08006000): %s" % (
        "YES" if any(p > 0x08006000 for p in m.pc_ring) else "NO"))
    pcs = m.pc_history(60)
    log("last %d PCs: %s" % (len(pcs), " ".join("%#x" % p for p in pcs)))
    firstpcs = list(m.pc_ring)[:40]
    log("first %d PCs: %s" % (len(firstpcs), " ".join("%#x" % p for p in firstpcs)))
    hang = analyze_hang(m)
    log("hang analysis: %s" % hang)
    log("")
    log("--- top hot MMIO ---")
    log(m.hot_mmio_report(30))
    log("")
    log("--- unknown / unmodeled MMIO ---")
    log(m.unknown_mmio_report(50) or "  (none)")
    log("")
    log("--- missing peripheral accesses (all) ---")
    for addr, ent in m.access.unknown.items():
        p = m.find_peripheral(addr)
        log("  %#010x R=%d W=%d %s" % (addr, ent[0], ent[1],
                                       p.name if p else ""))
    log("")
    log("--- register dump ---")
    log(m.register_string())
    log("")
    outdir = args.outdir or logdir
    os.makedirs(outdir, exist_ok=True)
    # LCD controller traffic (AT32 XMC window at 0x6C000000)
    try:
        lcd = getattr(m, "lcd_model", None)
        if lcd is not None and lcd.byte_count:
            log("--- XMC LCD (0x6C000000) traffic ---")
            log(lcd.command_summary(40))
            log("--- last LCD commands ---")
            log(lcd.last_commands(30))
            lcdlog = os.path.join(outdir, "lcd_%s.txt" % ts)
            lcd.save_log(lcdlog)
            log("LCD command log: %s" % lcdlog)
            if lcd.pixels:
                png = os.path.join(outdir, "lcd_screen_%s.png" % ts)
                try:
                    lcd.render_png(png)
                    log("LCD pixel stream PNG: %s (%d pixel bytes)"
                        % (png, len(lcd.pixels)))
                except Exception as e:
                    log("LCD PNG render failed: %r" % (e,))
        else:
            log("--- XMC LCD: no command/data traffic seen ---")
    except Exception as e:
        log("  (lcd summary unavailable: %r)" % (e,))
    log("")
    # DMA transfers
    try:
        for p in m.periphs:
            if type(p).__name__ == "DMA":
                log("--- %s ---" % p.name)
                log(p.summary(20) if hasattr(p, "summary") else "(no summary)")
    except Exception as e:
        log("  (dma summary unavailable: %r)" % (e,))
    log("")
    log("--- fault state ---")
    try:
        ppb = m.ppb
        log("HFSR=%#010x CFSR=%#010x SHCSR=%#010x ICSR=%#010x" % (
            ppb.hfsr, ppb.cfsr, ppb.shcsr, ppb.icsr_forced))
        log("NVIC enabled=%#018x pending=%#010x active_exception=%s" % (
            ppb.nvic_enabled, ppb.nvic_pending, m.active_exception))
        log("SysTick CTRL=%#x LOAD=%#x VAL=%#x" % (
            ppb.st_ctrl, ppb.st_load, ppb.systick_val()))
    except Exception as e:
        log("  (fault state unavailable: %r)" % (e,))
    log("")
    log("--- summary ---")
    log(m.summary())

    if args.trace:
        csvpath = os.path.join(logdir, "trace_%s.csv" % ts)
        m.access.csv(csvpath)
        log("access trace CSV: %s" % csvpath)

    outdir = args.outdir or logdir
    if args.scan_fb or args.fb:
        import display
        os.makedirs(outdir, exist_ok=True)
        if args.fb:
            base, w, h, fmt = args.fb
            path = os.path.join(outdir, "fb_%s.png" % ts)
            fb = display.FramebufferModel(int(base, 0), int(w, 0), int(h, 0), fmt)
            try:
                fb.save(m, path)
                log("framebuffer dumped: %s" % path)
            except Exception as e:
                log("framebuffer dump failed: %r" % (e,))
        if args.scan_fb:
            log("--- framebuffer candidates in SRAM ---")
            paths = display.dump_candidates(m, outdir, log=log)
            for p in paths:
                log("  PNG: %s" % p)
            if not paths:
                log("  (no plausible framebuffer found)")

    logf.close()
    print("\nlog written to: %s" % logpath)
    return 0


if __name__ == "__main__":
    sys.exit(main())
