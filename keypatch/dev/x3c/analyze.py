# -*- coding: utf-8 -*-
"""X3-C 私有密钥补丁 —— 静态证据/复核脚本（可复现）。

用法（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\analyze.py
    # 输出同时写入 keypatch/dev/x3c/analyze.log

内容：
  §1 同步字设置函数 0x08014864 / 信道设置 0x08014814 反汇编（原厂）
  §2 Thumb-2 MOVW 编码推导（KEY=0x6B21）+ 补丁后反汇编对照
  §3 全镜像扫描：所有写 CC2500 寄存器的 BL 调用点，找出写 reg4/reg5 的位置
  §4 0x08014864 的全部调用点（二进制 BL 扫描 + Ghidra functions.csv）
  §5 CC2500 初始化表 0x080232BC（含 SYNC1=0xC3/SYNC0=0x68）与启动调用链
  §6 0xC368 字面量在镜像中的全部位置
"""
from __future__ import annotations

import csv
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_LITTLE_ENDIAN  # noqa: E402

BASE = 0x08004800
ORIG = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_key6B21.bin")
FUNCS_CSV = os.path.join(ROOT, "devices", "x3c", "re", "ghidra_exports", "main", "functions.csv")

SYNC_FN = 0x08014864
CH_FN = 0x08014814
WRITE_REG = 0x08023540
WRITE_CMD = 0x08023520
DELAY = 0x080232B6
INIT_TABLE_FN = 0x080232BC
KEY = 0x6B21

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_LITTLE_ENDIAN)
md.detail = False

_lines: list[str] = []


def out(msg: str = "") -> None:
    _lines.append(msg)
    print(msg)


def load(path: str) -> bytes:
    with open(path, "rb") as f:
        return f.read()


def off(addr: int) -> int:
    return addr - BASE


def disasm(data: bytes, start: int, count_or_bytes: int, by_bytes: bool = False):
    o = off(start)
    end = o + (count_or_bytes if by_bytes else count_or_bytes * 4)
    return list(md.disasm(data[o:end], start))


def fmt(insns) -> None:
    for i in insns:
        out("  0x%08X  %-16s %s %s" % (i.address, i.bytes.hex(" "), i.mnemonic, i.op_str))


def thumb_bl_targets(data: bytes):
    """线性扫描（每 2 字节）所有 Thumb BL/BLX 编码，返回 [(addr, target)]。"""
    hits = []
    for o in range(0, len(data) - 3, 2):
        hw1 = struct.unpack_from("<H", data, o)[0]
        hw2 = struct.unpack_from("<H", data, o + 2)[0]
        if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) not in (0xD000, 0xC000):
            continue
        s = (hw1 >> 10) & 1
        imm10 = hw1 & 0x3FF
        j1 = (hw2 >> 13) & 1
        j2 = (hw2 >> 11) & 1
        i1 = 1 - (j1 ^ s)
        i2 = 1 - (j2 ^ s)
        imm = ((s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12)
               | ((hw2 & 0x7FF) << 1))
        if imm & (1 << 24):
            imm -= 1 << 25
        addr = BASE + o
        hits.append((addr, addr + 4 + imm))
    return hits


def context_before(data: bytes, addr: int, nbytes: int = 24):
    """返回在 addr 之前、与 addr 连续衔接的指令序列（用于回溯 r0 来源）。"""
    start = addr - nbytes
    ins = [i for i in md.disasm(data[off(start):off(addr)], start)
           if i.address < addr]
    run = []
    cur = addr
    for i in reversed(ins):
        if i.address + i.size == cur:
            run.append(i)
            cur = i.address
        else:
            break
    run.reverse()
    return run


def last_r0_write(insns):
    for i in reversed(insns):
        op = i.op_str.replace(" ", "")
        if i.mnemonic in ("movs", "mov", "movw") and op.startswith("r0,"):
            return i
        if i.mnemonic in ("ldr", "ldrb", "ldrh", "ldrsb", "ldrsh", "adr",
                          "add", "adds", "subs", "and", "orrs", "eors",
                          "lsl", "lsls", "lsr", "lsrs", "muls") \
                and op.startswith("r0,"):
            return i
    return None


def func_ranges():
    """从 Ghidra functions.csv 读 (addr, size) 区间。"""
    outv = []
    with open(FUNCS_CSV, encoding="utf-8-sig", newline="") as f:
        for r in csv.DictReader(f):
            try:
                outv.append((int(r["addr"], 16), int(r["size"])))
            except (ValueError, KeyError):
                pass
    return outv


def in_any_func(ranges, addr):
    for a, s in ranges:
        if a <= addr < a + s:
            return a, s
    return None


def main() -> int:
    orig = load(ORIG)
    # v1（固定 KEY）历史脚本：v1 参考补丁在内存里重建（当前 keypatch/out 已是 v2 双发补丁）。
    # v2 双发补丁的静态证据见 analyze_toggle.py / analyze_toggle.log。
    buf = bytearray(orig)
    o = 0x08014866 - BASE
    buf[o:o + 4] = struct.pack("<HH", 0xF646, 0x3121)   # movw r1, #0x6B21
    o = 0x0801486A - BASE
    buf[o:o + 2] = bytes.fromhex("0120")                # movs r0, #1
    patched = bytes(buf)
    out("X3-C v1.38 **v1（固定 KEY）历史** 私有密钥补丁 —— 静态复核")
    out("原厂镜像 : %s  (%d 字节)" % (os.path.relpath(ORIG, ROOT), len(orig)))
    out("v1 参考补丁: 内存重建（6 字节：0x08014866 movw + 0x0801486A movs）")
    out("基址     : 0x%08X（文件偏移 = 地址 - 基址）" % BASE)

    # ---------------- §1 原厂反汇编 ----------------
    out("\n" + "=" * 78)
    out("§1 原厂反汇编：信道设置 0x08014814 与 同步字设置 0x08014864")
    out("=" * 78)
    fmt(disasm(orig, CH_FN, 19))

    # ---------------- §2 MOVW 编码 ----------------
    out("\n" + "=" * 78)
    out("§2 Thumb-2 MOVW 编码推导（KEY=0x%04X）与补丁前后对照" % KEY)
    out("=" * 78)
    imm4 = (KEY >> 12) & 0xF
    i = (KEY >> 11) & 1
    imm3 = (KEY >> 8) & 0x7
    imm8 = KEY & 0xFF
    hw1 = 0xF240 | (i << 10) | imm4
    hw2 = (imm3 << 12) | (1 << 8) | imm8
    out("MOVW T3 编码：hw1 = 0xF240 | (i<<10) | imm4，hw2 = (imm3<<12) | (Rd<<8) | imm8")
    out("  imm16=0x%04X -> imm4=0x%X  i=%d  imm3=0x%X  imm8=0x%02X  Rd=r1"
        % (KEY, imm4, i, imm3, imm8))
    out("  hw1=0x%04X -> 小端 %s      hw2=0x%04X -> 小端 %s"
        % (hw1, struct.pack("<H", hw1).hex(" "), hw2, struct.pack("<H", hw2).hex(" ")))
    out("  => movw r1, #0x%04X = %s"
        % (KEY, (struct.pack("<HH", hw1, hw2)).hex(" ")))
    out("原厂 movw 对照：imm16=0xC368 -> hw1=0x%04X hw2=0x%04X = 4c f2 68 31"
        % (0xF24C, 0x3168))
    out("")
    out("原厂 0x08014864 .. 0x080148A4：")
    fmt(disasm(orig, SYNC_FN, 17))
    if patched:
        out("")
        out("补丁后 0x08014864 .. 0x080148A4：")
        fmt(disasm(patched, SYNC_FN, 17))
        # 用 capstone 反向确认 MOVW 语义
        ins = {x.address: x for x in disasm(patched, SYNC_FN, 17)}
        mv = ins.get(0x08014866)
        dv = ins.get(0x0801486A)
        ok_movw = mv is not None and mv.mnemonic == "movw" and \
            mv.op_str.replace(" ", "") == "r1,#0x6b21"
        ok_div = dv is not None and dv.mnemonic == "movs" and \
            dv.op_str.replace(" ", "") == "r0,#1"
        out("  [%s] capstone 解码 0x08014866 = %s %s（期望 movw r1, #0x6b21）"
            % ("PASS" if ok_movw else "FAIL",
               mv.mnemonic if mv else "?", mv.op_str if mv else "?"))
        out("  [%s] capstone 解码 0x0801486A = %s %s（期望 movs r0, #1）"
            % ("PASS" if ok_div else "FAIL",
               dv.mnemonic if dv else "?", dv.op_str if dv else "?"))

    # ---------------- §3 写寄存器调用点 ----------------
    out("\n" + "=" * 78)
    out("§3 全镜像扫描：所有 BL 到写寄存器辅助函数 0x%08X 的调用点" % WRITE_REG)
    out("=" * 78)
    bls = [a for a, t in thumb_bl_targets(orig) if t == WRITE_REG]
    out("BL -> 0x%08X 调用点共 %d 处；逐个回溯 r0（寄存器号）/ r1（写入值）:"
        % (WRITE_REG, len(bls)))
    reg45 = []
    for addr in sorted(bls):
        ctx = context_before(orig, addr)
        r0w = last_r0_write(ctx)
        reg = None
        if r0w is not None and r0w.mnemonic in ("movs", "mov") and \
                r0w.op_str.replace(" ", "").startswith("r0,#"):
            try:
                reg = int(r0w.op_str.split("#")[1], 0)
            except ValueError:
                reg = None
        val = None
        if reg is not None and len(ctx) >= 2:
            prev = ctx[-2]
            if prev.mnemonic in ("movs", "mov") and \
                    prev.op_str.replace(" ", "").startswith("r1,#"):
                try:
                    val = int(prev.op_str.split("#")[1], 0)
                except ValueError:
                    val = None
        tail = " ".join("%s %s" % (x.mnemonic, x.op_str) for x in ctx[-3:])
        mark = ""
        if reg in (4, 5):
            reg45.append((addr, reg, val))
            mark = "   <== 写 SYNC%d" % (1 if reg == 4 else 0)
        out("  0x%08X  r0=%s r1=%s | %s%s"
            % (addr, ("%d" % reg) if reg is not None else "?",
               ("0x%02X" % val) if val is not None else "?",
               tail, mark))
    out("")
    out("结论：写 CC2500 reg4/reg5 的调用点共 %d 处：" % len(reg45))
    for addr, reg, val in reg45:
        out("  0x%08X  reg%d = 0x%02X" % (addr, reg, val if val is not None else -1))
    want_sites = {0x08014874, 0x0801487C, 0x080232EA, 0x080232F2}
    got_sites = {a for a, r, v in reg45}
    out("  [%s] 命中集合 == 期望集合（同步字函数 2 处 + 初始化表 0x080232BC 2 处）"
        % ("PASS" if got_sites == want_sites else "FAIL"))

    # 初始化表 0x080232BC 的 reg4/reg5 上下文
    out("")
    out("CC2500 初始化表 0x%08X 中写 reg4/reg5 的上下文：" % INIT_TABLE_FN)
    fmt(disasm(orig, 0x080232C6, 13))

    # 辅助函数确认（写寄存器/命令/延时入口）
    out("")
    out("辅助函数入口反汇编：")
    for name, addr in (("WriteReg", WRITE_REG), ("WriteCmd", WRITE_CMD),
                       ("delay", DELAY)):
        ins = disasm(orig, addr, 3)
        out("  %-8s 0x%08X: %s" % (name, addr,
                                   " ; ".join("%s %s" % (x.mnemonic, x.op_str)
                                              for x in ins)))

    # ---------------- §4 调用者 ----------------
    out("\n" + "=" * 78)
    out("§4 0x%08X（同步字设置函数）的全部调用点" % SYNC_FN)
    out("=" * 78)
    callers = [a for a, t in thumb_bl_targets(orig) if t == SYNC_FN]
    out("二进制 BL 扫描：")
    for a in callers:
        out("  BL @ 0x%08X -> 0x%08X" % (a, SYNC_FN))
    out("")
    out("Ghidra functions.csv 中的调用者：")
    with open(FUNCS_CSV, encoding="utf-8-sig", newline="") as f:
        for r in csv.DictReader(f):
            if "0x08014864" in (r.get("callees") or ""):
                out("  %s %s size=%s callers=[%s]"
                    % (r["addr"], r["name"], r["size"], r["callers"]))
    out("")
    out("0x0801616C 中 ID=0 调用点（ID OFF 也走同一函数）：")
    fmt(disasm(orig, 0x08016190, 10))

    # ---------------- §5 初始化表与启动链 ----------------
    out("\n" + "=" * 78)
    out("§5 CC2500 初始化表 0x%08X 与启动调用链" % INIT_TABLE_FN)
    out("=" * 78)
    out("0x080232BC 的函数头（前 6 条）：")
    fmt(disasm(orig, INIT_TABLE_FN, 6))
    out("")
    out("调用链（二进制 BL 扫描；Ghidra functions.csv 交叉核对）：")
    with open(FUNCS_CSV, encoding="utf-8-sig", newline="") as f:
        rows = list(csv.DictReader(f))
    all_bl = thumb_bl_targets(orig)
    for target, label in ((INIT_TABLE_FN, "初始化表"),
                          (0x08006F58, "RF 初始化包装"),
                          (0x080118E8, "RF 状态设置"),
                          (SYNC_FN, "同步字设置"),
                          (0x080119DC, "工厂 RF 测试模式")):
        cs = sorted(a for a, t in all_bl if t == target)
        gh = [r["addr"] for r in rows
              if ("0x%08x" % target) in (r.get("callees") or "").lower()]
        # 只保留落在已知函数起点内的调用者（用 Ghidra 函数区间归属）
        out("  %-16s 0x%08X  BL调用点=%s  Ghidra调用者函数=%s"
            % (label, target,
               ", ".join("0x%08X" % a for a in cs) or "(无)",
               ", ".join(gh) or "(无)"))
    out("")
    out("0x08006F58 尾部（初始化后必定调用 0x080118E8(1,0x14,0)）：")
    fmt(disasm(orig, 0x08006FAE, 14))
    out("")
    # 注：Ghidra functions.csv 里 main 的 size=1136（到 0x08034290 为止）偏小，
    # 反编译 C 显示主循环体位于 0x08034290..0x08034338 的"函数间隙"里，仍属 main。
    out("main(0x08033E20) 及其主循环体 [0x08033E20, 0x08034338) 内的调用点：")
    for tgt in (0x08006F58, 0x080118E8, 0x080119DC, SYNC_FN):
        addrs = [a for a, t in all_bl if t == tgt
                 and 0x08033E20 <= a < 0x08034338]
        out("  BL -> 0x%08X @ %s"
            % (tgt, ", ".join("0x%08X" % a for a in addrs) or "(不在 main 区间)"))
    out("")
    out("全镜像中所有 BL -> 0x080118E8 的调用点：")
    out("  " + ", ".join("0x%08X" % a for a, t in all_bl if t == 0x080118E8))
    out("全镜像中所有 BL -> 0x080119DC 的调用点：")
    out("  " + ", ".join("0x%08X" % a for a, t in all_bl if t == 0x080119DC))

    # ---------------- §6 0xC368 字面量 ----------------
    out("\n" + "=" * 78)
    out("§6 0xC368（50024）在镜像中的全部出现位置")
    out("=" * 78)
    ranges = func_ranges()
    pos = 0
    while True:
        p = orig.find(b"\x68\xc3", pos)
        if p < 0:
            break
        pos = p + 1
        va = BASE + p
        fr = in_any_func(ranges, va)
        out("  文件 0x%05X  vaddr 0x%08X  对齐=%s  上下文 %s  %s"
            % (p, va, "4字节字对齐" if p % 4 == 0 else "未对齐(奇/2字节)",
               orig[max(0, p - 4):p + 6].hex(" "),
               ("函数内 0x%08X+0x%X" % fr) if fr else "不在任何函数区间（数据区）"))
    out("")
    out("说明：0x68 0xc3 只是字节对；只有 4 字节对齐的 68 c3 00 00 才可能是 .word 0x0000C368 字面量。")
    wl = [o for o in range(0, len(orig) - 3, 4)
          if orig[o:o + 4] == b"\x68\xc3\x00\x00"]
    out("4 字节对齐的 `68 c3 00 00` 字面量：%s"
        % (", ".join("vaddr 0x%08X" % (BASE + o) for o in wl) or "(无)"))
    out("")
    for o in range(0, len(orig) - 3, 2):
        if struct.unpack_from("<H", orig, o)[0] == 0xF24C and \
                struct.unpack_from("<H", orig, o + 2)[0] == 0x3168:
            out("  MOVW r1,#0xC368 编码 @ vaddr 0x%08X：%s"
                "（同步字函数中的原公式被除数）" % (BASE + o, orig[o:o + 4].hex(" ")))
    out("")
    out("结论：镜像中不存在 4 字节对齐的 0x0000C368 字面量；50024 只以 MOVW 立即数")
    out("      形式出现在 0x08014866，0x26085/0x3147D/0xF8781 三处 `68 c3` 均为")
    out("      未对齐的巧合字节（数据/指令流内部），无 .word 字面量可被 LDR 引用。")

    out("\n" + "=" * 78)
    out("静态复核完成。")
    out("=" * 78)
    log = os.path.join(HERE, "analyze.log")
    with open(log, "w", encoding="utf-8") as f:
        f.write("\n".join(_lines) + "\n")
    print("\n[log] %s" % os.path.relpath(log, ROOT))
    return 0


if __name__ == "__main__":
    sys.exit(main())
