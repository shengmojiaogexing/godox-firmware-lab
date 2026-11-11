# -*- coding: utf-8 -*-
"""Godox X3 Pro（X3 Pro，C 版）引闪器 V1.22 —— 私有密钥「长按开关 + 兼容双发」补丁。

参考仓库里 X3-C / X3-S 的 keybtn 做法（`keypatch/devices/x3c.py`、`x3s.py`），
为 `X3pro_C_v1.22_app.bin`（1,941,020 B，sha256 `2e2b8a3b…`）加入同一把 2.4G 私有密钥，
并用**物理按键长按**切换密钥模式（不再占用 ID 菜单）。

语义
----
* **密钥模式（KEY_ON=1）**：长按指定按键 ≈1.5s 开启（再长按一次关闭）：
    引闪帧用 SYNC = 私有 KEY 发出（引闪已打补丁的密钥灯）；
    紧接着再用 SYNC = floor(50024/(当前ID+1)) 发一遍（引闪同频道同 ID 的原厂灯，兼容
    普通灯）；发完把同步字恢复到 KEY，保证异步队列/中断路径发出的帧仍走密钥同步字。
* **普通模式（KEY_ON=0，出场默认，重启复位）**：与原厂完全一致
    （同步字 = 50024/(ID+1)；每帧单发；ID 菜单恢复原厂含义）。

开关（长按）实现
----------------
* 按键扫描函数 `0x08027E44`（main 主循环每轮调用一次，`0x0802B282`）入口 4 字节改
  `B.W inj_btn`；注入代码自己读 `PA6`（GPIOA `0x42020000` mask `0x40`，原厂长按会进入
  关机/电源流程的那颗键），用私有 RAM 计数：
    - 按下且未触发：计数 +1；到 `BTN_TICKS=34`（与原厂长按阈值 0x21+1 同量级，≈1.5s）
      翻转 `KEY_ON`，并置 latch；
    - 松开：计数清零、latch 清零（下次按住只触发一次）；
    - 之后走蹦床回原按键扫描函数体（`0x08027E48`），短按/双击/其它键逻辑完全保留。
* **原厂长按动作禁用**：按键扫描状态机里有两处 `cmp r3,#0x21`（`0x0802830A` 普通分支、
  `0x080284F2` 另一状态分支），都改成 `cmp r3,#0xFF`，原厂「PA6 长按 → 关机流程
  （0x0802B7D0）/ countdown 状态 0x16」不再触发；电源键（GPIOB0）长按关机不受影响。
* KEY_ON / 计数 / latch 放在 `0x20001096..0x2000109A`（本固件全镜像无字面量引用；
  与 X3-S 的 keybtn 所用同一段空白 RAM；**RAM 版，重启复位为 0=普通模式**）。

数据通路（与上一版 ID=99 方案相同，仅开关来源改变）
--------------------------------------------------
1) 同步字函数 `0x0802F040` 内 10 字节改写为 `B.W inj_sync` + 3×NOP：
   `inj_sync` 读 KEY_ON：=1 → r4 = KEY；=0 → 执行原厂 `movw r4,#0xC368; add r3,r2;
   udiv r4,r4,r3`（= floor(50024/(ID+1))），再跳回 `0x0802F050` 继续原厂 SYNC1/SYNC0
   写入 + SCAL/延时序列。这样**所有**发送路径（同步直发、中断队列）在密钥模式下都用
   KEY 同步字。
2) 两个「低层发送函数」入口 4 字节改 `B.W inj_tx4 / inj_tx2`：
   * `0x0802EDC4` = 发 4 字节帧（0x7F FIFO burst + STX + 600µs + SFTX/SIDLE）
   * `0x0802EE2C` = 发 2 字节帧（0xD5 xx 短命令，STX + 500µs + SFTX/SIDLE）
   密钥模式：唤醒后写 SYNC=KEY → 发第一帧 → 唤醒后写 SYNC=50024/(ID+1) → 再发一帧 →
   写回 SYNC=KEY；普通模式：唤醒后写原厂同步字 → 单发（等效原厂行为）。
   蹦床（trampoline）重建被覆盖的 `push {r4,lr}; bl 0x0802EC64` 后跳回原函数体
   （`0x0802EDCA` / `0x0802EE32`），原厂组帧/FIFO/发射时序 100% 保留。
3) 注入代码写在主固件「尾部空闲闪存」`0x081C0000` 起（原厂全 0xFF；已扫描确认该段
   无字面量/分支引用；0x081B0A30..0x081F4000 整段为擦除态）。

关键事实（证据见 keypatch/dev/x3pro/NOTES.md）
---------------------------------------------
* MCU：Artery AT32F435/437 类 Cortex-M4F（Thumb-2，含 UDIV/FPU 指令）。
* 镜像基址：`0x08020000`（向量表 SP=0x20270000、Reset=0x08031345；0x11344 处是
  标准 Reset_Handler：设 SP → 拷 .data → 清 .bss → SystemInit/main；BSS 0x20000440..
  0x2001AF38）。
* 「ID → 同步字」函数：`0x0802F040`（90 B，0x0802F09A 起 2 B NOP 对齐，下一函数
  0x0802F09C）：`movw r4,#0xC368; add r3,r2; udiv r4,r4,r3` → 写 CC2500 reg4/reg5 →
  delay(20) → SCAL(0x33) → 1ms → SIDLE(0x36) → delay(700)；共 6 个 BL 调用点。
* 设置结构：RAM `0x20001768`：[+0]=频道，[+1]=ID（调用路径都读 `ldrb r0,[r4,#1]`）。
* 全镜像只有 4 处写 CC2500 reg4/reg5：初始化表 `0x0802E950/0x0802E962`（0xC3/0x68，
  随后被同步字函数覆盖）与本函数 `0x0802F05E/0x0802F06E`。
* 按键扫描 `0x08027E44`：读 PA6=`0x42020000` mask 0x40、PB4 mask 0x10、PB5 mask 0x20、
  GPIOC15=`0x42020800` mask 0x8000（相机热靴触发输入）；main 循环每轮调用。
  PA6 长按（扫描计数 > 0x21）在 `0x08028310` 进入电源/关机流程 `0x0802B7D0`。
* 辅助函数：写寄存器 `0x0802E71C(reg, buf, len)`（reg 带 0x40 burst 位可连写）、
  strobe `0x0802E88C(cmd)`、延时 `0x0802B5DC(us)`、射频唤醒/置忙 `0x0802EC64()`
  （置 RAM 0x20001D09 忙标志，中断发送路径 0x0802EE9C 检查它）、
  读 GPIO `0x08032A58(port, mask)`（返回 0=低电平/按下）。

风险（首次刷入请按安全阶梯）
--------------------------
* 本仓库无 X3 Pro 整机模拟器；本模块只有 Unicorn **调用级**验证（同步字/双发顺序/
  按键计数逻辑），没有整机 boot、没有实机验证 —— 与 V1C 同级别证据。
* **键位需实机确认**：静态分析只能确定 PA6 是「原厂长按会进关机流程」的那颗键；
  若实机按下的不是它，把 `BTN_MASK` 改成 0x10(PB4) 或 0x20(PB5) 重新生成即可
  （三者都在按键扫描里被读取；改一个常量即可）。
* 长按切换沿用原厂关机键的长按时机（约 1.5s），原厂「PA6 长按关机」被占用；
  关机仍可用电源键长按 / 屏幕菜单（电源键是 GPIOB0，与本手势无关）。
* KEY_ON 是 **RAM 版**：关机/换电池后复位为 0（普通模式），需要重新长按开启。
* 双发会让 2 字节短命令（D5 11/0A/21，原厂每个连发 3 次）每帧多一次 500µs 发射，
  时序会略变长；若实机出现 HSS/连拍异常，优先怀疑这里（可把 DUAL_SEND 关掉只剩
  「密钥模式 = 全 KEY 单发」）。
* 注入代码在两次发射之间改同步字：sync 写入在 `0x0802EC64` 置忙之后；第二次发完
  「恢复 KEY」的那一次写不在忙区间，理论上有几十 µs 竞态窗口（极小）。
* 静态扫描无法排除 DMA/运行时间接引用；KEY_ON 所在 RAM 字节为静态扫描空白区。
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))
_KEYPATCH_DIR = os.path.dirname(_HERE)
if _KEYPATCH_DIR not in sys.path:
    sys.path.insert(0, _KEYPATCH_DIR)

import common  # noqa: E402

# ---------------------------------------------------------------------------
# 设备元数据（studio / keypatch CLI / selftest 共用）
# ---------------------------------------------------------------------------
DEVICE = {
    "id": "x3pro",
    "name": "Godox X3 Pro (X3 Pro trigger, C)",
    "image": "devices/x3pro/firmware/X3pro_C_v1.22_app.bin",
    "kind": "trigger",                       # trigger=引闪器 / flash=闪光灯
    "model": "X3 Pro",
    "firmware_version": "V1.22",
    "base": 0x08020000,
    "sha256": "2e2b8a3b5ff70e62c4acddc406b055ea2625e4833a213e81863f96c0da5a911b",
    "out_name": "X3pro_C_v1.22_app_key{key:04X}.bin",
    "container_name": "X3pro_C_v1.22_app_key{key:04X}_container.bin",
    "notes": "长按按键切换密钥模式（≈1.5s，KEY_ON 为 RAM 版）：密钥模式 KEY 帧 + "
             "当前ID普通帧双发；普通模式原厂单发（按键扫描/同步字/发送入口注入）",
}

# ---------------------------------------------------------------------------
# 镜像 / 地址常量（全部经静态反汇编确认，见 keypatch/dev/x3pro/NOTES.md）
# ---------------------------------------------------------------------------
BASE = 0x08020000
IMAGE_SIZE = 1_941_020
IMAGE_SHA256 = DEVICE["sha256"]

SYNC_FN = 0x0802F040                 # 原厂「ID -> CC2500 同步字」函数
SYNC_PATCH_ADDR = 0x0802F046         # 函数体里 10 字节：movw+add+udiv
SYNC_ORIG_10 = bytes.fromhex("4cf268341344b4fbf3f4")
SYNC_CONT = 0x0802F050               # 改完 r4 后跳回这里继续原厂流程

TX4 = 0x0802EDC4                     # 发 4 字节帧（0x7F FIFO burst + STX）
TX4_ORIG = bytes.fromhex("10b5fff7")       # push {r4,lr}; bl 0x0802EC64 的前 4 字节
TX4_RESUME = 0x0802EDCA              # 原函数体（BL 之后）
TX2 = 0x0802EE2C                     # 发 2 字节帧（0xD5 xx 短命令）
TX2_ORIG = bytes.fromhex("10b5fff7")       # 同上（两个函数入口开头相同）
TX2_RESUME = 0x0802EE32

# 按键扫描入口：main 主循环 0x0802B282 每轮 bl 0x08027E44
KEYSCAN_FN = 0x08027E44
KEYSCAN_RESUME = 0x08027E48          # 跳过 4 字节 push.w 之后的原函数体
KEYSCAN_ORIG = bytes.fromhex("2de9f84f")   # push.w {r3,r4,r5,r6,r7,r8,sb,sl,fp,lr}
# 原厂 PA6 长按阈值（两处 `cmp r3,#0x21`，均在按键扫描状态机里）：
#   0x0802830A：普通长按分支（>0x21 后进入 0x08008310 动作块，最终可走关机 0x0802B7D0）
#   0x080284F2：状态机另一分支（要求 [0x20000087]==0x14 且 [0x20001089]!=0，
#               >0x21 后置 countdown 状态 0x16）
# 两处都改成 `cmp r3,#0xFF`（ff 2b），让原厂 PA6 长按动作永不触发（长按被密钥开关复用）。
LONG_CMP_ADDR = 0x0802830A
LONG_CMP_ADDR2 = 0x080284F2
LONG_CMP_ORIG = bytes.fromhex("212b")
LONG_CMP_AFTER = bytes.fromhex("ff2b")

WRITE_REG = 0x0802E71C               # write_reg(reg, buf, len)；reg|0x40 = burst
STROBE = 0x0802E88C                  # strobe(cmd)
DELAY_US = 0x0802B5DC                # delay_us(n)
RADIO_WAKE = 0x0802EC64              # 置射频忙标志 0x20001D09 并返回

GPIO_WRITE = 0x08032A64
GPIO_READ = 0x08032A58               # read_gpio(port, mask) -> 0=低电平/按下

SETTINGS = 0x20001768                # [+0]=channel, [+1]=ID
ID_OFF = 1
SYNC_NUMERATOR = 50024               # 原厂公式常量 0xC368
BUSY_FLAG = 0x20001D09               # 射频忙标志（中断发送路径检查它）
TX_BUF = 0x20001D14                  # 4/2 字节帧缓冲区（发完不清）

# ---- 长按开关（keybtn）------------------------------------------------------
KEY_ON_FLAG_RAM = 0x20001096         # u8：0=普通模式，非0=密钥模式（RAM 版）
BTN_CNT_RAM = 0x20001098             # u16：长按扫描计数
BTN_LATCH_RAM = 0x2000109A           # u8：本次按住已触发标志
BTN_GPIO = 0x42020000                # GPIOA
BTN_MASK = 0x40                      # PA6：原厂长按进关机流程的那颗键
BTN_TICKS = 34                       # 长按阈值（扫描次数，≈1.5s；原厂 0x21+1）
# 注：这三字节在 0x20001090..0x200010D3 的静态空白区（全镜像无字面量引用），
# 与 X3-S keybtn 使用的 0x20001080/0x20001096 同一片区域（该固件布局略不同）。

# 注入区：主固件尾部空闲闪存 0x081C0000 起（原厂全 0xFF）。
# 复核：0x081B0A30..0x081F4000 全 0xFF；其中 0x081EC000/0x081EC004/0x081EC0E0、
# 0x081F0000/0x081F0004/0x081F06B0/0x081F3FFD 有代码字面量引用（配置/参数区），
# 本模块只用 0x081C0000 起 0x800 字节，离两端都远。
INJ_BASE = 0x081C0000
INJ_SIZE = 0x800
INJ_END = INJ_BASE + INJ_SIZE
INJ_ORIG = b"\xff" * INJ_SIZE

# 调试开关：True = 密钥模式下「KEY 帧 + 当前ID普通帧」双发（兼容原厂灯）；
# False = 密钥模式下只发 KEY 帧（最保守，时序变化最小，但普通灯不会被引闪）。
DUAL_SEND = True


def _off(addr: int) -> int:
    return addr - BASE


def _sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


# ===========================================================================
# 极小 Thumb-2 汇编器（只覆盖本模块用到的指令；支持标签回填）
# ===========================================================================
_COND = {"beq": 0xD000, "bne": 0xD100, "bcc": 0xD300, "bcs": 0xD200,
         "bhi": 0xD800, "blo": 0xD900}


def _h(v: int) -> bytes:
    return struct.pack("<H", v & 0xFFFF)


class _Asm:
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

    # --- 16 位指令 ------------------------------------------------------
    def movs(self, rd: int, imm: int) -> None:
        assert 0 <= rd <= 7 and 0 <= imm <= 0xFF
        self.emit(_h(0x2000 | (rd << 8) | imm))

    def mov(self, rd: int, rm: int) -> None:
        assert 0 <= rd <= 7 and 0 <= rm <= 7
        self.emit(_h(0x4600 | ((rm & 0xF) << 3) | (rd & 7)))

    def cmp_imm(self, rn: int, imm: int) -> None:
        assert 0 <= rn <= 7 and 0 <= imm <= 0xFF
        self.emit(_h(0x2800 | (rn << 8) | imm))

    def ldrb(self, rd: int, rn: int, imm5: int = 0) -> None:
        assert 0 <= rn <= 7 and 0 <= imm5 <= 0x1F
        self.emit(_h(0x7800 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rd & 7)))

    def strb(self, rt: int, rn: int, imm5: int = 0) -> None:
        assert 0 <= rn <= 7 and 0 <= imm5 <= 0x1F
        self.emit(_h(0x7000 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rt & 7)))

    def ldrh(self, rt: int, rn: int, byte_off: int = 0) -> None:
        imm5 = byte_off // 2
        assert byte_off % 2 == 0 and 0 <= imm5 <= 0x1F
        self.emit(_h(0x8800 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def strh(self, rt: int, rn: int, byte_off: int = 0) -> None:
        imm5 = byte_off // 2
        assert byte_off % 2 == 0 and 0 <= imm5 <= 0x1F
        self.emit(_h(0x8000 | (imm5 << 6) | ((rn & 7) << 3) | (rt & 7)))

    def adds_imm3(self, rd: int, rn: int, imm: int) -> None:
        assert 0 <= imm <= 7
        self.emit(_h(0x1C00 | (imm << 6) | ((rn & 7) << 3) | (rd & 7)))

    def eors(self, rdn: int, rm: int) -> None:
        self.emit(_h(0x4040 | ((rm & 7) << 3) | (rdn & 7)))

    def lsrs_imm(self, rd: int, rm: int, imm5: int) -> None:
        assert 1 <= imm5 <= 31
        self.emit(_h(0x0800 | (imm5 << 6) | ((rm & 7) << 3) | (rd & 7)))

    def sub_sp(self, imm: int) -> None:
        # SUB (SP minus immediate) T2：imm 必须是 4 的倍数，编码 = imm/4
        assert 0 <= imm <= 0x1FC and imm % 4 == 0
        self.emit(_h(0xB080 | ((imm >> 2) & 0x7F)))

    def add_sp(self, imm: int) -> None:
        assert 0 <= imm <= 0x1FC and imm % 4 == 0
        self.emit(_h(0xB000 | ((imm >> 2) & 0x7F)))

    def mov_sp(self, rd: int) -> None:
        """mov rd, sp（T1 高寄存器 MOV，Rd 需 r0-r7，Rm=SP=13）"""
        assert 0 <= rd <= 7
        self.emit(_h(0x4600 | (13 << 3) | (rd & 7)))

    def push(self, low_regs: int, lr: bool = False) -> None:
        self.emit(_h(0xB400 | (0x100 if lr else 0) | (low_regs & 0xFF)))

    def pop(self, low_regs: int, pc: bool = False) -> None:
        self.emit(_h(0xBC00 | (0x100 if pc else 0) | (low_regs & 0xFF)))

    def push_w(self, reglist: int) -> None:
        self.emit(struct.pack("<HH", 0xE92D, reglist & 0xFFFF))

    def bx_lr(self) -> None:
        self.emit(_h(0x4770))

    def nop(self) -> None:
        self.emit(b"\x00\xbf")

    # --- 条件分支（16 位） ------------------------------------------------
    def cbz(self, rn: int, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("cbz", off, target))

    def cbnz(self, rn: int, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("cbnz", off, target))

    def bne(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("bne", off, target))

    def beq(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("beq", off, target))

    # --- 32 位指令 ------------------------------------------------------
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

    def udiv(self, rd: int, rn: int, rm: int) -> None:
        # UDIV (T1): 0xFBB0|Rn, 0xF0F0|(Rd<<8)|Rm（与原厂 0x0802F04C 一致）
        self.emit(struct.pack("<HH", 0xFBB0 | (rn & 0xF),
                              0xF0F0 | ((rd & 0xF) << 8) | (rm & 0xF)))

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
    def _branch_off(addr: int, target: int) -> int:
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
            if kind in ("cbz", "cbnz", "bne", "beq"):
                imm = tgt - (addr + 4)
                base = {"cbz": 0xB100, "cbnz": 0xB900}.get(kind)
                if base is not None:
                    # CBZ/CBNZ T1：range 0..126，imm5 = offset/2
                    if imm % 2 or not (0 <= imm <= 126):
                        raise ValueError("%s 越界: 0x%08X -> 0x%08X (%d)"
                                         % (kind, addr, tgt, imm))
                    self.buf[off:off + 2] = _h(base | (((imm // 2) & 0x3F) << 3))
                else:
                    # BNE/BEQ T1：±256，imm8 = offset/2
                    if imm % 2 or not (-256 <= imm <= 254):
                        raise ValueError("%s 越界: 0x%08X -> 0x%08X (%d)"
                                         % (kind, addr, tgt, imm))
                    self.buf[off:off + 2] = _h(_COND[kind] | ((imm // 2) & 0xFF))
            else:
                imm = self._branch_off(addr, tgt)
                enc = self._enc_bw(imm) if kind == "bw" else self._enc_bl(imm)
                self.buf[off:off + 4] = enc
        return bytes(self.buf)


# ===========================================================================
# 注入代码
# ===========================================================================
def build_injection(key: int) -> tuple[bytes, dict]:
    """生成 0x081C0000 起的注入代码与布局信息。

    区域布局：
        inj_sync       同步字函数内 10 字节改写后的分流（KEY_ON -> KEY）
        inj_tx4/2      4/2 字节帧发送入口包装（密钥模式双发/普通模式单发）
        inj_btn        按键扫描入口包装（长按计数 + KEY_ON 翻转）
        set_sync       (r0 = 16 位同步字) 写 CC2500 SYNC1/SYNC0（burst reg 0x44）
        set_key        r0 = KEY
        set_ord        r0 = floor(50024/(当前ID+1))
        tramp*         蹦床：重建原入口被覆盖的指令，跳回原函数体
    """
    a = _Asm(INJ_BASE)

    def _emit_addr(reg: int, addr: int) -> None:
        a.movw(reg, addr & 0xFFFF)
        a.movt(reg, (addr >> 16) & 0xFFFF)

    # ---- 1) 同步字函数分流 -------------------------------------------------
    a.label("inj_sync")
    _emit_addr(3, KEY_ON_FLAG_RAM)
    a.ldrb(3, 3)
    a.cmp_imm(3, 0)
    a.beq("sync_orig")
    a.movw(4, key & 0xFFFF)
    a.b_w(SYNC_CONT)
    a.label("sync_orig")
    a.movs(2, 1)                    # 原厂 0x0802F040 的 movs r2,#1
    a.mov(3, 0)                     # 原厂 0x0802F042 的 mov r3,r0（r3=ID）
    a.emit(SYNC_ORIG_10)            # movw r4,#0xC368; add r3,r2; udiv r4,r4,r3
    a.b_w(SYNC_CONT)

    # ---- 2) 两个发送入口包装 ----------------------------------------------
    def _tx_wrapper(label: str, t_key: str, t_ord: str) -> None:
        a.label(label)
        a.push(0x10, lr=True)                       # push {r4,lr}
        _emit_addr(3, KEY_ON_FLAG_RAM)
        a.ldrb(3, 3)
        a.cmp_imm(3, 0)
        a.beq(label + "_single")
        # --- 密钥模式：KEY 帧 + (可选) 当前 ID 普通帧 ---
        a.bl(t_key)
        if DUAL_SEND:
            a.bl(t_ord)
            a.bl("set_key")                         # 恢复 KEY，队列/中断发帧也走密钥
        a.pop(0x10, pc=True)
        a.label(label + "_single")
        a.bl(t_ord)                                 # 普通模式：先写原厂同步字再单发
        a.pop(0x10, pc=True)

    _tx_wrapper("inj_tx4", "tramp4_key", "tramp4_ord")
    _tx_wrapper("inj_tx2", "tramp2_key", "tramp2_ord")

    # ---- 3) 按键长按开关 ---------------------------------------------------
    a.label("inj_btn")
    a.push(0x10, lr=True)                           # push {r4,lr}
    # 读 PA6：read_gpio(GPIOA, 0x40)，返回 0 = 按下
    _emit_addr(0, BTN_GPIO)
    a.movs(1, BTN_MASK)
    a.bl(GPIO_READ)
    a.cmp_imm(0, 0)
    a.bne("btn_released")
    # --- 按下 ---
    _emit_addr(1, BTN_LATCH_RAM)
    a.ldrb(2, 1)
    a.cmp_imm(2, 0)
    a.bne("btn_scan")                               # 本次按住已触发过
    _emit_addr(2, BTN_CNT_RAM)
    a.ldrh(3, 2)
    a.adds_imm3(3, 3, 1)
    a.strh(3, 2)
    a.cmp_imm(3, BTN_TICKS)
    a.bne("btn_scan")
    # 到阈值：KEY_ON ^= 1
    _emit_addr(3, KEY_ON_FLAG_RAM)
    a.ldrb(0, 3)
    a.movs(2, 1)
    a.eors(0, 2)
    a.strb(0, 3)
    # latch = 1
    _emit_addr(1, BTN_LATCH_RAM)
    a.movs(0, 1)
    a.strb(0, 1)
    a.b_w("btn_scan")
    # --- 松开：计数/latch 清零 ---
    a.label("btn_released")
    _emit_addr(1, BTN_CNT_RAM)
    a.movs(2, 0)
    a.strh(2, 1)
    _emit_addr(1, BTN_LATCH_RAM)
    a.strb(2, 1)
    a.label("btn_scan")
    a.bl("tramp_btn")                               # 执行原厂按键扫描
    a.pop(0x10, pc=True)

    # ---- 4) 写同步字辅助函数 ----------------------------------------------
    # set_sync(r0 = 16 位同步字)：CC2500 burst 写 reg 0x44 -> SYNC1(4)/SYNC0(5)
    a.label("set_sync")
    a.push(0x10, lr=True)
    a.sub_sp(8)
    a.mov(4, 0)
    a.lsrs_imm(3, 4, 8)
    a.mov_sp(1)                                     # r1 = 栈缓冲区指针
    a.strb(3, 1, 0)                                 # buf[0] = SYNC1
    a.strb(4, 1, 1)                                 # buf[1] = SYNC0
    a.movs(2, 2)
    a.movs(0, 0x44)
    a.bl(WRITE_REG)
    a.add_sp(8)
    a.pop(0x10, pc=True)

    a.label("set_key")
    a.movw(0, key & 0xFFFF)
    a.b_w("set_sync")

    # set_ord：r0 = floor(50024 / (settings[ID]+1)) 后写同步字
    a.label("set_ord")
    _emit_addr(3, SETTINGS)
    a.ldrb(0, 3, ID_OFF)
    a.adds_imm3(0, 0, 1)
    a.movw(1, SYNC_NUMERATOR & 0xFFFF)
    a.udiv(4, 1, 0)
    a.mov(0, 4)
    a.b_w("set_sync")

    # ---- 5) 蹦床：重建原函数入口被覆盖的指令，跳回原函数体 ----------------
    def _tramp_tx(label: str, resume: int, sync_word: int | None) -> None:
        a.label(label)
        a.push(0x10, lr=True)                       # 原 push {r4,lr}
        a.bl(RADIO_WAKE)                            # 原 bl 0x0802EC64（唤醒/置忙）
        if sync_word is not None:
            a.movw(0, sync_word & 0xFFFF)           # 射频已唤醒，写同步字才有效
            a.bl("set_sync")
        else:
            a.bl("set_ord")
        a.b_w(resume)                               # 原函数体（BL 之后）

    _tramp_tx("tramp4_key", TX4_RESUME, key)
    _tramp_tx("tramp4_ord", TX4_RESUME, None)
    _tramp_tx("tramp2_key", TX2_RESUME, key)
    _tramp_tx("tramp2_ord", TX2_RESUME, None)

    a.label("tramp_btn")
    a.push_w(0x4FF8)                                # 原 push.w {r3..r11,lr}
    a.b_w(KEYSCAN_RESUME)

    code = a.build()
    if len(code) > INJ_SIZE:
        raise ValueError("注入代码 %d 字节超出预留空闲区 %d 字节" % (len(code), INJ_SIZE))
    layout = {
        "inj_sync": a.labels["inj_sync"],
        "inj_tx4": a.labels["inj_tx4"],
        "inj_tx2": a.labels["inj_tx2"],
        "inj_btn": a.labels["inj_btn"],
        "set_sync": a.labels["set_sync"],
        "set_key": a.labels["set_key"],
        "set_ord": a.labels["set_ord"],
        "tramp_btn": a.labels["tramp_btn"],
        "size": len(code),
    }
    return code, layout


def _patched_regions(key: int) -> list[tuple[int, bytes, bytes, str]]:
    """返回 [(addr, before, after, desc)]；before 取自已确认的原厂常量。"""
    inj_code, layout = build_injection(key)
    inj_before = INJ_ORIG[:len(inj_code)]

    b1 = _Asm(TX4)
    b1.b_w(layout["inj_tx4"])
    b2 = _Asm(TX2)
    b2.b_w(layout["inj_tx2"])
    b3 = _Asm(SYNC_PATCH_ADDR)
    b3.b_w(layout["inj_sync"])
    b3.nop()
    b3.nop()
    b3.nop()
    b4 = _Asm(KEYSCAN_FN)
    b4.b_w(layout["inj_btn"])
    return [
        (TX4, TX4_ORIG, b1.build(),
         "4 字节帧发送入口 -> inj_tx4（KEY_ON 时 KEY+当前ID 双发）"),
        (TX2, TX2_ORIG, b2.build(),
         "2 字节帧发送入口 -> inj_tx2（KEY_ON 时 KEY+当前ID 双发）"),
        (SYNC_PATCH_ADDR, SYNC_ORIG_10, b3.build(),
         "同步字函数 10 B 改写：KEY_ON -> SYNC=KEY，否则原厂 50024/(ID+1)"),
        (KEYSCAN_FN, KEYSCAN_ORIG, b4.build(),
         "按键扫描入口 -> inj_btn（长按 PA6 ≈1.5s 翻转 KEY_ON）"),
        (LONG_CMP_ADDR, LONG_CMP_ORIG, LONG_CMP_AFTER,
         "禁用原厂 PA6 长按动作 A（0x0802830A，cmp #0x21 -> #0xFF）"),
        (LONG_CMP_ADDR2, LONG_CMP_ORIG, LONG_CMP_AFTER,
         "禁用原厂 PA6 长按动作 B（0x080284F2，状态机另一分支，cmp #0x21 -> #0xFF）"),
        (INJ_BASE, inj_before, inj_code,
         "注入区（长按/双发/同步字辅助 + 蹦床，%d 字节，原厂 0xFF）" % len(inj_code)),
    ]


def _expected_diff_offsets(key: int) -> set[int]:
    offs: set[int] = set()
    for addr, before, after, _desc in _patched_regions(key):
        o = _off(addr)
        for i, (b, a) in enumerate(zip(before, after)):
            if b != a:
                offs.add(o + i)
    return offs


def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """原位打补丁：返回 (patched, patch_log)，长度不变。"""
    key = common.validate_key(key)
    if len(data) != IMAGE_SIZE:
        raise ValueError("固件长度 %d 与预期 %d 不符（版本不对）"
                         % (len(data), IMAGE_SIZE))
    got = _sha256(data)
    if got != IMAGE_SHA256:
        raise ValueError("固件 sha256 不符：%s（预期 %s，版本不对）"
                         % (got, IMAGE_SHA256))

    buf = bytearray(data)
    log: list[dict] = []
    for addr, before, after, desc in _patched_regions(key):
        o = _off(addr)
        got_bytes = bytes(data[o:o + len(before)])
        if got_bytes != before:
            raise ValueError("补丁点 0x%08X 原始字节不符：%s（预期 %s，固件版本不对）"
                             % (addr, got_bytes.hex(" "), before.hex(" ")))
        if len(before) != len(after):
            raise ValueError("补丁点 0x%08X 长度不一致" % addr)
        buf[o:o + len(after)] = after
        log.append({
            "addr": addr,
            "offset": o,
            "before": " ".join("%02x" % b for b in before),
            "after": " ".join("%02x" % b for b in after),
            "desc": desc,
        })
    return bytes(buf), log


# ===========================================================================
# 离线验证
# ===========================================================================
UC_FLASH = 0x08000000
UC_FLASH_SIZE = 0x00200000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x00010000
UC_SENTINEL = 0x20008000
UC_STACK = 0x2000FF00


def _unpatch(patched: bytes, key: int) -> bytes:
    """把补丁区还原成原厂镜像（用于原厂自证）。"""
    buf = bytearray(patched)
    for addr, before, after, _desc in _patched_regions(key):
        o = _off(addr)
        buf[o:o + len(before)] = before
    return bytes(buf)


def _uc_run(image: bytes, entry: int, regs: dict | None = None,
            stop_at: int | None = None, gpio_pressed: bool = False,
            arg_r0: int | None = None, key_on: int = 0,
            max_insn: int = 200000) -> dict:
    """Unicorn 调用级执行：桩替换 radio/GPIO 辅助函数，记录 SYNC/FIFO 写入序列。

    - entry：起始 PC（Thumb 位自动 OR 1）
    - stop_at：PC 到该地址即停止（用于按键注入「只验证注入逻辑、不进原函数体」）
    - gpio_pressed：桩 GPIO_READ 的返回值（True -> r0=0=按下）
    """
    from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                         UC_HOOK_CODE)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_LR, UC_ARM_REG_PC,
                                   UC_ARM_REG_SP, UC_ARM_REG_CPSR)

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_FLASH, UC_FLASH_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))       # b . 哨兵
    uc.mem_write(SETTINGS, bytes([1, 5, 2, 3]))            # channel=1, ID=5
    uc.mem_write(TX_BUF, b"\x11\x22\x33\x44")
    uc.mem_write(BUSY_FLAG, b"\x00")
    uc.mem_write(KEY_ON_FLAG_RAM, bytes([key_on & 0xFF]))
    uc.mem_write(BTN_CNT_RAM, b"\x00\x00")
    uc.mem_write(BTN_LATCH_RAM, b"\x00")
    for addr, val in (regs or {}).get("mem", []):
        uc.mem_write(addr, val)

    seq: list[tuple] = []
    state = {"error": None, "hit_stop": False}

    def hook(_uc, address, _size, _user):
        if stop_at is not None and address == stop_at:
            state["hit_stop"] = True
            _uc.emu_stop()
            return
        if address == UC_SENTINEL:
            _uc.emu_stop()
            return
        if address == WRITE_REG:
            reg = _uc.reg_read(UC_ARM_REG_R0)
            buf = _uc.reg_read(UC_ARM_REG_R1)
            ln = _uc.reg_read(UC_ARM_REG_R2)
            data_bytes = bytes(_uc.mem_read(buf, ln)) if ln else b""
            if reg == 0x44 and ln == 2:
                seq.append(("sync", (data_bytes[0] << 8) | data_bytes[1]))
            elif reg == 0x7F:
                seq.append(("fifo", len(data_bytes)))
            elif reg in (4, 5) and ln == 1:
                seq.append(("syncbyte", reg, data_bytes[0]))
            else:
                seq.append(("wreg", reg, ln))
        elif address == STROBE:
            seq.append(("strobe", _uc.reg_read(UC_ARM_REG_R0)))
        elif address in (DELAY_US, RADIO_WAKE, GPIO_WRITE):
            pass
        elif address == GPIO_READ:
            _uc.reg_write(UC_ARM_REG_R0, 0 if gpio_pressed else 1)
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))
        else:
            return
        _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    if arg_r0 is not None:
        uc.reg_write(UC_ARM_REG_R0, arg_r0 & 0xFFFFFFFF)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)              # Thumb
    try:
        uc.emu_start(entry | 1, 0, count=max_insn)
    except UcError as exc:  # noqa: BLE001
        state["error"] = "%s (pc=0x%08X lr=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR))
    out = {"seq": seq, "error": state["error"], "hit_stop": state["hit_stop"]}
    # 读回注入用到的 RAM 状态（按键测试用）
    out["ram"] = {
        "key_on": uc.mem_read(KEY_ON_FLAG_RAM, 1)[0],
        "cnt": int.from_bytes(uc.mem_read(BTN_CNT_RAM, 2), "little"),
        "latch": uc.mem_read(BTN_LATCH_RAM, 1)[0],
    }
    return out


def _fmt(seq: list[tuple]) -> str:
    out = []
    for item in seq:
        if item[0] == "sync":
            out.append("SYNC=0x%04X" % item[1])
        elif item[0] == "syncbyte":
            out.append("SYNC%d=0x%02X" % (item[1], item[2]))
        elif item[0] == "fifo":
            out.append("FIFO(%d)" % item[1])
        elif item[0] == "wreg":
            out.append("wreg(0x%02X,%d)" % (item[1], item[2]))
        else:
            out.append("strobe(0x%02X)" % item[1])
    return " -> ".join(out)


def _sync_word_from_seq(seq: list[tuple]) -> tuple[int | None, list[int]]:
    hi = lo = None
    rest: list[tuple] = []
    for item in seq:
        if item[0] == "sync":
            hi, lo = (item[1] >> 8) & 0xFF, item[1] & 0xFF
        elif item[0] == "syncbyte":
            if item[1] == 4:
                hi = item[2]
            else:
                lo = item[2]
        else:
            rest.append(item)
    if hi is None or lo is None:
        return None, rest
    return (hi << 8) | lo, rest


def _ord_sync(ident: int) -> int:
    return SYNC_NUMERATOR // (ident + 1)


def _check_tx(patched: bytes, key: int, which: int, key_on: int,
              ident: int, patched_mode: bool) -> tuple[bool, str]:
    """which: 4 或 2（TX4/TX2）。"""
    image = patched if patched_mode else _unpatch(patched, key)
    fn = TX4 if which == 4 else TX2
    label = "补丁版" if patched_mode else "原厂(自证)"
    res = _uc_run(image, fn, regs={"mem": [(SETTINGS, bytes([1, ident, 2, 3]))]},
                  key_on=key_on)
    if res["error"]:
        return False, "TX%d ID=%d 执行失败: %s" % (which, ident, res["error"])
    seq = res["seq"]
    body = [("fifo", which), ("strobe", 0x35), ("strobe", 0x3B), ("strobe", 0x36)]
    want_sync = key if key_on else _ord_sync(ident)
    if not patched_mode:
        ok = seq == body
        return ok, ("TX%d ID=%-2d %s：原厂单发（无 SYNC 写入）" % (which, ident, label))
    if key_on:
        want = [("sync", want_sync)] + body
        if DUAL_SEND:
            want += [("sync", _ord_sync(ident))] + body + [("sync", key)]
        ok = seq == want
        return ok, ("TX%d ID=%-2d %s(密钥模式)：SYNC=0x%04X -> 发 -> SYNC=0x%04X -> 发"
                    " -> SYNC=0x%04X | %s"
                    % (which, ident, label, key, _ord_sync(ident), key, _fmt(seq)))
    want = [("sync", want_sync)] + body
    ok = seq == want
    return ok, ("TX%d ID=%-2d %s(普通模式)：SYNC=0x%04X -> 单发 | %s"
                % (which, ident, label, want_sync, _fmt(seq)))


def _check_sync_fn(image: bytes, key: int, ident: int, key_on: int,
                   patched_mode: bool) -> tuple[bool, str]:
    label = "补丁版" if patched_mode else "原厂(自证)"
    res = _uc_run(image, SYNC_FN, arg_r0=ident, key_on=key_on)
    if res["error"]:
        return False, "set_sync(ID=%d) %s 执行失败: %s" % (ident, label, res["error"])
    want = key if (patched_mode and key_on) else _ord_sync(ident)
    got, rest = _sync_word_from_seq(res["seq"])
    strobes = [item[1] for item in rest if item[0] == "strobe"]
    ok = (got == want) and strobes == [0x33, 0x36]
    mode = "密钥模式" if (patched_mode and key_on) else "原厂模式"
    return ok, ("set_sync(%s ID=%-2d, %s): SYNC=0x%04X（期望 0x%04X），"
                "SCAL/SIDLE=%s"
                % (label, ident, mode, got if got is not None else -1, want,
                   [hex(s) for s in strobes]))


def _check_button(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """Unicorn 按键长按逻辑测试：只跑注入段，到原扫描函数体入口即停。"""
    out: list[tuple[bool, str]] = []
    cases = [
        (False, "松开复位", {"key_on": 1, "cnt": 5, "latch": 1},
         {"cnt": 0, "key_on": 1, "latch": 0}),
        (True, "按下 #1", {"key_on": 0, "cnt": 0, "latch": 0},
         {"cnt": 1, "key_on": 0, "latch": 0}),
        (True, "按下 #34（到阈值）", {"key_on": 0, "cnt": 33, "latch": 0},
         {"cnt": 34, "key_on": 1, "latch": 1}),
        (True, "第 35 次（已 latch）", {"key_on": 1, "cnt": 34, "latch": 1},
         {"cnt": 34, "key_on": 1, "latch": 1}),
        (True, "再次长按 #34", {"key_on": 1, "cnt": 33, "latch": 0},
         {"cnt": 34, "key_on": 0, "latch": 1}),
    ]
    for pressed, desc, before, expect in cases:
        mem = [(KEY_ON_FLAG_RAM, bytes([before["key_on"]])),
               (BTN_CNT_RAM, int(before["cnt"]).to_bytes(2, "little")),
               (BTN_LATCH_RAM, bytes([before["latch"]]))]
        res = _uc_run(patched, KEYSCAN_FN, regs={"mem": mem},
                      stop_at=KEYSCAN_RESUME, gpio_pressed=pressed)
        if res["error"]:
            out.append((False, "按键 %s 执行失败: %s" % (desc, res["error"])))
            continue
        mem_ok = res["hit_stop"]
        got = res["ram"]
        ok = mem_ok and all(got[k] == v for k, v in expect.items())
        out.append((ok, "按键 %s：到达原扫描函数体=%s，KEY_ON=%d cnt=%d latch=%d"
                    "（期望 %s）"
                    % (desc, mem_ok, got["key_on"], got["cnt"], got["latch"],
                       {k: v for k, v in expect.items()})))
    return out


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证：长度 / 静态字节 / 差异范围 / Unicorn 调用级（长按+同步字+双发）。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁+注入）"
                    % (len(patched), IMAGE_SIZE)))

    inj_code, layout = build_injection(key)
    got = patched[_off(INJ_BASE):_off(INJ_BASE) + len(inj_code)]
    results.append((got == inj_code,
                    "注入区 0x%08X 起 %d 字节（长按开关 + 双发包装 + 同步字分流，共 %d/%d 字节）"
                    % (INJ_BASE, len(inj_code), len(inj_code), INJ_SIZE)))
    rest = patched[_off(INJ_BASE) + len(inj_code):_off(INJ_END)]
    results.append((rest == INJ_ORIG[len(inj_code):],
                    "注入区剩余 %d 字节保持擦除态 0xFF（不越界改写）" % len(rest)))

    for addr, before, after, desc in _patched_regions(key):
        if addr == INJ_BASE:
            continue
        got = patched[_off(addr):_off(addr) + len(after)]
        results.append((got == after, "补丁点 0x%08X = %s（%s）"
                        % (addr, got.hex(" "), desc)))

    # 与原厂差异必须只落在补丁区
    orig = _unpatch(patched, key)
    diffs = {i for i in range(len(patched)) if patched[i] != orig[i]}
    expected = _expected_diff_offsets(key)
    results.append((diffs == expected,
                    "与原厂差异 %d 字节（应为 %d，全部落在补丁点/注入区；前 12 个 %s）"
                    % (len(diffs), len(expected),
                       [("0x%05X" % d) for d in sorted(diffs)[:12]])))

    # Unicorn 调用级
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

    results.append((True, "Unicorn %s 调用级测试：桩替换 write_reg(0x%08X)/strobe(0x%08X)"
                          "/delay(0x%08X)/radio_wake(0x%08X)/gpio(0x%08X,0x%08X)"
                    % (uc_ver, WRITE_REG, STROBE, DELAY_US, RADIO_WAKE,
                       GPIO_READ, GPIO_WRITE)))
    hi, lo = (key >> 8) & 0xFF, key & 0xFF
    results.append((True, "密钥 KEY=0x%04X（SYNC1=0x%02X SYNC0=0x%02X）；KEY_ON 位于 "
                          "RAM 0x%08X（长按 PA6 ≈1.5s 切换，RAM 版）"
                    % (key, hi, lo, KEY_ON_FLAG_RAM)))

    # 同步字函数：KEY_ON=1 -> KEY（任意 ID）；KEY_ON=0 -> 原厂公式
    for ident in (0, 1, 37, 99):
        results.append(_check_sync_fn(patched, key, ident, 1, patched_mode=True))
    for ident in (0, 1, 37, 99):
        results.append(_check_sync_fn(patched, key, ident, 0, patched_mode=True))
    for ident in (0, 1, 37, 99):
        results.append(_check_sync_fn(orig, key, ident, 0, patched_mode=False))

    # 发送入口：密钥模式双发（第二帧=当前 ID 原厂同步字）/ 普通模式单发 / 原厂自证
    for ident in (99, 5, 0):
        results.append(_check_tx(patched, key, 4, 1, ident, patched_mode=True))
        results.append(_check_tx(patched, key, 2, 1, ident, patched_mode=True))
    for ident in (5, 0):
        results.append(_check_tx(patched, key, 4, 0, ident, patched_mode=True))
        results.append(_check_tx(patched, key, 2, 0, ident, patched_mode=True))
    results.append(_check_tx(orig, key, 4, 0, 99, patched_mode=False))
    results.append(_check_tx(orig, key, 2, 0, 99, patched_mode=False))

    # 按键长按逻辑
    results += _check_button(patched, key)
    return results
