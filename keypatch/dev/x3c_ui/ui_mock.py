# -*- coding: utf-8 -*-
"""X3-C 新 UI 的**设计稿**渲染器（不是固件渲染！）。

用途：在固件端照片/真机渲染拿不到之前，先把"快捷面板多两个磁贴（密钥 / 广播）
+ 广播页（全频道 / 全频道+全ID）"的视觉效果做出来给 Lead/用户确认。
坐标与配色全部照抄原厂 `FUN_080079E8` 的常量（368×448、底部页签
(0x0B,0x155,0xA8,0x5C)/(0xBB,0x155,0xA8,0x5C)、磁贴 (0x28,0x28,0x6C,0x6C) 等），
汉字用 Lead 的 24×24 点阵，所以观感与真机一致。

输出：shots/mock_home.png、shots/mock_broadcast.png

用法：tools\\python\\python.exe ui_mock.py
"""
from __future__ import annotations

import os
import struct
import sys
import zlib

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
OUT = os.path.join(HERE, "shots")
GLYPHS24 = os.path.join(ROOT, "keypatch", "dev", "lead", "glyphs24")

W, H = 368, 448

# 原厂配色（从截图取样：深灰底 / 白色图标 / 粉色描边 / 橙色高亮）
BG = (0x10, 0x10, 0x10)
PANEL = (0x1E, 0x1E, 0x1E)
TILE = (0x2A, 0x2A, 0x2A)
TILE_ON = (0x3A, 0x6E, 0xA8)
WHITE = (0xFF, 0xFF, 0xFF)
GREY = (0x9A, 0x9A, 0x9A)
ACCENT = (0xFF, 0x5A, 0x28)
PINK = (0xC8, 0x9A, 0xB0)


class Canvas:
    def __init__(self, w=W, h=H):
        self.w, self.h = w, h
        self.px = bytearray(w * h * 3)

    def put(self, x, y, c):
        if 0 <= x < self.w and 0 <= y < self.h:
            o = (y * self.w + x) * 3
            self.px[o:o + 3] = bytes(c)

    def rect(self, x, y, w, h, c, r=0):
        for yy in range(y, y + h):
            for xx in range(x, x + w):
                if r:
                    # 圆角裁剪
                    cx = min(max(xx - x, 0), w - 1)
                    cy = min(max(yy - y, 0), h - 1)
                    dx = min(cx, w - 1 - cx)
                    dy = min(cy, h - 1 - cy)
                    if dx < r and dy < r:
                        if (r - dx) ** 2 + (r - dy) ** 2 > r * r:
                            continue
                self.put(xx, yy, c)

    def circle(self, cx, cy, rad, c):
        for y in range(cy - rad, cy + rad + 1):
            for x in range(cx - rad, cx + rad + 1):
                if (x - cx) ** 2 + (y - cy) ** 2 <= rad * rad:
                    self.put(x, y, c)

    def glyph(self, x, y, rows, c=WHITE, scale=1):
        """rows: 24 个 u32，MSB 在左，bit=1 亮。"""
        for gy, row in enumerate(rows):
            for gx in range(24):
                if (row >> (23 - gx)) & 1:
                    for sy in range(scale):
                        for sx in range(scale):
                            self.put(x + gx * scale + sx, y + gy * scale + sy, c)

    def ascii(self, x, y, text, c=WHITE, scale=2):
        """5x7 位图 ASCII（够画 KEY/BROADCAST 等标签）。"""
        for ch in text:
            font = FONT5x7.get(ch.upper())
            if font is None:
                x += 6 * scale
                continue
            for ry, bits in enumerate(font):
                for rx in range(5):
                    if (bits >> (4 - rx)) & 1:
                        for sy in range(scale):
                            for sx in range(scale):
                                self.put(x + rx * scale + sx, y + ry * scale + sy, c)
            x += 6 * scale

    def save(self, path):
        raw = bytearray()
        for y in range(self.h):
            raw.append(0)
            raw += self.px[y * self.w * 3:(y + 1) * self.w * 3]

        def chunk(tag, data):
            return (struct.pack(">I", len(data)) + tag + data
                    + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF))

        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, "wb") as f:
            f.write(b"\x89PNG\r\n\x1a\n")
            f.write(chunk(b"IHDR", struct.pack(">IIBBBBB", self.w, self.h, 8, 2, 0, 0, 0)))
            f.write(chunk(b"IDAT", zlib.compress(bytes(raw), 6)))
            f.write(chunk(b"IEND", b""))
        return path


# 5x7 ASCII 字体（ASCII 0x20-0x5F 常用子集）
FONT5x7 = {
    "A": [0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11],
    "B": [0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E],
    "C": [0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E],
    "D": [0x1E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1E],
    "E": [0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F],
    "F": [0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10],
    "G": [0x0E, 0x11, 0x10, 0x17, 0x11, 0x11, 0x0F],
    "H": [0x11, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11],
    "I": [0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E],
    "K": [0x11, 0x12, 0x14, 0x18, 0x14, 0x12, 0x11],
    "L": [0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F],
    "N": [0x11, 0x19, 0x15, 0x13, 0x11, 0x11, 0x11],
    "O": [0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E],
    "P": [0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10],
    "R": [0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11],
    "S": [0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E],
    "T": [0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04],
    "U": [0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E],
    "Y": [0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04],
    " ": [0, 0, 0, 0, 0, 0, 0],
    "+": [0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00],
    ":": [0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00],
    "0": [0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E],
    "1": [0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E],
    "2": [0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F],
    "3": [0x1F, 0x02, 0x04, 0x02, 0x01, 0x11, 0x0E],
    "4": [0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02],
    "5": [0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E],
    "6": [0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E],
    "7": [0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08],
    "8": [0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E],
    "9": [0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C],
}


def load_glyph(name: str):
    rows = []
    with open(os.path.join(GLYPHS24, name + ".hex"), encoding="utf-8-sig") as f:
        for line in f:
            line = line.strip().lstrip("\ufeff")
            if line:
                rows.append(int(line, 16))
    return rows


G_MI = load_glyph("u5BC6")     # 密
G_YAO = load_glyph("u94A5")    # 钥
G_GUANG = load_glyph("u5E7F")  # 广
G_BO = load_glyph("u64AD")     # 播
G_QUAN = load_glyph("u5168")   # 全


def draw_glyph2(c, x, y, g1, g2, color=WHITE):
    c.glyph(x, y, g1, color)
    c.glyph(x + 26, y, g2, color)


def mock_home():
    """快捷面板 + 两个新磁贴（密钥 / 广播）。布局照抄原厂：磁贴 108×108。"""
    c = Canvas()
    c.rect(0, 0, W, H, BG)
    c.rect(8, 8, W - 16, H - 16, PANEL, r=18)
    c.rect(8, 8, W - 16, H - 16, PINK, r=18) if False else None
    # 顶部两个圆形磁贴：声音 / 造型灯（原厂）
    c.circle(40 + 54, 40 + 54, 54, TILE)
    c.rect(40 + 34, 40 + 40, 12, 28, WHITE)
    c.rect(40 + 50, 40 + 34, 8, 40, WHITE)          # 简化喇叭
    c.circle(40 + 84, 40 + 54, 54, TILE)
    c.circle(40 + 84, 40 + 46, 14, WHITE)           # 简化灯泡
    c.rect(40 + 80, 40 + 60, 8, 14, WHITE)
    # 原厂宽磁贴：锁
    c.rect(40, 172, 264, 108, TILE, r=54)
    c.rect(40 + 124, 172 + 34, 16, 24, WHITE)
    # ---- 新增：密钥 / 广播 两个磁贴（本次补丁要加的） ----
    c.rect(40, 288, 128, 44, TILE, r=22)
    draw_glyph2(c, 40 + 16, 288 + 10, G_MI, G_YAO, WHITE)
    c.rect(176, 288, 128, 44, TILE, r=22)
    draw_glyph2(c, 176 + 16, 288 + 10, G_GUANG, G_BO, WHITE)
    # 底部页签：原厂两个 + 新增 BROADCAST（三按钮布局，宽 0x6E=110）
    for i, label in enumerate(("频闪", "设置", "广播")):
        x = 11 + i * 118
        c.rect(x, 341, 110, 92, TILE, r=30)
        if i == 2:
            draw_glyph2(c, x + 30, 341 + 34, G_GUANG, G_BO, ACCENT)
    # 原厂两个页签里已经有中文（频闪/设置），这里用 ASCII 占位示意
    c.ascii(11 + 8, 341 + 40, "MULTI", GREY, 1)
    c.ascii(11 + 126, 341 + 40, "SET", GREY, 1)
    return c


def mock_broadcast():
    """广播页：范围选择（全频道 / 全频道+全ID）+ 开始/取消。"""
    c = Canvas()
    c.rect(0, 0, W, H, BG)
    c.rect(8, 8, W - 16, H - 16, PANEL, r=18)
    # 标题：广播（广 播）
    draw_glyph2(c, 20, 22, G_GUANG, G_BO, WHITE)
    # 选项 1：全频道
    c.rect(20, 70, W - 40, 64, TILE, r=16)
    c.glyph(36, 88, G_QUAN, WHITE)
    draw_glyph2(c, 66, 88, G_QUAN, G_QUAN, WHITE) if False else None
    c.ascii(66, 92, "ALL CH 1-32", WHITE, 2)
    c.circle(20 + W - 40 - 34, 70 + 32, 16, ACCENT)          # 选中圆点
    # 选项 2：全频道 + 全ID
    c.rect(20, 148, W - 40, 64, TILE, r=16)
    c.glyph(36, 166, G_QUAN, WHITE)
    c.ascii(66, 170, "ALL CH+ID", WHITE, 2)
    c.circle(20 + W - 40 - 34, 148 + 32, 16, WHITE)
    # 说明文字
    c.ascii(24, 236, "POWER: USE CURRENT", GREY, 1)
    c.ascii(24, 252, "MODEL LAMP: USE CURRENT", GREY, 1)
    c.ascii(24, 276, "TAKES ~1S / ~15S", GREY, 1)
    # 开始按钮
    c.rect(20, 340, W - 40, 64, ACCENT, r=32)
    draw_glyph2(c, 20 + 130, 340 + 20, G_GUANG, G_BO, WHITE)
    # 取消
    c.ascii(20 + 12, 300, "CANCEL", GREY, 2)
    return c


def main():
    p1 = mock_home().save(os.path.join(OUT, "mock_home.png"))
    p2 = mock_broadcast().save(os.path.join(OUT, "mock_broadcast.png"))
    print("设计稿:", p1)
    print("设计稿:", p2)


if __name__ == "__main__":
    main()
