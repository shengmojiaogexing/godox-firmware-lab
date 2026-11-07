# -*- coding: utf-8 -*-
"""AD100 Pro 固件分析工具箱（只读）。

用法:
  python analyze_core.py lits 0x0801F000 0x20000480 ...
  python analyze_core.py callers 0x0800B758
  python analyze_core.py callees 0x08008D00
  python analyze_core.py func 0x08008D00 [bytes]
  python analyze_core.py findcalls 0x0801F000
"""
from __future__ import annotations

import json
import os
import re
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_OP_IMM, CS_OP_MEM, CS_OP_REG

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
BASE = 0x08003000

with open(FW, "rb") as f:
    IMAGE = f.read()
FUNC_LIST = json.load(open(FUNCS, encoding="utf-8"))

# addr(int) -> function dict
BY_ADDR = {}
for fn in FUNC_LIST:
    BY_ADDR[int(fn["addr"], 16)] = fn

# build callers map (normalize thumb bit)
CALLERS = {}
for fn in FUNC_LIST:
    a = int(fn["addr"], 16)
    for c in fn.get("callees", []):
        ca = int(c, 16) & ~1
        CALLERS.setdefault(ca, []).append(a)


def off(addr):
    return addr - BASE


def func_at(addr):
    addr &= ~1
    return BY_ADDR.get(addr)


def read(addr, n):
    o = off(addr)
    return IMAGE[o:o + n]


def disasm(addr, nbytes, thumb=True):
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
    md.detail = True
    md.skipdata = True
    out = []
    for insn in md.disasm(read(addr, nbytes), addr):
        out.append(insn)
    return out


def fmt(insn):
    line = "0x%08X: %-24s %-8s %s" % (insn.address, insn.bytes.hex(" "), insn.mnemonic, insn.op_str)
    return line


def lit_refs(addr, nbytes):
    """Return (target, insn_addr, kind) for ldr-literal / adr refs."""
    refs = []
    for insn in disasm(addr, nbytes):
        if insn.mnemonic.startswith("ldr") and insn.operands:
            pass
        # capstone detail for ldr literal: operand type MEM with base PC
        if len(insn.operands) == 2 and insn.operands[1].type == CS_OP_MEM:
            m = insn.operands[1].mem
            if insn.reg_name(m.base) == "pc":
                pc = (insn.address + 4) & ~3
                target = pc + m.disp
                try:
                    val = struct.unpack("<I", read(target, 4))[0]
                except Exception:
                    val = None
                refs.append(("ldrlit", insn.address, target, val))
    return refs


def find_bx_lr_end(addr, maxbytes=0x800):
    """Rough: find first `pop {..pc}` / `bx lr` / `bx <reg>` end."""
    for insn in disasm(addr, maxbytes):
        if insn.mnemonic in ("bx", "pop") and ("lr" in insn.op_str or "pc" in insn.op_str):
            return insn.address + insn.size
        if insn.mnemonic == "b" and insn.address != addr:
            pass
    return None


def show_func(addr, limit=200):
    fn = func_at(addr)
    if fn:
        start = int(fn["addr"], 16)
        end = int(fn["end"], 16)
        size = int(fn["size"])
    else:
        start = addr & ~1
        end = start + 0x200
        size = 0x200
    for insn in disasm(start, max(end - start, 2))[:limit]:
        print(fmt(insn))
    return start, end


def main():
    cmd = sys.argv[1] if len(sys.argv) > 1 else "help"
    args = sys.argv[2:]
    if cmd == "lits":
        vals = [int(a, 16) for a in args]
        print("functions referencing literals:", [hex(v) for v in vals])
        for fn in FUNC_LIST:
            a = int(fn["addr"], 16)
            lits = [int(x, 16) for x in fn.get("lits", [])]
            hits = [hex(v) for v in vals if v in lits]
            if hits:
                print("  0x%08X size=%d callees=%d lits=%s" % (a, fn["size"], len(fn.get("callees", [])), hits))
    elif cmd == "callers":
        for a in args:
            addr = int(a, 16) & ~1
            print("callers of 0x%08X: %s" % (addr, [hex(x) for x in sorted(CALLERS.get(addr, []))]))
    elif cmd == "callees":
        for a in args:
            fn = func_at(int(a, 16))
            if fn:
                print("callees of 0x%08X: %s" % (int(fn["addr"], 16),
                                                 [hex(int(x, 16) & ~1) for x in fn.get("callees", [])]))
            else:
                print("no func", a)
    elif cmd == "func":
        show_func(int(args[0], 16), int(args[1]) if len(args) > 1 else 200)
    elif cmd == "findcalls":
        # scan whole image for BL targets == addr (decode linearly)
        targets = [int(a, 16) & ~1 for a in args]
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
        md.detail = True
        hits = []
        for insn in md.disasm(IMAGE, BASE):
            if insn.mnemonic in ("bl", "blx") and insn.operands:
                t = insn.operands[0].imm
                if t in targets:
                    hits.append((t, insn.address))
        for t in targets:
            print("BL -> 0x%08X:" % t, [hex(a) for tt, a in hits if tt == t])
    else:
        print(__doc__)


if __name__ == "__main__":
    main()
