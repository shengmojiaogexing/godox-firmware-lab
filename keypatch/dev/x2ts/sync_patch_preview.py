# -*- coding: utf-8 -*-
"""X2T-S 同步字函数「C.Fn 开关」补丁 — 原型构造 + Unicorn 行为验证（临时脚本）。

真实链接基址 0x08003000（re-sync 已确认）。
同步字函数 0x0800C8F8..0x0800C92B（52 B，下一函数 0x0800C92C）。
46 B 逻辑 + 字面量，复用同 SDK 频道设置函数 0x0800C8B0 的尾部
（0x0800C8DC: delay(20); SCAL(0x33); pop {r4,lr}; delay(1000)）——
因此保留原厂 delay/SCAL 行为，不丢射频校准。

用法: tools\\python\\python.exe keypatch\\dev\\x2ts\\sync_patch_preview.py
"""
from __future__ import annotations

import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
IMG = os.path.join(ROOT, "devices", "x2ts", "firmware", "X2T-S_V1.4.bin")

BASE = 0x08003000
IMAGE_SIZE = 58628
SYNC_FN = 0x0800C8F8
SYNC_FN_END = 0x0800C92C
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN
assert SYNC_FN_SIZE == 52

# 频道设置函数尾（delay20 / SCAL / pop / delay1000），与同步字函数原厂尾部等价
TAIL_HELPER = 0x0800C8DC

WRITE_REG = 0x0800E842
WRITE_CMD = 0x0800E932      # strobe
DELAY_US = 0x0800E5AA

ORIG_FUNC = bytes.fromhex(
    "10b54cf26831401cb1fbf0f4210a042001f00000"  # placeholder 占位，运行时用真实镜像替换
)

ROW_OFF = SYNC_FN - BASE


# ---------------------------------------------------------------------------
# 编码助手
# ---------------------------------------------------------------------------
def p16(hw: int) -> bytes:
    return struct.pack("<H", hw & 0xFFFF)


def movw(rd: int, imm16: int) -> bytes:
    imm4 = (imm16 >> 12) & 0xF
    i = (imm16 >> 11) & 1
    imm3 = (imm16 >> 8) & 0x7
    imm8 = imm16 & 0xFF
    return p16(0xF240 | (i << 10) | imm4) + p16((imm3 << 12) | ((rd & 0xF) << 8) | imm8)


def udiv(rd: int, rn: int, rm: int) -> bytes:
    return p16(0xFBB0 | (rn & 0xF)) + p16(0xF0F0 | ((rd & 0xF) << 8) | (rm & 0xF))


def bl(addr: int, target: int) -> bytes:
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target), off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = (i1 if s else ((~i1) & 1))
    j2 = (i2 if s else ((~i2) & 1))
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return (p16(0xF000 | (s << 10) | imm10)
            + p16(0xD000 | (j1 << 13) | (j2 << 11) | imm11))


def bw(addr: int, target: int) -> bytes:
    off = target - (addr + 4)
    assert off % 2 == 0 and -0x1000000 <= off < 0x1000000
    s = 1 if off < 0 else 0
    imm = off & 0x1FFFFFF
    i1 = (imm >> 23) & 1
    i2 = (imm >> 22) & 1
    imm10 = (imm >> 12) & 0x3FF
    imm11 = (imm >> 1) & 0x7FF
    j1 = (1 - i1) ^ s
    j2 = (1 - i2) ^ s
    return (p16(0xF000 | (s << 10) | imm10)
            + p16(0x9000 | (j1 << 13) | (j2 << 11) | imm11))


def b_cond(addr: int, target: int, cond: int) -> bytes:
    imm = target - (addr + 4)
    assert imm % 2 == 0 and -256 <= imm <= 254, (hex(addr), hex(target), imm)
    return p16(0xD000 | ((cond & 0xF) << 8) | ((imm // 2) & 0xFF))


def b_n(addr: int, target: int) -> bytes:
    imm = target - (addr + 4)
    assert imm % 2 == 0 and -2048 <= imm <= 2046, (hex(addr), hex(target), imm)
    return p16(0xE000 | ((imm // 2) & 0x7FF))


# ---------------------------------------------------------------------------
# 补丁体：52 字节（46 B 代码 + 2 B 对齐 NOP + 4 B FLAG 字面量）
# ---------------------------------------------------------------------------
def build_sync_function(key: int, flag_addr: int, on_value: int = 1) -> bytes:
    """on_value=0 表示「非 0 即密钥模式」；on_value=1/2 表示等于该值才是密钥模式。"""
    key &= 0xFFFF
    buf = bytearray()

    def here():
        return SYNC_FN + len(buf)

    def emit(b):
        buf.extend(b)

    # 0x00 push {r4, lr}
    emit(p16(0xB510))
    lit_fix = len(buf)              # LDR 字面量位置
    emit(b"\x00\x00")               # 0x02 ldr r3, =FLAG（回填）
    emit(p16(0x781B))               # 0x04 ldrb r3, [r3]
    emit(p16(0x2B00 | (on_value & 0xFF)))  # 0x06 cmp r3, #on
    if on_value == 0:
        # 非 0 -> 密钥模式：beq 原厂公式
        emit(b_cond(here(), SYNC_FN + 0x10, 0x0))     # 0x08 beq .orig
        emit(movw(4, key))                            # 0x0A movw r4, #KEY
        emit(b_n(here(), SYNC_FN + 0x1A))             # 0x0E b .write
        # 0x10 .orig
        emit(p16(0x1C40))                             # adds r0, r0, #1
        emit(movw(1, 0xC368))
        emit(udiv(4, 1, 0))
        # 0x1A .write
    else:
        emit(b_cond(here(), SYNC_FN + 0x16, 0x0))     # 0x08 beq .key
        emit(p16(0x1C40))                             # 0x0A adds r0, r0, #1
        emit(movw(1, 0xC368))                         # 0x0C
        emit(udiv(4, 1, 0))                           # 0x10
        emit(b_n(here(), SYNC_FN + 0x1A))             # 0x14 b .write
        # 0x16 .key
        emit(movw(4, key))                            # 0x16 movw r4, #KEY
        # 0x1A .write
    emit(p16(0x0A21))               # 0x1A lsrs r1, r4, #8
    emit(p16(0x2004))               # 0x1C movs r0, #4
    emit(bl(here(), WRITE_REG))     # 0x1E
    emit(p16(0xB2E1))               # 0x22 uxtb r1, r4
    emit(p16(0x2005))               # 0x24 movs r0, #5
    emit(bl(here(), WRITE_REG))     # 0x26
    emit(bw(here(), TAIL_HELPER))   # 0x2A b.w 原厂 delay/SCAL 尾
    emit(p16(0xBF00))               # 0x2E nop（对齐）
    # 0x30 .word FLAG
    assert len(buf) == 0x30, len(buf)
    emit(struct.pack("<I", flag_addr))
    assert len(buf) == 52, len(buf)

    # 回填 ldr 字面量偏移：pc = (instr_addr + 4) & ~3
    instr_addr = SYNC_FN + lit_fix
    pc = (instr_addr + 4) & ~3
    lit_addr = SYNC_FN + 0x30
    delta = lit_addr - pc
    assert delta % 4 == 0 and 0 <= delta // 4 <= 0xFF, (hex(pc), hex(lit_addr))
    buf[lit_fix:lit_fix + 2] = p16(0x4800 | (3 << 8) | (delta // 4))
    return bytes(buf)


# ---------------------------------------------------------------------------
# Unicorn 行为验证
# ---------------------------------------------------------------------------
UC_ROM = BASE
UC_ROM_SIZE = 0x20000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x40000
UC_STACK = 0x20021000
UC_SENTINEL = 0x20021500


def uc_run(image: bytes, flag_addr: int, flag: int, ident: int) -> list:
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC, UC_ARM_REG_CPSR)

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE)
    uc.mem_map(UC_RAM, UC_RAM_SIZE)
    uc.mem_write(BASE, image)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))
    for addr in (WRITE_REG, WRITE_CMD, DELAY_US):
        uc.mem_write(addr, bytes.fromhex("7047"))   # bx lr
    if 0 <= flag_addr - UC_RAM:
        uc.mem_write(flag_addr, bytes([flag & 0xFF]))

    calls = []
    state = {"ret": False, "err": None}

    def hook(u, address, size_, user):
        if address == UC_SENTINEL:
            state["ret"] = True
            u.emu_stop()
            return
        if address == WRITE_REG:
            calls.append(("wreg", u.reg_read(UC_ARM_REG_R0), u.reg_read(UC_ARM_REG_R1)))
        elif address == WRITE_CMD:
            calls.append(("cmd", u.reg_read(UC_ARM_REG_R0), None))
        elif address == DELAY_US:
            calls.append(("delay", u.reg_read(UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    uc.reg_write(UC_ARM_REG_R1, 0x1111)
    uc.reg_write(UC_ARM_REG_R2, 0x2222)
    uc.reg_write(UC_ARM_REG_R3, 0x3333)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    try:
        uc.emu_start(SYNC_FN | 1, 0, count=5000)
    except Exception as exc:  # noqa: BLE001
        state["err"] = str(exc)
    return calls, state


def fmt(calls):
    return " -> ".join(
        ("WriteReg(%d,0x%02X)" % (a, b)) if k == "wreg" else
        ("strobe(0x%02X)" % a) if k == "cmd" else ("delay(%d)" % a)
        for k, a, b in calls)


def main():
    data = open(IMG, "rb").read()
    assert len(data) == IMAGE_SIZE
    orig_func = data[ROW_OFF:ROW_OFF + SYNC_FN_SIZE]
    print("原厂 0x%08X..0x%08X = %s" % (SYNC_FN, SYNC_FN_END - 1, orig_func.hex(" ")))
    print("尾 helper 0x%08X 字节 = %s"
          % (TAIL_HELPER, data[TAIL_HELPER - BASE:TAIL_HELPER - BASE + 28].hex(" ")))
    print()

    key = 0xD798
    flag_addr = 0x20020100        # TODO: 等 re-cfn 定稿的真实 C.Fn 开关字节
    for on_value in (1, 2, 0):
        blob = build_sync_function(key, flag_addr, on_value)
        print("== 补丁体 on_value=%d ==" % on_value)
        print("  ", blob.hex(" "))
        patched = bytearray(data)
        patched[ROW_OFF:ROW_OFF + SYNC_FN_SIZE] = blob
        patched = bytes(patched)
        ok = True
        # 原厂自证
        for ident in (0, 5, 99):
            calls, st = uc_run(data, flag_addr, 0, ident)
            expect = 50024 // (ident + 1)
            want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF),
                    ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
            good = st["err"] is None and calls == want
            ok = ok and good
            print("  原厂 ID=%-2d sync=0x%04X -> %s %s"
                  % (ident, expect, "PASS" if good else "FAIL", fmt(calls)))
        # 补丁版：密钥模式（flag=on_value，若 on_value==0 用 1）
        key_flag = 1 if on_value == 0 else on_value
        for ident in (0, 5, 37, 99):
            calls, st = uc_run(patched, flag_addr, key_flag, ident)
            want = [("wreg", 4, (key >> 8) & 0xFF), ("wreg", 5, key & 0xFF),
                    ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
            good = st["err"] is None and calls == want
            ok = ok and good
            print("  密钥 ID=%-2d KEY=0x%04X -> %s %s"
                  % (ident, key, "PASS" if good else "FAIL", fmt(calls)))
        # 补丁版：原厂模式（flag=0）
        for ident in (0, 5, 37, 99):
            calls, st = uc_run(patched, flag_addr, 0, ident)
            expect = 50024 // (ident + 1)
            want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF),
                    ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
            good = st["err"] is None and calls == want
            ok = ok and good
            print("  原厂 ID=%-2d sync=0x%04X -> %s %s"
                  % (ident, expect, "PASS" if good else "FAIL", fmt(calls)))
        print("  =>", "ALL PASS" if ok else "FAILED")
        print()

    # 反汇编补丁体自检
    from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
    blob = build_sync_function(key, flag_addr, 2)
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    print("== 反汇编自检（on_value=2）==")
    for ins in md.disasm(blob, SYNC_FN):
        print("  0x%08X  %-20s %s %s" % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))
    print()
    print("TAIL:", hex(TAIL_HELPER))


if __name__ == "__main__":
    main()
