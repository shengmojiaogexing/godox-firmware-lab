# -*- coding: utf-8 -*-
"""V1C 同步字函数 / CC2500 写入点 / C.Fn 结构 静态定位（scan2）。

- 全镜像 BL/B.W 扫描，找同步字函数 0x080121BC 的调用点
- 找 CC2500 WriteReg(0x080156CE) 调用点，筛 reg4/reg5
- 找 AD100 校验表字节模式（判断是否同源 C.Fn 体系）
用法：tools\\python\\python.exe devices\\v1c\\re\\scan2.py
"""
import sys
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000
SYNC_FN = 0x080121BC
WRITE_REG = 0x080156CE

d = open(FW, 'rb').read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True

callers = defaultdict(list)
branch_targets = defaultdict(list)
insns = {}
for ins in md.disasm(d, BASE):
    insns[ins.address] = ins
    if ins.mnemonic in ('bl', 'blx', 'b', 'b.w'):
        op = ins.op_str.strip()
        if not op.startswith('#'):
            continue
        try:
            target = int(op[1:], 16)
        except ValueError:
            continue
        if ins.mnemonic in ('bl', 'blx'):
            callers[target].append(ins.address)
        branch_targets[target].append(ins.address)

print('=== 调用 0x%08X（同步字函数）的位置 ===' % SYNC_FN)
for a in callers.get(SYNC_FN, []):
    # 往上找函数入口（最近一个被调用目标，若有）
    print('  0x%08X' % a)

print('\n=== 调用 WriteReg 0x%08X 的位置（筛 r0 为 4/5 的上下文）===' % WRITE_REG)
wr_sites = callers.get(WRITE_REG, [])
print('  总调用点 %d' % len(wr_sites))
for a in wr_sites:
    ctx = []
    # 取往前 12 条指令
    addr = a
    for _ in range(14):
        addr -= 2
        if addr in insns:
            ctx.append(insns[addr])
        else:
            break
    ctx.reverse()
    text = ' | '.join('%s %s' % (i.mnemonic, i.op_str) for i in ctx[-8:])
    print('  0x%08X: %s' % (a, text))

print('\n=== AD100 校验表模式搜索 ===')
pattern = bytes.fromhex('0102039601630101')
for i in range(len(d) - len(pattern)):
    if d[i:i + len(pattern)] == pattern:
        print('  hit file 0x%05X  addr 0x%08X' % (i, BASE + i))
        print('    bytes:', d[i:i + 32].hex(' '))

print('\n=== 函数边界：同步字函数前后 0x08012180..0x08012220 反汇编 ===')
lo, hi = 0x08012180, 0x08012220
for ins in md.disasm(d[lo - BASE:hi - BASE], lo):
    print('  0x%08X  %-14s %s %s' % (ins.address, ins.bytes.hex(' '), ins.mnemonic, ins.op_str))
