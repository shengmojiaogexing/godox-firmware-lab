# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— sync 函数反汇编 + 调用点搜索。"""
import sys
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
FW = ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin"
data = FW.read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)


def dis(lo, n, base=BASE):
    out = []
    for ins in md.disasm(data[lo - base:lo - base + n], lo):
        out.append(ins)
    return out


def show(lo, n):
    print("\n--- 0x%08X .. 0x%08X ---" % (lo, lo + n))
    for ins in dis(lo, n):
        print("0x%08X: %-08s %s" % (ins.address, ins.mnemonic, ins.op_str))


lit = 0x0800B57C
show(lit - 0x60, 0xC0)

# 扫描 BL 到候选入口（先猜测函数入口 = 向后找 push 开头）
# 打印更前面找函数起点
