# -*- coding: utf-8 -*-
"""V1C C.Fn 子系统初扫（cfn_scan1）：
- settings 区（0x20001080/0x20001086 等）引用点
- C.Fn 改值处理函数 0x080061CC / 0x0800D3D8 附近
- 菜单/ID 显示 0x08004E00..0x08005050
- 设置持久化 0x08006000..0x08006060
输出到 devices/v1c/re/log_cfn_scan1.txt
"""
import os
import sys
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000
OUT = r'devices/v1c/re/log_cfn_scan1.txt'

d = open(FW, 'rb').read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.skipdata = True

insns = {}
literals = []          # (ins_addr, dest, lit_addr, value)
for ins in md.disasm(d, BASE):
    insns[ins.address] = ins
    if ins.mnemonic == 'ldr' and '[pc,' in ins.op_str and ins.op_str.startswith('r'):
        try:
            dest = int(ins.op_str.split(',')[0][1:])
            imm = int(ins.op_str.split('#')[1].rstrip(']'), 16)
        except ValueError:
            continue
        lit = ((ins.address + 4) & ~3) + imm
        off = lit - BASE
        if 0 <= off <= len(d) - 4:
            val = int.from_bytes(d[off:off + 4], 'little')
            literals.append((ins.address, dest, lit, val))

out = []


def w(s=''):
    out.append(s)


def dump(lo, hi, label):
    w('')
    w('--- %s  0x%08X..0x%08X ---' % (label, lo, hi))
    addr = lo
    while addr <= hi:
        ins = insns.get(addr)
        if ins is None:
            addr += 2
            continue
        extra = ''
        if ins.mnemonic == 'ldr' and '[pc,' in ins.op_str:
            try:
                imm = int(ins.op_str.split('#')[1].rstrip(']'), 16)
                lit = ((ins.address + 4) & ~3) + imm
                val = int.from_bytes(d[lit - BASE:lit - BASE + 4], 'little')
                extra = '   ; [0x%08X] = 0x%08X' % (lit, val)
            except (ValueError, IndexError):
                pass
        w('  0x%08X  %-14s %-7s %s%s' % (ins.address, ins.bytes.hex(' '),
                                          ins.mnemonic, ins.op_str, extra))
        addr += ins.size


w('=== RAM 字面量 0x20001000..0x20001180 的引用点 ===')
byval = defaultdict(list)
for ins_addr, dest, lit, val in literals:
    if 0x20001000 <= val < 0x20001180:
        byval[val].append((ins_addr, dest, lit))
for val in sorted(byval):
    w('')
    w('  0x%08X  被 %d 处引用' % (val, len(byval[val])))
    for ins_addr, dest, lit in byval[val][:20]:
        ctx = []
        a = ins_addr - 2
        for _ in range(8):
            p = insns.get(a)
            if p is None:
                a -= 2
                continue
            ctx.append(p)
            a -= p.size
        ctx.reverse()
        w('    0x%08X (%s)  ctx: %s' % (ins_addr, dest,
          ' | '.join('%s %s' % (p.mnemonic, p.op_str) for p in ctx[-5:])))

dump(0x08004D80, 0x08005060, 'C.Fn / ID 显示区域')
dump(0x08006000, 0x08006080, '设置持久化候选')
dump(0x08006190, 0x080062F0, 'C.Fn 改值处理 0x080061CC')
dump(0x0800D380, 0x0800D4A0, 'C.Fn 改值处理 0x0800D3D8')

w('')
w('=== 全部 ldrb/ldrb 偏移访问 settings 区（0x20001080..0x20001100）的代码点 ===')
# 找 ldr rX,=0x20001080.. + ldrb/strb [rX,#n]
targets = {}
for val in byval:
    targets[val] = [x[0] for x in byval[val]]
for val, sites in sorted(targets.items()):
    w('')
    w('base 0x%08X:' % val)
    count = 0
    for ins_addr, ins in sorted(insns.items()):
        if ins.mnemonic in ('ldrb', 'strb') and ins.op_str.startswith('r'):
            parts = ins.op_str.split(',')
            if len(parts) < 3:
                continue
            try:
                base_reg = int(parts[1].strip()[1:])
            except ValueError:
                continue
            # 回溯 40 条找该基址寄存器最近一次 ldr literal
            a = ins_addr - 2
            for _ in range(40):
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
                    lv = int.from_bytes(d[lit - BASE:lit - BASE + 4], 'little') if 0 <= lit - BASE <= len(d) - 4 else 0
                    if lv == val:
                        w('   0x%08X  %s %s' % (ins_addr, ins.mnemonic, ins.op_str))
                        count += 1
                    break
                a -= p.size
            if count >= 40:
                break

with open(OUT, 'w', encoding='utf-8') as fh:
    fh.write('\n'.join(out))
print('written', OUT, len(out), 'lines')
