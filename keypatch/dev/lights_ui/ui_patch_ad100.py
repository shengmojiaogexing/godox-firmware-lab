# -*- coding: utf-8 -*-
"""AD100 Pro V1.2 —— C.Fn 菜单新增「KEY」私有密钥开关项（UI 原型补丁）。

用户可见语义
------------
C.Fn 菜单（MENU 键进入）新增第 7 项（序号 6）「KEY」：
    KEY = OFF  -> 普通模式（与核心补丁约定：读设置字节 0x20000486 == 0）
    KEY = ON   -> 私有密钥模式（0x20000486 == 1）

存储（厂商预留槽位，无需新增存储代码）
--------------------------------------
    RAM 0x20000486  == 32 字节参数记录偏移 30
    记录：0x0801F000 + slot*100（magic 0x5C），slot 由 0x20000120 的保存任务维护。
    默认/上限表 0x0800DA2E[6] = 1（本来就是 ON/OFF 预留项）
    加载 0x0800AD44→0x0800CE6C、保存 0x08004690→0x0800B588/0x08005EF4 已覆盖第 6 字节。

补丁内容（全部原位，文件长度不变）
----------------------------------
P1. 0x08003F10..0x08003F88（120B 头部重写）：
    删掉纯装饰的 "Ver 1.2" 绘制，保留 "C.Fn" 标题（压缩为 36B，依赖 P4）；
    腾出的区域放注入代码：page>=2（选中项 6）时画 "KEY"（K=0x14/E=0x0E/Y=0x22）
    与值区域 ON/OFF（O=0x18/N=0x17/F=0x0F），读 0x20000486，然后跳公共高亮 0x08004318。
P2. 0x08003FA2：page>=2 原 `b 0x08004318` → `b 0x08003F12`。
P3. 导航 wrap：0x0800490C `cmp r0,#5`→`#6`；0x08008A8A `movs r1,#5`→`#6`。
P4. 0x08004F30（8x16 字形绘制）收尾改恢复 r0-r3（ABI 允许、对其它调用者透明）：
    `add sp,#0x14`→`add sp,#4`；`pop {r4,r5,r6,r7,pc}`→`pop {r0-r7,pc}`。

接口（给 fw-ad100 核心补丁）
----------------------------
密钥标志 = RAM 字节 **0x20000486**（flash 记录偏移 30）；0=普通，1=私有密钥模式。
"""
from __future__ import annotations

import struct

BASE = 0x08003000
IMAGE_SIZE = 50248

HEADER_START = 0x08003F10
HEADER_END = 0x08003F88
HEADER_SIZE = HEADER_END - HEADER_START            # 120

ORIG_HEADER = bytes.fromhex(
    "582102231f22002001f00af8"
    "5f2102232822002001f004f8"
    "662102233522002000f0feff"
    "6f2102230122002000f0f8ff"
    "762102234022002000f0f2ff"
    "022379211a46002000f0ecff"
    "002102230c22084600f0e6ff"
    "072102234022002000f0e0ff"
    "0a2102230f22002000f0daff"
    "112102233122002000f0d4ff")

PAGE_BRANCH = 0x08003FA2
PAGE_BRANCH_ORIG = bytes.fromhex("b9e1")
WRAP_PLUS = 0x0800490C
WRAP_PLUS_ORIG = bytes.fromhex("0528")
WRAP_MINUS = 0x08008A8A
WRAP_MINUS_ORIG = bytes.fromhex("0521")
GLYPH_EPILOGUE = 0x08004F88
GLYPH_EPILOGUE_ORIG = bytes.fromhex("05b0f0bd")

GLYPH_FN = 0x08004F30           # draw_glyph16(page, x, glyph_idx, mode)
HIGHLIGHT = 0x08004318          # 选中行反白 + 边框
FLAG_ADDR = 0x20000486          # 密钥标志
FLAG_RECORD_OFFSET = 30

# 8x16 字库 0x0800DEC6：'0'=0..'9'=9, 'A'=0x0A..'Z'=0x23
G_K, G_E, G_Y = 0x14, 0x0E, 0x22
G_O, G_N, G_F = 0x18, 0x17, 0x0F
TITLE_GLYPHS = ((0x00, 0x0C), (0x07, 0x40), (0x0A, 0x0F), (0x11, 0x31))


# ---- Thumb 编码 -----------------------------------------------------------
def _movs(rd, imm8):
    return struct.pack("<H", 0x2000 | ((rd & 7) << 8) | (imm8 & 0xFF))


def _cmp(rn, imm8):
    return struct.pack("<H", 0x2800 | ((rn & 7) << 8) | (imm8 & 0xFF))


def _ldrb(rt, rn, imm5):
    return struct.pack("<H", 0x7800 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rt & 7))


def _beq(off_bytes):
    assert off_bytes % 2 == 0 and 0 <= off_bytes // 2 <= 0xFF, off_bytes
    return struct.pack("<H", 0xD000 | ((off_bytes // 2) & 0xFF))


def _b(off_bytes):
    assert off_bytes % 2 == 0 and -0x800 <= off_bytes // 2 <= 0x7FF, off_bytes
    return struct.pack("<H", 0xE000 | ((off_bytes // 2) & 0x7FF))


def _bl(addr, target):
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target), off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = (i1 if s else ((~i1) & 1))
    j2 = (i2 if s else ((~i2) & 1))
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                       0xD000 | (j1 << 13) | (j2 << 11) | imm11)


class _Asm:
    def __init__(self, base):
        self.base = base
        self.code = bytearray()
        self.labels = {}
        self.fixups = []

    def here(self):
        return self.base + len(self.code)

    def label(self, name):
        self.labels[name] = self.here()

    def emit(self, b):
        self.code += b

    def bl(self, target):
        self.emit(_bl(self.here(), target))

    def branch(self, kind, label):
        self.fixups.append((len(self.code), kind, label))
        self.emit(b"\x00\x00")

    def finish(self):
        out = bytearray(self.code)
        for off, kind, label in self.fixups:
            target = self.labels[label]
            delta = target - (self.base + off + 4)
            if kind == "beq":
                out[off:off + 2] = _beq(delta)
            else:
                out[off:off + 2] = _b(delta)
        return bytes(out)


def build_header_patch() -> bytes:
    """生成 0x08003F10..0x08003F88 的 120 字节替换内容。"""
    a = _Asm(HEADER_START)
    a.branch("b", "title")                  # 0x08003F10: 正常页面流程跳到标题
    a.label("helper")                       # 0x08003F12: page>=2 注入代码
    # "KEY" 标签（page=2, mode=2）
    a.emit(_movs(0, 2))
    a.emit(_movs(3, 2))
    for x, g in ((0x0A, G_K), (0x11, G_E), (0x18, G_Y)):
        a.emit(_movs(1, x))
        a.emit(_movs(2, g))
        a.bl(GLYPH_FN)
    # 值区域公共 'O'（page=4, mode=1）
    a.emit(_movs(0, 4))
    a.emit(_movs(3, 1))
    a.emit(_movs(1, 0x4A))
    a.emit(_movs(2, G_O))
    a.bl(GLYPH_FN)
    # 读 0x20000486（r5=0x20000480 由 draw 入口设置；用 r7 避免破坏 page 寄存器 r0）
    a.emit(_ldrb(7, 5, FLAG_RECORD_OFFSET - 24))
    a.emit(_cmp(7, 0))
    a.branch("beq", "off")
    a.emit(_movs(1, 0x51))
    a.emit(_movs(2, G_N))
    a.bl(GLYPH_FN)
    a.branch("b", "done")
    a.label("off")
    a.emit(_movs(1, 0x51))
    a.emit(_movs(2, G_F))
    a.bl(GLYPH_FN)
    a.emit(_movs(1, 0x58))
    a.emit(_movs(2, G_F))
    a.bl(GLYPH_FN)
    a.label("done")
    a.emit(_b(HIGHLIGHT - (a.here() + 4)))
    # "C.Fn" 标题（依赖 P4 保持 r0-r3，每字形 8B）
    a.label("title")
    a.emit(_movs(0, 0))
    a.emit(_movs(3, 2))
    for i, (x, g) in enumerate(TITLE_GLYPHS):
        a.emit(_movs(1, x))
        a.emit(_movs(2, g))
        a.bl(GLYPH_FN)
    code = a.finish()
    assert a.here() <= HEADER_END, (hex(a.here()), hex(HEADER_END))
    if len(code) < HEADER_SIZE:
        code += b"\x00\xbf" * ((HEADER_SIZE - len(code)) // 2)
    assert len(code) == HEADER_SIZE, len(code)
    return code


# ---- 打补丁 ---------------------------------------------------------------
def _off(addr):
    return addr - BASE


def apply_ui(data: bytes, opts: dict | None = None) -> tuple[bytes, list[dict]]:
    """把原厂 AD100 Pro V1.2 镜像打上 C.Fn「KEY」菜单补丁。

    参数
    ----
    data : 原厂镜像（必须 50248 字节，基址 0x08003000）
    opts : 可选，保留扩展；目前支持
             {"flag_offset": 6}  # KEY 值在设置数组 0x20000480 中的偏移（0..7，默认 6）

    返回 (patched_bytes, patch_log)；patch_log 每项 {addr, offset, before, after, desc}。
    """
    opts = opts or {}
    flag_index = int(opts.get("flag_offset", FLAG_RECORD_OFFSET - 24))
    if not 0 <= flag_index <= 7:
        raise ValueError("flag_offset 必须在 0..7（默认 6 = 厂商预留槽位）")
    if flag_index != FLAG_RECORD_OFFSET - 24:
        raise NotImplementedError("原型只支持预留槽位 6（记录偏移 30）；其它槽位需重新生成注入代码")
    if len(data) != IMAGE_SIZE:
        raise ValueError("AD100 Pro V1.2 固件长度应为 %d，实际 %d（版本不符，拒绝打补丁）"
                         % (IMAGE_SIZE, len(data)))
    log = []
    buf = bytearray(data)

    def patch(addr, orig, new, desc):
        o = _off(addr)
        cur = bytes(buf[o:o + len(orig)])
        if cur != orig:
            raise ValueError("补丁点 0x%08X 原始字节不符（固件版本不符）\n  期望 %s\n  实际 %s"
                             % (addr, orig.hex(" "), cur.hex(" ")))
        assert len(new) == len(orig)
        buf[o:o + len(orig)] = new
        log.append({"addr": addr, "offset": o, "before": orig.hex(" "),
                    "after": new.hex(" "), "desc": desc})

    patch(HEADER_START, ORIG_HEADER, build_header_patch(),
          "重写 C.Fn 头部 120B：删 'Ver 1.2'，注入 page2 KEY 标签 + ON/OFF 绘制，保留 'C.Fn'")
    patch(PAGE_BRANCH, PAGE_BRANCH_ORIG, _b(0x08003F12 - (PAGE_BRANCH + 4)),
          "page>=2 分支改为进入 KEY 绘制代码 0x08003F12")
    patch(WRAP_PLUS, WRAP_PLUS_ORIG, _cmp(0, 6),
          "C.Fn 选择项 +1 上限 5→6（0x08004880）")
    patch(WRAP_MINUS, WRAP_MINUS_ORIG, _movs(1, 6),
          "C.Fn 选择项 -1 回绕 5→6（0x08008A5C）")
    patch(GLYPH_EPILOGUE, GLYPH_EPILOGUE_ORIG,
          struct.pack("<H", 0xB001) + struct.pack("<H", 0xBDFF),
          "0x08004F30 收尾恢复 r0-r3（add sp,#4; pop {r0-r7,pc}），供注入代码复用 page/mode")
    return bytes(buf), log


# ---- 静态验证 -------------------------------------------------------------
ZONES = ((HEADER_START, HEADER_SIZE), (PAGE_BRANCH, 2), (WRAP_PLUS, 2),
         (WRAP_MINUS, 2), (GLYPH_EPILOGUE, 4))


def verify(patched: bytes) -> list[tuple[bool, str]]:
    res: list[tuple[bool, str]] = []
    res.append((len(patched) == IMAGE_SIZE,
                "长度保持 %d 字节（官方更新文件同长）" % len(patched)))

    def check(addr, expect, label):
        o = _off(addr)
        got = bytes(patched[o:o + len(expect)])
        ok = got == expect
        res.append((ok, "%s @0x%08X %s" % (
            label, addr, "OK" if ok else "不符 期望 %s 实际 %s"
            % (expect.hex(" "), got.hex(" ")))))

    check(HEADER_START, build_header_patch(), "C.Fn 头部重写")
    check(PAGE_BRANCH, _b(0x08003F12 - (PAGE_BRANCH + 4)), "page>=2 分支")
    check(WRAP_PLUS, _cmp(0, 6), "选择项 +1 上限")
    check(WRAP_MINUS, _movs(1, 6), "选择项 -1 回绕")
    check(GLYPH_EPILOGUE, struct.pack("<H", 0xB001) + struct.pack("<H", 0xBDFF),
          "字形函数 r0-r3 保持")

    b = bytearray(patched)
    for addr, origbytes in ((HEADER_START, ORIG_HEADER), (PAGE_BRANCH, PAGE_BRANCH_ORIG),
                            (WRAP_PLUS, WRAP_PLUS_ORIG), (WRAP_MINUS, WRAP_MINUS_ORIG),
                            (GLYPH_EPILOGUE, GLYPH_EPILOGUE_ORIG)):
        o = _off(addr)
        b[o:o + len(origbytes)] = origbytes
    restored = bytes(b)
    diffs = [i for i in range(len(patched)) if patched[i] != restored[i]]
    bad = [BASE + i for i in diffs
           if not any(_off(a) <= i < _off(a) + n for a, n in ZONES)]
    res.append((not bad, "补丁区之外无字节变化（差异 %d 字节，越界 %s）"
                % (len(diffs), ["0x%08X" % x for x in bad[:8]])))
    return res


if __name__ == "__main__":
    import os
    import sys

    root = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
    src = os.path.join(root, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
    out = os.path.join(os.path.dirname(os.path.abspath(__file__)), "out")
    os.makedirs(out, exist_ok=True)
    data = open(src, "rb").read()
    patched, log = apply_ui(data)
    dst = os.path.join(out, "AD100pro_V1.2_keyUI.bin")
    open(dst, "wb").write(patched)
    print("patched -> %s" % dst)
    for e in log:
        print("  0x%08X: %s" % (e["addr"], e["desc"]))
    ok = True
    for good, msg in verify(patched):
        print("  [%s] %s" % ("PASS" if good else "FAIL", msg))
        ok = ok and good
    print("verdict:", "PASS" if ok else "FAIL")
    sys.exit(0 if ok else 1)
