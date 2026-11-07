# -*- coding: utf-8 -*-
"""X3-C 物理按键方案 —— 整机模拟器端到端测试（探针版 / 正式版）。

用 Machine 真实启动成品到 main，然后反复调用真实 `FUN_0800CC30`（完整按键扫描），
只桩替换 8 个外设/射频/动作接口：

    读键 0x08022EB4、低层发送 0x0801712C、set_channel、WriteReg、send_property、
    send_short、set_sync、以及原厂 key0 短按动作 0x08017018/0x08012540/延时。

断言：
  * 通用：三键短按不触发手势、原厂 bit1 短按标志 0x200003EB 仍翻转；
  * keyprobe：长按 bit0/1/2 分别发 1/2/3 个 TEST 帧（低层发送），
              同步字 = 当前 ID 公共同步字（ID=5 → 0x2091），且与 KEY_ON 无关；
  * keybtn2：长按拨轮切 KEY_ON；长按 ⚡ 广播（33 次频道设置/64 SYNC 写/64 属性帧/
              32 TEST 帧/1 次恢复同步字）；长按 M/C 不再触发手势；
              state[5]!=0 时手势仍应触发。

运行（在  下）::

    tools\\python\\python.exe keypatch\\dev\\x3c\\keybtn_runtime.py --mode all
"""
from __future__ import annotations

import argparse
import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
sys.path.insert(0, EMU)
sys.path.insert(0, os.path.join(ROOT, "keypatch"))

from machine import Machine, APP_BASE  # noqa: E402
from unicorn import UC_HOOK_CODE  # noqa: E402
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1,  # noqa: E402
                               UC_ARM_REG_R2, UC_ARM_REG_LR, UC_ARM_REG_PC,
                               UC_ARM_REG_SP)

import devices.x3c as x3c  # noqa: E402

KEY = 0xD798
MAIN = 0x08033E20
SENTINEL = 0x20046000
STACK = 0x20047000
CASES = {
    "keyprobe": "X3_C_v1.38_app_keyD798_keyprobe.bin",
    "keybtn2": "X3_C_v1.38_app_keyD798_keybtn2.bin",
    "keybtn3": "X3_C_v1.38_app_keyD798_keybtn3.bin",
    "keybtn4": "X3_C_v1.38_app_keyD798_keybtn4.bin",
    "keybtn4_nobc_a": "X3_C_v1.38_app_keyD798_keybtn4_nobc_a.bin",
    "keybtn4_nobc_b": "X3_C_v1.38_app_keyD798_keybtn4_nobc_b.bin",
    "keybtn4_k1": "X3_C_v1.38_app_keyD798_keybtn4_k1.bin",
    "keybtn5": "X3_C_v1.38_app_keyD798_keybtn5.bin",
    "keybtn7": "X3_C_v1.38_app_keyD798_keybtn7.bin",
    "keybtn8": "X3_C_v1.38_app_keyD798_keybtn8.bin",
}
LOG = os.path.join(HERE, "keybtn_runtime.log")
EXTRA_STUBS = {0x08017018, 0x08012540, 0x0801C118,
               0x08022E34, 0x08022E38, 0x08035FD0, 0x08013CDC}

lines: list[str] = []


def out(msg: str = "") -> None:
    lines.append(msg)
    print(msg)


class Bench:
    def __init__(self, bin_path: str):
        self.m = Machine(image=bin_path, image_base=APP_BASE, trace=False,
                         log_fn=lambda s: None, hardfault=True)
        self.m.uc.mem_write(0x0801C118, b"\x70\x47")
        self.m.uc.mem_write(0x0801C0FE, b"\x70\x47")
        self.m.uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
        self.m.boot()
        self.m.interrupt_after_chunk = False
        stop = self.m.run_until(MAIN, max_insns=2_000_000)
        if (self.m.reg("PC") & ~1) != MAIN:
            raise RuntimeError("未停在 main（%s, pc=0x%08X）"
                               % (stop, self.m.reg("PC") & ~1))
        self.m.uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
        self.keys = {"k0": False, "k1": False, "k2": False}
        self.rec = {"low_send": 0, "set_channel": [], "wreg": [],
                    "send_prop": [], "send_short": [], "set_sync": [],
                    "shutdown": 0}
        self._install_hooks()
        for ram in (x3c.BTN_RAM0_CNT, x3c.BTN_RAM0_FIRED,
                    x3c.BTN_RAM1_CNT, x3c.BTN_RAM1_FIRED,
                    x3c.BTN_RAM2_CNT, x3c.BTN_RAM2_FIRED):
            self.m.uc.mem_write(ram, b"\x00")
        self.m.uc.mem_write(x3c.KEY_ON_FLAG_RAM, b"\x00")
        self.m.uc.mem_write(x3c.STATE_INHIBIT_RAM, b"\x00")
        self.m.uc.mem_write(x3c.ID_RAM, b"\x05")
        self.n_scan = 0

    def _install_hooks(self):
        m = self.m
        masks = {x3c.BTN0_MASK: "k0", x3c.BTN1_MASK: "k1", x3c.BTN2_MASK: "k2"}

        def cb(uc, address, size, _user):
            if address == x3c.READ_KEY:
                mask = uc.reg_read(UC_ARM_REG_R1)
                r0 = 0 if self.keys.get(masks.get(mask, "")) else 1
                uc.reg_write(UC_ARM_REG_R0, r0)
                uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))
                return
            if address == x3c.LOW_SEND_SHORT:
                self.rec["low_send"] += 1
            elif address == x3c.SET_CHANNEL:
                self.rec["set_channel"].append(uc.reg_read(UC_ARM_REG_R0))
            elif address == x3c.WRITE_REG:
                self.rec["wreg"].append((uc.reg_read(UC_ARM_REG_R0),
                                         uc.reg_read(UC_ARM_REG_R1)))
            elif address == x3c.SEND_PROP:
                self.rec["send_prop"].append((uc.reg_read(UC_ARM_REG_R0),
                                              uc.reg_read(UC_ARM_REG_R1),
                                              uc.reg_read(UC_ARM_REG_R2)))
            elif address == x3c.SEND_SHORT:
                self.rec["send_short"].append(uc.reg_read(UC_ARM_REG_R0))
            elif address == x3c.SET_SYNC:
                self.rec["set_sync"].append(uc.reg_read(UC_ARM_REG_R0))
            elif address == x3c.SHUTDOWN_WRAPPER:
                self.rec["shutdown"] += 1
            elif address not in EXTRA_STUBS:
                return
            uc.reg_write(UC_ARM_REG_PC, uc.reg_read(UC_ARM_REG_LR))

        for a in ((x3c.READ_KEY, x3c.LOW_SEND_SHORT, x3c.SET_CHANNEL,
                   x3c.WRITE_REG, x3c.SEND_PROP, x3c.SEND_SHORT,
                   x3c.SET_SYNC) + tuple(EXTRA_STUBS)):
            m.uc.hook_add(UC_HOOK_CODE, cb, None, a, a + 4)

    def scan(self) -> bool:
        m = self.m
        m.uc.reg_write(UC_ARM_REG_SP, STACK)
        m.uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
        m.uc.reg_write(UC_ARM_REG_PC, x3c.KEYSCAN_FN | 1)
        m.stop_reason = None
        m._bp_skip = None
        m.run_until(SENTINEL, max_insns=200_000)
        self.n_scan += 1
        return (m.reg("PC") & ~1) == SENTINEL

    def press(self, keyname: str, n: int) -> bool:
        """按住 n 轮扫描，不松开。"""
        ok = True
        for _ in range(n):
            self.keys[keyname] = True
            ok &= self.scan()
        return ok

    def release(self, keyname: str) -> bool:
        self.keys[keyname] = False
        return self.scan()

    def hold(self, keyname: str, n: int) -> bool:
        ok = self.press(keyname, n)
        ok &= self.release(keyname)
        return ok

    def key_on(self) -> int:
        return self.m.uc.mem_read(x3c.KEY_ON_FLAG_RAM, 1)[0]

    def set_key_on(self, v: int):
        self.m.uc.mem_write(x3c.KEY_ON_FLAG_RAM, bytes([v & 0xFF]))

    def set_inhibit(self, v: int):
        self.m.uc.mem_write(x3c.STATE_INHIBIT_RAM, bytes([v & 0xFF]))


def test_common(b: Bench) -> tuple[bool, str]:
    ok_ret = True
    for k in ("k0", "k1", "k2"):
        ok_ret &= b.hold(k, 3)
    v = b.key_on()
    flag = b.m.uc.mem_read(0x200003EB, 1)[0]
    ok = ok_ret and v == 0 and flag == 1
    return ok, ("三键短按：KEY_ON=%d(应0)、bit1 原厂标志 0x200003EB=%d(应1)、"
                "扫描正常返回=%s" % (v, flag, ok_ret))


def test_probe(b: Bench) -> tuple[bool, str]:
    cfg = x3c._btn_cfg("keyprobe")
    sync = 50024 // (5 + 1)
    hi, lo = (sync >> 8) & 0xFF, sync & 0xFF
    msgs = []
    ok_all = True
    for bit, kname, n in ((0, "k0", 1), (1, "k1", 2), (2, "k2", 3)):
        l0 = b.rec["low_send"]
        w0 = len(b.rec["wreg"])
        ok_ret = b.hold(kname, cfg["threshold"])
        got = b.rec["low_send"] - l0
        w = b.rec["wreg"][w0:]
        ok = (ok_ret and got == n and (4, hi) in w and (5, lo) in w
              and b.key_on() == 0)
        ok_all &= ok
        msgs.append("bit%d=%d帧(应%d)" % (bit, got, n))
    return ok_all, ("探针长按（KEY_ON=0、ID=5）：%s，同步字均=0x%04X"
                    % (", ".join(msgs), sync))


def test_keybtn(b: Bench, mode: str) -> tuple[bool, str]:
    cfg = x3c._btn_cfg(mode)
    if mode in ("keybtn3", "keybtn4", "keybtn4_nobc_a", "keybtn4_nobc_b"):
        wheel_bit = x3c._btn_key_bit(x3c.BTN0_PORT, x3c.BTN0_MASK)
        trig_bit = x3c._btn_key_bit(x3c.BTN1_PORT, x3c.BTN1_MASK)
    else:
        wheel_bit = x3c._btn_key_bit(x3c.BTN_WHEEL_PORT, x3c.BTN_WHEEL_MASK)
        trig_bit = x3c._btn_key_bit(x3c.BTN_TRIG_PORT, x3c.BTN_TRIG_MASK)
    th = cfg["threshold"]
    msgs = []
    ok_all = True

    if mode in ("keybtn7", "keybtn8"):
        tickr = x3c.TICK_RAM
        def tset(v):
            b.m.uc.mem_write(tickr, int(v & 0xFFFFFFFF).to_bytes(4, "little"))
        def ccall(t, k0=False):
            tset(t); b.keys["k0"] = k0; return b.scan()
        def click(t0, dur=100):
            ccall(t0, True); ccall(t0 + dur, True); ccall(t0 + dur, False)
        # 初始 KEY_ON=1 -> 双击 M/C 广播
        b.set_key_on(1)
        for a in (x3c.BTN7_PRESS_START, x3c.BTN7_FIRST_PRESS, x3c.BTN7_PENDING,
                  x3c.BTN7_PREV, x3c.BTN7_ACTIVE, x3c.BTN7_SAVED_KEYON,
                  x3c.BTN7_CH, x3c.BTN7_ID, x3c.BTN_RAM_WHEEL_CNT16,
                  x3c.BTN_RAM5_STREAK):
            b.m.uc.mem_write(a, b"\x00")
        click(1000); click(1200)
        max_chunk = 0
        for i in range(500):
            n0 = len(b.rec["set_sync"])
            ccall(1300 + i * 50)
            max_chunk = max(max_chunk, len(b.rec["set_sync"]) - n0)
            if len(b.rec["set_sync"]) >= 3200:
                ccall(1300 + (i + 1) * 50)
                break
        n_ch = len(b.rec["set_channel"]); n_sync = len(b.rec["set_sync"])
        v_after = b.key_on()
        ok = (n_ch == 32 and n_sync == 3200
              and max_chunk <= x3c.BTN7_BUDGET and v_after == 1)
        msgs = ["双击M/C广播 channel=%d(应32) set_sync=%d(应3200) 单块最大=%d(应<=%d) KEY_ON=1→%d"
                % (n_ch, n_sync, max_chunk, x3c.BTN7_BUDGET, v_after)]
        # 单击不广播
        n0 = len(b.rec["set_channel"])
        click(30000); ccall(30100)
        ok = ok and len(b.rec["set_channel"]) == n0
        msgs.append("单击不广播=%s" % (len(b.rec["set_channel"]) == n0))
        # 长按 30 tick -> 切 KEY_ON；继续到 400 tick -> 关机
        b.set_key_on(0)
        for a in (x3c.BTN_RAM_WHEEL_CNT16, x3c.BTN_RAM5_STREAK):
            b.m.uc.mem_write(a, b"\x00")
        sd0 = b.rec["shutdown"]
        n0 = len(b.rec["set_channel"])
        for i in range(35):
            ccall(40000 + i * 50, True)
        v1 = b.key_on()
        for i in range(35, 420):
            ccall(40000 + i * 50, True)
        ccall(40000 + 420 * 50, False)
        sd1 = b.rec["shutdown"] - sd0
        ok = ok and v1 == 1 and sd1 > 0 and len(b.rec["set_channel"]) == n0
        msgs.append("长按30tick KEY_ON=%d(应1)、到400tick关机=%d(应>0)、不广播=%s"
                    % (v1, sd1, len(b.rec["set_channel"]) == n0))
        # ⚡ 无新行为：双击 ⚡ 不广播
        n0 = len(b.rec["set_channel"])
        b.keys["k1"] = True; tset(50000); b.scan()
        tset(50100); b.scan()
        b.keys["k1"] = False; tset(50100); b.scan()
        b.keys["k1"] = True; tset(50200); b.scan()
        tset(50300); b.scan()
        b.keys["k1"] = False; tset(50300); b.scan()
        ok = ok and len(b.rec["set_channel"]) == n0
        msgs.append("双击⚡不广播=%s" % (len(b.rec["set_channel"]) == n0))
        return ok, "keybtn7：" + "; ".join(msgs)
    if mode == "keybtn5":
        # 单计数器健壮版：无广播；⚡ 完全原厂（长按走原厂关机路径）
        th2 = cfg["threshold2"]
        b.set_key_on(0)
        b.set_inhibit(0)
        sd0 = b.rec["shutdown"]
        D = 3
        b.press("k0", th + D + 2)          # 去抖后 counter≈th，切密钥
        v1 = b.key_on()
        sd1 = b.rec["shutdown"] - sd0
        b.press("k0", th2 - (th + D + 2) - 20)   # 未到 400
        v2 = b.key_on()
        sd2a = b.rec["shutdown"] - sd0
        b.press("k0", 40)                  # 过 400
        sd2b = b.rec["shutdown"] - sd0
        b.release("k0")
        sd3 = b.rec["shutdown"] - sd0
        # 外部清零 counter 后再按 30 tick 仍能切
        b.m.uc.mem_write(x3c.BTN_RAM5_CNT16, b"\x00\x00")
        b.m.uc.mem_write(x3c.BTN_RAM5_STREAK, b"\x00")
        b.set_key_on(0)
        b.press("k0", th + D + 2)
        v4 = b.key_on()
        b.release("k0")
        # 空闲 200 次扫描：计数器归零、无动作
        cnt_ram = int.from_bytes(b.m.uc.mem_read(x3c.BTN_RAM5_CNT16, 2), "little")
        for _ in range(200):
            b.scan()
        sd4 = b.rec["shutdown"] - sd0
        # ⚡ 长按：原厂长按关机路径（A 行为）
        b.set_key_on(0)
        b.press("k1", 50)
        b.release("k1")
        sd5 = b.rec["shutdown"] - sd0
        ok = (v1 == 1 and sd1 == 0 and v2 == 1 and sd2a == 0 and sd2b == 1
              and sd3 == 1 and v4 == 1 and cnt_ram == 0 and sd4 == 1 and sd5 > 1)
        return ok, ("keybtn5: 30tick KEY_ON=%d(应1) 关机=%d(应0); 到 %d tick 前 KEY_ON=%d "
                    "关机=%d(应0); 再过 400 tick 关机=%d(应1); 外部清零后 30tick KEY_ON=%d(应1); "
                    "空闲计数器=%d(应0) 关机不变=%s; ⚡ 原厂关机路径调用累计=%d(应>1)"
                    % (v1, sd1, th2 - 100, v2, sd2a, sd2b, v4, cnt_ram, sd4 == 1, sd5))
    if mode in ("keybtn4", "keybtn4_nobc_a", "keybtn4_nobc_b"):
        # 两段式：30 tick 切密钥（不关机）→ 继续到 400 tick 关机一次；松开清零
        th2 = cfg["threshold2"]
        b.set_key_on(0)
        b.set_inhibit(0)
        sd0 = b.rec["shutdown"]
        ok_ret = b.press("k0", th)
        v1 = b.key_on()
        sd1 = b.rec["shutdown"] - sd0
        ok_ret &= b.press("k0", th2 - th)
        v2 = b.key_on()
        sd2 = b.rec["shutdown"] - sd0
        ok_ret &= b.press("k0", 10)
        sd3 = b.rec["shutdown"] - sd0
        b.release("k0")
        ok_ret &= b.press("k0", th)
        v3 = b.key_on()
        sd4 = b.rec["shutdown"] - sd0
        b.release("k0")
        ok = (ok_ret and v1 == 1 and sd1 == 0 and v2 == 1 and sd2 == 1
              and sd3 == 1 and v3 == 0 and sd4 == 1)
        ok_all &= ok
        msgs.append("%d tick:KEY_ON=%d(应1) 关机=%d(应0); 继续到 %d tick: 关机=%d(应1) "
                    "再按 10 tick=%d(应1); 松开再按 %d tick: KEY_ON=%d(应0) 关机仍=%d(应1)"
                    % (th, v1, sd1, th2, sd2, sd3, th, v3, sd4))
        # ⚡(bit1)：keybtn4=广播；nobc_a=原厂关机；nobc_b=无动作
        sd0 = b.rec["shutdown"]
        b.set_key_on(0)
        if mode == "keybtn4":
            n_ch0 = len(b.rec["set_channel"])
            n_w0, n_p0 = len(b.rec["wreg"]), len(b.rec["send_prop"])
            n_s0 = len(b.rec["send_short"])
            ok_ret = b.hold("k%d" % trig_bit, th)
            n_ch = len(b.rec["set_channel"]) - n_ch0
            n_w = len(b.rec["wreg"]) - n_w0
            n_p = len(b.rec["send_prop"]) - n_p0
            n_s = len(b.rec["send_short"]) - n_s0
            n_sync = len(b.rec["set_sync"])
            key_hi, key_lo = (KEY >> 8) & 0xFF, KEY & 0xFF
            ok = (ok_ret and n_ch == 33 and n_w == 64 and n_p == 64 and n_s == 32
                  and n_sync == 1
                  and all(x == (4, key_hi) or x == (5, key_lo) for x in b.rec["wreg"]))
            msgs.append("⚡(bit%d)长按 频道=%d(应33) SYNC写=%d(应64) 属性帧=%d(应64) "
                        "TEST=%d(应32) 恢复同步=%d(应1)"
                        % (trig_bit, n_ch, n_w, n_p, n_s, n_sync))
        elif mode == "keybtn4_nobc_a":
            cnt0 = (len(b.rec["set_channel"]), len(b.rec["send_prop"]),
                    len(b.rec["send_short"]), b.rec["low_send"])
            ok_ret = b.press("k%d" % trig_bit, 50)
            b.release("k%d" % trig_bit)
            sd1 = b.rec["shutdown"] - sd0
            cnt1 = (len(b.rec["set_channel"]), len(b.rec["send_prop"]),
                    len(b.rec["send_short"]), b.rec["low_send"])
            ok = ok_ret and sd1 > 0 and cnt0 == cnt1 and b.key_on() == 0
            msgs.append("⚡(bit%d)长按：原厂关机路径=%d(应>0)、无广播帧=%s"
                        % (trig_bit, sd1, cnt0 == cnt1))
        else:
            cnt0 = (len(b.rec["set_channel"]), len(b.rec["send_prop"]),
                    len(b.rec["send_short"]), b.rec["low_send"])
            ok_ret = b.press("k%d" % trig_bit, 50)
            b.release("k%d" % trig_bit)
            sd1 = b.rec["shutdown"] - sd0
            cnt1 = (len(b.rec["set_channel"]), len(b.rec["send_prop"]),
                    len(b.rec["send_short"]), b.rec["low_send"])
            ok = ok_ret and sd1 == 0 and cnt0 == cnt1 and b.key_on() == 0
            msgs.append("⚡(bit%d)长按：无任何动作（关机=%d、广播帧变化=%s）"
                        % (trig_bit, sd1, cnt0 == cnt1))
        ok_all &= ok
        # bit2 不绑定
        b.set_inhibit(0)
        b.set_key_on(0)
        cnt0 = (len(b.rec["set_channel"]), len(b.rec["wreg"]),
                len(b.rec["send_prop"]), len(b.rec["send_short"]),
                b.rec["shutdown"])
        ok_ret = b.hold("k2", th + 10)
        cnt1 = (len(b.rec["set_channel"]), len(b.rec["wreg"]),
                len(b.rec["send_prop"]), len(b.rec["send_short"]),
                b.rec["shutdown"])
        ok = ok_ret and b.key_on() == 0 and cnt0 == cnt1
        ok_all &= ok
        msgs.append("bit2 长按无动作=%s" % ok)
        # state[5]!=0 仍触发第一段
        b.set_inhibit(1)
        b.set_key_on(0)
        ok_ret = b.press("k0", th)
        v4 = b.key_on()
        b.release("k0")
        b.set_inhibit(0)
        ok = ok_ret and v4 == 1
        ok_all &= ok
        msgs.append("state[5]!=0 时拨轮长按仍切 KEY_ON=%d(应1)" % v4)
        return ok_all, ("%s（KEY=0x%04X）：%s" % (mode, KEY, "; ".join(msgs)))

    b.set_key_on(0)
    ok_ret = b.hold("k%d" % wheel_bit, th)
    v1 = b.key_on()
    ok_ret &= b.hold("k%d" % wheel_bit, th)
    v2 = b.key_on()
    ok = ok_ret and v1 == 1 and v2 == 0
    ok_all &= ok
    msgs.append("拨轮/M/C(bit%d)长按 KEY_ON=%d→%d(应1→0)" % (wheel_bit, v1, v2))

    n_ch0, n_w0 = len(b.rec["set_channel"]), len(b.rec["wreg"])
    n_p0, n_s0 = len(b.rec["send_prop"]), len(b.rec["send_short"])
    ok_ret = b.hold("k%d" % trig_bit, th)
    n_ch = len(b.rec["set_channel"]) - n_ch0
    n_w = len(b.rec["wreg"]) - n_w0
    n_p = len(b.rec["send_prop"]) - n_p0
    n_s = len(b.rec["send_short"]) - n_s0
    n_sync = len(b.rec["set_sync"])
    key_hi, key_lo = (KEY >> 8) & 0xFF, KEY & 0xFF
    ok = (ok_ret and n_ch == 33 and n_w == 64 and n_p == 64 and n_s == 32
          and n_sync == 1
          and all(x == (4, key_hi) or x == (5, key_lo) for x in b.rec["wreg"]))
    ok_all &= ok
    msgs.append("⚡(bit%d)长按 频道=%d(应33) SYNC写=%d(应64) 属性帧=%d(应64) "
                "TEST=%d(应32) 恢复同步=%d(应1)"
                % (trig_bit, n_ch, n_w, n_p, n_s, n_sync))

    # 未绑定的键长按不应触发手势
    unbound = [x for x in (0, 1, 2) if x not in {wheel_bit, trig_bit}]
    if unbound:
        ub = unbound[0]
        b.set_key_on(0)
        cnt0 = (len(b.rec["set_channel"]), len(b.rec["wreg"]),
                len(b.rec["send_prop"]), len(b.rec["send_short"]))
        ok_ret = b.hold("k%d" % ub, th + 10)
        cnt1 = (len(b.rec["set_channel"]), len(b.rec["wreg"]),
                len(b.rec["send_prop"]), len(b.rec["send_short"]))
        ok = ok_ret and b.key_on() == 0 and cnt0 == cnt1
        ok_all &= ok
        msgs.append("未绑定 bit%d 长按无手势=%s" % (ub, ok))

    # state[5]!=0 时仍触发（实机根因修复验证）
    b.set_inhibit(1)
    b.set_key_on(0)
    ok_ret = b.hold("k%d" % wheel_bit, th)
    v3 = b.key_on()
    b.set_inhibit(0)
    ok = ok_ret and v3 == 1
    ok_all &= ok
    msgs.append("state[5]!=0 时拨轮长按仍切 KEY_ON=%d(应1)" % v3)
    return ok_all, ("%s（KEY=0x%04X）：%s" % (mode, KEY, "; ".join(msgs)))


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--mode", default="all", choices=["keyprobe", "keybtn2", "keybtn3", "keybtn4", "keybtn4_nobc_a", "keybtn4_nobc_b", "keybtn4_k1", "keybtn5", "keybtn7", "keybtn8", "all"] + ["all"])
    args = ap.parse_args()
    modes = ["keyprobe", "keybtn2", "keybtn3", "keybtn4", "keybtn4_nobc_a", "keybtn4_nobc_b", "keybtn4_k1", "keybtn5", "keybtn7", "keybtn8"] if args.mode == "all" else [args.mode]
    out("X3-C 物理按键方案 —— 整机模拟器端到端测试")
    ok_all = True
    for mode in modes:
        path = os.path.join(ROOT, "keypatch", "out", CASES[mode])
        out("\n" + "=" * 78)
        out("模式 %s：%s" % (mode, os.path.relpath(path, ROOT)))
        if not os.path.isfile(path):
            out("[FAIL] 找不到成品（先跑 keybtn_build.py --mode all）")
            ok_all = False
            continue
        out("sha256 : %s" % hashlib.sha256(open(path, "rb").read()).hexdigest())
        out("=" * 78)
        b = Bench(path)
        out("已启动到 main（insn=%d）" % b.m.insn_count)
        ok1, m1 = test_common(b)
        out("  [%s] %s" % ("PASS" if ok1 else "FAIL", m1))
        if mode == "keyprobe":
            ok2, m2 = test_probe(b)
        else:
            ok2, m2 = test_keybtn(b, mode)
        out("  [%s] %s" % ("PASS" if ok2 else "FAIL", m2))
        ok_all &= (ok1 and ok2)
    out("\n结论: %s" % ("ALL PASS" if ok_all else "FAIL"))
    with open(LOG, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")
    print("\n[log] %s" % os.path.relpath(LOG, ROOT))
    return 0 if ok_all else 1


if __name__ == "__main__":
    sys.exit(main())
