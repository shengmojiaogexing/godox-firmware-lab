# -*- coding: utf-8 -*-
"""Lead R&D：X3-C 固件代码注入原型 —— "全频道广播引闪"。

在镜像尾部 0xFF 空闲区注入一段 Thumb 代码（约 64 字节），实现：

    for (ch = 0; ch < 32; ch++) {
        set_channel(ch);              // 0x08014814
        write_reg(4, KEY >> 8);       // CC2500 SYNC1
        write_reg(5, KEY & 0xFF);     // CC2500 SYNC0
        send_property(0x50, 0xB1, 0); // 组=ALL 的属性帧（占位，正式版用真实功率键值）
        send_short(0x09);             // TEST 引闪帧
    }
    return;

用 Unicorn 直接执行注入后的函数，验证：
* 32 个频道都设置了 FREQ2/1/0；
* 每个频道写入了 KEY 同步字；
* 每个频道发出 1 个属性帧 + 1 个 TEST 帧；
* 镜像长度不变、只改空闲区。
"""
from __future__ import annotations

import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from tx_proto import (TxHarness, IMAGE, BASE, KEY, SET_CHANNEL, WRITE_REG,
                      SEND_PROPERTY, SEND_SHORT)  # noqa: E402

CAVE_VA = 0x080EE000          # 镜像尾部 0xFF 空闲区（0x080ECF20..0x080F6FFF，无任何字面量引用）
CAVE_FILE = CAVE_VA - BASE


class Asm:
    """够用的 Thumb-1 汇编器：push/pop/movs/mov/adds/cmp/b/bl/ldr literal。"""

    def __init__(self, addr: int):
        self.addr = addr
        self.code = bytearray()
        self.labels = {}
        self.fix_branch = []   # (offset, kind, label)
        self.fix_ldr = []      # (offset, target)

    def here(self):
        return self.addr + len(self.code)

    def label(self, name):
        self.labels[name] = self.here()

    def emit(self, hw: int):
        self.code += struct.pack("<H", hw & 0xFFFF)
        return self

    def emit32(self, b: bytes):
        self.code += b

    def push(self, mask):
        return self.emit(0xB400 | mask)

    def movs(self, rd, imm8):
        return self.emit(0x2000 | (rd << 8) | imm8)

    def mov(self, rd, rm):
        return self.emit(0x4600 | (rm << 3) | rd)

    def adds_imm(self, rd, imm8):
        return self.emit(0x3000 | (rd << 8) | imm8)

    def cmp_imm(self, rn, imm8):
        return self.emit(0x2800 | (rn << 8) | imm8)

    def ldr_lit(self, rt, target):
        self.fix_ldr.append((len(self.code), target))
        return self.emit(0x4800 | (rt << 8))

    def bl(self, target):
        here = self.here()
        off = target - (here + 4)
        s = 1 if off < 0 else 0
        i1 = (off >> 23) & 1
        i2 = (off >> 22) & 1
        j1 = i1 if s else ((~i1) & 1)
        j2 = i2 if s else ((~i2) & 1)
        imm10 = (off >> 12) & 0x3FF
        imm11 = (off >> 1) & 0x7FF
        return self.emit32(struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                                       0xD000 | (j1 << 13) | (j2 << 11) | imm11))

    def branch(self, kind, label):
        self.fix_branch.append((len(self.code), kind, label))
        return self.emit(0x0000)

    def finish(self):
        code_len = len(self.code)
        while (self.addr + len(self.code)) % 4:
            self.code += b"\x00"
        lit_addr = self.addr + len(self.code)
        words = []
        for i, (off, target) in enumerate(self.fix_ldr):
            ins_addr = self.addr + off
            pc = (ins_addr + 4) & ~3
            this_lit = lit_addr + 4 * i
            imm8 = (this_lit - pc) // 4
            assert 0 <= imm8 <= 0xFF, (hex(this_lit), hex(pc), imm8)
            # 本文件只用 ldr r1, =...
            self.code[off:off + 2] = struct.pack("<H", 0x4800 | (1 << 8) | imm8)
            words.append(target & 0xFFFFFFFF)
        for w in words:
            self.code += struct.pack("<I", w)
        for off, kind, label in self.fix_branch:
            ins_addr = self.addr + off
            delta = self.labels[label] - (ins_addr + 4)
            if kind == "bcc":
                val = 0xD300 | ((delta // 2) & 0xFF)      # BCC T1 (0xD3 = CC)
            else:
                val = 0xE000 | ((delta // 2) & 0x7FF)     # B T1
            self.code[off:off + 2] = struct.pack("<H", val)
        return bytes(self.code)


def build_broadcast(va: int, key: int = KEY) -> bytes:
    a = Asm(va)
    a.push(0x130)              # push {r4, r5, lr}
    a.movs(4, 0)               # r4 = ch
    a.label("loop")
    a.mov(0, 4)                # r0 = ch
    a.bl(SET_CHANNEL)
    a.movs(0, 4)               # write_reg(4, KEY>>8)
    a.ldr_lit(1, key >> 8)
    a.bl(WRITE_REG)
    a.movs(0, 5)               # write_reg(5, KEY&0xFF)
    a.ldr_lit(1, key & 0xFF)
    a.bl(WRITE_REG)
    a.movs(0, 0x50)            # send_property(0x50, 0xB1, 0)
    a.movs(1, 0xB1)
    a.movs(2, 0x00)
    a.bl(SEND_PROPERTY)
    a.movs(0, 0x09)            # send_short(TEST)
    a.bl(SEND_SHORT)
    a.adds_imm(4, 1)
    a.cmp_imm(4, 32)
    a.branch("bcc", "loop")
    a.emit(0xBD30)             # pop {r4, r5, pc}
    return a.finish()


def main() -> int:
    orig = open(IMAGE, "rb").read()
    img = bytearray(orig)
    blob = build_broadcast(CAVE_VA)
    print("注入代码长度: %d 字节" % len(blob))
    print("注入地址: 0x%08X (文件偏移 0x%06X)" % (CAVE_VA, CAVE_FILE))
    assert all(b == 0xFF for b in img[CAVE_FILE:CAVE_FILE + len(blob)]), "目标区不是空闲 0xFF"
    img[CAVE_FILE:CAVE_FILE + len(blob)] = blob
    patched = bytes(img)
    print("镜像长度不变: %s" % (len(patched) == len(orig)))

    h = TxHarness()
    h.uc.mem_write(BASE, patched)
    h.uc.mem_write(0x20000146, b"\x00")
    h.clear()
    h.call(CAVE_VA, ())

    frames = [f["data"] for f in h.frames]
    regs = h.regs
    freq_sets = []
    i = 0
    while i + 2 < len(regs):
        if (regs[i][0], regs[i + 1][0], regs[i + 2][0]) == (0x0D, 0x0E, 0x0F):
            freq_sets.append((regs[i][1], regs[i + 1][1], regs[i + 2][1]))
            i += 3
        else:
            i += 1
    sync_sets = [(r, v) for (r, v) in regs if r in (4, 5)]
    prop_frames = [d for d in frames if len(d) == 4 and d[:1] == b"\xa9"]
    short_frames = [d for d in frames if len(d) == 2 and d[:1] == b"\xd5"]

    print("-" * 74)
    print("执行注入函数：")
    print("  频道设置次数        : %d (期望 32)" % len(freq_sets))
    print("  前 3 个频道 FREQ    : %s" % [tuple(hex(x) for x in f) for f in freq_sets[:3]])
    print("  同步字写入次数      : %d (期望 64 = 32×2)" % len(sync_sets))
    print("  属性帧数            : %d (期望 32)" % len(prop_frames))
    print("  TEST 帧数           : %d (期望 32)" % len(short_frames))
    ok = (len(freq_sets) == 32 and len(sync_sets) == 64 and
          len(prop_frames) == 32 and len(short_frames) == 32)

    freq_table_ok = True
    tbl_off = 0x08045390 - BASE
    for ch, (f2, f1, f0) in enumerate(freq_sets):
        expect = (orig[tbl_off + ch], orig[tbl_off + 33 + ch], orig[tbl_off + 66 + ch])
        if (f2, f1, f0) != expect:
            freq_table_ok = False
            print("  !! ch%d FREQ %s != %s" % (ch, (f2, f1, f0), expect))
    print("  频道表逐项一致      : %s" % freq_table_ok)
    print("  前 4 个同步字写入    : %s" % [(hex(r), hex(v)) for r, v in sync_sets[:4]])
    print("-" * 74)
    print("结论:", "PASS" if (ok and freq_table_ok) else "FAIL")
    return 0 if (ok and freq_table_ok) else 1


if __name__ == "__main__":
    sys.exit(main())
