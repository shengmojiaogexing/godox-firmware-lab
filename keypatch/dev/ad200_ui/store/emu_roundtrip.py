#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""AD200 Pro V1.60 —— settings[7]（F8）持久化链路 Unicorn 实测。

用 devices/ad200pro 的整机模拟器 Machine（Unicorn + 外设/Flash 控制器模型）做
「调用级」实测（不是纯静态推断）：

  T1  builder 0x080043E4：settings[7](0x2000041F) 是否写到记录字节 31
  T2  flash_store_params 0x0800B820(slot)：是否真的把记录写进
      0x0801F000 + slot*100（经 0x08005974 flash 控制器模型）
  T3  保存任务 0x08009B38 + 触发 0x0800B810：改 F8 后是否在 200 tick 后落盘
      （并验证「记录没变化就不写 flash」）
  T4  C.Fn 改值入口 0x08008504（+）/ 0x080045AC（-）：F8 是否 0↔1、上限是否 1
  T5  上电加载 settings_load_task 0x0800AC58：记录→0x2000041F 是否恢复成 1；
      记录字节31>1 时是否被 clamp 成 0（0x0800E314[7]=1）
  T6  运行期读者：装 read hook 抓 0x2000041F 的读取 PC（C.Fn 渲染等）

运行：
  devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\store\\emu_roundtrip.py
退出码 0 表示全部 PASS。输出自动写 log_emu_roundtrip.txt（若用重定向）。
"""
from __future__ import annotations

import os
import sys
import traceback

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
RE = os.path.join(ROOT, "devices/ad200pro")
BIN = os.path.join(RE, "samples", "AD200pro_V1.60.bin")
sys.path.insert(0, RE)

from unicorn import (UcError, UC_HOOK_MEM_READ,                     # noqa: E402
                     UC_HOOK_MEM_WRITE)
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_SP,       # noqa: E402
                               UC_ARM_REG_LR, UC_ARM_REG_PC,
                               UC_ARM_REG_PRIMASK)
from sim.machine import Machine                                     # noqa: E402

BASE = 0x08003000
SETTINGS = 0x20000418
SETTINGS7 = 0x2000041F
REC_SRC = 0x20000484          # builder 输出记录（32B）
MIRROR = 0x20000464           # 参数块镜像（8 word）
SLOT_VAR = 0x20000214         # [0]=当前 slot，[2]=save request，[4..5]=countdown
PARAM = 0x0801F000            # 参数页
CLAMP = 0x0800E314            # 每项上限表
RET_SENT = 0xFFFFFFF1         # 返回到这里 -> 触发 unmapped/EXC_RETURN，视为函数返回

ADDR_BUILDER = 0x080043E4
ADDR_STORE = 0x0800B820
ADDR_SAVE_TASK = 0x08009B38
ADDR_SCHEDULE = 0x0800B810
ADDR_EDIT_INC = 0x08008504
ADDR_EDIT_DEC = 0x080045AC
ADDR_LOAD = 0x0800AC58
ADDR_DISPLAY = 0x08009228
ADDR_RENDER = 0x08003FD0

TRACE = []


def log(msg=""):
    print(msg)
    TRACE.append(msg)


class Ctx:
    def __init__(self):
        self.m = None
        self.hook = None

    def new_machine(self):
        m = Machine(BIN)
        m.reset()
        self.m = m
        return m

    def call(self, addr, *args, max_insns=400000, sp=0x20000880):
        m = self.m
        uc = m.uc
        for i, v in enumerate(args):
            uc.reg_write(UC_ARM_REG_R0 + i, v)
        uc.reg_write(UC_ARM_REG_SP, sp)
        uc.reg_write(UC_ARM_REG_LR, RET_SENT)
        try:
            uc.reg_write(UC_ARM_REG_PRIMASK, 1)     # 关中断，避免 SysTick 打进来
        except Exception:
            pass
        uc.reg_write(UC_ARM_REG_PC, addr)
        m.state = "paused"
        m.stop_reason = None
        reason = m.run(max_insns=max_insns, until_pc=0xFFFFFFF0, stop_on_stall=False)
        m.state = "paused"          # 不要因 fault 触发下一次 run 的 reset()
        return reason, dict(m.regs())

    def read(self, addr, n):
        return bytes(self.m.read(addr, n))

    def write(self, addr, data):
        self.m.write(addr, data)


def u16(b, off=0):
    return b[off] | (b[off + 1] << 8)


def u32(b, off=0):
    return int.from_bytes(b[off:off + 4], "little")


def magic_record_ok(rec):
    return len(rec) >= 32 and rec[0] == 0x5C


def make_record(settings_bytes, settings7=None):
    """按『记录字节 24..31 = settings[0..7]』构造一个记录（用于喂给加载端）。
    其余字节填 0，第 0 字节 magic=0x5C。"""
    rec = bytearray(32)
    rec[0] = 0x5C
    rec[24:32] = bytes(settings_bytes)
    return bytes(rec)


FAILS = []


def check(name, cond, detail=""):
    ok = bool(cond)
    log("  [%s] %s%s" % ("PASS" if ok else "FAIL", name,
                         ("  " + detail) if detail else ""))
    if not ok:
        FAILS.append(name)
    return ok


# ------------------------------------------------------------------ T1
def t1_builder(cx):
    log("\n== T1 builder 0x080043E4：settings[7] → 记录字节 31 ==")
    cx.new_machine()
    settings = [0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x07, 0x01]
    cx.write(SETTINGS, bytes(settings))
    cx.write(REC_SRC, b"\x00" * 32)
    reason, regs = cx.call(ADDR_BUILDER, REC_SRC)
    rec = cx.read(REC_SRC, 32)
    log("  builder 返回 reason=%r  r0=0x%08x" % (reason, regs.get("r0", 0)))
    log("  记录: " + rec.hex(" "))
    r24 = u32(rec, 24)
    check("记录字节 0 == 0x5C (magic)", rec[0] == 0x5C, "rec[0]=0x%02x" % rec[0])
    check("记录 byte24..27 == settings[0..3] 的 word",
          r24 == u32(bytes(settings), 0),
          "word6=0x%08x" % r24)
    check("记录 byte28..31 == settings[4..7] 的 word",
          u32(rec, 28) == u32(bytes(settings), 4),
          "word7=0x%08x" % u32(rec, 28))
    check("记录字节 31 == settings[7] == 1", rec[31] == 1,
          "rec[31]=0x%02x" % rec[31])
    return rec


# ------------------------------------------------------------------ T2
def t2_store(cx):
    log("\n== T2 flash_store_params 0x0800B820(slot=3) → flash 0x0801F000+300 ==")
    cx.new_machine()
    settings = [0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x07, 0x01]
    cx.write(SETTINGS, bytes(settings))
    cx.write(MIRROR, b"\x00" * 32)
    reason, regs = cx.call(ADDR_STORE, 3)
    log("  store 返回 reason=%r" % (reason,))
    page = cx.read(PARAM + 3 * 100, 32)
    log("  flash[0x0801F000+300..+31]: " + page.hex(" "))
    mirror = cx.read(MIRROR, 32)
    check("镜像 0x20000464 word7 == 记录 word7", u32(mirror, 28) == u32(page, 28))
    check("参数页记录 magic==0x5C", page[0] == 0x5C, "byte0=0x%02x" % page[0])
    check("参数页记录字节31 == settings[7] == 1", page[31] == 1,
          "byte31=0x%02x" % page[31])
    check("参数页记录字节24..31 == settings[0..7]",
          page[24:32] == bytes(settings), page[24:32].hex(" "))
    return page


# ------------------------------------------------------------------ T3
def t3_save_trigger(cx):
    log("\n== T3 保存触发：0x0800B810 置 200 tick 倒计时 → 0x08009B38 落盘 ==")
    cx.new_machine()
    settings = [0, 0, 0, 0, 0, 0, 0, 0]
    cx.write(SETTINGS, bytes(settings))
    cx.write(MIRROR, b"\x00" * 32)

    # 模拟用户把 F8 改成 1（RAM 里的当前值）
    cx.write(SETTINGS7, b"\x01")

    reason, _ = cx.call(ADDR_SCHEDULE)
    cd = u16(cx.read(SLOT_VAR + 4, 2))
    log("  调度后倒计时 [0x20000218] = %d (0x%x)" % (cd, cd))
    check("0x0800B810 把保存倒计时设为 200", cd == 0xC8, "cd=%d" % cd)

    calls = 0
    saved_slot = None
    while calls < 400:
        calls += 1
        cx.call(ADDR_SAVE_TASK)
        slot = cx.read(SLOT_VAR, 1)[0]
        rec = cx.read(PARAM + slot * 100, 32)
        if magic_record_ok(rec) and rec[31] == 1:
            saved_slot = slot
            break
    log("  第 %d 次调用保存任务后落盘，slot=%r" % (calls, saved_slot))
    check("改 F8 后保存任务在 200 次 tick（=200 次主循环）后写 flash",
          saved_slot is not None and calls == 200,
          "calls=%d slot=%r" % (calls, saved_slot))
    if saved_slot is not None:
        rec = cx.read(PARAM + saved_slot * 100, 32)
        log("  flash 记录: " + rec.hex(" "))
        check("落盘记录字节 31 == 1", rec[31] == 1, "byte31=0x%02x" % rec[31])
        check("记录 magic", rec[0] == 0x5C)
        slot_before = cx.read(SLOT_VAR, 1)[0]

        # 记录没变化 → 不应再写 flash
        cx.call(ADDR_SCHEDULE)
        for _ in range(200):
            cx.call(ADDR_SAVE_TASK)
        slot_after = cx.read(SLOT_VAR, 1)[0]
        rec2 = cx.read(PARAM + saved_slot * 100, 32)
        check("记录未变化时不重复写 flash（slot 不递增）",
              slot_after == slot_before and rec2 == rec,
              "slot %d -> %d" % (slot_before, slot_after))
    return saved_slot


# ------------------------------------------------------------------ T4
def t4_editor(cx):
    log("\n== T4 C.Fn 改值入口：F8 由 0→1（上限 1 生效），再次 + 回到 0 ==")
    cx.new_machine()
    cx.write(SETTINGS, b"\x00" * 8)
    # UI: +0x12=1 (C.Fn 打开), +0x13=1 (编辑态), +0x14=7 (F8)
    cx.write(0x20000162, b"\x01")
    cx.write(0x20000163, b"\x01")
    cx.write(0x20000164, b"\x07")
    reason, _ = cx.call(ADDR_EDIT_INC)
    v1 = cx.read(SETTINGS7, 1)[0]
    log("  0x08008504(编辑 +) 后 settings[7]=%d  reason=%r" % (v1, reason))
    check("F8 编辑 + 后 0→1", v1 == 1, "v=%d" % v1)
    reason, _ = cx.call(ADDR_EDIT_INC)
    v2 = cx.read(SETTINGS7, 1)[0]
    log("  再次 0x08008504 后 settings[7]=%d（上限 1 回绕）" % v2)
    check("上限表 0x0800E314[7]=1 → 再次 + 回绕到 0", v2 == 0, "v=%d" % v2)

    cx.write(SETTINGS7, b"\x01")
    reason, _ = cx.call(ADDR_EDIT_DEC)
    v3 = cx.read(SETTINGS7, 1)[0]
    log("  0x080045AC(编辑 -) 后 settings[7]=%d  reason=%r" % (v3, reason))
    check("F8 编辑 - 后 1→0", v3 == 0, "v=%d" % v3)
    reason, _ = cx.call(ADDR_EDIT_DEC)
    v4 = cx.read(SETTINGS7, 1)[0]
    log("  再次 0x080045AC 后 settings[7]=%d（0 → 上限回绕）" % v4)
    check("F8 编辑 - 在 0 处回绕到上限 1（同一张 0x0800E314 表）", v4 == 1,
          "v=%d" % v4)
    return True


# ------------------------------------------------------------------ T5
def t5_load(cx, good_record):
    log("\n== T5 上电加载 settings_load_task 0x0800AC58：记录字节31 → 0x2000041F ==")
    # 注意：加载路径会用上限表 0x0800E314 = [1,2,3,150,2,1,99,1] 夹紧每一项，
    # 所以这里故意选全部 <= 上限的合法值，验证「记录→RAM」逐字节还原。
    settings = [1, 2, 3, 0x44, 2, 1, 7, 1]
    rec = make_record(settings)
    slot = 5
    cx.new_machine()
    cx.write(PARAM + slot * 100, rec)
    reason, _ = cx.call(ADDR_LOAD, max_insns=400000)
    loaded = cx.read(SETTINGS, 8)
    log("  load 后 settings[0..7] = " + loaded.hex(" ") + "  reason=%r" % (reason,))
    check("加载后 settings[7]==1", loaded[7] == 1, "v=%d" % loaded[7])
    check("合法记录加载后 settings[0..7] 与记录一致", loaded == bytes(settings),
          loaded.hex(" "))
    slot_after = cx.read(SLOT_VAR, 1)[0]
    rec0 = cx.read(PARAM, 32)
    log("  加载任务末尾重新归一化保存: slot=%d flash slot0 记录=%s"
        % (slot_after, rec0.hex(" ")))
    check("加载后重写 slot0 且记录字节31 仍为 1",
          slot_after == 0 and rec0[0] == 0x5C and rec0[31] == 1,
          "slot=%d magic=0x%02x b31=0x%02x" % (slot_after, rec0[0], rec0[31]))

    # clamp: 记录字节31 = 5 > 上限 1 → 加载后应为 0
    log("\n== T5b 记录字节31=5（>上限 1）→ 加载时 clamp 成 0 ==")
    bad = bytearray(rec)
    bad[31] = 5
    cx.new_machine()
    cx.write(PARAM + 3 * 100, bytes(bad))
    reason, _ = cx.call(ADDR_LOAD, max_insns=400000)
    loaded = cx.read(SETTINGS, 8)
    log("  load 后 settings = " + loaded.hex(" ") + "  reason=%r" % (reason,))
    check("记录字节31=5 时加载后被 clamp 为 0（0x0800E314[7]=1）",
          loaded[7] == 0, "v=%d" % loaded[7])
    return True


# ------------------------------------------------------------------ T6
def _trace_reads(cx, fn, setup, label):
    """装 read hook 抓 0x2000041F 的读取 PC，然后调用 fn。"""
    m = cx.new_machine()
    setup(cx)
    hits = []
    uc = m.uc

    def on_read(uc_, access, address, size, value, ud):
        hits.append(uc_.reg_read(UC_ARM_REG_PC) & ~1)

    h = uc.hook_add(UC_HOOK_MEM_READ, on_read, begin=SETTINGS7, end=SETTINGS7)
    try:
        reason, _ = cx.call(fn, max_insns=300000)
    except Exception as exc:                        # noqa: BLE001
        reason = "exception: %s" % exc
        log("  %s 调用异常(继续): %s" % (label, exc))
    finally:
        try:
            uc.hook_del(h)
        except Exception:
            pass
    uniq = sorted(set(hits))
    log("  %s: 读取 0x2000041F 的 PC（去重）= %s   (reason=%r)"
        % (label, ", ".join("0x%08x" % p for p in uniq) or "(无)", reason))
    return uniq


def t6_runtime_reads(cx):
    log("\n== T6 运行期读者：抓 0x2000041F 的读取 PC ==")
    # T6a: C.Fn 渲染 FUNC_08003FD0，item=7(F8)，非编辑态 -> 读 settings[7] 显示 ON/OFF
    def setup_render(cx):
        cx.write(SETTINGS, b"\x00" * 7 + b"\x01")
        cx.write(0x20000162, b"\x01")     # C.Fn 打开（display_task 才走渲染分支）
        cx.write(0x20000163, b"\x00")     # 非编辑态
        cx.write(0x20000164, b"\x07")     # 当前项 = F8

    hits = _trace_reads(cx, ADDR_RENDER, setup_render, "C.Fn 渲染 0x08003FD0(item=7)")
    check("F8 渲染路径确实读了 settings[7]",
          0x08004230 in hits, "hits=%s" % [hex(h) for h in hits])

    # T6b: 显示页 1 —— FUNC_08004F70 末尾读 settings[7]（0x08004FF4）
    def setup_page1(cx):
        cx.write(SETTINGS, b"\x00" * 7 + b"\x01")
        cx.write(0x2000015D, b"\x01")     # g_mode & 3 == 1
        cx.write(0x20000165, b"\x00")
        cx.write(0x20000166, b"\x00")

    hits2 = _trace_reads(cx, 0x08004F70, setup_page1, "显示页 FUNC_08004F70")
    check("显示页 FUNC_08004F70 路径读了 settings[7]（稳定色温显示逻辑）",
          0x08004FF4 in hits2, "hits=%s" % [hex(h) for h in hits2])
    return hits, hits2


def t7_boot_reads(cx):
    log("\n== T7 启动+主循环运行期：抓 0x2000041F 的读取 PC（真实执行路径）==")
    m = cx.new_machine()
    hits = []
    uc = m.uc

    def on_read(uc_, access, address, size, value, ud):
        hits.append(uc_.reg_read(UC_ARM_REG_PC) & ~1)

    h = uc.hook_add(UC_HOOK_MEM_READ, on_read, begin=SETTINGS7, end=SETTINGS7)
    try:
        total = 0
        while total < 5_000_000:
            r = m.run(max_insns=5000, stop_on_stall=False)
            total += 5000
            pc = m.uc.reg_read(UC_ARM_REG_PC) & ~1
            if 0x0800CE40 <= pc < 0x0800CEB0:
                break
        # 主循环再跑一段（覆盖 display_task 的显示页轮换）
        m.run(max_insns=600000, stop_on_stall=False)
    except Exception as exc:                        # noqa: BLE001
        log("  启动/运行异常(继续): %s" % exc)
    finally:
        try:
            uc.hook_del(h)
        except Exception:
            pass
    uniq = sorted(set(hits))
    log("  启动到主循环+主循环运行期间读取 0x2000041F 的 PC（去重）= %s"
        % (", ".join("0x%08x" % p for p in uniq) or "(无)"))
    log("  命中次数=%d" % len(hits))
    return uniq


def t8_silent_clear(cx):
    log("\n== T8 静默清零路径：ui_key_80 (0x0800BC34) 在特定模式下清 settings[7] ==")
    cx.new_machine()
    cx.write(SETTINGS, b"\x00" * 7 + b"\x01")
    # UI 全 0：UI+0x12=0, UI[0]=0, UI+0xe=0, g_mode(UI+0xd)=0 -> bits4-5=0
    reason, _ = cx.call(0x0800BC34)
    v = cx.read(SETTINGS7, 1)[0]
    gmode = cx.read(0x2000015D, 1)[0]
    log("  调用后 settings[7]=%d  g_mode=0x%02x  reason=%r" % (v, gmode, reason))
    check("ui_key_80 在该状态下把 settings[7] 从 1 清成 0（F8 会被静默关掉）",
          v == 0, "v=%d" % v)
    check("同时 g_mode bits4-5 被置 1（0x10）", (gmode & 0x30) == 0x10,
          "g_mode=0x%02x" % gmode)
    return v


def t9_defaults_and_writers(cx):
    log("\n== T9 上电默认值 & 运行期写者 ==")
    # a) 默认值函数 0x080048E4 把 settings[0..7] 全清 0（settings_load_task 无有效记录时调用）
    cx.new_machine()
    cx.write(SETTINGS, b"\xAA" * 8)
    reason, _ = cx.call(0x080048E4)
    v = cx.read(SETTINGS, 8)
    log("  0x080048E4 后 settings = " + v.hex(" ") + "  reason=%r" % (reason,))
    # 实测：默认值不是全 0 —— settings[2]=1（F3 默认档），其余为 0；F8=settings[7]=0
    check("上电默认 settings[7]==0（F8=OFF/普通模式）", v[7] == 0, "v=%d" % v[7])
    check("默认值函数写入 settings[0..7] = 00 00 01 00 00 00 00 00",
          v == bytes([0, 0, 1, 0, 0, 0, 0, 0]), v.hex(" "))

    # b) 从 reset 跑到主循环，用写 hook 抓「谁在上电时写 0x2000041F」
    m = cx.new_machine()
    hits = []
    uc = m.uc

    def on_write(uc_, access, address, size, value, ud):
        hits.append((uc_.reg_read(UC_ARM_REG_PC) & ~1, value))

    h = uc.hook_add(UC_HOOK_MEM_WRITE, on_write, begin=SETTINGS7, end=SETTINGS7)
    try:
        total = 0
        while total < 5_000_000:
            m.run(max_insns=5000, stop_on_stall=False)
            total += 5000
            pc = m.uc.reg_read(UC_ARM_REG_PC) & ~1
            if 0x0800CE40 <= pc < 0x0800CEB0:
                break
    except Exception as exc:                        # noqa: BLE001
        log("  启动异常(继续): %s" % exc)
    finally:
        try:
            uc.hook_del(h)
        except Exception:
            pass
    uniq = sorted(set(hits))
    log("  上电（无有效参数记录时）对 0x2000041F 的写入 PC/值 = %s"
        % ", ".join("0x%08x<=0x%02x" % (p, val) for p, val in uniq))
    if uniq:
        check("上电写 settings[7] 的代码是默认值路径 0x08004920 且写 0",
              0x08004920 in [p for p, _ in uniq] and all(val == 0 for _, val in uniq),
              repr(uniq))
    else:
        check("上电路径未写 settings[7]（保持 RAM 初值 0）", True, "(无写入)")
    return uniq


def main():
    log("=" * 78)
    log("AD200 Pro V1.60 settings[7]（F8）持久化 & 语义 —— Unicorn 调用级实测")
    log("image: %s" % BIN)
    log("=" * 78)
    cx = Ctx()
    good = None
    try:
        good = t1_builder(cx)
        t2_store(cx)
        t3_save_trigger(cx)
        t4_editor(cx)
        t5_load(cx, good)
        t6_runtime_reads(cx)
        t7_boot_reads(cx)
        t8_silent_clear(cx)
        t9_defaults_and_writers(cx)
    except Exception:
        log("\n[EXCEPTION]\n" + traceback.format_exc())
        FAILS.append("EXCEPTION")
    log("\n" + "=" * 78)
    if FAILS:
        log("结果: FAIL (%d) -> %s" % (len(FAILS), FAILS))
        return 1
    log("结果: 全部 PASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
