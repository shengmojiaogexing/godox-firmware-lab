# -*- coding: utf-8 -*-
"""X3 Pro（X3pro_C_v1.22_app.bin）私有密钥补丁 —— 独立验证脚本。

用途
----
对 keypatch/devices/x3pro.py 生成的补丁成品做**独立**复核：不 import
keypatch.devices.x3pro、不引用其 build_injection()/verify() 的任何输出，
全部从固件字节 + capstone 反汇编 + Unicorn 调用级执行重新推导。

覆盖的独立事实
--------------
1. 镜像基址 0x08020000 的推导证据（向量表 / Reset_Handler / SystemInit VTOR 写入）。
2. 同步字函数 0x0802F040 的函数边界、原厂公式 movw/add/udiv 语义、
   reg4/reg5 写入点和 SCAL/SIDLE/延时序列。
3. 全镜像写 CC2500 reg4/reg5 的唯一性扫描（含 write_reg 调用点 r0 常量回溯）。
4. 设置结构 0x20001768：[+1]=ID；ID 菜单钳位 99 的代码与 Unicorn 片段执行。
5. 两个发送入口 0x0802EDC4 / 0x0802EE2C 的原厂函数体与静态引用点统计。
6. 成品 vs 原厂逐字节差异（必须恰在 3 个补丁点 + 注入区代码）。
7. 注入区 0x081C0000 起代码的完整反汇编核对（分支目标 / 常量 / ID 分支 /
   蹦床顺序 / 栈平衡），以及 0x081C0000 段在原厂镜像中无字面量/分支引用。
8. Unicorn 调用级语义：set_sync(ID) 多组取值、TX4/TX2 在 ID=99 时
   SYNC=KEY -> FIFO -> SYNC=0x01F4 -> FIFO -> SYNC=KEY 双发、ID!=99 原厂单发，
   并做「原厂镜像自证」逐事件对比。

运行
----
    tools\\python\\python.exe keypatch\\dev\\x3pro\\verify_x3pro.py
    tools\\python\\python.exe keypatch\\dev\\x3pro\\verify_x3pro.py --image %TEMP%\\x3pro_indep\\X3pro_C_v1.22_app_keyD798.bin

退出码 0 = 全部 PASS，1 = 存在 FAIL，2 = 运行错误。
"""
from __future__ import annotations

import argparse
import hashlib
import os
import struct
import sys
import traceback

from capstone import (Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS,
                      CS_OP_IMM, CS_OP_MEM, CS_OP_REG)
from capstone.arm import ARM_REG_PC, ARM_REG_LR, ARM_REG_SP, ARM_REG_R0
from capstone.arm import ARM_REG_R1, ARM_REG_R2, ARM_REG_R3, ARM_REG_R4
from capstone.arm import ARM_REG_R5, ARM_REG_R6, ARM_REG_R7, ARM_REG_R8
from capstone.arm import ARM_REG_R9, ARM_REG_R10, ARM_REG_R11, ARM_REG_R12

# ---------------------------------------------------------------------------
# 路径 / 期望值（期望值来自任务书；验证脚本核对它们，而不是拿来当证据）
# ---------------------------------------------------------------------------
_HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(_HERE, "..", "..", ".."))

ORIG_REL = "devices/x3pro/firmware/X3pro_C_v1.22_app.bin"
PATCH_REL = "keypatch/out/X3pro_C_v1.22_app_keyD798.bin"

BASE = 0x08020000
EXPECT_SIZE = 1_941_020
EXPECT_ORIG_SHA = "2e2b8a3b5ff70e62c4acddc406b055ea2625e4833a213e81863f96c0da5a911b"
# Lead 2026-11-11 重设计的注入布局（同步字在 0x0802EC64 射频唤醒之后写入）
EXPECT_PATCH_SHA = "023ec3fefd2bd3a0dbe5b49bf4be962675d6d9a95664cd73515d64cce8a48243"

KEY = 0xD798
DEFAULT_ID = 99                       # 密钥模式 ID
ORDINARY_WORD = 50024 // (DEFAULT_ID + 1)   # = 500 = 0x01F4
SYNC_NUMERATOR = 0xC368

# 任务书中给出的地址（脚本独立反汇编后核对/修正）
SYNC_FN = 0x0802F040
SYNC_PATCH_ADDR = 0x0802F046
SYNC_CONT = 0x0802F050
SYNC_FN_LEN = 0x5A
SYNC_NEXT = 0x0802F09C
TX4 = 0x0802EDC4
TX2 = 0x0802EE2C
TX4_RESUME = 0x0802EDCA
TX2_RESUME = 0x0802EE32
WRITE_REG = 0x0802E71C
STROBE = 0x0802E88C
DELAY_US = 0x0802B5DC
RADIO_WAKE = 0x0802EC64
GPIO_WRITE = 0x08032A64
GPIO_READ = 0x08032A58
SETTINGS = 0x20001768
ID_FLAG = 0x20000F9C                  # ID 步进(1/10)标志字节（钳位片段读取）
BUSY_FLAG = 0x20001D09
INJ_BASE = 0x081C0000
INJ_WINDOW = 0x400
INJ_CODE_LEN = 232                    # 本成品注入代码 0xE8 字节（脚本反汇编核对）
FREE_LO, FREE_HI = 0x081B0A30, 0x081F4000
SENTINEL = 0x20006000
STACK = 0x20007000
UC_FLASH = 0x08000000
UC_FLASH_SIZE = 0x00200000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x00008000

# 原厂字节（从原厂 bin 读出后核对，不是从补丁模块抄的常量）
TRAMPOLINE_LOST = bytes.fromhex("10b5fff7")          # push {r4,lr}; bl ...
SYNC_ORIG_BYTES = bytes.fromhex("4cf268341344b4fbf3f4")

md_lite = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True

REL_REGS = {ARM_REG_R0: "r0", ARM_REG_R1: "r1", ARM_REG_R2: "r2",
            ARM_REG_R3: "r3", ARM_REG_R4: "r4", ARM_REG_R5: "r5",
            ARM_REG_R6: "r6", ARM_REG_R7: "r7", ARM_REG_R8: "r8",
            ARM_REG_R9: "r9", ARM_REG_R10: "r10", ARM_REG_R11: "r11",
            ARM_REG_R12: "r12", ARM_REG_LR: "lr", ARM_REG_PC: "pc",
            ARM_REG_SP: "sp"}


# ---------------------------------------------------------------------------
# 基础工具
# ---------------------------------------------------------------------------
def sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def load(path: str) -> bytes:
    with open(path, "rb") as f:
        return f.read()


def off(addr: int) -> int:
    return addr - BASE


def word(data: bytes, addr: int) -> int:
    o = off(addr)
    return struct.unpack("<I", data[o:o + 4])[0]


def dis_range(data: bytes, addr: int, size: int) -> list:
    o = off(addr)
    return list(md.disasm(data[o:o + size], addr))


def dis_one(data: bytes, addr: int):
    ins = dis_range(data, addr, 4)
    return ins[0] if ins and ins[0].address == addr else None


def ins_text(ins) -> str:
    return ("%s %s" % (ins.mnemonic, ins.op_str)).strip()


def branch_imm(ins):
    """取分支指令的立即数目标；不是分支或没有 #imm 时返回 None。"""
    if not ins.mnemonic.startswith("b"):
        return None
    for op in ins.operands:
        if op.type == CS_OP_IMM:
            return op.imm
    return None


def iter_branches(data: bytes):
    """扫描整幅镜像，返回所有分支指令 [(src, mnemonic, target)]。

    逐 2 字节偏移尝试解码（统计入口引用/引用窗口；原厂数据区可能产生少量
    伪分支，只用于“是否存在引用”的保守判断，不用于代码路径推导）。
    """
    out = []
    n = len(data)
    for i in range(0, n - 4, 2):
        addr = BASE + i
        for a, _sz, mn, ops in md_lite.disasm_lite(data[i:i + 4], addr, 1):
            if a == addr and mn.startswith("b") and ops.startswith("#"):
                try:
                    tgt = int(ops[1:], 16)
                except ValueError:
                    break
                out.append((addr, mn, tgt))
            break
    return out


def iter_word_refs(data: bytes, lo: int, hi: int):
    """返回所有 4 字节窗口（任意偏移）取值落在 [lo,hi) 的位置（含 Thumb 位）。"""
    out = []
    n = len(data)
    for i in range(0, n - 3):
        v = struct.unpack("<I", data[i:i + 4])[0]
        if lo <= (v & ~1) < hi:
            out.append((BASE + i, v))
    return out


def resolve_literal(data: bytes, ins):
    """解析 ldr rX,[pc,#imm] 的字面量地址与内容；不适用返回 (None,None)。"""
    if not ins.mnemonic.startswith("ldr") or len(ins.operands) < 2:
        return None, None
    m = ins.operands[1]
    if m.type != CS_OP_MEM or m.mem.base != ARM_REG_PC:
        return None, None
    target = ((ins.address + 4) & ~3) + m.mem.disp
    if not (BASE <= target <= BASE + len(data) - 4):
        return target, None
    return target, word(data, target)


UNK = object()


def trace_r0(data: bytes, call_addr: int, back: int = 0x90):
    """从 call_addr 向前做简单的寄存器常量传播，返回 (r0值或UNK, 是否被bl打断)。

    选择能连续解码到 call_addr 的最长回看窗口（避免从 0xFF 填充区或数据区
    起始导致 capstone 中途停住）。
    """
    seq = []
    for b in (0x400, 0x200, 0x100, back, 0x60, 0x40, 0x20, 0x10, 0x8):
        start = call_addr - b
        if start < BASE:
            continue
        cand = dis_range(data, start, b)
        if cand and cand[-1].address + len(cand[-1].bytes) == call_addr:
            seq = cand
            break
    if not seq:
        return UNK, False
    regs: dict[int, object] = {}
    clobbered = False
    for ins in seq:
        if ins.address >= call_addr:
            break
        ops = ins.operands
        mn = ins.mnemonic
        if not ops:
            continue
        d0 = ops[0]
        # 目的寄存器写
        if mn == "movw" and len(ops) == 2 and d0.type == CS_OP_REG and ops[1].type == CS_OP_IMM:
            regs[d0.reg] = ops[1].imm
        elif mn == "movt" and len(ops) == 2 and d0.type == CS_OP_REG and ops[1].type == CS_OP_IMM:
            v = regs.get(d0.reg, UNK)
            regs[d0.reg] = ((v & 0xFFFF) | (ops[1].imm << 16)) if isinstance(v, int) else UNK
        elif mn in ("movs", "mov", "mov.w", "movs.w") and len(ops) == 2 and d0.type == CS_OP_REG:
            if ops[1].type == CS_OP_IMM:
                regs[d0.reg] = ops[1].imm
            elif ops[1].type == CS_OP_REG:
                regs[d0.reg] = regs.get(ops[1].reg, UNK)
            else:
                regs[d0.reg] = UNK
        elif mn in ("adds", "add", "add.w", "subs", "sub", "sub.w") and len(ops) == 3 \
                and d0.type == CS_OP_REG and ops[1].type == CS_OP_REG and ops[2].type == CS_OP_IMM:
            v = regs.get(ops[1].reg, UNK)
            regs[d0.reg] = (v + ops[2].imm) if isinstance(v, int) else UNK
        elif mn.startswith("bfi") or mn in ("lsls", "lsrs", "ands", "orrs", "eors"):
            if d0.type == CS_OP_REG:
                regs[d0.reg] = UNK
        elif mn.startswith("ldr") or mn.startswith("ldrb") or mn.startswith("ldrh"):
            if d0.type == CS_OP_REG:
                regs[d0.reg] = UNK
        elif mn.startswith("bl") or mn == "blx":
            for r in (ARM_REG_R0, ARM_REG_R1, ARM_REG_R2, ARM_REG_R3,
                      ARM_REG_R12, ARM_REG_LR):
                regs[r] = UNK
            clobbered = True
        elif d0.type == CS_OP_REG and not (mn.startswith("str") or mn.startswith("stm")
                                           or mn in ("cmp", "tst", "cmn")):
            regs[d0.reg] = UNK
    return regs.get(ARM_REG_R0, UNK), clobbered


# ---------------------------------------------------------------------------
# Unicorn 调用级 harness
# ---------------------------------------------------------------------------
EVENT_NAMES = {"wake": "射频唤醒", "sync": "SYNC", "syncbyte": "SYNC字节",
               "fifo": "FIFO", "strobe": "STROBE", "delay": "delay"}


def fmt_events(ev) -> str:
    parts = []
    for e in ev:
        if e[0] == "sync":
            parts.append("SYNC=0x%04X" % e[1])
        elif e[0] == "syncbyte":
            parts.append("reg%d=0x%02X" % (e[1], e[2]))
        elif e[0] == "fifo":
            parts.append("FIFO(%d)" % e[1])
        elif e[0] == "strobe":
            parts.append("strobe(0x%02X)" % e[1])
        elif e[0] == "delay":
            parts.append("delay(%d)" % e[1])
        elif e[0] == "wake":
            parts.append("wake")
        else:
            parts.append(str(e))
    return " ".join(parts)


class UcProbe:
    """调用级 Unicorn：桩替换射频辅助函数，记录 SYNC/FIFO/STROBE/唤醒序列。"""

    def __init__(self, image: bytes):
        from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
        from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                       UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_SP,
                                       UC_ARM_REG_CPSR)
        self._Uc = Uc
        self._arch = (UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
        self._hook_code = UC_HOOK_CODE
        self.R0, self.R1, self.R2 = UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2
        self.LR, self.PC, self.SP, self.CPSR = (UC_ARM_REG_LR, UC_ARM_REG_PC,
                                                UC_ARM_REG_SP, UC_ARM_REG_CPSR)
        self.image = image

    def run(self, entry: int, ident: int = 0, fifo: bytes = b"\x11\x22\x33\x44",
            settings_id: int | None = None, max_insn: int = 200_000) -> dict:
        uc = self._Uc(*self._arch)
        uc.mem_map(UC_FLASH, UC_FLASH_SIZE)
        uc.mem_map(UC_RAM, UC_RAM_SIZE)
        uc.mem_write(BASE, self.image)
        uc.mem_write(SENTINEL, bytes.fromhex("fee7"))          # b . 哨兵
        # 设置结构：[+0]=channel(1) [+1]=ID
        sid = ident if settings_id is None else settings_id
        uc.mem_write(SETTINGS, bytes([1, sid & 0xFF, 2, 3]))
        uc.mem_write(BUSY_FLAG, b"\x00")
        tx_buf = fifo + b"\x00" * (8 - len(fifo))
        uc.mem_write(0x20001D14, tx_buf)

        events: list[tuple] = []
        state = {"error": None}

        def hook(_uc, address, _size, _user):
            a = address & ~1
            try:
                if a == SENTINEL:
                    _uc.emu_stop()
                    return
                if a == WRITE_REG:
                    reg = _uc.reg_read(self.R0)
                    buf = _uc.reg_read(self.R1)
                    ln = _uc.reg_read(self.R2)
                    data_bytes = bytes(_uc.mem_read(buf, ln)) if ln else b""
                    if reg == 0x44 and ln == 2:
                        events.append(("sync", (data_bytes[0] << 8) | data_bytes[1]))
                    elif reg == 0x7F:
                        events.append(("fifo", len(data_bytes)))
                    elif reg in (4, 5) and ln == 1:
                        events.append(("syncbyte", reg, data_bytes[0]))
                    else:
                        events.append(("wreg", reg, ln))
                elif a == STROBE:
                    events.append(("strobe", _uc.reg_read(self.R0)))
                elif a == DELAY_US:
                    events.append(("delay", _uc.reg_read(self.R0)))
                elif a == RADIO_WAKE:
                    events.append(("wake",))
                elif a in (GPIO_WRITE, GPIO_READ):
                    pass
                else:
                    return
                _uc.reg_write(self.PC, _uc.reg_read(self.LR))
            except Exception as exc:  # noqa: BLE001
                state["error"] = "hook 异常: %r" % (exc,)
                _uc.emu_stop()

        uc.hook_add(self._hook_code, hook)
        uc.reg_write(self.SP, STACK)
        uc.reg_write(self.LR, SENTINEL | 1)
        uc.reg_write(self.R0, ident)
        uc.reg_write(self.CPSR, 0x01000000)          # Thumb 状态
        try:
            uc.emu_start(entry | 1, 0, count=max_insn)
        except Exception as exc:  # noqa: BLE001
            state["error"] = "%s (pc=0x%08X lr=0x%08X)" % (
                exc, uc.reg_read(self.PC), uc.reg_read(self.LR))
        sp_after = uc.reg_read(self.SP)
        return {"events": events, "error": state["error"], "sp": sp_after,
                "pc": uc.reg_read(self.PC)}


def sync_word_of(events):
    """从事件序列中取同步字（reg4/5 单写 或 0x44 burst 写）。返回 (word, 是否burst)。"""
    hi = lo = None
    burst = False
    for e in events:
        if e[0] == "syncbyte":
            if e[1] == 4:
                hi = e[2]
            elif e[1] == 5:
                lo = e[2]
        elif e[0] == "sync":
            hi, lo = (e[1] >> 8) & 0xFF, e[1] & 0xFF
            burst = True
    if hi is None or lo is None:
        return None, burst
    return (hi << 8) | lo, burst


# ---------------------------------------------------------------------------
# 报表
# ---------------------------------------------------------------------------
class Report:
    def __init__(self):
        self.rows = []
        self.cur = ""
        self.sections = []

    def section(self, name):
        self.cur = name
        self.sections.append(name)
        print("\n## %s" % name)

    def chk(self, name, ok, detail=""):
        ok = bool(ok)
        self.rows.append((self.cur, name, ok, detail))
        line = "  [%s] %s" % ("PASS" if ok else "FAIL", name)
        if detail:
            line += "  | " + detail
        print(line)
        return ok

    def summary(self):
        npass = sum(1 for r in self.rows if r[2])
        nfail = len(self.rows) - npass
        print("\n## 汇总")
        for s in self.sections:
            rows = [r for r in self.rows if r[0] == s]
            if not rows:
                continue
            p = sum(1 for r in rows if r[2])
            print("  %-42s PASS %2d / %2d" % (s, p, len(rows)))
        for sec, name, ok, _detail in self.rows:
            if not ok:
                print("  FAIL: [%s] %s" % (sec, name))
        print("\n总计: PASS=%d  FAIL=%d" % (npass, nfail))
        print("结论:", "PASS —— 全部独立检查通过" if nfail == 0 else "FAIL —— 存在不一致项")
        return nfail


# ---------------------------------------------------------------------------
# 注入区期望反汇编表（脚本自己的核对基准，232 字节 / 74 条指令）
# ---------------------------------------------------------------------------
EXPECTED_INJ = [
    (0x081C0000, "cmp r0, #0x63"),
    (0x081C0002, "bne #0x81c000c"),
    (0x081C0004, "movw r4, #0xd798"),
    (0x081C0008, "b.w #0x802f050"),
    (0x081C000C, "movw r4, #0xc368"),
    (0x081C0010, "add r3, r2"),
    (0x081C0012, "udiv r4, r4, r3"),
    (0x081C0016, "b.w #0x802f050"),
    (0x081C001A, "push {r4, lr}"),
    (0x081C001C, "movw r3, #0x1768"),
    (0x081C0020, "movt r3, #0x2000"),
    (0x081C0024, "ldrb r3, [r3, #1]"),
    (0x081C0026, "cmp r3, #0x63"),
    (0x081C0028, "bne #0x81c0038"),
    (0x081C002A, "bl #0x81c008c"),
    (0x081C002E, "bl #0x81c009e"),
    (0x081C0032, "bl #0x81c007c"),
    (0x081C0036, "pop {r4, pc}"),
    (0x081C0038, "bl #0x81c00b0"),
    (0x081C003C, "pop {r4, pc}"),
    (0x081C003E, "push {r4, lr}"),
    (0x081C0040, "movw r3, #0x1768"),
    (0x081C0044, "movt r3, #0x2000"),
    (0x081C0048, "ldrb r3, [r3, #1]"),
    (0x081C004A, "cmp r3, #0x63"),
    (0x081C004C, "bne #0x81c005c"),
    (0x081C004E, "bl #0x81c00ba"),
    (0x081C0052, "bl #0x81c00cc"),
    (0x081C0056, "bl #0x81c007c"),
    (0x081C005A, "pop {r4, pc}"),
    (0x081C005C, "bl #0x81c00de"),
    (0x081C0060, "pop {r4, pc}"),
    (0x081C0062, "push {r4, lr}"),
    (0x081C0064, "sub sp, #8"),
    (0x081C0066, "mov r4, r0"),
    (0x081C0068, "lsrs r3, r4, #8"),
    (0x081C006A, "mov r1, sp"),
    (0x081C006C, "strb r3, [r1]"),
    (0x081C006E, "strb r4, [r1, #1]"),
    (0x081C0070, "movs r2, #2"),
    (0x081C0072, "movs r0, #0x44"),
    (0x081C0074, "bl #0x802e71c"),
    (0x081C0078, "add sp, #8"),
    (0x081C007A, "pop {r4, pc}"),
    (0x081C007C, "movw r0, #0xd798"),
    (0x081C0080, "b.w #0x81c0062"),
    (0x081C0084, "movw r0, #0x1f4"),
    (0x081C0088, "b.w #0x81c0062"),
    (0x081C008C, "push {r4, lr}"),
    (0x081C008E, "bl #0x802ec64"),
    (0x081C0092, "movw r0, #0xd798"),
    (0x081C0096, "bl #0x81c0062"),
    (0x081C009A, "b.w #0x802edca"),
    (0x081C009E, "push {r4, lr}"),
    (0x081C00A0, "bl #0x802ec64"),
    (0x081C00A4, "movw r0, #0x1f4"),
    (0x081C00A8, "bl #0x81c0062"),
    (0x081C00AC, "b.w #0x802edca"),
    (0x081C00B0, "push {r4, lr}"),
    (0x081C00B2, "bl #0x802ec64"),
    (0x081C00B6, "b.w #0x802edca"),
    (0x081C00BA, "push {r4, lr}"),
    (0x081C00BC, "bl #0x802ec64"),
    (0x081C00C0, "movw r0, #0xd798"),
    (0x081C00C4, "bl #0x81c0062"),
    (0x081C00C8, "b.w #0x802ee32"),
    (0x081C00CC, "push {r4, lr}"),
    (0x081C00CE, "bl #0x802ec64"),
    (0x081C00D2, "movw r0, #0x1f4"),
    (0x081C00D6, "bl #0x81c0062"),
    (0x081C00DA, "b.w #0x802ee32"),
    (0x081C00DE, "push {r4, lr}"),
    (0x081C00E0, "bl #0x802ec64"),
    (0x081C00E4, "b.w #0x802ee32"),
]


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description="X3 Pro V1.22 私有密钥补丁独立验证")
    ap.add_argument("--orig", default=os.path.join(ROOT, ORIG_REL),
                    help="原厂固件路径")
    ap.add_argument("--image", default=os.path.join(ROOT, PATCH_REL),
                    help="待验证的补丁成品路径")
    ap.add_argument("--key", default="0xD798", help="私有密钥（十六进制）")
    args = ap.parse_args(argv)

    rep = Report()
    key = int(args.key, 16)
    print("X3 Pro（X3pro_C_v1.22_app.bin）私有密钥补丁 —— 独立验证")
    print("原厂 : %s" % args.orig)
    print("成品 : %s" % args.image)
    print("密钥 : 0x%04X   密钥模式 ID=%d   普通帧同步字 0x%04X"
          % (key, DEFAULT_ID, ORDINARY_WORD))

    # =====================================================================
    rep.section("1. 输入镜像 / 哈希")
    try:
        orig = load(args.orig)
        patched = load(args.image)
    except OSError as exc:
        print("无法读取输入: %r" % (exc,))
        return 2
    rep.chk("原厂镜像大小 %d B" % len(orig), len(orig) == EXPECT_SIZE,
            "实际 %d" % len(orig))
    rep.chk("原厂镜像 sha256", sha256(orig) == EXPECT_ORIG_SHA, sha256(orig))
    rep.chk("成品镜像大小不变（原位补丁）", len(patched) == len(orig),
            "成品 %d B" % len(patched))
    rep.chk("成品 sha256 与 Lead 最新声明一致", sha256(patched) == EXPECT_PATCH_SHA,
            sha256(patched))
    if os.path.abspath(args.image) != os.path.abspath(os.path.join(ROOT, PATCH_REL)):
        rep.chk("--image 与 keypatch/out 成品逐字节一致",
                load(os.path.join(ROOT, PATCH_REL)) == patched,
                "外部镜像对比")

    # =====================================================================
    rep.section("2. 镜像基址判定（0x08020000）")
    vec = [word(orig, BASE + 4 * i) for i in range(16)]
    sp0 = vec[0]
    rep.chk("向量表 [0] 初值 SP=0x%08X 落在 SRAM 区 (0x20000000..0x3FFFFFFF)"
            % sp0, 0x20000000 <= sp0 < 0x40000000,
            "vec[0]=0x%08X" % sp0)
    rep.chk("向量表 [1] Reset 向量为 Thumb 地址 0x%08X" % vec[1],
            (vec[1] & 1) == 1, "vec[1]=0x%08X" % vec[1])
    reset = vec[1] & ~1
    rep.chk("Reset 地址 - 基址 == 0x11344（Reset_Handler 偏移）",
            reset - BASE == 0x11344, "实际 0x%X（0x%08X - 0x%08X）"
            % (reset - BASE, reset, BASE))
    reset_ins = dis_range(orig, reset, 0x10)
    rep.chk("offset 0x11344 处是 Reset_Handler（首条 ldr.w sp,[pc,#..]）",
            bool(reset_ins) and reset_ins[0].mnemonic == "ldr.w"
            and reset_ins[0].op_str.startswith("sp,"),
            ins_text(reset_ins[0]) if reset_ins else "反汇编失败")
    lit_sp_target, lit_sp = resolve_literal(orig, reset_ins[0])
    rep.chk("Reset_Handler 载入的初值 SP 与向量表 [0] 一致（0x%08X）" % sp0,
            lit_sp == sp0, "字面量@0x%08X = 0x%08X" % (lit_sp_target or 0, lit_sp or 0))
    # SystemInit：Reset_Handler 的第一个 bl 目标
    sysinit = None
    for ins in dis_range(orig, reset, 0x40):
        if ins.mnemonic == "bl":
            sysinit = branch_imm(ins)
            break
    rep.chk("Reset_Handler 首先调用系统初始化函数 0x%08X" % (sysinit or 0),
            sysinit is not None, "bl 目标")
    vtor_ok = False
    vtor_detail = ""
    if sysinit:
        scb_val = base_val = None
        str_ok = False
        for ins in dis_range(orig, sysinit, 0x60):
            if ins.mnemonic.startswith("ldr") and len(ins.operands) >= 2 \
                    and ins.operands[0].type == CS_OP_REG:
                _t, val = resolve_literal(orig, ins)
                if val == 0xE000ED00:
                    scb_val = (ins.operands[0].reg, ins.address, _t)
                elif val == BASE:
                    base_val = (ins.operands[0].reg, ins.address, _t)
            if ins.mnemonic.startswith("str") and len(ins.operands) == 2 \
                    and ins.operands[1].type == CS_OP_MEM:
                m = ins.operands[1]
                if scb_val and base_val and m.mem.base == scb_val[0] \
                        and m.mem.disp == 8 \
                        and ins.operands[0].type == CS_OP_REG \
                        and ins.operands[0].reg == base_val[0]:
                    str_ok = True
        vtor_ok = bool(scb_val and base_val and str_ok)
        if scb_val and base_val:
            vtor_detail = ("SCB=0xE000ED00 字面量@0x%08X；基址 0x08020000 字面量@0x%08X；"
                           "str [SCB,#8]（SCB->VTOR）= 基址：%s"
                           % (scb_val[2], base_val[2], "是" if str_ok else "否"))
        else:
            vtor_detail = "未找到 SCB/基址字面量"
    rep.chk("系统初始化把 SCB->VTOR 写成 0x08020000（应用向量表基址的铁证）",
            vtor_ok, vtor_detail)
    # .data 拷贝 / .bss 清零地址
    lits = []
    for ins in dis_range(orig, reset, 0x40):
        if ins.mnemonic.startswith("ldr") and len(ins.operands) >= 2 \
                and ins.operands[1].type == CS_OP_MEM and ins.operands[1].mem.base == ARM_REG_PC:
            lits.append(resolve_literal(orig, ins)[1])
    src = [v for v in lits if v is not None and BASE <= v < BASE + len(orig)]
    rep.chk("Reset_Handler 的 .data 源地址落在镜像内: %s"
            % (["0x%08X" % v for v in src]), bool(src))
    dflt = vec[7] & ~1
    dflt_ins = dis_one(orig, dflt)
    rep.chk("向量表 [7] 指向默认 handler 0x%08X 且为 b .（0xE7FE）"
            % dflt,
            dflt == 0x08031394 and dflt_ins is not None
            and dflt_ins.mnemonic == "b" and dflt_ins.op_str == "#0x8031394",
            ins_text(dflt_ins) if dflt_ins else "无")
    vec_bad = []
    for i, v in enumerate(vec):
        if i == 0 or v == 0:          # [0] 是初始 SP，不是 handler
            continue
        if not ((v & 1) == 1 and BASE <= (v & ~1) < BASE + len(orig)):
            vec_bad.append((i, v))
    rep.chk("所有非零向量表项（[0] 为 SP）均为 Thumb 奇数地址且落在镜像范围内",
            not vec_bad, "异常项 %r" % (vec_bad,))
    base_lits = [a for a in range(BASE, BASE + len(orig) - 3, 4)
                 if word(orig, a) == BASE]
    rep.chk("镜像 4 字节对齐字面量中 0x08020000 恰出现 1 次（VTOR 用）",
            len(base_lits) == 1, "位置 %s" % (["0x%08X" % a for a in base_lits],))

    # =====================================================================
    rep.section("3. 同步字函数 0x0802F040（边界 / 公式 / 写入序列）")
    fn = dis_range(orig, SYNC_FN, SYNC_FN_LEN + 0x10)
    rep.chk("函数头 0x%08X = movs r2,#1; mov r3,r0; push {r4,lr}"
            % SYNC_FN,
            len(fn) >= 3 and fn[0].mnemonic == "movs" and fn[0].op_str == "r2, #1"
            and fn[1].mnemonic == "mov" and fn[1].op_str == "r3, r0"
            and fn[2].mnemonic == "push" and fn[2].op_str == "{r4, lr}",
            " / ".join(ins_text(i) for i in fn[:3]))
    po = off(SYNC_PATCH_ADDR)
    rep.chk("0x%08X 处 10 字节 = movw r4,#0xC368; add r3,r2; udiv r4,r4,r3"
            % SYNC_PATCH_ADDR,
            orig[po:po + 10] == SYNC_ORIG_BYTES,
            "原厂原字节 " + orig[po:po + 10].hex(" "))
    f10 = dis_range(orig, SYNC_PATCH_ADDR, 10)
    expect10 = ["movw r4, #0xc368", "add r3, r2", "udiv r4, r4, r3"]
    rep.chk("10 字节反汇编正是 movw/add/udiv 且与常量 0xC368 一致",
            [ins_text(i) for i in f10[:3]] == expect10,
            " / ".join(ins_text(i) for i in f10[:3]))
    rep.chk("add r3,r2 语义：r2=1 使 r3 = ID+1（0x0802F04A）",
            f10[1].mnemonic == "add" and f10[1].op_str == "r3, r2")
    rep.chk("udiv r4,r4,r3 为无符号除法（原厂公式 floor(50024/(ID+1))）",
            f10[2].mnemonic == "udiv" and f10[2].op_str == "r4, r4, r3")
    # 函数尾：b.w delay_us 收尾，0x0802F09C 是下一函数
    tail = [i for i in fn if i.address < SYNC_FN + SYNC_FN_LEN]
    last = tail[-1]
    rep.chk("函数以 b.w delay_us(0x0802B5DC) 尾调用结束（0x%08X）" % last.address,
            last.mnemonic == "b.w" and branch_imm(last) == DELAY_US,
            ins_text(last))
    nxt = dis_one(orig, SYNC_NEXT)
    rep.chk("最后指令结束于 0x%08X（长 0x5A），2 字节 NOP 对齐后下一函数 0x%08X 以 push 开头"
            % (SYNC_FN + SYNC_FN_LEN, SYNC_NEXT),
            nxt is not None and nxt.mnemonic == "push"
            and SYNC_NEXT == SYNC_FN + SYNC_FN_LEN + 2
            and orig[off(SYNC_FN + SYNC_FN_LEN):off(SYNC_FN + SYNC_FN_LEN) + 2]
            == bytes.fromhex("00bf"),
            "%s；对齐 NOP=%s" % (ins_text(nxt) if nxt else "无",
                                orig[off(SYNC_FN + SYNC_FN_LEN):off(SYNC_FN + SYNC_FN_LEN) + 2].hex(" ")))
    # 本函数内写 reg4/reg5 + 延时/频闪序列：直接按地址核对关键指令
    seq_ok = True
    seq_txt = []
    checks = [
        (0x0802F058, "movs r0, #4"),
        (0x0802F05E, "bl #0x802e71c"),
        (0x0802F068, "movs r0, #5"),
        (0x0802F06E, "bl #0x802e71c"),
        (0x0802F072, "movs r0, #0x14"),     # delay 20
        (0x0802F078, "movs r0, #0x33"),     # SCAL
        (0x0802F07E, "mov.w r0, #0x3e8"),   # delay 1000
        (0x0802F086, "movs r0, #0x36"),     # SIDLE
        (0x0802F08C, "mov.w r0, #0x2bc"),   # delay 700
    ]
    for addr, text in checks:
        i = dis_one(orig, addr)
        seq_txt.append("%s:%s" % ("0x%08X" % addr, ins_text(i) if i else "?"))
        if i is None or ins_text(i) != text:
            seq_ok = False
    rep.chk("同步字函数写入/时序序列：reg4(0x0802F05E) -> reg5(0x0802F06E) -> "
            "delay20 -> SCAL(0x33) -> 1ms -> SIDLE(0x36) -> delay700", seq_ok,
            " ; ".join(seq_txt))

    # =====================================================================
    rep.section("4. 设置结构 0x20001768 / ID=99 可达性")
    call1 = 0x0802F234
    call2 = 0x0802F902
    # 两个已知 set_sync 调用点前的 ldrb r0,[rX,#1]
    def check_set_sync_call(call_addr, ldrb_addr, ldr_addr):
        i_ldr = dis_one(orig, ldr_addr)
        t, v = resolve_literal(orig, i_ldr) if i_ldr else (None, None)
        i_ldrb = dis_one(orig, ldrb_addr)
        return (v == SETTINGS and i_ldrb is not None and i_ldrb.mnemonic == "ldrb"
                and i_ldrb.op_str.endswith("#1]")), v, i_ldrb, i_ldr
    ok1, v1, ldrb1, ldr1 = check_set_sync_call(call1, 0x0802F230, 0x0802F20A)
    ok2, v2, ldrb2, ldr2 = check_set_sync_call(call2, 0x0802F900, 0x0802F8F0)
    rep.chk("调用点 0x%08X：设置指针 = 0x20001768（字面量@0x%08X），ID 取 [+1]（%s）"
            % (call1, resolve_literal(orig, ldr1)[0] or 0,
               ins_text(ldrb1) if ldrb1 else "?"), ok1,
            "实际设置指针 0x%08X" % (v1 or 0))
    rep.chk("调用点 0x%08X：设置指针 = 0x20001768（字面量@0x%08X），ID 取 [+1]（%s）"
            % (call2, resolve_literal(orig, ldr2)[0] or 0,
               ins_text(ldrb2) if ldrb2 else "?"), ok2,
            "实际设置指针 0x%08X" % (v2 or 0))
    # 菜单钳位片段
    clamp = dis_range(orig, 0x08029996, 0x1E)
    clamp_txt = [ins_text(i) for i in clamp]
    want_clamp = ["ldr r3, [pc, #0x1c]", "ldr r2, [pc, #0x1c]", "ldrb r1, [r3]",
                  "ldrb r3, [r2, #1]", "cbz r1, #0x80299ae", "adds r3, #0xa",
                  "uxtb r3, r3", "cmp r3, #0x63", "it hi", "movhi r3, #0x63",
                  "strb r3, [r2, #1]", "pop {r4, pc}"]
    rep.chk("id 菜单钳位片段起始 0x08029996（cmp 实际在 0x080299A4/0x080299B2 循环处）",
            clamp_txt[:len(want_clamp)] == want_clamp,
            " ; ".join(clamp_txt[:len(want_clamp)]))
    t_flag, v_flag = resolve_literal(orig, clamp[0])
    t_set, v_set = resolve_literal(orig, clamp[1])
    rep.chk("钳位片段读标志 0x20000F9C 与设置 0x20001768，写回 [+1]",
            v_flag == ID_FLAG and v_set == SETTINGS
            and any(i.mnemonic == "pop" and i.op_str == "{r4, pc}" for i in clamp),
            "flag@0x%08X=0x%08X settings@0x%08X=0x%08X"
            % (t_flag or 0, v_flag or 0, t_set or 0, v_set or 0))
    # Unicorn：执行钳位片段，验证 99 可达且为上限
    probe = UcProbe(orig)
    clamp_tests = [
        (98, 0, 99, "98+1 -> 99"),
        (99, 0, 99, "99+1 -> 钳位 99"),
        (89, 1, 99, "89+10 -> 99"),
        (98, 1, 99, "98+10 -> 钳位 99"),
        (0, 0, 1, "0+1 -> 1"),
        (0, 1, 10, "0+10 -> 10"),
    ]
    clamp_ok = True
    clamp_msg = []
    for start_id, flag, want_id, desc in clamp_tests:
        # 用独立 Unicorn 片段执行（栈上准备 pop {r4,pc} 的返回地址）
        from unicorn import (Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                             UC_HOOK_CODE)
        from unicorn.arm_const import (UC_ARM_REG_PC, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                       UC_ARM_REG_CPSR)
        uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
        uc.mem_map(UC_FLASH, UC_FLASH_SIZE)
        uc.mem_map(UC_RAM, UC_RAM_SIZE)
        uc.mem_write(BASE, orig)
        uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
        uc.mem_write(SETTINGS, bytes([1, start_id, 2, 3]))
        uc.mem_write(ID_FLAG, bytes([flag]))
        uc.mem_write(STACK, struct.pack("<II", 0xDEADBEEF, SENTINEL | 1))
        stopped = {"v": False}

        def h(_uc, address, _size, _u):
            if (address & ~1) == SENTINEL:
                stopped["v"] = True
                _uc.emu_stop()
        uc.hook_add(UC_HOOK_CODE, h)
        uc.reg_write(UC_ARM_REG_SP, STACK)
        uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
        uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
        err = None
        try:
            uc.emu_start(0x08029996 | 1, 0, count=2000)
        except Exception as exc:  # noqa: BLE001
            err = exc
        got = uc.mem_read(SETTINGS + 1, 1)[0]
        if err is not None or got != want_id:
            clamp_ok = False
            clamp_msg.append("FAIL(%d,%d)->%d" % (start_id, flag, got))
        else:
            clamp_msg.append("%s=>%d" % (desc, got))
    rep.chk("Unicorn 片段执行菜单钳位：99 可由 +1/+10 到达且为上限", clamp_ok,
            "; ".join(clamp_msg))

    # =====================================================================
    rep.section("5. 全镜像写 CC2500 reg4/reg5 的唯一性")
    branches_orig = iter_branches(orig)
    wr_sites = sorted({s for (s, _mn, t) in branches_orig if t == WRITE_REG})
    regs = []
    unresolved = []
    for s in wr_sites:
        val, _ = trace_r0(orig, s)
        if isinstance(val, int):
            regs.append((s, val))
        else:
            unresolved.append(s)
    reg45 = [s for s, v in regs if v in (4, 5)]
    reg44 = [s for s, v in regs if v == 0x44]
    thunk_site = 0x0802ECC4       # write_reg(r0, r1, 1) 单字节写 thunk 内部调用点
    unresolved = [s for s in unresolved if s != thunk_site]
    rep.chk("write_reg(0x0802E71C) 直接调用点 %d 个，r0 常量回溯 %d 个"
            "（另 1 个在字节写 thunk 内，r0 由调用者传入，另行核对）"
            % (len(wr_sites), len(regs)), len(wr_sites) == 80 and not unresolved,
            "无法回溯 %d 个: %s" % (len(unresolved),
                                    ["0x%08X" % s for s in unresolved[:8]]))
    rep.chk("写 reg4/reg5 的调用点恰为 4 处（初始化表 2 + 同步字函数 2）",
            sorted(reg45) == [0x0802E950, 0x0802E962, 0x0802F05E, 0x0802F06E],
            "实际 %s" % (["0x%08X" % s for s in reg45],))
    rep.chk("原厂镜像无 burst 写 reg0x44（写 reg4/5 只能来自上述 4 处）",
            not reg44, "0x44 调用点 %s" % (["0x%08X" % s for s in reg44],))
    # 间接路径：0x0802ECB4 是「write_reg(r0, r1, 1)」单字节写 thunk（r0 由调用者传入）
    thunk_callers = [s for (s, _mn, t) in branches_orig if t == 0x0802ECB4]
    thunk_vals = []
    for s in thunk_callers:
        seq = dis_range(orig, s - 8, 8)
        v = None
        for i in seq:
            if i.mnemonic in ("movs", "mov") and i.op_str.startswith("r0, #"):
                v = int(i.op_str.split("#")[1], 16)
        thunk_vals.append((s, v))
    rep.chk("字节写 thunk 0x0802ECB4 的 %d 个调用点也不写 reg4/5（r0=%s）"
            % (len(thunk_callers), [v for _s, v in thunk_vals]),
            all(v not in (4, 5) for _s, v in thunk_vals)
            and sorted(v for _s, v in thunk_vals) == [6, 0x18],
            "调用点 %s" % (["0x%08X:r0=%r" % (s, v) for s, v in thunk_vals],))
    # 初始化表写入值 0xC3 / 0x68
    def init_value(call_addr):
        ins = dis_range(orig, call_addr - 0x10, 0x10)
        for i in reversed(ins):
            if i.mnemonic == "strb.w" and i.op_str.startswith("r2"):
                for j in dis_range(orig, i.address - 0x10, 0x10):
                    if j.address < i.address and j.mnemonic == "movs" \
                            and j.op_str.startswith("r2,"):
                        return int(j.op_str.split("#")[1], 16)
        return None
    v4 = init_value(0x0802E950)
    v5 = init_value(0x0802E962)
    rep.chk("初始化表 reg4 <- 0xC3、reg5 <- 0x68（SYNC1/SYNC0 复位值）",
            v4 == 0xC3 and v5 == 0x68, "reg4=0x%02X reg5=0x%02X"
            % (v4 or 0, v5 or 0))
    # 补丁后：新增注入区 set_sync 的 burst 0x44 写入
    branches_patch = iter_branches(patched)
    wr_sites_p = sorted({s for (s, _mn, t) in branches_patch if t == WRITE_REG})
    new_sites = sorted(set(wr_sites_p) - set(wr_sites))
    inj44 = []
    for s in new_sites:
        val, _ = trace_r0(patched, s)
        if val == 0x44:
            inj44.append(s)
    rep.chk("成品新增唯一 write_reg 调用点 = 注入区 set_sync（reg=0x44 burst）",
            inj44 == [0x081C0074]
            and all(INJ_BASE <= s < INJ_BASE + INJ_WINDOW for s in new_sites),
            "新增 %s；其中 r0=0x44 的 %s"
            % (["0x%08X" % s for s in new_sites],
               ["0x%08X" % s for s in inj44]))

    # =====================================================================
    rep.section("6. 发送入口 0x0802EDC4 / 0x0802EE2C")
    body4 = dis_range(orig, TX4, 0x60)
    body2 = dis_range(orig, TX2, 0x60)
    rep.chk("TX4 入口 = push {r4,lr}; bl radio_wake(0x0802EC64)",
            len(body4) >= 2 and body4[0].mnemonic == "push"
            and body4[0].op_str == "{r4, lr}" and body4[1].mnemonic == "bl"
            and branch_imm(body4[1]) == RADIO_WAKE,
            " / ".join(ins_text(i) for i in body4[:2]))
    rep.chk("TX2 入口同样 = push {r4,lr}; bl radio_wake",
            len(body2) >= 2 and body2[0].mnemonic == "push"
            and body2[1].mnemonic == "bl" and branch_imm(body2[1]) == RADIO_WAKE)
    # 函数体关键指令
    def find_body(data, start, addr, text):
        ins = dis_one(data, addr)
        return ins is not None and ins_text(ins) == text
    checks4 = [
        (0x0802EDE0, "movs r2, #4"),
        (0x0802EDE2, "ldr r1, [pc, #0x40]"),
        (0x0802EDE4, "movs r0, #0x7f"),
        (0x0802EDE6, "bl #0x802e71c"),      # FIFO burst 0x7F, 4B
        (0x0802EDEA, "movs r0, #0x35"),     # STX
        (0x0802EDF0, "mov.w r0, #0x258"),   # delay 600
        (0x0802EDF8, "movs r0, #0x3b"),     # SFTX
        (0x0802EDFE, "movs r0, #0x36"),     # SIDLE
        (0x0802EE1E, "pop {r4, pc}"),
    ]
    ok = all(find_body(orig, TX4, a, t) for a, t in checks4)
    rep.chk("TX4 函数体：FIFO(0x7F,4B) -> STX(0x35) -> 600us -> SFTX(0x3B) -> "
            "SIDLE(0x36) -> pop {r4,pc}", ok,
            " ; ".join("%s=%s" % ("0x%08X" % a, ins_text(dis_one(orig, a)) if dis_one(orig, a) else "?")
                       for a, _t in checks4[:4]))
    checks2 = [
        (0x0802EE48, "movs r0, #0x28"),     # delay 40
        (0x0802EE4E, "movs r2, #2"),
        (0x0802EE50, "ldr r1, [pc, #0x40]"),
        (0x0802EE52, "movs r0, #0x7f"),
        (0x0802EE54, "bl #0x802e71c"),      # FIFO 0x7F, 2B
        (0x0802EE58, "movs r0, #0x35"),
        (0x0802EE5E, "mov.w r0, #0x1f4"),   # delay 500
        (0x0802EE66, "movs r0, #0x3b"),
        (0x0802EE6C, "movs r0, #0x36"),
        (0x0802EE8C, "pop {r4, pc}"),
    ]
    ok2 = all(find_body(orig, TX2, a, t) for a, t in checks2)
    rep.chk("TX2 函数体：40us -> FIFO(0x7F,2B) -> STX -> 500us -> SFTX -> SIDLE",
            ok2,
            " ; ".join("%s=%s" % ("0x%08X" % a, ins_text(dis_one(orig, a)) if dis_one(orig, a) else "?")
                       for a, _t in checks2[:5]))
    n4_bl = sum(1 for (_s, mn, t) in branches_orig if t == TX4 and mn == "bl")
    n4_bw = sum(1 for (_s, mn, t) in branches_orig if t == TX4 and mn == "b.w")
    n2_bl = sum(1 for (_s, mn, t) in branches_orig if t == TX2 and mn == "bl")
    n2_bw = sum(1 for (_s, mn, t) in branches_orig if t == TX2 and mn == "b.w")
    rep.chk("TX4 静态引用：%d 个 BL 调用点 + %d 个 B.W 尾跳（任务书称 16 调用点）"
            % (n4_bl, n4_bw), n4_bl == 16 and n4_bw == 2,
            "总入口引用 %d" % (n4_bl + n4_bw))
    rep.chk("TX2 静态引用：%d 个 BL 调用点 + %d 个 B.W 尾跳（任务书称 9 调用点）"
            % (n2_bl, n2_bw), n2_bl == 9 and n2_bw == 1,
            "总入口引用 %d" % (n2_bl + n2_bw))
    n_sync = sum(1 for (_s, _mn, t) in branches_orig if t == SYNC_FN)
    rep.chk("同步字函数静态引用 %d 处（含 2 个设置结构 ldrb 调用点）" % n_sync,
            n_sync >= 2, "")
    # 入口只覆盖前 4 字节；原 bl 指令的后 2 字节残留在 +4..+5（不再执行）
    dead_ok = True
    for entry in (TX4, TX2):
        o = off(entry)
        dead_ok = dead_ok and patched[o + 4:o + 6] == orig[o + 4:o + 6]
    rep.chk("TX4/TX2 入口覆盖后残留 2 字节 = 原 bl 指令后半（死字节，未被改写）",
            dead_ok,
            "TX4 残留 %s / TX2 残留 %s"
            % (patched[off(TX4) + 4:off(TX4) + 6].hex(" "),
               patched[off(TX2) + 4:off(TX2) + 6].hex(" ")))

    # =====================================================================
    rep.section("7. 成品差异范围（3 补丁点 + 注入区）")
    diff = {i for i in range(len(orig)) if orig[i] != patched[i]}
    # 注入区代码长度：0x081C0000 起连续非 0xFF 的最后位置
    inj_o = off(INJ_BASE)
    seg = patched[inj_o:inj_o + INJ_WINDOW]
    last = max((i for i, b in enumerate(seg) if b != 0xFF), default=-1)
    code_len = last + 1
    rep.chk("注入区实际代码长度 %d 字节（0x%X，原厂全 0xFF）" % (code_len, code_len),
            code_len == INJ_CODE_LEN, "上限窗口 0x%X" % INJ_WINDOW)
    rep.chk("原厂注入窗口 0x081C0000..0x081C0400 全 0xFF",
            orig[inj_o:inj_o + INJ_WINDOW] == b"\xff" * INJ_WINDOW)
    rep.chk("成品注入窗口剩余 %d 字节保持 0xFF"
            % (INJ_WINDOW - code_len),
            seg[code_len:] == b"\xff" * (INJ_WINDOW - code_len))
    expected = set(range(off(TX4), off(TX4) + 4)) \
        | set(range(off(TX2), off(TX2) + 4)) \
        | set(range(off(SYNC_PATCH_ADDR), off(SYNC_PATCH_ADDR) + 10)) \
        | {inj_o + i for i in range(code_len) if patched[inj_o + i] != 0xFF}
    rep.chk("与原厂差异字节数 = %d（预期 %d）" % (len(diff), len(expected)),
            diff == expected,
            "差异区间 %s" % (["0x%05X..0x%05X" % (g[0], g[-1])
                            for g in _group(sorted(diff))],))
    for addr, nbytes, want in ((TX4, 4, TRAMPOLINE_LOST),
                               (TX2, 4, TRAMPOLINE_LOST),
                               (SYNC_PATCH_ADDR, 10, SYNC_ORIG_BYTES)):
        o = off(addr)
        rep.chk("0x%08X 原厂字节 = %s" % (addr, want.hex(" ")),
                orig[o:o + nbytes] == want, "实际 " + orig[o:o + nbytes].hex(" "))
    # 更严格：所有差异都在 expected 内已由 diff==expected 保证
    rep.chk("差异仅落在补丁点与注入区代码内（无其它字节变化）", diff == expected)

    # =====================================================================
    rep.section("8. 注入区反汇编核对（分支目标 / 常量 / 蹦床顺序 / 栈平衡）")
    inj_ins = dis_range(patched, INJ_BASE, code_len)
    actual = [(i.address, ins_text(i)) for i in inj_ins]
    rep.chk("注入区线性反汇编恰好 %d 条指令、覆盖 %d 字节"
            % (len(EXPECTED_INJ), code_len),
            len(actual) == len(EXPECTED_INJ)
            and (actual[-1][0] + len(inj_ins[-1].bytes) == INJ_BASE + code_len),
            "%d 条 / %d 字节" % (len(actual), code_len))
    rep.chk("注入区完整指令序列与独立期望表逐条一致",
            actual == EXPECTED_INJ,
            "" if actual == EXPECTED_INJ else "首条不一致: %r" % (
                (actual[:1], EXPECTED_INJ[:1]),))
    # 入口 B.W
    for entry, want_target, name in ((TX4, 0x081C001A, "inj_tx4"),
                                     (TX2, 0x081C003E, "inj_tx2")):
        o = off(entry)
        rep.chk("0x%08X 入口 4 字节 = B.W -> %s(0x%08X)" % (entry, name, want_target),
                patched[o:o + 4] == _enc_bw(entry, want_target)
                and branch_imm(dis_one(patched, entry)) == want_target,
                "字节 %s" % patched[o:o + 4].hex(" "))
    rep.chk("0x%08X 起 10 字节 = B.W inj_sync + 3x NOP(0xBF00)"
            % SYNC_PATCH_ADDR,
            patched[off(SYNC_PATCH_ADDR):off(SYNC_PATCH_ADDR) + 4]
            == _enc_bw(SYNC_PATCH_ADDR, 0x081C0000)
            and patched[off(SYNC_PATCH_ADDR) + 4:off(SYNC_PATCH_ADDR) + 10]
            == bytes.fromhex("00bf00bf00bf"),
            patched[off(SYNC_PATCH_ADDR):off(SYNC_PATCH_ADDR) + 10].hex(" "))
    # 常量与 ID 分支
    inj_text = [t for _a, t in actual]
    rep.chk("ID=99 判断：cmp r0,#0x63 / cmp r3,#0x63（两处包装各一）",
            inj_text.count("cmp r0, #0x63") == 1
            and inj_text.count("cmp r3, #0x63") == 2,
            "cmp r0:#99=%d cmp r3:#99=%d"
            % (inj_text.count("cmp r0, #0x63"), inj_text.count("cmp r3, #0x63")))
    rep.chk("KEY 常量 0x%04X 出现 4 次（inj_sync 1 + set_key 1 + 两个 key 蹦床 2）" % key,
            inj_text.count("movw r4, #0x%04x" % key) == 1
            and inj_text.count("movw r0, #0x%04x" % key) == 3,
            "movw r4 KEY=%d movw r0 KEY=%d"
            % (inj_text.count("movw r4, #0x%04x" % key),
               inj_text.count("movw r0, #0x%04x" % key)))
    rep.chk("0x01F4（原厂 ID=99 同步字 500）出现 3 次（set_ord + 两个 ord 蹦床）",
            inj_text.count("movw r0, #0x1f4") == 3,
            "%d 次" % inj_text.count("movw r0, #0x1f4"))
    # 蹦床内顺序：push -> bl 0x0802EC64 -> (写同步字) -> b.w resume
    tramps = [0x081C008C, 0x081C009E, 0x081C00B0,
              0x081C00BA, 0x081C00CC, 0x081C00DE]
    order_ok = True
    order_msg = []
    for t in tramps:
        ins = dis_range(patched, t, 0x20)
        texts = [ins_text(i) for i in ins]
        has_wake = "bl #0x802ec64" in texts
        wake_i = texts.index("bl #0x802ec64") if has_wake else -1
        sync_i = texts.index("bl #0x81c0062") if "bl #0x81c0062" in texts else -1
        bw_i = next((k for k, x in enumerate(texts) if x.startswith("b.w #0x802")), -1)
        ok = has_wake and bw_i > wake_i and (sync_i == -1 or sync_i > wake_i)
        order_ok = order_ok and ok
        order_msg.append("0x%08X: %s" % (t, "wake->sync->resume" if ok else "顺序异常"))
    rep.chk("6 个蹦床均为 push -> bl 0x0802EC64(唤醒) ->[写同步字]-> b.w 原函数体",
            order_ok, "; ".join(order_msg))
    # 恢复 KEY：inj_tx4/inj_tx2 的 key 路径最后一步是 bl set_key
    rep.chk("包装函数在密钥模式末尾调用 set_key 恢复 SYNC=KEY",
            inj_text.count("bl #0x81c007c") == 2,
            "%d 次" % inj_text.count("bl #0x81c007c"))
    # 栈平衡（静态）
    n_push = sum(1 for i in inj_ins if i.mnemonic == "push" and i.op_str == "{r4, lr}")
    n_pop = sum(1 for i in inj_ins if i.mnemonic == "pop" and i.op_str == "{r4, pc}")
    rep.chk("注入区静态栈操作：9 个 push {r4,lr} / 5 个 pop {r4,pc}"
            "（其余 3 个 push 由原函数 pop 配对）",
            n_push == 9 and n_pop == 5, "push=%d pop=%d" % (n_push, n_pop))
    rep.chk("TX4 恢复点之后原函数体以 pop {r4,pc} 结束（0x0802EE1E）",
            (dis_one(orig, 0x0802EE1E) or None) is not None
            and dis_one(orig, 0x0802EE1E).mnemonic == "pop"
            and dis_one(orig, 0x0802EE1E).op_str == "{r4, pc}")
    rep.chk("TX2 恢复点之后原函数体以 pop {r4,pc} 结束（0x0802EE8C）",
            (dis_one(orig, 0x0802EE8C) or None) is not None
            and dis_one(orig, 0x0802EE8C).mnemonic == "pop"
            and dis_one(orig, 0x0802EE8C).op_str == "{r4, pc}")
    rep.chk("注入区没有关中断指令（cpsid/cpsie 均不存在，属已知残余风险）",
            not any(i.mnemonic.startswith("cps") for i in inj_ins),
            "cpsid/cpsie 数：%d" % sum(1 for i in inj_ins if i.mnemonic.startswith("cps")))
    # 注入区内存写入仅限 STACK 缓冲（其余通过 write_reg）
    writes = [i for i in inj_ins if i.mnemonic in ("str", "strb", "strh", "strd")
              or i.mnemonic.startswith("str")]
    rep.chk("注入区仅 2 处直接内存写（写栈缓冲 SYNC1/SYNC0），其余经 write_reg",
            len(writes) == 2
            and all(w.op_str.startswith(("r3, [r1]", "r4, [r1")) for w in writes),
            "; ".join(ins_text(w) for w in writes))

    # =====================================================================
    rep.section("9. 空闲闪存 / 引用扫描")
    seg_free = orig[off(FREE_LO):off(FREE_HI)]
    rep.chk("原厂 0x%08X..0x%08X 全 0xFF（%d 字节）" % (FREE_LO, FREE_HI, len(seg_free)),
            seg_free == b"\xff" * len(seg_free))
    word_refs_inj = iter_word_refs(orig, INJ_BASE, INJ_BASE + INJ_WINDOW)
    rep.chk("原厂无字面量引用 0x081C0000..0x081C0400（0 处）",
            not word_refs_inj,
            "发现 %s" % (["0x%08X=0x%08X" % r for r in word_refs_inj[:8]],))
    br_inj = [(s, mn, t) for (s, mn, t) in branches_orig
              if INJ_BASE <= t < INJ_BASE + INJ_WINDOW]
    rep.chk("原厂无分支目标落入 0x081C0000..0x081C0400（0 处）",
            not br_inj, "发现 %s" % (["0x%08X->0x%08X" % (s, t) for s, _m, t in br_inj[:8]],))
    ext = [(s, mn, t) for (s, mn, t) in branches_patch
           if INJ_BASE <= t < INJ_BASE + INJ_CODE_LEN and not (INJ_BASE <= s < INJ_BASE + INJ_CODE_LEN)]
    want_ext = {(TX4, "b.w", 0x081C001A), (TX2, "b.w", 0x081C003E),
                (SYNC_PATCH_ADDR, "b.w", 0x081C0000)}
    rep.chk("成品注入区外部分支入口恰为 3 个补丁点（无其它跳入）",
            set(ext) == want_ext,
            "实际 %s" % ([(hex(s), mn, hex(t)) for s, mn, t in ext],))
    word_refs_p = [(a, v) for (a, v) in iter_word_refs(patched, INJ_BASE, INJ_BASE + INJ_WINDOW)
                   if not (INJ_BASE <= a < INJ_BASE + INJ_WINDOW)]
    rep.chk("成品无字面量引用注入窗口（补丁只加分支，不加数据指针）",
            not word_refs_p, "发现 %s" % (["0x%08X" % a for a, _v in word_refs_p[:8]],))

    # =====================================================================
    rep.section("10. Unicorn 调用级语义（同步字 / 双发 / 原厂自证）")
    probe_o = UcProbe(orig)
    probe_p = UcProbe(patched)
    # 10.1 原厂同步字函数多组 ID
    ids = [0, 1, 2, 37, 50, 73, 98, 99]
    ok = True
    msgs = []
    for ident in ids:
        r = probe_o.run(SYNC_FN, ident)
        got, burst = sync_word_of(r["events"])
        want = SYNC_NUMERATOR // (ident + 1)
        good = (r["error"] is None and got == want and not burst
                and r["events"] == [("syncbyte", 4, want >> 8),
                                    ("syncbyte", 5, want & 0xFF),
                                    ("delay", 20), ("strobe", 0x33),
                                    ("delay", 1000), ("strobe", 0x36),
                                    ("delay", 700)])
        ok = ok and good
        msgs.append("ID=%-2d SYNC=0x%04X%s" % (ident, got if got is not None else 0,
                                               "" if good else " FAIL"))
    rep.chk("原厂镜像自证：set_sync(ID)=floor(50024/(ID+1))，reg4/reg5 单写 + "
            "20us/SCAL/1ms/SIDLE/700us 序列", ok, "; ".join(msgs))
    # 10.2 补丁版同步字函数
    ok = True
    msgs = []
    for ident in ids:
        r = probe_p.run(SYNC_FN, ident)
        got, burst = sync_word_of(r["events"])
        want = key if ident == DEFAULT_ID else SYNC_NUMERATOR // (ident + 1)
        rest_ok = r["events"][2:] == [("delay", 20), ("strobe", 0x33),
                                      ("delay", 1000), ("strobe", 0x36),
                                      ("delay", 700)]
        good = (r["error"] is None and got == want and not burst
                and r["events"][0] == ("syncbyte", 4, want >> 8)
                and r["events"][1] == ("syncbyte", 5, want & 0xFF) and rest_ok
                and r["sp"] == STACK)
        ok = ok and good
        msgs.append("ID=%-2d SYNC=0x%04X%s" % (ident, got if got is not None else 0,
                                               "" if good else " FAIL"))
    rep.chk("补丁版 set_sync：ID=99 -> KEY(0x%04X)，ID 0..98 与原厂逐位一致，"
            "后段时序/栈平衡不变" % key, ok, "; ".join(msgs))
    # 10.3 TX4/TX2 原厂自证（单发）
    BODY4 = [("fifo", 4), ("strobe", 0x35), ("delay", 600),
             ("strobe", 0x3B), ("strobe", 0x36)]
    BODY2 = [("delay", 40), ("fifo", 2), ("strobe", 0x35), ("delay", 500),
             ("strobe", 0x3B), ("strobe", 0x36)]
    r = probe_o.run(TX4, 0)
    rep.chk("原厂镜像自证 TX4(ID=0)：wake + 单发 FIFO(4)/STX/SFTX/SIDLE，无 SYNC 写入",
            r["error"] is None and r["events"] == [("wake",)] + BODY4
            and r["sp"] == STACK, fmt_events(r["events"]))
    r = probe_o.run(TX2, 0)
    rep.chk("原厂镜像自证 TX2(ID=0)：wake + 40us + 单发 FIFO(2)/STX/500us/SFTX/SIDLE",
            r["error"] is None and r["events"] == [("wake",)] + BODY2
            and r["sp"] == STACK, fmt_events(r["events"]))
    # 10.4 补丁版 TX：ID=99 双发
    want4 = ([("wake",), ("sync", key)] + BODY4
             + [("wake",), ("sync", ORDINARY_WORD)] + BODY4
             + [("sync", key)])
    r = probe_p.run(TX4, DEFAULT_ID)
    rep.chk("补丁版 TX4(ID=99)：wake+SYNC=KEY -> FIFO(4) -> wake+SYNC=0x01F4 -> "
            "FIFO(4) -> SYNC=KEY（双发）",
            r["error"] is None and r["events"] == want4 and r["sp"] == STACK,
            fmt_events(r["events"]))
    order_ok = (r["events"][:2] == [("wake",), ("sync", key)]
                and r["events"][7:9] == [("wake",), ("sync", ORDINARY_WORD)])
    rep.chk("同步字写入发生在射频唤醒 bl 0x0802EC64 之后（双发两帧均如此）",
            order_ok, "前两事件 %s / 第二帧 %s" % (r["events"][:2], r["events"][7:9]))
    want2 = ([("wake",), ("sync", key)] + BODY2
             + [("wake",), ("sync", ORDINARY_WORD)] + BODY2
             + [("sync", key)])
    r = probe_p.run(TX2, DEFAULT_ID)
    rep.chk("补丁版 TX2(ID=99)：wake+SYNC=KEY -> FIFO(2) -> wake+SYNC=0x01F4 -> "
            "FIFO(2) -> SYNC=KEY（双发）",
            r["error"] is None and r["events"] == want2 and r["sp"] == STACK,
            fmt_events(r["events"]))
    # 10.5 补丁版普通模式：与原厂逐事件一致
    ok = True
    msgs = []
    for ident in (0, 1, 5, 37, 98):
        ro = probe_o.run(TX4, ident)
        rp = probe_p.run(TX4, ident)
        good = (ro["events"] == rp["events"] == [("wake",)] + BODY4
                and rp["error"] is None)
        ok = ok and good
        msgs.append("ID=%-2d %s" % (ident, "一致" if good else "FAIL"))
    rep.chk("补丁版 TX4 普通模式(ID!=99) 与原厂逐事件一致（单发、无 SYNC 写入）",
            ok, "; ".join(msgs))
    ok = True
    msgs = []
    for ident in (0, 1, 5, 37, 98):
        ro = probe_o.run(TX2, ident)
        rp = probe_p.run(TX2, ident)
        good = (ro["events"] == rp["events"] == [("wake",)] + BODY2
                and rp["error"] is None)
        ok = ok and good
        msgs.append("ID=%-2d %s" % (ident, "一致" if good else "FAIL"))
    rep.chk("补丁版 TX2 普通模式(ID!=99) 与原厂逐事件一致", ok, "; ".join(msgs))
    # 10.6 ID=99 的双发第二帧同步字等于原厂 ID=99 的同步字
    rep.chk("双发第二帧 SYNC=0x%04X 正是原厂 ID=99 同步字 floor(50024/100)=500"
            % ORDINARY_WORD, ORDINARY_WORD == 500
            and SYNC_NUMERATOR // (DEFAULT_ID + 1) == ORDINARY_WORD,
            "50024//100 = %d" % (SYNC_NUMERATOR // (DEFAULT_ID + 1)))
    # 10.7 双发确实多一次发射（时序变化风险）
    ro = probe_o.run(TX2, DEFAULT_ID)
    rp = probe_p.run(TX2, DEFAULT_ID)
    n_fifo_o = sum(1 for e in ro["events"] if e[0] == "fifo")
    n_fifo_p = sum(1 for e in rp["events"] if e[0] == "fifo")
    rep.chk("双发使 2 字节短命令每帧发射次数 %d -> %d（时序变化已记录为风险）"
            % (n_fifo_o, n_fifo_p), n_fifo_o == 1 and n_fifo_p == 2)

    # =====================================================================
    nfail = rep.summary()
    return 0 if nfail == 0 else 1


def _group(sorted_offsets):
    groups = []
    for d in sorted_offsets:
        if groups and d == groups[-1][-1] + 1:
            groups[-1].append(d)
        else:
            groups.append([d])
    return groups


def _enc_bw(addr: int, target: int) -> bytes:
    """独立实现 Thumb-2 B.W 编码（用于核对入口 4 字节）。"""
    off = target - (addr + 4)
    s = 1 if off < 0 else 0
    imm = off & 0x1FFFFFF
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    imm10 = (imm >> 12) & 0x3FF
    imm11 = (imm >> 1) & 0x7FF
    j1 = (1 - i1) ^ s
    j2 = (1 - i2) ^ s
    return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                       0x9000 | (j1 << 13) | (j2 << 11) | imm11)


if __name__ == "__main__":
    try:
        sys.exit(main())
    except Exception:  # noqa: BLE001
        traceback.print_exc()
        sys.exit(2)
