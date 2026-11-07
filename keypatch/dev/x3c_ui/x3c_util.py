# -*- coding: utf-8 -*-
"""X3-C v1.38 固件静态分析辅助库（ui-x3c 任务专用，只读原厂固件）。

提供：
  * Image            —— 原厂镜像 + 地址换算
  * functions()      —— Ghidra functions.csv（addr/size/callers）
  * func_at(addr)    —— 地址落在哪个函数
  * bl_scan()        —— 全镜像 BL/BLX 目标扫描（不依赖线性反汇编，按指令编码扫描）
  * find_words(val)  —— 全镜像 4 字节对齐字面量搜索（用于找 ldr 字面量池引用）
  * dis(addr, n)     —— capstone 反汇编
  * strings()        —— 抽取 hwscan_strings.tsv 便于"地址→字符串"

用法：
    sys.path.insert(0, r"keypatch\\dev\\x3c_ui")
    from x3c_util import *
"""
from __future__ import annotations

import csv
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
BASE = 0x08004800
GHIDRA = os.path.join(ROOT, "devices", "x3c", "re", "ghidra_exports", "main")
STRINGS_TSV = os.path.join(ROOT, "devices", "x3c", "re", "hwscan_strings.tsv")


class Image:
    def __init__(self, path: str = IMAGE):
        with open(path, "rb") as f:
            self.data = f.read()
        self.base = BASE

    def off(self, va: int) -> int:
        return va - self.base

    def va(self, off: int) -> int:
        return off + self.base

    def u8(self, va: int) -> int:
        return self.data[self.off(va)]

    def u16(self, va: int) -> int:
        return struct.unpack_from("<H", self.data, self.off(va))[0]

    def u32(self, va: int) -> int:
        return struct.unpack_from("<I", self.data, self.off(va))[0]

    def bytes_at(self, va: int, n: int) -> bytes:
        return self.data[self.off(va):self.off(va) + n]

    def __len__(self):
        return len(self.data)


_IMG = None


def img() -> Image:
    global _IMG
    if _IMG is None:
        _IMG = Image()
    return _IMG


def functions():
    rows = []
    with open(os.path.join(GHIDRA, "functions.csv"), encoding="utf-8") as f:
        for r in csv.DictReader(f):
            rows.append({
                "addr": int(r["addr"], 16),
                "name": r["name"],
                "size": int(r["size"]),
                "callers": r["callers"],
                "callees": r["callees"],
                "param_count": r["param_count"],
            })
    rows.sort(key=lambda r: r["addr"])
    return rows


_FUNCS = None


def funcs():
    global _FUNCS
    if _FUNCS is None:
        _FUNCS = functions()
    return _FUNCS


def func_at(addr: int):
    lo, hi = 0, len(funcs()) - 1
    best = None
    while lo <= hi:
        mid = (lo + hi) // 2
        f = funcs()[mid]
        if f["addr"] <= addr:
            best = f
            lo = mid + 1
        else:
            hi = mid - 1
    if best and best["addr"] <= addr < best["addr"] + best["size"]:
        return best
    return None


def func_name(addr: int) -> str:
    f = func_at(addr)
    return f["name"] if f else "?"


# ---------------------------------------------------------------------------
# BL / BLX 目标扫描（按 Thumb-2 编码位判断，避免线性反汇编 desync）
# ---------------------------------------------------------------------------
def _bl_target(hw1: int, hw2: int):
    """解码 32 位 Thumb BL/BLX。返回 (is_bl, target) 其中 target 已含 Thumb 位清除。"""
    if (hw1 & 0xF800) == 0xF000 and (hw2 & 0xD000) in (0xD000, 0xC000, 0xE000):
        s = (hw1 >> 10) & 1
        j1 = (hw2 >> 13) & 1
        j2 = (hw2 >> 11) & 1
        i1 = 1 - (j1 ^ s)
        i2 = 1 - (j2 ^ s)
        imm10 = hw1 & 0x3FF
        imm11 = hw2 & 0x7FF
        imm = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | (imm11 << 1)
        if s:
            imm -= (1 << 25)
        if (hw2 & 0xD000) == 0xC000:      # BLX (register encoded low bits)
            return True, None
        return True, imm
    return False, None


def bl_scan(targets=None):
    """扫描全镜像所有 BL 指令，返回 {target: [site_va, ...]}。

    targets 为 None 时返回全部 {target: [sites]}（只保留 0x08000000..0x08100000 范围）。
    """
    im = img()
    d = im.data
    out: dict[int, list[int]] = {}
    # 只扫 code 区（跳过 hot-shoe 子镜像 0x080F8000+ 与字库区尾部，但仍用 BL 编码扫）
    for o in range(0, len(d) - 4, 2):
        hw1, hw2 = struct.unpack_from("<HH", d, o)
        ok, imm = _bl_target(hw1, hw2)
        if not ok or imm is None:
            continue
        site = im.va(o)
        tgt = site + 4 + imm
        if not (0x08004800 <= tgt < 0x08100000):
            continue
        if targets is not None and tgt not in targets:
            continue
        out.setdefault(tgt, []).append(site)
    return out


def bl_callers(target: int):
    return bl_scan({target}).get(target, [])


# ---------------------------------------------------------------------------
# 字面量搜索（4 字节对齐）—— 找 `ldr rX,[pc,#imm]` 引用的数据地址
# ---------------------------------------------------------------------------
def find_words(values):
    """返回 [(value, [file_off...])]，只统计 4 字节对齐位置。"""
    im = img()
    d = im.data
    vals = set(values)
    hits = {v: [] for v in vals}
    for o in range(0, len(d) - 4, 4):
        w = struct.unpack_from("<I", d, o)[0]
        if w in hits:
            hits[w].append(o)
    return hits


def literal_refs(target_va: int):
    """找引用 target_va 的字面量位置，并回溯"哪个函数的字面量池"。"""
    hits = {}
    for w, offs in find_words([target_va]).items():
        for o in offs:
            site = img().va(o)
            f = func_at(site)
            hits.setdefault(site, []).append(f["name"] if f else "NO_FUNC")
    return hits


# ---------------------------------------------------------------------------
# 字符串
# ---------------------------------------------------------------------------
_STRINGS = None


def strings(minlen: int = 3):
    """从镜像里自行抽取 NUL 结尾的 UTF-8/ASCII 字符串 -> [(file_off, va, kind, text)]。

    （devices/x3c/re/hwscan_strings.tsv 的 CJK 编码已损坏，故本模块自己扫描；
    固件里的中文是标准 UTF-8，见 UI_STRINGS.md。）
    """
    global _STRINGS
    if _STRINGS is not None:
        return _STRINGS
    im = img()
    d = im.data
    out = []
    i, n = 0, len(d)
    while i < n:
        c = d[i]
        if 0x20 <= c < 0x7F or c >= 0x80:
            j = i
            while j < n and d[j] != 0:
                j += 1
            raw = d[i:j]
            if 3 <= len(raw) <= 200:
                try:
                    t = raw.decode("utf-8")
                    if all(32 <= ord(ch) or ord(ch) in (9,) for ch in t):
                        kind = "ascii" if all(ord(ch) < 128 for ch in t) else "utf8cjk"
                        out.append((i, im.va(i), kind, t))
                except UnicodeDecodeError:
                    pass
            i = j + 1
        else:
            i += 1
    _STRINGS = out
    return out


def near_string(va: int, radius: int = 0x40):
    """给定数据地址，找最近的字符串（常用于把 DAT_xxx 映射成文本）。"""
    best = None
    for off, sva, kind, text in strings():
        if sva <= va <= sva + radius:
            if best is None or sva > best[1]:
                best = (off, sva, kind, text)
    return best


def adr_scan(lo_va: int = 0x08005000, hi_va: int = 0x08070000):
    """扫描全镜像的 Thumb-1 `adr rX, #imm`（10100ddd iiiiiiii），返回 [(site, target)]。

    该固件用 ADR 引用邻近 rodata 字符串（见 devices/x3c/re/UI_STRINGS.md），
    因此本函数是"字符串 → 使用它的函数"的关键 xref 工具。
    """
    im = img()
    d = im.data
    out = []
    n = len(d)
    for o in range(0, n - 4, 2):
        hw1, hw2 = struct.unpack_from("<HH", d, o)
        site = im.va(o)
        pc = site + 4
        # Thumb-1 ADR: 10100 ddd iiiiiiii
        if (hw1 & 0xF800) == 0xA000:
            tgt = (pc & ~3) + (hw1 & 0xFF) * 4
            if lo_va <= tgt < hi_va:
                out.append((site, tgt))
            continue
        # Thumb-2 ADDW/SUBW Rd, PC, #imm12  (11110 i 10 0000 1111 ... / 11110 i 10 1010 1111 ...)
        if (hw1 & 0xFBFF) == 0xF20F or (hw1 & 0xFBFF) == 0xF2AF:
            i = (hw1 >> 10) & 1
            imm3 = (hw2 >> 12) & 7
            imm8 = hw2 & 0xFF
            imm12 = (i << 11) | (imm3 << 8) | imm8
            tgt = (pc & ~3) + imm12 if (hw1 & 0xFBFF) == 0xF20F else (pc & ~3) - imm12
            if lo_va <= tgt < hi_va:
                out.append((site, tgt))
    return out


def adr_refs_to(target_va: int, slack: int = 0):
    return [site for site, tgt in adr_scan() if target_va - slack <= tgt <= target_va + slack]


def string_users(name_or_text: str):
    """给字符串文本或地址，返回引用它的 ADR 站点及所在函数。"""
    hits = []
    needle = name_or_text
    for off, va, kind, text in strings():
        if needle in text or needle == "0x%08x" % va:
            sites = adr_refs_to(va)
            for s in sites:
                hits.append((va, text, s, func_name(s)))
    return hits


def dis(va: int, n: int = 20):
    import capstone
    im = img()
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
    lines = []
    for i in md.disasm(im.bytes_at(va, n * 4), va):
        lines.append("  %08x  %-14s %s %s" % (i.address, i.bytes.hex(" "), i.mnemonic, i.op_str))
    return "\n".join(lines)


def hexdump(va: int, n: int = 64):
    im = img()
    b = im.bytes_at(va, n)
    out = []
    for i in range(0, len(b), 16):
        chunk = b[i:i + 16]
        out.append("%08x  %-47s  %s" % (va + i, chunk.hex(" "),
                                        "".join(chr(c) if 32 <= c < 127 else "." for c in chunk)))
    return "\n".join(out)


def ram_ptr(dat_va: int) -> int:
    """Ghidra 的 DAT_xxxxxxxx 若是字面量池条目，返回它指向的 RAM 地址。"""
    return img().u32(dat_va)


if __name__ == "__main__":
    print("image", len(img()), "bytes, base 0x%08X" % BASE)
    print("functions", len(funcs()))
    print(dis(0x08014B28, 8))
