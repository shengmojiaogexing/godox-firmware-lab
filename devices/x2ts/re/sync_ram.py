# -*- coding: utf-8 -*-
"""X2T-S V1.4 ID/C.Fn RAM 变量、写入路径、RAM 空闲字节（带静态 + Unicorn 证据）。

复现：cd D:\\神牛 && tools\\python\\python.exe devices\\x2ts\\re\\sync_ram.py

证据链：
  A. 启动初始化表（0x080114A8，2 条 record）：
       rec0: 解压 0x080114C8 -> 0x20020000，输出 0x188 B（.data）
       rec1: 清零 0x20020188，长度 0x1EB0（.bss）
     -> RAM 布局：0x20020000..0x20020188=.data；0x20020188..0x20022038=.bss；栈在 0x20022038 之下。
  B. ID 的 RAM 副本与写入点（静态反汇编 + r0 来源）。
  C. RAM 引用/写覆盖集（全镜像 u32 字面量 + 线性反汇编 store 目标）-> 空闲字节候选。
  D. Unicorn：跑 __main（0x08003198）到 main 入口，验证 .data 解压 / .bss 清零各发生一次。
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


def off2(a):
    return a - BASE


def u32(a):
    return struct.unpack_from("<I", DATA, off(a))[0]


def dis(a, nbytes, label=""):
    print("---- %s 0x%08X (off 0x%05X) ----" % (label, a, off(a)))
    for ins in md.disasm(DATA[off(a):off(a) + nbytes], a):
        line = "  0x%08X  %-20s %s %s" % (ins.address, ins.bytes.hex(" "),
                                          ins.mnemonic, ins.op_str)
        if ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str:
            try:
                imm = int(ins.op_str.split("#")[1].rstrip("]"), 0)
                tgt = ((ins.address + 4) & ~3) + imm
                if BASE <= tgt <= END - 4:
                    line += "   ; [0x%08X]=0x%08X" % (tgt, u32(tgt))
            except Exception:
                pass
        print(line)
    print()


print("=" * 78)
print("X2T-S V1.4 — ID/C.Fn RAM 变量、写入路径、RAM 空闲字节")
print("=" * 78)
print("镜像大小 %d (0x%X)，真实 base 0x%08X（自证见 sync_xref.py）" % (SIZE, SIZE, BASE))
print()

# ===========================================================================
# A. .data / .bss 段来源
# ===========================================================================
print("=" * 78)
print("A. 启动初始化：.data(.188B) 解压 + .bss(0x1EB0 B) 清零")
print("=" * 78)
INIT_TABLE = 0x080114A8
print("  初始化表 @0x%08X（2 条 16B record）：" % INIT_TABLE)
records = []
for k in range(2):
    r = INIT_TABLE + k * 0x10
    w = struct.unpack_from("<IIII", DATA, off(r))
    records.append(w)
    print("    rec%d @0x%08X: [0]=0x%08X [4]=0x%08X [8]=0x%08X [C]=0x%08X"
          % (k, r, w[0], w[1], w[2], w[3]))
check(records[0][1] == 0x20020000 and records[0][2] == 0x188,
      "rec0 = 解压到 0x20020000，输出 0x188 B（.data）")
check(records[1][1] == 0x20020188 and records[1][2] == 0x1EB0,
      "rec1 = 目标 0x20020188，长度 0x1EB0（.bss 清零）")
check(records[1][3] == 0x0800DC20, "rec1 执行函数 0x0800DC20")
print()
dis(0x080031D0, 0x30, "启动初始化表执行器 0x080031D0")
dis(0x080031F4, 0x40, "rec0 解压函数 0x080031F4（RLE: ldrb/strb 循环）")
dis(0x0800DC20, 0x10, "rec1 函数 0x0800DC20（BSS 清零：stm r1!,{r0=0} 循环；r0 入参不使用）")
check(records[1][0] == END, "rec1 的 [0]=0x%08X == 镜像结束地址（memset 忽略 src，故不读 flash）"
      % records[1][0])
print()

# ===========================================================================
# B. Unicorn：跑 __main -> main，验证 .data/.bss
# ===========================================================================
print("=" * 78)
print("B. Unicorn 启动验证（__main 0x08003198 跑到 main 0x0800F348 停机）")
print("=" * 78)
try:
    from unicorn import (Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                         UC_HOOK_CODE)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC, UC_ARM_REG_CPSR)
    HAVE_UC = True
except Exception:  # noqa: BLE001
    HAVE_UC = False
    print("  unicorn 不可用，跳过")

INIT_SP = u32(BASE)
MAIN = 0x0800F348
if HAVE_UC:
    UC_FLASH = 0x08000000
    UC_FLASH_SIZE = 0x20000
    UC_RAM = 0x20000000
    UC_RAM_SIZE = 0x40000
    SENT = 0x2003F000
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_FLASH, UC_FLASH_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_write(BASE, DATA)
    # RAM 预填 0xAA，以便识别「哪些字节被启动流程写过」
    uc.mem_write(UC_RAM, b"\xaa" * 0x1000)
    for page in range(1, UC_RAM_SIZE // 0x1000):
        uc.mem_write(UC_RAM + page * 0x1000, b"\xaa" * 0x1000)
    uc.mem_write(SENT, bytes.fromhex("fee7"))
    state = {"at_main": False, "err": None}

    def hook(u, address, size_, _user):
        if address == MAIN:
            state["at_main"] = True
            u.emu_stop()
            return

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, INIT_SP)
    uc.reg_write(UC_ARM_REG_LR, SENT | 1)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    try:
        uc.emu_start(0x08003198 | 1, SENT, count=2000000)
    except Exception as exc:  # noqa: BLE001
        state["err"] = str(exc)
    check(state["err"] is None and state["at_main"], "启动流程执行到 main（%s）"
          % (state["err"] or "ok"))
    ram = bytes(uc.mem_read(UC_RAM, 0x22038))

    def r8(a):
        return ram[a - UC_RAM]

    data_head = bytes(uc.mem_read(0x20020000, 0x188))
    print("  .data @0x20020000 头 0x30 字节: %s" % data_head[:0x30].hex(" "))
    check(data_head[0x72:0x72 + 17] == bytes.fromhex("01010109011f6301630201050103020601"),
          "0x20020072 = C.Fn 最大值表（17 项）: %s" % data_head[0x72:0x72 + 17].hex(" "))
    check(r8(0x20020188) == 0 and r8(0x20022037) == 0,
          ".bss 0x20020188..0x20022037 已清零（首/末字节 0）")
    check(r8(0x20022038 - 1) == 0, "SP 前一字节 0x20022037 属于 .bss 已清零")
    # 栈区预填值应保留（.bss 末尾即 SP，栈向下增长但启动阶段未用）
    print()

# ===========================================================================
# C. ID 的 RAM 副本
# ===========================================================================
print("=" * 78)
print("C. ID 的 RAM 副本（4 个）与写入点")
print("=" * 78)
print("  C1. 0x200203E6 = C.Fn 数组[6]（ID 生效值，同步字函数入参来源）")
dis(0x0800C7F0, 0x14, "apply 函数 case 6：比较后写回 0x200203E6")
print("     写点: 0x0800C7FE strb r0,[r5,#6]（r5=0x200203E0）")
print()
print("  C2. 0x2002003E = 0x20020028+0x16（菜单编辑缓存 ID，同步字 case6 入参来源）")
dis(0x0800926A, 0x14, "菜单 inc 处理函数：+0x16 自增并回绕（0x2002003E）")
dis(0x08005CB4, 0x10, "菜单 dec 处理函数：+0x16 自减并回绕（0x2002003E）")
print()
print("  C3. 0x2002002E = 0x20020028+0x06（另一份设置字节；被 3 处代码直接寻址）")
for ref in (0x080060AC, 0x08007004, 0x08007490):
    print("     字面量 0x2002002E 出现在 0x%08X" % ref)
dis(0x08005FB0, 0x20, "flash 设置加载函数 0x08005FA0 写 0x2002002E")
print()
print("  C4. 0x20020123 = 影子缓存 [+1]（周期应用比较/更新，0x20020122 存 ch、+1 存 ID）")
dis(0x0800ADB0, 0x30, "周期应用 0x0800AD8C：比较后写回影子缓存")
print()

# ===========================================================================
# D. RAM 引用/写覆盖集 -> 空闲字节候选
# ===========================================================================
print("=" * 78)
print("D. RAM 空闲字节候选（引用集 + 写覆盖集）")
print("=" * 78)
RAM_LO, RAM_HI = 0x20000000, 0x20022038
# D1: 全镜像 u32 字面量（RAM 地址，含 thumb 变体去 1）
ram_refs = {}
for o in range(0, SIZE - 3):
    v = struct.unpack_from("<I", DATA, o)[0]
    if RAM_LO <= v < RAM_HI:
        ram_refs.setdefault(v, []).append(BASE + o)
    if RAM_LO <= (v | 1) < RAM_HI and not (v & 1):
        ram_refs.setdefault(v | 1, []).append(BASE + o)
print("  RAM 直接字面量引用地址数: %d" % len(ram_refs))

# D2: 线性反汇编 store 目标（保守：能解析出地址常量的都算）
# 数据流：ldr literal / movw+movt / adds imm 跟踪
stores = {}
insn_at = {}
for o in range(0, SIZE - 4, 2):
    ins = next(md.disasm(DATA[o:o + 4], BASE + o), None)
    if ins is not None:
        insn_at[BASE + o] = ins

regs = {}  # 全局近似（跨指令沿用），只用于粗筛
for a in sorted(insn_at):
    ins = insn_at[a]
    ops = ins.operands
    if a == BASE:  # 重新开始
        regs = {}
    if ins.mnemonic in ("ldr", "ldr.w") and len(ops) == 2 and ops[0].type == ARM_OP_REG \
            and ops[1].type == ARM_OP_MEM and ops[1].mem.base == ARM_REG_PC:
        tgt = ((a + 4) & ~3) + ops[1].mem.disp
        if BASE <= tgt <= END - 4:
            regs[ops[0].reg] = struct.unpack_from("<I", DATA, tgt - BASE)[0]
        continue
    if ins.mnemonic == "movw" and len(ops) == 2 and ops[0].type == ARM_OP_REG \
            and ops[1].type == ARM_OP_IMM:
        regs[ops[0].reg] = ops[1].imm & 0xFFFF
        continue
    if ins.mnemonic == "movt" and len(ops) == 2 and ops[0].type == ARM_OP_REG \
            and ops[1].type == ARM_OP_IMM:
        regs[ops[0].reg] = (regs.get(ops[0].reg, 0) & 0xFFFF) | ((ops[1].imm & 0xFFFF) << 16)
        continue
    if ins.mnemonic in ("adds", "add", "add.w") and len(ops) == 3 \
            and ops[0].type == ARM_OP_REG and ops[1].type == ARM_OP_REG \
            and ops[2].type == ARM_OP_IMM and ops[1].reg in regs:
        regs[ops[0].reg] = regs[ops[1].reg] + ops[2].imm
        continue
    if ins.mnemonic in ("str", "strb", "strh", "str.w", "strb.w", "strh.w") \
            and len(ops) == 2 and ops[1].type == ARM_OP_MEM and ops[1].mem.base in regs:
        tgt = regs[ops[1].mem.base] + ops[1].mem.disp
        if RAM_LO <= tgt < RAM_HI:
            stores.setdefault(tgt, []).append(a)
    # 目标寄存器被写 -> 丢弃
    for o_ in ops:
        if o_.type == ARM_OP_REG and getattr(o_, "access", 0) and (o_.access & 2):
            if ins.mnemonic not in ("cmp", "cmn", "tst", "teq"):
                regs.pop(o_.reg, None)
print("  可静态解析的 RAM store 目标数: %d" % len(stores))

# D3: 空闲区：既不在引用集 ±22B 内，也不在写集 ±4B 内，且避开栈区(< 0x20021000)
cands = []
STEP = 1
used = set()
for v in ram_refs:
    for d in range(-22, 23):
        used.add(v + d)
for v in stores:
    for d in range(-4, 5):
        used.add(v + d)
a = 0x20020188
while a < 0x20021000:
    if a not in used:
        j = a
        while j < 0x20021000 and j not in used:
            j += 1
        if j - a >= 8:
            cands.append((a, j - a))
        a = j
    else:
        a += 1
print("  候选空闲区（>=8B，避开引用/写点/栈区；.bss 内）:")
for a, ln in cands[:12]:
    print("    0x%08X..0x%08X  len %d (0x%X)  当前初值(.bss 清零后)=0" % (a, a + ln, ln, ln))
print()
# 重点检查 Lead 提示的 0x200201xx
near = [(a, ln) for a, ln in cands if 0x20020100 <= a < 0x20020200]
print("  0x200201xx 内候选: %s" % ([("0x%08X+%d" % x) for x in near] or "无（该区已被引用/写点占满）"))
print()

# ===========================================================================
# E. 结论
# ===========================================================================
print("=" * 78)
print("E. 结论")
print("=" * 78)
print("  * RAM 布局: 0x20020000..0x20020187 = .data（启动解压 0x188 B）；")
print("              0x20020188..0x20022037 = .bss（启动 memset 0x1EB0 B）；SP=0x20022038")
print("  * ID 副本: 0x200203E6(C.Fn[6] 生效 ID) / 0x2002003E(菜单缓存) /")
print("             0x2002002E(设置字节) / 0x20020123(影子缓存)")
print("  * 写路径: 菜单 inc/dec -> 0x2002003E -> apply(0x0800C790) -> 0x200203E6 -> 影子 0x20020123")
print("  * 空闲 RAM 候选见 §D（给 keypatch 的私有标志/影子字节用；建议选 .bss 内候选并在实机复核）")
print()
print("FAIL 项: %d" % len(FAIL))
for f in FAIL:
    print("  ! " + f)
raise SystemExit(0 if not FAIL else 1)
