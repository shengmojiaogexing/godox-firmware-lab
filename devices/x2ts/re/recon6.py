# -*- coding: utf-8 -*-
"""X2T-S 侦察 6：0x00 空洞引用检查（作为注入区候选） + 尾部数据区反汇编边界。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000
SIZE = len(DATA)

CANDS = [
    (0x0800D91C, 16), (0x0800D939, 20), (0x0800D95E, 16), (0x0800D97F, 16),
    (0x0800D9A0, 16), (0x0800D9C1, 16), (0x0800D9E2, 16), (0x0800DA02, 17),
    (0x0800DA24, 16), (0x0800DA45, 16), (0x0800DB2C, 16), (0x0800DB47, 22),
    (0x0800DB64, 26), (0x0800DB86, 25), (0x0800DBF2, 16), (0x0800DE20, 18),
    (0x0800E001, 27), (0x0800E026, 39), (0x0800E061, 30),
]

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True

print("=== 所有 32 位字面量/指针引用这些区间？ ===")
for start, ln in CANDS:
    refs = []
    for off in range(0, SIZE - 3, 4):
        v = struct.unpack_from("<I", DATA, off)[0]
        if start <= v < start + ln:
            refs.append((BASE + off, v))
    print("  0x%08X len=%2d refs=%s" % (start, ln, refs))

print()
print("=== 分支目标落入这些区间？ ===")
targets = {}
for off in range(0, SIZE - 4, 2):
    for ins in md.disasm(DATA[off:off + 4], BASE + off):
        if ins.mnemonic in ("b", "b.w", "bl", "bl.w", "beq", "bne", "bcs", "bcc",
                            "bhi", "bls", "bgt", "ble", "bge", "blt"):
            if ins.operands and ins.operands[0].type == 1:  # IMM
                targets.setdefault(ins.operands[0].imm, []).append(ins.address)
        break
for start, ln in CANDS:
    hits = [(t, srcs) for t, srcs in targets.items() if start <= t < start + ln]
    print("  0x%08X len=%2d branch_targets=%s" % (start, ln, hits[:6]))

print()
print("=== 每个候选前后的反汇编上下文 ===")
for start, ln in CANDS:
    lo = start - 8
    hi = start + ln + 8
    print("---- 0x%08X len=%d ----" % (start, ln))
    for ins in md.disasm(DATA[lo - BASE:hi - BASE], lo):
        flag = "  <== CAVE" if start <= ins.address < start + ln else ""
        print("    0x%08X  %-16s %s %s%s"
              % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str, flag))
