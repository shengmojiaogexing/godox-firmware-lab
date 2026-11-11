# -*- coding: utf-8 -*-
"""Godox X2T-S V1.4 —— 私有密钥（CC2500 16 位硬件同步字）补丁模块 —— v1「C.Fn 开关」版。

用户可见语义
------------
引闪器 **C.Fn 菜单第 3 项（BEEP，原厂蜂鸣开关 OFF/ON）** 被复用为
**私有密钥开关**。开关值就是 C.Fn 值数组字节 **C.Fn[2] = RAM 0x200203E2**
（数组基址 0x200203E0，项 2 = BEEP，最大值表 0x20020072[2] = 1；随原厂
C.Fn 设置一起保存到 flash 参数记录，重启后保留）。

* BEEP = ON（值 1）  -> CC2500 同步字 = 私有 KEY（只有打了补丁的引闪器能引闪）
* BEEP = OFF（值 0） -> 同步字 = 50024 // (ID + 1)（原厂普通模式，ID 菜单恢复原厂含义）

把原厂「ID -> 16 位同步字」函数 0x0800C8F8 整段改写（52 B，到下个函数 0x0800C92C）：

    flag = *(uint8_t *)0x200203E2;     // C.Fn BEEP 密钥开关（1=ON）
    if (flag == 1) sync = KEY;         // 密钥模式
    else            sync = 50024 / (id + 1); // 原厂普通模式
    WriteReg(4, sync >> 8);            // SYNC1
    WriteReg(5, sync & 0xFF);          // SYNC0
    return;                            // 之后跳回同 SDK 频道设置函数 0x0800C8B0 的原厂尾部
                                       // （0x0800C8DC: delay(20); SCAL(0x33); delay(1000)）
                                       // —— 保留完整原厂射频时序，不丢校准。

改值即时生效（C.Fn BEEP 一改就重设同步字）
------------------------------------------
原厂只有 CH(5)/ID(6)/DIST(0xA) 三项会在菜单里触发 RF 重设，其它 C.Fn 项直接写数组、
不重设同步字。本补丁在 C.Fn 加/减处理函数的两条通路上注入 4 个小 hook（放在**未使用
的中断向量槽**里，槽内容原为默认陷阱 0x080031C7，对应 IRQ 从未使能）：

* 递增普通通路 0x08009232（原 `strb r4,[r1,r2]; ldrb r5,[r3,r2]`）-> HOOK_INC 0x0800306C
* 递减普通通路 0x08005C8E（原 `subs r3,r3,#1; strb r3,[r0,r1]`）-> HOOK_DEC 0x0800307C
* 递增回绕通路 0x0800923A（原 `cmp r2,#0xF; beq 0x0800928E`）  -> HOOK_WRAP_I 0x0800308C
* 递减回绕通路 0x08005CD4（原 `ldrb r2,[r2,r1]; strb r2,[r0,r1]`）-> HOOK_WRAP_D 0x080030A0

hook 逻辑统一为：执行被搬走的 2 条原指令（或等价写回），然后
`sync(C.Fn[6])`（本补丁版同步字函数会重新读 BEEP 开关），再恢复现场返回。
因此 **BEEP 开 -> 密钥模式、BEEP 关 -> 原厂模式** 在菜单里立刻生效。

补丁点复核（详见 devices/x2ts/re/ 与 keypatch/dev/x2ts/）
--------------------------------------------------------
* 镜像真实链接基址 **0x08003000**（Reset_Handler 0x080031AC / SystemInit 0x0800CF60 /
  尾部初始化表 0x080114A8 三处自证），文件长度 58,628 B。
* 启动链：0x080031D0 解释初始化表 0x080114A8（2 条 16B record）：
  rec0 解压 0x080114C8 -> 0x20020000（.data 0x188 B，由 0x080031F4 的 RLE 解压器执行）；
  rec1 memzero 0x20020188 -> 0x22038（.bss 0x1EB0 B，由 0x0800DC20 memset 执行）。
* C.Fn 值数组 0x200203E0（项 5=CH、6=ID、0xA=DIST 被 apply 函数 0x0800C790 特殊处理）；
  C.Fn 最大值表 0x20020072（.data 解压初值；17 项：01 01 01 09 01 1F 63 01 63 02 01 05 01 03 02 06 01，
  与 X2T 手册 C.Fn 顺序 SHUTTER/BLUE.T./BEEP/ZOOM/SCAN/CH/ID/PC SYNC/DELAY/SHOOT/DIST 完全对应）。
* 同步字函数 3 个调用点：0x08005F72（RF 初始化取 ID）、0x0800ADC4（主循环设置同步/
  周期应用：比较 0x200203E0[5]/[6] 与影子 0x20020122[0]/[1]，不同则 set_channel+set_sync）、
  0x0800C7F8（菜单 apply 函数 case 6：ID 改变时重设同步字）。
* 辅助函数：CC2500_WriteReg=0x0800E842、strobe=0x0800E932、delay_us=0x0800E5AA；
  频道设置函数 0x0800C8B0 的尾部 0x0800C8DC 是 delay(20)+SCAL(0x33)+pop+delay(1000)。
* 原厂 0xC368（50024）不是字面量，而是 `movw r1,#0xC368` 内联立即数，故本补丁无需
  V1C 那种 .word 常量，把 50024 原样 movw 进普通模式分支即可。

风险
----
* 无实机验证。密钥模式保真执行原厂尾部（delay 20 / SCAL / delay 1000），射频时序与原厂一致。
* BEEP 原厂「蜂鸣开关」语义被复用：BEEP=ON 时密钥模式生效，同时原厂蜂鸣也会按原厂逻辑工作
  （原厂 BEEP 读者未屏蔽，属已知副作用；不影响射频/引闪）。
* 隐藏项 15 的「回绕到最大值」特殊分支被回绕 hook 覆盖为常规回绕（该项不在手册 11 项内，
  属固件内部预留项，正常使用不可见）。
* 换密钥需重新生成并刷入；灯端/引闪器端必须使用同一把密钥的固件。
"""
from __future__ import annotations

import os
import struct
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
_KEYPATCH_DIR = os.path.dirname(_HERE)
if _KEYPATCH_DIR not in sys.path:
    sys.path.insert(0, _KEYPATCH_DIR)

import common  # noqa: E402

DEVICE = {
    "id": "x2ts",
    "name": "Godox X2T-S",
    "kind": "trigger",                   # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "X2T-S",
    "firmware_version": "V1.4",
    "image": "devices/x2ts/firmware/X2T-S_V1.4.bin",
    "base": 0x08003000,
    "sha256": "478476447ff8d55496180facf578662dcbf9a1af0cf21f79df048b7df0a76efc",
    "out_name": "X2T-S_V1.4_key{key:04X}.bin",
    "container_name": "X2TS_V1.4_key{key:04X}_container.bin",
    "notes": "C.Fn 第 3 项 BEEP 密钥开关(C.Fn[2]=0x200203E2)：ON(1) -> SYNC=私有 KEY；"
             "OFF(0) -> 原厂 50024/(ID+1)。52 B 整函数改写(保留原厂 delay+SCAL 尾部) + "
             "C.Fn 加/减即时生效 hook（未用中断向量槽注入）",
}

# ---- 镜像与目标函数 ---------------------------------------------------------
BASE = 0x08003000
IMAGE_SIZE = 58628

SYNC_FN = 0x0800C8F8              # void rf_set_id(uint8_t id)：写 CC2500 SYNC1/SYNC0
SYNC_FN_END = 0x0800C92C          # 下一函数起点；本补丁必须止步于此
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN   # 52 字节
assert SYNC_FN_SIZE == 52

# 原厂 0x0800C8F8 起 52 字节（apply 前逐字节校验，防固件版本不对）
ORIG_FUNC = bytes.fromhex(
    "10b54cf26831401cb1fbf0f4210a042001f09bffe1b2052001f097ff142001f0"
    "48fe332002f009f8bde810404ff47a7001f03fbe")
assert len(ORIG_FUNC) == SYNC_FN_SIZE, len(ORIG_FUNC)

# 同 SDK 频道设置函数 0x0800C8B0 的原厂尾部：delay(20); SCAL(0x33); pop {r4,lr}; delay(1000)
# 本补丁函数体以 b.w 跳到这里执行，保留完整原厂射频时序（含 SCAL 校准）。
TAIL_HELPER = 0x0800C8DC

# ---- C.Fn 设置区 / 开关项 ---------------------------------------------------
CFN_BASE = 0x200203E0             # C.Fn 值数组基址（项 n = base + n）
FLAG_IDX = 2                      # C.Fn 第 3 项 = BEEP（原厂蜂鸣开关 0=OFF/1=ON）
KEY_FLAG_ADDR = CFN_BASE + FLAG_IDX   # 0x200203E2
KEY_FLAG_ON_VALUE = 1             # BEEP=ON -> 密钥模式；0 -> 原厂普通模式
ID_IDX = 6                        # C.Fn 项 6 = 引闪 ID（同步字函数入参来源）

# C.Fn 最大值表（.data 解压初值，用于静态校验本固件项索引/值域）
CFN_MAX_TABLE_ADDR = 0x20020072
CFN_MAX_TABLE = bytes.fromhex("01010109011f6301630201050103020601")
CFN_ITEM_COUNT = 17               # 项 0..0x10（手册公开 11 项 + 6 个内部预留项）

# ---- 辅助函数（verify 里替换为记录/跳过桩） ---------------------------------
WRITE_REG = 0x0800E842            # CC2500_WriteReg(reg=r0, value=r1)
WRITE_CMD = 0x0800E932            # strobe(cmd=r0)（原厂 0x33 = SCAL）
DELAY_US = 0x0800E5AA             # delay_us(n=r0)

# ---- C.Fn 加/减处理函数里的 4 个 hook（注入到未使用的中断向量槽） -----------
# 未使用的中断向量槽原值均为默认陷阱 0x080031C7（这些 IRQ 从未使能）。
# 0x6C..0x9B 为 12 个连续空闲槽（48 B），0xA0..0xAF 为 4 个空闲槽（16 B）。
HOOK_INC = 0x0800306C             # 16 B：递增普通通路（0x08009232 处 4 B -> BL）
HOOK_DEC = 0x0800307C             # 14 B：递减普通通路（0x08005C8E 处 4 B -> BL）
HOOK_WRAP_I = 0x0800308C          # 16 B：递增回绕通路（0x0800923A 处 4 B -> BL）
HOOK_WRAP_D = 0x080030A0          # 14 B：递减回绕通路（0x08005CD4 处 4 B -> BL）
TRAP = 0x080031C7                 # 未使用中断的默认陷阱（b .）

# 4 个 hook 槽的原厂字节（从 TRAP 展开）
HOOK_ORIG_INC = struct.pack("<IIII", TRAP, TRAP, TRAP, TRAP)          # 16 B
HOOK_ORIG_DEC = struct.pack("<III", TRAP, TRAP, TRAP) + b"\xc7\x31"   # 14 B
HOOK_ORIG_WRAP_I = struct.pack("<IIII", TRAP, TRAP, TRAP, TRAP)       # 16 B
HOOK_ORIG_WRAP_D = struct.pack("<III", TRAP, TRAP, TRAP) + b"\xc7\x31"  # 14 B

# 补丁点：加/减处理函数里被替换成 BL hook 的 4 字节
CALL_INC = 0x08009232             # 原 strb r4,[r1,r2]; ldrb r5,[r3,r2]
CALL_INC_ORIG = bytes.fromhex("8c549d5c")
CALL_DEC = 0x08005C8E             # 原 subs r3,r3,#1; strb r3,[r0,r1]
CALL_DEC_ORIG = bytes.fromhex("5b1e4354")
CALL_WRAP_I = 0x0800923A          # 原 cmp r2,#0xF; beq 0x0800928E
CALL_WRAP_I_ORIG = bytes.fromhex("0f2a27d0")
CALL_WRAP_D = 0x08005CD4          # 原 ldrb r2,[r2,r1]; strb r2,[r0,r1]
CALL_WRAP_D_ORIG = bytes.fromhex("525c4254")

# Unicorn 测试用内存布局
UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000             # 128 KiB flash（app 位于 0x08003000）
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x40000             # SP=0x20022038
UC_STACK = 0x20021800             # 栈顶（8 字节对齐）
UC_SENTINEL = 0x20021C00          # 函数返回落点，放 `b .`
MAX_HARNESS_INSNS = 20000         # Unicorn 单次调用指令上限


# ---------------------------------------------------------------------------
# Thumb-1 指令编码
# ---------------------------------------------------------------------------
def _t_bl(addr: int, target: int) -> bytes:
    """Thumb BL T1 编码（addr = 本 BL 指令地址，target = 绝对地址）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target), off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = (i1 if s else ((~i1) & 1))
    j2 = (i2 if s else ((~i2) & 1))
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                       0xD000 | (j1 << 13) | (j2 << 11) | imm11)


def _t_bw(addr: int, target: int) -> bytes:
    """Thumb-2 B.W T4 编码。"""
    off = target - (addr + 4)
    assert -0x1000000 <= off < 0x1000000 and off % 2 == 0, (hex(addr), hex(target), off)
    s = 1 if off < 0 else 0
    imm = off & 0x1FFFFFF
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    imm10 = (imm >> 12) & 0x3FF
    imm11 = (imm >> 1) & 0x7FF
    j1 = (1 - i1) ^ s
    j2 = (1 - i2) ^ s
    return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                       0x9000 | (j1 << 13) | (j2 << 11) | imm11)


def _t_bcond(addr: int, target: int, cond: int) -> bytes:
    imm = target - (addr + 4)
    assert imm % 2 == 0 and -256 <= imm <= 254, (hex(addr), hex(target), imm)
    return struct.pack("<H", 0xD000 | ((cond & 0xF) << 8) | ((imm // 2) & 0xFF))


def _t_bn(addr: int, target: int) -> bytes:
    imm = target - (addr + 4)
    assert imm % 2 == 0 and -2048 <= imm <= 2046, (hex(addr), hex(target), imm)
    return struct.pack("<H", 0xE000 | ((imm // 2) & 0x7FF))


def _movw(rd: int, imm16: int) -> bytes:
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    return struct.pack("<HH", 0xF240 | (i << 10) | imm4,
                       (imm3 << 12) | ((rd & 0xF) << 8) | imm8)


def _udiv(rd: int, rn: int, rm: int) -> bytes:
    """UDIV (T1): FBB0|Rn, F0F0|(Rd<<8)|Rm。"""
    return struct.pack("<HH", 0xFBB0 | (rn & 0xF),
                       0xF0F0 | ((rd & 0xF) << 8) | (rm & 0xF))


# ---------------------------------------------------------------------------
# 补丁体构造：同步字函数（52 字节整段改写）
# ---------------------------------------------------------------------------
def build_sync_function(key: int) -> bytes:
    """生成 52 字节替换函数体。

    布局（相对 0x0800C8F8；真实反汇编见 devices/x2ts/re/ 与 verify 输出）::

        +00 push {r4, lr}
        +02 ldr  r3, =FLAG(0x200203E2)   ; 字面量在 +0x30
        +04 ldrb r3, [r3]
        +06 cmp  r3, #1                  ; BEEP=ON(1)
        +08 beq  .key
        +0A adds r0, r0, #1              ; 原厂模式除数 = ID+1
        +0C movw r1, #0xC368             ; 50024
        +10 udiv r4, r1, r0
        +14 b.n  .write
        +16 .key: movw r4, #KEY
        +1A .write: lsrs r1, r4, #8      ; SYNC1
        +1C movs r0, #4
        +1E bl   WriteReg
        +22 uxtb r1, r4                  ; SYNC0
        +24 movs r0, #5
        +26 bl   WriteReg
        +2A b.w  0x0800C8DC              ; 原厂 delay(20)+SCAL+delay(1000) 尾部
        +2E nop
        +30 .word FLAG
    """
    key = common.validate_key(key)
    buf = bytearray()

    def here() -> int:
        return SYNC_FN + len(buf)

    def emit(b: bytes) -> None:
        buf.extend(b)

    lit_fix = None
    emit(struct.pack("<H", 0xB510))          # +00 push {r4, lr}
    lit_fix = len(buf)                       # +02 ldr r3, [pc, #imm]（回填）
    emit(b"\x00\x00")
    emit(struct.pack("<H", 0x781B))          # +04 ldrb r3, [r3]
    emit(struct.pack("<H", 0x2B00 | KEY_FLAG_ON_VALUE))   # +06 cmp r3, #1
    emit(_t_bcond(here(), SYNC_FN + 0x16, 0x0))           # +08 beq .key
    emit(struct.pack("<H", 0x1C40))          # +0A adds r0, r0, #1
    emit(_movw(1, common.ID_NUMERATOR))      # +0C movw r1, #0xC368
    emit(_udiv(4, 1, 0))                     # +10 udiv r4, r1, r0
    emit(_t_bn(here(), SYNC_FN + 0x1A))      # +14 b.n .write
    emit(_movw(4, key))                      # +16 .key: movw r4, #KEY
    emit(struct.pack("<H", 0x0A21))          # +1A .write: lsrs r1, r4, #8
    emit(struct.pack("<H", 0x2004))          # +1C movs r0, #4
    emit(_t_bl(here(), WRITE_REG))           # +1E
    emit(struct.pack("<H", 0xB2E1))          # +22 uxtb r1, r4
    emit(struct.pack("<H", 0x2005))          # +24 movs r0, #5
    emit(_t_bl(here(), WRITE_REG))           # +26
    emit(_t_bw(here(), TAIL_HELPER))         # +2A b.w 原厂 delay/SCAL 尾部
    assert len(buf) == 0x2E, hex(len(buf))
    emit(struct.pack("<H", 0xBF00))          # +2E nop（对齐）
    assert len(buf) == 0x30, hex(len(buf))
    emit(struct.pack("<I", KEY_FLAG_ADDR))   # +30 .word FLAG
    assert len(buf) == 52, len(buf)

    # 回填 ldr 字面量偏移：pc = (instr_addr + 4) & ~3
    instr_addr = SYNC_FN + lit_fix
    pc = (instr_addr + 4) & ~3
    lit_addr = SYNC_FN + 0x30
    delta = lit_addr - pc
    assert delta % 4 == 0 and 0 <= delta // 4 <= 0xFF, (hex(pc), hex(lit_addr))
    buf[lit_fix:lit_fix + 2] = struct.pack("<H", 0x4800 | (3 << 8) | (delta // 4))
    return bytes(buf)


# ---------------------------------------------------------------------------
# 即时生效 hook（4 个，注入未使用中断向量槽）
# ---------------------------------------------------------------------------
def build_hooks() -> list[tuple[int, bytes, bytes, bytes, str]]:
    """返回 [(hook_addr, 原厂字节, 新代码, 调用点字节, 说明), ...]。"""
    out: list[tuple[int, bytes, bytes, bytes, str]] = []

    # HOOK_INC：搬走 strb r4,[r1,r2]; ldrb r5,[r3,r2]，再 sync(C.Fn[6])
    # 入口 r1=C.Fn 基址, r2=项号, r3=最大值表基址, r4=新值
    code = bytearray()
    code += bytes.fromhex("8c54")            # strb r4,[r1,r2]
    code += bytes.fromhex("9d5c")            # ldrb r5,[r3,r2]
    code += bytes.fromhex("06b5")            # push {r1,r2,lr}
    code += bytes.fromhex("8879")            # ldrb r0,[r1,#6]
    code += _t_bl(HOOK_INC + len(code), SYNC_FN)
    code += bytes.fromhex("0020")            # movs r0,#0（回绕重置用）
    code += bytes.fromhex("06bd")            # pop {r1,r2,pc}
    assert len(code) <= 16, len(code)
    code += b"\x00\xbf" * (16 - len(code))
    out.append((HOOK_INC, HOOK_ORIG_INC, bytes(code),
                _t_bl(CALL_INC, HOOK_INC),
                "C.Fn 递增普通通路 hook：写值后立即 sync(C.Fn[6])"))

    # HOOK_DEC：搬走 subs r3,r3,#1; strb r3,[r0,r1]，再 sync(C.Fn[6])
    # 入口 r0=C.Fn 基址, r1=项号, r3=值
    code = bytearray()
    code += bytes.fromhex("5b1e")            # subs r3,r3,#1
    code += bytes.fromhex("4354")            # strb r3,[r0,r1]
    code += bytes.fromhex("01b5")            # push {r0,lr}
    code += bytes.fromhex("8079")            # ldrb r0,[r0,#6]
    code += _t_bl(HOOK_DEC + len(code), SYNC_FN)
    code += bytes.fromhex("01bd")            # pop {r0,pc}
    assert len(code) <= 14, len(code)
    code += b"\x00\xbf" * (14 - len(code))
    out.append((HOOK_DEC, HOOK_ORIG_DEC, bytes(code),
                _t_bl(CALL_DEC, HOOK_DEC),
                "C.Fn 递减普通通路 hook：写值后立即 sync(C.Fn[6])"))

    # HOOK_WRAP_I：先把 C.Fn[项]=0 写进去，再 sync；返回后原 strb r0,[r1,r2] 再写一次 0
    # 入口 r0=0, r1=C.Fn 基址, r2=项号
    code = bytearray()
    code += bytes.fromhex("0020")            # movs r0,#0
    code += bytes.fromhex("8854")            # strb r0,[r1,r2]
    code += bytes.fromhex("06b5")            # push {r1,r2,lr}
    code += bytes.fromhex("8879")            # ldrb r0,[r1,#6]
    code += _t_bl(HOOK_WRAP_I + len(code), SYNC_FN)
    code += bytes.fromhex("06bd")            # pop {r1,r2,pc}
    assert len(code) <= 16, len(code)
    code += b"\x00" * (16 - len(code))       # 按字节补 0（不是按 2 字节指令补）
    out.append((HOOK_WRAP_I, HOOK_ORIG_WRAP_I, bytes(code),
                _t_bl(CALL_WRAP_I, HOOK_WRAP_I),
                "C.Fn 递增回绕通路 hook：回绕写 0 后立即 sync(C.Fn[6])"))

    # HOOK_WRAP_D：搬走 ldrb r2,[r2,r1]; strb r2,[r0,r1]，再 sync(C.Fn[6])
    # 入口 r0=C.Fn 基址, r1=项号, r2=最大值表基址
    code = bytearray()
    code += bytes.fromhex("525c")            # ldrb r2,[r2,r1]
    code += bytes.fromhex("4254")            # strb r2,[r0,r1]
    code += bytes.fromhex("01b5")            # push {r0,lr}
    code += bytes.fromhex("8079")            # ldrb r0,[r0,#6]
    code += _t_bl(HOOK_WRAP_D + len(code), SYNC_FN)
    code += bytes.fromhex("01bd")            # pop {r0,pc}
    assert len(code) <= 14, len(code)
    code += b"\x00\xbf" * (14 - len(code))
    out.append((HOOK_WRAP_D, HOOK_ORIG_WRAP_D, bytes(code),
                _t_bl(CALL_WRAP_D, HOOK_WRAP_D),
                "C.Fn 递减回绕通路 hook：回绕写最大值后立即 sync(C.Fn[6])"))

    return out


# ---------------------------------------------------------------------------
# 打补丁
# ---------------------------------------------------------------------------
def _off(addr: int) -> int:
    return addr - BASE


def _patched_regions(key: int) -> list[tuple[int, bytes, bytes, str]]:
    regions = [(SYNC_FN, ORIG_FUNC, build_sync_function(key),
                "整段改写同步字函数 0x%08X..0x%08X（%d 字节）：C.Fn BEEP 密钥开关"
                " RAM[0x%08X]==%d -> SYNC=KEY 0x%04X，否则 50024/(ID+1)；"
                "写 SYNC1/0 后跳原厂 delay(20)+SCAL+delay(1000) 尾部"
                % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE, KEY_FLAG_ADDR,
                   KEY_FLAG_ON_VALUE, key))]
    for hook_addr, orig, code, call_bytes, desc in build_hooks():
        regions.append((hook_addr, orig, code, "注入 hook 0x%08X：%s" % (hook_addr, desc)))
    for call_addr, orig4, call_bytes, desc in (
            (CALL_INC, CALL_INC_ORIG, _t_bl(CALL_INC, HOOK_INC), "递增普通通路入口 -> HOOK_INC"),
            (CALL_DEC, CALL_DEC_ORIG, _t_bl(CALL_DEC, HOOK_DEC), "递减普通通路入口 -> HOOK_DEC"),
            (CALL_WRAP_I, CALL_WRAP_I_ORIG, _t_bl(CALL_WRAP_I, HOOK_WRAP_I),
             "递增回绕通路入口 -> HOOK_WRAP_I"),
            (CALL_WRAP_D, CALL_WRAP_D_ORIG, _t_bl(CALL_WRAP_D, HOOK_WRAP_D),
             "递减回绕通路入口 -> HOOK_WRAP_D")):
        regions.append((call_addr, orig4, call_bytes, "C.Fn 加/减处理函数 " + desc))
    return regions


def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """把原厂镜像 data 打上「C.Fn BEEP 开关 + 即时生效 hook」补丁，返回 (patched, log)。"""
    key = common.validate_key(key)
    if len(data) != IMAGE_SIZE:
        raise ValueError("Godot X2T-S V1.4 固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
                         % (IMAGE_SIZE, len(data)))

    buf = bytearray(data)
    log: list[dict] = []
    for addr, before, after, desc in _patched_regions(key):
        o = _off(addr)
        got = bytes(buf[o:o + len(before)])
        if got != before:
            raise ValueError(
                "补丁点 0x%08X 原始字节与预期不符（固件版本不符，拒绝打补丁）\n"
                "  期望 %s\n  实际 %s\n  说明 %s"
                % (addr, before.hex(" "), got.hex(" "), desc))
        buf[o:o + len(after)] = after
        log.append({
            "addr": addr,
            "offset": o,
            "before": (before.hex(" ") if len(before) <= 8 else
                       before[:4].hex(" ") + " …(%d B)" % len(before)),
            "after": (after.hex(" ") if len(after) <= 8 else
                      after[:4].hex(" ") + " …(%d B)" % len(after)),
            "desc": desc,
        })

    patched = bytes(buf)
    assert len(patched) == len(data), "原位补丁改变了长度"
    return patched, log


# ---------------------------------------------------------------------------
# 离线验证
# ---------------------------------------------------------------------------
def _expected_diff_offsets(key: int) -> set[int]:
    offs: set[int] = set()
    for addr, before, after, _desc in _patched_regions(key):
        base = _off(addr)
        for i, (b, a) in enumerate(zip(before, after)):
            if b != a:
                offs.add(base + i)
    return offs


def _unpatch(patched: bytes) -> bytes:
    buf = bytearray(patched)
    for addr, before, _after, _desc in _patched_regions(0x6B21):
        o = _off(addr)
        buf[o:o + len(before)] = before
    return bytes(buf)


def _uc_run_sync(image: bytes, ident: int, flag: int) -> dict:
    """用 Unicorn 执行 0x0800C8F8 同步字函数（RAM[0x200203E2]=flag）。"""
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC, UC_ARM_REG_CPSR)

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(KEY_FLAG_ADDR, bytes([flag & 0xFF]))
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))    # b .
    for addr in (WRITE_REG, WRITE_CMD, DELAY_US):
        uc.mem_write(addr, bytes.fromhex("7047"))       # bx lr

    calls: list[tuple] = []
    state = {"error": None, "insn": 0, "returned": False}

    def hook_code(_uc, address, _size, _user):
        state["insn"] += 1
        if address == UC_SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
            return
        if address == WRITE_REG:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        elif address == WRITE_CMD:
            calls.append(("cmd", _uc.reg_read(UC_ARM_REG_R0), None))
        elif address == DELAY_US:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    uc.reg_write(UC_ARM_REG_R1, 0x1111)
    uc.reg_write(UC_ARM_REG_R2, 0x2222)
    uc.reg_write(UC_ARM_REG_R3, 0x3333)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    try:
        uc.emu_start(SYNC_FN | 1, 0, 0, MAX_HARNESS_INSNS)
    except Exception as exc:  # noqa: BLE001
        state["error"] = "%s (pc=0x%08X, lr=0x%08X, sp=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR),
            uc.reg_read(UC_ARM_REG_SP))
    if state["error"] is None and not state["returned"]:
        state["error"] = ("未在 %d 条指令内返回（执行 %d 条，pc=0x%08X），疑似跑飞"
                          % (MAX_HARNESS_INSNS, state["insn"], uc.reg_read(UC_ARM_REG_PC)))
    return {"calls": calls, "error": state["error"]}


def _fmt_calls(calls: list[tuple]) -> str:
    parts = []
    for kind, a, b in calls:
        if kind == "wreg":
            parts.append("WriteReg(%d,0x%02X)" % (a, b))
        elif kind == "cmd":
            parts.append("WriteCmd(0x%02X)" % a)
        else:
            parts.append("delay(%d)" % a)
    return " -> ".join(parts)


def _check_patched(image: bytes, key: int, ident: int, flag: int) -> tuple[bool, str]:
    res = _uc_run_sync(image, ident, flag)
    if res["error"]:
        return False, "ID=%d flag=%d 补丁版 Unicorn 执行失败: %s" % (ident, flag, res["error"])
    calls = res["calls"]
    key_mode = (flag == KEY_FLAG_ON_VALUE)
    expect = key if key_mode else common.ID_NUMERATOR // (ident + 1)
    hi, lo = expect >> 8, expect & 0xFF
    want = [("wreg", 4, hi), ("wreg", 5, lo),
            ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
    ok = calls == want
    mode = "密钥模式(BEEP=ON)" if key_mode else "原厂公式(BEEP=OFF)"
    msg = ("ID=%-2d flag=%d %s: sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
           % (ident, flag, mode, expect, hi, lo, _fmt_calls(calls)))
    if not ok:
        msg += "  期望调用序列 %s" % _fmt_calls(want)
    if key_mode:
        ok = ok and expect != common.ID_NUMERATOR // (ident + 1)
    return ok, msg


def _uc_run_hook(image: bytes, hook_addr: int, regs: dict, flag: int) -> dict:
    """用 Unicorn 执行单个 hook（调用点已改成 BL hook），验证写 C.Fn 与 sync 调用。"""
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
    import unicorn.arm_const as ac

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(KEY_FLAG_ADDR, bytes([flag & 0xFF]))
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))
    for addr in (WRITE_REG, WRITE_CMD, DELAY_US):
        uc.mem_write(addr, bytes.fromhex("7047"))
    # 原厂模式下 C.Fn[6]=ID；测试时写一个已知 ID
    cfn_addr = CFN_BASE
    uc.mem_write(cfn_addr + 6, bytes([37]))
    uc.mem_write(cfn_addr + FLAG_IDX, bytes([flag & 0xFF]))
    uc.mem_write(CFN_MAX_TABLE_ADDR, CFN_MAX_TABLE)   # 最大值表（.data 初值）
    name2reg = {"r0": ac.UC_ARM_REG_R0, "r1": ac.UC_ARM_REG_R1, "r2": ac.UC_ARM_REG_R2,
                "r3": ac.UC_ARM_REG_R3, "r4": ac.UC_ARM_REG_R4}
    for name, val in regs.items():
        aliases = {"r0": ac.UC_ARM_REG_R0, "r1": ac.UC_ARM_REG_R1,
                   "r2": ac.UC_ARM_REG_R2, "r3": ac.UC_ARM_REG_R3,
                   "r4": ac.UC_ARM_REG_R4}
        uc.reg_write(aliases[name], val & 0xFFFFFFFF)
    calls: list[tuple] = []
    state = {"error": None, "returned": False, "insn": 0}

    def hook_code(_uc, address, _size, _user):
        state["insn"] += 1
        if address == UC_SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
            return
        if address == WRITE_REG:
            calls.append(("wreg", _uc.reg_read(ac.UC_ARM_REG_R0), _uc.reg_read(ac.UC_ARM_REG_R1)))
        elif address == WRITE_CMD:
            calls.append(("cmd", _uc.reg_read(ac.UC_ARM_REG_R0), None))
        elif address == DELAY_US:
            calls.append(("delay", _uc.reg_read(ac.UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(ac.UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(ac.UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(ac.UC_ARM_REG_CPSR, 0x01000000)
    try:
        uc.emu_start(hook_addr | 1, 0, 0, MAX_HARNESS_INSNS)
    except Exception as exc:  # noqa: BLE001
        state["error"] = str(exc)
    return {"calls": calls, "error": state["error"],
            "returned": state["returned"], "insn": state["insn"],
            "cfn2": uc.mem_read(cfn_addr + FLAG_IDX, 1)[0]}


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证：长度 + 静态字节 + 差异范围 + Unicorn 同步字/原厂/hook 行为。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    # 1) 长度不变
    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁）" % (len(patched), IMAGE_SIZE)))

    # 2) 同步字函数 52 字节与期望完全一致
    blob = build_sync_function(key)
    off = _off(SYNC_FN)
    got = bytes(patched[off:off + SYNC_FN_SIZE])
    results.append((got == blob,
                    "0x%08X..0x%08X 补丁区 %d 字节与期望函数体完全一致"
                    % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE) if got == blob else
                    "补丁区字节与期望不符\n  期望 %s\n  实际 %s"
                    % (blob.hex(" "), got.hex(" "))))

    # 3) 不越过下一函数
    results.append((SYNC_FN + len(blob) == SYNC_FN_END,
                    "函数体结束于 0x%08X == 下一函数起点 0x%08X（不越界）"
                    % (SYNC_FN + len(blob), SYNC_FN_END)))

    # 4) 字面量：+0x30 = FLAG
    flag_lit = int.from_bytes(got[0x30:0x34], "little")
    results.append((flag_lit == KEY_FLAG_ADDR,
                    "字面量 FLAG=0x%08X @0x%08X（C.Fn[%d] BEEP 开关字节）"
                    % (flag_lit, SYNC_FN + 0x30, FLAG_IDX)))

    # 5) hook 静态字节与调用点
    for hook_addr, orig, code, call_bytes, desc in build_hooks():
        o = _off(hook_addr)
        got_h = bytes(patched[o:o + len(code)])
        results.append((got_h == code, "hook 0x%08X = %s（%s）"
                        % (hook_addr, got_h.hex(" "), desc)))
    for call_addr, orig4, call_bytes, desc in (
            (CALL_INC, CALL_INC_ORIG, _t_bl(CALL_INC, HOOK_INC), "递增普通通路"),
            (CALL_DEC, CALL_DEC_ORIG, _t_bl(CALL_DEC, HOOK_DEC), "递减普通通路"),
            (CALL_WRAP_I, CALL_WRAP_I_ORIG, _t_bl(CALL_WRAP_I, HOOK_WRAP_I), "递增回绕通路"),
            (CALL_WRAP_D, CALL_WRAP_D_ORIG, _t_bl(CALL_WRAP_D, HOOK_WRAP_D), "递减回绕通路")):
        got_c = bytes(patched[_off(call_addr):_off(call_addr) + 4])
        results.append((got_c == call_bytes,
                        "调用点 0x%08X = %s（%s -> BL hook）"
                        % (call_addr, got_c.hex(" "), desc)))

    # 6) 与原厂差异必须恰好只落在补丁点集合内
    orig = _unpatch(patched)
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    expected = _expected_diff_offsets(key)
    results.append((set(diffs) == expected,
                    "与原厂差异 %d 字节，offset=%s（应恰为 %s）"
                    % (len(diffs), ["0x%05X" % d for d in diffs],
                       ["0x%05X" % d for d in sorted(expected)])))
    outside = [i for i in diffs if i not in expected]
    results.append((not outside, "差异之外无任何字节变化（%d 处）" % len(outside)))

    # 7) Unicorn 调用级
    try:
        import unicorn
        has_uc = True
        uc_ver = getattr(unicorn, "__version__", "?")
    except Exception:  # noqa: BLE001
        has_uc = False
        uc_ver = None
    if not has_uc:
        results.append((True, "警告: unicorn 不可用，跳过 Unicorn 调用级测试（静态检查已通过）"))
        return results

    results.append((True, "Unicorn %s 调用级测试：同步字 0x%08X，RAM[0x%08X]=C.Fn[%d] BEEP"
                    % (uc_ver, SYNC_FN | 1, KEY_FLAG_ADDR, FLAG_IDX)))
    for ident in (0, 5, 37, 99):
        results.append(_check_patched(patched, key, ident, flag=0))
        results.append(_check_patched(patched, key, ident, flag=KEY_FLAG_ON_VALUE))

    # 原厂镜像自证：同一 harness 应得到原厂公式 + delay/SCAL
    for ident in (0, 5, 99):
        res = _uc_run_sync(patched, ident, 0)   # flag=0 即原厂模式（补丁版）
        expect = common.ID_NUMERATOR // (ident + 1)
        want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF),
                ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
        ok = res["error"] is None and res["calls"] == want
        results.append((ok, "补丁版 BEEP=OFF 等价原厂 ID=%-2d：sync=0x%04X | %s"
                        % (ident, expect, _fmt_calls(res["calls"]))))

    # 8) hook 行为（直接在 hook 入口执行）
    flag = KEY_FLAG_ON_VALUE
    hi, lo = (key >> 8) & 0xFF, key & 0xFF
    want_key = [("wreg", 4, hi), ("wreg", 5, lo),
                ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
    ident = 37
    orig_sync = common.ID_NUMERATOR // (ident + 1)
    want_orig = [("wreg", 4, orig_sync >> 8), ("wreg", 5, orig_sync & 0xFF),
                 ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]

    def _hook_expect(new_flag: int):
        return (KEY_FLAG_ON_VALUE if new_flag == KEY_FLAG_ON_VALUE else None), \
               (want_key if new_flag == KEY_FLAG_ON_VALUE else want_orig)

    hook_cases = [
        # (名称, hook 地址, 入口寄存器, 初始 RAM flag, hook 执行后 C.Fn[2], 期望 sync)
        ("HOOK_INC", HOOK_INC, {"r1": CFN_BASE, "r2": FLAG_IDX, "r3": CFN_MAX_TABLE_ADDR,
                                "r4": 1}, 0, 1, want_key),
        ("HOOK_DEC", HOOK_DEC, {"r0": CFN_BASE, "r1": FLAG_IDX, "r3": 1}, 1, 0, want_orig),
        ("HOOK_WRAP_I", HOOK_WRAP_I, {"r0": 0, "r1": CFN_BASE, "r2": FLAG_IDX}, 1, 0, want_orig),
        ("HOOK_WRAP_D", HOOK_WRAP_D, {"r0": CFN_BASE, "r1": FLAG_IDX,
                                      "r2": CFN_MAX_TABLE_ADDR}, 0, 1, want_key),
    ]
    for name, addr, regs, ram_flag, expect_cfn2, expect_calls in hook_cases:
        res = _uc_run_hook(patched, addr, regs, flag=ram_flag)
        ok = (res["error"] is None and res["returned"] and res["cfn2"] == expect_cfn2
              and res["calls"] == expect_calls)
        results.append((ok, "[hook] %s 执行%s，C.Fn[2]=%d，sync 序列 %s%s"
                        % (name, "返回" if res["returned"] else "未返回(FAIL)",
                           res["cfn2"], _fmt_calls(res["calls"]),
                           "" if res["error"] is None else " err=%s" % res["error"])))

    return results
