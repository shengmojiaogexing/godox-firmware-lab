# -*- coding: utf-8 -*-
"""Godox X3-C（X3 C）引闪器 V1.38 —— 私有密钥「可开关 + 兼容双发」补丁（v3）。

设计见 keypatch/DESIGN_TOGGLE.md §3。v3 起用 **UI 里的"密钥开关"状态字节**
（KEY_ON_FLAG=0x20001080，ui-x3c 最终定址）控制密钥模式，
同时兼容 v2 的旧开关（ID == 99 且 ID-OFF 标志 == 0）。

语义
----
* 密钥模式（KEY_ON_FLAG != 0，或（当前 ID == 99 且 ID-OFF 标志 == 0））：
    写 CC2500 SYNC1/SYNC0 = KEY             -> 发一次原始帧（密钥灯收到）
    现算并写 SYNC = floor(50024/(当前ID+1))  -> 再发一次原始帧（同 CH/同 ID 普通灯收到）
  发完同步字停留在该普通同步字（= 原厂同 ID 状态），收包/后续帧不受影响。
* 否则：原厂行为，只发一次（同步字由原厂 0x08014864 在初始化/改 ID 时设置，本补丁不动它）。

实现（原位字节补丁，文件长度不变）
--------------------------------
1. 入口跳转：把两个"组帧函数"入口改成 4 字节 B.W，跳到注入代码：
     send_short(cmd)              0x080118B0  -> INJ_SHORT
     send_property(group,key,val) 0x0801184C  -> INJ_PROP
2. 注入代码写在主固件尾部的**空闲闪存** 0x080EE000（该区域全 0xFF，且经静态
   扫描无任何字面量/分支引用；0x080F8000 起是热靴子固件镜像，不碰）。
   与 ui-x3c 约定：本补丁用 0x080EE000 起约 0x150 字节，ui-x3c 广播注入从
   0x080F0000 起。
3. 注入代码通过"蹦床"调用原函数体（原函数前 4 字节是 push 参数 + 载入 GPIO
   基址的 PC 相对 LDR，蹦床用等价指令重建后再 B.W 跳到原函数 +4），因此
   原厂组帧/发送逻辑 100% 保留，只是包了一层"改同步字 + 调用两次"。
4. 第二帧的普通同步字用 Cortex-M4 硬件 UDIV 现算，不调用原厂 0x08014864
   （后者带 SCAL + 1ms 延时，会拖慢双发）。

代码洞复核（详见 keypatch/dev/x3c/NOTES.md）
--------------------------------------------
* 可用：0x080EE000..0x080F6FFF（约 36 KB 连续 0xFF），无字面量/分支引用。
  本补丁只占用 0x080EE000 起的约 0xDA 字节。
* 不可用：0x080ECF20（被 0x080ECD34 数据引用，疑似资源结束指针）、
  0x080F7000（FUN_0801fffc/08010c8c 引用）、0x080F7800（FUN_08021778 等
  的 0x8A 字节步长校准表基址）。0x080F8000 起是热靴子固件镜像。

辅助函数：写寄存器 0x08023540(reg,val)、strobe 0x08023520、延时 0x080232B6。
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
    "id": "x3c",
    "name": "Godox X3-C (X3 C trigger)",
    "image": "devices/x3c/firmware/X3_C_v1.38_app.bin",
    "kind": "trigger",                   # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "X3-C",
    "firmware_version": "V1.38",
    "base": 0x08004800,
    "sha256": "45fdd9522c432f4be60a470741d3e5c12a7ee629eb2b277aed5187639f85ef62",
    "out_name": "X3_C_v1.38_app_key{key:04X}.bin",
    "container_name": "X3C_V1.38_app_key{key:04X}_container.bin",
    "notes": "ID=99 密钥模式：KEY 帧 + ID99(500) 帧双发；其它 ID 原厂单发（原位补丁+注入）",
}

# ---- 镜像 -------------------------------------------------------------------
BASE = 0x08004800
IMAGE_SIZE = 1021468

# ---- 组帧函数（入口跳转点） -------------------------------------------------
SEND_SHORT = 0x080118B0            # send_short(cmd)
SEND_PROP = 0x0801184C             # send_property(group, key, value)
SEND_SHORT_ENTRY = bytes.fromhex("10b50a4c")    # push {r4,lr}; ldr r4,[pc,#0x28]
SEND_PROP_ENTRY = bytes.fromhex("70b50b4c")     # push {r4,r5,r6,lr}; ldr r4,[pc,#0x2c]
SEND_SHORT_RESUME = 0x080118B4     # 原函数体（跳过前 4 字节）
SEND_PROP_RESUME = 0x08011850

# ---- 注入区：主固件尾部空闲闪存 0x080EE000（全 0xFF，无任何引用） -----------
# 与 Lead / ui-x3c 约定：本补丁从 0x080EE000 起（约 0x100 字节），
# ui-x3c 的广播注入从 0x080F0000 起，中间留足余量。
# 复核：0x080EE000..0x080F6FFF 全 0xFF 且无任何字面量/分支引用；
#       0x080ECF20（资源结束指针）、0x080F7000/0x080F7800（校准/flash 测试表）
#       均有引用，不用；0x080F8000 起是热靴子固件镜像，绝对不碰。
INJ_BASE = 0x080EE000
INJ_SIZE = 0x400                   # 预留 1 KiB（实际注入约 0xDA 字节）
INJ_END = INJ_BASE + INJ_SIZE
INJ_ORIG = b"\xff" * INJ_SIZE      # 原厂该区域为擦除态 0xFF

# ---- RAM 变量（已由静态复核 + 反编译确认） -----------------------------------
ID_RAM = 0x20000055                # 当前 ID 字节（main 主循环 *DAT_08034330 传入）
ID_OFF_FLAG_RAM = 0x20000143       # ID-OFF/特殊状态标志（FUN_080118E8 用它选 FUN_0801616C）
# UI 密钥开关 KEY_ON_FLAG = 0x20001080（ui-x3c 最终定址）。
# 证据（ui-x3c）：全镜像无任何字面量引用；40M 指令模拟器写追踪只有 BSS 清零 1 次写，
# 之后从未被写 —— 安全 RAM 字节。注意：这是 **RAM 版**，重启复位（= 关）；后续会做
# flash 持久化。语义：非 0 = 开密钥。
KEY_ON_FLAG_RAM = 0x20001080
TX_GATE_RAM = 0x20000146           # 组帧函数发送门控（非 0 则不发送）
TX_BUF_RAM = 0x20000152            # 组帧缓冲区

# ---- UI 补丁集成（来自 keypatch/dev/x3c_ui/ui_patch.py，ui-x3c 交付 v1.1） -----
# 内容：注入 0x080F0000 起的 ui_key_toggle / ui_broadcast_allch / ui_broadcast_allid /
# 汉字字模 / 标签 + 快捷面板 KEY、BCAST 两个磁贴 + 磁贴按下轮询 + 页 1 入口跳板。
# 与核心补丁区（0x080EE000、0x080118B0/0x0801184C）完全不相交。
_UI_PATCH_DIR = os.path.join(_KEYPATCH_DIR, "dev", "x3c_ui")
ENABLE_UI_PATCH = True


def _load_ui_patch():
    """加载 ui_patch 模块（不存在则返回 None，核心补丁仍可用）。"""
    if not ENABLE_UI_PATCH:
        return None
    if _UI_PATCH_DIR not in sys.path:
        sys.path.insert(0, _UI_PATCH_DIR)
    try:
        import ui_patch  # noqa: WPS433
        return ui_patch
    except Exception:  # noqa: BLE001
        return None


def _ui_opts(key: int) -> dict:
    return {
        "key": key,
        "key_flag": KEY_ON_FLAG_RAM,
        "enable_menu_patch": True,
    }


def _original_image() -> bytes:
    root = os.path.dirname(_KEYPATCH_DIR)
    return common.read_file(os.path.join(root, DEVICE["image"]))

ID_KEY_MODE = 99                   # 兼容旧方案的密钥模式 ID（v3 仍保留该触发条件）
SYNC_NUMERATOR = common.ID_NUMERATOR   # 50024 = 0xC368（原厂同步字公式分子）
ID99_SYNC = SYNC_NUMERATOR // (ID_KEY_MODE + 1)   # 50024/100 = 500 = 0x01F4（仅供文档/测试）
ID99_HI = (ID99_SYNC >> 8) & 0xFF
ID99_LO = ID99_SYNC & 0xFF

# ---- 辅助函数 ---------------------------------------------------------------
WRITE_REG = 0x08023540             # CC2500_WriteReg(reg=r0, val=r1)
WRITE_CMD = 0x08023520             # CC2500_WriteCmd(strobe=r0)
DELAY = 0x080232B6                 # delay(n=r0)
LOW_SEND_SHORT = 0x0801712C        # 低层：发送 2 字节 FIFO 帧
LOW_SEND_PROP = 0x0801708C         # 低层：发送 4 字节 FIFO 帧

# ---- 物理按键长按保底方案（v3p/v3k2：core + 按键注入） -----------------------
# 按键扫描 FUN_0800CC30（main 0x08034322，每主循环调用一次）读 3 个键：
#   bit0 = GPIOA bit9 (mask 0x200, PA9)
#   bit1 = GPIOA bit8 (mask 0x100, PA8)
#   bit2 = GPIOB bit15(mask 0x8000, PB15)
# 原厂行为见 NOTES §10/§11。物理丝印（M/C / ⚡ / 拨轮）无法静态确证 →
# 先用"探针版"（长按三个键分别闪 1/2/3 次）让用户确认键位。
KEYSCAN_FN = 0x0800CC30
KEYSCAN_RESUME = 0x0800CC34
KEYSCAN_ENTRY_BEFORE = bytes.fromhex("2de9f05f")   # push.w {r4,r5,r6,r7,r8,sb,sl,fp,ip,lr}
KEYSCAN_LONG1_CMP = 0x0800CD14     # cmp r0,#0x28（bit1 原厂长按阈值）
KEYSCAN_LONG2_CMP = 0x0800CD76     # cmp r0,#0x1E（bit2 原厂长按阈值）
KEYSCAN_CMP_BEFORE = bytes.fromhex("2828")
KEYSCAN_CMP2_BEFORE = bytes.fromhex("1e28")
KEYSCAN_CMP_DISABLED = bytes.fromhex("ff28")

BTN_INJ_BASE = 0x080EE400          # core 用 0x080EE000..0x080EE144
BTN_INJ_SIZE = 0x800               # 0x080EE400..0x080EEC00（0x080F0000 起留给 ui-x3c）
BTN_INJ_ORIG = b"\xff" * BTN_INJ_SIZE

GPIOA_BASE = 0x40020000
GPIOB_BASE = 0x40020400
BTN0_PORT, BTN0_MASK = GPIOA_BASE, 0x0200     # PA9
BTN1_PORT, BTN1_MASK = GPIOA_BASE, 0x0100     # PA8
BTN2_PORT, BTN2_MASK = GPIOB_BASE, 0x8000     # PB15

# 长按阈值（扫描次数）。实机反馈 60 次可能偏长（主循环扫描周期未标定，
# 可能 30~80ms/次），探针版用 25、正式版用 30，确保 ~1.5s 按住能触发。
BTN_PROBE_TICKS = 25
BTN_KEYBTN2_TICKS = 30
# keybtn2 键位假设：拨轮 = PA9(bit0)，⚡ = PB15(bit2)，M/C(bit1) 不绑定。
# 若探针显示拨轮/⚡ 分别是别的键，只改下面两组常量后重建（不用改其它代码）。
BTN_WHEEL_PORT, BTN_WHEEL_MASK = BTN0_PORT, BTN0_MASK
BTN_TRIG_PORT, BTN_TRIG_MASK = BTN2_PORT, BTN2_MASK
BTN_DISABLE_ORIGINAL_LONG = True   # 手势占用的 bit1/bit2 原厂长按动作禁用

# 按键手势私有 RAM（KEY_ON=0x20001080；0x20001084..8F 已被 ui-x3c 占用）
BTN_RAM0_CNT, BTN_RAM0_FIRED = 0x20001090, 0x20001091
BTN_RAM1_CNT, BTN_RAM1_FIRED = 0x20001092, 0x20001093
BTN_RAM2_CNT, BTN_RAM2_FIRED = 0x20001094, 0x20001095
STATE_RAM = 0x20000030             # 按键状态结构基址（r7）
STATE_INHIBIT_RAM = 0x20000035     # state[5]：原厂"按键锁"计数（本补丁不再据此跳过手势）
CHANNEL_RAM = 0x20000054           # 当前信道（1-based）
GROUP_TABLE_RAM = 0x20000B68       # 16 组设置表，每项 6 字节
READ_KEY = 0x08022EB4              # FUN_08022EB4(port, mask)：返回 0=按下
SET_CHANNEL = 0x08014814           # 设置信道 ch（0-based）
SET_SYNC = 0x08014864              # 按 ID 设置同步字（原厂，用于广播后恢复）
BTN_PROP_GROUP = 0x50
BTN_PROP_POWER = 0xD1
BTN_PROP_LAMP = 0xB1
BTN_TEST_CMD = 0x09

# ---- 原厂关机链路（证据见 NOTES §12）---------------------------------------
SHUTDOWN_WRAPPER = 0x08035FD0   # 原厂长按关机上层流程（内部调 0x08010C00）
SHUTDOWN_BODY = 0x08010C00      # 关机本体：停显示/RF -> 关屏 -> 等待 -> 断电序列
TEARDOWN_DISPLAY = 0x08013CDC   # 原厂长按分支关机前的显示收尾
BTN_KEYBTN4_TICKS1 = 30         # 1.5s：切密钥
BTN_KEYBTN4_TICKS2 = 400        # 20s：关机（按 30 tick≈1.5s 换算）
BTN_RAM_WHEEL_CNT16 = 0x20001096   # u16 长按计数
BTN_RAM_WHEEL_FIRED1 = 0x20001098  # 1.5s 阶段已触发
BTN_RAM_WHEEL_FIRED2 = 0x20001099  # 20s 阶段已触发
BTN_KEYBTN5_TICKS1 = 30         # 1.5s：切密钥
BTN_KEYBTN5_TICKS2 = 400        # 20s：关机
BTN_KEYBTN5_WATCHDOG = 600      # 30s：异常一直按下时强制清零（防卡死）
BTN_KEYBTN5_DEBOUNCE = 3        # 连续 3 次同状态才计数（去抖）
BTN_RAM5_CNT16 = 0x20001096     # u16 长按计数（单计数器，无 latch）
BTN_RAM5_STREAK = 0x20001098    # u8 连续按下次数（去抖）
# ---- keybtn7：双击 ⚡(PA8) -> 32ch × 100ID 公共同步字广播（分块状态机） ----
TICK_RAM = 0x20000310          # u32 1ms tick（读 0x080334D8 / 加 0x080334EC；0x2000030C 锁字节）
BTN7_CLICK_MS = 300            # 短按上限
BTN7_DOUBLE_MS = 400           # 双击间隔上限
IWDG_FEED = 0x0803BA4C         # 原厂喂狗函数（写 IWDG_KR=0xAAAA；main/按键扫描都会调）
BTN7_BUDGET = 8                # 每次主循环最多发 8 帧（分块，降低射频负载）
BTN7_CHUNK_MS = 100            # 两批之间的最小间隔（ms），降低平均发射占空比（全 ID 广播约 40 秒）
BTN7_LAST_TICK = 0x200010B0    # u32 上一批时间戳
BTN7_PRESS_START = 0x200010A0  # u32
BTN7_FIRST_PRESS = 0x200010A4  # u32
BTN7_PENDING = 0x200010A8      # u8
BTN7_PREV = 0x200010A9         # u8
BTN7_ACTIVE = 0x200010AA       # u8 广播进行中
BTN7_SAVED_KEYON = 0x200010AB  # u8 广播前的 KEY_ON
BTN7_CH = 0x200010AC           # u8 当前 channel 0..31
BTN7_ID = 0x200010AD           # u8 当前 id 0..99

# ---- Unicorn 测试用内存布局 -------------------------------------------------
UC_FLASH_ALIGN = 0x08004000
UC_FLASH_SIZE = 0x100000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x80000
UC_PERIPH = 0x40000000
UC_PERIPH_SIZE = 0x100000
UC_STACK = 0x20047000
UC_SENTINEL = 0x20040000


# ===========================================================================
# 极小 Thumb-2 汇编器（只覆盖本补丁用到的指令）
# ===========================================================================
def _pack16(hw: int) -> bytes:
    return struct.pack("<H", hw & 0xFFFF)


# 条件分支（T1，1101 cond imm8）操作码基址
_COND = {
    "beq": 0xD000,
    "bne": 0xD100,
    "bcc": 0xD300,   # LO / CC
    "bcs": 0xD200,   # HS
    "bhi": 0xD800,
}


class _Asm:
    """带标签/回填的两遍式 Thumb 汇编器。"""

    def __init__(self, base: int):
        self.base = base
        self.buf = bytearray()
        self.labels: dict[str, int] = {}
        self.fix: list[tuple[str, int, object]] = []

    def here(self) -> int:
        return self.base + len(self.buf)

    def label(self, name: str) -> None:
        if name in self.labels:
            raise ValueError("重复标签 %s" % name)
        self.labels[name] = self.here()

    def emit(self, b: bytes) -> None:
        self.buf += b

    # --- 指令 -----------------------------------------------------------
    def movs(self, rd: int, imm: int) -> None:
        if not (0 <= rd <= 7 and 0 <= imm <= 0xFF):
            raise ValueError("movs imm8 越界 rd=%r imm=%r" % (rd, imm))
        self.emit(_pack16(0x2000 | (rd << 8) | imm))

    def mov(self, rd: int, rm: int) -> None:
        if not (0 <= rd <= 7 and 0 <= rm <= 7):
            raise ValueError("mov reg T1 只支持 r0-r7")
        self.emit(_pack16(0x4600 | (((rd >> 3) & 1) << 7)
                          | ((rm & 0xF) << 3) | (rd & 7)))

    def cmp_imm(self, rn: int, imm: int) -> None:
        if not (0 <= rn <= 7 and 0 <= imm <= 0xFF):
            raise ValueError("cmp imm8 越界")
        self.emit(_pack16(0x2800 | (rn << 8) | imm))

    def ldrb(self, rd: int, rn: int, imm5: int = 0) -> None:
        # LDRB (immediate, T1): 0111 1 imm5 Rn Rt
        self.emit(_pack16(0x7800 | ((imm5 & 0x1F) << 6)
                          | ((rn & 7) << 3) | (rd & 7)))

    def adds_imm(self, rd: int, rn: int, imm: int) -> None:
        # ADD (immediate, T1): 0001 110 imm3 Rn Rd（0 <= imm <= 7）
        if not (0 <= imm <= 7):
            raise ValueError("adds_imm T1 立即数 0..7")
        self.emit(_pack16(0x1C00 | (imm << 6) | ((rn & 7) << 3) | (rd & 7)))

    def lsrs_imm(self, rd: int, rm: int, imm5: int) -> None:
        # LSR (immediate, T1): 0000 1 imm5 Rm Rd
        if not (1 <= imm5 <= 31):
            raise ValueError("lsrs_imm 1..31")
        self.emit(_pack16(0x0800 | (imm5 << 6) | ((rm & 7) << 3) | (rd & 7)))

    def uxtb(self, rd: int, rm: int) -> None:
        # UXTB (T1): 1011 0010 1101 Rm Rd
        self.emit(_pack16(0xB2C0 | ((rm & 7) << 3) | (rd & 7)))

    def udiv(self, rd: int, rn: int, rm: int) -> None:
        # UDIV (T1): hw1 = 0xFBB0 | Rn, hw2 = 0xF0F0 | (Rd<<8) | Rm
        # 与原厂 0x0801486C 的 `udiv r4,r1,r0` = b1 fb f0 f4 编码一致。
        self.emit(struct.pack("<HH", 0xFBB0 | (rn & 0xF),
                              0xF0F0 | ((rd & 0xF) << 8) | (rm & 0xF)))

    # --- 按键注入额外用到的 Thumb-1 指令 -------------------------------
    def strb_imm(self, rt: int, rn: int, imm5: int = 0) -> None:
        # STRB (immediate, T1): 0111 0 imm5 Rn Rt
        self.emit(_pack16(0x7000 | ((imm5 & 0x1F) << 6)
                          | ((rn & 7) << 3) | (rt & 7)))

    def subs_imm(self, rd: int, rn: int, imm: int) -> None:
        # SUBS (immediate, T1): 0011 100 imm3 Rn Rd（0 <= imm <= 7）
        if not (0 <= imm <= 7):
            raise ValueError("subs_imm T1 立即数 0..7")
        self.emit(_pack16(0x1E00 | (imm << 6) | ((rn & 7) << 3) | (rd & 7)))

    def eors(self, rdn: int, rm: int) -> None:
        # EORS (register, T1): 0100 0000 01 Rm Rdn
        self.emit(_pack16(0x4040 | ((rm & 7) << 3) | (rdn & 7)))

    def ands(self, rdn: int, rm: int) -> None:
        # ANDS (register, T1): 0100 0000 00 Rm Rdn
        self.emit(_pack16(0x4000 | ((rm & 7) << 3) | (rdn & 7)))

    def bx_lr(self) -> None:
        self.emit(_pack16(0x4770))

    def ldrh(self, rt: int, rn: int, byte_off: int) -> None:
        # LDRH (immediate, T1): 1000 1 imm5 Rn Rt，imm5 = byte_off/2
        imm5 = byte_off // 2
        if byte_off % 2 or not (0 <= imm5 <= 0x1F):
            raise ValueError("ldrh 偏移需为 0..62 的偶数")
        self.emit(_pack16(0x8800 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def strh(self, rt: int, rn: int, byte_off: int) -> None:
        # STRH (immediate, T1): 1000 0 imm5 Rn Rt，imm5 = byte_off/2
        imm5 = byte_off // 2
        if byte_off % 2 or not (0 <= imm5 <= 0x1F):
            raise ValueError("strh 偏移需为 0..62 的偶数")
        self.emit(_pack16(0x8000 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def ldr(self, rt: int, rn: int, byte_off: int) -> None:
        # LDR (immediate, T1): 0110 1 imm5 Rn Rt，imm5 = byte_off/4
        imm5 = byte_off // 4
        if byte_off % 4 or not (0 <= imm5 <= 0x1F):
            raise ValueError("ldr 偏移需为 0..124 的 4 的倍数")
        self.emit(_pack16(0x6800 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def bcs(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("bcs", off, target))

    def ldr(self, rt: int, rn: int, byte_off: int) -> None:
        # LDR (immediate, T1): 0110 1 imm5 Rn Rt
        imm5 = byte_off // 4
        if byte_off % 4 or not (0 <= imm5 <= 0x1F):
            raise ValueError("ldr 偏移需为 0..124 的 4 的倍数")
        self.emit(_pack16(0x6800 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def str(self, rt: int, rn: int, byte_off: int) -> None:
        # STR (immediate, T1): 0110 0 imm5 Rn Rt
        imm5 = byte_off // 4
        if byte_off % 4 or not (0 <= imm5 <= 0x1F):
            raise ValueError("str 偏移需为 0..124 的 4 的倍数")
        self.emit(_pack16(0x6000 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def subs_reg(self, rd: int, rn: int, rm: int) -> None:
        # SUBS (register, T1): 0001 101 Rm Rn Rd
        self.emit(_pack16(0x1A00 | ((rm & 7) << 6) | ((rn & 7) << 3) | (rd & 7)))

    def bcs(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("bcs", off, target))

    def _far_cond(self, cond, target) -> None:
        """条件满足时跳到远处 target：用反向条件跳过 b.w。"""
        inv = {"beq": "bne", "bne": "beq", "bcs": "bcc", "bcc": "bcs"}[cond]
        lbl = "__far_%d" % len(self.buf)
        getattr(self, inv)(lbl)
        self.b_w(target)
        self.label(lbl)

    def beq_far(self, target) -> None:
        self._far_cond("beq", target)

    def bne_far(self, target) -> None:
        self._far_cond("bne", target)

    def bcs_far(self, target) -> None:
        self._far_cond("bcs", target)

    def cmp_reg(self, rn: int, rm: int) -> None:
        # CMP (register, T1): 0100 0010 10 Rm Rn
        self.emit(_pack16(0x4280 | ((rm & 7) << 3) | (rn & 7)))

    def beq(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("beq", off, target))

    def bcc(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("bcc", off, target))

    def movw(self, rd: int, imm16: int) -> None:
        imm4 = (imm16 >> 12) & 0xF
        i = (imm16 >> 11) & 1
        imm3 = (imm16 >> 8) & 0x7
        imm8 = imm16 & 0xFF
        hw1 = 0xF240 | (i << 10) | imm4
        hw2 = (imm3 << 12) | ((rd & 0xF) << 8) | imm8
        self.emit(struct.pack("<HH", hw1, hw2))

    def movt(self, rd: int, imm16: int) -> None:
        imm4 = (imm16 >> 12) & 0xF
        i = (imm16 >> 11) & 1
        imm3 = (imm16 >> 8) & 0x7
        imm8 = imm16 & 0xFF
        hw1 = 0xF2C0 | (i << 10) | imm4
        hw2 = (imm3 << 12) | ((rd & 0xF) << 8) | imm8
        self.emit(struct.pack("<HH", hw1, hw2))

    def push(self, low_regs: int, lr: bool = False) -> None:
        self.emit(_pack16(0xB400 | (0x100 if lr else 0) | (low_regs & 0xFF)))

    def pop(self, low_regs: int, pc: bool = False) -> None:
        self.emit(_pack16(0xBC00 | (0x100 if pc else 0) | (low_regs & 0xFF)))

    def b_w(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00\x00\x00")
        self.fix.append(("bw", off, target))

    def bl(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00\x00\x00")
        self.fix.append(("bl", off, target))

    def bne(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("bne", off, target))

    # --- 回填 -----------------------------------------------------------
    def _resolve(self, target) -> int:
        if isinstance(target, str):
            if target not in self.labels:
                raise ValueError("未定义标签 %s" % target)
            return self.labels[target]
        return int(target)

    @staticmethod
    def _branch_imm(addr: int, target: int) -> int:
        off = target - (addr + 4)
        if off % 2:
            raise ValueError("分支目标未按 2 字节对齐: 0x%08X -> 0x%08X" % (addr, target))
        if not (-(1 << 24) <= off < (1 << 24)):
            raise ValueError("分支超出 ±16MB: 0x%08X -> 0x%08X" % (addr, target))
        return off

    @staticmethod
    def _enc_bl(off: int) -> bytes:
        if off & 1:
            raise ValueError("BL 目标未对齐")
        s = 1 if off < 0 else 0
        imm = off & 0x1FFFFFF
        i1 = (imm >> 23) & 1
        i2 = (imm >> 22) & 1
        imm10 = (imm >> 12) & 0x3FF
        imm11 = (imm >> 1) & 0x7FF
        j1 = (1 - i1) ^ s
        j2 = (1 - i2) ^ s
        hw1 = 0xF000 | (s << 10) | imm10
        hw2 = 0xD000 | (j1 << 13) | (j2 << 11) | imm11
        return struct.pack("<HH", hw1, hw2)

    @staticmethod
    def _enc_bw(off: int) -> bytes:
        if off & 1:
            raise ValueError("B.W 目标未对齐")
        s = 1 if off < 0 else 0
        imm = off & 0x1FFFFFF
        i1 = (imm >> 23) & 1
        i2 = (imm >> 22) & 1
        imm10 = (imm >> 12) & 0x3FF
        imm11 = (imm >> 1) & 0x7FF
        j1 = (1 - i1) ^ s
        j2 = (1 - i2) ^ s
        hw1 = 0xF000 | (s << 10) | imm10
        hw2 = 0x9000 | (j1 << 13) | (j2 << 11) | imm11
        return struct.pack("<HH", hw1, hw2)

    def build(self) -> bytes:
        for kind, off, target in self.fix:
            tgt = self._resolve(target)
            addr = self.base + off
            if kind in _COND:
                imm = tgt - (addr + 4)
                if imm % 2 or not (-256 <= imm <= 254):
                    raise ValueError("%s 越界: 0x%08X -> 0x%08X (%d)"
                                     % (kind, addr, tgt, imm))
                hw = _COND[kind] | ((imm // 2) & 0xFF)
                self.buf[off:off + 2] = _pack16(hw)
            else:
                imm = self._branch_imm(addr, tgt)
                enc = self._enc_bw(imm) if kind == "bw" else self._enc_bl(imm)
                self.buf[off:off + 4] = enc
        return bytes(self.buf)


# ===========================================================================
# 注入代码生成
# ===========================================================================
def _emit_dispatch(a: _Asm, single_label: str, keymode_label: str) -> None:
    """密钥模式判定（v3）：

        if (KEY_ON_FLAG != 0) or (当前ID == 99 and ID-OFF标志 == 0) -> keymode
        否则 -> single_label（原厂单发）
    """
    # UI 密钥开关 KEY_ON_FLAG=0x20001080（ui-x3c 最终定址）
    a.movw(3, KEY_ON_FLAG_RAM & 0xFFFF)
    a.movt(3, (KEY_ON_FLAG_RAM >> 16) & 0xFFFF)
    a.ldrb(3, 3)
    a.cmp_imm(3, 0)
    a.bne(keymode_label)
    # 兼容旧方案：ID == 99 且 ID-OFF 标志 == 0
    a.movw(0, ID_RAM & 0xFFFF)
    a.movt(0, (ID_RAM >> 16) & 0xFFFF)
    a.ldrb(0, 0)
    a.cmp_imm(0, ID_KEY_MODE)
    a.bne(single_label)
    a.movw(1, ID_OFF_FLAG_RAM & 0xFFFF)
    a.movt(1, (ID_OFF_FLAG_RAM >> 16) & 0xFFFF)
    a.ldrb(1, 1)
    a.cmp_imm(1, 0)
    a.bne(single_label)
    a.label(keymode_label)


def _emit_sync_key(a: _Asm, key: int) -> None:
    """写 CC2500 SYNC = 私有 KEY。"""
    a.movs(0, 4)
    a.movs(1, (key >> 8) & 0xFF)
    a.bl(WRITE_REG)
    a.movs(0, 5)
    a.movs(1, key & 0xFF)
    a.bl(WRITE_REG)


def _emit_ordinary_sync(a: _Asm, hi: bool) -> None:
    """现算并写 CC2500 SYNC = floor(50024 / (当前 ID + 1)) 的高/低字节。

    不调用原厂 0x08014864（它带 SCAL + 1ms 延时）；这里用 M4 硬件 UDIV 现算，
    直接写 reg4/reg5，和原厂公式完全一致：
        ID=5  -> 50024/6   = 8338 = 0x2091
        ID=99 -> 50024/100 = 500  = 0x01F4
    """
    a.movw(0, ID_RAM & 0xFFFF)
    a.movt(0, (ID_RAM >> 16) & 0xFFFF)
    a.ldrb(0, 0)
    a.adds_imm(0, 0, 1)                    # r0 = ID + 1
    a.movw(1, SYNC_NUMERATOR & 0xFFFF)     # r1 = 50024 = 0xC368
    a.udiv(2, 1, 0)                        # r2 = 50024 / (ID+1)
    if hi:
        a.lsrs_imm(1, 2, 8)
        a.movs(0, 4)
    else:
        a.uxtb(1, 2)
        a.movs(0, 5)
    a.bl(WRITE_REG)


def build_injection(key: int) -> tuple[bytes, dict]:
    """生成注入区字节（INJ_BASE 起）与布局信息。"""
    a = _Asm(INJ_BASE)

    # 1) send_short(cmd) 包装
    a.label("inj_short")
    a.push(0x10, lr=True)          # push {r4,lr}
    a.mov(4, 0)                    # r4 = cmd
    _emit_dispatch(a, "short_single", "short_keymode")
    # 双发白名单：按键 TEST(0x09) + 热靴触发路径命令(0x1A/0x21/0x29) 双发；
    # 其它命令（休眠/同步/心跳/设置状态等）保持原厂单发，避免密钥帧把休眠帧
    # 送到密钥灯（用户实机"快休眠时灯滴滴响"）。
    a.cmp_imm(4, 0x09)
    a.beq("short_double")
    a.cmp_imm(4, 0x1A)
    a.beq("short_double")
    a.cmp_imm(4, 0x21)
    a.beq("short_double")
    a.cmp_imm(4, 0x29)
    a.beq("short_double")
    a.b_w("short_single")
    a.label("short_double")
    # 密钥帧：SYNC=KEY
    _emit_sync_key(a, key)
    a.mov(0, 4)
    a.bl("tramp_short")
    # 当前 ID 的普通同步字帧（发送前现算；发完同步字停留在该值 = 原厂状态）
    _emit_ordinary_sync(a, hi=True)
    _emit_ordinary_sync(a, hi=False)
    a.mov(0, 4)
    a.bl("tramp_short")
    a.pop(0x10, pc=True)
    a.label("short_single")
    a.mov(0, 4)
    a.bl("tramp_short")
    a.pop(0x10, pc=True)

    # 2) send_property(group, key, value) 包装
    a.label("inj_prop")
    a.push(0x70, lr=True)          # push {r4,r5,r6,lr}
    a.mov(4, 0)
    a.mov(5, 1)
    a.mov(6, 2)
    _emit_dispatch(a, "prop_single", "prop_keymode")
    _emit_sync_key(a, key)
    a.mov(0, 4)
    a.mov(1, 5)
    a.mov(2, 6)
    a.bl("tramp_prop")
    _emit_ordinary_sync(a, hi=True)
    _emit_ordinary_sync(a, hi=False)
    a.mov(0, 4)
    a.mov(1, 5)
    a.mov(2, 6)
    a.bl("tramp_prop")
    a.pop(0x70, pc=True)
    a.label("prop_single")
    a.mov(0, 4)
    a.mov(1, 5)
    a.mov(2, 6)
    a.bl("tramp_prop")
    a.pop(0x70, pc=True)

    # 3) 蹦床：重建原函数被覆盖的 4 字节序言，再跳回原函数体
    a.label("tramp_short")
    a.push(0x10, lr=True)
    a.movw(4, 0x0414)              # r4 = 0x40020414（原 ldr r4,[pc] 载入的 GPIO 基址）
    a.movt(4, 0x4002)
    a.b_w(SEND_SHORT_RESUME)

    a.label("tramp_prop")
    a.push(0x70, lr=True)
    a.movw(4, 0x0414)
    a.movt(4, 0x4002)
    a.b_w(SEND_PROP_RESUME)

    code = a.build()
    if len(code) > INJ_SIZE:
        raise ValueError("注入代码 %d 字节超出预留空闲区 %d 字节"
                         % (len(code), INJ_SIZE))
    layout = {
        "inj_short": a.labels["inj_short"],
        "inj_prop": a.labels["inj_prop"],
        "short_keymode": a.labels["short_keymode"],
        "prop_keymode": a.labels["prop_keymode"],
        "tramp_short": a.labels["tramp_short"],
        "tramp_prop": a.labels["tramp_prop"],
        "size": len(code),
    }
    return code, layout


def _off(addr: int) -> int:
    return addr - BASE


def _patched_regions(key: int) -> list[tuple[int, bytes, bytes, str]]:
    """返回 [(addr, before, after, desc)]；before 取自已确认的原厂常量。"""
    inj_code, layout = build_injection(key)
    inj_before = INJ_ORIG[:len(inj_code)]

    b1 = _Asm(SEND_SHORT)
    b1.b_w(layout["inj_short"])
    b2 = _Asm(SEND_PROP)
    b2.b_w(layout["inj_prop"])
    return [
        (SEND_SHORT, SEND_SHORT_ENTRY, b1.build(),
         "send_short(0x080118B0) 入口: B.W -> 注入包装"),
        (SEND_PROP, SEND_PROP_ENTRY, b2.build(),
         "send_property(0x0801184C) 入口: B.W -> 注入包装"),
        (INJ_BASE, inj_before, inj_code,
         "空闲闪存 0x080EE000 注入：密钥模式双发（KEY 帧 + 当前ID普通同步字帧）"),
    ]


# ===========================================================================
# 打补丁
# ===========================================================================
def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """把原厂镜像 data 打上"ID=99 密钥模式双发"补丁，返回 (patched_bytes, patch_log)。"""
    key = common.validate_key(key)
    if len(data) != IMAGE_SIZE:
        raise ValueError("X3-C V1.38 固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
                         % (IMAGE_SIZE, len(data)))

    inj_code, _layout = build_injection(key)

    # 版本/完整性校验：两个入口必须与原厂一致；注入区必须是擦除态 0xFF
    got = bytes(data[_off(SEND_SHORT):_off(SEND_SHORT) + 4])
    if got != SEND_SHORT_ENTRY:
        raise ValueError("0x%08X 原始字节不匹配：期望 %s 实际 %s（固件版本不符）"
                         % (SEND_SHORT, SEND_SHORT_ENTRY.hex(" "), got.hex(" ")))
    got = bytes(data[_off(SEND_PROP):_off(SEND_PROP) + 4])
    if got != SEND_PROP_ENTRY:
        raise ValueError("0x%08X 原始字节不匹配：期望 %s 实际 %s（固件版本不符）"
                         % (SEND_PROP, SEND_PROP_ENTRY.hex(" "), got.hex(" ")))
    got = bytes(data[_off(INJ_BASE):_off(INJ_BASE) + len(inj_code)])
    if got != b"\xff" * len(inj_code):
        raise ValueError("注入区 0x%08X 起 %d 字节不是擦除态 0xFF（固件版本不符）"
                         % (INJ_BASE, len(inj_code)))

    buf = bytearray(data)
    log: list[dict] = []
    for addr, before, after, desc in _patched_regions(key):
        o = _off(addr)
        got = bytes(buf[o:o + len(before)])
        if got != before:
            raise ValueError("补丁点 0x%08X 原始字节不匹配：期望 %s 实际 %s"
                             % (addr, before.hex(" "), got.hex(" ")))
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

    # ---- 叠加 UI 补丁（快捷面板磁贴 + 广播/密钥注入 + 点击轮询） ----
    ui = _load_ui_patch()
    if ui is not None:
        try:
            patched, ui_log = ui.apply_ui(patched, _ui_opts(key))
        except Exception as exc:  # noqa: BLE001
            raise ValueError("UI 补丁失败: %s" % exc)
        for e in ui_log:
            log.append({
                "addr": e.get("addr", 0),
                "offset": e.get("offset", 0),
                "before": str(e.get("before", "")),
                "after": str(e.get("after", "")),
                "desc": "[UI] " + str(e.get("desc", "")),
            })
    if len(patched) != len(data):
        raise ValueError("叠加 UI 补丁后长度变化（%d -> %d）" % (len(data), len(patched)))
    return patched, log


# ===========================================================================
# 离线验证
# ===========================================================================
def _unpatch(patched: bytes) -> bytes:
    """把补丁还原成原厂字节（原厂 harness 自证用）。含核心补丁 + UI 补丁。"""
    buf = bytearray(patched)
    buf[_off(SEND_SHORT):_off(SEND_SHORT) + 4] = SEND_SHORT_ENTRY
    buf[_off(SEND_PROP):_off(SEND_PROP) + 4] = SEND_PROP_ENTRY
    buf[_off(INJ_BASE):_off(INJ_END)] = INJ_ORIG
    # 还原 UI 补丁区域：用原厂镜像与 ui_patch 的差异集逐字节恢复
    try:
        ui = _load_ui_patch()
        if ui is not None:
            orig = _original_image()
            ui_img, _ = ui.apply_ui(orig, _ui_opts(0x6B21))
            for i in range(min(len(buf), len(orig))):
                if ui_img[i] != orig[i]:
                    buf[i] = orig[i]
    except Exception:  # noqa: BLE001
        pass
    return bytes(buf)


def _expected_diff_offsets(key: int) -> set[int]:
    offs: set[int] = set()
    for addr, before, after, _desc in _patched_regions(key):
        base_off = _off(addr)
        for i, (b, a) in enumerate(zip(before, after)):
            if b != a:
                offs.add(base_off + i)
    return offs


def _uc_run(patched: bytes, entry: int, args: tuple, ident: int,
            id_off: int = 0, key_on: int = 0) -> dict:
    """用 Unicorn 调用注入后的入口函数，记录 CC2500 写寄存器与低层发送调用。"""
    from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB,
                         UC_MODE_MCLASS, UC_HOOK_CODE)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_SP,
                                   UC_ARM_REG_CPSR)

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_FLASH_ALIGN, UC_FLASH_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_map(UC_PERIPH, UC_PERIPH_SIZE)
    uc.mem_write(BASE, patched)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))    # b .（返回哨兵）
    # RAM 初值：当前 ID、ID-OFF 标志、UI 密钥开关、发送门控
    uc.mem_write(ID_RAM, bytes([ident & 0xFF]))
    uc.mem_write(ID_OFF_FLAG_RAM, bytes([id_off & 0xFF]))
    uc.mem_write(KEY_ON_FLAG_RAM, bytes([key_on & 0xFF]))
    uc.mem_write(TX_GATE_RAM, b"\x00")

    calls: list[tuple] = []
    state = {"error": None}

    def hook(_uc, address, _size, _user):
        if address == UC_SENTINEL:
            _uc.emu_stop()
            return
        if address == WRITE_REG:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0),
                          _uc.reg_read(UC_ARM_REG_R1)))
        elif address == LOW_SEND_SHORT:
            calls.append(("send_short", None, None))
        elif address == LOW_SEND_PROP:
            calls.append(("send_prop", None, None))
        elif address == DELAY:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))
        else:
            return
        # 辅助函数不执行函数体，直接返回到调用者（LR 原值，bit0=1 保持 Thumb）
        _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    arg_regs = (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2)
    for i, v in enumerate(args):
        uc.reg_write(arg_regs[i], v)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)   # Thumb
    try:
        uc.emu_start(entry | 1, 0, count=200000)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X lr=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR))
    return {"calls": calls, "error": state["error"]}


def _fmt_calls(calls: list[tuple]) -> str:
    parts = []
    for k, a, b in calls:
        if k == "wreg":
            parts.append("WriteReg(%d,0x%02X)" % (a, b))
        elif k == "send_short":
            parts.append("SEND_SHORT")
        elif k == "send_prop":
            parts.append("SEND_PROP")
        else:
            parts.append("delay(%d)" % a)
    return " -> ".join(parts)


def _expected_key_pair(key: int) -> tuple[int, int]:
    return (key >> 8) & 0xFF, key & 0xFF


def _check_short(patched: bytes, key: int, ident: int, id_off: int,
                 key_on: int, patched_mode: bool,
                 cmd: int = 0x11) -> tuple[bool, str]:
    image = patched if patched_mode else _unpatch(patched)
    res = _uc_run(image, SEND_SHORT, (cmd,), ident, id_off, key_on)
    label = "补丁版" if patched_mode else "原厂(自证)"
    if res["error"]:
        return False, "send_short ID=%d %s 执行失败: %s" % (ident, label, res["error"])
    calls = res["calls"]
    seq = _fmt_calls(calls)
    if not patched_mode:
        ok = calls == [("send_short", None, None)]
        return ok, ("send_short ID=%-2d KEY_ON=%d %s: 单发(原厂) | %s"
                    % (ident, key_on, label, seq))
    keymode = bool(key_on) or (ident == ID_KEY_MODE and id_off == 0)
    if keymode and cmd in (0x09, 0x1A, 0x21, 0x29):
        hi, lo = _expected_key_pair(key)
        sync = SYNC_NUMERATOR // (ident + 1)
        want = [("wreg", 4, hi), ("wreg", 5, lo), ("send_short", None, None),
                ("wreg", 4, (sync >> 8) & 0xFF), ("wreg", 5, sync & 0xFF),
                ("send_short", None, None)]
        ok = calls == want
        msg = ("send_short(cmd=0x%02X) ID=%-2d KEY_ON=%d %s(密钥模式/白名单): "
               "SYNC=0x%02X%02X -> SEND -> SYNC=50024/%d=0x%04X -> SEND | %s"
               % (cmd, ident, key_on, label, hi, lo, ident + 1, sync, seq))
        return ok, msg
    if keymode:
        ok = calls == [("send_short", None, None)]
        return ok, ("send_short(cmd=0x%02X) ID=%-2d KEY_ON=%d %s(密钥模式/白名单外): "
                    "保持原厂单发（不写 SYNC） | %s"
                    % (cmd, ident, key_on, label, seq))
    ok = calls == [("send_short", None, None)]
    return ok, ("send_short ID=%-2d KEY_ON=%d %s(普通模式, id_off=%d): 单发 | %s"
                % (ident, key_on, label, id_off, seq))


def _check_prop(patched: bytes, key: int, ident: int, id_off: int,
                key_on: int, patched_mode: bool) -> tuple[bool, str]:
    image = patched if patched_mode else _unpatch(patched)
    res = _uc_run(image, SEND_PROP, (0x50, 0x09, 0x00), ident, id_off, key_on)
    label = "补丁版" if patched_mode else "原厂(自证)"
    if res["error"]:
        return False, "send_property ID=%d %s 执行失败: %s" % (ident, label, res["error"])
    calls = res["calls"]
    seq = _fmt_calls(calls)
    if not patched_mode:
        ok = calls == [("send_prop", None, None)]
        return ok, ("send_property ID=%-2d KEY_ON=%d %s: 单发(原厂) | %s"
                    % (ident, key_on, label, seq))
    keymode = bool(key_on) or (ident == ID_KEY_MODE and id_off == 0)
    if keymode:
        hi, lo = _expected_key_pair(key)
        sync = SYNC_NUMERATOR // (ident + 1)
        want = [("wreg", 4, hi), ("wreg", 5, lo), ("send_prop", None, None),
                ("wreg", 4, (sync >> 8) & 0xFF), ("wreg", 5, sync & 0xFF),
                ("send_prop", None, None)]
        ok = calls == want
        msg = ("send_property ID=%-2d KEY_ON=%d %s(密钥模式): SYNC=0x%02X%02X -> SEND -> "
               "SYNC=0x%04X(0x%02X/0x%02X) -> SEND | %s"
               % (ident, key_on, label, hi, lo, sync,
                  (sync >> 8) & 0xFF, sync & 0xFF, seq))
        return ok, msg
    ok = calls == [("send_prop", None, None)]
    return ok, ("send_property ID=%-2d KEY_ON=%d %s(普通模式, id_off=%d): 单发 | %s"
                % (ident, key_on, label, id_off, seq))


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证：长度 + 静态字节 + 差异范围 + Unicorn 调用级（双发/单发）。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁+注入）"
                    % (len(patched), IMAGE_SIZE)))

    # 2) 静态：入口跳转 + 注入区
    inj_code, layout = build_injection(key)
    got = patched[_off(INJ_BASE):_off(INJ_BASE) + len(inj_code)]
    results.append((got == inj_code,
                    "注入区 0x%08X 起 %d 字节（双发包装，共 %d/%d 字节）"
                    % (INJ_BASE, len(inj_code), len(inj_code), INJ_SIZE)))
    rest = patched[_off(INJ_BASE) + len(inj_code):_off(INJ_END)]
    results.append((rest == INJ_ORIG[len(inj_code):],
                    "注入区剩余 %d 字节保持擦除态 0xFF（不越界改写）"
                    % len(rest)))
    for addr, before, after, desc in _patched_regions(key):
        if addr in (SEND_SHORT, SEND_PROP):
            got = patched[_off(addr):_off(addr) + len(after)]
            results.append((got == after, "入口 0x%08X = %s（%s）"
                            % (addr, got.hex(" "), desc)))

    # 3) 与原厂差异必须只落在补丁区（核心 + UI）
    orig = _unpatch(patched)
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    expected = _expected_diff_offsets(key)
    ui_expected: set[int] = set()
    try:
        ui = _load_ui_patch()
        if ui is not None:
            orig_img = _original_image()
            ui_img, _ = ui.apply_ui(orig_img, _ui_opts(key))
            ui_expected = {i for i in range(min(len(orig_img), len(ui_img)))
                           if ui_img[i] != orig_img[i]}
    except Exception:  # noqa: BLE001
        ui_expected = set()
    results.append((set(diffs) == (expected | ui_expected),
                    "与原厂差异 %d 字节（核心 %d + UI %d），offset=%s（应恰为并集，前 12 个）"
                    % (len(diffs), len(expected), len(ui_expected),
                       [("0x%05X" % d) for d in diffs[:12]])))

    # 4) Unicorn 调用级测试
    try:
        import unicorn  # noqa: F401
        has_uc = True
        uc_ver = getattr(unicorn, "__version__", "?")
    except Exception:  # noqa: BLE001
        has_uc = False
        uc_ver = None
    if not has_uc:
        results.append((True, "警告: unicorn 不可用，跳过 Unicorn 调用级测试"))
        return results

    results.append((True, "Unicorn %s 调用级测试：桩替换 WriteReg(0x%08X)/低层发送"
                          "(0x%08X,0x%08X)/delay(0x%08X)"
                    % (uc_ver, WRITE_REG, LOW_SEND_SHORT, LOW_SEND_PROP, DELAY)))
    hi, lo = _expected_key_pair(key)
    results.append((True, "密钥 KEY=0x%04X（SYNC1=0x%02X SYNC0=0x%02X）；密钥模式第二帧"
                          "同步字 = floor(50024/(当前ID+1)) 现算"
                    % (key, hi, lo)))
    results.append((True, "KEY_ON_FLAG = 0x%08X（ui-x3c 已验证：无字面量引用、40M 指令"
                          "模拟写追踪仅 BSS 清零；RAM 版，重启复位；后续 flash 持久化）"
                    % KEY_ON_FLAG_RAM))

    # v3 验收三组 + 边界：
    #   (ID, KEY_ON, ID_OFF) -> 期望
    #   ( 5, 1, 0) KEY_ON=1     -> 双发，第二帧 SYNC=50024/6=0x2091
    #   ( 5, 0, 0) KEY_ON=0     -> 单发
    #   (99, 0, 0) 旧开关命中   -> 双发，第二帧 SYNC=500=0x01F4
    #   (99, 1, 0) 两种开关都开 -> 双发，第二帧 0x01F4
    #   (99, 0, 1) ID-OFF 置位 -> 单发
    #   ( 5, 1, 1) 开关优先     -> 双发
    for ident, key_on, id_off in ((5, 1, 0), (5, 0, 0), (99, 0, 0),
                                  (99, 1, 0), (99, 0, 1), (5, 1, 1)):
        ok, msg = _check_short(patched, key, ident, id_off, key_on, True)
        results.append((ok, msg))
    ok, msg = _check_short(patched, key, 5, 0, 1, True, cmd=0x09)
    results.append((ok, msg))
    for hcmd in (0x1A, 0x21, 0x29):
        ok, msg = _check_short(patched, key, 5, 0, 1, True, cmd=hcmd)
        results.append((ok, msg))
    ok, msg = _check_short(patched, key, 5, 0, 1, True, cmd=0x11)
    results.append((ok, msg))
    ok, msg = _check_short(patched, key, 5, 0, 1, True, cmd=0x40)
    results.append((ok, msg))
    for ident, key_on in ((5, 1), (5, 0), (99, 0)):
        ok, msg = _check_prop(patched, key, ident, 0, key_on, True)
        results.append((ok, msg))
    # 原厂镜像自证：同一 harness 应看到"只发一次、不写 SYNC"
    for ident, key_on in ((99, 0), (5, 0)):
        ok, msg = _check_short(patched, key, ident, 0, key_on, False)
        results.append((ok, msg))
    ok, msg = _check_prop(patched, key, 99, 0, 0, False)
    results.append((ok, msg))

    # 5) UI 补丁验证（在"原厂+UI"独立镜像上验证，避免核心补丁差异影响其检查）
    try:
        ui = _load_ui_patch()
        if ui is None:
            results.append((False, "UI 补丁模块未加载（keypatch/dev/x3c_ui/ui_patch.py 不存在）"))
        else:
            orig_img = _original_image()
            ui_img, _ui_log = ui.apply_ui(orig_img, _ui_opts(key))
            for ok2, msg2 in ui.verify_ui(ui_img, _ui_opts(key)):
                results.append((ok2, "[UI] " + msg2))
            # 最终镜像里 UI 区域必须与"原厂+UI"一致
            same = all(ui_img[i] == patched[i] for i in ui_expected)
            results.append((same, "[UI] 最终镜像的 UI 区域与 ui_patch 输出一致"))
    except Exception as exc:  # noqa: BLE001
        results.append((False, "UI 补丁验证异常: %s" % exc))

    return results


# ===========================================================================
# 物理按键长按保底方案：v3p 探针版 / v3k2 正式版（参数化）
# ===========================================================================
def _apply_core_only(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """只打 core 补丁（KEY_ON/ID=99 双发），**不叠加 UI 补丁**。"""
    key = common.validate_key(key)
    if len(data) != IMAGE_SIZE:
        raise ValueError("X3-C V1.38 固件长度应为 %d 字节，实际 %d 字节（版本不符）"
                         % (IMAGE_SIZE, len(data)))
    inj_code, _layout = build_injection(key)
    got = bytes(data[_off(SEND_SHORT):_off(SEND_SHORT) + 4])
    if got != SEND_SHORT_ENTRY:
        raise ValueError("0x%08X 原始字节不匹配（固件版本不符）" % SEND_SHORT)
    got = bytes(data[_off(SEND_PROP):_off(SEND_PROP) + 4])
    if got != SEND_PROP_ENTRY:
        raise ValueError("0x%08X 原始字节不匹配（固件版本不符）" % SEND_PROP)
    got = bytes(data[_off(INJ_BASE):_off(INJ_BASE) + len(inj_code)])
    if got != b"\xff" * len(inj_code):
        raise ValueError("core 注入区 0x%08X 起不是擦除态 0xFF（固件版本不符）" % INJ_BASE)
    buf = bytearray(data)
    log: list[dict] = []
    for addr, before, after, desc in _patched_regions(key):
        o = _off(addr)
        got = bytes(buf[o:o + len(before)])
        if got != before:
            raise ValueError("补丁点 0x%08X 原始字节不匹配（固件版本不符）" % addr)
        buf[o:o + len(after)] = after
        log.append({
            "addr": addr, "offset": o,
            "before": (before.hex(" ") if len(before) <= 8 else
                       before[:4].hex(" ") + " …(%d B)" % len(before)),
            "after": (after.hex(" ") if len(after) <= 8 else
                      after[:4].hex(" ") + " …(%d B)" % len(after)),
            "desc": desc,
        })
    patched = bytes(buf)
    assert len(patched) == len(data), "原位补丁改变了长度"
    return patched, log


def _emit_addr(a: _Asm, rd: int, addr: int) -> None:
    a.movw(rd, addr & 0xFFFF)
    a.movt(rd, (addr >> 16) & 0xFFFF)


def _emit_read_key(a: _Asm, port: int, mask: int) -> None:
    """r0 = FUN_08022EB4(port, mask)（0 = 按下）。"""
    _emit_addr(a, 0, port)
    a.movw(1, mask & 0xFFFF)
    a.bl(READ_KEY)


def _btn_key_bit(port: int, mask: int):
    for bit, (p, m) in enumerate(((BTN0_PORT, BTN0_MASK),
                                  (BTN1_PORT, BTN1_MASK),
                                  (BTN2_PORT, BTN2_MASK))):
        if port == p and mask == m:
            return bit
    return None


def _btn_cfg(mode: str) -> dict:
    """返回某模式的按键绑定/阈值/需禁用的原厂长按位。"""
    extra_disable = set()
    if mode == "keyprobe":
        buttons = [
            ("b0", BTN0_PORT, BTN0_MASK, "probe1", BTN_RAM0_CNT, BTN_RAM0_FIRED),
            ("b1", BTN1_PORT, BTN1_MASK, "probe2", BTN_RAM1_CNT, BTN_RAM1_FIRED),
            ("b2", BTN2_PORT, BTN2_MASK, "probe3", BTN_RAM2_CNT, BTN_RAM2_FIRED),
        ]
        actions = ["probe1", "probe2", "probe3"]
        threshold = BTN_PROBE_TICKS
    elif mode == "keybtn4":
        # 最终两段式：拨轮/M/C(bit0) 长按 30 tick 切密钥、400 tick 关机；
        # ⚡(bit1) 长按广播；bit2(PB15) 不绑定、保持原厂。
        buttons = [
            ("wheel", BTN0_PORT, BTN0_MASK, "toggle",
             BTN_RAM_WHEEL_CNT16, BTN_RAM_WHEEL_FIRED1),
            ("trig", BTN1_PORT, BTN1_MASK, "broadcast",
             BTN_RAM1_CNT, BTN_RAM1_FIRED),
        ]
        actions = ["toggle", "broadcast", "shutdown"]
        threshold = BTN_KEYBTN4_TICKS1
    elif mode in ("keybtn7", "keybtn8"):
        # keybtn5 行为（M/C 两段式 + ⚡ 原厂）+ 双击 ⚡ 全 ID 广播；无长按广播。
        buttons = [
            ("wheel", BTN0_PORT, BTN0_MASK, "toggle",
             BTN_RAM_WHEEL_CNT16, BTN_RAM_WHEEL_FIRED1),
        ]
        actions = ["toggle", "shutdown", "start_broadcast"]
        threshold = BTN_KEYBTN4_TICKS1
        extra_disable = set()
    elif mode == "keybtn12":
        # 诊断：keybtn11 的轻量广播（32 频道 × 2 帧 = 64 帧），用来判断关机是否与发射量有关
        buttons = [
            ("wheel", BTN0_PORT, BTN0_MASK, "toggle",
             BTN_RAM5_CNT16, BTN_RAM5_STREAK),
        ]
        actions = ["toggle", "shutdown", "start_broadcast"]
        threshold = BTN_KEYBTN5_TICKS1
        extra_disable = set()
    elif mode == "keybtn11":
        # keybtn5 行为 + 双击 ⚡(PA8) 全 ID 广播（M/C 双击是原厂关机，不可用）
        buttons = [
            ("wheel", BTN0_PORT, BTN0_MASK, "toggle",
             BTN_RAM5_CNT16, BTN_RAM5_STREAK),
        ]
        actions = ["toggle", "shutdown", "start_broadcast"]
        threshold = BTN_KEYBTN5_TICKS1
        extra_disable = set()
    elif mode == "keybtn5":
        # 单计数器健壮版：无广播；⚡(bit1) 完全不动（原厂长按 0x28 恢复）。
        buttons = [
            ("wheel", BTN0_PORT, BTN0_MASK, "toggle",
             BTN_RAM5_CNT16, BTN_RAM5_STREAK),
        ]
        actions = ["toggle", "shutdown"]
        threshold = BTN_KEYBTN5_TICKS1
        extra_disable = set()          # bit1/bit2 都保持原厂
    elif mode == "keybtn4_k1":
        # 诊断 K1：与 keybtn4 完全相同，仅把 stage2(关机) 阈值抬到 0xFFFF ≈ 永不触发。
        buttons = [
            ("wheel", BTN0_PORT, BTN0_MASK, "toggle",
             BTN_RAM_WHEEL_CNT16, BTN_RAM_WHEEL_FIRED1),
            ("trig", BTN1_PORT, BTN1_MASK, "broadcast",
             BTN_RAM1_CNT, BTN_RAM1_FIRED),
        ]
        actions = ["toggle", "broadcast", "shutdown"]
        threshold = BTN_KEYBTN4_TICKS1
    elif mode in ("keybtn4_nobc_a", "keybtn4_nobc_b"):
        # 无广播变体：只有 bit0(拨轮/M/C) 两段式（1.5s 切密钥 / 20s 关机）。
        # A: ⚡(bit1) 原厂长按阈值恢复 0x28（原厂关机）；不绑定广播。
        # B: ⚡(bit1) 原厂长按禁用（0xFF）且不绑定任何动作。
        buttons = [
            ("wheel", BTN0_PORT, BTN0_MASK, "toggle",
             BTN_RAM_WHEEL_CNT16, BTN_RAM_WHEEL_FIRED1),
        ]
        actions = ["toggle", "shutdown"]
        threshold = BTN_KEYBTN4_TICKS1
        if mode == "keybtn4_nobc_b":
            extra_disable = {1}
    elif mode == "keybtn3":
        # 用户实机探针最终确认：拨轮/M/C = bit0(PA9)，⚡ = bit1(PA8)，
        # bit2(PB15) 不是按键（编码器旋转相关），完全不绑定、保持原厂。
        buttons = [
            ("wheel", BTN0_PORT, BTN0_MASK, "toggle",
             BTN_RAM0_CNT, BTN_RAM0_FIRED),
            ("trig", BTN1_PORT, BTN1_MASK, "broadcast",
             BTN_RAM1_CNT, BTN_RAM1_FIRED),
        ]
        actions = ["toggle", "broadcast"]
        threshold = BTN_KEYBTN2_TICKS
    elif mode == "keybtn2":
        buttons = [
            ("wheel", BTN_WHEEL_PORT, BTN_WHEEL_MASK, "toggle",
             BTN_RAM0_CNT, BTN_RAM0_FIRED),
            ("trig", BTN_TRIG_PORT, BTN_TRIG_MASK, "broadcast",
             BTN_RAM2_CNT, BTN_RAM2_FIRED),
        ]
        actions = ["toggle", "broadcast"]
        threshold = BTN_KEYBTN2_TICKS
    else:
        raise ValueError("未知按键模式 %r（keyprobe/keybtn2/keybtn3/keybtn4）" % mode)
    disable_bits = set(extra_disable)
    if BTN_DISABLE_ORIGINAL_LONG:
        for _n, port, mask, _act, _c, _f in buttons:
            bit = _btn_key_bit(port, mask)
            if bit in (1, 2):
                disable_bits.add(bit)
    return {"mode": mode, "buttons": buttons, "actions": actions,
            "threshold": threshold, "disable_bits": disable_bits,
            "two_stage": mode in ("keybtn4", "keybtn4_nobc_a",
                                  "keybtn4_nobc_b", "keybtn4_k1"),
            "threshold2": (0xFFFF if mode == "keybtn4_k1"
                           else (BTN_KEYBTN5_TICKS2 if mode in ("keybtn5", "keybtn11")
                                 else BTN_KEYBTN4_TICKS2)),
            "stage2_disabled": mode == "keybtn4_k1",
            "robust": mode == "keybtn5",
            "watchdog": BTN_KEYBTN5_WATCHDOG,
            "broadcast_allid": mode in ("keybtn7", "keybtn8", "keybtn11", "keybtn12"),
            "broadcast_lite": mode in ("keybtn8", "keybtn12"),
            "pa8_double": mode in ("keybtn11", "keybtn12")}


def _emit_button_watch(a: _Asm, prefix: str, port: int, mask: int,
                       cnt_ram: int, fired_ram: int, action_label: str,
                       threshold: int) -> None:
    """一个键的"长按一次性触发"逻辑（调用前需已 push {r4,r5,r6,lr}）。"""
    _emit_read_key(a, port, mask)
    a.cmp_imm(0, 0)
    a.bne("%s_release" % prefix)
    _emit_addr(a, 4, fired_ram)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0)
    a.bne("%s_done" % prefix)
    _emit_addr(a, 4, cnt_ram)
    a.ldrb(5, 4)
    a.adds_imm(5, 5, 1)
    a.strb_imm(5, 4, 0)
    a.cmp_imm(5, threshold)
    a.bcc("%s_done" % prefix)
    a.movs(5, 1)
    _emit_addr(a, 4, fired_ram)
    a.strb_imm(5, 4, 0)
    a.bl(action_label)
    a.b_w("%s_done" % prefix)
    a.label("%s_release" % prefix)
    _emit_addr(a, 4, cnt_ram)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, fired_ram)
    a.strb_imm(5, 4, 0)
    a.label("%s_done" % prefix)


def _emit_pa9_gesture_watch(a: _Asm) -> None:
    """bit0/PA9 三手势：
    - 短按：本函数不动（原厂短按逻辑照常）
    - 双击（两次短按 <300ms、按下沿间隔 <400ms）：start_broadcast
    - 长按 30 tick：切 KEY_ON（并取消双击判定）
    - 继续按住 400 tick：关机；>600 tick 看门狗清零
    计数用 u16（0x20001096），去抖/按键锁沿用 keybtn5 风格。
    """
    _emit_addr(a, 6, TICK_RAM)          # r6 = &tick
    # MODER 输入模式校验（读不到/全 1 时跳过）
    _emit_addr(a, 4, GPIOA_BASE)
    a.ldr(0, 4, 0)
    a.movw(1, 0xFFFF)
    a.movt(1, 0xFFFF)
    a.cmp_reg(0, 1)
    a.beq_far("p9_moder_ok")
    a.lsrs_imm(0, 0, 18)
    a.movs(1, 3)
    a.ands(0, 1)
    a.cmp_imm(0, 0)
    a.bne_far("p9_release")
    a.label("p9_moder_ok")

    _emit_read_key(a, BTN0_PORT, BTN0_MASK)
    _emit_addr(a, 4, BTN7_PREV)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0)
    a.beq_far("p9_was_up")

    # ---- 上次按下 ----
    a.cmp_imm(0, 0)
    a.beq_far("p9_held")               # 仍按住：去抖+长按计数
    # 松开沿
    _emit_addr(a, 4, BTN7_PREV)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN7_PRESS_START)
    a.ldr(5, 4, 0)
    a.ldr(0, 6, 0)
    a.subs_reg(0, 0, 5)                # dur
    a.movw(1, BTN7_CLICK_MS)
    a.cmp_reg(0, 1)
    a.bcs_far("p9_long")
    # 短按：双击判定
    _emit_addr(a, 4, BTN7_PENDING)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0)
    a.beq_far("p9_first")
    _emit_addr(a, 4, BTN7_FIRST_PRESS)
    a.ldr(5, 4, 0)
    _emit_addr(a, 4, BTN7_PRESS_START)
    a.ldr(0, 4, 0)
    a.subs_reg(0, 0, 5)
    a.movw(1, BTN7_DOUBLE_MS)
    a.cmp_reg(0, 1)
    a.bcs_far("p9_first")
    _emit_addr(a, 4, BTN7_PENDING)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    # 清长按计数，再广播
    _emit_addr(a, 4, BTN_RAM_WHEEL_CNT16)
    a.movs(5, 0)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM_WHEEL_FIRED1)
    a.strb_imm(5, 4, 0)
    a.bl("start_broadcast")
    a.b_w("p9_done")
    a.label("p9_first")
    _emit_addr(a, 4, BTN7_PENDING)
    a.movs(5, 1)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN7_PRESS_START)
    a.ldr(5, 4, 0)
    _emit_addr(a, 4, BTN7_FIRST_PRESS)
    a.str(5, 4, 0)
    # 短按结束后长按计数清零
    _emit_addr(a, 4, BTN_RAM_WHEEL_CNT16)
    a.movs(5, 0)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM_WHEEL_FIRED1)
    a.strb_imm(5, 4, 0)
    a.b_w("p9_done")
    a.label("p9_long")
    _emit_addr(a, 4, BTN7_PENDING)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM_WHEEL_CNT16)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM_WHEEL_FIRED1)
    a.strb_imm(5, 4, 0)
    a.b_w("p9_done")

    # ---- 上次松开：按下沿 ----
    a.label("p9_was_up")
    a.cmp_imm(0, 0)
    a.bne_far("p9_done")               # 松开常态：结束
    _emit_addr(a, 4, BTN7_PREV)
    a.movs(5, 1)
    a.strb_imm(5, 4, 0)
    a.ldr(5, 6, 0)
    _emit_addr(a, 4, BTN7_PRESS_START)
    a.str(5, 4, 0)
    a.b_w("p9_done")

    # ---- 按住中：去抖 + 长按计数/阶段 ----
    a.label("p9_held")
    _emit_addr(a, 4, BTN7_PREV)
    a.movs(5, 1)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM5_STREAK)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0xFF)
    a.bcs("p9_sk")
    a.adds_imm(5, 5, 1)
    a.strb_imm(5, 4, 0)
    a.label("p9_sk")
    a.cmp_imm(5, 3)
    a.bcc("p9_done")
    _emit_addr(a, 4, BTN_RAM_WHEEL_CNT16)
    a.ldrh(6, 4, 0)
    a.adds_imm(6, 6, 1)
    a.strh(6, 4, 0)
    a.cmp_imm(6, 30)
    a.bne("p9_s1")
    _emit_addr(a, 4, BTN7_PENDING)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    a.bl("toggle")
    a.label("p9_s1")
    a.movw(5, 400)
    a.cmp_reg(6, 5)
    a.bne("p9_s2")
    a.bl("shutdown")
    a.label("p9_s2")
    a.movw(5, 600)
    a.cmp_reg(6, 5)
    a.bcc("p9_done")
    _emit_addr(a, 4, BTN_RAM_WHEEL_CNT16)
    a.movs(5, 0)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM5_STREAK)
    a.strb_imm(5, 4, 0)
    a.b_w("p9_done")

    # ---- 未按下（松开常态）----
    a.label("p9_release")
    _emit_addr(a, 4, BTN7_PREV)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM_WHEEL_CNT16)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM5_STREAK)
    a.strb_imm(5, 4, 0)
    a.label("p9_done")


def _emit_double_click_watch(a: _Asm, port: int, mask: int,
                             action_label: str) -> None:
    """PA8 双击：两次短按(<300ms)、按下沿间隔 <400ms -> action 一次。"""
    P = "dc"
    _emit_addr(a, 6, TICK_RAM)          # r6 = &tick
    _emit_read_key(a, port, mask)       # r0: 0=按下
    _emit_addr(a, 4, BTN7_PREV)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0)
    a.beq_far("%s_was_up" % P)

    # 上一次按下：仍按住 -> 结束；刚松开 -> 判断短按/双击
    a.cmp_imm(0, 0)
    a.beq_far("%s_done" % P)
    _emit_addr(a, 4, BTN7_PREV)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN7_PRESS_START)
    a.ldr(5, 4, 0)
    a.ldr(0, 6, 0)
    a.subs_reg(0, 0, 5)                # dur
    a.movw(1, BTN7_CLICK_MS)
    a.cmp_reg(0, 1)
    a.bcs_far("%s_long" % P)
    _emit_addr(a, 4, BTN7_PENDING)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0)
    a.beq_far("%s_first" % P)
    _emit_addr(a, 4, BTN7_FIRST_PRESS)
    a.ldr(5, 4, 0)
    _emit_addr(a, 4, BTN7_PRESS_START)
    a.ldr(0, 4, 0)
    a.subs_reg(0, 0, 5)
    a.movw(1, BTN7_DOUBLE_MS)
    a.cmp_reg(0, 1)
    a.bcs_far("%s_first" % P)              # 间隔 >=400ms：重新当第一击
    _emit_addr(a, 4, BTN7_PENDING)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    a.bl(action_label)
    a.b_w("%s_done" % P)
    a.label("%s_first" % P)
    _emit_addr(a, 4, BTN7_PENDING)
    a.movs(5, 1)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN7_PRESS_START)
    a.ldr(5, 4, 0)
    _emit_addr(a, 4, BTN7_FIRST_PRESS)
    a.str(5, 4, 0)
    a.b_w("%s_done" % P)
    a.label("%s_long" % P)
    _emit_addr(a, 4, BTN7_PENDING)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    a.b_w("%s_done" % P)
    a.label("%s_was_up" % P)
    a.cmp_imm(0, 0)
    a.bne_far("%s_done" % P)
    _emit_addr(a, 4, BTN7_PREV)
    a.movs(5, 1)
    a.strb_imm(5, 4, 0)
    a.ldr(5, 6, 0)
    _emit_addr(a, 4, BTN7_PRESS_START)
    a.str(5, 4, 0)
    a.label("%s_done" % P)


def _emit_start_broadcast(a: _Asm) -> None:
    """置 active、保存 KEY_ON、临时清 0、游标归零；已在广播中则忽略。"""
    a.label("start_broadcast")
    _emit_addr(a, 4, BTN7_ACTIVE)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0)
    a.bne("sb_done")
    _emit_addr(a, 4, KEY_ON_FLAG_RAM)
    a.ldrb(5, 4)
    _emit_addr(a, 4, BTN7_SAVED_KEYON)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, KEY_ON_FLAG_RAM)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN7_ACTIVE)
    a.movs(5, 1)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN7_CH)
    a.movs(5, 0)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, BTN7_ID)
    a.strb_imm(5, 4, 0)
    a.bl(IWDG_FEED)
    a.label("sb_done")
    a.bx_lr()


def _emit_bc_run(a: _Asm, key: int, lite: bool = False) -> None:
    """广播分块执行器（节流 + 喂狗）。

    full（keybtn7）：32ch × id 0..99 公共同步字 + 每频道开始先发一帧私钥帧；
    lite（keybtn8）：32ch × (私钥帧 + 当前 ID 普通帧)，共 64 帧。
    """
    a.label("bc_run")
    _emit_addr(a, 4, BTN7_ACTIVE)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0)
    a.beq("bc_end")
    # 节流：两批之间至少间隔 BTN7_CHUNK_MS（降低射频平均占空比）
    _emit_addr(a, 0, TICK_RAM)
    a.ldr(0, 0, 0)
    _emit_addr(a, 1, BTN7_LAST_TICK)
    a.ldr(2, 1, 0)
    a.subs_reg(2, 0, 2)
    a.movw(1, BTN7_CHUNK_MS)
    a.cmp_reg(2, 1)
    a.bcc("bc_end")
    _emit_addr(a, 1, BTN7_LAST_TICK)
    a.str(0, 1, 0)
    a.push(0x170, lr=True)             # r4,r5,r6
    a.bl(IWDG_FEED)
    a.movs(6, BTN7_BUDGET)
    a.label("bc_loop")
    if lite:
        _emit_addr(a, 4, BTN7_CH)
        a.ldrb(0, 4)
        a.bl(SET_CHANNEL)
        _emit_bc_key_frame(a, key)
        _emit_addr(a, 4, ID_RAM)
        a.ldrb(0, 4)
        a.bl(SET_SYNC)
        a.movs(0, 0x09)
        a.bl(SEND_SHORT)
        _emit_addr(a, 4, BTN7_CH)
        a.ldrb(1, 4)
        a.adds_imm(1, 1, 1)
        a.strb_imm(1, 4, 0)
        a.cmp_imm(1, 32)
        a.bne("bc_next")
    else:
        _emit_addr(a, 4, BTN7_ID)
        a.ldrb(1, 4)
        a.cmp_imm(1, 0)
        a.bne("bc_no_ch")
        _emit_addr(a, 4, BTN7_CH)
        a.ldrb(0, 4)
        a.bl(SET_CHANNEL)
        _emit_bc_key_frame(a, key)
        a.label("bc_no_ch")
        _emit_addr(a, 4, BTN7_ID)
        a.ldrb(0, 4)
        a.bl(SET_SYNC)
        a.movs(0, 0x09)
        a.bl(SEND_SHORT)
        _emit_addr(a, 4, BTN7_ID)
        a.ldrb(1, 4)
        a.adds_imm(1, 1, 1)
        a.strb_imm(1, 4, 0)
        a.cmp_imm(1, 100)
        a.bne("bc_next")
        _emit_addr(a, 4, BTN7_ID)
        a.movs(1, 0)
        a.strb_imm(1, 4, 0)
        _emit_addr(a, 4, BTN7_CH)
        a.ldrb(1, 4)
        a.adds_imm(1, 1, 1)
        a.strb_imm(1, 4, 0)
        a.cmp_imm(1, 32)
        a.bne("bc_next")
    _emit_addr(a, 4, BTN7_ACTIVE)
    a.movs(1, 0)
    a.strb_imm(1, 4, 0)
    _emit_addr(a, 4, BTN7_SAVED_KEYON)
    a.ldrb(2, 4)
    _emit_addr(a, 4, KEY_ON_FLAG_RAM)
    a.strb_imm(2, 4, 0)
    a.b_w("bc_pop")
    a.label("bc_next")
    a.subs_imm(6, 6, 1)
    a.bne("bc_loop")
    a.label("bc_pop")
    a.pop(0x170, pc=True)
    a.label("bc_end")
    a.bx_lr()


def _emit_bc_key_frame(a: _Asm, key: int) -> None:
    """写私钥同步字并发一帧 TEST（KEY_ON 已被清 0 -> 单发）。"""
    a.movw(5, key & 0xFFFF)
    a.movs(0, 4)
    a.lsrs_imm(1, 5, 8)
    a.bl(WRITE_REG)
    a.movs(0, 5)
    a.uxtb(1, 5)
    a.bl(WRITE_REG)
    a.movs(0, 0x09)
    a.bl(SEND_SHORT)


def _emit_robust_watch(a: _Asm, prefix: str, port: int, mask: int,
                       moder_base: int, moder_shift: int,
                       cnt16_ram: int, streak_ram: int,
                       th1: int, th2: int, watchdog: int) -> None:
    """单计数器 + 去抖 + 输入模式校验 + 看门狗。

    * 先校验 GPIO MODER 对应位 == 00（输入），否则按"未按下"处理（防待机重配引脚误判）；
    * 连续 N 次读为按下才开始计数（去抖）；
    * counter == th1 的沿切 KEY_ON；counter == th2 的沿关机一次（无 latch 标志）；
    * 松开/非输入模式：counter=0、streak=0；
    * counter > watchdog 强制清零（异常一直按下时防卡死）。
    """
    # 1) MODER 输入模式校验（0xFFFFFFFF=未建模/读不到时跳过，避免误判）
    _emit_addr(a, 4, moder_base)
    a.ldr(0, 4, 0)
    a.movw(1, 0xFFFF)
    a.movt(1, 0xFFFF)
    a.cmp_reg(0, 1)
    a.beq("%s_moder_ok" % prefix)
    a.lsrs_imm(0, 0, moder_shift)
    a.movs(1, 3)
    a.ands(0, 1)
    a.cmp_imm(0, 0)
    a.bne("%s_release" % prefix)
    a.label("%s_moder_ok" % prefix)
    # 2) 读键（0 = 按下）
    _emit_read_key(a, port, mask)
    a.cmp_imm(0, 0)
    a.bne("%s_release" % prefix)
    # 3) 去抖 streak
    _emit_addr(a, 4, streak_ram)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0xFF)
    a.bcs("%s_streak_done" % prefix)
    a.adds_imm(5, 5, 1)
    a.strb_imm(5, 4, 0)
    a.label("%s_streak_done" % prefix)
    a.cmp_imm(5, BTN_KEYBTN5_DEBOUNCE)
    a.bcc("%s_done" % prefix)
    # 4) counter++
    _emit_addr(a, 4, cnt16_ram)
    a.ldrh(6, 4, 0)
    a.adds_imm(6, 6, 1)
    a.strh(6, 4, 0)
    # 5) stage1 沿：counter == th1 -> 切密钥
    a.cmp_imm(6, th1)
    a.bne("%s_s1" % prefix)
    a.bl("toggle")
    a.label("%s_s1" % prefix)
    # 6) stage2 沿：counter == th2 -> 关机一次
    a.movw(5, th2 & 0xFFFF)
    a.cmp_reg(6, 5)
    a.bne("%s_s2" % prefix)
    a.bl("shutdown")
    a.label("%s_s2" % prefix)
    # 7) 看门狗
    a.movw(5, watchdog & 0xFFFF)
    a.cmp_reg(6, 5)
    a.bcc("%s_done" % prefix)
    _emit_addr(a, 4, cnt16_ram)
    a.movs(5, 0)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, streak_ram)
    a.strb_imm(5, 4, 0)
    a.b_w("%s_done" % prefix)
    # 8) 释放/非输入：清零
    a.label("%s_release" % prefix)
    _emit_addr(a, 4, cnt16_ram)
    a.movs(5, 0)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, streak_ram)
    a.strb_imm(5, 4, 0)
    a.label("%s_done" % prefix)


def _emit_two_stage_watch(a: _Asm, prefix: str, port: int, mask: int,
                          cnt16_ram: int, fired1_ram: int, fired2_ram: int,
                          action1: str, action2: str,
                          th1: int, th2: int) -> None:
    """两段式长按：th1 触发一次 action1，继续到 th2 触发一次 action2，松开清零。

    计数用 u16（20s 阈值 400 超过 u8 范围）。调用前需已 push {r4,r5,r6,lr}。
    """
    _emit_read_key(a, port, mask)
    a.cmp_imm(0, 0)
    a.bne("%s_release" % prefix)
    # u16 计数 +1
    _emit_addr(a, 4, cnt16_ram)
    a.ldrh(5, 4, 0)
    a.adds_imm(5, 5, 1)
    a.strh(5, 4, 0)
    # 第一段：th1 次 -> action1 一次
    _emit_addr(a, 4, fired1_ram)
    a.ldrb(6, 4)
    a.cmp_imm(6, 0)
    a.bne("%s_skip1" % prefix)
    a.cmp_imm(5, th1)
    a.bcc("%s_skip1" % prefix)
    a.movs(6, 1)
    a.strb_imm(6, 4, 0)
    a.bl(action1)
    a.label("%s_skip1" % prefix)
    # 第二段：th2 次 -> action2 一次
    _emit_addr(a, 4, fired2_ram)
    a.ldrb(6, 4)
    a.cmp_imm(6, 0)
    a.bne("%s_done" % prefix)
    a.movw(6, th2 & 0xFFFF)
    a.cmp_reg(5, 6)
    a.bcc("%s_done" % prefix)
    a.movs(6, 1)
    a.strb_imm(6, 4, 0)
    a.bl(action2)
    a.b_w("%s_done" % prefix)
    a.label("%s_release" % prefix)
    _emit_addr(a, 4, cnt16_ram)
    a.movs(5, 0)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, fired1_ram)
    a.strb_imm(5, 4, 0)
    _emit_addr(a, 4, fired2_ram)
    a.strb_imm(5, 4, 0)
    a.label("%s_done" % prefix)


def _emit_shutdown(a: _Asm) -> None:
    """原厂关机：显示收尾 + 原厂长按关机流程（内部调 0x08010C00 关机本体）。"""
    a.label("shutdown")
    a.push(0x10, lr=True)              # push {r4, lr}
    a.bl(TEARDOWN_DISPLAY)
    a.bl(SHUTDOWN_WRAPPER)
    a.pop(0x10, pc=True)               # pop {r4, pc}


def _emit_toggle(a: _Asm) -> None:
    """KEY_ON ^= 1（只用 caller-saved r0/r1/r2）。"""
    a.label("toggle")
    _emit_addr(a, 0, KEY_ON_FLAG_RAM)
    a.ldrb(1, 0)
    a.movs(2, 1)
    a.eors(1, 2)
    a.strb_imm(1, 0, 0)
    a.bx_lr()


def _emit_probe(a: _Asm, n_frames: int) -> None:
    """探针动作：写当前 ID 的公共同步字，然后低层发 n_frames 个 TEST 帧。

    刻意绕过 core 的 send_short 包装（0x080118B0 已被改成双发包装），
    直接调用低层 2 字节发送 0x0801712C，保证正好 N 帧、且同步字与 KEY_ON 无关。
    """
    tag = "probe%d" % n_frames
    a.label(tag)
    a.push(0x170, lr=True)
    # SYNC = floor(50024/(当前ID+1))，写 reg4/reg5
    _emit_ordinary_sync(a, hi=True)
    _emit_ordinary_sync(a, hi=False)
    # TX 缓冲 [D5, 09]
    _emit_addr(a, 6, TX_BUF_RAM)
    a.movs(0, 0xD5)
    a.strb_imm(0, 6, 0)
    a.movs(0, BTN_TEST_CMD)
    a.strb_imm(0, 6, 1)
    a.movs(4, n_frames & 0xFF)
    a.label("%s_loop" % tag)
    a.bl(LOW_SEND_SHORT)
    a.subs_imm(4, 4, 1)
    a.bne("%s_loop" % tag)
    a.pop(0x170, pc=True)


def _emit_broadcast(a: _Asm, key: int) -> None:
    """32 频道广播引闪（走原厂高层发送，KEY_ON=1 时 core 自动双发）。"""
    a.label("broadcast")
    a.push(0x170, lr=True)
    a.movs(4, 0)
    a.movw(5, key & 0xFFFF)
    _emit_addr(a, 6, GROUP_TABLE_RAM)
    a.label("bc_loop")
    a.mov(0, 4)
    a.bl(SET_CHANNEL)
    a.movs(0, 4)
    a.lsrs_imm(1, 5, 8)
    a.bl(WRITE_REG)
    a.movs(0, 5)
    a.uxtb(1, 5)
    a.bl(WRITE_REG)
    a.ldrb(2, 6, 4)
    a.movs(3, 0x7F)
    a.ands(2, 3)
    a.movs(0, BTN_PROP_GROUP)
    a.movs(1, BTN_PROP_POWER)
    a.bl(SEND_PROP)
    a.ldrb(2, 6, 0)
    a.cmp_imm(2, 0)
    a.beq("bc_lamp_on")
    a.movs(2, 0)
    a.b_w("bc_lamp_send")
    a.label("bc_lamp_on")
    a.movs(2, 1)
    a.label("bc_lamp_send")
    a.movs(0, BTN_PROP_GROUP)
    a.movs(1, BTN_PROP_LAMP)
    a.bl(SEND_PROP)
    a.movs(0, BTN_TEST_CMD)
    a.bl(SEND_SHORT)
    a.adds_imm(4, 4, 1)
    a.cmp_imm(4, 32)
    a.bcc("bc_loop")
    _emit_addr(a, 0, CHANNEL_RAM)
    a.ldrb(0, 0)
    a.subs_imm(0, 0, 1)
    a.bl(SET_CHANNEL)
    _emit_addr(a, 0, ID_RAM)
    a.ldrb(0, 0)
    a.bl(SET_SYNC)
    a.pop(0x170, pc=True)


def build_btn_injection(key: int, mode: str = "keybtn2") -> tuple[bytes, dict]:
    """生成按键注入代码（BTN_INJ_BASE 起）与布局信息。"""
    cfg = _btn_cfg(mode)
    a = _Asm(BTN_INJ_BASE)
    a.emit(KEYSCAN_ENTRY_BEFORE)
    a.bl("btn_update")
    a.b_w(KEYSCAN_RESUME)

    a.label("btn_update")
    a.push(0x170, lr=True)
    # 注：不再根据 state[5] 跳过（实机反馈该"按键锁"字节会置位导致手势永不触发）
    if cfg.get("broadcast_allid"):
        if cfg.get("pa8_double"):
            _emit_addr(a, 4, BTN7_ACTIVE)
            a.ldrb(5, 4)
            a.cmp_imm(5, 0)
            a.bne_far("k7_bc_only")
            w = cfg["buttons"][0]
            _emit_robust_watch(a, w[0], w[1], w[2], GPIOA_BASE, 18,
                               w[4], w[5], cfg["threshold"],
                               cfg["threshold2"], cfg["watchdog"])
            wd = cfg.get("wheel_debounce", BTN_KEYBTN5_DEBOUNCE)
            _emit_double_click_watch(a, BTN1_PORT, BTN1_MASK,
                                     "start_broadcast")
        else:
            _emit_addr(a, 4, BTN7_ACTIVE)
            a.ldrb(5, 4)
            a.cmp_imm(5, 0)
            a.bne_far("k7_bc_only")
            _emit_pa9_gesture_watch(a)
        a.label("k7_bc_only")
        a.bl("bc_run")
    elif cfg.get("robust"):
        w = cfg["buttons"][0]
        _emit_robust_watch(a, w[0], w[1], w[2], GPIOA_BASE, 18,
                           w[4], w[5], cfg["threshold"], cfg["threshold2"],
                           cfg["watchdog"])
    elif cfg.get("two_stage"):
        w = cfg["buttons"][0]
        _emit_two_stage_watch(a, w[0], w[1], w[2], w[4], w[5],
                              BTN_RAM_WHEEL_FIRED2, "toggle", "shutdown",
                              cfg["threshold"], cfg["threshold2"])
        for prefix, port, mask, action, cnt, fired in cfg["buttons"][1:]:
            _emit_button_watch(a, prefix, port, mask, cnt, fired, action,
                               cfg["threshold"])
    else:
        for prefix, port, mask, action, cnt, fired in cfg["buttons"]:
            _emit_button_watch(a, prefix, port, mask, cnt, fired, action,
                               cfg["threshold"])
    a.pop(0x170, pc=True)

    for action in cfg["actions"]:
        if action == "toggle":
            _emit_toggle(a)
        elif action == "broadcast":
            _emit_broadcast(a, key)
        elif action == "shutdown":
            _emit_shutdown(a)
        elif action == "start_broadcast":
            _emit_start_broadcast(a)
        elif action.startswith("probe"):
            _emit_probe(a, int(action[-1]))

    if cfg.get("broadcast_allid"):
        _emit_bc_run(a, key, cfg.get("broadcast_lite", False))
    code = a.build()
    if len(code) > BTN_INJ_SIZE:
        raise ValueError("按键注入代码 %d 字节超出预留区 %d 字节"
                         % (len(code), BTN_INJ_SIZE))
    layout = {
        "mode": mode,
        "entry": BTN_INJ_BASE,
        "btn_update": a.labels["btn_update"],
        "size": len(code),
    }
    for action in cfg["actions"]:
        layout[action] = a.labels.get(action)
    return code, layout


def _btn_patched_regions(key: int, mode: str = "keybtn2") -> list:
    cfg = _btn_cfg(mode)
    inj_code, _layout = build_btn_injection(key, mode)
    b = _Asm(KEYSCAN_FN)
    b.b_w(BTN_INJ_BASE)
    regions = [(KEYSCAN_FN, KEYSCAN_ENTRY_BEFORE, b.build(),
                "按键扫描入口: B.W -> 按键手势注入（每轮扫描检测长按）")]
    if 1 in cfg["disable_bits"]:
        regions.append((KEYSCAN_LONG1_CMP, KEYSCAN_CMP_BEFORE,
                        KEYSCAN_CMP_DISABLED,
                        "bit1 原厂长按阈值 cmp #0x28 -> #0xFF（禁原厂长按，改由注入处理）"))
    if 2 in cfg["disable_bits"]:
        regions.append((KEYSCAN_LONG2_CMP, KEYSCAN_CMP2_BEFORE,
                        KEYSCAN_CMP_DISABLED,
                        "bit2 原厂长按阈值 cmp #0x1E -> #0xFF（禁原厂长按，改由注入处理）"))
    regions.append((BTN_INJ_BASE, BTN_INJ_ORIG[:len(inj_code)], inj_code,
                    "按键手势注入（%s）：%s" % (mode, " / ".join(cfg["actions"]))))
    return regions


def apply_keys(data: bytes, key: int, mode: str = "keybtn2") -> tuple[bytes, list[dict]]:
    """core 补丁 + 物理按键注入（不叠加 UI 补丁），长度不变。"""
    key = common.validate_key(key)
    _btn_cfg(mode)
    core, log = _apply_core_only(data, key)
    buf = bytearray(core)
    inj_code, _layout = build_btn_injection(key, mode)
    for addr, before, after, desc in _btn_patched_regions(key, mode):
        o = _off(addr)
        got = bytes(buf[o:o + len(before)])
        if got != before:
            raise ValueError("按键补丁点 0x%08X 原始字节不匹配：期望 %s 实际 %s"
                             % (addr, before.hex(" "), got.hex(" ")))
    got = bytes(buf[_off(BTN_INJ_BASE):_off(BTN_INJ_BASE) + len(inj_code)])
    if got != b"\xff" * len(inj_code):
        raise ValueError("按键注入区 0x%08X 起不是擦除态 0xFF" % BTN_INJ_BASE)
    for addr, before, after, desc in _btn_patched_regions(key, mode):
        o = _off(addr)
        buf[o:o + len(after)] = after
        log.append({
            "addr": addr, "offset": o,
            "before": (before.hex(" ") if len(before) <= 8 else
                       before[:4].hex(" ") + " …(%d B)" % len(before)),
            "after": (after.hex(" ") if len(after) <= 8 else
                      after[:4].hex(" ") + " …(%d B)" % len(after)),
            "desc": desc,
        })
    patched = bytes(buf)
    assert len(patched) == len(data), "原位补丁改变了长度"
    return patched, log


def _unpatch_core_only(patched: bytes) -> bytes:
    buf = bytearray(patched)
    buf[_off(SEND_SHORT):_off(SEND_SHORT) + 4] = SEND_SHORT_ENTRY
    buf[_off(SEND_PROP):_off(SEND_PROP) + 4] = SEND_PROP_ENTRY
    buf[_off(INJ_BASE):_off(INJ_END)] = INJ_ORIG
    return bytes(buf)


def _unpatch_keys(patched: bytes, key: int, mode: str = "keybtn2") -> bytes:
    buf = bytearray(_unpatch_core_only(patched))
    for addr, before, _after, _desc in _btn_patched_regions(key, mode):
        o = _off(addr)
        buf[o:o + len(before)] = before
    return bytes(buf)


def _key_expected_diff_offsets(key: int, mode: str = "keybtn2") -> set:
    offs: set = set()
    for addr, before, after, _desc in _btn_patched_regions(key, mode):
        base_off = _off(addr)
        for i, (b, a) in enumerate(zip(before, after)):
            if b != a:
                offs.add(base_off + i)
    return offs


# ---------------------------------------------------------------------------
# 按键注入 Unicorn 测试
# ---------------------------------------------------------------------------
def _uc_btn_env(patched: bytes, state: dict):
    """state: k0/k1/k2（bool 按下）、key_on、inhibit、id。返回 (uc, rec, hook)。"""
    from unicorn import (Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                         UC_HOOK_CODE)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_LR, UC_ARM_REG_PC)

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_FLASH_ALIGN, UC_FLASH_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_map(UC_PERIPH, UC_PERIPH_SIZE)
    uc.mem_write(BASE, patched)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))
    uc.mem_write(KEY_ON_FLAG_RAM, bytes([state.get("key_on", 0) & 0xFF]))
    uc.mem_write(STATE_INHIBIT_RAM, bytes([state.get("inhibit", 0) & 0xFF]))
    uc.mem_write(ID_RAM, bytes([state.get("id", 5) & 0xFF]))
    for ram in (BTN_RAM0_CNT, BTN_RAM0_FIRED, BTN_RAM1_CNT, BTN_RAM1_FIRED,
                BTN_RAM2_CNT, BTN_RAM2_FIRED):
        uc.mem_write(ram, b"\x00")
    rec = {"read_key": [], "low_send": [], "set_channel": [], "wreg": [],
           "send_prop": [], "send_short": [], "set_sync": [], "shutdown": 0}
    bit_masks = {BTN0_MASK: "k0", BTN1_MASK: "k1", BTN2_MASK: "k2"}
    # 完整扫描（FUN_0800CC30）时其它会被调用到的原厂外设/动作函数，
    # 测试中一律用"记录桩"直接返回，避免未建模外设/长延时干扰。
    extra_stubs = {0x08017018, 0x08012540, 0x0801C118,
                   0x08022E34, 0x08022E38, 0x08035FD0, 0x08013CDC}

    def hook(_uc, address, _size, _user):
        if address == UC_SENTINEL:
            _uc.emu_stop()
            return
        if address == READ_KEY:
            mask = _uc.reg_read(UC_ARM_REG_R1)
            rec["read_key"].append(mask)
            r0 = 0 if state.get(bit_masks.get(mask, ""), False) else 1
            _uc.reg_write(UC_ARM_REG_R0, r0)
        elif address == LOW_SEND_SHORT:
            rec["low_send"].append(_uc.reg_read(UC_ARM_REG_R0))
        elif address == SET_CHANNEL:
            rec["set_channel"].append(_uc.reg_read(UC_ARM_REG_R0))
        elif address == WRITE_REG:
            rec["wreg"].append((_uc.reg_read(UC_ARM_REG_R0),
                                _uc.reg_read(UC_ARM_REG_R1)))
        elif address == SEND_PROP:
            rec["send_prop"].append((_uc.reg_read(UC_ARM_REG_R0),
                                     _uc.reg_read(UC_ARM_REG_R1),
                                     _uc.reg_read(UC_ARM_REG_R2)))
        elif address == SEND_SHORT:
            rec["send_short"].append(_uc.reg_read(UC_ARM_REG_R0))
        elif address == SET_SYNC:
            rec["set_sync"].append(_uc.reg_read(UC_ARM_REG_R0))
        elif address == SHUTDOWN_WRAPPER:
            rec["shutdown"] += 1
        elif address not in extra_stubs:
            return
        _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    return uc, rec, hook


def _uc_btn_call(uc, entry: int) -> bool:
    from unicorn import UcError
    from unicorn.arm_const import (UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_SP,
                                   UC_ARM_REG_CPSR)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    uc.reg_write(UC_ARM_REG_PC, entry | 1)
    try:
        uc.emu_start(entry | 1, 0, count=500000)
    except UcError:
        return (uc.reg_read(UC_ARM_REG_PC) & ~1) == UC_SENTINEL
    return (uc.reg_read(UC_ARM_REG_PC) & ~1) == UC_SENTINEL


def _uc_read_byte(uc, addr: int) -> int:
    return uc.mem_read(addr, 1)[0]


def _hold(uc, state: dict, keyname: str, n: int, upd: int) -> bool:
    state[keyname] = True
    ok = True
    for _ in range(n):
        ok &= _uc_btn_call(uc, upd)
    state[keyname] = False
    ok &= _uc_btn_call(uc, upd)      # 松开清零
    return ok


def _check_probe(patched: bytes, key: int) -> tuple[bool, str]:
    cfg = _btn_cfg("keyprobe")
    _inj, layout = build_btn_injection(key, "keyprobe")
    state = {"k0": False, "k1": False, "k2": False,
             "key_on": 1, "inhibit": 0, "id": 5}
    uc, rec, _h = _uc_btn_env(patched, state)
    upd = layout["btn_update"]
    sync = common.ID_NUMERATOR // (5 + 1)      # 0x2091
    want_pair = [(4, (sync >> 8) & 0xFF), (5, sync & 0xFF)]
    msgs = []
    ok_all = True
    for bit, kname, nframes in ((0, "k0", 1), (1, "k1", 2), (2, "k2", 3)):
        n_low0 = len(rec["low_send"])
        n_w0 = len(rec["wreg"])
        ok_ret = _hold(uc, state, kname, cfg["threshold"], upd)
        got_low = rec["low_send"][n_low0:]
        got_w = rec["wreg"][n_w0:]
        ok = (ok_ret and len(got_low) == nframes
              and all(w in got_w for w in want_pair)
              and (4, key >> 8) not in got_w
              and _uc_read_byte(uc, KEY_ON_FLAG_RAM) == 1)
        ok_all &= ok
        msgs.append("bit%d 长按 -> %d 帧(应%d) SYNC=0x%02X%02X(应 %s)，KEY_ON 不变=%s"
                    % (bit, len(got_low), nframes, (sync >> 8) & 0xFF, sync & 0xFF,
                       "0x%02X%02X" % ((sync >> 8) & 0xFF, sync & 0xFF),
                       _uc_read_byte(uc, KEY_ON_FLAG_RAM) == 1))
    return ok_all, "探针：" + "; ".join(msgs)


def _check_keybtn(patched: bytes, key: int, mode: str) -> tuple[bool, str]:
    if mode == "keybtn4":
        return _check_keybtn4(patched, key)
    cfg = _btn_cfg(mode)
    _inj, layout = build_btn_injection(key, mode)
    state = {"k0": False, "k1": False, "k2": False,
             "key_on": 0, "inhibit": 0, "id": 5}
    uc, rec, _h = _uc_btn_env(patched, state)
    upd = layout["btn_update"]
    if mode == "keybtn3":
        # 实机探针最终键位：拨轮/M/C = bit0(PA9)，⚡ = bit1(PA8)
        wheel = _btn_key_bit(BTN0_PORT, BTN0_MASK)
        trig = _btn_key_bit(BTN1_PORT, BTN1_MASK)
    else:
        wheel = _btn_key_bit(BTN_WHEEL_PORT, BTN_WHEEL_MASK)
        trig = _btn_key_bit(BTN_TRIG_PORT, BTN_TRIG_MASK)
    ok_all = True
    msgs = []
    # 1) 拨轮/M/C 长按 -> KEY_ON 翻转
    state["key_on"] = 0
    _hold(uc, state, "k%d" % wheel, cfg["threshold"], upd)
    v1 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    _hold(uc, state, "k%d" % wheel, cfg["threshold"], upd)
    v2 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    ok = v1 == 1 and v2 == 0
    ok_all &= ok
    msgs.append("拨轮/M/C(bit%d)长按：KEY_ON %d(应1) / 再长按 %d(应0)" % (wheel, v1, v2))
    # 2) ⚡ 长按 -> 广播
    n_ch0 = len(rec["set_channel"])
    n_w0, n_p0 = len(rec["wreg"]), len(rec["send_prop"])
    n_s0 = len(rec["send_short"])
    _hold(uc, state, "k%d" % trig, cfg["threshold"], upd)
    n_ch = len(rec["set_channel"]) - n_ch0
    n_w = len(rec["wreg"]) - n_w0
    n_prop = len(rec["send_prop"]) - n_p0
    n_short = len(rec["send_short"]) - n_s0
    n_sync = len(rec["set_sync"])
    key_hi, key_lo = (key >> 8) & 0xFF, key & 0xFF
    ok = (n_ch == 33 and n_w == 64 and n_prop == 64 and n_short == 32 and n_sync == 1
          and all(w == (4, key_hi) or w == (5, key_lo) for w in rec["wreg"]))
    ok_all &= ok
    msgs.append("⚡(bit%d)长按：set_channel=%d(应33) SYNC写=%d(应64) 属性帧=%d(应64) "
                "TEST=%d(应32) 恢复同步=%d(应1)" % (trig, n_ch, n_w, n_prop,
                                                   n_short, n_sync))
    # 3) 未绑定的键长按不应触发任何手势
    bound = {wheel, trig}
    unbound = [b for b in (0, 1, 2) if b not in bound]
    if unbound:
        ub = unbound[0]
        cnt0 = (len(rec["set_channel"]), len(rec["wreg"]),
                len(rec["send_prop"]), len(rec["send_short"]))
        before = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
        _hold(uc, state, "k%d" % ub, cfg["threshold"] + 10, upd)
        after = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
        cnt1 = (len(rec["set_channel"]), len(rec["wreg"]),
                len(rec["send_prop"]), len(rec["send_short"]))
        ok = before == after and cnt0 == cnt1
        ok_all &= ok
        msgs.append("未绑定 bit%d 长按：无手势动作=%s" % (ub, ok))
    # 4) 删除 state[5] 跳过后，inhibit 非 0 仍应触发
    state["inhibit"] = 1
    state["key_on"] = 0
    _hold(uc, state, "k%d" % wheel, cfg["threshold"], upd)
    v3 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    ok = v3 == 1
    ok_all &= ok
    msgs.append("state[5]!=0 时拨轮长按仍切 KEY_ON=%d(应1)" % v3)
    return ok_all, ("%s：%s" % (mode, "; ".join(msgs)))


def _check_keybtn4(patched: bytes, key: int) -> tuple[bool, str]:
    cfg = _btn_cfg("keybtn4")
    _inj, layout = build_btn_injection(key, "keybtn4")
    state = {"k0": False, "k1": False, "k2": False,
             "key_on": 0, "inhibit": 0, "id": 5}
    uc, rec, _h = _uc_btn_env(patched, state)
    upd = layout["btn_update"]
    th1, th2 = cfg["threshold"], cfg["threshold2"]
    msgs = []
    ok_all = True
    # 1) 30 tick：只切 KEY_ON，不关机
    state["key_on"] = 0
    state["k0"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    v1 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    sd1 = rec["shutdown"]
    ok = v1 == 1 and sd1 == 0
    ok_all &= ok
    msgs.append("30 tick：KEY_ON=%d(应1) 关机=%d(应0)" % (v1, sd1))
    # 2) 继续到 400 tick：关机恰好一次
    for _ in range(th2 - th1):
        _uc_btn_call(uc, upd)
    sd2 = rec["shutdown"]
    v2 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    for _ in range(10):                       # 继续按住不应重复关机
        _uc_btn_call(uc, upd)
    sd3 = rec["shutdown"]
    ok = sd2 == 1 and sd3 == 1 and v2 == 1
    ok_all &= ok
    msgs.append("400 tick：关机=%d(应1) 再按 10 tick=%d(应1) KEY_ON 保持=%d" 
                % (sd2, sd3, v2))
    # 3) 松开清零；再次长按 30 tick：KEY_ON 切回 0，且不重复关机
    state["k0"] = False
    _uc_btn_call(uc, upd)
    state["k0"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    v3 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    sd4 = rec["shutdown"]
    state["k0"] = False
    _uc_btn_call(uc, upd)
    ok = v3 == 0 and sd4 == 1
    ok_all &= ok
    msgs.append("松开后再按 30 tick：KEY_ON=%d(应0) 关机次数仍=%d(应1)" % (v3, sd4))
    # 4) ⚡(bit1) 长按广播
    n_ch0 = len(rec["set_channel"])
    n_w0, n_p0 = len(rec["wreg"]), len(rec["send_prop"])
    n_s0 = len(rec["send_short"])
    state["k1"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    state["k1"] = False
    _uc_btn_call(uc, upd)
    n_ch = len(rec["set_channel"]) - n_ch0
    n_w = len(rec["wreg"]) - n_w0
    n_p = len(rec["send_prop"]) - n_p0
    n_s = len(rec["send_short"]) - n_s0
    n_sync = len(rec["set_sync"])
    key_hi, key_lo = (key >> 8) & 0xFF, key & 0xFF
    ok = (n_ch == 33 and n_w == 64 and n_p == 64 and n_s == 32 and n_sync == 1
          and all(w == (4, key_hi) or w == (5, key_lo) for w in rec["wreg"]))
    ok_all &= ok
    msgs.append("⚡(bit1)长按：频道=%d(应33) SYNC写=%d(应64) 属性帧=%d(应64) "
                "TEST=%d(应32) 恢复同步=%d(应1)" % (n_ch, n_w, n_p, n_s, n_sync))
    # 5) bit2 不绑定：长按无动作
    cnt0 = (len(rec["set_channel"]), len(rec["wreg"]),
            len(rec["send_prop"]), len(rec["send_short"]))
    sd0 = rec["shutdown"]
    before = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    state["k2"] = True
    for _ in range(th1 + 10):
        _uc_btn_call(uc, upd)
    state["k2"] = False
    _uc_btn_call(uc, upd)
    cnt1 = (len(rec["set_channel"]), len(rec["wreg"]),
            len(rec["send_prop"]), len(rec["send_short"]))
    ok = (before == _uc_read_byte(uc, KEY_ON_FLAG_RAM) and cnt0 == cnt1
          and sd0 == rec["shutdown"])
    ok_all &= ok
    msgs.append("bit2 长按无动作=%s" % ok)
    # 6) state[5]!=0 时仍触发第一段
    state["inhibit"] = 1
    state["key_on"] = 0
    state["k0"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    state["k0"] = False
    _uc_btn_call(uc, upd)
    v4 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    ok = v4 == 1
    ok_all &= ok
    msgs.append("state[5]!=0 时拨轮长按仍切 KEY_ON=%d(应1)" % v4)
    return ok_all, "keybtn4：" + "; ".join(msgs)


def _check_nobc(patched: bytes, key: int, mode: str) -> tuple[bool, str]:
    """无广播 A/B：bit0 两段式 + ⚡ 行为检查。"""
    cfg = _btn_cfg(mode)
    _inj, layout = build_btn_injection(key, mode)
    state = {"k0": False, "k1": False, "k2": False,
             "key_on": 0, "inhibit": 0, "id": 5}
    uc, rec, _h = _uc_btn_env(patched, state)
    upd = layout["btn_update"]
    th1, th2 = cfg["threshold"], cfg["threshold2"]
    msgs = []
    ok_all = True
    # 1) 30 tick 只切 KEY_ON
    state["key_on"] = 0
    state["k0"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    v1 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    sd1 = rec["shutdown"]
    ok = v1 == 1 and sd1 == 0
    ok_all &= ok
    msgs.append("30 tick：KEY_ON=%d(应1) 关机=%d(应0)" % (v1, sd1))
    # 2) 400 tick 关机一次，再按不重复
    for _ in range(th2 - th1):
        _uc_btn_call(uc, upd)
    sd2 = rec["shutdown"]
    for _ in range(10):
        _uc_btn_call(uc, upd)
    sd3 = rec["shutdown"]
    ok = sd2 == 1 and sd3 == 1
    ok_all &= ok
    msgs.append("400 tick：关机=%d(应1) 再按 10 tick=%d(应1)" % (sd2, sd3))
    # 3) 松开清零；再按 30 tick 只切回 KEY_ON
    state["k0"] = False
    _uc_btn_call(uc, upd)
    state["k0"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    v3 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    sd4 = rec["shutdown"]
    state["k0"] = False
    _uc_btn_call(uc, upd)
    ok = v3 == 0 and sd4 == 1
    ok_all &= ok
    msgs.append("松开后再按 30 tick：KEY_ON=%d(应0) 关机次数仍=%d(应1)" % (v3, sd4))
    # 4) ⚡ 长按：A=原厂关机路径被调用；B=完全无动作
    n_bc0 = (len(rec["set_channel"]), len(rec["send_prop"]), len(rec["send_short"]),
             len(rec["low_send"]))
    sd0 = rec["shutdown"]
    v_before = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    state["k1"] = True
    for _ in range(50):            # 原厂阈值 40：A 版应触发原厂关机路径
        _uc_btn_call(uc, KEYSCAN_FN)
    state["k1"] = False
    _uc_btn_call(uc, KEYSCAN_FN)
    sd1 = rec["shutdown"]
    n_bc1 = (len(rec["set_channel"]), len(rec["send_prop"]), len(rec["send_short"]),
             len(rec["low_send"]))
    v_after = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    if mode == "keybtn4_nobc_a":
        ok = sd1 > sd0
        msgs.append("⚡ 长按：原厂关机 FUN_08035FD0 调用=%d(应>0)、无广播帧=%s"
                    % (sd1 - sd0, n_bc0 == n_bc1))
        ok_all &= ok and (n_bc0 == n_bc1)
    else:
        ok = sd1 == sd0 and n_bc0 == n_bc1 and v_before == v_after
        ok_all &= ok
        msgs.append("⚡ 长按：无任何动作（关机=%d、广播帧变化=%s、KEY_ON 不变=%s）"
                    % (sd1 - sd0, n_bc0 == n_bc1, v_before == v_after))
    # 5) bit2 长按无动作
    cnt0 = (len(rec["set_channel"]), len(rec["send_prop"]),
            len(rec["send_short"]), len(rec["low_send"]), rec["shutdown"])
    v0 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    state["k2"] = True
    for _ in range(th1 + 10):
        _uc_btn_call(uc, upd)
    state["k2"] = False
    _uc_btn_call(uc, upd)
    cnt1 = (len(rec["set_channel"]), len(rec["send_prop"]),
            len(rec["send_short"]), len(rec["low_send"]), rec["shutdown"])
    ok = v0 == _uc_read_byte(uc, KEY_ON_FLAG_RAM) and cnt0 == cnt1
    ok_all &= ok
    msgs.append("bit2 长按无动作=%s" % ok)
    # 6) state[5]!=0 仍触发第一段
    state["inhibit"] = 1
    state["key_on"] = 0
    state["k0"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    state["k0"] = False
    _uc_btn_call(uc, upd)
    v4 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    ok = v4 == 1
    ok_all &= ok
    msgs.append("state[5]!=0 时拨轮长按仍切 KEY_ON=%d(应1)" % v4)
    return ok_all, "%s：%s" % (mode, "; ".join(msgs))


def _check_k1(patched: bytes, key: int, mode: str) -> tuple[bool, str]:
    """K1：两段式 stage1 正常切密钥；stage2 阈值 0xFFFF 永不触发关机。"""
    cfg = _btn_cfg(mode)
    _inj, layout = build_btn_injection(key, mode)
    state = {"k0": False, "k1": False, "k2": False,
             "key_on": 0, "inhibit": 0, "id": 5}
    uc, rec, _h = _uc_btn_env(patched, state)
    upd = layout["btn_update"]
    th1 = cfg["threshold"]
    msgs = []
    ok_all = True
    # 30 tick 切 KEY_ON（不关机）
    state["key_on"] = 0
    state["k0"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    v1 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    sd1 = rec["shutdown"]
    ok = v1 == 1 and sd1 == 0
    ok_all &= ok
    msgs.append("30 tick：KEY_ON=%d(应1) 关机=%d(应0)" % (v1, sd1))
    # 继续按 1000 tick：stage2 阈值 0xFFFF，应仍不关机
    for _ in range(1000 - th1):
        _uc_btn_call(uc, upd)
    sd2 = rec["shutdown"]
    ok = sd2 == 0
    ok_all &= ok
    msgs.append("继续按到 1000 tick：关机=%d(应0)" % sd2)
    # 松开清零，再按 30 tick：KEY_ON 切回 0
    state["k0"] = False
    _uc_btn_call(uc, upd)
    state["k0"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    v3 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    state["k0"] = False
    _uc_btn_call(uc, upd)
    ok = v3 == 0
    ok_all &= ok
    msgs.append("松开后再按 30 tick：KEY_ON=%d(应0)" % v3)
    # ⚡ 广播保留
    n_ch0 = len(rec["set_channel"])
    state["k1"] = True
    for _ in range(th1):
        _uc_btn_call(uc, upd)
    state["k1"] = False
    _uc_btn_call(uc, upd)
    n_ch = len(rec["set_channel"]) - n_ch0
    ok = n_ch == 33
    ok_all &= ok
    msgs.append("⚡ 广播频道=%d(应33)" % n_ch)
    return ok_all, "%s：%s" % (mode, "; ".join(msgs))


def _check_keybtn5(patched: bytes, key: int) -> tuple[bool, str]:
    """keybtn5：单计数器健壮版（无广播、⚡ 完全不动）。"""
    cfg = _btn_cfg("keybtn5")
    _inj, layout = build_btn_injection(key, "keybtn5")
    state = {"k0": False, "k1": False, "k2": False,
             "key_on": 0, "inhibit": 0, "id": 5}
    uc, rec, _h = _uc_btn_env(patched, state)
    upd = layout["btn_update"]
    th1, th2 = cfg["threshold"], cfg["threshold2"]
    msgs = []
    ok_all = True

    def press(n):
        state["k0"] = True
        for _ in range(n):
            _uc_btn_call(uc, upd)

    def release():
        state["k0"] = False
        _uc_btn_call(uc, upd)

    # 去抖会先消耗 3 次扫描：counter 在 (th1+3) 附近到 30、在 (th2+3) 附近到 400
    D = BTN_KEYBTN5_DEBOUNCE
    m_uc = uc
    # 1) 未到阈值（th1+D-6 次）不切；再多按 10 次到阈值 → 切 KEY_ON、不关机
    state["key_on"] = 0
    press(max(0, th1 + D - 6))
    v0 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    press(10)
    v1 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    sd1 = rec["shutdown"]
    ok = v0 == 0 and v1 == 1 and sd1 == 0
    ok_all &= ok
    msgs.append("阈值前 KEY_ON=%d(应0) → 到阈值 KEY_ON=%d(应1) 关机=%d(应0)"
                % (v0, v1, sd1))
    # 2) 继续按住：接近但未到 400 不关机；再按 30 次过 400 → 关机一次
    press(max(0, th2 - th1 - D - 20))
    v2 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    sd2 = rec["shutdown"]
    press(30)
    sd3 = rec["shutdown"]
    press(10)
    sd4 = rec["shutdown"]
    ok = v2 == 1 and sd2 == 0 and sd3 == 1 and sd4 == 1
    ok_all &= ok
    msgs.append("400 tick 前关机=%d(应0) → 到 400 tick 关机=%d(应1) → 再按 10 tick=%d(应1)"
                % (sd2, sd3, sd4))
    release()
    # 3) 外部清零 counter（模拟固件改写）→ 再按 30 tick 仍能切
    m_uc.mem_write(BTN_RAM5_CNT16, b"\x00\x00")
    m_uc.mem_write(BTN_RAM5_STREAK, b"\x00")
    m_uc.mem_write(KEY_ON_FLAG_RAM, b"\x00")
    state["key_on"] = 0
    press(th1 + D + 2)
    v3 = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    release()
    ok = v3 == 1
    ok_all &= ok
    msgs.append("外部清零 counter 后再按 30 tick：KEY_ON=%d(应1)" % v3)
    # 4) 空闲 2000 tick：不误动作、计数器归零
    m_uc.mem_write(KEY_ON_FLAG_RAM, b"\x00")
    state["key_on"] = 0
    state["k0"] = False
    sd_before = rec["shutdown"]
    for _ in range(2000):
        _uc_btn_call(uc, upd)
    cnt = int.from_bytes(m_uc.mem_read(BTN_RAM5_CNT16, 2), "little")
    ok = (_uc_read_byte(uc, KEY_ON_FLAG_RAM) == 0
          and rec["shutdown"] == sd_before and cnt == 0)
    ok_all &= ok
    msgs.append("空闲 2000 tick：计数器=%d(应0) 关机不变=%s"
                % (cnt, rec["shutdown"] == sd_before))
    # 5) MODER 非输入模式：即使读键为按下也不计数（防待机重配引脚）
    m_uc.mem_write(0x40020000, (1 << 18).to_bytes(4, "little"))  # PA9=output
    m_uc.mem_write(KEY_ON_FLAG_RAM, b"\x00")
    state["key_on"] = 0
    state["k0"] = True
    for _ in range(200):
        _uc_btn_call(uc, upd)
    cnt2 = int.from_bytes(m_uc.mem_read(BTN_RAM5_CNT16, 2), "little")
    state["k0"] = False
    m_uc.mem_write(0x40020000, (0).to_bytes(4, "little"))
    _uc_btn_call(uc, upd)
    ok = cnt2 == 0 and _uc_read_byte(uc, KEY_ON_FLAG_RAM) == 0
    ok_all &= ok
    msgs.append("MODER=输出时：计数器=%d(应0)、未切密钥=%s" % (cnt2, ok))
    # 6) ⚡(bit1) 完全原厂：完整扫描长按 50 次 → 原厂关机路径被调用（A 行为）
    sd0 = rec["shutdown"]
    b_cnt0 = (len(rec["set_channel"]), len(rec["send_prop"]),
              len(rec["send_short"]), len(rec["low_send"]))
    v_before = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    state["k1"] = True
    for _ in range(50):
        _uc_btn_call(uc, KEYSCAN_FN)
    state["k1"] = False
    _uc_btn_call(uc, KEYSCAN_FN)
    b_cnt1 = (len(rec["set_channel"]), len(rec["send_prop"]),
              len(rec["send_short"]), len(rec["low_send"]))
    ok = (rec["shutdown"] > sd0 and b_cnt0 == b_cnt1
          and v_before == _uc_read_byte(uc, KEY_ON_FLAG_RAM))
    ok_all &= ok
    msgs.append("⚡ 长按走原厂关机路径=%d(应>0)、无广播=%s、密钥不变=%s"
                % (rec["shutdown"] - sd0, b_cnt0 == b_cnt1,
                   v_before == _uc_read_byte(uc, KEY_ON_FLAG_RAM)))
    return ok_all, "keybtn5：" + "; ".join(msgs)


def _check_keybtn7(patched: bytes, key: int, mode: str = "keybtn7") -> tuple[bool, str]:
    """keybtn7/8：双击 M/C/拨轮(bit0) -> 32ch × 100ID 公共同步字广播（分块 + 喂狗）。"""
    _inj, layout = build_btn_injection(key, mode)
    upd = layout["btn_update"]
    msgs = []
    ok_all = True

    def new_env(key_on):
        env = {"k0": False, "k1": False, "k2": False,
               "key_on": key_on, "inhibit": 0, "id": 5}
        uc, rec, _h = _uc_btn_env(patched, env)
        for a in (BTN7_PRESS_START, BTN7_FIRST_PRESS, BTN7_PENDING,
                  BTN7_PREV, BTN7_ACTIVE, BTN7_SAVED_KEYON,
                  BTN7_CH, BTN7_ID, BTN_RAM_WHEEL_CNT16,
                  BTN_RAM5_STREAK):
            uc.mem_write(a, b"\x00")
        return uc, rec, env

    def call(uc, env, t, k0=False):
        uc.mem_write(TICK_RAM, int(t & 0xFFFFFFFF).to_bytes(4, "little"))
        env["k0"] = k0
        return _uc_btn_call(uc, upd)

    ck = "k1" if mode in ("keybtn11", "keybtn12") else "k0"

    def click(uc, env, t0, dur=100):
        env[ck] = True
        call(uc, env, t0, False)
        env[ck] = True
        call(uc, env, t0 + dur, False)
        env[ck] = False
        call(uc, env, t0 + dur, False)

    def finish(uc, env, rec, t0):
        chunks = []
        t = t0 + 1000          # 跳过与上一次调用相同的 tick（节流会让同 tick 无动作）
        idle = 0
        for _ in range(6000):
            n0 = (len(rec["set_channel"]), len(rec["set_sync"]),
                  len(rec["send_short"]))
            call(uc, env, t)
            t += 50
            n1 = (len(rec["set_channel"]), len(rec["set_sync"]),
                  len(rec["send_short"]))
            d = (n1[0] - n0[0], n1[1] - n0[1], n1[2] - n0[2])
            chunks.append(d)
            idle = idle + 1 if d == (0, 0, 0) else 0
            if idle >= 5:
                break
        return chunks

    # 0) KEY_ON=1 -> 双击 M/C 广播 -> 恢复 1
    uc, rec, env = new_env(1)
    click(uc, env, 1000)
    click(uc, env, 1200)
    chunks = finish(uc, env, rec, 1300)
    n_ch = len(rec["set_channel"])
    n_sync = len(rec["set_sync"])
    n_send = len(rec["send_short"])
    max_chunk = max((c[2] for c in chunks), default=0)
    v_after = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    lite = mode in ("keybtn8", "keybtn12")
    exp_sync = 32 if lite else 3200
    exp_send = 64 if lite else 3232
    max_send = BTN7_BUDGET * 2 if lite else BTN7_BUDGET + 2
    ok = (n_ch == 32 and n_sync == exp_sync and n_send == exp_send
          and rec["set_channel"] == list(range(32))
          and max_chunk <= max_send and v_after == 1)
    ok_all &= ok
    msgs.append("双击M/C广播 channel=%d(应32) set_sync=%d(应%d) send=%d(应%d) "
                "单块最大=%d(应<=%d) KEY_ON=1→%d" %
                (n_ch, n_sync, exp_sync, n_send, exp_send, max_chunk,
                 max_send, v_after))
    if not lite:
        ok = all(rec["set_sync"][c * 100 + i] == i
                 for c in range(32) for i in range(100))
        ok_all &= ok
        msgs.append("set_sync id 序列 0..99×32=%s" % ok)

    # 1) KEY_ON=0 时广播后仍 0
    uc, rec, env = new_env(0)
    click(uc, env, 5000)
    click(uc, env, 5200)
    finish(uc, env, rec, 5300)
    ok = (_uc_read_byte(uc, KEY_ON_FLAG_RAM) == 0
          and len(rec["set_channel"]) == 32)
    ok_all &= ok
    msgs.append("KEY_ON=0 广播后仍0=%s" % ok)

    # 2) 单击不广播
    uc, rec, env = new_env(0)
    click(uc, env, 9000)
    call(uc, env, 9100)
    ok = len(rec["set_channel"]) == 0
    ok_all &= ok
    msgs.append("单击不广播=%s" % ok)

    # 3) 长按 1.5s：切 KEY_ON（不广播）
    uc, rec, env = new_env(0)
    call(uc, env, 10000, True)
    for i in range(1, 35):
        call(uc, env, 10000 + i * 50, True)
    v_long = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    ok = v_long == 1 and len(rec["set_channel"]) == 0
    ok_all &= ok
    msgs.append("长按30tick切KEY_ON=%d(应1) 不广播=%s" % (v_long, len(rec["set_channel"]) == 0))
    # 4) 继续按住 20s：关机一次
    sd0 = rec["shutdown"]
    for i in range(35, 420):
        call(uc, env, 10000 + i * 50, True)
    env["k0"] = False
    call(uc, env, 10000 + 420 * 50)
    ok = rec["shutdown"] > sd0
    ok_all &= ok
    msgs.append("继续到400tick关机=%d(应>0)" % (rec["shutdown"] - sd0))

    # 5) 三连击只广播一次
    uc, rec, env = new_env(0)
    click(uc, env, 20000)
    click(uc, env, 20200)
    click(uc, env, 20400)
    finish(uc, env, rec, 20500)
    ok = len(rec["set_channel"]) == 32
    ok_all &= ok
    msgs.append("三连击只广播一次=%s(ch=%d sync=%d send=%d)" % (ok, len(rec["set_channel"]), len(rec["set_sync"]), len(rec["send_short"])))

    # 6) 空闲 2000 tick 无误动作
    uc, rec, env = new_env(0)
    for i in range(2000):
        call(uc, env, 30000 + i)
    ok = len(rec["set_channel"]) == 0
    ok_all &= ok
    msgs.append("空闲2000tick无动作=%s" % ok)

    # 7) 另一键双击不广播（keybtn7/8: ⚡；keybtn11: M/C）
    uc, rec, env = new_env(0)
    other = "k0" if mode in ("keybtn11", "keybtn12") else "k1"
    def oclick(t0, dur=100):
        env[other] = True; call(uc, env, t0)
        env[other] = True; call(uc, env, t0 + dur)
        env[other] = False; call(uc, env, t0 + dur)
    oclick(40000); oclick(40200)
    ok = len(rec["set_channel"]) == 0
    ok_all &= ok
    msgs.append("双击⚡不广播=%s" % ok)
    return ok_all, "keybtn7：" + "; ".join(msgs)


def _check_short_original(patched: bytes, key: int, mode: str) -> tuple[bool, str]:
    """完整扫描 FUN_0800CC30：短按走原厂，不触发手势。"""
    state = {"k0": False, "k1": False, "k2": False,
             "key_on": 0, "inhibit": 0, "id": 5}
    uc, rec, _h = _uc_btn_env(patched, state)
    ok_ret = True
    for kname in ("k0", "k1", "k2"):
        state[kname] = True
        for _ in range(3):
            ok_ret &= _uc_btn_call(uc, KEYSCAN_FN)
        state[kname] = False
        ok_ret &= _uc_btn_call(uc, KEYSCAN_FN)
    v = _uc_read_byte(uc, KEY_ON_FLAG_RAM)
    flag = _uc_read_byte(uc, 0x200003EB)   # bit1 短按 release 原厂会翻转
    ok = ok_ret and v == 0 and flag == 1
    return ok, ("完整扫描短按：三键短按后 KEY_ON=%d(应0)、bit1 原厂标志 0x200003EB=%d(应1)、"
                "全程正常返回=%s" % (v, flag, ok_ret))


def verify_keys(patched: bytes, key: int, mode: str = "keybtn2") -> list:
    """验证"core + 按键注入"补丁（mode=keyprobe / keybtn2）。"""
    key = common.validate_key(key)
    cfg = _btn_cfg(mode)
    results: list[tuple[bool, str]] = []
    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，长度不变）" % (len(patched), IMAGE_SIZE)))
    core_inj, _l = build_injection(key)
    got = patched[_off(INJ_BASE):_off(INJ_BASE) + len(core_inj)]
    results.append((got == core_inj, "core 注入区 0x%08X 起 %d 字节保持正确"
                    % (INJ_BASE, len(core_inj))))
    inj_code, layout = build_btn_injection(key, mode)
    got = patched[_off(BTN_INJ_BASE):_off(BTN_INJ_BASE) + len(inj_code)]
    results.append((got == inj_code,
                    "[%s] 按键注入区 0x%08X 起 %d 字节正确（布局 %s）"
                    % (mode, BTN_INJ_BASE, len(inj_code),
                       " ".join("%s=0x%08X" % (k, v) for k, v in layout.items()
                                if isinstance(v, int)))))
    rest = patched[_off(BTN_INJ_BASE) + len(inj_code):_off(BTN_INJ_BASE) + BTN_INJ_SIZE]
    results.append((rest == b"\xff" * len(rest),
                    "按键注入区剩余 %d 字节保持擦除态 0xFF（未越界）" % len(rest)))
    for addr, before, after, desc in _btn_patched_regions(key, mode):
        got = patched[_off(addr):_off(addr) + len(after)]
        results.append((got == after, "补丁点 0x%08X = %s（%s）"
                        % (addr, got.hex(" "), desc)))
    # 差异范围
    orig = _unpatch_keys(patched, key, mode)
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    expected = _expected_diff_offsets(key) | _key_expected_diff_offsets(key, mode)
    results.append((set(diffs) == expected,
                    "与原厂差异 %d 字节，全部落在补丁区（越界 %d）"
                    % (len(diffs), len([d for d in diffs if d not in expected]))))
    # core 双发行为 + 热靴白名单
    for ident, key_on in ((5, 1), (5, 0)):
        ok, msg = _check_short(patched, key, ident, 0, key_on, True)
        results.append((ok, msg))
    for hcmd in (0x09, 0x1A, 0x21, 0x29):
        ok, msg = _check_short(patched, key, 5, 0, 1, True, cmd=hcmd)
        results.append((ok, "[热靴/引闪白名单] " + msg))
    ok, msg = _check_short(patched, key, 5, 0, 1, True, cmd=0x40)
    results.append((ok, "[休眠/状态命令-单发] " + msg))
    try:
        import unicorn  # noqa: F401
        uc_ver = getattr(unicorn, "__version__", "?")
    except Exception:  # noqa: BLE001
        results.append((True, "警告: unicorn 不可用，跳过按键测试"))
        return results
    if mode.startswith("keybtn4_nobc"):
        results.append(("broadcast" not in layout,
                        "[%s] 无广播注入：注入代码里没有 broadcast 动作（layout=%s）"
                        % (mode, sorted(k for k in layout if isinstance(layout[k], str)))))
    results.append((True, "Unicorn %s 按键测试（mode=%s，阈值 %d 次扫描）"
                    % (uc_ver, mode, cfg["threshold"])))
    ok, msg = _check_short_original(patched, key, mode)
    results.append((ok, "短按完全走原厂: %s" % msg))
    if mode == "keyprobe":
        ok, msg = _check_probe(patched, key)
    elif mode.startswith("keybtn4_nobc"):
        ok, msg = _check_nobc(patched, key, mode)
    elif mode == "keybtn4_k1":
        ok, msg = _check_k1(patched, key, mode)
    elif mode == "keybtn5":
        ok, msg = _check_keybtn5(patched, key)
    elif mode in ("keybtn7", "keybtn8", "keybtn11", "keybtn12"):
        ok, msg = _check_keybtn7(patched, key, mode)
    else:
        ok, msg = _check_keybtn(patched, key, mode)
    results.append((ok, "模式行为 %s: %s" % (mode, msg)))
    return results
