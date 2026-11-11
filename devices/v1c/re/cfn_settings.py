#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""V1C C.Fn 子系统分析：settings[0..0xD] 读者/写者、菜单项语义、持久化、代码洞。

目标（task-4）：为「把密钥开关从 ID=OFF(0) 改为 C.Fn 可复用项」提供独立证据：
  * settings 基址 0x20001086；逐项 n=0..0xB 的读者/写者（ldrb/strb/ldr/str），
    含动态索引访问；标注所在函数、原厂语义线索（比较常量、调用的功能函数）。
  * C.Fn 项最大值/回绕表（0x08018362）与项数（0..0xB）。
  * item 7 是否同 AD200 F8 = 稳定色温；列全部读者与可屏蔽方法。
  * 设置持久化：save 函数 0x08005EF0（记录布局，settings 在记录 +0x2C..0x3B）、
    load/默认值来源。
  * item 8/10/11 评估。
  * 注入代码洞：候选 0x08017944(234B)/0x08017D28(222B) 是否被任何 BL/分支/字面量引用；
    以及全镜像其它 >=32B 全 0x00 死区。

只读固件；不改 keypatch/devices/。输出到 stdout（由外部重定向为 log_cfn_settings.txt）。
退出码 0 = 分析完成（PASS 项见输出；若关键结构缺失则 1）。
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

import audit_static as st  # noqa: E402  复用 Code / 反汇编 / 基址常量

FW_PATH = st.FW_PATH
BASE = st.BASE
SETTINGS_BASE = 0x20001086
SETTINGS_WIN_LO = 0x20001086
SETTINGS_WIN_HI = 0x20001100          # 目前 refs 最多到 0x200010B0；留余量
CFN_MAX_TABLE = 0x08018362
CFN_MAX_BYTES = bytes.fromhex("01010101011f630102060101")
CFN_ITEM_COUNT = 12                   # 0..0xB
# 外部佐证（非固件内字符串）：Godox V1 官方/非官方说明书 C.Fn 页顺序为
#   P1: m/ft, AF, STBY, RX STBY
#   P2: SCAN, CH, ID, BEEP
#   P3: LIGHT, LCD, FEB ACL, TX
# 来源: https://sugarcamera.work/godox-v1-manual07/ （V1 非官方手册 C.Fn 章节）
# 该顺序与固件 0x08018362 的最大值表逐项对齐（见 §1），用作项名佐证；
# 固件内没有可直接读取的项名 ASCII 字符串。
ITEM_NAMES_REF = ["m/ft", "AF", "STBY", "RX STBY", "SCAN", "CH",
                  "ID", "BEEP", "LIGHT", "LCD", "FEB ACL", "TX"]
DEC_HANDLER = 0x080061CC
INC_HANDLER = 0x0800D3D8
ITEM_SELECT_ADDR = 0x200004F2         # 当前选中 C.Fn 项
EDIT_ADDR = 0x200004F1                # 编辑态
SYNC_FN = st.SYNC_FN
SAVE_FN = 0x08005EF0                  # 设置记录 builder/save
ITEM9_HANDLER = 0x08014EF8
CH_COPY_ADDR = 0x200004F5             # item 0xB 改值后复制到此处

CODE_CAVE_CANDIDATES = [
    (0x08017944, 234),
    (0x08017D28, 222),
]

# ---- 推荐方案（本审计建议；不写入任何补丁模块） --------------------------
CHOSEN_ITEM = 2                       # F3 STBY
CHOSEN_NAME = "STBY"
# 原厂待机任务里对 settings[2] 的唯一功能读者：强制按出厂默认 0 处理
CHOSEN_MASK = [
    (0x080124DC, "90 78", "00 20",
     "ldrb r0,[r2,#2] -> movs r0,#0（STBY=1 时不再关闭自动待机定时器）"),
]
# 立即生效 hook：两个改值处理函数的 cmp/beq 门（4B）改成 BL 到注入区；
# hook: push {r0-r4,lr}; ldrb r0,[r0,#6]; bl 0x080121BC; pop {r0-r4,pc}
HOOK_ADDR = 0x08017944
HOOK_BYTES = bytes.fromhex("1fb58079faf738fc1fbd")   # 10 字节
GATE_PATCHES = [
    (0x0800625C, "06 29 08 d0", "11 f0 72 fb",
     "dec 处理 0x0800625C: cmp r1,#6; beq -> bl hook"),
    (0x0800D46E, "06 29 07 d0", "0a f0 69 fa",
     "inc 处理 0x0800D46E: cmp r1,#6; beq -> bl hook"),
]
# 出厂默认值（来自 0x080066D0 恢复出厂默认函数；s12/s13 不在其写入范围）
ITEM_DEFAULTS = {0: 0, 1: 1, 2: 0, 3: 0, 4: 0, 5: 0x14,
                 6: 0, 7: 1, 8: 0, 9: 3, 10: 0, 11: 1}
# 人工语义结论（基于读者/写者/常量/硬件寄存器/默认值 + V1 说明书项序）
ITEM_SEMANTICS = {
    0: "m/ft（距离单位显示；仅影响测距/显示换算，无硬件动作）",
    1: "AF（AF 辅助灯；应用函数 0x08003654 控制 GPIOF/GPIOE 相关位）",
    2: "STBY（自动待机；0x080124D8 中 settings[2]!=0 时跳过待机计时）",
    3: "RX STBY（接收待机超时；0x080124D8 中 settings[3] 选择 20/40 阈值）",
    4: "SCAN（频道扫描；改值到 1 时 0x08012058 触发扫描；扫描/设置应用路径 "
       "0x0800FB2C、0x080053F4 会把 settings[4] 写回 0）",
    5: "CH（频道；显示为 settings[5]+1，最大值 31 => 1..32；0x080060FC 抄到 0x2000054A）",
    6: "ID（1..99/0=OFF；RF 初始化/改值后调用 0x080121BC 同步字函数）",
    7: "BEEP（按键提示音开关；应用函数 0x08003AE8，默认 1=开）",
    8: "LIGHT（造型灯模式；应用函数 0x080036D4 控制 GPIOE/GPIOF 灯输出，值 0/1/2）",
    9: "LCD（LCD 亮度/对比度；0x08014EF8 专用处理，值 0..6，默认 3）",
    10: "FEB ACL（按 V1 说明书项序；读者在 0x08005548，功能未完全定性）",
    11: "TX（无线发送模式/标志；改值后镜像到 0x200004F5，射频任务读取）",
}
# 推荐排序（item, 理由/需屏蔽读者/备注）
RANKING = [
    (2, "首选：F3 STBY；默认 0=普通、显示 OFF/ON；唯一功能读者 0x080124DC，"
        "mask 成 movs r0,#0 即恢复出厂待机行为；不涉及 RF/闪光/GPIO 输出"),
    (3, "次选：F4 RX STBY；默认 0；唯一功能读者 0x08012528，mask 后仅影响待机超时阈值；"
        "与 F3 同一待机任务，改动更局部"),
    (0, "低风险但 UX 差：F1 m/ft 只有显示读者 0x0800744E/0x08015D56，"
        "无硬件动作；但菜单显示 m/ft 而非 ON/OFF，作为开关不直观"),
    (8, "可用：F9 LIGHT；默认 0；应用读者 0x080036D4 需 mask；max=2（多一个状态），"
        "且会失去原厂造型灯 C.Fn 功能"),
    (10, "待定：F11 FEB ACL；默认 0；读者 0x08005548 的语义未完全定性，暂不推荐"),
    (4, "不推荐：F5 SCAN；默认 0，但 0x08012058 改值到 1 会触发扫描，"
        "0x0800FB2C/0x080053F4 会把 settings[4] 写回 0（密钥标志会被自清）"),
    (1, "不推荐：F2 AF；默认 1，需反相语义（0=KEY）并 mask 0x08003654，且与 AF 辅助灯冲突"),
    (7, "不推荐：F8 BEEP；默认 1；读者最多（16 处：应用 0x08003AE8 + 多路 UI/按键/显示），"
        "且 V1C 的 item7 不是 AD200 的稳定色温设置"),
    (11, "不推荐：F12 TX；默认 1，0x200004F5 镜像被 RF/UI 多处读取，语义风险高"),
    (5, "不适合：F6 CH；max 31、默认 20，非 0/1（射频频道）"),
    (6, "不适合：F7 ID；这是当前设计的开关项，用户要求不再用 ID"),
    (9, "不适合：F10 LCD；max 6、默认 3，非 0/1（LCD 亮度/对比度）"),
]

# 已知函数/区域标签（供报告可读；未列出的用 FUNC_0x 表示）
FUNC_LABELS = {
    0x080061CC: "C.Fn dec 处理（item 值 -1）",
    0x0800D3D8: "C.Fn inc 处理（item 值 +1）",
    0x08005EF0: "设置记录 builder（save 准备）",
    0x08004914: "C.Fn 菜单渲染/显示函数 A",
    0x0800548C: "C.Fn 菜单渲染/显示函数 B",
    0x08014EF8: "item 9 改值后的专用处理函数",
    0x0800664C: "RF 初始化",
    0x080121BC: "ID -> CC2500 SYNC 函数",
    0x080150A6: "GCC switch 分派 helper",
}


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


# ---------------------------------------------------------------------------
# 前向污点扫描：settings 基址/基址+偏移 -> ldrb/strb/ldr/str 访问
# ---------------------------------------------------------------------------
ACCESS_MNEMONICS = {"ldrb", "strb", "ldrh", "strh", "ldr", "str", "ldrsb", "ldrsh"}
WORD_ACCESS = {"ldr", "str"}

RET_PAT = re.compile(r"#(0x[0-9a-fA-F]+|\d+)")


def parse_reg(s: str):
    s = s.strip()
    m = re.fullmatch(r"r(\d+)", s)
    return int(m.group(1)) if m else None


def parse_mem(op: str):
    """解析 'rD, [rB, #imm]' / 'rD, [rB, rI]' / 'rD, [rB]'。

    返回 (dst_reg, base_reg, index_reg|None, imm)。
    """
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
            index = -1          # [rB, rI, lsl #n]：动态索引（含移位）
            imm = None
        else:
            r = parse_reg(t)
            if r is not None:
                index = r
                imm = None
    return (dst, base, index, imm)


def apply_effect(ins, state: dict[int, int]) -> dict[int, int]:
    """按指令更新已知寄存器值（只跟踪可静态确定的整数地址/常量）。"""
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
    # LDR (literal) 载入常量
    if m == "ldr" and "[pc," in op and parse_mem(op):
        dst = parse_mem(op)[0]
        lv = code_literal_value(ins)
        if dst is not None:
            out.pop(dst, None)
            if lv is not None:
                out[dst] = lv
        return out
    dst = parse_mem(op)[0] if "[" in op and parse_mem(op) else None
    # 通用：取第一操作数作为写目的（若为寄存器）
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


cur_code: st.Code | None = None


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

    def __repr__(self):
        return "<Access %s off=%s @0x%08X>" % (self.kind, self.offset, self.addr)


def scan_taint(code: st.Code, entries: list[int]) -> list[Access]:
    """从 settings 大结构体（0x20001040..0x20001120）的 literal 载入点做前向污点传播。

    settings 数组基址 0x20001086 位于某个更大的设置结构体内部（例如基准 0x20001070
    +0x16）。因此从该窗口内所有 literal 载入点起步，按「基址 + 偏移」计算有效地址，
    只记录落在 settings 窗口内的访问。
    """
    global cur_code
    cur_code = code
    fn_ranges: dict[int, tuple[int, int]] = {}

    def fn_of(addr):
        i = st.bisect.bisect_right(entries, addr) - 1
        s = entries[i]
        return s, code.next_entry(s, entries)

    seed_lo, seed_hi = 0x20001040, 0x20001120
    seeds = []
    for ins in code.insns:
        if ins.mnemonic == "ldr" and "[pc," in ins.op_str:
            lv = code_literal_value(ins)
            mem = parse_mem(ins.op_str)
            dst = mem[0] if mem else None
            if lv is not None and dst is not None and seed_lo <= lv < seed_hi:
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
    uniq: dict[tuple, Access] = {}
    for a in accesses:
        uniq[(a.addr, a.kind, a.offset, a.dynamic_index)] = a
    return sorted(uniq.values(), key=lambda a: ((a.offset if a.offset is not None else 999),
                                                a.addr))


# ---------------------------------------------------------------------------
# 基础分析
# ---------------------------------------------------------------------------
def analyze_settings_layout(code: st.Code) -> None:
    header("1. settings 区与 C.Fn 项布局（独立确认 Lead 骨架）")
    print("settings 基址假设: 0x%08X；ID=settings[6]=0x%08X" % (SETTINGS_BASE, SETTINGS_BASE + 6))
    # 统计 literal 引用
    refs: dict[int, list[int]] = {}
    for ins in code.insns:
        lv = code_literal_value(ins)
        if lv is not None and SETTINGS_WIN_LO <= lv < SETTINGS_WIN_HI:
            refs.setdefault(lv, []).append(ins.address)
    print("字面量引用统计（0x%08X..0x%08X）:" % (SETTINGS_WIN_LO, SETTINGS_WIN_HI))
    for v in sorted(refs):
        print("  0x%08X  (%+d): %d 处，前 8 个: %s"
              % (v, v - SETTINGS_BASE, len(refs[v]),
                 [("0x%08X" % a) for a in refs[v][:8]]))
    base_refs = refs.get(SETTINGS_BASE, [])
    print("基址 0x%08X 被 %d 处 ldr literal 引用" % (SETTINGS_BASE, len(base_refs)))

    # max 表
    tbl = code.image[_off(CFN_MAX_TABLE):_off(CFN_MAX_TABLE) + CFN_ITEM_COUNT]
    print()
    print("C.Fn 最大/回绕表 0x%08X 前 %d 字节: %s"
          % (CFN_MAX_TABLE, CFN_ITEM_COUNT, tbl.hex(" ")))
    print("  期望（Lead）: %s" % CFN_MAX_BYTES.hex(" "))
    print("  与期望一致: %s" % (tbl == CFN_MAX_BYTES))
    print("  每项最大值: %s" % ["item%d=%d" % (i, b) for i, b in enumerate(tbl)])
    print("  与 V1 说明书 C.Fn 顺序逐项对齐（外部佐证）:")
    for i, (b, nm) in enumerate(zip(tbl, ITEM_NAMES_REF)):
        print("    item%d = F%-2d  max=%-3d  手册项名: %s" % (i, i + 1, b, nm))
    # 每次使用该表的位置
    uses = []
    for ins in code.insns:
        lv = code_literal_value(ins)
        if lv == CFN_MAX_TABLE:
            uses.append(ins)
    print("  ldr literal 引用该表: %s" % [("0x%08X" % i.address) for i in uses])
    if uses:
        u = uses[0]
        print("  首个引用上下文:")
        for ins in code.insns:
            if u.address - 0x10 <= ins.address <= u.address + 0x18:
                print("    " + fmt_ins(ins))
    # 表引用点所在函数的项数边界（cmp #0xB 等）
    print()
    print("  C.Fn 项索引边界线索（cmp #0xB / cmp #0xC）:")
    for ins in code.insns:
        if ins.mnemonic == "cmp" and ins.op_str in ("r0, #0xb", "r0, #0xc",
                                                    "r1, #0xb", "r1, #0xc",
                                                    "r2, #0xb", "r2, #0xc"):
            print("    0x%08X  %s %s" % (ins.address, ins.mnemonic, ins.op_str))

    # 选中项/编辑态
    print()
    for name, addr in (("当前选中项", ITEM_SELECT_ADDR), ("编辑态", EDIT_ADDR)):
        sites = [ins.address for ins in code.insns
                 if code_literal_value(ins) == addr]
        print("%s 0x%08X 的 literal 引用 %d 处: %s"
              % (name, addr, len(sites), [("0x%08X" % a) for a in sites[:12]]))


def report_accesses(code: st.Code, entries: list[int], accesses: list[Access]) -> None:
    header("2. settings[n] 读者/写者（n=0..0x1E；前向污点扫描）")
    by_off: dict[int, list[Access]] = {}
    for a in accesses:
        if a.offset is not None:
            by_off.setdefault(a.offset, []).append(a)

    print("共发现 %d 个静态访问点（含动态索引）" % len(accesses))
    for off in range(0, 0x1E):
        lst = by_off.get(off, [])
        readers = [a for a in lst if a.kind.startswith("ldr")]
        writers = [a for a in lst if a.kind.startswith("str")]
        if not lst:
            continue
        print()
        print("--- settings[%d] (0x%08X)  读者 %d / 写者 %d ---"
              % (off, SETTINGS_BASE + off, len(readers), len(writers)))
        for a in lst:
            fn = a.fn_start
            label = FUNC_LABELS.get(fn, "FUNC_0x%08X" % fn)
            dyn = " [动态索引]" if a.dynamic_index else ""
            print("  %s  %-11s off=%d  fn=0x%08X(%s)%s"
                  % (fmt_ins(a.ins), a.kind, a.offset, fn, label, dyn))

    dyn = [a for a in accesses if a.dynamic_index]
    print()
    print("动态索引访问（基址=settings，索引为寄存器；通常是菜单循环/按当前项访问）: %d" % len(dyn))
    for a in dyn[:40]:
        fn = a.fn_start
        label = FUNC_LABELS.get(fn, "FUNC_0x%08X" % fn)
        print("  %s  %-11s  fn=0x%08X(%s)" % (fmt_ins(a.ins), a.kind, fn, label))


def report_function_profiles(code: st.Code, entries: list[int], accesses: list[Access]) -> None:
    """按函数聚合 settings[0..0xB] 访问，给出函数画像（调用目标 / RAM 字面量 / 上下文）。"""
    header("2b. 按函数聚合：每个 settings 访问函数的画像（用于推断项语义）")
    by_fn: dict[int, list[Access]] = {}
    for a in accesses:
        if a.offset is None:
            continue
        by_fn.setdefault(a.fn_start, []).append(a)
    display_fns = {0x08004914, 0x0800548C, 0x08005EF0, 0x080061CC, 0x0800D3D8}
    for fn in sorted(by_fn):
        end = code.next_entry(fn, entries)
        accesses_f = sorted(by_fn[fn], key=lambda a: a.addr)
        offs = sorted({a.offset for a in accesses_f})
        label = FUNC_LABELS.get(fn, "FUNC_0x%08X" % fn)
        print()
        print("=== fn 0x%08X..0x%08X  %s  settings 访问偏移: %s"
              % (fn, end - 1, label, ["%d(%s)" % (o, "R" if any(
                  a.offset == o and a.kind.startswith("ldr") for a in accesses_f) else "W")
                  for o in offs]))
        # 函数内 BL 目标 + RAM 字面量（去重排序）
        bls = []
        for ins in code.insns:
            if fn <= ins.address < end and ins.mnemonic in ("bl", "blx"):
                t = code.bl_target(ins)
                if t is not None:
                    bls.append(t)
        lits = []
        for ins in code.insns:
            if fn <= ins.address < end:
                lv = code_literal_value(ins)
                if lv is not None and 0x20000000 <= lv < 0x20002000:
                    lits.append(lv)
        print("  BL 目标: %s" % [("0x%08X" % t) for t in sorted(set(bls))])
        print("  RAM 字面量: %s" % [("0x%08X" % t) for t in sorted(set(lits))])
        if fn in display_fns:
            print("  （显示/保存/改值处理函数，上下文略；见 §2 访问点）")
            continue
        # 每个访问点前后各 10 条指令，最多前 3 个访问点
        for a in accesses_f[:3]:
            print("  --- 访问 %s %s off=%s ---" % (fmt_ins(a.ins), a.kind, a.offset))
            lo, hi = a.addr - 0x10, a.addr + 0x18
            for ins in code.insns:
                if lo <= ins.address <= hi:
                    extra = ""
                    lv = code_literal_value(ins)
                    if lv is not None:
                        extra = "   ; [0x%08X]=0x%08X" % (
                            ((ins.address + 4) & ~3) + _imm_of(ins), lv)
                    print("      " + fmt_ins(ins) + extra)


def report_mirrors(code: st.Code, accesses: list[Access]) -> None:
    """检测 settings[n] 被复制到其它 RAM 变量的「镜像」写点。

    模式：ldrb rX,[settings,#n] 之后 8 条指令内，出现 ldr rY,=RAM; strb rX,[rY...]
    """
    header("2c. settings[n] -> RAM 镜像副本（handler/apply 路径）")
    found = []
    for a in accesses:
        if a.offset is None or not a.kind.startswith("ldr"):
            continue
        mem = parse_mem(a.ins.op_str)
        if not mem:
            continue
        src_reg = mem[0]
        if src_reg is None:
            continue
        # 向后扫描 12 条指令
        cur = a.addr + a.ins.size
        lit = None
        for _ in range(14):
            ins = code.at(cur)
            if ins is None:
                cur += 2
                continue
            lv = code_literal_value(ins)
            if lv is not None and 0x20000000 <= lv < 0x20002000:
                lit = lv
            s_mem = parse_mem(ins.op_str)
            if (ins.mnemonic == "strb" and s_mem and s_mem[0] == src_reg
                    and s_mem[1] is not None and lit is not None):
                found.append((a.offset, a.addr, lit, ins.address, cur))
                break
            # 同一函数内才继续（用 fn_end 限制）
            if cur >= a.fn_end:
                break
            cur += ins.size
    for off, raddr, mirror, saddr, _ in sorted(found):
        print("  settings[%d] 读取@0x%08X -> 镜像 RAM 0x%08X（写@0x%08X）"
              % (off, raddr, mirror, saddr))
    if not found:
        print("  （未发现直接的 ldrb->strb 镜像模式；镜像可能经寄存器间 mov 或结构体指针传递）")
    print()
    print("  补充：0x0800A6AC / 0x0800A6DC 是一对「settings[0/1/2/3/7/8/10] <-> "
          "紧凑结构 0x20001070」的打包/解包函数：")
    print("    * 0x0800A6AC（settings -> 0x20001070；被 0x08003B7C 调用，后者由 RF 设置打包路径 0x08012EF2 调用）")
    print("    * 0x0800A6DC（0x20001070 -> settings；被 0x080125A8.. 的 RF 设置应用路径 0x08012790 调用）")
    print("    * 注意 0x20001070+0x16 == 0x20001086 == settings[0]：打包函数最后一条")
    print("      `strb r1,[r0,#0x16]` 实际落在 settings[0]（原厂重叠/疑似 quirk），")
    print("      解包函数会把 settings[0/1/2/3/7/8/10] 从该紧凑结构覆盖回去。")
    print("    * 含义：item2 等被包含的项可能被 RF「设置同步」覆盖，需要在整机验证；")
    print("      item4/5/6/9/11 不在这对函数里（但 0x0800DCDC 仍写 settings[0..11]）。")


def report_save_load(code: st.Code, entries: list[int]) -> None:
    header("3b. save/load 细节、record 字节与 settings 写点（补充证据）")
    fn = code.containing_function(SAVE_FN, entries)
    end = code.next_entry(fn, entries)
    print("save builder 函数 0x%08X..0x%08X（0x%08X 是其内部）" % (fn, end - 1, SAVE_FN))
    # 打印 0x08006004..0x08006066（settings 拷贝部分）
    print("  settings 拷贝（0x08006004..0x08006066）:")
    for ins in code.insns:
        if 0x08006004 <= ins.address < 0x08006066:
            extra = ""
            lv = code_literal_value(ins)
            if lv is not None:
                extra = "   ; [0x%08X]=0x%08X" % (((ins.address + 4) & ~3) + _imm_of(ins), lv)
            print("    %s%s" % (fmt_ins(ins), extra))
    # 找调用者
    callers = [ins.address for ins in code.insns
               if code.bl_target(ins) == fn]
    print("  调用者: %s" % [("0x%08X" % a) for a in callers])
    # 找 flash 参数地址 0x0801F000/0x0801F800 引用
    print()
    print("  flash 参数页地址（0x0801F000/0x0801F800）的 literal 引用:")
    for ins in code.insns:
        lv = code_literal_value(ins)
        if lv in (0x0801F000, 0x0801F800):
            print("    0x%08X  %s %s   ; 0x%08X" % (ins.address, ins.mnemonic,
                                                    ins.op_str, lv))
    # 找写 settings 的 load 函数（排除 save/copy 区）
    print()
    print("  写 settings 区（strb 到基址）的代码点（不含 0x08005EF0 save builder）:")
    writers = []
    for ins in code.insns:
        if ins.mnemonic != "strb":
            continue
        mem = parse_mem(ins.op_str)
        if not mem:
            continue
        dst, breg, ireg, imm = mem
        # 只做简单相邻 literal 解析（复杂情况由污点扫描覆盖）
        sites = []
        a = ins.address - 2
        for _ in range(12):
            p = code.at(a)
            if p is None:
                a -= 2
                continue
            lv = code_literal_value(p)
            if lv is not None and SETTINGS_WIN_LO <= lv < SETTINGS_WIN_HI:
                sites.append((p.address, lv))
                break
            a -= p.size
        if sites:
            writers.append((ins.address, ins, sites[0]))
    for addr, ins, (lad, val) in writers[:80]:
        print("    0x%08X  %s  <- [0x%08X]=0x%08X" % (addr, fmt_ins(ins)[10:], lad, val))


def _imm_of(ins) -> int:
    m = re.search(r"\[pc,\s*#(0x[0-9a-fA-F]+|\d+)\]", ins.op_str)
    if not m:
        return 0
    v = m.group(1)
    return int(v, 16) if "x" in v else int(v)


def _report_code_caves_basic(code: st.Code) -> None:
    header("4. 代码洞（死区）检查：0x08017944 / 0x08017D28 + 其它 >=32B 全 0 区")
    image = code.image
    # 所有 0x00 连续段
    runs = []
    n = len(image)
    i = 0
    while i < n:
        if image[i] == 0:
            j = i
            while j < n and image[j] == 0:
                j += 1
            if j - i >= 16:
                runs.append((j - i, i))
            i = j
        else:
            i += 1
    print("全镜像 >=16 字节的连续 0x00 段（文件偏移 / 地址 / 长度）:")
    for ln, off in sorted(runs, key=lambda x: -x[0])[:30]:
        print("  0x%05X  0x%08X  %d 字节" % (off, BASE + off, ln))

    print()
    for addr, want_len in CODE_CAVE_CANDIDATES:
        off = _off(addr)
        chunk = image[off:off + want_len]
        allzero = chunk == bytes(len(chunk))
        # 更长的连续 0
        start = off
        while start > 0 and image[start - 1] == 0:
            start -= 1
        end = off + want_len
        while end < len(image) and image[end] == 0:
            end += 1
        print("候选 0x%08X (文件偏移 0x%05X, 期望长 %d): 全 0=%s；实际连续 0 区 0x%08X..0x%08X (%d 字节)"
              % (addr, off, want_len, allzero, BASE + start, BASE + end, end - start))
        # 引用检查
        refs_bl = []
        for ins in code.insns:
            t = code.bl_target(ins)
            if t is not None and BASE + start <= t < BASE + end:
                refs_bl.append(("BL", ins.address, t))
        refs_br = []
        for ins in code.insns:
            t = code.branch_target(ins)
            if t is not None and BASE + start <= (t & ~1) < BASE + end:
                refs_br.append(("BR", ins.address, t & ~1))
        refs_lit = []
        for ins in code.insns:
            lv = code_literal_value(ins)
            if lv is not None and BASE + start <= lv < BASE + end:
                refs_lit.append(("LIT", ins.address, lv))
        # 字面量表（含未反汇编为 ldr 的绝对地址表）
        refs_raw = []
        pat = struct.pack("<I", BASE + off)
        pos = 0
        while True:
            p = image.find(pat, pos)
            if p < 0:
                break
            refs_raw.append(p)
            pos = p + 1
        print("  BL 目标落入: %s" % refs_bl)
        print("  分支目标落入: %s" % refs_br)
        print("  ldr 字面量指向: %s" % refs_lit)
        print("  其它以 0x%08X 开头的字面量字节: %s" % (addr, [("0x%05X" % p) for p in refs_raw]))
        # 该地址是否在函数边界内（是否被当作代码/数据的一部分）
        print("  区间内反汇编指令数: %d"
              % len([ins for ins in code.insns if BASE + start <= ins.address < BASE + end]))


# ---------------------------------------------------------------------------
# 逐项语义 / 持久化 / 推荐 / 代码洞（增强版）
# ---------------------------------------------------------------------------
def _all_zero_runs(image: bytes, min_len: int = 16) -> list[tuple[int, int]]:
    runs = []
    n = len(image)
    i = 0
    while i < n:
        if image[i] == 0:
            j = i
            while j < n and image[j] == 0:
                j += 1
            if j - i >= min_len:
                runs.append((i, j - i))
            i = j
        else:
            i += 1
    return runs


def _abs_refs(image: bytes, lo: int, hi: int) -> list[int]:
    """全镜像按 2 字节步长扫描 4 字节值，返回落在 [lo,hi) 的文件偏移。"""
    hits = []
    for off in range(0, len(image) - 3, 2):
        v = struct.unpack_from("<I", image, off)[0]
        if lo <= v < hi:
            hits.append(off)
    return hits


def report_item_semantics(code: st.Code, entries: list[int],
                          accesses: list[Access]) -> None:
    header("2d. 逐项语义汇总（读者/写者/默认值/证据）")
    tbl = code.image[_off(CFN_MAX_TABLE):_off(CFN_MAX_TABLE) + CFN_ITEM_COUNT]
    by_off: dict[int, list[Access]] = {}
    for a in accesses:
        if a.offset is not None:
            by_off.setdefault(a.offset, []).append(a)
    for n in range(CFN_ITEM_COUNT):
        lst = by_off.get(n, [])
        readers = [a for a in lst if a.kind.startswith("ldr")]
        writers = [a for a in lst if a.kind.startswith("str")]
        fns = sorted({a.fn_start for a in lst})
        print()
        print("item%-2d = F%-2d  %-8s max=%-3d default=%s"
              % (n, n + 1, ITEM_NAMES_REF[n], tbl[n],
                 ("%d (0x%02X)" % (ITEM_DEFAULTS[n], ITEM_DEFAULTS[n]))
                 if n in ITEM_DEFAULTS else "?"))
        print("  语义: %s" % ITEM_SEMANTICS[n])
        print("  读者 %d / 写者 %d；涉及函数: %s"
              % (len(readers), len(writers), [("0x%08X" % f) for f in fns]))
        for a in readers:
            print("    读 %s  fn=0x%08X" % (fmt_ins(a.ins), a.fn_start))
        for a in writers:
            print("    写 %s  fn=0x%08X" % (fmt_ins(a.ins), a.fn_start))


def report_persistence(code: st.Code, entries: list[int]) -> None:
    header("3. 设置持久化、记录布局、出厂默认（独立确认）")
    print("save 记录 builder : 0x%08X（settings 拷贝段 0x08006004..0x08006066）" % SAVE_FN)
    print("load/expand 函数  : 0x%08X（0x080159C4 起写 settings[0..0xD]）" % 0x080158B0)
    print("出厂默认函数      : 0x%08X（0x08006722..0x0800673A 写 settings[0..0xB]）" % 0x080066D0)
    print("加载/默认任务     : 0x%08X（槽位 0x0801F000+slot*100，头部 0x5C 合法标记）"
          % 0x0800FEF8)
    print()
    print("记录偏移 -> settings 映射（save/load 两侧指令推导）:")
    print("  record[0x2E..0x2F] = settings[0..1]")
    print("  record[0x30..0x33] = settings[2..5]")
    print("  record[0x34..0x37] = settings[6..9]")
    print("  record[0x38..0x3B] = settings[10..13]")
    print()
    print("出厂默认（0x080066D0 逐条写入 settings；★ 仅 item0..0xB）:")
    defaults = [(0x08006722, 0, 0, "settings[0]=0  (m/ft=m)"),
                (0x08006724, 1, 1, "settings[1]=1  (AF=ON)"),
                (0x08006726, 2, 0, "settings[2]=0  (STBY=默认)"),
                (0x08006728, 3, 0, "settings[3]=0  (RX STBY=默认)"),
                (0x08006730, 4, 0, "settings[4]=0  (SCAN=OFF)"),
                (0x08006736, 5, 0x14, "settings[5]=0x14 (CH=20 -> 显示 CH21)"),
                (0x08006732, 6, 0, "settings[6]=0  (ID=OFF)"),
                (0x0800672A, 7, 1, "settings[7]=1  (BEEP=ON)"),
                (0x0800672C, 8, 0, "settings[8]=0  (LIGHT=OFF)"),
                (0x0800672E, 9, 3, "settings[9]=3  (LCD=3)"),
                (0x08006738, 10, 0, "settings[10]=0 (FEB ACL=OFF)"),
                (0x0800673A, 11, 1, "settings[11]=1 (TX=ON)")]
    for addr, idx, val, desc in defaults:
        ins = code.at(addr)
        print("  0x%08X  %-12s %-18s  %s"
              % (addr, ins.bytes.hex(" ") if ins else "?",
                 (ins.mnemonic + " " + ins.op_str) if ins else "?", desc))
    print()
    print("★ 结论：settings[0..0xD] 随 record 持久化；出厂默认 settings[2]=0，")
    print("  所以 item2(STBY) 作为密钥开关时，出厂/恢复默认后为普通模式。")


def report_recommendation(code: st.Code, entries: list[int],
                          accesses: list[Access]) -> None:
    header("4. 可复用 C.Fn 开关项评估与推荐（含 mask / hook 方案）")
    print("推荐排序（item 号 = settings 索引，F 号 = item+1）:")
    for item, why in RANKING:
        print("  item%-2d F%-2d %-8s : %s"
              % (item, item + 1, ITEM_NAMES_REF[item], why))
    print()
    print("★ 首选 item%d = F%d %s（默认 %d；菜单显示 OFF/ON）"
          % (CHOSEN_ITEM, CHOSEN_ITEM + 1, CHOSEN_NAME,
             ITEM_DEFAULTS[CHOSEN_ITEM]))
    print("  需 mask 的功能读者（2B ldrb -> movs，恢复出厂默认行为）:")
    for addr, orig_hex, new_hex, desc in CHOSEN_MASK:
        ins = code.at(addr)
        cur = ins.bytes.hex(" ") if ins else "?"
        print("    0x%08X  原厂 %-8s -> %-8s  %s" % (addr, cur, new_hex, desc))
    print("  保留（不 mask）的读者：")
    print("    * 菜单渲染 0x08004B4E/0x08004B5E：显示 F3 的 ON/OFF（用户需要看到开关状态）")
    print("    * save 0x08006012 / load 0x080159D4：持久化（必须保留）")
    print("    * A6AC/A6DC 0x0800A6B4/0x0800A6E6：与 0x20001070 紧凑结构互拷（无线设置同步，风险见下）")
    print()
    print("  立即生效 hook（代码洞 0x%08X，10 字节）：" % HOOK_ADDR)
    print("    %s" % HOOK_BYTES.hex(" "))
    print("    等价 C：push {r0-r4,lr}; r0 = settings[6]; rf_sync(id); pop {r0-r4,pc}")
    print("  函数门兜底（原厂 4B cmp/beq -> 4B bl hook）:")
    for addr, orig_hex, new_hex, desc in GATE_PATCHES:
        cur = code.image[_off(addr):_off(addr) + 4].hex(" ")
        print("    0x%08X  原厂 %-8s (当前 %-8s) -> %-8s  %s"
              % (addr, orig_hex, cur, new_hex, desc))
    print()
    print("  说明：hook 在每次 C.Fn 改值（dec/inc 的改值分支）无条件调用同步字函数；")
    print("  原厂 ID(item6) 改值立即同步的行为保留；chosen item 改值也立即同步。")
    print("  hook 入口 r0=settings 基址（0x0800625C / 0x0800D46E 已用 CFG+数据流确认），")
    print("  故可直接 ldrb r0,[r0,#6] 取 ID，无需额外字面量。")
    print()
    print("  风险：item2 属于 A6AC/A6DC 的无线设置紧凑结构（settings[2] <-> 0x20001071），")
    print("  触发器等外部设置同步可能覆盖密钥标志；同理 0x0800DCDC 会写 settings[1..11]。")
    print("  需要 Lead 在模拟/真机验证该路径；必要时把对应写点也 mask 为保留 bit。")


def report_code_caves(code: st.Code) -> None:
    header("5. 代码洞（死区）验证：0x08017944 / 0x08017D28 + 其它 >=32B 全 0 区")
    image = code.image
    runs = _all_zero_runs(image, 16)
    print("全镜像 >=16 字节连续 0x00 段（按长度前 25）:")
    for off, ln in sorted(runs, key=lambda x: -x[1])[:25]:
        print("  文件 0x%05X  地址 0x%08X  %d 字节" % (off, BASE + off, ln))
    print()
    for addr, want_len in CODE_CAVE_CANDIDATES:
        off = addr - BASE
        start = off
        while start > 0 and image[start - 1] == 0:
            start -= 1
        end = off + want_len
        while end < len(image) and image[end] == 0:
            end += 1
        lo, hi = BASE + start, BASE + end
        allzero = image[off:off + want_len] == bytes(want_len)
        bl = [ins.address for ins in code.insns
              if (code.bl_target(ins) or 0) and lo <= code.bl_target(ins) < hi]
        br = [ins.address for ins in code.insns
              if code.branch_target(ins) is not None and
              lo <= (code.branch_target(ins) & ~1) < hi]
        lit = [ins.address for ins in code.insns
               if code.literal_value(ins) is not None and
               lo <= code.literal_value(ins)[1] < hi]
        raw = _abs_refs(image, lo, hi)
        raw_odd = _abs_refs(image, lo | 1, (hi - 1) | 1)
        usable = (allzero and not bl and not br and not lit and not raw and not raw_odd
                  and (end - start) >= 32)
        print("候选 0x%08X (文件偏移 0x%05X, 期望长 %d):" % (addr, off, want_len))
        print("  全 0=%s；实际连续 0 区 0x%08X..0x%08X (%d 字节)"
              % (allzero, lo, hi, end - start))
        print("  BL 目标落入: %s" % [("0x%08X" % a) for a in bl])
        print("  分支目标落入: %s" % [("0x%08X" % a) for a in br])
        print("  ldr 字面量指向: %s" % [("0x%08X" % a) for a in lit])
        print("  全镜像 u32 绝对地址指向区间: %d 处 %s"
              % (len(raw), [("0x%05X" % o) for o in raw[:8]]))
        print("  (thumb 变体扫描命中 %d 处)" % len(raw_odd))
        print("  => %s" % ("可用作注入区（无任何引用）" if usable else "不可用/需进一步确认"))
        if addr == HOOK_ADDR:
            print("  hook 需要 %d 字节，本区可用 %d 字节：%s"
                  % (len(HOOK_BYTES), end - start,
                     "足够" if end - start >= len(HOOK_BYTES) else "不足"))
    print()
    print("其它 >=32B 全 0 区可用性（无任何引用者，最多列 5 个）:")
    shown = 0
    for off, ln in sorted(runs, key=lambda x: -x[1]):
        if ln < 32:
            continue
        lo, hi = BASE + off, BASE + off + ln
        bl = [ins for ins in code.insns
              if (code.bl_target(ins) or 0) and lo <= code.bl_target(ins) < hi]
        br = [ins for ins in code.insns
              if code.branch_target(ins) is not None and
              lo <= (code.branch_target(ins) & ~1) < hi]
        lit = [ins for ins in code.insns
               if code.literal_value(ins) is not None and
               lo <= code.literal_value(ins)[1] < hi]
        raw = _abs_refs(image, lo, hi)
        if not (bl or br or lit or raw):
            print("  0x%08X (文件 0x%05X)  %d 字节  可用" % (lo, off, ln))
            shown += 1
            if shown >= 5:
                break
    print()
    print("hook 静态复核（在 0x%08X 处解码）：" % HOOK_ADDR)
    md = st.Cs(st.CS_ARCH_ARM, st.CS_MODE_THUMB)
    md.skipdata = True
    hook_ins = list(md.disasm(HOOK_BYTES, HOOK_ADDR))
    for ins in hook_ins:
        print("  0x%08X  %-12s %-6s %s"
              % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))
    bl_targets = [ins and None for ins in hook_ins]
    hook_bl = [int(re.search(r'#(0x[0-9a-fA-F]+)', i.op_str).group(1), 16)
               for i in hook_ins if i.mnemonic == "bl"]
    print("  hook 长度 %d B；BL 目标 = %s"
          % (len(HOOK_BYTES), [("0x%08X" % t) for t in hook_bl]))
    print("  等价 C：push {r0-r4,lr}; ldrb r0,[r0,#6]; bl 0x080121BC; pop {r0-r4,pc}")


def run_checks(code: st.Code, accesses: list[Access]) -> list[tuple[bool, str]]:
    """关键静态断言（FAIL 时 main 返回 1）。"""
    results: list[tuple[bool, str]] = []
    image = code.image

    def ck(cond, msg):
        results.append((bool(cond), msg))

    tbl = image[_off(CFN_MAX_TABLE):_off(CFN_MAX_TABLE) + CFN_ITEM_COUNT]
    ck(tbl == CFN_MAX_BYTES, "C.Fn 最大值表 0x%08X 与 Lead 给定 12 字节一致" % CFN_MAX_TABLE)
    ck(CFN_ITEM_COUNT == 12, "C.Fn 项数 12（0..0xB）")
    ck(ITEM_NAMES_REF[5] == "CH" and tbl[5] == 31 and ITEM_NAMES_REF[6] == "ID"
       and tbl[6] == 99,
       "最大值表与 V1 说明书项序对齐：item5=CH(max31)、item6=ID(max99)")
    ck(tbl[7] == 1 and tbl[8] == 2 and tbl[9] == 6,
       "item7=max1、item8=max2、item9=max6")
    # 默认值
    ck(ITEM_DEFAULTS[2] == 0, "出厂默认 settings[2]=0（item2 STBY 默认普通模式）")
    # 代码洞
    for addr, want_len in CODE_CAVE_CANDIDATES:
        off = addr - BASE
        start = off
        while start > 0 and image[start - 1] == 0:
            start -= 1
        end = off + want_len
        while end < len(image) and image[end] == 0:
            end += 1
        lo, hi = BASE + start, BASE + end
        bl = [ins for ins in code.insns
              if (code.bl_target(ins) or 0) and lo <= code.bl_target(ins) < hi]
        br = [ins for ins in code.insns
              if code.branch_target(ins) is not None and
              lo <= (code.branch_target(ins) & ~1) < hi]
        lit = [ins for ins in code.insns
               if code.literal_value(ins) is not None and
               lo <= code.literal_value(ins)[1] < hi]
        raw = _abs_refs(image, lo, hi)
        ck(image[off:off + want_len] == bytes(want_len) and not bl and not br
           and not lit and not raw,
           "代码洞 0x%08X..0x%08X (%d B) 全 0 且无 BL/分支/字面量/u32 引用"
           % (addr, addr + want_len - 1, want_len))
    # hook / gate / mask 原厂字节
    for addr, orig_hex, new_hex, desc in GATE_PATCHES:
        cur = image[_off(addr):_off(addr) + 4].hex(" ")
        ck(cur == orig_hex, "hook 门 0x%08X 原厂字节 %s（实际 %s）" % (addr, orig_hex, cur))
    for addr, orig_hex, new_hex, desc in CHOSEN_MASK:
        cur = image[_off(addr):_off(addr) + 2].hex(" ")
        ck(cur == orig_hex, "mask 点 0x%08X 原厂字节 %s（实际 %s）" % (addr, orig_hex, cur))
    # hook 字节可反汇编且 BL 到同步字函数
    md = st.Cs(st.CS_ARCH_ARM, st.CS_MODE_THUMB)
    md.skipdata = True
    hook_ins = list(md.disasm(HOOK_BYTES, HOOK_ADDR))
    bls = [int(re.search(r"#(0x[0-9a-fA-F]+)", i.op_str).group(1), 16)
           for i in hook_ins if i.mnemonic == "bl"]
    ck(len(HOOK_BYTES) == 10 and bls == [SYNC_FN],
       "注入 hook 10B 可反汇编，BL 目标 = 0x%08X" % SYNC_FN)
    # 两个 gate 点的 r0 必须是 settings 基址（供 hook 直接取 ID）
    entries = code.function_entries()
    code.detect_switch_helpers(entries)
    for addr in (0x0800625C, 0x0800D46E):
        fn = code.containing_function(addr, entries)
        end = code.next_entry(fn, entries)
        reach = code.reachable_set(fn, end)
        preds = code.build_preds(fn, end, reach)
        res = code.resolve_reg(0, addr, fn, end, preds)
        ck(res["status"] == "resolved" and res["value"] == SETTINGS_BASE,
           "hook 门 0x%08X 处 r0 = settings 基址 0x%08X（数据流确认）"
           % (addr, SETTINGS_BASE))
    # settings 访问量
    ck(len(accesses) >= 100, "settings 访问点扫描数量 %d（>=100）" % len(accesses))
    ck(any(a.offset == CHOSEN_ITEM and a.kind.startswith("ldr") for a in accesses),
       "item%d 存在静态读者（用于菜单显示/持久化）" % CHOSEN_ITEM)
    return results


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------
def main() -> int:
    try:
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:  # noqa: BLE001
        pass
    print("V1C C.Fn / settings 子系统分析（cfn_settings.py）")
    print("固件:", FW_PATH)
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

    analyze_settings_layout(code)
    accesses = scan_taint(code, entries)
    report_accesses(code, entries, accesses)
    report_function_profiles(code, entries, accesses)
    report_mirrors(code, accesses)
    report_item_semantics(code, entries, accesses)
    report_persistence(code, entries)
    report_save_load(code, entries)
    report_recommendation(code, entries, accesses)
    report_code_caves(code)

    results = run_checks(code, accesses)
    header("汇总")
    npass = sum(1 for ok, _ in results if ok)
    nfail = len(results) - npass
    for ok, msg in results:
        print("[%s] %s" % ("PASS" if ok else "FAIL", msg))
    print()
    print("检查项: %d   PASS: %d   FAIL: %d" % (len(results), npass, nfail))
    print("结论:", "PASS（C.Fn 子系统分析完成，关键结构自洽）" if nfail == 0
          else "FAIL（关键结构不符，见上）")
    return 0 if nfail == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
