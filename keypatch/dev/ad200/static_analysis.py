# -*- coding: utf-8 -*-
"""静态证据：CC2500 SYNC1/SYNC0 (reg4/reg5) 的全部写入路径。

基于 devices/ad200pro/analysis/disasm.txt（CFG 递归反汇编产物）与原始二进制扫描。
用法：
    tools\\python\\python.exe keypatch\\dev\\ad200\\static_analysis.py
"""
import hashlib
import os
import re
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
BASE = 0x08003000
BIN = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")
DISASM = os.path.join(ROOT, "devices/ad200pro", "analysis", "disasm.txt")
SYNC_FUNC = 0x0800B9AC
CHIP_CONFIG = 0x0800C858
RF_INIT = 0x08004860
CHIP_RESET = 0x0800CA2C
WRITE_REG8 = 0x0800CAA2


def parse_disasm():
    """返回 (insns, func_at): insns=[(addr, mnemonic, op_str, func_addr)]。

    行格式示例：
      '080045f2: 07f0dbf9 bl #0x800b9ac             '   (4 字节指令)
      '0800c884: 0420     movs r0, #4               '   (2 字节指令)
      '080030c0:   ; 0x080030d0 = 0x200008b0 (ram)'    (字面量注释，跳过)
    """
    insns = []
    func_at = {}
    insn_re = re.compile(r"^([0-9a-f]{8}):\s+([0-9a-f]{4}(?:[0-9a-f]{4})?)\s+(\S.*)$")
    lines = open(DISASM, encoding="utf-8", errors="replace").read().splitlines()
    cur = None
    for ln in lines:
        m = re.match(r"; FUNC_([0-9a-f]{8}) ", ln)
        if m:
            cur = int(m.group(1), 16)
            continue
        m = insn_re.match(ln.rstrip())
        if not m:
            continue
        addr = int(m.group(1), 16)
        text = m.group(3).strip()
        tokens = text.split(None, 1)
        mn = tokens[0]
        op = tokens[1] if len(tokens) > 1 else ""
        func_at[addr] = cur
        insns.append((addr, mn, op, cur))
    return insns, func_at


def main():
    data = open(BIN, "rb").read()
    print("== AD200 Pro V1.60 静态证据 ==")
    print("镜像 %d 字节, sha256 %s" % (len(data), hashlib.sha256(data).hexdigest()))

    # 1) 补丁点字节与 50024 字面量唯一性
    print("\n[1] 补丁点原始字节")
    print("  0x0800B9B0 (file+0x%04X): %s   (adds r1, r0, #1)"
          % (0x0800B9B0 - BASE, data[0x0800B9B0 - BASE:0x0800B9B0 - BASE + 2].hex(" ")))
    lit = struct.pack("<I", 50024)
    print("  0x0800B9E0 (file+0x%04X): %s   (字面量 0x%08X = 50024)"
          % (0x0800B9E0 - BASE, data[0x0800B9E0 - BASE:0x0800B9E0 - BASE + 4].hex(" "), 50024))
    hits = [i for i in range(len(data) - 3) if data[i:i + 4] == lit]
    print("  全镜像中 50024 (LE 68 c3 00 00) 出现次数: %d, offset=%s"
          % (len(hits), [hex(i) for i in hits]))
    assert hits == [0x0800B9E0 - BASE], "50024 字面量不唯一！"

    insns, func_at = parse_disasm()
    print("  CFG 反汇编条目: %d 条指令, %d 个函数" %
          (len(insns), len({f for f in func_at.values() if f})))

    def calls_to(target):
        out = []
        for addr, mn, op, f in insns:
            if mn in ("bl", "b", "bne", "beq", "bhi", "blo", "bls", "bcc", "bcs"):
                m = re.search(r"#0x([0-9a-f]+)", op)
                if m and int(m.group(1), 16) == target:
                    out.append((addr, f))
        return sorted(out)

    # 2) 同步字设置函数的全部调用点
    print("\n[2] 调用 0x%08X（同步字设置函数）的全部位置：" % SYNC_FUNC)
    calls = calls_to(SYNC_FUNC)
    for a, f in calls:
        print("  0x%08X  in FUNC_0x%08X" % (a, f))
    assert len(calls) >= 4, "少于预期的 4 个调用点"
    print("  共 %d 处；补丁后无论谁来调用都写 KEY（与 ID 无关）。" % len(calls))

    # 3) rf_chip_config / rf_chip_reset(SRES) 的唯一调用者
    print("\n[3] 调用 rf_chip_config 0x%08X 的位置（唯一调用者应为 rf_init）：" % CHIP_CONFIG)
    cc = calls_to(CHIP_CONFIG)
    for a, f in cc:
        print("  0x%08X  in FUNC_0x%08X" % (a, f))
    assert cc == [(0x08004876, RF_INIT)], "rf_chip_config 有其它调用者！"

    print("\n[4] 调用 rf_chip_reset/SRES 0x%08X 的位置：" % CHIP_RESET)
    cr = calls_to(CHIP_RESET)
    for a, f in cr:
        print("  0x%08X  in FUNC_0x%08X" % (a, f))
    assert cr == [(0x08004872, RF_INIT)], "SRES 有其它调用者！"

    # 4) 直接写 reg4/reg5
    print("\n[5] 写 CC2500 reg4/reg5 的直接代码"
          "（movs r0,#4|#5 后紧跟 bl 0x%08X）：" % WRITE_REG8)
    write_sites = []
    for idx, (addr, mn, op, f) in enumerate(insns[:-1]):
        if mn == "movs" and re.match(r"r0,\s*#[45]$", op.strip()):
            reg = 4 if op.strip().endswith("4") else 5
            for j in range(idx + 1, min(idx + 3, len(insns))):
                _na, nm, nop, _nf = insns[j]
                m = re.search(r"#0x([0-9a-f]+)", nop)
                if nm == "bl" and m and int(m.group(1), 16) == WRITE_REG8:
                    write_sites.append((addr, reg, f))
                    break
    for addr, reg, f in write_sites:
        print("  0x%08X  reg=%d  in FUNC_0x%08X" % (addr, reg, f or 0))
    assert sorted((a, r) for a, r, f in write_sites) == [
        (0x0800B9BC, 4), (0x0800B9C4, 5), (0x0800C884, 4), (0x0800C88C, 5)], \
        "写 reg4/5 的位置与预期不符"
    print("  => 仅两处：rf_chip_config 写原厂 0xC3/0x68；同步字设置函数写公式值（已补丁）。")

    # 5) rf_init 内部顺序
    print("\n[6] rf_init 0x%08X 内部与同步字相关的指令：" % RF_INIT)
    targets = (CHIP_CONFIG, SYNC_FUNC, CHIP_RESET)
    for addr, mn, op, f in insns:
        if f != RF_INIT:
            continue
        m = re.search(r"#0x([0-9a-f]+)", op)
        tgt = int(m.group(1), 16) if m else None
        if (mn == "bl" and tgt in targets) or \
           (mn == "ldrb" and addr in (0x08004898, 0x080048C4)):
            print("  0x%08X  %-5s %s" % (addr, mn, op))

    # 6) v2：整段改写区域边界与原始 56 字节
    print("\n[7] v2 整段改写区域 0x%08X..0x%08X 与下一函数边界：" % (SYNC_FUNC, 0x0800B9E4))
    func_addrs = set()
    for ln in open(DISASM, encoding="utf-8", errors="replace"):
        m = re.match(r"; FUNC_([0-9a-f]{8}) ", ln)
        if m:
            func_addrs.add(int(m.group(1), 16))
    nxt = min(a for a in func_addrs if a > SYNC_FUNC)
    print("  disasm.txt 中大于 0x%08X 的最近函数起点: 0x%08X  (预期 0x0800B9E4)"
          % (SYNC_FUNC, nxt))
    assert nxt == 0x0800B9E4, "下一函数起点不是 0x0800B9E4！"
    sys.path.insert(0, os.path.join(ROOT, "keypatch"))
    from devices import ad200  # noqa: E402
    off = SYNC_FUNC - BASE
    print("  函数区间: 0x%08X..0x%08X（%d 字节，正好到下一函数）"
          % (ad200.SYNC_FUNC, ad200.SYNC_FUNC_END, ad200.SYNC_FUNC_SIZE))
    assert ad200.SYNC_FUNC_END == ad200.SYNC_FUNC + ad200.SYNC_FUNC_SIZE == nxt
    print("  原厂 56 字节: %s" % data[off:off + 56].hex(" "))
    assert data[off:off + 56] == ad200.ORIGINAL_FUNC, "原厂函数字节与模块常量不一致！"
    print("  == 模块 ORIGINAL_FUNC ✔（apply 逐字节校验用）")
    print("  该区域只被 [2] 的 4 个调用点按函数入口调用；v2 把整段替换成")
    print("  'ID==0->KEY，否则 50024/(ID+1)' 的 48B 代码 + KEY/50024 字面量。")

    print("\n结论：SYNC 只在 rf_chip_config（唯一调用者 rf_init，先配置）与")
    print("      0x0800B9AC（rf_init 末尾 0x080048C6 必调）写入；v2 补丁后：")
    print("      ID=0 -> SYNC=KEY（密钥模式），ID=1..99 -> 原厂 50024/(ID+1)（普通模式）。")
    print("\nPASS static_analysis (v2)")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except AssertionError as e:
        print("FAIL static_analysis:", e)
        sys.exit(1)
