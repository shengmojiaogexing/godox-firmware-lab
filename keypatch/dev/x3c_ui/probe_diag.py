# -*- coding: utf-8 -*-
"""探针 4：直接调用的 HardFault 诊断（读 CFSR/HFSR + 栈帧 PC + 最近 PC 环）。"""
from __future__ import annotations

import argparse
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import probe_ui as P  # noqa: E402
import probe_direct as D  # noqa: E402

from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                               UC_ARM_REG_R3, UC_ARM_REG_LR, UC_ARM_REG_SP,
                               UC_ARM_REG_PC, UC_ARM_REG_CPSR)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=P.IMAGE)
    ap.add_argument("--at", type=int, default=6_400_000)
    ap.add_argument("--fn", type=lambda x: int(x, 0), default=0x0800FE78)
    ap.add_argument("--args", default="")
    ap.add_argument("--budget", type=int, default=200_000)
    args = ap.parse_args()

    m, bus, done = D.boot_to(args.at, args.img)
    uc = m.uc
    print("booted %d PC=0x%08x px=%d" % (done, m.reg("PC"), bus.n_pixels))

    ring = []
    from unicorn import UC_HOOK_CODE

    def hook(uc_, address, size, user):
        ring.append(address)
        if len(ring) > 200:
            del ring[:100]

    uc.mem_write(D.SENTINEL, bytes.fromhex("fee7"))
    uc.hook_add(UC_HOOK_CODE, hook)
    m.set_fine(True)
    callargs = [int(x, 0) for x in args.args.split(",") if x]
    for i, v in enumerate(callargs[:4]):
        uc.reg_write([UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3][i], v)
    uc.reg_write(UC_ARM_REG_SP, D.STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, D.SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    uc.reg_write(UC_ARM_REG_PC, args.fn | 1)

    m.run(args.budget)
    pc = m.reg("PC")
    print("after call: insn=%d PC=0x%08x SP=0x%08x LR=0x%08x fault=%s" % (
        m.insn_count, pc, uc.reg_read(UC_ARM_REG_SP), uc.reg_read(UC_ARM_REG_LR), m.fault))
    cfsr = struct.unpack("<I", bytes(uc.mem_read(0xE000ED28, 4)))[0]
    hfsr = struct.unpack("<I", bytes(uc.mem_read(0xE000ED2C, 4)))[0]
    mmfar = struct.unpack("<I", bytes(uc.mem_read(0xE000ED34, 4)))[0]
    bfar = struct.unpack("<I", bytes(uc.mem_read(0xE000ED38, 4)))[0]
    print("CFSR=0x%08x HFSR=0x%08x MMFAR=0x%08x BFAR=0x%08x" % (cfsr, hfsr, mmfar, bfar))
    sp = uc.reg_read(UC_ARM_REG_SP)
    try:
        frame = bytes(uc.mem_read(sp, 32))
        r0, r1, r2, r3, r12, lr_, pc_, xpsr = struct.unpack("<8I", frame)
        print("stacked frame: R0=0x%08x R1=0x%08x R2=0x%08x R3=0x%08x R12=0x%08x LR=0x%08x PC=0x%08x xPSR=0x%08x"
              % (r0, r1, r2, r3, r12, lr_, pc_, xpsr))
    except Exception as e:
        print("stack read fail", e)
    print("last PCs:", " ".join("%08x" % x for x in ring[-40:]))
    print("counts:", {hex(k): v for k, v in m.call_targets.items() if v > 0} if m.call_targets else None)
    print("px=%d cmds=%d" % (bus.n_pixels, bus.n_cmds))
    print("snapshot:", bus.snapshot("diag"))


if __name__ == "__main__":
    main()
