# -*- coding: utf-8 -*-
"""解压 X2T-C 开机 RAM 初始化压缩块，还原 0x20000000..0x2000017F。

背景
----
payload 尾部 `0x0801871C` 起是一段 RLE 压缩数据；开机初始化表（0x080186FC 处
16 B/项，func 指针 `0x08005228`）把它解压到 RAM `0x20000000`，长度 0x180。

其中 `0x20000092` 起 16 字节就是 **C.Fn 最大值表**：
`02 01 01 09 01 1f 63 01 63 02 01 08 01 03 02 06`
（index5=31 CH、index6=99 ID、index14=2 LIGHT、index15=6 LCD）。

本脚本只用 Unicorn 在内存里执行 `0x08005228` 的解压函数，不改任何文件。

用法（仓库根）::

    tools\\python\\python.exe devices\\x2tc\\re\\decompress_ram_init.py
"""
from __future__ import annotations

import os
import sys

sys.dont_write_bytecode = True
_HERE = os.path.dirname(os.path.abspath(__file__))
if _HERE not in sys.path:
    sys.path.insert(0, _HERE)

import fwlib as st  # noqa: E402

SRC = 0x0801871C          # 压缩块（payload 内）
DST = 0x20000000          # 解压目标
LEN = 0x180               # 解压长度
DECOMPRESS = 0x08005228   # RLE/bit-pack 解压函数（r0=src, r1=dst, r2=len）
SENTINEL = 0x20000F00     # 返回落点（放 b .）


def main() -> int:
    try:
        sys.stdout.reconfigure(encoding="utf-8", errors="replace")
    except Exception:  # noqa: BLE001
        pass
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_LR, UC_ARM_REG_PC, UC_ARM_REG_R0,
                                   UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_SP)

    image = st.load_image()
    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(0x08000000, 0x20000, UC_PROT_ALL)
    uc.mem_map(0x20000000, 0x2000, UC_PROT_ALL)
    uc.mem_write(st.BASE, image)
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
    uc.reg_write(UC_ARM_REG_R0, SRC)
    uc.reg_write(UC_ARM_REG_R1, DST)
    uc.reg_write(UC_ARM_REG_R2, LEN)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_SP, 0x20000E00)
    try:
        uc.emu_start(DECOMPRESS | 1, 0, 0, 200000)
    except UcError as exc:  # pragma: no cover - 正常固件不会走到
        print("解压失败:", exc)
        return 1
    if uc.reg_read(UC_ARM_REG_PC) != SENTINEL:
        print("解压未返回（pc=0x%08X）" % uc.reg_read(UC_ARM_REG_PC))
        return 1
    data = bytes(uc.mem_read(DST, LEN))
    print("解压 %d 字节：0x%08X -> 0x%08X" % (LEN, SRC, DST))
    for off in range(0, LEN, 16):
        print("0x%03X: %s" % (off, data[off:off + 16].hex(" ")))
    max_tbl = data[0x92:0x92 + 16]
    print()
    print("C.Fn 最大值表 @0x20000092 = %s" % max_tbl.hex(" "))
    print("  每项 max:", ["item%d=%d" % (i, b) for i, b in enumerate(max_tbl)])
    ok = (len(max_tbl) == 16 and max_tbl[5] == 0x1F and max_tbl[6] == 0x63
          and max_tbl[14] == 2 and max_tbl[15] == 6)
    print("断言(index5=31/CH, index6=99/ID, index14=2/LIGHT, index15=6/LCD): %s"
          % ("PASS" if ok else "FAIL"))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
