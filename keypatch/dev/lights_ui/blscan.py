# -*- coding: utf-8 -*-
"""在整镜像中暴力搜索 BL/BLX 目标（不依赖函数边界）。

python blscan.py 0x0800A30C [0x0800B5C0 ...]
"""
from __future__ import annotations

import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_OP_IMM

BASE = 0x08003000
IMAGE = open(r"devices\ad100pro\firmware\AD100pro_V1.2.bin", "rb").read()


def main():
    targets = [int(a, 16) & ~1 for a in sys.argv[1:]]
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    md.detail = True
    hits = {t: [] for t in targets}
    # 只尝试 2 字节对齐的起始地址；解码 4 字节判断是否 BL
    for off in range(0, len(IMAGE) - 4, 2):
        addr = BASE + off
        for insn in md.disasm(IMAGE[off:off + 4], addr):
            if insn.mnemonic in ("bl", "blx") and insn.operands and insn.operands[0].type == CS_OP_IMM:
                t = insn.operands[0].imm & ~1
                if t in hits:
                    hits[t].append(addr)
            break
    for t in targets:
        print("BL -> 0x%08X: %s" % (t, [hex(a) for a in hits[t]]))


if __name__ == "__main__":
    main()
