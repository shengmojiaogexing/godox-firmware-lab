# -*- coding: utf-8 -*-
"""X2T-S V1.4 侦察 2：反汇编关键区域、扫描 movw/movt 常量 0xC368、字符串、函数边界。"""
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from capstone.arm import ARM_OP_IMM, ARM_OP_REG

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000


def dis(addr, n=32, label=""):
    off = addr - BASE
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True
    print("---- %s 0x%08X (off 0x%05X) ----" % (label, addr, off))
    count = 0
    for ins in md.disasm(DATA[off:off + n * 4], addr):
        print("  0x%08X  %-24s %s %s" % (ins.address, ins.bytes.hex(" "),
                                         ins.mnemonic, ins.op_str))
        count += 1
        if count >= n:
            break


def scan_movw():
    """扫描 movw/movt 立即数，找 0xC368 / 0x0000 / 0x2000xxxx 之类。"""
    print("== movw/movt immediate scan ==")
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True
    hits = []
    for off in range(0, len(DATA) - 4, 2):
        for ins in md.disasm(DATA[off:off + 4], BASE + off):
            if ins.mnemonic in ("movw", "movt", "mov.w", "mov"):
                ops = ins.operands
                if len(ops) == 2 and ops[0].type == ARM_OP_REG and ops[1].type == ARM_OP_IMM:
                    imm = ops[1].imm & 0xFFFFFFFF
                    if imm in (0xC368, 0xC368 >> 1, 0x61B4):
                        hits.append((ins.address, ins.mnemonic, imm))
            break
    for h in hits[:50]:
        print("  %s 0x%08X %s 0x%X" % ("", h[0], h[1], h[2]))
    print("  total:", len(hits))
    print()


def scan_word(vals):
    print("== data word scan %s ==" % [hex(v) for v in vals])
    for v in vals:
        pat = struct.pack("<I", v)
        idx = DATA.find(pat)
        out = []
        while idx >= 0:
            out.append(hex(BASE + idx))
            idx = DATA.find(pat, idx + 1)
        print("  0x%08X: %s" % (v, out[:20]))


def strings():
    print("== strings (len>=4) ==")
    cur = bytearray()
    start = 0
    for i, b in enumerate(DATA):
        if 0x20 <= b < 0x7F:
            if not cur:
                start = i
            cur.append(b)
        else:
            if len(cur) >= 5:
                print("  0x%05X %s" % (start, cur.decode()))
            cur = bytearray()
    if len(cur) >= 5:
        print("  0x%05X %s" % (start, cur.decode()))
    print()


def main():
    print("entropy:", end=" ")
    import math, collections
    c = collections.Counter(DATA)
    ent = -sum((n / len(DATA)) * math.log2(n / len(DATA)) for n in c.values())
    print("%.3f" % ent)
    print()
    dis(0x080031AC, 40, "reset vector")
    print()
    dis(0x080031C6, 20, "NMI?")
    print()
    dis(0x080031C4, 20, "??")
    print()
    dis(0x0800E4C0, 24, "near tail")
    print()
    print("== 0x2F00..0x3080 hex ==")
    for a in range(0x2F00, 0x3080, 16):
        print("  0x%05X  %s" % (a, DATA[a:a + 16].hex(" ")))
    print()
    scan_movw()
    scan_word([0xC368, 0x61B4, 50024])
    strings()


if __name__ == "__main__":
    main()
