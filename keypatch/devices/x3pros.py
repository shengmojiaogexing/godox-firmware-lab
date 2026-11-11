# -*- coding: utf-8 -*-
"""Godox X3 Pro S（X3 Pro，S 版）引闪器 V1.11 —— 私有密钥「可开关 + 兼容双发」补丁。

参考仓库里 X3-C（`keypatch/devices/x3c.py`、`devices/x3c/`）的做法，为
`X3pro_S_v1.11_app.bin`（1,931,868 B，sha256 `4b3465ca…`）加入同一把 2.4G 私有密钥。

语义（与 X3-C v2「ID 开关」一致，见 keypatch/README.md §2.1）
--------------------------------------------------------------
* **密钥模式 = ID 菜单设为 99**：
    引闪帧用 SYNC = 私有 KEY 发出（引闪已打补丁的密钥灯）；
    紧接着再用 SYNC = floor(50024/(99+1)) = 500 (0x01F4) 发一遍（引闪同频道、
    ID=99 的原厂灯，兼容普通灯）；
    发完把同步字恢复到 KEY，保证异步队列/中断路径发出的帧仍走密钥同步字。
* **普通模式 = ID 0..98**：完全不改动原厂行为（同步字 = 50024/(ID+1)，单发）。

实现（原位字节补丁 + 空闲闪存注入，输出长度不变）
--------------------------------------------------
1) 同步字函数 `0x0802F0C0`（92 B）内 10 字节改写为 `B.W inj_sync` + 3×NOP：
   `inj_sync` 判断入参 ID：ID==99 → r4 = KEY；否则执行原厂 `movw r4,#0xC368;
   add r3,r2; udiv r4,r4,r3`（= floor(50024/(ID+1))），再跳回 `0x0802F0D0`
   继续原厂的 SYNC1/SYNC0 写入 + SCAL/延时序列。
   这样**所有**发送路径（同步直发、中断队列）在密钥模式下都用 KEY 同步字。
2) 两个「低层发送函数」入口 4 字节改为 `B.W inj_tx4 / inj_tx2`：
   * `0x0802EE44` = 发 4 字节帧（0x7F FIFO burst + STX + 600µs + 0x3B/SIDLE）
   * `0x0802EEAC` = 发 2 字节帧（0xD5 xx 短命令 + STX + 500µs + 0x3B/SIDLE）
   `inj_tx*` 在密钥模式下：写 SYNC=KEY → 调蹦床发第一帧 → 写 SYNC=0x01F4 →
   再调蹦床发第二帧 → 写回 SYNC=KEY；普通模式只发一帧（原厂行为）。
   蹦床（trampoline）重建被覆盖的 `push {r4,lr}; bl 0x0802ECE4` 后跳回原函数体
   （`0x0802EE4A` / `0x0802EEB2`），原厂组帧/FIFO/发射时序 100% 保留。
3) 注入代码写在主固件「尾部空闲闪存」`0x081CA000` 起（原厂全 0xFF；已扫描确认
   全镜像无字面量/分支引用该段；热靴子镜像在 0x081F4000 起，避开）。

关键事实（证据见 keypatch/dev/x3pros/NOTES.md）
---------------------------------------------
* MCU：Artery AT32F435/437 类 Cortex-M4F（Thumb-2，含 UDIV/FPU 指令）。
* 镜像基址：`0x08020000`（向量表 SP=0x20270000、Reset=0x0803139D；0x1139C 处
  是标准 Reset_Handler：设 SP → 拷 .data → 清 .bss → SystemInit/main）。
* 「ID → 同步字」函数：`0x0802F0C0`（0x5C 字节，下一个函数 0x0802F11C），
  结构：`movw r4,#0xC368; add r3,r2; udiv r4,r4,r3` → 写 CC2500 reg4/reg5 →
  delay(20) → SCAL(0x33) → 1ms → SIDLE(0x36) → delay(700)。
* 设置结构：RAM `0x20001768`：[+0]=频道，[+1]=ID（set_sync 调用点 0x0802F1B4 /
  0x0802F1C4 均读 `ldrb r0,[r6,#1]` 作为 ID）；ID 菜单最大值 = 99（0x08029A1C
  处 `cmp r3,#0x63; movhi r3,#0x63` 钳位，+1/+10 步进都可到 99）。
* 全镜像只有两处写 CC2500 reg4/reg5：初始化表 `0x0802E9D0/0x0802E9E2`
  （0xC3/0x68，随后被同步字函数覆盖）与本函数 `0x0802F0DE/0x0802F0EE`。
* 辅助函数：写寄存器 `0x0802E79C(reg, buf, len)`（reg 带 0x40 burst 位可连写）、
  strobe `0x0802E90C(cmd)`、延时 `0x0802B678(us)`、射频唤醒/置忙
  `0x0802ECE4()`（置 RAM 0x20001D09 忙标志，中断发送路径检查它）。
* 固件尾部：`0x1D4000` 起是 Sony 热靴子镜像（独立链接在 0x08000000），不动；
  `0x1A9378`–`0x1D4000` 大段 0xFF 是安全注入区。

风险（首次刷入请按安全阶梯）
--------------------------
* 本仓库无 X3 Pro 整机模拟器；本模块只有 Unicorn **调用级**验证（同步字/双发顺序），
  没有整机 boot、没有实机验证 —— 与 V1C 同级别证据。
* 双发会让 2 字节短命令（D5 11/0A/21，原厂每个连发 3 次）每帧多一次 500µs 发射，
  时序会略变长；若实机出现 HSS/连拍异常，优先怀疑这里（可把 DUAL_SEND 关掉只剩
  「密钥模式 = 全 KEY 单发」，见文件末尾 DUAL_SEND 开关）。
* 关中断窗口：注入代码在两次发射之间改同步字，未做 cpsid；射频中断发送路径
  由原厂忙标志接管，窗口约几十~几百 µs，属已知残余风险。
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
    "id": "x3pros",
    "name": "Godox X3 Pro (X3 Pro trigger, S)",
    "image": "devices/x3pros/firmware/X3pro_S_v1.11_app.bin",
    "kind": "trigger",                       # trigger=引闪器 / flash=闪光灯
    "model": "X3 Pro S",
    "firmware_version": "V1.11",
    "base": 0x08020000,
    "sha256": "4b3465cab7975749dd3d109aa1b6f60ee065402869aa88d7b007325def90e103",
    "out_name": "X3pro_S_v1.11_app_key{key:04X}.bin",
    "container_name": "X3pro_S_v1.11_app_key{key:04X}_container.bin",
    "notes": "ID=99 密钥模式：KEY 帧 + ID99(0x01F4) 帧双发；ID 0..98 原厂单发"
             "（同步字函数 10 B 改写 + 两个发送入口 B.W 注入；S 版 V1.11）",
}

# ---------------------------------------------------------------------------
# 镜像 / 地址常量（全部经静态反汇编确认，见 keypatch/dev/x3pro/NOTES.md）
# ---------------------------------------------------------------------------
BASE = 0x08020000
IMAGE_SIZE = 1_931_868
IMAGE_SHA256 = DEVICE["sha256"]

SYNC_FN = 0x0802F0C0                 # 原厂「ID -> CC2500 同步字」函数
SYNC_PATCH_ADDR = 0x0802F0C6         # 函数体里 10 字节：movw+add+udiv
SYNC_ORIG_10 = bytes.fromhex("4cf268341344b4fbf3f4")
SYNC_CONT = 0x0802F0D0               # 改完 r4 后跳回这里继续原厂流程

TX4 = 0x0802EE44                     # 发 4 字节帧（0x7F FIFO burst + STX）
TX4_ORIG = bytes.fromhex("10b5fff7")       # push {r4,lr}; bl 0x0802ECE4 的前 4 字节
TX4_RESUME = 0x0802EE4A              # 原函数体（BL 之后）
TX2 = 0x0802EEAC                     # 发 2 字节帧（0xD5 xx 短命令）
TX2_ORIG = bytes.fromhex("10b5fff7")       # 同上（两个函数入口开头相同）
TX2_RESUME = 0x0802EEB2

WRITE_REG = 0x0802E79C               # write_reg(reg, buf, len)；reg|0x40 = burst
STROBE = 0x0802E90C                  # strobe(cmd)
DELAY_US = 0x0802B678                # delay_us(n)
RADIO_WAKE = 0x0802ECE4              # 置射频忙标志 0x20001D09 并返回

GPIO_WRITE = 0x08032ABC
GPIO_READ = 0x08032AB0

SETTINGS = 0x20001768                # [+0]=channel, [+1]=ID
ID_OFF = 1
ID_KEY_MODE = 99                     # ID=99 -> 密钥模式（与原厂 ID 0..99 复用）
SYNC_NUMERATOR = 50024               # 原厂公式常量 0xC368
ORD_SYNC_ID99 = SYNC_NUMERATOR // (ID_KEY_MODE + 1)   # = 500 = 0x01F4
BUSY_FLAG = 0x20001D09               # 射频忙标志（中断发送路径检查它）
TX_BUF = 0x20001D14                  # 4/2 字节帧缓冲区（发完不清）

# 注入区：主固件尾部空闲闪存 0x081C0000 起（原厂全 0xFF）。
# 复核：0x081A9378..0x081F3FFF 全 0xFF；其中 0x081EC000/0x081EC004/0x081EC0E0、
# 0x081F0000/0x081F0004/0x081F06B0/0x081F3FFD 有代码字面量引用（配置/参数区），
# 本模块只用 0x081C0000 起 0x400 字节，离两端都远。
INJ_BASE = 0x081CA000
INJ_SIZE = 0x400
INJ_END = INJ_BASE + INJ_SIZE
INJ_ORIG = b"\xff" * INJ_SIZE

# 调试开关：True = 密钥模式下「KEY 帧 + ID99 普通帧」双发（兼容原厂灯）；
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

    def cmp_reg(self, rn: int, rm: int) -> None:
        self.emit(_h(0x4280 | ((rm & 7) << 3) | (rn & 7)))

    def ldrb(self, rd: int, rn: int, imm5: int = 0) -> None:
        assert 0 <= rn <= 7 and 0 <= imm5 <= 0x1F
        self.emit(_h(0x7800 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rd & 7)))

    def strb(self, rt: int, rn: int, imm5: int = 0) -> None:
        assert 0 <= rn <= 7 and 0 <= imm5 <= 0x1F
        self.emit(_h(0x7000 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rt & 7)))

    def mov_sp(self, rd: int) -> None:
        """mov rd, sp（T1 高寄存器 MOV，Rd 需 r0-r7，Rm=SP=13）"""
        assert 0 <= rd <= 7
        self.emit(_h(0x4600 | (13 << 3) | (rd & 7)))

    def adds_imm3(self, rd: int, rn: int, imm: int) -> None:
        assert 0 <= imm <= 7
        self.emit(_h(0x1C00 | (imm << 6) | ((rn & 7) << 3) | (rd & 7)))

    def lsrs_imm(self, rd: int, rm: int, imm5: int) -> None:
        assert 1 <= imm5 <= 31
        self.emit(_h(0x0800 | (imm5 << 6) | ((rm & 7) << 3) | (rd & 7)))

    def sub_sp(self, imm: int) -> None:
        # SUB (SP minus immediate) T2：imm 必须是 4 的倍数，编码 = imm/4
        assert 0 <= imm <= 0x1FC and imm % 4 == 0
        self.emit(_h(0xB080 | ((imm >> 2) & 0x7F)))

    def add_sp(self, imm: int) -> None:
        # ADD (SP plus immediate) T2：imm 必须是 4 的倍数，编码 = imm/4
        assert 0 <= imm <= 0x1FC and imm % 4 == 0
        self.emit(_h(0xB000 | ((imm >> 2) & 0x7F)))

    def push(self, low_regs: int, lr: bool = False) -> None:
        self.emit(_h(0xB400 | (0x100 if lr else 0) | (low_regs & 0xFF)))

    def pop(self, low_regs: int, pc: bool = False) -> None:
        self.emit(_h(0xBC00 | (0x100 if pc else 0) | (low_regs & 0xFF)))

    def bx_lr(self) -> None:
        self.emit(_h(0x4770))

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
        # UDIV (T1): 0xFBB0|Rn, 0xF0F0|(Rd<<8)|Rm（与原厂 0x0802F0CC 一致）
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

    def bne(self, target) -> None:
        off = len(self.buf)
        self.emit(b"\x00\x00")
        self.fix.append(("bne", off, target))

    def nop(self) -> None:
        self.emit(b"\x00\xbf")

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
            if kind in _COND:
                imm = tgt - (addr + 4)
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
        inj_sync       同步字函数内 10 字节改写后的分流（ID==99 -> KEY）
        inj_tx4        4 字节帧发送入口包装（双发）
        inj_tx2        2 字节帧发送入口包装（双发）
        set_sync       (r0 = 16 位同步字) 写 CC2500 SYNC1/SYNC0（burst reg 0x44）
        set_key        r0 = KEY
        set_ord        r0 = 0x01F4（ID=99 的原厂同步字）
        tramp4/tramp2  蹦床：重建原厂入口被覆盖的 push + bl，跳回函数体
    """
    a = _Asm(INJ_BASE)

    # ---- 1) 同步字函数分流 -------------------------------------------------
    a.label("inj_sync")
    a.cmp_imm(0, ID_KEY_MODE)
    a.bne("sync_orig")
    a.movw(4, key & 0xFFFF)
    a.b_w(SYNC_CONT)
    a.label("sync_orig")
    a.emit(SYNC_ORIG_10)            # movw r4,#0xC368; add r3,r2; udiv r4,r4,r3
    a.b_w(SYNC_CONT)

    # ---- 2) 两个发送入口包装 ----------------------------------------------
    # 注意：同步字必须在原厂 `bl 0x0802ECE4`（射频唤醒/置忙）之后、写 FIFO 之前写入，
    # 所以每个「蹦床」都重建 push + bl 唤醒，再写同步字，最后跳回原函数体。
    def _tx_wrapper(label: str, t_key: str, t_ord: str, t_orig: str) -> None:
        a.label(label)
        a.push(0x10, lr=True)                       # push {r4,lr}
        a.movw(3, SETTINGS & 0xFFFF)
        a.movt(3, (SETTINGS >> 16) & 0xFFFF)
        a.ldrb(3, 3, ID_OFF)                        # r3 = 当前 ID
        a.cmp_imm(3, ID_KEY_MODE)
        a.bne(label + "_single")
        # --- 密钥模式：KEY 帧 + (可选) ID99 普通帧 ---
        a.bl(t_key)
        if DUAL_SEND:
            a.bl(t_ord)
            a.bl("set_key")                         # 恢复 KEY，队列/中断发帧也走密钥
        a.pop(0x10, pc=True)
        a.label(label + "_single")
        a.bl(t_orig)
        a.pop(0x10, pc=True)

    _tx_wrapper("inj_tx4", "tramp4_key", "tramp4_ord", "tramp4_orig")
    _tx_wrapper("inj_tx2", "tramp2_key", "tramp2_ord", "tramp2_orig")

    # ---- 3) 写同步字辅助函数 ----------------------------------------------
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

    a.label("set_ord")
    a.movw(0, ORD_SYNC_ID99 & 0xFFFF)               # 0x01F4 = 50024/100
    a.b_w("set_sync")

    # ---- 4) 蹦床：重建原函数入口被覆盖的 4 字节，跳回原函数体 -------------
    # tramp*_key / tramp*_ord：唤醒（置忙）后写同步字再发；tramp*_orig：纯原厂单发。
    def _tramp(label: str, resume: int, sync_word: int | None) -> None:
        a.label(label)
        a.push(0x10, lr=True)                       # 原 push {r4,lr}
        a.bl(RADIO_WAKE)                            # 原 bl 0x0802ECE4（唤醒/置忙）
        if sync_word is not None:
            a.movw(0, sync_word & 0xFFFF)           # 射频已唤醒，写同步字才有效
            a.bl("set_sync")
        a.b_w(resume)                               # 原函数体（BL 之后）

    _tramp("tramp4_key", TX4_RESUME, key)
    _tramp("tramp4_ord", TX4_RESUME, ORD_SYNC_ID99)
    _tramp("tramp4_orig", TX4_RESUME, None)
    _tramp("tramp2_key", TX2_RESUME, key)
    _tramp("tramp2_ord", TX2_RESUME, ORD_SYNC_ID99)
    _tramp("tramp2_orig", TX2_RESUME, None)

    code = a.build()
    if len(code) > INJ_SIZE:
        raise ValueError("注入代码 %d 字节超出预留空闲区 %d 字节" % (len(code), INJ_SIZE))
    layout = {
        "inj_sync": a.labels["inj_sync"],
        "inj_tx4": a.labels["inj_tx4"],
        "inj_tx2": a.labels["inj_tx2"],
        "set_sync": a.labels["set_sync"],
        "set_key": a.labels["set_key"],
        "set_ord": a.labels["set_ord"],
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

    return [
        (TX4, TX4_ORIG, b1.build(),
         "4 字节帧发送入口 -> inj_tx4（密钥模式 KEY+ID99 双发）"),
        (TX2, TX2_ORIG, b2.build(),
         "2 字节帧发送入口 -> inj_tx2（密钥模式 KEY+ID99 双发）"),
        (SYNC_PATCH_ADDR, SYNC_ORIG_10, b3.build(),
         "同步字函数 10 B 改写：ID=99 -> SYNC=KEY，其余原厂 50024/(ID+1)"),
        (INJ_BASE, inj_before, inj_code,
         "注入区（双发/同步字辅助 + 蹦床，%d 字节，原厂 0xFF）" % len(inj_code)),
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
UC_RAM_SIZE = 0x00008000
UC_SENTINEL = 0x20006000
UC_STACK = 0x20007F00


def _unpatch(patched: bytes, key: int) -> bytes:
    """把补丁区还原成原厂镜像（用于原厂自证）。"""
    buf = bytearray(patched)
    for addr, before, after, _desc in _patched_regions(key):
        o = _off(addr)
        buf[o:o + len(before)] = before
    return bytes(buf)


def _uc_run(image: bytes, entry: int, ident: int, fifo: bytes = b"\x11\x22\x33\x44",
            max_insn: int = 200000) -> dict:
    """Unicorn 调用级执行：桩替换 radio 辅助函数，记录 SYNC/FIFO 写入序列。"""
    from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                         UC_HOOK_CODE)
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_SP,
                                   UC_ARM_REG_CPSR)

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_FLASH, UC_FLASH_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))       # b . 哨兵
    uc.mem_write(SETTINGS, bytes([1, ident & 0xFF, 2, 3]))
    uc.mem_write(TX_BUF, fifo)
    uc.mem_write(BUSY_FLAG, b"\x00")

    seq: list[tuple] = []
    state = {"error": None}

    def hook(_uc, address, _size, _user):
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
        elif address == DELAY_US:
            pass
        elif address in (RADIO_WAKE, GPIO_WRITE, GPIO_READ):
            pass
        else:
            return
        _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_R0, ident)                     # 部分入口的 r0 入参
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)              # Thumb
    try:
        uc.emu_start(entry | 1, 0, count=max_insn)
    except UcError as exc:  # noqa: BLE001
        state["error"] = "%s (pc=0x%08X lr=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR))
    return {"seq": seq, "error": state["error"]}


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
    """从调用序列里取同步字：burst 写（reg 0x44）或原厂两次单字节写（reg4/reg5）。

    返回 (sync_word, 剩余序列)；同步字写被剥离后，剩余序列用于确认 SCAL/延时序列。
    """
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


def _check_tx4(patched: bytes, key: int, ident: int, patched_mode: bool) -> tuple[bool, str]:
    image = patched if patched_mode else _unpatch(patched, key)
    res = _uc_run(image, TX4, ident)
    label = "补丁版" if patched_mode else "原厂(自证)"
    if res["error"]:
        return False, "TX4(4B帧) ID=%d %s 执行失败: %s" % (ident, label, res["error"])
    seq = res["seq"]
    body = [("fifo", 4), ("strobe", 0x35), ("strobe", 0x3B), ("strobe", 0x36)]
    if not patched_mode:
        ok = seq == body
        return ok, "TX4(4B帧) ID=%-2d %s：单发 FIFO(4)（原厂行为）" % (ident, label)
    if ident == ID_KEY_MODE:
        want = [("sync", key)] + body
        if DUAL_SEND:
            want += [("sync", ORD_SYNC_ID99)] + body + [("sync", key)]
        ok = seq == want
        return ok, ("TX4(4B帧) ID=%d %s(密钥模式)：%s | %s"
                    % (ident, label,
                       "SYNC=0x%04X -> FIFO(4) -> SYNC=0x%04X -> FIFO(4) -> SYNC=0x%04X"
                       % (key, ORD_SYNC_ID99, key) if DUAL_SEND
                       else "SYNC=0x%04X -> FIFO(4)（单发，DUAL_SEND=False）" % key,
                       _fmt(seq)))
    ok = seq == body
    return ok, ("TX4(4B帧) ID=%-2d %s(普通模式)：单发 FIFO(4) | %s"
                % (ident, label, _fmt(seq)))


def _check_tx2(patched: bytes, key: int, ident: int, patched_mode: bool) -> tuple[bool, str]:
    image = patched if patched_mode else _unpatch(patched, key)
    res = _uc_run(image, TX2, ident, fifo=b"\xd5\x21\x00\x00")
    label = "补丁版" if patched_mode else "原厂(自证)"
    if res["error"]:
        return False, "TX2(2B帧) ID=%d %s 执行失败: %s" % (ident, label, res["error"])
    seq = res["seq"]
    body = [("fifo", 2), ("strobe", 0x35), ("strobe", 0x3B), ("strobe", 0x36)]
    if not patched_mode:
        ok = seq == body
        return ok, "TX2(2B短命令) ID=%-2d %s：单发 FIFO(2)（原厂行为）" % (ident, label)
    if ident == ID_KEY_MODE:
        want = [("sync", key)] + body
        if DUAL_SEND:
            want += [("sync", ORD_SYNC_ID99)] + body + [("sync", key)]
        ok = seq == want
        return ok, ("TX2(2B短命令) ID=%d %s(密钥模式)：%s | %s"
                    % (ident, label,
                       "SYNC=0x%04X -> FIFO(2) -> SYNC=0x%04X -> FIFO(2) -> SYNC=0x%04X"
                       % (key, ORD_SYNC_ID99, key) if DUAL_SEND
                       else "SYNC=0x%04X -> FIFO(2)（单发，DUAL_SEND=False）" % key,
                       _fmt(seq)))
    ok = seq == body
    return ok, ("TX2(2B短命令) ID=%-2d %s(普通模式)：单发 FIFO(2) | %s"
                % (ident, label, _fmt(seq)))


def _sync_from_seq(seq: list[tuple]) -> int | None:
    vals = [item[1] for item in seq if item[0] == "sync"]
    return vals[0] if len(vals) == 1 else None


def _check_sync_fn(image: bytes, key: int, ident: int, patched_mode: bool) -> tuple[bool, str]:
    res = _uc_run(image, SYNC_FN, ident)
    label = "补丁版" if patched_mode else "原厂(自证)"
    if res["error"]:
        return False, "set_sync(ID=%d) %s 执行失败: %s" % (ident, label, res["error"])
    want_key = patched_mode and ident == ID_KEY_MODE
    want = key if want_key else SYNC_NUMERATOR // (ident + 1)
    got, rest = _sync_word_from_seq(res["seq"])
    # 原厂流程：写 SYNC1/0 -> delay(20) -> SCAL(0x33) -> 1ms -> SIDLE(0x36) -> delay(700)
    strobes = [item[1] for item in rest if item[0] == "strobe"]
    ok = (got == want) and strobes == [0x33, 0x36]
    mode = "密钥模式" if want_key else "原厂模式"
    return ok, ("set_sync(%s ID=%-2d, %s): SYNC=0x%04X（期望 0x%04X），"
                "SCAL/SIDLE=%s | %s"
                % (label, ident, mode, got if got is not None else -1, want,
                   [hex(s) for s in strobes], _fmt(res["seq"])))


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证：长度 / 静态字节 / 差异范围 / Unicorn 调用级（同步字 + 双发）。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁+注入）"
                    % (len(patched), IMAGE_SIZE)))

    inj_code, layout = build_injection(key)
    got = patched[_off(INJ_BASE):_off(INJ_BASE) + len(inj_code)]
    results.append((got == inj_code,
                    "注入区 0x%08X 起 %d 字节（双发包装 + 同步字分流，共 %d/%d 字节）"
                    % (INJ_BASE, len(inj_code), len(inj_code), INJ_SIZE)))
    rest = patched[_off(INJ_BASE) + len(inj_code):_off(INJ_END)]
    results.append((rest == INJ_ORIG[len(inj_code):],
                    "注入区剩余 %d 字节保持擦除态 0xFF（不越界改写）" % len(rest)))

    for addr, before, after, desc in _patched_regions(key):
        if addr == INJ_BASE:
            continue
        got = patched[_off(addr):_off(addr) + len(after)]
        results.append((got == after, "入口 0x%08X = %s（%s）"
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
                          "/delay(0x%08X)/radio_wake(0x%08X)"
                    % (uc_ver, WRITE_REG, STROBE, DELAY_US, RADIO_WAKE)))
    hi, lo = (key >> 8) & 0xFF, key & 0xFF
    results.append((True, "密钥 KEY=0x%04X（SYNC1=0x%02X SYNC0=0x%02X）；密钥模式第二帧"
                          "同步字 = floor(50024/100) = 0x%04X"
                    % (key, hi, lo, ORD_SYNC_ID99)))

    # 同步字函数：补丁版 ID=99 -> KEY；其它 ID 与原厂逐位一致
    ids = (0, 1, 37, 99)
    for ident in ids:
        results.append(_check_sync_fn(patched, key, ident, patched_mode=True))
    orig_img = _unpatch(patched, key)
    for ident in (0, 1, 37, 99):
        results.append(_check_sync_fn(orig_img, key, ident, patched_mode=False))

    # 发送入口：密钥模式双发 / 普通模式单发 / 原厂单发自证
    for ident in (ID_KEY_MODE, 5, 0):
        results.append(_check_tx4(patched, key, ident, patched_mode=True))
        results.append(_check_tx2(patched, key, ident, patched_mode=True))
    results.append(_check_tx4(orig_img, key, ID_KEY_MODE, patched_mode=False))
    results.append(_check_tx2(orig_img, key, ID_KEY_MODE, patched_mode=False))
    return results
