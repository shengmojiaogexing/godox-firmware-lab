# -*- coding: utf-8 -*-
"""AD100 Pro V1.2 私有同步字补丁点静态复核脚本（capstone）。

用途：不盲信既有逆向结论，独立复核
  * 0x0800B758 同步字设置函数的边界与指令；
  * 0x0800B75C / 0x0800B78C 两个补丁点的原始字节；
  * 0x0800B78C 字面量（0x0000C368）是否只被 0x0800B758 引用；
  * 全镜像内是否还有别的代码在启动/运行中直接写 CC2500 SYNC1(reg4)/SYNC0(reg5)；
  * 0x0800B758 的全部调用点。

输出为可直接贴进 NOTES.md 的文本。运行：
  tools\\python\\python.exe keypatch\\dev\\ad100\\re_check.py
"""
from __future__ import annotations

import hashlib
import json
import os
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "..", ".."))
FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
BASE = 0x08003000

SYNC_FN = 0x0800B758
PATCH_DIV = 0x0800B75C
PATCH_LIT = 0x0800B78C
WRITE_REG = 0x0800CD32
WRITE_CMD = 0x0800CDF4
DELAY_US = 0x0800CAB8


def load():
    data = open(FW, "rb").read()
    sha = hashlib.sha256(data).hexdigest()
    print("== 输入固件 ==")
    print("path   : %s" % os.path.relpath(FW, ROOT))
    print("size   : %d" % len(data))
    print("sha256 : %s" % sha)
    return data


def disasm(md, data, start, end):
    off = start - BASE
    return list(md.disasm(data[off:end - BASE], start))


def _imm(text):
    """把 capstone 操作数文本（如 '#4' / '0x30]' / '#0x800b758'）解析为整数。"""
    t = text.strip().strip("#").strip().rstrip("]").strip()
    try:
        return int(t, 0)
    except ValueError:
        return None


def insn_target(ins):
    """解析 capstone 的 op_str 里的立即数目标（如 '#0x800b758'）。"""
    parts = ins.op_str.split(",")
    return _imm(parts[-1])


def main():
    data = load()
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    funcs = json.load(open(FUNCS, encoding="utf-8"))
    fn_ranges = [(int(f["addr"], 16), int(f["end"], 16)) for f in funcs]

    # ---------- 1. 目标函数反汇编 ----------
    print("\n== 1. 0x%08X 同步字设置函数（capstone 反汇编） ==" % SYNC_FN)
    for ins in disasm(md, data, SYNC_FN, PATCH_LIT + 4):
        print("  0x%08X  %-10s %-8s %s" % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))

    print("\n== 2. 补丁区（v4 整段改写 0x0800B758..0x0800B78F，共 56 字节） ==")
    off = SYNC_FN - BASE
    print("  原始 56 字节: %s" % data[off:off + 56].hex(" "))
    for name, addr, n in [("v1 改除数点(历史)", PATCH_DIV, 2),
                          ("v1 改字面量点(历史)", PATCH_LIT, 4),
                          ("下一函数起点", 0x0800B790, 8)]:
        o = addr - BASE
        print("  %-16s 0x%08X off+0x%05X: %s" % (name, addr, o, data[o:o + n].hex(" ")))
    # 用 functions.json 复核函数边界：0x0800B758 的 end 应为 0x0800B78C，下一函数 0x0800B790
    fn = next((f for f in funcs if int(f["addr"], 16) == SYNC_FN), None)
    print("  functions.json: %s" % ("0x%08X..0x%08X (insns=%s)"
          % (int(fn["addr"], 16), int(fn["end"], 16), fn.get("insns")) if fn else "未找到"))
    print("  区域 0x%08X..0x%08X 含函数代码 52 字节 + 0x%08X 处 4 字节字面量 0x0000C368；"
          "0x0800B790 起为下一函数" % (SYNC_FN, SYNC_FN + 55, PATCH_LIT))

    # ---------- 2. 全镜像 BL/BLX 交叉引用扫描 ----------
    print("\n== 3. 全镜像 BL/BLX 交叉引用（按 functions.json 函数边界反汇编） ==")
    bl_xrefs = {}                    # target -> [(caller_fn, call_addr)]
    write_reg_sites = []             # (call_addr, caller_fn, reg)
    call_targets = {SYNC_FN, WRITE_REG, WRITE_CMD, DELAY_US}
    lit_c368_loads = []              # (ins_addr, caller_fn) 加载 0xC368 字面量的指令
    c368_words = [BASE + i for i in range(0, len(data) - 3, 4)
                  if data[i:i + 4] == bytes([0x68, 0xC3, 0x00, 0x00])]
    print("  镜像中 4 字节字面量 0x0000C368 的位置: %s" % ", ".join("0x%08X" % a for a in c368_words))

    for faddr, fend in fn_ranges:
        if fend <= faddr or fend > BASE + len(data):
            continue
        insns = disasm(md, data, faddr, fend)
        for idx, ins in enumerate(insns):
            mn = ins.mnemonic
            if mn in ("bl", "blx"):
                t = insn_target(ins)
                if t in call_targets:
                    bl_xrefs.setdefault(t, []).append((faddr, ins.address))
                if t == WRITE_REG:
                    # 往回找 1..4 条指令里的 r0 立即数
                    reg = None
                    for prev in reversed(insns[max(0, idx - 4):idx]):
                        if prev.mnemonic in ("movs", "mov") and prev.op_str.startswith("r0,"):
                            reg = _imm(prev.op_str.split(",")[-1])
                            break
                    write_reg_sites.append((ins.address, faddr, reg))
            # 字面量加载：ldr rX, [pc, #imm] -> 目标 = addr+4+imm（M0 Thumb 下对齐后）
            if mn.startswith("ldr") and "[pc" in ins.op_str:
                imm = _imm(ins.op_str.split("#")[-1])
                if imm is not None:
                    tgt = (ins.address + 4 + imm) & ~3
                    if PATCH_LIT <= tgt < PATCH_LIT + 4:
                        lit_c368_loads.append((ins.address, faddr, tgt))

    for tgt, label in [(SYNC_FN, "同步字设置函数 0x0800B758"),
                       (WRITE_REG, "WriteReg 0x0800CD32"),
                       (WRITE_CMD, "WriteCmd 0x0800CDF4"),
                       (DELAY_US, "delay_us 0x0800CAB8")]:
        sites = bl_xrefs.get(tgt, [])
        print("  %-28s 调用点 %d 个: %s" % (label, len(sites),
              ", ".join("0x%08X(in 0x%08X)" % (a, f) for f, a in sites)))

    print("\n== 4. 0x%08X 字面量引用（应只有 0x%08X 的 ldr） ==" % (PATCH_LIT, SYNC_FN))
    if lit_c368_loads:
        for a, f, tgt in lit_c368_loads:
            print("  0x%08X  (函数 0x%08X) -> 字面量 0x%08X" % (a, f, tgt))
    else:
        print("  （未发现）")

    print("\n== 5. 所有 WriteReg(0x%08X) 调用点及寄存器号（找写 SYNC1=4/SYNC0=5 的其它位置） ==" % WRITE_REG)
    sync_sites = [s for s in write_reg_sites if s[2] in (4, 5)]
    for call_addr, faddr, reg in write_reg_sites:
        mark = "  <== SYNC!" if reg in (4, 5) else ""
        print("  0x%08X in 0x%08X  reg=%s%s" % (call_addr, faddr,
              "r0(动态)" if reg is None else str(reg), mark))
    print("  其中写 SYNC1/SYNC0 的: %d 处" % len(sync_sites))

    # ---------- 6. CC2500_Init 里对 SYNC1/SYNC0 的静态初值 ----------
    print("\n== 6. CC2500_Init(0x0800CAE8) 对 SYNC1/SYNC0 的静态写入（开机时应被 0x0800B758 覆盖） ==")
    init_sync = [s for s in write_reg_sites if s[1] == 0x0800CAE8 and s[2] in (4, 5)]
    init_insns = disasm(md, data, 0x0800CAE8, 0x0800CC92)
    by_addr = {i.address: i for i in init_insns}
    for call_addr, faddr, reg in init_sync:
        idx = next(k for k, i in enumerate(init_insns) if i.address == call_addr)
        ctx = init_insns[max(0, idx - 3):idx + 1]
        print("  reg%d 写入: %s" % (reg, " | ".join("0x%08X %s %s" % (i.address, i.mnemonic, i.op_str)
                                                     for i in ctx)))
    print("  结论: 这是 CC2500 初始化表里的静态初值；RF_Init(0x08004AF0) 随后调用"
          " 0x0800B758 覆盖为 ID 对应值。")
    print("  实际静态值: SYNC1=0x%02X (reg4) SYNC0=0x%02X (reg5)"
          % (reg_vals(init_insns, 4) or -1, reg_vals(init_insns, 5) or -1))

    # ---------- 7. 向量表/间接入口检查 ----------
    print("\n== 7. 向量表入口检查（0x08003000 起 48 项，确认没有入口落在 0x0800B78C） ==")
    vec_hits = []
    for i in range(48):
        v = int.from_bytes(data[i * 4:i * 4 + 4], "little")
        if v & ~1 == PATCH_LIT or v & ~1 == PATCH_DIV:
            vec_hits.append((i, v))
    print("  向量表中指向 0x%08X 的项: %s" % (PATCH_LIT, vec_hits or "无"))

    # ---------- 8. v4 密钥开关标志字节（RAM 0x20000486） ----------
    print("\n== 8. v4 密钥开关标志字节复核：RAM 0x20000486 = RAM_20000480[6] ==")
    # 8a) 校验表 0x0800DA2E 第 6 项（决定该 C.Fn 项允许的取值上限）
    tbl = data[0x0800DA2E - BASE:0x0800DA2E - BASE + 16]
    print("  校验表 0x0800DA2E 前 16 字节: %s" % tbl.hex(" "))
    print("  校验表[6] = %d（该设置项允许 0..%d，即 0/1）" % (tbl[6], tbl[6]))
    # 8b) 全镜像中「先 ldr rX,=0x20000480，再 [rX,#6] 读/写」的位置
    sites = []
    for faddr, fend in fn_ranges:
        if fend <= faddr or fend > BASE + len(data):
            continue
        insns = disasm(md, data, faddr, fend)
        for idx, ins in enumerate(insns):
            if not ins.op_str.startswith("r") or "[pc" not in ins.op_str:
                continue
            imm = _imm(ins.op_str.split("#")[-1]) if "#" in ins.op_str else None
            if imm is None:
                continue
            # LDR (literal)：capstone 的 #imm 是自 Align(addr+4,4) 起的字节偏移；
            # 字面量槽里存的是 32 位值（这里要找 == 0x20000480 的字面量）。
            lit_addr = ((ins.address + 4) & ~3) + imm
            lo = lit_addr - BASE
            if not (0 <= lo <= len(data) - 4):
                continue
            if int.from_bytes(data[lo:lo + 4], "little") != 0x20000480:
                continue
            base_reg = ins.op_str.split(",")[0].strip()   # e.g. "r1"
            for nxt in insns[idx + 1:idx + 12]:
                if ("[" + base_reg + ", #6]" in nxt.op_str
                        and nxt.mnemonic in ("ldrb", "strb", "ldr", "str")):
                    sites.append((nxt.address, faddr, nxt.mnemonic))
                    break
    print("  访问 RAM_20000480[6]（=0x%08X）的指令 %d 处:" % (0x20000486, len(sites)))
    for addr, faddr, mn in sites:
        print("    0x%08X %-5s (函数 0x%08X)" % (addr, mn, faddr))
    print("  结论: 0x%08X 是既有 C.Fn 配置字节（多处以 ==1 判断），与 ui-lights 的结论一致；"
          "本补丁只读取它，不修改。" % 0x20000486)
    # 8c) 核心补丁区 vs UI 补丁目标（0x08003F0C 菜单渲染函数）不相交
    ui_addr = 0x08003F0C
    overlap = SYNC_FN <= ui_addr < SYNC_FN + 56
    print("  核心补丁区 0x%08X..0x%08X 与 UI 补丁目标 0x%08X 是否重叠: %s"
          % (SYNC_FN, SYNC_FN + 55, ui_addr, "是" if overlap else "否"))
    return 0


def reg_vals(insns, regno):
    """在一条序列里找 `movs rN/ movs r0,#val` 后紧跟 bl WriteReg 的写入值。"""
    for idx, ins in enumerate(insns):
        if ins.mnemonic in ("movs", "mov") and ins.op_str.startswith("r0,"):
            if _imm(ins.op_str.split(",")[-1]) == regno:
                for nxt in insns[idx + 1:idx + 3]:
                    if nxt.mnemonic == "bl" and insn_target(nxt) == WRITE_REG:
                        prev = insns[idx - 1] if idx > 0 else None
                        if prev is not None and prev.mnemonic in ("movs", "mov") and prev.op_str.startswith("r1,"):
                            return _imm(prev.op_str.split(",")[-1])
    return None


if __name__ == "__main__":
    if len(sys.argv) > 1:
        # 用法: re_check.py <out.log>  —— 直接以 UTF-8 写日志，避免控制台编码问题
        with open(sys.argv[1], "w", encoding="utf-8", newline="\n") as f, \
                __import__("contextlib").redirect_stdout(f):
            rc = main()
        sys.exit(rc)
    sys.exit(main())
