# -*- coding: utf-8 -*-
"""探针 3：不依赖整机主循环，直接调用固件的"页面渲染函数"并截屏。

思路（Lead 建议的方案）：
  1. 用 devices/x3c/emu 的 Machine 启动固件，跑到 LCD/GUI 初始化完成（例如 2~6M 指令）；
     只做**启动**，不需要跑到主循环 —— 因为 X3-C 主循环要等 TIM7(IRQ55) 的 1ms
     节拍，而模拟器没建模该中断（README/FINDINGS 有写）。
  2. 手工构造一次函数调用：PC = 目标函数|1（Thumb），LR = RAM 里的 `b .` 哨兵，
     SP 用固件当前栈顶（留出余量）。
  3. 跑一段预算，用 LCD 8080 总线钩子（0x6C000000）重建 368x448 RGB565 屏幕并写 PNG。
  4. 若 PC 停在哨兵 → 调用正常返回；若 HardFault / 卡死 → 记录证据。

可调用目标（同一套 VM/WM 库，换页面就是换函数）：
  0x0800FE78  显示 Home 首页（含主菜单/底部页签）     —— 无参
  0x080345B4  page 0（无参，实际带 r0=0；见反汇编）
  0x08006F0C  FUN_08006f0c(win, page)  页面内容构建（page 2..13）

用法：
  python probe_direct.py --at 3000000 --call 0x0800FE78
  python probe_direct.py --at 3000000 --call 0x0800FE78,0x080345B4
"""
from __future__ import annotations

import argparse
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import probe_ui as P  # noqa: E402

SENTINEL = 0x20047000          # RAM `b .` 哨兵（返回落点）
STACK_TOP = 0x200477C0


def boot_to(insn, img=P.IMAGE, tick_every=200_000):
    m = P.Machine(image=img, log_fn=lambda s: None, trace=True)
    P.patch_option_bytes(m)
    P.patch_adc(m, 2600)
    P.patch_delays(m)

    from unicorn import UC_HOOK_MEM_WRITE
    bus = P.LcdBus()

    def wr(uc, access, address, size, value, user):
        if address == P.LCD:
            bus.write_a0(value & 0xFF)
        elif address == P.LCD + 1:
            bus.write_a1(value & 0xFF)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=P.LCD, end=P.LCD + 2)
    m.boot()
    m.set_fine(False)
    tick = 0
    done = 0
    while done < insn:
        m.run(min(tick_every, insn - done))
        done = m.insn_count
        tick += 1
        m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
        m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
    return m, bus, done


def call_fn(m, bus, addr, args=(), budget=8_000_000, chunk=200_000, tag=None):
    """在已启动的 machine 上调用 addr（Thumb），返回 (ret_ok, pixels_added, pc)。"""
    uc = m.uc
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))     # b .
    px0 = bus.n_pixels
    cmds0 = bus.n_cmds
    sp = uc.reg_read(14)  # dummy
    # 寄存器：r0..r3 传参
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_LR, UC_ARM_REG_SP,
                                   UC_ARM_REG_PC, UC_ARM_REG_CPSR)
    for i, v in enumerate(args[:4]):
        uc.reg_write([UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3][i], v)
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    uc.reg_write(UC_ARM_REG_PC, addr | 1)
    m.set_fine(False)
    done = 0
    sentinel = False
    while done < budget:
        m.run(min(chunk, budget - done))
        done = m.insn_count
        if m.reg("PC") == SENTINEL:
            sentinel = True
            break
        # 让页面动画/计时器有进展
        uc.mem_write(P.RAM_TICK, struct.pack("<I", (done // 1000) & 0xFFFFFFFF))
        uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
    ok = sentinel and not m.fault
    print("  call 0x%08x(%s): sentinel=%s fault=%s pc=0x%08x px+%d cmds+%d" % (
        addr, ",".join(hex(a) for a in args), sentinel, m.fault,
        m.reg("PC"), bus.n_pixels - px0, bus.n_cmds - cmds0))
    if tag:
        print("   snapshot:", bus.snapshot(tag))
    return ok, bus.n_pixels - px0, m.reg("PC")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=P.IMAGE)
    ap.add_argument("--at", type=int, default=3_000_000, help="启动到多少指令后调用")
    ap.add_argument("--call", default="0x0800FE78")
    ap.add_argument("--budget", type=int, default=8_000_000)
    args = ap.parse_args()

    m, bus, done = boot_to(args.at, args.img)
    print("booted %d insn, PC=0x%08x, lcd px=%d cmds=%d" % (done, m.reg("PC"), bus.n_pixels, bus.n_cmds))
    print("boot snapshot:", bus.snapshot("boot_%08d" % args.at))
    for spec in args.call.split(","):
        spec = spec.strip()
        if not spec:
            continue
        if ":" in spec:
            a, rest = spec.split(":", 1)
            callargs = tuple(int(x, 0) for x in rest.split("+") if x)
        else:
            a, callargs = spec, ()
        call_fn(m, bus, int(a, 0), callargs, budget=args.budget,
                tag="call_%s" % a.replace("0x", ""))


if __name__ == "__main__":
    main()
