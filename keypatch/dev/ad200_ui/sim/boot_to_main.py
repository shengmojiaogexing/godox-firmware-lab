#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""boot_to_main.py —— 加载任意 AD200 Pro 固件，跑到主循环并记录启动指标。

用途：给 v4 固件（以及原厂）做"能启动"的快速回归；支持原厂 vs 补丁对比。

用法（必须用 devices/ad200pro 自带解释器，unicorn/capstone 在里面）：
    devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\sim\\boot_to_main.py
    ... boot_to_main.py --bin keypatch\\out\\AD200pro_V1.60_keyD798.bin
    ... boot_to_main.py --compare                      # 原厂 vs keypatch/out 默认补丁
    ... boot_to_main.py --compare --patched 别的.bin --extra 4000000 --json out.json

判定（对每个镜像，与 devices/ad200pro/tests/test_boot.py 的验收口径一致）：
  1. reset PC == 0x080030D4、SP == 0x200008B0（向量表）；
  2. 500 万条指令内精确命中主循环入口 0x0800CE40（breakpoint，单步精确计数）；
  3. 主循环再跑 --extra 条指令，期间无 fault / stall；
  4. 启动墙钟 < 30s。

退出码：0 = PASS；1 = FAIL。
日志：直接 `> log_boot_to_main.txt` 重定向即可（本脚本强制 UTF-8 输出）。
"""
import argparse
import json
import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import _sim_common as C  # noqa: E402


def analyse(path, extra=2_000_000, exact=True):
    C.ensure_stdout_utf8()
    m = C.load_machine(path)
    data = C.read_file_bytes(path)
    rec = {
        "path": os.path.abspath(path),
        "size": len(data),
        "sha256": C.sha256_hex(data),
        "reset_pc": m.fw.reset_pc,
        "reset_sp": m.fw.reset_sp,
    }
    info = C.boot_to_main(m, exact_insns=exact)
    rec.update({k: v for k, v in info.items() if k != "machine"})
    rec["fault"] = m.state in ("fault", "stalled")
    # 主循环再跑一段，确认稳定
    t0 = time.time()
    ok, used, reason = C.run_slices(m, extra, slice_insns=20_000)
    rec["extra_insns"] = used
    rec["extra_ok"] = ok
    rec["extra_wall"] = round(time.time() - t0, 3)
    rec["final_state"] = m.state
    rec["final_pc"] = m.regs()["pc"] & ~1
    rec["final_pc_name"] = C.read_symbol_name(m, rec["final_pc"])
    rec["total_insns"] = m.insn_count
    rec["uptime_ms"] = m.stats().get("uptime_ms")
    return rec, m


def report(tag, rec):
    print("[%s] %s" % (tag, rec["path"]))
    print("  镜像: %d 字节 sha256=%s" % (rec["size"], rec["sha256"]))
    print("  reset: pc=0x%08X sp=0x%08X" % (rec["reset_pc"], rec["reset_sp"]))
    print("  到主循环: insns=%d pc=0x%08X state=%s reason=%s wall=%.2fs"
          % (rec["insns"], rec["pc"], rec["state"], rec["reason"], rec["wall"]))
    print("  主循环再跑 %d 条: ok=%s state=%s final_pc=0x%08X %s uptime=%sms"
          % (rec["extra_insns"], rec["extra_ok"], rec["final_state"],
             rec["final_pc"], rec["final_pc_name"], rec["uptime_ms"]))
    print("  总指令数=%d" % rec["total_insns"])


def check_single(rec):
    ok = True

    def need(cond, msg):
        nonlocal ok
        print("  [%s] %s" % ("PASS" if cond else "FAIL", msg))
        ok = ok and cond

    need(rec["reset_pc"] == C.RESET_PC, "复位 PC = 0x%08X（期望 0x%08X）"
         % (rec["reset_pc"], C.RESET_PC))
    need(rec["reset_sp"] == 0x200008B0, "复位 SP = 0x%08X（期望 0x200008B0）"
         % rec["reset_sp"])
    need(rec["reason"] == "breakpoint" and rec["pc"] == C.LOOP_LO,
         "精确命中主循环入口 0x%08X（reason=%s pc=0x%08X）"
         % (C.LOOP_LO, rec["reason"], rec["pc"]))
    need(not rec["fault"], "启动到主循环无 fault/stall")
    need(rec["extra_ok"], "主循环额外 %d 条指令无 fault/stall" % rec["extra_insns"])
    need(rec["wall"] < 30.0, "启动墙钟 %.2fs < 30s" % rec["wall"])
    return ok


def main(argv=None):
    C.ensure_stdout_utf8()
    ap = argparse.ArgumentParser(description="AD200 Pro 固件启动到主循环验证")
    ap.add_argument("--compare", action="store_true",
                    help="原厂 vs 补丁对比（默认原厂 + keypatch/out/AD200pro_V1.60_keyD798.bin）")
    ap.add_argument("--bin", help="单个固件路径（默认原厂 devices/ad200pro/firmware/AD200pro_V1.60.bin）")
    ap.add_argument("--orig", help="对比模式：原厂固件路径")
    ap.add_argument("--patched", help="对比模式：补丁固件路径（默认 keypatch/out/AD200pro_V1.60_keyD798.bin）")
    ap.add_argument("--extra", type=int, default=2_000_000, help="到主循环后再跑的指令数（默认 200 万）")
    ap.add_argument("--json", help="把结果写到 JSON 文件")
    args = ap.parse_args(argv)

    compare = bool(args.compare or args.orig or args.patched)
    if args.bin and compare:
        ap.error("--bin 与 --orig/--patched 不能同时使用")

    if not compare:
        path = args.bin or C.resolve_orig()
        print("== boot_to_main: 启动到主循环 ==")
        rec, _ = analyse(path, extra=args.extra)
        report("镜像", rec)
        print("\n判定：")
        ok = check_single(rec)
        result = {"mode": "single", "ok": ok, "image": rec}
        print("\n%s boot_to_main" % ("PASS" if ok else "FAIL"))
    else:
        orig = C.resolve_orig(args.orig)
        patched = os.path.abspath(args.patched or C.PATCHED_DEFAULT)
        print("== boot_to_main: 原厂 vs 补丁 启动对比 ==")
        missing = not os.path.isfile(patched)
        if missing:
            print("[SKIP] 补丁镜像不存在：%s" % patched)
            print("       （只跑原厂自检；Lead 产出补丁后重跑本命令即可对比）")
            rec_o, _ = analyse(orig, extra=args.extra)
            report("原厂", rec_o)
            ok = check_single(rec_o)
            result = {"mode": "compare", "ok": ok, "skipped_patched": True,
                      "orig": rec_o, "patched": None}
            print("\n%s boot_to_main（SKIP 对比：缺补丁）" % ("PASS" if ok else "FAIL"))
            if args.json:
                _write_json(args.json, result)
            return 0 if ok else 1

        rec_o, mo = analyse(orig, extra=args.extra)
        rec_p, mp = analyse(patched, extra=args.extra)
        report("原厂", rec_o)
        report("补丁", rec_p)

        print("\n对比判定：")
        ok = check_single(rec_o) and check_single(rec_p)
        delta = rec_o["insns"] - rec_p["insns"]
        same_reset = (rec_o["reset_pc"] == rec_p["reset_pc"] == C.RESET_PC
                      and rec_o["reset_sp"] == rec_p["reset_sp"] == 0x200008B0)
        same_entry = rec_o["pc"] == rec_p["pc"] == C.LOOP_LO
        delta_ok = abs(delta) < 2000
        same_total_delta = (rec_o["total_insns"] - rec_p["total_insns"]) == delta
        for cond, msg in (
            (same_reset, "两镜像复位 PC/SP 一致"),
            (same_entry, "两镜像精确命中同一主循环入口 0x%08X" % C.LOOP_LO),
            (delta_ok, "启动指令数差 %d（|Δ| < 2000；补丁只改同步字/菜单小段）" % delta),
            (same_total_delta, "主循环各跑 %d 条后总指令数差仍为 %d（运行路径未分叉）"
             % (args.extra, delta)),
        ):
            print("  [%s] %s" % ("PASS" if cond else "FAIL", msg))
            ok = ok and cond
        result = {"mode": "compare", "ok": ok, "orig": rec_o, "patched": rec_p,
                  "insns_delta": delta}
        print("\n%s boot_to_main（原厂 %d 条 / 补丁 %d 条到主循环）"
              % ("PASS" if ok else "FAIL", rec_o["insns"], rec_p["insns"]))

    if args.json:
        _write_json(args.json, result)
    return 0 if ok else 1


def _write_json(path, obj):
    with open(path, "w", encoding="utf-8") as f:
        json.dump(obj, f, ensure_ascii=False, indent=1)
    print("（结果已写入 %s）" % os.path.abspath(path))


if __name__ == "__main__":
    sys.exit(main())
