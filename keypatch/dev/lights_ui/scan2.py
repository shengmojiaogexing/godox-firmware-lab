# -*- coding: utf-8 -*-
"""全镜像线性反汇编扫描：找 BL 目标、字面量引用、代码/数据边界。

python scan2.py bl                # 统计 BL 目标（过滤已知函数起点）
python scan2.py callers ADDR...   # 全镜像扫描 BL -> ADDR 的调用点
python scan2.py refs VAL...       # 全镜像扫描 32 位字面量 VAL 的引用点（LDR literal）
python scan2.py coverage          # functions.json 覆盖面/空洞
"""
from __future__ import annotations

import json
import struct
import sys
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_OP_IMM, CS_OP_MEM, CS_OP_REG

ROOT = __import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.abspath(__file__)))))
FW = ROOT + r"\devices\ad100pro\firmware\AD100pro_V1.2.bin"
FUNCS = ROOT + r"\devices\ad100pro\out\functions.json"
BASE = 0x08003000
IMAGE = open(FW, "rb").read()
FUNC_LIST = json.load(open(FUNCS, encoding="utf-8"))
STARTS = sorted({int(fn["addr"], 16) for fn in FUNC_LIST})
STARTSET = set(STARTS)


def func_containing(addr):
    import bisect
    i = bisect.bisect_right(STARTS, addr) - 1
    if i < 0:
        return None
    return STARTS[i]


def linear_scan():
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    md.detail = True
    calls = []      # (site, target)
    lits = []       # (site, val)
    total = 0
    for insn in md.disasm(IMAGE, BASE):
        total += 1
        if insn.mnemonic in ("bl", "blx") and insn.operands and insn.operands[0].type == CS_OP_IMM:
            calls.append((insn.address, insn.operands[0].imm & ~1))
        if insn.mnemonic in ("ldr", "ldrh", "ldrb") and len(insn.operands) >= 2:
            op = insn.operands[1]
            if op.type == CS_OP_MEM and insn.reg_name(op.mem.base) == "pc":
                pc = (insn.address + 4) & ~3
                la = pc + op.mem.disp
                o = la - BASE
                if 0 <= o <= len(IMAGE) - 4:
                    val = struct.unpack("<I", IMAGE[o:o + 4])[0]
                    lits.append((insn.address, la, val))
    return calls, lits, total


CALLS = LITS = TOTAL = None


def scan():
    global CALLS, LITS, TOTAL
    if CALLS is None:
        CALLS, LITS, TOTAL = linear_scan()
    return CALLS, LITS, TOTAL


def main():
    cmd = sys.argv[1] if len(sys.argv) > 1 else "help"
    args = sys.argv[2:]
    calls, lits, total = scan()
    if cmd == "bl":
        targets = defaultdict(list)
        unknown = []
        for site, tgt in calls:
            targets[tgt].append(site)
            if tgt not in STARTSET:
                unknown.append((tgt, site))
        print("linear insns=%d, BL=%d, distinct targets=%d, targets not in functions.json=%d"
              % (total, len(calls), len(targets), len(set(t for t, _ in unknown))))
        print("top targets:")
        for t, sites in sorted(targets.items(), key=lambda kv: -len(kv[1]))[:60]:
            mark = "" if t in STARTSET else "  <UNKNOWN>"
            print("  0x%08X  x%-4d  %s%s" % (t, len(sites), "known-func=0x%08X" % func_containing(t), mark))
        print("unknown targets sample:")
        for t, s in unknown[:40]:
            print("   0x%08X <- 0x%08X" % (t, s))
    elif cmd == "callers":
        targets = [int(a, 16) & ~1 for a in args]
        for t in targets:
            sites = [s for s, tt in calls if tt == t]
            print("BL -> 0x%08X: %s" % (t, [hex(s) for s in sites]))
    elif cmd == "refs":
        vals = [int(a, 16) for a in args]
        for v in vals:
            hits = [(s, la) for s, la, val in lits if val == v]
            print("literal 0x%08X: %s" % (v, [(hex(s), hex(la)) for s, la in hits]))
    elif cmd == "coverage":
        ranges = []
        for fn in FUNC_LIST:
            s = int(fn["addr"], 16)
            e = int(fn["end"], 16)
            if e > s:
                ranges.append((s, e))
        ranges.sort()
        merged = []
        for s, e in ranges:
            if merged and s <= merged[-1][1]:
                merged[-1] = (merged[-1][0], max(merged[-1][1], e))
            else:
                merged.append((s, e))
        covered = sum(e - s for s, e in merged)
        print("functions.json: %d entries, merged %d ranges, covered 0x%X / 0x%X bytes"
              % (len(FUNC_LIST), len(merged), covered, len(IMAGE)))
        prev = BASE
        for s, e in merged:
            if s > prev:
                print("  GAP 0x%08X..0x%08X (%d bytes)" % (prev, s, s - prev))
            prev = max(prev, e)
        if prev < BASE + len(IMAGE):
            print("  GAP 0x%08X..0x%08X (%d bytes)" % (prev, BASE + len(IMAGE), BASE + len(IMAGE) - prev))
    else:
        print(__doc__)


if __name__ == "__main__":
    main()
