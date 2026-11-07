# -*- coding: utf-8 -*-
"""生成 v1 可交付补丁（enable_menu_patch=True）+ 静态证据日志。

用法：tools\\python\\python.exe make_patch.py
输出：
  out/X3_C_v1.38_app_ui.bin     —— 打好的镜像（长度不变，可直接刷）
  out/menu_evidence.txt          —— 补丁点、反汇编、验证结果（可贴进报告）
  out/X3_C_v1.38_app_ui.diff.txt —— 全部差异字节清单（用于回滚核对）
"""
from __future__ import annotations

import io
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

import ui_patch as U  # noqa: E402

OUT = os.path.join(HERE, "out")


def main() -> int:
    os.makedirs(OUT, exist_ok=True)
    orig = open(U.IMAGE, "rb").read()
    opts = {"enable_menu_patch": True}
    patched, log = U.apply_ui(orig, opts)
    results = U.verify_ui(patched, opts)

    lines: list[str] = []
    lines.append("X3-C v1.38 UI 原型补丁（密钥开关 + 广播引闪）v1")
    lines.append("=" * 78)
    lines.append("原厂镜像 : %s (%d 字节, sha256 %s)" % (U.IMAGE, len(orig), U.SHA256))
    lines.append("补丁镜像 : 长度 %d（不变）；与原厂差异 %d 字节"
                 % (len(patched), sum(1 for a, b in zip(orig, patched) if a != b)))
    lines.append("KEY_ON   : 0x%08X（RAM，上电默认 0 / 关）" % U.DEFAULT_KEY_FLAG)
    lines.append("")
    lines.append("一、补丁点（%d 个）" % len(log))
    lines.append("%-12s %-10s %s" % ("地址", "字节数", "说明"))
    for e in log:
        lines.append("%-12s %-10d %s" % ("0x%08X" % e["addr"], e["len"], e["desc"]))
    lines.append("")
    lines.append("二、自动验证")
    for ok, msg in results:
        lines.append("[%s] %s" % ("PASS" if ok else "FAIL", msg))
    lines.append("总判定: %s" % ("PASS" if all(o for o, _ in results) else "FAIL"))

    import capstone
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
    lines.append("")
    lines.append("三、补丁后 FUN_080079E8（快捷面板构建）尾部反汇编")
    for i in md.disasm(patched[U._off(0x08008220):U._off(0x08008250)], 0x08008220):
        lines.append("  %08x  %-12s %s %s" % (i.address, i.bytes.hex(" "),
                                               i.mnemonic, i.op_str))
    lines.append("")
    lines.append("四、注入函数 ui_quickpanel_ext @0x%08X（照抄原厂磁贴完整属性序列）" % U.UI_PANEL_EXT)
    for i in md.disasm(patched[U._off(U.UI_PANEL_EXT):U._off(U.UI_PANEL_EXT) + 500],
                       U.UI_PANEL_EXT):
        lines.append("  %08x  %-12s %s %s" % (i.address, i.bytes.hex(" "),
                                               i.mnemonic, i.op_str))
    lines.append("")
    lines.append("五、注入函数 ui_key_toggle @0x%08X" % U.CAVE)
    for i in md.disasm(patched[U._off(U.CAVE):U._off(U.CAVE) + 24], U.CAVE):
        lines.append("  %08x  %-12s %s %s" % (i.address, i.bytes.hex(" "),
                                               i.mnemonic, i.op_str))
    lines.append("")
    lines.append("六、注入函数 ui_broadcast_allch @0x%08X" % (U.CAVE + U.OFF_BROADCAST_ALLCH))
    for i in md.disasm(patched[U._off(U.CAVE + U.OFF_BROADCAST_ALLCH):
                               U._off(U.CAVE + U.OFF_BROADCAST_ALLCH) + 60],
                       U.CAVE + U.OFF_BROADCAST_ALLCH):
        lines.append("  %08x  %-12s %s %s" % (i.address, i.bytes.hex(" "),
                                               i.mnemonic, i.op_str))
    lines.append("")
    lines.append("七、差异字节清单（文件偏移, 原 -> 新）")
    diff = [(i, orig[i], patched[i]) for i in range(len(orig)) if orig[i] != patched[i]]
    for off, a, b in diff:
        lines.append("  0x%06X (va 0x%08X)  %02x -> %02x" % (off, off + U.BASE, a, b))

    if opts.get("enable_menu_patch"):
        lines.append("")
        lines.append("八、点击动作接线（v1.1）：页 1 入口跳板 + 轮询函数")
        lines.append("页 1 处理器 0x%08X（补丁后，入口已被改写）：" % U.UI_PAGE1_HANDLER)
        for i in md.disasm(patched[U._off(U.UI_PAGE1_HANDLER):
                                   U._off(U.UI_PAGE1_HANDLER) + 8], U.UI_PAGE1_HANDLER):
            lines.append("  %08x  %-12s %s %s" % (i.address, i.bytes.hex(" "),
                                                   i.mnemonic, i.op_str))
        lines.append("跳板 @0x%08X：" % (U.CAVE + U.OFF_TRAMP))
        for i in md.disasm(patched[U._off(U.CAVE + U.OFF_TRAMP):
                                   U._off(U.CAVE + U.OFF_TRAMP) + 16],
                           U.CAVE + U.OFF_TRAMP):
            lines.append("  %08x  %-12s %s %s" % (i.address, i.bytes.hex(" "),
                                                   i.mnemonic, i.op_str))
        lines.append("轮询 @0x%08X：" % (U.CAVE + U.OFF_POLL))
        for i in md.disasm(patched[U._off(U.CAVE + U.OFF_POLL):
                                   U._off(U.CAVE + U.OFF_POLL) + 96],
                           U.CAVE + U.OFF_POLL):
            lines.append("  %08x  %-12s %s %s" % (i.address, i.bytes.hex(" "),
                                                   i.mnemonic, i.op_str))
        lines.append("磁贴句柄 RAM：KEY=0x%08X  BCAST=0x%08X  KEY_ON=0x%08X"
                     % (U.RAM_TILE_KEY, U.RAM_TILE_BCAST, U.DEFAULT_KEY_FLAG))

    ev = os.path.join(OUT, "menu_evidence.txt")
    io.open(ev, "w", encoding="utf-8").write("\n".join(lines) + "\n")
    p = os.path.join(OUT, "X3_C_v1.38_app_ui.bin")
    with open(p, "wb") as f:
        f.write(patched)
    print("patch  :", p)
    print("evidence:", ev)
    print("diff bytes:", len(diff))
    ok = all(o for o, _ in results)
    print("总判定:", "PASS" if ok else "FAIL")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
