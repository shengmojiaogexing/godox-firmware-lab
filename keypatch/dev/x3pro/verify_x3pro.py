# -*- coding: utf-8 -*-
"""X3 Pro（X3pro_C_v1.22_app.bin）私有密钥补丁 v3「长按开关」版（task-7 最终版）—— 独立验证脚本。

用途
----
对 keypatch/devices/x3pro.py（v3：物理按键长按开关）生成的**最终**成品做**独立**复核：
不 import keypatch.devices.x3pro、不引用其 build_injection()/verify() 输出，全部从
固件字节 + capstone 反汇编 + Unicorn 调用级执行重新推导。

覆盖的独立事实
--------------
1. 镜像基址 0x08020000（向量表 / Reset_Handler / SystemInit VTOR 写入）。
2. 同步字函数 0x0802F040 边界 / 原厂公式 / reg4/5 写入 / SCAL-SIDLE 序列。
3. 全镜像写 CC2500 reg4/reg5 的唯一性（write_reg r0 常量回溯）。
4. 设置结构 0x20001768：[+1]=ID（TX 第二帧同步字动态来源）。
5. 发送入口 0x0802EDC4 / 0x0802EE2C 及其静态引用点。
6. 按键扫描入口 0x08027E44：原厂 push.w 字节、main 调用点、PA6(mask 0x40) 链路。
7. KEY_ON / 计数 / latch 三个 RAM 地址在静态扫描中无字面量引用（空白区）。
8. 成品 vs 原厂差异恰 369 B（6 个补丁点 + 注入区 356 B）。
9. 注入区完整反汇编：轮询 GPIO -> 计数/翻转逻辑、TX 双发包装、6 个蹦床顺序、
   push/pop 栈配对、KEY_ON 读写、PA6 mask。
10. 原厂 PA6 长按关机：0x0802830A `cmp r3,#0x21` -> `#0xFF`，动作块 0x08028310
    变为不可达（无其他分支目标）；并记录镜像中第二处 `cmp r3,#0x21`。
11. 空闲闪存 / 注入窗口引用扫描。
12. Unicorn 调用级语义：KEY_ON=0/1 下 set_sync、TX4/TX2 单发/双发（第二帧同步字
    = floor(50024/(当前ID+1))，动态）、原厂镜像自证、按键 5 用例 + 34 次长按翻转序列。

运行
----
    tools\\python\\python.exe keypatch\\dev\\x3pro\\verify_x3pro.py
    tools\\python\\python.exe keypatch\\dev\\x3pro\\verify_x3pro.py --image %TEMP%\\x3pro_indep2\\X3pro_C_v1.22_app_keyD798.bin

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
from capstone.arm import (ARM_REG_PC, ARM_REG_LR, ARM_REG_SP, ARM_REG_R0,
                          ARM_REG_R1, ARM_REG_R2, ARM_REG_R3, ARM_REG_R4,
                          ARM_REG_R5, ARM_REG_R6, ARM_REG_R7, ARM_REG_R8,
                          ARM_REG_R9, ARM_REG_R10, ARM_REG_R11, ARM_REG_R12)

# ---------------------------------------------------------------------------
# 路径 / 期望值（来自 task-4；脚本独立核对它们）
# ---------------------------------------------------------------------------
_HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(_HERE, "..", "..", ".."))

ORIG_REL = "devices/x3pro/firmware/X3pro_C_v1.22_app.bin"
PATCH_REL = "keypatch/out/X3pro_C_v1.22_app_keyD798.bin"

BASE = 0x08020000
EXPECT_SIZE = 1_941_020
EXPECT_ORIG_SHA = "2e2b8a3b5ff70e62c4acddc406b055ea2625e4833a213e81863f96c0da5a911b"
EXPECT_PATCH_SHA = "11f58c71e83192fdf6dabd15d541602f391c9db6991bcbf49daf8db4156e4ae6"

KEY = 0xD798
SYNC_NUMERATOR = 0xC368          # 50024
SYNC_FN = 0x0802F040
SYNC_PATCH_ADDR = 0x0802F046
SYNC_CONT = 0x0802F050
SYNC_FN_LEN = 0x5A
SYNC_NEXT = 0x0802F09C
TX4 = 0x0802EDC4
TX2 = 0x0802EE2C
TX4_RESUME = 0x0802EDCA
TX2_RESUME = 0x0802EE32
KEYSCAN_FN = 0x08027E44
KEYSCAN_RESUME = 0x08027E48
KEYSCAN_ORIG_PUSH = bytes.fromhex("2de9f84f")   # push.w {r3..r11,lr}
MAIN_KEYSCAN_CALL = 0x0802B282
LONG_CMP_ADDR = 0x0802830A
LONG_CMP_ORIG = bytes.fromhex("212b")
LONG_CMP_PATCH = bytes.fromhex("ff2b")
LONG_CMP2_ADDR = 0x080284F2          # 第二处 PA6 长按闸门（状态 0x14 分支）
LONG_CMP2_ORIG = bytes.fromhex("212b")
LONG_CMP2_PATCH = bytes.fromhex("ff2b")
LONG_ACTION_ADDR = 0x08028310
LONG_ACTION2_ADDR = 0x080284FA
SHUTDOWN_FN = 0x0802B7D0

WRITE_REG = 0x0802E71C
STROBE = 0x0802E88C
DELAY_US = 0x0802B5DC
RADIO_WAKE = 0x0802EC64
GPIO_WRITE = 0x08032A64
GPIO_READ = 0x08032A58

SETTINGS = 0x20001768
KEY_ON = 0x20001096
BTN_CNT = 0x20001098
BTN_LATCH = 0x2000109A
BTN_GPIO = 0x42020000
BTN_MASK = 0x40
BTN_TICKS = 34

BUSY_FLAG = 0x20001D09
INJ_BASE = 0x081C0000
INJ_WINDOW = 0x800
INJ_CODE_LEN = 356
FREE_LO, FREE_HI = 0x081B0A30, 0x081F4000
SENTINEL = 0x20008000
STACK = 0x2000FF00
UC_FLASH = 0x08000000
UC_FLASH_SIZE = 0x00200000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x00010000

md_lite = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True


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
    if ins is None or not ins.mnemonic.startswith("b"):
        return None
    for op in ins.operands:
        if op.type == CS_OP_IMM:
            return op.imm
    return None


def iter_branches(data: bytes):
    """逐 2 字节偏移保守扫描分支 [(src, mnemonic, target)]（含数据区伪分支）。"""
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
    out = []
    n = len(data)
    for i in range(0, n - 3):
        v = struct.unpack("<I", data[i:i + 4])[0]
        if lo <= (v & ~1) < hi:
            out.append((BASE + i, v))
    return out


def resolve_literal(data: bytes, ins):
    if ins is None or not ins.mnemonic.startswith("ldr") or len(ins.operands) < 2:
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
    """回看常量传播求 write_reg 调用点的 r0（选能连续解码到调用点的最长窗口）。"""
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
    for ins in seq:
        if ins.address >= call_addr:
            break
        ops = ins.operands
        mn = ins.mnemonic
        if not ops:
            continue
        d0 = ops[0]
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
        elif mn.startswith("bfi") or mn in ("lsls", "lsrs", "ands", "orrs", "eors", "uxtb"):
            if d0.type == CS_OP_REG:
                regs[d0.reg] = UNK
        elif mn.startswith("ldr") or mn.startswith("ldrb") or mn.startswith("ldrh"):
            if d0.type == CS_OP_REG:
                regs[d0.reg] = UNK
        elif mn.startswith("bl") or mn == "blx":
            for r in (ARM_REG_R0, ARM_REG_R1, ARM_REG_R2, ARM_REG_R3, ARM_REG_R12, ARM_REG_LR):
                regs[r] = UNK
        elif d0.type == CS_OP_REG and not (mn.startswith("str") or mn.startswith("stm")
                                           or mn in ("cmp", "tst", "cmn")):
            regs[d0.reg] = UNK
    return regs.get(ARM_REG_R0, UNK), False


# ---------------------------------------------------------------------------
# Unicorn 调用级 harness
# ---------------------------------------------------------------------------
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
        elif e[0] == "gpio_read":
            parts.append("gpio_read(0x%08X,0x%X)->%d" % (e[1], e[2], e[3]))
        else:
            parts.append(str(e))
    return " ".join(parts)


class UcProbe:
    """调用级 Unicorn：桩替换 radio/GPIO 辅助函数并记录事件。

    - KEY_ON / 计数 / latch 由调用者预置到 RAM
    - button 模式在 0x08027E48 停止（只验证注入逻辑，不进原厂按键函数体）
    - gpio_pressed=True 时 GPIO_READ 返回 0（PA6 按下）
    """

    def __init__(self, image: bytes):
        from unicorn import (Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                             UC_HOOK_CODE)
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
            settings_id: int | None = None, key_on: int = 0,
            stop_at: int | None = None, gpio_pressed: bool = False,
            ram: dict | None = None, max_insn: int = 200_000) -> dict:
        from unicorn.arm_const import UC_ARM_REG_SP as _SP
        uc = self._Uc(*self._arch)
        uc.mem_map(UC_FLASH, UC_FLASH_SIZE)
        uc.mem_map(UC_RAM, UC_RAM_SIZE)
        uc.mem_write(BASE, self.image)
        uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
        sid = ident if settings_id is None else settings_id
        uc.mem_write(SETTINGS, bytes([1, sid & 0xFF, 2, 3]))
        uc.mem_write(BUSY_FLAG, b"\x00")
        uc.mem_write(KEY_ON, bytes([key_on & 0xFF]))
        uc.mem_write(BTN_CNT, b"\x00\x00")
        uc.mem_write(BTN_LATCH, b"\x00")
        uc.mem_write(0x20001D14, fifo + b"\x00" * (8 - len(fifo)))
        for addr, val in (ram or {}).items():
            uc.mem_write(addr, val)

        events: list[tuple] = []
        state = {"error": None, "hit_stop": False}

        def hook(_uc, address, _size, _user):
            a = address & ~1
            try:
                if stop_at is not None and a == (stop_at & ~1):
                    state["hit_stop"] = True
                    _uc.emu_stop()
                    return
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
                elif a == GPIO_WRITE:
                    pass
                elif a == GPIO_READ:
                    port = _uc.reg_read(self.R0)
                    mask = _uc.reg_read(self.R1)
                    pressed = bool(gpio_pressed) if mask == BTN_MASK else False
                    val = 0 if pressed else 1
                    events.append(("gpio_read", port, mask, val))
                    _uc.reg_write(self.R0, val)
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
        uc.reg_write(self.CPSR, 0x01000000)
        try:
            uc.emu_start(entry | 1, 0, count=max_insn)
        except Exception as exc:  # noqa: BLE001
            state["error"] = "%s (pc=0x%08X lr=0x%08X)" % (
                exc, uc.reg_read(self.PC), uc.reg_read(self.LR))
        sp_after = uc.reg_read(self.SP)
        out = {"events": events, "error": state["error"], "sp": sp_after,
               "hit_stop": state["hit_stop"],
               "key_on": uc.mem_read(KEY_ON, 1)[0],
               "cnt": struct.unpack("<H", bytes(uc.mem_read(BTN_CNT, 2)))[0],
               "latch": uc.mem_read(BTN_LATCH, 1)[0]}
        return out


def sync_word_of(events):
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
            print("  %-46s PASS %2d / %2d" % (s, sum(1 for r in rows if r[2]), len(rows)))
        for sec, name, ok, _detail in self.rows:
            if not ok:
                print("  FAIL: [%s] %s" % (sec, name))
        print("\n总计: PASS=%d  FAIL=%d" % (npass, nfail))
        print("结论:", "PASS —— 全部独立检查通过" if nfail == 0 else "FAIL —— 存在不一致项")
        return nfail


def _group(sorted_offsets):
    groups = []
    for d in sorted_offsets:
        if groups and d == groups[-1][-1] + 1:
            groups[-1].append(d)
        else:
            groups.append([d])
    return groups


def _enc_bw(addr: int, target: int) -> bytes:
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


BODY4 = [("fifo", 4), ("strobe", 0x35), ("delay", 600),
         ("strobe", 0x3B), ("strobe", 0x36)]
BODY2 = [("delay", 40), ("fifo", 2), ("strobe", 0x35), ("delay", 500),
         ("strobe", 0x3B), ("strobe", 0x36)]


def main(argv=None) -> int:
    ap = argparse.ArgumentParser(description="X3 Pro v3（长按开关）私有密钥补丁独立验证")
    ap.add_argument("--orig", default=os.path.join(ROOT, ORIG_REL))
    ap.add_argument("--image", default=os.path.join(ROOT, PATCH_REL))
    ap.add_argument("--key", default="0xD798")
    args = ap.parse_args(argv)

    rep = Report()
    key = int(args.key, 16)
    print("X3 Pro v3「长按开关」最终版（task-7）私有密钥补丁 —— 独立验证")
    print("原厂 : %s" % args.orig)
    print("成品 : %s" % args.image)
    print("密钥 : 0x%04X   KEY_ON=1 时 KEY 帧 + floor(50024/(当前ID+1)) 帧双发" % key)

    # =====================================================================
    rep.section("1. 输入镜像 / 哈希")
    try:
        orig = load(args.orig)
        patched = load(args.image)
    except OSError as exc:
        print("无法读取输入: %r" % (exc,))
        return 2
    rep.chk("原厂镜像大小 %d B" % len(orig), len(orig) == EXPECT_SIZE, "实际 %d" % len(orig))
    rep.chk("原厂镜像 sha256", sha256(orig) == EXPECT_ORIG_SHA, sha256(orig))
    rep.chk("成品镜像大小不变（原位补丁）", len(patched) == len(orig), "成品 %d B" % len(patched))
    rep.chk("成品 sha256 与 task-7 最终版一致", sha256(patched) == EXPECT_PATCH_SHA,
            sha256(patched))
    if os.path.abspath(args.image) != os.path.abspath(os.path.join(ROOT, PATCH_REL)):
        rep.chk("--image 与 keypatch/out 成品逐字节一致",
                load(os.path.join(ROOT, PATCH_REL)) == patched, "外部镜像对比")

    # =====================================================================
    rep.section("2. 镜像基址判定（0x08020000）")
    vec = [word(orig, BASE + 4 * i) for i in range(16)]
    sp0 = vec[0]
    rep.chk("向量表 [0] 初值 SP=0x%08X 落在 SRAM 区" % sp0,
            0x20000000 <= sp0 < 0x40000000, "vec[0]=0x%08X" % sp0)
    rep.chk("向量表 [1] Reset 向量为 Thumb 地址 0x%08X" % vec[1], (vec[1] & 1) == 1)
    reset = vec[1] & ~1
    rep.chk("Reset 地址 - 基址 == 0x11344", reset - BASE == 0x11344,
            "实际 0x%X" % (reset - BASE))
    reset_ins = dis_range(orig, reset, 0x10)
    rep.chk("offset 0x11344 处是 Reset_Handler（首条 ldr.w sp,[pc,#..]）",
            bool(reset_ins) and reset_ins[0].mnemonic == "ldr.w"
            and reset_ins[0].op_str.startswith("sp,"),
            ins_text(reset_ins[0]) if reset_ins else "反汇编失败")
    lit_sp_target, lit_sp = resolve_literal(orig, reset_ins[0])
    rep.chk("Reset_Handler 载入的初值 SP 与向量表 [0] 一致", lit_sp == sp0,
            "字面量@0x%08X = 0x%08X" % (lit_sp_target or 0, lit_sp or 0))
    sysinit = None
    for ins in dis_range(orig, reset, 0x40):
        if ins.mnemonic == "bl":
            sysinit = branch_imm(ins)
            break
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
                if scb_val and base_val and m.mem.base == scb_val[0] and m.mem.disp == 8 \
                        and ins.operands[0].type == CS_OP_REG \
                        and ins.operands[0].reg == base_val[0]:
                    str_ok = True
        vtor_ok = bool(scb_val and base_val and str_ok)
        if scb_val and base_val:
            vtor_detail = ("SCB 字面量@0x%08X；基址字面量@0x%08X；str [SCB,#8]（VTOR）= 基址：%s"
                           % (scb_val[2], base_val[2], "是" if str_ok else "否"))
    rep.chk("系统初始化把 SCB->VTOR 写成 0x08020000（应用向量表基址铁证）", vtor_ok, vtor_detail)
    lits = []
    for ins in dis_range(orig, reset, 0x40):
        if ins.mnemonic.startswith("ldr") and len(ins.operands) >= 2 \
                and ins.operands[1].type == CS_OP_MEM and ins.operands[1].mem.base == ARM_REG_PC:
            lits.append(resolve_literal(orig, ins)[1])
    src = [v for v in lits if v is not None and BASE <= v < BASE + len(orig)]
    rep.chk("Reset_Handler 的 .data 源地址落在镜像内: %s" % (["0x%08X" % v for v in src],), bool(src))
    dflt = vec[7] & ~1
    dflt_ins = dis_one(orig, dflt)
    rep.chk("向量表 [7] 指向默认 handler 0x08031394 且为 b .", dflt == 0x08031394
            and dflt_ins is not None and dflt_ins.mnemonic == "b"
            and dflt_ins.op_str == "#0x8031394", ins_text(dflt_ins) if dflt_ins else "无")
    vec_bad = []
    for i, v in enumerate(vec):
        if i == 0 or v == 0:
            continue
        if not ((v & 1) == 1 and BASE <= (v & ~1) < BASE + len(orig)):
            vec_bad.append((i, v))
    rep.chk("所有非零向量表项（[0] 为 SP）均为 Thumb 奇数地址且落在镜像内", not vec_bad,
            "异常项 %r" % (vec_bad,))
    base_lits = [a for a in range(BASE, BASE + len(orig) - 3, 4) if word(orig, a) == BASE]
    rep.chk("镜像 4 字节对齐字面量中 0x08020000 恰出现 1 次（VTOR 用）", len(base_lits) == 1,
            "位置 %s" % (["0x%08X" % a for a in base_lits],))

    # =====================================================================
    rep.section("3. 同步字函数 0x0802F040（边界 / 公式 / 序列）")
    fn = dis_range(orig, SYNC_FN, SYNC_FN_LEN + 0x10)
    rep.chk("函数头 = movs r2,#1; mov r3,r0; push {r4,lr}",
            len(fn) >= 3 and fn[0].mnemonic == "movs" and fn[0].op_str == "r2, #1"
            and fn[1].mnemonic == "mov" and fn[1].op_str == "r3, r0"
            and fn[2].mnemonic == "push" and fn[2].op_str == "{r4, lr}",
            " / ".join(ins_text(i) for i in fn[:3]))
    po = off(SYNC_PATCH_ADDR)
    rep.chk("0x0802F046 处原厂 10 字节 = movw r4,#0xC368; add r3,r2; udiv r4,r4,r3",
            orig[po:po + 10] == bytes.fromhex("4cf268341344b4fbf3f4"),
            "原厂原字节 " + orig[po:po + 10].hex(" "))
    f10 = dis_range(orig, SYNC_PATCH_ADDR, 10)
    rep.chk("10 字节反汇编 = movw/add/udiv（常量 0xC368，r2=1 -> r3=ID+1）",
            [ins_text(i) for i in f10[:3]] == ["movw r4, #0xc368", "add r3, r2",
                                               "udiv r4, r4, r3"],
            " / ".join(ins_text(i) for i in f10[:3]))
    tail = [i for i in fn if i.address < SYNC_FN + SYNC_FN_LEN]
    last = tail[-1]
    rep.chk("函数以 b.w delay_us 尾调用结束（0x%08X）" % last.address,
            last.mnemonic == "b.w" and branch_imm(last) == DELAY_US, ins_text(last))
    nxt = dis_one(orig, SYNC_NEXT)
    rep.chk("最后指令止于 0x%08X（长 0x5A），2B NOP 对齐后下一函数 0x%08X 以 push 开头"
            % (SYNC_FN + SYNC_FN_LEN, SYNC_NEXT),
            nxt is not None and nxt.mnemonic == "push"
            and orig[off(SYNC_FN + SYNC_FN_LEN):off(SYNC_FN + SYNC_FN_LEN) + 2] == bytes.fromhex("00bf"),
            ins_text(nxt) if nxt else "无")
    seq_ok = True
    seq_txt = []
    checks = [
        (0x0802F058, "movs r0, #4"), (0x0802F05E, "bl #0x802e71c"),
        (0x0802F068, "movs r0, #5"), (0x0802F06E, "bl #0x802e71c"),
        (0x0802F072, "movs r0, #0x14"), (0x0802F078, "movs r0, #0x33"),
        (0x0802F07E, "mov.w r0, #0x3e8"), (0x0802F086, "movs r0, #0x36"),
        (0x0802F08C, "mov.w r0, #0x2bc"),
    ]
    for addr, text in checks:
        i = dis_one(orig, addr)
        seq_txt.append("%s:%s" % ("0x%08X" % addr, ins_text(i) if i else "?"))
        if i is None or ins_text(i) != text:
            seq_ok = False
    rep.chk("写 reg4(0x0802F05E)/reg5(0x0802F06E) + delay20/SCAL33/1ms/SIDLE36/700us 序列",
            seq_ok, " ; ".join(seq_txt))

    # =====================================================================
    rep.section("4. 按键扫描入口 0x08027E44 / PA6 链路 / KEY_ON RAM")
    rep.chk("原厂 0x08027E44 4 字节 = push.w {r3..r11,lr}（%s）"
            % KEYSCAN_ORIG_PUSH.hex(" "), orig[off(KEYSCAN_FN):off(KEYSCAN_FN) + 4] == KEYSCAN_ORIG_PUSH,
            "实际 " + orig[off(KEYSCAN_FN):off(KEYSCAN_FN) + 4].hex(" "))
    rep.chk("成品 0x08027E44 = B.W -> inj_btn(0x081C0070)",
            patched[off(KEYSCAN_FN):off(KEYSCAN_FN) + 4] == _enc_bw(KEYSCAN_FN, 0x081C0070)
            and branch_imm(dis_one(patched, KEYSCAN_FN)) == 0x081C0070,
            "字节 " + patched[off(KEYSCAN_FN):off(KEYSCAN_FN) + 4].hex(" "))
    rep.chk("恢复点 0x08027E48 = 原 push.w 之后的原函数体起点（原厂字节 ldr r2,[pc,#..]）",
            dis_one(orig, KEYSCAN_RESUME) is not None
            and dis_one(orig, KEYSCAN_RESUME).mnemonic == "ldr",
            ins_text(dis_one(orig, KEYSCAN_RESUME)) if dis_one(orig, KEYSCAN_RESUME) else "?")
    branches_orig = iter_branches(orig)
    ks_callers = [(s, mn) for (s, mn, t) in branches_orig if t == KEYSCAN_FN]
    rep.chk("按键扫描入口静态调用点恰 1 处：main 0x%08X bl 0x08027E44" % MAIN_KEYSCAN_CALL,
            ks_callers == [(MAIN_KEYSCAN_CALL, "bl")],
            "实际 %s" % ([(hex(s), mn) for s, mn in ks_callers],))
    main_ins = dis_one(orig, MAIN_KEYSCAN_CALL)
    rep.chk("main 主循环 0x0802B282 调用按键扫描（bl 目标 = 0x08027E44）",
            main_ins is not None and main_ins.mnemonic == "bl"
            and branch_imm(main_ins) == KEYSCAN_FN, ins_text(main_ins) if main_ins else "?")
    # PA6 链路：读 GPIOA mask 0x40 -> r4 bit0；0x08027F34 bmi -> PA6 保持块
    pa6_read_ok = False
    gpio_lit_ok = False
    for ins in dis_range(orig, 0x08027E5C, 0x10):
        _t, val = resolve_literal(orig, ins)
        if ins.mnemonic == "movs" and ins.op_str == "r1, #0x40":
            pa6_read_ok = True
        if val == BTN_GPIO:
            gpio_lit_ok = True
    rep.chk("原厂按键扫描读 PA6：movs r1,#0x40 + 字面量 GPIOA 0x42020000", pa6_read_ok and gpio_lit_ok)
    hold = dis_range(orig, 0x08027F34, 8)
    rep.chk("PA6 保持分支：lsls r3,r4,#0x1f; bmi.w 0x080282D6（r4 bit0=PA6）",
            len(hold) >= 2 and hold[0].mnemonic == "lsls" and hold[0].op_str == "r3, r4, #0x1f"
            and hold[1].mnemonic == "bmi.w" and branch_imm(hold[1]) == 0x080282D6,
            " / ".join(ins_text(i) for i in hold[:2]))
    rep.chk("PA6 保持块：计数器 0x20000F92 递增后 cmp #0x21，超阈值落入动作块 0x08028310",
            ins_text(dis_one(orig, 0x080282FC)) == "adds r3, #1"
            and dis_one(orig, 0x080282FE).mnemonic == "uxtb"
            and ins_text(dis_one(orig, 0x0802830A)) == "cmp r3, #0x21"
            and dis_one(orig, 0x0802830C).mnemonic == "bls.w"
            and branch_imm(dis_one(orig, 0x0802830C)) == 0x08027FEE,
            "adds/uxtb/cmp/bls 链")
    # KEY_ON 三个 RAM 地址：原厂无字面量引用 + 落在 BSS 内
    keyon_refs = []
    for v in (KEY_ON, BTN_CNT, BTN_LATCH):
        keyon_refs.append((v, [BASE + i for i in range(0, len(orig) - 3)
                               if struct.unpack("<I", orig[i:i + 4])[0] == v]))
    rep.chk("KEY_ON/计数/latch 三地址在原厂全镜像无字面量引用（静态空白区）",
            all(not r for _v, r in keyon_refs),
            "; ".join("0x%08X:%s" % (v, ["0x%08X" % a for a in r[:4]]) for v, r in keyon_refs))
    bss_lo = word(orig, 0x0803138C)
    bss_hi = word(orig, 0x08031390)
    rep.chk("KEY_ON..latch 落在 .bss（0x%08X..0x%08X）内且三字节连续" % (bss_lo, bss_hi),
            bss_lo <= KEY_ON < BTN_CNT < BTN_LATCH < bss_hi
            and BTN_CNT == KEY_ON + 2 and BTN_LATCH == KEY_ON + 4,
            "KEY_ON=0x%08X +2 count +4 latch" % KEY_ON)

    # =====================================================================
    rep.section("5. 全镜像写 CC2500 reg4/reg5 的唯一性")
    wr_sites = sorted({s for (s, _mn, t) in branches_orig if t == WRITE_REG})
    regs, unresolved = [], []
    for s in wr_sites:
        val, _ = trace_r0(orig, s)
        (regs.append((s, val)) if isinstance(val, int) else unresolved.append(s))
    thunk_site = 0x0802ECC4
    unresolved = [s for s in unresolved if s != thunk_site]
    reg45 = [s for s, v in regs if v in (4, 5)]
    reg44 = [s for s, v in regs if v == 0x44]
    rep.chk("write_reg 调用点 %d 个，r0 常量回溯 %d 个（1 个字节写 thunk 另行核对）"
            % (len(wr_sites), len(regs)), len(wr_sites) == 80 and not unresolved,
            "无法回溯 %s" % (["0x%08X" % s for s in unresolved],))
    rep.chk("写 reg4/reg5 的调用点恰 4 处（初始化表 2 + 同步字函数 2）",
            sorted(reg45) == [0x0802E950, 0x0802E962, 0x0802F05E, 0x0802F06E],
            "实际 %s" % (["0x%08X" % s for s in reg45],))
    rep.chk("原厂无 burst 写 reg0x44", not reg44,
            "0x44 调用点 %s" % (["0x%08X" % s for s in reg44],))
    thunk_callers = [s for (s, _mn, t) in branches_orig if t == 0x0802ECB4]
    thunk_vals = []
    for s in thunk_callers:
        v = None
        for i in dis_range(orig, s - 8, 8):
            if i.mnemonic in ("movs", "mov") and i.op_str.startswith("r0, #"):
                v = int(i.op_str.split("#")[1], 16)
        thunk_vals.append((s, v))
    rep.chk("字节写 thunk 0x0802ECB4 的调用点也不写 reg4/5（r0=[0x18,6]）",
            sorted(v for _s, v in thunk_vals) == [6, 0x18],
            "%s" % (["0x%08X:r0=%r" % (s, v) for s, v in thunk_vals],))
    # 初始化表写入值
    def init_value(call_addr):
        for i in reversed(dis_range(orig, call_addr - 0x10, 0x10)):
            if i.mnemonic == "strb.w" and i.op_str.startswith("r2"):
                for j in dis_range(orig, i.address - 0x10, 0x10):
                    if j.address < i.address and j.mnemonic == "movs" and j.op_str.startswith("r2,"):
                        return int(j.op_str.split("#")[1], 16)
        return None
    rep.chk("初始化表 reg4 <- 0xC3、reg5 <- 0x68",
            init_value(0x0802E950) == 0xC3 and init_value(0x0802E962) == 0x68,
            "reg4=0x%02X reg5=0x%02X" % (init_value(0x0802E950) or 0, init_value(0x0802E962) or 0))
    branches_patch = iter_branches(patched)
    wr_sites_p = sorted({s for (s, _mn, t) in branches_patch if t == WRITE_REG})
    new_sites = sorted(set(wr_sites_p) - set(wr_sites))
    inj44 = [s for s in new_sites if trace_r0(patched, s)[0] == 0x44]
    rep.chk("成品新增唯一 write_reg 调用点 = 注入区 set_sync（reg=0x44 burst）",
            new_sites == [0x081C00F2] and inj44 == [0x081C00F2],
            "新增 %s" % (["0x%08X" % s for s in new_sites],))

    # =====================================================================
    rep.section("6. 发送入口 0x0802EDC4 / 0x0802EE2C")
    body4 = dis_range(orig, TX4, 0x60)
    body2 = dis_range(orig, TX2, 0x60)
    rep.chk("TX4 入口 = push {r4,lr}; bl radio_wake(0x0802EC64)",
            len(body4) >= 2 and body4[0].mnemonic == "push" and body4[0].op_str == "{r4, lr}"
            and body4[1].mnemonic == "bl" and branch_imm(body4[1]) == RADIO_WAKE,
            " / ".join(ins_text(i) for i in body4[:2]))
    rep.chk("TX2 入口 = push {r4,lr}; bl radio_wake",
            len(body2) >= 2 and body2[0].mnemonic == "push"
            and body2[1].mnemonic == "bl" and branch_imm(body2[1]) == RADIO_WAKE)
    checks4 = [(0x0802EDE0, "movs r2, #4"), (0x0802EDE2, "ldr r1, [pc, #0x40]"),
               (0x0802EDE4, "movs r0, #0x7f"), (0x0802EDE6, "bl #0x802e71c"),
               (0x0802EDEA, "movs r0, #0x35"), (0x0802EDF0, "mov.w r0, #0x258"),
               (0x0802EDF8, "movs r0, #0x3b"), (0x0802EDFE, "movs r0, #0x36"),
               (0x0802EE1E, "pop {r4, pc}")]
    ok = all((dis_one(orig, a) is not None and ins_text(dis_one(orig, a)) == t)
             for a, t in checks4)
    rep.chk("TX4 函数体：FIFO(0x7F,4B) -> STX(0x35) -> 600us -> SFTX(0x3B) -> SIDLE(0x36)",
            ok, "0x0802EDE0=movs r2,#4 ; 0x0802EDE6=bl write_reg ; 0x0802EE1E=pop")
    checks2 = [(0x0802EE48, "movs r0, #0x28"), (0x0802EE4E, "movs r2, #2"),
               (0x0802EE50, "ldr r1, [pc, #0x40]"), (0x0802EE52, "movs r0, #0x7f"),
               (0x0802EE54, "bl #0x802e71c"), (0x0802EE58, "movs r0, #0x35"),
               (0x0802EE5E, "mov.w r0, #0x1f4"), (0x0802EE66, "movs r0, #0x3b"),
               (0x0802EE6C, "movs r0, #0x36"), (0x0802EE8C, "pop {r4, pc}")]
    ok2 = all((dis_one(orig, a) is not None and ins_text(dis_one(orig, a)) == t)
              for a, t in checks2)
    rep.chk("TX2 函数体：40us -> FIFO(0x7F,2B) -> STX -> 500us -> SFTX -> SIDLE", ok2,
            "0x0802EE48=movs r0,#0x28 ; 0x0802EE54=bl write_reg ; 0x0802EE8C=pop")
    n4_bl = sum(1 for (_s, mn, t) in branches_orig if t == TX4 and mn == "bl")
    n4_bw = sum(1 for (_s, mn, t) in branches_orig if t == TX4 and mn == "b.w")
    n2_bl = sum(1 for (_s, mn, t) in branches_orig if t == TX2 and mn == "bl")
    n2_bw = sum(1 for (_s, mn, t) in branches_orig if t == TX2 and mn == "b.w")
    rep.chk("TX4 静态引用：%d BL + %d B.W 尾跳（总 %d）" % (n4_bl, n4_bw, n4_bl + n4_bw),
            n4_bl == 16 and n4_bw == 2)
    rep.chk("TX2 静态引用：%d BL + %d B.W 尾跳（总 %d）" % (n2_bl, n2_bw, n2_bl + n2_bw),
            n2_bl == 9 and n2_bw == 1)
    dead = all(patched[off(e) + 4:off(e) + 6] == orig[off(e) + 4:off(e) + 6]
               for e in (TX4, TX2))
    rep.chk("TX4/TX2 入口覆盖后残留 2 字节 = 原 bl 后半（死字节，未改写）", dead)

    # =====================================================================
    rep.section("7. 成品差异范围（6 区域 / 7 改动位置 + 注入区）")
    diff = {i for i in range(len(orig)) if orig[i] != patched[i]}
    inj_o = off(INJ_BASE)
    seg = patched[inj_o:inj_o + INJ_WINDOW]
    last = max((i for i, b in enumerate(seg) if b != 0xFF), default=-1)
    code_len = last + 1
    rep.chk("注入区实际代码长度 %d 字节（0x%X，原厂全 0xFF）" % (code_len, code_len),
            code_len == INJ_CODE_LEN, "预留窗口 0x%X" % INJ_WINDOW)
    rep.chk("原厂注入窗口 0x081C0000..0x081C0800 全 0xFF",
            orig[inj_o:inj_o + INJ_WINDOW] == b"\xff" * INJ_WINDOW)
    rep.chk("成品注入窗口剩余 %d 字节保持 0xFF" % (INJ_WINDOW - code_len),
            seg[code_len:] == b"\xff" * (INJ_WINDOW - code_len))
    expected = set(range(off(KEYSCAN_FN), off(KEYSCAN_FN) + 4)) \
        | {off(LONG_CMP_ADDR), off(LONG_CMP2_ADDR)} \
        | set(range(off(TX4), off(TX4) + 4)) \
        | set(range(off(TX2), off(TX2) + 4)) \
        | set(range(off(SYNC_PATCH_ADDR), off(SYNC_PATCH_ADDR) + 10)) \
        | {inj_o + i for i in range(code_len) if patched[inj_o + i] != 0xFF}
    rep.chk("与原厂差异字节数 = %d（预期 %d）" % (len(diff), len(expected)), diff == expected,
            "差异区间 %s" % (["0x%05X..0x%05X" % (g[0], g[-1])
                            for g in _group(sorted(diff))],))
    # 原厂字节核对
    for addr, nbytes, want in ((TX4, 4, bytes.fromhex("10b5fff7")),
                               (TX2, 4, bytes.fromhex("10b5fff7")),
                               (SYNC_PATCH_ADDR, 10, bytes.fromhex("4cf268341344b4fbf3f4")),
                               (KEYSCAN_FN, 4, KEYSCAN_ORIG_PUSH),
                               (LONG_CMP_ADDR, 2, LONG_CMP_ORIG),
                               (LONG_CMP2_ADDR, 2, LONG_CMP2_ORIG)):
        o = off(addr)
        rep.chk("0x%08X 原厂字节 = %s" % (addr, want.hex(" ")),
                orig[o:o + nbytes] == want, "实际 " + orig[o:o + nbytes].hex(" "))
    for addr, pat in ((LONG_CMP_ADDR, LONG_CMP_PATCH), (LONG_CMP2_ADDR, LONG_CMP2_PATCH)):
        rep.chk("0x%08X 只改 1 个字节（21 -> ff；任务书按 2 字节指令描述）" % addr,
                patched[off(addr):off(addr) + 2] == pat
                and sum(1 for i in range(2) if patched[off(addr) + i]
                        != orig[off(addr) + i]) == 1,
                "成品 " + patched[off(addr):off(addr) + 2].hex(" "))
    rep.chk("差异仅落在 6 个补丁区域（7 个改动位置）与注入区代码内（无其它字节变化）",
            diff == expected)

    # =====================================================================
    rep.section("8. 注入区反汇编核对（包装 / 蹦床顺序 / 栈 / KEY_ON / PA6 mask）")
    inj_ins = dis_range(patched, INJ_BASE, code_len)
    rep.chk("注入区线性反汇编覆盖 %d 字节（%d 条指令，无空洞）" % (code_len, len(inj_ins)),
            len(inj_ins) >= 100
            and inj_ins[-1].address + len(inj_ins[-1].bytes) == INJ_BASE + code_len)

    def exp(addr, text):
        i = dis_one(patched, addr)
        return i is not None and ins_text(i) == text

    anchors = [
        (0x081C0000, "movw r3, #0x1096"),
        (0x081C0004, "movt r3, #0x2000"),
        (0x081C0008, "ldrb r3, [r3]"),
        (0x081C000A, "cmp r3, #0"),
        (0x081C000C, "beq #0x81c0016"),
        (0x081C000E, "movw r4, #0xd798"),
        (0x081C0012, "b.w #0x802f050"),
        (0x081C0016, "movs r2, #1"),
        (0x081C0018, "mov r3, r0"),
        (0x081C001A, "movw r4, #0xc368"),
        (0x081C001E, "add r3, r2"),
        (0x081C0020, "udiv r4, r4, r3"),
        (0x081C0024, "b.w #0x802f050"),
        (0x081C0028, "push {r4, lr}"),
        (0x081C002A, "movw r3, #0x1096"),
        (0x081C0032, "ldrb r3, [r3]"),
        (0x081C0034, "cmp r3, #0"),
        (0x081C0036, "beq #0x81c0046"),
        (0x081C0038, "bl #0x81c011c"),
        (0x081C003C, "bl #0x81c012e"),
        (0x081C0040, "bl #0x81c00fa"),
        (0x081C0044, "pop {r4, pc}"),
        (0x081C0046, "bl #0x81c012e"),
        (0x081C004A, "pop {r4, pc}"),
        (0x081C004C, "push {r4, lr}"),
        (0x081C004E, "movw r3, #0x1096"),
        (0x081C0056, "ldrb r3, [r3]"),
        (0x081C0058, "cmp r3, #0"),
        (0x081C005A, "beq #0x81c006a"),
        (0x081C005C, "bl #0x81c013c"),
        (0x081C0060, "bl #0x81c014e"),
        (0x081C0064, "bl #0x81c00fa"),
        (0x081C0068, "pop {r4, pc}"),
        (0x081C006A, "bl #0x81c014e"),
        (0x081C006E, "pop {r4, pc}"),
        (0x081C0070, "push {r4, lr}"),
        (0x081C0072, "movw r0, #0"),
        (0x081C0076, "movt r0, #0x4202"),
        (0x081C007A, "movs r1, #0x40"),
        (0x081C007C, "bl #0x8032a58"),
        (0x081C0080, "cmp r0, #0"),
        (0x081C0082, "bne #0x81c00c4"),
        (0x081C0084, "movw r1, #0x109a"),
        (0x081C008C, "ldrb r2, [r1]"),
        (0x081C0090, "bne #0x81c00da"),
        (0x081C0092, "movw r2, #0x1098"),
        (0x081C009A, "ldrh r3, [r2]"),
        (0x081C009C, "adds r3, r3, #1"),
        (0x081C009E, "strh r3, [r2]"),
        (0x081C00A0, "cmp r3, #0x22"),
        (0x081C00A2, "bne #0x81c00da"),
        (0x081C00A4, "movw r3, #0x1096"),
        (0x081C00AC, "ldrb r0, [r3]"),
        (0x081C00AE, "movs r2, #1"),
        (0x081C00B0, "eors r0, r2"),
        (0x081C00B2, "strb r0, [r3]"),
        (0x081C00B4, "movw r1, #0x109a"),
        (0x081C00BC, "movs r0, #1"),
        (0x081C00BE, "strb r0, [r1]"),
        (0x081C00C0, "b.w #0x81c00da"),
        (0x081C00C4, "movw r1, #0x1098"),
        (0x081C00CC, "movs r2, #0"),
        (0x081C00CE, "strh r2, [r1]"),
        (0x081C00D0, "movw r1, #0x109a"),
        (0x081C00D8, "strb r2, [r1]"),
        (0x081C00DA, "bl #0x81c015c"),
        (0x081C00DE, "pop {r4, pc}"),
        (0x081C00E0, "push {r4, lr}"),
        (0x081C00E2, "sub sp, #8"),
        (0x081C00E4, "mov r4, r0"),
        (0x081C00E6, "lsrs r3, r4, #8"),
        (0x081C00E8, "mov r1, sp"),
        (0x081C00EA, "strb r3, [r1]"),
        (0x081C00EC, "strb r4, [r1, #1]"),
        (0x081C00EE, "movs r2, #2"),
        (0x081C00F0, "movs r0, #0x44"),
        (0x081C00F2, "bl #0x802e71c"),
        (0x081C00F6, "add sp, #8"),
        (0x081C00F8, "pop {r4, pc}"),
        (0x081C00FA, "movw r0, #0xd798"),
        (0x081C00FE, "b.w #0x81c00e0"),
        (0x081C0102, "movw r3, #0x1768"),
        (0x081C0106, "movt r3, #0x2000"),
        (0x081C010A, "ldrb r0, [r3, #1]"),
        (0x081C010C, "adds r0, r0, #1"),
        (0x081C010E, "movw r1, #0xc368"),
        (0x081C0112, "udiv r4, r1, r0"),
        (0x081C0116, "mov r0, r4"),
        (0x081C0118, "b.w #0x81c00e0"),
        (0x081C011C, "push {r4, lr}"),
        (0x081C011E, "bl #0x802ec64"),
        (0x081C0122, "movw r0, #0xd798"),
        (0x081C0126, "bl #0x81c00e0"),
        (0x081C012A, "b.w #0x802edca"),
        (0x081C012E, "push {r4, lr}"),
        (0x081C0130, "bl #0x802ec64"),
        (0x081C0134, "bl #0x81c0102"),
        (0x081C0138, "b.w #0x802edca"),
        (0x081C013C, "push {r4, lr}"),
        (0x081C013E, "bl #0x802ec64"),
        (0x081C0142, "movw r0, #0xd798"),
        (0x081C0146, "bl #0x81c00e0"),
        (0x081C014A, "b.w #0x802ee32"),
        (0x081C014E, "push {r4, lr}"),
        (0x081C0150, "bl #0x802ec64"),
        (0x081C0154, "bl #0x81c0102"),
        (0x081C0158, "b.w #0x802ee32"),
        (0x081C015C, "push.w {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}"),
        (0x081C0160, "b.w #0x8027e48"),
    ]
    bad = [(a, t, ins_text(dis_one(patched, a)) if dis_one(patched, a) else None)
           for a, t in anchors if not exp(a, t)]
    rep.chk("注入区 %d 个关键指令锚点（含 6 个包装/蹦床入口与分支目标）逐条一致"
            % len(anchors), not bad, "" if not bad else "不一致 %r" % (bad[:5],))
    # 常量计数
    texts = [ins_text(i) for i in inj_ins]
    rep.chk("KEY=0x%04X：movw r4 1 次 + movw r0 3 次" % key,
            texts.count("movw r4, #0x%04x" % key) == 1
            and texts.count("movw r0, #0x%04x" % key) == 3,
            "r4=%d r0=%d" % (texts.count("movw r4, #0x%04x" % key),
                             texts.count("movw r0, #0x%04x" % key)))
    rep.chk("第二帧同步字动态计算：注入区无固定 0x01F4，set_ord 用 0xC368/(ID+1) 的 udiv",
            texts.count("movw r0, #0x1f4") == 0
            and texts.count("movw r1, #0xc368") == 1
            and texts.count("udiv r4, r1, r0") == 1,
            "0x1F4 固定常量出现 %d 次" % texts.count("movw r0, #0x1f4"))
    rep.chk("KEY_ON 地址 0x20001096 由 movw/movt 4 次访问；计数 0x1098 2 次、latch 0x109A 3 次",
            texts.count("movw r3, #0x1096") == 4
            and texts.count("movw r1, #0x1098") == 1 and texts.count("movw r2, #0x1098") == 1
            and texts.count("movw r1, #0x109a") == 3,
            "1096=%d 1098=%d 109a=%d" % (texts.count("movw r3, #0x1096"),
                                         texts.count("movw r1, #0x1098") + texts.count("movw r2, #0x1098"),
                                         texts.count("movw r1, #0x109a")))
    rep.chk("PA6 读取参数：GPIOA 0x42020000 + mask 0x40（movw r0,#0/movt r0,#0x4202/movs r1,#0x40）",
            texts.count("movt r0, #0x4202") == 1 and texts.count("movs r1, #0x40") == 1
            and texts.count("movw r0, #0") == 1)
    rep.chk("长按阈值 34（cmp #0x22）与翻转指令（ldrb/movs 1/eors/strb）齐备",
            texts.count("cmp r3, #0x22") == 1 and texts.count("eors r0, r2") == 1
            and texts.count("ldrb r0, [r3]") == 1)
    rep.chk("注入区无关中断指令（cpsid/cpsie，属残余风险记录）",
            not any(t.startswith("cps") for t in texts))
    n_push = sum(1 for i in inj_ins if i.mnemonic == "push" and i.op_str == "{r4, lr}")
    n_pop = sum(1 for i in inj_ins if i.mnemonic == "pop" and i.op_str == "{r4, pc}")
    rep.chk("静态栈配对：8 个 push {r4,lr} / 6 个 pop {r4,pc}（每个函数路径平衡）",
            n_push == 8 and n_pop == 6, "push=%d pop=%d" % (n_push, n_pop))
    # 6 个蹦床：4 个 TX 蹦床 + tramp_btn（+ 两个 inj 包装算 6 个路径入口）
    tramp_checks = [
        (0x081C011C, 0x0802EC64, 0x081C00E0, 0x0802EDCA),   # tramp4_key: wake -> set_sync -> TX4
        (0x081C012E, 0x0802EC64, 0x081C0102, 0x0802EDCA),   # tramp4_ord: wake -> set_ord -> TX4
        (0x081C013C, 0x0802EC64, 0x081C00E0, 0x0802EE32),   # tramp2_key
        (0x081C014E, 0x0802EC64, 0x081C0102, 0x0802EE32),   # tramp2_ord
        (0x081C015C, None, None, 0x08027E48),               # tramp_btn: push.w -> resume
    ]
    order_ok = True
    order_msg = []
    for start, wake_t, sync_t, resume_t in tramp_checks:
        ins = dis_range(patched, start, 0x18)
        texts_t = [ins_text(i) for i in ins]
        if wake_t is None:
            ok = (texts_t[0].startswith("push.w")
                  and ("b.w #0x%x" % resume_t) in texts_t)
        else:
            wi = texts_t.index("bl #0x%x" % wake_t) if ("bl #0x%x" % wake_t) in texts_t else -1
            si = texts_t.index("bl #0x%x" % sync_t) if ("bl #0x%x" % sync_t) in texts_t else -1
            ri = next((k for k, x in enumerate(texts_t) if x == "b.w #0x%x" % resume_t), -1)
            ok = wi >= 1 and si > wi and ri > si
        order_ok = order_ok and ok
        order_msg.append("0x%08X:%s" % (start, "OK" if ok else "BAD"))
    rep.chk("蹦床顺序：push -> bl 0x0802EC64(唤醒) -> 写同步字/恢复 -> b.w 原函数体",
            order_ok, "; ".join(order_msg))
    rep.chk("inj_btn 恢复原厂 push.w（10 寄存器）-> b.w 0x08027E48 执行原厂扫描体",
            exp(0x081C015C, "push.w {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}")
            and exp(0x081C0160, "b.w #0x8027e48"))
    rep.chk("TX 包装恢复 KEY：两个包装代码路径均 bl set_key（0x081C00FA）",
            texts.count("bl #0x81c00fa") == 2)
    rep.chk("TX 普通模式也先写原厂同步字再单发（beq -> bl tramp*_ord）",
            texts.count("bl #0x81c012e") == 2 and texts.count("bl #0x81c014e") == 2)

    # =====================================================================
    rep.section("9. 原厂 PA6 长按动作的禁用（两处 cmp 均已改 #0xFF）")
    rep.chk("0x0802830A 原厂 = cmp r3,#0x21，成品 = cmp r3,#0xFF",
            orig[off(LONG_CMP_ADDR):off(LONG_CMP_ADDR) + 2] == LONG_CMP_ORIG
            and patched[off(LONG_CMP_ADDR):off(LONG_CMP_ADDR) + 2] == LONG_CMP_PATCH,
            "原厂 %s -> 成品 %s" % (LONG_CMP_ORIG.hex(" "), LONG_CMP_PATCH.hex(" ")))
    rep.chk("0x080284F2 原厂 = cmp r3,#0x21，成品 = cmp r3,#0xFF（task-7 新增）",
            orig[off(LONG_CMP2_ADDR):off(LONG_CMP2_ADDR) + 2] == LONG_CMP2_ORIG
            and patched[off(LONG_CMP2_ADDR):off(LONG_CMP2_ADDR) + 2] == LONG_CMP2_PATCH,
            "原厂 %s -> 成品 %s" % (LONG_CMP2_ORIG.hex(" "), LONG_CMP2_PATCH.hex(" ")))
    uxtb1 = any(i.mnemonic == "uxtb" and i.op_str == "r3, r3"
                for i in dis_range(orig, 0x080282F8, 0x8))
    bls1 = (dis_one(orig, 0x0802830C) is not None
            and dis_one(orig, 0x0802830C).mnemonic == "bls.w"
            and branch_imm(dis_one(orig, 0x0802830C)) == 0x08027FEE)
    uxtb2 = any(i.mnemonic == "uxtb" and i.op_str == "r3, r3"
                for i in dis_range(orig, 0x080284EA, 0x8))
    bls2 = (dis_one(orig, 0x080284F6) is not None
            and dis_one(orig, 0x080284F6).mnemonic == "bls.w"
            and branch_imm(dis_one(orig, 0x080284F6)) == 0x0802814A)
    rep.chk("两处 r3 均在比较前 uxtb（0..255）；cmp #0xFF 后 bls 恒成立 -> 两动作块均被跳过",
            uxtb1 and bls1 and uxtb2 and bls2,
            "uxtb1=%s bls1=%s / uxtb2=%s bls2->0x0802814A=%s"
            % (uxtb1, bls1, uxtb2, bls2))
    targets_action = [(s, mn) for (s, mn, t) in branches_orig if t == LONG_ACTION_ADDR]
    rep.chk("动作块 0x08028310 在原厂无任何分支目标（唯一入口是 0x0802830A 的 fall-through）",
            not targets_action, "发现 %s" % (targets_action,))
    targets_action2 = [(s, mn) for (s, mn, t) in branches_orig if t == LONG_ACTION2_ADDR]
    rep.chk("动作块 0x080284FA 在原厂无任何分支目标（唯一入口是 0x080284F2 的 fall-through）",
            not targets_action2, "发现 %s" % (targets_action2,))
    # 动作块2 下游（0x0802850A 起）仍可从别处进入，但其不再由「PA6 长按」触发
    rep.chk("动作块 0x080284FA 内部首指令仍为原厂 movs r0,#0xB4（未被执行路径，仅代码保留）",
            orig[off(LONG_ACTION2_ADDR):off(LONG_ACTION2_ADDR) + 2] == bytes.fromhex("b420"))
    targets_shutdown_path = [(s, mn) for (s, mn, t) in branches_orig if t == 0x0802852A]
    rep.chk("关机调用路径 0x0802852A 唯一分支来源 = 0x0802832A（动作块 1 内部）",
            targets_shutdown_path == [(0x0802832A, "beq.w")],
            "实际 %s" % ([(hex(s), mn) for s, mn in targets_shutdown_path],))
    # 全镜像 cmp r3,#0x21 代码点：两处目标已改 #0xFF，其余两处与 PA6 长按无关且未改动
    other_cmp = []
    for a in range(BASE, BASE + len(orig) - 3, 2):
        i = dis_one(orig, a)
        if i is not None and ins_text(i) == "cmp r3, #0x21":
            other_cmp.append(a)
    rep.chk("代码中 cmp r3,#0x21 共 4 处：0x0802830A/0x080284F2 已改 #0xFF；"
            "0x08195E96/0x081F6C98 保持原样（与 PA6 长按状态机无关）",
            0x0802830A in other_cmp and 0x080284F2 in other_cmp
            and 0x08195E96 in other_cmp and 0x081F6C98 in other_cmp
            and patched[off(0x08195E96):off(0x08195E96) + 2] == bytes.fromhex("212b")
            and patched[off(0x081F6C98):off(0x081F6C98) + 2] == bytes.fromhex("212b"),
            "4 处：%s" % (["0x%08X" % a for a in other_cmp],))
    rep.chk("两处 PA6 长按动作块（0x08028310 / 0x080284FA）在成品中均不可达："
            "无分支目标 + fall-through 被恒成立的 bls 跳过",
            not targets_action and not targets_action2 and uxtb1 and bls1 and uxtb2 and bls2)

    # =====================================================================
    rep.section("10. 空闲闪存 / 引用扫描")
    seg_free = orig[off(FREE_LO):off(FREE_HI)]
    rep.chk("原厂 0x%08X..0x%08X 全 0xFF（%d 字节）" % (FREE_LO, FREE_HI, len(seg_free)),
            seg_free == b"\xff" * len(seg_free))
    word_refs_inj = iter_word_refs(orig, INJ_BASE, INJ_BASE + INJ_WINDOW)
    rep.chk("原厂无字面量引用 0x081C0000..0x081C0800", not word_refs_inj,
            "发现 %s" % (["0x%08X=0x%08X" % r for r in word_refs_inj[:8]],))
    br_inj = [(s, mn, t) for (s, mn, t) in branches_orig
              if INJ_BASE <= t < INJ_BASE + INJ_WINDOW]
    rep.chk("原厂无分支目标落入 0x081C0000..0x081C0800", not br_inj,
            "发现 %s" % (["0x%08X->0x%08X" % (s, t) for s, _m, t in br_inj[:8]],))
    ext = [(s, mn, t) for (s, mn, t) in branches_patch
           if INJ_BASE <= t < INJ_BASE + INJ_CODE_LEN
           and not (INJ_BASE <= s < INJ_BASE + INJ_CODE_LEN)]
    want_ext = {(KEYSCAN_FN, "b.w", 0x081C0070), (TX4, "b.w", 0x081C0028),
                (TX2, "b.w", 0x081C004C), (SYNC_PATCH_ADDR, "b.w", 0x081C0000)}
    rep.chk("成品注入区外部分支入口恰为 4 个补丁点（无其它跳入）",
            set(ext) == want_ext,
            "实际 %s" % ([(hex(s), mn, hex(t)) for s, mn, t in ext],))
    word_refs_p = [(a, v) for (a, v) in iter_word_refs(patched, INJ_BASE, INJ_BASE + INJ_WINDOW)
                   if not (INJ_BASE <= a < INJ_BASE + INJ_WINDOW)]
    rep.chk("成品无字面量引用注入窗口", not word_refs_p,
            "发现 %s" % (["0x%08X" % a for a, _v in word_refs_p[:8]],))

    # =====================================================================
    rep.section("11. Unicorn：KEY_ON 开关语义 / 双发 / 原厂自证")
    probe_o = UcProbe(orig)
    probe_p = UcProbe(patched)
    ids = [0, 1, 2, 37, 50, 73, 98, 99]
    # 11.1 原厂同步字函数自证
    ok = True
    msgs = []
    for ident in ids:
        r = probe_o.run(SYNC_FN, ident)
        got, burst = sync_word_of(r["events"])
        want = SYNC_NUMERATOR // (ident + 1)
        good = (r["error"] is None and got == want and not burst
                and r["events"] == [("syncbyte", 4, want >> 8), ("syncbyte", 5, want & 0xFF),
                                    ("delay", 20), ("strobe", 0x33), ("delay", 1000),
                                    ("strobe", 0x36), ("delay", 700)] and r["sp"] == STACK)
        ok = ok and good
        msgs.append("ID=%-2d=0x%04X" % (ident, got if got is not None else 0))
    rep.chk("原厂镜像自证：set_sync(ID)=floor(50024/(ID+1))，reg4/reg5 单写 + 时序/栈不变",
            ok, "; ".join(msgs))
    # 11.2 补丁版 KEY_ON=0/1
    for key_on, name in ((0, "KEY_ON=0"), (1, "KEY_ON=1")):
        ok = True
        msgs = []
        for ident in ids:
            r = probe_p.run(SYNC_FN, ident, key_on=key_on)
            got, burst = sync_word_of(r["events"])
            want = key if key_on else SYNC_NUMERATOR // (ident + 1)
            good = (r["error"] is None and got == want and not burst and r["sp"] == STACK)
            ok = ok and good
            msgs.append("ID=%-2d=0x%04X" % (ident, got if got is not None else 0))
        rep.chk("补丁版 set_sync %s：%s" % (
            name, "所有 ID -> KEY(0x%04X)" % key if key_on else "ID -> 原厂公式"), ok,
            "; ".join(msgs))
    # 11.3 原厂镜像自证 TX 单发（无 SYNC 写入）
    for fn, body, label in ((TX4, BODY4, "TX4"), (TX2, BODY2, "TX2")):
        r = probe_o.run(fn, 0)
        rep.chk("原厂镜像自证 %s：wake + 单发（无 SYNC 写入）" % label,
                r["error"] is None and r["events"] == [("wake",)] + body and r["sp"] == STACK,
                fmt_events(r["events"]))
    # 11.4 补丁版 KEY_ON=0：单发 + 先写原厂同步字（动态公式）
    ok = True
    msgs = []
    for ident in (0, 1, 37, 99):
        want = SYNC_NUMERATOR // (ident + 1)
        r4 = probe_p.run(TX4, ident, key_on=0)
        r2 = probe_p.run(TX2, ident, key_on=0)
        want4 = [("wake",), ("sync", want)] + BODY4
        want2 = [("wake",), ("sync", want)] + BODY2
        good = (r4["error"] is None and r4["events"] == want4 and r4["sp"] == STACK
                and r2["error"] is None and r2["events"] == want2 and r2["sp"] == STACK)
        ok = ok and good
        msgs.append("ID=%-2d sync=0x%04X %s" % (ident, want, "OK" if good else "FAIL"))
    rep.chk("补丁版 KEY_ON=0：TX 单发且首帧前写原厂同步字（随当前 ID 动态）", ok, "; ".join(msgs))
    # 11.5 补丁版 KEY_ON=1：双发，第二帧同步字 = 50024/(ID+1) 动态
    ok = True
    msgs = []
    for ident in (0, 1, 37, 99):
        want = SYNC_NUMERATOR // (ident + 1)
        want4 = [("wake",), ("sync", key)] + BODY4 \
            + [("wake",), ("sync", want)] + BODY4 + [("sync", key)]
        want2 = [("wake",), ("sync", key)] + BODY2 \
            + [("wake",), ("sync", want)] + BODY2 + [("sync", key)]
        r4 = probe_p.run(TX4, ident, key_on=1)
        r2 = probe_p.run(TX2, ident, key_on=1)
        good = (r4["error"] is None and r4["events"] == want4 and r4["sp"] == STACK
                and r2["error"] is None and r2["events"] == want2 and r2["sp"] == STACK)
        ok = ok and good
        msgs.append("ID=%-2d 第二帧=0x%04X %s" % (ident, want, "OK" if good else "FAIL"))
    rep.chk("补丁版 KEY_ON=1：SYNC=KEY -> 发 -> SYNC=floor(50024/(ID+1)) -> 发 -> SYNC=KEY", ok,
            "; ".join(msgs))
    rep.chk("第二帧同步字确实随 ID 变化（ID=0 -> 0xC368，ID=37 -> 0x0524，非固定 0x01F4）",
            SYNC_NUMERATOR // 1 == 0xC368 and SYNC_NUMERATOR // 38 == 0x0524
            and 0xC368 != 0x01F4 and 0x0524 != 0x01F4)
    # 11.6 KEY_ON=1 双发时同步字写入发生在 wake 之后
    r = probe_p.run(TX4, 0, key_on=1)
    order_ok = (r["events"][:2] == [("wake",), ("sync", key)]
                and r["events"][7:9] == [("wake",), ("sync", SYNC_NUMERATOR // 1)])
    rep.chk("双发两帧的同步字写入均发生在 bl 0x0802EC64 唤醒之后（新旧设计关键点）",
            order_ok, "前两事件 %s / 第二帧 %s" % (r["events"][:2], r["events"][7:9]))
    # 11.7 双发次数
    n_o = sum(1 for e in probe_o.run(TX2, 0)["events"] if e[0] == "fifo")
    n_p0 = sum(1 for e in probe_p.run(TX2, 0, key_on=0)["events"] if e[0] == "fifo")
    n_p1 = sum(1 for e in probe_p.run(TX2, 0, key_on=1)["events"] if e[0] == "fifo")
    rep.chk("2B 短命令每帧发射次数：原厂 %d / KEY_ON=0 %d / KEY_ON=1 %d（双发仅密钥模式）"
            % (n_o, n_p0, n_p1), n_o == 1 and n_p0 == 1 and n_p1 == 2)

    # =====================================================================
    rep.section("12. Unicorn：按键长按注入逻辑（到 0x08027E48 即停）")
    SP_AT_STOP = STACK - 8 - 40      # inj_btn push {r4,lr} + tramp_btn push.w(10 regs)
    cases = [
        ("松开复位", dict(key_on=0, cnt=5, latch=0, pressed=False), (0, 0, 0)),
        ("第 1 次按下", dict(key_on=0, cnt=0, latch=0, pressed=True), (0, 1, 0)),
        ("第 34 次按下翻转", dict(key_on=0, cnt=33, latch=0, pressed=True), (1, 34, 1)),
        ("第 35 次（latch 已置，不翻转）", dict(key_on=1, cnt=34, latch=1, pressed=True), (1, 34, 1)),
        ("再按住 34 次关闭", dict(key_on=1, cnt=33, latch=0, pressed=True), (0, 34, 1)),
    ]
    ok = True
    msgs = []
    for name, st, want in cases:
        ram = {BTN_CNT: struct.pack("<H", st["cnt"]), BTN_LATCH: bytes([st["latch"]])}
        r = probe_p.run(KEYSCAN_FN, 0, key_on=st["key_on"], stop_at=KEYSCAN_RESUME,
                        gpio_pressed=st["pressed"], ram=ram)
        got = (r["key_on"], r["cnt"], r["latch"])
        good = (r["error"] is None and r["hit_stop"] and got == want
                and r["sp"] == SP_AT_STOP)
        ok = ok and good
        msgs.append("%s -> %s%s" % (name, got, "" if good else " FAIL"))
    rep.chk("按键注入 5 用例：松开复位 / 第1次 / 第34次翻转 / 第35次不翻转 / 再按一次关", ok,
            "; ".join(msgs))
    # GPIO 读取参数
    r = probe_p.run(KEYSCAN_FN, 0, stop_at=KEYSCAN_RESUME, gpio_pressed=True)
    greads = [e for e in r["events"] if e[0] == "gpio_read"]
    rep.chk("注入逻辑读取 PA6：gpio_read(0x42020000, 0x40)（返回 0=按下）",
            greads and greads[0][1] == BTN_GPIO and greads[0][2] == BTN_MASK
            and greads[0][3] == 0, fmt_events(greads[:1]))
    # 34 次连续按下（模拟主循环每轮调用一次），第 34 次翻转；继续按住不再翻转
    state = {"key_on": 0, "cnt": 0, "latch": 0}
    toggled_at = None
    bad = False
    for n in range(1, 41):
        ram = {BTN_CNT: struct.pack("<H", state["cnt"]), BTN_LATCH: bytes([state["latch"]])}
        r = probe_p.run(KEYSCAN_FN, 0, key_on=state["key_on"], stop_at=KEYSCAN_RESUME,
                        gpio_pressed=True, ram=ram)
        if r["error"] is not None or not r["hit_stop"]:
            bad = True
            break
        state = {"key_on": r["key_on"], "cnt": r["cnt"], "latch": r["latch"]}
        if state["key_on"] == 1 and toggled_at is None:
            toggled_at = n
    rep.chk("34 次连续扫描（按住 PA6）在第 34 次翻转 KEY_ON=1，第 35..40 次保持不变",
            not bad and toggled_at == 34 and state["key_on"] == 1 and state["cnt"] == 34,
            "翻转于第 %r 次，最终 key_on=%d cnt=%d" % (toggled_at, state["key_on"], state["cnt"]))
    # 松开后再按 34 次关闭
    ram = {BTN_CNT: struct.pack("<H", state["cnt"]), BTN_LATCH: bytes([state["latch"]])}
    r = probe_p.run(KEYSCAN_FN, 0, key_on=state["key_on"], stop_at=KEYSCAN_RESUME,
                    gpio_pressed=False, ram=ram)
    state = {"key_on": r["key_on"], "cnt": r["cnt"], "latch": r["latch"]}
    off_at = None
    for n in range(1, 41):
        ram = {BTN_CNT: struct.pack("<H", state["cnt"]), BTN_LATCH: bytes([state["latch"]])}
        r = probe_p.run(KEYSCAN_FN, 0, key_on=state["key_on"], stop_at=KEYSCAN_RESUME,
                        gpio_pressed=True, ram=ram)
        state = {"key_on": r["key_on"], "cnt": r["cnt"], "latch": r["latch"]}
        if state["key_on"] == 0 and off_at is None:
            off_at = n
    rep.chk("松开一次（计数/latch 复位）后再按 34 次：KEY_ON 翻回 0（密钥模式可关闭）",
            off_at == 34 and state["key_on"] == 0,
            "松开后 count=%d latch=%d，再按翻转于第 %r 次" % (r["cnt"], r["latch"], off_at))

    # =====================================================================
    nfail = rep.summary()
    return 0 if nfail == 0 else 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except Exception:  # noqa: BLE001
        traceback.print_exc()
        sys.exit(2)
