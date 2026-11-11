# -*- coding: utf-8 -*-
"""V860IIIC 关键代码段对照反汇编（读者屏蔽 / C.Fn 渲染 / 初始化表 / WriteReg 调用点）。"""
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

BASE = 0x08003000
V860 = open(r"devices/v860iiic/firmware/V860IIIC_V1.3.bin", "rb").read()
V1C = open(r"devices/v1c/firmware/V1C_V2.0.bin", "rb").read()


def dis(data, start, length, base=BASE):
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.skipdata = True
    return list(md.disasm(data[start - base:start - base + length], start))


def show(title, insns):
    print("=" * 70)
    print(title)
    print("=" * 70)
    for i in insns:
        print("0x%08X  %-12s %-7s %s" % (i.address, i.bytes.hex(" "), i.mnemonic, i.op_str))


show("V860 造型灯/读者函数 0x080036A8..0x0800379F", dis(V860, 0x080036A8, 0xF8))
show("V1C  造型灯/读者函数 0x080036D4..0x080037CB", dis(V1C, 0x080036D4, 0xF8))

show("V860 C.Fn 渲染（CH/ID/LIGHT 段）0x08004DD0..0x08005060", dis(V860, 0x08004DD0, 0x290))
show("V1C  C.Fn 渲染（CH/ID/LIGHT 段）0x08004E34..0x080050C4", dis(V1C, 0x08004E34, 0x290))

# WriteReg(4/5) call sites
print("=" * 70)
print("写 CC2500 寄存器 4/5 的调用点（WriteReg=0x080157EE）")
print("=" * 70)
insns = dis(V860, BASE, len(V860))
WR = 0x080157EE
for idx, i in enumerate(insns):
    if i.mnemonic == "bl" and i.op_str == "#0x%x" % WR:
        ctx = insns[max(0, idx - 4):idx + 1]
        print("  call 0x%08X: %s" % (i.address, " | ".join("%s %s" % (c.mnemonic, c.op_str) for c in ctx)))

show("V860 CC2500 初始化表函数 0x08015590..0x08015620", dis(V860, 0x08015590, 0x90))
