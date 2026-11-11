# -*- coding: utf-8 -*-
"""Godox AD300 Pro V1.30 —— 私有密钥（CC2500 16 位硬件同步字）补丁模块 —— C.Fn「F8」开关版。

用户可见语义
------------
灯端 **C.Fn 第 8 项 F8（原厂为「稳定色温模式」OFF/ON）** 被复用为
**私有密钥开关**：开关值就是 C.Fn 设置区字节 **settings[7] = RAM 0x20000493**
（`settings` 基址 `0x2000048C`；随原厂参数记录持久化，记录 +0x1C 字的最高字节，
保存/加载见下文证据）。

* F8 = ON  -> CC2500 同步字 = 私有 KEY（只有打了补丁的引闪器能引闪）
* F8 = OFF -> 同步字 = 50024 // (ID + 1)（原厂普通模式，ID 菜单恢复原厂含义；出厂默认）

把原厂「ID -> 16 位同步字」函数 0x0800B548 整段改写为：

    flag = *(uint8_t *)0x20000493;      // C.Fn F8 密钥开关（1=ON）
    if (flag == 1) sync = KEY;          // 密钥模式
    else           sync = 50024 / (id + 1);  // 原厂普通模式
    WriteReg(4, sync >> 8);             // SYNC1
    WriteReg(5, sync & 0xFF);           // SYNC0
    return;                             // 只改同步字：无 SCAL、无延时

实现（原位字节补丁，文件长度不变 44,220 B）
------------------------------------------
1. **同步字函数整段改写**：0x0800B548 起 **56 字节**（到下一函数 0x0800B580），
   布局与 AD200 Pro / V1C 相同：44 B 代码 + KEY@+0x2C + FLAG@+0x30 + 50024@+0x34
   （50024 与原厂同址同值，普通模式公式用）。
2. **改值即时生效 hook**：原厂只有改 ID（C.Fn item 6）才重设同步字。两个 +/- 处理
   函数里的 `cmp rX,#6; bne return` 门限，把 bne 条件字节改成 blo（`item >= 6` 才
   重设）——于是 item 6(ID) 与 item 7(F8) 改值后都立即重设同步字，item 0..5 保持
   原厂行为：
   - C.Fn 递减 handler 0x080045E8 内：0x08004674 `06 2a e0 d1` -> `06 2a e0 d3`
   - C.Fn 递增 handler 0x08008388 内：0x08008422 `06 28 de d1` -> `06 28 de d3`
3. **F8 原厂功能读者屏蔽**：复用 F8 后，原厂「稳定色温模式」的 6 处功能读者全部
   原位改成常数 0（`ldrb rX,[base,#7]` -> `movs rX,#0`，2 字节；等价原厂 F8=OFF
   分支，F8=OFF 时与原厂逐指令行为一致）：
   - 0x08004B08 显示/数值表：F8=1 不切稳定色温备用时长表
   - 0x08004DC8 显示页：F8=1 不改页面/位标志
   - 0x08005980 测量/待机路径：F8=1 不切备用表
   - 0x08006062 level 查表：F8=1 不切备用标定表
   - 0x0800ADBC muldiv/缩放：F8=1 不改功率/时长缩放
   - 0x0800AEA0 RF 远程设置：F8=1 不改设置分叉
   保留 C.Fn 渲染函数（0x0800421C 读 settings[7] 显示 ON/OFF 图标）与参数记录
   builder（0x080044C4）不变。
4. **防静默清零**：退出/模式切换路径 0x0800B6C8 会 `strb 0,[settings+7]`（原厂用于
   关闭稳定色温），复用后这会悄悄关掉密钥开关；原位 NOP（`d1 71` -> `00 bf`）。

补丁点复核（证据与可复跑脚本见 keypatch/dev/ad300/）
----------------------------------------------------
* 镜像基址 `0x08003000`：向量表初始 SP=`0x200008F8`、Reset=`0x080030D5`；
  app 镜像 `0x08003000..0x0800DCBB`（44,220 B）。
* 全镜像 50024（0xC368）字面量**只出现一次**，在 0x0800B57C（= 本函数 +0x34）。
* 只有本函数与 CC2500 初始化表（0x0800C2xx 区域，早于本函数执行）写 SYNC1/SYNC0。
* 本函数 4 个调用点（0x0800467A / 0x080048CE / 0x08008428 / 0x08008872）入参都是
  settings[6]（当前 ID）。
* C.Fn 共 **8 项**（item 0..7，+/- handler 里 item 0↔7 回绕）；item 6 = ID，
  item 7 = F8；最大值表 0x0800DA64 前 8 字节 = `01 02 03 96 02 01 63 01`
  （item7 最大=1 => 二进制 OFF/ON；item6 最大=0x63=99 => ID 0..99）。
* 设置持久化：参数记录 builder 0x08004410 把 settings[0..3] 写记录 +0x18、
  settings[4..7] 写记录 +0x1C（最高字节即 F8）；保存函数 0x0800B38C 写
  flash `0x0801F000 + slot*100`；加载函数 0x0800C814 把记录回填 settings。
* 辅助函数：__aeabi_uidiv=0x080030F0、CC2500_WriteReg=0x0800C47A、
  strobe=0x0800C53C、delay_us=0x0800C200。

风险：无实机验证；去掉原厂 SCAL(0x33)+1ms 延时是相对原厂的唯一射频差异（只改同步
数字匹配字，不改频率寄存器，理论上不需要重校准；若实机首包异常优先怀疑此处）。
F8 原厂「稳定色温模式」功能被复用后固定为出厂 OFF 行为。换密钥需重新生成并刷入。
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

# 允许 `import common`（keypatch 目录），与 keypatch.py 的加载方式一致。
_HERE = os.path.dirname(os.path.abspath(__file__))
_KEYPATCH_DIR = os.path.dirname(_HERE)
if _KEYPATCH_DIR not in sys.path:
    sys.path.insert(0, _KEYPATCH_DIR)

import common  # noqa: E402

DEVICE = {
    "id": "ad300",
    "name": "Godox AD300 Pro",
    "kind": "flash",                     # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "AD300 Pro",
    "firmware_version": "V1.30",
    "image": "devices/ad300pro/firmware/AD300pro_V1.30.bin",
    "base": 0x08003000,
    "sha256": "e3be6a63581adb2b3d3225286706ee14d185f8a8ddd18c50ea50069a98765932",
    "out_name": "AD300pro_V1.30_key{key:04X}.bin",
    "container_name": "AD300Pro_V1.30_key{key:04X}_container.bin",
    "notes": "C.Fn 第 8 项 F8 密钥开关(settings[7]=0x20000493)：ON(1) -> SYNC=私有 KEY；"
             "OFF(0，出厂默认) -> 原厂 50024/(ID+1)。原位 56 B 整函数改写 + F8/ID 改值即时生效"
             "（bne->blo 门限）+ 屏蔽 6 处稳定色温读者 + 阻断退出路径静默清零",
}

# ---------------------------------------------------------------- 镜像与目标函数
BASE = 0x08003000                     # 镜像链接/加载基址（文件偏移 = 地址 - BASE）
ORIGINAL_SIZE = 44220                 # AD300pro_V1.30.bin 长度
IMAGE_END = BASE + ORIGINAL_SIZE      # 0x0800DCBC

SYNC_FUNC = 0x0800B548                # void rf_set_id(uint8_t id)：写 CC2500 SYNC1/SYNC0
SYNC_FUNC_SIZE = 56
SYNC_FUNC_END = 0x0800B580            # 下一函数起点（0x0800B580 起是另一函数；不可越过）
assert SYNC_FUNC + SYNC_FUNC_SIZE == SYNC_FUNC_END

LITERAL_KEY_ADDR = SYNC_FUNC + 0x2C   # 0x0800B574：私有密钥字面量
LITERAL_FLAG_ADDR = SYNC_FUNC + 0x30  # 0x0800B578：F8 开关地址字面量
LITERAL_ORIG_ADDR = SYNC_FUNC + 0x34  # 0x0800B57C：50024（与原厂同址同值）

# 原厂 0x0800B548..0x0800B57F 的 56 字节（apply 前逐字节校验，防固件版本不对）
ORIGINAL_FUNC = bytes.fromhex(
    "0c4a10b5411c1046f7f7cefd0446010a042000f08effe1b2052000f08aff"
    "142000f04afe332000f0e5ff7d20c00000f043fe10bd68c30000")
assert len(ORIGINAL_FUNC) == SYNC_FUNC_SIZE

# ---------------------------------------------------------------- C.Fn 设置区 / 开关项
SETTINGS_BASE = 0x2000048C            # C.Fn 设置项数组基址（item n = base + n）
SETTINGS_SIZE = 8                     # C.Fn 共 8 项（item 0..7）
FLAG_IDX = 7                          # item 7 = C.Fn 第 8 项 F8（原厂稳定色温模式）
KEY_FLAG_ADDR = SETTINGS_BASE + FLAG_IDX   # 0x20000493
KEY_FLAG_ON_VALUE = 1                 # F8=ON（值 1）-> 密钥模式；0 -> 原厂公式
ID_IDX = 6                            # item 6 = 引闪 ID（原厂既有，0..99）

# C.Fn 项最大值/回绕表（0x0800DA64；前 8 项 = 本机 C.Fn 项 0..7）
CFN_MAX_TABLE_ADDR = 0x0800DA64
CFN_MAX_TABLE = bytes.fromhex("0102039602016301")
CFN_ITEM_COUNT = 8
assert len(CFN_MAX_TABLE) == CFN_ITEM_COUNT
assert CFN_MAX_TABLE[ID_IDX] == 99 and CFN_MAX_TABLE[FLAG_IDX] == 1

# 辅助函数（verify 里替换为记录/跳过桩）
DIV_HELPER = 0x080030F0               # __aeabi_uidiv: r0 = r0 / r1
HELPER_REG_WRITE = 0x0800C47A         # CC2500_WriteReg(reg=r0, value=r1)
HELPER_STROBE = 0x0800C53C            # strobe(cmd=r0)（补丁版不再调用 SCAL）
HELPER_DELAY = 0x0800C200             # delay_us(n=r0)（补丁版不再调用）

ID_NUMERATOR = common.ID_NUMERATOR    # 50024
TEST_IDS = (0, 1, 5, 37, 99)

# ---------------------------------------------------------------- 改值即时生效 hook
# 原厂 +/- handler 只在 item==ID(6) 才重设同步字（cmp rX,#6; bne return）。
# 把 bne 条件字节 0xd1 改成 blo 0xd3：item<6 才跳过，item 6(ID)/7(F8) 改值后立即重设。
# 每项：(地址, 原厂 4 字节, 补丁后 4 字节, 说明)
ITEM_GATES = (
    (0x08004674, b"\x06\x2a\xe0\xd1", b"\x06\x2a\xe0\xd3",
     "C.Fn 递减 handler：cmp r2,#6; bne -> blo（item>=6 即 ID/F8 改值后立即重设同步字）"),
    (0x08008422, b"\x06\x28\xde\xd1", b"\x06\x28\xde\xd3",
     "C.Fn 递增 handler：cmp r0,#6; bne -> blo（item>=6 即 ID/F8 改值后立即重设同步字）"),
)

# ---------------------------------------------------------------- F8 原厂功能读者屏蔽
# 原厂「稳定色温模式」的 6 处功能读者（settings[7]==1 时切备用时长/能量/标定表、
# 改显示、改 RF 远程设置分叉）。复用 F8 当密钥开关后把这些读取原位改成常数 0
# （`ldrb rX,[base,#7]` -> `movs rX,#0`，2 字节），等价于原厂 F8=OFF 分支；
# F8=OFF（出厂默认）时与原厂逐指令行为一致。
READER_MASK_PATCHES = (
    (0x08004B08, b"\xc9\x79", b"\x00\x21",
     "显示/数值表：F8=1 不切稳定色温备用时长表"),
    (0x08004DC8, b"\xc0\x79", b"\x00\x20",
     "显示页：F8=1 不改页面标志/清 UI bit4"),
    (0x08005980, b"\xc9\x79", b"\x00\x21",
     "测量/待机路径：F8=1 不切备用表"),
    (0x08006062, b"\xd2\x79", b"\x00\x22",
     "level 查表：F8=1 不切备用标定表"),
    (0x0800ADBC, b"\xdb\x79", b"\x00\x23",
     "muldiv/缩放：F8=1 不改功率/时长缩放"),
    (0x0800AEA0, b"\xd2\x79", b"\x00\x22",
     "RF 远程设置：F8=1 不改设置分叉"),
)

# 退出/模式切换路径会静默把 settings[7] 清 0（原厂用于关掉稳定色温）；复用后
# 这会悄悄关掉密钥开关，原位 NOP 掉该写（`strb r1,[r2,#7]` -> nop）。
SILENT_CLEAR = (0x0800B6C8, b"\xd1\x71", b"\x00\xbf",
                "退出/模式切换路径：不再静默清 settings[7]（防密钥开关被关）")

# ---------------------------------------------------------------- Unicorn 内存布局
UC_FLASH = 0x08000000
UC_FLASH_SIZE = 0x20000               # 128 KiB flash
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x4000                  # 16 KiB RAM（初始 SP=0x200008F8，够用）
UC_STACK = 0x20000E00                 # 栈顶（8 字节对齐）
UC_SENTINEL = 0x20000F00              # 函数返回落点，放 `b .`
MAX_HARNESS_INSNS = 40000

# Unicorn 调用级测试用：C.Fn handler / UI 状态 / 参数记录
UI_BASE = 0x20000190                  # C.Fn/按键 UI 状态结构（两个 +/- handler 的 r4）
UI_EDIT = UI_BASE + 0x15              # ==1：值编辑态
UI_CHANGE = UI_BASE + 0x16            # !=0：真正改值（==0 时 +/- 是切换 C.Fn 项）
UI_ITEM = UI_BASE + 0x17              # 当前 C.Fn 项 0..7
UI_MODE_FLAG = 0x20000105             # ==0 走 C.Fn 输入路径
CFN_DEC_HANDLER = 0x080045E8          # C.Fn/按键 递减 handler
CFN_INC_HANDLER = 0x08008388          # C.Fn/按键 递增 handler
RECORD_BUILDER = 0x08004410           # 参数记录 builder：settings[4..7] -> record+0x1C
RECORD_LOAD = 0x0800C814              # 参数加载：record(+0x18/+0x1C) -> settings
RECORD_BUF = 0x20000440               # 参数记录 RAM 缓冲（保存/加载共用）
RECORD_WORK = 0x20000460              # builder 输出的工作记录
LEVEL_LOOKUP = 0x08006044             # u16 level_lookup()：F8=1 时切备用标定表
UI_CLEAR_FN = 0x0800B670              # 退出/模式切换路径（原厂会清 settings[7]）


# ---------------------------------------------------------------------------
# Thumb-1 指令编码（与 AD200 Pro / V1C 模块同一套已验证编码器）
# ---------------------------------------------------------------------------
def _t_ldr_literal(rt: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x4800 | ((rt & 7) << 8) | (imm8 & 0xFF))


def _t_push_r4lr() -> bytes:
    return struct.pack("<H", 0xB510)


def _t_pop_r4pc() -> bytes:
    return struct.pack("<H", 0xBD10)


def _t_movs_imm(rd: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x2000 | ((rd & 7) << 8) | (imm8 & 0xFF))


def _t_mov(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0x4600 | ((rm & 7) << 3) | (rd & 7))


def _t_lsrs(rd: int, rm: int, imm5: int) -> bytes:
    return struct.pack("<H", 0x0800 | ((imm5 & 0x1F) << 6) | ((rm & 7) << 3) | (rd & 7))


def _t_uxtb(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0xB2C0 | ((rm & 7) << 3) | (rd & 7))


def _t_bl(addr: int, target: int) -> bytes:
    """Thumb BL T1 编码（addr = 本 BL 指令地址，target = 绝对地址）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target), off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    # I1 = NOT(J1 XOR S)  ->  J1 = NOT(I1) XOR S
    j1 = (i1 if s else ((~i1) & 1))
    j2 = (i2 if s else ((~i2) & 1))
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                       0xD000 | (j1 << 13) | (j2 << 11) | imm11)


def _ldr_lit_here(rt: int, instr_addr: int, target: int) -> bytes:
    """按当前指令地址直接编码 LDR(literal)，避免依赖汇编器状态。"""
    pc = (instr_addr + 4) & ~3
    imm8 = (target - pc) // 4
    assert (target - pc) % 4 == 0 and 0 <= imm8 <= 0xFF, (hex(target), hex(pc), imm8)
    return _t_ldr_literal(rt, imm8)


# ---------------------------------------------------------------------------
# 补丁体构造
# ---------------------------------------------------------------------------
def build_sync_function(key: int) -> bytes:
    """生成 56 字节替换函数体：44 B 代码 + KEY/FLAG/50024 三个字面量。

    布局（相对 0x0800B548）::

        +00 push {r4, lr}
        +02 ldr  r3, =FLAG(0x20000493)   ; 字面量 +0x30
        +04 ldr  r2, =50024              ; 字面量 +0x34（原厂同址）
        +06 ldrb r3, [r3]                ; F8 开关
        +08 adds r1, r0, #1              ; 普通模式除数 = ID+1
        +0A cmp  r3, #0
        +0C beq  go                      ; F8=OFF -> 原厂公式
        +0E ldr  r2, =KEY                ; 字面量 +0x2C
        +10 movs r1, #1                  ; 密钥模式除数 = 1
        +12 go: mov r0, r2
        +14 bl   __aeabi_uidiv
        +18 mov  r4, r0
        +1A lsrs r1, r0, #8              ; SYNC1
        +1C movs r0, #4
        +1E bl   CC2500_WriteReg
        +22 uxtb r1, r4                  ; SYNC0
        +24 movs r0, #5
        +26 bl   CC2500_WriteReg
        +2A pop  {r4, pc}
        +2C .word KEY
        +30 .word FLAG
        +34 .word 50024                  ; 与原厂同址同值
    """
    key = common.validate_key(key)
    orig_lit = SYNC_FUNC + 0x34
    assert orig_lit + 4 == SYNC_FUNC_END

    code = bytearray()
    code += _t_push_r4lr()                                    # +00
    code += _ldr_lit_here(3, SYNC_FUNC + len(code), LITERAL_FLAG_ADDR)   # +02
    code += _ldr_lit_here(2, SYNC_FUNC + len(code), LITERAL_ORIG_ADDR)   # +04
    code += struct.pack("<H", 0x781B)                          # +06 ldrb r3,[r3]
    code += struct.pack("<H", 0x1C41)                          # +08 adds r1,r0,#1
    code += struct.pack("<H", 0x2B01)                          # +0A cmp r3,#1（严格 ==1）
    code += struct.pack("<H", 0xD101)                          # +0C bne go(+0x12)：非 1 -> 原厂公式
    code += _ldr_lit_here(2, SYNC_FUNC + len(code), LITERAL_KEY_ADDR)    # +0E
    code += struct.pack("<H", 0x2101)                          # +10 movs r1,#1
    code += _t_mov(0, 2)                                       # +12 mov r0,r2
    code += _t_bl(SYNC_FUNC + len(code), DIV_HELPER)           # +14
    code += _t_mov(4, 0)                                       # +18
    code += _t_lsrs(1, 0, 8)                                   # +1A
    code += _t_movs_imm(0, 4)                                  # +1C
    code += _t_bl(SYNC_FUNC + len(code), HELPER_REG_WRITE)     # +1E
    code += _t_uxtb(1, 4)                                      # +22
    code += _t_movs_imm(0, 5)                                  # +24
    code += _t_bl(SYNC_FUNC + len(code), HELPER_REG_WRITE)     # +26
    code += _t_pop_r4pc()                                      # +2A
    assert len(code) == 0x2C, len(code)
    blob = bytes(code) + struct.pack("<III", key, KEY_FLAG_ADDR, ID_NUMERATOR)
    assert len(blob) == SYNC_FUNC_SIZE, len(blob)
    assert blob[0x34:0x38] == ORIGINAL_FUNC[0x34:0x38]          # 50024 同址同值
    return blob


# ---------------------------------------------------------------------------
# 打补丁
# ---------------------------------------------------------------------------
def _off(addr: int) -> int:
    return addr - BASE


def _hex(data: bytes) -> str:
    return data.hex(" ")


def _root() -> str:
    """工作区根目录（由本文件位置推导，不写死盘符）。"""
    return os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def _resolve_image() -> str:
    return os.path.join(_root(), DEVICE["image"])


def apply(data: bytes, key: int, full: bool = True) -> tuple[bytes, list[dict]]:
    """把原厂镜像打上 C.Fn F8 密钥开关补丁，返回 (patched, patch_log)。

    full=True（默认，正式成品）：同步字函数 + ID/F8 改值即时生效 + 屏蔽 6 处
    稳定色温读者 + 阻断退出路径静默清零。
    full=False（安全阶梯用）：只做同步字函数 + 改值即时生效（不屏蔽原厂功能读者）。

    原位字节补丁，输出长度不变；修改前先校验长度、sha256 与每处原始字节。
    """
    key = common.validate_key(key)
    if len(data) != ORIGINAL_SIZE:
        raise ValueError("原厂固件长度应为 %d 字节，实际 %d（固件版本不对？）"
                         % (ORIGINAL_SIZE, len(data)))
    got_sha = hashlib.sha256(data).hexdigest()
    if got_sha != DEVICE["sha256"]:
        raise ValueError("原厂固件 sha256 不匹配，拒绝打补丁\n  期望 %s\n  实际 %s"
                         % (DEVICE["sha256"], got_sha))

    off = _off(SYNC_FUNC)
    before = data[off:off + SYNC_FUNC_SIZE]
    if before != ORIGINAL_FUNC:
        raise ValueError("同步字函数 0x%08X..0x%08X 原始字节不匹配（固件版本不对？）\n"
                         "  期望 %s\n  实际 %s"
                         % (SYNC_FUNC, SYNC_FUNC_END - 1,
                            _hex(ORIGINAL_FUNC), _hex(before)))
    blob = build_sync_function(key=key)
    out = bytearray(data)
    out[off:off + SYNC_FUNC_SIZE] = blob

    log: list[dict] = [{
        "addr": SYNC_FUNC, "offset": off,
        "before": _hex(before[:0x2C]),
        "after": _hex(blob[:0x2C]),
        "desc": "整段改写同步字函数 44B：C.Fn F8 开关 RAM[0x%08X]==1 -> SYNC=KEY 0x%04X，"
                "==0 -> 50024/(ID+1)；只写 SYNC1/0（无 SCAL、无延时）"
                % (KEY_FLAG_ADDR, key),
    }, {
        "addr": LITERAL_KEY_ADDR, "offset": off + 0x2C,
        "before": _hex(before[0x2C:0x30]),
        "after": _hex(blob[0x2C:0x30]),
        "desc": "字面量池：私有密钥 0x%04X（小端）" % key,
    }, {
        "addr": LITERAL_FLAG_ADDR, "offset": off + 0x30,
        "before": _hex(before[0x30:0x34]),
        "after": _hex(blob[0x30:0x34]),
        "desc": "字面量池：密钥开关地址 0x%08X（settings[7]，C.Fn F8）" % KEY_FLAG_ADDR,
    }, {
        "addr": LITERAL_ORIG_ADDR, "offset": off + 0x34,
        "before": _hex(before[0x34:0x38]),
        "after": _hex(blob[0x34:0x38]),
        "desc": "字面量池：50024（与原厂同址同值，普通模式公式用）",
    }]

    # ---- 改值即时生效 hook ----
    for addr, want, patched_bytes, desc in ITEM_GATES:
        o = _off(addr)
        got = bytes(out[o:o + len(want)])
        if got != want:
            raise ValueError("即时生效补丁点 0x%08X 原始字节不符（固件版本不对？）\n"
                             "  期望 %s\n  实际 %s" % (addr, _hex(want), _hex(got)))
        out[o:o + len(patched_bytes)] = patched_bytes
        log.append({"addr": addr, "offset": o,
                    "before": _hex(want), "after": _hex(patched_bytes),
                    "desc": desc})

    # ---- full：屏蔽稳定色温读者 + 阻断静默清零 ----
    if full:
        for addr, want, patched_bytes, desc in list(READER_MASK_PATCHES) + [SILENT_CLEAR]:
            o = _off(addr)
            got = bytes(out[o:o + len(want)])
            if got != want:
                raise ValueError("F8 读者屏蔽/静默清零补丁点 0x%08X 原始字节不符"
                                 "（固件版本不对？）\n  期望 %s\n  实际 %s"
                                 % (addr, _hex(want), _hex(got)))
            out[o:o + len(patched_bytes)] = patched_bytes
            log.append({"addr": addr, "offset": o,
                        "before": _hex(want), "after": _hex(patched_bytes),
                        "desc": desc})

    patched = bytes(out)
    assert len(patched) == len(data)
    return patched, log


# ---------------------------------------------------------------------------
# 静态差异范围
# ---------------------------------------------------------------------------
def _expected_diff_offsets(key: int, full: bool = True) -> set:
    """打补丁后相对原厂镜像应当且只应当变化的文件偏移。"""
    diffs = set()
    blob = build_sync_function(key)
    base = _off(SYNC_FUNC)
    for i in range(SYNC_FUNC_SIZE):
        if blob[i] != ORIGINAL_FUNC[i]:
            diffs.add(base + i)
    for addr, want, new, _desc in ITEM_GATES:
        base = _off(addr)
        for i in range(len(want)):
            if new[i] != want[i]:
                diffs.add(base + i)
    if full:
        for addr, want, new, _desc in list(READER_MASK_PATCHES) + [SILENT_CLEAR]:
            base = _off(addr)
            for i in range(len(want)):
                if new[i] != want[i]:
                    diffs.add(base + i)
    return diffs


def _unpatch(patched: bytes) -> bytes:
    """把补丁区还原成原厂字节（用于原厂 harness 自证）。"""
    buf = bytearray(patched)
    off = _off(SYNC_FUNC)
    buf[off:off + SYNC_FUNC_SIZE] = ORIGINAL_FUNC
    for addr, want, _new, _desc in ITEM_GATES:
        o = _off(addr)
        buf[o:o + len(want)] = want
    for addr, want, _new, _desc in list(READER_MASK_PATCHES) + [SILENT_CLEAR]:
        o = _off(addr)
        buf[o:o + len(want)] = want
    return bytes(buf)


# ---------------------------------------------------------------------------
# Unicorn 调用级 harness
# ---------------------------------------------------------------------------
def _uc_new(image: bytes, pre_ram: dict | None = None, patch_flash: dict | None = None):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_PROT_ALL

    if len(image) > UC_FLASH_SIZE - (BASE - UC_FLASH):
        raise ValueError("镜像过大，无法加载")
    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)      # Thumb 由入口 bit0 决定
    uc.mem_map(UC_FLASH, UC_FLASH_SIZE, UC_PROT_ALL)
    uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))       # b .
    for addr in (HELPER_REG_WRITE, HELPER_STROBE, HELPER_DELAY):
        uc.mem_write(addr, bytes.fromhex("7047"))          # bx lr（记录调用参数）
    for addr, blob in (patch_flash or {}).items():
        uc.mem_write(addr, blob)
    for addr, blob in (pre_ram or {}).items():
        uc.mem_write(addr, blob)
    return uc


def _uc_run(uc, entry: int, calls: list, regs: dict | None = None,
            max_insn: int = MAX_HARNESS_INSNS) -> dict:
    """从 entry（Thumb 地址，可带 bit0）执行到返回 sentinel；返回执行状态。"""
    from unicorn import UC_HOOK_CODE, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_SP,
                                   UC_ARM_REG_LR, UC_ARM_REG_PC)

    state = {"returned": False, "error": None, "insn": 0}

    def hook_code(_uc, address, _size, _user):
        state["insn"] += 1
        if address == UC_SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
            return
        if address == HELPER_REG_WRITE:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        elif address == HELPER_STROBE:
            calls.append(("cmd", _uc.reg_read(UC_ARM_REG_R0), None))
        elif address == HELPER_DELAY:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    for reg, value in (regs or {}).items():
        uc.reg_write(reg, value)
    try:
        uc.emu_start(entry | 1, 0, 0, max_insn)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X, lr=0x%08X, sp=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR),
            uc.reg_read(UC_ARM_REG_SP))
    if state["error"] is None and not state["returned"]:
        state["error"] = ("未在 %d 条指令内返回（执行 %d 条，pc=0x%08X），疑似跑飞"
                          % (max_insn, state["insn"], uc.reg_read(UC_ARM_REG_PC)))
    return state


def _uc_run_sync_fn(image: bytes, ident: int, flag: int) -> dict:
    """用 Unicorn 执行同步字设置函数（RAM[KEY_FLAG_ADDR]=flag）。"""
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3)
    pre = {KEY_FLAG_ADDR: bytes([flag & 0xFF])}
    uc = _uc_new(image, pre_ram=pre)
    calls: list[tuple] = []
    state = _uc_run(uc, SYNC_FUNC, calls, regs={UC_ARM_REG_R0: ident & 0xFF,
                                                UC_ARM_REG_R1: 0x1234,
                                                UC_ARM_REG_R2: 0x5678,
                                                UC_ARM_REG_R3: 0x9ABC})
    return {"calls": calls, "error": state["error"], "insn": state["insn"]}


def _uc_run_cfn_handler(image: bytes, handler: int, item: int, value: int,
                        ident: int) -> dict:
    """执行一次 C.Fn +/- handler，返回 {"settings": bytes(8), "calls": [...], "error"}。

    现场（与真实 handler 入口一致）：
    * UI_BASE+0x15 = 1（值编辑态）、+0x16 = 1（真正改值）、+0x17 = 当前项；
    * UI_MODE_FLAG = 0（走 C.Fn 输入路径）；
    * settings[6] = ID，settings[item] = value。
    """
    settings = bytearray(SETTINGS_SIZE)
    settings[ID_IDX] = ident & 0xFF
    if 0 <= item < SETTINGS_SIZE:
        settings[item] = value & 0xFF
    pre = {
        UI_MODE_FLAG: b"\x00",
        UI_EDIT: b"\x01",
        UI_CHANGE: b"\x01",
        UI_ITEM: bytes([item & 0xFF]),
        SETTINGS_BASE: bytes(settings),
    }
    uc = _uc_new(image, pre_ram=pre)
    calls: list[tuple] = []
    state = _uc_run(uc, handler, calls)
    try:
        new_settings = bytes(uc.mem_read(SETTINGS_BASE, SETTINGS_SIZE))
    except Exception:  # noqa: BLE001
        new_settings = b""
    return {"settings": new_settings, "calls": calls, "error": state["error"]}


def _format_calls(calls: list[tuple]) -> str:
    parts = []
    for kind, a, b in calls:
        if kind == "wreg":
            parts.append("WriteReg(%d,0x%02X)" % (a, b))
        elif kind == "cmd":
            parts.append("WriteCmd(0x%02X)" % a)
        else:
            parts.append("delay(%d)" % a)
    return " -> ".join(parts) if parts else "(无)"


def _expected_calls(sync: int) -> list[tuple]:
    return [("wreg", 4, (sync >> 8) & 0xFF), ("wreg", 5, sync & 0xFF)]


def _check_sync_fn(image: bytes, key: int, ident: int, flag: int) -> tuple[bool, str]:
    """补丁版同步字函数：F8=1 -> KEY；F8=0 -> 50024/(ID+1)。"""
    res = _uc_run_sync_fn(image, ident, flag)
    if res["error"]:
        return False, "ID=%d flag=%d 补丁版 Unicorn 执行失败: %s" % (
            ident, flag, res["error"])
    key_mode = (flag == KEY_FLAG_ON_VALUE)
    expect = key if key_mode else ID_NUMERATOR // (ident + 1)
    want = _expected_calls(expect)
    ok = res["calls"] == want
    msg = ("ID=%-2d F8=%d %s: sync=0x%04X -> %s | %s"
           % (ident, flag, "密钥模式" if key_mode else "原厂公式", expect,
              _format_calls(res["calls"]), _format_calls(want)))
    if key_mode:
        ok = ok and expect != ID_NUMERATOR // (ident + 1)
    return ok, msg


def _check_original_sync(image: bytes, ident: int) -> tuple[bool, str]:
    """原厂 harness 自证：50024/(ID+1) + delay(20)+SCAL(0x33)+delay(1000)。"""
    res = _uc_run_sync_fn(image, ident, 0)
    if res["error"]:
        return False, "ID=%d 原厂自证 Unicorn 执行失败: %s" % (ident, res["error"])
    expect = ID_NUMERATOR // (ident + 1)
    want = _expected_calls(expect) + [("delay", 20, None), ("cmd", 0x33, None),
                                      ("delay", 1000, None)]
    ok = res["calls"] == want
    msg = ("ID=%-2d 原厂(自证): sync=0x%04X | %s"
           % (ident, expect, _format_calls(res["calls"])))
    if not ok:
        msg += "  期望 %s" % _format_calls(want)
    return ok, msg


def _check_cfn_toggle(image: bytes, key: int, handler: int, item: int, value: int,
                      ident: int, label: str) -> tuple[bool, str]:
    """执行一次 C.Fn 改值，断言 settings[item] 变化 + 立即重设同步字。"""
    res = _uc_run_cfn_handler(image, handler, item, value, ident)
    if res["error"]:
        return False, "%s: handler 执行失败: %s" % (label, res["error"])
    if len(res["settings"]) != SETTINGS_SIZE:
        return False, "%s: 读不到 settings" % label
    newv = res["settings"][item]
    expect_flag = 1 if newv != 0 else 0
    sync = key if expect_flag == KEY_FLAG_ON_VALUE else ID_NUMERATOR // (ident + 1)
    want = _expected_calls(sync)
    ok = res["calls"] == want
    msg = ("%s: settings[%d] %d -> %d，立即写 SYNC %s（期望 %s）"
           % (label, item, value, newv, _format_calls(res["calls"]),
              _format_calls(want)))
    return ok, msg


def _uc_level_lookup(image: bytes, flag: int, patched: bool = True) -> int:
    """调用 level_lookup(0x08006044)：F8=1 时原厂会切到备用标定表（+0x1C）。"""
    from unicorn.arm_const import UC_ARM_REG_R0
    # 0x20000140 -> 0x20000300（两套表指针）；两套表放不同常数，便于区分
    pre = {
        KEY_FLAG_ADDR: bytes([flag & 0xFF]),
        0x200001A1: b"\x00",
        0x200001A9: b"\x00",
        0x200001CC: b"\x00",
        # 0x0800605C: ldr r1,=0x20000140；紧接 ldr r1,[r1,#0x18]/[r1,#0x1C]
        # 读的是 0x20000158 / 0x2000015C 处的两个表指针（不是 0x20000318）。
        0x20000158: struct.pack("<I", 0x20000320),   # 默认表
        0x2000015C: struct.pack("<I", 0x20000360),   # F8=1 备用表
        0x20000320 + 0xA0: b"\x11\x11",              # default: index (0x50)<<1 = 0xA0
        0x20000360 + 0x142: b"\x22\x22",             # F8:      index (0x50)<<2+2 = 0x142
    }
    uc = _uc_new(image, pre_ram=pre)
    calls: list[tuple] = []
    state = _uc_run(uc, LEVEL_LOOKUP, calls)
    if state["error"]:
        raise RuntimeError("level_lookup 执行失败: %s" % state["error"])
    return uc.reg_read(UC_ARM_REG_R0)


def _uc_ui_clear(image: bytes) -> int:
    """调用退出/模式切换路径 0x0800B670（原厂会静默把 settings[7] 清 0）。"""
    pre = {
        KEY_FLAG_ADDR: b"\x01",          # F8=ON（密钥模式）
        UI_BASE: bytes(0x40),            # UI 结构清零
        UI_BASE + 0x15: b"\x00",
        UI_BASE + 0x11: b"\x00",         # 触发「清 settings[7]」分支
    }
    uc = _uc_new(image, pre_ram=pre)
    calls: list[tuple] = []
    state = _uc_run(uc, UI_CLEAR_FN, calls)
    if state["error"]:
        raise RuntimeError("UI 退出路径执行失败: %s" % state["error"])
    return uc.mem_read(KEY_FLAG_ADDR, 1)[0]


def _uc_record_builder(image: bytes) -> bytes:
    """调用参数记录 builder 0x08004410，返回 0x20 字节记录（+0x1C 含 settings[4..7]）。"""
    from unicorn.arm_const import UC_ARM_REG_R0
    pre = {
        SETTINGS_BASE: bytes([1, 0, 2, 0, 0, 0, 37, 1]),   # 含 settings[7]=F8=1
        0x20000300: bytes(0x20),
    }
    uc = _uc_new(image, pre_ram=pre)
    calls: list[tuple] = []
    state = _uc_run(uc, RECORD_BUILDER, calls, regs={UC_ARM_REG_R0: 0x20000300})
    if state["error"]:
        raise RuntimeError("参数记录 builder 执行失败: %s" % state["error"])
    return bytes(uc.mem_read(0x20000300, 0x20))


def _uc_record_load(image: bytes) -> bytes:
    """调用参数加载 0x0800C814，返回回填后的 settings[0..7]。"""
    pre = {
        RECORD_BUF: bytes(0x20),
        0x20000440 + 0x18: bytes([1, 0, 2, 0]),      # settings[0..3]
        0x20000440 + 0x1C: bytes([0, 0, 37, 1]),     # settings[4..7]（最高字节 = F8）
        SETTINGS_BASE: bytes(SETTINGS_SIZE),
    }
    uc = _uc_new(image, pre_ram=pre)
    calls: list[tuple] = []
    state = _uc_run(uc, RECORD_LOAD, calls)
    if state["error"]:
        raise RuntimeError("参数加载执行失败: %s" % state["error"])
    return bytes(uc.mem_read(SETTINGS_BASE, SETTINGS_SIZE))


# ---------------------------------------------------------------------------
# 离线验证
# ---------------------------------------------------------------------------
def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证：静态字节/边界/差异范围 + Unicorn 同步字/handler/读者/持久化。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    # 1) 长度不变
    results.append((len(patched) == ORIGINAL_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁）"
                    % (len(patched), ORIGINAL_SIZE)))

    # 2) 同步字函数 56 字节与期望完全一致
    blob = build_sync_function(key)
    off = _off(SYNC_FUNC)
    got = bytes(patched[off:off + SYNC_FUNC_SIZE])
    results.append((got == blob,
                    "0x%08X..0x%08X 补丁区 %d 字节与期望函数体完全一致"
                    % (SYNC_FUNC, SYNC_FUNC_END - 1, SYNC_FUNC_SIZE) if got == blob else
                    "补丁区字节与期望不符\n  期望 %s\n  实际 %s"
                    % (blob.hex(" "), got.hex(" "))))

    # 3) 不越过下一函数
    results.append((SYNC_FUNC + len(blob) == SYNC_FUNC_END,
                    "函数体结束于 0x%08X == 下一函数起点 0x%08X（不越界）"
                    % (SYNC_FUNC + len(blob), SYNC_FUNC_END)))

    # 4) 字面量：+0x2C=KEY、+0x30=FLAG、+0x34=50024（原厂同址同值）
    lits = (int.from_bytes(got[0x2C:0x30], "little"),
            int.from_bytes(got[0x30:0x34], "little"),
            int.from_bytes(got[0x34:0x38], "little"))
    results.append((lits == (key, KEY_FLAG_ADDR, ID_NUMERATOR),
                    "字面量: KEY=0x%04X @0x%08X, FLAG=0x%08X @0x%08X, 50024 @0x%08X"
                    % (lits[0], SYNC_FUNC + 0x2C, lits[1], SYNC_FUNC + 0x30,
                       SYNC_FUNC + 0x34)))

    # 5) 改值即时生效 hook
    for addr, want, patched_bytes, desc in ITEM_GATES:
        o = _off(addr)
        gotg = bytes(patched[o:o + len(want)])
        results.append((gotg == patched_bytes,
                        "0x%08X = %s（%s）" % (addr, gotg.hex(" "), desc)))

    # 6) F8 读者屏蔽 + 静默清零阻断（core 变体 full=False 时这些点应保持原厂字节）
    all_masks = list(READER_MASK_PATCHES) + [SILENT_CLEAR]
    is_full = all(bytes(patched[_off(a):_off(a) + len(new)]) == new
                  for a, _want, new, _desc in all_masks)
    for addr, want, new, desc in all_masks:
        o = _off(addr)
        gotr = bytes(patched[o:o + len(new)])
        if is_full:
            results.append((gotr == new, "0x%08X = %s（%s）" % (addr, gotr.hex(" "), desc)))
        else:
            results.append((gotr == want,
                            "0x%08X = %s（core 变体：保持原厂字节，%s 由 full=False 跳过）"
                            % (addr, gotr.hex(" "), desc)))

    # 7) 与原厂差异必须恰好只落在补丁点集合内
    orig = _unpatch(patched)
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    expected = _expected_diff_offsets(key, full=is_full)
    results.append((set(diffs) == expected,
                    "与原厂差异 %d 字节，offset=%s（应恰为 %s）"
                    % (len(diffs), ["0x%05X" % d for d in diffs],
                       ["0x%05X" % d for d in sorted(expected)])))
    outside = [i for i in diffs if i not in expected]
    results.append((not outside, "差异之外无任何字节变化（%d 处）" % len(outside)))

    # 8) Unicorn 调用级
    try:
        import unicorn  # noqa: F401
        has_uc = True
    except Exception:  # noqa: BLE001
        has_uc = False
    if not has_uc:
        results.append((True, "警告: unicorn 不可用，跳过 Unicorn 调用级测试（静态检查已通过）"))
        return results

    results.append((True, "Unicorn 调用级测试：同步字 0x%08X，RAM[0x%08X]=F8，"
                          "C.Fn handler 0x%08X/0x%08X，参数记录 0x%08X/0x%08X"
                    % (SYNC_FUNC | 1, KEY_FLAG_ADDR, CFN_DEC_HANDLER,
                       CFN_INC_HANDLER, RECORD_BUILDER, RECORD_LOAD)))

    # 8a) 同步字函数：F8 两态 + 越界值（2 应等同 OFF，防止任何非 1 值误开密钥）
    for flag in (KEY_FLAG_ON_VALUE, 0, 2):
        for ident in (0, 1, 5, 37, 99):
            results.append(_check_sync_fn(patched, key, ident, flag))

    # 8b) 原厂镜像自证：原函数应有 delay(20)+SCAL(0x33)+delay(1000)
    for ident in (0, 1, 5, 37, 99):
        results.append(_check_original_sync(orig, ident))

    # 8c) C.Fn F8 改值即时生效（补齐 settings 变化 + 立即重设同步字）
    results.append(_check_cfn_toggle(patched, key, CFN_INC_HANDLER, FLAG_IDX, 0,
                                     ident=37, label="F8 + (OFF->ON)"))
    results.append(_check_cfn_toggle(patched, key, CFN_DEC_HANDLER, FLAG_IDX, 1,
                                     ident=37, label="F8 - (ON->OFF)"))
    results.append(_check_cfn_toggle(patched, key, CFN_DEC_HANDLER, FLAG_IDX, 0,
                                     ident=37, label="F8 - (OFF->ON，回绕到最大值 1)"))
    results.append(_check_cfn_toggle(patched, key, CFN_INC_HANDLER, FLAG_IDX, 1,
                                     ident=37, label="F8 + (ON->OFF，回绕到 0)"))

    # 8d) ID 改值仍立即重设同步字（hook 未破坏原厂 ID 行为）
    res = _uc_run_cfn_handler(patched, CFN_INC_HANDLER, ID_IDX, 5, ident=5)
    if res["error"]:
        results.append((False, "ID 改值 handler 执行失败: %s" % res["error"]))
    else:
        new_id = res["settings"][ID_IDX]
        sync = ID_NUMERATOR // (new_id + 1) if new_id < 100 else None
        want = _expected_calls(sync) if sync is not None else []
        results.append((new_id == 6 and res["calls"] == want,
                        "C.Fn ID(F7) 改值：settings[6] 5 -> %d，立即写 SYNC %s（期望 %s）"
                        % (new_id, _format_calls(res["calls"]), _format_calls(want))))

    # 8e) item<6 的 C.Fn 改值不应触发重设（保持原厂行为）
    res = _uc_run_cfn_handler(patched, CFN_INC_HANDLER, 5, 0, ident=37)
    results.append((res["error"] is None and res["calls"] == [],
                    "C.Fn F6(item5) 改值：不触发同步字重设（calls=%s，原厂行为）"
                    % _format_calls(res["calls"])))

    # 8f) 原厂镜像：F8 改值不会重设同步字（自证 hook 确实新增了立即生效）
    res = _uc_run_cfn_handler(orig, CFN_INC_HANDLER, FLAG_IDX, 0, ident=37)
    results.append((res["error"] is None and len(res["settings"]) == SETTINGS_SIZE
                    and res["settings"][FLAG_IDX] == 1 and res["calls"] == [],
                    "原厂(自证)：F8 + 改值 settings[7] 0 -> %s 且不重设同步字（calls=%s）"
                    % (res["settings"][FLAG_IDX] if len(res["settings"]) > FLAG_IDX else "?",
                       _format_calls(res["calls"]))))

    # 8g) reader-mask：level_lookup 在 F8=1/0 下应返回相同值；原厂应不同
    if not is_full:
        results.append((True, "core 变体(full=False)：跳过读者屏蔽/静默清零验证"
                              "（F8=ON 时原厂稳定色温读者仍生效，属预期）"))
    else:
        try:
            v_on = _uc_level_lookup(patched, flag=1)
            v_off = _uc_level_lookup(patched, flag=0)
            v_orig_on = _uc_level_lookup(orig, flag=1)
            v_orig_off = _uc_level_lookup(orig, flag=0)
            ok_reader = (v_on == v_off == v_orig_off and v_orig_on != v_orig_off)
            results.append((ok_reader,
                            "读者屏蔽(level 查表)：补丁 F8=1 -> 0x%04X，F8=0 -> 0x%04X；"
                            "原厂 F8=1 -> 0x%04X，F8=0 -> 0x%04X（应 前者相等且=原厂OFF）"
                            % (v_on, v_off, v_orig_on, v_orig_off)))
        except Exception as exc:  # noqa: BLE001
            results.append((False, "level 查表读者屏蔽验证失败: %s" % exc))

        # 8h) 静默清零阻断：退出路径后补丁仍保持 F8=1，原厂会被清 0
        try:
            after_p = _uc_ui_clear(patched)
            after_o = _uc_ui_clear(orig)
            results.append((after_p == 1 and after_o == 0,
                            "退出/模式切换路径后 settings[7]：补丁=%d（应 1），原厂=%d（应 0）"
                            % (after_p, after_o)))
        except Exception as exc:  # noqa: BLE001
            results.append((False, "静默清零阻断验证失败: %s" % exc))

    # 8i) 持久化：参数记录 +0x1C 最高字节 = settings[7]；加载后回填
    try:
        rec = _uc_record_builder(patched)
        ok_rec = len(rec) == 0x20 and rec[0x1F] == 1 and rec[0x1E] == 37
        results.append((ok_rec,
                        "参数记录 builder：settings[4..7]=0,0,37,1 -> 记录 +0x1C=%s"
                        "（应 00 00 25 01，F8 在最高字节）" % rec[0x1C:0x20].hex(" ")))
        loaded = _uc_record_load(patched)
        ok_load = (len(loaded) == SETTINGS_SIZE and loaded[ID_IDX] == 37
                   and loaded[FLAG_IDX] == 1)
        results.append((ok_load,
                        "参数加载：记录 +0x1C=00 00 25 01 -> settings=%s（应含 ID=37, F8=1）"
                        % loaded.hex(" ")))
    except Exception as exc:  # noqa: BLE001
        results.append((False, "持久化路径验证失败: %s" % exc))

    return results
