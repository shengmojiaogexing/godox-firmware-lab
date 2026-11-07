# -*- coding: utf-8 -*-
"""用 capstone 直接反汇编 AD200 Pro V1.60 的关键函数（带字面量/内存注释）。

输出：disasm_menu.log（UTF-8），供 CFN.md 引用证据。
用法：
    devices\ad200pro\\python\\python.exe dump_funcs.py
"""
from __future__ import annotations

import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(HERE))))
BASE = 0x08003000
BIN = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")
FUNCTIONS_JSON = os.path.join(ROOT, "devices/ad200pro", "analysis", "functions.json")
OUT = os.path.join(HERE, "disasm_menu.log")

# 需要转储的函数：地址 -> 名字
FUNCS = [
    (0x08008818, "ui_key_scan_task  (按键扫描+分派)"),
    (0x0800C2D8, "ui_menu_task      (MENU 键短按任务，由扫描任务在 UI+0xC 置位后调用)"),
    (0x0800BC34, "ui_key_80         (键 0x080 处理)"),
    (0x0800B90C, "ui_key_20         (键 0x020 处理)"),
    (0x08004270, "ui_key_100        (键 0x100 处理)"),
    (0x080042A0, "ui_key_10         (键 0x010 处理 / 长按重复)"),
    (0x08008D08, "ui_key_1          (键 0x001 处理)"),
    (0x0800C190, "ui_key_2          (键 0x002 处理)"),
    (0x08009B7C, "ui_key_4          (键 0x004 处理)"),
    (0x080048E4, "ui_key_14         (键 0x014 = 0x10|0x04 长按组合)"),
    (0x08003FD0, "cfn_render        (C.Fn 渲染, display_task 在 0x080093C4 调用)"),
    (0x08009228, "display_task      (显示任务)"),
    (0x0800B9AC, "sync_word_func    (同步字: 50024/(id+1) -> reg4/5)"),
    (0x08009B38, "factory_reset_task(恢复出厂)"),
    (0x0800AC58, "settings_load_task"),
    (0x0800CCC8, "settings_expand"),
    (0x080043E4, "param_record_build"),
    (0x0800B820, "flash_store_params"),
    (0x0800B9E4, "rf_status_read    (ui_key_scan_task 入口调用, 写 UI+0xF/UI+0x12)"),
    (0x080063A0, "FUNC_080063a0     (adc_result_update? 键 0x010 延迟处理, 写 UI+0x12)"),
    (0x08005484, "EXTI4_15_IRQHandler (外部触发中断, 写 UI+0x12=0)"),
    (0x080045AC, "FUNC_080045ac     (charge SM, 读 UI+0x12/13/14 + settings[6] -> sync)"),
    (0x08008504, "FUNC_08008504     (trigger SM, 读 UI+0x12/13/14 + settings[6] -> sync)"),
    (0x0800AA8C, "rf_apply_settings (C.Fn 关闭/菜单任务末尾调用)"),
    (0x0800C248, "board_init        (写 UI+0x17)"),
]


def main():
    from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
    data = open(BIN, "rb").read()
    funcs = {}
    with open(FUNCTIONS_JSON, encoding="utf-8") as fh:
        for fn in json.load(fh):
            funcs[fn["start"]] = fn
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.detail = True

    lines = []
    lines.append("AD200 Pro V1.60 关键函数反汇编（capstone 5.0.7, thumb/mclass）")
    lines.append("镜像 %s  %d 字节  base=0x%08X" % (BIN, len(data), BASE))
    lines.append("")

    ram_names = {
        0x20000150: "UI(base)",
        0x20000418: "settings[0]",
        0x2000041E: "settings[6](ID)",
        0x2000041F: "settings[7](F8)",
        0x20000464: "flash_param_mirror",
        0x20000484: "record_src",
    }

    for addr, title in FUNCS:
        fn = funcs.get(addr)
        if fn is None:
            # 退化为线性反汇编 64 字节
            end = addr + 64
        else:
            end = fn["end"]
        off = addr - BASE
        blob = data[off:end - BASE]
        lines.append("=" * 78)
        lines.append(";; 0x%08X  %s   [%d insns]" %
                     (addr, title, (end - addr) // 2))
        lines.append("=" * 78)
        for ins in md.disasm(blob, addr):
            line = "0x%08X: %-10s %-34s" % (ins.address, ins.mnemonic, ins.op_str)
            # 常见 PC 相对 ldr 的字面量
            if ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str:
                try:
                    imm = ins.operands[1].mem.disp
                    lit_addr = ((ins.address + 4) & ~3) + imm
                    lo = lit_addr - BASE
                    val = int.from_bytes(data[lo:lo + 4], "little")
                    note = "  @0x%08X = 0x%08X" % (lit_addr, val)
                    if val in ram_names:
                        note += "  (%s)" % ram_names[val]
                    line += note
                except Exception:
                    pass
            lines.append(line)
        lines.append("")

    text = "\n".join(lines) + "\n"
    with open(OUT, "w", encoding="utf-8") as fh:
        fh.write(text)
    print("written %s (%d lines)" % (OUT, len(lines)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
