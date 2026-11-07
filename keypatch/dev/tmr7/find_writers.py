# -*- coding: utf-8 -*-
"""Find every LDR-literal site that reads a given RAM address literal and show
whether the loaded register is later stored to (i.e. who writes the variable)."""
import os
import struct

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
BASE = 0x08004800
DATA = open(IMAGE, "rb").read()


def u16(va):
    return struct.unpack_from("<H", DATA, va - BASE)[0]


def u32(va):
    return struct.unpack_from("<I", DATA, va - BASE)[0]


def pool_positions(value):
    pat = struct.pack("<I", value)
    out = []
    i = 0
    while True:
        j = DATA.find(pat, i)
        if j < 0:
            break
        if j % 4 == 0:
            out.append(BASE + j)
        i = j + 1
    return out


def ldr_sites(pool_va):
    """All LDR-literal instructions whose target is *pool_va*."""
    hits = []
    n = len(DATA)
    for o in range(0, n - 4, 2):
        site = BASE + o
        hw = struct.unpack_from("<H", DATA, o)[0]
        pc = site + 4
        target = None
        if (hw & 0xF800) == 0x4800:               # LDR Rd,[PC,#imm8*4]
            target = (pc & ~3) + (hw & 0xFF) * 4
        elif (hw & 0xFF7F) == 0xF85F:             # LDR.W Rt,[PC,#+/-imm12]
            hw2 = struct.unpack_from("<H", DATA, o + 2)[0]
            imm = hw2 & 0xFFF
            target = (pc & ~3) + imm if (hw & 0x0080) else (pc & ~3) - imm
        if target == pool_va:
            hits.append(site)
    return hits


def disasm_region(lo, hi, ctx=0):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
    lo = max(BASE, lo - ctx)
    hi = hi + ctx
    out = []
    for ins in md.disasm(DATA[lo - BASE:hi - BASE], lo):
        out.append("  %08x  %-10s %s %s"
                   % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
    return "\n".join(out)


def analyse(value, label):
    print("=" * 72)
    print("%s = %#010x" % (label, value))
    for pool in pool_positions(value):
        for site in ldr_sites(pool):
            print("pool %#x read by LDR @ %#x" % (pool, site))
            print(disasm_region(site, site + 24, ctx=0))


if __name__ == "__main__":
    import sys
    addrs = [int(a, 0) for a in sys.argv[1:]] or [0x200003E7]
    for a in addrs:
        analyse(a, "RAM")
