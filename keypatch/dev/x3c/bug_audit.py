# -*- coding: utf-8 -*-
"""Bug 审计 v2：空闲长跑，抓 RF 帧序列 / KEY_ON 写 / 读键 / GPIOA 配置 / 扫描周期。

用法（）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\bug_audit.py --insn 20000000
    # 输出写 keypatch/dev/x3c/bug_audit.log（UTF-8）
"""
from __future__ import annotations

import argparse
import hashlib
import os
import statistics
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

from machine import Machine, APP_BASE  # noqa: E402
from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_WRITE  # noqa: E402
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,  # noqa: E402
                               UC_ARM_REG_LR, UC_ARM_REG_PC)

import devices.x3c as x3c  # noqa: E402

BIN = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798_keybtn4.bin")
MAIN = 0x08033E20
WATCH_LO, WATCH_HI = 0x20001080, 0x200010A0
GPIOA = 0x40020000
DELAY = 0x0801C118
SEND_SHORT = x3c.SEND_SHORT
SEND_PROP = x3c.SEND_PROP

lines: list[str] = []


def out(msg=""):
    lines.append(msg)
    print(msg)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--insn", type=int, default=20_000_000)
    args = ap.parse_args()

    out("审计镜像: %s" % os.path.relpath(BIN, ROOT))
    data = open(BIN, "rb").read()
    out("sha256: %s" % hashlib.sha256(data).hexdigest())

    m = Machine(image=BIN, image_base=APP_BASE, trace=False,
                log_fn=lambda s: None, hardfault=True)
    m.uc.mem_write(0x0801C118, b"\x70\x47")
    m.uc.mem_write(0x0801C0FE, b"\x70\x47")
    SENT = 0x20046000
    m.uc.mem_write(SENT, bytes.fromhex("fee7"))
    m.boot()
    m.interrupt_after_chunk = False
    m.run_until(MAIN, max_insns=2_000_000)
    out("启动到 main: insn=%d PC=0x%08X" % (m.insn_count, m.reg("PC") & ~1))
    # 恢复真实延时函数（校准扫描周期）
    orig = open(os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin"), "rb").read()
    for a in (DELAY, 0x0801C0FE):
        m.uc.mem_write(a, orig[a - APP_BASE:a - APP_BASE + 2])

    # KEY_ON=1，清 keybtn4 计数器/flags
    m.uc.mem_write(x3c.KEY_ON_FLAG_RAM, b"\x01")
    for ram in (x3c.BTN_RAM_WHEEL_CNT16, x3c.BTN_RAM_WHEEL_FIRED1,
                x3c.BTN_RAM_WHEEL_FIRED2, x3c.BTN_RAM1_CNT, x3c.BTN_RAM1_FIRED):
        m.uc.mem_write(ram, b"\x00")

    st = {"scans": 0, "writes": [], "reads": {}, "gpioa": [],
          "frames": [], "shutdown": 0, "delays": []}
    pending_reads = []
    pending_delay = None
    masks = {x3c.BTN0_MASK: "PA9", x3c.BTN1_MASK: "PA8", x3c.BTN2_MASK: "PB15"}

    def code_hook(uc, address, size, _u):
        nonlocal pending_delay
        if address == x3c.KEYSCAN_FN:
            st["scans"] += 1
            return
        if address == x3c.READ_KEY:
            pending_reads.append((uc.reg_read(UC_ARM_REG_LR) & ~1,
                                  uc.reg_read(UC_ARM_REG_R1)))
            return
        if pending_reads and address == pending_reads[-1][0]:
            _lr, mask = pending_reads.pop()
            name = masks.get(mask, hex(mask))
            d = st["reads"].setdefault(name, {0: 0, 1: 0})
            r0 = uc.reg_read(UC_ARM_REG_R0)
            d[0 if r0 == 0 else 1] += 1
            return
        if address == DELAY:
            pending_delay = [uc.reg_read(UC_ARM_REG_LR) & ~1,
                             uc.reg_read(UC_ARM_REG_R0), m.insn_count]
            return
        if pending_delay is not None and address == pending_delay[0]:
            if pending_delay[1] == 1000:
                st["delays"].append((1000, m.insn_count - pending_delay[2]))
            pending_delay = None
            return
        if address == SEND_SHORT:
            st["frames"].append(("S", uc.reg_read(UC_ARM_REG_R0), 0, 0,
                                 m.insn_count, st["scans"]))
            return
        if address == SEND_PROP:
            st["frames"].append(("P", uc.reg_read(UC_ARM_REG_R0),
                                 uc.reg_read(UC_ARM_REG_R1),
                                 uc.reg_read(UC_ARM_REG_R2),
                                 m.insn_count, st["scans"]))
            return
        if address == x3c.SHUTDOWN_WRAPPER:
            st["shutdown"] += 1
            return

    def mem_hook(uc, access, address, size, value, _u):
        if WATCH_LO <= address < WATCH_HI:
            st["writes"].append((uc.reg_read(UC_ARM_REG_PC), address,
                                 value & 0xFF, m.insn_count))
        if GPIOA <= address < GPIOA + 0x20:
            st["gpioa"].append((uc.reg_read(UC_ARM_REG_PC), address,
                                value, m.insn_count))

    for a in (x3c.KEYSCAN_FN, x3c.READ_KEY, DELAY, SEND_SHORT, SEND_PROP,
              x3c.SHUTDOWN_WRAPPER):
        m.uc.hook_add(UC_HOOK_CODE, code_hook, None, a, a + 4)
    m.uc.hook_add(UC_HOOK_MEM_WRITE, mem_hook, None, WATCH_LO, WATCH_HI)
    m.uc.hook_add(UC_HOOK_MEM_WRITE, mem_hook, None, GPIOA, GPIOA + 0x20)

    out("空闲运行 %d 条指令 ..." % args.insn)
    reason = m.run(max_insns=args.insn)
    out("stop=%s insn=%d" % (reason, m.insn_count))
    out("扫描次数=%d KEY_ON=%d shutdown=%d" % (
        st["scans"], m.uc.mem_read(x3c.KEY_ON_FLAG_RAM, 1)[0], st["shutdown"]))
    out("keybtn4 计数=%d flags=%d/%d" % (
        int.from_bytes(m.uc.mem_read(x3c.BTN_RAM_WHEEL_CNT16, 2), "little"),
        m.uc.mem_read(x3c.BTN_RAM_WHEEL_FIRED1, 1)[0],
        m.uc.mem_read(x3c.BTN_RAM_WHEEL_FIRED2, 1)[0]))
    out("KEY_ON 区写入=%d:" % len(st["writes"]))
    for rec in st["writes"][:60]:
        out("   WRITE pc=0x%08X addr=0x%08X val=0x%02X insn=%d" % rec)
    out("读键统计=%s" % st["reads"])
    out("GPIOA 写=%d 次（前 40 条）:" % len(st["gpioa"]))
    for rec in st["gpioa"][:40]:
        out("   GPIOA pc=0x%08X addr=0x%08X val=0x%08X insn=%d" % rec)
    out("RF 逻辑帧=%d 条（前 120）:" % len(st["frames"]))
    for rec in st["frames"][:120]:
        if rec[0] == "S":
            out("   SHORT cmd=0x%02X insn=%d scan=%d" % (rec[1], rec[4], rec[5]))
        else:
            out("   PROP group=0x%02X key=0x%02X val=0x%02X insn=%d scan=%d"
                % (rec[1], rec[2], rec[3], rec[4], rec[5]))
    if st["delays"]:
        ds = [d for _, d in st["delays"]]
        med = int(statistics.median(ds))
        out("delay(1000)=1ms 校准：样本=%d 指令数 min/med/max=%d/%d/%d"
            % (len(ds), min(ds), med, max(ds)))
        if st["scans"]:
            ins_per_scan = m.insn_count / st["scans"]
            out("insn/scan≈%.1f → 扫描周期≈%.1f ms"
                % (ins_per_scan, ins_per_scan / med))
    log = os.path.join(HERE, "bug_audit.log")
    with open(log, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("[log]", os.path.relpath(log, ROOT))
    return 0


if __name__ == "__main__":
    sys.exit(main())
