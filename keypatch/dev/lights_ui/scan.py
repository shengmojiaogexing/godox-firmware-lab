# -*- coding: utf-8 -*-
"""AD100 Pro 全镜像线性扫描 -> 调用图 / 字面量引用 / 函数归属。

  python scan.py callgraph 0x0800A30C          # 谁调用 lcd_flush
  python scan.py callsite 0x0800A30C           # 同上但显示调用点地址
  python scan.py dumpfunc 0x0800993C 0x08009B00
  python scan.py refs 0x0800DB0C               # 谁引用该地址（字面量）
  python scan.py find 0x0801F000               # 扫描全镜像里出现的 32 位常量
"""
from __future__ import annotations

import json
import struct
import sys
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_OP_IMM, CS_OP_MEM

ROOT = __import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.dirname(__import__("os").path.abspath(__file__)))))
FW = ROOT + r"\devices\ad100pro\firmware\AD100pro_V1.2.bin"
FUNCS = ROOT + r"\devices\ad100pro\out\functions.json"
BASE = 0x08003000
with open(FW, "rb") as f:
    IMAGE = f.read()
FUNC_LIST = json.load(open(FUNCS, encoding="utf-8"))
STARTS = sorted({int(fn["addr"], 16) for fn in FUNC_LIST})
FUNC_BY_START = {int(fn["addr"], 16): fn for fn in FUNC_LIST}


def func_containing(addr):
    lo, hi = 0, len(STARTS) - 1
    best = None
    while lo <= hi:
        mid = (lo + hi) // 2
        if STARTS[mid] <= addr:
            best = STARTS[mid]
            lo = mid + 1
        else:
            hi = mid - 1
    return best


def scan_code():
    """逐个已知函数体解码（用 functions.json 的 end），收集 BL 目标与字面量。
    返回 dict: caller -> {'callees': set, 'lits': [(insn_addr, lit_addr, value)]}
    """
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    md.detail = True
    out = {}
    for fn in FUNC_LIST:
        start = int(fn["addr"], 16)
        end = int(fn["end"], 16)
        if end <= start:
            end = start + 2
        data = IMAGE[start - BASE:end - BASE]
        info = {"callees": set(), "lits": [], "insns": []}
        for insn in md.disasm(data, start):
            info["insns"].append((insn.address, insn.mnemonic, insn.op_str))
            if insn.mnemonic in ("bl", "blx") and insn.operands and insn.operands[0].type == CS_OP_IMM:
                tgt = insn.operands[0].imm & ~1
                info["callees"].add(tgt)
            if insn.mnemonic.startswith("ldr") and len(insn.operands) >= 2:
                op = insn.operands[1]
                if op.type == CS_OP_MEM and insn.reg_name(op.mem.base) == "pc":
                    pc = (insn.address + 4) & ~3
                    lit_addr = pc + op.mem.disp
                    o = lit_addr - BASE
                    if 0 <= o <= len(IMAGE) - 4:
                        val = struct.unpack("<I", IMAGE[o:o + 4])[0]
                        info["lits"].append((insn.address, lit_addr, val))
        out[start] = info
    return out


SCAN = None


def get_scan():
    global SCAN
    if SCAN is None:
        SCAN = scan_code()
    return SCAN


def main():
    cmd = sys.argv[1] if len(sys.argv) > 1 else "help"
    args = sys.argv[2:]
    scan = get_scan()
    if cmd in ("callgraph", "callsite"):
        targets = [int(a, 16) & ~1 for a in args]
        for t in targets:
            rows = []
            for caller, info in scan.items():
                if t in info["callees"]:
                    sites = [hex(a) for a, m, o in info["insns"] if m in ("bl", "blx")][:0]
                    rows.append(caller)
            print("callers of 0x%08X (%d): %s" % (t, len(rows), [hex(r) for r in sorted(rows)]))
    elif cmd == "dumpfunc":
        start = int(args[0], 16)
        stop = int(args[1], 16) if len(args) > 1 else start + 0x200
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
        for insn in md.disasm(IMAGE[start - BASE:stop - BASE], start):
            print("0x%08X: %-20s %s" % (insn.address, insn.mnemonic, insn.op_str))
    elif cmd == "refs":
        targets = [int(a, 16) for a in args]
        for t in targets:
            hits = []
            for caller, info in scan.items():
                for ia, la, val in info["lits"]:
                    if val == t:
                        hits.append((caller, ia, la))
            print("literal 0x%08X referenced by:" % t)
            for caller, ia, la in hits:
                print("   insn 0x%08X (func 0x%08X) pool@0x%08X" % (ia, caller, la))
    elif cmd == "find":
        vals = [int(a, 16) for a in args]
        for v in vals:
            pos = []
            pat = struct.pack("<I", v)
            i = IMAGE.find(pat)
            while i >= 0:
                pos.append(BASE + i)
                i = IMAGE.find(pat, i + 1)
            print("value 0x%08X at: %s" % (v, [hex(p) for p in pos[:40]]))
    else:
        print(__doc__)


if __name__ == "__main__":
    main()
