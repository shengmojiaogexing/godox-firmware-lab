# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 检查所有 CC2500_WriteReg 调用点的寄存器号（是否只有 4/5 被写）。"""
from pathlib import Path
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
WRITE_REG = 0x0800C47A


def decode_bl(hw1, hw2):
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0xD000:
        return None
    s = (hw1 >> 10) & 1
    i1 = (hw2 >> 13) & 1
    i2 = (hw2 >> 11) & 1
    j1 = i1 if s else 1 - i1
    j2 = i2 if s else 1 - i2
    off = (s << 24) | (j1 << 23) | (j2 << 22) | ((hw1 & 0x3FF) << 12) | ((hw2 & 0x7FF) << 1)
    if s:
        off -= 1 << 25
    return off


hits = []
for i in range(0, len(data) - 3, 2):
    hw1, hw2 = struct.unpack_from("<HH", data, i)
    o = decode_bl(hw1, hw2)
    if o is not None and BASE + i + 4 + o == WRITE_REG:
        hits.append(BASE + i)

print("WriteReg 调用点 %d 个：" % len(hits))
reg4_5 = []
for site in hits:
    # 反汇编前 0x10 字节，找最后一条 movs r0,#imm
    pre = data[site - BASE - 0x10:site - BASE]
    reg = None
    for ins in md.disasm(pre, site - 0x10):
        if ins.mnemonic == "movs" and ins.op_str.startswith("r0, #"):
            reg = int(ins.op_str.split("#")[1], 0)
        elif ins.mnemonic in ("ldr", "adds", "mov") and ins.op_str.startswith("r0"):
            reg = None
    tag = ""
    if reg == 4:
        reg4_5.append(site)
        tag = "  <== 写 SYNC1(reg4)"
    elif reg == 5:
        reg4_5.append(site)
        tag = "  <== 写 SYNC0(reg5)"
    print("  0x%08X  r0=%s%s" % (site, reg, tag))

print("\n写寄存器 4/5 的调用点：", ["0x%08X" % s for s in reg4_5])
