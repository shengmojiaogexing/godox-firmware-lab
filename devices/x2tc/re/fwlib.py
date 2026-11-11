# -*- coding: utf-8 -*-
"""X2TC_Z02_V1.01.bin（Godox X2T-C 引闪器）静态分析基础库。

固件身份（2026-11-11 复核）
---------------------------
* 文件 `devices/x2tc/firmware/X2TC_Z02_V1.01.bin` 是 **G3 V2.0 容器**：
  payload(79,704 B) + MD5(16) + 文件名 "X2TC_Z02_V1.01.bin"(18) + u16(18) + magic(8)
  总长 79,748 B，sha256 d185e410…；payload sha256 f12927c5…
* payload 是 **app 代码区**，加载基址 **0x08005000**（前 20 KB 为 bootloader，不在更新文件里）。
  向量表首字 = 0x20001158（初始 MSP），次字 = 0x080050D1（Reset，Thumb）→ base 0x08005000。
  资源/字模区 0x08018000+ 在 payload 内（例如字模表指针 0x08018574、0x080180CB）；
  参数页在 0x0801F000（payload 之外，由 G3/设备保留）。
* 「ID → CC2500 同步字」函数 **0x0801184C**（56 字节：0x0801184C..0x08011883，
  下一函数 0x08011884，50024 字面量在 +0x34 = 0x08011880）。
* 辅助函数：__aeabi_uidiv=0x080050EC、CC2500_WriteReg=0x08015CCE、
  delay_us=0x08015A2C、strobe=0x08015DB8。
* 设置区（菜单项字节数组）基址 **0x200009B8**，item 6 = ID（写同步字时取 settings[6]）；
  item 5 = CH。RAM 0x20000978 起是一张按 item 索引的「最大值表」（代码 0x0801178C 处
  `r2 = 0x200009B8 - 0x40; ldrb r2,[r2, idx]` 可证）。

本库只做静态分析，不写固件。
用法（仓库根）::

    tools\\python\\python.exe devices\\x2tc\\re\\cfn.py
"""
from __future__ import annotations

import bisect
import hashlib
import os
import re
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

_HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(_HERE)))      # D:\神牛
FW_PATH = os.path.join(ROOT, "devices", "x2tc", "firmware", "X2TC_Z02_V1.01.bin")

BASE = 0x08005000                     # app 镜像加载基址（payload 起点）
IMAGE_SIZE = 79704                    # payload 长度（不含 G3 容器 footer）
IMAGE_SHA256 = "f12927c54209ef258ae2246f9375063baad973ef0c857821169143aab2dde438"
CONTAINER_SHA256 = "d185e4101ad61422f2ac2bc9ee7ade1e6c245c6002361dfd50ed5f9e7b90210b"
CONTAINER_TOTAL = 79748

SYNC_FN = 0x0801184C                  # void rf_set_sync(uint8_t id)
SYNC_FN_END = 0x08011884
SYNC_FN_SIZE = 56
NEXT_FN = 0x08011884

ORIG_FUNC_HEX = (
    "0c4a10b5411c1046f3f74afc0446010a042004f036fae1b2052004f032fa"
    "142004f0def8332004f0a1fa7d20c00004f0d7f810bd68c30000"
)
ORIG_FUNC = bytes.fromhex(ORIG_FUNC_HEX)
assert len(ORIG_FUNC) == SYNC_FN_SIZE

ID_NUMERATOR = 50024                  # 0xC368
REG_SYNC1 = 4
REG_SYNC0 = 5

UIDIV = 0x080050EC                    # __aeabi_uidiv(r0/r1) -> r0=商, r1=余
WRITE_REG = 0x08015CCE                # WriteReg(r0=reg, r1=val)
STROBE = 0x08015DB8                   # strobe(r0=cmd)
DELAY = 0x08015A2C                    # delay_us(r0)

SETTINGS_BASE = 0x200009B8            # 菜单项字节数组（item n = base + n）
ID_ADDR = SETTINGS_BASE + 6
CH_ADDR = SETTINGS_BASE + 5
MAX_TABLE_BASE = 0x20000978           # 按 item 索引的 max 表（64 B，紧挨 settings 之前）
UI_BASE = 0x20000044                  # 菜单 UI 状态
UI2_BASE = 0x20000064                 # 菜单编辑状态（+4 编辑中 / +5 方向 / +6 item 索引）
FB_BASE = 0x20000268                  # 点阵帧缓存（stride 0xA0 = 160 B/行）

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


# ---------------------------------------------------------------------------
# 容器 / 通用工具
# ---------------------------------------------------------------------------
def split_container(blob: bytes) -> dict:
    """拆 G3 V2.0 容器：返回 {ok,total,payload,name,md5}（不依赖 keypatch.container）。"""
    magic = bytes.fromhex("aa55669977882233")
    info = {"ok": False, "total": len(blob), "payload": b"", "name": "", "md5": ""}
    if len(blob) < 26 or blob[-8:] != magic:
        return info
    name_len = int.from_bytes(blob[-10:-8], "little")
    if not (0 < name_len <= len(blob) - 26):
        return info
    payload = blob[:-26 - name_len]
    md5 = blob[-26 - name_len:-10 - name_len]
    name = blob[-10 - name_len:-10].decode("utf-8", "replace")
    info.update(ok=(hashlib.md5(payload).digest() == md5), payload=payload,
                name=name, md5=md5.hex())
    return info


def load_container() -> bytes:
    with open(FW_PATH, "rb") as f:
        return f.read()


def load_image() -> bytes:
    """返回容器里的 app payload（分析/补丁都针对 payload）。"""
    blob = load_container()
    info = split_container(blob)
    if not info["ok"]:
        raise SystemExit("固件不是合法 G3 V2.0 容器: %r" % info)
    return info["payload"]


def _off(addr: int) -> int:
    return addr - BASE


def parse_imm(op: str):
    m = re.search(r"#(-?(?:0x[0-9a-fA-F]+|\d+))", op)
    if not m:
        return None
    s = m.group(1)
    return int(s, 16) if "x" in s else int(s)


class Code:
    """capstone 线性反汇编结果（含 skipdata，避免字面量池处停止）。"""

    def __init__(self, image: bytes):
        self.image = image
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
        md.skipdata = True
        self.insns = list(md.disasm(image, BASE))
        self.by_addr = {i.address: i for i in self.insns}
        self.gaps = [a for a in range(BASE, BASE + len(image), 2)
                     if a not in self.by_addr]
        self.switch_helpers: set[int] = set()

    def detect_switch_helpers(self, entries: list[int]) -> set[int]:
        found: set[int] = set()
        for e in entries:
            ins = self.by_addr.get(e)
            if ins is None or ins.mnemonic != "push":
                continue
            seq = [ins]
            a = e + ins.size
            for _ in range(16):
                i2 = self.by_addr.get(a)
                if i2 is None:
                    break
                seq.append(i2)
                a += i2.size
            text = " | ".join("%s %s" % (i.mnemonic, i.op_str) for i in seq)
            reads_lr = ("mov r4, lr" in text or "mov r12, lr" in text
                        or "mov ip, lr" in text)
            has_bx = "bx r3" in text or "bx r12" in text or "bx ip" in text
            has_sub1 = any(i.mnemonic == "subs" and i.op_str.endswith("#1")
                           for i in seq)
            if reads_lr and has_bx and has_sub1:
                found.add(e)
        self.switch_helpers = found
        return found

    def is_switch_dispatch(self, ins) -> bool:
        t = self.bl_target(ins)
        return t is not None and t in self.switch_helpers

    def at(self, addr: int):
        return self.by_addr.get(addr)

    def disasm(self, lo: int, hi: int):
        """按需反汇编 [lo, hi)（线性，skipdata）。"""
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
        md.skipdata = True
        return md.disasm(self.image[_off(lo):_off(hi)], lo)

    def literal_value(self, ins):
        if ins.mnemonic != "ldr" or "[pc," not in ins.op_str:
            return None
        m = re.search(r"#(-?0x[0-9a-fA-F]+|-?\d+)", ins.op_str)
        if not m:
            return None
        imm = int(m.group(1), 16) if "x" in m.group(1) else int(m.group(1))
        lit_addr = ((ins.address + 4) & ~3) + imm
        off = lit_addr - BASE
        if off < 0 or off + 4 > len(self.image):
            return None
        return lit_addr, struct.unpack_from("<I", self.image, off)[0]

    def branch_target(self, ins):
        if not (ins.mnemonic == "b" or ins.mnemonic in COND_BRANCHES
                or ins.mnemonic in ("bl", "blx", "cbz", "cbnz")):
            return None
        m = re.search(r"#(0x[0-9a-fA-F]+)", ins.op_str)
        return int(m.group(1), 16) if m else None

    def bl_target(self, ins):
        if ins.mnemonic not in ("bl", "blx"):
            return None
        t = self.branch_target(ins)
        return None if t is None else (t & ~1)

    @staticmethod
    def written_regs(ins) -> set[int]:
        regs: set[int] = set()
        op = ins.op_str
        m = ins.mnemonic
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
        if m in DEST_WRITE_MNEMONICS:
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
        if ins.mnemonic in DEST_WRITE_MNEMONICS:
            if ins.op_str.split(",", 1)[0].strip() == "pc":
                return True
        return False

    def successors(self, ins, fn_start: int, fn_end: int) -> list[int]:
        nxt = ins.address + ins.size
        m = ins.mnemonic
        out: list[int] = []
        if m == "b":
            t = self.branch_target(ins)
            if t is not None:
                out.append(t & ~1)
        elif m in COND_BRANCHES or m in ("cbz", "cbnz"):
            t = self.branch_target(ins)
            if t is not None:
                out.append(t & ~1)
            out.append(nxt)
        elif m in ("bx", "blx") or self._has_pc_write(ins):
            out = []
        elif m in ("bl", "blx"):
            if self.is_switch_dispatch(ins):
                out = []
            else:
                out = [nxt]
        else:
            out = [nxt]
        return [a for a in out
                if fn_start <= a < fn_end and a in self.by_addr]

    def function_entries(self) -> list[int]:
        entries: set[int] = set()
        for ins in self.insns:
            t = self.bl_target(ins)
            if t is not None and BASE <= t < BASE + len(self.image):
                entries.add(t)
        for off in range(0, 0x50, 4):
            if off + 4 > len(self.image):
                break
            v = struct.unpack_from("<I", self.image, off)[0]
            if 0x08000000 <= v < 0x09000000 and BASE <= (v & ~1) < BASE + len(self.image):
                entries.add(v & ~1)
        return sorted(entries)

    def containing_function(self, addr: int, entries: list[int]) -> int:
        i = bisect.bisect_right(entries, addr) - 1
        return entries[i] if i >= 0 else BASE

    def next_entry(self, addr: int, entries: list[int]) -> int:
        i = bisect.bisect_right(entries, addr)
        return entries[i] if i < len(entries) else BASE + len(self.image)

    def reachable_set(self, fn_start: int, fn_end: int) -> set[int]:
        seen: set[int] = set()
        work = [fn_start]
        while work:
            a = work.pop()
            if a in seen or not (fn_start <= a < fn_end):
                continue
            ins = self.by_addr.get(a)
            if ins is None:
                continue
            seen.add(a)
            work.extend(self.successors(ins, fn_start, fn_end))
        return seen

    def build_preds(self, fn_start: int, fn_end: int,
                    reachable: set[int] | None = None) -> dict[int, set[int]]:
        preds: dict[int, set[int]] = {}
        addrs = [a for a in sorted(self.by_addr) if fn_start <= a < fn_end
                 and (reachable is None or a in reachable)]
        for a in addrs:
            ins = self.by_addr[a]
            for s in self.successors(ins, fn_start, fn_end):
                if reachable is not None and s not in reachable:
                    continue
                preds.setdefault(s, set()).add(a)
        return preds
