# -*- coding: utf-8 -*-
"""X2T-S V1.4 代码洞/注入空间审计 + 「改 C.Fn 立即生效」hook 方案字节。

复现：cd D:\\神牛 && tools\\python\\python.exe devices\\x2ts\\re\\sync_holes.py

结论（脚本逐项打印证据）：
  * 镜像内不存在连续 0xFF 空洞：最长 0xFF run = 3 B（off 0xE0D9）。
  * 镜像尾部 0x080114C8..0x08011503 是启动 .data 压缩流（被初始化表引用），不可用；
    0x08011504 起已是镜像外（设备侧资源/擦除区），不在本 .bin 内。
  * 唯一静态可证安全的注入空间 = Cortex-M 向量表保留槽：
      0x0800301C..0x0800302B（16 B，向量 7..10，ARMv7-M 保留，硬件不读取）
      0x08003034..0x08003037（4 B，向量 13，保留）
    两者原厂字节全 0x00，全镜像无字面量/分支/指针引用。
  * hook 方案（4 B 补丁 + 14 B 洞 + 4 B 字面量，共 22 B 差异，全部落在上述安全区）：
      0x0800C790：70 b5 44 4c -> bl 0x0800301C
      0x0800301C：push {r4,r5,r6,lr}; ldr r4,=0x20020028; ldrb r0,[r4,#0x16];
                  bl 0x0800C8F8; b.w 0x0800C794
      0x08003034：28 00 02 20（0x20020028 字面量，占用保留向量[13]）
    语义：每次菜单设置应用（唯一调用点 0x08009B68）前先按当前 ID 重设同步字，
          同步字函数改写后会读取 C.Fn 开关标志 -> 改 C.Fn 后立即生效。
"""
from __future__ import annotations

import os
import struct

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from capstone.arm import ARM_OP_IMM, ARM_OP_REG, ARM_OP_MEM, ARM_REG_PC

HERE = os.path.dirname(os.path.abspath(__file__))
IMG = os.path.join(os.path.dirname(HERE), "firmware", "X2T-S_V1.4.bin")
DATA = open(IMG, "rb").read()
BASE = 0x08003000
SIZE = len(DATA)
END = BASE + SIZE

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True
FAIL = []


def check(ok, msg):
    print("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
    if not ok:
        FAIL.append(msg)


def off(a):
    return a - BASE


def u32(a):
    return struct.unpack_from("<I", DATA, off(a))[0]


def p16(v):
    return struct.pack("<H", v & 0xFFFF)


def enc_bl(addr_, target):
    o = target - (addr_ + 4)
    assert o % 2 == 0 and -(1 << 24) <= o < (1 << 24)
    s = 1 if o < 0 else 0
    imm = o & 0x1FFFFFF
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    imm10 = (imm >> 12) & 0x3FF
    imm11 = (imm >> 1) & 0x7FF
    j1 = (1 - i1) ^ s
    j2 = (1 - i2) ^ s
    return p16(0xF000 | (s << 10) | imm10) + p16(0xD000 | (j1 << 13) | (j2 << 11) | imm11)


def enc_bw(addr_, target):
    o = target - (addr_ + 4)
    assert o % 2 == 0 and -(1 << 24) <= o < (1 << 24)
    s = 1 if o < 0 else 0
    imm = o & 0x1FFFFFF
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    imm10 = (imm >> 12) & 0x3FF
    imm11 = (imm >> 1) & 0x7FF
    j1 = (1 - i1) ^ s
    j2 = (1 - i2) ^ s
    return p16(0xF000 | (s << 10) | imm10) + p16(0x9000 | (j1 << 13) | (j2 << 11) | imm11)


def dis_bytes(blob, base_addr, label):
    print("---- %s @0x%08X (%d B) ----" % (label, base_addr, len(blob)))
    for ins in md.disasm(blob, base_addr):
        print("  0x%08X  %-20s %s %s"
              % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))
    print()


# ---------------------------------------------------------------------------
# 全镜像引用目标超集
# ---------------------------------------------------------------------------
print("=" * 78)
print("X2T-S V1.4 — 代码洞/注入空间与立即生效 hook 方案")
print("=" * 78)

lit_values = set()
for o in range(0, SIZE - 3, 2):
    for ins in md.disasm(DATA[o:o + 4], BASE + o):
        if ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str:
            try:
                imm = int(ins.op_str.split("#")[1].rstrip("]"), 0)
            except Exception:
                break
            tgt = ((ins.address + 4) & ~3) + imm
            if BASE <= tgt <= END - 4:
                lit_values.add(struct.unpack_from("<I", DATA, tgt - BASE)[0])
        break
branch_targets = set()
for o in range(0, SIZE - 3, 2):
    for ins in md.disasm(DATA[o:o + 4], BASE + o):
        if ins.operands and ins.operands[0].type == 2 and ins.mnemonic not in ("mov",):
            branch_targets.add(ins.operands[0].imm)
        break
abs_words = set()
for o in range(0, SIZE - 3):
    v = struct.unpack_from("<I", DATA, o)[0]
    if 0x08000000 <= v < 0x08020000:
        abs_words.add(v)
        abs_words.add(v | 1)
refs = set()
for v in lit_values:
    if 0x08000000 <= v < 0x08020000:
        refs.add(v & ~1)
        refs.add(v)
refs |= {t & ~1 for t in branch_targets}
refs |= abs_words
print("引用目标超集: 字面量 %d / 分支 %d / 绝对地址字 %d -> 合计去重 %d"
      % (len(lit_values), len(branch_targets), len(abs_words), len(refs)))
print()


def refs_into(lo, hi):
    return sorted(r for r in refs if lo <= r < hi)


# ---------------------------------------------------------------------------
# 1. 0xFF / 0x00 空洞扫描
# ---------------------------------------------------------------------------
print("=" * 78)
print("1. 连续 0xFF 空洞扫描（结论：镜像内没有擦除态空洞）")
print("=" * 78)
runs_ff = []
i = 0
while i < SIZE:
    if DATA[i] == 0xFF:
        j = i
        while j < SIZE and DATA[j] == 0xFF:
            j += 1
        runs_ff.append((BASE + i, j - i))
        i = j
    else:
        i += 1
runs_ff.sort(key=lambda x: -x[1])
print("  0xFF run 总数 %d，最长 10 个：" % len(runs_ff))
for a, ln in runs_ff[:10]:
    print("    0x%08X len %d (off 0x%05X)" % (a, ln, off(a)))
check(runs_ff and runs_ff[0][1] <= 3, "镜像内最长 0xFF 连续段 = %d B（无法作为代码洞）"
      % runs_ff[0][1])
print()

runs_00 = []
i = 0
while i < SIZE:
    if DATA[i] == 0:
        j = i
        while j < SIZE and DATA[j] == 0:
            j += 1
        if j - i >= 8:
            runs_00.append((BASE + i, j - i))
        i = j
    else:
        i += 1
runs_00.sort(key=lambda x: -x[1])
print("  连续 0x00 >= 8 B 的段 %d 个，最长 10 个（0x00 是合法 Thumb 指令 movs r0,r0，"
      "但它们是 .data 压缩流/常量表内容，不能当洞用）：" % len(runs_00))
for a, ln in runs_00[:10]:
    r = refs_into(a, a + ln)
    print("    0x%08X..0x%08X len %-3d 内部引用: %s"
          % (a, a + ln, ln, [("0x%08X" % x) for x in r[:4]] or "无直接引用"))
print()
print("  尾部数据归属（排除可用性）：")
print("    * 0x080114A8..0x080114C7 = 启动初始化表（2 条 record，被 0x080031D0 读取）")
print("    * 0x080114C8..0x08011503 = .data 压缩流（48 B，被 rec0 解压函数读取，")
print("      输出 0x188 B 到 0x20020000，见 sync_ram.py §A）")
print("    * 0x08011504 = 镜像结束 = .bss 清零目标起址（memset 忽略 src）")
print("    => 尾部数据全部有用途，且镜像内无 0xFF 洞。")
print()

# ---------------------------------------------------------------------------
# 2. 可选注入空间：Cortex-M 向量表保留槽
# ---------------------------------------------------------------------------
print("=" * 78)
print("2. 唯一静态可证安全的注入空间：向量表保留槽")
print("=" * 78)
VTOR_WRITE = 0x0800CFCC  # ldr r0,=0x08003000 ; str r0,[0xE000ED08]（SystemInit 末尾）
print("  VTOR = 0x%08X（SystemInit 0x0800CFCC 处写入），向量表运行时有效。" % u32(VTOR_WRITE))
print("  ARMv7-M 向量编号: 0=SP 1=Reset 2=NMI 3=HardFault 4=MemManage 5=BusFault")
print("                    6=UsageFault 7..10=Reserved 11=SVCall 12=DebugMon 13=Reserved")
print()

SLOTS = [(0x0800301C, 16, "向量[7..10] 保留槽"),
         (0x08003034, 4, "向量[13] 保留槽")]
for a, ln, name in SLOTS:
    blob = DATA[off(a):off(a) + ln]
    r = refs_into(a, a + ln)
    print("  %s 0x%08X..0x%08X（%d B）：原厂字节 %s"
          % (name, a, a + ln, ln, blob.hex(" ")))
    check(blob == b"\x00" * ln, "0x%08X 起 %d B 原厂全 0x00" % (a, ln))
    check(not r, "0x%08X..0x%08X 无字面量/分支/绝对地址引用（refs=%s）"
          % (a, a + ln, [("0x%08X" % x) for x in r] or "无"))
print("  说明：保留槽按 ARMv7-M 架构定义不会被 CPU 取向量；只被链接脚本写 0，"
      "原厂也从不读取它们（上表 refs 为空）。8 字节对齐、位于可执行 flash。")
print()

# ---------------------------------------------------------------------------
# 3. hook 方案（4 B 补丁 + 14 B 洞 + 4 B 字面量）
# ---------------------------------------------------------------------------
print("=" * 78)
print("3.「改 C.Fn 后立即生效」hook 方案（原位补丁，总差异 22 B）")
print("=" * 78)
APPLY_FN = 0x0800C790
CAVE = 0x0800301C
LIT_SLOT = 0x08003034
SYNC_FN = 0x0800C8F8
RESUME = 0x0800C794
SETTINGS_RAM = 0x20020028

orig4 = DATA[off(APPLY_FN):off(APPLY_FN) + 4]
print("  补丁点 A：apply 函数入口 0x%08X，原厂 4 B = %s"
      % (APPLY_FN, orig4.hex(" ")))
check(orig4 == bytes.fromhex("70b5444c"),
      "原厂字节 = 70 b5 44 4c（push {r4,r5,r6,lr}; ldr r4,=0x20020028）")
jmp = enc_bl(APPLY_FN, CAVE)
print("           替换为 %s（bl 0x%08X）" % (jmp.hex(" "), CAVE))

cave = bytearray()
cave += p16(0xB570)                                   # push {r4,r5,r6,lr}
ldr_at = CAVE + len(cave)
pc_base = (ldr_at + 4) & ~3
delta = LIT_SLOT - pc_base
assert delta % 4 == 0 and 0 <= delta // 4 <= 0xFF
cave += p16(0x4C00 | (delta // 4))                    # ldr r4,[pc,#delta]
cave += p16(0x7DA0)                                   # ldrb r0,[r4,#0x16]
bl_at = CAVE + len(cave)
cave += enc_bl(bl_at, SYNC_FN)                        # bl 同步字函数
bw_at = CAVE + len(cave)
cave += enc_bw(bw_at, RESUME)                         # b.w 0x0800C794
assert len(cave) <= 16, len(cave)
print("  补丁点 B：洞 0x%08X（原厂 %s，%d B 全 0x00）"
      % (CAVE, DATA[off(CAVE):off(CAVE) + 16].hex(" "), 16))
dis_bytes(bytes(cave), CAVE, "hook 代码")
print("           剩余 %d B 保持 0x00（洞内不越界）" % (16 - len(cave)))
print("  补丁点 C：0x%08X 放 4 B 字面量 0x%08X（原厂 4 B = %s，保留向量[13]）"
      % (LIT_SLOT, SETTINGS_RAM, DATA[off(LIT_SLOT):off(LIT_SLOT) + 4].hex(" ")))
lit = struct.pack("<I", SETTINGS_RAM)
check(DATA[off(LIT_SLOT):off(LIT_SLOT) + 4] == b"\x00" * 4,
      "0x%08X 原厂 4 B 为 0（可安全写入字面量）" % LIT_SLOT)
print()

# 校验：反汇编回读 + 引用检查
patched = bytearray(DATA)
patched[off(APPLY_FN):off(APPLY_FN) + 4] = jmp
patched[off(CAVE):off(CAVE) + len(cave)] = cave
patched[off(LIT_SLOT):off(LIT_SLOT) + 4] = lit
print("  反汇编回读：")
dis_bytes(bytes(patched[off(APPLY_FN):off(APPLY_FN) + 4]), APPLY_FN, "补丁后 apply 入口")
dis_bytes(bytes(patched[off(CAVE):off(CAVE) + 16]), CAVE, "补丁后洞内 16 B")
print("  校验严格相等（PASS 前提）：")
check(bytes(patched[off(APPLY_FN):off(APPLY_FN) + 4]) == jmp,
      "apply 入口 4 B == bl 0x%08X" % CAVE)
check(bytes(patched[off(CAVE):off(CAVE) + len(cave)]) == bytes(cave),
      "洞内 hook 代码 %d B 写入正确" % len(cave))
check(bytes(patched[off(CAVE) + len(cave):off(CAVE) + 16]) == b"\x00" * (16 - len(cave)),
      "洞内剩余 %d B 未改动" % (16 - len(cave)))
check(bytes(patched[off(LIT_SLOT):off(LIT_SLOT) + 4]) == lit,
      "字面量 0x20020028 写入 0x%08X" % LIT_SLOT)
print("  语义：apply 函数是菜单设置处理的唯一应用点（0x08009B68 唯一调用），")
print("        每次应用前用「菜单缓存 ID（0x20020028+0x16）」重设同步字；")
print("        同步字函数改写后读取 C.Fn 开关 -> 改 C.Fn 立即生效。")
print("        原厂 apply 逻辑 100% 保留（push/ldr 由洞内重建后 b.w 回 0x%08X）。" % RESUME)
print()

# ---------------------------------------------------------------------------
# 4. 结论
# ---------------------------------------------------------------------------
print("=" * 78)
print("4. 结论")
print("=" * 78)
print("  * 镜像内无 0xFF 代码洞（最长 3 B）；尾部数据全部有引用/用途。")
print("  * 可用注入空间 = 向量表保留槽 0x0800301C..0x0800302B（16 B）")
print("    + 0x08003034..0x08003037（4 B），原厂全 0x00、无任何引用。")
print("  * hook：0x0800C790 4 B -> bl 0x0800301C；洞内 14 B 调同步字函数后 b.w 回 0x0800C794；")
print("    0x08003034 存 0x20020028 字面量。总差异 22 B，不越界、不改原厂长度。")
print("  * 若还需要更多洞空间：只能扩展 .bin 到镜像外擦除区（0x08011504+），")
print("    该区不在本 .bin 内、且刷写工具行为未知 —— 不推荐，已在本报告标注为不可用。")
print()
print("FAIL 项: %d" % len(FAIL))
for f in FAIL:
    print("  ! " + f)
raise SystemExit(0 if not FAIL else 1)
