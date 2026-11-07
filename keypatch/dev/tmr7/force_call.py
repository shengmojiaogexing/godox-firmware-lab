# -*- coding: utf-8 -*-
"""Force-call UI init / page-builder functions from a RAM stub.

The firmware's normal UI init is not reachable in the emulator yet (see
NOTES.md section 11).  This tool lets us call arbitrary firmware functions on a
*fully booted* machine (tick + LCD models active) by planting a small Thumb
stub in the (unused) CCM RAM at 0x1000F000:

    ldr r12,[pc,#4] ; blx r12 ; nop ; nop ; .word function   (12 bytes/call)
    ...  ; b .

Usage:
  python force_call.py --max 12000000 --gpio PA1=1,PA8=1 \
      --calls 0x0800BA08,0x0800FE78,0x080148A8 --after 9500000 --run 4000000
"""
from __future__ import annotations

import argparse
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine                        # noqa: E402
import tmr7_model                                  # noqa: E402
from lcd_capture import LcdCapture                  # noqa: E402

IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
OUT = os.path.join(HERE, "out")
STUB = 0x1000F000


def _arm_imm(hw, reg):
    """Encode movs/movw+movt to load a 32-bit immediate into *reg*."""
    out = bytearray()
    v = hw & 0xFFFFFFFF
    if v <= 0xFF:
        out += struct.pack("<H", 0x2000 | (reg << 8) | v)   # movs rX,#imm8
        return bytes(out)
    # movw rX,#lo16
    lo = v & 0xFFFF
    imm4 = (lo >> 12) & 0xF
    i = (lo >> 11) & 1
    imm3 = (lo >> 8) & 7
    imm8 = lo & 0xFF
    hw1 = 0xF240 | (i << 10) | imm4
    hw2 = (imm3 << 12) | (reg << 8) | imm8
    out += struct.pack("<HH", hw1, hw2)
    # movt rX,#hi16
    hi = (v >> 16) & 0xFFFF
    if hi:
        imm4 = (hi >> 12) & 0xF
        i = (hi >> 11) & 1
        imm3 = (hi >> 8) & 7
        imm8 = hi & 0xFF
        hw1 = 0xF2C0 | (i << 10) | imm4
        hw2 = (imm3 << 12) | (reg << 8) | imm8
        out += struct.pack("<HH", hw1, hw2)
    return bytes(out)


def build_stub(targets):
    """targets: list of dicts: {"addr": int, "args": {reg: value}}.

    Per call: optional movs/movw+movt for r0..r3, 4-byte alignment, then
        ldr r4,[pc,#4] ; blx r4 ; b +6 ; nop ; .word target
    """
    code = bytearray()
    code += struct.pack("<H", 0xB500)          # push {lr}  (save return addr)
    code += struct.pack("<H", 0xBF00)          # nop (align to 4)
    for item in targets:
        t = item["addr"]
        for reg in (0, 1, 2, 3):
            if reg in item.get("args", {}):
                code += _arm_imm(item["args"][reg], reg)
        while len(code) % 4:
            code += struct.pack("<H", 0xBF00)  # nop padding
        # ldr r4,[pc,#4] -> pool = (base+4)&~3 + 4 = base+8
        code += struct.pack("<H", 0x4C01)      # ldr r4,[pc,#4]
        code += struct.pack("<H", 0x47A0)      # blx r4
        code += struct.pack("<H", 0xE002)      # b +6  (skip nop + pool)
        code += struct.pack("<H", 0xBF00)      # nop
        code += struct.pack("<I", t | 1)       # thumb target
    code += struct.pack("<H", 0xBD00)          # pop {pc}  (return to firmware)
    return bytes(code)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=IMAGE)
    ap.add_argument("--max", type=int, default=12_000_000)
    ap.add_argument("--gpio", default="PA1=1,PA8=1")
    ap.add_argument("--calls", default="0x0800BA08,0x0800FE78,0x080148A8")
    ap.add_argument("--after", type=int, default=9_500_000,
                    help="instruction count after which the stub is entered")
    ap.add_argument("--run", type=int, default=4_000_000,
                    help="instructions to run after entering the stub")
    ap.add_argument("--scale", type=float, default=1.0)
    ap.add_argument("--out", default=OUT)
    ap.add_argument("--tag", default="fc")
    ap.add_argument("--poke", action="append", default=[],
                    help="ADDR=VALUE written every chunk after the stub")
    ap.add_argument("--force-mode", type=int, default=None)
    ap.add_argument("--force-page", type=int, default=None)
    ap.add_argument("--force-tick", type=int, default=5,
                    help="apply --force-mode/--force-page once tick >= this")
    ap.add_argument("--jump", type=lambda s: int(s, 0), default=None,
                    help="instead of the stub, jump to this firmware address "
                         "(e.g. 0x08033F88 = start of main's cold-boot block)")
    ap.add_argument("--sp", type=lambda s: int(s, 0), default=0x200477C0,
                    help="SP to set for --jump (main entry SP = 0x200477c0)")
    ap.add_argument("--break-at", type=lambda s: int(s, 0), default=None,
                    help="run until this PC, then do --jump (skip a bad call)")
    ap.add_argument("--snapshot-gap", type=int, default=800_000)
    ap.add_argument("--progress", type=int, default=1_000_000)
    args = ap.parse_args()

    targets = []
    for x in args.calls.split(","):
        x = x.strip()
        if not x:
            continue
        parts = x.split(":")
        addr = int(parts[0], 0)
        args_d = {}
        for i, v in enumerate(parts[1:4]):
            v = v.strip()
            if v:
                args_d[i] = int(v, 0)
        targets.append({"addr": addr, "args": args_d})
    os.makedirs(args.out, exist_ok=True)

    m = Machine(log_fn=lambda s: None, trace=True)
    tmr7_model.patch_real_device(m)
    if args.gpio:
        for item in args.gpio.split(","):
            n, v = item.split("=")
            m.gpio["GPIO" + n[1].upper()].set_input(int(n[2:]), int(v),
                                                    driven=True)
    num, den = (int(args.scale), 1) if args.scale == int(args.scale) else \
        (round(args.scale * 1_000_000), 1_000_000)
    model = tmr7_model.attach(m, scale_num=num, scale_den=den)
    cap = LcdCapture().install(m)
    m.boot()

    def tick():
        return struct.unpack("<I", bytes(m.uc.mem_read(
            tmr7_model.TICK_COUNTER, 4)))[0]

    print("booting to %d instructions ..." % args.after)
    if args.break_at is not None:
        m.breakpoints[args.break_at & ~1] = "stop"
        while m.insn_count < args.after:
            m.run(100_000)
            if m.stop_reason and m.stop_reason.startswith("breakpoint"):
                print("  stopped at breakpoint %#x  PC=%#x insn=%d"
                      % (args.break_at, m.uc.reg_read(11) & ~1,
                         m.insn_count))
                del m.breakpoints[args.break_at & ~1]
                m.stop_reason = None
                m._bp_skip = args.break_at & ~1
                break
    else:
        while m.insn_count < args.after:
            m.run(min(1_000_000, args.after - m.insn_count))
            print("  insn=%d tick=%d PC=%#x mode=%d lcd_px=%d"
                  % (m.insn_count, tick(), m.uc.reg_read(11) & ~1,
                     m.uc.mem_read(0x200003E7, 1)[0], cap.n_pixels))

    if args.jump is not None:
        # Directly replay a firmware code region: jump into main's cold-boot
        # block as if FUN_08010C8C had returned.  main() itself has no stack
        # frame (it pushes nothing), so restore the entry SP and the callee
        # registers the block needs (r7=0, sl=0x20000035).
        from unicorn.arm_const import UC_ARM_REG_SP
        m.uc.reg_write(UC_ARM_REG_SP, args.sp)
        m.uc.reg_write(7, 0)                   # r7 = 0  (strb r7,[sl])
        m.uc.reg_write(10, 0x20000035)         # sl
        if targets:
            stub = build_stub(targets)
            m.uc.mem_write(STUB, stub)
            print("stub @ %#x: %s" % (STUB, stub.hex()))
            print("calls:", [("%#x" % t["addr"])
                             + (("(args=%r)" % (t["args"],)) if t["args"] else "")
                             for t in targets])
            m.uc.reg_write(10, (args.jump & ~1) | 1)   # LR = jump target
            m.uc.reg_write(11, STUB | 1)
            print("stub calls then jump -> PC=%#x SP=%#x"
                  % (args.jump & ~1, args.sp))
        else:
            m.uc.reg_write(11, (args.jump & ~1) | 1)
            print("jump -> PC=%#x SP=%#x r7=0 sl=0x20000035"
                  % (args.jump & ~1, args.sp))
    else:
        stub = build_stub(targets)
        try:
            m.uc.mem_write(STUB, stub)
        except Exception as e:
            print("stub write failed:", e)
            return
        print("stub @ %#x: %s" % (STUB, stub.hex()))
        print("calls:", [("%#x" % t["addr"])
                         + (("(args=%r)" % (t["args"],)) if t["args"] else "")
                         for t in targets])
        # enter the stub; the stub saves LR, calls the functions, then pop {pc}
        # returns to the firmware wait loop so the normal ON loop continues.
        fw_pc = m.uc.reg_read(11) & ~1
        print("return to firmware at %#x" % fw_pc)
        m.uc.reg_write(10, fw_pc | 1)          # LR: return address for pop {pc}
        m.uc.reg_write(11, STUB | 1)           # PC: stub

    last_px = -1
    last_snap = -10**9
    done = 0
    while done < args.run:
        n = min(200_000, args.run - done)
        m.run(n)
        done = m.insn_count - args.after
        t = tick()
        if t >= args.force_tick:
            if args.force_mode is not None:
                m.uc.mem_write(0x200003E7, bytes([args.force_mode & 0xFF]))
            if args.force_page is not None:
                m.uc.mem_write(0x200004C4, bytes([args.force_page & 0xFF]))
            for spec in args.poke:
                a_s, v_s = spec.split("=")
                m.uc.mem_write(int(a_s, 0), bytes([int(v_s, 0) & 0xFF]))
        if done and done % args.progress < 200_000:
            print("  [+%9d] tick=%d PC=%#x mode=%d req=%d px=%d cmds=%d "
                  "root=%#x" % (done, t, m.uc.reg_read(11) & ~1,
                                m.uc.mem_read(0x200003E7, 1)[0],
                                m.uc.mem_read(0x200004C4, 1)[0],
                                cap.n_pixels, cap.n_cmds,
                                struct.unpack("<I", bytes(m.uc.mem_read(
                                    0x20000470, 4)))[0]))
        if (cap.n_pixels != last_px and done - last_snap >= args.snapshot_gap):
            last_px = cap.n_pixels
            last_snap = done
            p = cap.snapshot(os.path.join(args.out, "ui_%s_%08d.png"
                                          % (args.tag, done)))
            print("    snapshot:", p)

    print("after: insn=%d tick=%d PC=%#x mode=%d root=%#x"
          % (m.insn_count, tick(), m.uc.reg_read(11) & ~1,
             m.uc.mem_read(0x200003E7, 1)[0],
             struct.unpack("<I", bytes(m.uc.mem_read(0x20000470, 4)))[0]))
    print("last log:", m.log_lines[-3:])
    path = cap.snapshot(os.path.join(args.out, "ui_%s_tick%05d.png"
                                     % (args.tag, tick())))
    print("snapshot:", path, " px=", cap.n_pixels, " cmds=", cap.n_cmds,
          " nonblack=%.3f" % cap.nonblack_ratio())


if __name__ == "__main__":
    main()
