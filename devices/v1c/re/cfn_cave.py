# -*- coding: utf-8 -*-
"""检查 V1C 镜像中的 0x00 填充空洞是否可作为代码注入区（cfn_cave）。"""
import sys
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000
d = open(FW, 'rb').read()

CANDIDATES = [(0x14944, 234), (0x14D28, 222), (0x1449E, 57), (0x13F0A, 36)]

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.skipdata = True

flow = []          # (addr, kind, target)
lits = []          # (ins_addr, lit_addr, value)
for ins in md.disasm(d, BASE):
    if ins.mnemonic in ('bl', 'blx', 'b', 'b.w', 'bx', 'cbz', 'cbnz'):
        op = ins.op_str.strip()
        if op.startswith('#'):
            try:
                t = int(op[1:], 16)
                flow.append((ins.address, ins.mnemonic, t))
            except ValueError:
                pass
    if ins.mnemonic == 'ldr' and '[pc,' in ins.op_str:
        try:
            imm = int(ins.op_str.split('#')[1].rstrip(']'), 16)
        except (ValueError, IndexError):
            continue
        lit = ((ins.address + 4) & ~3) + imm
        off = lit - BASE
        if 0 <= off <= len(d) - 4:
            lits.append((ins.address, lit, int.from_bytes(d[off:off + 4], 'little')))

print('=== 长 0x00 空洞检查 ===')
for foff, n in CANDIDATES:
    lo = BASE + foff
    hi = lo + n
    branch_in = [(hex(a), k, hex(t)) for a, k, t in flow if lo <= t < hi]
    lit_into = [(hex(a), hex(l), hex(v)) for a, l, v in lits if lo <= v < hi]
    lit_inside = [(hex(a), hex(l), hex(v)) for a, l, v in lits if lo <= a < hi]
    # 前后字节
    pre = d[max(0, foff - 16):foff].hex(' ')
    post = d[hi - BASE:hi - BASE + 16].hex(' ')
    print('')
    print('cave addr 0x%08X..0x%08X  len=%d  file 0x%05X' % (lo, hi - 1, n, foff))
    print('  pre :', pre)
    print('  post:', post)
    print('  branch_into:', branch_in or 'NONE')
    print('  literal_ptr_into:', lit_into or 'NONE')
    print('  ldr_inside:', lit_inside or 'NONE')
    # 判断是否全 0
    print('  all_zero:', all(b == 0 for b in d[foff:foff + n]))
