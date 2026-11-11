# -*- coding: utf-8 -*-
"""X3 Pro S (X3pro_S_v1.11_app.bin) 补丁点与注入区安全性 —— 可复跑的静态复核。

用法（仓库根）：
    tools\\python\\python.exe keypatch\\dev\\x3pros\\scan_checks.py

结论见同目录 NOTES.md。检查项：
  A. 镜像基址 0x08020000（向量表 / Reset_Handler / fault stub / 默认 handler）
  B. 同步字函数 0x0802F0C0（可用 0x5C 字节）与 0x0802F0C6 处 10 字节补丁点
  C. 发送入口 TX4=0x0802EE44 / TX2=0x0802EEAC（入口 4 字节 + radio_wake + 0x7F 帧）
  D. 辅助函数 writeReg / strobe / delay
  E. SYNC1/SYNC0 运行期写点唯一性（reg 4/5）
  F. 设置结构 0x20001768（+1 = ID）与 ID 上限 99
  G. 注入区 0x081CA000..0x081CA3FF（全 0xFF + 无字面量/分支引用）+ 热靴子镜像边界
  H. 初始化表默认 SYNC 值与同步字函数调用点
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
# HERE = <repo>/keypatch/dev/x3pros → 仓库根是上三级
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
IMAGE = os.path.join(ROOT, "devices", "x3pros", "firmware", "X3pro_S_v1.11_app.bin")
BASE = 0x08020000
SHA256 = "4b3465cab7975749dd3d109aa1b6f60ee065402869aa88d7b007325def90e103"
INJ = (0x081CA000, 0x081CA3FF)

d = open(IMAGE, "rb").read()
n = len(d)
res: list[tuple[bool, str]] = []


def chk(ok: bool, msg: str) -> None:
    res.append((bool(ok), msg))
    print(("  [PASS] " if ok else "  [FAIL] ") + msg)


def w(o: int) -> int:
    return struct.unpack_from("<I", d, o)[0]


def h(o: int) -> int:
    return struct.unpack_from("<H", d, o)[0]


def hexb(o: int, ln: int) -> str:
    return d[o:o + ln].hex(" ")


def bl_target(o: int):
    """32 位 BL/B.W 的目标虚拟地址（BL 与 B.W 编码仅 hw2 bit12 不同）。"""
    if o + 4 > n:
        return None
    hw1, hw2 = struct.unpack_from("<HH", d, o)
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) not in (0xD000, 0x9000):
        return None
    s = (hw1 >> 10) & 1
    i1 = 1 - (((hw2 >> 13) & 1) ^ s)
    i2 = 1 - (((hw2 >> 11) & 1) ^ s)
    imm = (s << 24) | (i1 << 23) | (i2 << 22) | ((hw1 & 0x3FF) << 12) | ((hw2 & 0x7FF) << 1)
    if imm & (1 << 24):
        imm -= 1 << 25
    return BASE + o + 4 + imm


print("=" * 78)
print("X3 Pro S 静态复核：", os.path.relpath(IMAGE, ROOT))
print("=" * 78)

# ---- 0) 身份 ----
print("\n[0] 固件身份")
chk(len(d) == 1931868, "文件长度 %d B（期望 1,931,868）" % len(d))
got = hashlib.sha256(d).hexdigest()
chk(got == SHA256, "sha256 %s…（期望 4b3465ca…）" % got[:16])

# ---- A) 基址 ----
print("\n[A] 镜像基址 0x08020000")
sp, reset = w(0), w(4)
chk(sp == 0x20270000, "向量表[0] 初始 SP = 0x%08X（期望 0x20270000）" % sp)
chk(reset == 0x0803139D, "向量表[1] Reset = 0x%08X（期望 0x0803139D）" % reset)
reset_off = (reset & ~1) - BASE
chk(d[reset_off:reset_off + 4] == bytes.fromhex("dff834d0"),
    "Reset_Handler @VA 0x%08X（file 0x%X）= %s（ldr.w sp,[pc,#0x34]）"
    % (reset & ~1, reset_off, hexb(reset_off, 4)))
chk(d[0x113EC:0x113EE] == b"\xfe\xe7", "默认 handler VA 0x080313EC（file 0x113EC）= fe e7（b .）")
stub_ok = True
for i, va in enumerate((0x0803022C, 0x08030230, 0x08030234, 0x08030238, 0x0803023C)):
    o = va - BASE
    ok = d[o:o + 2] == b"\xfe\xe7"
    stub_ok &= ok
    print("      fault stub #%d VA 0x%08X = %s %s" % (i, va, hexb(o, 4), "[ok]" if ok else "[MISMATCH]"))
chk(stub_ok, "NMI/HardFault/MemManage/BusFault/UsageFault stub 全为 b .（VA 0x0803022C/30/34/38/3C）")
bx_ok = True
for i, va in enumerate((0x08030240, 0x08030244, 0x08030248, 0x0803024C)):
    o = va - BASE
    ok = d[o:o + 2] == b"\x70\x47"
    bx_ok &= ok
    print("      stub #%d VA 0x%08X = %s %s" % (i, va, hexb(o, 4), "[ok]" if ok else "[MISMATCH]"))
chk(bx_ok, "SVC/DebugMon/PendSV/SysTick stub 全为 bx lr（VA 0x08030240/44/48/4C）")

# ---- B) 同步字函数 ----
print("\n[B] 同步字函数 0x0802F0C0")
fn = 0x0802F0C0 - BASE
chk(d[fn:fn + 6] == bytes.fromhex("0122034610b5"),
    "函数头 6 字节 = %s（movs r2,#1; mov r3,r0; push {r4,lr}）" % hexb(fn, 6))
PATCH10 = bytes.fromhex("4cf268341344b4fbf3f4")
chk(d[fn + 6:fn + 16] == PATCH10,
    "补丁点 VA 0x0802F0C6 处 10 字节 = %s（movw r4,#0xC368; add r3,r2; udiv r4,r4,r3）" % hexb(fn + 6, 10))
chk(d[fn + 0x5A:fn + 0x5C] == b"\x00\xbf", "函数尾 VA 0x0802F11A = 00 bf（nop 对齐）")
chk(d[0xF11C:0xF11E] == b"\x10\xb5", "下一函数 VA 0x0802F11C 起点 = 10 b5（push {r4,lr}）→ 可用 0x5C 字节")
want = {fn + 0x1E: 0x0802E79C, fn + 0x2E: 0x0802E79C, fn + 0x34: 0x0802B678,
        fn + 0x3A: 0x0802E90C, fn + 0x42: 0x0802B678, fn + 0x48: 0x0802E90C,
        fn + 0x56: 0x0802B678}
bls = {}
for o in range(fn, fn + 0x5C + 1):
    t = bl_target(o)
    if t is not None and o in want:
        bls[o] = t
ok = all(bls.get(a) == t for a, t in want.items())
chk(ok, "函数内 BL：写寄存器 x2 → 0x0802E79C；delay x3 → 0x0802B678；strobe x2 → 0x0802E90C（%s）"
    % {("file+0x%02X" % (a - fn)): "0x%08X" % bls[a] for a in sorted(want) if a in bls})

# ---- C) 发送入口 ----
print("\n[C] 发送入口 TX4 / TX2")
for name, va in (("TX4(4B帧)", 0x0802EE44), ("TX2(2B命令)", 0x0802EEAC)):
    o = va - BASE
    entry = d[o:o + 4]
    chk(entry == bytes.fromhex("10b5fff7"),
        "%s @VA 0x%08X 入口 4 字节 = %s（push {r4,lr}; bl …）" % (name, va, entry.hex(" ")))
    t = bl_target(o + 2)
    chk(t == 0x0802ECE4, "%s 入口第 2 条指令 bl → 0x%08X（radio_wake）" % (name, t or 0))
    chk(0x7F in d[o + 6:o + 0x60], "%s 函数体含 FIFO burst 寄存器 0x7F 立即数" % name)

# ---- D) 辅助函数 ----
print("\n[D] 辅助函数")
chk(d[0xE79C:0xE79E] == b"\x2d\xe9", "writeReg @VA 0x0802E79C 起始 = push.w（file 0xE79C = 2d e9 …）")
chk(d[0xE90C:0xE90E] == b"\x70\xb5", "strobe @VA 0x0802E90C = push {r4,r5,r6,lr}（file 0xE90C = 70 b5）")
chk(d[0xB678:0xB67A] == b"\x1d\x4b", "delay @VA 0x0802B678 是忙等延时函数（file 0xB678 = 1d 4b 1b 78，循环 subs r0,#1）")

# ---- E) SYNC reg4/reg5 写点唯一性 ----
print("\n[E] SYNC1/SYNC0 写点唯一性")
WREG = BASE + 0xE79C


def r0_before(o: int, span: int = 24):
    for x in range(o - 2, max(0, o - span), -2):
        hw = h(x)
        if (hw & 0xFF00) == 0x2000:
            return hw & 0xFF
        if x >= 4:
            h1, h2 = struct.unpack_from("<HH", d, x - 4)
            if (h1 & 0xFBF0) == 0xF240 and ((h2 >> 8) & 0xF) == 0:
                i = (h1 >> 10) & 1
                return ((h1 & 0xF) << 12) | (i << 11) | (((h2 >> 12) & 7) << 8) | (h2 & 0xFF)
    return None


callers = [(o, r0_before(o)) for o in range(0x270, n - 4, 2) if bl_target(o) == WREG]
reg4 = [o for o, r in callers if r == 4]
reg5 = [o for o, r in callers if r == 5]
print("    writeReg 调用点 %d 个；reg4 写点 = %s；reg5 写点 = %s"
      % (len(callers), [hex(x) for x in reg4], [hex(x) for x in reg5]))
chk(set(reg4) == {0xE9D0, 0xF0DE}, "reg4 只有 2 处写入：初始化表 file 0xE9D0（0xC3）与同步字函数 file 0xF0DE")
chk(set(reg5) == {0xE9E2, 0xF0EE}, "reg5 只有 2 处写入：初始化表 file 0xE9E2（0x68）与同步字函数 file 0xF0EE")

# ---- F) 设置结构与 ID 上限 ----
print("\n[F] 设置结构 / ID 上限")
chk(w(0xF1E8) == 0x20001768, "同步字调用点 ldr 字面量 file 0xF1E8 = 0x%08X（设置结构 RAM）" % w(0xF1E8))
chk(d[0x9A14:0x9A16] == b"\x53\x78", "ID 加减 handler（file 0x9A0E 起）：file 0x9A14 = ldrb r3,[r2,#1]（settings[+1]=ID）")
chk(d[0x9A1C:0x9A1E] == b"\x63\x2b", "file 0x9A1C = cmp r3,#0x63（ID 上限 99）")
chk(d[0x9A20:0x9A22] == b"\x63\x23", "file 0x9A20 = movs r3,#0x63（movhi 钳位）")

# ---- G) 注入区 ----
print("\n[G] 注入区 0x081CA000..0x081CA3FF")
o0, o1 = INJ[0] - BASE, INJ[1] - BASE + 1
chk(all(b == 0xFF for b in d[o0:o1]), "注入区 file 0x%X..0x%X 全 0xFF（%d B）" % (o0, o1 - 1, o1 - o0))
i = o0
while i > 0 and d[i - 1] == 0xFF:
    i -= 1
j = o1
while j < n and d[j] == 0xFF:
    j += 1
chk(i == 0x1A9378 and j == 0x1D4000,
    "所在 FF 大段 = file 0x%X..0x%X（VA 0x%08X..0x%08X）" % (i, j, BASE + i, BASE + j))
hits = [(k, w(k)) for k in range(0, n - 4, 4) if INJ[0] <= w(k) <= INJ[1]]
chk(not hits, "全镜像 4 字节字面量无任何值落在注入区（命中 %d 个）" % len(hits))
bt_hits = []
for k in range(0x270, n - 4, 2):
    t = bl_target(k)
    if t is not None and INJ[0] <= t <= INJ[1]:
        bt_hits.append((k, t))
for k in range(0x270, n - 2, 2):
    hw = h(k)
    if (hw & 0xF800) == 0xE000:
        imm = hw & 0x7FF
        if imm & 0x400:
            imm -= 0x800
        t = BASE + k + 4 + (imm << 1)
        if INJ[0] <= t <= INJ[1]:
            bt_hits.append((k, t))
chk(not bt_hits, "全镜像无 B/BL 分支目标落在注入区（命中 %d 个）" % len(bt_hits))
chk(w(0x1D4000) == 0x200004F8 and w(0x1D4004) == 0x080020CD,
    "file 0x1D4000 起是热靴子镜像：SP=0x%08X Reset=0x%08X（独立链接，勿动）"
    % (w(0x1D4000), w(0x1D4004)))

# ---- H) 初始化表与调用点 ----
print("\n[H] 初始化表默认值 / 同步字函数调用点")
chk(d[0xE9CC:0xE9CE] == b"\x04\x20", "初始化表 reg4 写点前 file 0xE9CC = movs r0,#4（数据 0xC3）")
chk(d[0xE9C2:0xE9C4] == b"\xc3\x22", "初始化表 reg4 数据 file 0xE9C2 = movs r2,#0xC3")
chk(d[0xE9D4:0xE9D6] == b"\x68\x22", "初始化表 reg5 数据 file 0xE9D4 = movs r2,#0x68")
sync_callers = [o for o in range(0x270, n - 4, 2) if bl_target(o) == BASE + 0xF0C0]
print("    bl 0x0802F0C0 调用点 file: %s" % [hex(x) for x in sync_callers])
chk(len(sync_callers) == 6, "同步字函数共 6 个直接调用点（上电/改 ID/应用设置路径）")

# ---- 汇总 ----
print("\n" + "=" * 78)
fail = [m for ok, m in res if not ok]
print("结论：%d 项检查，%d PASS / %d FAIL" % (len(res), len(res) - len(fail), len(fail)))
for m in fail:
    print("  FAIL:", m)
print("=" * 78)
sys.exit(0 if not fail else 1)
