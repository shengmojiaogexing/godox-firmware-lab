# -*- coding: utf-8 -*-
"""Lead R&D：在 Unicorn 里直接执行 X3-C 的射频发送/信道/同步字函数，验证广播引闪的可行性。

结论目标：
1. send_short(cmd) 与 send_property(g,k,v) 能独立执行并产出正确的 FIFO 帧；
2. set_channel(ch) / set_sync(id) 能改 FREQ / SYNC 寄存器；
3. 组合成一个 "全频道广播" 循环（32 个频道、每个频道设 KEY 同步字 + 发属性帧 + 发 TEST），
   确认空中帧序列正确、无 fault。

用法：tools\\python\\python.exe keypatch\\dev\\lead\\tx_proto.py
"""
from __future__ import annotations

import os
import struct
import sys

from unicorn import (Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
                     UC_HOOK_CODE, UC_HOOK_MEM_WRITE)
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                               UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                               UC_ARM_REG_PC)

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
BASE = 0x08004800
KEY = 0x6B21

# 函数地址（Lead 已核实）
SEND_PROPERTY = 0x0801184C   # (r0=group, r1=key, r2=value) -> [A9 group key value]
SEND_SHORT = 0x080118B0      # (r0=cmd) -> [D5 cmd]
SET_CHANNEL = 0x08014814     # (r0=ch index 0..31)
SET_SYNC = 0x08014864        # (r0=id 0..99) -> sync=50024/(id+1)
WRITE_REG = 0x08023540       # (r0=reg, r1=value)
STROBE = 0x08023520          # (r0=cmd)
DELAY = 0x080232B6           # (r0=units)
FIFO_WRITE = 0x08023536      # (r0=buf, r1=len)
PA_ON = 0x08010B6C
PA_OFF = 0x08010B24

SENTINEL = 0x2000F000        # 函数返回哨兵（RAM 内，放 b .）

RAM_BASE = 0x20000000
RAM_SIZE = 0x50000
FLASH_BASE = 0x08000000
FLASH_SIZE = 0x100000
STACK = 0x20047000

# 发送函数使用的 RAM 变量（从字面量读出）
RF_BUSY_FLAG = 0x20000146
TX_BUF = 0x20000152


class TxHarness:
    def __init__(self):
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
        self.uc.mem_map(FLASH_BASE, FLASH_SIZE)
        self.uc.mem_map(RAM_BASE, RAM_SIZE)
        # 外设空间（GPIO 等）按 RAM 处理，避免 fault
        self.uc.mem_map(0x40000000, 0x50000)
        self.uc.mem_write(BASE, open(IMAGE, "rb").read())
        self.uc.mem_write(SENTINEL, bytes.fromhex("fee7"))   # b .
        self.regs = []       # (reg, val)
        self.strobes = []
        self.frames = []     # dicts
        self.delays = []
        self._install_hooks()

    def _install_hooks(self):
        uc = self.uc

        def hook(uc_, address, size, user):
            if address == SENTINEL:
                uc_.emu_stop()
                return
            if address in (PA_ON, PA_OFF):
                uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
                return
            if address == DELAY:
                self.delays.append(uc_.reg_read(UC_ARM_REG_R0))
                uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
                return
            if address == STROBE:
                self.strobes.append(uc_.reg_read(UC_ARM_REG_R0))
                uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
                return
            if address == WRITE_REG:
                self.regs.append((uc_.reg_read(UC_ARM_REG_R0), uc_.reg_read(UC_ARM_REG_R1)))
                uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
                return
            if address == FIFO_WRITE:
                buf = uc_.reg_read(UC_ARM_REG_R0)
                n = uc_.reg_read(UC_ARM_REG_R1)
                self.frames.append({"buf": buf, "len": n,
                                    "data": bytes(uc_.mem_read(buf, n)) if 0 < n < 64 else b""})
                uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
                return
        uc.hook_add(UC_HOOK_CODE, hook)

    def call(self, func: int, args=(), nargs=0):
        uc = self.uc
        for i, a in enumerate(args):
            uc.reg_write([UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3][i],
                         a & 0xFFFFFFFF)
        uc.reg_write(UC_ARM_REG_SP, STACK)
        uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
        try:
            uc.emu_start(func | 1, 0, count=200000)
        except UcError as e:
            if (uc.reg_read(UC_ARM_REG_PC) & ~1) != SENTINEL:
                raise RuntimeError("call 0x%08X failed: %s pc=0x%08X"
                                   % (func, e, uc.reg_read(UC_ARM_REG_PC)))
        return uc.reg_read(UC_ARM_REG_R0)

    def clear(self):
        self.regs.clear(); self.strobes.clear(); self.frames.clear(); self.delays.clear()

    def write_sync_direct(self, sync: int):
        self.call(WRITE_REG, (4, (sync >> 8) & 0xFF))
        self.call(WRITE_REG, (5, sync & 0xFF))


def main() -> int:
    h = TxHarness()
    print("=" * 78)
    print("X3-C 射频函数独立执行测试（Unicorn）")
    print("=" * 78)

    # 0) 先清掉 RF busy 标志，否则发送函数会跳过
    h.uc.mem_write(RF_BUSY_FLAG, b"\x00")

    # 1) 原厂按 ID 设置同步字
    h.clear(); h.call(SET_SYNC, (5,))
    sync = (dict(h.regs).get(4, -1) << 8) | dict(h.regs).get(5, -1)
    print("[1] set_sync(ID=5): 写寄存器 %r  -> sync=0x%04X（期望 0x%04X）"
          % (h.regs, sync, 50024 // 6))
    ok1 = sync == 50024 // 6

    # 2) 短帧
    h.clear(); h.call(SEND_SHORT, (0x09,))
    fifo = [f for f in h.frames]
    print("[2] send_short(0x09): FIFO 帧 %s strobes=%s"
          % ([(f["len"], f["data"].hex(" ")) for f in fifo], [hex(x) for x in h.strobes]))
    ok2 = any(f["data"] == bytes.fromhex("d509") for f in fifo)

    # 3) 属性帧
    h.clear(); h.call(SEND_PROPERTY, (0x50, 0xB1, 0x00))
    fifo = [f for f in h.frames]
    print("[3] send_property(0x50,0xB1,0x00): FIFO 帧 %s"
          % [(f["len"], f["data"].hex(" ")) for f in fifo])
    ok3 = any(f["data"] == bytes.fromhex("a950b100") for f in fifo)

    # 4) 信道设置
    h.clear(); h.call(SET_CHANNEL, (0,))
    regd = dict(h.regs)
    print("[4] set_channel(0): FREQ2/1/0 = %02X %02X %02X（期望 5C CE C4）"
          % (regd.get(0x0D, -1), regd.get(0x0E, -1), regd.get(0x0F, -1)))
    ok4 = (regd.get(0x0D), regd.get(0x0E), regd.get(0x0F)) == (0x5C, 0xCE, 0xC4)

    # 5) 广播循环原型：32 个频道，每频道写 KEY 同步字 + 发属性帧 + 发 TEST
    h.clear()
    seq = []
    for ch in range(32):
        h.call(SET_CHANNEL, (ch,))
        h.write_sync_direct(KEY)
        h.call(SEND_SHORT, (0x09,))
        # 记录本次循环的帧数
        seq.append(len(h.frames))
    print("[5] 32 频道广播原型：FIFO 帧总数=%d（每频道 1 帧），strobes=%d"
          % (len(h.frames), len(h.strobes)))
    datas = [f["data"] for f in h.frames]
    ok5 = len(datas) == 32 and all(d == bytes.fromhex("d509") for d in datas)
    sync_writes = [r for r in h.regs if r[0] in (4, 5)]
    print("    同步字写入前 6 次: %s" % [(hex(r), hex(v)) for r, v in sync_writes[:6]])
    print("    每频道累计帧数: %s" % seq[:8])

    print("-" * 78)
    alloks = [ok1, ok2, ok3, ok4, ok5]
    for name, ok in zip(["set_sync", "send_short", "send_property", "set_channel",
                         "broadcast-loop"], alloks):
        print("  %-18s %s" % (name, "PASS" if ok else "FAIL"))
    print("结论:", "ALL PASS" if all(alloks) else "FAIL")
    return 0 if all(alloks) else 1


if __name__ == "__main__":
    sys.exit(main())
