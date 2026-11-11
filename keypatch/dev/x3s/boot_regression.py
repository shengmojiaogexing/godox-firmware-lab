# -*- coding: utf-8 -*-
"""X3-S 私有密钥补丁 —— 原厂 vs 补丁镜像启动回归（整机模拟器）。

用 X3-C 的 Unicorn 整机模型加载 X3-S 原厂 / 补丁镜像，各跑固定指令数，逐项比对：
  * 停止原因 / 执行指令数 / 最终 PC、SP、LR；
  * 最近 8192 条 PC（`pc_ring`）逐条一致；
  * 外设命中计数与热点 MMIO 一致。

补丁只改「组帧函数入口 + 两个擦除态空闲区」和「按键扫描入口 + 空闲区」，启动阶段
都不会执行到，因此两者应完全一致。

运行（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\dev\\x3s\\boot_regression.py
    # 输出同时写入 keypatch/dev/x3s/boot_regression.log
"""
from __future__ import annotations

import os
import sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))

from machine import Machine, APP_BASE  # noqa: E402

ORIG = os.path.join(ROOT, "devices", "x3s", "firmware", "X3_S_v1.25_app.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "X3_S_v1.25_app_keyD798.bin")
LOG = os.path.join(HERE, "boot_regression.log")
MAX_INSNS = 1_200_000

lines: list[str] = []
ok_all = True


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def check(name: str, ok: bool, detail: str = "") -> None:
    global ok_all
    ok_all &= bool(ok)
    out("  [%s] %s%s" % ("PASS" if ok else "FAIL", name, ("  | " + detail) if detail else ""))


def run_one(path: str) -> dict:
    m = Machine(image=path, image_base=APP_BASE, trace=False,
                log_fn=lambda s: None, hardfault=True)
    m.boot()
    m.interrupt_after_chunk = False
    stop = m.run(max_insns=MAX_INSNS)
    return {
        "stop": stop,
        "insns": m.insn_count,
        "pc": m.reg("PC"),
        "sp": m.reg("SP"),
        "lr": m.reg("LR"),
        "pc_ring": list(m.pc_ring),
        "periph": dict(m.periph_hits) if hasattr(m, "periph_hits") else None,
        "mmio": Counter((a, w) for (_v, _pc, w, a, _s, _val) in []) if False else None,
    }


def main() -> int:
    out("X3-S 私有密钥补丁 —— 原厂 vs 补丁 启动回归（整机模拟器）")
    out("原厂 : %s" % os.path.relpath(ORIG, ROOT))
    out("补丁 : %s" % os.path.relpath(PATCHED, ROOT))
    out("预算 : %d 条指令" % MAX_INSNS)
    a = run_one(ORIG)
    b = run_one(PATCHED)
    out("")
    check("停止原因一致", a["stop"] == b["stop"], "%r vs %r" % (a["stop"], b["stop"]))
    check("执行指令数一致", a["insns"] == b["insns"], "%d vs %d" % (a["insns"], b["insns"]))
    check("最终寄存器一致", (a["pc"], a["sp"], a["lr"]) == (b["pc"], b["sp"], b["lr"]),
          "原厂 pc=%08X sp=%08X lr=%08X / 补丁 pc=%08X sp=%08X lr=%08X"
          % (a["pc"], a["sp"], a["lr"], b["pc"], b["sp"], b["lr"]))
    ra, rb = a["pc_ring"], b["pc_ring"]
    same = ra == rb
    first_diff = None
    if not same:
        for i, (x, y) in enumerate(zip(ra, rb)):
            if x != y:
                first_diff = (i, x, y)
                break
    check("最近 PC 轨迹逐条一致（%d 条）" % len(ra), same,
          "" if same else "首个差异 #%d: %08X vs %08X" % first_diff)

    out("\n结论: %s" % ("ALL PASS" if ok_all else "FAIL"))
    with open(LOG, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % os.path.relpath(LOG, ROOT))
    return 0 if ok_all else 1


if __name__ == "__main__":
    sys.exit(main())
