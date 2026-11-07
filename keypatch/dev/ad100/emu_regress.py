# -*- coding: utf-8 -*-
"""AD100 Pro（v4 C.Fn 密钥开关）整机启动 / 射频初始化回归（devices/ad100pro/emu）。

检查：
1) 复位 -> 2,000,000 条指令的启动前缀：原厂 vs 补丁 的 PC/寄存器/MMIO trace/
   故障/LCD 是否逐条一致。
2) 整机射频 bring-up（真 RF_Init 0x08004AF0 + 虚拟 CC2500）：对同一矩阵
   (ID, flag) 读取 CC2500 SYNC1/SYNC0：
     - 原厂镜像恒为 50024/(ID+1)（不读 flag）；
     - 补丁镜像 flag=1 -> KEY，flag=0 -> 50024/(ID+1)。
   同时比较 FREQ/PKTLEN/状态与故障数。
3) 运行期切换：直接调用 0x0800B758，flag=1 -> KEY、flag=0 -> 原厂值，
   证明函数每次调用都读标志字节。

运行：
  tools\\python\\python.exe keypatch\\dev\\ad100\\emu_regress.py [out.log]
"""
from __future__ import annotations

import hashlib
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU_DIR = os.path.join(ROOT, "devices/ad100pro", "emu")
if EMU_DIR not in sys.path:
    sys.path.insert(0, EMU_DIR)

import core  # noqa: E402  (devices/ad100pro/emu/core.py)

ORIG = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
KEY = 0xD798
PATCHED = os.path.join(ROOT, "keypatch", "out", "AD100pro_V1.2_key%04X.bin" % KEY)
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
BOOT_INSNS = 2_000_000
ID_RAM_ADDR = 0x20000485       # 设置区 RAM_20000480[5] = 引闪 ID
FLAG_RAM_ADDR = 0x20000486     # 设置区 RAM_20000480[6] = C.Fn 密钥开关
NUM = 50024
MATRIX = ((0, 0), (1, 0), (5, 0), (37, 0), (99, 0),
          (0, 1), (5, 1), (99, 1))


def sha(path):
    return hashlib.sha256(open(path, "rb").read()).hexdigest()


def boot_compare():
    print("== 1. 复位 -> %d 条指令：原厂 vs 补丁 逐项比较 ==" % BOOT_INSNS)
    ea = core.Emulator(ORIG, FUNCS, trace=True)
    eb = core.Emulator(PATCHED, FUNCS, trace=True)
    ea.reset()
    eb.reset()
    sa = ea.run(BOOT_INSNS)
    sb = eb.run(BOOT_INSNS)
    ra, rb = ea.regs_all(), eb.regs_all()
    ok = True

    def chk(cond, msg):
        nonlocal ok
        ok = ok and cond
        print("  [%s] %s" % ("PASS" if cond else "FAIL", msg))

    chk(sa["pc"] == sb["pc"],
        "停止 PC 一致: 0x%08X == 0x%08X（状态 %s/%s）" % (sa["pc"], sb["pc"], sa["status"], sb["status"]))
    chk(sa["status"] == sb["status"] == "ok", "运行状态一致且正常: %s/%s" % (sa["status"], sb["status"]))
    chk(ea.insn_count == eb.insn_count, "指令计数一致: %d == %d" % (ea.insn_count, eb.insn_count))
    chk(len(ea.faults) == len(eb.faults) == 0,
        "无故障: 原厂 %d / 补丁 %d" % (len(ea.faults), len(eb.faults)))
    reg_names = ("R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
                 "R8", "R9", "R10", "R11", "R12", "SP", "LR", "PC")
    diff_regs = [n for n in reg_names if ra[n] != rb[n]]
    chk(not diff_regs, "16 个寄存器完全一致" + ("" if not diff_regs else "（差异: %s）" % diff_regs))
    chk(ea.lcd.frames == eb.lcd.frames,
        "LCD 帧数一致: %d == %d" % (ea.lcd.frames, eb.lcd.frames))
    chk(bytes(ea.lcd.fb) == bytes(eb.lcd.fb),
        "LCD 帧缓冲内容一致（nonzero 原厂 %d / 补丁 %d）"
        % (sum(1 for b in ea.lcd.fb if b), sum(1 for b in eb.lcd.fb if b)))

    ta, tb = list(ea.trace), list(eb.trace)
    chk(len(ta) == len(tb), "MMIO 外设访问条数一致: %d == %d" % (len(ta), len(tb)))

    def tkey(t):
        return (t.insn, t.pc, t.addr, t.dev, t.reg, t.rw, t.value, t.size)

    first_diff = next((i for i in range(min(len(ta), len(tb))) if tkey(ta[i]) != tkey(tb[i])), None)
    chk(first_diff is None, "MMIO 外设访问逐条一致" +
        ("" if first_diff is None else "（首个差异 index=%d: %s vs %s）"
         % (first_diff, ta[first_diff].line(), tb[first_diff].line())))
    print("  （采样: 原厂 trace[0]=%s，补丁 trace[0]=%s）"
          % (ta[0].line() if ta else None, tb[0].line() if tb else None))
    print("  原厂 LCD: frames=%d bytes=%d ; 补丁 LCD: frames=%d bytes=%d"
          % (ea.lcd.frames, len(ea.lcd.bytes), eb.lcd.frames, len(eb.lcd.bytes)))
    return ok


def rf_bringup(fw_path, label):
    tag = "2a" if label == "原厂" else "2b"
    print("\n== %s. 整机 RF bring-up（跳过启动动画 -> 真 RF_Init 0x08004AF0）: %s ==" % (tag, label))
    e = core.Emulator(fw_path, FUNCS, trace=False)
    e.reset()
    e.skip_to_main_loop(1_000_000)
    e.run(200_000)                      # 让显示/调度状态机稳定
    results = []
    for ident, flag in MATRIX:
        e.rf_initialized = False
        e.write_mem(ID_RAM_ADDR, bytes([ident]))
        e.write_mem(FLAG_RAM_ADDR, bytes([flag]))
        e.cc2500.reset()
        rf = e.rf_init()
        e.run(200_000)
        regs = e.cc2500.regs
        sync1, sync0 = regs.get(0x04, -1), regs.get(0x05, -1)
        sync = (sync1 << 8) | sync0
        if label == "补丁":
            expect = KEY if flag else NUM // (ident + 1)
        else:
            expect = NUM // (ident + 1)      # 原厂不读 flag
        results.append((ident, flag, sync1, sync0, sync, expect,
                        rf.get("state"), len(e.faults),
                        regs.get(0x0D), regs.get(0x06)))
        print("  ID=%-2d flag=%d -> SYNC1=0x%02X SYNC0=0x%02X sync=0x%04X (期望 0x%04X) | "
              "CC2500 state=%s freq2=0x%02X pktlen=%d faults=%d"
              % (ident, flag, sync1, sync0, sync, expect, rf.get("state"),
                 regs.get(0x0D), regs.get(0x06), len(e.faults)))
    print("  rf_status=%s" % json.dumps({k: v for k, v in e.rf_status().items()
                                         if k != "recent"}, ensure_ascii=False))
    return results, e


def runtime_toggle(e):
    """运行期直接调用 0x0800B758，验证每次调用都重新读 flag。"""
    print("\n== 3. 运行期切换 flag（模拟器内直接调用 0x0800B758） ==")
    ok = True
    out = {}
    for flag, ident, expect in ((1, 5, KEY), (0, 5, NUM // 6), (1, 99, KEY), (0, 99, NUM // 100)):
        e.write_mem(FLAG_RAM_ADDR, bytes([flag]))
        e.cc2500.reset()
        e.call_function(0x0800B758, (ident,))
        regs = e.cc2500.regs
        sync = (regs.get(0x04, -1) << 8) | regs.get(0x05, -1)
        good = sync == expect
        ok = ok and good
        out[(ident, flag)] = sync
        print("  [%s] ID=%-2d flag=%d -> sync=0x%04X（期望 0x%04X），CC2500 state=%s"
              % ("PASS" if good else "FAIL", ident, flag, sync, expect,
                 {0x01: 'IDLE', 0x0D: 'RX', 0x13: 'TX'}.get(e.cc2500.state, hex(e.cc2500.state))))
    print("  结论: 补丁函数每次调用都读 RAM[0x%08X]：flag=1/0 分别 0x%04X / 0x%04X"
          % (FLAG_RAM_ADDR, out[(5, 1)], out[(5, 0)]))
    return ok


def main():
    ok = True
    ok &= boot_compare()

    print("\n== 2. 整机（模拟器 + 虚拟 CC2500）射频初始化：同步字寄存器实测 ==")
    res_o, _ = rf_bringup(ORIG, "原厂")
    res_p, emu_p = rf_bringup(PATCHED, "补丁")
    for (i, fl, s1, s0, sync, exp, st, nf, f2, pl), (i2, fl2, p1, p0, psync, pexp, pst, pnf, pf2, ppl) in zip(res_o, res_p):
        good = (i == i2 and fl == fl2 and sync == exp and psync == pexp
                and st == pst and nf == 0 and pnf == 0)
        ok &= good
        print("  [%s] ID=%-2d flag=%d 原厂 sync=0x%04X(期望0x%04X) / 补丁 sync=0x%04X(期望0x%04X) "
              "| state %s/%s | freq2 0x%02X/0x%02X pktlen %s/%s faults %d/%d"
              % ("PASS" if good else "FAIL", i, fl, sync, exp, psync, pexp, st, pst, f2, pf2,
                 pl, ppl, nf, pnf))

    ok &= runtime_toggle(emu_p)

    print("\n== 4. LCD 画面 PNG（CLI 2,000,000 条指令）一致性 ==")
    pa = os.path.join(HERE, "emu_orig_screen.png")
    pb = os.path.join(HERE, "emu_patch_screen.png")
    if os.path.isfile(pa) and os.path.isfile(pb):
        ha, hb = sha(pa), sha(pb)
        good = ha == hb
        ok &= good
        print("  [%s] 原厂画面 sha256=%s" % ("PASS" if good else "FAIL", ha))
        print("       补丁画面 sha256=%s" % hb)
    else:
        print("  （跳过：未找到 emu_*_screen.png，可用 CLI --screen 生成）")

    print("\n== 结论: %s ==" % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    if len(sys.argv) > 1:
        import contextlib
        with open(sys.argv[1], "w", encoding="utf-8", newline="\n") as f, \
                contextlib.redirect_stdout(f):
            rc = main()
        sys.exit(rc)
    sys.exit(main())
