# -*- coding: utf-8 -*-
"""打印 v2 整段改写的同步字函数 0x0800B9AC..0x0800B9E4（56 字节）原厂/补丁反汇编对比。

用法：
    tools\\python\\python.exe keypatch\\dev\\ad200\\dis_patch_region.py
"""
import os

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
BASE = 0x08003000
ORIG = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "AD200pro_V1.60_key6B21.bin")
A0, A1 = 0x0800B9AC, 0x0800B9E4          # 56 字节；A1 = 下一函数起点
NEXT_FUNC = 0x0800B9E4


def disasm(data, a0, a1):
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    rows = []
    for i in md.disasm(data[a0 - BASE:a1 - BASE], a0):
        raw = data[i.address - BASE:i.address - BASE + i.size].hex(" ")
        rows.append((i.address, raw, i.mnemonic, i.op_str))
    return rows


def show(title, rows):
    print("\n---- %s ----" % title)
    for a, raw, mn, op in rows:
        print("  0x%08X  %-14s %-7s %s" % (a, raw, mn, op))


def main():
    orig = open(ORIG, "rb").read()
    pat = open(PATCHED, "rb").read()
    print("== v2 同步字函数整段改写 0x%08X..0x%08X（%d 字节，到下一函数 0x%08X）=="
          % (A0, A1, A1 - A0, NEXT_FUNC))
    print("原厂 : %s" % ORIG)
    print("补丁 : %s" % PATCHED)

    show("原厂 0x0800B9AC..0x0800B9E4（含末尾字面量 50024/代码尾部）", disasm(orig, A0, A1))
    show("补丁 0x0800B9AC..0x0800B9E4（v2：48B 代码 + KEY + 50024）", disasm(pat, A0, A1))

    # 逐字节差异
    diffs = [i for i in range(len(orig)) if orig[i] != pat[i]]
    print("\n全镜像差异偏移（%d 字节）: %s" % (len(diffs), [hex(x) for x in diffs]))
    print("差异范围: [0x%05X, 0x%05X)；补丁区域: [0x%05X, 0x%05X)"
          % (min(diffs), max(diffs) + 1, A0 - BASE, A1 - BASE))
    assert all(A0 - BASE <= x < A1 - BASE for x in diffs), "差异超出补丁区域"
    assert diffs, "没有任何差异？"

    off = A0 - BASE
    print("\n补丁区域前后字节：")
    print("  before[0x%04X:0x%04X] = %s" % (off - 0, off + 16, orig[off:off + 16].hex(" ")))
    print("  after [0x%04X:0x%04X] = %s" % (off - 0, off + 16, pat[off:off + 16].hex(" ")))
    print("  ...")
    print("  before[0x%04X:0x%04X] = %s" % (off + 0x30, off + 0x38,
                                            orig[off + 0x30:off + 0x38].hex(" ")))
    print("  after [0x%04X:0x%04X] = %s" % (off + 0x30, off + 0x38,
                                            pat[off + 0x30:off + 0x38].hex(" ")))

    # 下一函数边界保持不变
    tail_o = orig[A1 - BASE:A1 - BASE + 16]
    tail_p = pat[A1 - BASE:A1 - BASE + 16]
    print("\n下一函数 0x%08X 起的 16 字节：原厂 %s / 补丁 %s -> %s"
          % (NEXT_FUNC, tail_o.hex(" "), tail_p.hex(" "),
             "一致" if tail_o == tail_p else "不一致!"))
    assert tail_o == tail_p, "越界改写了下一函数！"

    # 语义说明
    print("\n补丁后语义：")
    print("  ID=0      -> sync = KEY (0x6B21)      -> WriteReg(4,0x6B) WriteReg(5,0x21) delay_us(20)")
    print("  ID=1..99  -> sync = 50024/(ID+1)      -> 同原厂公式（普通模式）")
    print("  不再执行 SCAL(0x33) + delay(1000us)")
    print("\nPASS dis_patch_region (v2)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
