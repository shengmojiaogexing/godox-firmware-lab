# -*- coding: utf-8 -*-
"""探针 7：直接"手工搭 GUI 环境 + 调页面渲染函数"出截图。

背景（见 FEASIBILITY.md §6）：
  * devices/x3c/emu 的整机模拟器跑不到"显示页面"状态：主循环要等 TIM7(IRQ55) 1ms 节拍
    （未建模），且上电后屏幕状态机停在开机 logo，`*0x200003E7`(UI 模式) 永远是 0。
  * 直接调页面渲染函数 FUN_0800FE78 / FUN_080345B4 会在入口处 `FUN_0802b798
    (*(u32*)0x20000470)` 上解引用 NULL（0x20000470 = "当前页面窗口" 全局）。

本探针的做法：先手工调 `0x08010ACC(0,9,0,0,0x170,0x1c0)` 建一个根窗口，把返回值
写进 0x20000470（模拟"当前页面已存在"），再调目标页面渲染函数。这样渲染路径就能
跑起来；LCD 8080 总线解码成 PNG。

用法：
  python probe_render.py --out shots/home.png --call 0x0800FE78
"""
from __future__ import annotations

import argparse
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import probe_ui as P  # noqa: E402

from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                               UC_ARM_REG_R3, UC_ARM_REG_LR, UC_ARM_REG_SP,
                               UC_ARM_REG_PC, UC_ARM_REG_CPSR)

SENTINEL = 0x20047000
STACK_TOP = 0x200477C0


def boot_machine(img=P.IMAGE, insn=10_000_000, log=None):
    m = P.Machine(image=img, log_fn=log or (lambda s: None), trace=False)
    P.patch_option_bytes(m)
    P.patch_adc(m, 2600)
    P.patch_delays(m)
    from unicorn import UC_HOOK_MEM_WRITE
    bus = P.LcdBus()

    def wr_lcd(uc, access, address, size, value, user):
        if address == P.LCD:
            bus.write_a0(value & 0xFF)
        elif address == P.LCD + 1:
            bus.write_a1(value & 0xFF)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr_lcd, begin=P.LCD, end=P.LCD + 2)
    m.boot()
    m.set_fine(False)
    tick = 0
    done = 0
    while done < insn:
        m.run(min(200_000, insn - done))
        done = m.insn_count
        tick += 1
        m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
        m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
    return m, bus


def call(m, addr, args=(), budget=4_000_000, chunk=100_000, want_r0=False, trace=False):
    """调 addr（Thumb），返回 (ok, r0, pc)。ok=正常返回哨兵。"""
    uc = m.uc
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
    for i, v in enumerate(list(args)[:4]):
        uc.reg_write([UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3][i], v)
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    uc.reg_write(UC_ARM_REG_PC, addr | 1)
    m.set_fine(False)
    done = 0
    ok = False
    while done < budget:
        m.run(min(chunk, budget - done))
        done = m.insn_count
        if m.reg("PC") == SENTINEL:
            ok = True
            break
        uc.mem_write(P.RAM_TICK, struct.pack("<I", (done // 1000) & 0xFFFFFFFF))
    r0 = uc.reg_read(UC_ARM_REG_R0)
    return ok, r0, m.reg("PC")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=P.IMAGE)
    ap.add_argument("--boot-insn", type=int, default=10_000_000)
    ap.add_argument("--call", default="0x0800FE78")
    ap.add_argument("--args", default="")
    ap.add_argument("--out", default="")
    ap.add_argument("--verbose", action="store_true")
    args = ap.parse_args()

    m, bus = boot_machine(args.img, args.boot_insn, log=print if args.verbose else None)
    print("booted: PC=0x%08x px=%d" % (m.reg("PC"), bus.n_pixels))

    # 1) 建一个根窗口充当"当前页面"，写进 0x20000470
    ok, root, pc = call(m, 0x08010ACC, (0, 9, 0, 0, 0x170, 0x1C0), budget=200_000)
    print("create root: ok=%s handle=0x%08x pc=0x%08x" % (ok, root, pc))
    if not ok:
        print("!! root creation failed")
        return 1
    m.uc.mem_write(0x20000470, struct.pack("<I", root))

    # 2) 调目标页面渲染函数
    callargs = tuple(int(x, 0) for x in args.args.split(",") if x)
    px0 = bus.n_pixels
    ok, r0, pc = call(m, int(args.call, 0), callargs, budget=6_000_000)
    print("call %s: ok=%s r0=0x%08x pc=0x%08x px+%d cmds=%d" % (
        args.call, ok, r0, pc, bus.n_pixels - px0, bus.n_cmds))
    out = args.out or os.path.join(P.OUT, "render_%s.png" % args.call.replace("0x", ""))
    print("screenshot:", bus.snapshot("render_" + args.call.replace("0x", "")))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
