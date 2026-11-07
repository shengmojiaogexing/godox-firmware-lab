# -*- coding: utf-8 -*-
"""AD100 Pro 死代码/空闲区分析：找出所有"无引用"的函数，作为代码洞候选。

引用来源：
  - 向量表 0x08003000..0x080030C0（48 项，含 Thumb 位）
  - init_array 0x0800F3EC..0x0800F40C（每项 16 字节，末字为函数指针）
  - 全镜像 32 位字面量（含指针表）
  - BL/BLX 调用（逐函数解码 + 暴力扫全部 2 字节对齐位置）
  - 0x0800F448 之后不存在（镜像内）
输出：无引用函数清单 + 连续区间，按大小排序。
"""
from __future__ import annotations

import json
import struct
import sys
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_OP_IMM

ROOT = __import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.abspath(__file__)))))
FW = ROOT + r"\devices\ad100pro\firmware\AD100pro_V1.2.bin"
FUNCS = ROOT + r"\devices\ad100pro\out\functions.json"
BASE = 0x08003000
IMAGE = open(FW, "rb").read()
FUNC_LIST = json.load(open(FUNCS, encoding="utf-8"))
STARTS = sorted({int(f["addr"], 16) for f in FUNC_LIST})
FUNC_BY_START = {int(f["addr"], 16): f for f in FUNC_LIST}


def u32(addr):
    o = addr - BASE
    if 0 <= o <= len(IMAGE) - 4:
        return struct.unpack("<I", IMAGE[o:o + 4])[0]
    return None


def main():
    refs = defaultdict(set)  # target -> set(source kind/addr)

    # 1) 向量表（48 项）
    for i in range(48):
        v = u32(BASE + i * 4)
        if v and 0x08003000 <= v < 0x0800F448:
            refs[v & ~1].add("vector[%d]" % i)
    # 2) init_array
    for i in range(2):
        entry = 0x0800F3EC + i * 16
        v = u32(entry + 0x0C)
        if v:
            refs[v & ~1].add("init_array[%d]" % i)
    # 3) 全镜像 32 位字面量扫描（只统计指向镜像范围的值）
    for o in range(0, len(IMAGE) - 3, 4):
        v = struct.unpack("<I", IMAGE[o:o + 4])[0]
        if 0x08003000 <= v < 0x0800F448:
            refs[v & ~1].add("lit@0x%08X" % (BASE + o))
    # 4) BL 调用（逐函数解码）
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    md.detail = True
    call_edges = []
    for f in FUNC_LIST:
        s = int(f["addr"], 16)
        e = int(f["end"], 16)
        if e <= s:
            e = s + 2
        for insn in md.disasm(IMAGE[s - BASE:e - BASE], s):
            if insn.mnemonic in ("bl", "blx") and insn.operands and insn.operands[0].type == CS_OP_IMM:
                t = insn.operands[0].imm & ~1
                refs[t].add("bl@0x%08X" % insn.address)
                call_edges.append((t, insn.address))
    # 5) 暴力 BL 扫描（覆盖未解码片段）
    for off in range(0, len(IMAGE) - 4, 2):
        addr = BASE + off
        for insn in md.disasm(IMAGE[off:off + 4], addr):
            if insn.mnemonic in ("bl", "blx") and insn.operands and insn.operands[0].type == CS_OP_IMM:
                t = insn.operands[0].imm & ~1
                if 0x08003000 <= t < 0x0800F448:
                    refs[t].add("blbrute@0x%08X" % addr)
            break

    # 无引用函数
    dead = []
    for s in STARTS:
        if not refs.get(s):
            f = FUNC_BY_START[s]
            dead.append((int(f["size"]), s, int(f["end"], 16)))
    dead.sort(reverse=True)
    print("functions with no references: %d" % len(dead))
    for size, s, e in dead:
        if size >= 16:
            print("  0x%08X..0x%08X size=%-4d" % (s, e, size))
    print("\n-- small (<16) dead count:", sum(1 for size, s, e in dead if size < 16))

    # 相邻死函数合并区间
    merged = []
    for size, s, e in sorted(dead, key=lambda x: x[1]):
        if merged and s <= merged[-1][1]:
            merged[-1] = (merged[-1][0], max(merged[-1][1], e))
        else:
            merged.append((s, e))
    print("\nmerged dead ranges >=64B:")
    for s, e in merged:
        if e - s >= 64:
            print("  0x%08X..0x%08X  %d bytes" % (s, e, e - s))


if __name__ == "__main__":
    main()
