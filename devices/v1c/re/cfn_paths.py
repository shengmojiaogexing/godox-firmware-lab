# -*- coding: utf-8 -*-
"""V1C：设置应用/RF 重配路径分析（cfn_paths）：
- 函数 0x0800DCDC..0x0800E19B（含两个 sync 调用点）与调用者 0x08015B52
- RF 频率函数 0x0801217C 的调用点
- 0x200004F5（疑似 CH 生效字节）的读写点
"""
from collections import defaultdict

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

FW = r'devices/v1c/firmware/V1C_V2.0.bin'
BASE = 0x08003000
OUT = r'devices/v1c/re/log_cfn_paths.txt'
d = open(FW, 'rb').read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.skipdata = True

insns = {}
flow = defaultdict(list)
lits = []
for ins in md.disasm(d, BASE):
    insns[ins.address] = ins
    if ins.mnemonic in ('bl', 'blx', 'b', 'b.w'):
        op = ins.op_str.strip()
        if op.startswith('#'):
            try:
                flow[int(op[1:], 16)].append(ins.address)
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

out = []


def w(s=''):
    out.append(s)


def dump(lo, hi):
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
                extra = '   ; [0x%08X]=0x%08X' % (lit, val)
            except (ValueError, IndexError):
                pass
        w('  0x%08X  %-14s %-7s %s%s' % (ins.address, ins.bytes.hex(' '),
                                          ins.mnemonic, ins.op_str, extra))
        addr += ins.size


w('=== callers of 0x0800DCDC ===')
w(', '.join(hex(x) for x in flow.get(0x0800DCDC, [])))
w('')
w('=== callers of 0x0801217C (RF freq apply?) ===')
w(', '.join(hex(x) for x in flow.get(0x0801217C, [])))
w('')
w('=== callers of 0x0800664C (RF init) ===')
w(', '.join(hex(x) for x in flow.get(0x0800664C, [])))
w('')
w('=== literals == 0x200004F5 的指令及上下文 ===')
for ins_addr, lit, val in lits:
    if val == 0x200004F5:
        seq = []
        a = ins_addr - 2
        while len(seq) < 8 and a in insns:
            seq.append(insns[a])
            a -= insns[a].size
        w('')
        w('lit-user 0x%08X' % ins_addr)
        for p in reversed(seq):
            w('  0x%08X  %s %s' % (p.address, p.mnemonic, p.op_str))
        w('  0x%08X  %s %s' % (ins_addr, insns[ins_addr].mnemonic, insns[ins_addr].op_str))

w('')
w('=== 0x0800DCDC..0x0800E1A0 全函数 ===')
dump(0x0800DCDC, 0x0800E1A0)
w('')
w('=== 调用者 0x08015B52 附近 ===')
dump(0x08015AE0, 0x08015B80)
w('')
w('=== RF freq 函数 0x0801217C ===')
dump(0x0801217C, 0x080121BC)

with open(OUT, 'w', encoding='utf-8') as fh:
    fh.write('\n'.join(out))
print('written', OUT, len(out))
