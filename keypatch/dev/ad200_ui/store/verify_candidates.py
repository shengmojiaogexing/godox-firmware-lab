#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""对 patch_candidates.py 生成的候选镜像做回归/语义验证：

  V1  原厂 / cand_reader_mask / cand_plus_toggle：reset→主循环无 fault/stall
  V2  F8=1 时 level_table_lookup_b(0x0800626C) 的返回值：
        原厂 = 备用（稳定色温）时长表；候选 = 原厂 OFF 时长表
  V3  builder 仍把 settings[7] 写进记录字节 31（持久化不受 reader 屏蔽影响）
  V4  候选镜像里 F8 渲染 0x08003FD0 仍读 settings[7]（显示 ON/OFF 保留）

运行：
  devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\store\\verify_candidates.py
"""
from __future__ import annotations

import os
import sys
import traceback

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
RE = os.path.join(ROOT, "devices/ad200pro")
ORIG = os.path.join(RE, "samples", "AD200pro_V1.60.bin")
HERE = os.path.dirname(os.path.abspath(__file__))
CAND1 = os.path.join(HERE, "cand_reader_mask.bin")
CAND2 = os.path.join(HERE, "cand_plus_toggle.bin")
sys.path.insert(0, RE)

from unicorn import UC_HOOK_MEM_READ, UC_HOOK_CODE                   # noqa: E402
from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1,       # noqa: E402
                               UC_ARM_REG_SP, UC_ARM_REG_LR,
                               UC_ARM_REG_PC, UC_ARM_REG_PRIMASK)
from sim.machine import Machine                                     # noqa: E402

SETTINGS = 0x20000418
SETTINGS7 = 0x2000041F
REC_SRC = 0x20000484
RET_SENT = 0xFFFFFFF1
FAILS = []


def check(name, cond, detail=""):
    ok = bool(cond)
    print("  [%s] %s%s" % ("PASS" if ok else "FAIL", name,
                           ("  " + detail) if detail else ""))
    if not ok:
        FAILS.append(name)


def call(m, addr, args=(), max_insns=300000):
    uc = m.uc
    for i, v in enumerate(args):
        uc.reg_write(UC_ARM_REG_R0 + i, v)
    uc.reg_write(UC_ARM_REG_SP, 0x20000880)
    uc.reg_write(UC_ARM_REG_LR, RET_SENT)
    try:
        uc.reg_write(UC_ARM_REG_PRIMASK, 1)
    except Exception:
        pass
    uc.reg_write(UC_ARM_REG_PC, addr)
    m.state = "paused"
    r = m.run(max_insns=max_insns, until_pc=0xFFFFFFF0, stop_on_stall=False)
    m.state = "paused"
    return r, dict(m.regs())


def boot_ok(path, label):
    print("\n== V1 启动回归: %s ==" % label)
    try:
        m = Machine(path)
        total = 0
        reached = False
        while total < 5_000_000:
            r = m.run(max_insns=5000, stop_on_stall=False)
            total += 5000
            pc = m.uc.reg_read(UC_ARM_REG_PC) & ~1
            if r and "fault" in str(r):
                check("%s 启动无 fault" % label, False, str(r))
                return False
            if 0x0800CE40 <= pc < 0x0800CEB0:
                reached = True
                break
        if not reached:
            check("%s 到达主循环" % label, False, "total=%d" % total)
            return False
        r = m.run(max_insns=300000, stop_on_stall=False)
        check("%s 到达主循环且继续运行 300k 无 fault" % label,
              not (r and "fault" in str(r)), "reason=%r total=%d" % (r, total))
        return True
    except Exception as exc:                        # noqa: BLE001
        check("%s 启动回归" % label, False, repr(exc))
        return False


def level_lookup(path):
    m = Machine(path)
    m.write(0x20000166, b"\x00")        # 0x20000184 索引
    m.write(0x20000184, b"\x28")        # 档位 = 0x28 → (0x50-0x28)=0x28
    m.write(0x2000015D, b"\x00")        # g_mode: bit4=0
    m.write(SETTINGS, b"\x00" * 7 + b"\x01")   # F8=1
    r, regs = call(m, 0x0800626C)
    return regs.get("r0", 0)


def builder_rec(path):
    m = Machine(path)
    m.write(SETTINGS, bytes([1, 2, 3, 0x44, 2, 1, 7, 1]))
    m.write(REC_SRC, b"\x00" * 32)
    call(m, 0x080043E4, (REC_SRC,))
    return bytes(m.read(REC_SRC, 32))


def render_reads(path, item=7):
    m = Machine(path)
    m.write(SETTINGS, b"\x00" * 7 + b"\x01")
    m.write(0x20000162, b"\x01")
    m.write(0x20000163, b"\x00")
    m.write(0x20000164, bytes([item]))
    hits = []

    def on_read(uc_, access, address, size, value, ud):
        hits.append(uc_.reg_read(UC_ARM_REG_PC) & ~1)

    h = m.uc.hook_add(UC_HOOK_MEM_READ, on_read, begin=SETTINGS7, end=SETTINGS7)
    try:
        call(m, 0x08003FD0)
    finally:
        try:
            m.uc.hook_del(h)
        except Exception:
            pass
    return sorted(set(hits))


def key80_clear(path):
    """ui_key_80(0x0800BC34) 在 g_mode bits4-5=0 时是否把 settings[7] 清 0。"""
    m = Machine(path)
    m.write(SETTINGS, b"\x00" * 7 + b"\x01")
    m.write(0x2000015D, b"\x00")
    call(m, 0x0800BC34)
    return m.read(SETTINGS7, 1)[0]


def editor_sync_count(path, editor, item):
    """调用 C.Fn 改值入口，统计其是否调用同步字函数 0x0800B9AC。"""
    m = Machine(path)
    m.write(SETTINGS, b"\x00" * 8)
    m.write(0x20000162, b"\x01")       # C.Fn 打开
    m.write(0x20000163, b"\x01")       # 编辑态
    m.write(0x20000164, bytes([item]))  # 当前项
    cnt = [0]

    def on_code(uc_, address, size, ud):
        cnt[0] += 1

    h = m.uc.hook_add(UC_HOOK_CODE, on_code, begin=0x0800B9AC, end=0x0800B9AC)
    try:
        call(m, editor)
    finally:
        try:
            m.uc.hook_del(h)
        except Exception:
            pass
    return cnt[0], m.read(SETTINGS7, 1)[0]


def main():
    print("=" * 78)
    print("候选镜像回归 / 语义验证")
    print("=" * 78)
    bins = [(ORIG, "原厂"), (CAND1, "cand_reader_mask"), (CAND2, "cand_plus_toggle")]
    for path, label in bins:
        if not os.path.exists(path):
            print("! 缺少 %s（先运行 patch_candidates.py --emit）" % path)
            return 2
        boot_ok(path, label)

    print("\n== V2 F8=1 时 level_table_lookup_b(0x0800626C) 返回值 ==")
    v_orig = level_lookup(ORIG)
    v_c1 = level_lookup(CAND1)
    v_c2 = level_lookup(CAND2)
    print("  原厂=%d (0x%04x)  cand_reader_mask=%d  cand_plus_toggle=%d"
          % (v_orig, v_orig, v_c1, v_c2))
    check("候选镜像的 F8 读者已不再走稳定色温备用表", v_c1 != v_orig and v_c2 != v_orig)
    check("候选镜像返回值与 F8=0 时的原厂行为一致", v_c1 == v_c2)

    print("\n== V3 builder 持久化不受屏蔽影响 ==")
    recs = {label: builder_rec(path) for path, label in bins}
    for label, rec in recs.items():
        print("  %-18s rec[24:32]=%s rec[31]=%d"
              % (label, rec[24:32].hex(" "), rec[31]))
    check("三个镜像 builder 记录字节 31 仍等于 settings[7]=1",
          all(r[31] == 1 for r in recs.values()))

    print("\n== V4 F8 渲染仍读 settings[7]（ON/OFF 显示保留）==")
    for path, label in bins:
        hits = render_reads(path)
        print("  %-18s reader PC=%s" % (label, [hex(x) for x in hits]))
        check("%s F8 渲染读 settings[7]" % label, 0x08004230 in hits)

    print("\n== V5 ui_key_80 静默清零：B 组 NOP 是否阻断 ==")
    clears = {label: key80_clear(path) for path, label in bins}
    for label, v in clears.items():
        print("  %-18s 调用后 settings[7]=%d" % (label, v))
    check("原厂 ui_key_80 会清 settings[7]（复现静默关开关）", clears["原厂"] == 0)
    check("cand_reader_mask（仅 A 组）仍会被清 → 需 B 组", clears["cand_reader_mask"] == 0)
    check("cand_plus_toggle（含 B 组）不再清 settings[7]",
          clears["cand_plus_toggle"] == 1)

    print("\n== V6 改 F8 后是否立即重设同步字（C 组 NOP 效果）==")
    for editor, ename in ((0x08008504, "编辑+ 0x08008504"),
                          (0x080045AC, "编辑- 0x080045AC")):
        for path, label in bins:
            n, v = editor_sync_count(path, editor, 7)
            print("  %-12s %-18s item=7: sync 调用次数=%d settings[7]=%d"
                  % (ename, label, n, v))
        n_orig, _ = editor_sync_count(ORIG, editor, 7)
        n_c2, v_c2b = editor_sync_count(CAND2, editor, 7)
        check("%s: 原厂改 F8 不调 sync" % ename, n_orig == 0)
        check("%s: cand_plus_toggle 改 F8 会调 1 次 sync(settings[6]) 且 F8=1"
              % ename, n_c2 == 1 and v_c2b == 1)
        # item=6（F7 ID）原本就会 sync，补丁不改变该行为
        n_o6, _ = editor_sync_count(ORIG, editor, 6)
        n_c6, _ = editor_sync_count(CAND2, editor, 6)
        check("%s: item=6(F7) 原厂/候选都调 1 次 sync" % ename,
              n_o6 == 1 and n_c6 == 1, "orig=%d cand=%d" % (n_o6, n_c6))

    print("\n" + "=" * 78)
    if FAILS:
        print("结果: FAIL (%d) -> %s" % (len(FAILS), FAILS))
        return 1
    print("结果: 全部 PASS")
    return 0


if __name__ == "__main__":
    sys.exit(main())
