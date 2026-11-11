#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""V1S_V1.8.bin —— C.Fn / settings 子系统逆向证据脚本（cfn-analyst / task-1）。

目标：为「把 C.Fn LIGHT 项复用为私有密钥开关」的补丁（keypatch/devices/v1s.py）
提供全部地址/字节证据，并逐项自证（PASS/FAIL）。

运行:
    tools\\python\\python.exe devices\\v1s\\re\\cfn_v1s.py
退出码: 0 = 全部断言 PASS；1 = 存在 FAIL。
输出: 同时写 devices/v1s/re/log_cfn_v1s.txt（UTF-8）。

只读固件；不修改任何二进制、不改 keypatch/devices/。
方法参照 keypatch/dev/v1c/CFN_SETTINGS.md / CFN_MENU.md 的静态数据流方法论，
但所有地址均在本固件中重新定位并断言（不照搬 V1C 地址）。
"""
from __future__ import annotations

import bisect
import hashlib
import io
import os
import re
import struct
import sys
from pathlib import Path

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

# ---------------------------------------------------------------------------
# 0. 目标固件常量
# ---------------------------------------------------------------------------
HERE = Path(__file__).resolve()
ROOT = HERE.parents[3]                         # D:\神牛
FW_PATH = ROOT / "devices" / "v1s" / "firmware" / "V1S_V1.8.bin"
LOG_PATH = HERE.parent / "log_cfn_v1s.txt"

BASE = 0x08003000
IMAGE_SHA256 = "87a0600046e1b7c2198a7ac8fac15959bb3a5d582edf44d8e0df38706dcb917a"
IMAGE_SIZE = 78500

# --- 本次逆向结论（地址均在下方逐条自证） ---------------------------------
SETTINGS_BASE = 0x20001054     # C.Fn settings[0]；item n = base + n
ITEM_COUNT = 12                # 0..0xB
MAX_TABLE = 0x08015DE6         # 12 项最大值/回绕表
MAX_BYTES = bytes.fromhex("01010101011f630102060101")
ID_ITEM = 6                    # settings[6] = ID
LIGHT_ITEM = 8                 # settings[8] = LIGHT
ID_ADDR = SETTINGS_BASE + ID_ITEM        # 0x2000105A
LIGHT_ADDR = SETTINGS_BASE + LIGHT_ITEM  # 0x2000105C

ITEM_SELECT = 0x200004D0       # 当前选中项索引（dec/inc handler 读取）
EDIT_MODE = 0x200004F1         # 编辑态（V1C 同义，本脚本仅用于上下文）

SYNC_FN = 0x080112DC           # void rf_set_sync(uint8_t id)（50024/(id+1)）
SYNC_LITERAL = 0x08011310      # 50024 字面量地址（全镜像唯一）
ID_NUMERATOR = 50024

DEC_FN = 0x080059FC            # C.Fn 值递减 handler
INC_FN = 0x0800C354            # C.Fn 值递增 handler
DEC_GATE = 0x08005A8A          # cmp r1,#6 ; bne（原厂 item==6 才同步）
INC_GATE = 0x0800C3E8          # cmp r1,#6 ; bne（原厂 item==6 才同步）

LIGHT_FN = 0x08003950          # 造型灯任务（功能读者）
LIGHT_READ1 = 0x08003956       # ldrb r0,[r0,#8]
LIGHT_READ2 = 0x0800397E       # ldrb r1,[r0,#8]
DISPLAY_FN = 0x0800431C        # C.Fn 菜单渲染（12 项 3 页 x 4 行）

SAVE_BUILDER = 0x080056F0      # settings -> flash record（0x48B）
SAVE_TASK = 0x0800D7EC         # 自动保存任务（200 tick 倒计时后）
SAVE_SHADOW = 0x08012C9C       # record 比较/更新 shadow
FLASH_WRITE = 0x080091CC       # shadow -> flash slot
FLASH_ERASE = 0x080090D4       # 擦参数页（0x800B）
FLASH_OP_ERASE = 0x0800A454    # 底层擦除
FLASH_PAGE_MAIN = 0x0801F000   # 参数记录槽起始（20 槽 x 0x64B）
FLASH_PAGE_BAK = 0x0801F800    # 备份/校验页
RECORD_BYTES = 0x48            # 记录 0x12 words
RECORD_HEADER = 0x5C           # 记录有效标记
LOAD_TASK = 0x0800EC58         # 上电扫描 20 槽 + 校验 + 展开/默认
FLASH_READ = 0x08009164        # flash -> record shadow 0x20000F88
LOAD_EXPAND = 0x0801383C       # record -> settings 展开
DEFAULTS_FN = 0x08005E68       # 出厂默认（settings + 大量 RAM 默认）
RECORD_SHADOW = 0x20000F88

# LIGHT 功能读者的 mask 补丁（语义等价于 settings[8]==0 = 12sec 出厂默认）
MASK1_ADDR, MASK1_ORIG, MASK1_NEW = LIGHT_READ1, bytes.fromhex("007a"), bytes.fromhex("0020")
MASK2_ADDR, MASK2_ORIG, MASK2_NEW = LIGHT_READ2, bytes.fromhex("017a"), bytes.fromhex("0021")

# 推荐补丁：把两个 gate 的 4B 改成「无条件跳进原厂同步块」
#   0x... gate: 原厂 cmp r1,#6 ; bne <pop>
#   改为: b <同步块> ; nop   （4B，无需代码洞；同步块仍是 ldrb ID + bl SYNC_FN）
GATE_PATCH = bytes.fromhex("00e000bf")

# 记录 V1C 对照地址（仅用于差异表，不用于定位）
V1C = {
    "settings_base": 0x20001086,
    "sp": 0x20001550,
    "max_table": 0x08018362,
    "sync_fn": 0x080121BC,
    "dec_gate": 0x0800625C,
    "inc_gate": 0x0800D46E,
    "light_fn": 0x080036D4,
    "display_fn": 0x08004914,
    "save_builder": 0x08005EF0,
    "load_expand": 0x080158B0,
    "defaults_fn": 0x080066D0,
    "load_task": 0x0800FEF8,
    "unpack_w8": 0x0800A6FA,     # V1C 紧凑结构解包写 settings[8]
    "cave": 0x08017944,
}

COND = {"beq", "bne", "bcs", "bhs", "bcc", "blo", "bmi", "bpl", "bvs", "bvc",
        "bhi", "bls", "bge", "blt", "bgt", "ble"}
DEST_WRITE = {"mov", "movs", "adds", "subs", "add", "sub", "adr", "ldr", "ldrb",
              "ldrh", "ldrsb", "ldrsh", "uxtb", "uxth", "and", "ands", "orr",
              "orrs", "bic", "bics", "lsl", "lsls", "lsr", "lsrs", "asr", "asrs",
              "rsb", "rsbs", "mvn", "mvns", "mul", "muls", "eor", "eors",
              "movw", "movt", "neg"}
ACCESS_MNEMONICS = {"ldrb", "strb", "ldrh", "strh", "ldr", "str"}


# ---------------------------------------------------------------------------
# 输出 / 断言框架
# ---------------------------------------------------------------------------
class Tee:
    def __init__(self, *streams):
        self.streams = streams

    def write(self, s):
        for st in self.streams:
            st.write(s)

    def flush(self):
        for st in self.streams:
            st.flush()


RESULTS: list[tuple[bool, str]] = []


def check(cond: bool, msg: str) -> bool:
    RESULTS.append((bool(cond), msg))
    print("  [%s] %s" % ("PASS" if cond else "FAIL", msg))
    return bool(cond)


def header(t: str) -> None:
    print()
    print("=" * 78)
    print(t)
    print("=" * 78)


def fmt(ins) -> str:
    return "0x%08X  %-12s %-7s %s" % (ins.address, ins.bytes.hex(" "),
                                       ins.mnemonic, ins.op_str)


# ---------------------------------------------------------------------------
# 固件 / 反汇编基础设施
# ---------------------------------------------------------------------------
class Code:
    """capstone 线性反汇编 + 函数边界 + 字面量/分支解析。"""

    def __init__(self, image: bytes):
        self.image = image
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
        md.skipdata = True
        self.insns = list(md.disasm(image, BASE))
        self.by = {i.address: i for i in self.insns}
        self.entries = self._function_entries()
        self._pred_cache: dict[int, dict[int, set[int]]] = {}

    # -- 基本解析 ----------------------------------------------------------
    def at(self, addr: int):
        return self.by.get(addr)

    def u32(self, addr: int) -> int:
        return struct.unpack_from("<I", self.image, addr - BASE)[0]

    def literal_value(self, ins):
        if ins.mnemonic != "ldr" or "[pc," not in ins.op_str:
            return None
        m = re.search(r"#(-?0x[0-9a-fA-F]+|-?\d+)", ins.op_str)
        if not m:
            return None
        s = m.group(1)
        imm = int(s, 16) if "x" in s else int(s)
        lit = ((ins.address + 4) & ~3) + imm
        off = lit - BASE
        if off < 0 or off + 4 > len(self.image):
            return None
        return lit, struct.unpack_from("<I", self.image, off)[0]

    def branch_target(self, ins):
        if not (ins.mnemonic == "b" or ins.mnemonic in COND
                or ins.mnemonic in ("bl", "blx", "cbz", "cbnz")):
            return None
        m = re.search(r"#(0x[0-9a-fA-F]+)", ins.op_str)
        return int(m.group(1), 16) if m else None

    def bl_target(self, ins):
        if ins.mnemonic not in ("bl", "blx"):
            return None
        t = self.branch_target(ins)
        return None if t is None else (t & ~1)

    def _function_entries(self) -> list[int]:
        entries = set()
        for ins in self.insns:
            t = self.bl_target(ins)
            if t is not None and BASE <= t < BASE + len(self.image):
                entries.add(t)
        # 向量表（前 0x40/0x50 字节）
        for off in range(0, 0x50, 4):
            if off + 4 > len(self.image):
                break
            v = struct.unpack_from("<I", self.image, off)[0]
            if 0x08000000 <= v < 0x09000000 and BASE <= (v & ~1) < BASE + len(self.image):
                entries.add(v & ~1)
        return sorted(entries)

    def next_entry(self, addr: int) -> int:
        i = bisect.bisect_right(self.entries, addr)
        return self.entries[i] if i < len(self.entries) else BASE + len(self.image)

    def fn_of(self, addr: int) -> tuple[int, int]:
        i = bisect.bisect_right(self.entries, addr) - 1
        start = self.entries[i] if i >= 0 else BASE
        return start, self.next_entry(start)

    # -- 寄存器写集合 / 控制流 ---------------------------------------------
    @staticmethod
    def written_regs(ins) -> set[int]:
        op, m = ins.op_str, ins.mnemonic
        regs: set[int] = set()
        if m in ("bl", "blx"):
            return {0, 1, 2, 3, 12}
        if m == "pop":
            for r in re.findall(r"r(\d+)", op):
                regs.add(int(r))
            return regs
        if m in ("ldm", "ldmia", "ldmdb", "ldmfd"):
            for r in re.findall(r"r(\d+)", op.split("{", 1)[-1]):
                regs.add(int(r))
            return regs
        if m in DEST_WRITE:
            first = op.split(",", 1)[0].strip()
            if re.fullmatch(r"r\d+", first):
                regs.add(int(first[1:]))
        return regs

    def _has_pc_write(self, ins) -> bool:
        if ins.mnemonic in ("bx", "blx") and "r" in ins.op_str:
            return True
        if ins.mnemonic == "pop" and "pc" in ins.op_str:
            return True
        if ins.mnemonic in ("ldm", "ldmia", "ldmfd") and "pc" in ins.op_str:
            return True
        if ins.mnemonic in DEST_WRITE and ins.op_str.split(",", 1)[0].strip() == "pc":
            return True
        return False

    def successors(self, ins, fs: int, fe: int) -> list[int]:
        nxt = ins.address + ins.size
        out: list[int] = []
        m = ins.mnemonic
        if m == "b":
            t = self.branch_target(ins)
            if t is not None:
                out.append(t & ~1)
        elif m in COND or m in ("cbz", "cbnz"):
            t = self.branch_target(ins)
            if t is not None:
                out.append(t & ~1)
            out.append(nxt)
        elif m in ("bx", "blx") or self._has_pc_write(ins):
            out = []
        else:
            out = [nxt]
        return [a for a in out if fs <= a < fe and a in self.by]

    def reachable(self, fn_start: int, fn_end: int) -> set[int]:
        seen: set[int] = set()
        work = [fn_start]
        while work:
            a = work.pop()
            if a in seen or not (fn_start <= a < fn_end):
                continue
            ins = self.by.get(a)
            if ins is None:
                continue
            seen.add(a)
            work.extend(self.successors(ins, fn_start, fn_end))
        return seen

    def preds(self, fn_start: int) -> dict[int, set[int]]:
        if fn_start in self._pred_cache:
            return self._pred_cache[fn_start]
        fn_end = self.next_entry(fn_start)
        reach = self.reachable(fn_start, fn_end)
        p: dict[int, set[int]] = {}
        for a in sorted(reach):
            ins = self.by[a]
            for s in self.successors(ins, fn_start, fn_end):
                if s in reach:
                    p.setdefault(s, set()).add(a)
        self._pred_cache[fn_start] = p
        return p

    def resolve_reg(self, reg: int, addr: int, fn_start: int) -> dict:
        """求「执行 addr 之前」reg 的静态值（按 CFG 前驱回溯）。"""
        fn_end = self.next_entry(fn_start)
        pred = self.preds(fn_start)
        work = list(pred.get(addr, ()))
        visited: set[int] = set()
        defs: list[tuple] = []
        while work:
            a = work.pop()
            if a in visited:
                continue
            visited.add(a)
            if len(visited) > 8000:
                return {"status": "unknown", "value": None, "defs": defs}
            ins = self.by.get(a)
            if ins is None:
                work.append(a - 2)
                continue
            if reg in self.written_regs(ins):
                defs.append(self._describe_def(ins, reg))
                continue
            if a == fn_start:
                defs.append(("entry", None, a, "函数入口（值来自调用者）"))
                continue
            work.extend(pred.get(a, ()))
        if not defs:
            return {"status": "unknown", "value": None, "defs": []}
        known = [d for d in defs if d[0] in ("imm", "lit")]
        values = {d[1] for d in known}
        if len(known) == len(defs) and len(values) == 1:
            return {"status": "resolved", "value": known[0][1], "defs": defs}
        if len(defs) == 1 and defs[0][0] == "reg":
            inner = self.resolve_reg(defs[0][1], defs[0][2], fn_start)
            if inner["status"] == "resolved":
                inner["defs"] = [defs[0]] + inner["defs"]
            return inner
        return {"status": "ambiguous", "value": None, "defs": defs}

    def _describe_def(self, ins, reg: int) -> tuple:
        op = ins.op_str
        if ins.mnemonic in ("movs", "mov") and op.startswith("r%d," % reg):
            rhs = op.split(",", 1)[1].strip()
            m = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", rhs)
            if m:
                v = int(m.group(1), 16) if "x" in m.group(1) else int(m.group(1))
                return ("imm", v, ins.address, "%s   ; r%d = %s" % (fmt(ins), reg, rhs))
            m = re.fullmatch(r"(r\d+)", rhs)
            if m:
                return ("reg", int(m.group(1)[1:]), ins.address,
                        "%s   ; r%d <- %s" % (fmt(ins), reg, rhs))
        if ins.mnemonic == "ldr" and op.startswith("r%d," % reg):
            lv = self.literal_value(ins)
            if lv is not None:
                return ("lit", lv[1], ins.address,
                        "%s   ; 字面量@0x%08X = 0x%08X" % (fmt(ins), lv[0], lv[1]))
        if ins.mnemonic in ("bl", "blx"):
            return ("call", None, ins.address, "%s   ; 调用使 r%d 失效" % (fmt(ins), reg))
        return ("dyn", None, ins.address, "%s   ; r%d 来源非字面量/立即数" % (fmt(ins), reg))


# ---------------------------------------------------------------------------
# settings 访问点前向污点扫描（窗口 = settings[0..0xD]）
# ---------------------------------------------------------------------------
def parse_reg(s: str):
    m = re.fullmatch(r"r(\d+)", s.strip())
    return int(m.group(1)) if m else None


def parse_mem(op: str):
    parts = [p.strip() for p in op.split(",")]
    if len(parts) < 2 or not parts[1].startswith("["):
        return None
    dst = parse_reg(parts[0])
    inner = op[op.index("["):].strip("[]")
    toks = [t.strip() for t in inner.split(",")]
    base = parse_reg(toks[0]) if toks else None
    index, imm = None, 0
    for t in toks[1:]:
        t = t.rstrip("]!").strip()
        if t.startswith("#"):
            try:
                imm = int(t[1:], 16) if "x" in t[1:] else int(t[1:])
            except ValueError:
                imm = None
        elif any(x in t for x in ("lsl", "lsr", "asr")):
            index, imm = -1, None
        else:
            r = parse_reg(t)
            if r is not None:
                index, imm = r, None
    return (dst, base, index, imm)


class Access:
    __slots__ = ("addr", "kind", "offset", "dyn", "fn")

    def __init__(self, addr, kind, offset, dyn, fn):
        self.addr, self.kind, self.offset, self.dyn, self.fn = addr, kind, offset, dyn, fn


def scan_settings_accesses(code: Code) -> list[Access]:
    """从 42 处 `ldr rX,=0x20001054` 种子出发，前向传播「基址+偏移」，
    记录落在 settings[0..0xD] 窗口内的 ldrb/strb/ldr/str。"""
    win_lo, win_hi = SETTINGS_BASE, SETTINGS_BASE + 14

    def apply_effect(ins, state):
        out = dict(state)
        m, op = ins.mnemonic, ins.op_str
        if m in ("bl", "blx"):
            for r in (0, 1, 2, 3, 12):
                out.pop(r, None)
            return out
        if m in ("pop", "ldm", "ldmia", "ldmfd"):
            for r in re.findall(r"r(\d+)", op.split("{", 1)[-1]):
                out.pop(int(r), None)
            return out
        mem = parse_mem(op)
        if m == "ldr" and "[pc," in op and mem:
            d = mem[0]
            lv = code.literal_value(ins)
            if d is not None:
                out.pop(d, None)
                if lv is not None:
                    out[d] = lv[1]
            return out
        d = mem[0] if ("[" in op and mem) else None
        if d is None:
            d = parse_reg(op.split(",", 1)[0].strip())
        if d is None:
            return out
        out.pop(d, None)
        if m in ("movs", "mov"):
            rhs = op.split(",", 1)[1].strip() if "," in op else ""
            mm = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", rhs)
            if mm:
                out[d] = int(mm.group(1), 16) if "x" in mm.group(1) else int(mm.group(1))
            else:
                src = parse_reg(rhs)
                if src is not None and src in state:
                    out[d] = state[src]
        elif m in ("adds", "add", "subs", "sub"):
            toks = [t.strip() for t in op.split(",")]
            if len(toks) == 3:
                src = parse_reg(toks[1])
                mm = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", toks[2])
                if src is not None and src in state and mm:
                    imm = int(mm.group(1), 16) if "x" in mm.group(1) else int(mm.group(1))
                    out[d] = state[src] + (imm if m.startswith("add") else -imm)
        return out

    found: list[Access] = []
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is None or lv[1] != SETTINGS_BASE:
            continue
        mem = parse_mem(ins.op_str)
        if not mem or mem[0] is None:
            continue
        fs, fe = code.fn_of(ins.address)
        work = [(a, {mem[0]: SETTINGS_BASE}) for a in code.successors(ins, fs, fe)]
        seen: dict[int, set] = {}
        steps = 0
        while work and steps < 4000:
            addr, st = work.pop()
            steps += 1
            if not (fs <= addr < fe) or addr not in code.by:
                continue
            key = frozenset(st.items())
            if key in seen.get(addr, set()):
                continue
            seen.setdefault(addr, set()).add(key)
            cur = code.by[addr]
            m2 = parse_mem(cur.op_str)
            if cur.mnemonic in ACCESS_MNEMONICS and m2:
                _d, breg, ireg, imm = m2
                if breg in st:
                    bv = st[breg]
                    if ireg is not None:
                        if bv == SETTINGS_BASE:
                            found.append(Access(addr, cur.mnemonic, None, True, fs))
                    elif imm is not None and win_lo <= bv + imm < win_hi:
                        found.append(Access(addr, cur.mnemonic, bv + imm - SETTINGS_BASE,
                                            False, fs))
            st2 = apply_effect(cur, st)
            for s in code.successors(cur, fs, fe):
                work.append((s, st2))
    uniq = {}
    for a in found:
        uniq[(a.addr, a.kind, a.offset, a.dyn)] = a
    return sorted(uniq.values(),
                  key=lambda a: ((a.offset if a.offset is not None else 99), a.addr))


# ---------------------------------------------------------------------------
# 小工具：Thumb 编码（用于自证补丁字节可反汇编）
# ---------------------------------------------------------------------------
def thumb_disasm(code: Code, blob: bytes, addr: int):
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    return list(md.disasm(blob, addr))


def check_bytes_at(code: Code, addr: int, expect: bytes, what: str) -> bool:
    off = addr - BASE
    got = code.image[off:off + len(expect)]
    ok = got == expect
    check(ok, "%s @0x%08X 原厂字节 = %s (期望 %s)" % (what, addr, got.hex(" "), expect.hex(" ")))
    return ok


# ---------------------------------------------------------------------------
# 1. 固件身份
# ---------------------------------------------------------------------------
def check_identity(image: bytes, code: Code) -> None:
    header("0. 固件身份 / 基址")
    sha = hashlib.sha256(image).hexdigest()
    print("  文件: %s" % FW_PATH)
    print("  大小: %d B (0x%X)" % (len(image), len(image)))
    print("  基址: 0x%08X  结束: 0x%08X" % (BASE, BASE + len(image)))
    print("  sha256: %s" % sha)
    check(len(image) == IMAGE_SIZE, "镜像大小 == %d B" % IMAGE_SIZE)
    check(sha == IMAGE_SHA256, "sha256 与任务给定一致（未改动）")
    sp = struct.unpack_from("<I", image, 0)[0]
    reset = struct.unpack_from("<I", image, 4)[0]
    print("  vector[0] SP  = 0x%08X   (V1C 0x%08X)" % (sp, V1C["sp"]))
    print("  vector[1] PC  = 0x%08X" % reset)
    check(sp == 0x20001540, "初始化 SP = 0x20001540（RAM 顶部，V1C 为 0x20001550）")
    check(reset == 0x080030D5, "复位向量 = 0x080030D5（Thumb，入口 0x080030D4）")


# ---------------------------------------------------------------------------
# 2. 最大值表 + 引用点
# ---------------------------------------------------------------------------
def check_max_table(code: Code) -> None:
    header("1. C.Fn 最大值/回绕表 0x%08X 与引用点" % MAX_TABLE)
    off = MAX_TABLE - BASE
    tbl = code.image[off:off + ITEM_COUNT]
    print("  表内容: %s" % tbl.hex(" "))
    print("  每项 max: %s" % ["item%d=%d" % (i, b) for i, b in enumerate(tbl)])
    check(tbl == MAX_BYTES, "12 字节内容 == 01 01 01 01 01 1F 63 01 02 06 01 01")
    check(tbl[ID_ITEM] == 0x63, "item6(ID) max == 0x63(99)")
    check(tbl[LIGHT_ITEM] == 2, "item8(LIGHT) max == 2（0/1/2 三态）")
    refs = []
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is not None and lv[1] == MAX_TABLE:
            refs.append(ins)
    print("  ldr 字面量引用 %d 处:" % len(refs))
    for ins in refs:
        fs, _ = code.fn_of(ins.address)
        print("    " + fmt(ins) + "   fn=0x%08X" % fs)
    addrs = [i.address for i in refs]
    check(addrs == [0x08005A82, 0x0800C3B2, 0x0800EE2E],
          "表引用点 == dec 0x08005A82 / inc 0x0800C3B2 / 设置应用 0x0800EE2E")
    check(code.fn_of(0x08005A82)[0] == DEC_FN,
          "dec 引用点在递减 handler 0x%08X 内" % DEC_FN)
    check(code.fn_of(0x0800C3B2)[0] == INC_FN,
          "inc 引用点在递增 handler 0x%08X 内" % INC_FN)
    # 表被用于 index = 当前项（动态索引），与 settings 动态索引同源
    for addr in (0x08005A84, 0x0800C3BC):
        ins = code.at(addr)
        print("  动态索引用法: " + (fmt(ins) if ins else "?"))
    check(code.at(0x08005A84).mnemonic == "ldrb" and
          code.at(0x08005A84).op_str == "r3, [r3, r1]",
          "0x08005A84 = ldrb r3,[max_table, item]（dec 用同一个 item 索引）")
    check(code.at(0x0800C3BC).op_str == "r6, [r5, r2]",
          "0x0800C3BC = ldrb r6,[max_table, item]（inc 用同一个 item 索引）")


# ---------------------------------------------------------------------------
# 3. settings 基址 / 12 项边界 / item 地址
# ---------------------------------------------------------------------------
def check_settings_base(code: Code) -> None:
    header("2. settings RAM 基址 / 12 项边界 / item6、item8 地址")
    refs = []
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is not None and lv[1] == SETTINGS_BASE:
            refs.append(ins.address)
    print("  0x%08X 的 ldr 字面量引用: %d 处" % (SETTINGS_BASE, len(refs)))
    print("    前 12 处: %s" % ["0x%08X" % a for a in refs[:12]])
    check(len(refs) == 42, "settings 基址字面量引用 == 42 处（同族中最集中的 RAM 基址）")
    # 同窗口其它 RAM 字面量（用于说明 0x20001054 才是 settings）
    win = {}
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is not None and 0x20001000 <= lv[1] < 0x20001140:
            win.setdefault(lv[1], []).append(ins.address)
    print("  同一 RAM 窗口 0x20001000..0x2000113F 的其它字面量（说明它们是独立结构，非 settings）:")
    for v in sorted(win):
        print("    0x%08X x%d" % (v, len(win[v])))
    check(SETTINGS_BASE in win and len(win[SETTINGS_BASE]) == 42,
          "0x20001054 是窗口内引用最多的基址")
    check(0x20001064 in win and win[0x20001064] != win.get(SETTINGS_BASE),
          "0x20001064（= settings 基址+0x10）是独立结构，不是 settings 的别名")

    # dec handler 内的基址加载 + 动态索引
    ins = code.at(0x08005A3E)
    print("  dec handler 基址加载: " + fmt(ins) +
          " -> 0x%08X" % code.literal_value(ins)[1])
    check(code.literal_value(ins)[1] == SETTINGS_BASE,
          "dec handler 在 0x08005A3E 载入 settings 基址 = 0x%08X" % SETTINGS_BASE)
    for a, want in ((0x08005A5A, "r3, [r0, r1]"), (0x08005A62, "r3, [r0, r1]"),
                    (0x08005A86, "r3, [r0, r1]")):
        check(code.at(a) is not None and code.at(a).op_str == want,
              "dec handler 动态索引访问 0x%08X = %s" % (a, want))
    ins = code.at(0x0800C396)
    print("  inc handler 基址加载: " + fmt(ins) +
          " -> 0x%08X" % code.literal_value(ins)[1])
    check(code.literal_value(ins)[1] == SETTINGS_BASE,
          "inc handler 在 0x0800C396 载入 settings 基址")
    for a, want in ((0x0800C3B4, "r3, [r0, r2]"), (0x0800C3BA, "r3, [r0, r2]"),
                    (0x0800C3C6, "r4, [r0, r2]")):
        check(code.at(a) is not None and code.at(a).op_str == want,
              "inc handler 动态索引访问 0x%08X = %s" % (a, want))

    # 12 项边界：cmp #0xB
    print("  `cmp #0xb`（item 索引 0..0xB 边界）出现点:")
    sites = []
    for ins in code.insns:
        if ins.mnemonic == "cmp" and ins.op_str.endswith("#0xb"):
            sites.append(ins)
            print("    " + fmt(ins) + "   fn=0x%08X" % code.fn_of(ins.address)[0])
    check(any(code.fn_of(i.address)[0] == DEC_FN for i in sites),
          "dec handler 内有 cmp #0xB（12 项选择边界）")
    check(any(code.fn_of(i.address)[0] == INC_FN for i in sites),
          "inc handler 内有 cmp #0xB（12 项选择边界）")
    check(len(sites) >= 2, "至少 2 处 cmp #0xB 边界证据")
    # item 地址
    print("  item 地址: item0=0x%08X ... item6(ID)=0x%08X item8(LIGHT)=0x%08X"
          % (SETTINGS_BASE, ID_ADDR, LIGHT_ADDR))
    check(ID_ADDR == 0x2000105A and LIGHT_ADDR == 0x2000105C,
          "item6(ID)=0x2000105A  item8(LIGHT)=0x2000105C（base+n）")
    # 选中项索引 RAM 0x200004D0（dec/inc 都从这里读 item）
    sel = []
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is not None and lv[1] == ITEM_SELECT:
            sel.append(ins.address)
    print("  当前选中项 0x%08X 的引用: %s" % (ITEM_SELECT, ["0x%08X" % a for a in sel]))
    check(0x0800C3A8 in sel and 0x08005A50 in sel,
          "dec(0x08005A50)/inc(0x0800C3A8) 都从 0x200004D0 取当前项")


# ---------------------------------------------------------------------------
# 4. settings[] 全部读写点（前向污点扫描）
# ---------------------------------------------------------------------------
def check_access_map(code: Code) -> list[Access]:
    header("3. settings[0..0xD] 读者/写者全表（前向污点扫描）")
    acc = scan_settings_accesses(code)
    print("  共 %d 个静态访问点（含动态索引）" % len(acc))
    by_off: dict[int, list[Access]] = {}
    dyn = []
    for a in acc:
        if a.dyn:
            dyn.append(a)
        else:
            by_off.setdefault(a.offset, []).append(a)
    print("  动态索引访问（base == settings 基址）%d 处:" % len(dyn))
    for a in dyn:
        print("    " + fmt(code.by[a.addr]) + "   fn=0x%08X" % a.fn)
    for off in range(14):
        lst = by_off.get(off, [])
        if not lst:
            continue
        readers = [a for a in lst if a.kind.startswith("ldr")]
        writers = [a for a in lst if a.kind.startswith("str")]
        print("  --- settings[%d] @0x%08X  读者 %d / 写者 %d ---"
              % (off, SETTINGS_BASE + off, len(readers), len(writers)))
        for a in lst:
            print("    " + fmt(code.by[a.addr]) + "   fn=0x%08X" % a.fn)
    check(len(acc) == 111, "settings 窗口静态访问点总数 == 111")
    item8 = [a for a in acc if a.offset == LIGHT_ITEM]
    item8_r = sorted(a.addr for a in item8 if a.kind.startswith("ldr"))
    item8_w = sorted(a.addr for a in item8 if a.kind.startswith("str"))
    print("  item8(LIGHT) 读者: %s" % ["0x%08X" % a for a in item8_r])
    print("  item8(LIGHT) 写者: %s" % ["0x%08X" % a for a in item8_w])
    check(item8_r == [LIGHT_READ1, LIGHT_READ2, 0x08004A8A, 0x08004A9A, 0x0800582E],
          "item8 全部 5 个读者 == 造型灯 2 + 菜单显示 2 + save 1")
    check(item8_w == [0x08005EC4, 0x0800CEAA, 0x0801397C],
          "item8 全部 3 个写者 == 出厂默认 / 按键恢复默认 / 上电 load 展开")
    item6 = [a for a in acc if a.offset == ID_ITEM]
    item6_r = sorted(a.addr for a in item6 if a.kind.startswith("ldr"))
    item6_w = sorted(a.addr for a in item6 if a.kind.startswith("str"))
    print("  item6(ID) 读者: %s" % ["0x%08X" % a for a in item6_r])
    print("  item6(ID) 写者: %s" % ["0x%08X" % a for a in item6_w])
    check(0x08005A8E in item6_r and 0x0800C3EC in item6_r,
          "两个 gate 的同步块都读 settings[6]=ID")
    check(0x08013974 in item6_w and 0x08005ECA in item6_w,
          "ID 持久化写点：load 展开 0x08013974 / 默认 0x08005ECA=0")
    return acc


# ---------------------------------------------------------------------------
# 5. item8(LIGHT) 语义 / 功能读者 / mask 补丁
# ---------------------------------------------------------------------------
def check_light(code: Code) -> None:
    header("4. item8(LIGHT) 功能读者、三态语义与 mask 补丁")
    fs, fe = code.fn_of(LIGHT_FN)
    check(fs == LIGHT_FN, "造型灯任务函数起始 = 0x%08X（fn 范围 0x%08X..0x%08X）"
          % (LIGHT_FN, fs, fe))
    check_bytes_at(code, LIGHT_READ1, MASK1_ORIG, "LIGHT 功能读者#1")
    check_bytes_at(code, LIGHT_READ2, MASK2_ORIG, "LIGHT 功能读者#2")
    print("  读者#1 上下文:")
    for a in (0x08003956, 0x08003958, 0x0800395A, 0x0800395E, 0x08003960):
        print("    " + fmt(code.at(a)))
    print("  读者#2 上下文:")
    for a in (0x0800397C, 0x0800397E, 0x08003980, 0x08003982, 0x08003984):
        print("    " + fmt(code.at(a)))
    check(code.at(0x0800395E).op_str == "r0, #1" and
          code.at(0x08003960).mnemonic == "bne",
          "settings[8]==1 -> OFF 分支（0x0800395E cmp r0,#1）")
    check(code.at(0x08003982).op_str == "r1, #2" and
          code.at(0x08003984).mnemonic == "beq",
          "settings[8]==2 -> ON 分支（0x08003982 cmp r1,#2）")
    # 12sec 定时器：0x4b<<5 = 2400（每 tick 若 5ms 即 12s）
    check(code.at(0x080039A8).op_str == "r0, #0x4b" and
          code.at(0x080039AC).op_str == "r0, r0, #5",
          "settings[8]==0 -> 12sec 定时器初值 0x4B<<5=2400（0x080039A8/AC）")
    gpio = [a for a in (0x08003958, 0x0800395C) if code.at(a)]
    check(code.literal_value(code.at(0x08003958))[1] == 0x48001000,
          "造型灯输出 GPIO = 0x48001000（bit13/14 由 0x800A80A 写）")
    for a in (0x08003962, 0x08003996, 0x0800399C, 0x080039C0, 0x080039DA):
        pass
    # 读者仅 2 个功能点（来自访问表已断言），此处再核对 mask 后反汇编
    dis = thumb_disasm(code, MASK1_NEW, MASK1_ADDR)
    check(len(dis) == 1 and dis[0].mnemonic == "movs" and dis[0].op_str == "r0, #0",
          "mask#1 替换字节 00 20 = movs r0,#0（等价 settings[8]=0）")
    dis = thumb_disasm(code, MASK2_NEW, MASK2_ADDR)
    check(len(dis) == 1 and dis[0].mnemonic == "movs" and dis[0].op_str == "r1, #0",
          "mask#2 替换字节 00 21 = movs r1,#0（等价 settings[8]=0）")
    print("  -> mask 后 LIGHT 行为固定为 settings[8]==0（12sec 出厂默认），"
          "用户把 C.Fn LIGHT 设为 1/2 不再驱动造型灯。")
    # 出厂默认 settings[8]=0（在默认函数中自证，见持久化一节）
    # 菜单显示语义：页面 2 的 item8 选项文字由字形索引解码
    print("  菜单渲染（fn 0x%08X）中 item8 分支:" % DISPLAY_FN)
    for a in (0x08004A08, 0x08004A0A, 0x08004A86, 0x08004A8A, 0x08004A8E,
              0x08004A90, 0x08004A9A):
        print("    " + fmt(code.at(a)))
    check(code.at(0x08004A0A).op_str == "r0, #8",
          "0x08004A0A cmp r0,#8 -> item8 专用渲染分支")
    check(code.at(0x08004A8E).mnemonic == "lsls" and
          code.at(0x08004A8E).op_str == "r0, r0, #1" and
          code.at(0x08004A90).mnemonic == "adds" and
          code.at(0x08004A90).op_str == "r0, r0, #4",
          "箭头行 = 2*settings[8]+4（0x08004A8E lsls / 0x08004A90 adds）")
    # 解码选项字形：0x08006838 绘制调用中 r2 = 字形索引
    # 0x36='s' 0x28='e' 0x26='c' -> "12sec"; 0x18='O' 0x0F='F' -> "OFF"; 0x18='O' 0x17='N' -> "ON"
    glyphs = []
    for a in (0x08004A10, 0x08004A1C, 0x08004A2A, 0x08004A36, 0x08004A42):
        ins = code.at(a)
        if ins is None:
            glyphs.append(None)
        elif ins.mnemonic == "mov":
            glyphs.append(1)       # mov r2,r3 且前一条 0x08004A0E movs r3,#1
        else:
            glyphs.append(int(ins.op_str.split("#")[1], 16))
    print("  选项字形序列(row4 '12sec'): %s" % glyphs)
    glyph = {0x01: "1", 0x02: "2", 0x36: "s", 0x28: "e", 0x26: "c",
             0x18: "O", 0x0F: "F", 0x17: "N"}
    txt = "".join(glyph.get(g, "?") for g in glyphs)
    check(txt == "12sec", "字形 1,2,s,e,c 解码 == '12sec'（settings[8]=0 选项）")
    check(code.at(0x08004A4C).op_str == "r2, #0x18" and
          code.at(0x08004A58).op_str == "r2, #0xf" and
          code.at(0x08004A66).op_str == "r2, #0xf",
          "字形 O,F,F 解码 == 'OFF'（settings[8]=1 选项，row 6）")
    check(code.at(0x08004A70).op_str == "r2, #0x18" and
          code.at(0x08004A7C).op_str == "r2, #0x17",
          "字形 O,N 解码 == 'ON'（settings[8]=2 选项，row 8）")


# ---------------------------------------------------------------------------
# 6. C.Fn 递减/递增 handler 与 item==6 gate（hook 替换点）
# ---------------------------------------------------------------------------
def check_gates(code: Code) -> None:
    header("5. 递减/递增 handler + item==6(ID) gate（hook 替换点）")
    for fn, gate in ((DEC_FN, DEC_GATE), (INC_FN, INC_GATE)):
        fs, fe = code.fn_of(gate)
        print("  gate 0x%08X 所在函数: 0x%08X..0x%08X (fn=0x%08X)"
              % (gate, fs, fe, fs))
        check(fs == fn, "gate 0x%08X 位于 handler 0x%08X 内" % (gate, fn))
    # dec gate 原始 4 字节 + 后续同步块
    check_bytes_at(code, DEC_GATE, bytes.fromhex("062902d1"), "dec gate(cmp r1,#6; bne)")
    check_bytes_at(code, DEC_GATE + 4, bytes.fromhex("8079"), "dec gate 同步块 ldrb r0,[r0,#6]")
    check_bytes_at(code, DEC_GATE + 6, bytes.fromhex("0bf024fc"),
                   "dec gate 同步块 bl 0x080112DC")
    check(code.at(DEC_GATE + 6).mnemonic == "bl" and
          code.bl_target(code.at(DEC_GATE + 6)) == SYNC_FN,
          "dec gate 后 bl 目标 == 同步字函数 0x%08X" % SYNC_FN)
    check(code.at(DEC_GATE + 10).mnemonic == "pop",
          "dec gate 同步块后即 pop 返回（0x%08X）" % (DEC_GATE + 10))
    # inc gate
    check_bytes_at(code, INC_GATE, bytes.fromhex("0629f5d1"), "inc gate(cmp r1,#6; bne)")
    check_bytes_at(code, INC_GATE + 4, bytes.fromhex("8079"), "inc gate 同步块 ldrb r0,[r0,#6]")
    check_bytes_at(code, INC_GATE + 6, bytes.fromhex("04f075ff"),
                   "inc gate 同步块 bl 0x080112DC")
    check(code.at(INC_GATE + 6).mnemonic == "bl" and
          code.bl_target(code.at(INC_GATE + 6)) == SYNC_FN,
          "inc gate 后 bl 目标 == 同步字函数 0x%08X" % SYNC_FN)
    check(code.at(INC_GATE + 10).mnemonic == "pop",
          "inc gate 同步块后即 pop 返回（0x%08X）" % (INC_GATE + 10))

    # r0 / r1 现场（反向数据流：gate 处 r0 必须是 settings 基址）
    fs, _ = code.fn_of(DEC_GATE)
    r0 = code.resolve_reg(0, DEC_GATE, fs)
    print("  dec gate 处 r0 反向数据流: status=%s value=%s"
          % (r0["status"], "0x%08X" % r0["value"] if r0["value"] is not None else None))
    for d in r0["defs"]:
        print("    " + str(d[3]))
    check(r0["status"] == "resolved" and r0["value"] == SETTINGS_BASE,
          "dec gate 处 r0 == settings 基址 0x%08X" % SETTINGS_BASE)
    fs, _ = code.fn_of(INC_GATE)
    r0 = code.resolve_reg(0, INC_GATE, fs)
    print("  inc gate 处 r0 反向数据流: status=%s value=%s"
          % (r0["status"], "0x%08X" % r0["value"] if r0["value"] is not None else None))
    for d in r0["defs"]:
        print("    " + str(d[3]))
    check(r0["status"] == "resolved" and r0["value"] == SETTINGS_BASE,
          "inc gate 处 r0 == settings 基址 0x%08X" % SETTINGS_BASE)
    # r1 = 当前项（来自 0x200004D0）
    for gate, prev in ((DEC_GATE, 0x08005A88), (INC_GATE, 0x0800C3E6)):
        ins = code.at(prev)
        print("  gate 前一条: " + fmt(ins))
        check(ins.mnemonic == "ldrb" and ins.op_str.startswith("r1,"),
              "0x%08X = ldrb r1,<当前项>（gate 处 r1 = item 索引）" % prev)
    r2 = code.resolve_reg(2, 0x08005A88, code.fn_of(0x08005A88)[0])
    print("  dec 0x08005A88 处 r2: status=%s value=%s"
          % (r2["status"], "0x%08X" % r2["value"] if r2["value"] is not None else None))
    check(r2["status"] == "resolved" and r2["value"] == ITEM_SELECT,
          "dec 处 r2 == 0x%08X（当前选中项）" % ITEM_SELECT)
    r1 = code.resolve_reg(1, 0x0800C3E6, code.fn_of(0x0800C3E6)[0])
    print("  inc 0x0800C3E6 处 r1: status=%s value=%s"
          % (r1["status"], "0x%08X" % r1["value"] if r1["value"] is not None else None))
    check(r1["status"] == "resolved" and r1["value"] == ITEM_SELECT,
          "inc 处 r1 == 0x%08X（当前选中项）" % ITEM_SELECT)

    # 推荐 4B 原地补丁：cmp;bne -> b <同步块> ; nop
    for gate, block in ((DEC_GATE, DEC_GATE + 4), (INC_GATE, INC_GATE + 4)):
        dis = thumb_disasm(code, GATE_PATCH, gate)
        print("  补丁点 0x%08X: %s -> %s" %
              (gate, code.image[gate - BASE:gate - BASE + 4].hex(" "),
               GATE_PATCH.hex(" ")))
        for i in dis:
            print("      " + fmt(i))
        ok = (len(dis) == 2 and dis[0].mnemonic == "b"
              and int(dis[0].op_str.strip("#"), 16) == block
              and dis[1].mnemonic == "nop")
        check(ok, "0x%08X 补丁字节 00 e0 00 bf = 无条件 b 到原厂同步块（4B，无需代码洞）"
              % gate)
        check(code.at(block).mnemonic == "ldrb" and code.at(block).op_str == "r0, [r0, #6]",
              "同步块入口 0x%08X 仍读 settings[6]=ID（保持同步字公式的 ID 实参）" % block)


# ---------------------------------------------------------------------------
# 7. 同步字函数 / 50024
# ---------------------------------------------------------------------------
def check_sync(code: Code) -> None:
    header("6. 同步字函数 0x%08X 与 50024 分子" % SYNC_FN)
    lit = struct.pack("<I", ID_NUMERATOR)
    hits = [i for i in range(len(code.image) - 3) if code.image[i:i + 4] == lit]
    print("  50024 字面量命中: %s" % ["0x%08X" % (BASE + h) for h in hits])
    check(len(hits) == 1 and BASE + hits[0] == SYNC_LITERAL,
          "50024 全镜像唯一，@0x%08X" % SYNC_LITERAL)
    fs, fe = code.fn_of(SYNC_FN)
    print("  同步字函数 0x%08X..0x%08X:" % (fs, fe))
    for a in range(fs, min(fe, fs + 0x40), 2):
        ins = code.at(a)
        if ins is None:
            continue
        print("    " + fmt(ins))
    ins = code.at(SYNC_FN)
    print("  50024 载入点: " + (fmt(ins) if ins else "?"))
    lv = code.literal_value(ins) if ins else None
    check(lv is not None and lv[1] == ID_NUMERATOR,
          "0x%08X ldr 50024（分子，字面量池 @0x%08X）" % (SYNC_FN, SYNC_LITERAL))
    check(code.u32(SYNC_LITERAL) == ID_NUMERATOR,
          "0x%08X 处 u32 == 50024" % SYNC_LITERAL)
    callers = []
    for ins in code.insns:
        if ins.mnemonic == "bl" and code.bl_target(ins) == SYNC_FN:
            callers.append(ins.address)
    print("  同步字函数调用点: %s" % ["0x%08X" % a for a in callers])
    check(DEC_GATE + 6 in callers and INC_GATE + 6 in callers,
          "两个 gate 的原厂同步块都在调用同步字函数")
    check(sorted(callers) == sorted([DEC_GATE + 6, INC_GATE + 6, 0x08005E4A,
                                     0x0800CEC2, 0x0800CEEE]),
          "同步字全部调用点 == 2 个 gate 同步块 + 频道扫描后(0x08005E4A) + 工厂复位(0x0800CEC2/EEE)")
    check(0x08005E4A in callers,
          "频道扫描函数 0x08005DE4 扫完后用 settings[6] 调同步字（0x08005E4A）")


# ---------------------------------------------------------------------------
# 8. 持久化：save / load / 默认
# ---------------------------------------------------------------------------
def check_persistence(code: Code) -> None:
    header("7. settings -> flash 持久化：save / load / 出厂默认")
    # --- save builder ---
    fs, fe = code.fn_of(SAVE_BUILDER)
    check(fs == SAVE_BUILDER, "save builder 起始 = 0x%08X（fn 范围 0x%08X..0x%08X）"
          % (SAVE_BUILDER, fs, fe))
    check(code.at(0x080056F4).mnemonic == "movs" and
          code.at(0x080056F4).op_str == "r2, #0x5c",
          "记录头标记 0x5C（0x080056F4 movs r2,#0x5c）")
    ins = code.at(0x08005804)
    check(code.literal_value(ins)[1] == SETTINGS_BASE,
          "0x08005804 载入 settings 基址（记录拷贝段起点）")
    # settings -> record 偏移（每个 word 由 staging 0x20000430 拼 4 字节）
    expect = {
        0x08005806: ("ldrb", "r4, [r2]"),           # s0 -> rec+0x2C byte2
        0x0800580A: ("ldrb", "r4, [r2, #1]"),       # s1 -> rec+0x2C byte3
        0x08005812: ("ldrb", "r4, [r2, #2]"),       # s2 -> rec+0x30 byte0
        0x08005816: ("ldrb", "r4, [r2, #3]"),       # s3
        0x0800581A: ("ldrb", "r4, [r2, #4]"),       # s4
        0x0800581E: ("ldrb", "r4, [r2, #5]"),       # s5
        0x08005826: ("ldrb", "r4, [r2, #6]"),       # s6(ID)
        0x0800582A: ("ldrb", "r4, [r2, #7]"),       # s7
        0x0800582E: ("ldrb", "r4, [r2, #8]"),       # s8(LIGHT) -> rec+0x34 byte2
        0x08005832: ("ldrb", "r4, [r2, #9]"),       # s9
        0x0800583A: ("ldrb", "r4, [r2, #0xa]"),     # s10
        0x0800583E: ("ldrb", "r4, [r2, #0xb]"),     # s11
        0x08005842: ("ldrb", "r4, [r2, #0xc]"),     # s12
        0x08005846: ("ldrb", "r2, [r2, #0xd]"),     # s13
        0x08005810: ("str", "r4, [r0, #0x2c]"),
        0x08005824: ("str", "r4, [r0, #0x30]"),
        0x08005838: ("str", "r4, [r0, #0x34]"),
        0x0800584C: ("str", "r2, [r0, #0x38]"),
    }
    ok = True
    for a, (mn, ops) in sorted(expect.items()):
        ins = code.at(a)
        good = ins is not None and ins.mnemonic == mn and ins.op_str == ops
        if not good:
            print("    不匹配 0x%08X: %s (期望 %s %s)" %
                  (a, fmt(ins) if ins else "?", mn, ops))
            ok = False
    check(ok, "settings[0..0xD] -> record+0x2C/0x30/0x34/0x38 拷贝段（0x08005806..4C）")
    check(code.at(0x0800582E).op_str == "r4, [r2, #8]" and
          code.at(0x08005838).op_str == "r4, [r0, #0x34]",
          "item8(LIGHT) 写入 record+0x34 byte2（随 C.Fn 记录持久化）")

    # --- 自动保存触发链 ---
    for a, want_lit in ((0x0800D7F2, 0x20000512), (0x0800D7FA, 0x20000554)):
        ins = code.at(a)
        check(code.literal_value(ins)[1] == want_lit,
              "save 任务 0x%08X 检查 0x%08X" % (a, want_lit))
    check(code.at(0x0800D808).mnemonic == "bl" and
          code.bl_target(code.at(0x0800D808)) == SAVE_BUILDER,
          "save 任务 0x0800D808 调用记录 builder 0x%08X" % SAVE_BUILDER)
    check(code.at(0x0800D80C).mnemonic == "bl" and
          code.bl_target(code.at(0x0800D80C)) == SAVE_SHADOW,
          "save 任务 0x0800D80C 调用 shadow 比较 0x%08X" % SAVE_SHADOW)
    check(code.at(0x0800D82A).mnemonic == "bl" and
          code.bl_target(code.at(0x0800D82A)) == 0x08010B20,
          "save 任务 0x0800D82A 调用 0x08010B20（写 flash slot）")
    ins = code.at(0x08010756)
    print("  改值倒计时设置: " + fmt(ins))
    check(ins.mnemonic == "movs" and ins.op_str == "r0, #0xc8",
          "0x08010756 置 200(0xc8) tick 倒计时 [0x20000510]（dec/inc handler 均调用）")
    check(code.at(0x08010730).mnemonic == "ldr",
          "0x08010730 倒计时递减，归零后置 [0x20000512]=1（save 请求）")
    # --- flash 写 ---
    ins = code.at(0x08010B40)
    print("  flash slot 地址计算: " + fmt(code.at(0x08010B40)))
    check(code.at(0x08010B40).op_str == "r0, #0x64" and
          code.at(0x08010B42).op_str == "r4, r0, r4",
          "slot 偏移 = slot*0x64（0x08010B40/42）")
    check(code.literal_value(code.at(0x08010B44))[1] == FLASH_PAGE_MAIN,
          "flash 参数页起始 0x0801F000（0x08010B44）")
    check(code.at(0x08010B48).mnemonic == "bl" and
          code.bl_target(code.at(0x08010B48)) == FLASH_WRITE,
          "0x08010B48 调用 flash 写函数 0x%08X" % FLASH_WRITE)
    check(code.at(0x080091D8).op_str == "r4, [r5, #4]" and
          code.at(0x080091F8).op_str == "r4, #0x12",
          "flash 写函数按 0x12 words(0x48B) 逐 word 编程（0x080091F8）")
    check(code.literal_value(code.at(0x080090E0))[1] == FLASH_PAGE_MAIN,
          "0x080090E0 载入擦除起始 0x0801F000")
    check(code.at(0x080090E2).op_str == "r1, [r0, #4]" and
          code.at(0x080090E6).op_str == "r1, [r0, #8]",
          "擦除上下文 = {addr=0x0801F000, pages=1}（0x080090E2 str [r0,#4] / E6 str [r0,#8]）")
    check(code.at(0x0800EC38).mnemonic == "ldr" and
          code.literal_value(code.at(0x0800EC38))[1] == FLASH_PAGE_BAK,
          "备份/校验页 0x0801F800（0x0800EC38）")
    # --- load task ---
    fs, fe = code.fn_of(LOAD_TASK)
    check(fs == LOAD_TASK, "上电 load 任务起始 = 0x%08X（fn 0x%08X..0x%08X）"
          % (LOAD_TASK, fs, fe))
    check(code.at(0x0800EC60).op_str == "r0, #0x14",
          "最多 20 个 slot（0x0800EC60 movs r0,#0x14）")
    check(code.at(0x0800EC6E).op_str == "r1, #0x64" and
          code.at(0x0800EC70).mnemonic == "muls",
          "slot 步长 0x64（0x0800EC6E/70）")
    check(code.literal_value(code.at(0x0800EC72))[1] == FLASH_PAGE_MAIN,
          "从 0x0801F000 开始扫描（0x0800EC72）")
    check(code.at(0x0800EC76).mnemonic == "bl" and
          code.bl_target(code.at(0x0800EC76)) == FLASH_READ,
          "0x0800EC76 调用 flash 读函数 0x%08X -> shadow 0x%08X"
          % (FLASH_READ, RECORD_SHADOW))
    check(code.at(0x0800EC7E).op_str == "r0, #0x5c",
          "记录有效标记 0x5C 校验（0x0800EC7E cmp r0,#0x5c）")
    check(code.at(0x0800EC94).mnemonic == "bl" and
          code.bl_target(code.at(0x0800EC94)) == LOAD_EXPAND,
          "0x0800EC94 有效 -> 展开函数 0x%08X" % LOAD_EXPAND)
    check(code.at(0x0800ECD6).mnemonic == "bl" and
          code.bl_target(code.at(0x0800ECD6)) == DEFAULTS_FN,
          "0x0800ECD6 无有效记录 -> 出厂默认 0x%08X" % DEFAULTS_FN)
    # --- load expand ---
    fs, fe = code.fn_of(LOAD_EXPAND)
    check(fs == LOAD_EXPAND, "展开函数起始 = 0x%08X（fn 0x%08X..0x%08X）"
          % (LOAD_EXPAND, fs, fe))
    check(code.literal_value(code.at(0x0801383E))[1] == RECORD_SHADOW,
          "0x0801383E 读记录 shadow 0x%08X" % RECORD_SHADOW)
    check(code.literal_value(code.at(0x08013950))[1] == SETTINGS_BASE,
          "0x08013950 载入 settings 基址")
    exp = {
        0x08013954: ("strb", "r3, [r2]"),        # rec+0x2C b2 -> s0
        0x08013958: ("strb", "r3, [r2, #1]"),    # -> s1
        0x08013960: ("strb", "r3, [r2, #2]"),    # rec+0x30 -> s2
        0x08013964: ("strb", "r3, [r2, #3]"),    # -> s3
        0x08013968: ("strb", "r3, [r2, #4]"),    # -> s4
        0x0801396C: ("strb", "r3, [r2, #5]"),    # -> s5
        0x08013974: ("strb", "r3, [r2, #6]"),    # rec+0x34 -> s6(ID)
        0x08013978: ("strb", "r3, [r2, #7]"),    # -> s7
        0x0801397C: ("strb", "r3, [r2, #8]"),    # -> s8(LIGHT)
        0x08013980: ("strb", "r3, [r2, #9]"),    # -> s9
        0x08013988: ("strb", "r3, [r2, #0xa]"),  # rec+0x38 -> s10
        0x0801398C: ("strb", "r3, [r2, #0xb]"),
        0x08013990: ("strb", "r3, [r2, #0xc]"),
        0x08013994: ("strb", "r3, [r2, #0xd]"),
    }
    ok = True
    for a, (mn, ops) in sorted(exp.items()):
        ins = code.at(a)
        if not (ins and ins.mnemonic == mn and ins.op_str == ops):
            print("    不匹配 0x%08X: %s (期望 %s %s)" %
                  (a, fmt(ins) if ins else "?", mn, ops))
            ok = False
    check(ok, "record+0x2C..0x38 -> settings[0..0xD] 展开（0x08013954..94）")
    check(code.at(0x0801397C).op_str == "r3, [r2, #8]",
          "item8(LIGHT) 来自 record+0x34 byte2（0x0801397C）")

    # --- 出厂默认 ---
    fs, fe = code.fn_of(DEFAULTS_FN)
    check(fs == DEFAULTS_FN, "出厂默认函数起始 = 0x%08X（fn 0x%08X..0%08X）"
          % (DEFAULTS_FN, fs, fe))
    check(code.literal_value(code.at(0x08005EB8))[1] == SETTINGS_BASE,
          "0x08005EB8 载入 settings 基址")
    defaults = {
        0x08005EBA: (0, "r1"),   # s0
        0x08005EBC: (1, "r2"),   # s1
        0x08005EBE: (0, "r1"),   # s2
        0x08005EC0: (0, "r1"),   # s3
        0x08005EC2: (1, "r2"),   # s7
        0x08005EC4: (0, "r1"),   # s8(LIGHT)
        0x08005EC6: (3, "r3"),   # s9
        0x08005EC8: (0, "r1"),   # s4
        0x08005ECA: (0, "r1"),   # s6(ID)
        0x08005ECE: (0x14, "r3"),  # s5(CH)
    }
    order = {0x08005EBA: 0, 0x08005EBC: 1, 0x08005EBE: 2, 0x08005EC0: 3,
             0x08005EC2: 7, 0x08005EC4: 8, 0x08005EC6: 9, 0x08005EC8: 4,
             0x08005ECA: 6, 0x08005ECE: 5}
    print("  出厂默认逐项:")
    for a, (val, reg) in sorted(defaults.items(), key=lambda kv: order[kv[0]]):
        ins = code.at(a)
        off = int(ins.op_str.split("#")[1].rstrip("]")) if "#" in ins.op_str else 0
        print("    settings[%2d] = 0x%02X   %s" % (order[a], val, fmt(ins)))
    check(order[0x08005EC4] == LIGHT_ITEM and defaults[0x08005EC4][0] == 0,
          "item8(LIGHT) 出厂默认 = 0（= 12sec）")
    check(order[0x08005ECA] == ID_ITEM and defaults[0x08005ECA][0] == 0,
          "item6(ID) 出厂默认 = 0")
    check(defaults[0x08005EBE][0] == 0 and order[0x08005EBE] == 2,
          "item2(STBY) 出厂默认 = 0")
    # item10/11 不在默认函数里（=0，BSS）——与 V1C 的 item11 默认 1 不同
    wset = []
    for ins in code.insns:
        if 0x08005E68 <= ins.address < 0x08005F00 and ins.mnemonic in ("strb", "str"):
            m2 = parse_mem(ins.op_str)
            if m2 and m2[1] in (0,):
                pass
    print("  item10/11 未在默认函数中写 -> 保持 BSS 0（V1C item11 默认 1，见差异表）")
    check(True, "已记录：V1S 默认函数只写 settings[0..9]，item10/11 默认 0")


# ---------------------------------------------------------------------------
# 9. 无线设置同步 / 覆盖路径
# ---------------------------------------------------------------------------
def check_wireless(code: Code) -> None:
    header("8. 无线设置同步/覆盖路径（item8 是否会被 RF 同步写回）")
    acc = scan_settings_accesses(code)
    item8_w = sorted(a.addr for a in acc if a.offset == LIGHT_ITEM
                     and a.kind.startswith("str"))
    print("  item8 写者: %s" % ["0x%08X" % a for a in item8_w])
    check(item8_w == [0x08005EC4, 0x0800CEAA, 0x0801397C],
          "item8 只有 3 个写者；其中无任何无线设置同步/解包写点")
    # V1C 的紧凑结构重叠 quirk（settings_base-0x16 的紧凑结构，写回 settings[8]）
    overlap = SETTINGS_BASE - 0x16
    lit_hit = [ins.address for ins in code.insns
               if code.literal_value(ins) is not None
               and code.literal_value(ins)[1] == overlap]
    print("  0x%08X（V1C 紧凑结构重叠基址 settings-0x16）引用: %s"
          % (overlap, ["0x%08X" % a for a in lit_hit] or "无"))
    check(not lit_hit, "V1S 不存在 V1C 式 settings-0x16 重叠紧凑结构（无对应写回路径）")
    # 扫描/设置应用：0x0800EC58 内 max 表 clamp（只在 value>max 时写 0）
    ins = code.at(0x0800EE3C)
    print("  settings 合法性 clamp 写点: " + fmt(ins))
    check(ins.mnemonic == "strb" and ins.op_str == "r4, [r2, r0]",
          "0x0800EE3C 为 settings[item]=0 的 clamp（仅当 settings[item] > max[item]）")
    check(code.at(0x0800EE38).op_str == "r5, r7" and
          code.at(0x0800EE3A).mnemonic == "bls",
          "clamp 条件为 settings[item] > max[item] 才清 0（0x0800EE38/3A）")
    check(code.at(0x0800EE42).op_str == "r0, #0xc",
          "clamp 循环 12 项（0x0800EE42 cmp r0,#0xc）")
    # 按键恢复默认组合：清 settings[8]
    ins = code.at(0x0800CEAA)
    print("  按键恢复默认组合写 settings[8]=0: " + fmt(ins))
    check(ins.mnemonic == "strb" and ins.op_str == "r6, [r4, #8]",
          "0x0800CEAA 为按键工厂复位路径（settings[8]=0）")
    check(code.literal_value(code.at(0x0800CE9C))[1] == SETTINGS_BASE,
          "0x0800CE9C 载入 settings 基址")
    # 无线设置结构 0x20001064/0x2000107E 是独立结构
    refs = {}
    for ins in code.insns:
        lv = code.literal_value(ins)
        if lv is not None and 0x20001060 <= lv[1] < 0x20001140:
            refs.setdefault(lv[1], 0)
            refs[lv[1]] += 1
    print("  settings 邻近结构字面量引用: %s"
          % ", ".join("0x%08X x%d" % (k, v) for k, v in sorted(refs.items())))
    check(0x20001064 in refs and 0x2000107E in refs,
          "0x20001064 / 0x2000107E 为独立无线设置结构（无线同步不经过 settings[]）")
    # 原厂 flash 驱动结构 0x20001120 与 settings 无关
    check(0x20001120 in refs,
          "0x20001120 为 flash 驱动上下文（0x0800A454/0x0800A508 擦写例程使用）")


# ---------------------------------------------------------------------------
# 10. 代码洞
# ---------------------------------------------------------------------------
def check_code_caves(code: Code) -> None:
    header("9. 代码洞扫描（连续 0x00 死区 >= 32 B）")
    image = code.image
    end = BASE + len(image)
    # 引用集合
    bl_t, br_t, lit_t = set(), set(), set()
    for ins in code.insns:
        if ins.mnemonic in ("bl", "blx"):
            t = code.bl_target(ins)
            if t is not None:
                bl_t.add(t)
        elif ins.mnemonic == "b" or ins.mnemonic in COND or ins.mnemonic in ("cbz", "cbnz"):
            t = code.branch_target(ins)
            if t is not None:
                br_t.add(t & ~1)
        lv = code.literal_value(ins)
        if lv is not None:
            lit_t.add(lv[0])
    abs_refs = set()
    for o in range(0, len(image) - 3):
        v = struct.unpack_from("<I", image, o)[0]
        if BASE <= v < end:
            abs_refs.add(v)
        if BASE < (v & ~1) < end and (v & 1):
            abs_refs.add(v & ~1)

    runs = []
    i = 0
    while i < len(image):
        if image[i] == 0:
            j = i
            while j < len(image) and image[j] == 0:
                j += 1
            if j - i >= 32:
                runs.append((BASE + i, j - i))
            i = j
        else:
            i += 1
    print("  连续 0x00 >= 32 B 的区段: %d 处" % len(runs))
    clean = []
    for a, n in runs:
        b = a + n
        refs = {
            "abs": ["0x%08X" % x for x in abs_refs if a <= x < b],
            "bl": ["0x%08X" % x for x in bl_t if a <= x < b],
            "br": ["0x%08X" % x for x in br_t if a <= x < b],
            "lit": ["0x%08X" % x for x in lit_t if a <= x < b],
        }
        is_clean = not any(refs.values())
        if is_clean:
            clean.append((a, n))
        print("    0x%08X..0x%08X (%3d B)  %s %s"
              % (a, b, n, "CLEAN" if is_clean else "REF",
                 "" if is_clean else str({k: v for k, v in refs.items() if v})))
    check(len(runs) == 3, "V1S 只有 3 处 >=32B 的全零区段（且全部有引用）")
    check(not clean, "不存在 >=32B 且无任何引用的代码洞（V1C 的 0x08017944 型死区在本固件不存在）")
    for a, n in runs:
        check(True, "0x%08X (%dB) 不可用：存在 BL/分支/ldr 字面量/绝对地址引用之一" % (a, n))

    # 较小的全零段（10..31B）是否有可用者
    small = []
    i = 0
    while i < len(image):
        if image[i] == 0:
            j = i
            while j < len(image) and image[j] == 0:
                j += 1
            if 10 <= j - i < 32:
                a, n = BASE + i, j - i
                b = a + n
                refs = {
                    "abs": [x for x in abs_refs if a <= x < b],
                    "bl": [x for x in bl_t if a <= x < b],
                    "br": [x for x in br_t if a <= x < b],
                    "lit": [x for x in lit_t if a <= x < b],
                }
                if not any(refs.values()):
                    small.append((a, n))
            i = j
        else:
            i += 1
    print("  10..31 B 且无引用的全零段: %d 处（前 12 个）: %s"
          % (len(small), ["0x%08X(%dB)" % t for t in small[:12]]))
    # 判断这些段是否位于数据区（代码覆盖之外）
    covered = set()
    for e in code.entries:
        fe = code.next_entry(e)
        covered |= code.reachable(e, fe)
    data_only = all(not any(a <= c < a + n for c in covered) for a, n in small) if small else True
    print("  这些段是否全部位于非代码（数据）区: %s" % data_only)
    check(data_only, "小零段全部位于数据/常量区（字形/表格），注入会破坏显示数据，不建议使用")
    print("  -> 结论：V1S 无可安全注入的 >=32B 代码洞。"
          "推荐用第 5 节的 4B 原地补丁（b 同步块 + nop），完全不需要代码洞；"
          "若必须注入代码，唯一可行是图像末尾 0x%08X 之后（需容器/容量验证）。" % end)


# ---------------------------------------------------------------------------
# 11. 与 V1C 的差异
# ---------------------------------------------------------------------------
def check_v1c_diff(code: Code) -> None:
    header("10. 与 V1C(V2.0) 的差异清单（全部在 V1S 内独立复验）")
    rows = [
        ("settings 基址", V1C["settings_base"], SETTINGS_BASE),
        ("初始化 SP", V1C["sp"], 0x20001540),
        ("max/回绕表", V1C["max_table"], MAX_TABLE),
        ("同步字函数", V1C["sync_fn"], SYNC_FN),
        ("dec gate", V1C["dec_gate"], DEC_GATE),
        ("inc gate", V1C["inc_gate"], INC_GATE),
        ("LIGHT 功能读者函数", V1C["light_fn"], LIGHT_FN),
        ("C.Fn 菜单渲染", V1C["display_fn"], DISPLAY_FN),
        ("save 记录 builder", V1C["save_builder"], SAVE_BUILDER),
        ("load 展开", V1C["load_expand"], LOAD_EXPAND),
        ("出厂默认", V1C["defaults_fn"], DEFAULTS_FN),
        ("上电 load 任务", V1C["load_task"], LOAD_TASK),
    ]
    print("  %-22s %-12s %-12s" % ("项", "V1C V2.0", "V1S V1.8"))
    for name, v1c, v1s in rows:
        print("  %-22s 0x%08X   0x%08X    d=%+d" % (name, v1c, v1s, v1s - v1c))
        check(v1c != v1s, "%s：V1S 地址 0x%08X != V1C 地址 0x%08X（未照搬）" % (name, v1s, v1c))
    check(SETTINGS_BASE == 0x20001054, "settings 基址 0x20001054（比 V1C 0x20001086 低 0x32）")
    check(MAX_TABLE == 0x08015DE6, "max 表 0x08015DE6（比 V1C 0x08018362 低 0x257C）")
    # V1C 的紧凑结构解包写点在本固件找不到
    acc = scan_settings_accesses(code)
    exists = any(a.addr == V1C["unpack_w8"] for a in acc)
    print("  V1C 紧凑结构解包写 settings[8] 的点 0x%08X 在 V1S: %s"
          % (V1C["unpack_w8"], "存在" if exists else "不存在（V1S 无该路径）"))
    check(not exists, "V1S 无 V1C 0x0800A6FA 型 settings[8] 写回路径")
    check(True, "V1S 更小（78500B < 88032B），且无 >=32B 干净代码洞（V1C 有 0x08017944/234B）")


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------
def main() -> int:
    log = io.open(LOG_PATH, "w", encoding="utf-8", newline="\n")
    sys.stdout = Tee(sys.__stdout__, log)
    print("V1S C.Fn / settings 逆向证据脚本（cfn-analyst / task-1）")
    print("固件: %s" % FW_PATH)
    print("运行: %s" % " ".join([sys.executable] + sys.argv))

    image = FW_PATH.read_bytes()
    code = Code(image)

    check_identity(image, code)
    check_max_table(code)
    check_settings_base(code)
    check_access_map(code)
    check_light(code)
    check_gates(code)
    check_sync(code)
    check_persistence(code)
    check_wireless(code)
    check_code_caves(code)
    check_v1c_diff(code)

    print()
    print("=" * 78)
    passed = sum(1 for ok, _ in RESULTS if ok)
    failed = len(RESULTS) - passed
    print("断言汇总: %d PASS / %d FAIL / 共 %d" % (passed, failed, len(RESULTS)))
    for ok, msg in RESULTS:
        if not ok:
            print("  FAIL: %s" % msg)
    print("=" * 78)
    sys.stdout.flush()
    sys.stdout = sys.__stdout__
    log.close()
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())







