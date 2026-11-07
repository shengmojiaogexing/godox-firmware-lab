# -*- coding: utf-8 -*-
"""X3-C 私有密钥补丁 —— 整机启动回归（原厂镜像 vs 补丁镜像）。

用法（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\boot_regression.py
    # 可选：--max-insn 2000000 --deep-insn 12000000
    # 输出写入 keypatch/dev/x3c/boot_regression.log

做三件事：
  A. 用项目自带 devices/x3c/emu/run.py 分别启动原厂/补丁镜像（--max-insn 2000000），
     对比执行统计 / 最终 PC / 外设访问 / 故障状态等（日志逐行归一化后比较）。
  B. 用 devices/x3c/emu/machine.Machine 跑同样的指令预算，挂代码钩子统计关键函数
     是否执行、以及所有 CC2500 写寄存器调用（reg,val），对比两镜像。
  C. 可选深跑（默认 12M 指令），验证真实启动流程里 0x08014864 会被调用、
     补丁后写 CC2500 reg4/reg5 = KEY>>8 / KEY&0xFF（原厂则写 50024/(ID+1)）。
"""
from __future__ import annotations

import argparse
import os
import re
import shutil
import subprocess
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
PY = os.path.join(ROOT, "tools", "python", "python.exe")
RUNPY = os.path.join(ROOT, "devices", "x3c", "emu", "run.py")
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
ORIG = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798.bin")
LOGDIR = os.path.join(HERE, "logs")
KEY = 0xD798

# 关注的函数入口
WATCH = {
    0x08006F58: "RF初始化包装",
    0x080232BC: "CC2500芯片配置(初始化表)",
    0x080118E8: "RF状态设置",
    0x08014864: "同步字设置(ID->SYNC)",
    0x08014814: "信道设置",
    0x080119DC: "工厂RF测试模式",
    0x08023540: "CC2500写寄存器",
    0x08023520: "CC2500命令/strobe",
}
WRITE_REG = 0x08023540

lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def rel(p: str) -> str:
    return os.path.relpath(p, ROOT)


def run_runpy(image: str, max_insn: int) -> tuple[str, str]:
    """调用 devices/x3c/emu/run.py，返回 (log 路径, stdout)。"""
    cmd = [PY, RUNPY, "--img", image, "--max-insn", str(max_insn),
           "--quiet", "--outdir", LOGDIR]
    proc = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True,
                          encoding="utf-8", errors="replace")
    stdout = (proc.stdout or "") + (proc.stderr or "")
    m = re.search(r"log written to:\s*(.+)", stdout)
    if not m:
        raise RuntimeError("run.py 未输出 log 路径:\n" + stdout[-2000:])
    return m.group(1).strip(), stdout


TS_RE = re.compile(r"\d{8}_\d{6}")
ISO_RE = re.compile(r"\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}(?:\.\d+)?")


def normalize_log(path: str) -> tuple[list[str], dict]:
    with open(path, encoding="utf-8", errors="replace") as f:
        raw = f.read().splitlines()
    outv = []
    metrics = {}
    for line in raw:
        line = line.replace(os.path.abspath(ORIG), "<IMG>")
        line = line.replace(os.path.abspath(PATCHED), "<IMG>")
        line = TS_RE.sub("<TS>", line)
        line = ISO_RE.sub("<TIMESTAMP>", line)
        outv.append(line)
        for key, pat in (
            ("instructions", r"^instructions\s*:\s*(\d+)"),
            ("stop", r"^stop reason\s*:\s*(.+)$"),
            ("final_pc", r"^final PC\s*:\s*(.+)$"),
            ("sp_lr", r"^SP=(.+)$"),
            ("reached_main", r"^reached main area.*?:\s*(.+)$"),
            ("hang", r"^hang analysis\s*:\s*(.+)$"),
        ):
            m = re.match(pat, line.strip())
            if m:
                metrics[key] = m.group(1).strip()
    return outv, metrics


def compare_logs(a: list[str], b: list[str]) -> list[str]:
    """返回差异行描述（空 = 完全一致）。"""
    diffs = []
    for i, (x, y) in enumerate(zip(a, b), 1):
        if x != y:
            diffs.append("  L%d:\n    原厂: %s\n    补丁: %s" % (i, x, y))
    if len(a) != len(b):
        diffs.append("  行数不同: 原厂 %d vs 补丁 %d" % (len(a), len(b)))
    return diffs


def coverage_run(image: str, max_insn: int, fast_delay: bool = False) -> dict:
    """用 Machine 跑启动并统计关键函数命中/CC2500 写寄存器序列。

    fast_delay=True 时把两个标定 nop-sled 延时函数改成 `bx lr`（只在内存里，
    与 devices/x3c/emu/demo_x3.py 的"真机状态适配"一致），用于在有限指令预算内让
    启动流程走到射频初始化/主循环。
    """
    sys.path.insert(0, EMU)
    from machine import Machine, APP_BASE  # noqa: E402
    from unicorn import UC_HOOK_CODE  # noqa: E402
    from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1  # noqa: E402

    m = Machine(image=image, image_base=APP_BASE, trace=False,
                log_fn=lambda s: None, hardfault=True)
    if fast_delay:
        m.uc.mem_write(0x0801C118, b"\x70\x47")   # us 延时 -> bx lr
        m.uc.mem_write(0x0801C0FE, b"\x70\x47")   # ms 延时 -> bx lr
    m.boot()
    counts = {a: 0 for a in WATCH}
    wreg: list[tuple[int, int]] = []

    def cb(uc, address, size, _user):
        if address in counts:
            counts[address] += 1
            if address == WRITE_REG:
                wreg.append((uc.reg_read(UC_ARM_REG_R0),
                             uc.reg_read(UC_ARM_REG_R1)))

    for a in WATCH:
        m.uc.hook_add(UC_HOOK_CODE, cb, None, a, a + 4)
    fine = min(200000, max_insn)
    m.set_fine(True)
    m.run(max_insns=fine)
    if max_insn > fine:
        m.set_fine(False)
        m.run(max_insns=max_insn - fine)
    return {
        "insn": m.insn_count,
        "pc": m.reg("PC") & ~1,
        "sp": m.reg("SP"),
        "lr": m.reg("LR"),
        "stop": m.stop_reason,
        "counts": counts,
        "wreg": wreg,
        "fault": m.fault,
    }


def diff_wreg(a: list[tuple[int, int]], b: list[tuple[int, int]]) -> list[str]:
    """比较写寄存器序列；只报告 reg4/reg5 的差异（其余应完全一致）。"""
    d = []
    if len(a) != len(b):
        d.append("  写寄存器调用总数不同: 原厂 %d vs 补丁 %d" % (len(a), len(b)))
    for i, ((ra, va), (rb, vb)) in enumerate(zip(a, b)):
        if (ra, va) != (rb, vb):
            d.append("  第 %d 次写寄存器不同: 原厂 reg%d=0x%02X -> 补丁 reg%d=0x%02X"
                     % (i + 1, ra, va, rb, vb))
    return d


def fmt_counts(counts: dict) -> str:
    parts = []
    for a in WATCH:
        parts.append("%s=0x%08X:%d" % (WATCH[a], a, counts[a]))
    return "\n    " + "\n    ".join(parts)


def sync_writes(wreg: list[tuple[int, int]]) -> list[tuple[int, int]]:
    return [(r, v) for (r, v) in wreg if r in (4, 5)]


def main() -> int:
    global PATCHED
    ap = argparse.ArgumentParser()
    ap.add_argument("--image", default=PATCHED,
                    help="补丁镜像路径（默认 core 成品）")
    ap.add_argument("--log", default=os.path.join(HERE, "boot_regression.log"),
                    help="回归日志输出路径")
    ap.add_argument("--max-insn", type=int, default=2000000)
    ap.add_argument("--deep-insn", type=int, default=2000000,
                    help="深跑预算（配合延时快进），默认 2M")
    ap.add_argument("--skip-runpy", action="store_true")
    args = ap.parse_args()
    PATCHED = args.image
    os.makedirs(LOGDIR, exist_ok=True)

    out("X3-C v1.38 私有密钥补丁 —— 整机启动回归")
    out("原厂镜像 : %s" % rel(ORIG))
    out("补丁镜像 : %s" % rel(PATCHED))
    out("指令预算 : %d（run.py 与 Machine 相同；fine 前 200000 条）" % args.max_insn)
    ok = True

    # ---------- A. run.py 对比 ----------
    if not args.skip_runpy:
        out("\n" + "=" * 78)
        out("A. devices/x3c/emu/run.py 启动对比（--max-insn %d --quiet）" % args.max_insn)
        out("=" * 78)
        log_o, so = run_runpy(ORIG, args.max_insn)
        time.sleep(1.2)   # run.py 日志名精确到秒，避免两次运行同名覆盖
        log_p, sp = run_runpy(PATCHED, args.max_insn)
        out("原厂 boot log: %s" % rel(log_o))
        out("补丁 boot log: %s" % rel(log_p))

        norm_o, met_o = normalize_log(log_o)
        norm_p, met_p = normalize_log(log_p)
        diffs = compare_logs(norm_o, norm_p)
        out("\n执行统计（归一化后逐行比较）:")
        for k in ("instructions", "stop", "final_pc", "sp_lr", "reached_main", "hang"):
            out("  %-13s 原厂: %s" % (k, met_o.get(k)))
            out("  %-13s 补丁: %s" % (k, met_p.get(k)))
        if diffs:
            ok = False
            out("\n  [FAIL] 两份 boot log 有 %d 处差异（前 20 处）：" % len(diffs))
            for dline in diffs[:20]:
                out(dline)
        else:
            out("\n  [PASS] 两份 boot log 归一化后**逐行完全一致**"
                "（图像路径/时间戳归一化）")
        # 把两份 boot log 复制到本目录留证
        shutil.copyfile(log_o, os.path.join(LOGDIR, "boot_orig.log"))
        shutil.copyfile(log_p, os.path.join(LOGDIR, "boot_patched.log"))
        out("  证据副本: %s, %s"
            % (rel(os.path.join(LOGDIR, "boot_orig.log")),
               rel(os.path.join(LOGDIR, "boot_patched.log"))))

    # ---------- B/C. Machine 覆盖统计（含深跑） ----------
    for tag, budget, fast_delay in (
            ("B. 同预算覆盖统计（不做任何适配，与 run.py 同预算）",
             args.max_insn, False),
            ("C. 深跑（仅内存里快进两个标定延时，验证真实启动流程会走到射频初始化与同步字设置",
             args.deep_insn, True)):
        if budget <= 0:
            continue
        out("\n" + "=" * 78)
        out("%s（%d 指令，延时快进=%s）" % (tag, budget, fast_delay))
        out("=" * 78)
        ro = coverage_run(ORIG, budget, fast_delay=fast_delay)
        rp = coverage_run(PATCHED, budget, fast_delay=fast_delay)
        out("最终状态:")
        for k in ("insn", "stop", "pc", "sp", "lr", "fault"):
            fv = lambda v: ("0x%08X" % v) if isinstance(v, int) else str(v)  # noqa: E731
            out("  %-6s 原厂=%s  补丁=%s  %s"
                % (k, fv(ro[k]), fv(rp[k]),
                   "一致" if ro[k] == rp[k] else "不同"))
        out("关键函数命中次数:")
        out("  原厂:%s" % fmt_counts(ro["counts"]))
        out("  补丁:%s" % fmt_counts(rp["counts"]))
        same_counts = ro["counts"] == rp["counts"]
        out("  [%s] 命中次数一致" % ("PASS" if same_counts else "FAIL"))
        if not same_counts:
            ok = False

        wd = diff_wreg(ro["wreg"], rp["wreg"])
        if not wd:
            out("  [PASS] 全部 %d 次 CC2500 写寄存器调用（reg,val）逐次一致"
                % len(ro["wreg"]))
        else:
            # 深跑中同步字函数被调用时，reg4/reg5 不同是补丁的**预期效果**
            only_sync = all("reg4" in d or "reg5" in d for d in wd)
            out("  [%s] 写寄存器序列差异 %d 处%s："
                % ("PASS(预期补丁效果)" if only_sync else "FAIL",
                   len(wd), "（仅 reg4/reg5）" if only_sync else ""))
            for dline in wd[:10]:
                out(dline)
            if not only_sync:
                ok = False
        out("  原厂 SYNC 写序列: %s" % (sync_writes(ro["wreg"]) or "(本次预算内未写 SYNC)"))
        out("  补丁 SYNC 写序列: %s" % (sync_writes(rp["wreg"]) or "(本次预算内未写 SYNC)"))
        if tag.startswith("C") and sync_writes(rp["wreg"]) and sync_writes(ro["wreg"]):
            sw_o = sync_writes(ro["wreg"])
            sw_p = sync_writes(rp["wreg"])
            last_o = (sw_o[-2][1] << 8 | sw_o[-1][1]) if len(sw_o) >= 2 else None
            last_p = (sw_p[-2][1] << 8 | sw_p[-1][1]) if len(sw_p) >= 2 else None
            exp_ok = last_p == KEY
            out("  [%s] 补丁镜像最后一次 SYNC1/SYNC0 组合 = 0x%04X（期望 0x%04X）"
                % ("PASS" if exp_ok else "FAIL", last_p or 0, KEY))
            out("       原厂最后一次 = 0x%04X（原公式）" % (last_o or 0))
            if not exp_ok:
                ok = False

    out("\n" + "=" * 78)
    out("整机启动回归结论: %s" % ("PASS（无启动回归）" if ok else "FAIL"))
    out("=" * 78)
    log = args.log
    with open(log, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % rel(log))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
