# -*- coding: utf-8 -*-
"""TMR7 recon: find where the firmware configures the 1 ms tick timer.

Self-contained, read-only analysis of devices/x3c/firmware/X3_C_v1.38_app.bin.
Uses Ghidra exports only as an index (functions.csv) and capstone for the
actual instruction bytes.
"""
from __future__ import annotations

import csv
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
GH = os.path.join(ROOT, "devices", "x3c", "re", "ghidra_exports", "main")
BASE = 0x08004800


class Image:
    def __init__(self, path=IMAGE):
        with open(path, "rb") as f:
            self.data = f.read()
        self.base = BASE

    def off(self, va):
        return va - self.base

    def bytes_at(self, va, n):
        return self.data[self.off(va):self.off(va) + n]

    def u16(self, va):
        return struct.unpack_from("<H", self.data, self.off(va))[0]

    def u32(self, va):
        return struct.unpack_from("<I", self.data, self.off(va))[0]


IMG = Image()


def functions():
    rows = []
    with open(os.path.join(GH, "functions.csv"), encoding="utf-8") as f:
        for r in csv.DictReader(f):
            rows.append((int(r["addr"], 16), int(r["size"]), r["name"]))
    rows.sort()
    return rows


FUNCS = functions()


def func_at(addr):
    for a, size, name in FUNCS:
        if a <= addr < a + size:
            return a, size, name
    return None


def bl_target(hw1, hw2):
    """Decode Thumb-2 BL/BLX (immediate) -> target vaddr, or None."""
    if (hw1 & 0xF800) != 0xF000:
        return None
    if (hw2 & 0xD000) not in (0xD000, 0xC000):
        return None
    if (hw2 & 0xD000) == 0xC000:
        return None                      # BLX reg
    s = (hw1 >> 10) & 1
    j1 = (hw2 >> 13) & 1
    j2 = (hw2 >> 11) & 1
    i1 = 1 - (j1 ^ s)
    i2 = 1 - (j2 ^ s)
    imm = (s << 24) | (i1 << 23) | (i2 << 22) | \
          ((hw1 & 0x3FF) << 12) | ((hw2 & 0x7FF) << 1)
    if s:
        imm -= (1 << 25)
    return imm


def bl_callers(target):
    """All code sites that BL to *target* (via the BL encoding)."""
    out = []
    d = IMG.data
    for o in range(0, len(d) - 4, 2):
        site = IMG.base + o
        hw1, hw2 = struct.unpack_from("<HH", d, o)
        imm = bl_target(hw1, hw2)
        if imm is None:
            continue
        if site + 4 + imm == target:
            out.append(site)
    return out


def literal_refs(value):
    """All 4-byte-aligned image positions holding *value* (vaddr)."""
    out = []
    d = IMG.data
    pat = struct.pack("<I", value)
    start = 0
    while True:
        i = d.find(pat, start)
        if i < 0:
            break
        if i % 4 == 0:
            out.append(IMG.base + i)
        start = i + 1
    return out


def dis(va, n=60):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
    out = []
    for i in md.disasm(IMG.bytes_at(va, n * 4), va):
        out.append("  %08x  %-14s %s %s"
                   % (i.address, i.bytes.hex(" "), i.mnemonic, i.op_str))
    return "\n".join(out)


def code_refs_to_literal(value, window=0x40):
    """Find LDR-literal instructions that load a literal pool entry == value.

    Scans backward from each pool word for the nearest Thumb-1 `ldr rX,[pc,#imm]`
    or Thumb-2 `ldr.w rX,[pc,#imm12]`/`ldr.w rX,[pc,#-imm12]` whose PC+4+imm
    points at the pool word.
    """
    hits = []
    for pool_va in literal_refs(value):
        for back in range(2, window, 2):
            site = pool_va - back
            try:
                hw = IMG.u16(site)
            except IndexError:
                continue
            pc = site + 4
            target = None
            if (hw & 0xF800) == 0x4800:            # Thumb-1 LDR Rt,[PC,#imm8*4]
                target = (pc & ~3) + (hw & 0xFF) * 4
            else:
                try:
                    hw2 = IMG.u16(site + 2)
                except IndexError:
                    continue
                if (hw & 0xFF7F) == 0xF85F:        # Thumb-2 LDR.W Rt,[PC,#+/-imm12]
                    imm12 = hw2 & 0xFFF
                    target = (pc & ~3) + imm12 if (hw & 0x0080) else (pc & ~3) - imm12
            if target == pool_va:
                hits.append(site)
    return sorted(set(hits))


def show_literal(value, label):
    print("=" * 70)
    print("%s = %#010x" % (label, value))
    refs = code_refs_to_literal(value)
    for site in refs:
        f = func_at(site)
        print("  used @ %#010x  in %s" % (site, ("%#x %s" % (f[0], f[2])) if f else "?"))
    return refs


if __name__ == "__main__":
    for val, label in [
        (0x40001400, "TMR7"),
        (0x40001000, "TMR6"),
        (0x40000000, "TMR2"),
        (0x40000C00, "TMR5"),
        (0x40001800, "TMR12"),
        (0x20000310, "tick counter"),
        (0x20000004, "tick flag"),
    ]:
        show_literal(val, label)
