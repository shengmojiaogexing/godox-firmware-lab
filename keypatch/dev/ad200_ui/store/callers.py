#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""枚举调用指定函数的 BL/BLX 点（capstone 线性扫描），并输出调用者函数。

用法：
  python callers.py 0x0800b820 0x08005974 0x0800ac58 0x0800cc64 0x0800cc40
  python callers.py --all-save          # 预置的保存/加载链路函数
"""
from __future__ import annotations

import json
import os
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_OP_IMM

BASE = 0x08003000
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
BIN = os.path.join(ROOT, "devices/ad200pro", "samples", "AD200pro_V1.60.bin")
IMAGE = open(BIN, "rb").read()
SIZE = len(IMAGE)

SYMBOLS = {}
try:
    obj = json.load(open(os.path.join(ROOT, "devices/ad200pro", "analysis", "symbols.json"),
                         encoding="utf-8"))
    SYMBOLS = {int(k, 16): v for k, v in obj.get("symbols", {}).items()}
except Exception:
    pass

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_MCLASS)
md.detail = True
md.skipdata = True

STARTS = sorted(SYMBOLS)


def func_of(addr):
    lo, hi = 0, len(STARTS) - 1
    best = BASE
    while lo <= hi:
        mid = (lo + hi) // 2
        if STARTS[mid] <= addr:
            best = STARTS[mid]
            lo = mid + 1
        else:
            hi = mid - 1
    return best


def scan_calls(targets):
    out = {}
    for ins in md.disasm(IMAGE, BASE):
        if ins.id == 0:
            continue
        if ins.mnemonic in ("bl", "blx") and ins.operands and ins.operands[0].type == CS_OP_IMM:
            t = ins.operands[0].imm & ~1
            if t in targets:
                out.setdefault(t, []).append(ins.address)
    return out


def main(argv):
    targets = set()
    if "--all-save" in argv:
        for a in (0x0800b820, 0x08005974, 0x0800ac58, 0x0800cc64, 0x0800cc40,
                  0x0800b7f4, 0x0800c740, 0x080048e4, 0x0800ccc8, 0x080043e4,
                  0x0800587c):
            targets.add(a)
    for a in argv[1:]:
        if a.startswith("--"):
            continue
        targets.add(int(a, 0) & ~1)
    res = scan_calls(targets)
    print("扫描 BL 指向: %s" % ", ".join("0x%08x" % t for t in sorted(targets)))
    for t in sorted(targets):
        print("\n== 0x%08x %s  callers=%d ==" % (t, SYMBOLS.get(t, "?"),
                                                 len(res.get(t, []))))
        for a in res.get(t, []):
            f = func_of(a)
            print("   %08x  in %s @%08x" % (a, SYMBOLS.get(f, "?"), f))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
