# -*- coding: utf-8 -*-
"""合并 AD100 Pro v4 核心补丁（密钥开关读 RAM[0x20000486]）与 ui-lights 的 C.Fn「KEY」UI 补丁。

输出：keypatch/out/AD100pro_V1.2_keyD798_ui.bin（最终 UI 版，原位、50248 字节）
验证：
  1. 单独补丁各自 verify PASS（UI 用 ui-lights 的 verify，核心用我们的 verify）；
  2. 两种 apply 顺序结果逐字节相同（可交换）；
  3. 合并镜像 = 原厂 + 核心 diff ∪ UI diff，且两补丁区各自精确；
  4. 核心 Unicorn：flag=1/0 同步字正确、原厂自证 PASS；
  5. 整机 UI 功能（模拟器）：C.Fn 能渲染 KEY 项、菜单 +/- 能切换 flag、
     切换后调用 0x0800B758 同步字立即变化（UI→核心端到端）、保存/加载链路 PASS；
  6. 整机启动回归：复位->200 万条指令与原厂逐项一致 + RF bring-up + 运行期调用。

运行：
  tools\\python\\python.exe keypatch\\dev\\ad100\\build_ui_merged.py [key] [out.log]
"""
from __future__ import annotations

import hashlib
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
KEYPATCH = os.path.join(ROOT, "keypatch")
LIGHTS_UI = os.path.join(KEYPATCH, "dev", "lights_ui")
EMU = os.path.join(ROOT, "devices/ad100pro", "emu")
for p in (KEYPATCH, LIGHTS_UI, EMU, HERE):
    if p not in sys.path:
        sys.path.insert(0, p)

import core  # noqa: E402  (devices/ad100pro/emu/core.py)
import ui_patch_ad100 as U  # noqa: E402  (ui-lights，只读引用)
from devices import ad100  # noqa: E402
import emu_regress_variant as R  # noqa: E402

OFFICIAL = os.path.join(ROOT, "devices", "ad100pro", "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "devices/ad100pro", "out", "functions.json")
KEY = 0xD798

# UI 侧常量（与 ui_proof_ad100.py 相同，来自 ui-lights 的反汇编）
FB = 0x200004CC
UI = 0x20000164
UI_EDIT = UI + 0x16
UI_SEL = UI + 0x17
UI_CFN = UI + 0x15
SET = 0x20000480
FLAG = 0x20000486
DRAW_CFN = 0x08003F0C
PLUS_FN = 0x08004880
MINUS_FN = 0x08008A5C
BUILD_REC = 0x08004690
SAVE_SLOT = 0x0800B588
EXPAND_REC = 0x0800CE6C


def parse_key(text):
    return int(text, 0) if text.lower().startswith("0x") else int(text, 16)


def static_checks(orig, ui_only, core_only, merged, ui_log, core_log):
    ok = True
    out = []

    def chk(cond, msg):
        nonlocal ok
        ok = ok and cond
        out.append("  [%s] %s" % ("PASS" if cond else "FAIL", msg))

    out.append("== 1. 合并镜像静态检查 ==")
    chk(len(merged) == ad100.IMAGE_SIZE == 50248,
        "合并镜像长度 %d 字节（原厂 %d）" % (len(merged), len(orig)))
    chk(merged != orig, "合并镜像与原厂不同")

    # 两个补丁的可交换性
    merged2 = U.apply_ui(core_only)[0]
    chk(merged == merged2, "先 UI 后核心 与 先核心 后 UI 结果逐字节相同（两补丁不重叠）")

    # 核心补丁区
    coff = ad100.SYNC_FN - ad100.BASE
    chk(merged[coff:coff + 56] == ad100.build_sync_function(parse_key(KEY_TEXT)),
        "核心补丁区 0x%08X..0x%08X = v4 期望函数体" % (ad100.SYNC_FN, ad100.SYNC_FN_END - 1))
    # UI 各补丁区与 ui-lights 的期望一致（以 ui_only 为参照）
    for addr, size in U.ZONES:
        o = addr - U.BASE
        chk(merged[o:o + size] == ui_only[o:o + size],
            "UI 补丁区 0x%08X(%d 字节) 与 ui-lights 单独补丁结果一致" % (addr, size))

    # 差异集合 = 核心 diff ∪ UI diff，且无越界
    core_diff = {i for i in range(len(orig)) if core_only[i] != orig[i]}
    ui_diff = {i for i in range(len(orig)) if ui_only[i] != orig[i]}
    mdiff = {i for i in range(len(orig)) if merged[i] != orig[i]}
    chk(mdiff == core_diff | ui_diff,
        "合并差异 %d 字节 = 核心 %d ∪ UI %d（多出 %s）"
        % (len(mdiff), len(core_diff), len(ui_diff),
           ["0x%05X" % i for i in sorted(mdiff - (core_diff | ui_diff))[:8]]))
    chk(not (core_diff & ui_diff),
        "两补丁差异集合无交集（核心 %d / UI %d 字节）" % (len(core_diff), len(ui_diff)))

    # 两个单补丁的 verify
    out.append("== 2. 各自 verify ==")
    for good, msg in U.verify(ui_only):
        chk(good, "UI 补丁自检: " + msg)
    out.append("  （核心补丁 verify 见 bench 输出：keypatch.py patch --verify）")
    return ok, out


def ui_functional(merged, merged_path):
    """在模拟器里验证合并镜像的 C.Fn「KEY」菜单 + UI→核心端到端。"""
    out = []
    ok = True

    def chk(cond, msg):
        nonlocal ok
        ok = ok and cond
        out.append("  [%s] %s" % ("PASS" if cond else "FAIL", msg))

    eo = core.Emulator(OFFICIAL, FUNCS, trace=False)
    eo.boot_interactive()
    ep = core.Emulator(merged_path, FUNCS, trace=False)
    ep.boot_interactive()

    out.append("== 3. 整机 UI 功能（模拟器） ==")

    def draw(e, sel, flag, clear=True):
        e.write_mem(UI_CFN, b"\x01")
        e.write_mem(UI_EDIT, b"\x00")
        e.write_mem(UI_SEL, bytes([sel]))
        e.write_mem(SET + 6, bytes([flag]))
        if clear:
            e.write_mem(FB, b"\x00" * 1024)
        return e.call_function(DRAW_CFN, args=(), max_insns=1_000_000)

    # 3a. 现有页 sel=3 的 body（FB 256 之后）与原厂一致；页头允许改
    eo.write_mem(UI_CFN, b"\x01")
    eo.write_mem(UI_EDIT, b"\x00")
    eo.write_mem(UI_SEL, b"\x03")
    eo.write_mem(FB, b"\x00" * 1024)
    eo.call_function(DRAW_CFN, args=(), max_insns=1_000_000)
    fb_orig3 = bytes(eo.read_mem(FB, 1024))
    draw(ep, 3, 0)
    fb_new3 = bytes(ep.read_mem(FB, 1024))
    chk(fb_orig3[256:] == fb_new3[256:], "现有 C.Fn 页 sel=3 的列表区域与原厂一致（只改页头）")

    # 3b. sel=6 渲染 KEY OFF / KEY ON，且两帧不同
    r = draw(ep, 6, 0)
    fb_off = bytes(ep.read_mem(FB, 1024))
    chk(bool(r), "C.Fn sel=6（KEY 项）渲染调用成功")
    draw(ep, 6, 1)
    fb_on = bytes(ep.read_mem(FB, 1024))
    chk(fb_off != fb_on, "KEY OFF / ON 画面不同（值被真实绘制）")
    chk(sum(1 for b in fb_on if b) > 0, "KEY 页画面非空（%d 个非零字节）" % sum(1 for b in fb_on if b))

    # 3c. +/- 导航 wrap 0..6
    for fn, label, exp in ((PLUS_FN, "+", [1, 2, 3, 4, 5, 6, 0, 1]),
                           (MINUS_FN, "-", [6, 5, 4, 3, 2, 1, 0, 6, 5])):
        ep.write_mem(UI_CFN, b"\x01")
        ep.write_mem(UI_EDIT, b"\x00")
        ep.write_mem(UI_SEL, b"\x00")
        seq = []
        for _ in range(len(exp)):
            ep.call_function(fn, args=(), max_insns=200_000)
            seq.append(ep.read_mem(UI_SEL, 1)[0])
        chk(seq == exp, "『%s』选择项 wrap 序列 %s（期望 %s）" % (label, seq, exp))

    # 3d. 选中 KEY 项时 +/- 切换 flag（0->1->0->1）
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
    chk((v1, v2, v3) == (1, 0, 1), "菜单 +/- 切换 KEY：0 -> %d -> %d -> %d（期望 1,0,1）" % (v1, v2, v3))

    # 3e. UI→核心 端到端：菜单把 flag 置 1/0 后，调用 0x0800B758 同步字立即变化
    for flag, exp in ((1, parse_key(KEY_TEXT)), (0, 50024 // 6)):
        ep.write_mem(FLAG, bytes([flag]))
        ep.cc2500.reset()
        ep.call_function(0x0800B758, (5,))
        r4 = ep.cc2500.regs.get(0x04, -1)
        r5 = ep.cc2500.regs.get(0x05, -1)
        sync = (r4 << 8) | r5
        chk(sync == exp, "UI 开关 flag=%d -> 调用 0x0800B758(ID=5) 得 sync=0x%04X（期望 0x%04X）"
            % (flag, sync, exp))

    # 3f. 保存/加载链路：组包 -> 记录偏移 30 -> flash -> 展开恢复
    ep.write_mem(FLAG, b"\x01")
    ep.call_function(BUILD_REC, args=(0x20000454,), max_insns=500_000)
    rec = bytes(ep.read_mem(0x20000454, 32))
    chk(rec[0] == 0x5C and rec[U.FLAG_RECORD_OFFSET] == 1,
        "设置记录 magic=0x%02X、偏移 %d=%d（KEY ON）"
        % (rec[0], U.FLAG_RECORD_OFFSET, rec[U.FLAG_RECORD_OFFSET]))
    ep.write_mem(0x0801F000, b"\xff" * 100)
    ep.call_function(SAVE_SLOT, args=(0,), max_insns=2_000_000)
    flash_rec = bytes(ep.read_mem(0x0801F000, 32))
    chk(flash_rec[0] == 0x5C and flash_rec[U.FLAG_RECORD_OFFSET] == 1,
        "写 flash 记录 0x0801F000: magic=0x%02X off30=%d"
        % (flash_rec[0], flash_rec[U.FLAG_RECORD_OFFSET]))
    ep.write_mem(0x20000434, flash_rec)
    ep.write_mem(FLAG, b"\x00")
    ep.call_function(EXPAND_REC, args=(), max_insns=500_000)
    chk(ep.read_mem(FLAG, 1)[0] == 1, "加载链路 0x0800CE6C 恢复 0x20000486=1")
    return ok, out


def main():
    key = parse_key(sys.argv[1]) if len(sys.argv) > 1 else KEY
    global KEY_TEXT
    KEY_TEXT = "0x%04X" % key
    lines = []

    def out(msg):
        lines.append(msg)
        print(msg)

    orig = open(OFFICIAL, "rb").read()
    out("== AD100 Pro UI 合并版（v4 核心 + ui-lights C.Fn KEY） ==")
    out("原厂: %s  %d 字节  sha256 %s"
        % (os.path.relpath(OFFICIAL, ROOT), len(orig), hashlib.sha256(orig).hexdigest()))

    ui_only, ui_log = U.apply_ui(orig)
    core_only, core_log = ad100.apply(orig, key)
    merged = ad100.apply(ui_only, key)[0]
    assert ad100.apply(ui_only, key)[0] == U.apply_ui(core_only)[0]

    # 模拟器 Emulator 需要文件路径；先写临时副本做整机 UI 功能验证，PASS 后再写成品
    tmp_path = os.path.join(HERE, "_ui_merged_tmp.bin")
    with open(tmp_path, "wb") as f:
        f.write(merged)

    ok = True
    st_ok, st_lines = static_checks(orig, ui_only, core_only, merged, ui_log, core_log)
    for ln in st_lines:
        out(ln)
    ok &= st_ok

    # 核心 Unicorn 功能（合并镜像上，flag 语义）
    out("\n== 4. 核心补丁 Unicorn（合并镜像上） ==")
    uc_ok = True
    for ident, flag, exp in ((0, 1, key), (5, 1, key), (99, 1, key),
                             (0, 0, 50024), (5, 0, 50024 // 6), (99, 0, 50024 // 100)):
        calls = ad100._uc_run_sync_fn(merged, ident, flag)["calls"]
        sync = (calls[0][2] << 8 | calls[1][2]) if len(calls) >= 2 else -1
        calls_ok = (calls == [("wreg", 4, exp >> 8), ("wreg", 5, exp & 0xFF)])
        uc_ok = uc_ok and calls_ok
        out("  [%s] ID=%-2d flag=%d -> sync=0x%04X（期望 0x%04X）"
            % ("PASS" if calls_ok else "FAIL", ident, flag, sync, exp))
    # 原厂自证
    for ident in (0, 1, 5, 37, 99):
        good, msg = ad100._check_original(orig, ident)
        uc_ok = uc_ok and good
        out("  [%s] %s" % ("PASS" if good else "FAIL", msg))
    ok &= uc_ok

    ui_ok, ui_lines = ui_functional(merged, tmp_path)
    for ln in ui_lines:
        out(ln)
    ok &= ui_ok

    # 整机启动/RF 回归（复用变体回归脚本）
    out("\n== 5. 整机启动 + RF 回归 ==")
    ok &= R.boot_compare(OFFICIAL, tmp_path, out)
    ok &= R.rf_bringup(OFFICIAL, tmp_path, "flag", out)
    ok &= R.runtime_call(tmp_path, "flag", out)

    dst = os.path.join(ROOT, "keypatch", "out", "AD100pro_V1.2_key%04X_ui.bin" % key)
    with open(dst, "wb") as f:
        f.write(merged)
    try:
        os.remove(tmp_path)
    except OSError:
        pass
    out("\n输出: %s  %d 字节  sha256 %s"
        % (os.path.relpath(dst, ROOT), len(merged), hashlib.sha256(merged).hexdigest()))
    out("\n== 结论: %s ==" % ("PASS" if ok else "FAIL"))

    if len(sys.argv) > 2:
        with open(sys.argv[2], "w", encoding="utf-8", newline="\n") as f:
            f.write("\n".join(lines) + "\n")
    return 0 if ok else 1


KEY_TEXT = "0xD798"

if __name__ == "__main__":
    sys.exit(main())
