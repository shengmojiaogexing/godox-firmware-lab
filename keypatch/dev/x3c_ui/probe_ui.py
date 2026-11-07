# -*- coding: utf-8 -*-
"""让 devices/x3c/emu 的 Unicorn 模拟器跑到 X3-C 的 UI 主循环并截图（ui-x3c 任务探针）。

原厂模拟器缺口：TIM7(IRQ55) 的 1ms 节拍中断没有建模，主循环
`do {...} while (*0x20000004 == 0)` 永远等不到 tick，UI tick（100ms 软定时器
回调 FUN_08014b28）从不执行，所以只能看到开机电池图。

本探针用"模拟时间注入"补上这一块：
  * 每跑 chunk 条指令就 tick+1（写 RAM 0x20000310）并置位主循环等待标志
    （0x20000004 = 1），等价于 TIM7 中断服务里 tick_advance(1) + 置标志。
  * 同时解码 XMC 0x6C000000 上的 LCD 8080 总线，生成 PNG。
这样就能验证"改 UI 代码 → 屏幕真的变了"。

用法：
  tools\\python\\python.exe probe_ui.py --img <bin> --max 200000000
"""
from __future__ import annotations

import argparse
import os
import struct
import sys
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
sys.path.insert(0, EMU)

from machine import Machine  # noqa: E402

IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
OUT = os.path.join(HERE, "shots")
LCD = 0x6C000000

# 关键 RAM 变量（见 FEASIBILITY.md §1）
RAM_TICK = 0x20000310      # 1ms tick 计数（get_tick 0x080334d8 读它）
RAM_TICK_FLAG = 0x20000004  # 主循环等待标志
RAM_UI_MODE = 0x200003E7   # 当前页面/模式 id（UI tick 比较它）
RAM_UI_REQ = 0x200004C4    # 请求切换到的页面 id


# ---------------------------------------------------------------- 面板模型
class LcdBus:
    def __init__(self, w=368, h=448):
        self.w, self.h = w, h
        self.fb = bytearray(w * h * 2)
        self.cmd = None
        self.args = []
        self.madctl = 0
        self.xs = self.xe = self.ys = self.ye = 0
        self.px = self.py = 0
        self.ram = False
        self.pixbuf = bytearray()
        self.n_pixels = 0
        self.n_cmds = 0
        self.touched = 0

    def write_a0(self, v):
        if v == 0xF9:
            return
        self.cmd = v
        self.args = []
        self.n_cmds += 1
        if v == 0x2C:
            self.ram = True
            self.px, self.py = self.xs, self.ys
        elif v in (0x28, 0x2A, 0x2B):
            self.ram = False

    def write_a1(self, v):
        if self.ram:
            self.pixbuf.append(v)
            if len(self.pixbuf) >= 2:
                hi, lo = self.pixbuf[0], self.pixbuf[1]
                del self.pixbuf[:2]
                if self.px <= self.xe and self.py <= self.ye:
                    self._put(self.px, self.py, (hi << 8) | lo)
                    self.n_pixels += 1
                self.px += 1
                if self.px > self.xe:
                    self.px, self.py = self.xs, self.py + 1
            return
        self.args.append(v)
        a = self.args
        if self.cmd == 0x2A and len(a) == 4:
            self.xs, self.xe = (a[0] << 8) | a[1], (a[2] << 8) | a[3]
        elif self.cmd == 0x2B and len(a) == 4:
            self.ys, self.ye = (a[0] << 8) | a[1], (a[2] << 8) | a[3]
        elif self.cmd == 0x36 and len(a) == 1:
            self.madctl = a[0]

    def _put(self, x, y, v):
        if self.madctl & 1:
            x = self.w - 1 - x
        if (self.madctl >> 5) & 1:
            y = self.h - 1 - y
        if 0 <= x < self.w and 0 <= y < self.h:
            o = (y * self.w + x) * 2
            self.fb[o] = v & 0xFF
            self.fb[o + 1] = v >> 8

    def snapshot(self, tag):
        rgb = bytearray(self.w * self.h * 3)
        for i in range(0, len(self.fb), 2):
            v = self.fb[i] | (self.fb[i + 1] << 8)
            o = (i // 2) * 3
            rgb[o] = ((v >> 11) & 0x1F) << 3
            rgb[o + 1] = ((v >> 5) & 0x3F) << 2
            rgb[o + 2] = (v & 0x1F) << 3
        os.makedirs(OUT, exist_ok=True)
        path = os.path.join(OUT, "ui_%s.png" % tag)
        _png_write(path, self.w, self.h, rgb)
        return path


def _png_write(path, w, h, rgb):
    raw = bytearray()
    for y in range(h):
        raw.append(0)
        raw += rgb[y * w * 3:(y + 1) * w * 3]

    def chunk(tag, data):
        return (struct.pack(">I", len(data)) + tag + data
                + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF))

    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n")
        f.write(chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0)))
        f.write(chunk(b"IDAT", zlib.compress(bytes(raw), 6)))
        f.write(chunk(b"IEND", b""))


# ---------------------------------------------------------------- 硬件补丁
def patch_option_bytes(m):
    # machine.Machine 已经预置 option bytes；这里只保证没有被改写
    try:
        m.uc.mem_write(0x1FFFC002, struct.pack("<H", 0x000F))
        m.uc.mem_write(0x1FFFC010, struct.pack("<H", 0x0002))
    except Exception:
        pass


class AdcBlock:
    def __init__(self, raw=2600):
        self.regs = {}
        self.raw = raw

    def read(self, offset, size):
        off = offset & ~3
        if off == 0x00:
            return 0x42
        if off == 0x08:
            return self.regs.get(off, 0) & ~0x0C
        if off in (0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C):
            return self.raw
        return self.regs.get(off, 0)

    def write(self, offset, size, value):
        self.regs[offset & ~3] = value


def patch_adc(m, raw):
    p = m.find_peripheral(0x40012000)
    dev = AdcBlock(raw)
    if p is None:
        m.add_peripheral(dev)
    else:
        p.read = dev.read
        p.write = dev.write
        p.name = "ADC"


def patch_delays(m):
    """把校准 nop-sled 延时函数短路成 `bx lr`（0x4770）。"""
    m.uc.mem_write(0x0801C118, b"\x70\x47")
    m.uc.mem_write(0x0801C0FE, b"\x70\x47")


# ---------------------------------------------------------------- 主流程
def run(img_path, max_insn, chunk, shot_every, tick_per_chunk, dry_run=0):
    m = Machine(image=img_path, log_fn=lambda s: None, trace=True)
    patch_option_bytes(m)
    patch_adc(m, 2600)
    patch_delays(m)

    from unicorn import UC_HOOK_MEM_WRITE
    bus = LcdBus()

    def wr(uc, access, address, size, value, user):
        if address == LCD:
            bus.write_a0(value & 0xFF)
        elif address == LCD + 1:
            bus.write_a1(value & 0xFF)

    m.uc.hook_add(UC_HOOK_MEM_WRITE, wr, begin=LCD, end=LCD + 2)
    sp, pc = m.boot()
    print("reset: SP=0x%08x PC=0x%08x" % (sp, pc))

    tick = 0
    done = 0
    last_pix = 0
    while done < max_insn:
        n = min(chunk, max_insn - done)
        m.run(n)
        done = m.insn_count
        # ---- 模拟 TIM7 1ms 节拍 + 主循环标志
        tick += tick_per_chunk
        m.uc.mem_write(RAM_TICK, struct.pack("<I", tick & 0xFFFFFFFF))
        m.uc.mem_write(RAM_TICK_FLAG, b"\x01")
        if bus.n_pixels != last_pix:
            print("[%10d] px=%d cmds=%d PC=0x%08x ui_mode=0x%02x ui_req=0x%02x" % (
                done, bus.n_pixels, bus.n_cmds, m.reg("PC"),
                m.uc.mem_read(RAM_UI_MODE, 1)[0],
                m.uc.mem_read(RAM_UI_REQ, 1)[0]))
            last_pix = bus.n_pixels
        if shot_every and done % shot_every < chunk:
            print("  snapshot:", bus.snapshot("%09d" % done))
    final = bus.snapshot("final")
    print("final:", final)
    print("pixels=%d cmds=%d tick=%d PC=0x%08x stop=%s" % (
        bus.n_pixels, bus.n_cmds, tick, m.reg("PC"), m.stop_reason))
    return m, bus


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--img", default=IMAGE)
    ap.add_argument("--max", type=int, default=200_000_000)
    ap.add_argument("--chunk", type=int, default=200_000)
    ap.add_argument("--shot-every", type=int, default=0)
    ap.add_argument("--tick-per-chunk", type=int, default=1)
    args = ap.parse_args()
    run(args.img, args.max, args.chunk, args.shot_every, args.tick_per_chunk)


if __name__ == "__main__":
    main()
