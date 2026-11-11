# -*- coding: utf-8 -*-
"""Godox X3-S（X3 S 引引闪器）V1.25 —— 私有密钥 v1 补丁静态证据 / 复核脚本。

用法（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\dev\\x3s\\analyze.py
    # 输出同时写入 keypatch/dev/x3s/analyze.log

内容：
  §1 镜像信息（长度 / sha256 / 向量表 / 基址）
  §2 原厂同步字函数 0x0801474C 反汇编
  §3 补丁版同一函数反汇编（关键：movw 立即数、除数）
  §4 set_sync 的全部 BL 调用点
  §5 全镜像写 CC2500 SYNC1/SYNC0（寄存器 4/5）的代码位置扫描
  §6 CC2500 初始化表（0x080231A4）里的 SYNC 初值 + 启动链调用证据
  §7 MOVW(T3) 编码推导 + 补丁前后差异
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
KEYPATCH_DIR = os.path.join(ROOT, "keypatch")
if KEYPATCH_DIR not in sys.path:
    sys.path.insert(0, KEYPATCH_DIR)

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_LITTLE_ENDIAN  # noqa: E402

BASE = 0x08004800
IMAGE = os.path.join(ROOT, "devices", "x3s", "firmware", "X3_S_v1.25_app.bin")
KEY = 0xD798

SYNC_FN = 0x0801474C
SET_CH = 0x080146FC
WRITE_REG = 0x08023428
WRITE_CMD = 0x08023408
DELAY = 0x0802319E
INIT_TABLE_FN = 0x080231A4
SYNC_CALLERS = (0x0801177A, 0x080118FC, 0x0801193A, 0x0801608A)
ID_NUMERATOR = 50024

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_LITTLE_ENDIAN)
_lines: list[str] = []


def out(msg: str = "") -> None:
    _lines.append(msg)
    print(msg)


def off(addr: int) -> int:
    return addr - BASE


def disasm(data: bytes, addr: int, nbytes: int):
    o = off(addr)
    return list(md.disasm(data[o:o + nbytes], addr))


def fmt(insns) -> None:
    for i in insns:
        out("  0x%08X  %-16s %-8s %s" % (i.address, i.bytes.hex(" "), i.mnemonic, i.op_str))


def movw(imm16: int, rd: int = 1) -> bytes:
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    hw1 = 0xF240 | (i << 10) | imm4
    hw2 = (imm3 << 12) | ((rd & 0xF) << 8) | imm8
    return struct.pack("<HH", hw1, hw2)


def bl_targets(data: bytes):
    hits = []
    for o in range(0, len(data) - 4, 2):
        hw1, hw2 = struct.unpack_from("<HH", data, o)
        if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) not in (0xD000, 0xC000):
            continue
        s = (hw1 >> 10) & 1
        imm10 = hw1 & 0x3FF
        j1 = (hw2 >> 13) & 1
        j2 = (hw2 >> 11) & 1
        i1 = 1 - (j1 ^ s)
        i2 = 1 - (j2 ^ s)
        imm = ((s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | ((hw2 & 0x7FF) << 1))
        if imm & (1 << 24):
            imm -= 1 << 25
        a = BASE + o
        hits.append((a, a + 4 + imm))
    return hits


def main() -> int:
    data = open(IMAGE, "rb").read()
    out("Godox X3-S V1.25 —— 私有密钥 v1 补丁静态证据")
    out("=" * 72)
    out("")

    # §1 -------------------------------------------------------------------
    out("§1 镜像信息")
    out("  文件      : %s" % os.path.relpath(IMAGE, ROOT))
    out("  长度      : %d 字节" % len(data))
    out("  sha256    : %s" % hashlib.sha256(data).hexdigest())
    words = struct.unpack_from("<8I", data, 0)
    out("  向量表    : SP=0x%08X Reset=0x%08X NMI=0x%08X HardFault=0x%08X"
        % (words[0], words[1], words[2], words[3]))
    out("  镜像基址  : 0x%08X（Reset 偏移 0x%05X，落在向量表之后）" % (BASE, words[1] - BASE))
    out("")

    # §2 -------------------------------------------------------------------
    out("§2 原厂同步字函数 %#010x（void set_sync(uint8_t id)）" % SYNC_FN)
    fmt(disasm(data, SYNC_FN, 0x42))
    out("")

    # §3 -------------------------------------------------------------------
    out("§3 补丁版同一函数（KEY=0x%04X）" % KEY)
    patched = bytearray(data)
    patched[off(SYNC_FN) + 2:off(SYNC_FN) + 6] = movw(KEY, rd=1)
    patched[off(SYNC_FN) + 6:off(SYNC_FN) + 8] = bytes.fromhex("0120")
    fmt(disasm(bytes(patched), SYNC_FN, 0x42))
    out("")

    # §4 -------------------------------------------------------------------
    out("§4 set_sync 的全部 BL 调用点（补丁后仍指向同一函数）")
    hits = bl_targets(data)
    found = [(a, t) for a, t in hits if t == SYNC_FN]
    for a, _t in found:
        out("  0x%08X -> 0x%08X" % (a, SYNC_FN))
    out("  合计 %d 处" % len(found))
    out("")

    # §5 -------------------------------------------------------------------
    out("§5 全镜像写 CC2500 SYNC1/SYNC0（寄存器 4/5）的位置")
    wr_callers = [a for a, t in hits if t == WRITE_REG]
    out("  WriteReg 调用点共 %d 处；其中写 reg=4/5 的：" % len(wr_callers))
    sync_sites = []
    for a in wr_callers:
        ctx = disasm(data, a - 12, 14)
        txt = " ; ".join("%s %s" % (i.mnemonic, i.op_str) for i in ctx)
        if "r0, #4" in txt or "r0, #5" in txt:
            sync_sites.append(a)
            out("    0x%08X  %s" % (a, txt))
    out("")
    out("  => 只有 set_sync(0x%08X/0x%08X) 与 CC2500 初始化表(0x080231D2/0x080231DA) 写 SYNC。"
        % (0x0801475C, 0x08014764))
    out("")

    # §6 -------------------------------------------------------------------
    out("§6 CC2500 初始化表 %#010x 与启动链" % INIT_TABLE_FN)
    fmt(disasm(data, 0x080231C6, 0x1A))
    out("  启动函数里对初始化表与 set_sync 包装函数的调用：")
    for a, t in hits:
        if 0x08006F40 <= a < 0x08007000 and t in (INIT_TABLE_FN, 0x08011884, WRITE_REG):
            out("    0x%08X -> 0x%08X" % (a, t))
    out("  说明：初始化表先写 SYNC1=0xC3/SYNC0=0x68（原厂 ID=0 的同步字），")
    out("        随后 0x08006FCA 调 0x08011884（内部 0x080118FC 调 set_sync 0x%08X），" % SYNC_FN)
    out("        在首次发射前用当前 ID 覆盖成 KEY。")
    out("")

    # §7 -------------------------------------------------------------------
    out("§7 MOVW(T3) 编码推导与差异")
    out("  原厂 movw r1,#0xC368 = %s" % data[off(SYNC_FN) + 2:off(SYNC_FN) + 6].hex(" "))
    out("  补丁 movw r1,#0x%04X = %s" % (KEY, movw(KEY, rd=1).hex(" ")))
    out("  除数字节: 原厂 %s (adds r0,r0,#1) -> 补丁 %s (movs r0,#1)"
        % (data[off(SYNC_FN) + 6:off(SYNC_FN) + 8].hex(" "), "01 20"))
    diffs = [i for i in range(len(data)) if data[i] != patched[i]]
    out("  差异字节 : %d 个 @ %s" % (len(diffs), [hex(x) for x in diffs]))
    out("  补丁后 sha256: %s" % hashlib.sha256(bytes(patched)).hexdigest())
    out("")
    out("结论：补丁把「sync = 50024/(ID+1)」改写成「sync = KEY/1 = KEY」，")
    out("      函数其余逻辑（WriteReg 4/5、SCAL strobe、延时）100% 保留；文件长度不变。")
    out("")

    # §8 -------------------------------------------------------------------
    out("§8 keybtn（默认）补丁证据：地址映射 + 注入代码反汇编 + 差异")
    sys.path.insert(0, KEYPATCH_DIR)
    import devices.x3s as x3s  # noqa: E402
    out("  地址映射（X3-C -> X3-S）：")
    for name, ca, sa in (
            ("set_sync", 0x08014864, x3s.SYNC_FN),
            ("set_channel", 0x08014814, 0x080146FC),
            ("WriteReg", 0x08023540, x3s.WRITE_REG),
            ("Strobe", 0x08023520, x3s.WRITE_CMD),
            ("delay", 0x080232B6, x3s.DELAY),
            ("按键扫描", 0x0800CC30, x3s.KEYSCAN_FN),
            ("读键", 0x08022EB4, x3s.READ_KEY),
            ("send_short", 0x080118B0, x3s.SEND_SHORT),
            ("send_property", 0x0801184C, x3s.SEND_PROP),
            ("低层发2B", 0x0801712C, x3s.LOW_SEND_SHORT),
            ("低层发4B", 0x0801708C, x3s.LOW_SEND_PROP),
    ):
        out("    %-14s %08X -> %08X  (%+d)" % (name, ca, sa, sa - ca))
    core_code, core_layout = x3s.build_injection(KEY)
    btn_code, btn_layout = x3s.build_btn_injection(KEY)
    out("  核心注入 0x%08X（%d B）：" % (x3s.INJ_BASE, len(core_code)))
    out("    入口 inj_short=0x%08X inj_prop=0x%08X tramp_short=0x%08X tramp_prop=0x%08X"
        % (core_layout["inj_short"], core_layout["inj_prop"],
           core_layout["tramp_short"], core_layout["tramp_prop"]))
    fmt(disasm(bytes(core_code), x3s.INJ_BASE, 0x60))
    out("  按键注入 0x%08X（%d B）：" % (x3s.BTN_INJ_BASE, len(btn_code)))
    out("    entry=0x%08X btn_update=0x%08X toggle=0x%08X"
        % (btn_layout["entry"], btn_layout["btn_update"], btn_layout["toggle"]))
    fmt(disasm(bytes(btn_code), x3s.BTN_INJ_BASE, 0x40))

    # 3 个入口补丁的 B.W 目标（把补丁字节写进副本后解码）
    def _decode_bw(buf, addr: int) -> int | None:
        oo = off(addr)
        hw1, hw2 = struct.unpack_from("<HH", buf, oo)
        if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0x9000:
            return None
        s = (hw1 >> 10) & 1
        imm10 = hw1 & 0x3FF
        j1 = (hw2 >> 13) & 1
        j2 = (hw2 >> 11) & 1
        i1 = 1 - (j1 ^ s)
        i2 = 1 - (j2 ^ s)
        imm = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | ((hw2 & 0x7FF) << 1)
        if imm & (1 << 24):
            imm -= 1 << 25
        return addr + 4 + imm

    tmp = bytearray(data)
    for addr, _b, after, _d in (x3s._core_patched_regions(KEY) + x3s._btn_patched_regions(KEY)):
        tmp[off(addr):off(addr) + len(after)] = after
    out("  入口补丁（补丁后 B.W 解码）：")
    for name, addr in (("send_short", x3s.SEND_SHORT),
                       ("send_property", x3s.SEND_PROP),
                       ("按键扫描", x3s.KEYSCAN_FN)):
        tgt = _decode_bw(bytes(tmp), addr)
        out("    %-14s @0x%08X  %s -> B.W 0x%08X"
            % (name, addr, data[off(addr):off(addr) + 4].hex(" "), tgt or 0))
    out("  结论：默认补丁 = 双发（send_short/send_property 入口 B.W + 0x080EE000 注入）")
    out("        + 机内开关（按键扫描入口 B.W + 0x080EE400 注入，长按 30 次扫描 KEY_ON ^= 1）。")

    log = os.path.join(HERE, "analyze.log")
    with open(log, "w", encoding="utf-8") as f:
        f.write("\n".join(_lines) + "\n")
    print("\n[log] %s" % os.path.relpath(log, ROOT))
    return 0


if __name__ == "__main__":
    sys.exit(main())
