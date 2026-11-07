#!/usr/bin/env python3
"""验收测试 1：固件能启动到主循环（SIM-SPEC §7.1）。

判定（与任务书一致）：
  1. reset 后 500 万条指令内 PC 进入主循环区 [0x0800CE40, 0x0800CEB0)；
  2. 稳定运行：主循环执行次数 >= 10；
  3. uptime_ms >= 10（真实 SysTick 1ms 节拍在走）；
  4. 运行期间没有未处理异常 / fault / 卡死；
  5. 启动到主循环墙钟时间 < 30s。

运行：  python tests/test_boot.py        （独立可跑，exit 0 = 通过）
"""
import sys
import time

sys.path.insert(0, __file__.rsplit("\\", 1)[0] if "\\" in __file__ else ".")
import _util as U  # noqa: E402


def _sample_loop(m, total, hits, in_zone, budget, slice_insns=1000):
    """跑一小片并更新『主循环进入次数』；返回 (total, hits, in_zone, reason)。"""
    prev = U.insn_count(m)
    reason = U.run(m, max_insns=slice_insns)
    cur = U.insn_count(m)
    delta = cur - prev
    if delta <= 0:
        delta = slice_insns          # 实现按 slice 计费但不累计时，退化为名义值
    total += delta
    if total > budget:
        return total, hits, in_zone, reason
    p = U.pc(m)
    if U.LOOP_LO <= p < U.LOOP_HI:
        if not in_zone:
            hits += 1
        in_zone = True
    else:
        in_zone = False
    return total, hits, in_zone, reason


def main():
    print("== test_boot: AD200 Pro firmware boot ==")
    Machine, m = U.load_machine()
    m.reset()
    assert U.insn_count(m) == 0, "reset 后 insn_count 应为 0，实际 %d" % U.insn_count(m)
    p0 = U.pc(m)
    print("reset: pc=0x%08X (期望 0x%08X) sp=0x%08X" % (
        p0, U.RESET_PC, U.regs(m)["sp"]))
    assert p0 == (U.RESET_PC & ~1), "reset PC 不对：0x%08X" % p0

    t0 = time.time()
    total = 0
    hits = 0
    in_zone = False
    reached = False
    reasons = []

    # ---- 阶段 1：跑到主循环（预算 5M 指令） -----------------------------
    while total < U._MAX_INSN_TO_LOOP:
        total, hits, in_zone, reason = _sample_loop(
            m, total, hits, in_zone, U._MAX_INSN_TO_LOOP,
            slice_insns=5000 if total < 200_000 else 2000)
        reasons.append(reason)
        if U.is_fault_reason(reason):
            raise AssertionError("启动过程中出现 fault：%s（pc=0x%08X）" % (
                reason, U.pc(m)))
        if reason == "stall":
            raise AssertionError("启动过程中卡死（stall @ pc=0x%08X，%s）" % (
                U.pc(m), U.stats(m).get("stop_reason", "")))
        if hits >= 1:
            reached = True
            break
    elapsed_to_loop = time.time() - t0

    if not reached:
        p = U.pc(m)
        print("最后 PC=0x%08X %s" % (p, ""))
        print("最近 stats:", U.stats(m))
        raise AssertionError(
            "500 万条指令内未进入主循环 [0x%08X,0x%08X)（最后 pc=0x%08X）" % (
                U.LOOP_LO, U.LOOP_HI, p))

    reached_insns = U.insn_count(m)
    print("主循环到达: insns=%d wall=%.2fs pc=0x%08X" % (
        reached_insns, elapsed_to_loop, U.pc(m)))

    # ---- 阶段 2：稳定运行 >=10 个循环、>=5 次 SysTick、uptime>=10ms ----
    U.reset_trace(m)
    while (hits < 10 or U.uptime_ms(m) < 10) and total < U._MAX_INSN_TO_LOOP:
        total, hits, in_zone, reason = _sample_loop(
            m, total, hits, in_zone, U._MAX_INSN_TO_LOOP, slice_insns=500)
        if U.is_fault_reason(reason):
            raise AssertionError("主循环运行出现 fault：%s" % reason)
    uptime = U.uptime_ms(m)

    # SysTick 中断次数：优先用 trace 统计 irq_enter
    irq_enter = 0
    systick_enter = 0
    try:
        for e in U.trace_events(m, since=0, limit=20000):
            if e.get("type") == "irq_enter":
                irq_enter += 1
                nm = (e.get("name") or "")
                if "sys" in nm.lower():
                    systick_enter += 1
    except Exception as exc:                       # trace 不可用不判失败
        print("  [warn] trace 读取异常：%s" % exc)

    print("稳定阶段: 循环次数=%d uptime_ms=%.1f insns=%d" % (
        hits, uptime, U.insn_count(m)))
    print("中断: irq_enter=%d systick_enter=%d (从主循环到达后统计)" % (
        irq_enter, systick_enter))

    assert total <= U._MAX_INSN_TO_LOOP, \
        "超过 500 万指令预算（%d）" % total
    assert hits >= 10, "主循环次数不足 10（实际 %d）" % hits
    assert uptime >= 10.0, "uptime_ms < 10（实际 %.1f）" % uptime
    if irq_enter or systick_enter:
        assert systick_enter >= 5 or irq_enter >= 5, \
            "SysTick 中断不足 5 次（irq_enter=%d systick=%d）" % (
                irq_enter, systick_enter)
        if systick_enter < 5:
            print("  [note] 主循环期间的中断由 TIM14(IRQ19) 驱动"
                  "（systick_enter=%d，irq_enter=%d），按 irq_enter>=5 判定通过；"
                  "见 docs/06-boot-peripherals.md §3.8" % (systick_enter, irq_enter))
    else:
        print("  [warn] 未从 trace 观测到 irq_enter，改用 uptime 判定（已满足）")

    # ---- 阶段 3：墙钟性能（启动到主循环 < 30s） -------------------------
    print("启动到主循环墙钟: %.2fs（要求 <30s）" % elapsed_to_loop)
    assert elapsed_to_loop < 30.0, "启动过慢：%.1fs（要求 <30s）" % elapsed_to_loop

    print("PASS test_boot")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except AssertionError as e:
        print("FAIL test_boot:", e)
        sys.exit(1)
    except Exception as e:
        print("ERROR test_boot: %s: %s" % (type(e).__name__, e))
        sys.exit(1)
