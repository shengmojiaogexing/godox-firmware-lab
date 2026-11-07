#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""F8/settings[7] 补丁候选点：逐条校验原始字节、打印改前/改后反汇编，
并可生成候选镜像（仅写入本目录，绝不改原厂 bin）。

用法：
  python patch_candidates.py            # 只校验 + 打印
  python patch_candidates.py --emit     # 另写 cand_reader_mask.bin / cand_plus_toggle.bin
"""
from __future__ import annotations

import hashlib
import os
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

BASE = 0x08003000
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..", ".."))
BIN = os.path.join(ROOT, "devices/ad200pro", "samples", "AD200pro_V1.60.bin")
IMAGE = open(BIN, "rb").read()
SIZE = len(IMAGE)

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_MCLASS)
md.detail = True
md.skipdata = True

# kind='mask' : ldrb rX,[base,#7] -> movs rX,#0（屏蔽稳定色温读者）
# kind='nop'  : 指令 -> nop（阻止静默清零 / 让每次改值都重设同步字）
CANDIDATES = [
    # ---- A 组：屏蔽 settings[7] 的原有「稳定色温模式」读者（保留 0x080048E4）
    dict(addr=0x08004AFC, kind="mask",
         why="FUNC_08004AC8（时长/数值显示，选 0x0800D412/… 备用表）"),
    dict(addr=0x08004FF4, kind="mask",
         why="FUNC_08004F70（显示页 1：清 g_mode bit4 + 页面标志）"),
    dict(addr=0x08005C54, kind="mask",
         why="FUNC_08005C30（measurement_task 调用：闪光时长/能量换算）"),
    dict(addr=0x08006290, kind="mask",
         why="level_table_lookup_b（功率/时长表选择，被 7 处 power_level_* 调用）"),
    dict(addr=0x0800B144, kind="mask",
         why="muldiv_scale（功率缩放计算，同样被 7 处 power_level_* 调用）"),
    dict(addr=0x0800B228, kind="mask",
         why="RF 远程设置处理 0x0800B170（r2==1 分支）"),
    # ---- B 组（可选）：阻止「模式键」静默把 F8 清 0
    dict(addr=0x0800BC7E, kind="nop",
         why="ui_key_80(0x0800BC34)：g_mode bits4-5 0→1 时 strb 0 → settings[7]"),
    # ---- C 组（可选，供 cfn-re/Lead 参考）：让 C.Fn 每次改值都重设一次同步字
    dict(addr=0x0800854E, kind="bne_nop",
         why="flash_trigger_state_machine 0x08008504（编辑 +）：仅 idx==6 才 sync"),
    dict(addr=0x080045EE, kind="bne_nop",
         why="flash_charge_state_machine 0x080045AC（编辑 -）：仅 idx==6 才 sync"),
]


def dis1(data, addr):
    o = addr - BASE
    for ins in md.disasm(data[o:o + 4], addr):
        return ins
    return None


def main(argv):
    print("=" * 78)
    print("AD200 Pro V1.60 F8/settings[7] 补丁候选点校验")
    print("image : %s" % BIN)
    print("size  : %d  sha256: %s" % (SIZE, hashlib.sha256(IMAGE).hexdigest()))
    print("=" * 78)
    mask = bytearray(IMAGE)
    toggle = bytearray(IMAGE)
    problems = []
    for c in CANDIDATES:
        a = c["addr"]
        o = a - BASE
        orig = bytes(IMAGE[o:o + 2])
        ins = dis1(IMAGE, a)
        if c["kind"] == "mask":
            # movs rX,#0，rd 取自原 ldrb 的目的寄存器（用 capstone 寄存器名解析编号）
            rd = 0
            if ins and "ldrb" in ins.mnemonic and ins.operands:
                rname = ins.reg_name(ins.operands[0].reg)
                if rname.startswith("r") and rname[1:].isdigit():
                    rd = int(rname[1:])
            new = struct.pack("<H", 0x2000 | (rd << 8))
            exp_mnem = "movs"
            check_txt = "r%d" % rd
        elif c["kind"] in ("nop", "bne_nop"):
            new = b"\x00\xbf"           # nop
            exp_mnem = "nop"
            check_txt = ""
        else:
            raise SystemExit("unknown kind %r" % c["kind"])
        after = dis1(IMAGE[:o] + new + IMAGE[o + 2:], a)
        ok_orig = (bne_ok := (ins is not None)
                   and (c["kind"] != "bne_nop" or ins.mnemonic == "bne")
                   and (c["kind"] != "mask" or "ldrb" in ins.mnemonic)
                   and (c["kind"] != "nop" or "strb" in ins.mnemonic))
        ok_new = after is not None and after.mnemonic == exp_mnem and (
            not check_txt or check_txt in after.op_str)
        status = "OK" if (ok_orig and ok_new) else "BAD"
        if status == "BAD":
            problems.append(a)
        print("\n%s @0x%08x  %s" % (status, a, c["why"]))
        print("   before: %-8s %-8s %s" % (orig.hex(), ins.mnemonic if ins else "?",
                                           ins.op_str if ins else "?"))
        print("   after : %-8s %-8s %s" % (new.hex(), after.mnemonic if after else "?",
                                           after.op_str if after else "?"))
        # cand_reader_mask.bin 只含 A 组（屏蔽 6 个 settings[7] 功能读者）；
        # cand_plus_toggle.bin = A + B（防静默清零）+ C（每次改值重设同步字）
        if c["kind"] == "mask":
            mask[o:o + 2] = new
        toggle[o:o + 2] = new

    print("\n" + "=" * 78)
    if problems:
        print("校验失败点: %s" % [hex(p) for p in problems])
    else:
        print("全部候选点原始字节/编码校验通过。")
    print("A 组=6 处 readers 屏蔽；B 组=1 处防静默清零；C 组=2 处立即重设同步字（可选）")

    if "--emit" in argv:
        p1 = os.path.join(os.path.dirname(os.path.abspath(__file__)), "cand_reader_mask.bin")
        p2 = os.path.join(os.path.dirname(os.path.abspath(__file__)), "cand_plus_toggle.bin")
        open(p1, "wb").write(bytes(mask))
        open(p2, "wb").write(bytes(toggle))
        print("写出候选镜像: %s (%d B)" % (p1, len(mask)))
        print("写出候选镜像: %s (%d B)" % (p2, len(toggle)))
        print("注意：候选镜像只用于验证，不替代 Lead 的 devices/ad200.py 正式补丁。")
    print("原厂 bin 未修改，sha256=%s" % hashlib.sha256(IMAGE).hexdigest())
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
