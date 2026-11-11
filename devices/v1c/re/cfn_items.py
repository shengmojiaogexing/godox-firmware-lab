# -*- coding: utf-8 -*-
"""V1C：settings[0..0xD]（基址 0x20001086）读写点与 C.Fn 项语义初扫（cfn_items）。"""
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000
SETTINGS = 0x20001086
d = open(FW, 'rb').read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.skipdata = True

insns = {}
for ins in md.disasm(d, BASE):
    insns[ins.address] = ins

# 反向数据流：在函数内找 ldr rX,=SETTINGS，随后追踪 add/sub 常量、ldrb/strb [rX,#imm]
hits = defaultdict(list)          # (op, offset) -> [(addr, func_start, context)]
func_starts = sorted({0x080030D4} | {
    ins.address for ins in insns.values() if ins.address == 0 or False})


def func_of(addr, depth=300):
    a = addr
    for _ in range(depth):
        a -= 2
        p = insns.get(a)
        if p is None:
            continue
        if p.mnemonic == 'push' and 'lr' in p.op_str:
            return p.address
    return None


for addr, ins in sorted(insns.items()):
    if ins.mnemonic not in ('ldrb', 'strb', 'ldrsb'):
        continue
    parts = ins.op_str.split(',')
    if len(parts) < 3:
        continue
    try:
        base_reg = int(parts[1].strip()[1:])
    except ValueError:
        continue
    # 回溯 60 条：找 rBase 的 ldr literal == SETTINGS 或 SETTINGS+n
    a = addr - 2
    base_val = None
    for _ in range(60):
        p = insns.get(a)
        if p is None:
            a -= 2
            continue
        if p.mnemonic == 'ldr' and p.op_str.startswith('r%d,' % base_reg) and '[pc,' in p.op_str:
            try:
                imm = int(p.op_str.split('#')[1].rstrip(']'), 16)
            except (ValueError, IndexError):
                break
            lit = ((p.address + 4) & ~3) + imm
            if 0 <= lit - BASE <= len(d) - 4:
                v = int.from_bytes(d[lit - BASE:lit - BASE + 4], 'little')
                if SETTINGS <= v <= SETTINGS + 0xD:
                    base_val = v
            break
        a -= p.size
    if base_val is None:
        continue
    # 偏移
    off_txt = parts[2].strip()
    if off_txt.startswith('#'):
        try:
            off = int(off_txt[1:], 16)
        except ValueError:
            continue
    else:
        off = 0
    total = base_val - SETTINGS + off
    if 0 <= total <= 0xD:
        ctx = []
        b = addr - 2
        while len(ctx) < 10 and b in insns:
            ctx.append(insns[b])
            b -= insns[b].size
        ctx.reverse()
        hits[(ins.mnemonic, total)].append((addr, func_of(addr),
            ' | '.join('%s %s' % (p.mnemonic, p.op_str) for p in ctx[-6:])))

for (op, off), items in sorted(hits.items(), key=lambda kv: (kv[0][1], kv[0][0])):
    print('\n== settings[%d] (%s) 0x%08X  共 %d 处 ==' % (off, op, SETTINGS + off, len(items)))
    for addr, fn, ctx in items:
        print('  0x%08X (func~0x%08X): %s' % (addr, fn or 0, ctx))
