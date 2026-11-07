# -*- coding: utf-8 -*-
"""用 machine.access.hot()（含 PC 样本）找主循环里轮询 GPIO / 写 GPIO 的代码。"""
import struct
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import probe_ui as P  # noqa: E402

m = P.Machine(image=P.IMAGE, log_fn=lambda s: None, trace=True)
P.patch_option_bytes(m)
P.patch_adc(m, 2600)
P.patch_delays(m)
m.boot()
m.set_fine(False)
tick = 0
done = 0
while done < 26_000_000:
    m.run(200_000)
    done = m.insn_count
    tick += 100                    # 快速推进 UI 定时器
    m.uc.mem_write(P.RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
    m.uc.mem_write(P.RAM_TICK_FLAG, b"\x01")
print("insn=%d pc=0x%08x" % (m.insn_count, m.reg("PC")))
print("=== GPIO accesses (hot, with pc samples) ===")
rows = m.access.hot(400)
for cnt, d, addr, size, pcs in rows:
    p = m.find_peripheral(addr)
    name = p.name if p else "?"
    if not name.startswith("GPIO"):
        continue
    print("  %s %-8s 0x%08x size=%d x%-5d pcs=%s"
          % (d, name, addr, size, cnt, [hex(x) for x in pcs]))
