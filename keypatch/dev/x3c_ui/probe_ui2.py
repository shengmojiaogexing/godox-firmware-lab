# -*- coding: utf-8 -*-
"""探针 2：确认模拟器能否跑到 UI 主循环 / UI tick 回调，并统计关键函数命中。

结论用于 FEASIBILITY.md 的"模拟器验证可行性"一节。
用法：python probe_ui2.py --img ...
"""
from __future__ import annotations

import argparse
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import probe_ui as P  # noqa: E402

KEY_PCS = {
    0x08033E20: "main",
    0x08014B28: "ui_tick(100ms)",
    0x08006F0C: "page_builder(FUN_08006f0c)",
    0x080079E8: "main_menu_builder",
    0x080335BC: "timer_poll",
    0x0803BA4C: "wdt_refresh",
}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=P.IMAGE)
    ap.add_argument("--fast", type=int, default=6_000_000)
    ap.add_argument("--max", type=int, default=30_000_000)
    ap.add_argument("--chunk", type=int, default=100_000)
    args = ap.parse_args()

    m = P.Machine(image=args.img, log_fn=lambda s: None, trace=True)
    P.patch_option_bytes(m)
    P.patch_adc(m, 2600)
    P.patch_delays(m)

    from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_WRITE
    bus = P.LcdBus()

    def wr(uc, access, address, size, value, user):
        if address == P.LCD:
            bus.write_a0(value & 0xFF)
        elif address == P.LCD + 1:
            bus.write_a1(value & 0xFF)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=P.LCD, end=P.LCD + 2)
    hits = {k: 0 for k in KEY_PCS}

    def code_hook(uc, address, size, user):
        n = KEY_PCS.get(address)
        if n:
            hits[address] += 1

    m.boot()
    tick = 0
    # ---- 阶段 1：fast 冲到 main 附近
    m.set_fine(False)
    done = 0
    while done < args.fast:
        m.run(min(200_000, args.fast - done))
        done = m.insn_count
        tick += 1
        m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
        m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
    print("phase1 done: %d insn PC=0x%08x px=%d" % (done, m.reg("PC"), bus.n_pixels))

    # ---- 阶段 2：fine 模式 + 命中统计
    m.set_fine(True)
    m.uc.hook_add(UC_HOOK_CODE, code_hook)
    last_pix = bus.n_pixels
    last_report = done
    while done < args.max:
        m.run(min(args.chunk, args.max - done))
        done = m.insn_count
        tick += 1
        m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
        m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
        if done - last_report >= 2_000_000:
            last_report = done
            print("  %10d PC=0x%08x tick=%d px=%d hits=%s" % (
                done, m.reg("PC"), tick, bus.n_pixels,
                {KEY_PCS[k]: v for k, v in hits.items() if v}))
        if bus.n_pixels != last_pix:
            print("  * LCD px %d -> %d @%d PC=0x%08x" % (last_pix, bus.n_pixels, done, m.reg("PC")))
            last_pix = bus.n_pixels
    print("hits:", {KEY_PCS[k]: v for k, v in hits.items()})
    print("final px=%d cmds=%d PC=0x%08x" % (bus.n_pixels, bus.n_cmds, m.reg("PC")))
    print("snapshot:", bus.snapshot("probe2_final"))


if __name__ == "__main__":
    main()
