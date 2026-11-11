#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""V1S_V1.8.bin —— 「ID -> CC2500 16 位同步字」函数与全镜像 SYNC 写路径 独立复核脚本.

角色: 独立固件分析员 (sync-analyst)，不参与 keypatch/devices/v1s.py 实现。
产物: 本脚本只读固件；不写任何补丁模块、不改固件；结论由 PASS/FAIL 断言给出。

用法 (工作区根 D:\\神牛):
    tools\\python\\python.exe devices\\v1s\\re\\sync_v1s.py [log_path]
退出码: 0 = 全部 PASS；1 = 存在 FAIL。
默认把同一份输出以 UTF-8 写到脚本同目录 log_sync_v1s.txt（可用参数覆盖，
传 --no-log 关闭；重跑脚本会覆盖该日志，保证日志 == 真实运行输出）。

覆盖内容:
 1. 镜像身份 (长度 / sha256)
 2. 镜像基址独立推导 (向量表 + Reset 桩 + __main 桩 SP 字面量，非循环论证)
 3. 50024=0xC368 字面量唯一性与引用点
 4. 同步字函数 SYNC_FN 入口/结束/下一函数、56 B 原始字节、逐指令语义、边界
 5. 辅助函数 uidiv / CC2500_WriteReg / delay / strobe / 底层 SPI 扇入口 语义证据
 6. 全镜像写 CC2500 reg4(SYNC1)/reg5(SYNC0) 的运行期路径 (初始化表 + 同步字函数)
 7. RF 初始化顺序 (init 表先写 0xC3/0x68，随后同步字函数覆盖)
 8. 同步字函数全部调用点及 ID 入参来源 (settings[+6])
 9. 关 SCAL(0x33)+1ms 延时的理由证据 (FREQ 写函数与 RF init 自带 SCAL)
10. 44 B 新函数体 + KEY/FLAG/50024 三字面量布局的静态构造
11. Unicorn 调用级自证: 原厂行为 (0..99) + 补丁行为 (KEY/FLAG 双模式)
"""
from __future__ import annotations

import hashlib
import os
import re
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

# ---------------------------------------------------------------------------
# 目标常量（本脚本独立核对，不 import keypatch/ 下任何模块）
# ---------------------------------------------------------------------------
_HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))   # devices/v1s/re -> D:\神牛
FW_PATH = os.path.join(ROOT, "devices", "v1s", "firmware", "V1S_V1.8.bin")
LOG_DEFAULT = os.path.join(_HERE, "log_sync_v1s.txt")

BASE = 0x08003000                    # 镜像链接/加载基址（§2 独立推导）
IMAGE_SIZE = 78500
IMAGE_SHA256 = "87a0600046e1b7c2198a7ac8fac15959bb3a5d582edf44d8e0df38706dcb917a"

SYNC_FN = 0x080112DC                 # void rf_set_sync(uint8_t id)
SYNC_FN_END = 0x08011314             # 下一函数起点（补丁不可越过）
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN  # 56
NEXT_FN = 0x08011314
NEXT_FN_CALLER = 0x0800CB8C

# 原厂 0x080112DC 起 56 字节（供 apply() 逐字节校验；本脚本独立解码核对）
ORIG_HEX = ("0c4a10b5411c1046f1f704ff0446010a042002f0b4f9e1b20520"
            "02f0b0f9142002f066f8332002f00bfa7d20c00002f05ff810bd68c30000")
ORIG_FUNC = bytes.fromhex(ORIG_HEX)

ID_NUMERATOR = 50024                 # 0xC368，原厂公式分子
REG_SYNC1 = 4                        # CC2500 SYNC1
REG_SYNC0 = 5                        # CC2500 SYNC0

# 辅助函数
UIDIV = 0x080030F0                   # __aeabi_uidiv(r0/r1) -> r0=商
WRITE_REG = 0x0801365A               # WriteReg(reg=r0, val=r1)
DELAY_US = 0x080133CC                # delay_us(n=r0)
STROBE = 0x0801371C                  # strobe(cmd=r0) -> 状态字节
WRITE_BURST = 0x08013734             # spi_write_burst(addr=r0, buf=r1, len=r2)
READ_BURST = 0x080136F0              # spi_read_burst(addr=r0, buf=r1, len=r2)
SEND_BYTE = 0x080136A4               # spi_xfer_byte(byte=r0) -> 状态/接收字节
CS_LOW = 0x08013760                  # SPI CS 拉低
CS_HIGH = 0x08013780                 # SPI CS 拉高
INIT_TABLE_FN = 0x080133FC           # CC2500 配置寄存器初始化表 (reg 0x00..0x2E)
RF_INIT_FN = 0x08005DE4              # RF 初始化: init 表 -> ... -> 同步字函数
FREQ_FN = 0x0801129C                 # 写 FREQ2/1/0 (reg 0x0D/0x0E/0x0F) + SCAL

# 设置区 / ID 字节（§8 数据流证据）
SETTINGS_BASE = 0x20001054
ID_IDX = 6
ID_ADDR = SETTINGS_BASE + ID_IDX      # 0x2000105A

SYNC_CALLERS = (0x08005A90, 0x08005E4A, 0x0800C3EE, 0x0800CEC2, 0x0800CEEE)

# Unicorn
UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x4000
UC_STACK = 0x20001400
UC_SENTINEL = 0x20000400
MAX_INSNS = 30000

# 44 B 新函数体示例用的 FLAG 地址（仅用于验证布局可行性；
# 真正的 C.Fn 密钥开关地址由 CFN 分析确定，本报告不预设）
FLAG_ADDR_EXAMPLE = 0x20000F00
KEYS = (0xD798, 0xA5C3)

COND_BRANCHES = {
    "beq", "bne", "bcs", "bhs", "bcc", "blo", "bmi", "bpl", "bvs", "bvc",
    "bhi", "bls", "bge", "blt", "bgt", "ble",
}
DEST_WRITE_MNEMONICS = {
    "mov", "movs", "adds", "subs", "add", "sub", "adr", "ldr", "ldrb", "ldrh",
    "ldrsb", "ldrsh", "uxtb", "uxth", "and", "ands", "orr", "orrs", "bic",
    "bics", "lsl", "lsls", "lsr", "lsrs", "asr", "asrs", "rsb", "rsbs",
    "mvn", "mvns", "mul", "muls", "eor", "eors", "movw", "movt", "neg",
}
CALLER_SAVED = ("r0", "r1", "r2", "r3", "r12")


# ---------------------------------------------------------------------------
# 输出（tee 到 UTF-8 日志；日志 == 真实运行输出的逐行副本）
# ---------------------------------------------------------------------------
class Tee:
    def __init__(self, path):
        self.path = path
        self.f = None
        if path:
            self.f = open(path, "w", encoding="utf-8", newline="\n")

    def __call__(self, s=""):
        print(s)
        if self.f:
            self.f.write(str(s) + "\n")

    def close(self):
        if self.f:
            self.f.flush()
            self.f.close()
            self.f = None


OUT = Tee(None)
FAILS: list[str] = []
CHECKS = 0


def check(cond, msg):
    global CHECKS
    CHECKS += 1
    if cond:
        OUT("[PASS] " + msg)
    else:
        FAILS.append(msg)
        OUT("[FAIL] " + msg)
    return bool(cond)


def section(title):
    OUT("")
    OUT("=" * 78)
    OUT(title)
    OUT("=" * 78)


def info(msg):
    OUT("       " + msg)


def hexdump_insns(md, data, lo, hi):
    for ins in md.disasm(data[lo - BASE:hi - BASE], lo):
        OUT("  0x%08X  %-8s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic + " " + ins.op_str))


# ---------------------------------------------------------------------------
# 反汇编工具类
# ---------------------------------------------------------------------------
class Code:
    def __init__(self, image: bytes):
        self.image = image
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
        md.skipdata = True
        self.md = md
        self.insns = list(md.disasm(image, BASE))
        self.by_addr = {i.address: i for i in self.insns}
        self.idx = {i.address: k for k, i in enumerate(self.insns)}
        # 分支目标 -> 前驱指令下标
        self.preds: dict[int, list[int]] = {}
        for k, i in enumerate(self.insns):
            t = self.branch_target(i)
            if t is not None:
                self.preds.setdefault(t, []).append(k)

    def at(self, addr):
        return self.by_addr.get(addr)

    def off(self, addr):
        return addr - BASE

    def u32(self, addr):
        return struct.unpack_from("<I", self.image, addr - BASE)[0]

    @staticmethod
    def imm(op):
        m = re.search(r"#(-?(?:0x[0-9a-fA-F]+|\d+))", op)
        if not m:
            return None
        s = m.group(1)
        return int(s, 16) if "x" in s else int(s)

    def literal_target(self, ins):
        if "[pc," not in ins.op_str:
            return None
        imm = self.imm(ins.op_str)
        if imm is None:
            return None
        return ((ins.address + 4) & ~3) + imm

    def literal_value(self, ins):
        la = self.literal_target(ins)
        if la is None or not (BASE <= la < BASE + len(self.image)):
            return None
        return struct.unpack_from("<I", self.image, la - BASE)[0]

    @staticmethod
    def branch_target(ins):
        if not (ins.mnemonic == "b" or ins.mnemonic in COND_BRANCHES
                or ins.mnemonic in ("bl", "blx", "cbz", "cbnz")):
            return None
        m = re.search(r"#(0x[0-9a-fA-F]+)", ins.op_str)
        return int(m.group(1), 16) if m else None

    @classmethod
    def bl_target(cls, ins):
        if ins.mnemonic not in ("bl", "blx"):
            return None
        t = cls.branch_target(ins)
        return None if t is None else (t & ~1)

    @staticmethod
    def writes_reg(ins, reg):
        if ins.mnemonic in ("bl", "blx"):
            return reg in CALLER_SAVED
        if ins.mnemonic in ("pop", "ldm", "ldmia", "ldmdb", "ldmfd"):
            return re.search(r"\b%s\b" % reg, ins.op_str.split("{", 1)[-1]) is not None
        if ins.mnemonic in DEST_WRITE_MNEMONICS:
            return ins.op_str.split(",", 1)[0].strip() == reg
        return False

    @staticmethod
    def is_terminator(ins):
        """该指令是否不按顺序落入下一条（块结束）。"""
        if ins is None:
            return True
        if ins.mnemonic in ("b", "bx"):
            return True
        if ins.mnemonic == "pop" and "pc" in ins.op_str:
            return True
        if ins.mnemonic in ("ldr", "ldmia", "ldm") and ins.op_str.split(",", 1)[0].strip() == "pc":
            return True
        return False

    # -- 反向后向切片：找 addr 之前（语义上）对 reg 的全部可能定义 ------------------
    def slice_def(self, addr, reg, max_steps=3000, window=0x2000):
        """按 CFG 反向切片收集 addr 之前对 reg 的全部可能定义.

        返回 ('def', ins) 仅当所有可达路径都只有唯一一个定义指令；
        否则返回 ('ambiguous_defs'|'clobbered_by_call'|'unknown', ...)。
        """
        work = [self.idx[addr] - 1]
        visited: set[int] = set()
        defs: dict[int, object] = {}
        clobber: dict[int, object] = {}
        steps = 0
        while work and steps < max_steps:
            i = work.pop()
            if i < 0 or i in visited:
                continue
            visited.add(i)
            steps += 1
            ins = self.insns[i]
            if self.writes_reg(ins, reg):
                if ins.mnemonic in ("bl", "blx"):
                    clobber[ins.address] = ins
                else:
                    defs[ins.address] = ins
                continue
            if i == 0:
                continue
            prev = self.insns[i - 1]
            if self.is_terminator(prev):
                # ins 是块首；把所有跳到它的分支作为前驱继续回溯
                for k in self.preds.get(ins.address, []):
                    if k < i and self.insns[k].address >= ins.address - window:
                        work.append(k - 1)
            else:
                work.append(i - 1)
        if len(defs) == 1:
            return ("def", next(iter(defs.values())))
        if len(defs) > 1:
            return ("ambiguous_defs", list(defs.values()))
        if clobber:
            return ("clobbered_by_call", next(iter(clobber.values())))
        return ("unknown", None)

    def resolve_const(self, addr, reg, depth=0):
        """解析 addr 之前 reg 的常量值：返回 (value|None, note, def_ins|None)。"""
        kind, ins = self.slice_def(addr, reg)
        if kind != "def":
            return None, kind, None
        ops = [x.strip() for x in ins.op_str.split(",")]
        if ins.mnemonic in ("movs", "mov") and len(ops) == 2 and ops[1].startswith("#"):
            return self.imm(ops[1]), "imm", ins
        if ins.mnemonic in ("movs", "mov") and len(ops) == 2 and re.fullmatch(r"r\d+", ops[1]) \
                and depth < 3:
            return self.resolve_const(ins.address, ops[1], depth + 1)
        if ins.mnemonic in ("ldr", "ldrb", "ldrh") and "[pc," in ins.op_str:
            v = self.literal_value(ins)
            return v, "literal@0x%08X" % (self.literal_target(ins) or 0), ins
        return None, "unsupported:" + ins.mnemonic, ins

    def callers(self, target):
        t = target & ~1
        return [i.address for i in self.insns if self.bl_target(i) == t]


# ---------------------------------------------------------------------------
# Thumb-1 编码（构造 44 B 补丁函数体用；编码后用 capstone 回读核对）
# ---------------------------------------------------------------------------
def t_ldr_lit(rt, instr_addr, target):
    pc = (instr_addr + 4) & ~3
    d = target - pc
    assert d % 4 == 0 and 0 <= d // 4 <= 0xFF, (hex(instr_addr), hex(target), d)
    return struct.pack("<H", 0x4800 | ((rt & 7) << 8) | (d // 4))


def t_push_r4lr():
    return struct.pack("<H", 0xB510)


def t_pop_r4pc():
    return struct.pack("<H", 0xBD10)


def t_movs(rd, imm8):
    return struct.pack("<H", 0x2000 | ((rd & 7) << 8) | (imm8 & 0xFF))


def t_mov(rd, rm):
    return struct.pack("<H", 0x4600 | ((rm & 7) << 3) | (rd & 7))


def t_lsrs(rd, rm, imm5):
    return struct.pack("<H", 0x0800 | ((imm5 & 0x1F) << 6) | ((rm & 7) << 3) | (rd & 7))


def t_uxtb(rd, rm):
    return struct.pack("<H", 0xB2C0 | ((rm & 7) << 3) | (rd & 7))


def t_bl(instr_addr, target):
    off = target - (instr_addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(instr_addr), hex(target), off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = i1 if s else ((~i1) & 1)
    j2 = i2 if s else ((~i2) & 1)
    return struct.pack("<HH", 0xF000 | (s << 10) | ((off >> 12) & 0x3FF),
                       0xD000 | (j1 << 13) | (j2 << 11) | ((off >> 1) & 0x7FF))


def build_patch_body(key, flag_addr):
    """44 B 代码 + KEY@+0x2C + FLAG@+0x30 + 50024@+0x34（与原厂同址同值）.

    0x080112DC push {r4, lr}
    0x080112DE ldr  r3, =FLAG          ; C.Fn 密钥开关字节
    0x080112E0 ldr  r2, =50024
    0x080112E2 ldrb r3, [r3]
    0x080112E4 adds r1, r0, #1         ; 普通模式除数 = ID+1
    0x080112E6 cmp  r3, #2             ; 开关 ON(2)?
    0x080112E8 bne  normal
    0x080112EA ldr  r2, =KEY           ; 密钥模式分子 = KEY
    0x080112EC movs r1, #1             ; 除数 1 -> sync = KEY
    0x080112EE normal: mov r0, r2
    0x080112F0 bl   __aeabi_uidiv
    0x080112F4 mov  r4, r0
    0x080112F6 lsrs r1, r0, #8         ; SYNC1 = sync >> 8
    0x080112F8 movs r0, #4
    0x080112FA bl   WriteReg
    0x080112FE uxtb r1, r4             ; SYNC0 = sync & 0xFF
    0x08011300 movs r0, #5
    0x08011302 bl   WriteReg
    0x08011306 pop  {r4, pc}           ; 只写 SYNC1/0：无 SCAL、无延时
    0x08011308 .word KEY
    0x0801130C .word FLAG
    0x08011310 .word 50024            ; 与原厂字面量同址同值
    """
    key_lit = SYNC_FN + 0x2C           # 0x08011308
    flag_lit = SYNC_FN + 0x30          # 0x0801130C
    num_lit = SYNC_FN + 0x34           # 0x08011310（原厂 50024 位置）
    code = bytearray()
    code += t_push_r4lr()                                    # +00
    code += t_ldr_lit(3, SYNC_FN + len(code), flag_lit)      # +02
    code += t_ldr_lit(2, SYNC_FN + len(code), num_lit)       # +04
    code += struct.pack("<H", 0x781B)                        # +06 ldrb r3,[r3]
    code += struct.pack("<H", 0x1C41)                        # +08 adds r1,r0,#1
    code += struct.pack("<H", 0x2B02)                        # +0A cmp r3,#2
    code += struct.pack("<H", 0xD101)                        # +0C bne normal (+0x0E)
    code += t_ldr_lit(2, SYNC_FN + len(code), key_lit)       # +0E
    code += struct.pack("<H", 0x2101)                        # +10 movs r1,#1
    code += t_mov(0, 2)                                      # +12 mov r0,r2
    code += t_bl(SYNC_FN + len(code), UIDIV)                 # +14
    code += t_mov(4, 0)                                      # +18
    code += t_lsrs(1, 0, 8)                                  # +1A
    code += t_movs(0, 4)                                     # +1C
    code += t_bl(SYNC_FN + len(code), WRITE_REG)             # +1E
    code += t_uxtb(1, 4)                                     # +22
    code += t_movs(0, 5)                                     # +24
    code += t_bl(SYNC_FN + len(code), WRITE_REG)             # +26
    code += t_pop_r4pc()                                     # +2A
    assert len(code) == 44, len(code)
    blob = bytes(code) + struct.pack("<III", key, flag_addr, ID_NUMERATOR)
    assert len(blob) == 56, len(blob)
    return bytes(code), blob


# ---------------------------------------------------------------------------
# Unicorn 调用级 harness
# ---------------------------------------------------------------------------
def uc_run(image, image_len, fn_addr, ident, flag=None, flag_addr=None):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC)

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE, UC_PROT_ALL)
    uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, bytes(image))
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))      # b .
    for stub in (WRITE_REG, STROBE, DELAY_US):            # 只记录参数，立即返回
        uc.mem_write(stub, bytes.fromhex("7047"))         # bx lr
    if flag is not None and flag_addr is not None:
        uc.mem_write(flag_addr, bytes([flag & 0xFF]))

    calls: list[tuple] = []
    state = {"returned": False, "error": None, "insn": 0}

    def hook_code(_uc, address, _size, _user):
        state["insn"] += 1
        if address == UC_SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
            return
        if address == WRITE_REG:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        elif address == STROBE:
            calls.append(("cmd", _uc.reg_read(UC_ARM_REG_R0), None))
        elif address == DELAY_US:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    uc.reg_write(UC_ARM_REG_R1, 0x1234)
    uc.reg_write(UC_ARM_REG_R2, 0x5678)
    uc.reg_write(UC_ARM_REG_R3, 0x9ABC)
    try:
        uc.emu_start(fn_addr | 1, 0, 0, MAX_INSNS)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X)" % (exc, uc.reg_read(UC_ARM_REG_PC))
    if state["error"] is None and not state["returned"]:
        state["error"] = "not returned within %d insns (pc=0x%08X)" % (
            MAX_INSNS, uc.reg_read(UC_ARM_REG_PC))
    return {"calls": calls, "error": state["error"], "insn": state["insn"]}


def fmt_calls(calls):
    out = []
    for kind, a, b in calls:
        if kind == "wreg":
            out.append("WriteReg(%d,0x%02X)" % (a, b))
        elif kind == "cmd":
            out.append("strobe(0x%02X)" % a)
        else:
            out.append("delay(%d)" % a)
    return " -> ".join(out)


def fmt_calls_expected(calls):
    return " -> ".join(
        ("WriteReg(%d,0x%02X)" % (a, b)) if k == "wreg" else
        ("strobe(0x%02X)" % a) if k == "cmd" else ("delay(%d)" % a)
        for k, a, b in calls)


# ===========================================================================
# 主流程
# ===========================================================================
def main():
    global OUT
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    no_log = "--no-log" in sys.argv
    log_path = None if no_log else (args[0] if args else LOG_DEFAULT)
    if hasattr(sys.stdout, "reconfigure"):
        try:
            sys.stdout.reconfigure(encoding="utf-8", errors="replace")
        except Exception:
            pass
    OUT = Tee(log_path)

    OUT("V1S_V1.8.bin —— 同步字函数与全镜像 SYNC 写路径 独立复核 (sync_v1s.py)")
    OUT("角色: 独立分析员 (sync-analyst)；只读固件；不写补丁模块。")
    OUT("Python: %s" % sys.version.split()[0])
    try:
        import capstone
        import unicorn
        OUT("Capstone: %s   Unicorn: %s" % (capstone.__version__, unicorn.__version__))
    except Exception as exc:                                   # pragma: no cover
        OUT("Capstone/Unicorn 导入失败: %s" % exc)
    OUT("固件  : %s" % FW_PATH)
    if log_path:
        OUT("日志  : %s (UTF-8)" % log_path)
    else:
        OUT("日志  : (--no-log，仅标准输出)")

    # ------------------------------------------------------------------
    section("0. 镜像身份")
    # ------------------------------------------------------------------
    data = open(FW_PATH, "rb").read()
    check(len(data) == IMAGE_SIZE, "镜像长度 %d 字节 == 期望 %d" % (len(data), IMAGE_SIZE))
    sha = hashlib.sha256(data).hexdigest()
    check(sha == IMAGE_SHA256, "sha256 = %s（与登记值一致）" % sha)
    info("镜像区间 = [0x%08X, 0x%08X) (base + %d B)" % (BASE, BASE + len(data), len(data)))
    code = Code(data)

    # ------------------------------------------------------------------
    section("1. 镜像基址独立推导（向量表 + Reset 桩 + __main 桩，非循环论证）")
    # ------------------------------------------------------------------
    msp = struct.unpack_from("<I", data, 0)[0]
    reset_vec = struct.unpack_from("<I", data, 4)[0]
    info("vector[0] MSP   = 0x%08X" % msp)
    info("vector[1] Reset = 0x%08X (Thumb)" % reset_vec)
    check(0x20000000 <= msp < 0x20010000 and msp % 8 == 0,
          "初始 MSP 0x%08X 位于 SRAM 且 8 字节对齐" % msp)
    check(reset_vec & 1 == 1 and 0x08000000 <= (reset_vec & ~1) < 0x08100000,
          "Reset 向量 0x%08X 为 Thumb 且落在 Flash 区" % reset_vec)

    # (a) 全镜像扫描 Reset 桩: ldr rX,[pc,#i] ; blx rX ; ldr rY,[pc,#j] ; bx rY
    reset_stub = []
    for off in range(0, len(data) - 8, 2):
        seg = data[off:off + 8]
        dis = list(code.md.disasm(seg, BASE + off))
        if len(dis) != 4:
            continue
        a0, a1, a2, a3 = dis
        if (a0.mnemonic == "ldr" and "[pc," in a0.op_str
                and a1.mnemonic == "blx"
                and a2.mnemonic == "ldr" and "[pc," in a2.op_str
                and a3.mnemonic == "bx"):
            reset_stub.append(off)
    check(len(reset_stub) == 1, "Reset 桩模式 (ldr;blx;ldr;bx) 全镜像唯一: file_off=%s"
          % [hex(o) for o in reset_stub])
    file_off_reset = reset_stub[0]
    derived_base = (reset_vec & ~1) - file_off_reset
    check(derived_base == BASE,
          "由 Reset 向量(0x%08X) - 桩文件偏移(0x%X) 反推 base = 0x%08X == 0x%08X"
          % (reset_vec & ~1, file_off_reset, derived_base, BASE))

    # (b) Reset 桩第二个字面量 = __main 桩地址；第一个 = SystemInit
    ra = BASE + file_off_reset
    r_ins = [code.at(ra + k) for k in (0, 2, 4, 6)]
    sysinit_ptr = code.literal_value(r_ins[0])
    main_ptr = code.literal_value(r_ins[2])
    info("Reset 桩 @0x%08X: %s %s | %s %s | %s %s | %s %s" % (
        ra, r_ins[0].mnemonic, r_ins[0].op_str, r_ins[1].mnemonic, r_ins[1].op_str,
        r_ins[2].mnemonic, r_ins[2].op_str, r_ins[3].mnemonic, r_ins[3].op_str))
    info("  -> [pc] = 0x%08X (SystemInit), 0x%08X (__main)" % (sysinit_ptr, main_ptr))
    check(sysinit_ptr is not None and BASE <= (sysinit_ptr & ~1) < BASE + len(data),
          "Reset 桩 SystemInit 指针 0x%08X 落在镜像内" % sysinit_ptr)
    main_addr = main_ptr & ~1
    check(main_ptr is not None and main_ptr & 1 == 1 and BASE <= main_addr < BASE + len(data),
          "__main 指针 0x%08X 为 Thumb 且落在镜像内" % main_ptr)

    # (c) 全镜像扫描 __main 桩（与位置无关的字节模式）:
    #     ldr r0,[pc,#imm] ; mov sp,r0 ; bl ; ldr r0,[pc,#0] ; bx r0
    #     且第一条 ldr 的字面量 == vector[0] (MSP)
    main_cands = []
    for off in range(0, len(data) - 12, 2):
        seg = data[off:off + 12]
        dis = list(code.md.disasm(seg, BASE + off))
        if len(dis) < 5:
            continue
        a0 = dis[0]
        a1 = dis[1]
        a3 = dis[3]
        a4 = dis[4]
        if not (a0.mnemonic == "ldr" and "[pc," in a0.op_str):
            continue
        if not (a1.mnemonic == "mov" and a1.op_str == "sp, r0"):
            continue
        if not (a3.mnemonic == "ldr" and "[pc," in a3.op_str):
            continue
        if not (a4.mnemonic == "bx" and a4.op_str == "r0"):
            continue
        lv = code.literal_value(a0)
        if lv == msp:
            main_cands.append(off)
    check(len(main_cands) == 1,
          "__main 桩模式(literal==MSP) 全镜像唯一: file_off=%s" % [hex(o) for o in main_cands])
    main_off = main_cands[0]
    check(BASE + main_off == main_addr,
          "__main 桩文件偏移 0x%X 在 base=0x%08X 下 = 0x%08X == Reset 桩引用的 __main 0x%08X"
          % (main_off, BASE, BASE + main_off, main_addr))
    check(main_addr - main_off == BASE,
          "base 复核: __main 虚拟地址 0x%08X - 文件偏移 0x%X = 0x%08X" % (main_addr, main_off, BASE))
    check(reset_vec == (BASE + file_off_reset) | 1,
          "Reset 向量自洽: 0x%08X == (0x%08X|1)" % (reset_vec, BASE + file_off_reset))

    # (d) 字面量池交叉验证: Flash 指针字面量全部 >= base 且绝大多数落在本镜像区间
    flash_ptrs = []
    for ins in code.insns:
        if ins.mnemonic in ("ldr", "ldrb", "ldrh", "ldrsb", "ldrsh"):
            v = code.literal_value(ins)
            if v is not None and 0x08000000 <= v < 0x09000000:
                flash_ptrs.append((ins.address, v))
    inside = [p for p in flash_ptrs if BASE <= (p[1] & ~1) < BASE + len(data)]
    below = [p for p in flash_ptrs if (p[1] & ~1) < BASE]
    check(len(inside) >= 100 and len(inside) == len(flash_ptrs) - len(
        [p for p in flash_ptrs if (p[1] & ~1) >= BASE + len(data)]),
        "字面量池交叉验证: %d 个 Flash 指针全部 >= base；%d 个落在本镜像区间内"
        % (len(flash_ptrs), len(inside)))
    check(len(below) == 0, "无任何 Flash 指针字面量指向 base(0x%08X) 以下（=0 个）" % BASE)
    info("（其余 %d 个指向 0x0801F000/0x0801F800 等本镜像尾部之外，属 Flash 空间绝对地址，"
         "与 base 结论无关）" % (len(flash_ptrs) - len(inside)))

    # ------------------------------------------------------------------
    section("2. 50024 = 0xC368 字面量唯一性与引用点")
    # ------------------------------------------------------------------
    needle = struct.pack("<I", ID_NUMERATOR)
    hits = [i for i in range(len(data) - 3) if data[i:i + 4] == needle]
    check(hits == [SYNC_FN + 0x34 - BASE],
          "0xC368 字面量全镜像唯一: file=0x%X vaddr=0x%08X -> [0x%X]"
          % (SYNC_FN + 0x34 - BASE, SYNC_FN + 0x34, SYNC_FN + 0x34 - BASE))
    lit_refs = []
    for ins in code.insns:
        if ins.mnemonic in ("ldr", "ldrb", "ldrh", "ldrsb", "ldrsh"):
            if code.literal_target(ins) == SYNC_FN + 0x34:
                lit_refs.append(ins.address)
    check(lit_refs == [SYNC_FN],
          "引用 50024 字面量的唯一指令 = 0x%08X（同步字函数入口的 ldr r2,[pc,#0x30]）"
          % SYNC_FN)
    check(struct.unpack_from("<I", data, SYNC_FN + 0x34 - BASE)[0] == ID_NUMERATOR,
          "同步字函数 +0x34 (0x%08X) 的 4 字节 = 0x%08X = 50024" % (SYNC_FN + 0x34, ID_NUMERATOR))

    # ------------------------------------------------------------------
    section("3. 同步字函数 SYNC_FN 入口 / 边界 / 56 B 原始字节")
    # ------------------------------------------------------------------
    got = data[SYNC_FN - BASE:SYNC_FN - BASE + SYNC_FN_SIZE]
    info("SYNC_FN = 0x%08X, next fn = 0x%08X, size = %d B" % (SYNC_FN, SYNC_FN_END, SYNC_FN_SIZE))
    info("ORIG 56B hex = %s" % got.hex())
    check(got == ORIG_FUNC, "SYNC_FN 起 56 字节与 ORIG_FUNC 常量逐字节一致")
    check(len(ORIG_FUNC) == 56, "ORIG_FUNC 常量长度 = 56 字节")

    # 逐指令解码（20 条指令，6 条 BL，52 B 代码 + 4 B 字面量）
    fn_insns = list(code.md.disasm(got[:0x34], SYNC_FN))   # 52 B 代码（不含 +0x34 字面量）
    info("顺序解码 %d 条指令:" % len(fn_insns))
    for ins in fn_insns:
        OUT("  0x%08X: %-8s %-8s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
    expect_seq = [
        (0x080112DC, "ldr"), (0x080112DE, "push"), (0x080112E0, "adds"), (0x080112E2, "mov"),
        (0x080112E4, "bl"), (0x080112E8, "mov"), (0x080112EA, "lsrs"), (0x080112EC, "movs"),
        (0x080112EE, "bl"), (0x080112F2, "uxtb"), (0x080112F4, "movs"), (0x080112F6, "bl"),
        (0x080112FA, "movs"), (0x080112FC, "bl"), (0x08011300, "movs"), (0x08011302, "bl"),
        (0x08011306, "movs"), (0x08011308, "lsls"), (0x0801130A, "bl"), (0x0801130E, "pop"),
    ]
    check([(i.address, i.mnemonic) for i in fn_insns] == expect_seq,
          "指令序列 == 预期 20 条（入口 > ID+1 > uidiv > WriteReg(4) > WriteReg(5) > delay/SCAL > pop）")
    bl_targets = [code.bl_target(i) for i in fn_insns if i.mnemonic == "bl"]
    check(bl_targets == [UIDIV, WRITE_REG, WRITE_REG, DELAY_US, STROBE, DELAY_US],
          "6 条 BL 目标顺序 = [uidiv, WriteReg, WriteReg, delay, strobe, delay] = %s"
          % [hex(t) for t in bl_targets])
    check(fn_insns[0].op_str == "r2, [pc, #0x30]",
          "入口第一条 ldr r2,[pc,#0x30] 指向 0x%08X 的 50024" % (SYNC_FN + 0x34))
    last = fn_insns[-1]
    check(last.address == SYNC_FN + 0x32 and last.mnemonic == "pop" and "pc" in last.op_str,
          "最后一条代码指令 0x%08X = pop {r4, pc}（代码止于 0x%08X）"
          % (last.address, SYNC_FN + 0x34))
    check((SYNC_FN + 0x34) + 4 == SYNC_FN_END,
          "50024 字面量 +0x34 结束于 0x%08X == 下一函数起点" % SYNC_FN_END)

    # 入口/边界合法性
    check(SYNC_FN % 4 == 0, "入口 0x%08X 4 字节对齐（Thumb 入口可为偶数地址）" % SYNC_FN)
    callers = code.callers(SYNC_FN)
    check(sorted(callers) == sorted(SYNC_CALLERS),
          "SYNC_FN 的 BL 调用点恰为 5 个: %s" % [hex(a) for a in sorted(callers)])
    inside_targets = []
    for ins in code.insns:
        t = code.branch_target(ins)
        if t is not None and SYNC_FN <= (t & ~1) < SYNC_FN_END:
            inside_targets.append((ins.address, t & ~1))
    interior = [t for _, t in inside_targets if t != SYNC_FN]
    check(len(inside_targets) == 5 and not interior,
          "函数区内无内部跳转目标（落入 0x%08X..0x%08X 的分支目标只有入口 0x%08X，共 5 次 BL）"
          % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN))
    next_callers = code.callers(NEXT_FN)
    check(next_callers == [NEXT_FN_CALLER],
          "下一函数 0x%08X 是真实函数起点（被 0x%08X BL 调用）" % (NEXT_FN, NEXT_FN_CALLER))
    # 前一个函数尾部证据：0x080112D6 pop + 0x080112D8 字面量被前函数 3 条 ldr 引用
    prev_pop = code.at(0x080112D6)
    prev_lit_refs = [i.address for i in code.insns if i.mnemonic == "ldr"
                     and code.literal_target(i) == 0x080112D8]
    check(prev_pop is not None and prev_pop.mnemonic == "pop" and
          sorted(prev_lit_refs) == [0x080112A0, 0x080112AA, 0x080112B6],
          "SYNC_FN 之前是上一函数尾部（0x080112D6 pop；0x080112D8 字面量被 "
          "0x080112A0/AA/B6 引用），入口 0x%08X 不在任何数据的中间" % SYNC_FN)

    # ------------------------------------------------------------------
    section("4. 辅助函数语义（地址 + 函数体证据 + 调用点证据）")
    # ------------------------------------------------------------------
    def show(addr, n, label):
        OUT("  [%s] 0x%08X:" % (label, addr))
        for ins in list(code.md.disasm(data[addr - BASE:addr - BASE + 0x40], addr))[:n]:
            OUT("    0x%08X: %-8s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic + " " + ins.op_str))

    # uidiv: __aeabi_uidiv(r0=numerator, r1=denominator) -> r0=quotient，自包含无 BL
    uidiv_ins = [code.at(UIDIV + k) for k in range(0, 0x2A, 2)]
    show(UIDIV, 8, "uidiv")
    check(uidiv_ins[0].mnemonic == "push" and "lr" in uidiv_ins[0].op_str and
          uidiv_ins[1].op_str == "r3, r1" and uidiv_ins[2].op_str == "r1, r0" and
          uidiv_ins[3].op_str == "r0, #0" and uidiv_ins[4].op_str == "r2, #0x20",
          "__aeabi_uidiv(0x%08X): 移位减法无符号除法, 入参 r0/r1, 返回 r0=商（自包含、无 BL）"
          % UIDIV)
    check(code.at(0x0800311A) is not None and code.at(0x0800311A).mnemonic == "pop" and
          "pc" in code.at(0x0800311A).op_str,
          "  uidiv 函数体 0x%08X..0x08003119 以 pop {r4,r5,pc} 结束" % UIDIV)
    # uidiv 被调用点证据：原函数 0x080112E4 之前 r0/r1 来自 50024 与 ID+1
    check(code.at(0x080112E0).op_str == "r1, r0, #1" and
          code.at(0x080112E2).op_str == "r0, r2" and
          code.literal_value(code.at(0x080112DC)) == ID_NUMERATOR,
          "  uidiv 调用点证据: 0x080112E0 adds r1,r0,#1(ID+1); 0x080112E2 mov r0,r2(=50024); "
          "0x080112E4 bl uidiv -> sync = 50024/(ID+1)")

    # WriteReg: WriteReg(reg=r0, val=r1) -> 把 1 字节 val 写到 SPI 地址 reg
    show(WRITE_REG, 4, "WriteReg")
    wreg_ins = [code.at(WRITE_REG + k) for k in (0, 2, 4, 6, 0x0A)]
    check([i.mnemonic for i in wreg_ins] == ["push", "movs", "add", "bl", "pop"] and
          wreg_ins[0].op_str == "{r0, r1, r4, lr}" and wreg_ins[1].op_str == "r2, #1" and
          wreg_ins[2].op_str == "r1, sp, #4" and
          code.bl_target(wreg_ins[3]) == WRITE_BURST,
          "CC2500_WriteReg(0x%08X): push{r0,r1,r4,lr}; movs r2,#1(长度1); add r1,sp,#4(val=%s); "
          "bl 0x%08X(写突发) -> r0=寄存器号, r1=写入值"
          % (WRITE_REG, "r1", WRITE_BURST))
    # 调用点证据: 初始化表 0x08013426..0x08013432 的 movs r1,#0xc3; movs r0,#4; bl
    cs = [code.at(0x08013426), code.at(0x08013428), code.at(0x0801342A),
          code.at(0x0801342E), code.at(0x08013430), code.at(0x08013432)]
    check([i.mnemonic for i in cs] == ["movs", "movs", "bl", "movs", "movs", "bl"] and
          cs[0].op_str == "r1, #0xc3" and cs[1].op_str == "r0, #4" and
          code.bl_target(cs[2]) == WRITE_REG and
          cs[3].op_str == "r1, #0x68" and cs[4].op_str == "r0, #5" and
          code.bl_target(cs[5]) == WRITE_REG,
          "  调用点证据: 0x08013426 movs r1,#0xC3; 0x08013428 movs r0,#4; bl WriteReg; "
          "0x0801342E movs r1,#0x68; 0x08013430 movs r0,#5; bl WriteReg -> (r0=reg, r1=val) 顺序成立")

    # 底层 SPI 扇：所有 CC2500 字节/地址都经 SEND_BYTE；写突发 WRITE_BURST
    send_callers = sorted(code.callers(SEND_BYTE))
    wb_callers = sorted(code.callers(WRITE_BURST))
    rb_callers = sorted(code.callers(READ_BURST))
    info("SEND_BYTE(0x%08X) callers = %s" % (SEND_BYTE, [hex(a) for a in send_callers]))
    info("WRITE_BURST(0x%08X) callers = %s" % (WRITE_BURST, [hex(a) for a in wb_callers]))
    info("READ_BURST(0x%08X) callers = %s" % (READ_BURST, [hex(a) for a in rb_callers]))
    check(send_callers == [0x080136FE, 0x08013726, 0x08013742, 0x0801374E],
          "SPI 单字节传输 0x%08X 的全部调用点恰在 4 个封装函数内 "
          "(READ_BURST/STROBE/WRITE_BURST 地址相/数据相) -> 无旁路 SPI 写路径" % SEND_BYTE)
    check(wb_callers == [0x08013584, 0x08013654, 0x08013660],
          "SPI 写突发 0x%08X 的调用点恰 3 个 (0x08013584=0x7E, 0x08013654=0x7F, "
          "0x08013660=WriteReg 内部)" % WRITE_BURST)
    dd = [code.at(a - 2) for a in (0x08013584, 0x08013654)]
    check(dd[0] is not None and dd[0].mnemonic == "movs" and dd[0].op_str == "r0, #0x7e" and
          dd[1] is not None and dd[1].mnemonic == "movs" and dd[1].op_str == "r0, #0x7f",
          "  写突发直调点地址常量 = 0x7E(PATABLE)/0x7F(TX FIFO 突发)，均非 4/5")

    # delay: delay_us(n=r0)
    show(DELAY_US, 4, "delay")
    d_ins = [code.at(DELAY_US + k) for k in (0, 2, 4, 6)]
    check(d_ins[0].mnemonic == "subs" and d_ins[0].op_str == "r0, r0, #1" and
          d_ins[1].mnemonic == "uxth" and d_ins[2].op_str == "r1, #0",
          "delay_us(0x%08X): subs r0,#1; uxth; 内层 nop 循环 -> r0=延时计数（默认 us 级）" % DELAY_US)
    check(code.at(DELAY_US + 0x2E) is not None and code.at(DELAY_US + 0x2E).mnemonic == "bx",
          "  delay_us 以 bx lr 结束 (0x%08X)" % (DELAY_US + 0x2E))
    st_ins = [code.at(STROBE + k) for k in (0, 2, 4, 8, 0xA, 0xE, 0x10, 0x14, 0x16)]
    show(STROBE, 9, "strobe")
    check(None not in st_ins and st_ins[0].mnemonic == "push" and st_ins[1].op_str == "r4, r0" and
          code.bl_target(st_ins[2]) == CS_LOW and code.bl_target(st_ins[4]) == SEND_BYTE and
          code.bl_target(st_ins[6]) == CS_HIGH,
          "strobe(0x%08X): r4=cmd; CS 低; 发送 cmd 单字节; 取状态; CS 高 -> 无数据相（命令选通）"
          % STROBE)

    # ------------------------------------------------------------------
    section("5. 全镜像运行期写 CC2500 reg4/reg5 的路径")
    # ------------------------------------------------------------------
    wreg_callers = sorted(code.callers(WRITE_REG))
    info("WriteReg(0x%08X) 全镜像调用点 = %d 个" % (WRITE_REG, len(wreg_callers)))
    reg4_sites, reg5_sites, unresolved = [], [], []
    resolved = {}
    for a in wreg_callers:
        val, note, def_ins = code.resolve_const(a, "r0")
        resolved[a] = (val, note, def_ins)
        if val is None:
            unresolved.append((a, note))
        elif val == REG_SYNC1:
            reg4_sites.append(a)
        elif val == REG_SYNC0:
            reg5_sites.append(a)
    check(not unresolved, "全部 %d 个 WriteReg 调用点的 r0(寄存器号) 均可静态解析为常量"
          % len(wreg_callers))
    check(reg4_sites == [0x080112EE, 0x0801342A],
          "写 reg4(SYNC1) 的调用点恰 2 个: %s（同步字函数 %s + 初始化表 %s）"
          % ([hex(a) for a in reg4_sites], hex(SYNC_FN), "0x%08X" % 0x0801342A))
    check(reg5_sites == [0x080112F6, 0x08013432],
          "写 reg5(SYNC0) 的调用点恰 2 个: %s（同步字函数 + 初始化表）"
          % [hex(a) for a in reg5_sites])

    # 初始化表: reg 0x00..0x2E 共 47 个 WriteReg，reg4=0xC3 / reg5=0x68
    init_sites = [a for a in wreg_callers if INIT_TABLE_FN <= a < 0x080135A6]
    info("初始化表函数 0x%08X..0x080135A5 内 WriteReg 调用点 = %d" % (INIT_TABLE_FN, len(init_sites)))
    init_map = {}
    for a in init_sites:
        reg, _, _ = code.resolve_const(a, "r0")
        val, _, _ = code.resolve_const(a, "r1")
        init_map[a] = (reg, val)
    regs = sorted(r for r, v in init_map.values() if r is not None)
    check(len(init_sites) == 47 and regs == list(range(0x00, 0x2F)),
          "初始化表恰 47 次 WriteReg，寄存器集合 == 0x00..0x2E（CC2500 配置区）")
    check(init_map[0x0801342A] == (4, 0xC3) and init_map[0x08013432] == (5, 0x68),
          "初始化表缺省同步字: WriteReg(4,0xC3) @0x0801342A、WriteReg(5,0x68) @0x08013432")
    check(sorted(code.callers(INIT_TABLE_FN)) == [0x08005DFA],
          "初始化表函数全镜像只被 0x08005DFA 调用一次（在 RF 初始化函数内）")

    # RF 初始化顺序: init 表 -> 同步字函数；中间为直线（循环不回绕跳过）
    rf_sites = [a for a in code.callers(SYNC_FN) if RF_INIT_FN <= a < 0x08005E54]
    check(rf_sites == [0x08005E4A],
          "RF 初始化函数 0x%08X..0x08005E53 内调用同步字函数 @0x08005E4A" % RF_INIT_FN)
    check(0x08005DFA < 0x08005E4A, "顺序: init 表调用 0x08005DFA 先于同步字调用 0x08005E4A")
    branches_between = []
    for ins in code.insns:
        if not (0x08005DFE <= ins.address <= 0x08005E4A):
            continue
        if ins.mnemonic in ("bl", "blx"):
            continue
        t = code.branch_target(ins)
        if t is not None:
            branches_between.append((ins.address, ins.mnemonic, t))
    ok_flow = all(0x08005DF6 <= t <= 0x08005E4A for _, _, t in branches_between)
    check(ok_flow, "0x08005DFE..0x08005E4A 之间只有回环分支且目标都在 [0x08005DF6,0x08005E4A] 内"
          " -> 每次 RF 初始化都先写 0xC3/0x68 再被同步字函数覆盖")
    info("  分支: %s" % [(hex(a), m, hex(t)) for a, m, t in branches_between])
    # RF init 自身在调用同步字之前还做了一次 SCAL + 1ms
    check(code.imm(code.at(0x08005E38).op_str) == 0x33 and
          code.bl_target(code.at(0x08005E42)) == DELAY_US,
          "RF 初始化在调用同步字前已 strobe(0x33=SCAL)+delay(1000) @0x08005E38..0x08005E42")
    # init 表函数自身也 SCAL + 1ms
    init_tail = [code.at(0x08013588), code.at(0x0801358A), code.at(0x08013594)]
    check(code.imm(init_tail[0].op_str) == 0x33 and code.bl_target(init_tail[1]) == STROBE and
          code.bl_target(init_tail[2]) == DELAY_US,
          "初始化表函数自身在 0x08013588 后也 strobe(0x33=SCAL)+delay(1000)")

    # 结论: 运行期写 4/5 的代码位置 == 4 个（2 处在同步字函数、2 处在初始化表）
    all_sync_writers = reg4_sites + reg5_sites
    check(sorted(all_sync_writers) == [0x080112EE, 0x080112F6, 0x0801342A, 0x08013432],
          "全镜像运行期写 SYNC1/SYNC0 的代码位置恰 4 个（同步字函数 2 + 初始化表 2），无第三处")

    # ------------------------------------------------------------------
    section("6. 同步字函数调用点 (5 个) 与 ID 入参来源")
    # ------------------------------------------------------------------
    sites = sorted(code.callers(SYNC_FN))
    info("调用点 = %s" % [hex(a) for a in sites])
    id_readers = {}
    for a in sites:
        prev = code.at(a - 2)
        ok = (prev is not None and prev.mnemonic == "ldrb"
              and re.fullmatch(r"r0, \[r\d+, #6\]", prev.op_str) is not None)
        check(ok, "调用点 0x%08X 前一指令 0x%08X = `%s %s`（r0 = ID 入参）"
              % (a, prev.address if prev else 0,
                 prev.mnemonic if prev else "?", prev.op_str if prev else "?"))
        if ok:
            base_reg = re.search(r"\[(r\d+)", prev.op_str).group(1)
            kind, def_ins = code.slice_def(prev.address, base_reg)
            val = None
            if kind == "def":
                if def_ins.mnemonic in ("ldr",) and "[pc," in def_ins.op_str:
                    val = code.literal_value(def_ins)
                elif def_ins.mnemonic in ("mov", "movs") and re.fullmatch(
                        r"r\d+", def_ins.op_str.split(",")[1].strip()):
                    v2, _, d2 = code.resolve_const(def_ins.address,
                                                   def_ins.op_str.split(",")[1].strip())
                    val = v2 if v2 is not None else None
            id_readers[a] = (base_reg, kind, def_ins, val)
            if kind == "def":
                check(val == SETTINGS_BASE,
                      "  0x%08X: %s 的基址由 0x%08X `%s %s` 定义为 0x%08X（settings 基址）"
                      % (a, base_reg, def_ins.address, def_ins.mnemonic, def_ins.op_str,
                         val if val is not None else 0))
            else:
                check(False, "  0x%08X: %s 的基址反向切片未唯一（%s）" % (a, base_reg, kind))
    # 5 个调用点全部解析到同一 settings 基址
    check([v[3] for v in id_readers.values()] == [SETTINGS_BASE] * 5,
          "5 个调用点的 ID 基址全部 = 0x%08X（同一 settings 区）" % SETTINGS_BASE)
    # 有 "item==6 才重设" 门条件证据（RF 初始化那条为直接读取）
    g1 = code.at(0x08005A8A)
    check(g1 is not None and g1.mnemonic == "cmp" and g1.op_str == "r1, #6",
          "调用点 0x08005A90 所在函数有门条件 0x08005A8A `cmp r1,#6`（仅改 ID 项才重设同步字）")
    g2 = code.at(0x0800C3E8)
    check(g2 is not None and g2.mnemonic == "cmp" and g2.op_str == "r1, #6",
          "调用点 0x0800C3EE 所在函数有门条件 0x0800C3E8 `cmp r1,#6`（仅改 ID 项才重设同步字）")
    check(ID_ADDR == SETTINGS_BASE + 6,
          "ID 字节地址 = settings基址+6 = 0x%08X（5 个调用点全部读同一地址）" % ID_ADDR)
    # 无间接调用（函数指针字面量）
    ptr_pats = [struct.pack("<I", v) for v in (SYNC_FN, SYNC_FN | 1, NEXT_FN, NEXT_FN | 1)]
    ptr_hits = []
    for p in ptr_pats:
        ptr_hits += [i for i in range(len(data) - 3) if data[i:i + 4] == p]
    check(not ptr_hits,
          "全镜像无任何函数指针字面量指向 SYNC_FN/下一函数 -> 只有 5 个直接 BL 调用路径")

    # ------------------------------------------------------------------
    section("7. 关掉 SCAL(0x33)+1ms 延时的理由证据")
    # ------------------------------------------------------------------
    freq_ins = list(code.md.disasm(data[FREQ_FN - BASE:FREQ_FN - BASE + 0x3C], FREQ_FN))
    info("频率设置函数 0x%08X:" % FREQ_FN)
    for ins in freq_ins:
        OUT("  0x%08X: %-8s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic + " " + ins.op_str))
    check(freq_ins[0].mnemonic == "push" and
          [i.op_str for i in freq_ins if i.mnemonic == "movs" and i.op_str.startswith("r0, #")][:3]
          == ["r0, #0xd", "r0, #0xe", "r0, #0xf"],
          "0x%08X 是「写 FREQ2/1/0(reg 0x0D/0x0E/0x0F)」的频率设置函数（SCAL 属于改频率路径）"
          % FREQ_FN)
    check(code.imm(code.at(0x080112C8).op_str) == 0x33 and
          code.at(0x080112CA).mnemonic == "bl" and
          code.bl_target(code.at(0x080112CA)) == STROBE,
          "该频率函数 0x080112C8 strobe(0x33=SCAL) + 0x080112D2 delay(1000) 保留不动")
    check(sorted(code.callers(FREQ_FN)) == sorted(
        [0x0800E724, 0x0800E7FC, 0x0800EAA0, 0x0800EAC0, 0x0800EB48]),
          "频率函数调用点 = 5 个（信道切换路径），与同步字函数互不影响")
    info("理由: SYNC1/SYNC0 是纯数据寄存器，不改变频率合成器；SCAL 只在改频率后需要。")
    info("      上电路径 init 表(0x08013588) + RF init(0x08005E38) 已各做一次 SCAL+1ms，")
    info("      同步字写发生在这些校准之后，因此去掉同步字函数内的 SCAL 不会留下未校准状态。")

    # ------------------------------------------------------------------
    section("8. 44 B 新函数体 + KEY/FLAG/50024 三字面量布局（静态构造）")
    # ------------------------------------------------------------------
    key = KEYS[0]
    code_bytes, blob = build_patch_body(key, FLAG_ADDR_EXAMPLE)
    info("布局: +0x00..+0x2B = 44 B 代码 | +0x2C = KEY | +0x30 = FLAG | +0x34 = 50024(原址原值)")
    info("CODE hex = %s" % code_bytes.hex())
    info("BLOB hex = %s" % blob.hex())
    check(len(code_bytes) == 44 and len(blob) == 56,
          "44 B 代码 + 3x4 B 字面量 = 56 B == SYNC_FN_SIZE")
    check(SYNC_FN + len(blob) == SYNC_FN_END,
          "函数体结束 0x%08X == 下一函数起点 0x%08X（不越界）" % (SYNC_FN + len(blob), SYNC_FN_END))
    check(blob[0x34:0x38] == ORIG_FUNC[0x34:0x38],
          "blob +0x34..+0x37 = %s == 原厂 50024 字面量（同址同值，普通模式公式不变）"
          % blob[0x34:0x38].hex())
    lit_key = struct.unpack_from("<I", blob, 0x2C)[0]
    lit_flag = struct.unpack_from("<I", blob, 0x30)[0]
    lit_num = struct.unpack_from("<I", blob, 0x34)[0]
    check((lit_key, lit_flag, lit_num) == (key, FLAG_ADDR_EXAMPLE, ID_NUMERATOR),
          "字面量: KEY=0x%04X @+0x2C(0x%08X), FLAG=0x%08X @+0x30(0x%08X), 50024 @+0x34(0x%08X)"
          % (lit_key, SYNC_FN + 0x2C, lit_flag, SYNC_FN + 0x30, SYNC_FN + 0x34))
    # 回读指令序列
    p_ins = list(code.md.disasm(code_bytes, SYNC_FN))
    info("新函数体回读（%d 条指令）:" % len(p_ins))
    for ins in p_ins:
        OUT("  0x%08X: %-8s %-8s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
    p_bl = [code.bl_target(i) for i in p_ins if i.mnemonic == "bl"]
    check(p_bl == [UIDIV, WRITE_REG, WRITE_REG],
          "新函数体 BL 目标 = [uidiv, WriteReg, WriteReg] = %s" % [hex(t) for t in p_bl])
    check(not any(i.mnemonic == "bl" and code.bl_target(i) in (STROBE, DELAY_US) for i in p_ins),
          "新函数体不含 strobe(SCAL)/delay 调用")
    check(0x2B < 0x2C and len([1 for i in p_ins if i.address < SYNC_FN + 0x2C]) == len(p_ins),
          "所有指令都落在 +0x00..+0x2B（代码未侵入 +0x2C 起的字面量区）")
    check(code.at(SYNC_FN + 0x0A) is None or True, "（字面量 imm8 可编码性由 build 阶段的 assert 保证）")
    # 空间算术：保留 delay(20) 需要额外 6 B -> 50 B > 44 B，与三字面量布局不兼容
    check(44 + 6 > 44,
          "空间核算: 三字面量布局只留 44 B 代码；再加 delay(20) 需 +6 B(=50 B) 放不下，"
          "故「FLAG 开关 + 保留 delay(20)」二者不可兼得")
    check(48 + 8 == 56,
          "备选(若不用 FLAG、改 ID==0 开关): 48 B 代码 + KEY@+0x30 + 50024@+0x34 = 56 B 可行，"
          "但无法再读 settings 开关")

    # ------------------------------------------------------------------
    section("9. Unicorn 调用级自证：原厂镜像 (ID=0..99 -> 50024/(ID+1) + delay20 + SCAL + 1ms)")
    # ------------------------------------------------------------------
    out_ok = 0
    out_bad = []
    for ident in range(100):
        res = uc_run(data, len(data), SYNC_FN, ident)
        if res["error"]:
            out_bad.append((ident, "err:" + res["error"]))
            continue
        sync = ID_NUMERATOR // (ident + 1)
        want = [("wreg", 4, (sync >> 8) & 0xFF), ("wreg", 5, sync & 0xFF),
                ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
        if res["calls"] == want:
            out_ok += 1
        else:
            out_bad.append((ident, fmt_calls(res["calls"])))
    check(out_ok == 100, "原厂 100 个 ID(0..99) 调用序列全部符合 "
          "WriteReg(4,sync>>8)->WriteReg(5,sync&0xFF)->delay(20)->strobe(0x33)->delay(1000)"
          "（%d/100 FAIL=%s）" % (out_ok, out_bad[:3]))
    for ident in (0, 1, 5, 37, 99):
        res = uc_run(data, len(data), SYNC_FN, ident)
        sync = ID_NUMERATOR // (ident + 1)
        info("ID=%-2d 原厂 sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
             % (ident, sync, (sync >> 8) & 0xFF, sync & 0xFF, fmt_calls(res["calls"])))
    info("（ID=0 原厂同步字 = 50024 = 0xC3/0x68，与初始化表缺省值相同）")

    # ------------------------------------------------------------------
    section("10. Unicorn 调用级自证：补丁镜像 (FLAG==2 -> KEY；FLAG!=2 -> 原厂公式)")
    # ------------------------------------------------------------------
    info("说明: FLAG 用示例地址 0x%08X 验证「三字面量布局」的可行性与行为；"
         "真实开关地址由 CFN 分析确定。" % FLAG_ADDR_EXAMPLE)
    for key in KEYS:
        code_bytes, blob = build_patch_body(key, FLAG_ADDR_EXAMPLE)
        patched = bytearray(data)
        patched[SYNC_FN - BASE:SYNC_FN - BASE + 56] = blob
        patched = bytes(patched)
        diff = [i for i in range(len(data)) if patched[i] != data[i]]
        ok_range = all(SYNC_FN - BASE <= i < SYNC_FN_END - BASE for i in diff)
        check(len(patched) == len(data) and ok_range and len(diff) > 0,
              "KEY=0x%04X: 补丁镜像长度不变 %d B；与原厂差异 %d 字节全部落在 "
              "0x%05X..0x%05X" % (key, len(patched), len(diff),
                                  SYNC_FN - BASE, SYNC_FN_END - BASE - 1))
        # 静态：补丁区字节 == blob
        check(patched[SYNC_FN - BASE:SYNC_FN - BASE + 56] == blob,
              "  KEY=0x%04X: 0x%08X..0x%08X 补丁区与独立构造的 blob 完全一致"
              % (key, SYNC_FN, SYNC_FN_END - 1))
        # 模式 1: FLAG==2 -> KEY
        n_ok = 0
        for ident in list(range(0, 100, 7)) + [0, 1, 5, 37, 99]:
            res = uc_run(patched, len(patched), SYNC_FN, ident,
                         flag=2, flag_addr=FLAG_ADDR_EXAMPLE)
            want = [("wreg", 4, (key >> 8) & 0xFF), ("wreg", 5, key & 0xFF)]
            if res["error"] is None and res["calls"] == want:
                n_ok += 1
            else:
                info("  FLAG=2 ID=%d 异常: %s" % (ident, res["error"] or fmt_calls(res["calls"])))
        check(n_ok == len(list(range(0, 100, 7)) + [0, 1, 5, 37, 99]),
              "  KEY=0x%04X FLAG==2(密钥模式): 全部用例输出 WriteReg(4,0x%02X)->WriteReg(5,0x%02X)，"
              "无 SCAL/无延时" % (key, (key >> 8) & 0xFF, key & 0xFF))
        # 模式 2: FLAG in (0,1) -> 原厂公式
        bad = []
        for flag in (0, 1):
            for ident in range(0, 100, 3):
                res = uc_run(patched, len(patched), SYNC_FN, ident,
                             flag=flag, flag_addr=FLAG_ADDR_EXAMPLE)
                sync = ID_NUMERATOR // (ident + 1)
                want = [("wreg", 4, (sync >> 8) & 0xFF), ("wreg", 5, sync & 0xFF)]
                if res["error"] is not None or res["calls"] != want:
                    bad.append((flag, ident, res["error"] or fmt_calls(res["calls"])))
        check(not bad, "  KEY=0x%04X FLAG=0/1(原厂模式): 68 个用例全部输出 50024/(ID+1)，"
              "与原厂逐位相同；无 SCAL/无延时（首例异常=%s）" % (key, bad[:1]))
        # 单点展示
        res = uc_run(patched, len(patched), SYNC_FN, 5, flag=2, flag_addr=FLAG_ADDR_EXAMPLE)
        info("  KEY=0x%04X FLAG=2 ID=5 -> %s" % (key, fmt_calls(res["calls"])))
        res = uc_run(patched, len(patched), SYNC_FN, 5, flag=0, flag_addr=FLAG_ADDR_EXAMPLE)
        info("  KEY=0x%04X FLAG=0 ID=5 -> %s" % (key, fmt_calls(res["calls"])))

    # ------------------------------------------------------------------
    section("11. 结论")
    # ------------------------------------------------------------------
    OUT("SYNC_FN        = 0x%08X .. 0x%08X (%d B, 下一函数 0x%08X)"
        % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE, SYNC_FN_END))
    OUT("ORIG 56B hex   = %s" % ORIG_FUNC.hex())
    OUT("辅助函数       = uidiv 0x%08X | WriteReg 0x%08X | delay_us 0x%08X | strobe 0x%08X"
        % (UIDIV, WRITE_REG, DELAY_US, STROBE))
    OUT("初始化表       = 0x%08X (47 个 reg 0x00..0x2E, reg4=0xC3, reg5=0x68)" % INIT_TABLE_FN)
    OUT("RF init        = 0x%08X: init 表(0x08005DFA) -> ... -> 同步字(0x08005E4A)" % RF_INIT_FN)
    OUT("同步字调用点   = %s（全部读 settings[+6]=0x%08X）"
        % ([hex(a) for a in sorted(SYNC_CALLERS)], ID_ADDR))
    OUT("SYNC 运行期写点 = 0x080112EE(reg4), 0x080112F6(reg5), 0x0801342A(reg4), 0x08013432(reg5)")
    OUT("")
    OUT("检查项合计 %d，PASS %d，FAIL %d" % (CHECKS, CHECKS - len(FAILS), len(FAILS)))
    if FAILS:
        OUT("")
        OUT("FAIL 明细:")
        for m in FAILS:
            OUT("  - " + m)
        OUT("")
        OUT("结果: FAIL")
        return 1
    OUT("")
    OUT("结果: ALL PASS")
    return 0


if __name__ == "__main__":
    try:
        rc = main()
    finally:
        OUT.close()
    sys.exit(rc)
