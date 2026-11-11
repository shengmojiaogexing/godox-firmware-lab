# -*- coding: utf-8 -*-
"""X3-S 私有密钥补丁 —— 整机模拟器端到端测试（复用 X3-C 的 Unicorn 整机模型）。

X3-S 与 X3-C 是同一套代码的机型构建（同 AT32F435 平台、同 RAM 布局），因此可以直接用
`devices/x3c/emu/machine.py` 加载 **X3-S 原厂/补丁镜像** 跑真实固件。

本脚本做三件事：
  1. **BSS 安全 RAM 复核**：原厂 X3-S 启动时对 0x20001080（KEY_ON）只有 BSS 清零一次写；
  2. **整机启动 + 真实按键扫描**：加载补丁镜像启动到 main，桩替换「读键 / 低层发送 /
     写 CC2500 寄存器」，反复调用真实 `KEYSCAN_FN`（含注入的手势逻辑），断言：
        * 短按不切 KEY_ON；
        * 长按（去抖 3 + 阈值 30 = 32 次扫描）KEY_ON 0->1，继续按住保持 1；
        * 松开清零、再次长按切回 0；
  3. **端到端双发**：在启动完成的整机上调用真实 `send_short` / `send_property`，断言：
        * KEY_ON=0 -> 原厂单发；
        * KEY_ON=1 -> 先写 SYNC=KEY 发一次，再写 SYNC=50024/(ID+1) 发一次；
        * 白名单外 cmd 保持单发。

运行（在 <repo> 根目录下）::

    tools\\python\\python.exe keypatch\\dev\\x3s\\keybtn_runtime.py
    # 输出同时写入 keypatch/dev/x3s/keybtn_runtime.log
"""
from __future__ import annotations

import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
sys.path.insert(0, EMU)
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

from machine import Machine, APP_BASE  # noqa: E402
from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_WRITE  # noqa: E402
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1,  # noqa: E402
                               UC_ARM_REG_R2, UC_ARM_REG_LR, UC_ARM_REG_PC,
                               UC_ARM_REG_SP)

import devices.x3s as x3s  # noqa: E402

KEY = 0xD798
MAIN = 0x08033D08                 # X3-S main（由 key scan 调用点 0x08034202 反推）
SENTINEL = 0x20046000
STACK = 0x20047000
ORIG = os.path.join(ROOT, "devices", "x3s", "firmware", "X3_S_v1.25_app.bin")
PATCHED = os.path.join(ROOT, "keypatch", "out", "X3_S_v1.25_app_keyD798.bin")
LOG = os.path.join(HERE, "keybtn_runtime.log")

lines: list[str] = []
ok_all = True


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


def check(name: str, ok: bool, detail: str = "") -> bool:
    global ok_all
    ok_all &= bool(ok)
    out("  [%s] %s%s" % ("PASS" if ok else "FAIL", name, ("  | " + detail) if detail else ""))
    return ok


# ---------------------------------------------------------------------------
# 1) BSS 安全 RAM 复核
# ---------------------------------------------------------------------------
def check_bss_safe() -> None:
    out("=" * 78)
    out("1) BSS 安全 RAM 复核（原厂 X3-S 启动写追踪）")
    out("=" * 78)
    m = Machine(image=ORIG, image_base=APP_BASE, trace=False,
                log_fn=lambda s: None, hardfault=True)
    hits: list[tuple] = []

    def cb(uc, access, address, size, value, user):
        if 0x20001080 <= address < 0x200010A0:
            hits.append((address, size, value, uc.reg_read(UC_ARM_REG_PC)))

    m.uc.hook_add(UC_HOOK_MEM_WRITE, cb)
    m.boot()
    m.interrupt_after_chunk = False
    m.run(max_insns=300000)
    writes = [(a, v, pc) for (a, s, v, pc) in hits if v != 0]
    bss_clear = [(a, pc) for (a, s, v, pc) in hits if v == 0]
    check("0x20001080..0x2000109F 在启动 30 万条指令内只有 BSS 清零写（非 0 写 = %d 次）"
          % len(writes), len(writes) == 0,
          "示例: %s" % (["%08X v=0x%X pc=%08X" % (a, v, pc) for a, v, pc in writes[:4]]))
    pcs = sorted({pc for (_a, pc) in bss_clear})
    check("BSS 清零写来自同一处（PC=%s）" % ", ".join("0x%08X" % p for p in pcs),
          len(pcs) == 1, "共 %d 次写" % len(bss_clear))


# ---------------------------------------------------------------------------
# 2/3) 整机运行时测试
# ---------------------------------------------------------------------------
class Bench:
    def __init__(self, image: str):
        self.m = Machine(image=image, image_base=APP_BASE, trace=False,
                         log_fn=lambda s: None, hardfault=True)
        self.m.boot()
        self.m.interrupt_after_chunk = False
        stop = self.m.run_until(MAIN, max_insns=2_000_000)
        if (self.m.reg("PC") & ~1) != MAIN:
            raise RuntimeError("未停在 main（%s, pc=0x%08X）"
                               % (stop, self.m.reg("PC") & ~1))
        self.m.uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
        self.keys = {"k0": False}
        self.rec = {"low_short": 0, "low_prop": 0, "wreg": []}
        self._install_hooks()
        for ram, size in ((x3s.BTN_RAM5_CNT16, 2), (x3s.BTN_RAM5_STREAK, 1),
                          (x3s.KEY_ON_FLAG_RAM, 1), (x3s.ID_RAM, 1),
                          (x3s.ID_OFF_FLAG_RAM, 1)):
            self.m.uc.mem_write(ram, b"\x00" * size)
        self.m.uc.mem_write(x3s.ID_RAM, b"\x05")

    def _install_hooks(self) -> None:
        m = self.m

        def cb(uc, address, size, _user):
            if address == x3s.READ_KEY:
                mask = uc.reg_read(UC_ARM_REG_R1)
                pressed = (mask == x3s.BTN0_MASK) and self.keys["k0"]
                uc.reg_write(UC_ARM_REG_R0, 0 if pressed else 1)
                uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
                return
            if address == x3s.LOW_SEND_SHORT:
                self.rec["low_short"] += 1
            elif address == x3s.LOW_SEND_PROP:
                self.rec["low_prop"] += 1
            elif address == x3s.WRITE_REG:
                self.rec["wreg"].append((uc.reg_read(UC_ARM_REG_R0),
                                         uc.reg_read(UC_ARM_REG_R1)))
            else:
                return
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))

        for a in (x3s.READ_KEY, x3s.LOW_SEND_SHORT, x3s.LOW_SEND_PROP, x3s.WRITE_REG):
            m.uc.hook_add(UC_HOOK_CODE, cb, None, a, a + 4)

    def _call(self, entry: int, args: tuple, regs: int = 2) -> bool:
        m = self.m
        m.uc.reg_write(UC_ARM_REG_SP, STACK)
        m.uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
        m.uc.reg_write(UC_ARM_REG_PC, entry | 1)
        for i, v in enumerate(args):
            m.uc.reg_write((UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2)[i], v)
        m.stop_reason = None
        m._bp_skip = None
        m.run_until(SENTINEL, max_insns=300000)
        return (m.reg("PC") & ~1) == SENTINEL

    def scan(self, pressed: bool) -> bool:
        """执行一次真实按键扫描里的注入手势段（停在原函数体入口 0x0800CBD4）。

        用真实固件入口 + 真实注入代码 + 真实外设模型（读键用桩），原厂函数体不执行
        （外层已由 Unicorn 调用级测试与启动回归覆盖）。
        """
        m = self.m
        self.keys["k0"] = pressed
        m.uc.reg_write(UC_ARM_REG_SP, STACK)
        m.uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
        m.uc.reg_write(UC_ARM_REG_PC, x3s.KEYSCAN_FN | 1)
        m.stop_reason = None
        m._bp_skip = None
        m.run_until(x3s.KEYSCAN_RESUME, max_insns=100000)
        return (m.reg("PC") & ~1) == x3s.KEYSCAN_RESUME

    def key_on(self) -> int:
        return self.m.uc.mem_read(x3s.KEY_ON_FLAG_RAM, 1)[0]

    def key_on_write(self, value: int) -> None:
        self.m.uc.mem_write(x3s.KEY_ON_FLAG_RAM, bytes([value & 0xFF]))

    def reset_rec(self) -> None:
        self.rec = {"low_short": 0, "low_prop": 0, "wreg": []}


def test_button(b: Bench) -> None:
    out("\n" + "=" * 78)
    out("2) 整机运行时：真实按键扫描 + 注入手势（长按切 KEY_ON）")
    out("=" * 78)
    need = x3s.BTN_KEYBTN5_DEBOUNCE - 1 + x3s.BTN_KEYBTN5_TICKS1   # 2 + 30 = 32
    ok = True
    for _ in range(need - 1):
        ok &= b.scan(True)
    mid = b.key_on()
    ok &= b.scan(True)
    after = b.key_on()
    ok &= b.scan(True)
    stable = b.key_on()
    check("长按 %d 次扫描：第 %d 次前 KEY_ON=0，第 %d 次切到 1，继续按住保持 1"
          % (need + 1, need - 1, need),
          ok and mid == 0 and after == 1 and stable == 1,
          "mid=%d after=%d stable=%d" % (mid, after, stable))
    ok = b.scan(False) and b.key_on() == 1
    for _ in range(need):
        ok &= b.scan(True)
    check("松开清零后再次长按 %d 次 -> KEY_ON 切回 0" % need, ok and b.key_on() == 0,
          "KEY_ON=%d" % b.key_on())
    ok = True
    for _ in range(5):
        ok &= b.scan(True)
        ok &= b.scan(False)
    check("短按（<5 次扫描后松开）不改变 KEY_ON", ok and b.key_on() == 0,
          "KEY_ON=%d" % b.key_on())


def test_dual_send(b: Bench) -> None:
    out("\n" + "=" * 78)
    out("3) 整机端到端：真实组帧函数 + 双发（桩替换低层发送/写寄存器）")
    out("=" * 78)
    hi, lo = (KEY >> 8) & 0xFF, KEY & 0xFF
    sync5 = x3s.SYNC_NUMERATOR // 6

    b.key_on_write(0)
    b.reset_rec()
    ok = b._call(x3s.SEND_SHORT, (0x09,))
    check("KEY 关：send_short(cmd=0x09) 原厂单发",
          ok and b.rec["low_short"] == 1 and b.rec["wreg"] == [],
          "low_short=%d wreg=%s" % (b.rec["low_short"], b.rec["wreg"]))

    b.key_on_write(1)
    b.reset_rec()
    ok = b._call(x3s.SEND_SHORT, (0x09,))
    check("KEY 开：send_short(cmd=0x09) 双发（KEY 帧 + ID=5 普通帧 0x%04X）" % sync5,
          ok and b.rec["low_short"] == 2
          and b.rec["wreg"] == [(4, hi), (5, lo), (4, (sync5 >> 8) & 0xFF), (5, sync5 & 0xFF)],
          "low_short=%d wreg=%s" % (b.rec["low_short"], b.rec["wreg"]))

    b.reset_rec()
    ok = b._call(x3s.SEND_SHORT, (0x11,))
    check("KEY 开：白名单外 cmd=0x11 保持原厂单发",
          ok and b.rec["low_short"] == 1 and b.rec["wreg"] == [],
          "low_short=%d wreg=%s" % (b.rec["low_short"], b.rec["wreg"]))

    b.reset_rec()
    ok = b._call(x3s.SEND_PROP, (0x50, 0xD1, 0x40))
    check("KEY 开：send_property(0x50,0xD1,0x40) 双发",
          ok and b.rec["low_prop"] == 2
          and b.rec["wreg"] == [(4, hi), (5, lo), (4, (sync5 >> 8) & 0xFF), (5, sync5 & 0xFF)],
          "low_prop=%d wreg=%s" % (b.rec["low_prop"], b.rec["wreg"]))


def main() -> int:
    out("Godox X3-S V1.25 私有密钥补丁 —— 整机模拟器端到端测试")
    out("原厂 : %s" % os.path.relpath(ORIG, ROOT))
    out("补丁 : %s" % os.path.relpath(PATCHED, ROOT))
    orig_sha = hashlib.sha256(open(ORIG, "rb").read()).hexdigest()
    ok_orig = orig_sha == x3s.DEVICE["sha256"]
    check("原厂镜像 sha256 未变", ok_orig, orig_sha[:16] + "…")
    if not ok_orig or not os.path.isfile(PATCHED):
        out("[FAIL] 缺少原厂/补丁镜像")
        return 2

    check_bss_safe()
    b = Bench(PATCHED)
    check("补丁镜像启动到 main（PC=0x%08X）" % MAIN, True,
          "执行 %d 条指令" % b.m.insn_count)
    test_button(b)
    test_dual_send(b)

    out("\n结论: %s" % ("ALL PASS" if ok_all else "FAIL"))
    with open(LOG, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % os.path.relpath(LOG, ROOT))
    return 0 if ok_all else 1


if __name__ == "__main__":
    sys.exit(main())
