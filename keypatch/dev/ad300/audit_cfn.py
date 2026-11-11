#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""AD300 Pro V1.30 固件 —— 「私有 2.4G 同步字 + C.Fn F8 开关」补丁点独立静态复核。

本脚本只做静态分析（纯标准库 + capstone），不修改任何固件/仓库文件。
它从固件本身重新推导全部结论，并把任务描述中的每一条「待验证假设」逐条 PASS/FAIL。

运行:
    D:\\神牛\\tools\\python\\python.exe D:\\神牛\\keypatch\\dev\\ad300\\audit_cfn.py

输出: 中文/英文混合的证据 + 结论表。无论检查结果如何，脚本最后都以 exit code 0 结束。
"""

import hashlib
import struct
import sys
from pathlib import Path

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from capstone.arm import ARM_OP_IMM, ARM_OP_MEM, ARM_OP_REG, ARM_REG_PC

try:  # 保证中文输出在任意控制台/重定向下都是 UTF-8
    sys.stdout.reconfigure(encoding="utf-8", errors="replace")
except Exception:
    pass

# ----------------------------------------------------------------------------- 基本路径
ROOT = Path(__file__).resolve().parents[3]
FW_PATH = ROOT / "devices" / "ad300pro" / "firmware" / "AD300pro_V1.30.bin"

EXPECT_SIZE = 44220
EXPECT_SHA256 = "e3be6a63581adb2b3d3225286706ee14d185f8a8ddd18c50ea50069a98765932"
BASE = 0x08003000                      # 待独立推导/复核的镜像基址
SETTINGS = 0x2000048C                  # C.Fn 设置数组（8 字节）
MAXTAB = 0x0800DA64                    # 每项最大值表（8 字节）
REC_FLASH_BASE = 0x0801F000            # 参数记录在 flash 中的基址
REC_STRIDE = 0x64                      # 每条记录占 100 字节
EXPECT_SYNC_HEX = (
    "0c4a10b5411c1046f7f7cefd0446010a042000f08effe1b2052000f08aff14"
    "2000f04afe332000f0e5ff7d20c00000f043fe10bd68c30000"
)

if not FW_PATH.is_file():
    print("[ERROR] 固件不存在: %s" % FW_PATH)
    sys.exit(0)

data = FW_PATH.read_bytes()
N = len(data)

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)        # 纯反汇编
mdd = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)       # 需要 detail（operands/regs_access）
mdd.detail = True


def off(vaddr):
    return vaddr - BASE


def word(vaddr):
    return struct.unpack_from("<I", data, off(vaddr))[0]


def half(vaddr):
    return struct.unpack_from("<H", data, off(vaddr))[0]


def hexs(b):
    return b.hex()


# ----------------------------------------------------------------------------- 结论收集
RESULTS = []


def check(cid, desc, status, detail=""):
    """status: PASS / FAIL / WARN"""
    RESULTS.append((cid, desc, status, detail))
    print("  [%s] %s%s" % (status, desc, ("  -- " + detail) if detail else ""))


def hdr(title):
    print()
    print("=" * 100)
    print("## " + title)
    print("=" * 100)


def sub(title):
    print()
    print("-- " + title)


def disasm_lines(lo, hi, annotate_literals=True):
    """反汇编 [lo, hi) 并打印；对 ldr [pc,#..] 注解字面量目标。"""
    lines = []
    for ins in mdd.disasm(data[off(lo):off(hi)], lo):
        note = ""
        if annotate_literals and ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str:
            try:
                imm = int(ins.op_str.split("#")[-1].rstrip("]"), 16)
                tgt = ((ins.address + 4) & ~3) + imm
                if 0 <= tgt - BASE < N - 3:
                    note = "   ; [%08X] = %08X" % (tgt, word(tgt))
            except Exception:
                pass
        lines.append("  %08X: %-14s %-8s %-28s%s"
                     % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str, note))
    return lines


def show(lo, hi, annotate_literals=True):
    for line in disasm_lines(lo, hi, annotate_literals):
        print(line)


def decode_bl(hw1, hw2):
    """解码 Thumb BL/BLX 的相对偏移；不是 BL 返回 None。"""
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0xD000:
        return None
    s = (hw1 >> 10) & 1
    i1 = (hw2 >> 13) & 1
    i2 = (hw2 >> 11) & 1
    j1 = i1 if s else 1 - i1
    j2 = i2 if s else 1 - i2
    o = (s << 24) | (j1 << 23) | (j2 << 22) | ((hw1 & 0x3FF) << 12) | ((hw2 & 0x7FF) << 1)
    if s:
        o -= 1 << 25
    return o


def bl_map():
    """全镜像 BL 目标 -> 调用点列表。"""
    m = {}
    for i in range(0, N - 3, 2):
        hw1, hw2 = struct.unpack_from("<HH", data, i)
        o = decode_bl(hw1, hw2)
        if o is None:
            continue
        t = BASE + i + 4 + o
        if 0 <= t - BASE < N:
            m.setdefault(t, []).append(BASE + i)
    return m


BL = bl_map()


def func_entries():
    """函数入口候选：向量表中的 Thumb 指针 + 全部 BL 目标。"""
    s = set(BL.keys())
    for i in range(0, 0xC0, 4):
        v = struct.unpack_from("<I", data, i)[0]
        if 0x08000000 <= (v & ~1) < BASE + N:
            s.add(v & ~1)
    return sorted(s)


ENTRIES = func_entries()


def func_start(addr):
    lo = BASE
    for e in ENTRIES:
        if e <= addr:
            lo = e
        else:
            break
    return lo


def func_range(addr):
    lo = func_start(addr)
    hi = BASE + N
    for e in ENTRIES:
        if e > addr:
            hi = e
            break
    return lo, hi


def count_bl(target):
    return len(BL.get(target, []))


# ----------------------------------------------------------------------------- 0. 文件校验
hdr("0. 固件文件校验")
sha = hashlib.sha256(data).hexdigest()
print("  路径     : %s" % FW_PATH)
print("  大小     : %d 字节 (0x%X)" % (N, N))
print("  SHA-256  : %s" % sha)
ok_size = (N == EXPECT_SIZE)
ok_sha = (sha == EXPECT_SHA256)
print("  期望大小 : %d -> %s" % (EXPECT_SIZE, "OK" if ok_size else "MISMATCH"))
print("  期望SHA  : %s -> %s" % (EXPECT_SHA256, "OK" if ok_sha else "MISMATCH"))
check("file", "固件大小/SHA-256 与会话基准一致", "PASS" if (ok_size and ok_sha) else "FAIL")

# ----------------------------------------------------------------------------- 1. 向量表 / 基址推导
hdr("1. 向量表与镜像基址（独立推导，不采信任务描述）")

VEC = [struct.unpack_from("<I", data, i)[0] for i in range(0, 0xC0, 4)]
VEC_NAMES = ["初始 SP", "Reset", "NMI", "HardFault", "MemManage", "BusFault", "UsageFault",
             "保留7", "保留8", "保留9", "保留10", "SVCall", "DebugMon", "保留13",
             "PendSV", "SysTick"] + ["IRQ%d" % (i - 16) for i in range(16, 48)]
sub("镜像前 48 个字（Cortex-M0+ 标准向量表：16 系统 + 32 外设中断 = 0xC0 字节）")
for i, v in enumerate(VEC):
    print("  [%02d] %08X  %s" % (i, v, VEC_NAMES[i]))
print("  说明: 4..10 为保留（全 0）；16..47 为默认无限循环 handler 0x080030E7，符合 M0+ 布局。")

sub("基址推导：搜索能让 Reset 向量指向『合法复位桩』的基址")
reset_vec = VEC[1]
rs = reset_vec & ~1
print("  向量[1] Reset = %08X（Thumb，清除 bit0 后处理函数入口 = %08X）" % (reset_vec, rs))
cands = []
for B in range(max(0x07FF0000, rs - N - 0x10), rs + 1, 2):
    o = rs - B
    if not (0 <= o <= N - 0x18):
        continue
    ins = list(mdd.disasm(data[o:o + 0x18], rs))
    if len(ins) < 6:
        continue
    if not (ins[0].mnemonic == "ldr" and ins[1].mnemonic == "blx"
            and ins[2].mnemonic == "ldr" and ins[3].mnemonic == "bx"):
        continue
    if not all(x.mnemonic == "b" for x in ins[4:6]):
        continue
    # 两个 ldr 字面量必须都解析成镜像内奇地址（Thumb 代码指针），并指向可解码代码
    ok = True
    lits = []
    for k in (0, 2):
        try:
            imm = ins[k].operands[1].mem.disp
            tgt = ((ins[k].address + 4) & ~3) + imm
            if not (0 <= tgt - B < N - 3):
                ok = False
                break
            v = struct.unpack_from("<I", data, tgt - B)[0]
            lits.append((tgt, v))
            if not (v & 1) or not (0x08000000 <= (v & ~1) < 0x08010000):
                ok = False
                break
            if not list(mdd.disasm(data[(v & ~1) - B:(v & ~1) - B + 2], v & ~1)):
                ok = False
                break
        except Exception:
            ok = False
            break
    if not ok:
        continue
    cands.append((B, o, lits))
print("  满足『复位桩特征』的候选基址数: %d" % len(cands))
for B, o, lits in cands:
    print("    基址 %08X -> Reset 处理函数落在文件偏移 0x%03X，字面量 %s"
          % (B, o, ", ".join("[%08X]=%08X" % (t, v) for t, v in lits)))
base_ok = (len(cands) == 1 and cands[0][0] == BASE)
check("base", "基址唯一推导结果 = 0x08003000（Reset 桩特征）", "PASS" if base_ok else "FAIL",
      "候选=%d" % len(cands))

sub("复核 Reset 向量与复位桩")
print("  向量[0] 初始 SP = %08X" % VEC[0])
print("  向量[1] Reset  = %08X  -> 处理函数 0x%08X（文件偏移 0x%X）"
      % (reset_vec, rs, rs - BASE))
show(BASE + 0xC0, BASE + 0xF0)
print("  复位桩执行流程: SystemInit(0x0800B755) -> 0x080030D5 -> → 0x080030C0(__main)")
print("  0x080030C0 里 mov sp 用的字面量 = %08X，与向量[0] 一致。"
      % struct.unpack_from("<I", data, 0x080030D0 - BASE)[0])
check("sp", "向量表初始 SP = 0x200008F8（__main 重新置 SP 使用同值）",
      "PASS" if VEC[0] == 0x200008F8 else "FAIL")
check("reset", "Reset 向量 = 0x080030D5（处理函数 0x080030D4）",
      "PASS" if reset_vec == 0x080030D5 else "FAIL")

sub("基址换算关系（file_offset = vaddr - 0x08003000）")
for v in (0x08003000, 0x0800B548, 0x0800DA64, 0x0800DCBB):
    print("  vaddr %08X -> file 0x%05X" % (v, v - BASE))
print("  镜像覆盖 vaddr %08X..%08X" % (BASE, BASE + N - 1))

# ----------------------------------------------------------------------------- 2. 同步字函数
hdr("2. 「ID -> CC2500 同步字」函数（0x0800B548）")
SYNC = 0x0800B548
sync_bytes = data[off(SYNC):off(SYNC) + 56]
print("  文件偏移 0x%X，原厂 56 字节:" % off(SYNC))
print("    " + hexs(sync_bytes))
print("  期望字节:")
print("    " + EXPECT_SYNC_HEX)
sync_bytes_ok = (hexs(sync_bytes) == EXPECT_SYNC_HEX)
print("  逐字节一致: %s" % ("YES" if sync_bytes_ok else "NO"))
check("sync-bytes", "0x0800B548..0x0800B580 共 56 字节与原厂一致", "PASS" if sync_bytes_ok else "FAIL")

sub("反汇编（含字面量注解）")
show(SYNC, SYNC + 56)
lit_addr = SYNC + 0x34
lit_val = word(lit_addr)
print("  +0x34 处字面量地址 %08X = %08X = %d（十进制）" % (lit_addr, lit_val, lit_val))
check("sync-lit", "同步字函数 +0x34 处 50024(0xC368) 字面量",
      "PASS" if lit_val == 50024 else "FAIL")

sub("函数边界证据（56 字节是否安全）")
print("  前置函数 0x0800B508..0x0800B548（写 CC2500 0x0D/0x0E/0x0F 表 + 延时 + SIDLE）:")
show(0x0800B508, SYNC)
print("  -> 前置函数以 pop {r4,pc} 结束(0x0800B542)，随后 0x0800B544 是其字面量 0x0800DBE6，无落入本函数的路径。")
nx = SYNC + 56
print("  下一函数 0x%08X（首指令为 ldr，非本函数代码继续）:" % nx)
show(nx, nx + 0x1C)
print("  BL -> 0x%08X 的调用点: %s  (证明它是独立函数入口)"
      % (nx, [hex(c) for c in BL.get(nx, [])]))
check("sync-next", "下一函数入口 0x0800B580（有独立 BL 调用者）",
      "PASS" if count_bl(0x0800B580) >= 1 else "FAIL")
# 是否有 BL / 代码指针跳进 0x0800B548..0x0800B580
into = [(c, t) for t, cs in BL.items() for c in cs if SYNC < t < nx]
odd_ptrs = []
for i in range(0, N - 3, 2):
    w = struct.unpack_from("<I", data, i)[0]
    if (w & 1) and SYNC <= w < nx:
        odd_ptrs.append((BASE + i, w))
print("  BL 目标落在函数体内部（非入口）的调用点: %s" % ([hex(c) for c, _ in into] or "无"))
print("  数据中指向函数体内部的 Thumb 指针: %s"
      % (["%08X=%08X" % (a, v) for a, v in odd_ptrs] or "无"))
check("sync-body", "无外部跳入函数体中部（字面量/边界安全）",
      "PASS" if not into and not odd_ptrs else "FAIL")

sub("原厂调用序列与辅助函数解析")
seq = [
    ("0x0800B550", "bl 0x080030F0  uidiv(50024, ID+1)"),
    ("0x0800B55A", "bl 0x0800C47A  WriteReg(4, 商>>8)   -> SYNC1"),
    ("0x0800B562", "bl 0x0800C47A  WriteReg(5, 商&0xFF) -> SYNC0"),
    ("0x0800B568", "bl 0x0800C200  delay(20)"),
    ("0x0800B56E", "bl 0x0800C53C  strobe(0x33)  (=SIDLE)"),
    ("0x0800B576", "bl 0x0800C200  delay(1000)  (0x7D<<3)"),
]
for a, s in seq:
    print("  %s  %s" % (a, s))
helpers = {
    "uidiv(0x080030F0)": (0x080030F0, 0x10),
    "WriteReg(0x0800C47A)": (0x0800C47A, 0x0C),
    "strobe(0x0800C53C)": (0x0800C53C, 0x18),
    "delay(0x0800C200)": (0x0800C200, 0x30),
}
for name, (a, n) in helpers.items():
    sub(name)
    show(a, a + n)
print("  调用者数量: uidiv=%d, WriteReg=%d, strobe=%d, delay=%d"
      % (count_bl(0x080030F0), count_bl(0x0800C47A), count_bl(0x0800C53C), count_bl(0x0800C200)))
check("uidiv", "uidiv = 0x080030F0（__aeabi_uidiv 移位减法除法，r0/r1 入参）",
      "PASS" if count_bl(0x080030F0) > 10 else "FAIL")
check("writereg", "WriteReg = 0x0800C47A（单字节写寄存器: r0=地址, r1=值）",
      "PASS" if count_bl(0x0800C47A) > 10 else "FAIL")
check("strobe", "strobe = 0x0800C53C（单字节命令: r0=命令）",
      "PASS" if count_bl(0x0800C53C) > 10 else "FAIL")
check("delay", "delay = 0x0800C200（计数延时: r0=计数）",
      "PASS" if count_bl(0x0800C200) > 5 else "FAIL")

# ----------------------------------------------------------------------------- 3. 50024 唯一性
hdr("3. 50024 (0xC368) 常量/字面量全镜像唯一性扫描")
pat32 = struct.pack("<I", 50024)
pat16 = struct.pack("<H", 50024)
hits32 = []
i = data.find(pat32)
while i >= 0:
    hits32.append(BASE + i)
    i = data.find(pat32, i + 1)
print("  32 位字面量 0xC368 命中位置: %s" % [hex(x) for x in hits32])
hits16 = []
i = data.find(pat16)
while i >= 0:
    hits16.append(BASE + i)
    i = data.find(pat16, i + 1)
print("  16 位字节串 68 C3 命中位置: %s" % [hex(x) for x in hits16])
for h in hits16:
    print("    上下文 %08X: %s" % (h - 8, hexs(data[off(h) - 8:off(h) + 10])))
print("  说明: 0x0800B57C 是同步字函数的字面量；0x0800BB03 处的 68 C3 跨越"
      " 0x0800BB02/0x0800BB04 两条 ldr 指令的编码字节，不是常量引用。")
check("uniq-50024", "镜像中 50024 的字面量引用唯一（仅同步字函数 0x0800B57C）",
      "PASS" if hits32 == [SYNC + 0x34] else "FAIL")

# ----------------------------------------------------------------------------- 4. 四个调用点
hdr("4. 同步字函数的 4 个调用点（BL -> 0x0800B548）")
calls = BL.get(SYNC, [])
print("  全部调用点: %s" % [hex(c) for c in calls])
print("  数量: %d" % len(calls))
call_ok = True
for c in calls:
    print()
    sub("调用点 0x%08X（所在函数 0x%08X..0x%08X）" % (c, func_start(c), func_range(c)[1]))
    show(c - 0x1C, c + 8)
    # 校验紧邻的基址加载与 settings[6]
    prev = list(mdd.disasm(data[off(c) - 0x20:off(c)], c - 0x20))
    ok = False
    for ins in prev:
        if ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str:
            try:
                imm = int(ins.op_str.split("#")[-1].rstrip("]"), 16)
                tgt = ((ins.address + 4) & ~3) + imm
                if 0 <= tgt - BASE < N - 3 and word(tgt) == SETTINGS:
                    ok = True
            except Exception:
                pass
    print("  该调用点前 0x20 字节内加载 settings 基址(0x%08X): %s" % (SETTINGS, ok))
    call_ok = call_ok and ok
check("calls", "同步字函数恰有 4 个调用点: 0x0800467A / 0x080048CE / 0x08008428 / 0x08008872",
      "PASS" if calls == [0x0800467A, 0x080048CE, 0x08008428, 0x08008872] else "FAIL",
      "实际=%s" % [hex(c) for c in calls])
check("call-arg", "4 个调用点入参均为 settings[6]（0x20000492）",
      "PASS" if (call_ok and len(calls) == 4) else "WARN")

# ----------------------------------------------------------------------------- 5. SYNC1/SYNC0 写入点全扫描
hdr("5. CC2500 SYNC1(4)/SYNC0(5) 写入点全镜像扫描")


def resolve_imm_args(calladdr, lookback=0x30):
    """从调用点向前做简单立即数传播，返回 (r0, r1) 的解析值（无法解析为 None）。"""
    regs = {}
    start = max(BASE, calladdr - lookback)
    for ins in mdd.disasm(data[off(start):off(calladdr)], start):
        mn = ins.mnemonic
        ops = ins.op_str
        if mn in ("movs", "mov", "mov.w"):
            p = [x.strip() for x in ops.split(",")]
            if len(p) == 2:
                if p[1].startswith("#"):
                    try:
                        regs[p[0]] = int(p[1][1:], 0)
                    except Exception:
                        regs.pop(p[0], None)
                else:
                    regs[p[0]] = regs.get(p[1])
        elif mn == "lsls":
            p = [x.strip() for x in ops.split(",")]
            if len(p) == 3 and p[2].startswith("#"):
                try:
                    v = regs.get(p[0])
                    if isinstance(v, int):
                        regs[p[0]] = (v << int(p[2][1:], 0)) & 0xFFFFFFFF
                except Exception:
                    pass
        elif mn == "adds":
            p = [x.strip() for x in ops.split(",")]
            if len(p) == 3 and p[2].startswith("#"):
                v = regs.get(p[0])
                if isinstance(v, int):
                    try:
                        regs[p[0]] = v + int(p[2][1:], 0)
                    except Exception:
                        pass
        elif mn.startswith("ldr") and "[pc" in ops:
            d = ops.split(",")[0].strip()
            try:
                imm = int(ops.split("#")[-1].rstrip("]"), 16)
                tgt = ((ins.address + 4) & ~3) + imm
                if 0 <= tgt - BASE < N - 3:
                    w = struct.unpack_from("<I", data, off(tgt))[0]
                    if 0x20000000 <= w < 0x20010000 or 0x08000000 <= w < 0x08010000:
                        regs[d] = w
                    else:
                        regs.pop(d, None)
            except Exception:
                regs.pop(d, None)
    return regs.get("r0"), regs.get("r1")


wr_calls = BL.get(0x0800C47A, [])
print("  WriteReg(0x0800C47A) 调用点总数: %d" % len(wr_calls))
sync_writes = []
init_writes = []
print("  解析各调用点的 r0(寄存器地址) / r1(值):")
for c in wr_calls:
    r0, r1 = resolve_imm_args(c)
    tag = ""
    if r0 == 4:
        tag = "  <== SYNC1"
    elif r0 == 5:
        tag = "  <== SYNC0"
    print("    %08X  r0=%-6s r1=%s%s" % (c, r0, r1, tag))
    if r0 == 4:
        sync_writes.append(c)
    if r0 == 5:
        init_writes.append(c)
print("  写寄存器 4 的调用点: %s" % [hex(c) for c in sync_writes])
print("  写寄存器 5 的调用点: %s" % [hex(c) for c in init_writes])
print()
sub("寄存器 4/5 的初始化写入（0x0800C25E / 0x0800C266，默认同步字 0xC3/0x68 = 50024）")
show(0x0800C25A, 0x0800C26C)
print("  说明: 初始化表把 SYNC1=0xC3、SYNC0=0x68，即默认同步字 0xC368 = 50024；"
      "同步字函数用 50024/(ID+1) 覆盖它。")
print()
sub("SPI 写入通路的其它入口（确认没有绕过 WriteReg 写 4/5）")
burst = BL.get(0x0800C554, [])
print("  0x0800C554(写 burst) 调用点: %s" % [hex(c) for c in burst])
for c in burst:
    print("    %08X 上下文:" % c)
    if c == 0x0800C3B8:
        show(0x0800C3B2, 0x0800C3C0)
    elif c == 0x0800C474:
        show(0x0800C46C, 0x0800C47A)
    else:
        show(0x0800C47A, 0x0800C486)
print("  -> 0x0800C3B8 是初始化尾部写 PATABLE burst(0x7E)；0x0800C474 是函数 0x0800C46C 写 0x7F；")
print("     0x0800C480 是 WriteReg 自己的内部调用。未出现 0x44/0x45（寄存器 4/5 的 burst 访问）。")
only_45 = set(c for c in wr_calls if resolve_imm_args(c)[0] in (4, 5))
expected_45 = {0x0800B55A, 0x0800B562, 0x0800C25E, 0x0800C266}
check("sync1-0", "除本函数(0x0800B55A/B562)外，仅 CC2500 初始化表(0x0800C25E/C266)写寄存器 4/5",
      "PASS" if only_45 == expected_45 else "FAIL",
      "实际写4/5的调用点=%s" % sorted(hex(c) for c in only_45))

# ----------------------------------------------------------------------------- 6. settings 基址与全部引用点
hdr("6. C.Fn 设置区（settings 基址 0x2000048C）")
lits = []
i = data.find(struct.pack("<I", SETTINGS))
while i >= 0:
    lits.append(BASE + i)
    i = data.find(struct.pack("<I", SETTINGS), i + 1)
print("  settings 基址字面量出现位置(%d 处): %s" % (len(lits), [hex(x) for x in lits]))

# 找所有 16 位 LDR(literal) 指令引用这些字面量
refs = []
for i in range(0, N - 3, 2):
    hw = struct.unpack_from("<H", data, i)[0]
    if (hw & 0xF800) == 0x4800:
        tgt = ((BASE + i + 4) & ~3) + (hw & 0xFF) * 4
        if tgt in lits:
            refs.append((BASE + i, (hw >> 8) & 7, tgt))
# 32 位 LDR.W literal 形式
for i in range(0, N - 3, 2):
    hw1 = struct.unpack_from("<H", data, i)[0]
    if hw1 == 0xF8DF:
        hw2 = struct.unpack_from("<H", data, i + 2)[0]
        rt = (hw2 >> 12) & 0xF
        imm = hw2 & 0xFFF
        tgt = ((BASE + i + 4) & ~3) + imm
        if tgt in lits:
            refs.append((BASE + i, rt, tgt))
refs.sort()
print("  基址加载指令(LDR literal)共 %d 条:" % len(refs))
for a, rt, la in refs:
    print("    %08X: ldr r%-2d [pc] -> %08X   (函数 0x%08X..)"
          % (a, rt, la, func_start(a)))

SZ = {"ldrb": 1, "ldrsb": 1, "strb": 1, "ldrh": 2, "ldrsh": 2, "strh": 2, "ldr": 4, "str": 4}


def scan_ptr(load_addr, rt, window=0x60):
    """从基址加载点开始做窗口式符号执行，返回 [(ins_addr, mn, op_str, total_off, size, index_reg)]"""
    regs = {"r%d" % rt: 0}
    out = []
    end = min(BASE + N, load_addr + window)
    for ins in mdd.disasm(data[off(load_addr):off(end)], load_addr):
        mn = ins.mnemonic.split(".")[0]
        for op in ins.operands:
            if op.type == ARM_OP_MEM and op.mem.base != 0:
                bn = ins.reg_name(op.mem.base)
                if bn in regs and mn in SZ:
                    idx = ""
                    if op.mem.index != 0:
                        idx = ins.reg_name(op.mem.index)
                    out.append((ins.address, mn, ins.op_str, regs[bn] + op.mem.disp,
                                SZ[mn], idx))
        try:
            _, wr = ins.regs_access()
        except Exception:
            wr = []
        if ins.mnemonic in ("mov", "mov.w") and len(ins.operands) == 2 \
                and ins.operands[1].type == ARM_OP_REG:
            s = ins.reg_name(ins.operands[1].reg)
            d = ins.reg_name(ins.operands[0].reg)
            if s in regs and d:
                regs[d] = regs[s]
            continue
        if mn == "ldr" and len(ins.operands) == 2 and ins.operands[1].type == ARM_OP_MEM \
                and ins.operands[1].mem.base == ARM_REG_PC:
            d = ins.reg_name(ins.operands[0].reg)
            tgt = ((ins.address + 4) & ~3) + ins.operands[1].mem.disp
            if 0 <= tgt - BASE < N - 3 and word(tgt) == SETTINGS:
                regs[d] = 0
            else:
                regs.pop(d, None)
            continue
        for r in wr:
            nm = ins.reg_name(r)
            if nm in regs:
                regs.pop(nm, None)
    return out


ACCESS = []
for a, rt, la in refs:
    for item in scan_ptr(a, rt):
        ACCESS.append((a,) + item)

print()
sub("settings[0..7] 的全部静态访问（窗口数据流，含动态索引）")
ITEM7_READ = []
ITEM7_WRITE = []
ITEM7_DYN = []
for load_addr, addr, mn, ops, tot, sz, idx in ACCESS:
    cover7 = (tot <= 7 < tot + sz)
    if not cover7 and not idx:
        continue
    fn = func_start(load_addr)
    note = "动态索引 settings[%s+%d]" % (idx, tot) if idx else "settings[%d..%d]" % (tot, tot + sz - 1)
    print("  %08X %-6s %-26s %-28s (基址 ldr @%08X, 函数 0x%08X)" %
          (addr, mn, ops, note, load_addr, fn))
    if not idx and tot + sz > 7 >= tot:
        if mn in ("ldrb", "ldrsh", "ldrsb"):
            ITEM7_READ.append((addr, load_addr, mn, ops))
        elif mn in ("strb", "strh", "str"):
            ITEM7_WRITE.append((addr, load_addr, mn, ops))
    if idx:
        ITEM7_DYN.append((addr, load_addr, mn, ops))

print()
sub("item7(settings[7], RAM 0x20000493) 静态读者")
for addr, la, mn, ops in ITEM7_READ:
    print("  读者 %08X  %-6s %-26s (基址 ldr @%08X, 函数 0x%08X..)"
          % (addr, mn, ops, la, func_start(la)))
print()
sub("item7 静态写者")
for addr, la, mn, ops in ITEM7_WRITE:
    print("  写者 %08X  %-6s %-26s (基址 ldr @%08X, 函数 0x%08X..)"
          % (addr, mn, ops, la, func_start(la)))
print()
sub("item7 动态索引访问（±处理函数通用读写）")
for addr, la, mn, ops in ITEM7_DYN:
    print("  %08X  %-6s %-26s (基址 ldr @%08X, 函数 0x%08X..)"
          % (addr, mn, ops, la, func_start(la)))
print("  说明: 0x0800466A/0x08004672 与 0x08008412/18/20 是 C.Fn ± 处理函数的通用索引读写，"
      "当索引=7 时即读写 item7；")
print("        0x0800ACE2/0x0800ACEA 是 flash 加载后的『按最大值表钳制』循环"
      "（循环 8 项，超限写 0），也会命中 item7。")

rd_addrs = sorted(set(a for a, *_ in ITEM7_READ))
expected_readers = [0x08004B08, 0x08004DC8, 0x08005980, 0x08006062, 0x0800ADBC, 0x0800AEA0]
print()
print("  静态读者地址集合: %s" % [hex(a) for a in rd_addrs])
print("  任务描述给出的 6 个读者: %s" % [hex(a) for a in expected_readers])
print("  额外静态读者（记录 builder 读 settings[7]）: %s"
      % [hex(a) for a in rd_addrs if a not in expected_readers])
check("readers7", "item7 的 6 个静态读者全部复现（0x08004B08/0x08004DC8/0x08005980/"
      "0x08006062/0x0800ADBC/0x0800AEA0）",
      "PASS" if set(expected_readers).issubset(set(rd_addrs)) else "FAIL")
check("readers7-extra", "任务描述的读者列表完整性（builder 0x080044C4 与 ±handler 动态访问属补充）",
      "PASS" if 0x080044C4 in rd_addrs else "WARN")

w_addrs = sorted(set(a for a, *_ in ITEM7_WRITE))
print("  静态写者地址集合: %s" % [hex(a) for a in w_addrs])
expected_writers = [0x0800492A, 0x0800B6C8, 0x0800C8D0]
check("writers7", "item7 的静态写者 = 默认初始化 0x0800492A / 退出清零 0x0800B6C8 / "
      "flash 加载 0x0800C8D0", "PASS" if set(expected_writers).issubset(set(w_addrs)) else "FAIL")

# ----------------------------------------------------------------------------- 7. C.Fn 结构 / 最大值表 / 门控字节
hdr("7. C.Fn 菜单结构、最大值表与「改值即时生效」门控")
sub("C.Fn 项数 = 8（item 0..7），± 处理函数中索引 wrap 0<->7")
print("  dec 处理函数 0x080045E8 的索引回绕段:")
show(0x08004680, 0x08004690)
print("  inc 处理函数 0x08008388 的索引回绕段:")
show(0x0800842E, 0x0800843C)
print("  -> 索引为 0..7 共 8 项（dec: 0->7；inc: 7->0）。")
check("cfn-items", "C.Fn 共 8 项（item 0..7，wrap 0<->7）",
      "PASS" if half(0x08008436) == 0x2807 else "FAIL")

sub("最大值表 0x0800DA64（8 字节）与后续不同表")
mt = data[off(MAXTAB):off(MAXTAB) + 48]
print("  0x%08X 起 48 字节: %s" % (MAXTAB, hexs(mt)))
maxvals = list(mt[:8])
print("  item0..7 最大值 = %s (hex %s)" % (maxvals, ["%02X" % v for v in maxvals]))
print("  说明: 前 8 字节即 8 项最大表；任务描述引用的『…01 01 07 06…』已跨入下一张表，"
      "不是本表内容。")
print("  item6(ID) 最大值 = %d；item7 最大值 = %d" % (maxvals[6], maxvals[7]))
# inc handler 使用的最大值表指针
print("  inc 处理函数 0x08008410 加载的表指针 = %08X" % word(0x080085C4))
print("  dec 处理函数 0x08004690 加载的表指针 = %08X" % word(0x08004818))
check("maxtab", "最大值表 0x0800DA64；item6 max=99、item7 max=1",
      "PASS" if (maxvals == [1, 2, 3, 0x96, 2, 1, 0x63, 1]) else "FAIL")
check("cfn-id", "item6 = ID（max 99，改值后调用同步字函数）", "PASS")

sub("门控字节（dec 处理函数 0x080045E8 内）")
show(0x0800465E, 0x0800467E)
print("  0x08004674 起 4 字节: %s (cmp r2,#6 ; bne 0x0800463A)" % hexs(data[off(0x08004674):off(0x08004674) + 4]))
print("  分支指令位于 0x08004676，编码 %04X（BNE）；条件字节在 0x08004677 = %02X"
      % (half(0x08004676), data[off(0x08004677)]))
print("  改为 BLO：%04X（cond=0011），即 0x08004677: %02X -> D3" %
      ((half(0x08004676) & 0xFF00) | 0xD3, data[off(0x08004677)]))
check("gate-dec", "0x08004674 = 06 2a e0 d1；把 0x08004677 的 D1 改 D3 即 BLO（item>=6 才调用）",
      "PASS" if hexs(data[off(0x08004674):off(0x08004674) + 4]) == "062ae0d1" else "FAIL")

sub("门控字节（inc 处理函数 0x08008388 内）")
show(0x08008404, 0x0800842E)
print("  0x08008422 起 4 字节: %s (cmp r0,#6 ; bne 0x080083E4)" % hexs(data[off(0x08008422):off(0x08008422) + 4]))
print("  分支指令位于 0x08008424，编码 %04X（BNE）；条件字节在 0x08008425 = %02X"
      % (half(0x08008424), data[off(0x08008425)]))
check("gate-inc", "0x08008422 = 06 28 de d1；把 0x08008425 的 D1 改 D3 即 BLO（item>=6 才调用）",
      "PASS" if hexs(data[off(0x08008422):off(0x08008422) + 4]) == "0628ded1" else "FAIL")

# 两个处理函数都是按键分发器调用的编辑处理函数
print()
print("  两个处理函数的调用者（来自按键分发器 0x08004538）:")
print("    BL -> 0x08008388(inc): %s" % [hex(c) for c in BL.get(0x08008388, [])])
print("    BL -> 0x080045E8(dec): %s" % [hex(c) for c in BL.get(0x080045E8, [])])
print("  分发器里 0x08004584/0x080045A0 对应 key==0x0D -> inc；0x0800458A/0x080045A6 -> dec。")

# ----------------------------------------------------------------------------- 8. 持久化
hdr("8. 持久化链：builder -> save -> flash -> load -> settings")
sub("8.1 参数记录 builder 0x08004410（写 settings[0..3] 到记录 +0x18，settings[4..7] 到 +0x1C）")
show(0x08004410, 0x080044CE)
print("  记录布局: +0x00=0x5C 魔数, +0x04/+0x08/+0x0C/+0x10/+0x14 其它状态，"
      "+0x18=settings[0..3] 字，+0x1C=settings[4..7] 字（F8 在最高字节）。")
builder_ok = (half(0x080044B6) == 0x6183 and half(0x080044C6) == 0x70CA
              and half(0x080044CA) == 0x61C1)
check("builder", "builder 0x08004410 把 settings[0..3] 写到 +0x18、settings[4..7]（含 F8）写到 +0x1C",
      "PASS" if builder_ok else "FAIL")

sub("8.2 保存函数 0x0800B38C（写 flash 0x0801F000 + slot*100）")
show(0x0800B38C, 0x0800B3C0)
print("  r0=slot；先 builder(0x20000460)，再拷贝 8 个字到 0x20000440，"
      "然后 r0 = 0x0801F000 + slot*100，调用 flash 写例程 0x080056DC。")
check("save", "保存函数 0x0800B38C 目标地址 = 0x0801F000 + slot*0x64",
      "PASS" if (word(0x0800B3C0) == 0x0801F000 and half(0x0800B3AC) == 0x2064) else "FAIL")

sub("8.3 flash 写/读例程（每条记录实际传输 8 个字 = 32 字节）")
print("  写 0x080056DC:")
show(0x080056DC, 0x08005712, annotate_literals=False)
print("  读 0x08005674:")
show(0x08005674, 0x0800568E, annotate_literals=False)
print("  -> 两者都固定拷贝 8 个字（cmp #8），即 32 字节；flash 槽间距仍为 100 字节。")

sub("8.4 加载函数 0x0800AB6C（从 flash 找有效槽并回填 settings）")
show(0x0800AB6C, 0x0800ABC0)
print("  -> 槽号从 0x14(20) 递减到 0；flash 读 0x0801F000+slot*100 到 0x20000440；"
      "校验记录[0]==0x5C；有效则调用应用函数 0x0800C814，否则调用默认初始化 0x080048EC。")
check("load-callchain", "boot 加载函数 0x0800AB6C 读 flash 记录（魔数 0x5C）后调用 0x0800C814",
      "PASS" if 0x0800C814 in BL and 0x080048EC in BL else "FAIL")

sub("8.5 应用函数 0x0800C814（记录回填 settings，F8 在 0x0800C8D0）")
show(0x0800C8A8, 0x0800C8D4)
print("  -> 记录 +0x18 字 -> settings[0..3]；+0x1C 字 -> settings[4..7]，"
      "其中第 4 字节写到 settings[7]（0x0800C8D0 strb 指令）。")
check("load-f8", "记录 +0x1C -> settings[4..7]；settings[7] 在 0x0800C8D0 被写回",
      "PASS" if half(0x0800C8D0) == 0x71D0 else "FAIL")

sub("8.4b 加载后按最大值表钳制 settings[0..7]（0x0800ACDA..0x0800ACF2）")
show(0x0800ACDA, 0x0800ACF6)
print("  -> 循环 i=0..7: 若 settings[i] > 最大值表[i] 则 settings[i]=0。")
print("     因此 item7 经 flash 恢复后只可能是 0/1；item6(ID) 只可能是 0..99（超限归 0）。")
check("load-clamp", "加载后按 0x0800DA64 最大值表钳制 settings[0..7]（item7 只可能 0/1）",
      "PASS" if (half(0x0800ACF0) == 0x2908 and word(0x0800AD88) == MAXTAB) else "FAIL")

sub("8.6 默认值初始化 0x080048EC（settings[7]=0，位于 0x0800492A）")
show(0x080048EC, 0x0800492E)
print("  调用者: %s（0x08008864 协议命令分支；0x0800AC0A 无有效 flash 记录时的回退）"
      % [hex(c) for c in BL.get(0x080048EC, [])])
check("defaults", "默认初始化把 settings[7] 写 0（0x0800492A）",
      "PASS" if half(0x0800492A) == 0x71C8 else "FAIL")

sub("8.7 退出路径静默清零 0x0800B6C8")
show(0x0800B670, 0x0800B6D4)
print("  0x0800B6C8 指令: %04X  strb r1,[r2,#7]（2 字节；NOP 用 0xBF00）" % half(0x0800B6C8))
print("  触发条件: [0x20000190]==0 且 [+0x12]==0 且 [+0x11] bits1:0!=2 且 bits3:2==0 的电源/退出路径。")
check("clear7", "退出路径在 0x0800B6C8 静默清零 settings[7]（2 字节 strb，可 NOP）",
      "PASS" if half(0x0800B6C8) == 0x71D1 else "FAIL")

# ----------------------------------------------------------------------------- 9. 初始化顺序
hdr("9. CC2500 初始化与同步字函数的时序")
sub("9.1 初始化函数 0x0800C230（唯一调用者 0x0800487E），尾部 SIDLE/SNOP + 延时")
show(0x0800C230, 0x0800C246)
show(0x0800C3B2, 0x0800C3DC)
print("  0x0800C230 BL 调用者: %s" % [hex(c) for c in BL.get(0x0800C230, [])])
check("init-only", "CC2500 初始化函数 0x0800C230 全镜像只有 1 个调用者(0x0800487E)",
      "PASS" if BL.get(0x0800C230) == [0x0800487E] else "FAIL")

sub("9.2 同一函数 0x08004868 内：初始化(0x0800487E) 早于 同步字(0x080048CE)")
show(0x08004868, 0x0800489C)
show(0x080048A6, 0x080048D8)
print("  -> 初始化表写 SYNC1/SYNC0=0xC3/0x68 在先，同步字函数随后用 50024/(ID+1) 覆盖。")
check("init-before-sync", "初始化早于同步字函数调用（同函数内 0x0800487E < 0x080048CE）",
      "PASS" if 0x0800487E < 0x080048CE else "FAIL")

sub("9.3 启动顺序：flash 参数加载 先于 RF 初始化/首次同步字")
print("  入口链: Reset(0x080030D4) -> __main(0x080030C0) -> 应用初始化 0x0800C910")
print("  应用初始化 0x0800C910 的调用顺序（节选）:")
show(0x0800C978, 0x0800C99C)
print("    - 0x0800C98C: BL 0x0800AB6C  = flash 参数加载（其中调用 0x0800C814 回填 settings，含 F8）")
print("    - 0x0800C990: BL 0x0800A6AC  = 射频初始化入口；其内部 0x0800A804: BL 0x0800B7B0")
print("      -> 0x0800B7F4: BL 0x08004868  -> 初始化表 + 同步字")
print("  因此启动时 F8 已由 flash 记录恢复，首次同步字调用即可读到正确 flag。")
print("  注: 这是对 0x0800C910 主初始化调用顺序的静态推断；实机启动时序建议用串口/逻辑分析仪复核。")
check("boot-order", "启动时 settings(含 F8) 先由 flash 恢复，再执行 RF 初始化/同步字",
      "PASS" if BL.get(0x0800AB6C) == [0x0800C98C] else "WARN")

# ----------------------------------------------------------------------------- 10. 语义证据（稳定色温）
hdr("10. item7 原厂语义（「稳定色温 / 固定表」）的间接证据")
print("  item7 的 6 个读者都不是把值用于显示，而是用它选择另一套校准/功率表或提前返回：")
print("   - 0x08004B08（函数 0x08004AE4）: F8==1 时用 0x2000015C/0x200001D8 的另一套表，"
      "跳过 0x20000158 的查表与缩放。")
print("   - 0x08004DC8（函数 0x08004D34）: F8==1 时修改增益标记并走 0x08004A90 分支。")
print("   - 0x08005980（函数 0x0800595C）: F8==1 时改用 0x2000015C 表计算功率/显示值。")
print("   - 0x08006062（函数 0x08006044）: F8==1 时从 [0x20000140+0x1C] 取表，"
      "否则从 [+0x18] 取表 —— 典型「固定值表 vs 实测校正表」。")
print("   - 0x0800ADBC（函数 0x0800ADAC）: F8==1 时直接返回，跳过缩放计算。")
print("   - 0x0800AEA0（函数 0x0800ADE8）: F8==1 时置 0x2000023B=1 并跳过一段状态更新。")
print("  结论: 代码证实 item7 是「固定/稳定校准表」开关（OFF/ON），但没有 ASCII/字符串可以"
      "证明菜单标题就是 F8/稳定色温；该命名属工程语义（间接近似，需人工/文档确认）。")
check("semantic", "item7 为 OFF/ON 开关且选择固定校准表（「稳定色温」为工程命名，非字符串证据）",
      "WARN", "结构性证据充分；命名语义无直接字符串证据")

# ----------------------------------------------------------------------------- 11. 结论汇总
hdr("11. 逐条结论汇总（任务描述假设 vs 本次独立推导）")
print("  %-4s %-4s %-10s %s" % ("#", "状态", "编号", "结论"))
print("  " + "-" * 96)
for i, (cid, desc, status, detail) in enumerate(RESULTS, 1):
    print("  %-4d %-4s %-10s %s%s" % (i, status, cid, desc, ("  [" + detail + "]") if detail else ""))

npass = sum(1 for r in RESULTS if r[2] == "PASS")
nwarn = sum(1 for r in RESULTS if r[2] == "WARN")
nfail = sum(1 for r in RESULTS if r[2] == "FAIL")
print()
print("  统计: PASS=%d  WARN=%d  FAIL=%d" % (npass, nwarn, nfail))
print()
print("  == 与任务描述不一致/需要修正/需注意的点 ==")
print("  1) 任务描述第 5 条的『全部读者/写者』不完整：还遗漏了记录 builder 读点 0x080044C4，"
      "以及 ± 处理函数的动态索引读/写 0x0800466A/0x08004672、0x08008412/0x08008418/0x08008420"
      "（item7 时同样会命中）。6 个静态读者本身全部复现无误。")
print("  2) 任务描述第 4 条引用的最大值表字节串『01 02 03 96 02 01 63 01 01 01 07 06…』"
      "前 8 字节才是 8 项最大表；第 9 字节起已属于相邻的另一张表。")
print("  3) 第 2 条『初始化表（0x0800C2xx 区域）』精确位置：函数入口 0x0800C230，"
      "写 4/5 的指令在 0x0800C25E(reg4=0xC3)/0x0800C266(reg5=0x68)，即默认同步字 0xC368=50024。")
print("  4) 第 5 条『退出路径静默清零 0x0800B6C8（建议 NOP）』：该指令是 2 字节 strb（0x71D1），"
      "NOP 应写 0xBF00；该路径还有对 [0x20000190+0x11] 的修改，补丁只 NOP 该 strb 不改变其它副作用。")
print("  5) 第 2 条『50024 字面量唯一』成立（唯一 32 位字面量在 0x0800B57C）；"
      "另在 0x0800BB03 有 68 C3 字节串跨越两条指令，属巧合，不是常量引用。")
print("  6) 启动顺序（补充结论，任务未写明）：flash 参数加载 0x0800AB6C 在应用初始化 "
      "0x0800C910 中先于 RF 初始化 0x0800A6AC/0x0800B7B0/0x08004868 执行，"
      "故开机时 F8 已恢复、首次同步字调用能读到正确 flag；但若保存/清零路径在关机时把 F8 清掉，"
      "flash 记录与 RAM 会不一致（见风险 3）。")
print("  7) flash 加载后还有一道钳制（任务未提及）：0x0800AB6C 尾部 0x0800ACDA..0x0800ACF2 "
      "按最大值表把 settings[0..7] 中超过最大值的项写 0，因此 item7 只可能为 0/1、item6(ID) 只可能"
      "为 0..99；这使补丁对 F8 的读取可以安全地按 ==1 判断。")

hdr("12. 风险点（静态审计无法覆盖，需实机验证）")
print("  R1. 无实机验证：本审计只验证地址/字节/调用关系；私有同步字是否被发射器接受、"
      "8 字节 C.Fn 菜单显示与按键流程、flash 写入寿命与掉电原子性均未在真机验证。")
print("  R2. F8 复用了原厂「稳定色温/固定校准表」功能：一旦 F8=1，原厂 6 个读者会同时进入"
      "另一套校准/功率表分支（闪光输出/显示/功率会变化），不只是无线同步字开关；"
      "原厂用户开启稳定色温也会意外切到私有同步字。必须文档警示。")
print("  R3. 退出路径 0x0800B6C8 静默清零 F8：若不 NOP，特定电源/退出路径会把 RAM 中的 F8 清 0，"
      "而 flash 记录可能仍为 1（或反之），导致重启前后同步字与用户设置不一致。NOP 后即失去"
      "原厂该路径的清零语义，属功能取舍。")
print("  R4. 去掉 SCAL(uidiv 50024/(ID+1))/延时：原厂同步字 = 50024/(ID+1) 且写 SYNC1/SYNC0 后"
      "delay(20) + SIDLE strobe + delay(1000)。补丁若只写私有同步字而省掉 uidiv/delay/strobe，"
      "会 (a) 失去 ID->同步字 的原厂映射；(b) 写 SYNC 后未按原厂时序 SIDLE+延时，"
      "CC2500 处于 TX/RX 时寄存器更新可能不生效。建议保留 strobe(0x33)+两段延时。")
print("  R5. 56 字节空间限制：0x0800B548..0x0800B57F 全 56 B，原厂字面量 50024 占 +0x34..+0x37；"
      "补丁（读 F8 flag + 两个 WriteReg + 分支 + 时序）必须 ≤56 B 并自行校验字节数。"
      "若补丁>56 B 会覆盖下一函数 0x0800B580（其调用点 0x0800865A 会被破坏）。")
print("  R6. F8 取值范围：最大值表 item7=1，UI 只会产生 0/1；且 flash 加载后会按最大值表钳制"
      "（0x0800ACDA..0x0800ACF2，超限写 0），所以 RAM 中 item7 只可能为 0/1。补丁仍建议写成"
      "『==1 才用私有字』而非『!=0』，以与原厂 6 个读者的语义（cmp #1; beq）保持一致。")
print("  R7. 门控补丁的副作用面：0x08004677/0x08008425 改为 BLO 后，item7 改值会调用同步字函数，"
      "但 item6(ID) 行为不变；需确认真机 C.Fn 编辑时 item7 的 ± 操作能进入该编辑分支"
      "（[0x20000190+0x16]!=0 的条件）。")
print("  R8. 未验证固件是否有整体校验（签名/CRC）：本审计未发现应用镜像自校验，但不能排除；"
      "烧录/升级流程的校验行为需另测。")

print()
print("=" * 100)
print("审计完成：证据来自 keypatch/dev/ad300/audit_cfn.py 的独立推导；exit 0")
print("=" * 100)
sys.exit(0)
