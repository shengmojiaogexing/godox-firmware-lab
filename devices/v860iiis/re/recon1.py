# -*- coding: utf-8 -*-
"""V860IIIS_V1.1.bin 初勘：向量表 / base / 架构 / 50024 字面量 / 同步字函数候选。

只读固件，输出到 stdout（可重定向到 log_recon1.txt）。
用法: tools\\python\\python.exe devices\\v860iiis\\re\\recon1.py
"""
import struct
import sys
import os

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
IMAGE = os.path.join(ROOT, "devices", "v860iiis", "firmware", "V860IIIS_V1.1.bin")

sys.path.insert(0, os.path.join(ROOT, "keypatch"))
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_MODE_LITTLE_ENDIAN  # noqa: E402


def main():
    data = open(IMAGE, "rb").read()
    n = len(data)
    print("image: %s" % IMAGE)
    print("size : %d (0x%X)" % (n, n))

    # ---- 1. 向量表 ----
    print("\n== 1. 向量表（文件偏移 0 起的 16 个字，按 app base 0x08003000 解释） ==")
    base = 0x08003000
    for i in range(16):
        w = struct.unpack_from("<I", data, i * 4)[0]
        print("  v[%2d] = 0x%08X  (off 0x%03X)" % (i, w, i * 4))
    print("  -> 初值 SP=0x%08X  Reset=0x%08X (Thumb 位=%d)"
          % (struct.unpack_from("<I", data, 0)[0],
             struct.unpack_from("<I", data, 4)[0],
             struct.unpack_from("<I", data, 4)[0] & 1))
    print("  -> 若 base=0x08003000，镜像末端 = 0x%08X" % (base + n))

    # 向量表非零项是否全部落在 [base, base+n)
    bad = []
    for i in range(16):
        w = struct.unpack_from("<I", data, i * 4)[0]
        if i == 0:
            continue
        a = w & ~1
        if w and not (base <= a < base + n):
            bad.append((i, w))
    print("  -> 落在 [0x%08X,0x%08X) 之外的向量: %s" % (base, base + n, bad or "无"))

    # ---- 2. 架构初判 ----
    print("\n== 2. 架构初判：用 M0(Thumb-1) 反汇编一段 reset handler，统计 32 位指令 ==")
    md0 = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS | CS_MODE_LITTLE_ENDIAN)
    reset = base + 0xD4
    off = reset - base
    insns = list(md0.disasm(data[off:off + 0x120], reset, count=80))
    wide = [x for x in insns if x.size == 4]
    print("  reset=0x%08X 反汇编 %d 条，其中 4 字节指令 %d 条" % (reset, len(insns), len(wide)))
    for ins in insns[:40]:
        print("    0x%08X  %-10s %s" % (ins.address, ins.mnemonic, ins.op_str))
    if wide:
        print("  !! 出现 32 位指令（M0 不支持 movw/movt/ldrd 等，说明可能是 M3/M4 或数据被误当指令）")
        for ins in wide[:10]:
            print("    0x%08X  %-10s %s" % (ins.address, ins.mnemonic, ins.op_str))
    else:
        print("  -> 该段全部 16 位，符合 Cortex-M0 / Thumb-1 特征")

    # ---- 3. 50024 字面量 ----
    needle = struct.pack("<I", 50024)
    hits = []
    start = 0
    while True:
        i = data.find(needle, start)
        if i < 0:
            break
        hits.append(i)
        start = i + 1
    print("\n== 3. 50024(0x%04X) 小端字面量命中: %s =="
          % (50024, [("off 0x%05X / va 0x%08X" % (h, base + h)) for h in hits]))

    # ---- 4. 找引用 50024 的 LDR(literal) ----
    print("\n== 4. 扫描所有 LDR(literal)（含 32 位形式）指向 50024 的指令 ==")
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS | CS_MODE_LITTLE_ENDIAN)
    md.detail = False
    # 逐 2 字节扫描全镜像：解码成功的 LDR(literal) 指令
    refs = []
    for i in range(0, n - 4, 2):
        addr = base + i
        for ins in md.disasm(data[i:i + 4], addr, count=1):
            if ins.mnemonic in ("ldr", "ldr.w", "ldr.w ") or ins.mnemonic == "ldr":
                pass
        # 16-bit thumb ldr: 01001 Rt imm8
        hw = struct.unpack_from("<H", data, i)[0]
        if (hw & 0xF800) == 0x4800:
            rt = (hw >> 8) & 7
            imm8 = hw & 0xFF
            pc = (addr + 4) & ~3
            target = pc + imm8 * 4
            if target in [(base + h) for h in hits]:
                refs.append((addr, rt, target, 2))
        # 32-bit thumb2 ldr.w: 1111 1000 1101 ... (T2) 或 1111 1000 0101 ... (T1)
        if (hw & 0xFF7F) in (0xF85F,):  # ldr.w Rt,[pc,#imm12]
            hw2 = struct.unpack_from("<H", data, i + 2)[0]
            rt = (hw >> 8) & 0xF
            imm12 = hw2 & 0xFFF
            pc = (addr + 4) & ~3
            target = pc + imm12
            if target in [(base + h) for h in hits]:
                refs.append((addr, rt, target, 4))
    for addr, rt, target, size in refs:
        print("  LDR ref @0x%08X -> [%s] r%d (imm=%d)  指令=%s"
              % (addr, ("0x%08X" % target), rt, target - ((addr + 4) & ~3),
                 data[addr - base:addr - base + size].hex(" ")))

    # ---- 5. 反汇编字面量前 0x100 ~ 后 0x20 ----
    for h in hits:
        print("\n== 5. 50024 字面量附近（0x%08X 前 0x100 字节）反汇编 ==" % (base + h))
        lo = max(0, h - 0x100)
        hi = min(n, h + 8)
        for ins in md.disasm(data[lo:hi], base + lo):
            mark = "  <== literal" if ins.address == base + h else ""
            print("    0x%08X: %-10s %-30s%s" % (ins.address, ins.mnemonic, ins.op_str, mark))


if __name__ == "__main__":
    main()
