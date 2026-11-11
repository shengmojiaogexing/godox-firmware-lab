# -*- coding: utf-8 -*-
"""V1S_V1.8.bin 初步侦察：基址/50024 字面量/同步字函数候选/CC2500 写寄存器。"""
import sys, struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "keypatch"))
try:
    from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
except Exception as e:
    print("capstone missing", e); raise

FW = ROOT / "devices" / "v1s" / "firmware" / "V1S_V1.8.bin"
data = FW.read_bytes()
BASE = 0x08003000
print("size", len(data), "end", hex(BASE + len(data)))
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)

def u32(off):
    return struct.unpack_from("<I", data, off)[0]

# 1) 50024 字面量
needle = struct.pack("<I", 50024)
hits = [i for i in range(len(data) - 4) if data[i:i + 4] == needle]
print("50024 literal hits:", [(hex(h), hex(BASE + h)) for h in hits])

# 2) 全镜像 BL 目标统计，找写寄存器函数调用（movs r0,#4 ; bl X / movs r0,#5 ; bl X）
cands = {}
for off in range(0, len(data) - 4, 2):
    ins = next(md.disasm(data[off:off + 2], BASE + off), None)
    if ins is None or ins.mnemonic != "bl":
        continue
    tgt = ins.operands[0].imm if ins.operands else 0
    prev = next(md.disasm(data[off - 2:off], BASE + off - 2), None)
    if prev is not None and prev.mnemonic == "movs" and prev.reg_name(prev.operands[0]) == "r0":
        imm = prev.operands[1].imm
        if imm in (4, 5, 6):
            cands.setdefault(tgt, {}).setdefault(imm, []).append(hex(BASE + off))

print("\nCandidates for CC2500_WriteReg (calls with r0=4/5/6):")
for tgt, d in sorted(cands.items()):
    if 4 in d and 5 in d:
        print("  fn 0x%08X  reg4=%d reg5=%d reg6=%d" % (tgt, len(d.get(4, [])), len(d.get(5, [])), len(d.get(6, []))))
        for r, sites in sorted(d.items()):
            print("    r0=%d at %s" % (r, sites))

# 3) 50024 附近反汇编
for h in hits:
    lo = max(0, h - 0x80); hi = min(len(data), h + 0x20)
    print("\n--- around literal @0x%08X ---" % (BASE + h))
    for ins in md.disasm(data[lo:hi], BASE + lo):
        print("  0x%08X: %-8s %s" % (ins.address, ins.mnemonic, ins.op_str))
