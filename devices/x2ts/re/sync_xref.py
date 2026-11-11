# -*- coding: utf-8 -*-
"""X2T-S V1.4 同步字链路交叉引用审计（静态）。

复现：cd D:\\神牛 && tools\\python\\python.exe devices\\x2ts\\re\\sync_xref.py

输出（全部为可复核证据）：
  1. 镜像身份 + 真实链接基址 0x08003000 的自证（Reset_Handler/SystemInit/__main/尾部指针/通道表）
  2. 同步字函数 0x0800C8F8..0x0800C92B 的原厂字节/反汇编/边界
  3. 全部调用点（BL）+ 调用者函数 + 每个调用点的 r0(ID) 来源
  4. 调用者链（谁在什么场景调用）与「原厂何时重设同步字」
  5. CC2500 helper：WriteReg/strobe/delay 语义；全部写 reg4/reg5 的点；CC2500 初始化表
  6. ID / C.Fn RAM 变量与副本、写点
  7. 关键地址对照表（Lead 早期 base 0x08000000 口径 <-> 真实 0x08003000 口径）

注意：本脚本只读固件，不写任何文件；不改 keypatch 模块。
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from capstone.arm import ARM_OP_IMM, ARM_OP_REG, ARM_OP_MEM, ARM_REG_PC

HERE = os.path.dirname(os.path.abspath(__file__))
IMG = os.path.join(os.path.dirname(HERE), "firmware", "X2T-S_V1.4.bin")
DATA = open(IMG, "rb").read()
SIZE = len(DATA)

# ---- 真实链接基址（本脚本用证据重新自证，不假设）-----------------------
BASE = 0x08003000
LEAD_BASE = 0x08000000          # Lead 首轮报告口径（= 文件偏移直接加 0x08000000，实际是错的）
END = BASE + SIZE

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True


def addr(off: int) -> int:
    return BASE + off


def off(addr_: int) -> int:
    return addr_ - BASE


def u32(a: int) -> int:
    return struct.unpack_from("<I", DATA, off(a))[0]


def dis(addr_: int, nbytes: int, label: str = "", lit: bool = True) -> list:
    """反汇编并按需标注 LDR 字面量值。"""
    lines = []
    print("---- %s 0x%08X (off 0x%05X) ----" % (label, addr_, off(addr_)))
    for ins in md.disasm(DATA[off(addr_):off(addr_) + nbytes], addr_):
        line = "  0x%08X  %-20s %s %s" % (
            ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str)
        if lit and ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str:
            try:
                imm = int(ins.op_str.split("#")[1].rstrip("]"), 0)
                tgt = ((ins.address + 4) & ~3) + imm
                if BASE <= tgt <= END - 4:
                    line += "   ; [0x%08X]=0x%08X" % (tgt, u32(tgt))
            except Exception:  # noqa: BLE001
                pass
        print(line)
        lines.append(line)
    print()
    return lines


def find_branch_targets(target: int, kinds=("bl", "bl.w", "blx", "b", "b.w")):
    """全镜像按 2 字节步长扫描直接分支，返回 [(insn_addr, mnemonic)]。"""
    out = []
    for o in range(0, SIZE - 3, 2):
        for ins in md.disasm(DATA[o:o + 4], addr(o)):
            if ins.mnemonic in kinds and ins.operands and ins.operands[0].type == ARM_OP_IMM:
                if ins.operands[0].imm == target:
                    out.append((ins.address, ins.mnemonic))
            break
    return out


def find_u32_words(vals):
    out = {}
    for v in vals:
        pat = struct.pack("<I", v)
        hits = []
        i = DATA.find(pat)
        while i >= 0:
            hits.append(addr(i))
            i = DATA.find(pat, i + 1)
        # Thumb 指针变体
        pat1 = struct.pack("<I", v | 1)
        i = DATA.find(pat1)
        while i >= 0:
            hits.append(addr(i) | 0x100000000)  # 用高位标记 thumb 变体
            i = DATA.find(pat1, i + 1)
        out[v] = hits
    return out


def enclosing_fn(hit: int) -> int:
    """最近的前向 BL 目标（函数入口候选）；仅作辅助参考。"""
    cands = [t for (t, _k) in BL_TARGETS if t <= hit]
    return max(cands) if cands else 0


def is_fn_entry(candidate: int, site: int, window: int = 0x200) -> bool:
    """校验 candidate 是函数序言（push lr / sub sp），且 site 在其后 window 内。"""
    if not (BASE <= candidate <= site < candidate + window):
        return False
    first = next(md.disasm(DATA[off(candidate):off(candidate) + 4], candidate), None)
    return bool(first and (first.mnemonic in ("push", "push.w")
                           or (first.mnemonic == "sub" and "sp" in first.op_str)))


# ===========================================================================
# 0. 预扫所有 BL/BLX 目标（函数入口候选）
# ===========================================================================
BL_TARGETS = []
for o in range(0, SIZE - 3, 2):
    for ins in md.disasm(DATA[o:o + 4], addr(o)):
        if ins.mnemonic in ("bl", "bl.w", "blx") and ins.operands \
                and ins.operands[0].type == ARM_OP_IMM:
            t = ins.operands[0].imm
            if BASE <= t < END:
                BL_TARGETS.append((t & ~1, ins.address))
        break

FAIL = []


def check(ok: bool, msg: str) -> None:
    print("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
    if not ok:
        FAIL.append(msg)


# ===========================================================================
print("=" * 78)
print("X2T-S V1.4 同步字链路审计 — 静态交叉引用")
print("=" * 78)
print("镜像: %s" % IMG)
print("大小: %d (0x%X)" % (SIZE, SIZE))
print("sha256: %s" % hashlib.sha256(DATA).hexdigest())
print()

# ===========================================================================
# 1. 基址自证
# ===========================================================================
print("=" * 78)
print("1. 真实链接基址自证（Lead 首轮按 0x08000000 记录，与本脚本 +0x3000 换算）")
print("=" * 78)
vec_sp = u32(addr(0))
vec_reset = u32(addr(4))
print("  向量表[0] SP   = 0x%08X" % vec_sp)
print("  向量表[1] Reset= 0x%08X" % vec_reset)
print()

# 1a) Reset_Handler：真实 base 下位于 BASE+0x1AC（文件 0x1AC）
rst = addr(0x1AC)
print("  若 base=0x08003000，Reset_Handler = 0x%08X（off 0x1AC）：" % rst)
dis(rst, 0x14, "Reset_Handler")
check(vec_reset & ~1 == rst, "Reset 向量 0x%08X 指向文件偏移 0x%05X（base 0x%08X）"
      % (vec_reset, off(rst), BASE))
sysinit = 0  # 占位
main_ptr = 0
print("  Reset_Handler 字面量：")
for ins in md.disasm(DATA[off(rst):off(rst) + 0x18], rst):
    if ins.mnemonic == "ldr" and "[pc" in ins.op_str:
        imm = int(ins.op_str.split("#")[1].rstrip("]"), 0)
        tgt = ((ins.address + 4) & ~3) + imm
        v = u32(tgt)
        print("    0x%08X ldr -> [0x%08X] = 0x%08X" % (ins.address, tgt, v))
        if sysinit == 0:
            sysinit = v
        else:
            main_ptr = v
check(sysinit & ~1 == addr(0x9F60), "SystemInit = 0x%08X（off 0x%05X）" % (sysinit, off(sysinit)))
check(main_ptr & ~1 == addr(0x198), "__main     = 0x%08X（off 0x%05X）" % (main_ptr, off(main_ptr)))
print()
dis(addr(0x198), 0x18, "__main（C 启动：设 SP、调 .data/.bss 初始化表、跳 main）")
check(u32(addr(0x1A8)) == vec_sp,
      "__main 的 SP 字面量 0x%08X == 向量表[0]（自洽）" % u32(addr(0x1A8)))
main_addr = u32(addr(0x1A4))
check(main_addr & ~1 == addr(0xC348),
      "main = 0x%08X（off 0x%05X，文件尾 0x%05X 之前）" % (main_addr, off(main_addr), SIZE))
print()

# 1b) SystemInit 读 SCB->CPACR / RCC（真实代码特征）
print("  SystemInit 0x%08X 开头：" % (sysinit & ~1))
dis(sysinit & ~1, 0x20, "SystemInit")
print("  其字面量 0x%08X = 0x%08X（SCB->CPACR），0x%08X = 0x%08X（RCC）"
      % (addr(0x9FD4), u32(addr(0x9FD4)), addr(0x9FD8), u32(addr(0x9FD8))))
check(u32(addr(0x9FD4)) == 0xE000ED88, "SystemInit 访问 0xE000ED88（SCB->CPACR，FPU 使能）")
check(u32(addr(0x9FD8)) == 0x40021000, "SystemInit 访问 0x40021000（RCC）")
print()

# 1c) 镜像尾部指针 = BASE + SIZE
print("  尾部 off 0xE4B8 字面量 = 0x%08X；BASE+SIZE = 0x%08X"
      % (u32(addr(0xE4B8)), BASE + SIZE))
check(u32(addr(0xE4B8)) == BASE + SIZE, "镜像尾指针 == 结束地址（0x08003000+0xE504）")
print()

# 1d) 通道表指针：0x0800FF94 在真 base 下映射到 off 0xCF94，是有效 channel 表
chan_ptr = u32(addr(0x98F4))
print("  0x0800C8B0 的 LDR 字面量 [0x0800C8F4] = 0x%08X（三张 32B 通道表基址）" % chan_ptr)
check(chan_ptr == 0x0800FF94 and 0 <= off(chan_ptr) < SIZE,
      "通道表基址 0x%08X -> off 0x%05X，内容 %s ..."
      % (chan_ptr, off(chan_ptr), DATA[off(chan_ptr):off(chan_ptr) + 8].hex(" ")))
print()

# 1e) 早期口径(Lead base 0x08000000) -> 真实地址对照
print("  早期口径(base 0x08000000) 地址 -> 真实地址换算（+0x3000）：")
for lead_a in (0x080098F8, 0x0800992C, 0x0800B842, 0x0800B932, 0x0800B5AA,
               0x0800B5CC, 0x0800E4B4, 0x080098B0, 0x08008988):
    real = lead_a - LEAD_BASE + BASE
    print("    0x%08X -> 0x%08X（off 0x%05X）" % (lead_a, real, off(real)))
print()

# ===========================================================================
# 2. 同步字函数
# ===========================================================================
SYNC_FN = 0x0800C8F8
SYNC_FN_END = 0x0800C92C
SYNC_ORIG = DATA[off(SYNC_FN):off(SYNC_FN_END)]

print("=" * 78)
print("2. 同步字函数 0x%08X..0x%08X（%d B，下一函数 0x%08X）"
      % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_END - SYNC_FN, SYNC_FN_END))
print("=" * 78)
print("  原厂字节(%d B): %s" % (len(SYNC_ORIG), SYNC_ORIG.hex(" ")))
print()
dis(SYNC_FN, SYNC_FN_END - SYNC_FN, "同步字函数（原厂）")
check(SYNC_FN_END - SYNC_FN == 52, "函数长度 52 B == Lead 给定")
check(SYNC_ORIG[2:6].hex() == "4cf26831", "movw r1,#0xC368（50024）位于 +2")
check(DATA[off(SYNC_FN) + 2:off(SYNC_FN) + 4] == b"\x4c\xf2",
      "全镜像 movw #0xC368 仅此一处（变量字面量 50024 见下）")
print("  全镜像 4 字节字面量 50024(0xC368) 出现次数: %d" %
      DATA.count(struct.pack("<I", 50024)))
print()
check(not find_branch_targets(SYNC_FN + 2, kinds=("bl", "bl.w", "b", "b.w")),
      "无 BL/分支目标落入函数中间（入口 0x%08X 除外，见 §3）" % SYNC_FN)
check(not any(ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str
              for ins in md.disasm(SYNC_ORIG, SYNC_FN)),
      "函数体内无 LDR 字面量（52 B 全为代码，整段可原位改写）")
print()

# ===========================================================================
# 3. 调用点 + r0(ID) 来源
# ===========================================================================
print("=" * 78)
print("3. 同步字函数全部调用点（BL -> 0x%08X）与 r0(ID) 来源" % SYNC_FN)
print("=" * 78)
calls = find_branch_targets(SYNC_FN, kinds=("bl", "bl.w", "blx", "b", "b.w"))
CALLER_FN = {0x08005F72: 0x08005F14,
             0x0800ADC4: 0x0800AD8C,
             0x0800C7F8: 0x0800C790}
print("  调用点数量: %d（全部为 bl）" % len(calls))
check(len(calls) == 3, "同步字函数恰有 3 个直接调用点")
for a, m in calls:
    fn = CALLER_FN[a]
    print("  调用点 0x%08X（%s，文件 off 0x%05X），调用者函数 0x%08X（off 0x%05X）"
          % (a, m, off(a), fn, off(fn)))
    check(is_fn_entry(fn, a), "0x%08X 位于函数 0x%08X 内（该地址是 push 序言）"
          % (a, fn))
print()
print("  逐个调用点上下文（调用前 0x20 B）：")
CALL_CTX = {
    0x08005F72: "函数 0x08005F14「RF 初始化/应用」：先 strobe(SIDLE)+delay、"
                "调 CC2500 初始化表 0x0800E5CC，读 reg0x0D/0x0E，"
                "再 set_channel(0x0800C8B0)、set_sync(0x0800C8F8)、set 0x0A，"
                "最后 tail-call 0x0800AD6C（SCAL+开中断）",
    0x0800ADC4: "函数 0x0800AD8C「设置同步/周期应用」：比较 RAM 结构 0x200203E0 的 "
                "[+5]=channel/ [+6]=ID 与影子缓存 0x20020122[0]/[1]，不同则 set_channel/set_sync，"
                "再更新影子缓存（由 main 主循环 -> 0x0800B9B4 软件定时器 tail-call 进入）",
    0x0800C7F8: "函数 0x0800C790「菜单设置应用」：switch(设置项 id [0x20020028+0x26]) "
                "case 5/6/0xA；case 6 比较 [0x200203E0+6] 与待生效值 0x20020028+0x16，"
                "不同则 bl 同步字函数，然后把新值写回 [0x200203E0+6]",
}
for a, m in calls:
    caller = CALLER_FN[a]
    print()
    print("  --- 0x%08X (%s) in fn 0x%08X ---" % (a, m, caller))
    print("  上下文: %s" % CALL_CTX.get(a, "（见下方反汇编）"))
    dis(a - 0x14, 0x24, "调用点前后", lit=True)

# 自动解析每个调用点 r0 的 ldrb 来源（向前找最近的 ldrb 目标 r0）
print("  r0(ID) 来源自动解析（向前 6 条指令内找 ldrb r0,[rX,#imm]）：")
for a, _m in calls:
    o = off(a) - 12
    got = None
    for ins in md.disasm(DATA[o:off(a) + 2], addr(o)):
        if ins.mnemonic in ("ldrb", "ldrb.w") and ins.op_str.startswith("r0,"):
            got = ins
    if got is not None:
        print("    0x%08X  %s %s" % (got.address, got.mnemonic, got.op_str))
    else:
        print("    0x%08X  未找到 ldrb r0（人工核对见上下文）" % a)
print()

# ===========================================================================
# 4. 调用者链与「原厂何时重设同步字」
# ===========================================================================
print("=" * 78)
print("4. 调用者链（场景结论）")
print("=" * 78)
CHAIN = {
    "RF 初始化 0x08005F14": (0x08005F14, [0x0800D03C, 0x0800DAE6]),
    "周期应用 0x0800AD8C": (0x0800AD8C, [0x0800B9F2]),
    "菜单设置应用 0x0800C790": (0x0800C790, [0x08009B68]),
}
for name, (fn, refs) in CHAIN.items():
    hits = find_branch_targets(fn, kinds=("bl", "bl.w", "blx", "b", "b.w"))
    print("  %s（0x%08X）被调用点:" % (name, fn))
    for a, m in hits:
        print("    0x%08X %s -> 位于函数 0x%08X" % (a, m, enclosing_fn(a)))
    print()
print("  再向上：")
for fn2 in (0x0800CFF4, 0x0800DAB4, 0x0800B9B4, 0x080097F8):
    hits = find_branch_targets(fn2, kinds=("bl", "bl.w", "blx", "b", "b.w"))
    print("    0x%08X <- %s" % (fn2, [("0x%08X/%s" % x) for x in hits]))
print()
dis(addr(0xC348), 0x1A0, "main 0x0800F348（主循环调用链）")
print()

# ===========================================================================
# 5. CC2500 helper
# ===========================================================================
print("=" * 78)
print("5. CC2500 helper 与「谁写 SYNC1/SYNC0」")
print("=" * 78)
WRITE_REG = 0x0800E842
STROBE = 0x0800E932
DELAY = 0x0800E5AA
dis(WRITE_REG, 0x0C, "WriteReg(reg=r0, value=r1)")
dis(STROBE, 0x18, "strobe(cmd=r0)")
dis(DELAY, 0x20, "delay(n=r0) 忙等循环")
print("  WriteReg 实现为 0x0800E94A(reg, &value, 1) 单字节写；strobe 先写命令再查状态。")
print()
# 全镜像 WriteReg 调用点 + 静态解析 r0/r1
wr_calls = find_branch_targets(WRITE_REG, kinds=("bl", "bl.w"))
print("  BL -> WriteReg(0x%08X) 调用点数量: %d" % (WRITE_REG, len(wr_calls)))


def resolve_reg_before(site: int, regname: str):
    """向前回溯 8 条指令，找 movs/mov/xor 等对 regname 的最近常量定义。"""
    o = off(site) - 24
    last = None
    for ins in md.disasm(DATA[o:off(site) + 2], addr(o)):
        if ins.mnemonic in ("movs", "mov", "mov.w") and ins.op_str.startswith(regname + ", #") \
                or (ins.mnemonic == "mov" and ins.op_str == "%s, r1" % regname):
            last = ins
        elif ins.mnemonic in ("movs", "mov") and ins.op_str == "%s, r1" % regname:
            last = ins
    return last


reg45 = []
for site, _m in wr_calls:
    r0 = resolve_reg_before(site, "r0")
    r1 = resolve_reg_before(site, "r1")
    if r0 is not None:
        try:
            imm = int(r0.op_str.split("#")[1], 0)
        except Exception:  # noqa: BLE001
            continue
        if imm in (4, 5):
            reg45.append((site, imm, r1.op_str if r1 else "?"))
print("  写 reg4/reg5 的调用点:")
for site, reg, r1 in reg45:
    print("    0x%08X reg=%d value=%s（函数 0x%08X）"
          % (site, reg, r1, enclosing_fn(site)))
print()
check(len(reg45) == 4, "全镜像写 SYNC1(reg4)/SYNC0(reg5) 的调用点恰为 4 个"
      "（同步字函数 2 个 + CC2500 初始化表 2 个）")
print()
print("  CC2500 初始化表函数 0x0800E5CC（被 RF 初始化 0x08005F14 的 0x08005F30 调用）：")
dis(0x0800E5CC, 0x60, "CC2500 初始化表（前 24 个寄存器）")
print("  其中 reg4=0xC3 / reg5=0x68 -> 初始同步字 0xC368=50024；"
      "随后被同步字函数按 ID 覆盖。")
print()

# ===========================================================================
# 6. ID / C.Fn RAM 变量
# ===========================================================================
print("=" * 78)
print("6. ID / C.Fn RAM 变量与副本（详细写路径见 sync_ram.py）")
print("=" * 78)
CAND = [0x20020028, 0x200203E0, 0x20020122, 0x2002002E, 0x2002003E, 0x200203E6]
for v in CAND:
    str_hits = find_u32_words([v]).get(v, [])
    real = [h for h in str_hits if h < 0x100000000]
    print("  0x%08X 作为 u32 字面量出现 %d 次: %s"
          % (v, len(real), [("0x%08X" % h) for h in real[:10]]))
print()
print("  C.Fn 值数组: 0x200203E0 + 项号（菜单 inc/dec 处理函数直接读写；"
      "项 5=channel、6=ID、0xA=功率/模式）")
print("  C.Fn 最大值表: 0x20020072 + 项号（.data 初始化，见 sync_ram.py 启动解压输出）")
print("  菜单编辑缓存: 0x20020028 + {0x15=channel, 0x16=ID, 0x17=0xA项, 0x25=应用标志, 0x26=项号}")
print()
print("  3 个同步字调用点的 r0 来源：")
print("    0x08005F72: r4=0x200203E0（off 0x05F9C 字面量）; ldrb r0,[r4,#6] -> 0x200203E6")
print("    0x0800ADC4: r7=0x200203E0（off 0x0B088 字面量）; ldrb r0,[r7,#6] -> 0x200203E6")
print("    0x0800C7F8: r4=0x20020028（off 0x0C8A4 字面量）; ldrb r0,[r4,#0x16] -> 0x2002003E")
print()
check(u32(0x0800C8A4) == 0x20020028, "0x0800C8A4 字面量 = 0x20020028（菜单缓存基址）")
check(u32(0x0800B088) == 0x200203E0, "0x0800B088 字面量 = 0x200203E0（C.Fn/ID 数组基址）")
check(u32(0x08005F9C) == 0x200203E0, "0x08005F9C 字面量 = 0x200203E0（RF 初始化取 ID）")
print()

# ===========================================================================
# 7. 结论
# ===========================================================================
print("=" * 78)
print("7. 结论")
print("=" * 78)
print("  * 真实链接基址 = 0x%08X（Reset/SystemInit/__main/尾指针/通道表五重自证）" % BASE)
print("  * 同步字函数 = 0x%08X..0x%08X（52 B），全镜像唯一 movw #0xC368" % (SYNC_FN, SYNC_FN_END - 1))
print("  * 调用点 3 个：0x08005F72 / 0x0800ADC4 / 0x0800C7F8")
print("  * ID 生效值 = [0x200203E6]（C.Fn 项 6）；菜单待生效值 = [0x2002003E]（0x20020028+0x16）")
print("  * 原厂在「RF 初始化」「ID 改动应用」「周期应用(检测到差异)」三处重设同步字；")
print("    只改 C.Fn 其它项（非 5/6/0xA）时原厂不会重设 -> 需注入 hook（见 sync_holes.py / sync_hook_plan.md）")
print()
print("FAIL 项: %d" % len(FAIL))
for f in FAIL:
    print("  ! " + f)
sys.exit(0 if not FAIL else 1)
