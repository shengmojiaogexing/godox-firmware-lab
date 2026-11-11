# -*- coding: utf-8 -*-
"""V1C_V2.0.bin 初步静态扫描：
- 向量表 / base 假设
- 全镜像 BL/B.W 目标（函数入口候选）
- 50024 字面量及其引用
- CC2500 register 4/5 写入点
用法：tools\\python\\python.exe devices\\v1c\\re\\scan1.py
"""
import sys
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000

d = open(FW, 'rb').read()
print('size = %d (0x%X)' % (len(d), len(d)))

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True

# 全量反汇编（线性扫描；Thumb 代码中夹有字面量，可能产生伪指令，仅用于找 BL 调用）
bl_targets = defaultdict(list)   # target -> [call_site]
for ins in md.disasm(d, BASE):
    if ins.mnemonic in ('bl', 'blx', 'b.w', 'b'):
        try:
            target = int(ins.op_str, 16)
        except ValueError:
            continue
        bl_targets[target].append(ins.address)

print('\n=== 0x08003000 附近被调用最多的目标（函数入口候选 TOP 40）===')
for tgt, sites in sorted(bl_targets.items(), key=lambda kv: -len(kv[1]))[:40]:
    print('  0x%08X  calls=%3d  from %s' % (tgt, len(sites),
          ', '.join('0x%08X' % s for s in sites[:6])))

# 50024 字面量
needle = (50024).to_bytes(4, 'little')
hits = [i for i in range(len(d) - 3) if d[i:i + 4] == needle]
print('\n=== 50024(0xC368) 字面量：')
for h in hits:
    print('  file 0x%05X  base+ = 0x%08X' % (h, BASE + h))

print('\n=== 字面量池里所有指向 0x0801xxxx/0x2000xxxx 的 4 字节值（按值统计）===')
# 简单扫描 4 字节小端值
vals = defaultdict(int)
for off in range(0, len(d) - 3, 2):
    v = int.from_bytes(d[off:off + 4], 'little')
    if 0x08000000 <= v < 0x08030000 or 0x20000000 <= v < 0x20004000:
        vals[v] += 1
for v, n in sorted(vals.items()):
    print('  0x%08X  x%d' % (v, n))
