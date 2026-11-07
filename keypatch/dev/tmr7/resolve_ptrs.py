# -*- coding: utf-8 -*-
"""Resolve the literal-pool pointers used by the TMR7 ISR / tick helpers."""
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
BASE = 0x08004800

with open(IMAGE, "rb") as f:
    DATA = f.read()


def u32(va):
    return struct.unpack_from("<I", DATA, va - BASE)[0]


def u16(va):
    return struct.unpack_from("<H", DATA, va - BASE)[0]


TARGETS = {
    "DAT_08016fa0": 0x08016FA0,   # ISR arg to FUN_08039916/08039910
    "DAT_08016fa4": 0x08016FA4,   # ISR counter base
    "DAT_08016fa8": 0x08016FA8,
    "DAT_08016fac": 0x08016FAC,
    "DAT_08016fb0": 0x08016FB0,
    "DAT_08016fb4": 0x08016FB4,
    "DAT_08016fb8": 0x08016FB8,
    "DAT_08016fbc": 0x08016FBC,
    "DAT_08016fc0": 0x08016FC0,
    "DAT_08016fc4": 0x08016FC4,
    "DAT_08016fc8": 0x08016FC8,
    "DAT_08016fcc": 0x08016FCC,
    "DAT_08016fd0": 0x08016FD0,
    "DAT_08016fd4": 0x08016FD4,
    "DAT_08016fd8": 0x08016FD8,
    "DAT_08016fdc": 0x08016FDC,
    "DAT_08016fe0": 0x08016FE0,
    "DAT_08016fe4": 0x08016FE4,
    "DAT_08016fe8": 0x08016FE8,
    "DAT_08016fec": 0x08016FEC,
    "DAT_08016ff0": 0x08016FF0,
    "DAT_08016ff4": 0x08016FF4,
    "DAT_08016ff8": 0x08016FF8,
    "DAT_08016ffc": 0x08016FFC,
    "DAT_08017000": 0x08017000,
    "DAT_08017004": 0x08017004,
    "DAT_08017008": 0x08017008,
    "DAT_0801700c": 0x0801700C,
    "DAT_08017010": 0x08017010,
    "DAT_08017014": 0x08017014,
    "DAT_080334e8": 0x080334E8,   # get_tick ptr
    "DAT_080334fc": 0x080334FC,   # tick_advance ptr
    "DAT_080334f4": 0x080334F4,
    "DAT_080334f8": 0x080334F8,
    "DAT_080334f0": 0x080334F0,
}

print("%-18s %-10s %-10s  bytes" % ("name", "pool", "value"))
for name, va in TARGETS.items():
    v = u32(va)
    raw = DATA[va - BASE:va - BASE + 4].hex(" ")
    print("%-18s %#010x %#010x  %s" % (name, va, v, raw))

print("\nvector[55+16] =", hex(u32(0x08004800 + 4 * 71)))
print("vector[54+16] =", hex(u32(0x08004800 + 4 * 70)))
