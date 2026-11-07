# -*- coding: utf-8 -*-
"""AD200 Pro V1.60 —— C.Fn 菜单交互路径静态扫描（cfn-re / task-1）。

数据源：devices/ad200pro/analysis/disasm.txt（CFG 递归反汇编，含字面量注释）。
做法：解析 disasm.txt -> 按函数分组 -> 在函数内做一次"常量传播"抽象解释
（worklist + 在汇合点求交集），把 `ldrb/strb/ldr/str/ldrh/strh [rB,#imm]`
的基址解析成绝对 RAM 地址，从而回答：

  * 谁读/写 UI+0x12(0x20000162)/UI+0x14(0x20000164)/UI+0x13/0x16/0x17/0x26？
  * 谁读/写 settings[0..7] (0x20000418..0x2000041F)？
  * 0x08003FD0（C.Fn 渲染）、0x0800B9AC（同步字函数）、0x0800B820（保存）
    的全部调用点及其所在函数/调用者；
  * 关键函数反汇编转储（ui_key_scan_task / ui_menu_task / C.Fn 渲染 / display_task
    / 各按键处理函数 / 同步字函数）。

用法：
    python menu_scan.py            # 打印报告到 stdout（同时写 report.txt）
"""
from __future__ import annotations

import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))))
# 本文件: keypatch\dev\ad200_ui\menu\menu_scan.py -> 上 5 级 = 
BASE = 0x08003000
BIN = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")
DISASM = os.path.join(ROOT, "devices/ad200pro", "analysis", "disasm.txt")
FUNCTIONS_JSON = os.path.join(ROOT, "devices/ad200pro", "analysis", "functions.json")
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "report.txt")

# ------------------------------------------------------------------ 关键地址
UI = 0x20000150
SETTINGS = 0x20000418
FLASH_PARAM_PAGE = 0x0801F000

INTEREST = {
    UI + 0x04: "UI+0x04 key_pressed",
    UI + 0x05: "UI+0x05 scan_counter",
    UI + 0x06: "UI+0x06 repeat_counter",
    UI + 0x07: "UI+0x07 repeat_flag",
    UI + 0x08: "UI+0x08 flag",
    UI + 0x0C: "UI+0x0C menu_flag",
    UI + 0x0F: "UI+0x0F flag",
    UI + 0x12: "UI+0x12 CFN_OPEN  0x20000162",
    UI + 0x13: "UI+0x13 cfn_state 0x20000163",
    UI + 0x14: "UI+0x14 cfn_item  0x20000164",
    UI + 0x15: "UI+0x15 repeat    0x20000165",
    UI + 0x16: "UI+0x16 edit      0x20000166",
    UI + 0x17: "UI+0x17 flag",
    UI + 0x19: "UI+0x19 flag",
    UI + 0x1A: "UI+0x1A flag",
    UI + 0x26: "UI+0x26 last_key",
    UI + 0x2A: "UI+0x2A timer",
    UI + 0x2C: "UI+0x2C timer",
}
for _i in range(8):
    INTEREST[SETTINGS + _i] = "settings[%d] 0x%08X" % (_i, SETTINGS + _i)
# UI 结构全范围 0x20000150..0x200001A0（含 UI+0x20/0x21 的 +/- 状态机变量）
for _a in range(0x20000200, 0x20000240):
    INTEREST.setdefault(_a, "0x%08X" % _a)
for _a in range(UI, UI + 0x50):
    INTEREST.setdefault(_a, "UI+0x%02X 0x%08X" % (_a - UI, _a))

TARGET_FUNCS = {
    0x08008818: "ui_key_scan_task",
    0x0800C2D8: "ui_menu_task",
    0x08003FD0: "cfn_render",
    0x08009228: "display_task",
    0x08008D08: "ui_key_1",
    0x0800C190: "ui_key_2",
    0x08009B7C: "ui_key_4",
    0x0800B90C: "ui_key_20",
    0x0800BC34: "ui_key_80",
    0x08004270: "ui_key_100",
    0x080042A0: "ui_key_10",
    0x080048E4: "ui_key_14",
    0x0800B9AC: "rf_sync_word_func",
    0x0800B820: "flash_store_params",
    0x0800B578: "FUNC_0800b578",
    0x0800C768: "delay_us",
    0x0800CCAC: "segment_bit_set",
    0x0800CCC8: "settings_expand",
    0x080043E4: "param_record_build",
    0x0800AC58: "settings_load_task",
    0x080048E4 + 0: "ui_key_14",
}

CALLS_OF_INTEREST = [
    0x08003FD0,   # C.Fn 渲染
    0x0800B9AC,   # 同步字函数
    0x0800B820,   # flash_store_params
    0x0800CCC8,   # settings 展开
    0x080043E4,   # 记录构建
    0x0800B578,
    0x0800CC64,   # settings_save?
    0x0800C2D8,   # ui_menu_task
    0x08009B38,   # factory reset
    0x0800B7F4,   # flash_erase_params
    0x08005974,   # flash_write_params
]

# ------------------------------------------------------------------ 解析 disasm
INSN_RE = re.compile(r"^([0-9a-f]{8}):\s+([0-9a-f]{4}(?:[0-9a-f]{4})?)\s+(\S.*)$")
LIT_RE = re.compile(r"^;\s+0x([0-9a-f]{8})\s+=\s+0x([0-9a-f]{8})")
FUNC_RE = re.compile(r"^; FUNC_([0-9a-f]{8})\s")


class Insn:
    __slots__ = ("addr", "raw", "mn", "op", "lit", "func", "index")

    def __init__(self, addr, raw, mn, op, lit, func):
        self.addr = addr
        self.raw = raw
        self.mn = mn
        self.op = op
        self.lit = lit
        self.func = func
        self.index = -1

    def __repr__(self):
        return "0x%08X %-6s %-24s lit=%s" % (
            self.addr, self.mn, self.op, ("0x%08X" % self.lit) if self.lit is not None else "-")


def parse_disasm():
    lines = open(DISASM, encoding="utf-8", errors="replace").read().splitlines()
    insns = []
    funcs = {}          # func_addr -> [Insn]
    by_addr = {}
    cur_func = None
    for ln in lines:
        m = FUNC_RE.match(ln)
        if m:
            cur_func = int(m.group(1), 16)
            funcs.setdefault(cur_func, [])
            continue
        m = INSN_RE.match(ln.rstrip())
        if m:
            addr = int(m.group(1), 16)
            raw = m.group(2)
            text = m.group(3).strip()
            tok = text.split(None, 1)
            mn = tok[0]
            op = tok[1].strip() if len(tok) > 1 else ""
            ins = Insn(addr, raw, mn, op, None, cur_func)
            insns.append(ins)
            by_addr[addr] = ins
            if cur_func is not None:
                funcs[cur_func].append(ins)
            continue
    # 字面量从 analysis/functions.json 取（每条 literal 带加载它的指令地址）
    if os.path.isfile(FUNCTIONS_JSON):
        import json
        with open(FUNCTIONS_JSON, encoding="utf-8") as fh:
            jf = json.load(fh)
        for fn in jf:
            for lit in fn.get("literals", []):
                ins = by_addr.get(lit.get("insn"))
                if ins is not None:
                    ins.lit = int(lit["value"])
    for f, lst in funcs.items():
        for i, ins in enumerate(lst):
            ins.index = i
    return insns, funcs, by_addr


# ------------------------------------------------------------------ 常量传播
UNKNOWN = None


def _reg(tok):
    return tok.strip().lstrip("{}").strip()


def _parse_imm(tok):
    tok = tok.strip()
    m = re.match(r"^#(-?)(0x[0-9a-fA-F]+|\d+)$", tok)
    if not m:
        return None
    neg = m.group(1) == "-"
    v = int(m.group(2), 0)
    return -v if neg else v


def _parse_mem(op):
    """'r5, #0x12' -> (base, imm, None)；'r1, r2' -> (base, 0, 'r2')；'r1, r2, lsl #2' 同理。"""
    parts = [p.strip() for p in op.split(",")]
    if not parts:
        return None
    base = parts[0]
    if not re.match(r"^[a-z0-9]+$", base):
        return None
    imm = 0
    if len(parts) >= 2:
        v = _parse_imm(parts[1])
        if v is not None:
            imm = v
            if len(parts) >= 3:
                # ldr r0, [r1, #imm]! 之类，忽略
                pass
        elif re.match(r"^[a-z0-9]+$", parts[1]):
            return base, 0, parts[1]
        else:
            return None
    return base, imm, None


def analyze(funcs, by_addr):
    """返回 per-function: list of (insn, kind, address) 访问记录。"""
    accesses = {}      # func -> [(insn, 'R'/'W', addr)]
    for f, lst in funcs.items():
        if not lst:
            continue
        try:
            res = _analyze_func(f, lst, by_addr)
        except Exception as exc:  # 防御：単个函数失败不影响整体
            res = [("error", str(exc), None)]
        accesses[f] = res
    return accesses


def _succ(ins, lst, by_addr):
    """返回 [(addr, kind)]，kind='fall'/'branch'。"""
    out = []
    nxt = ins.addr + len(ins.raw) // 2
    mn, op = ins.mn, ins.op
    # 函数返回：pop {.., pc} / ldm ..!, {.., pc} / bx lr / mov pc, lr
    if mn in ("pop", "pop.w", "ldm", "ldmia", "ldm.w") and "pc" in op:
        return []
    if mn == "bx":
        return []
    if mn == "mov" and op.replace(" ", "") in ("pc,lr",):
        return []
    # 计算分支目标
    tgt = None
    m = re.search(r"#(0x[0-9a-f]+)$", op)
    if m and (mn.startswith("b") or mn in ("cbz", "cbnz")):
        if mn not in ("bl", "blx", "bx"):
            tgt = int(m.group(1), 16)
    if mn in ("b", "b.n", "bw"):
        if tgt is not None:
            return [(tgt, "branch")]
        return []
    if mn.startswith("b") and mn not in ("bl", "blx", "bx", "bics", "bfi", "bfc"):
        if tgt is not None:
            out.append((tgt, "branch"))
    if mn in ("bx", "blx"):
        return out
    # 下一条
    if nxt in by_addr and by_addr[nxt].func == ins.func:
        out.append((nxt, "fall"))
    return out


def _merge(a, b):
    out = {}
    for k in set(a) | set(b):
        va = a.get(k)
        vb = b.get(k)
        if k in a and k in b and va == vb:
            out[k] = va
    return out


def _analyze_func(f, lst, by_addr):
    """worklist 抽象解释；返回访问列表 [(insn, kind, addr, val)]"""
    entry = f
    start_state = {}
    work = [(entry, start_state)]
    seen = {}            # addr -> merged state
    recs = []
    rec_seen = set()
    steps = 0
    limit = 200000
    while work and steps < limit:
        steps += 1
        addr, state = work.pop()
        if addr not in by_addr or by_addr[addr].func != f:
            continue
        if addr in seen:
            merged = _merge(seen[addr], state)
            if merged == seen[addr]:
                continue
            state = merged
        seen[addr] = dict(state)
        ins = by_addr[addr]
        mn = ins.mn
        op = ins.op
        st = dict(state)

        def get(r):
            return st.get(r)

        def setr(r, v):
            st[r] = v

        # ---- literal load
        if mn in ("ldr", "ldr.w") and ins.lit is not None:
            m = re.match(r"^([a-z0-9]+),", op)
            if m:
                setr(m.group(1), ins.lit)
        # ---- 数据访问（内存）
        elif mn in ("ldrb", "strb", "ldrh", "strh", "ldr", "str", "ldrsb", "ldrsh"):
            m = re.match(r"^([a-z0-9]+),\s*\[([^\]]+)\]", op)
            if m:
                rd = m.group(1)
                mem = _parse_mem(m.group(2))
                if mem:
                    base, imm, idx = mem
                    bv = get(base)
                    if isinstance(bv, int):
                        kind = "W" if mn.startswith("str") else "R"
                        val = get(rd) if kind == "W" else None
                        if idx is None and imm is not None:
                            recs.append((ins, kind, bv + imm, val))
                        else:
                            # [base, idx_reg, ...]：基址已知、偏移为寄存器（如 settings[item]）
                            iv = get(idx) if idx else None
                            note = "%s + %s" % (hex(bv), idx if iv is None else "0x%X" % iv)
                            recs.append((ins, kind + "rel", bv, note))
                    if mn.startswith("ldr"):
                        setr(rd, UNKNOWN)
        # ---- movs rd,#imm
        if mn in ("movs", "mov", "mov.w", "mvn", "mvn.w"):
            parts = [p.strip() for p in op.split(",")]
            if len(parts) == 2:
                v = _parse_imm(parts[1])
                if v is not None:
                    setr(parts[0], v)
                elif re.match(r"^[a-z0-9]+$", parts[1]):
                    setr(parts[0], get(parts[1]))
        elif mn in ("adds", "add", "add.w", "subs", "sub", "sub.w", "addw", "subw"):
            parts = [p.strip() for p in op.split(",")]
            if len(parts) == 3:
                a = get(parts[1])
                v = _parse_imm(parts[2])
                if v is None and re.match(r"^[a-z0-9]+$", parts[2]):
                    v2 = get(parts[2])
                    v = v2 if isinstance(v2, int) else None
                if isinstance(a, int) and isinstance(v, int):
                    r = a - v if ("sub" in mn) else a + v
                    setr(parts[0], r & 0xFFFFFFFF)
                else:
                    setr(parts[0], UNKNOWN)
            elif len(parts) == 2:
                v = _parse_imm(parts[1])
                a = get(parts[0])
                if isinstance(a, int) and isinstance(v, int):
                    setr(parts[0], (a - v if "sub" in mn else a + v) & 0xFFFFFFFF)
                else:
                    setr(parts[0], UNKNOWN)
        elif mn in ("lsls", "lsrs", "asrs", "lsl", "lsr", "asr"):
            parts = [p.strip() for p in op.split(",")]
            if len(parts) == 3:
                a = get(parts[1])
                v = _parse_imm(parts[2])
                if isinstance(a, int) and isinstance(v, int):
                    if mn.startswith("lsl"):
                        setr(parts[0], (a << v) & 0xFFFFFFFF)
                    elif mn.startswith("lsr"):
                        setr(parts[0], (a & 0xFFFFFFFF) >> v)
                    else:
                        setr(parts[0], a >> v)
                else:
                    setr(parts[0], UNKNOWN)
        elif mn in ("ands", "and", "orrs", "orr", "eors", "eor", "bics", "bic", "ands.w", "orrs.w"):
            setr(op.split(",")[0].strip(), UNKNOWN)
        elif mn in ("uxtb", "uxth", "sxtb", "sxth"):
            parts = [p.strip() for p in op.split(",")]
            if parts:
                setr(parts[0], UNKNOWN)
        elif mn in ("adr", "add"):
            pass
        # ---- push/pop
        elif mn in ("push", "push.w"):
            pass
        elif mn in ("pop", "pop.w"):
            for r in op.strip("{} ").split(","):
                setr(r.strip(), UNKNOWN)
        # ---- call clobbers r0-r3
        elif mn in ("bl", "blx"):
            for r in ("r0", "r1", "r2", "r3", "ip", "lr"):
                st[r] = UNKNOWN

        for succ, _kind in _succ(ins, lst, by_addr):
            work.append((succ, dict(st)))

    # 去重
    out = []
    seen_rec = set()
    for ins, kind, a, val in recs:
        key = (ins.addr, kind, a)
        if key in seen_rec:
            continue
        seen_rec.add(key)
        out.append((ins, kind, a, val))
    return out


def fmt_name(f):
    return TARGET_FUNCS.get(f) or "FUNC_%08x" % f


def dump_func(func_addr, funcs, by_addr):
    lst = funcs.get(func_addr, [])
    print("\n; ================= FUNC 0x%08X (%s) %d insns ================" %
          (func_addr, fmt_name(func_addr), len(lst)))
    for ins in lst:
        lit = ""
        if ins.lit is not None:
            lit = "   ; lit 0x%08X" % ins.lit
        print("0x%08X: %-18s %-28s%s" % (ins.addr, ins.raw, ins.op, lit))


def main():
    insns, funcs, by_addr = parse_disasm()
    print("解析 disasm.txt：%d 条指令，%d 个函数（含字面量注释 %d 条）" %
          (len(insns), len(funcs), sum(1 for i in insns if i.lit is not None)))
    accesses = analyze(funcs, by_addr)

    # 1) 关注 RAM 地址的读者/写者
    print("\n" + "=" * 78)
    print("[1] 关键 RAM 地址的读/写点（常量传播解析基址）")
    print("=" * 78)
    hits = {}
    for f, recs in accesses.items():
        for ins, kind, a, val in recs:
            if a in INTEREST:
                hits.setdefault((a, kind), []).append((f, ins))
    for a in sorted(INTEREST):
        for kind in ("R", "W"):
            lst = hits.get((a, kind))
            if not lst:
                continue
            print("\n  %-42s %s" % (INTEREST[a], "读" if kind == "R" else "写"))
            for f, ins in sorted(lst, key=lambda x: x[1].addr):
                print("    0x%08X  in %-22s  0x%08X %-6s %s" %
                      (ins.addr, fmt_name(f), ins.addr, ins.mn, ins.op))

    # 1b) 寄存器偏移访问（[base, reg]）：例如 settings[UI+0x14] 这样的动态索引
    print("\n" + "-" * 78)
    print("[1b] 寄存器偏移访问（基址已知、索引为寄存器；典型如 settings[item]）")
    print("-" * 78)
    rel = {}
    for f, recs in accesses.items():
        for ins, kind, a, note in recs:
            if kind.endswith("rel") and a in INTEREST:
                rel.setdefault((a, kind), []).append((f, ins, note))
    for a in sorted(INTEREST):
        for kind in ("Rrel", "Wrel"):
            lst = rel.get((a, kind))
            if not lst:
                continue
            print("\n  %-42s %s" % (INTEREST[a], "读(索引)" if kind == "Rrel" else "写(索引)"))
            for f, ins, note in sorted(lst, key=lambda x: x[1].addr):
                print("    0x%08X  in %-22s  0x%08X %-6s %-22s  [%s]" %
                      (ins.addr, fmt_name(f), ins.addr, ins.mn, ins.op, note))

    # 2) 目标函数的调用点（bl/b）
    print("\n" + "=" * 78)
    print("[2] 关注函数被调用点（bl/b 目标）")
    print("=" * 78)
    callmap = {}
    for ins in insns:
        if ins.mn in ("bl", "blx", "b", "b.w"):
            m = re.search(r"#(0x[0-9a-f]+)$", ins.op)
            if m:
                tgt = int(m.group(1), 16)
                callmap.setdefault(tgt, []).append(ins)
    for tgt in CALLS_OF_INTEREST:
        lst = callmap.get(tgt, [])
        print("\n  0x%08X %-20s  调用点 %d 个:" % (tgt, fmt_name(tgt), len(lst)))
        for ins in sorted(lst, key=lambda x: x.addr):
            print("    0x%08X  in %-22s  0x%08X %-6s %s" %
                  (ins.addr, fmt_name(ins.func), ins.addr, ins.mn, ins.op))

    # 3) 反汇编转储
    print("\n" + "=" * 78)
    print("[3] 关键函数反汇编")
    print("=" * 78)
    for f in (0x08008818, 0x0800C2D8, 0x08004270, 0x080042A0, 0x0800B90C,
              0x0800BC34, 0x08008D08, 0x0800C190, 0x08009B7C, 0x08003FD0):
        dump_func(f, funcs, by_addr)

    return 0


if __name__ == "__main__":
    try:
        import io
        buf = io.StringIO()
        old = sys.stdout
        sys.stdout = buf
        rc = main()
        sys.stdout = old
        text = buf.getvalue()
        print(text[:200])
        with open(OUT, "w", encoding="utf-8") as fh:
            fh.write(text)
        print("\n[written] %s (%d chars)" % (OUT, len(text)))
        sys.exit(rc)
    except AssertionError as e:
        print("FAIL:", e)
        sys.exit(1)
