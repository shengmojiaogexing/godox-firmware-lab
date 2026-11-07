# -*- coding: utf-8 -*-
"""AD100 Pro C.Fn「KEY」菜单补丁 —— 模拟器验证 + 截图。

  tools\\python\\python.exe keypatch\\dev\\lights_ui\\ui_proof_ad100.py

验证项：
  A. 渲染：原厂 vs 补丁的 C.Fn 各页截图（KEY ON / KEY OFF / 现有页）
  B. 导航：真实按键处理函数 0x08004880/0x08008A5C 下选择项 0..6 循环
  C. 值切换：选中 KEY 时 +/- 切 ON/OFF（写 0x20000486）
  D. 保存：0x08004690 组包 → 记录第 30 字节；0x0800B588 写 flash 0x0801F000+30
  E. 加载：0x0800CE6C 从记录恢复到 0x20000486
"""
from __future__ import annotations

import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
sys.path.insert(0, os.path.join(ROOT, "devices/ad100pro", "emu"))
sys.path.insert(0, HERE)

import core  # noqa: E402
import main as emu_main  # noqa: E402
from render_fb import fb_to_pixels, save_png  # noqa: E402
import ui_patch_ad100 as U  # noqa: E402

FW = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
OUT = os.path.join(HERE, "out")
os.makedirs(OUT, exist_ok=True)

FB = 0x200004CC
UI = 0x20000164
UI_EDIT = UI + 0x16      # 0x2000017A 编辑态
UI_SEL = UI + 0x17       # 0x2000017B C.Fn 选中项
UI_CFN = UI + 0x15       # 0x20000179 C.Fn 打开
SET = 0x20000480
FLAG = 0x20000486
DRAW_CFN = 0x08003F0C
PLUS_FN = 0x08004880
MINUS_FN = 0x08008A5C
BUILD_REC = 0x08004690       # (dst) 用当前设置组 32B 记录
SAVE_SLOT = 0x0800B588       # (slot) 0x20000454 -> 0x20000434 -> flash record
EXPAND_REC = 0x0800CE6C      # 0x20000434 -> settings globals（含 0x20000486）
FB_CLEAR = 0x0800B5C0


def save_png_fb(e, name):
    fb = bytes(e.read_mem(FB, 1024))
    pix = fb_to_pixels(fb, top_bit=7)
    save_png(pix, os.path.join(OUT, name), scale=6)
    return fb


def render_cfn(e, sel, flag, name):
    e.write_mem(UI_CFN, bytes([1]))
    e.write_mem(UI_EDIT, bytes([0]))
    e.write_mem(UI_SEL, bytes([sel]))
    e.write_mem(SET + 6, bytes([flag]))
    e.write_mem(FB, b"\x00" * 1024)
    ok = e.call_function(DRAW_CFN, args=(), max_insns=1_000_000)
    fb = save_png_fb(e, name)
    return bool(ok), fb


def main():
    ok = True

    def chk(cond, msg):
        nonlocal ok
        ok = ok and bool(cond)
        print("  [%s] %s" % ("PASS" if cond else "FAIL", msg))

    data = open(FW, "rb").read()
    patched, log = U.apply_ui(data)
    pbin = os.path.join(OUT, "AD100pro_V1.2_keyUI.bin")
    open(pbin, "wb").write(patched)
    print("== 静态验证 ==")
    for good, msg in U.verify(patched):
        chk(good, msg)

    # 原始镜像用于对照
    eo = core.Emulator(FW, FUNCS, trace=False)
    eo.boot_interactive()
    ep = core.Emulator(pbin, FUNCS, trace=False)
    ep.boot_interactive()
    print("\n== A. 渲染截图 ==")
    # 原厂：sel 0..5
    for sel in range(6):
        eo.write_mem(UI_CFN, b"\x01")
        eo.write_mem(UI_EDIT, b"\x00")
        eo.write_mem(UI_SEL, bytes([sel]))
        eo.write_mem(SET + 6, b"\x00")
        eo.write_mem(FB, b"\x00" * 1024)
        eo.call_function(DRAW_CFN, args=(), max_insns=1_000_000)
        save_png_fb(eo, "orig_cfn_sel%d.png" % sel)
    for sel in range(6):
        okr, _ = render_cfn(ep, sel, 0, "patch_cfn_sel%d.png" % sel)
        chk(okr, "补丁版 C.Fn sel=%d 渲染调用成功" % sel)
    okr, fb_off = render_cfn(ep, 6, 0, "patch_cfn_KEY_OFF.png")
    chk(okr, "补丁版 sel=6 渲染 KEY OFF 成功")
    okr, fb_on = render_cfn(ep, 6, 1, "patch_cfn_KEY_ON.png")
    chk(okr, "补丁版 sel=6 渲染 KEY ON 成功")
    chk(fb_off != fb_on, "KEY OFF/ON 两帧不同（值确实被绘制）")
    # 现有页 body 不应改变（只改页头）：比较 sel=3 的行区域（page2..7 全部）
    eo.write_mem(UI_CFN, b"\x01"); eo.write_mem(UI_EDIT, b"\x00")
    eo.write_mem(UI_SEL, b"\x03"); eo.write_mem(FB, b"\x00" * 1024)
    eo.call_function(DRAW_CFN, args=(), max_insns=1_000_000)
    fb_orig3 = bytes(eo.read_mem(FB, 1024))
    render_cfn(ep, 3, 0, "patch_cfn_sel3_check.png")
    fb_new3 = bytes(ep.read_mem(FB, 1024))
    same_body = fb_orig3[256:] == fb_new3[256:]
    chk(same_body, "现有 C.Fn 页 sel=3 的列表/值区域（FB 偏移 256+）与原厂一致")

    print("\n== B. 导航：选择项 wrap 0..6 ==")
    for fn, label in ((PLUS_FN, "+"), (MINUS_FN, "-")):
        ep.write_mem(UI_CFN, b"\x01")
        ep.write_mem(UI_EDIT, b"\x00")
        ep.write_mem(UI_SEL, b"\x00")
        seq = []
        for _ in range(9):
            ep.call_function(fn, args=(), max_insns=200_000)
            seq.append(ep.read_mem(UI_SEL, 1)[0])
        if label == "+":
            chk(seq[:8] == [1, 2, 3, 4, 5, 6, 0, 1],
                "『+』选择序列 %s（期望 1,2,3,4,5,6,0,1）" % seq)
        else:
            exp = [6, 5, 4, 3, 2, 1, 0, 6, 5]
            chk(seq[:9] == exp, "『-』选择序列 %s（期望 %s）" % (seq, exp))

    print("\n== C. 值切换（选中 KEY，编辑态）==")
    ep.write_mem(UI_CFN, b"\x01")
    ep.write_mem(UI_EDIT, b"\x01")
    ep.write_mem(UI_SEL, b"\x06")
    ep.write_mem(FLAG, b"\x00")
    ep.call_function(PLUS_FN, args=(), max_insns=200_000)
    v1 = ep.read_mem(FLAG, 1)[0]
    ep.call_function(PLUS_FN, args=(), max_insns=200_000)
    v2 = ep.read_mem(FLAG, 1)[0]
    ep.call_function(MINUS_FN, args=(), max_insns=200_000)
    v3 = ep.read_mem(FLAG, 1)[0]
    chk(v1 == 1, "『+』把 KEY 从 0 → 1（实际 %d）" % v1)
    chk(v2 == 0, "再『+』在最大值 1 处回绕到 0（实际 %d）" % v2)
    chk(v3 == 1, "『-』从 0 回绕到 1（实际 %d）" % v3)

    print("\n== D. 保存链路 ==")
    ep.write_mem(FLAG, b"\x01")
    okr = ep.call_function(BUILD_REC, args=(0x20000454,), max_insns=500_000)
    rec = bytes(ep.read_mem(0x20000454, 32))
    chk(rec[0] == 0x5C, "记录 magic=0x%02X（期望 0x5C）" % rec[0])
    chk(rec[U.FLAG_RECORD_OFFSET] == 1,
        "记录偏移 %d = %d（KEY=ON）" % (U.FLAG_RECORD_OFFSET, rec[U.FLAG_RECORD_OFFSET]))
    ep.write_mem(0x0801F000, b"\xff" * 100)
    okr = ep.call_function(SAVE_SLOT, args=(0,), max_insns=2_000_000)
    flash_rec = bytes(ep.read_mem(0x0801F000, 32))
    chk(flash_rec[0] == 0x5C and flash_rec[U.FLAG_RECORD_OFFSET] == 1,
        "写 flash 记录 0x0801F000：magic=0x%02X, off30=%d"
        % (flash_rec[0], flash_rec[U.FLAG_RECORD_OFFSET]))

    print("\n== E. 加载链路 ==")
    # 把刚写的 flash 记录拷到 0x20000434 再展开
    ep.write_mem(0x20000434, flash_rec)
    ep.write_mem(FLAG, b"\x00")
    okr = ep.call_function(EXPAND_REC, args=(), max_insns=500_000)
    v = ep.read_mem(FLAG, 1)[0]
    chk(v == 1, "0x0800CE6C 从记录恢复 0x20000486=%d（期望 1）" % v)

    print("\n== 结论: %s ==" % ("PASS" if ok else "FAIL"))
    print("截图目录:", OUT)
    for f in sorted(os.listdir(OUT)):
        if f.startswith(("orig_cfn", "patch_cfn")):
            print("  ", f)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
