# -*- coding: utf-8 -*-
"""Godox X3-S（X3 S 引闪器）V1.25 —— 私有密钥补丁模块。

两种模式
--------
1. **keybtn（默认，v2）** —— 机内开关 + 双发核心（对应 X3-C 的 keybtn 方案）：
   * **长按拨轮/M/C ≈1.5s 切换密钥模式**（KEY_ON 标志字节 0x20001080 取反）；
   * 密钥模式（KEY_ON=1）下，每个要发的帧发两次：
       ① 写 CC2500 SYNC = 私有 KEY → 发送（密钥灯收到）
       ② 现算并写 SYNC = floor(50024/(当前ID+1)) → 再发送（同 CH/同 ID 的普通灯收到）
     发完同步字停留在普通同步字（= 原厂同 ID 状态）。
   * 关闭密钥模式（KEY_ON=0）→ 完全原厂单发。
   * 兼容旧触发条件：ID == 99 且 ID-OFF 标志 == 0 时也算密钥模式（与 X3-C v3 一致）。
2. **fixed（v1，历史）** —— 固定 KEY：同步字恒等于私有 KEY（无开关、无双发）。

实现（全部原位字节补丁，文件长度不变 1,012,268 B）
--------------------------------------------------
* 核心（双发）：把两个「组帧函数」入口改成 4 字节 B.W，跳到 0x080EE000 注入代码：
      send_short(cmd)              0x0801184C -> INJ_SHORT
      send_property(group,key,val) 0x080117E8 -> INJ_PROP
  注入代码用「蹦床」重建被覆盖的 4 字节序言（push + ldr r4,=0x40020414）再 B.W 回
  原函数 +4，原厂组帧/发送逻辑 100% 保留。
* 按键（开关）：把按键扫描函数根 0x0800CBD0 入口改成 B.W，跳到 0x080EE400 注入：
  先执行原序言 `2de9f05f`，再跑手势检测（读键 API 0x08022D9C；PA9 mask 0x200 =
  拨轮/M/C），长按 30 次扫描 → `KEY_ON ^= 1`；松开清零。之后 B.W 回原函数体 +4。
  **不修改**原厂长按阈值（PA9 本身无原厂长按动作），短按/其它键逻辑完全不动。

证据（详见 keypatch/dev/x3s/NOTES.md、devices/x3s/README.md）
-------------------------------------------------------------
* X3-S 与 X3-C 是同一套代码的机型构建，关键函数逐字节相同、地址分段平移：
      send_short   0x080118B0 -> 0x0801184C (-0x64)
      send_property 0x0801184C -> 0x080117E8 (-0x64)
      set_sync/set_channel/写寄存器/延时/按键扫描/读键/低层发送 -> 统一 -0x118
        （set_sync 0x08014864->0x0801474C、按键扫描 0x0800CC30->0x0800CBD0、
          读键 0x08022EB4->0x08022D9C、WriteReg 0x08023540->0x08023428、
          低层发2字节 0x0801712C->0x08017014、低层发4字节 0x0801708C->0x08016F74）
* 注入区取主固件尾部擦除态 0x080EE000（core，1 KiB）与 0x080EE400（按键，2 KiB）：
  该区间（0x080ECE04 起约 0xB1FC 字节）全 0xFF，全镜像无字面量/分支引用。
* KEY_ON=0x20001080 及手势计数 0x20001096..0x20001099 在 X3-S 全镜像无字面量引用；
  **用 X3-C 的 Unicorn 整机模拟机加载 X3-S 原厂镜像实测**：启动 BSS 清零只在
  PC=0x08004A88 写一次 0（之后 30 万条指令内再无其它写），是安全 RAM 字节。

风险 / 未确认
-------------
1. 无实机验证：静态 + Unicorn 调用级 + 整机模拟器（X3-C 模拟机加载 X3-S）验证。
   刷机前请确认官方 G3 工具能识别设备，保留原厂 bin。
2. 键位映射沿用 X3-C 实机结论（PA9=拨轮/M/C），X3-S 按键扫描代码与 X3-C 逐字节相同，
   若实机手感/键位不符，改 BTN0_PORT/BTN0_MASK 一个常量重生成即可。
3. KEY_ON 是 **RAM 版**：关机/换电池后复位为 0（= 普通模式），需要重新长按开启。
4. 长按阈值为 30 次扫描（按主循环节奏估算 ≈1.5s，与 X3-C 相同），可改常量后重生成。
5. 双发是顺序发送，密钥模式下每帧耗时约 2 倍；普通模式无额外开销。
6. 硬件同步字不是密码学认证：SDR 抓包理论上可克隆。
"""
from __future__ import annotations

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
    "id": "x3s",
    "name": "Godox X3-S (X3 S trigger)",
    "kind": "trigger",                   # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "X3-S",
    "firmware_version": "V1.25",
    "image": "devices/x3s/firmware/X3_S_v1.25_app.bin",
    "base": 0x08004800,
    "sha256": "3cd40ef3f91369d3147c538cf6745f9308d82e14ad94c9bad9c8f129e0753e4d",
    "out_name": "X3_S_v1.25_app_key{key:04X}.bin",
    "container_name": "X3S_v1.25_app_key{key:04X}_container.bin",
    "notes": "机内开关（长按拨轮/M/C ≈1.5s 切换）+ 双发核心：密钥模式先发 KEY 帧再发"
             "当前 ID 普通帧（普通灯也兼容）；KEY_ON 是 RAM 版，重启回普通模式",
}

# ---- 镜像 -------------------------------------------------------------------
BASE = 0x08004800
IMAGE_SIZE = 1012268

# ---- v1「固定 KEY」补丁点（保留给 fixed 模式/历史对照） ----------------------
SYNC_FN = 0x0801474C               # void rf_set_id(uint8_t id)：写 CC2500 SYNC1/SYNC0
PATCH_MOVW_ADDR = 0x0801474E       # movw r1, #0xC368 -> movw r1, #KEY
PATCH_DIV_ADDR = 0x08014752        # adds r0, r0, #1 -> movs r0, #1
ORIG_MOVW = bytes.fromhex("4cf26831")
ORIG_DIV = bytes.fromhex("401c")
NEW_DIV = bytes.fromhex("0120")

ORIG_FN_HEAD = bytes.fromhex(
    "10b54cf26831401cb1fbf0f4210a0420"
    "0ef064fee1b205200ef060fe14200ef0"
)
assert len(ORIG_FN_HEAD) == 32

# ---- 组帧函数（核心双发入口跳转点） ----------------------------------------
SEND_SHORT = 0x0801184C            # send_short(cmd)
SEND_PROP = 0x080117E8             # send_property(group, key, value)
SEND_SHORT_ENTRY = bytes.fromhex("10b50a4c")    # push {r4,lr}; ldr r4,[pc,#0x28]
SEND_PROP_ENTRY = bytes.fromhex("70b50b4c")     # push {r4,r5,r6,lr}; ldr r4,[pc,#0x2c]
SEND_SHORT_RESUME = 0x08011850     # 原函数体（跳过前 4 字节）
SEND_PROP_RESUME = 0x080117EC

# ---- 注入区：主固件尾部擦除态 0xFF（全镜像无字面量/分支引用） ---------------
INJ_BASE = 0x080EE000
INJ_SIZE = 0x400                   # 预留 1 KiB（实际注入约 0x150 字节）
INJ_END = INJ_BASE + INJ_SIZE
INJ_ORIG = b"\xff" * INJ_SIZE

# ---- 辅助函数（Unicorn 验证里替换为记录/跳过桩） ----------------------------
WRITE_REG = 0x08023428             # CC2500_WriteReg(reg=r0, value=r1)
WRITE_CMD = 0x08023408             # CC2500_Strobe(cmd=r0)
DELAY = 0x0802319E                 # delay(n=r0)
LOW_SEND_SHORT = 0x08017014        # 低层：发送 2 字节 FIFO 帧
LOW_SEND_PROP = 0x08016F74         # 低层：发送 4 字节 FIFO 帧
SET_CHANNEL = 0x080146FC           # 设置信道（0-based）
SET_SYNC = 0x0801474C              # 按 ID 设置同步字（原厂；广播后恢复用）

# ---- RAM 变量（与 X3-C 同布局；均由 X3-C 模拟器实证/本模块验证） -------------
ID_RAM = 0x20000055                # 当前 ID 字节（0..99）
ID_OFF_FLAG_RAM = 0x20000143       # ID-OFF/特殊状态标志
KEY_ON_FLAG_RAM = 0x20001080       # 机内密钥开关（0=普通，非0=密钥模式；RAM 版）
TX_GATE_RAM = 0x20000146           # 组帧发送门控（本补丁不写，仅测试时清零）

ID_KEY_MODE = 99                   # 兼容旧方案：ID == 99（且 ID-OFF==0）也算密钥模式
SYNC_NUMERATOR = common.ID_NUMERATOR   # 50024 = 0xC368

# ---- 物理按键（长按开关） ----------------------------------------------------
KEYSCAN_FN = 0x0800CBD0            # 按键扫描（main 0x08034202 每主循环调用一次）
KEYSCAN_RESUME = 0x0800CBD4
KEYSCAN_ENTRY_BEFORE = bytes.fromhex("2de9f05f")  # push.w {r4..r8,sb,sl,fp,ip,lr}
KEYSCAN_LONG1_CMP = 0x0800CCB4     # cmp r0,#0x28（bit1 原厂长按阈值，本补丁不改）
KEYSCAN_LONG2_CMP = 0x0800CD16     # cmp r0,#0x1E（bit2 原厂长按阈值，本补丁不改）
KEYSCAN_CMP_BEFORE = bytes.fromhex("2828")
KEYSCAN_CMP2_BEFORE = bytes.fromhex("1e28")

BTN_INJ_BASE = 0x080EE400          # core 用 0x080EE000..0x080EE150
BTN_INJ_SIZE = 0x800               # 0x080EE400..0x080EEC00
BTN_INJ_ORIG = b"\xff" * BTN_INJ_SIZE

GPIOA_BASE = 0x40020000
BTN0_PORT, BTN0_MASK = GPIOA_BASE, 0x0200     # PA9 = 拨轮/M/C（X3-C 实机结论）
READ_KEY = 0x08022D9C              # FUN_08022D9C(port, mask)：返回 0=按下

BTN_RAM5_CNT16 = 0x20001096        # u16 长按计数
BTN_RAM5_STREAK = 0x20001098       # u8 连续按下次数（去抖）
BTN_KEYBTN5_TICKS1 = 30            # 1.5s：长按阈值（扫描次数）
BTN_KEYBTN5_WATCHDOG = 600         # 30s：异常一直按下时强制清零（防卡死）
BTN_KEYBTN5_DEBOUNCE = 3           # 连续 3 次同状态才计数（去抖）

# ---- Unicorn 测试用内存布局 -------------------------------------------------
UC_FLASH_ALIGN = 0x08004000
UC_FLASH_SIZE = 0x100000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x80000
UC_PERIPH = 0x40000000
UC_PERIPH_SIZE = 0x100000
UC_STACK = 0x20047000
UC_SENTINEL = 0x20046000


# ===========================================================================
# 极小 Thumb-2 汇编器（只覆盖本补丁用到的指令）
# ===========================================================================
def _pack16(hw: int) -> bytes:
    return struct.pack("<H", hw & 0xFFFF)


_COND = {
    "beq": 0xD000,
    "bne": 0xD100,
    "bcs": 0xD200,
    "bcc": 0xD300,
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
        self.emit(_pack16(0x4600 | (((rd >> 3) & 1) << 7) | ((rm & 0xF) << 3) | (rd & 7)))

    def cmp_imm(self, rn: int, imm: int) -> None:
        if not (0 <= rn <= 7 and 0 <= imm <= 0xFF):
            raise ValueError("cmp imm8 越界")
        self.emit(_pack16(0x2800 | (rn << 8) | imm))

    def cmp_reg(self, rn: int, rm: int) -> None:
        self.emit(_pack16(0x4280 | ((rm & 7) << 3) | (rn & 7)))

    def ldrb(self, rd: int, rn: int, imm5: int = 0) -> None:
        self.emit(_pack16(0x7800 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rd & 7)))

    def strb_imm(self, rt: int, rn: int, imm5: int = 0) -> None:
        self.emit(_pack16(0x7000 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rt & 7)))

    def ldrh(self, rt: int, rn: int, byte_off: int) -> None:
        imm5 = byte_off // 2
        if byte_off % 2 or not (0 <= imm5 <= 0x1F):
            raise ValueError("ldrh 偏移需为 0..62 的偶数")
        self.emit(_pack16(0x8800 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def strh(self, rt: int, rn: int, byte_off: int) -> None:
        imm5 = byte_off // 2
        if byte_off % 2 or not (0 <= imm5 <= 0x1F):
            raise ValueError("strh 偏移需为 0..62 的偶数")
        self.emit(_pack16(0x8000 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def ldr(self, rt: int, rn: int, byte_off: int) -> None:
        imm5 = byte_off // 4
        if byte_off % 4 or not (0 <= imm5 <= 0x1F):
            raise ValueError("ldr 偏移需为 0..124 的 4 的倍数")
        self.emit(_pack16(0x6800 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def str(self, rt: int, rn: int, byte_off: int) -> None:
        imm5 = byte_off // 4
        if byte_off % 4 or not (0 <= imm5 <= 0x1F):
            raise ValueError("str 偏移需为 0..124 的 4 的倍数")
        self.emit(_pack16(0x6000 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def adds_imm(self, rd: int, rn: int, imm: int) -> None:
        if not (0 <= imm <= 7):
            raise ValueError("adds_imm T1 立即数 0..7")
        self.emit(_pack16(0x1C00 | (imm << 6) | ((rn & 7) << 3) | (rd & 7)))

    def subs_imm(self, rd: int, rn: int, imm: int) -> None:
        if not (0 <= imm <= 7):
            raise ValueError("subs_imm T1 立即数 0..7")
        self.emit(_pack16(0x1E00 | (imm << 6) | ((rn & 7) << 3) | (rd & 7)))

    def eors(self, rdn: int, rm: int) -> None:
        self.emit(_pack16(0x4040 | ((rm & 7) << 3) | (rdn & 7)))

    def ands(self, rdn: int, rm: int) -> None:
        self.emit(_pack16(0x4000 | ((rm & 7) << 3) | (rdn & 7)))

    def lsrs_imm(self, rd: int, rm: int, imm5: int) -> None:
        if not (1 <= imm5 <= 31):
            raise ValueError("lsrs_imm 1..31")
        self.emit(_pack16(0x0800 | (imm5 << 6) | ((rm & 7) << 3) | (rd & 7)))

    def uxtb(self, rd: int, rm: int) -> None:
        self.emit(_pack16(0xB2C0 | ((rm & 7) << 3) | (rd & 7)))

    def udiv(self, rd: int, rn: int, rm: int) -> None:
        self.emit(struct.pack("<HH", 0xFBB0 | (rn & 0xF), 0xF0F0 | ((rd & 0xF) << 8) | (rm & 0xF)))

    def bx_lr(self) -> None:
        self.emit(_pack16(0x4770))

    # --- 分支（8 位条件 + 32 位 B/BL） ---------------------------------
    def _cond(self, kind: str, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append((kind, off, target))

    def beq(self, target) -> None:
        self._cond("beq", target)

    def bne(self, target) -> None:
        self._cond("bne", target)

    def bcc(self, target) -> None:
        self._cond("bcc", target)

    def bcs(self, target) -> None:
        self._cond("bcs", target)

    def _far_cond(self, cond, target) -> None:
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

    def movw(self, rd: int, imm16: int) -> None:
        imm4 = (imm16 >> 12) & 0xF
        i = (imm16 >> 11) & 1
        imm3 = (imm16 >> 8) & 0x7
        imm8 = imm16 & 0xFF
        self.emit(struct.pack("<HH", 0xF240 | (i << 10) | imm4,
                              (imm3 << 12) | ((rd & 0xF) << 8) | imm8))

    def movt(self, rd: int, imm16: int) -> None:
        imm4 = (imm16 >> 12) & 0xF
        i = (imm16 >> 11) & 1
        imm3 = (imm16 >> 8) & 0x7
        imm8 = imm16 & 0xFF
        self.emit(struct.pack("<HH", 0xF2C0 | (i << 10) | imm4,
                              (imm3 << 12) | ((rd & 0xF) << 8) | imm8))

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
        s = 1 if off < 0 else 0
        imm = off & 0x1FFFFFF
        i1 = (imm >> 23) & 1
        i2 = (imm >> 22) & 1
        imm10 = (imm >> 12) & 0x3FF
        imm11 = (imm >> 1) & 0x7FF
        j1 = (1 - i1) ^ s
        j2 = (1 - i2) ^ s
        return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                           0xD000 | (j1 << 13) | (j2 << 11) | imm11)

    @staticmethod
    def _enc_bw(off: int) -> bytes:
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

    def build(self) -> bytes:
        for kind, off, target in self.fix:
            tgt = self._resolve(target)
            addr = self.base + off
            if kind in _COND:
                imm = tgt - (addr + 4)
                if imm % 2 or not (-256 <= imm <= 254):
                    raise ValueError("%s 越界: 0x%08X -> 0x%08X (%d)" % (kind, addr, tgt, imm))
                self.buf[off:off + 2] = _pack16(_COND[kind] | ((imm // 2) & 0xFF))
            else:
                imm = self._branch_imm(addr, tgt)
                enc = self._enc_bw(imm) if kind == "bw" else self._enc_bl(imm)
                self.buf[off:off + 4] = enc
        return bytes(self.buf)


def _off(addr: int) -> int:
    return addr - BASE


def _movw(imm16: int, rd: int = 1) -> bytes:
    """Thumb-2 `movw rd, #imm16` 编码（T3，4 字节）。"""
    imm16 &= 0xFFFF
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    return struct.pack("<HH", 0xF240 | (i << 10) | imm4,
                       (imm3 << 12) | ((rd & 0xF) << 8) | imm8)


# ===========================================================================
# 注入代码生成：核心双发（0x080EE000）
# ===========================================================================
def _emit_addr(a: _Asm, rd: int, addr: int) -> None:
    a.movw(rd, addr & 0xFFFF)
    a.movt(rd, (addr >> 16) & 0xFFFF)


def _emit_dispatch(a: _Asm, single_label: str, keymode_label: str) -> None:
    """密钥模式判定：KEY_ON != 0，或（当前 ID == 99 且 ID-OFF == 0）。"""
    _emit_addr(a, 3, KEY_ON_FLAG_RAM)
    a.ldrb(3, 3)
    a.cmp_imm(3, 0)
    a.bne(keymode_label)
    _emit_addr(a, 0, ID_RAM)
    a.ldrb(0, 0)
    a.cmp_imm(0, ID_KEY_MODE)
    a.bne(single_label)
    _emit_addr(a, 1, ID_OFF_FLAG_RAM)
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
    """现算并写 SYNC = floor(50024 / (当前 ID + 1)) 的高/低字节（M4 硬件 UDIV）。"""
    _emit_addr(a, 0, ID_RAM)
    a.ldrb(0, 0)
    a.adds_imm(0, 0, 1)
    a.movw(1, SYNC_NUMERATOR & 0xFFFF)
    a.udiv(2, 1, 0)
    if hi:
        a.lsrs_imm(1, 2, 8)
        a.movs(0, 4)
    else:
        a.uxtb(1, 2)
        a.movs(0, 5)
    a.bl(WRITE_REG)


def build_injection(key: int) -> tuple[bytes, dict]:
    """生成核心注入区字节（INJ_BASE 起）与布局信息。"""
    a = _Asm(INJ_BASE)

    # 1) send_short(cmd) 包装
    a.label("inj_short")
    a.push(0x10, lr=True)          # push {r4,lr}
    a.mov(4, 0)                    # r4 = cmd
    _emit_dispatch(a, "short_single", "short_keymode")
    # 双发白名单（与 X3-C 一致）：TEST(0x09) + 热靴触发路径命令(0x1A/0x21/0x29)。
    # 其它命令（休眠/心跳/设置状态等）保持原厂单发，避免密钥帧把休眠帧送到灯上。
    for cmd in (0x09, 0x1A, 0x21, 0x29):
        a.cmp_imm(4, cmd)
        a.beq("short_double")
    a.b_w("short_single")
    a.label("short_double")
    _emit_sync_key(a, key)
    a.mov(0, 4)
    a.bl("tramp_short")
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

    # 3) 蹦床：重建被覆盖的 4 字节序言，再 B.W 回原函数体
    a.label("tramp_short")
    a.push(0x10, lr=True)
    _emit_addr(a, 4, 0x40020414)   # 原 ldr r4,[pc] 载入的 GPIO 基址
    a.b_w(SEND_SHORT_RESUME)

    a.label("tramp_prop")
    a.push(0x70, lr=True)
    _emit_addr(a, 4, 0x40020414)
    a.b_w(SEND_PROP_RESUME)

    code = a.build()
    if len(code) > INJ_SIZE:
        raise ValueError("核心注入代码 %d 字节超出预留空闲区 %d 字节" % (len(code), INJ_SIZE))
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


def _core_patched_regions(key: int) -> list[tuple[int, bytes, bytes, str]]:
    inj_code, layout = build_injection(key)
    inj_before = INJ_ORIG[:len(inj_code)]
    b1 = _Asm(SEND_SHORT)
    b1.b_w(layout["inj_short"])
    b2 = _Asm(SEND_PROP)
    b2.b_w(layout["inj_prop"])
    return [
        (SEND_SHORT, SEND_SHORT_ENTRY, b1.build(),
         "send_short(0x0801184C) 入口: B.W -> 双发注入包装"),
        (SEND_PROP, SEND_PROP_ENTRY, b2.build(),
         "send_property(0x080117E8) 入口: B.W -> 双发注入包装"),
        (INJ_BASE, inj_before, inj_code,
         "空闲闪存 0x080EE000 注入：密钥模式双发（KEY 帧 + 当前ID普通同步字帧）"),
    ]


# ===========================================================================
# 注入代码生成：按键长按开关（0x080EE400）
# ===========================================================================
def _emit_read_key(a: _Asm, port: int, mask: int) -> None:
    """r0 = FUN_08022D9C(port, mask)（0 = 按下）。"""
    _emit_addr(a, 0, port)
    a.movw(1, mask & 0xFFFF)
    a.bl(READ_KEY)


def _emit_toggle(a: _Asm) -> None:
    """KEY_ON ^= 1（只用 caller-saved r0/r1/r2）。"""
    a.label("toggle")
    _emit_addr(a, 0, KEY_ON_FLAG_RAM)
    a.ldrb(1, 0)
    a.movs(2, 1)
    a.eors(1, 2)
    a.strb_imm(1, 0, 0)
    a.bx_lr()


def _emit_watch(a: _Asm, prefix: str, port: int, mask: int) -> None:
    """单键长按检测：去抖 + 计数到阈值切 KEY_ON + 看门狗 + 松开清零。

    与 X3-C keybtn5 的 robust_watch 相同，只是去掉「20s 关机」第二段动作
    （X3-S 已有独立电源键，避免占用原厂关机手势）。
    """
    # 1) GPIOA MODER 输入模式校验（读不到/全 1 时跳过，防待机重配引脚误判）
    _emit_addr(a, 4, GPIOA_BASE)
    a.ldr(0, 4, 0)
    a.movw(1, 0xFFFF)
    a.movt(1, 0xFFFF)
    a.cmp_reg(0, 1)
    a.beq("%s_moder_ok" % prefix)
    a.lsrs_imm(0, 0, 18)           # PA9 的 MODER 位 = bit18..19
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
    _emit_addr(a, 4, BTN_RAM5_STREAK)
    a.ldrb(5, 4)
    a.cmp_imm(5, 0xFF)
    a.bcs("%s_streak_done" % prefix)
    a.adds_imm(5, 5, 1)
    a.strb_imm(5, 4, 0)
    a.label("%s_streak_done" % prefix)
    a.cmp_imm(5, BTN_KEYBTN5_DEBOUNCE)
    a.bcc("%s_done" % prefix)
    # 4) u16 计数器 +1
    _emit_addr(a, 4, BTN_RAM5_CNT16)
    a.ldrh(6, 4, 0)
    a.adds_imm(6, 6, 1)
    a.strh(6, 4, 0)
    # 5) 到达阈值：切 KEY_ON（只在沿上触发一次）
    a.cmp_imm(6, BTN_KEYBTN5_TICKS1)
    a.bne("%s_s1" % prefix)
    a.bl("toggle")
    a.label("%s_s1" % prefix)
    # 6) 看门狗：异常长时间按住时清零防卡死
    a.movw(5, BTN_KEYBTN5_WATCHDOG & 0xFFFF)
    a.cmp_reg(6, 5)
    a.bcc("%s_done" % prefix)
    _emit_addr(a, 4, BTN_RAM5_CNT16)
    a.movs(5, 0)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM5_STREAK)
    a.strb_imm(5, 4, 0)
    a.b_w("%s_done" % prefix)
    # 7) 松开/非输入模式：清零
    a.label("%s_release" % prefix)
    _emit_addr(a, 4, BTN_RAM5_CNT16)
    a.movs(5, 0)
    a.strh(5, 4, 0)
    _emit_addr(a, 4, BTN_RAM5_STREAK)
    a.strb_imm(5, 4, 0)
    a.label("%s_done" % prefix)


def build_btn_injection(key: int) -> tuple[bytes, dict]:
    """生成按键注入区字节（BTN_INJ_BASE 起）与布局信息。"""
    a = _Asm(BTN_INJ_BASE)
    # 入口：等价原序言 -> 手势检测 -> 回原函数体
    a.emit(KEYSCAN_ENTRY_BEFORE)
    a.bl("btn_update")
    a.b_w(KEYSCAN_RESUME)

    a.label("btn_update")
    a.push(0x170, lr=True)         # push {r4,r5,r6,lr}
    _emit_watch(a, "wheel", BTN0_PORT, BTN0_MASK)
    a.pop(0x170, pc=True)

    _emit_toggle(a)

    code = a.build()
    if len(code) > BTN_INJ_SIZE:
        raise ValueError("按键注入代码 %d 字节超出预留区 %d 字节" % (len(code), BTN_INJ_SIZE))
    return code, {"entry": BTN_INJ_BASE, "btn_update": a.labels["btn_update"],
                  "toggle": a.labels["toggle"], "size": len(code)}


def _btn_patched_regions(key: int) -> list[tuple[int, bytes, bytes, str]]:
    inj_code, _layout = build_btn_injection(key)
    b = _Asm(KEYSCAN_FN)
    b.b_w(BTN_INJ_BASE)
    return [
        (KEYSCAN_FN, KEYSCAN_ENTRY_BEFORE, b.build(),
         "按键扫描入口 0x0800CBD0: B.W -> 按键注入（每轮检测长按 PA9=拨轮/M/C）"),
        (BTN_INJ_BASE, BTN_INJ_ORIG[:len(inj_code)], inj_code,
         "按键注入：长按 30 次扫描 -> KEY_ON ^= 1；松开清零"),
    ]


# ===========================================================================
# 打补丁
# ===========================================================================
def _original_image() -> bytes:
    path = os.path.normpath(os.path.join(os.path.dirname(_KEYPATCH_DIR), DEVICE["image"]))
    with open(path, "rb") as f:
        data = f.read()
    want = DEVICE.get("sha256")
    if want and common.sha256_hex(data) != want:
        raise ValueError("原厂固件 sha256 不匹配（%s）" % path)
    return data


def _check_len(data: bytes) -> None:
    if len(data) != IMAGE_SIZE:
        raise ValueError("Godox X3-S V1.25 固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
                         % (IMAGE_SIZE, len(data)))


def _patch_regions(data: bytes, regions: list[tuple[int, bytes, bytes, str]]) -> tuple[bytes, list[dict]]:
    buf = bytearray(data)
    log: list[dict] = []
    for addr, before, after, desc in regions:
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
    if len(patched) != len(data):
        raise ValueError("原位补丁改变了长度（%d -> %d）" % (len(data), len(patched)))
    return patched, log


def apply_fixed(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """v1「固定 KEY」补丁：同步字恒为私有 KEY（无开关、无双发）。"""
    key = common.validate_key(key)
    _check_len(data)
    got = bytes(data[_off(SYNC_FN):_off(SYNC_FN) + len(ORIG_FN_HEAD)])
    if got != ORIG_FN_HEAD:
        raise ValueError("0x%08X 函数头与原厂不符（固件版本不对）：期望 %s 实际 %s"
                         % (SYNC_FN, ORIG_FN_HEAD[:16].hex(" "), got[:16].hex(" ")))
    regions = [
        (PATCH_MOVW_ADDR, ORIG_MOVW, _movw(key, rd=1),
         "0x0801474E movw r1,#0xC368(50024) -> movw r1,#0x%04X(KEY)" % key),
        (PATCH_DIV_ADDR, ORIG_DIV, NEW_DIV,
         "0x08014752 adds r0,r0,#1(除数 ID+1) -> movs r0,#1(除数 1)"),
    ]
    return _patch_regions(data, regions)


def apply_core(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """只打核心双发补丁（不含按键开关；密钥模式可由 KEY_ON 或 ID=99 触发）。"""
    key = common.validate_key(key)
    _check_len(data)
    patched, log = _patch_regions(data, _core_patched_regions(key))
    return patched, log


def apply(data: bytes, key: int, mode: str = "keybtn") -> tuple[bytes, list[dict]]:
    """默认 keybtn：核心双发 + 长按拨轮/M/C 切换密钥模式。

    mode:
        "keybtn" —— 双发 + 机内长按开关（默认，studio/CLI 用）
        "core"   —— 只双发（密钥模式靠 ID=99 或外部写 KEY_ON 触发）
        "fixed"  —— v1 固定 KEY（无开关、无双发）
    """
    if mode == "fixed":
        return apply_fixed(data, key)
    key = common.validate_key(key)
    _check_len(data)
    patched, log = _patch_regions(data, _core_patched_regions(key))
    if mode == "core":
        return patched, log
    if mode != "keybtn":
        raise ValueError("未知补丁模式 %r（keybtn/core/fixed）" % mode)
    patched, log2 = _patch_regions(patched, _btn_patched_regions(key))
    log.extend(log2)
    return patched, log


# ===========================================================================
# 通用小工具
# ===========================================================================
def _decode_bw(data: bytes, addr: int) -> int | None:
    """解码 addr 处的 Thumb-2 B.W，返回目标地址；不是 B.W 返回 None。"""
    o = _off(addr)
    if o < 0 or o + 4 > len(data):
        return None
    hw1, hw2 = struct.unpack_from("<HH", data, o)
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0x9000:
        return None
    s = (hw1 >> 10) & 1
    imm10 = hw1 & 0x3FF
    j1 = (hw2 >> 13) & 1
    j2 = (hw2 >> 11) & 1
    i1 = 1 - (j1 ^ s)
    i2 = 1 - (j2 ^ s)
    imm = ((s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | ((hw2 & 0x7FF) << 1))
    if imm & (1 << 24):
        imm -= 1 << 25
    return addr + 4 + imm


def _diff_offsets(orig: bytes, patched: bytes) -> set[int]:
    return {i for i in range(min(len(orig), len(patched))) if orig[i] != patched[i]}


def _expected_diff_offsets(regions: list[tuple[int, bytes, bytes, str]]) -> set[int]:
    offs: set[int] = set()
    for addr, before, after, _desc in regions:
        base_off = _off(addr)
        for i, (b, a) in enumerate(zip(before, after)):
            if b != a:
                offs.add(base_off + i)
    return offs


# ===========================================================================
# Unicorn 调用级验证
# ===========================================================================
def _uc_core_run(image: bytes, entry: int, args: tuple, ident: int,
                 id_off: int = 0, key_on: int = 0, count: int = 200000) -> dict:
    """执行（注入后的）send_short / send_property 入口，记录 SYNC 写与低层发送。"""
    from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                         UC_HOOK_CODE)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_SP,
                                   UC_ARM_REG_CPSR)
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_FLASH_ALIGN, UC_FLASH_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_map(UC_PERIPH, UC_PERIPH_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))
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
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        elif address == LOW_SEND_SHORT:
            calls.append(("send_short", None, None))
        elif address == LOW_SEND_PROP:
            calls.append(("send_prop", None, None))
        elif address == DELAY:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))
        else:
            return
        _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    arg_regs = (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2)
    for i, v in enumerate(args):
        uc.reg_write(arg_regs[i], v)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    try:
        uc.emu_start(entry | 1, 0, count=count)
    except UcError as exc:  # noqa: BLE001
        state["error"] = "%s (pc=0x%08X lr=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR))
    return {"calls": calls, "error": state["error"]}


def _fmt_calls(calls: list[tuple]) -> str:
    parts = []
    for k, a, _b in calls:
        if k == "wreg":
            parts.append("WriteReg(%d,0x%02X)" % (a, _b))
        elif k == "send_short":
            parts.append("SEND_SHORT")
        elif k == "send_prop":
            parts.append("SEND_PROP")
        else:
            parts.append("delay(%d)" % a)
    return " -> ".join(parts)


def _uc_btn_env(image: bytes):
    """构建按键扫描注入的 Unicorn 环境（读键用桩，停在原函数体入口）。"""
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_LR, UC_ARM_REG_PC,
                                   UC_ARM_REG_SP, UC_ARM_REG_CPSR)
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_FLASH_ALIGN, UC_FLASH_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_map(UC_PERIPH, UC_PERIPH_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))
    for ram in (BTN_RAM5_CNT16, BTN_RAM5_CNT16 + 1, BTN_RAM5_STREAK, KEY_ON_FLAG_RAM):
        uc.mem_write(ram, b"\x00")
    state = {"pressed": False, "stopped": False}

    def hook(_uc, address, _size, _user):
        if address == READ_KEY:
            _uc.reg_write(UC_ARM_REG_R0, 0 if state["pressed"] else 1)
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))
        elif address == KEYSCAN_RESUME:
            _uc.emu_stop()
            state["stopped"] = True

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    return uc, state


def _uc_scan(uc, state, pressed: bool) -> bool:
    """执行一次按键扫描（真实补丁后的入口 + 注入手势逻辑）。返回是否正常返回。"""
    from unicorn import UcError
    from unicorn.arm_const import (UC_ARM_REG_LR, UC_ARM_REG_SP,
                                   UC_ARM_REG_PC, UC_ARM_REG_CPSR)
    state["pressed"] = pressed
    state["stopped"] = False
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    try:
        uc.emu_start(KEYSCAN_FN | 1, 0, count=60000)
    except UcError:
        return False
    return state["stopped"]


def _uc_key_on(uc) -> int:
    return uc.mem_read(KEY_ON_FLAG_RAM, 1)[0]


# ===========================================================================
# 离线验证
# ===========================================================================
def verify_fixed(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """v1 固定 KEY 模式的验证（静态 + Unicorn）。"""
    results: list[tuple[bool, str]] = []
    key = common.validate_key(key)
    results.append((len(patched) == IMAGE_SIZE,
                    "输出长度 %d 与原厂 %d 一致" % (len(patched), IMAGE_SIZE)))
    o = _off(PATCH_MOVW_ADDR)
    want_movw = _movw(key, rd=1)
    results.append((bytes(patched[o:o + 4]) == want_movw,
                    "0x%08X movw r1,#0x%04X 编码 = %s"
                    % (PATCH_MOVW_ADDR, key, want_movw.hex(" "))))
    o = _off(PATCH_DIV_ADDR)
    results.append((bytes(patched[o:o + 2]) == NEW_DIV,
                    "0x%08X 除数为 1（movs r0,#1 = %s）" % (PATCH_DIV_ADDR, NEW_DIV.hex(" "))))
    try:
        orig = _original_image()
    except Exception as exc:  # noqa: BLE001
        results.append((False, "读取原厂镜像失败：%s" % exc))
        return results
    expected = _expected_diff_offsets([
        (PATCH_MOVW_ADDR, ORIG_MOVW, _movw(key, rd=1), ""),
        (PATCH_DIV_ADDR, ORIG_DIV, NEW_DIV, ""),
    ])
    diffs = _diff_offsets(orig, patched)
    results.append((diffs == expected,
                    "差异字节 %d 个 = 预期 %d 个（offset %s）"
                    % (len(diffs), len(expected), ", ".join("0x%05X" % d for d in sorted(diffs)))))
    results.append((_uc_sync_probe(orig, key, "原厂") and _uc_sync_probe(patched, key, "补丁"),
                    "Unicorn：原厂公式自证 + 补丁恒为 KEY（详见 unicorn_sync_test.py）"))
    return results


def _uc_sync_probe(image: bytes, key: int, label: str) -> bool:
    """调用 sync 函数，检查写入的 SYNC 是否符合期望（原厂公式或 KEY）。"""
    from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                         UC_HOOK_CODE)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC, UC_ARM_REG_SP, UC_ARM_REG_CPSR)
    sentinel = UC_SENTINEL
    for ident in (0, 5, 37, 99):
        uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
        uc.mem_map(UC_FLASH_ALIGN, UC_FLASH_SIZE)
        uc.mem_map(UC_RAM, UC_RAM_SIZE)
        uc.mem_write(BASE, image)
        uc.mem_write(sentinel, bytes.fromhex("fee7"))
        calls = []

        def hook(_uc, address, _size, _user):
            if address == sentinel:
                _uc.emu_stop()
                return
            if address in (WRITE_REG, WRITE_CMD, DELAY):
                calls.append((address, _uc.reg_read(UC_ARM_REG_R0),
                              _uc.reg_read(UC_ARM_REG_R1)))
            else:
                return
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

        uc.hook_add(UC_HOOK_CODE, hook)
        uc.reg_write(UC_ARM_REG_SP, UC_STACK)
        uc.reg_write(UC_ARM_REG_LR, sentinel | 1)
        uc.reg_write(UC_ARM_REG_R0, ident)
        uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
        try:
            uc.emu_start(SYNC_FN | 1, 0, count=50000)
        except UcError:
            return False
        regs = {a: b for (addr, a, b) in calls if addr == WRITE_REG}
        got = ((regs.get(4, 0) << 8) | regs.get(5, 0)) if (4 in regs and 5 in regs) else None
        want = key if label == "补丁" else SYNC_NUMERATOR // (ident + 1)
        if got != want:
            return False
    return True


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """自动识别补丁模式并验证（studio / CLI 共用）。"""
    key = common.validate_key(key)
    # 1) 先看是否 keybtn（核心双发 + 按键注入）：两个组帧入口被改成 B.W 即为注入模式
    t_short = _decode_bw(patched, SEND_SHORT)
    if t_short is not None and t_short != SEND_SHORT:
        return verify_keybtn(patched, key)
    # 2) 再看 fixed（movw 补丁）
    if (bytes(patched[_off(PATCH_MOVW_ADDR):_off(PATCH_MOVW_ADDR) + 4]) == _movw(key, rd=1)
            and bytes(patched[_off(PATCH_DIV_ADDR):_off(PATCH_DIV_ADDR) + 2]) == NEW_DIV):
        return verify_fixed(patched, key)
    return [(False, "未识别的补丁（既不是 keybtn 双发/按键补丁，也不是 fixed 固定密钥补丁）")]


def verify_keybtn(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """keybtn 模式验证：静态补丁点/差异 + Unicorn 双发语义 + 长按开关行为。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []
    results.append((len(patched) == IMAGE_SIZE,
                    "输出长度 %d 与原厂 %d 一致" % (len(patched), IMAGE_SIZE)))

    inj_core, layout_core = build_injection(key)
    inj_btn, layout_btn = build_btn_injection(key)

    # 1) 入口跳转
    results.append((_decode_bw(patched, SEND_SHORT) == layout_core["inj_short"],
                    "send_short 入口 B.W -> 0x%08X" % layout_core["inj_short"]))
    results.append((_decode_bw(patched, SEND_PROP) == layout_core["inj_prop"],
                    "send_property 入口 B.W -> 0x%08X" % layout_core["inj_prop"]))
    results.append((_decode_bw(patched, KEYSCAN_FN) == BTN_INJ_BASE,
                    "按键扫描入口 0x%08X B.W -> 0x%08X（长按切 KEY）" % (KEYSCAN_FN, BTN_INJ_BASE)))

    # 2) 注入区内容
    o = _off(INJ_BASE)
    results.append((bytes(patched[o:o + len(inj_core)]) == inj_core,
                    "核心注入区 0x%08X（%d B）与生成一致" % (INJ_BASE, len(inj_core))))
    o = _off(BTN_INJ_BASE)
    results.append((bytes(patched[o:o + len(inj_btn)]) == inj_btn,
                    "按键注入区 0x%08X（%d B）与生成一致" % (BTN_INJ_BASE, len(inj_btn))))

    # 3) 差异范围
    try:
        orig = _original_image()
    except Exception as exc:  # noqa: BLE001
        results.append((False, "读取原厂镜像失败：%s" % exc))
        return results
    regions = _core_patched_regions(key) + _btn_patched_regions(key)
    expected = _expected_diff_offsets(regions)
    diffs = _diff_offsets(orig, patched)
    results.append((diffs == expected,
                    "差异字节 %d 个 = 预期 %d 个（仅入口跳转 + 注入区）"
                    % (len(diffs), len(expected))))
    # 原厂长按阈值不能被改
    results.append((bytes(patched[_off(KEYSCAN_LONG1_CMP):_off(KEYSCAN_LONG1_CMP) + 2]) == KEYSCAN_CMP_BEFORE
                    and bytes(patched[_off(KEYSCAN_LONG2_CMP):_off(KEYSCAN_LONG2_CMP) + 2]) == KEYSCAN_CMP2_BEFORE,
                    "原厂其它键长按阈值保持原样（cmp #0x28 / #0x1E）"))

    # 4) Unicorn：原厂自证 + 双发语义
    hi, lo = (key >> 8) & 0xFF, key & 0xFF
    # 4a) 原厂 send_short 单发（自证 harness）
    r = _uc_core_run(orig, SEND_SHORT, (0x09,), 5, 0, 0)
    ok_orig = (r["error"] is None
               and [c[0] for c in r["calls"]] == ["send_short"])
    results.append((ok_orig, "原厂 send_short 自证：单发一次（%s）"
                    % (_fmt_calls(r["calls"]) if not r["error"] else r["error"])))
    # 4b) KEY_ON=0 → 单发，不写 SYNC
    r = _uc_core_run(patched, SEND_SHORT, (0x09,), 5, 0, 0)
    ok = (r["error"] is None and r["calls"] == [("send_short", None, None)])
    results.append((ok, "KEY 关：send_short 原厂单发（%s）"
                    % (_fmt_calls(r["calls"]) if not r["error"] else r["error"])))
    # 4c) KEY_ON=1 + 白名单 cmd → 双发（KEY 帧 + ID=5 普通帧 0x2091）
    r = _uc_core_run(patched, SEND_SHORT, (0x09,), 5, 0, 1)
    sync5 = SYNC_NUMERATOR // 6
    want = [("wreg", 4, hi), ("wreg", 5, lo), ("send_short", None, None),
            ("wreg", 4, (sync5 >> 8) & 0xFF), ("wreg", 5, sync5 & 0xFF),
            ("send_short", None, None)]
    ok = (r["error"] is None and r["calls"] == want)
    results.append((ok, "KEY 开：send_short(cmd=0x09) 双发：SYNC=0x%02X%02X -> 发 -> SYNC=0x%04X -> 发（%s）"
                    % (hi, lo, sync5, _fmt_calls(r["calls"]) if not r["error"] else r["error"])))
    # 4d) 白名单外 cmd → 保持单发
    r = _uc_core_run(patched, SEND_SHORT, (0x11,), 5, 0, 1)
    ok = (r["error"] is None and r["calls"] == [("send_short", None, None)])
    results.append((ok, "KEY 开：白名单外 send_short(cmd=0x11) 保持单发（%s）"
                    % (_fmt_calls(r["calls"]) if not r["error"] else r["error"])))
    # 4e) send_property KEY 开 → 双发
    r = _uc_core_run(patched, SEND_PROP, (0x50, 0xD1, 0x40), 5, 0, 1)
    want = [("wreg", 4, hi), ("wreg", 5, lo), ("send_prop", None, None),
            ("wreg", 4, (sync5 >> 8) & 0xFF), ("wreg", 5, sync5 & 0xFF),
            ("send_prop", None, None)]
    ok = (r["error"] is None and r["calls"] == want)
    results.append((ok, "KEY 开：send_property 双发（%s）"
                    % (_fmt_calls(r["calls"]) if not r["error"] else r["error"])))
    # 4f) 兼容旧语义：ID=99 且 KEY_ON=0 → 双发（第二帧 50024/100=0x01F4）
    r = _uc_core_run(patched, SEND_SHORT, (0x09,), 99, 0, 0)
    sync99 = SYNC_NUMERATOR // 100
    want = [("wreg", 4, hi), ("wreg", 5, lo), ("send_short", None, None),
            ("wreg", 4, (sync99 >> 8) & 0xFF), ("wreg", 5, sync99 & 0xFF),
            ("send_short", None, None)]
    ok = (r["error"] is None and r["calls"] == want)
    results.append((ok, "兼容 ID=99：KEY 关时也双发（第二帧 0x%04X）" % sync99))

    # 5) Unicorn：长按（去抖 3 次后开始计数，累计 32 次扫描）切 KEY_ON
    need = BTN_KEYBTN5_DEBOUNCE - 1 + BTN_KEYBTN5_TICKS1   # 2 + 30 = 32
    uc, st = _uc_btn_env(patched)
    seq_ok = True
    for _ in range(need - 1):
        seq_ok &= _uc_scan(uc, st, True)
    mid = _uc_key_on(uc)
    seq_ok &= _uc_scan(uc, st, True)
    after = _uc_key_on(uc)
    seq_ok &= _uc_scan(uc, st, True)
    stable = _uc_key_on(uc)
    results.append((seq_ok and mid == 0 and after == 1 and stable == 1,
                    "长按 %d 次扫描（去抖 %d + 阈值 %d）：第 %d 次前=0，第 %d 次切到 1，继续按住保持 1"
                    % (need, BTN_KEYBTN5_DEBOUNCE, BTN_KEYBTN5_TICKS1, need - 1, need)))
    # 6) 松开后再长按 → 切回 0
    ok = _uc_scan(uc, st, False) and _uc_key_on(uc) == 1   # 松开只清零，不改 KEY
    for _ in range(need):
        ok &= _uc_scan(uc, st, True)
    ok = ok and _uc_key_on(uc) == 0
    results.append((ok, "松开清零后再次长按 %d 次 -> KEY_ON 切回 0" % need))
    # 7) 短按不切换
    ok = True
    for _ in range(5):
        ok &= _uc_scan(uc, st, True)
        ok &= _uc_scan(uc, st, False)
    ok = ok and _uc_key_on(uc) == 0
    results.append((ok, "短按（<5 次扫描后松开）不改变 KEY_ON"))
    return results
