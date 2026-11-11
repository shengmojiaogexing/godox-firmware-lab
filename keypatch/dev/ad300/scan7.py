# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 —— 定位所有 settings 基址(0x2000048C) 的 ldr literal 引用点。"""
import struct
from pathlib import Path
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = Path(__file__).resolve().parents[3]
data = (ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin").read_bytes()
BASE = 0x08003000
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
TARGET_VAL = 0x2000048C

lit_addrs = set()
for i in range(0, len(data) - 3, 2):
    if data[i:i + 4] == struct.pack("<I", TARGET_VAL):
        lit_addrs.add(BASE + i)

print("literal addresses:", ["0x%08X" % a for a in sorted(lit_addrs)])


def find_ldr_ref(lit_addr):
    """向前找 Thumb LDR(literal) 指令，其目标 == lit_addr。"""
    hits = []
    for back in range(2, 0x400, 2):
        a = lit_addr - back
        if a < BASE:
            break
        off = a - BASE
        hw = struct.unpack_from("<H", data, off)[0]
        if (hw & 0xF800) == 0x4800:
            rt = (hw >> 8) & 7
            pc = (a + 4) & ~3
            tgt = pc + ((hw & 0xFF) * 4)
            if tgt == lit_addr:
                hits.append((a, rt))
    return hits


sites = []
for la in sorted(lit_addrs):
    for a, rt in find_ldr_ref(la):
        sites.append((a, rt, la))
        print("ldr ref @0x%08X  rt=r%d  -> literal 0x%08X" % (a, rt, la))

# 打印每个引用点前 0x50 / 后 0x20 的反汇编，便于看函数语义
def show(lo, n):
    print("\n--- 0x%08X ---" % lo)
    for ins in md.disasm(data[lo - BASE:lo - BASE + n], lo):
        print("0x%08X: %-8s %-8s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))


for a, rt, la in sites:
    show(a - 0x30, 0x50)
