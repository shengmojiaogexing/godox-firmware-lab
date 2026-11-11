# -*- coding: utf-8 -*-
"""V1C：按 LDR(literal) 精确解析字面量池 + 找 ID/settings 结构（scan4）。

⚠️ 已作废的候选：本脚本的「settings 基址」回溯有 bug —— 它把 `ldrb r0,[rX,#6]`
的目的寄存器 r0 当成了基址寄存器（`op_str.split(',')[0]`），因此输出的
0x200004F5 / 0x20000555 / 0x200004DB 等候选**不可信**。
真实设置区基址 = **0x20001086**，ID 字节 = **RAM[0x2000108C]**，证据见
`devices/v1c/re/audit_static.py` 与 `keypatch/dev/v1c/AUDIT.md` §8。
本文件仅作定位过程留档。
"""
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000
SYNC_FN = 0x080121BC

d = open(FW, 'rb').read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.skipdata = True

insns = {}
ldr_lit = []      # (ins_addr, dest_reg, lit_addr, lit_value)
for ins in md.disasm(d, BASE):
    insns[ins.address] = ins
    if ins.mnemonic == 'ldr' and ins.op_str.startswith('r'):
        try:
            dest = int(ins.op_str.split(',')[0][1:])
        except ValueError:
            dest = -1
        # 形如 "r0, [pc, #0x1c]"
        if '[pc,' in ins.op_str:
            try:
                imm = int(ins.op_str.split('#')[1].rstrip(']'), 16)
            except ValueError:
                continue
            lit_addr = ((ins.address + 4) & ~3) + imm
            off = lit_addr - BASE
            if 0 <= off <= len(d) - 4:
                val = int.from_bytes(d[off:off + 4], 'little')
                ldr_lit.append((ins.address, dest, lit_addr, val))

by_lit = defaultdict(list)
for ins_addr, dest, lit_addr, val in ldr_lit:
    by_lit[val].append(ins_addr)

print('=== 同步字函数 0x%08X 各调用点前 8 条指令 ===' % SYNC_FN)
callers = [a for a, ins in insns.items()
           if ins.mnemonic in ('bl', 'blx') and ins.op_str.strip() == '#0x%x' % SYNC_FN]
callers.sort()
for a in callers:
    print('\n-- caller 0x%08X' % a)
    seq = []
    addr = a - 2
    while len(seq) < 10 and addr in insns:
        seq.append(insns[addr])
        addr -= insns[addr].size
    for ins in reversed(seq):
        extra = ''
        if ins.mnemonic == 'ldr' and '[pc,' in ins.op_str:
            try:
                imm = int(ins.op_str.split('#')[1].rstrip(']'), 16)
                lit = ((ins.address + 4) & ~3) + imm
                val = int.from_bytes(d[lit - BASE:lit - BASE + 4], 'little')
                extra = '   ; lit@0x%08X = 0x%08X' % (lit, val)
            except (ValueError, IndexError):
                pass
        print('   0x%08X  %-14s %-7s %s%s' % (ins.address, ins.bytes.hex(' '), ins.mnemonic, ins.op_str, extra))
    print('   0x%08X  %-14s %-7s %s' % (a, insns[a].bytes.hex(' '), insns[a].mnemonic, insns[a].op_str))

print('\n=== 指向 RAM 且被 ldrb [rX,#6] 使用的 settings 基址候选 ===')
# 找 ldrb rY,[rX,#6] 指令，回溯同函数内最近一次给 rX 的 ldr literal
for addr, ins in sorted(insns.items()):
    if ins.mnemonic == 'ldrb' and ins.op_str.endswith(', #6]'):
        base_reg = int(ins.op_str.split(',')[0][1:])
        # 回溯 30 条
        cur = addr - 2
        found = None
        steps = 0
        while cur >= addr - 0x80 and steps < 30:
            p = insns.get(cur)
            if p is None:
                cur -= 2
                continue
            steps += 1
            if p.mnemonic == 'ldr' and p.op_str.startswith('r%d,' % base_reg):
                try:
                    imm = int(p.op_str.split('#')[1].rstrip(']'), 16)
                except (ValueError, IndexError):
                    pass
                else:
                    lit = ((p.address + 4) & ~3) + imm
                    if 0 <= lit - BASE <= len(d) - 4:
                        val = int.from_bytes(d[lit - BASE:lit - BASE + 4], 'little')
                        if 0x20000000 <= val < 0x20002000:
                            found = (p.address, lit, val)
                            break
            cur -= p.size
        if found:
            print('  ldrb@0x%08X  <- ldr@0x%08X lit=0x%08X value=0x%08X (ID addr=0x%08X)'
                  % (addr, found[0], found[1], found[2], found[2] + 6))
