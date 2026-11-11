# -*- coding: utf-8 -*-
"""X2T-S 侦察 5：代码洞（0xFF 连续区）扫描 + 原厂同步字函数 Unicorn 行为验证。"""
import struct

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08000000
SIZE = len(DATA)

SYNC_FN = 0x080098F8
WRITE_REG = 0x0800B842
WRITE_CMD = 0x0800B932
DELAY_US = 0x0800B5AA

UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x40000
UC_STACK = 0x20021000
UC_SENTINEL = 0x20021500


def scan_ff():
    print("=== 0xFF runs (>= 16 bytes) ===")
    i = 0
    runs = []
    while i < SIZE:
        if DATA[i] == 0xFF:
            j = i
            while j < SIZE and DATA[j] == 0xFF:
                j += 1
            if j - i >= 16:
                runs.append((i, j - i))
            i = j
        else:
            i += 1
    for off, ln in runs:
        print("  0x%08X..0x%08X len=%d (off 0x%05X)" % (BASE + off, BASE + off + ln - 1, ln, off))
    print("  total runs:", len(runs), " bytes:", sum(l for _, l in runs))
    print()

    print("=== 0x00 runs (>= 16 bytes) ===")
    i = 0
    while i < SIZE:
        if DATA[i] == 0x00:
            j = i
            while j < SIZE and DATA[j] == 0x00:
                j += 1
            if j - i >= 16:
                print("  0x%08X..0x%08X len=%d (off 0x%05X)"
                      % (BASE + i, BASE + j - 1, j - i, i))
            i = j
        else:
            i += 1
    print()


def is_referenced(start, length):
    """静态检查：该区间是否被 32 位字面量/分支目标引用。"""
    refs = []
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True
    for off in range(0, SIZE - 3, 4):
        v = struct.unpack_from("<I", DATA, off)[0]
        if start <= v < start + length:
            refs.append((BASE + off, v))
    return refs


def unicorn_check():
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC, UC_ARM_REG_CPSR)

    print("=== Unicorn: 原厂同步字函数 0x%08X ===" % SYNC_FN)
    for ident in (0, 1, 5, 37, 99):
        uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
        uc.mem_map(UC_ROM, UC_ROM_SIZE)
        uc.mem_map(UC_RAM, UC_RAM_SIZE)
        uc.mem_write(BASE, DATA)
        uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))
        for addr in (WRITE_REG, WRITE_CMD, DELAY_US):
            uc.mem_write(addr, bytes.fromhex("7047"))   # bx lr
        calls = []
        state = {"ret": False, "err": None, "insn": 0}

        def hook(u, address, size_, user):
            state["insn"] += 1
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
        uc.reg_write(UC_ARM_REG_R0, ident)
        uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
        try:
            uc.emu_start(SYNC_FN | 1, 0, count=2000)
        except Exception as exc:  # noqa: BLE001
            state["err"] = str(exc)
        expect = 50024 // (ident + 1)
        seq = " -> ".join(
            ("WriteReg(%d,0x%02X)" % (a, b)) if k == "wreg" else
            ("strobe(0x%02X)" % a) if k == "cmd" else ("delay(%d)" % a)
            for k, a, b in calls)
        print("  ID=%-2d sync=0x%04X | %s | err=%s ret=%s insn=%d"
              % (ident, expect, seq, state["err"], state["ret"], state["insn"]))
    print()


if __name__ == "__main__":
    scan_ff()
    print("=== 可疑 0xFF 区引用检查 ===")
    for off in range(0, SIZE):
        pass
    # 对第一段大 0xFF 区做引用检查
    runs = []
    i = 0
    while i < SIZE:
        if DATA[i] == 0xFF:
            j = i
            while j < SIZE and DATA[j] == 0xFF:
                j += 1
            if j - i >= 32 and j - i < 0x1000:
                runs.append((BASE + i, j - i))
            i = j
        else:
            i += 1
    for start, ln in runs:
        refs = is_referenced(start, ln)
        print("  0x%08X len=%d refs=%s" % (start, ln, refs[:8]))
    unicorn_check()
