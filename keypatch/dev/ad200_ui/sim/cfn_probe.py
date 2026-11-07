#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""cfn_probe.py —— 在整机模拟器里驱动 C.Fn 菜单，dump 段码屏位图（RAM 0x20000408）。

做什么：
  1. 启动到主循环；
  2. 进入 C.Fn：默认走**真实按键路径**（注入 3x3 键盘 MENU 键 row2,col2，
     由固件 ui_key_scan_task 自己扫描/去抖/分派，置 UI+0x12=1），
     也可用 --direct 直接置 UI 状态（按键路径失败时也会自动回退并说明）；
  3. 遍历 C.Fn 项 index 0..7（写 UI+0x14），调用渲染函数 0x08003FD0，
     dump 16 字节段码位图 + 字段渲染调用翻译出的可读文本；
  4. settings[7]（F8=密钥开关）分别置 0/1 渲染同一个 F8 页，对比位图，
     证明 ON/OFF 两个状态的"值区"不同、且 F8 标签不变。

用法：
    devices\ad200pro\\python\\python.exe keypatch\\dev\\ad200_ui\\sim\\cfn_probe.py
    ... cfn_probe.py --direct            # 不用按键，直接写 UI 状态
    ... cfn_probe.py --bin 补丁.bin      # 对补丁固件跑同样探测
    ... cfn_probe.py --json out.json

退出码：0 = PASS；1 = FAIL。
日志：`> log_cfn_probe.txt`。
"""
import argparse
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import _sim_common as C  # noqa: E402

N_ITEMS = 8


def describe_bitmap(bm):
    """16 字节位图 → 每字节二进制（作为"参考图像"的紧凑形式）。"""
    lines = []
    for i, b in enumerate(bm):
        bits = "".join("1" if (b >> bit) & 1 else "0" for bit in range(7, -1, -1))
        lines.append("    byte[%2d] = 0x%02X  %s %s" % (i, b, bits[:4], bits[4:]))
    return "\n".join(lines)


def decode_text(transcript, glyphs):
    """按 char 索引拼出显示文本（未渲染位置用 '.'）。"""
    row = ["."] * 16
    for ev in transcript:
        ch = ev["char"]
        if 0 <= ch < 16:
            row[ch] = glyphs.get(ev["code"]) or "?"
    return "".join(row)


def fmt_events(transcript, glyphs):
    out = []
    for ev in transcript:
        g = glyphs.get(ev["code"])
        g = ("'%s'" % g) if g is not None else "?"
        out.append("field#%d code=%d(%s) char=%d bit=%d"
                   % (ev["field"], ev["code"], g, ev["char"], ev["bitpos"]))
    return out


def main(argv=None):
    C.ensure_stdout_utf8()
    ap = argparse.ArgumentParser(description="AD200 Pro C.Fn 菜单/段码位图模拟器探测")
    ap.add_argument("--bin", help="固件路径（默认原厂）")
    ap.add_argument("--direct", action="store_true", help="直接置 UI 状态进入 C.Fn，不走按键")
    ap.add_argument("--json", help="把位图/文本写入 JSON")
    args = ap.parse_args(argv)

    path = args.bin or C.resolve_orig()
    print("== cfn_probe: C.Fn 菜单 + 段码位图（RAM 0x20000408）==")
    print("镜像: %s" % os.path.abspath(path))
    if args.bin:
        print("sha256: %s" % C.sha256_hex(C.read_file_bytes(path)))

    m = C.load_machine(path)
    boot = C.boot_to_main(m, exact_insns=False)
    print("启动到主循环: insns=%d pc=0x%08X state=%s" % (boot["insns"], boot["pc"], boot["state"]))
    if not (boot["reason"] in ("breakpoint", "loop_zone")
            and C.LOOP_LO <= boot["pc"] < C.LOOP_HI):
        print("[FAIL] 未能启动到主循环（reason=%s pc=0x%08X）" % (boot["reason"], boot["pc"]))
        return 1

    glyphs = C.decode_font_codes(m)
    print("字体表 0x%08X code->glyph: %s"
          % (C.FONT_TABLE, {k: v for k, v in glyphs.items() if v}))

    # 字段渲染调用追踪要在"跑主循环/按键路径"之前安装（unicorn hook 若在长跑之后
    # 再安装，在本模拟器上会只命中最后一个字段函数；先装好则完全正常）。
    trace = C.FieldTrace(m)

    # ---- 进入 C.Fn ----
    key_info = None
    if args.direct:
        m.write(C.UI_OPEN, b"\x01")
        print("\n进入 C.Fn：--direct 直接置 UI+0x12=1")
    else:
        keys = C.KeyMatrix(m)
        t0 = m.insn_count
        key_info = C.open_cfn_via_key(m, keys)
        print("\n进入 C.Fn：真实按键路径（MENU=row2,col2/code 0x100）"
              " -> ok=%s attempts=%d 消耗 %d 条指令" % (key_info["ok"], key_info["attempts"],
                                                       m.insn_count - t0))
        if key_info["ok"]:
            print("  [PASS] 按键注入可打开 C.Fn：UI+0x12(0x20000162) = %d"
                  % C.read_u8(m, C.UI_OPEN))
        else:
            print("  [WARN] 按键路径未打开 C.Fn，回退为直接置 UI 状态")
            m.write(C.UI_OPEN, b"\x01")
        keys.restore()

    if C.read_u8(m, C.UI_OPEN) != 1:
        m.write(C.UI_OPEN, b"\x01")
    print("  UI 状态: UI+0x12=%d UI+0x13=%d UI+0x14=%d UI+0x15=%d UI+0x16=%d"
          % tuple(C.read_bytes(m, C.UI + 0x12, 5)))

    results = {}
    checks = []

    def need(cond, msg):
        checks.append((bool(cond), msg))
        print("  [%s] %s" % ("PASS" if cond else "FAIL", msg))
        return cond

    # 基准：settings[7] = 0（原厂默认 OFF）
    m.write(C.SETTINGS7, b"\x00")

    # ---- 遍历 index 0..7 ----
    print("\n[遍历 C.Fn 项 0..7：写 UI+0x14，调 0x%08X 渲染，dump 16B 位图]" % C.FUNC_RENDER)
    for idx in range(N_ITEMS):
        bm, transcript, text, res = C.render_cfn(m, idx, settings7=None,
                                                 field_trace=trace, glyphs=glyphs)
        cur_idx = C.read_u8(m, C.UI_INDEX)
        label_ev = [ev for ev in transcript if ev["char"] == 8]
        label_code = label_ev[0]["code"] if label_ev else None
        label_glyph = glyphs.get(label_code) if label_code is not None else None
        results[idx] = {"bitmap": C.hexs(bm), "text": text,
                        "transcript": fmt_events(transcript, glyphs),
                        "label_code": label_code, "label_glyph": label_glyph}
        print("  item %d（%s）UI+0x14=%d 渲染返回 reason=%s"
              % (idx, C.item_label_text(idx), cur_idx, res["reason"]))
        print("    bitmap: %s" % C.hexs(bm))
        print("    text  : %s" % text)
        print("    fields: %s" % ", ".join(results[idx]["transcript"][:8]))
        need(not res["fault"], "item %d 渲染无 fault" % idx)
        need(any(bm), "item %d 位图非空" % idx)
        need(cur_idx == idx, "item %d 写入 UI+0x14 生效（读到 %d）" % (idx, cur_idx))
        need(label_glyph == str(idx + 1),
             "item %d 标签字符 = '%s'（期望 '%s'，说明画的是 F%d）"
             % (idx, label_glyph, str(idx + 1), idx + 1))

    # ---- settings[7]=0 vs 1 对比 F8 页 ----
    print("\n[F8/密钥开关：settings[7]=0（ON=OFF/OFF=普通模式） vs =1]")
    bm0, tr0, text0, _ = C.render_cfn(m, 7, settings7=0, field_trace=trace, glyphs=glyphs)
    bm1, tr1, text1, _ = C.render_cfn(m, 7, settings7=1, field_trace=trace, glyphs=glyphs)
    diff = [i for i in range(16) if bm0[i] != bm1[i]]
    print("  settings[7]=0 bitmap: %s" % C.hexs(bm0))
    print("  settings[7]=0 text  : %s" % text0)
    print("  settings[7]=0 fields: %s" % ", ".join(fmt_events(tr0, glyphs)))
    print("  settings[7]=1 bitmap: %s" % C.hexs(bm1))
    print("  settings[7]=1 text  : %s" % text1)
    print("  settings[7]=1 fields: %s" % ", ".join(fmt_events(tr1, glyphs)))
    print("  位图差异字节: %s（共 %d 字节）" % (diff, len(diff)))
    if diff:
        for i in diff:
            print("    byte[%2d] 0x%02X -> 0x%02X" % (i, bm0[i], bm1[i]))
    need(bm0 != bm1, "settings[7]=0 与 =1 的 F8 页位图不同（ON/OFF 值区不同）")
    need(bm0[8] == bm1[8], "两种状态下 F8 标签字节 byte[8] 相同（0x%02X，标签是 'F8'）" % bm0[8])
    need(C.read_u8(m, C.SETTINGS7) == 1, "settings[7] 寄存器 = 1（渲染读的就是该值）")
    label_chars = [ev for ev in tr0 if ev["char"] == 8]
    need(any(glyphs.get(ev["code"]) == "8" for ev in label_chars),
         "settings[7]=0 时 F8 页标签仍为 'F8'")
    label_chars1 = [ev for ev in tr1 if ev["char"] == 8]
    need(any(glyphs.get(ev["code"]) == "8" for ev in label_chars1),
         "settings[7]=1 时 F8 页标签仍为 'F8'")
    # 值区：F8 页上除标签/固定段之外的字符，两种状态渲染的 code 序列应不同
    need(tr0 != tr1, "settings[7]=0/1 的字段渲染调用序列不同（值区渲染分支不同）")
    need(text0 != text1, "settings[7]=0/1 的 F8 页显示文本不同（%r vs %r）" % (text0, text1))
    # 值区在 char 1..3：OFF 渲染 3 个字符、ON 渲染 2 个（字体 code0 画成 O/0）
    for tag, tr in (("settings[7]=0（OFF/普通模式）", tr0), ("settings[7]=1（ON/密钥模式）", tr1)):
        chars = {}
        for ev in tr:
            if ev["char"] in (1, 2, 3):
                chars[ev["char"]] = glyphs.get(ev["code"]) or "?"
        vals = "".join(chars.get(i, ".") for i in (1, 2, 3))
        print("  值区 chars1..3（%s）: %s   # 字体 code0 即 O/0，读作 %r"
              % (tag, vals, vals.replace("0", "O").rstrip(".")))

    # ---- 参考图像（二进制位图）----
    print("\n[settings[7]=0 的 F8 页位图按字节展开]")
    print(describe_bitmap(bm0))
    print("[settings[7]=1 的 F8 页位图按字节展开]")
    print(describe_bitmap(bm1))

    ok = all(c for c, _ in checks)
    trace.close()
    print("\n%s cfn_probe（%d 项检查，%d 失败）"
          % ("PASS" if ok else "FAIL", len(checks), sum(1 for c, _ in checks if not c)))
    if args.json:
        with open(args.json, "w", encoding="utf-8") as f:
            json.dump({"ok": ok, "image": os.path.abspath(path),
                       "key_path_ok": (key_info or {}).get("ok"),
                       "items": results,
                       "f8": {"bitmap_0": C.hexs(bm0), "bitmap_1": C.hexs(bm1),
                              "text_0": text0, "text_1": text1, "diff_bytes": diff,
                              "fields_0": fmt_events(tr0, glyphs),
                              "fields_1": fmt_events(tr1, glyphs)}},
                      f, ensure_ascii=False, indent=1)
        print("（结果已写入 %s）" % os.path.abspath(args.json))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
