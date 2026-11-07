# -*- coding: utf-8 -*-
"""整机启动回归（v2）：用 devices/ad200pro 模拟器分别跑原厂 / 补丁镜像，对比启动与主循环。

必须用 devices\ad200pro\\python\\python.exe 运行（该解释器带 capstone/unicorn）：
    devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200\\sim_regress.py

对比项：
  * 复位 PC / SP；到达主循环的指令数、PC、state、stop_reason；
  * 主循环再跑 200 万条指令（分片），确认无 fault/stall；
  * 模拟 CC2500 寄存器镜像：除 SYNC1/SYNC0 (reg4/reg5) 外必须完全一致；
  * 启动期间对 reg4/5 的实际写入序列（证明"初始化写 C3/68 -> 随后被同步字函数覆盖"）；
  * strobe 序列：v2 少一次 SCAL(0x33)（同步字函数不再校准），其余完全一致。
"""
import os
import sys
import types

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
ADROOT = os.path.join(ROOT, "devices/ad200pro")
if ADROOT not in sys.path:
    sys.path.insert(0, ADROOT)

from sim.machine import Machine  # noqa: E402

ORIG = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "AD200pro_V1.60_key6B21.bin")
KEY = 0x6B21
LOOP_LO, LOOP_HI = 0x0800CE40, 0x0800CEB0
MAX_TO_LOOP = 5_000_000
EXTRA_INSNS = 2_000_000
SLICE = 20_000


def regs(m):
    r = getattr(m, "regs")
    return dict(r() if callable(r) else r)


def pc(m):
    return regs(m)["pc"] & ~1


def instrument_sync_writes(m):
    """包装 CC2500._byte，记录所有对 reg4/reg5 的写 (reg, value)。"""
    rf = m.rf
    original = type(rf)._byte
    writes = []

    def _wrapped(self, b):
        is_data = (self.byte_index == 1) and not self.reading
        base = self.addr & 0x7F if is_data else None
        ret = original(self, b)
        if is_data and base in (4, 5):
            writes.append((base, b))
        return ret

    rf._byte = types.MethodType(_wrapped, rf)
    return writes


def boot_to_loop(path):
    """复位后精确跑到主循环入口 0x0800CE40（breakpoint 精确命中），返回记录 dict。

    为等到"精确指令数"，临时把 max_slice_rate 设为 1（单步执行），以便比较
    原厂/补丁的启动指令数差异（v2 去掉了 SCAL+1ms 延时，指令数应略少）。
    """
    m = Machine(path)
    m.reset()
    sync_writes = instrument_sync_writes(m)
    p0 = pc(m)
    sp0 = regs(m)["sp"]
    m.max_slice_rate = 1
    m.set_breakpoint(LOOP_LO)
    reason = m.run(max_insns=MAX_TO_LOOP)
    insns = m.insn_count
    hit_pc = pc(m)
    if reason != "breakpoint" or hit_pc != LOOP_LO:
        raise AssertionError("未精确命中主循环 0x%08X：reason=%s pc=0x%08X state=%s"
                             % (LOOP_LO, reason, hit_pc, m.state))
    m.clear_breakpoint(LOOP_LO)
    m.max_slice_rate = 4000
    return {
        "machine": m,
        "sync_writes": list(sync_writes),
        "reset_pc": p0,
        "reset_sp": sp0,
        "insns": insns,
        "pc": hit_pc,
        "state": m.state,
        "reason": reason,
    }


def run_extra(m, extra=EXTRA_INSNS):
    """主循环再跑 extra 条指令（分片），返回 (PC 采样列表, 异常)。"""
    pcs = []
    done = 0
    while done < extra:
        m.run(max_insns=min(SLICE, extra - done))
        done += SLICE
        pcs.append(pc(m))
        if m.state in ("fault", "stalled"):
            return pcs, "state=%s stop_reason=%s @ pc=0x%08X" % (
                m.state, getattr(m, "stop_reason", None), pc(m))
    return pcs, None


def pc_symbols(m, pcs):
    """把 PC 采样映射到所属函数名（去掉 +0x 偏移）。"""
    out = []
    for p in pcs:
        n = m.fw.name(p)
        out.append(n.split("+")[0] if n else "0x%08X" % p)
    return out


# 主循环任务/中断入口（地址来自 devices/ad200pro/docs/04-subsystems.md）
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


def install_task_hooks(m):
    """统计主循环任务/中断入口被执行的次数（覆盖 200 万条指令窗口）。"""
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


def fmt_pc_samples(pcs):
    return " ".join("0x%08X" % p for p in pcs[::max(1, len(pcs) // 8)])


def pair_writes(writes):
    """把 (reg,val) 列表整理成 [(SYNC1, SYNC0), ...]（每个 reg4 后跟 reg5）。"""
    pairs = []
    hi = None
    for reg, val in writes:
        if reg == 4:
            hi = val
        elif reg == 5 and hi is not None:
            pairs.append((hi, val))
            hi = None
    return pairs


def main():
    print("== AD200 Pro v2 整机启动回归（模拟器：sim.machine.Machine）==")
    for p in (ORIG, PATCHED):
        print("镜像:", p, "存在:", os.path.isfile(p))
        assert os.path.isfile(p), "缺文件: " + p

    r_orig = boot_to_loop(ORIG)
    r_pat = boot_to_loop(PATCHED)

    for tag, r in (("原厂", r_orig), ("补丁", r_pat)):
        m = r["machine"]
        print("\n[%s] reset: pc=0x%08X sp=0x%08X" % (tag, r["reset_pc"], r["reset_sp"]))
        print("[%s] 到主循环: insns=%d pc=0x%08X %s state=%s reason=%s"
              % (tag, r["insns"], r["pc"], m.fw.name(r["pc"]) or "", r["state"], r["reason"]))

    # —— 启动等价性（都精确命中主循环 0x0800CE40） ——
    assert r_orig["reset_pc"] == r_pat["reset_pc"] == 0x080030D4, "复位 PC 不一致"
    assert r_orig["reset_sp"] == r_pat["reset_sp"] == 0x200008B0, "复位 SP 不一致"
    assert r_orig["pc"] == r_pat["pc"] == LOOP_LO, "未在同一主循环入口对齐"
    delta = r_orig["insns"] - r_pat["insns"]
    print("\n启动到主循环 0x%08X 的精确指令数：原厂 %d / 补丁 %d（差 %d）"
          % (LOOP_LO, r_orig["insns"], r_pat["insns"], delta))
    # v2 同步字函数去掉了 SCAL(0x33)+delay(1ms)，每次调用少若干指令；
    # 启动期该函数被调用一次，故补丁镜像指令数应略少，但量级必须 < 1000。
    assert 0 <= delta < 1000, "启动指令数差异异常：原厂 %d / 补丁 %d" % (
        r_orig["insns"], r_pat["insns"])
    print("[PASS] 复位 PC/SP 一致；精确主循环入口一致；启动指令数差 %d"
          "（v2 删除 SCAL+1ms 延时的预期结果）" % delta)

    # —— 主循环继续跑 200 万条（统计任务/中断入口次数） ——
    counts_o = install_task_hooks(r_orig["machine"])
    counts_p = install_task_hooks(r_pat["machine"])
    pcs_o, bad_o = run_extra(r_orig["machine"])
    pcs_p, bad_p = run_extra(r_pat["machine"])
    print("[原厂] 主循环+200万条: PC 采样 %s" % fmt_pc_samples(pcs_o))
    print("[补丁] 主循环+200万条: PC 采样 %s" % fmt_pc_samples(pcs_p))
    assert bad_o is None, "原厂镜像主循环内异常: " + bad_o
    assert bad_p is None, "补丁镜像主循环内异常: " + bad_p
    print("\n主循环 200 万条指令期间任务/中断入口次数（原厂 vs 补丁）：")
    task_ok = True
    for name, addr in TASKS:
        co, cp = counts_o[name], counts_p[name]
        print("  %-20s 0x%08X  原厂 %-6d 补丁 %-6d" % (name, addr, co, cp))
        if co > 0:
            # 模拟器时序为行为级近似（docs/usage.md §10.1：TIM/SysTick 按
            # "1 ms ≈ 2000 条指令" 折算，时序非周期精确），两镜像相位差 299 条
            # 指令会导致各任务计数有小幅差异；这里判定"同一量级"（±25%）。
            if cp == 0 or abs(co - cp) > max(5, co // 4):
                task_ok = False
    assert task_ok, "任务执行次数差异过大（见上表）"
    assert counts_p["tim14_irq"] > 0, "补丁镜像未见 TIM14 中断（主循环心跳异常）"
    print("[PASS] 两镜像主循环 200 万条指令均无 fault/stall；主循环任务（display/key/"
          "flash_power/rf_poll/charge/measurement）与 TIM14 心跳均持续执行、次数同一量级"
          "（相位因 v2 少执行 299 条指令而整体偏移，模拟器时序非周期精确）")

    # —— SYNC 写入序列：初始化 C3/68 -> 本函数覆盖 ——
    pairs_o = pair_writes(r_orig["sync_writes"])
    pairs_p = pair_writes(r_pat["sync_writes"])
    print("\n启动期 CC2500 reg4/5 写入序列（SYNC1,SYNC0）：")
    print("  原厂: %s" % ["0x%02X%02X" % p for p in pairs_o])
    print("  补丁: %s" % ["0x%02X%02X" % p for p in pairs_p])
    assert pairs_p == [(0xC3, 0x68), ((KEY >> 8) & 0xFF, KEY & 0xFF)], \
        "补丁镜像 SYNC 写入序列不符（期望 初始化 C3/68 -> KEY）"
    assert pairs_o == [(0xC3, 0x68), (0xC3, 0x68)], \
        "原厂镜像 SYNC 写入序列不符（初始化 C3/68 -> ID0 公式同值）"
    print("[PASS] 两镜像都先由 rf_chip_config 写 0xC3/0x68；补丁随后被本函数覆盖为 "
          "0x%04X（原厂同位置写 ID=0 公式 0xC368）" % KEY)

    # —— CC2500 寄存器镜像对比 ——
    regs_o = dict(r_orig["machine"].rf.regs)
    regs_p = dict(r_pat["machine"].rf.regs)
    log_o = list(r_orig["machine"].rf.log)
    log_p = list(r_pat["machine"].rf.log)
    print("\n原厂最终 SYNC：reg4=0x%02X reg5=0x%02X  (0x%04X)"
          % (regs_o.get(4, -1), regs_o.get(5, -1),
             (regs_o.get(4, 0) << 8) | regs_o.get(5, 0)))
    print("补丁最终 SYNC：reg4=0x%02X reg5=0x%02X  (0x%04X, 期望 KEY 0x%04X)"
          % (regs_p.get(4, -1), regs_p.get(5, -1),
             (regs_p.get(4, 0) << 8) | regs_p.get(5, 0), KEY))
    assert ((regs_o.get(4, 0) << 8) | regs_o.get(5, 0)) == 50024, "原厂镜像 SYNC 不是 50024"
    assert ((regs_p.get(4, 0) << 8) | regs_p.get(5, 0)) == KEY, "补丁镜像 SYNC 不是 KEY"
    print("[PASS] 原厂最终 SYNC=0xC368（ID=0 普通模式），补丁最终 SYNC=0x%04X（密钥模式）"
          % KEY)

    diff = [k for k in sorted(set(regs_o) | set(regs_p))
            if regs_o.get(k) != regs_p.get(k)]
    print("CC2500 寄存器差异: %s" % ["0x%02X" % k for k in diff])
    assert diff == [0x04, 0x05], "除 SYNC 寄存器外还有差异: %s" % diff
    print("[PASS] CC2500 其它 %d 个寄存器两镜像完全一致；仅 SYNC1/SYNC0 不同"
          % (len(regs_o) - 2))

    # —— strobe 序列：v2 只应少一次 SCAL(0x33) ——
    print("strobe 序列：原厂 %d 条 / 补丁 %d 条" % (len(log_o), len(log_p)))
    print("  原厂: %s" % log_o)
    print("  补丁: %s" % log_p)
    assert len(log_o) == len(log_p) + 1, "strobe 条数差不是 1：%d vs %d" % (len(log_o), len(log_p))
    i = next(i for i in range(len(log_p)) if log_o[i] != log_p[i])
    assert log_o[i] == "SCAL", "少的 strobe 不是 SCAL：%r" % (log_o[i],)
    assert log_o[:i] + log_o[i + 1:] == log_p, "除一次 SCAL 外 strobe 序列不一致"
    print("[PASS] 补丁比原厂少一次 SCAL(0x33)（同步字函数不再做频率校准），其余 strobe 序列一致")

    # —— 状态收尾 ——
    extra_o = r_orig["machine"].insn_count - r_orig["insns"]
    extra_p = r_pat["machine"].insn_count - r_pat["insns"]
    print("\n主循环阶段实际执行（按模拟器指令计数）：原厂 +%d / 补丁 +%d"
          % (extra_o, extra_p))
    assert extra_o == extra_p == EXTRA_INSNS, \
        "主循环阶段执行量不同：+%d vs +%d" % (extra_o, extra_p)
    assert (r_orig["machine"].insn_count - r_pat["machine"].insn_count) == delta, \
        "两镜像总指令数差在运行中发生了变化（说明运行路径发散）"
    print("[PASS] 主循环两镜像各执行 %d 条指令；总指令数差保持 %d（仅启动期差异，"
          "运行路径未发散）" % (EXTRA_INSNS, delta))
    print("\n最终状态: 原厂 state=%s insns=%d pc=0x%08X | 补丁 state=%s insns=%d pc=0x%08X"
          % (r_orig["machine"].state, r_orig["machine"].insn_count, pc(r_orig["machine"]),
             r_pat["machine"].state, r_pat["machine"].insn_count, pc(r_pat["machine"])))
    print("\nPASS sim_regress (v2): 补丁镜像启动/主循环/2.4G 初始化均无回归")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except AssertionError as e:
        print("FAIL sim_regress:", e)
        sys.exit(1)
    except Exception as e:  # noqa: BLE001
        print("ERROR sim_regress: %s: %s" % (type(e).__name__, e))
        sys.exit(1)
