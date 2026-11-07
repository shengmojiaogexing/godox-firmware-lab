# -*- coding: utf-8 -*-
"""AD100 Pro 变体镜像整机回归（可指定镜像/语义）：
  - boot：复位 -> 200 万条指令，与原厂逐项比对（PC/寄存器/MMIO trace/fault/LCD）；
  - RF bring-up：真 RF_Init(0x08004AF0) + 虚拟 CC2500，检查 SYNC1/SYNC0；
  - 运行期调用 0x0800B758：flag/id0 两种语义分别校验。

用法：
  python emu_regress_variant.py --image <bin> --mode flag|id0 [--screen <png>] [--log <log>]

mode=flag : v4 语义，RAM[0x20000486]=1 -> KEY，=0 -> 50024/(ID+1)
mode=id0  : 临时版语义，ID==0 -> KEY，ID!=0 -> 50024/(ID+1)（flag 不参与）
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU_DIR = os.path.join(ROOT, "devices/ad100pro", "emu")
if EMU_DIR not in sys.path:
    sys.path.insert(0, EMU_DIR)

import core  # noqa: E402

ORIG = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
KEY = 0xD798
NUM = 50024
BOOT_INSNS = 2_000_000
ID_RAM_ADDR = 0x20000485
FLAG_RAM_ADDR = 0x20000486
STATE_NAME = {0x01: "IDLE", 0x0D: "RX", 0x13: "TX"}
MATRIX_FLAG = ((0, 0), (1, 0), (5, 0), (37, 0), (99, 0),
               (0, 1), (5, 1), (99, 1))
MATRIX_ID0 = ((0, 0), (1, 0), (5, 0), (37, 0), (99, 0))


def sha(path):
    return hashlib.sha256(open(path, "rb").read()).hexdigest()


def expected(ident, flag, mode):
    if mode == "id0":
        return KEY if ident == 0 else NUM // (ident + 1)
    return KEY if flag else NUM // (ident + 1)


def boot_compare(orig, img, out=print):
    out("== 1. 复位 -> %d 条指令：原厂 vs 待测镜像 逐项比较 ==" % BOOT_INSNS)
    ea = core.Emulator(orig, FUNCS, trace=True)
    eb = core.Emulator(img, FUNCS, trace=True)
    ea.reset()
    eb.reset()
    sa = ea.run(BOOT_INSNS)
    sb = eb.run(BOOT_INSNS)
    ra, rb = ea.regs_all(), eb.regs_all()
    ok = True

    def chk(cond, msg):
        nonlocal ok
        ok = ok and cond
        out("  [%s] %s" % ("PASS" if cond else "FAIL", msg))

    chk(sa["pc"] == sb["pc"],
        "停止 PC 一致: 0x%08X == 0x%08X（状态 %s/%s）" % (sa["pc"], sb["pc"], sa["status"], sb["status"]))
    chk(sa["status"] == sb["status"] == "ok", "运行状态一致且正常: %s/%s" % (sa["status"], sb["status"]))
    chk(ea.insn_count == eb.insn_count, "指令计数一致: %d == %d" % (ea.insn_count, eb.insn_count))
    chk(len(ea.faults) == len(eb.faults) == 0,
        "无故障: 原厂 %d / 待测 %d" % (len(ea.faults), len(eb.faults)))
    names = ("R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
             "R8", "R9", "R10", "R11", "R12", "SP", "LR", "PC")
    diff_regs = [n for n in names if ra[n] != rb[n]]
    chk(not diff_regs, "16 个寄存器完全一致" + ("" if not diff_regs else "（差异 %s）" % diff_regs))
    chk(ea.lcd.frames == eb.lcd.frames, "LCD 帧数一致: %d == %d" % (ea.lcd.frames, eb.lcd.frames))
    chk(bytes(ea.lcd.fb) == bytes(eb.lcd.fb),
        "LCD 帧缓冲一致（nonzero 原厂 %d / 待测 %d）"
        % (sum(1 for b in ea.lcd.fb if b), sum(1 for b in eb.lcd.fb if b)))
    ta, tb = list(ea.trace), list(eb.trace)
    chk(len(ta) == len(tb), "MMIO 外设访问条数一致: %d == %d" % (len(ta), len(tb)))

    def tkey(t):
        return (t.insn, t.pc, t.addr, t.dev, t.reg, t.rw, t.value, t.size)

    first = next((i for i in range(min(len(ta), len(tb))) if tkey(ta[i]) != tkey(tb[i])), None)
    chk(first is None, "MMIO 外设访问逐条一致" +
        ("" if first is None else "（首个差异 #%d: %s vs %s）"
         % (first, ta[first].line(), tb[first].line())))
    return ok


def rf_bringup(orig, img, mode, out=print):
    out("\n== 2. 整机 RF bring-up（跳过启动动画 -> 真 RF_Init 0x08004AF0）: %s ==" % mode)
    eo = core.Emulator(orig, FUNCS, trace=False)
    ep = core.Emulator(img, FUNCS, trace=False)
    for e in (eo, ep):
        e.reset()
        e.skip_to_main_loop(1_000_000)
        e.run(200_000)
    matrix = MATRIX_ID0 if mode == "id0" else MATRIX_FLAG
    rows = []
    for ident, flag in matrix:
        vals = []
        for e, label in ((eo, "原厂"), (ep, "待测")):
            e.rf_initialized = False
            e.write_mem(ID_RAM_ADDR, bytes([ident]))
            e.write_mem(FLAG_RAM_ADDR, bytes([flag]))
            e.cc2500.reset()
            rf = e.rf_init()
            e.run(200_000)
            r = e.cc2500.regs
            sync = (r.get(0x04, -1) << 8) | r.get(0x05, -1)
            vals.append((sync, r.get(0x04, -1), r.get(0x05, -1), rf.get("state"),
                         len(e.faults), r.get(0x0D), r.get(0x06)))
        (os_, oh, ol, ost, onf, of2, opl), (ps, ph, pl, pst, pnf, pf2, ppl) = vals
        exp_o = NUM // (ident + 1)          # 原厂不读 flag
        exp_p = expected(ident, flag, mode)
        good = (os_ == exp_o and ps == exp_p and ost == pst and onf == 0 and pnf == 0)
        rows.append(good)
        out("  [%s] ID=%-2d flag=%d 原厂 sync=0x%04X(期望0x%04X) / 待测 sync=0x%04X(期望0x%04X) "
            "| state %s/%s | freq2 0x%02X/0x%02X pktlen %s/%s faults %d/%d"
            % ("PASS" if good else "FAIL", ident, flag, os_, exp_o, ps, exp_p,
               ost, pst, of2, pf2, opl, ppl, onf, pnf))
    return all(rows)


def runtime_call(img, mode, out=print):
    out("\n== 3. 运行期调用 0x0800B758：%s 语义 ==" % mode)
    e = core.Emulator(img, FUNCS, trace=False)
    e.reset()
    e.skip_to_main_loop(1_000_000)
    e.run(200_000)
    ok = True
    cases = [(0, 0, KEY), (5, 0, NUM // 6), (99, 0, NUM // 100)] if mode == "id0" \
        else [(5, 1, KEY), (5, 0, NUM // 6), (99, 1, KEY), (99, 0, NUM // 100)]
    for ident, flag, exp in cases:
        e.write_mem(FLAG_RAM_ADDR, bytes([flag]))
        e.cc2500.reset()
        e.call_function(0x0800B758, (ident,))
        r = e.cc2500.regs
        sync = (r.get(0x04, -1) << 8) | r.get(0x05, -1)
        good = sync == exp
        ok = ok and good
        out("  [%s] ID=%-2d flag=%d -> sync=0x%04X（期望 0x%04X），state=%s"
            % ("PASS" if good else "FAIL", ident, flag, sync, exp,
               STATE_NAME.get(e.cc2500.state, hex(e.cc2500.state))))
    return ok


def screen_check(orig_screen, img_screen, out=print):
    out("\n== 4. LCD 画面 PNG（CLI 2,000,000 条指令）一致性 ==")
    if not (os.path.isfile(orig_screen) and os.path.isfile(img_screen)):
        out("  （跳过：缺少 %s 或 %s）" % (orig_screen, img_screen))
        return True
    ha, hb = sha(orig_screen), sha(img_screen)
    good = ha == hb
    out("  [%s] 原厂画面 sha256=%s" % ("PASS" if good else "FAIL", ha))
    out("       待测画面 sha256=%s" % hb)
    return good


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--image", required=True)
    ap.add_argument("--mode", choices=["flag", "id0"], default="flag")
    ap.add_argument("--screen", default=None, help="待测镜像的 2M LCD PNG（与 emu_orig_screen.png 比对）")
    ap.add_argument("--orig-screen", default=os.path.join(HERE, "emu_orig_screen.png"))
    ap.add_argument("--log", default=None)
    ap.add_argument("--tag", default=None)
    args = ap.parse_args(argv)

    buf = []

    def out(msg):
        buf.append(msg)
        print(msg)

    ok = True
    print("镜像: %s\nsha256: %s\nmode: %s" % (os.path.relpath(args.image, ROOT), sha(args.image), args.mode))
    ok &= boot_compare(ORIG, args.image, out)
    ok &= rf_bringup(ORIG, args.image, args.mode, out)
    ok &= runtime_call(args.image, args.mode, out)
    if args.screen:
        ok &= screen_check(args.orig_screen, args.screen, out)
    out("\n== 结论: %s ==" % ("PASS" if ok else "FAIL"))
    if args.log:
        with open(args.log, "w", encoding="utf-8", newline="\n") as f:
            f.write("镜像: %s\nsha256: %s\nmode: %s\n\n" % (args.image, sha(args.image), args.mode))
            f.write("\n".join(buf) + "\n")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
