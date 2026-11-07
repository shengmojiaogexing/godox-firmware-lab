# -*- coding: utf-8 -*-
"""Debug the first IRQ55 dispatch: verify vtor / vector value / handler path."""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "emu"))
sys.path.insert(0, HERE)

from machine import Machine
import tmr7_model

m = Machine(log_fn=print, trace=True)
tmr7_model.patch_real_device(m)
model = tmr7_model.attach(m, scale_num=1, scale_den=1, verbose=True)
sp, pc = m.boot()
print("boot SP=%#x PC=%#x vtor=%#x" % (sp, pc, m.vtor))

orig_take = m.take_exception


def take(exc):
    vec = m.peek_u32(m.vtor + 4 * exc)
    print("[dbg] take_exception(%d) vtor=%#x vec=%#x pc=%#x"
          % (exc, m.vtor, vec, m.uc.reg_read(11) & ~1))
    ok = orig_take(exc)
    print("[dbg]  -> ok=%s newPC=%#x active=%r"
          % (ok, m.uc.reg_read(11) & ~1, m.active_exception))
    return ok


m.take_exception = take

for i in range(6):
    m.run(100_000)
    print("[dbg] insn=%d PC=%#x vtor=%#x active=%r pending=%r tick=%r"
          % (m.insn_count, m.uc.reg_read(11) & ~1, m.vtor,
             m.active_exception, sorted(m.pending_irqs),
             bytes(m.uc.mem_read(tmr7_model.TICK_COUNTER, 4)).hex()))
