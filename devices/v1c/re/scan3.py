# -*- coding: utf-8 -*-
"""V1C 同步字调用点上下文 + CC2500 初始化表 分析（scan3）。"""
import sys
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000
SYNC_FN = 0x080121BC
WRITE_REG = 0x080156CE

d = open(FW, 'rb').read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.skipdata = True
insns = {}
for ins in md.disasm(d, BASE):
    insns[ins.address] = ins


def dump(lo, hi, label=''):
    print('\n--- %s 0x%08X..0x%08X ---' % (label, lo, hi))
    addr = lo
    while addr <= hi:
        ins = insns.get(addr)
        if ins is None:
            addr += 2
            continue
        print('  0x%08X  %-14s %-7s %s' % (ins.address, ins.bytes.hex(' '), ins.mnemonic, ins.op_str))
        addr += ins.size


# 1. 同步字函数调用点上下文
for a in (0x08006274, 0x080066B2, 0x0800D484, 0x0800E0C2, 0x0800E138):
    dump(a - 0x30, a + 0x10, 'caller of SYNC_FN')

# 2. CC2500 初始化表
dump(0x080153F0, 0x0801560A, 'CC2500 init table region')
dump(0x080156CE, 0x08015700, 'WriteReg')

# 3. 其余 WriteReg 调用点上下文（0x0800669C / 0x0800F9B8..0x08010500）
for a in (0x0800669C, 0x0800F9B8, 0x0800FA90, 0x0800FB44, 0x0800FBF6, 0x08010500):
    dump(a - 0x10, a + 0x6, 'WR site')

# 4. 找 r0=4/5 的写法：扫描 bl WriteReg 前 3 条指令
print('\n=== WriteReg 调用前的寄存器设置（筛 reg 4/5）===')
for a, ins in insns.items():
    if ins.mnemonic in ('bl', 'blx') and ins.op_str.strip() == '#0x%x' % WRITE_REG:
        prev = []
        addr = a - 2
        while addr >= a - 8 and len(prev) < 3:
            p = insns.get(addr)
            if p is not None:
                prev.append(p)
                addr -= p.size
            else:
                addr -= 2
        prev.reverse()
        text = ' | '.join('%s %s' % (p.mnemonic, p.op_str) for p in prev)
        print('  0x%08X  <- %s' % (a, text))
