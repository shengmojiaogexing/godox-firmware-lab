# -*- coding: utf-8 -*-
"""run_ui.py -- boot the X3-C firmware with the TMR7 1 ms tick model, let the
UI run, and save real LCD screenshots.

Usage (from ):
  devices\x3c\\py\\python.exe keypatch\\dev\\tmr7\\run_ui.py --max 30000000
  devices\x3c\\py\\python.exe keypatch\\dev\\tmr7\\run_ui.py --max 60000000 \
      --scale 4 --snapshot-every 2000000

Outputs (keypatch/dev/tmr7/out by default):
  ui_<insn>_tick<ms>.png   screen snapshots
  run_<timestamp>.log      text log (progress, tick, IRQ, LCD stats)
"""
from __future__ import annotations

import argparse
import os
import struct
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
for p in (EMU, HERE):
    if p not in sys.path:
        sys.path.insert(0, p)

from machine import Machine                     # noqa: E402
import tmr7_model                                # noqa: E402
from lcd_capture import LcdCapture               # noqa: E402

IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
OUT = os.path.join(HERE, "out")
LOGS = os.path.join(HERE, "logs")


def read_u8(m, addr):
    try:
        return m.uc.mem_read(addr, 1)[0]
    except Exception:
        return -1


def read_u32(m, addr):
    try:
        return struct.unpack("<I", bytes(m.uc.mem_read(addr, 4)))[0]
    except Exception:
        return -1


def pc_of(m):
    try:
        return m.uc.reg_read(11) & ~1          # UC_ARM_REG_PC == 11
    except Exception:
        return 0


class Logger:
    def __init__(self, path):
        self.path = path
        self.f = open(path, "w", encoding="utf-8")

    def __call__(self, msg=""):
        print(msg)
        self.f.write(str(msg) + "\n")
        self.f.flush()

    def close(self):
        self.f.close()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=IMAGE)
    ap.add_argument("--max", type=int, default=30_000_000)
    ap.add_argument("--chunk", type=int, default=200_000)
    ap.add_argument("--progress-every", type=int, default=1_000_000)
    ap.add_argument("--scale", type=float, default=1.0,
                    help="emulated instructions per TMR7 input clock")
    ap.add_argument("--snapshot-every", type=int, default=1_000_000,
                    help="min instructions between dirty-screen snapshots")
    ap.add_argument("--out", default=OUT)
    ap.add_argument("--tag", default="ui")
    ap.add_argument("--no-tick", action="store_true",
                    help="control run: no TMR7 model (stock emulator)")
    ap.add_argument("--fast", action="store_true",
                    help="switch to fast burst mode after boot init")
    ap.add_argument("--fast-after", type=int, default=3_000_000)
    ap.add_argument("--adc-raw", type=int, default=2600)
    ap.add_argument("--gpio", default="",
                    help="force GPIO input levels, e.g. PA1=1,PA8=1")
    ap.add_argument("--no-tmr5-capture", action="store_true")
    ap.add_argument("--force-page", type=int, default=None,
                    help="write requested UI page id to RAM 0x200004C4 "
                         "once the main loop is reached")
    ap.add_argument("--force-mode", type=int, default=None,
                    help="write UI mode RAM 0x200003E7 (2 = UI active)")
    ap.add_argument("--poke", action="append", default=[],
                    help="repeat RAM write every chunk: ADDR=VALUE (e.g. "
                         "0x2000007F=1); applies once tick>=5")
    ap.add_argument("--brighten", action="store_true")
    args = ap.parse_args()

    os.makedirs(args.out, exist_ok=True)
    os.makedirs(LOGS, exist_ok=True)
    log_path = os.path.join(LOGS, "run_%s.log" % time.strftime("%Y%m%d_%H%M%S"))
    log = Logger(log_path)

    m = Machine(image=args.img, log_fn=lambda s: None, trace=True)
    tmr7_model.patch_real_device(m, adc_raw=args.adc_raw)
    if args.gpio:
        for item in args.gpio.split(","):
            item = item.strip()
            if not item:
                continue
            name, val = item.split("=")
            port = "GPIO" + name[1].upper()
            pin = int(name[2:])
            m.gpio[port].set_input(pin, int(val), driven=True)
    model = None
    if not args.no_tick:
        num, den = (round(args.scale * 1_000_000), 1_000_000)
        if args.scale == int(args.scale):
            num, den = int(args.scale), 1
        model = tmr7_model.attach(m, scale_num=num, scale_den=den,
                                  tmr5_capture=not args.no_tmr5_capture)
    cap = LcdCapture(brighten=args.brighten).install(m)

    sp, pc = m.boot()
    log("=== X3-C TMR7 UI run ===")
    log("image      : %s" % args.img)
    log("reset      : SP=%#010x PC=%#010x" % (sp, pc))
    log("tmr7 model : %s" % ("DISABLED (control)" if model is None else
                             "attached (IRQ55, scale %s insn per TMR7 clock)"
                             % args.scale))
    log("budget     : %d instructions, chunk %d" % (args.max, args.chunk))
    log("")

    snapshots = []
    last_snap_insn = -10**18
    last_log_insn = 0
    next_progress = args.progress_every
    last_tick = read_u32(m, tmr7_model.TICK_COUNTER)
    t0 = time.time()
    fault = None
    page_forced = False

    while m.insn_count < args.max:
        n = min(args.chunk, args.max - m.insn_count)
        m.run(n)
        insn = m.insn_count
        reason = m.stop_reason
        if reason and reason != "max_insn":
            fault = reason
            log("!! stop: %s at %d instructions PC=%#010x"
                % (reason, insn, pc_of(m)))
            break
        if args.fast and insn >= args.fast_after and m.fine:
            m.set_fine(False)
            log("[%10d] switched to fast burst mode" % insn)

        tick = read_u32(m, tmr7_model.TICK_COUNTER)
        sub = read_u8(m, tmr7_model.MS_SUBCOUNTER)

        if (args.force_page is not None and not page_forced
                and tick >= 5):
            try:
                if args.force_mode is not None:
                    m.uc.mem_write(0x200003E7,
                                   bytes([args.force_mode & 0xFF]))
                m.uc.mem_write(0x200004C4,
                               bytes([args.force_page & 0xFF]))
                page_forced = True
                log("[%10d] forced UI mode RAM[0x200003E7]=%s, requested "
                    "page RAM[0x200004C4]=%d"
                    % (insn, args.force_mode, args.force_page))
            except Exception as e:
                log("force_page failed: %r" % (e,))

        for spec in args.poke:
            try:
                addr_s, val_s = spec.split("=")
                addr = int(addr_s, 0)
                val = int(val_s, 0)
                m.uc.mem_write(addr, bytes([val & 0xFF]))
            except Exception as e:
                log("poke %r failed: %r" % (spec, e))

        if insn >= next_progress:
            log("[%10d] %5.1fs PC=%#010x tick=%d sub=%d irq_pends=%d "
                "lcd px=%d cmds=%d page=%d req=%d"
                % (insn, time.time() - t0, pc_of(m), tick, sub,
                   model.irq_pends if model else 0,
                   cap.n_pixels, cap.n_cmds,
                   read_u8(m, 0x200003E7), read_u8(m, 0x200004C4)))
            next_progress = insn + args.progress_every

        if cap.dirty and insn - last_snap_insn >= args.snapshot_every:
            tag = "%s_%010d_tick%05d" % (args.tag, insn, max(tick, 0))
            path = cap.snapshot(os.path.join(args.out, "ui_%s.png" % tag))
            snapshots.append((insn, tick, path))
            last_snap_insn = insn
            log("    snapshot -> %s (tick=%d px=%d cmds=%d nonblack=%.2f)"
                % (os.path.basename(path), tick, cap.n_pixels, cap.n_cmds,
                   cap.nonblack_ratio()))
        if tick != last_tick:
            last_tick = tick

    # final snapshot
    tick = read_u32(m, tmr7_model.TICK_COUNTER)
    final = cap.snapshot(os.path.join(args.out, "ui_final.png"))
    snapshots.append((m.insn_count, tick, final))
    st = cap.stats()
    log("")
    log("=== result ===")
    log("instructions : %d  (vtime=%d, %.1fs wall)"
        % (m.insn_count, m.vtime, time.time() - t0))
    log("PC           : %#010x  (%d)" % (pc_of(m), pc_of(m)))
    log("tick counter : %d  (0x20000310)" % tick)
    log("subcounter   : %d  (0x20000000)" % read_u8(m, tmr7_model.MS_SUBCOUNTER))
    if model is not None:
        log("TMR7         : pends=%d periods=%d advances=%d" %
            (model.irq_pends, model.periods, model.advances))
        log("TMR7 CNT=%d SR=%#x CR1=%#x PSC=%d ARR=%d DIER=%#x"
            % (model.timer.cnt, model.timer.sr, model.timer.cr1,
               model.timer.psc, model.timer.arr, model.timer.dier))
        if model.irq_entry_pcs:
            log("first IRQ entries: %r"
                % (model.irq_entry_pcs[:5],))
    log("LCD          : %r" % (st,))
    log("snapshots    :")
    for insn, t, path in snapshots:
        log("  %10d tick=%-8d %s" % (insn, t, path))
    if fault:
        log("FAULT: %s" % fault)
    log("\nlog: %s" % log_path)
    log.close()
    return 0 if not fault else 1


if __name__ == "__main__":
    sys.exit(main())
