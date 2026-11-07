# -*- coding: utf-8 -*-
"""探针 6：用"主循环等待标志 0x20000004"判定固件是否进入 main 主循环。

main() 主循环：
    do { ... FUN_0803ba4c(); FUN_080335bc(); } while (*(u8*)0x20000004 == 0);
                                 ^ 等待 TIM7 节拍中断置位，然后由主循环清零
模拟器没有 TIM7 中断，所以我们每跑一个 chunk 就手工把标志置 1；
若下一个 chunk 结束后标志变回 0（且 PC 落在主循环区间），说明主循环真的在跑。

顺带统计：
  * LCD 像素增长（是否有新画面）
  * RAM 0x200003E7（UI 模式）/ 0x200004C4（请求页面）
用法：python probe_main.py --max 60000000
"""
from __future__ import annotations

import argparse
import os
import struct
import sys
import time

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import probe_ui as P  # noqa: E402

# main() 主循环体区间（FUN_08033e20 中 do{...}while 部分）
LOOP_LO, LOOP_HI = 0x08033EA0, 0x08034340


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=P.IMAGE)
    ap.add_argument("--max", type=int, default=60_000_000)
    ap.add_argument("--chunk", type=int, default=200_000)
    ap.add_argument("--tick-per-chunk", type=int, default=1)
    args = ap.parse_args()

    m = P.Machine(image=args.img, log_fn=lambda s: None, trace=True)
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

    uc = m.uc
    tick = 0
    done = 0
    t0 = time.time()
    loop_hits = 0
    last_pix = 0
    while done < args.max:
        m.run(min(args.chunk, args.max - done))
        done = m.insn_count
        # 注入 1ms tick + 主循环标志
        tick += args.tick_per_chunk
        uc.mem_write(P.RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
        uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
        m.run(min(2000, max(0, args.max - done)))
        done = m.insn_count
        flag = uc.mem_read(P.RAM_TICK_FLAG, 1)[0]
        pc = m.reg("PC")
        if flag == 0 and LOOP_LO <= pc < LOOP_HI:
            loop_hits += 1
        if loop_hits == 1 and done > 100_000:
            print("MAIN LOOP confirmed @insn=%d PC=0x%08x (%.1fs)" % (done, pc, time.time() - t0))
        if done % 5_000_000 < args.chunk + 2000:
            mode = uc.mem_read(P.RAM_UI_MODE, 1)[0]
            req = uc.mem_read(P.RAM_UI_REQ, 1)[0]
            print("  %10d insn %.1fs PC=0x%08x px=%d cmds=%d ui_mode=%02x req=%02x loop_hits=%d" % (
                done, time.time() - t0, pc, bus.n_pixels, bus.n_cmds, mode, req, loop_hits))
        if bus.n_pixels != last_pix:
            print("  * LCD px %d -> %d @%d PC=0x%08x" % (last_pix, bus.n_pixels, done, pc))
            last_pix = bus.n_pixels
    print("done: %d insn, %.1fs, loop_hits=%d, px=%d cmds=%d" % (
        done, time.time() - t0, loop_hits, bus.n_pixels, bus.n_cmds))
    print("snapshot:", bus.snapshot("main_probe"))


if __name__ == "__main__":
    main()
