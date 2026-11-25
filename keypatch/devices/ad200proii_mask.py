# -*- coding: utf-8 -*-
"""Godox AD200 Pro II —— 「设置字节遮罩」版（ad200proii_mask）

作者：夕梦

这一版不占菜单项、不看 ID，读设置结构体 0x200009B4 的 +0x13 字节决定同步字来源：

    +0x13 <  2   ->  原厂公式   sync = 50024 / (ID + 1)
    +0x13 >= 2   ->  私有密钥   sync = KEY

字节只在开机初始化时读一次，改完必须重新开关机才生效。

补丁两处，共 44 字节（文件长度不变 387,248 B）：

    0x0800D226  10 B   B.W 0x0805BF1A + 填充（替换原厂 movw r1,#0xC368）
    0x0805BF1A  34 B   判字节 -> 选 KEY 或原厂公式 -> 跳回 0x0800D230

函数体 0x0800D230 起的 40 字节（写 CC2500 reg4/reg5、delay(20)、SCAL 0x33、
delay(1000)）原样保留，所以发送时序与原厂完全一致。

与同设备的另一个模块 `ad200proii.py`（ID=99 版）互不干扰：补丁点不同，可分别生成。
"""

import hashlib
import struct

BASE = 0x08000000
ORIGINAL_SIZE = 387248

IMAGE_NAME = "AD200ProII_V1.05.bin"
IMAGE_SHA256 = "30783860ede87b4bd30678cf820a8f2f6e0d8bc505344d42de1d3561c69f400b"

SYNC_FUNC = 0x0800D224
HEAD_ADDR = 0x0800D226
HEAD_SIZE = 10
HEAD_END = 0x0800D230

CAVE_ADDR = 0x0805BF1A
CAVE_SIZE = 34
LITERAL_ADDR = CAVE_ADDR + 0x1E     # 0x0805BF38，洞穴内的字面量槽

SETTINGS_ADDR = 0x200009B4
MODE_OFFSET = 0x13
MODE_THRESHOLD = 2

ORIG_HEAD = bytes.fromhex("4cf26831")          # movw r1,#0xC368
ORIG_HEAD_PATCH = bytes.fromhex("4ef078be")    # b.w 0x0805BF1A
ORIG_CAVE = b"\xff" * CAVE_SIZE

BRANCH_KEY = bytes.fromhex("b1f783b9")         # 0x0805BF26 -> 0x0800D230
BRANCH_ORIG = bytes.fromhex("b1f77cb9")        # 0x0805BF34 -> 0x0800D230

# 原厂同步字函数的后续时序（未改动，仅用于校验）
TAIL_ADDR = 0x0800D230
TAIL_SIZE = 40

DEVICE = {
    "id": "ad200proii_mask",
    "name": "Godox AD200 Pro II（遮罩版）",
    "kind": "flash",
    "model": "AD200 Pro II",
    "firmware_version": "V1.05",
    "image": "devices/ad200proii/firmware/" + IMAGE_NAME,
    "base": BASE,
    "sha256": IMAGE_SHA256,
    "out_name": "AD200ProII_V1.05_key{key:04X}_mask.bin",
    "container_name": "AD200ProII_V1.05_key{key:04X}_mask_container.bin",
    "notes": "设置结构体 +0x13 >=2 时用私有密钥（菜单蒙版=4），否则原厂公式（蒙版=2）；切档后需重新开关机。（作者：夕梦）",
}

# ---------------------------------------------------------------- 编码


def _off(va: int) -> int:
    return va - BASE


def _t_movw(rd: int, imm16: int) -> bytes:
    """Thumb-2 `movw rd,#imm16`。h1 = 11110 i 10 0100 imm4，h2 = 0 imm3 Rd imm8。"""
    imm16 &= 0xFFFF
    h1 = 0xF240 | (((imm16 >> 11) & 1) << 10) | ((imm16 >> 12) & 0xF)
    h2 = (((imm16 >> 8) & 0x7) << 12) | ((rd & 0xF) << 8) | (imm16 & 0xFF)
    return struct.pack("<HH", h1, h2)


def build_head() -> bytes:
    code = ORIG_HEAD_PATCH + b"\x00\x00" + b"\x00\xbf" * 2
    assert len(code) == HEAD_SIZE, len(code)
    return code


def build_cave(key: int) -> bytes:
    c = b""
    c += b"\x07\x4b"                        # ldr  r3,[pc,#0x1c] -> 0x0805BF38
    c += b"\xdb\x7c"                        # ldrb r3,[r3,#0x13]
    c += b"\x02\x2b"                        # cmp  r3,#2
    c += b"\x03\xd3"                        # blo  0x0805BF2A（原厂公式）
    c += _t_movw(4, key)                    # movw r4,#KEY
    c += BRANCH_KEY                         # b.w  0x0800D230
    c += b"\x4c\xf2\x68\x31"                # movw r1,#0xC368
    c += b"\x40\x1c"                        # adds r0,r0,#1
    c += b"\xb1\xfb\xf0\xf4"                # udiv r4,r1,r0
    c += BRANCH_ORIG                        # b.w  0x0800D230
    c += struct.pack("<I", SETTINGS_ADDR)   # 字面量
    assert len(c) == CAVE_SIZE, len(c)
    return c


# ---------------------------------------------------------------- 补丁


def _check(data: bytes) -> None:
    if len(data) != ORIGINAL_SIZE:
        raise ValueError("固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
                         % (ORIGINAL_SIZE, len(data)))
    digest = hashlib.sha256(data).hexdigest()
    if digest != IMAGE_SHA256:
        raise ValueError("原厂固件 sha256 不匹配（%s），拒绝打补丁" % digest)
    if data[_off(HEAD_ADDR):_off(HEAD_ADDR) + len(ORIG_HEAD)] != ORIG_HEAD:
        raise ValueError("0x%08X 处原厂字节不符（已打过补丁或版本不同）" % HEAD_ADDR)
    if data[_off(CAVE_ADDR):_off(CAVE_ADDR) + CAVE_SIZE] != ORIG_CAVE:
        raise ValueError("代码洞 0x%08X 不是空白，放弃写入" % CAVE_ADDR)


def apply(data: bytes, key: int, full: bool = True):
    """打补丁，返回 (patched, patch_log)。原位、长度不变。"""
    key &= 0xFFFF
    _check(data)
    out = bytearray(data)
    out[_off(HEAD_ADDR):_off(HEAD_ADDR) + HEAD_SIZE] = build_head()
    out[_off(CAVE_ADDR):_off(CAVE_ADDR) + CAVE_SIZE] = build_cave(key)
    log = [
        {"addr": HEAD_ADDR, "offset": _off(HEAD_ADDR),
         "before": ORIG_HEAD.hex(), "after": ORIG_HEAD_PATCH.hex(),
         "desc": "B.W 0x%08X（判设置字节）" % CAVE_ADDR},
        {"addr": CAVE_ADDR, "offset": _off(CAVE_ADDR),
         "before": "ff" * CAVE_SIZE,
         "after": build_cave(key)[:20].hex() + "…",
         "desc": "+0x%02X >= %d → 0x%04X，否则原厂公式"
                 % (MODE_OFFSET, MODE_THRESHOLD, key)},
    ]
    return bytes(out), log


def verify(patched: bytes, key: int, full: bool = True):
    """静态检查 + Unicorn 执行补丁后的真机器码。"""
    key &= 0xFFFF
    res = []

    def check(ok, msg):
        res.append((bool(ok), msg))

    check(len(patched) == ORIGINAL_SIZE, "长度 %d 字节，与原厂一致" % len(patched))
    check(patched[_off(HEAD_ADDR):_off(HEAD_ADDR) + HEAD_SIZE] == build_head(),
          "0x%08X 处 10 B 跳转正确" % HEAD_ADDR)
    check(patched[_off(CAVE_ADDR):_off(CAVE_ADDR) + CAVE_SIZE] == build_cave(key),
          "0x%08X 处 34 B 注入正确" % CAVE_ADDR)

    orig = _original()
    check(patched[_off(TAIL_ADDR):_off(TAIL_ADDR) + TAIL_SIZE]
          == orig[_off(TAIL_ADDR):_off(TAIL_ADDR) + TAIL_SIZE],
          "函数体 0x%08X 起 %d B 与原厂一致" % (TAIL_ADDR, TAIL_SIZE))
    check(patched[_off(LITERAL_ADDR):_off(LITERAL_ADDR) + 4]
          == struct.pack("<I", SETTINGS_ADDR),
          "字面量指向设置结构体 0x%08X" % SETTINGS_ADDR)

    diff = {i for i in range(ORIGINAL_SIZE) if patched[i] != orig[i]}
    allowed = set(range(_off(HEAD_ADDR), _off(HEAD_ADDR) + HEAD_SIZE)) | \
        set(range(_off(CAVE_ADDR), _off(CAVE_ADDR) + CAVE_SIZE))
    check(diff <= allowed, "差异 %d 字节，全部落在补丁区" % len(diff))

    for mode, want_mode in ((0, "原厂"), (MODE_THRESHOLD, "私钥")):
        got = _run_sync(patched, 5, mode)
        want = (50024 // 6) if mode < MODE_THRESHOLD else key
        check(got == want, "设置字节=%d（%s）、ID=5 → 0x%04X（实际 0x%04X）"
              % (mode, want_mode, want, got))

    got99 = _run_sync(patched, 99, MODE_THRESHOLD)
    check(got99 == key, "设置字节=%d、ID=99 → 0x%04X（实际 0x%04X，与 ID 无关）"
          % (MODE_THRESHOLD, key, got99))
    return res


# ---------------------------------------------------------------- 原厂镜像


_ORIG_CACHE = []
_IMAGE_OVERRIDE = [None]


def set_image(path: str) -> None:
    """指定原厂固件路径（独立使用时用；仓库内通常不调用）。"""
    _IMAGE_OVERRIDE[0] = path
    _ORIG_CACHE.clear()


def _resolve_image() -> str:
    import os
    if _IMAGE_OVERRIDE[0]:
        return _IMAGE_OVERRIDE[0]
    here = os.path.dirname(os.path.abspath(__file__))
    root = os.path.dirname(os.path.dirname(here))
    return os.path.join(root, DEVICE["image"].replace("/", os.sep))


def _original() -> bytes:
    if not _ORIG_CACHE:
        with open(_resolve_image(), "rb") as f:
            _ORIG_CACHE.append(f.read())
    return _ORIG_CACHE[0]


def _run_sync(image: bytes, ident: int, mode: int) -> int:
    """在 Unicorn 里执行 0x0800D224，桩掉三个 helper，返回写进 CC2500 的同步字。"""
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_PC,
                                   UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_CPSR)

    FLASH, FLASH_SIZE = 0x08000000, 0x60000
    RAM, RAM_SIZE = 0x20000000, 0x10000
    CCM, CCM_SIZE = 0x10000000, 0x10000
    SENTINEL = 0x20000100
    STACK = 0x20000800
    REG_WRITE = 0x08018EB4
    DELAY = 0x08011962
    STROBE = 0x08018F80

    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(FLASH, FLASH_SIZE)
    uc.mem_map(RAM, RAM_SIZE)
    uc.mem_map(CCM, CCM_SIZE)
    uc.mem_write(FLASH, image)
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
    uc.reg_write(UC_ARM_REG_SP, STACK)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    uc.reg_write(UC_ARM_REG_R0, ident)
    uc.mem_write(SETTINGS_ADDR + MODE_OFFSET, bytes([mode & 0xFF]))
    uc.mem_write(SETTINGS_ADDR + 0x12, bytes([ident & 0xFF]))

    calls = []

    def hook(_uc, address, _size, _user):
        if address == REG_WRITE:
            calls.append((_uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook, begin=REG_WRITE, end=REG_WRITE)
    uc.hook_add(UC_HOOK_CODE, hook, begin=DELAY, end=DELAY)
    uc.hook_add(UC_HOOK_CODE, hook, begin=STROBE, end=STROBE)
    uc.emu_start(SYNC_FUNC | 1, SENTINEL)

    hi = lo = None
    for reg, val in calls:
        if reg == 4:
            hi = val & 0xFF
        elif reg == 5:
            lo = val & 0xFF
    if hi is None or lo is None:
        raise RuntimeError("没有捕获到同步字写入")
    return (hi << 8) | lo
