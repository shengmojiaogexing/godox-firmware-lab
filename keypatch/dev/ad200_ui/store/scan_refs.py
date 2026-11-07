#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""AD200 Pro V1.60 —— settings[7] / 存储链路 独立静态复核脚本。

不依赖 BRIEF 结论：自己用 capstone 线性扫描全镜像，枚举
  * 所有把 settings 基址 0x20000418 装进寄存器的字面量加载点，
    并在所在函数内做「基址寄存器 + 偏移」的轻量数据流跟踪，
    得到每个函数实际读/写了 settings[0..7] 的哪一项（0x08003000 基址）；
  * 直接引用 0x2000041F 的字面量；
  * 通过 movw/movs/add 立即数构造 0x418/0x41F 的点；
  * 与存储链路相关的字面量地址（0x20000464 / 0x20000484 / 0x0801F000 等）。

输出：可读报告（stdout） + JSON（--json 指定）。用法示例：

    devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\store\\scan_refs.py

依赖：capstone 5（devices/ad200pro 自带 python312 已安装）。
"""
from __future__ import annotations

import hashlib
import json
import os
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_OP_IMM, CS_OP_MEM, CS_OP_REG
from capstone.arm import ARM_REG_PC

BASE = 0x08003000
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
BIN = os.path.join(ROOT, "devices/ad200pro", "samples", "AD200pro_V1.60.bin")
SYMBOLS_PATH = os.path.join(ROOT, "devices/ad200pro", "analysis", "symbols.json")

SETTINGS_BASE = 0x20000418          # settings[0] .. settings[7] = 0x20000418..0x2000041F
SETTINGS7 = 0x2000041F
UI_BASE = 0x20000150
REC_MIRROR = 0x20000464             # 参数块镜像（8 个 word）
REC_SRC = 0x20000484                # builder 的 record 源
FLASH_PARAM = 0x0801F000            # 参数页基址（slot*100）

IMAGE = open(BIN, "rb").read()
SIZE = len(IMAGE)
SYMBOLS = {}
try:
    obj = json.load(open(SYMBOLS_PATH, encoding="utf-8"))
    for k, v in obj.get("symbols", {}).items():
        SYMBOLS[int(k, 16)] = v
except Exception:
    pass

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_MCLASS)
md.detail = True
md.skipdata = True


def off(addr: int) -> int:
    return addr - BASE


def rd32(addr: int):
    o = off(addr)
    if 0 <= o <= SIZE - 4:
        return struct.unpack_from("<I", IMAGE, o)[0]
    return None


def dis1(addr: int):
    """解码 addr 处一条指令（用于线性扫描；数据区可能解出假指令）。"""
    o = off(addr)
    if not (0 <= o <= SIZE - 2):
        return None
    for ins in md.disasm(IMAGE[o:o + 4], addr):
        return ins
    return None


def dis_range(start: int, end: int):
    start, end = max(start, BASE), min(end, BASE + SIZE)
    if end <= start:
        return []
    return list(md.disasm(IMAGE[off(start):off(end)], start))


def is_data(ins) -> bool:
    """skipdata 伪指令（.byte/.short/.word）没有 operands 语义。"""
    return ins is None or ins.id == 0 or ins.mnemonic.startswith(".")


def lit_target(ins):
    """ldr rX,[pc,#imm] -> 字面量池地址。"""
    if is_data(ins):
        return None
    if len(ins.operands) == 2 and ins.operands[0].type == CS_OP_REG \
            and ins.operands[1].type == CS_OP_MEM:
        m = ins.operands[1].mem
        if m.base == ARM_REG_PC:
            return ((ins.address + 4) & ~3) + m.disp
    return None


def lit_value(ins):
    lt = lit_target(ins)
    if lt is None:
        return None
    v = rd32(lt)
    if v is None:
        return None, lt
    return v, lt


def sym(addr: int) -> str:
    return SYMBOLS.get(addr & ~1, "")


# ---------------------------------------------------------------- 全镜像线性扫描
def scan_literals():
    """返回 {value: [(insn_addr, lit_addr)]}，只收录 ldr rX,[pc,#imm] 的 32 位字面量。"""
    refs = {}
    for a in range(BASE, BASE + SIZE - 3, 2):
        ins = dis1(a)
        if ins is None:
            continue
        lv = lit_value(ins)
        if lv is None:
            continue
        v, la = lv
        refs.setdefault(v, []).append((a, la))
    return refs


def scan_immediates():
    """找出立即数形式引用 0x418 / 0x41F 的指令（movs/movw/add/addw/cmp 等）。"""
    hits = []
    for a in range(BASE, BASE + SIZE - 3, 2):
        ins = dis1(a)
        if is_data(ins):
            continue
        for op in ins.operands:
            if op.type == CS_OP_IMM and op.imm in (0x418, 0x41F):
                hits.append((a, ins.mnemonic + " " + ins.op_str, op.imm))
    return hits


# ---------------------------------------------------------------- 函数边界
def load_functions():
    funcs = []
    try:
        raw = json.load(open(os.path.join(ROOT, "devices/ad200pro", "analysis", "functions.json"),
                             encoding="utf-8"))
        for f in raw:
            funcs.append((f["start"] & ~1, f.get("end", f["start"] + 2) & ~1))
    except Exception:
        pass
    starts = sorted({s for s, _ in funcs} | set(SYMBOLS))
    return starts


FUNC_STARTS = load_functions()


def func_start_of(addr: int) -> int:
    lo, hi = 0, len(FUNC_STARTS) - 1
    best = BASE
    while lo <= hi:
        mid = (lo + hi) // 2
        if FUNC_STARTS[mid] <= addr:
            best = FUNC_STARTS[mid]
            lo = mid + 1
        else:
            hi = mid - 1
    return best


def func_end_guess(start: int) -> int:
    """函数结束 = 下一个已知函数起点（或 +0x800）。

    注意：不能用第一个 pop{..pc} 截断——本固件很多函数是「早返回」多出口风格，
    被调函数常出现在函数中段，按 pop 截断会漏掉后半段对 settings 的访问。
    """
    later = [s for s in FUNC_STARTS if s > start]
    nxt = later[0] if later else start + 0x800
    return min(nxt, start + 0x800)


# ---------------------------------------------------------------- 基址+偏移数据流跟踪
def track_settings_offsets(func_addr: int):
    """在函数内跟踪 settings 基址寄存器，返回 [(insn_addr, kind, offset, text)]。

    kind: 'read' / 'write' / 'ptr'（指针算术）。offset 为相对 0x20000418 的字节偏移。
    """
    end = func_end_guess(func_addr)
    insns = dis_range(func_addr, end)
    reg_off = {}          # reg_id -> offset 或 None(未知)
    out = []
    for ins in insns:
        if is_data(ins):
            continue
        ops = ins.operands
        # 1) 字面量加载：基址或其它
        lt = lit_target(ins)
        if lt is not None and len(ops) == 2 and ops[0].type == CS_OP_REG:
            rd = ops[0].reg
            v = rd32(lt)
            if v == SETTINGS_BASE:
                reg_off[rd] = 0
            elif v == SETTINGS7:
                reg_off[rd] = 7
            else:
                reg_off[rd] = None
            continue
        # 2) 寄存器算术 / 移动
        handled = False
        if ins.mnemonic in ("adds", "add", "subs", "sub", "addw", "subw"):
            # add rD, rS, #imm  或 add rD, #imm
            if len(ops) == 3 and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_REG \
                    and ops[2].type == CS_OP_IMM:
                src = reg_off.get(ops[1].reg)
                dv = ops[2].imm if ins.mnemonic in ("adds", "add", "addw") else -ops[2].imm
                reg_off[ops[0].reg] = None if src is None else src + dv
                handled = True
            elif len(ops) == 2 and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_IMM:
                rd = ops[0].reg
                cur = reg_off.get(rd)
                dv = ops[1].imm if ins.mnemonic in ("adds", "add", "addw") else -ops[1].imm
                reg_off[rd] = None if cur is None else cur + dv
                handled = True
        elif ins.mnemonic in ("mov", "movs") and len(ops) == 2 \
                and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_REG:
            reg_off[ops[0].reg] = reg_off.get(ops[1].reg)
            handled = True
        if handled:
            continue
        # 3) 内存访问 [R, #imm]
        if len(ops) == 2 and ops[1].type == CS_OP_MEM:
            m = ops[1].mem
            if m.base in reg_off and reg_off[m.base] is not None and m.index == 0:
                base_off = reg_off[m.base]
                ea = base_off + m.disp
                mem = ins.mnemonic
                if mem in ("ldrb", "ldrsb", "ldrh", "ldr", "ldrsh"):
                    kind = "read"
                elif mem in ("strb", "strh", "str"):
                    kind = "write"
                else:
                    kind = "other"
                out.append((ins.address, kind, ea,
                            ins.mnemonic + " " + ins.op_str))
                # 目的寄存器被覆写
                if ops[0].type == CS_OP_REG and mem.startswith("ldr"):
                    reg_off[ops[0].reg] = None
                continue
        # 4) 其它指令写寄存器 -> 失效
        if ops and ops[0].type == CS_OP_REG and ins.mnemonic not in (
                "cmp", "tst", "strb", "str", "strh", "push", "cmn"):
            if not ins.mnemonic.startswith("b"):
                reg_off[ops[0].reg] = None
    return out


# ---------------------------------------------------------------- 报告
def fmt_access(rows):
    lines = []
    for a, kind, offv, txt in rows:
        idx = ("settings[%d]" % (offv // 4)) if offv % 4 == 0 and offv >= 0 \
            else ("settings[%d]+%d" % (offv // 4, offv % 4))
        lines.append("  %08x  %-5s %-14s (byte +0x%02x)  %-22s  in %s @%08x"
                     % (a, kind, idx, offv, txt, sym(func_start_of(a)) or "?",
                        func_start_of(a)))
    return lines


def main() -> int:
    print("=" * 78)
    print("AD200 Pro V1.60  settings[7] 独立静态复核")
    print("=" * 78)
    print("image : %s" % BIN)
    print("size  : %d (0x%x)" % (SIZE, SIZE))
    print("sha256: %s" % hashlib.sha256(IMAGE).hexdigest())
    print("base  : 0x%08x .. 0x%08x" % (BASE, BASE + SIZE - 1))
    print("settings base = 0x%08x, settings[7] = 0x%08x" % (SETTINGS_BASE, SETTINGS7))

    refs = scan_literals()
    print("\n-- 字面量引用计数（全镜像线性扫描，%d 个不同取值）--" % len(refs))

    def show(v, label):
        r = refs.get(v, [])
        print("  %-28s 0x%08x  refs=%d" % (label, v, len(r)))
        for a, la in r:
            ins = dis1(a)
            txt = (ins.mnemonic + " " + ins.op_str) if ins is not None else "?"
            print("      insn %08x (lit@%08x)  %-20s in %s @%08x"
                  % (a, la, txt, sym(func_start_of(a)) or "?", func_start_of(a)))

    for v, label in [
        (SETTINGS_BASE, "settings[0] 基址"),
        (SETTINGS7, "settings[7] 地址"),
        (0x2000041E, "settings[6] (=F7 ID)"),
        (UI_BASE, "UI 结构基址"),
        (REC_MIRROR, "参数块镜像 0x20000464"),
        (REC_SRC, "builder record 源 0x20000484"),
        (FLASH_PARAM, "flash 参数页 0x0801F000"),
        (0x0801F800, "flash 标志页 0x0801F800"),
        (0x50024 / 1, "占位"),
    ]:
        if label == "占位":
            continue
        show(v, label)

    imm_hits = scan_immediates()
    print("\n-- 立即数 0x418/0x41F 引用: %d --" % len(imm_hits))
    for a, t, v in imm_hits[:40]:
        print("  %08x  %s" % (a, t))

    # 全镜像基址加载点 -> 逐函数跟踪
    base_loads = refs.get(SETTINGS_BASE, [])
    funcs = sorted({func_start_of(a) for a, _ in base_loads})
    print("\n-- 通过 settings 基址字面量加载并访问 settings[] 的代码 --")
    print("   （基址加载点 %d 处，涉及函数 %d 个）" % (len(base_loads), len(funcs)))
    all_access = {}
    for fc in funcs:
        name = sym(fc) or ("FUNC_%08x" % fc)
        try:
            rows = track_settings_offsets(fc)
        except Exception as exc:                        # noqa: BLE001
            print("  [%s @%08x] track error: %s" % (name, fc, exc))
            continue
        if not rows:
            continue
        all_access[fc] = rows
        print("\n  [%s @%08x]" % (name, fc))
        for line in fmt_access(rows):
            print(line)

    # 关注 settings[7]
    print("\n" + "=" * 78)
    print("== settings[7] (0x2000041F) 访问者汇总 ==")
    print("=" * 78)
    s7 = []
    for fc, rows in all_access.items():
        for a, kind, offv, txt in rows:
            if offv == 7:
                s7.append((fc, a, kind, txt))
    if not s7:
        print("  （静态数据流未发现 settings[7] 访问——见下方『直接字面量/近似命中』）")
    for fc, a, kind, txt in sorted(s7):
        print("  %-5s @%08x in %-24s @%08x  %s" % (kind, a, sym(fc) or "?", fc, txt))

    # 逐字节汇总
    print("\n-- 每个函数对 settings 字节的读(read)/写(write)汇总 --")
    for fc in sorted(all_access):
        rows = all_access[fc]
        reads = sorted({o for _, k, o, _ in rows if k == "read"})
        writes = sorted({o for _, k, o, _ in rows if k == "write"})
        other = sorted({(o, k) for _, k, o, _ in rows if k not in ("read", "write")})
        print("  %-26s @%08x  read=%s write=%s %s"
              % (sym(fc) or "?", fc,
                 ["+0x%02x" % o for o in reads],
                 ["+0x%02x" % o for o in writes],
                 ("OTHER:" + repr(other)) if other else ""))

    result = {
        "image": BIN,
        "size": SIZE,
        "sha256": hashlib.sha256(IMAGE).hexdigest(),
        "settings_base": SETTINGS_BASE,
        "settings7": SETTINGS7,
        "literal_refs": {("0x%08x" % k): [("0x%08x" % a, "0x%08x" % la) for a, la in v]
                         for k, v in refs.items()},
        "settings7_access": [{"func": "0x%08x" % fc, "name": sym(fc),
                              "insn": "0x%08x" % a, "kind": kind,
                              "text": txt} for fc, a, kind, txt in sorted(s7)],
        "per_func": {"0x%08x" % fc: [{"insn": "0x%08x" % a, "kind": k,
                                       "off": o, "text": t} for a, k, o, t in rows]
                     for fc, rows in all_access.items()},
    }
    out_json = os.path.join(os.path.dirname(os.path.abspath(__file__)), "scan_refs.json")
    for i, argv in enumerate(sys.argv):
        if argv == "--json" and i + 1 < len(sys.argv):
            out_json = sys.argv[i + 1]
    with open(out_json, "w", encoding="utf-8") as f:
        json.dump(result, f, indent=1, ensure_ascii=False)
    print("\nJSON 写出: %s" % out_json)
    return 0


if __name__ == "__main__":
    sys.exit(main())
