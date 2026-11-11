# -*- coding: utf-8 -*-
"""Godox V860III-S（V860IIIS_V1.1.bin）C.Fn 设置机制 / 密钥开关 hook / settings[8] 读者屏蔽
—— 独立静态 + Unicorn 调用级验证脚本。

对应报告: keypatch/dev/v860iiis/CFN_SETTINGS.md
复现方式（PowerShell）::

    cd D:\\神牛
    tools\\python\\python.exe devices\\v860iiis\\re\\cfn_settings.py

退出码 0 = 全部断言 PASS；1 = 存在 FAIL / 异常（会把 FAIL 明细打印在最后）。
固件只读，本脚本不写固件；只做静态读取 + Unicorn 内存仿真。

验证内容
--------
1  固件身份（长度 / sha256 / 镜像末端）。
2  settings 基址 0x20001058、12 项布局、item6=ID、item8=LIGHT、最大值表 0x08016102。
3  同步字函数 0x080115B8..0x080115EF（56B）、字面量 50024、5 个 BL 调用点。
4  C.Fn 递减 handler 0x080059E4 / 递增 handler 0x0800C528：gate 角色、r0/r1 语义、
   item9(LCD) 原厂排除路径、菜单项索引范围。
5  补丁方案 (a) 2 字节 NOP 与方案 (b) BL 注入 hook 的字节/差异范围/代码洞候选。
6  settings[8]=LIGHT 原厂读者（造型灯任务 0x08003950，GPIO 0x48001000）与全部写者
   （出厂默认 / flash 记录加载 / RF 设置复位 / 上限 clamp）。
7  持久化：save builder 0x080056D8、flash 写 0x08010D94、load task 0x0800EEB0、
   展开函数 0x08013B08；record 布局中 settings[8] = record+0x36；
   flash 槽 0x0801F000 + slot*0x64（20 槽，头字节 0x5C）。
8  自动保存倒计时链路（0x080109C8 -> 0x20000516=0xC8 -> 0x080109A4 -> 0x20000518=1
   -> save task 0x0800D9C8）。
9  Unicorn 真实执行两个 handler（stub 外设/显示/延时，hook 记录 sync 调用）：
   - 原厂 vs NOP vs BL-hook，item 0..11，值编辑模式（含正常/回绕两种取值）；
   - 光标移动模式（0x200004D5=0）；
   - 提前返回路径（0x20000536=1）；
   - 每次断言：sync 次数/入参、settings 改值结果、SP 与 r3-r7 恢复、非目标字节不变；
   - item9(LCD) 必须 0 次 sync 且调用 0x080128F0。
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

# ---------------------------------------------------------------------------
# 路径 / 常量
# ---------------------------------------------------------------------------
HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))  # D:\神牛
IMAGE = os.path.join(ROOT, "devices", "v860iiis", "firmware", "V860IIIS_V1.1.bin")

BASE = 0x08003000
IMAGE_SIZE = 79300
IMAGE_END = BASE + IMAGE_SIZE                      # 0x080165C4
IMAGE_SHA256 = "bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20"

# ---- C.Fn 设置区 / 开关项（与 keypatch/devices/v860iiis.py 常量名对齐）----
SETTINGS_BASE = 0x20001058        # C.Fn 设置项数组基址（item n = base + n）
ID_IDX = 6                        # item 6 = 引闪 ID（原厂既有）
FLAG_IDX = 8                      # item 8 = LIGHT（原厂造型灯模式 0=12sec/1=OFF/2=ON）
KEY_FLAG_ADDR = SETTINGS_BASE + FLAG_IDX           # 0x20001060
KEY_FLAG_ON_VALUE = 2             # LIGHT=ON(2) -> 密钥模式；0/1 -> 原厂普通模式
CFN_ITEM_COUNT = 12               # item 0..0xB
CFN_MAX_TABLE_ADDR = 0x08016102
CFN_MAX_TABLE = bytes.fromhex("01010101011f630102060101")
CFN_MAX = tuple(CFN_MAX_TABLE)    # (1,1,1,1,1,31,99,1,2,6,1,1)
SETTINGS_BYTES_USED = 14          # save/load 覆盖 settings[0..0xD]（12 项 + 2 扩展）

# ---- 同步字函数 ----
SYNC_FN = 0x080115B8              # void rf_set_id(uint8_t id) -> CC2500 SYNC1/SYNC0
SYNC_FN_END = 0x080115F0          # 下一函数起点（Lead 更正后的边界）
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN               # 56
SYNC_FN_HEAD = bytes.fromhex("0c4a10b5411c1046f1f796fd")
ID_NUMERATOR = 50024              # 原厂 50024/(ID+1)，字面量 0xC368 @0x080115EC
SYNC_BL_CALLS = (0x08005A78, 0x08005E36, 0x0800C5C2, 0x0800D0BA, 0x0800D0E6)

# ---- C.Fn 改值 handler / gate ----
DEC_FN = 0x080059E4               # 递减 handler（键事件分发 0x080058E4 / 事件 0x78）
INC_FN = 0x0800C528               # 递增 handler（键事件分发 0x080058E4 / 事件 0xB4）
ITEM_IDX = 0x200004D6             # 当前 C.Fn 项索引（handler 从该 RAM 字节读取）
FLAG_EDIT = 0x200004D4            # [1] = C.Fn 编辑态
FLAG_SUBMODE = 0x200004D5         # [1] = 值编辑；[0] = 光标移动
FLAG_BUSY = 0x20000536            # [1] = 忙，handler 提前返回
FLAG_MODE = 0x20000563            # [3] 时走 0x0800EAE8 分支
LCD_APPLY_FN = 0x080128F0         # item9(LCD) 专用函数

# 方案 (a)：2 字节 NOP，让 gate 处 `cmp r1,#6; bne skip` 永远落入既有 sync 调用
GATE_DEC = 0x08005A74             # 原 02 d1 (bne 0x08005A7C)；新 00 bf
GATE_INC = 0x0800C5BE             # 原 f5 d1 (bne 0x0800C5AC)；新 00 bf
GATE_DEC_ORIG = bytes.fromhex("02d1")
GATE_INC_ORIG = bytes.fromhex("f5d1")
GATE_DEC_NEW = bytes.fromhex("00bf")
GATE_INC_NEW = bytes.fromhex("00bf")
# 方案 (b)：4 字节 BL 注入（V1C 式），hook 16B
GATE_DEC_BL_SITE = 0x08005A72     # 原 06 29 02 d1（cmp r1,#6; bne）
GATE_INC_BL_SITE = 0x0800C5BC     # 原 06 29 f5 d1
INJ_CAVE = 0x080162BA             # 备选代码洞（尾部数据段 0x00 padding，未证死区）
INJ_CAVE_SIZE = 16                # hook 长度

# ---- settings[8]（LIGHT）原厂读者 / 写者 ----
LIGHT_FN = 0x08003950             # 造型灯任务：GPIO 0x48001000 bit13/bit14 三态
LIGHT_READER_1 = 0x08003956       # 00 7a  ldrb r0,[r0,#8] -> 00 20 movs r0,#0
LIGHT_READER_2 = 0x0800397E       # 01 7a  ldrb r1,[r0,#8] -> 00 21 movs r1,#0
LIGHT_READER_ORIG = (bytes.fromhex("007a"), bytes.fromhex("017a"))
LIGHT_READER_NEW = (bytes.fromhex("0020"), bytes.fromhex("0021"))
LIGHT_FN_CALLER = 0x08013DB2      # 主循环任务（与 load task 0x08013D60 同函数链）
LIGHT_GPIO_POOL_VALUE = 0x48001000

DEFAULTS_FN = 0x08005E54          # 出厂默认设置（settings[0..9]；s8=0）
DEFAULTS_S8_WRITE = 0x08005EB0    # 01 72  strb r1,[r0,#8]（r1=0）
RF_RESET_S8_WRITE = 0x0800D0A2    # 26 72  strb r6,[r4,#8]（r6=0）RF 设置复位分支
RF_RESET_S8_ORIG = bytes.fromhex("2672")
RF_RESET_S8_NEW = bytes.fromhex("00bf")

# ---- 持久化 ----
SAVE_BUILDER = 0x080056D8         # 生成 72B(0x48) 记录到 0x20000FD4
SAVE_BUILDER_S8_READ = 0x08005816  # 14 7a  ldrb r4,[r2,#8]（settings[8] -> record+0x36）
SAVE_TASK = 0x0800D9C8            # 保存任务：倒计时/比对/写槽
SAVE_COMMIT = 0x08012F68          # 记录比对 + 提交到 0x20000F8C
FLASH_WRITE_FN = 0x08010D94       # 写 flash 槽：0x0801F000 + slot*0x64
FLASH_PROGRAM_FN = 0x080093A0     # 底层 flash 编程
FLASH_ERASE_MGR = 0x080092A8      # 槽管理/换页初始化
LOAD_TASK = 0x0800EEB0            # 上电加载：扫 20 槽、校验 0x5C、展开/默认/clamp
LOAD_EXPAND = 0x08013B08          # record -> settings[0..0xD]（含 settings[8] @0x08013C48）
LOAD_DEFAULT_CALL = 0x08005E54
FLASH_SLOT_BASE = 0x0801F000
FLASH_SLOT_SIZE = 0x64
FLASH_SLOT_COUNT = 0x14           # 20 槽
RECORD_HEAD = 0x5C
RECORD_SIZE = 0x48                # 72 字节 / 18 words（0x08012F68 循环 0x12 次）
S8_RECORD_OFF = 0x36              # settings[8] 在 record 中的字节偏移

# ---- 自动保存倒计时 ----
SAVE_TIMER_SET_FN = 0x080109C8    # 0x20000516 = 0xC8(200)
SAVE_TIMER_TICK_FN = 0x080109A4   # 递减；归零时 0x20000518 = 1
SAVE_TIMER_HW = 0x20000516
SAVE_PENDING = 0x20000518

# ---- Unicorn 现场 ----
UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x4000
UC_STACK = 0x20001480
UC_SENTINEL = 0x20001300
MAX_HARNESS_INSNS = 2000           # 实测单次 handler ≤ 58 条，留足余量
HARNESS_STUBS = (0x08003A48, 0x08011790, 0x080109C8,
                 0x0800EAE8, 0x080128F0, 0x08012A9E)
STUB_BX_LR = bytes.fromhex("7047")
HANDLERS = {"dec": DEC_FN, "inc": INC_FN}


# ---------------------------------------------------------------------------
# 通用工具
# ---------------------------------------------------------------------------
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_MODE_LITTLE_ENDIAN  # noqa: E402
from capstone.arm import (  # noqa: E402
    ARM_INS_BL, ARM_INS_BLX, ARM_OP_MEM, ARM_OP_REG,
    ARM_REG_R0, ARM_REG_R1, ARM_REG_R2, ARM_REG_R3,
    ARM_REG_R4, ARM_REG_R5, ARM_REG_R6, ARM_REG_R7,
)

MD = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS | CS_MODE_LITTLE_ENDIAN)
MD.detail = True

# 16 位 LDR 编码里的 Rt 是 3 位裸编号 0..7；capstone 的 reg/mem.base 用枚举值。
_RAW_TO_REG = (ARM_REG_R0, ARM_REG_R1, ARM_REG_R2, ARM_REG_R3,
               ARM_REG_R4, ARM_REG_R5, ARM_REG_R6, ARM_REG_R7)


def _off(addr):
    return addr - BASE


def u16(data, addr):
    return struct.unpack_from("<H", data, _off(addr))[0]


def u32(data, addr):
    return struct.unpack_from("<I", data, _off(addr))[0]


def hx(data, addr, n):
    return data[_off(addr):_off(addr) + n].hex(" ")


def insn_at(data, addr):
    for ins in MD.disasm(data[_off(addr):_off(addr) + 4], addr, count=1):
        return ins
    return None


def all_u32_sites(data, value):
    """2 字节对齐扫描全镜像，返回所有值等于 value 的 u32 位置（字面量池位置）。"""
    return [BASE + i for i in range(0, len(data) - 4, 2)
            if u32(data, BASE + i) == value]


def ldr_literal_sites(data, pool_locs):
    """返回 (指令地址, Rt, 字面量池地址, 池内容) —— 16 位 LDR Rt,[pc,#imm] 形式。"""
    out = []
    for i in range(0, len(data) - 2, 2):
        a = BASE + i
        hw = struct.unpack_from("<H", data, i)[0]
        if (hw & 0xF800) != 0x4800:
            continue
        rt = _RAW_TO_REG[(hw >> 8) & 7]
        pc = (a + 4) & ~3
        target = pc + (hw & 0xFF) * 4
        if target in pool_locs:
            out.append((a, rt, target, u32(data, target)))
    return out


def bl_sites(data, target):
    """全镜像 BL/BLX 目标等于 target 的指令地址。"""
    out = []
    for i in range(0, len(data) - 4, 2):
        a = BASE + i
        ins = insn_at(data, a)
        if ins is None:
            continue
        if ins.id in (ARM_INS_BL, ARM_INS_BLX) and ins.op_str.startswith("#"):
            tgt = int(ins.op_str.replace("#", ""), 16) & ~1
            if tgt == (target & ~1):
                out.append(a)
    return out


def t_ldr_literal(instr_addr, target):
    pc = (instr_addr + 4) & ~3
    imm8 = (target - pc) // 4
    assert (target - pc) % 4 == 0 and 0 <= imm8 <= 0xFF, (hex(instr_addr), hex(target))
    return struct.pack("<H", 0x4800 | (imm8 & 0xFF))


def t_bl(addr, target):
    """Thumb BL T1 编码（addr = BL 指令地址，target = 绝对地址）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target), off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = i1 if s else ((~i1) & 1)
    j2 = i2 if s else ((~i2) & 1)
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                       0xD000 | (j1 << 13) | (j2 << 11) | imm11)


def build_hook(addr):
    """方案 (b) 的 16B hook：sync(settings[6]) 后跳到 gate 之后的 pop 点（LR+6）。

    进入时 LR 指向原 gate 的 fall-through（0x08005A76 / 0x0800C5C0），
    gate 之后的 pop 点 = LR+6（0x08005A7C / 0x0800C5C6）。

        push {r0-r4,lr}      ; b51f
        ldrb r0,[r0,#6]      ; 7980
        bl   sync_fn         ; 4B
        pop  {r0-r4}         ; bc1f
        pop  {r0}            ; bc01  （弹出保存的 LR）
        adds r0,#6           ; 3006
        bx   r0              ; 4700
    """
    code = bytearray()
    code += struct.pack("<H", 0xB51F)
    code += struct.pack("<H", 0x7980)
    code += t_bl(addr + 4, SYNC_FN)
    code += struct.pack("<H", 0xBC1F)
    code += struct.pack("<H", 0xBC01)
    code += struct.pack("<H", 0x3006)
    code += struct.pack("<H", 0x4700)
    assert len(code) == INJ_CAVE_SIZE
    return bytes(code)


# ---------------------------------------------------------------------------
# 断言收集
# ---------------------------------------------------------------------------
class Checker:
    def __init__(self):
        self.passed = 0
        self.failed = 0
        self.failures = []

    def section(self, title):
        print("\n== %s ==" % title)

    def info(self, msg):
        print("INFO  " + msg)

    def check(self, ok, msg):
        if ok:
            self.passed += 1
            print("PASS  " + msg)
        else:
            self.failed += 1
            self.failures.append(msg)
            print("FAIL  " + msg)

    def eq(self, got, want, msg):
        self.check(got == want, "%s（实际=%s 期望=%s）" % (msg, _fmt(got), _fmt(want)))


def _fmt(v):
    if isinstance(v, bytes):
        return v.hex(" ")
    if isinstance(v, int):
        return "0x%X" % v if v > 9 else str(v)
    return repr(v)


# ---------------------------------------------------------------------------
# 1) 静态检查
# ---------------------------------------------------------------------------
def static_checks(chk, data):
    chk.section("1. 固件身份")
    chk.eq(len(data), IMAGE_SIZE, "镜像长度")
    chk.eq(IMAGE_END, 0x080165C4, "镜像末端 = base + size")
    chk.eq(hashlib.sha256(data).hexdigest(), IMAGE_SHA256, "sha256")
    chk.eq(u32(data, BASE), 0x20001548, "向量[0] 初值 SP")
    chk.eq(u32(data, BASE + 4) & ~1, 0x080030D4, "向量[1] Reset（Thumb 位已清）")

    chk.section("2. settings 基址 / 12 项布局 / item6=ID / item8=LIGHT / 最大值表")
    pools = all_u32_sites(data, SETTINGS_BASE)
    chk.eq(len(pools), 30, "settings 基址 0x20001058 的 u32 字面量数量")
    expected_pools = {
        0x0800367C, 0x08003A30, 0x08003DCC, 0x08004724, 0x08004B2C, 0x08004D4C,
        0x080058A4, 0x080059C0, 0x08005D58, 0x08005E50, 0x08005F20, 0x08006EA0,
        0x08007B48, 0x08007ED4, 0x08008138, 0x0800888C, 0x0800C8D8, 0x0800D15C,
        0x0800EAE4, 0x0800EB14, 0x0800EC84, 0x0800F164, 0x0800F640, 0x0800F80C,
        0x0800FAA8, 0x08011500, 0x080119B0, 0x08012958, 0x08013CD4, 0x08014030,
    }
    chk.eq(set(pools), expected_pools, "settings 字面量池位置集合与预期一致")
    settings_loads = ldr_literal_sites(data, set(pools))
    chk.eq(len(settings_loads), 42, "LDR 引用 settings 字面量池的指令数")
    chk.eq(SETTINGS_BASE + ID_IDX, 0x2000105E, "item6(ID) 地址 = settings+6")
    chk.eq(KEY_FLAG_ADDR, 0x20001060, "item8(LIGHT) 地址 = settings+8")
    # 两个 handler 里的 settings / item 索引载入点（gate 现场语义）
    chk.eq(hx(data, 0x08005A26, 2), "cc 48", "递减 handler: ldr r0,[pc,#0x330] -> settings")
    chk.eq(u32(data, 0x08005D58), SETTINGS_BASE, "递减 handler settings 字面量池 = 0x20001058")
    chk.eq(hx(data, 0x08005A38, 2), "c9 4a", "递减 handler: ldr r2,[pc,#0x324] -> &item_idx")
    chk.eq(u32(data, 0x08005D60), ITEM_IDX, "递减 handler 项索引指针 = 0x200004D6")
    chk.eq(hx(data, 0x0800C56A, 2), "db 48", "递增 handler: ldr r0,[pc,#0x36c] -> settings")
    chk.eq(u32(data, 0x0800C8D8), SETTINGS_BASE, "递增 handler settings 字面量池 = 0x20001058")
    chk.eq(hx(data, 0x0800C57C, 2), "d8 49", "递增 handler: ldr r1,[pc,#0x360] -> &item_idx")
    chk.eq(u32(data, 0x0800C8E0), ITEM_IDX, "递增 handler 项索引指针 = 0x200004D6")

    chk.eq(hx(data, CFN_MAX_TABLE_ADDR, 12), CFN_MAX_TABLE.hex(" "),
           "最大值/回绕表 0x08016102 的 12 字节")
    max_pools = all_u32_sites(data, CFN_MAX_TABLE_ADDR)
    chk.eq(set(max_pools), {0x08005D64, 0x0800C8E4, 0x0800F160}, "最大值表字面量池位置")
    max_loads = ldr_literal_sites(data, set(max_pools))
    chk.eq(sorted(a for a, _rt, _p, _v in max_loads),
           [0x08005A6A, 0x0800C586, 0x0800F086], "3 个 LDR 引用最大值表的指令")
    chk.eq(CFN_MAX, (1, 1, 1, 1, 1, 31, 99, 1, 2, 6, 1, 1), "CFN_MAX 解码")
    # 上限 clamp 函数循环上界 0xC（12 项）
    chk.eq(hx(data, 0x0800F09A, 2), "0c 28", "clamp 循环 cmp r0,#0xC（12 项）")

    chk.section("3. 同步字函数 0x080115B8（56B）与调用点")
    chk.eq(hx(data, SYNC_FN, 12), SYNC_FN_HEAD.hex(" "), "同步字函数入口 12 字节")
    chk.eq(SYNC_FN_SIZE, 56, "同步字函数长度（到下一函数 0x080115F0）")
    chk.eq(u32(data, SYNC_FN + 0x34), ID_NUMERATOR, "字面量 50024 @0x080115EC")
    chk.eq(hx(data, SYNC_FN + 0x30, 6), "57 f8 10 bd 68 c3", "函数尾 bl delay + pop {r4,pc} + 50024 字面量")
    chk.eq(tuple(bl_sites(data, SYNC_FN)), SYNC_BL_CALLS, "全镜像 BL 到同步字函数的 5 个调用点")
    # 每个调用点入参来自 settings[6]
    for call_addr in SYNC_BL_CALLS:
        prev = insn_at(data, call_addr - 2)
        chk.check(prev is not None and prev.mnemonic == "ldrb"
                  and prev.op_str in ("r0, [r0, #6]", "r0, [r4, #6]"),
                  "调用点 0x%08X 前一指令为 ldrb r0,[settings,#6]（settings[6]）" % call_addr)

    chk.section("4. C.Fn 递减/递增 handler 与 gate 角色")
    chk.eq(hx(data, DEC_FN, 2), "f8 b5", "递减 handler 0x080059E4 入口 push {r3-r7,lr}")
    chk.eq(hx(data, INC_FN, 2), "f8 b5", "递增 handler 0x0800C528 入口 push {r3-r7,lr}")
    chk.check(0x0800592E in [a for a in bl_sites(data, DEC_FN)], "递减 handler 由键事件分发 0x080058E4 调用")
    chk.check(0x08005928 in [a for a in bl_sites(data, INC_FN)], "递增 handler 由键事件分发 0x080058E4 调用")
    chk.eq(hx(data, GATE_DEC - 2, 6), "06 29 " + GATE_DEC_ORIG.hex(" ") + " 80 79",
           "递减 gate: cmp r1,#6; bne skip; ldrb r0,[r0,#6]")
    chk.eq(hx(data, GATE_INC - 2, 8), "06 29 " + GATE_INC_ORIG.hex(" ") + " 80 79 04 f0",
           "递增 gate: cmp r1,#6; bne skip; ldrb r0,[r0,#6]")
    chk.eq(hx(data, GATE_DEC - 2, 2), "06 29", "递减 gate 前 cmp #6 在 0x08005A72")
    chk.eq(hx(data, GATE_DEC + 2, 6), "80 79 0b f0 9e fd", "递减 gate 后 ldrb + bl sync")
    chk.eq(hx(data, GATE_INC + 2, 6), "80 79 04 f0 f9 ff", "递增 gate 后 ldrb + bl sync")
    # 非 item9 的强制排除（原厂在 gate 之前用 cmp #9 分到专用函数）
    chk.eq(hx(data, 0x08005A4E, 4), "09 29 15 d0", "递减：值路径 cmp r1,#9; beq LCD 专用")
    chk.eq(hx(data, 0x08005A66, 4), "09 29 09 d0", "递减：回绕路径 cmp r1,#9; beq LCD 专用")
    chk.eq(hx(data, 0x0800C596, 4), "09 2a 09 d0", "递增：回绕路径 cmp r2,#9; beq LCD 专用")
    chk.eq(hx(data, 0x0800C5B6, 4), "09 2a 06 d0", "递增：正常路径 cmp r2,#9; beq LCD 专用")
    chk.check(0x08005A7E in [a for a in bl_sites(data, LCD_APPLY_FN)], "递减 item9 分支调用 0x080128F0")
    chk.check(0x0800C5C8 in [a for a in bl_sites(data, LCD_APPLY_FN)], "递增 item9 分支调用 0x080128F0")
    # gate 现场 r0/r1 语义：r0=settings 基址、r1=当前项索引（来自 0x200004D6）
    chk.eq(hx(data, 0x08005A70, 2), "11 78", "递减 gate 前 ldrb r1,[r2]（r2=&item_idx）")
    chk.eq(hx(data, 0x0800C5BA, 2), "09 78", "递增 gate 前 ldrb r1,[r1]（r1=&item_idx）")
    # 菜单项索引范围 0..9（wrap 逻辑）
    chk.eq(hx(data, 0x08005A60, 4), "09 20 10 70", "递减：索引 0 -> 9")
    chk.eq(hx(data, 0x0800C5A6, 6), "09 2a 04 d9 0c 70", "递增：索引 +1 后 cmp #9 / 越界写 0")
    chk.eq(hx(data, 0x0800C5AA, 2), "0c 70", "递增：索引 >9 -> 0")
    chk.eq(CFN_ITEM_COUNT, 12, "settings 侧 12 项（上限表/clamp/持久化均 12+）")
    chk.info("菜单可编辑索引范围 0..9（10 行）；item10/11 无菜单改值路径，但仍在 settings/记录中"
             "（LIGHT=8、ID=6 均在可达范围）")

    chk.section("5. 补丁方案字节 / 差异范围 / 代码洞")
    nop_image = apply_nop(data)
    diff = [i for i in range(len(data)) if nop_image[i] != data[i]]
    chk.eq(sorted(diff), sorted([GATE_DEC - BASE, GATE_DEC - BASE + 1,
                                 GATE_INC - BASE, GATE_INC - BASE + 1]),
           "方案(a) 与原厂差异恰为 2 处 NOP 共 4 字节")
    chk.eq(hx(nop_image, GATE_DEC, 2), GATE_DEC_NEW.hex(" "), "方案(a) 递减 gate 改为 00 bf")
    chk.eq(hx(nop_image, GATE_INC, 2), GATE_INC_NEW.hex(" "), "方案(a) 递增 gate 改为 00 bf")
    chk.eq(len(nop_image), IMAGE_SIZE, "方案(a) 固件长度不变")
    # 方案 (b)：BL 编码 + 16B hook + 洞候选
    chk.eq(t_bl(GATE_DEC_BL_SITE, INJ_CAVE).hex(" "), hx(apply_hook(data), GATE_DEC_BL_SITE, 4),
           "方案(b) 递减 gate 的 BL 编码")
    chk.eq(t_bl(GATE_INC_BL_SITE, INJ_CAVE).hex(" "), hx(apply_hook(data), GATE_INC_BL_SITE, 4),
           "方案(b) 递增 gate 的 BL 编码")
    hook_bytes = build_hook(INJ_CAVE)
    chk.eq(hx(apply_hook(data), INJ_CAVE, INJ_CAVE_SIZE), hook_bytes.hex(" "), "方案(b) hook 16 字节")
    hook_image = apply_hook(data)
    hook_diff = [i for i in range(len(data)) if hook_image[i] != data[i]]
    allowed = set()
    for bl_site in (GATE_DEC_BL_SITE, GATE_INC_BL_SITE):
        allowed.update(range(_off(bl_site), _off(bl_site) + 4))
    allowed.update(range(_off(INJ_CAVE), _off(INJ_CAVE) + INJ_CAVE_SIZE))
    chk.eq([i for i in hook_diff if i not in allowed], [], "方案(b) 差异只落在 2 处 gate(4B×2) + hook 区(16B)")
    chk.check(len(hook_diff) >= 20, "方案(b) 实际差异 %d 字节（BL 编码与部分原字节重合）" % len(hook_diff))
    # hook 反汇编语义
    mnemonics = []
    bl_target = None
    hook_image = apply_hook(data)
    for ins in MD.disasm(hook_image[_off(INJ_CAVE):_off(INJ_CAVE) + INJ_CAVE_SIZE], INJ_CAVE):
        mnemonics.append(ins.mnemonic)
        if ins.mnemonic == "bl":
            bl_target = int(ins.op_str.replace("#", ""), 16) & ~1
    chk.eq(mnemonics, ["push", "ldrb", "bl", "pop", "pop", "adds", "bx"], "hook 反汇编指令序列")
    chk.eq(bl_target, SYNC_FN, "hook 内 BL 目标 = 同步字函数")
    # 洞候选：0x080162BA 起 16B 全 0；无真实代码分支/BL 指入；u32 绝对地址无指入
    chk.eq(data[_off(INJ_CAVE):_off(INJ_CAVE) + INJ_CAVE_SIZE], bytes(INJ_CAVE_SIZE),
           "备选洞 0x080162BA 起 16B 全 0x00")
    cave_lo, cave_hi = INJ_CAVE, INJ_CAVE + INJ_CAVE_SIZE
    branch_hits = []
    ldrhits = []
    for i in range(0, len(data) - 4, 2):
        a = BASE + i
        ins = insn_at(data, a)
        if ins is None:
            continue
        if ins.id in (ARM_INS_BL, ARM_INS_BLX) and ins.op_str.startswith("#"):
            t = int(ins.op_str.replace("#", ""), 16) & ~1
            if cave_lo <= t < cave_hi:
                branch_hits.append((a, "bl", hex(t)))
        elif ins.mnemonic.startswith("b") and ins.op_str.startswith("#"):
            t = int(ins.op_str.replace("#", ""), 16) & ~1
            if cave_lo <= t < cave_hi:
                branch_hits.append((a, ins.mnemonic, hex(t)))
        elif ins.mnemonic in ("ldr", "ldrb", "ldrh") and "[pc" in ins.op_str:
            try:
                imm = int(ins.op_str.split("#")[-1].rstrip("]"), 16)
            except ValueError:
                continue
            pool = (a + 4) & ~3
            pool += imm
            if cave_lo <= pool < cave_hi:
                ldrhits.append((a, hex(pool)))
    chk.eq(branch_hits, [], "备选洞：无 BL/分支目标指入")
    chk.eq([h for h in ldrhits if h[0] < 0x08016000], [], "备选洞：代码区无 LDR 字面量指向")
    abs_hits = [BASE + i for i in range(0, len(data) - 4, 2)
                if cave_lo <= u32(data, BASE + i) < cave_hi
                or cave_lo <= (u32(data, BASE + i) & ~1) < cave_hi]
    chk.eq(abs_hits, [], "备选洞：无 u32 绝对地址（含 Thumb 变体）指向")
    chk.info("备选洞 0x080162B9..0x08016328 为数据段尾部 0x00 padding（112B）；"
             "线性反汇编命中的 LDR 池共 %d 处（0x080161EE/0x0801624C/0x0801624E），"
             "源地址本身位于数据区（>0x08016000），属数据误码，不能据此证明死区；"
             "正式补丁采用方案 (a)，不需要代码洞。" % len(ldrhits))

    chk.section("6. settings[8]=LIGHT 原厂读者（造型灯任务）/ 写者 / 屏蔽点")
    chk.eq(hx(data, LIGHT_FN, 2), "f8 b5", "造型灯任务 0x08003950 入口 push {r3-r7,lr}")
    chk.eq(u32(data, 0x08003A34), LIGHT_GPIO_POOL_VALUE, "造型灯任务使用 GPIO 基址 0x48001000（LIGHT 同 V1C）")
    chk.eq(hx(data, LIGHT_FN_CALLER, 4), "ef f7 cd fd", "主循环调用造型灯任务（0x08013DB2）")
    chk.eq(hx(data, LIGHT_READER_1, 2), LIGHT_READER_ORIG[0].hex(" "), "LIGHT 读者 1 原字节 00 7a")
    chk.eq(hx(data, LIGHT_READER_2, 2), LIGHT_READER_ORIG[1].hex(" "), "LIGHT 读者 2 原字节 01 7a")
    chk.eq(hx(data, 0x0800395E, 2), "01 28", "读者 1 后 cmp r0,#1（LIGHT=OFF 分支）")
    chk.eq(hx(data, 0x08003982, 2), "02 29", "读者 2 后 cmp r1,#2（LIGHT=ON 分支）")
    light_fixed = apply_reader_masks(data)
    chk.eq(hx(light_fixed, LIGHT_READER_1, 2), LIGHT_READER_NEW[0].hex(" "), "读者 1 屏蔽为 movs r0,#0")
    chk.eq(hx(light_fixed, LIGHT_READER_2, 2), LIGHT_READER_NEW[1].hex(" "), "读者 2 屏蔽为 movs r1,#0")
    chk.eq(hx(light_fixed, RF_RESET_S8_WRITE, 2), RF_RESET_S8_NEW.hex(" "),
           "RF 设置复位写 settings[8] 处 NOP 保护")
    # 静态污点扫描：settings[n] 全部立即数/动态索引访问
    access = taint_settings_accesses(data)
    item_reads = {}
    item_writes = {}
    dyn = set()
    for addr, (kind, imm, idx) in access.items():
        if idx:
            dyn.add((kind, addr))
        elif 0 <= imm < SETTINGS_BYTES_USED:
            (item_reads if kind == "R" else item_writes).setdefault(imm, set()).add(addr)
    chk.eq(sorted(item_reads.get(FLAG_IDX, set())),
           [0x08003956, 0x0800397E, 0x08004A8A, 0x08004A9A, 0x08005816],
           "settings[8] 全部静态读者（造型灯 x2 / 菜单显示 x2 / save builder）")
    chk.eq(sorted(item_writes.get(FLAG_IDX, set())),
           [0x08005EB0, 0x0800D0A2, 0x08013C48],
           "settings[8] 全部静态写者（出厂默认 / RF 复位 / flash 记录加载）")
    chk.eq(sorted(dyn),
           sorted([("R", 0x08005A42), ("W", 0x08005A4A),
                   ("R", 0x0800C588), ("W", 0x0800C58E), ("W", 0x0800C59A),
                   ("R", 0x0800F08C), ("W", 0x0800F094)]),
           "settings 动态索引访问点（两个改值 handler + clamp）")
    chk.eq(hx(data, DEFAULTS_FN + 4, 2), "00 21", "出厂默认函数中 r1=0（随后写 settings[8]=0）")
    chk.eq(hx(data, DEFAULTS_S8_WRITE, 2), "01 72", "出厂默认写 settings[8] 原字节")
    chk.eq(hx(data, RF_RESET_S8_WRITE, 2), RF_RESET_S8_ORIG.hex(" "), "RF 复位写 settings[8] 原字节 26 72")
    # clamp：settings[i] 只有在 > max 时才归 0；LIGHT max=2，密钥值 2 合法
    chk.eq(hx(data, 0x0800F08C, 8), "15 5c 1f 5c bd 42 00 d9", "clamp: 读 settings[i]/max[i] 并比较")
    chk.eq(CFN_MAX[FLAG_IDX], KEY_FLAG_ON_VALUE, "LIGHT max=2 == 密钥模式值（clamp 不会清除）")

    chk.section("7. 持久化：save / flash / load 是否覆盖 settings[8]")
    chk.eq(hx(data, SAVE_BUILDER, 4), "10 b5 5d 49", "save builder 0x080056D8 入口")
    chk.eq(u32(data, 0x080058A4), SETTINGS_BASE, "save builder 内 settings 字面量")
    chk.eq(hx(data, SAVE_BUILDER_S8_READ, 2), "14 7a", "save builder 读 settings[8]（ldrb r4,[r2,#8]）")
    chk.eq(hx(data, 0x08005820, 2), "44 63", "save builder 把 s6..s9 组写入 record+0x34")
    chk.eq(S8_RECORD_OFF, 0x34 + 2, "settings[8] 在 record 内偏移 = 0x36")
    chk.eq(RECORD_SIZE, 0x48, "record 长度 72 字节（0x08012F68 循环 0x12 words）")
    chk.eq(hx(data, 0x08012F86, 2), "12 29", "0x08012F68 比对/提交 0x12 words")
    chk.eq(hx(data, SAVE_TASK + 6, 4), "0f 48 01 78", "save task 读 0x20000518 待保存标志")
    chk.eq(u32(data, 0x0800DA14), 0x20000FD4, "save builder 目标缓冲 = 0x20000FD4")
    chk.eq(hx(data, 0x0800D9E4, 4), "f7 f7 78 fe", "save task 调 save builder")
    chk.eq(bl_sites(data, SAVE_BUILDER), [0x0800D9E4, 0x08010D9A], "save builder 两个调用点")
    chk.eq(hx(data, FLASH_WRITE_FN, 2), "70 b5", "flash 写函数 0x08010D94 入口")
    chk.eq(u32(data, 0x08010DC8), FLASH_SLOT_BASE, "flash 槽基址 0x0801F000")
    chk.eq(hx(data, 0x08010DB4, 2), "64 20", "槽大小 0x64（muls r4,r0）")
    chk.eq(hx(data, 0x08010DBC, 4), "f8 f7 f0 fa", "flash 写函数调 0x080093A0 编程")
    chk.eq(bl_sites(data, FLASH_WRITE_FN), [0x0800DA06, 0x0800F112], "flash 写函数调用点（save 任务 / load 任务）")
    chk.eq(hx(data, LOAD_TASK, 2), "f8 b5", "load task 0x0800EEB0 入口")
    chk.eq(hx(data, 0x0800EEB8, 2), "14 20", "load task 槽数 0x14（20）")
    chk.eq(hx(data, 0x0800EEC6, 2), "64 21", "load task 槽大小 0x64")
    chk.eq(u32(data, 0x0800F12C), FLASH_SLOT_BASE, "load task 槽基址 0x0801F000")
    chk.eq(hx(data, 0x0800EED6, 2), "5c 28", "load task 校验记录头 0x5C")
    chk.eq(hx(data, 0x0800EEEC, 4), "04 f0 0c fe", "load task 调 0x08013B08 展开 settings")
    chk.eq(hx(data, 0x0800EF2E, 4), "f6 f7 91 ff", "load task 无效记录调 0x08005E54 出厂默认")
    chk.eq(hx(data, LOAD_EXPAND, 4), "5d 48 5c 49", "展开函数 0x08013B08 取记录/工作缓冲")
    chk.eq(u32(data, 0x08013CD4), SETTINGS_BASE, "展开函数 settings 字面量")
    chk.eq(hx(data, 0x08013C3A, 2), "4b 6b", "展开函数读 record+0x34（s6..s9 组）")
    chk.eq(hx(data, 0x08013C48, 2), "13 72", "展开函数写 settings[8]（ldrb r3,[r0,#2]; strb r3,[r2,#8]）")
    chk.info("结论：settings[8] 随 72B 记录写入 flash 槽（record+0x36），上电由 load task 恢复；"
             "出厂默认/恢复出厂写 0（=普通模式），属于预期语义。")

    chk.section("8. 自动保存倒计时（只影响何时落盘，不影响 settings[8] 内容）")
    chk.eq(hx(data, SAVE_TIMER_SET_FN, 8), "01 49 c8 20 08 80 70 47", "0x080109C8: 0x20000516 = 0xC8")
    chk.eq(hx(data, SAVE_TIMER_TICK_FN, 8), "06 48 01 88 00 29 07 d0", "0x080109A4: 递减 0x20000516")
    chk.eq(hx(data, 0x080109B6, 6), "03 49 01 20 08 70", "归零时写 0x20000518 = 1")
    chk.eq(bl_sites(data, SAVE_TIMER_SET_FN),
           [0x080059F6, 0x0800C53A, 0x0800CF26, 0x08010E0C],
           "倒计时重置点（两个 C.Fn handler 入口 + RF handler + 另一路径）")
    return nop_image, light_fixed


def apply_nop(data):
    """方案 (a)：两个 gate 的 bne -> nop。"""
    buf = bytearray(data)
    for site, want, new in ((GATE_DEC, GATE_DEC_ORIG, GATE_DEC_NEW),
                            (GATE_INC, GATE_INC_ORIG, GATE_INC_NEW)):
        got = bytes(buf[_off(site):_off(site) + 2])
        if got != want:
            raise ValueError("gate 0x%08X 原字节不符：%s != %s" % (site, got.hex(" "), want.hex(" ")))
        buf[_off(site):_off(site) + 2] = new
    return bytes(buf)


def apply_hook(data):
    """方案 (b)：两个 gate（cmp+bne 4B）-> BL cave，并写 16B hook。"""
    buf = bytearray(data)
    for site, bl_site in ((GATE_DEC, GATE_DEC_BL_SITE), (GATE_INC, GATE_INC_BL_SITE)):
        want = bytes.fromhex("0629") + (GATE_DEC_ORIG if site == GATE_DEC else GATE_INC_ORIG)
        got = bytes(buf[_off(bl_site):_off(bl_site) + 4])
        if got != want:
            raise ValueError("gate 0x%08X 原字节不符：%s != %s" % (bl_site, got.hex(" "), want.hex(" ")))
        buf[_off(bl_site):_off(bl_site) + 4] = t_bl(bl_site, INJ_CAVE)
    if bytes(buf[_off(INJ_CAVE):_off(INJ_CAVE) + INJ_CAVE_SIZE]) != bytes(INJ_CAVE_SIZE):
        raise ValueError("备选洞 0x%08X 不是全 0" % INJ_CAVE)
    buf[_off(INJ_CAVE):_off(INJ_CAVE) + INJ_CAVE_SIZE] = build_hook(INJ_CAVE)
    return bytes(buf)


def apply_reader_masks(data):
    """读者屏蔽 + RF 复位写者保护（方案 (a) 的配套补丁）。"""
    buf = bytearray(data)
    for site, orig, new in ((LIGHT_READER_1, LIGHT_READER_ORIG[0], LIGHT_READER_NEW[0]),
                            (LIGHT_READER_2, LIGHT_READER_ORIG[1], LIGHT_READER_NEW[1]),
                            (RF_RESET_S8_WRITE, RF_RESET_S8_ORIG, RF_RESET_S8_NEW)):
        got = bytes(buf[_off(site):_off(site) + 2])
        if got != orig:
            raise ValueError("补丁点 0x%08X 原字节不符：%s != %s" % (site, got.hex(" "), orig.hex(" ")))
        buf[_off(site):_off(site) + 2] = new
    return bytes(buf)


def taint_settings_accesses(data):
    """对每个 settings 字面量载入点做前向线性污点，收集 [settings + imm] / [settings + reg] 访问。

    返回 {指令地址: (kind, imm, index_reg)}；kind = "R"/"W"。
    说明：这是面向 Thumb-1 函数体的保守线性扫描（遇到 pop pc / bx lr 结束），
    在 V860IIIS 上与原厂代码风格匹配；结果在报告 §4 中人工复核过。
    """
    pool_locs = set(all_u32_sites(data, SETTINGS_BASE))
    loads = ldr_literal_sites(data, pool_locs)
    access = {}
    for site, rt, _pool, _val in loads:
        regs = {rt: SETTINGS_BASE}
        a = site + 2
        end = min(site + 0x400, IMAGE_END)
        while a < end:
            ins = insn_at(data, a)
            if ins is None:
                a += 2
                continue
            ops = ins.operands
            if ins.mnemonic in ("ldrb", "ldrh", "ldr", "ldrsb", "ldrsh",
                                "strb", "strh", "str"):
                if len(ops) >= 2 and ops[1].type == ARM_OP_MEM:
                    base = ops[1].mem.base
                    if base in regs and regs[base] == SETTINGS_BASE:
                        imm = int(ops[1].mem.disp)
                        idx = int(ops[1].mem.index)
                        kind = "R" if ins.mnemonic.startswith("ldr") else "W"
                        access[a] = (kind, imm, idx)
            try:
                _reads, writes = ins.regs_access()
            except Exception:  # noqa: BLE001
                writes = []
            handled = set()
            if ins.mnemonic in ("mov", "movs") and len(ops) == 2 \
                    and ops[0].type == ARM_OP_REG and ops[1].type == ARM_OP_REG:
                regs[ops[0].reg] = regs.get(ops[1].reg)
                handled.add(ops[0].reg)
            elif ins.mnemonic == "ldr" and "[pc" in ins.op_str and ops[0].type == ARM_OP_REG:
                try:
                    imm = int(ins.op_str.split("#")[-1].rstrip("]"), 16)
                except ValueError:
                    imm = None
                if imm is not None:
                    val = u32(data, ((a + 4) & ~3) + imm)
                    regs[ops[0].reg] = val if val == SETTINGS_BASE else None
                    handled.add(ops[0].reg)
            for r in writes:
                if r not in handled and r not in ("sp", "pc"):
                    regs.pop(r, None)
            if ins.mnemonic == "pop" and "pc" in ins.op_str:
                break
            if ins.mnemonic == "bx" and ins.op_str == "lr":
                break
            a += ins.size
    return access


# ---------------------------------------------------------------------------
# 10) Unicorn handler 级测试
# ---------------------------------------------------------------------------
def _uconst():
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_R4, UC_ARM_REG_R5,
                                   UC_ARM_REG_R6, UC_ARM_REG_R7, UC_ARM_REG_SP,
                                   UC_ARM_REG_LR, UC_ARM_REG_PC)
    return dict(R0=UC_ARM_REG_R0, R1=UC_ARM_REG_R1, R2=UC_ARM_REG_R2, R3=UC_ARM_REG_R3,
                R4=UC_ARM_REG_R4, R5=UC_ARM_REG_R5, R6=UC_ARM_REG_R6, R7=UC_ARM_REG_R7,
                SP=UC_ARM_REG_SP, LR=UC_ARM_REG_LR, PC=UC_ARM_REG_PC)


R = _uconst()
SENTINELS = {R["R3"]: 0x1003, R["R4"]: 0x1004, R["R5"]: 0x1005,
             R["R6"]: 0x1006, R["R7"]: 0x1007}


class HandlerMachine:
    """在一个 Uc 实例里反复执行 C.Fn handler。

    ROM 里把外设/显示/延时/除法/保存倒计时函数入口改成 `bx lr`，
    同步字函数入口也改成 `bx lr`，用 UC_HOOK_CODE 记录调用并入参。
    """

    def __init__(self, firmware, mode):
        from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_PROT_ALL, UC_HOOK_CODE
        self.firmware = firmware
        self.mode = mode
        image = bytearray(firmware)
        for addr in HARNESS_STUBS + (SYNC_FN,):
            image[_off(addr):_off(addr) + 2] = STUB_BX_LR
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
        self.uc.mem_map(UC_ROM, UC_ROM_SIZE, UC_PROT_ALL)
        self.uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
        self.uc.mem_write(BASE, bytes(image))
        self.sync_calls = []
        self.stub_calls = []
        self.returned = False
        self.error = None
        self.insn = 0
        self._loads = 0
        self.uc.hook_add(UC_HOOK_CODE, self._hook_code)

    # ---- 运行 ----
    def run(self, handler, item, value, edit=True, early=False):
        from unicorn import UcError
        uc = self.uc
        uc.mem_write(UC_RAM, bytes(UC_RAM_SIZE))
        uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))       # b .
        uc.mem_write(SETTINGS_BASE, bytes(16))
        uc.mem_write(SETTINGS_BASE + ID_IDX, bytes([37]))      # 默认 ID=37
        if edit:
            # 值编辑模式：settings[item]=测试初值（item==6 时即 ID 初值）
            uc.mem_write(SETTINGS_BASE + item, bytes([value]))
        uc.mem_write(FLAG_EDIT, bytes([1]))
        uc.mem_write(FLAG_SUBMODE, bytes([1 if edit else 0]))
        uc.mem_write(FLAG_BUSY, bytes([1 if early else 0]))
        uc.mem_write(FLAG_MODE, bytes([0]))
        uc.mem_write(ITEM_IDX, bytes([item]))
        self.sync_calls = []
        self.stub_calls = []
        self.returned = False
        self.error = None
        self.insn = 0
        uc.reg_write(R["SP"], UC_STACK)
        uc.reg_write(R["LR"], UC_SENTINEL | 1)
        uc.reg_write(R["R0"], SETTINGS_BASE)
        uc.reg_write(R["R1"], item)
        uc.reg_write(R["R2"], 0x1002)
        for reg, val in SENTINELS.items():
            uc.reg_write(reg, val)
        try:
            uc.emu_start(HANDLERS[handler] | 1, 0, 0, MAX_HARNESS_INSNS)
        except UcError as exc:
            self.error = "%s pc=0x%08X" % (exc, uc.reg_read(R["PC"]))
        settings = bytes(uc.mem_read(SETTINGS_BASE, 16))
        return {
            "sync": list(self.sync_calls),
            "stubs": list(self.stub_calls),
            "special_lcd": LCD_APPLY_FN in self.stub_calls,
            "returned": self.returned,
            "error": self.error,
            "insn": self.insn,
            "sp_ok": uc.reg_read(R["SP"]) == UC_STACK,
            "regs_ok": all(uc.reg_read(reg) == val for reg, val in SENTINELS.items()),
            "settings": settings,
            "item_idx": bytes(uc.mem_read(ITEM_IDX, 1))[0],
        }

    # ---- hook ----
    def _hook_code(self, uc, address, _size, _user):
        self.insn += 1
        if address == UC_SENTINEL:
            self.returned = True
            uc.emu_stop()
            return
        if address == SYNC_FN:
            self.sync_calls.append(uc.reg_read(R["R0"]))
            uc.reg_write(R["PC"], uc.reg_read(R["LR"]))
            return
        if address in HARNESS_STUBS:
            self.stub_calls.append(address)
            if address == 0x0800EAE8:
                uc.mem_write(FLAG_MODE, bytes([0]))   # 模拟原函数清 mode
            uc.reg_write(R["PC"], uc.reg_read(R["LR"]))
            return


def _expected_new_value(handler, item, value):
    """handler 修改后的 settings[item] 期望值（含 item9 专用分支的差异）。"""
    if handler == "dec":
        if value > 0:
            return value - 1
        return 0 if item == 9 else CFN_MAX[item]   # item9 回绕分支直接转 LCD 专用函数
    if value < CFN_MAX[item]:
        return value + 1
    return CFN_MAX[item] if item == 9 else 0       # item9 回绕分支恢复 max 后转 LCD 专用函数


def _expected_edit_sync(mode, item):
    if mode == "orig":
        return 1 if item == ID_IDX else 0
    return 0 if item == 9 else 1


def unicorn_checks(chk, firmware):
    """handler 级实测：原厂 / NOP / BL-hook 三态。"""
    chk.section("9. Unicorn handler 级实测（原厂 vs 方案(a) NOP vs 方案(b) BL-hook）")
    variants = {
        "orig": firmware,
        "nop": apply_nop(firmware),
        "hook": apply_hook(firmware),
    }
    machines = {name: HandlerMachine(img, name) for name, img in variants.items()}
    chk.info("Unicorn 现场：ROM=0x08000000+0x20000, RAM=0x20000000+0x4000, SP=0x%08X, "
             "哨兵=0x%08X, 指令上限=%d" % (UC_STACK, UC_SENTINEL, MAX_HARNESS_INSNS))

    # ---- 9.1 值编辑模式：item 0..11，正常 + 回绕 ----
    base_settings = bytearray(16)
    base_settings[ID_IDX] = 37
    for handler in ("dec", "inc"):
        for item in range(CFN_ITEM_COUNT):
            cases = []
            # 正常值：dec 用 max（ID 用 50），inc 用 0
            if handler == "dec":
                normal = 50 if item == ID_IDX else CFN_MAX[item]
            else:
                normal = 0
            cases.append(("正常", normal))
            # 回绕值：dec 用 0，inc 用 max
            if handler == "dec":
                wrap = 0
            else:
                wrap = CFN_MAX[item]
            if wrap != normal:
                cases.append(("回绕", wrap))
            for case_name, value in cases:
                results = {mode: machines[mode].run(handler, item, value)
                           for mode in ("orig", "nop", "hook")}
                exp_settings = bytearray(base_settings)
                exp_settings[item] = _expected_new_value(handler, item, value)

                def _ok_one(mode):
                    res = results[mode]
                    exp_sync = _expected_edit_sync(mode, item)
                    cond = (res["returned"] and res["error"] is None and res["sp_ok"]
                            and res["regs_ok"] and res["insn"] <= MAX_HARNESS_INSNS
                            and len(res["sync"]) == exp_sync
                            and res["settings"] == bytes(exp_settings)
                            and res["item_idx"] == item)
                    if exp_sync:
                        cond = cond and res["sync"][0] == res["settings"][ID_IDX]
                    if item == 9:
                        cond = cond and res["special_lcd"] and not res["sync"]
                    return cond

                chk.check(all(_ok_one(m) for m in ("orig", "nop", "hook")),
                          "%s item=%-2d %s v=%-2d：原厂 sync=%d / NOP sync=%d / hook sync=%d，"
                          "改值后 settings[%d]=%d%s"
                          % (handler, item, case_name, value,
                             len(results["orig"]["sync"]), len(results["nop"]["sync"]),
                             len(results["hook"]["sync"]), item,
                             results["nop"]["settings"][item],
                             "（item9 走 0x080128F0 且不 sync）" if item == 9 else ""))

    # ---- 9.2 光标移动模式（0x200004D5=0）----
    for handler in ("dec", "inc"):
        for item in range(10):
            results = {mode: machines[mode].run(handler, item, 0, edit=False)
                       for mode in ("orig", "nop", "hook")}
            if handler == "dec":
                exp_idx = item - 1 if item > 0 else 9
                exp_orig = 1 if item == 7 else 0
                exp_nop = 0 if item == 0 else 1
            else:
                exp_idx = item + 1 if item < 9 else 0
                exp_orig = 1 if item == 5 else 0
                exp_nop = 1 if item <= 7 else 0
            exp_special = (handler == "dec" and item == 0) or (handler == "inc" and item == 8)
            ok = True
            for mode, exp in (("orig", exp_orig), ("nop", exp_nop), ("hook", exp_nop)):
                res = results[mode]
                cond = (res["returned"] and res["error"] is None and res["sp_ok"]
                        and res["regs_ok"] and len(res["sync"]) == exp
                        and res["item_idx"] == exp_idx
                        and res["settings"] == bytes(base_settings))
                if exp:
                    cond = cond and res["sync"][0] == base_settings[ID_IDX]
                if exp_special:
                    cond = cond and res["special_lcd"] and not res["sync"]
                ok = ok and cond
            chk.check(ok, "%s 光标移动 item=%-2d：新索引=%d，原厂 sync=%d / NOP sync=%d / hook sync=%d"
                      % (handler, item, exp_idx, len(results["orig"]["sync"]),
                         len(results["nop"]["sync"]), len(results["hook"]["sync"])))

    # ---- 9.3 提前返回（0x20000536=1）非改动路径 ----
    for handler in ("dec", "inc"):
        for mode in ("orig", "nop", "hook"):
            res = machines[mode].run(handler, 0, 0, edit=True, early=True)
            ok = (res["returned"] and res["error"] is None and res["sp_ok"] and res["regs_ok"]
                  and not res["sync"] and res["settings"] == bytes(base_settings)
                  and res["item_idx"] == 0)
            chk.check(ok, "%s [%s] 忙标志提前返回：0 次 sync、settings 不变、栈/寄存器恢复"
                      % (handler, mode))

    # ---- 9.4 方案 (a) 与 (b) 行为等价 ----
    same = True
    for handler in ("dec", "inc"):
        for item in range(CFN_ITEM_COUNT):
            for value in (0, CFN_MAX[item]):
                a = machines["nop"].run(handler, item, value)
                b = machines["hook"].run(handler, item, value)
                if (a["sync"] != b["sync"] or a["settings"] != b["settings"]
                        or a["item_idx"] != b["item_idx"] or a["returned"] != b["returned"]):
                    same = False
    chk.check(same, "方案(b) BL-hook 与方案(a) NOP 在全部 item/取值上行为等价")
    return machines


# ---------------------------------------------------------------------------
# 可选：与 Lead 的 keypatch 模块对照（信息性，不计入 PASS/FAIL）
# ---------------------------------------------------------------------------
def cross_check_lead_module(chk):
    path = os.path.join(ROOT, "keypatch", "devices", "v860iiis.py")
    if not os.path.exists(path):
        chk.info("keypatch/devices/v860iiis.py 不存在，跳过对照")
        return
    text = open(path, "r", encoding="utf-8", errors="replace").read()
    keys = ["0x080115B8", "0x20001058", "0x20001060", "0x08016102",
            "0x08005A74", "0x0800C5BE", "0x08003956", "0x0800397E", "0x0800D0A2"]
    found = [k for k in keys if k in text]
    chk.info("对照 Lead 模块 keypatch/devices/v860iiis.py：%d/%d 个关键地址出现（%s）"
             % (len(found), len(keys), ", ".join(found)))


# ---------------------------------------------------------------------------
# main
# ---------------------------------------------------------------------------
def main():
    chk = Checker()
    if not os.path.exists(IMAGE):
        print("FAIL  固件不存在: %s" % IMAGE)
        return 1
    data = open(IMAGE, "rb").read()
    nop_image, light_fixed = static_checks(chk, data)
    try:
        unicorn_checks(chk, data)
    except ImportError as exc:
        chk.check(False, "unicorn 不可用，无法完成 handler 级验证: %s" % exc)
    cross_check_lead_module(chk)

    print("\n" + "=" * 78)
    print("总结：PASS %d / FAIL %d" % (chk.passed, chk.failed))
    if chk.failed:
        print("失败明细：")
        for msg in chk.failures:
            print("  - " + msg)
        return 1
    print("全部断言 PASS（退出码 0）")
    return 0


if __name__ == "__main__":
    sys.exit(main())
