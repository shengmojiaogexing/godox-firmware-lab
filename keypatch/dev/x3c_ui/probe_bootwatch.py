# -*- coding: utf-8 -*-
"""探针 5：用 IWDG 喂狗写（0x40003000 <- 0xAAAA）作为"主循环已启动"的探针。

main() 主循环每轮都调 FUN_0803ba4c() 写 IWDG->KR=0xAAAA（*DAT_0803ba58）。
因此监视 0x40003000 的写就能廉价判断"固件已进入 main 主循环"，
不必用慢速 fine 模式逐条指令跑。
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


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=P.IMAGE)
    ap.add_argument("--max", type=int, default=60_000_000)
    ap.add_argument("--chunk", type=int, default=200_000)
    args = ap.parse_args()

    m = P.Machine(image=args.img, log_fn=lambda s: None, trace=True)
    P.patch_option_bytes(m)
    P.patch_adc(m, 2600)
    P.patch_delays(m)

    from unicorn import UC_HOOK_MEM_WRITE
    bus = P.LcdBus()
    st = {"wdt": 0, "wdt_pc": 0, "wdt_insn": 0}

    def wr(uc, access, address, size, value, user):
        if address == P.LCD:
            bus.write_a0(value & 0xFF)
        elif address == P.LCD + 1:
            bus.write_a1(value & 0xFF)
        elif 0x40003000 <= address < 0x40003004:
            st["wdt"] += 1
            if value == 0xAAAA:
                st.setdefault("kr_aaaa", 0)
                st["kr_aaaa"] += 1
                st["wdt_pc"] = uc.reg_read(15)
            else:
                st.setdefault("kr_other", []).append(value)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=0x40003000, end=0x40003004)

    def wr_lcd(uc, access, address, size, value, user):
        if address == P.LCD:
            bus.write_a0(value & 0xFF)
        elif address == P.LCD + 1:
            bus.write_a1(value & 0xFF)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr_lcd, begin=P.LCD, end=P.LCD + 2)

    m.boot()
    m.set_fine(False)
    t0 = time.time()
    tick = 0
    done = 0
    while done < args.max:
        m.run(min(args.chunk, args.max - done))
        done = m.insn_count
        tick += 1
        m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
        m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
        if st.get("kr_aaaa", 0) >= 5:
            print("MAIN LOOP reached: 0xAAAA writes=%d pc=0x%08x (insn=%d, %.1fs)"
                  % (st["kr_aaaa"], st["wdt_pc"], done, time.time() - t0))
            break
        if done % 5_000_000 < args.chunk:
            print("  %d insn, %.1fs (%.0f insn/s) PC=0x%08x px=%d kr=%s" % (
                done, time.time() - t0, done / max(1e-9, time.time() - t0),
                m.reg("PC"), bus.n_pixels,
                (st["wdt"], st.get("kr_aaaa", 0), st.get("kr_other", [])[:5])))
    if not st.get("kr_aaaa"):
        print("no IWDG 0xAAAA write up to %d insn (PC=0x%08x)" % (done, m.reg("PC")))
    print("wdt writes=%s snapshot=%s" % ({k: v for k, v in st.items() if k != "kr_other"},
                                        bus.snapshot("bootwatch")))
    return m, bus, st


if __name__ == "__main__":
    main()
