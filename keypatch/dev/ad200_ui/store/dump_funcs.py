#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""把指定函数/区间反汇编成带字面量注解的文本，用于人工核验。

用法：
  python dump_funcs.py 0x080043e4 0x0800b820 0x0800ac58 0x0800ccc8
  python dump_funcs.py --range 0x0800ccc8:0x0800cdb0

注解规则：
  ldr rX,[pc,#imm] 后跟 `; = 0x........  (class)`；
  对 0x20000418 / 0x2000041F 的字面量额外标 SETTINGS / SETTINGS[7]。
"""
from __future__ import annotations

import json
import os
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_OP_MEM, CS_OP_REG
from capstone.arm import ARM_REG_PC

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


def rd32(addr):
    o = addr - BASE
    if 0 <= o <= SIZE - 4:
        return struct.unpack_from("<I", IMAGE, o)[0]
    return None


def classify(v):
    if v is None:
        return "none"
    if v == 0x20000418:
        return "SETTINGS base"
    if v == 0x2000041F:
        return "SETTINGS[7]"
    if v == 0x2000041E:
        return "SETTINGS[6] ID"
    if 0x20000000 <= v < 0x20020000:
        return "ram"
    if 0x08010000 <= v < 0x08020000:
        return "flash-param?"
    if BASE <= v < BASE + SIZE:
        return "flash"
    if 0x40000000 <= v < 0x40100000:
        return "periph"
    if 0x48000000 <= v < 0x48100000:
        return "gpio"
    if 0xE0000000 <= v < 0xE0100000:
        return "scs"
    if v < 0x10000:
        return "small"
    return "other"


def dump(start, end):
    start &= ~1
    end &= ~1
    print("=" * 78)
    print("range 0x%08x .. 0x%08x  %s" % (start, end, SYMBOLS.get(start, "")))
    print("=" * 78)
    o = start - BASE
    for ins in md.disasm(IMAGE[o:end - BASE], start):
        note = ""
        if ins.id and len(ins.operands) == 2 and ins.operands[0].type == CS_OP_REG \
                and ins.operands[1].type == CS_OP_MEM \
                and ins.operands[1].mem.base == ARM_REG_PC:
            m = ins.operands[1].mem
            la = ((ins.address + 4) & ~3) + m.disp
            v = rd32(la)
            if v is not None:
                note = "   ; lit@%08x = 0x%08x (%s)" % (la, v, classify(v))
        s = SYMBOLS.get(ins.address, "")
        print("%08x: %-10s %-28s%s%s" % (ins.address, ins.bytes.hex(),
                                         (ins.mnemonic + " " + ins.op_str).strip(),
                                         note, ("   <" + s + ">") if s else ""))


def main(argv):
    args = [a for a in argv[1:] if not a.startswith("--")]
    i = 0
    while i < len(argv):
        if argv[i] == "--range" and i + 1 < len(argv):
            a, b = argv[i + 1].split(":")
            dump(int(a, 0), int(b, 0))
            i += 2
            continue
        i += 1
    for a in args:
        addr = int(a, 0)
        end = addr + 0x200
        # 截到下一个符号起点
        later = sorted(s for s in SYMBOLS if s > addr)
        if later:
            end = later[0]
        dump(addr, end)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
