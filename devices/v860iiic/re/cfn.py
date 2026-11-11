# -*- coding: utf-8 -*-
"""V860IIIC C.Fn / settings 子系统静态分析（只读固件）。

目标：为「C.Fn 开关」补丁定位以下事实并留下可复现证据：
  1. settings 基址与各项（0..0xB）的读者/写者（前向污点扫描）；
  2. 最大值/回绕表与项数；
  3. 出厂默认值来源；
  4. settings 持久化 save/load 链路；
  5. 可用代码洞（注入 hook）与 gate 原厂字节；
  6. 同步字函数 / 辅助函数 / 调用点。

用法:
    tools\\python\\python.exe devices\\v860iiic\\re\\cfn.py
退出码 0 = 关键检查全 PASS。
"""
from __future__ import annotations

import hashlib
import os
import re
import struct
import sys

sys.dont_write_bytecode = True
_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

import fwlib as st  # noqa: E402

BASE = st.BASE
SETTINGS_BASE = 0x2000108A
SETTINGS_WIN_LO = 0x20001040
SETTINGS_WIN_HI = 0x20001120
CFN_MAX_TABLE = 0x080181AA
CFN_MAX_BYTES = bytes.fromhex("01010101011f630102060101")
CFN_ITEM_COUNT = 12
ITEM_NAMES_REF = ["m/ft", "AF", "STBY", "RX STBY", "SCAN", "CH",
                  "ID", "BEEP", "LIGHT", "LCD", "FEB ACL", "TX"]
DEC_HANDLER = 0x0800627C
INC_HANDLER = 0x0800D4A8
SAVE_FN = 0x08005FA0
LOAD_FN = 0x080159D0
ITEM9_HANDLER = 0x08015018
ITEM_SELECT_ADDR = 0x200004F2
EDIT_ADDR = 0x200004F3
SYNC_FN = st.SYNC_FN


def _off(addr: int) -> int:
    return addr - BASE


def header(t: str) -> None:
    print()
    print("=" * 78)
    print(t)
    print("=" * 78)


def fmt_ins(ins) -> str:
    return "0x%08X  %-12s %-7s %s" % (ins.address, ins.bytes.hex(" "),
                                      ins.mnemonic, ins.op_str)


ACCESS_MNEMONICS = {"ldrb", "strb", "ldrh", "strh", "ldr", "str", "ldrsb", "ldrsh"}
cur_code: st.Code | None = None


def parse_reg(s: str):
    s = s.strip()
    m = re.fullmatch(r"r(\d+)", s)
    return int(m.group(1)) if m else None


def parse_mem(op: str):
    parts = [p.strip() for p in op.split(",")]
    if len(parts) < 2 or not parts[1].startswith("["):
        return None
    dst = parse_reg(parts[0])
    mem = op[op.index("["):]
    inner = mem.strip("[]")
    toks = [t.strip() for t in inner.split(",")]
    base = parse_reg(toks[0]) if toks else None
    index = None
    imm = 0
    for t in toks[1:]:
        t = t.rstrip("]!").strip()
        if t.startswith("#"):
            v = t[1:]
            try:
                imm = int(v, 16) if "x" in v else int(v)
            except ValueError:
                imm = None
        elif "lsl" in t or "lsr" in t or "asr" in t:
            index = -1
            imm = None
        else:
            r = parse_reg(t)
            if r is not None:
                index = r
                imm = None
    return (dst, base, index, imm)


def code_literal_value(ins):
    m = re.search(r"\[pc,\s*#(0x[0-9a-fA-F]+|\d+)\]", ins.op_str)
    if not m or cur_code is None:
        return None
    v = m.group(1)
    imm = int(v, 16) if "x" in v else int(v)
    la = ((ins.address + 4) & ~3) + imm
    off = la - BASE
    if 0 <= off <= len(cur_code.image) - 4:
        return struct.unpack_from("<I", cur_code.image, off)[0]
    return None


def apply_effect(ins, state):
    out = dict(state)
    m = ins.mnemonic
    op = ins.op_str
    if m in ("bl", "blx"):
        for r in (0, 1, 2, 3, 12):
            out.pop(r, None)
        return out
    if m in ("pop", "ldm", "ldmia", "ldmfd"):
        for r in re.findall(r"r(\d+)", op.split("{", 1)[-1]):
            out.pop(int(r), None)
        return out
    if m == "ldr" and "[pc," in op and parse_mem(op):
        dst = parse_mem(op)[0]
        lv = code_literal_value(ins)
        if dst is not None:
            out.pop(dst, None)
            if lv is not None:
                out[dst] = lv
        return out
    dst = parse_mem(op)[0] if "[" in op and parse_mem(op) else None
    if dst is None:
        first = op.split(",", 1)[0].strip()
        dst = parse_reg(first)
    if dst is None:
        return out
    out.pop(dst, None)
    if m in ("movs", "mov"):
        rhs = op.split(",", 1)[1].strip() if "," in op else ""
        m2 = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", rhs)
        if m2:
            out[dst] = int(m2.group(1), 16) if "x" in m2.group(1) else int(m2.group(1))
        else:
            src = parse_reg(rhs)
            if src is not None and src in state:
                out[dst] = state[src]
    elif m in ("adds", "add", "subs", "sub"):
        toks = [t.strip() for t in op.split(",")]
        if len(toks) == 3:
            src = parse_reg(toks[1])
            imm_m = re.fullmatch(r"#(0x[0-9a-fA-F]+|\d+)", toks[2])
            if src is not None and src in state and imm_m:
                imm = int(imm_m.group(1), 16) if "x" in imm_m.group(1) else int(imm_m.group(1))
                out[dst] = state[src] + (imm if m.startswith("add") else -imm)
    return out


class Access:
    __slots__ = ("addr", "ins", "kind", "offset", "base_val", "fn_start", "fn_end",
                 "dynamic_index")

    def __init__(self, addr, ins, kind, offset, base_val, fn_start, fn_end, dyn=False):
        self.addr = addr
        self.ins = ins
        self.kind = kind
        self.offset = offset
        self.base_val = base_val
        self.fn_start = fn_start
        self.fn_end = fn_end
        self.dynamic_index = dyn


def scan_taint(code: st.Code, entries: list[int]) -> list[Access]:
    global cur_code
    cur_code = code

    def fn_of(addr):
        i = st.bisect.bisect_right(entries, addr) - 1
        s = entries[i]
        return s, code.next_entry(s, entries)

    seeds = []
    for ins in code.insns:
        if ins.mnemonic == "ldr" and "[pc," in ins.op_str:
            lv = code_literal_value(ins)
            mem = parse_mem(ins.op_str)
            dst = mem[0] if mem else None
            if lv is not None and dst is not None and SETTINGS_WIN_LO <= lv < SETTINGS_WIN_HI:
                seeds.append((ins, dst, lv))

    accesses: list[Access] = []
    for seed_ins, dst, base_val in seeds:
        fs, fe = fn_of(seed_ins.address)
        succs = code.successors(seed_ins, fs, fe)
        state0 = {dst: base_val}
        work = [(a, state0) for a in succs]
        seen: dict[int, set] = {}
        steps = 0
        while work and steps < 4000:
            addr, state = work.pop()
            steps += 1
            if not (fs <= addr < fe) or addr not in code.by_addr:
                continue
            key = frozenset(state.items())
            if key in seen.get(addr, set()):
                continue
            seen.setdefault(addr, set()).add(key)
            ins = code.by_addr[addr]
            mem = parse_mem(ins.op_str)
            if ins.mnemonic in ACCESS_MNEMONICS and mem:
                _dst2, breg, ireg, imm = mem
                if breg is not None and breg in state:
                    bv = state[breg]
                    if ireg is not None:
                        if SETTINGS_WIN_LO <= bv < SETTINGS_WIN_HI:
                            accesses.append(Access(addr, ins, ins.mnemonic, None, bv,
                                                   fs, fe, True))
                    elif imm is not None:
                        eff = bv + imm
                        if SETTINGS_WIN_LO <= eff < SETTINGS_WIN_HI:
                            accesses.append(Access(addr, ins, ins.mnemonic,
                                                   eff - SETTINGS_BASE, eff, fs, fe))
            state2 = apply_effect(ins, state)
            for s in code.successors(ins, fs, fe):
                work.append((s, state2))
    uniq = {}
    for a in accesses:
        uniq[(a.addr, a.kind, a.offset, a.dynamic_index)] = a
    return sorted(uniq.values(), key=lambda a: ((a.offset if a.offset is not None else 999),
                                                a.addr))


def fn_context(code, fn_start, entries, limit=8):
    """函数内 BL 目标 + 附近的 RAM 字面量，帮助推断语义。"""
    end = code.next_entry(fn_start, entries)
    bls, lits = [], []
    for ins in code.insns:
        if fn_start <= ins.address < end:
            if ins.mnemonic in ("bl", "blx"):
                t = code.bl_target(ins)
                if t is not None:
                    bls.append(t)
            lv = code_literal_value(ins)
            if lv is not None and 0x20000000 <= lv < 0x20002000:
                lits.append((ins.address, lv))
    return end, sorted(set(bls)), lits


def _all_zero_runs(image, min_len):
    runs = []
    start = None
    for i, b in enumerate(image):
        if b == 0:
            if start is None:
                start = i
        else:
            if start is not None and i - start >= min_len:
                runs.append((start, i - start))
            start = None
    if start is not None and len(image) - start >= min_len:
        runs.append((start, len(image) - start))
    return runs


def _abs_refs(image, lo, hi):
    out = []
    for off in range(0, len(image) - 3):
        v = struct.unpack_from("<I", image, off)[0]
        if lo <= v < hi:
            out.append(off)
    return out


def cave_refs(code, lo, hi):
    bl = [ins.address for ins in code.insns
          if (code.bl_target(ins) or 0) and lo <= code.bl_target(ins) < hi]
    br = [ins.address for ins in code.insns
          if code.branch_target(ins) is not None and
          lo <= (code.branch_target(ins) & ~1) < hi]
    lit = [ins.address for ins in code.insns
           if code.literal_value(ins) is not None and
           lo <= code.literal_value(ins)[1] < hi]
    raw = _abs_refs(code.image, lo, hi)
    raw_odd = _abs_refs(code.image, lo | 1, (hi - 1) | 1)
    return bl, br, lit, raw, raw_odd


def main() -> int:
    try:
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:  # noqa: BLE001
        pass
    print("V860IIIC C.Fn / settings 子系统分析（cfn.py）")
    print("固件:", st.FW_PATH)
    image = st.load_image()
    print("镜像: %d 字节 sha256=%s" % (len(image), hashlib.sha256(image).hexdigest()))
    if len(image) != st.IMAGE_SIZE or hashlib.sha256(image).hexdigest() != st.IMAGE_SHA256:
        print("FATAL: 固件身份不匹配")
        return 1
    code = st.Code(image)
    entries = code.function_entries()
    code.detect_switch_helpers(entries)
    global cur_code
    cur_code = code

    header("1. 镜像身份 / 基址 / 同步字函数")
    print("base=0x%08X  vector[0]=0x%08X  vector[1]=0x%08X"
          % (BASE, struct.unpack_from("<I", image, 0)[0],
             struct.unpack_from("<I", image, 4)[0]))
    print("SYNC_FN=0x%08X 原厂 56B：%s" % (SYNC_FN, image[_off(SYNC_FN):_off(SYNC_FN) + 56].hex(" ")))
    print("期望 ORIG_FUNC：%s" % st.ORIG_FUNC.hex(" "))
    print("一致: %s" % (image[_off(SYNC_FN):_off(SYNC_FN) + 56] == st.ORIG_FUNC))
    for name, a in (("UIDIV", st.UIDIV), ("WRITE_REG", st.WRITE_REG),
                    ("STROBE", st.STROBE), ("DELAY", st.DELAY), ("READ_REG", st.READ_REG),
                    ("INIT_TABLE_FN", st.INIT_TABLE_FN), ("RF_INIT_FN", st.RF_INIT_FN),
                    ("DEC_HANDLER", DEC_HANDLER), ("INC_HANDLER", INC_HANDLER),
                    ("SAVE_FN", SAVE_FN), ("LOAD_FN", LOAD_FN),
                    ("ITEM9_HANDLER", ITEM9_HANDLER)):
        print("  %-14s 0x%08X" % (name, a))
    callers = [ins.address for ins in code.insns
               if ins.mnemonic == "bl" and code.bl_target(ins) == SYNC_FN]
    print("BL -> SYNC_FN 调用点: %s" % [("0x%08X" % a) for a in callers])

    header("2. settings 字面量引用 / 最大值表 / 项数")
    refs: dict[int, list[int]] = {}
    for ins in code.insns:
        lv = code_literal_value(ins)
        if lv is not None and SETTINGS_WIN_LO <= lv < SETTINGS_WIN_HI:
            refs.setdefault(lv, []).append(ins.address)
    for v in sorted(refs):
        print("  字面量 0x%08X (%+d): %d 处，前 8: %s"
              % (v, v - SETTINGS_BASE, len(refs[v]),
                 ["0x%08X" % a for a in refs[v][:8]]))
    tbl = image[_off(CFN_MAX_TABLE):_off(CFN_MAX_TABLE) + CFN_ITEM_COUNT]
    print("最大值表 0x%08X: %s" % (CFN_MAX_TABLE, tbl.hex(" ")))
    print("与 V1C 表一致: %s" % (tbl == CFN_MAX_BYTES))
    print("每项 max:", ["item%d=%d" % (i, b) for i, b in enumerate(tbl)])
    for ins in code.insns:
        if code_literal_value(ins) == CFN_MAX_TABLE:
            print("  表引用 0x%08X %s %s" % (ins.address, ins.mnemonic, ins.op_str))

    header("3. settings[n] 读者/写者（前向污点扫描）")
    accesses = scan_taint(code, entries)
    by_off: dict[int, list[Access]] = {}
    for a in accesses:
        if a.offset is not None:
            by_off.setdefault(a.offset, []).append(a)
    print("总访问点: %d（含动态索引 %d）"
          % (len(accesses), sum(1 for a in accesses if a.dynamic_index)))
    for off in range(0, 0x1E):
        lst = by_off.get(off, [])
        if not lst:
            continue
        readers = [a for a in lst if a.kind.startswith("ldr")]
        writers = [a for a in lst if a.kind.startswith("str")]
        print()
        print("--- settings[%d] 0x%08X  R=%d W=%d ---"
              % (off, SETTINGS_BASE + off, len(readers), len(writers)))
        for a in lst:
            end, bls, lits = fn_context(code, a.fn_start, entries)
            print("  %s  off=%d fn=0x%08X..0x%08X bl=%s%s"
                  % (fmt_ins(a.ins), a.offset, a.fn_start, end - 1,
                     ["0x%08X" % b for b in bls[:5]],
                     " [动态索引]" if a.dynamic_index else ""))
    dyn = [a for a in accesses if a.dynamic_index]
    print()
    print("动态索引访问: %d" % len(dyn))
    for a in dyn[:40]:
        end, bls, lits = fn_context(code, a.fn_start, entries)
        print("  %s fn=0x%08X..0x%08X bl=%s"
              % (fmt_ins(a.ins), a.fn_start, end - 1, ["0x%08X" % b for b in bls[:5]]))

    header("4. 出厂默认 / 保存记录（函数画像）")
    for label, fn in (("DEFAULT", 0x08006780), ("SAVE", SAVE_FN), ("LOAD", LOAD_FN)):
        if code.at(fn) is None:
            print("%s 0x%08X: 不在函数入口，跳过" % (label, fn))
            continue
        end, bls, lits = fn_context(code, fn, entries)
        print("%s 0x%08X..0x%08X  bl=%s" % (label, fn, end - 1, ["0x%08X" % b for b in bls]))
        print("  RAM 字面量: %s" % [("0x%08X->0x%08X" % (a, v)) for a, v in lits[:12]])
        for ins in code.insns:
            if fn <= ins.address < end and ins.mnemonic in ("strb", "str", "ldrb", "ldr"):
                print("    " + fmt_ins(ins))

    header("5. 代码洞（全 0 死区，无引用）")
    runs = _all_zero_runs(image, 16)
    print(">=16B 全 0 段（长度前 20）:")
    for off, ln in sorted(runs, key=lambda x: -x[1])[:20]:
        print("  文件 0x%05X  地址 0x%08X  %d 字节" % (off, BASE + off, ln))
    print()
    usable = []
    for off, ln in sorted(runs, key=lambda x: -x[1]):
        if ln < 10:
            continue
        lo, hi = BASE + off, BASE + off + ln
        bl, br, lit, raw, raw_odd = cave_refs(code, lo, hi)
        if not (bl or br or lit or raw or raw_odd):
            usable.append((lo, ln))
    print("无任何引用的全 0 死区（>=10B，最多列 10 个）:")
    for lo, ln in usable[:10]:
        print("  0x%08X  %d 字节" % (lo, ln))

    header("6. gate / 原厂字节 / 数据流（C.Fn 改值即时生效 hook 用）")
    for label, site in (("DEC", 0x0800630C), ("INC", 0x0800D53E)):
        b = image[_off(site):_off(site) + 4]
        print("%s gate 0x%08X 原厂字节: %s" % (label, site, b.hex(" ")))
        fn = code.containing_function(site, entries)
        end = code.next_entry(fn, entries)
        reach = code.reachable_set(fn, end)
        preds = code.build_preds(fn, end, reach)
        res = code.resolve_reg(0, site, fn, end, preds)
        print("  所在函数 0x%08X..0x%08X；gate 处 r0 数据流: %s"
              % (fn, end - 1, {k: res[k] for k in ("status", "value") if k in res}))

    header("7. 无线设置同步解包（settings 覆盖保护候选）")
    # V1C 0x0800A6DC 解包函数在这里的对应地址用对齐推断；打印附近写入点
    for cand in (0x0800A7D0, 0x0800A7EA):
        try:
            b = image[_off(cand):_off(cand) + 2]
            print("0x%08X: %s  %s" % (cand, b.hex(" "),
                                      "strb r2,[r1,#8]" if b == bytes.fromhex("0872") else ""))
        except Exception:  # noqa: BLE001
            pass
    print("settings[8] 相关写者（来自第 3 节）应在此函数附近：")
    for a in by_off.get(8, []):
        if a.kind.startswith("str"):
            print("  " + fmt_ins(a.ins))

    # ---- 检查项 ----
    results = []

    def ck(cond, msg):
        results.append((bool(cond), msg))

    ck(image[_off(SYNC_FN):_off(SYNC_FN) + 56] == st.ORIG_FUNC,
       "同步字函数 0x%08X 原厂 56B 与期望一致" % SYNC_FN)
    ck(tbl == CFN_MAX_BYTES, "C.Fn 最大值表 0x%08X = V1C 12 项同值" % CFN_MAX_TABLE)
    ck(len(callers) == 5, "BL -> SYNC_FN 调用点 5 处: %s"
       % ["0x%08X" % a for a in callers])
    ck(any(a.offset == 8 and a.kind.startswith("ldr") for a in accesses),
       "settings[8](LIGHT 候选) 有静态读者")
    ck(any(a.offset == 6 and a.kind.startswith("ldr") for a in accesses),
       "settings[6](ID) 有静态读者")
    ck(any(a.offset == 8 and a.kind.startswith("str") for a in accesses),
       "settings[8] 有静态写者（默认值/解包/保存）")
    ck(bool(usable) and usable[0][1] >= 10,
       "存在无引用全 0 死区（>=10B 可放 hook）: %s"
       % ["0x%08X(%dB)" % (a, n) for a, n in usable[:3]])

    header("汇总")
    npass = sum(1 for ok, _ in results if ok)
    nfail = len(results) - npass
    for ok, msg in results:
        print("[%s] %s" % ("PASS" if ok else "FAIL", msg))
    print()
    print("检查项: %d  PASS: %d  FAIL: %d" % (len(results), npass, nfail))
    return 0 if nfail == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
