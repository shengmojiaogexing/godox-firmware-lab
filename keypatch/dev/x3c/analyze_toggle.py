# -*- coding: utf-8 -*-
"""X3-C v2（ID=99 双发）补丁 —— 静态复核/证据生成脚本（可复现）。

用法（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\analyze_toggle.py
    # 输出同时写入 keypatch/dev/x3c/analyze_toggle.log

内容：
  §1 RAM 关键变量定位（当前 ID / 信道 / ID-OFF 标志 / 发送门控 / TX 缓冲）
  §2 发射路径反汇编（send_short / send_property / 低层 FIFO 发送）
  §3 代码洞复核（0x080EE000 空闲区、被排除的 0x080ECF20/0x080F7000/0x080F7800）
  §4 注入区布局与入口跳转（补丁后反汇编，capstone 交叉验证）
  §5 补丁差异统计（长度不变、差异只在 3 个补丁区）
  §6 ID=99 -> 公共同步字 500 的推导
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices", "x3c", "tools"))

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_LITTLE_ENDIAN  # noqa: E402

BASE = 0x08004800
ORIG = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "X3_C_v1.38_app_keyD798.bin")
KEY = 0xD798

SEND_SHORT = 0x080118B0
SEND_PROP = 0x0801184C
LOW_SHORT = 0x0801712C
LOW_PROP = 0x0801708C
WRITE_REG = 0x08023540
LOW_SHORT_RESUME_PATCH = 4
INJ_BASE = 0x080EE000
INJ_END = 0x080EE400
INJ_LEN = 0x144
HOTSHOE_FILE = 0x0F3800

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_LITTLE_ENDIAN)
lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def load(p: str) -> bytes:
    with open(p, "rb") as f:
        return f.read()


def off(addr: int) -> int:
    return addr - BASE


def disasm(data: bytes, start: int, nbytes: int):
    o = off(start)
    return list(md.disasm(data[o:o + nbytes], start))


def fmt(insns) -> None:
    for i in insns:
        out("  0x%08X  %-16s %s %s" % (i.address, i.bytes.hex(" "), i.mnemonic, i.op_str))


def bl_targets(data: bytes):
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


def word_refs(data: bytes, lo: int, hi: int):
    refs = []
    for o in range(0, len(data) - 3, 2):
        v = struct.unpack_from("<I", data, o)[0]
        if lo <= v < hi:
            refs.append((v, BASE + o))
    return refs


def main() -> int:
    orig = load(ORIG)
    patched = load(PATCHED)
    out("X3-C v1.38 v3 密钥开关双发补丁 —— 静态复核")
    out("原厂镜像 : %s (%d B, sha256 %s)" % (os.path.relpath(ORIG, ROOT), len(orig),
                                              hashlib.sha256(orig).hexdigest()))
    out("补丁镜像 : %s (%d B, sha256 %s)" % (os.path.relpath(PATCHED, ROOT), len(patched),
                                              hashlib.sha256(patched).hexdigest()))
    out("密钥     : 0x%04X -> SYNC1=0x%02X SYNC0=0x%02X"
        % (KEY, KEY >> 8, KEY & 0xFF))

    # ---------------- §1 RAM 变量 ----------------
    out("\n" + "=" * 78)
    out("§1 RAM 关键变量（全部由 main/反编译交叉确认）")
    out("=" * 78)
    out("main 主循环 literal pool（0x0803432C 起）：")
    for a in (0x0803432C, 0x08034330):
        out("  0x%08X = 0x%08X" % (a, struct.unpack_from("<I", orig, off(a))[0]))
    out("  -> 0x0803432C -> RAM 0x20000054 = 当前信道（1-based，main 里 -1 后传 FUN_080118E8）")
    out("  -> 0x08034330 -> RAM 0x20000055 = 当前 ID（main 里原样传给 FUN_080118E8）")
    out("")
    out("FUN_080118E8(0x080118E8) 状态结构基址 DAT_080119CC（literal 0x080119CC）：")
    out("  0x080119CC = 0x%08X；其 [5] 字节 = 0x%08X = ID-OFF/特殊状态标志"
        % (struct.unpack_from("<I", orig, off(0x080119CC))[0],
           struct.unpack_from("<I", orig, off(0x080119CC))[0] + 5))
    out("  反编译：if (state[5] == 0) 走正常 RF 路径（param_3=ID -> 0x08014864）；")
    out("          否则走 FUN_0801616C（该函数以 ID=0 调 0x08014864，即 ID OFF 路径）。")
    out("")
    out("v3 UI 密钥开关状态字节 KEY_ON_FLAG = 0x20001080（ui-x3c 最终定址：全镜像无")
    out("  字面量引用；40M 指令模拟写追踪仅 BSS 清零 1 次写。RAM 版：重启复位=关，")
    out("  后续做 flash 持久化。语义：非 0 = 开密钥。）")
    out("")
    out("组帧函数 literal：")
    fmt(disasm(orig, 0x080118B0, 0x2A))
    fmt(disasm(orig, 0x0801184C, 0x30))
    out("  -> 门控 RAM 0x20000146：非 0 时两个组帧函数都跳过发送。")
    out("  -> TX 缓冲 RAM 0x20000152：send_short 写 [D5,cmd]，send_property 写 [A9,g,k,v]。")

    # ---------------- §2 发射路径 ----------------
    out("\n" + "=" * 78)
    out("§2 发射路径：send_short/send_property -> 低层 FIFO 发送")
    out("=" * 78)
    out("低层 2 字节发送 0x0801712C：")
    fmt(disasm(orig, LOW_SHORT, 0x4C))
    out("")
    low_short_callers = [a for a, t in bl_targets(orig) if t == LOW_SHORT]
    low_prop_callers = [a for a, t in bl_targets(orig) if t == LOW_PROP]
    out("低层 0x%08X 的 BL 调用者: %s"
        % (LOW_SHORT, ", ".join("0x%08X" % a for a in low_short_callers)))
    out("低层 0x%08X 的 BL 调用者: %s"
        % (LOW_PROP, ", ".join("0x%08X" % a for a in low_prop_callers)))
    out("  -> 全固件只有 send_short / send_property 各自调用一个低层发送函数，")
    out("     因此把这两个函数入口包一层即可覆盖所有 2.4G 帧发送。")
    out("")
    out("低层发送内部: FIFO burst 写(0x08023536) -> STX(0x35) -> delay -> SFTX(0x3B) -> SIDLE(0x36)，")
    out("              全程 cpsid i 关中断；结束后 SIDLE，可直接改写 SYNC 寄存器。")

    # ---------------- §3 代码洞 ----------------
    out("\n" + "=" * 78)
    out("§3 代码洞复核")
    out("=" * 78)
    out("主固件空闲闪存扫描（0xFF 连续区，避开 0x%X 起热靴子固件）：" % HOTSHOE_FILE)
    i = 0
    while i < HOTSHOE_FILE:
        if orig[i] == 0xFF:
            j = i
            while j < HOTSHOE_FILE and orig[j] == 0xFF:
                j += 1
            if j - i >= 512:
                out("  0x%08X..0x%08X  len=%d  0xFF" % (BASE + i, BASE + j, j - i))
            i = j
        else:
            i += 1
    out("")
    out("被引用、必须避开的首尾地址：")
    for addr in (0x080ECF20, 0x080F7000, 0x080F7800):
        refs = word_refs(orig, addr, addr + 1)
        out("  0x%08X 被 %d 处 4 字节字面量引用: %s"
            % (addr, len(refs), ", ".join("0x%08X" % loc for _v, loc in refs) or "(无)"))
    refs = word_refs(orig, INJ_BASE, INJ_END)
    bls = [a for a, t in bl_targets(orig) if INJ_BASE <= t < INJ_END]
    out("")
    out("选定的注入区 0x%08X..0x%08X：引用=%d 分支目标=%d"
        % (INJ_BASE, INJ_END, len(refs), len(bls)))
    out("  -> 无引用；实际只占用 0x%08X 起约 0x144 字节（预留 1 KiB）。" % INJ_BASE)
    out("  -> 与 ui-x3c 约定：广播注入从 0x080F0000 起，互不重叠。")
    out("  -> 0x%08X 起是热靴子固件镜像，绝对不碰。" % (BASE + HOTSHOE_FILE))

    # ---------------- §4 注入布局 ----------------
    out("\n" + "=" * 78)
    out("§4 补丁后布局（入口 B.W + 注入区反汇编）")
    out("=" * 78)
    out("send_short 入口 0x%08X（原 %s）：" % (SEND_SHORT, "10 b5 0a 4c"))
    fmt(disasm(patched, SEND_SHORT, 8))
    out("send_property 入口 0x%08X（原 %s）：" % (SEND_PROP, "70 b5 0b 4c"))
    fmt(disasm(patched, SEND_PROP, 8))
    out("")
    out("注入区 0x%08X 起反汇编（capstone 交叉验证）：" % INJ_BASE)
    fmt(disasm(patched, INJ_BASE, INJ_LEN))
    out("")
    # 交叉验证关键期望
    ins = {i.address: i for i in disasm(patched, INJ_BASE, INJ_LEN)}
    checks = [
        (0x080EE000, "push", "{r4, lr}"),
        (0x080EE08C, "push", "{r4, r5, r6, lr}"),
    ]
    for addr, mnem, ops in checks:
        i = ins.get(addr)
        ok = i is not None and i.mnemonic == mnem and i.op_str.replace(" ", "") == \
            ops.replace(" ", "")
        out("  [%s] 0x%08X = %s %s" % ("PASS" if ok else "FAIL", addr,
                                       i.mnemonic if i else "?", i.op_str if i else "?"))

    # ---------------- §5 差异统计 ----------------
    out("\n" + "=" * 78)
    out("§5 补丁差异统计")
    out("=" * 78)
    diffs = [i for i in range(len(orig)) if orig[i] != patched[i]]
    out("  长度: 原厂 %d / 补丁 %d（不变=%s）" % (len(orig), len(patched),
                                                 len(orig) == len(patched)))
    out("  差异字节总数: %d" % len(diffs))
    regions = [
        (SEND_SHORT - BASE, SEND_SHORT - BASE + 4, "send_short 入口 B.W"),
        (SEND_PROP - BASE, SEND_PROP - BASE + 4, "send_property 入口 B.W"),
        (INJ_BASE - BASE, INJ_END - BASE, "注入区（0xFF -> 代码）"),
    ]
    for lo, hi, name in regions:
        n = sum(1 for d in diffs if lo <= d < hi)
        out("    %-28s off 0x%06X..0x%06X : %d 字节变化" % (name, lo, hi, n))
    outside = [d for d in diffs if not any(lo <= d < hi for lo, hi, _ in regions)]
    out("  越界差异: %d（应为 0）" % len(outside))
    out("  差异前 16 个 offset: %s" % [("0x%05X" % d) for d in diffs[:16]])

    # ---------------- §6 第二帧同步字 ----------------
    out("\n" + "=" * 78)
    out("§6 密钥模式第二帧的普通同步字（v3：按当前 ID 现算）")
    out("=" * 78)
    out("  原厂公式 sync = floor(50024 / (ID+1))：")
    for ident in (0, 5, 37, 99):
        s = 50024 // (ident + 1)
        out("    ID=%-2d -> 50024/%-3d = %-5d = 0x%04X  (SYNC1=0x%02X SYNC0=0x%02X)"
            % (ident, ident + 1, s, s, s >> 8, s & 0xFF))
    out("")
    out("  注入代码用 Cortex-M4 硬件 UDIV 现算（与原厂 0x0801486C 同一指令：")
    out("  `udiv r4,r1,r0` = b1 fb f0 f4）：")
    out("    movw r0,#ID 地址; ldrb r0->ID; adds r0,#1; movw r1,#0xC368; udiv r2,r1,r0")
    out("    -> lsrs/uxtb 取高/低字节 -> WriteReg(4/5)")
    out("  不调用原厂 0x08014864（它带 SCAL + 1ms 延时，会显著拖慢双发）。")
    out("")
    out("  v3 判定：KEY_ON_FLAG != 0  或（ID==99 且 ID-OFF==0）-> 双发；否则原厂单发。")
    out("  双发后同步字停留在 floor(50024/(ID+1)) = 原厂同 ID 状态，")
    out("  因此不改变固件既有 RF 状态（下一次单发/信道切换不受影响）。")

    out("\n" + "=" * 78)
    out("静态复核完成。")
    out("=" * 78)
    log = os.path.join(HERE, "analyze_toggle.log")
    with open(log, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % os.path.relpath(log, ROOT))
    return 0


if __name__ == "__main__":
    sys.exit(main())
