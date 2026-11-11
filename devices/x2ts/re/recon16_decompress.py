# -*- coding: utf-8 -*-
"""X2T-S 侦察 16：用 Unicorn 执行启动解压函数 0x080031F4，还原 .data 段内容。"""
from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                               UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_CPSR)

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08003000
ROM = 0x08000000
ROM_SIZE = 0x20000
RAM = 0x20000000
RAM_SIZE = 0x40000
DECOMP = 0x080031F4
SRC = 0x080114C8
DST = 0x20020000
LEN = 0x188


def main():
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(ROM, ROM_SIZE)
    uc.mem_map(RAM, RAM_SIZE)
    uc.mem_write(BASE, DATA)
    uc.mem_write(0x20021500, bytes.fromhex("fee7"))
    uc.reg_write(UC_ARM_REG_SP, 0x20021000)
    uc.reg_write(UC_ARM_REG_R0, SRC)
    uc.reg_write(UC_ARM_REG_R1, DST)
    uc.reg_write(UC_ARM_REG_R2, LEN)
    uc.reg_write(UC_ARM_REG_LR, 0x20021501)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    uc.emu_start(DECOMP | 1, 0x20021500, count=2000000)
    out = bytes(uc.mem_read(DST, LEN))
    print("decompressed %d bytes at 0x%08X:" % (LEN, DST))
    for i in range(0, LEN, 16):
        print("  0x%08X  %s" % (DST + i, out[i:i + 16].hex(" ")))
    return out


if __name__ == "__main__":
    main()
