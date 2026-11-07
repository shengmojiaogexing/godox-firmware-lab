# -*- coding: utf-8 -*-
"""按地址区间用 capstone 反汇编（用于 C.Fn 路径细节复核）。

用法：
    python dump_region.py 0x08004500 0x120
    python dump_region.py 0x080045AC 0x60
"""
from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(HERE))))
BASE = 0x08003000
BIN = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")

RAM = {
    0x20000150: "UI(base)",
    0x20000162: "UI+0x12 CFN_OPEN",
    0x20000163: "UI+0x13 CFN_STATE",
    0x20000164: "UI+0x14 CFN_ITEM",
    0x20000165: "UI+0x15",
    0x20000166: "UI+0x16",
    0x2000011C: "tmp_11C",
    0x20000418: "settings[0]",
    0x2000041E: "settings[6]=ID",
    0x2000041F: "settings[7]=F8",
    0x0800E314: "value_limit_table?",
    0x0801F000: "flash_param_page",
}


def main(argv):
    if len(argv) < 3:
        print(__doc__)
        return 2
    from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
    data = open(BIN, "rb").read()
    start = int(argv[1], 0)
    length = int(argv[2], 0)
    end = start + length
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True
    off = start - BASE
    print(";; region 0x%08X..0x%08X (%d bytes)  file+0x%04X" % (start, end, length, off))
    for ins in md.disasm(data[off:end - BASE], start):
        line = "0x%08X: %-10s %-34s" % (ins.address, ins.mnemonic, ins.op_str)
        if ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str:
            try:
                imm = ins.operands[1].mem.disp
                lit = ((ins.address + 4) & ~3) + imm
                val = int.from_bytes(data[lit - BASE:lit - BASE + 4], "little")
                line += "  @0x%08X=0x%08X" % (lit, val)
                if val in RAM:
                    line += " (%s)" % RAM[val]
            except Exception:
                pass
        print(line)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
