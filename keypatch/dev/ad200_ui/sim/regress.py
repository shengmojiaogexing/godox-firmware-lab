#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""regress.py —— 原厂 vs 补丁 整机启动回归（启动指令数 / 主循环任务计数 / 无 fault）。

参考 keypatch/dev/ad200/sim_regress.py 的做法，但面向 AD200 Pro v4（密钥开关）：
只对比"启动与主循环不回归"，不假设补丁的具体字节（补丁由 Lead 产出）。

对比项：
  * 复位 PC / SP 一致；
  * 复位后精确跑到主循环入口 0x0800CE40（单步精确计数）—— 两镜像入口一致；
  * 启动指令数差 |Δ| < 2000（补丁只改同步字函数/C.Fn 小段，量级必须很小）；
  * 主循环再各跑 --extra 条（默认 200 万）都无 fault/stall；
  * 主循环任务/中断入口计数同一量级（±25%）；
  * TIM14 心跳（IRQ19）在补丁镜像里仍在跳；
  * 主循环阶段总指令数差保持不变（说明运行路径没有分叉）。

补丁镜像不存在时：打印 SKIP，只跑原厂自检，退出码 0（便于在 Lead 产出前先跑通脚手架）。

用法：
    devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\sim\\regress.py
    ... regress.py --patched D:\\...\\AD200pro_V1.60_keyD798.bin --extra 4000000
    ... regress.py --orig 原厂.bin --json out.json

退出码：0 = PASS（或 SKIP）；1 = FAIL。
日志：`> log_regress.txt`。
"""
import argparse
import json
import os
import sys
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import _sim_common as C  # noqa: E402

SLICE = 20_000

# 主循环任务/中断入口（地址来自 devices/ad200pro/docs/04-subsystems.md 与 BRIEF）
TASKS = [
    ("main", 0x0800CDB0),
    ("adc_task", 0x08003598),
    ("display_task", 0x08009228),
    ("ui_key_scan_task", 0x08008818),
    ("flash_power_task", 0x08004500),
    ("rf_tx_task", 0x0800AAF4),
    ("rf_poll_settings", 0x0800B5B0),
    ("charge_enable_task", 0x0800BA00),
    ("charge_monitor", 0x08005A04),
    ("iwdg_refresh", 0x08006F68),
    ("tim14_irq", 0x0800BDE0),
    ("rf_rx_timeout_task", 0x0800B888),
    ("measurement_task", 0x08005F6C),
]


def boot_to_loop(path):
    """复位后精确跑到主循环入口（breakpoint + 单步，指令数精确）。"""
    m = C.load_machine(path)
    info = C.boot_to_main(m, exact_insns=True, until_pc=C.LOOP_LO)
    return {
        "machine": m,
        "insns": info["insns"],
        "pc": info["pc"],
        "state": info["state"],
        "reason": info["reason"],
        "wall": info["wall"],
        "reset_pc": m.fw.reset_pc,
        "reset_sp": m.fw.reset_sp,
    }


def install_task_hooks(m):
    from unicorn import UC_HOOK_CODE
    counts = {name: 0 for name, _ in TASKS}
    by_addr = {addr: name for name, addr in TASKS}

    def _hook(uc, address, size, _ud):
        n = by_addr.get(address)
        if n is not None:
            counts[n] += 1

    for _, addr in TASKS:
        m.uc.hook_add(UC_HOOK_CODE, _hook, begin=addr, end=addr + 1)
    return counts


def run_extra(m, extra):
    """主循环再跑 extra 条（分片），返回 (ok, reason)。"""
    total = 0
    while total < extra:
        prev = m.insn_count
        m.run(max_insns=min(SLICE, extra - total))
        total += m.insn_count - prev
        if m.state in ("fault", "stalled"):
            return False, "state=%s stop_reason=%s pc=0x%08X" % (
                m.state, m.stop_reason, m.regs()["pc"] & ~1)
    return True, "ok"


def fmt_pcs(m, pcs):
    return " ".join("0x%08X" % p for p in pcs[::max(1, len(pcs) // 6)])


def main(argv=None):
    C.ensure_stdout_utf8()
    ap = argparse.ArgumentParser(description="AD200 Pro 原厂 vs 补丁 整机启动回归")
    ap.add_argument("--orig", help="原厂固件路径（默认 devices/ad200pro/firmware/AD200pro_V1.60.bin）")
    ap.add_argument("--patched", help="补丁固件路径（默认 keypatch/out/AD200pro_V1.60_keyD798.bin）")
    ap.add_argument("--extra", type=int, default=2_000_000, help="主循环额外指令数（默认 200 万）")
    ap.add_argument("--json", help="结果写入 JSON")
    args = ap.parse_args(argv)

    orig = C.resolve_orig(args.orig)
    patched = os.path.abspath(args.patched or C.PATCHED_DEFAULT)
    print("== regress: AD200 Pro 整机启动回归（模拟器 sim.machine.Machine）==")
    print("原厂镜像: %s（%d 字节 sha256=%s）"
          % (orig, os.path.getsize(orig), C.sha256_hex(C.read_file_bytes(orig))))
    print("补丁镜像: %s（存在: %s）" % (patched, os.path.isfile(patched)))
    result = {"orig": orig, "patched": patched, "extra": args.extra}

    if not os.path.isfile(patched):
        print("\n[SKIP] 补丁镜像不存在，只跑原厂自检；Lead 产出补丁后重跑即可得到完整对比。")
        r_o = boot_to_loop(orig)
        print("[原厂] 复位 pc=0x%08X sp=0x%08X 到主循环 insns=%d pc=0x%08X wall=%.2fs"
              % (r_o["reset_pc"], r_o["reset_sp"], r_o["insns"], r_o["pc"], r_o["wall"]))
        ok = (r_o["reason"] == "breakpoint" and r_o["pc"] == C.LOOP_LO
              and r_o["reset_pc"] == C.RESET_PC and r_o["reset_sp"] == 0x200008B0)
        print("  [%s] 原厂自检：精确命中主循环入口 0x%08X" % ("PASS" if ok else "FAIL", C.LOOP_LO))
        counts = install_task_hooks(r_o["machine"])
        extra_ok, why = run_extra(r_o["machine"], min(args.extra, 400_000))
        print("  [%s] 原厂主循环额外 40 万条无 fault/stall（%s）"
              % ("PASS" if extra_ok else "FAIL", why))
        ok = ok and extra_ok
        result.update({"ok": ok, "skipped_patched": True})
        print("\n%s regress（SKIP：缺补丁镜像）" % ("PASS" if ok else "FAIL"))
        _finish(args, result, ok)
        return 0 if ok else 1

    # ---- 启动到主循环（精确单步计数）----
    r_o = boot_to_loop(orig)
    r_p = boot_to_loop(patched)
    for tag, r in (("原厂", r_o), ("补丁", r_p)):
        print("[%s] reset pc=0x%08X sp=0x%08X 到主循环 insns=%d pc=0x%08X state=%s wall=%.2fs"
              % (tag, r["reset_pc"], r["reset_sp"], r["insns"], r["pc"],
                 r["state"], r["wall"]))
    result["orig_insns_to_loop"] = r_o["insns"]
    result["patched_insns_to_loop"] = r_p["insns"]
    delta = r_o["insns"] - r_p["insns"]
    result["insns_delta"] = delta

    checks = []

    def need(cond, msg):
        checks.append((cond, msg))
        print("  [%s] %s" % ("PASS" if cond else "FAIL", msg))

    print("\n[启动等价性]")
    need(r_o["reset_pc"] == r_p["reset_pc"] == C.RESET_PC, "复位 PC 一致 = 0x%08X" % C.RESET_PC)
    need(r_o["reset_sp"] == r_p["reset_sp"] == 0x200008B0, "复位 SP 一致 = 0x200008B0")
    need(r_o["pc"] == r_p["pc"] == C.LOOP_LO, "精确命中同一主循环入口 0x%08X" % C.LOOP_LO)
    need(abs(delta) < 2000, "启动指令数差 %d（原厂 %d / 补丁 %d，|Δ| < 2000）"
         % (delta, r_o["insns"], r_p["insns"]))

    # ---- 主循环额外执行 + 任务计数 ----
    print("\n[主循环额外 %d 条]" % args.extra)
    counts_o = install_task_hooks(r_o["machine"])
    counts_p = install_task_hooks(r_p["machine"])
    ok_o, why_o = run_extra(r_o["machine"], args.extra)
    ok_p, why_p = run_extra(r_p["machine"], args.extra)
    print("  [%s] 原厂无 fault/stall（%s）" % ("PASS" if ok_o else "FAIL", why_o))
    print("  [%s] 补丁无 fault/stall（%s）" % ("PASS" if ok_p else "FAIL", why_p))
    print("\n  主循环任务/中断入口次数（原厂 vs 补丁）：")
    task_ok = True
    for name, addr in TASKS:
        co, cp = counts_o[name], counts_p[name]
        print("    %-20s 0x%08X  原厂 %-6d 补丁 %-6d" % (name, addr, co, cp))
        if co > 0:
            # 模拟器时序是行为级近似（1ms ≈ 2000 条指令），两镜像相位差会让计数
            # 有小幅波动；这里只要求同一量级（±25%）。
            if cp == 0 or abs(co - cp) > max(5, co // 4):
                task_ok = False
    need(ok_o and ok_p, "两镜像主循环 %d 条均无 fault/stall" % args.extra)
    need(task_ok, "主循环任务/中断计数同一量级（±25%）")
    need(counts_p["tim14_irq"] > 0, "补丁镜像 TIM14 心跳仍在跳（tim14_irq=%d）"
         % counts_p["tim14_irq"])

    # ---- 路径不分叉：主循环阶段增量相同 ----
    extra_o = r_o["machine"].insn_count - r_o["insns"]
    extra_p = r_p["machine"].insn_count - r_p["insns"]
    same_delta = (r_o["machine"].insn_count - r_p["machine"].insn_count) == delta
    need(extra_o == args.extra and extra_p == args.extra,
         "两镜像主循环各执行 %d 条（原厂 +%d / 补丁 +%d）"
         % (args.extra, extra_o, extra_p))
    need(same_delta, "总指令数差保持 %d（运行路径未分叉）" % delta)

    print("\n最终状态: 原厂 state=%s insns=%d pc=0x%08X %s"
          % (r_o["machine"].state, r_o["machine"].insn_count,
             r_o["machine"].regs()["pc"] & ~1,
             C.read_symbol_name(r_o["machine"], r_o["machine"].regs()["pc"] & ~1)))
    print("          补丁 state=%s insns=%d pc=0x%08X %s"
          % (r_p["machine"].state, r_p["machine"].insn_count,
             r_p["machine"].regs()["pc"] & ~1,
             C.read_symbol_name(r_p["machine"], r_p["machine"].regs()["pc"] & ~1)))

    task_counts = {name: {"orig": counts_o[name], "patched": counts_p[name]}
                   for name, _ in TASKS}
    result["task_counts"] = task_counts
    ok = all(c for c, _ in checks)
    result["ok"] = ok
    print("\n%s regress（原厂 %d 条 / 补丁 %d 条到主循环，Δ=%d）"
          % ("PASS" if ok else "FAIL", r_o["insns"], r_p["insns"], delta))
    _finish(args, result, ok)
    return 0 if ok else 1


def _finish(args, result, ok):
    if args.json:
        with open(args.json, "w", encoding="utf-8") as f:
            json.dump(result, f, ensure_ascii=False, indent=1)
        print("（结果已写入 %s）" % os.path.abspath(args.json))


if __name__ == "__main__":
    sys.exit(main())
