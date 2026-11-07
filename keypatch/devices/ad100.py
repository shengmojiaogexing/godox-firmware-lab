# -*- coding: utf-8 -*-
"""Godox AD100 Pro V1.2 私有密钥（CC2500 16 位硬件同步字）补丁模块 —— v4「C.Fn 开关」版。

用户可见语义（v4，取代 v2 的 "ID==0 当开关"）
--------------------------------------------
灯端 C.Fn 菜单新增独立「密钥开关」项，标志字节为 RAM **0x20000486**
（= 设置区 RAM_20000480[6]，C.Fn 预留项 6；0=普通、1=密钥）。该字节随
flash 参数记录（0x0801F000 + slot*100，记录内偏移 30）自动加载/保存，
校验表 0x0800DA2E[6]=1（允许 0/1）。

把原厂「ID -> 16 位同步字」函数 0x0800B758 整段改写成：

    flag = *(uint8_t *)0x20000486;
    if (flag) sync = KEY;                 // 密钥模式
    else      sync = 50024 / (id + 1);    // 原厂普通模式
    WriteReg(4, sync >> 8);               // SYNC1
    WriteReg(5, sync & 0xFF);             // SYNC0
    return;                               // 只改同步字：无 SCAL、无延时

实现：原位字节补丁，0x0800B758 起 56 字节（到下一函数 0x0800B790 为止），
输出长度不变。函数体 44 字节 + 3 个 32 位字面量（KEY、0x20000486、50024）。

补丁点复核（见 ../dev/ad100/NOTES.md）
-------------------------------------
* 0x0800B758 函数占 0x0800B758..0x0800B78F（52 字节代码 + 0x0800B78C 处 4 字节
  字面量 0x0000C368），下一函数 0x0800B790。
* 全镜像只有该函数与 CC2500_Init(0x0800CAE8) 写 SYNC1/SYNC0；CC2500_Init 的静态
  初值 0xC3/0x68 在 RF_Init(0x08004AF0) 中随后被本函数覆盖。
* 0x0800B758 的 4 个调用点（RF 初始化 + 三个改 ID 路径）都走本函数。
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
    "id": "ad100",
    "name": "Godox AD100 Pro",
    "image": "devices/ad100pro/firmware/AD100pro_V1.2.bin",
    "kind": "flash",                     # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "AD100 Pro",
    "firmware_version": "V1.2",
    "base": 0x08003000,
    "sha256": "3a3d3b917615bb20339f6c355ebe66d8d94e8199509c1e820007542c1ebf4e7c",
    "out_name": "AD100pro_V1.2_key{key:04X}.bin",
    "container_name": "AD100Pro_V1.2_key{key:04X}_container.bin",
    "notes": "C.Fn 密钥开关(RAM 0x20000486)：1 -> SYNC=私有 KEY；0 -> 原厂 50024/(ID+1)。"
             "原位 56 字节整函数改写",
}

# ---- 目标函数与地址 ---------------------------------------------------------
BASE = 0x08003000
IMAGE_SIZE = 50248

SYNC_FN = 0x0800B758          # void rf_set_id(uint8_t id)：写 CC2500 SYNC1/SYNC0
SYNC_FN_END = 0x0800B790      # 下一函数起点；本补丁必须止步于此
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN   # 56 字节
assert SYNC_FN_SIZE == 56

# v4：密钥开关标志字节（C.Fn 预留项 6，RAM_20000480[6]，flash 记录偏移 30）
KEY_FLAG_ADDR = 0x20000486
# 校验表 0x0800DA2E 第 6 项 = 1（允许 0/1），由 re_check.py 静态核对

# 原厂 0x0800B758 起 56 字节（用于严格校验固件版本/补丁点）
ORIG_FUNC = bytes.fromhex(
    "0c4a10b5411c1046f7f7c6fc0446010a042001f0e2fae1b2052001f0defa"
    "142001f09ef9332001f039fb7d20c00001f097f910bd68c30000")

DIV_FUNC = 0x080030F0         # __aeabi_uidiv
WRITE_REG = 0x0800CD32        # CC2500_WriteReg(reg=r0, val=r1)
WRITE_CMD = 0x0800CDF4        # CC2500_WriteCmd(cmd=r0)   （v4 不调用）
DELAY_US = 0x0800CAB8         # delay_us(n=r0)            （v4 不调用）

# Unicorn 测试用内存布局
UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000         # 128 KiB flash（app 位于 0x08003000）
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x4000          # RAM 8 KiB，多映射一点给栈
UC_STACK = 0x20001800         # 栈顶（8 字节对齐）
UC_SENTINEL = 0x20000400      # 函数返回落点，放 `b .`
MAX_HARNESS_INSNS = 20000     # Unicorn 单次调用指令上限（正常约 250 条）


# ---------------------------------------------------------------------------
# Thumb-1 指令编码（仅实现本补丁用到的指令；语义用 capstone/Unicorn 复核）
# ---------------------------------------------------------------------------
def _t_ldr_literal(rt: int, imm8: int) -> bytes:
    """LDR (literal) T1: 01001 Rt imm8；目标 = (PC & ~3) + imm8*4。"""
    return struct.pack("<H", 0x4800 | ((rt & 7) << 8) | (imm8 & 0xFF))


def _t_ldrb_imm(rt: int, rn: int, imm5: int = 0) -> bytes:
    """LDRB (immediate) T1: 01111 imm5 Rn Rt。"""
    return struct.pack("<H", 0x7800 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rt & 7))


def _t_push_r4lr() -> bytes:
    return struct.pack("<H", 0xB510)


def _t_pop_r4pc() -> bytes:
    return struct.pack("<H", 0xBD10)


def _t_cmp_imm(rn: int, imm8: int) -> bytes:
    """CMP (immediate) T1: 00101 Rn imm8。"""
    return struct.pack("<H", 0x2800 | ((rn & 7) << 8) | (imm8 & 0xFF))


def _t_beq(offset_bytes: int) -> bytes:
    """条件分支 T1（BEQ）：offset 相对 (本条地址+4)。"""
    assert offset_bytes % 2 == 0 and 0 <= offset_bytes // 2 <= 0xFF
    return struct.pack("<H", 0xD000 | ((offset_bytes // 2) & 0xFF))


def _t_b(offset_bytes: int) -> bytes:
    """无条件分支 T2：offset 相对 (本条地址+4)。"""
    assert offset_bytes % 2 == 0 and -0x800 <= offset_bytes // 2 <= 0x7FF
    return struct.pack("<H", 0xE000 | ((offset_bytes // 2) & 0x7FF))


def _t_adds_rn_imm3(rn: int, rd: int, imm3: int) -> bytes:
    """ADDS (imm3) T1: 0001110 imm3 Rn Rd。"""
    return struct.pack("<H", 0x1C00 | ((imm3 & 7) << 6) | ((rn & 7) << 3) | (rd & 7))


def _t_movs_imm(rd: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x2000 | ((rd & 7) << 8) | (imm8 & 0xFF))


def _t_mov(rd: int, rm: int) -> bytes:
    """MOV (register) T1: 0100 0110 D Rm Rd。"""
    return struct.pack("<H", 0x4600 | ((rm & 7) << 3) | (rd & 7))


def _t_lsrs(rd: int, rm: int, imm5: int) -> bytes:
    """LSRS (immediate) T1: 00001 imm5 Rm Rd。"""
    return struct.pack("<H", 0x0800 | ((imm5 & 0x1F) << 6) | ((rm & 7) << 3) | (rd & 7))


def _t_uxtb(rd: int, rm: int) -> bytes:
    """UXTB T1: 1011 0010 11 Rm Rd。"""
    return struct.pack("<H", 0xB2C0 | ((rm & 7) << 3) | (rd & 7))


def _t_bl(addr: int, target: int) -> bytes:
    """Thumb BL T1 编码（addr = 本 BL 指令地址，target = 绝对地址）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (addr, target, off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    # 解码关系: I1 = NOT(J1 XOR S) -> J1 = NOT(I1) XOR S
    j1 = (i1 if s else ((~i1) & 1))
    j2 = (i2 if s else ((~i2) & 1))
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    hw1 = 0xF000 | (s << 10) | imm10
    hw2 = 0xD000 | (j1 << 13) | (j2 << 11) | imm11
    return struct.pack("<HH", hw1, hw2)


class _Asm:
    """极小的 Thumb 汇编器：支持标签/前向修正（参考 lead 已验证的原型）。"""

    def __init__(self, base: int):
        self.base = base
        self.code = bytearray()
        self.labels: dict[str, int] = {}
        self.fixups: list[tuple[int, str, str]] = []

    def here(self) -> int:
        return self.base + len(self.code)

    def label(self, name: str) -> None:
        self.labels[name] = self.here()

    def emit(self, b: bytes) -> None:
        self.code += b

    def branch(self, kind: str, label: str) -> None:
        self.fixups.append((len(self.code), kind, label))
        self.emit(b"\x00\x00")

    def ldr_literal(self, rt: int, target: int) -> None:
        here = self.here()
        pc = (here + 4) & ~3
        imm8 = (target - pc) // 4
        assert (target - pc) % 4 == 0 and 0 <= imm8 <= 0xFF, (target, pc)
        self.emit(_t_ldr_literal(rt, imm8))

    def bl(self, target: int) -> None:
        self.emit(_t_bl(self.here(), target))

    def finish(self) -> bytes:
        out = bytearray(self.code)
        for off, kind, label in self.fixups:
            target = self.labels[label]
            delta = target - (self.base + off + 4)
            if kind == "beq":
                out[off:off + 2] = _t_beq(delta)
            elif kind == "b":
                out[off:off + 2] = _t_b(delta)
            else:
                raise AssertionError("unsupported branch kind %r" % kind)
        return bytes(out)


def build_sync_function(key: int) -> bytes:
    """生成 56 字节替换函数体：44 字节代码 + KEY/FLAG/50024 三个字面量。"""
    key = common.validate_key(key)
    key_lit = SYNC_FN + 44
    flag_lit = SYNC_FN + 48
    orig_lit = SYNC_FN + 52        # 50024 保持原厂字面量地址 0x0800B78C
    assert orig_lit + 4 == SYNC_FN_END

    a = _Asm(SYNC_FN)
    a.emit(_t_push_r4lr())               # +00 push {r4, lr}
    a.ldr_literal(3, flag_lit)           # +02 ldr r3, =0x20000486
    a.ldr_literal(2, orig_lit)           # +04 ldr r2, =50024（普通模式默认被除数）
    a.emit(_t_ldrb_imm(3, 3))            # +06 ldrb r3, [r3]   ; flag
    a.emit(_t_adds_rn_imm3(0, 1, 1))     # +08 adds r1, r0, #1 ; 普通模式除数 ID+1
    a.emit(_t_cmp_imm(3, 0))             # +0A cmp r3, #0
    a.branch("beq", "go")                # +0C beq go          ; flag==0 -> 原厂公式
    a.ldr_literal(2, key_lit)            # +0E ldr r2, =KEY    ; 密钥模式被除数
    a.emit(_t_movs_imm(1, 1))            # +10 movs r1, #1     ; 除数=1
    a.label("go")                        # +12
    a.emit(_t_mov(0, 2))                 # +12 mov r0, r2
    a.bl(DIV_FUNC)                       # +14 bl __aeabi_uidiv
    a.emit(_t_mov(4, 0))                 # +18 mov r4, r0
    a.emit(_t_lsrs(1, 0, 8))             # +1A lsrs r1, r0, #8
    a.emit(_t_movs_imm(0, 4))            # +1C movs r0, #4     ; SYNC1
    a.bl(WRITE_REG)                      # +1E bl CC2500_WriteReg
    a.emit(_t_uxtb(1, 4))                # +22 uxtb r1, r4
    a.emit(_t_movs_imm(0, 5))            # +24 movs r0, #5     ; SYNC0
    a.bl(WRITE_REG)                      # +26 bl CC2500_WriteReg
    a.emit(_t_pop_r4pc())                # +2A pop {r4, pc}
    code = a.finish()
    assert len(code) == 44, len(code)
    lits = struct.pack("<III", key, KEY_FLAG_ADDR, common.ID_NUMERATOR)
    blob = code + lits
    assert len(blob) == SYNC_FN_SIZE
    return blob


def build_sync_function_id0(key: int) -> bytes:
    """v2 语义（临时可切换版）：ID==0 -> SYNC=KEY；ID!=0 -> 50024/(ID+1)。

    与 v4 同一 56 字节区域、同样只写 SYNC1/0（无 SCAL、无延时），区别仅判断条件：
    比较 r0(ID)==0，而不是读 RAM[0x20000486]。
    布局：42 字节代码 + 2 字节 nop 对齐 + KEY(+0x2C) + 4 字节 nop + 50024(+0x34)。
    """
    key = common.validate_key(key)
    key_lit = SYNC_FN + 44
    orig_lit = SYNC_FN + 52        # 50024 保持原厂字面量地址 0x0800B78C
    assert orig_lit + 4 == SYNC_FN_END

    a = _Asm(SYNC_FN)
    a.ldr_literal(2, key_lit)            # +00 ldr r2, =KEY
    a.emit(_t_push_r4lr())               # +02 push {r4, lr}
    a.emit(_t_cmp_imm(0, 0))             # +04 cmp r0, #0      ; id == 0 ?
    a.branch("beq", "keymode")           # +06 beq keymode
    a.ldr_literal(2, orig_lit)           # +08 ldr r2, =50024
    a.emit(_t_adds_rn_imm3(0, 1, 1))     # +0A adds r1, r0, #1
    a.branch("b", "go")                  # +0C b go
    a.label("keymode")                   # +0E
    a.emit(_t_movs_imm(1, 1))            # +0E movs r1, #1     ; 密钥模式除数=1
    a.label("go")                        # +10
    a.emit(_t_mov(0, 2))                 # +10 mov r0, r2
    a.bl(DIV_FUNC)                       # +12 bl __aeabi_uidiv
    a.emit(_t_mov(4, 0))                 # +16 mov r4, r0
    a.emit(_t_lsrs(1, 0, 8))             # +18 lsrs r1, r0, #8
    a.emit(_t_movs_imm(0, 4))            # +1A movs r0, #4     ; SYNC1
    a.bl(WRITE_REG)                      # +1C bl CC2500_WriteReg
    a.emit(_t_uxtb(1, 4))                # +20 uxtb r1, r4
    a.emit(_t_movs_imm(0, 5))            # +22 movs r0, #5     ; SYNC0
    a.bl(WRITE_REG)                      # +24 bl CC2500_WriteReg
    a.emit(_t_pop_r4pc())                # +28 pop {r4, pc}
    code = a.finish()
    assert len(code) == 42, len(code)
    blob = (code + b"\x00\xbf"            # +2A 对齐填充
            + struct.pack("<I", key)      # +2C KEY
            + b"\x00\xbf\x00\xbf"         # +30 填充
            + struct.pack("<I", common.ID_NUMERATOR))  # +34 50024（原厂字面量地址）
    assert len(blob) == SYNC_FN_SIZE
    return blob


# ---------------------------------------------------------------------------
# 打补丁
# ---------------------------------------------------------------------------
def _off(addr: int) -> int:
    return addr - BASE


def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """把原厂镜像 data 打上 v4「C.Fn 开关密钥」补丁，返回 (patched_bytes, patch_log)。"""
    key = common.validate_key(key)

    if len(data) != IMAGE_SIZE:
        raise ValueError("AD100 Pro V1.2 固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
                         % (IMAGE_SIZE, len(data)))

    off = _off(SYNC_FN)
    before = bytes(data[off:off + SYNC_FN_SIZE])
    if before != ORIG_FUNC:
        raise ValueError(
            "补丁点 0x%08X..0x%08X 原始字节与预期不符（固件版本不符，拒绝打补丁）\n"
            "  期望 %s\n  实际 %s"
            % (SYNC_FN, SYNC_FN_END - 1, ORIG_FUNC.hex(" "), before.hex(" ")))

    blob = build_sync_function(key)
    assert len(blob) == SYNC_FN_SIZE
    assert SYNC_FN + len(blob) == SYNC_FN_END  # 不越过下一函数

    buf = bytearray(data)
    buf[off:off + SYNC_FN_SIZE] = blob
    patched = bytes(buf)
    assert len(patched) == len(data)

    log = [{
        "addr": SYNC_FN,
        "offset": off,
        "before": before.hex(" "),
        "after": blob.hex(" "),
        "desc": ("整段改写同步字函数 0x%08X..0x%08X（%d 字节）：C.Fn 密钥开关"
                 " RAM[0x%08X]==1 -> SYNC=KEY 0x%04X；==0 -> 50024/(ID+1)；"
                 "只写 SYNC1/0（无 SCAL、无延时）"
                 % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE, KEY_FLAG_ADDR, key)),
    }]
    return patched, log


def apply_id0(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """临时可切换版：ID==0 -> SYNC=KEY，ID!=0 -> 原厂公式（零 UI 改动，ID 菜单当开关）。

    与 apply() 同一补丁位置/长度，只是语义从「读 C.Fn 标志」改为「判断 ID==0」。
    输出文件名由调用方决定（keypatch.py 默认仍产出 v4 的 out_name，本函数供
    dev/ad100/make_id0.py 生成 AD100pro_V1.2_key{key:04X}_id0.bin）。
    """
    key = common.validate_key(key)
    if len(data) != IMAGE_SIZE:
        raise ValueError("AD100 Pro V1.2 固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
                         % (IMAGE_SIZE, len(data)))
    off = _off(SYNC_FN)
    before = bytes(data[off:off + SYNC_FN_SIZE])
    if before != ORIG_FUNC:
        raise ValueError(
            "补丁点 0x%08X..0x%08X 原始字节与预期不符（固件版本不符，拒绝打补丁）\n"
            "  期望 %s\n  实际 %s"
            % (SYNC_FN, SYNC_FN_END - 1, ORIG_FUNC.hex(" "), before.hex(" ")))
    blob = build_sync_function_id0(key)
    assert len(blob) == SYNC_FN_SIZE and SYNC_FN + len(blob) == SYNC_FN_END
    buf = bytearray(data)
    buf[off:off + SYNC_FN_SIZE] = blob
    patched = bytes(buf)
    assert len(patched) == len(data)
    log = [{
        "addr": SYNC_FN,
        "offset": off,
        "before": before.hex(" "),
        "after": blob.hex(" "),
        "desc": ("临时可切换版（零 UI）：ID==0 -> SYNC=KEY 0x%04X，ID!=0 -> 50024/(ID+1)；"
                 "只写 SYNC1/0（无 SCAL、无延时）" % key),
    }]
    return patched, log


# ---------------------------------------------------------------------------
# 离线验证
# ---------------------------------------------------------------------------
def _expected_diff_offsets(key: int) -> set[int]:
    """打补丁后相对原厂镜像应当且只应当变化的文件偏移。"""
    blob = build_sync_function(key)
    base = _off(SYNC_FN)
    return {base + i for i in range(SYNC_FN_SIZE) if blob[i] != ORIG_FUNC[i]}


def _unpatch(patched: bytes) -> bytes:
    """把补丁区还原成原厂 56 字节（用于原厂 harness 自证，无需再读文件）。"""
    buf = bytearray(patched)
    off = _off(SYNC_FN)
    buf[off:off + SYNC_FN_SIZE] = ORIG_FUNC
    return bytes(buf)


def _uc_run_sync_fn(image: bytes, ident: int, flag: int) -> dict:
    """用 Unicorn 执行 image 里 0x0800B758 的同步字设置函数。

    先把 RAM[KEY_FLAG_ADDR] 置为 flag（0/1），再把 WriteReg(0x0800CD32) /
    WriteCmd(0x0800CDF4) / delay_us(0x0800CAB8) 入口替换成 `bx lr` 返回桩，
    用代码钩子记录调用参数。
    返回 {"calls": [(kind, a, b), ...], "error": str|None, "insn": int}
    """
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC)

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE, UC_PROT_ALL)
    uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    uc.mem_write(KEY_FLAG_ADDR, bytes([flag & 0xFF]))   # C.Fn 密钥开关标志
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))    # b .
    for addr in (WRITE_REG, WRITE_CMD, DELAY_US):
        uc.mem_write(addr, bytes.fromhex("7047"))        # bx lr

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

    # hook 必须覆盖 RAM 里的哨兵地址（0x20000400），故不传 begin/end（全地址挂钩）
    uc.hook_add(UC_HOOK_CODE, hook_code)

    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)   # 返回落点（Thumb）
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    uc.reg_write(UC_ARM_REG_R1, 0x1234)
    uc.reg_write(UC_ARM_REG_R2, 0x5678)
    uc.reg_write(UC_ARM_REG_R3, 0x9ABC)

    try:
        # unicorn 2.1.4 在 MCLASS 下 emu_start(count=0) 实测会卡住，显式给指令上限
        uc.emu_start(SYNC_FN | 1, 0, 0, MAX_HARNESS_INSNS)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X, lr=0x%08X, sp=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR),
            uc.reg_read(UC_ARM_REG_SP))
    if state["error"] is None and not state["returned"]:
        state["error"] = ("未在 %d 条指令内返回（执行 %d 条，pc=0x%08X），疑似跑飞"
                          % (MAX_HARNESS_INSNS, state["insn"], uc.reg_read(UC_ARM_REG_PC)))
    return {"calls": calls, "error": state["error"], "insn": state["insn"]}


def _format_calls(calls: list[tuple]) -> str:
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
    """v4 语义：flag==1 -> KEY；flag==0 -> 50024/(ID+1)；只写 SYNC1/0、无 SCAL/延时。"""
    res = _uc_run_sync_fn(image, ident, flag)
    if res["error"]:
        return False, "ID=%d flag=%d 补丁版 Unicorn 执行失败: %s" % (ident, flag, res["error"])
    calls = res["calls"]
    expect = key if flag else common.ID_NUMERATOR // (ident + 1)
    hi, lo = expect >> 8, expect & 0xFF
    want = [("wreg", 4, hi), ("wreg", 5, lo)]
    ok = calls == want
    mode = "密钥模式(KEY)" if flag else "原厂公式"
    msg = ("ID=%-2d flag=%d %s: sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X（仅 2 次写寄存器）| %s"
           % (ident, flag, mode, expect, hi, lo, _format_calls(calls)))
    if not ok:
        msg += "  期望调用序列 %s" % _format_calls(want)
    # 密钥模式必须与原厂值不同（隔离性）；同时证明同一 ID 下 flag 真正切换了同步字
    if flag:
        public_val = common.ID_NUMERATOR // (ident + 1)
        ok = ok and expect != public_val
    return ok, msg


def _check_original(image: bytes, ident: int) -> tuple[bool, str]:
    """原厂 harness 自证：应得到 50024/(ID+1)，且包含 SCAL(0x33)+delay(1000)。"""
    res = _uc_run_sync_fn(image, ident, 0)
    if res["error"]:
        return False, "ID=%d 原厂自证 Unicorn 执行失败: %s" % (ident, res["error"])
    calls = res["calls"]
    expect = common.ID_NUMERATOR // (ident + 1)
    want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF),
            ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
    ok = calls == want
    msg = ("ID=%-2d 原厂(自证): sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
           % (ident, expect, expect >> 8, expect & 0xFF, _format_calls(calls)))
    if not ok:
        msg += "  期望调用序列 %s" % _format_calls(want)
    return ok, msg


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证 v4 补丁：静态字节 + 边界 + 差异范围 + Unicorn 调用级功能测试。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    # 1) 长度不变
    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁）" % (len(patched), IMAGE_SIZE)))

    # 2) 补丁区 56 字节与期望完全一致
    blob = build_sync_function(key)
    off = _off(SYNC_FN)
    got = bytes(patched[off:off + SYNC_FN_SIZE])
    results.append((got == blob,
                    "0x%08X..0x%08X 补丁区 %d 字节与期望函数体完全一致"
                    % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE) if got == blob else
                    "补丁区字节与期望不符\n  期望 %s\n  实际 %s"
                    % (blob.hex(" "), got.hex(" "))))

    # 3) 不得越过下一函数 0x0800B790
    results.append((SYNC_FN + len(blob) == SYNC_FN_END,
                    "函数体结束于 0x%08X == 下一函数起点 0x%08X（不越界）"
                    % (SYNC_FN + len(blob), SYNC_FN_END)))

    # 4) 字面量落位：+0x2C=KEY、+0x30=标志地址、+0x34=50024（原厂字面量地址不变）
    lits = struct.unpack("<III", got[44:56])
    lit_ok = (lits[0] == key and lits[1] == KEY_FLAG_ADDR and lits[2] == common.ID_NUMERATOR)
    results.append((lit_ok,
                    "字面量: KEY=0x%04X @0x%08X, FLAG=0x%08X @0x%08X, 50024 @0x%08X"
                    % (lits[0], SYNC_FN + 44, lits[1], SYNC_FN + 48, SYNC_FN + 52)))

    # 5) 与原厂镜像的差异必须只落在补丁区
    orig = _unpatch(patched)
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    expected = _expected_diff_offsets(key)
    results.append((set(diffs) == expected,
                    "与原厂差异 %d 字节，offset=%s（应恰为 %s）"
                    % (len(diffs), ["0x%05X" % d for d in diffs],
                       ["0x%05X" % d for d in sorted(expected)])))
    outside_diff = [i for i in diffs if not (off <= i < off + SYNC_FN_SIZE)]
    results.append((not outside_diff,
                    "补丁区之外无任何字节变化（差异 %d 处）" % len(outside_diff)))

    # 6) Unicorn 调用级功能测试
    try:
        import unicorn
        has_uc = True
    except Exception:  # noqa: BLE001
        has_uc = False
    if not has_uc:
        results.append((True, "警告: unicorn 不可用，跳过 Unicorn 调用级测试（静态检查已通过）"))
        return results

    uc_ver = getattr(unicorn, "__version__", "?")
    results.append((True, "Unicorn %s 调用级测试：从 0x%08X 执行，RAM[0x%08X]=flag，"
                    "桩替换 WriteReg/WriteCmd/delay_us" % (uc_ver, SYNC_FN | 1, KEY_FLAG_ADDR)))

    # 6a) 密钥模式：flag=1，任意 ID 都得到 KEY
    for ident in (0, 5, 99):
        ok, msg = _check_patched(patched, key, ident, flag=1)
        results.append((ok, msg))
    # 6b) 普通模式：flag=0，回到原厂公式
    for ident in (0, 1, 5, 37, 99):
        ok, msg = _check_patched(patched, key, ident, flag=0)
        results.append((ok, msg))
    # 6c) 同一 ID 下 flag 切换必须真正改变同步字（排除残留/常量折叠之类）
    res_on = _uc_run_sync_fn(patched, 5, 1)["calls"]
    res_off = _uc_run_sync_fn(patched, 5, 0)["calls"]
    on_sync = (res_on[0][2] << 8 | res_on[1][2]) if len(res_on) >= 2 else -1
    off_sync = (res_off[0][2] << 8 | res_off[1][2]) if len(res_off) >= 2 else -1
    results.append((on_sync == key and off_sync == common.ID_NUMERATOR // 6,
                    "同一 ID=5 下 flag=1/0 同步字分别为 0x%04X / 0x%04X（应 0x%04X / 0x%04X）"
                    % (on_sync, off_sync, key, common.ID_NUMERATOR // 6)))

    # 7) 原厂镜像自证（同一 harness 必须还原原厂公式 + SCAL 序列）
    orig_img = _unpatch(patched)
    for ident in (0, 1, 5, 37, 99):
        ok, msg = _check_original(orig_img, ident)
        results.append((ok, msg))
    return results


# ---------------------------------------------------------------------------
# 临时可切换版（v2 语义：ID==0 -> KEY）的验证
# ---------------------------------------------------------------------------
def _expected_diff_offsets_id0(key: int) -> set[int]:
    blob = build_sync_function_id0(key)
    base = _off(SYNC_FN)
    return {base + i for i in range(SYNC_FN_SIZE) if blob[i] != ORIG_FUNC[i]}


def _check_id0(image: bytes, key: int, ident: int) -> tuple[bool, str]:
    """v2 语义：ID==0 -> KEY；ID!=0 -> 50024/(ID+1)；只写 SYNC1/0。"""
    res = _uc_run_sync_fn(image, ident, 0)
    if res["error"]:
        return False, "ID=%d id0 版 Unicorn 执行失败: %s" % (ident, res["error"])
    calls = res["calls"]
    expect = key if ident == 0 else common.ID_NUMERATOR // (ident + 1)
    hi, lo = expect >> 8, expect & 0xFF
    want = [("wreg", 4, hi), ("wreg", 5, lo)]
    ok = calls == want
    mode = "密钥模式(KEY, ID==0)" if ident == 0 else "原厂公式"
    msg = ("ID=%-2d %s: sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X（仅 2 次写寄存器）| %s"
           % (ident, mode, expect, hi, lo, _format_calls(calls)))
    if not ok:
        msg += "  期望调用序列 %s" % _format_calls(want)
    if ident == 0:
        ok = ok and expect != common.ID_NUMERATOR
    return ok, msg


def verify_id0(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """验证临时可切换版（ID==0 -> KEY）补丁：静态字节 + 边界 + Unicorn 功能测试。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁）" % (len(patched), IMAGE_SIZE)))
    blob = build_sync_function_id0(key)
    off = _off(SYNC_FN)
    got = bytes(patched[off:off + SYNC_FN_SIZE])
    results.append((got == blob,
                    "0x%08X..0x%08X 补丁区 %d 字节与期望（id0 版）完全一致"
                    % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE) if got == blob else
                    "补丁区字节与期望不符\n  期望 %s\n  实际 %s"
                    % (blob.hex(" "), got.hex(" "))))
    results.append((SYNC_FN + len(blob) == SYNC_FN_END,
                    "函数体结束于 0x%08X == 下一函数起点 0x%08X（不越界）"
                    % (SYNC_FN + len(blob), SYNC_FN_END)))
    lits = (int.from_bytes(got[44:48], "little"), int.from_bytes(got[52:56], "little"))
    results.append((lits == (key, common.ID_NUMERATOR),
                    "字面量: KEY=0x%04X @0x%08X, 50024 @0x%08X"
                    % (lits[0], SYNC_FN + 44, SYNC_FN + 52)))

    orig = _unpatch(patched)
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    expected = _expected_diff_offsets_id0(key)
    results.append((set(diffs) == expected,
                    "与原厂差异 %d 字节，offset=%s（应恰为 %s）"
                    % (len(diffs), ["0x%05X" % d for d in diffs],
                       ["0x%05X" % d for d in sorted(expected)])))
    outside = [i for i in diffs if not (off <= i < off + SYNC_FN_SIZE)]
    results.append((not outside, "补丁区之外无任何字节变化（差异 %d 处）" % len(outside)))

    try:
        import unicorn
        has_uc = True
    except Exception:  # noqa: BLE001
        has_uc = False
    if not has_uc:
        results.append((True, "警告: unicorn 不可用，跳过 Unicorn 调用级测试（静态检查已通过）"))
        return results

    results.append((True, "Unicorn %s 调用级测试（id0 版）：从 0x%08X 执行，桩替换 WriteReg/WriteCmd/delay_us"
                    % (getattr(unicorn, "__version__", "?"), SYNC_FN | 1)))
    for ident in (0, 1, 5, 37, 99):
        ok, msg = _check_id0(patched, key, ident)
        results.append((ok, msg))
    for ident in (0, 1, 5, 37, 99):
        ok, msg = _check_original(orig, ident)
        results.append((ok, msg))
    return results
