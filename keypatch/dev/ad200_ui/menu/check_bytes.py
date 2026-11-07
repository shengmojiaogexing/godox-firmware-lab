# -*- coding: utf-8 -*-
"""C.Fn 相关常量/字节核对（cfn-re task-1）。

核对并断言：
  1) Lead 补丁 A 两处原始 4 字节（cmp rX,#6 / bne）
  2) 建议补丁 B：两处 bne(0xd1) -> blo(0xd3) 原始字节
  3) 值上限表 0x0800E314[0..7]：F8=1、F7(ID)=99 => F8 只能 0/1
  4) 同步字函数 0x0800B9AC 原始 56 字节 + 50024 字面量
  5) 4 个同步字函数调用点字节
  6) 0x0800B810（置 200 tick 保存倒计时）/ 0x0800B7F4（倒计时递减）
  7) 0x0800B9E4（UI+0x2C 超时退出 overlay）
  8) F8 渲染分支（settings[7]==0 -> OFF 渲染，否则 ON 渲染）
  9) ui_key_scan_task 的 C.Fn 相关指令字节：0x08008AB2/0x08008AB4/0x08008AB6

运行（脚本内部写 UTF-8 日志 check_bytes.log）：
    devices\ad200pro\\python\\python.exe check_bytes.py
"""
from __future__ import annotations

import hashlib
import os
import sys
from contextlib import redirect_stdout

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(HERE))))
BASE = 0x08003000
BIN = os.path.join(ROOT, "devices", "x3c", "firmware", "AD200pro_V1.60.bin")
LOG = os.path.join(HERE, "check_bytes.log")

checks = []


def check(ok, msg):
    checks.append((bool(ok), msg))
    print(("[PASS] " if ok else "[FAIL] ") + msg)
    return ok


def main():
    data = open(BIN, "rb").read()

    def hx(a, n):
        return data[a - BASE:a - BASE + n].hex(" ")

    print("C.Fn 常量/字节核对（cfn-re / task-1）")
    print("镜像 %s  %d 字节  sha256=%s\n"
          % (BIN, len(data), hashlib.sha256(data).hexdigest()))

    print("[1] Lead 补丁 A（0x080045EC / 0x0800854C，各 4B -> 00 bf 00 bf）")
    for a, why in ((0x080045EC, "dec: cmp r2,#6 / bne"),
                   (0x0800854C, "inc: cmp r0,#6 / bne")):
        b = data[a - BASE:a - BASE + 4]
        print("    0x%08X  原厂 %s   %s" % (a, b.hex(" "), why))
        check(b[3] == 0xD1 and b[1] in (0x2A, 0x28),
              "0x%08X 结构确认：byte1=0x%02X(cmp imm) byte3=0xD1(bne)" % (a, b[1]))

    print("\n[2] 建议补丁 B（只改条件码 bne 0xD1 -> blo 0xD3，共 2 字节）")
    for a in (0x080045EF, 0x0800854F):
        print("    0x%08X  %02X -> D3" % (a, data[a - BASE]))
        check(data[a - BASE] == 0xD1, "0x%08X 原字节为 0xD1(bne)" % a)
    print("    语义：cmp rX,#6 ; blo skip => 仅 rX>=6（item6=ID / item7=F8）才重新同步")

    print("\n[3] 值上限表 0x0800E314[0..7]（inc: 0x08008544 越限回绕 0；dec: 0x08004608 取上限）")
    tbl = data[0x0800E314 - BASE:0x0800E314 - BASE + 8]
    names = ["F1 beeper", "F2 S1/S2", "F3 auto-off", "F4 delay",
             "F5 mask", "F6 t0.1", "F7 ID", "F8 稳定色温"]
    for i, v in enumerate(tbl):
        print("    [%d] %-10s = %3d (0x%02X)" % (i, names[i], v, v))
    check(tbl[7] == 1, "F8 上限=1 => settings[7] 只能 0/1")
    check(tbl[6] == 99, "F7(ID) 上限=99")

    print("\n[4] 同步字函数 0x0800B9AC（原厂 56B；Lead 将整段改写为 v4 语义）")
    print("    0x0800B9AC: %s" % hx(0x0800B9AC, 56))
    check(hx(0x0800B9E0, 4) == "68 c3 00 00", "0x0800B9E0 字面量 = 50024")
    check(hx(0x0800B9AC, 2) == "0c 4a", "0x0800B9AC 首指令 ldr r2,[pc]（函数入口）")

    print("\n[5] 同步字函数 4 个调用点")
    for a, who in ((0x080048C6, "rf_init 0x08004860（上电）"),
                   (0x080045F2, "dec 处理 0x080045AC 值编辑路径"),
                   (0x08008552, "inc 处理 0x08008504 值编辑路径"),
                   (0x08008AB6, "ui_key_scan_task：0x010|0x004 长按恢复出厂后")):
        print("    0x%08X: %s   %s" % (a, hx(a, 4), who))
        hw = int.from_bytes(data[a - BASE:a - BASE + 2], "little")
        check((hw & 0xF800) == 0xF000, "0x%08X 是 bl 指令（halfword 0x%04X）" % (a, hw))

    print("\n[6] 保存倒计时：0x0800B810 置 200；0x0800B7F4 每 tick 递减；0x08009B38 到期落盘")
    for a in (0x0800B810, 0x0800B7F4):
        print("    0x%08X: %s" % (a, hx(a, 12)))
    # 0x0800B810: ldr r1,=0x20000214 ; movs r0,#0xC8 ; strh r0,[r1,#4]
    check(hx(0x0800B812, 2) == "c8 20", "0x0800B810 写常量 0xC8=200")
    check(hx(0x0800B814, 2) == "88 80", "0x0800B810 strh 到 [0x20000214+4]=0x20000218")
    print("    -> main 每轮调用 0x08009B38：0x0800B7F4 递减；到 0 置 [0x20000214+2]=1，")
    print("       随后 param_record_build(0x20000484) -> flash_store_params 0x0800B820 ->")
    print("       flash_write_params 0x08005974 写 0x0801F000+slot*100 记录")
    print("    -> 结论：F8 改值后 200 个主循环 tick 保存（记录字节 31 = settings[7]）")

    print("\n[7] 0x0800B9E4（ui_key_scan_task 入口每轮调用；UI+0x2C 超时清 UI+0xF / UI+0x12）")
    print("    0x0800B9E4: %s" % hx(0x0800B9E4, 24))
    check(hx(0x0800B9F8, 2) == "81 74", "0x0800B9F8 strb r1,[r0,#0x12] => 超时自动退出 C.Fn")

    print("\n[8] F8 渲染分支（cfn_render 0x08003FD0）")
    for a, n in ((0x0800422A, 4), (0x08004230, 4), (0x08004234, 4),
                 (0x08004236, 4), (0x08004244, 4)):
        print("    0x%08X: %s" % (a, hx(a, n)))
    check(hx(0x08004230, 2) == "e0 79", "0x08004230 ldrb r0,[r4,#7] = settings[7]")
    check(hx(0x08004234, 2) == "06 d0", "0x08004234 beq -> settings[7]==0 走 OFF 渲染")
    print("    OFF: 0x08004244 渲染 3 段；ON: 0x08004236 渲染（F1 beeper 的 ON/OFF 同款）")

    print("\n[9] ui_key_scan_task 的 0x014 长按路径（0x08008AAA..0x08008AB6，恢复出厂后重同步）")
    for a in (0x08008AAA, 0x08008AAE, 0x08008AB0, 0x08008AB2, 0x08008AB4, 0x08008AB6):
        print("    0x%08X: %s" % (a, hx(a, 2)))
    check(hx(0x08008AAA, 4).startswith("fb f7"), "0x08008AAA bl ui_key_14 0x080048E4（恢复出厂默认）")
    check(hx(0x08008AB2, 2) == "ae 74", "0x08008AB2 strb r6,[r5,#0x12] => 关 C.Fn")
    check(hx(0x08008AB4, 2) == "80 79", "0x08008AB4 ldrb r0,[r0,#6] => 取 settings[6]=ID")
    check(hx(0x08008AB6, 4).startswith("02 f0"), "0x08008AB6 bl 0x0800B9AC")

    print("\n[10] 矩阵键解码（ui_key_scan_task 中 GPIO 掩码与位累加）")
    for a, n in ((0x080089b6, 4), (0x08008a48, 4), (0x08008a62, 4),
                 (0x080089f8, 4), (0x080089fc, 4)):
        print("    0x%08X: %s" % (a, hx(a, n)))

    bad = [m for ok, m in checks if not ok]
    print("\n===== 汇总：%d/%d PASS =====" % (len(checks) - len(bad), len(checks)))
    for m in bad:
        print("  FAIL: " + m)
    return 1 if bad else 0


class _Tee:
    def __init__(self, path, stream):
        self.f = open(path, "w", encoding="utf-8")
        self.s = stream

    def write(self, text):
        self.s.write(text)
        self.f.write(text)

    def flush(self):
        self.s.flush()
        self.f.flush()


if __name__ == "__main__":
    tee = _Tee(LOG, sys.stdout)
    with redirect_stdout(tee):
        rc = main()
    print("[log] %s" % LOG)
    sys.exit(rc)
