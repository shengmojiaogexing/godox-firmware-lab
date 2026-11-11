# -*- coding: utf-8 -*-
"""Godox V860IIIS (V860IIIS_V1.1.bin) C.Fn 菜单 / UI 逆向 —— 可重复运行脚本。

    tools\\python\\python.exe devices\\v860iiis\\re\\cfn_menu.py

产物：
  * 全部证据与结论打印到 stdout，并写入 devices/v860iiis/re/log_cfn_menu.txt
    （真实输出，重复运行覆盖）。
  * 退出码 0 = 全部检查 PASS。

覆盖内容（对应任务板的四个问题）：
  1. C.Fn 列表渲染函数地址 / 调用点 / 页(>>2)·槽位(&3)逻辑；
  2. 字形表（8x16 17B/字形、两套 8x8 9B/字形）、绘制原语、列表选中行反显、
     帧缓冲地址、LCD 刷新链路；
  3. 用 Unicorn 2.1.4 真实执行渲染函数，逐页/逐项 dump 168x96 ASCII 帧，
     从真实绘制调用反推出“每项显示名 + 右栏取值 + 取值语义 + settings 字节”；
  4. 用真实执行 INC/DEC/SET/开关/返回 handler 验证可达项、隐藏项、按键路径与
     改值后 200 tick 自动保存链路。

方法：Capstone 5.0.7 静态反汇编（skipdata=True）+ Unicorn 真实执行。
脚本只读固件；Unicorn 只在内存里跑镜像代码（对会碰外设的函数在内存副本里写
`bx lr` 桩），不写盘、不影响任何补丁模块。
"""
from __future__ import annotations

import hashlib
import os
import re
import struct
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from unicorn import Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
from unicorn.arm_const import (
    UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3,
    UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC,
)

HERE = os.path.dirname(os.path.abspath(__file__))
FW = os.path.join(HERE, '..', 'firmware', 'V860IIIS_V1.1.bin')
OUT = os.path.join(HERE, 'log_cfn_menu.txt')
BASE = 0x08003000
IMG_SIZE = 79300
IMG_SHA256 = 'bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20'
RESET_VECTOR = 0x080030D5

# ---- 关键地址常量（全部由本脚本反汇编/真实执行自证） ----------------------
FB = 0x200007AC            # 帧缓冲：12 条带 x 0xA8(168) = 2016B = 168x96 单色像素
FB_W = 0xA8                # 每个 8 行条带 168 字节（=168 列）
FB_STRIPS = 12
FB_BYTES = FB_W * FB_STRIPS
FONT16 = 0x080143D6        # 8x16 字形表：17B/字形（1B 宽 + 16B 列数据）
FONT8A = 0x08014088        # 8x8 字形表 A：9B/字形（按钮 / 8x8 文字）
FONT8B = 0x0801437C        # 8x8 字形表 B：9B/字形
DRAW16 = 0x080069C0        # 8x16 字形绘制(row, col, glyph, shift)
DRAW8A = 0x08006908        # 8x8 字形绘制 A(row, col, glyph)
DRAW8B = 0x0800693C        # 8x8 字形绘制 B(row, col, glyph)
BTN = 0x08006704           # 底部按钮框 + 8x8 标签(8 个字形)
ICON_MOON = 0x08007414     # 16x16 图标（表 0x080154A8：STBY 月亮）
ICON_BEEP = 0x08006CD0     # 16x16 图标（表 0x08015272：BEEP 喇叭）
ICON_STATUS = 0x08006EE4   # 16x16 图标 XOR（表 0x08015356：右下闪电/状态）
SHAPE1 = 0x08007230        # 多形状绘制（SCAN 进度图形，表 0x08015199）
SHAPE2 = 0x080071C8        # 多形状绘制（SCAN 2x4 结果网格，表 0x08015252）
FN_3A48 = 0x08003A48       # C.Fn 画面里周期性调用的“短音/刷新”纯 RAM 函数
INVERT = 0x08004D50        # 编辑态：把当前值/选项行反显（XOR，[EDIT]==1 时生效）
SELBAR_INLINE = (0x08004B76, 0x08004BFC)  # 选中行整行反显（render 内联，非独立函数）
RENDER = 0x0800431C        # C.Fn 画面渲染函数（0x0800431C..0x08004D03）
RENDER_NEXT = 0x08004D04   # 渲染函数的下一函数起点
DISPATCH = 0x08012124      # 画面分派任务：C.Fn 打开时调 RENDER（调用点 0x0801219E）
FILLFB = 0x08005F78        # 用 r0 填满帧缓冲（清屏）
DISP_TASK = 0x08005F4C     # 显示任务：4 次 flush + 每 5 帧一次画面重绘
LCD_FLUSH = 0x08012880     # 刷新 3 个条带(3*0xA8)到 LCD，r0=page(0..3)
LCD_DATA = 0x08010CC4      # 单字节写 LCD 数据
SCREEN = 0x200004D4        # C.Fn 画面打开标志（1=打开）
EDIT = 0x200004D5          # C.Fn 编辑态（1=编辑）
SEL = 0x200004D6           # C.Fn 当前选中项索引（0..9）
SUBSTATE = 0x200004D7      # C.Fn/菜单子状态
SETTINGS = 0x20001058      # 设置字节数组基址（item n = SETTINGS+n）
MAXTAB = 0x08016102        # 12 项最大值/回绕表
MAX_BYTES = bytes.fromhex('01010101011f630102060101')
CFN_TOGGLE = 0x08004D04    # C.Fn 画面开/关切换（键码 8）
CFN_BACK = 0x0800CCB0      # 返回键（键码 1）：关闭 C.Fn
SET_KEY = 0x08011454       # SET 键：切换编辑态（SCAN 特例）
PM_SCAN = 0x080058E4       # +/- 键扫描（GPIOB 0x48000800 bit7/8）
INC = 0x0800C528           # C.Fn '+' handler
DEC = 0x080059E4           # C.Fn '-' handler
KEY_TASK = 0x0800CD7C      # 按键矩阵扫描/分发任务（含键码分派 0x0800CF72）
KEY_UI = 0x2000045E        # 按键任务 UI 结构基址（键码在 +0xA = 0x20000468）
MAIN_LOOP = 0x08013D60     # 主循环任务调度区（0x08013DA6 起按顺序调任务）
SYNC_FN = 0x080115B8       # 同步字函数 void rf_set_sync(uint8_t id)
UIDIV = 0x080030F0         # __aeabi_uidiv(r0/r1)
WRITE_REG = 0x08013926     # WriteReg(r0=reg, r1=val)
DELAY = 0x08013698         # delay(r0=count)
STROBE = 0x080139E8        # strobe(r0=cmd)
SAVE_CNT = 0x20000516      # 保存倒计时（半字）
SAVE_SET = 0x080109C8      # [0x20000516] = 200（每次按键）
SAVE_TICK = 0x080109A4     # 倒计时递减；到 0 置 [0x20000518]=1
SAVE_FLAG = 0x20000518     # 保存请求标志
SAVE_TASK = 0x0800D9C8     # 保存任务：tick -> 标志 -> 组记录 -> 写 flash
SAVE_REC = 0x0800EEB0      # 组/写 flash 记录（比较 20 槽）
REC_WRITE = 0x08010D94     # 复制 0x20000FD4.. 到记录缓冲后写 flash
FLASH_REC = 0x0801F000     # 设置记录区（app 镜像之外：镜像末 0x080165C4）
REC_NUM = 0x20000428       # 当前记录槽号（0..19）
DEFAULTS_FN = 0x08005E54   # 出厂默认值初始化（写 settings[0..9]）
LCD_SCALE_TAB = 0x08015A48 # LCD(item9) 档位表：7 档 x 2B（0x42='-'/0x43='+'）
LCD_APPLY = 0x080128F0     # 把 settings[9] 应用到 LCD 亮度（导航到 9 / 改值时调用）
SCAN_STATE = 0x20000563    # SCAN 子状态（0=OFF/START，1/2=进度，3=结果网格）
SCAN_PCT = 0x20000569      # SCAN 进度值（0..100）
SCAN_GRID = 0x2000057D     # SCAN 结果 8 个值
SCAN_FN = 0x0800EAE8       # 停止/启动 SCAN 的外设函数（测试中桩化）
KEY_TIMEOUT = KEY_UI + 0xC # 画面超时计数（每次按键置 0x7D0）
BACKLIGHT = 0x20000542     # 每次按键置 0x28

SCREEN_FLAG = 0x20000536   # 值为 1 时 +/- handler 直接返回（非 C.Fn 画面）
SPECIAL_545 = 0x20000545   # 其它菜单/子画面特殊标志
FMODE = 0x20000563         # =SCAN_STATE 的别名（同一地址）

SYM = {62: '=', 63: '+', 64: '.', 65: '>', 66: '-', 67: '+', 68: '/',
       69: '=', 70: '-'}

lines = []


def w(s=''):
    lines.append(s)


def g2c(g):
    """字形索引 → 字符（0-9 / A-Z / a-z / 符号）。"""
    if 0 <= g <= 9:
        return chr(0x30 + g)
    if 10 <= g <= 35:
        return chr(0x41 + g - 10)
    if 36 <= g <= 61:
        return chr(0x61 + g - 36)
    return SYM.get(g, '{%02X}' % g)


def main():
    data = open(FW, 'rb').read()
    ok = True

    def chk(cond, msg, extra=''):
        nonlocal ok
        ok = ok and bool(cond)
        w('  [%s] %s%s' % ('PASS' if cond else 'FAIL', msg,
                           ('  ' + extra) if extra else ''))

    # =====================================================================
    w('=' * 78)
    w('Godox V860IIIS C.Fn 菜单 / UI 逆向  —  cfn_menu.py 输出')
    w('=' * 78)
    w('')
    w('固件: devices/v860iiis/firmware/V860IIIS_V1.1.bin')
    w('  长度   : %d 字节' % len(data))
    w('  sha256 : %s' % hashlib.sha256(data).hexdigest())
    w('  镜像基址: 0x%08X（vector[1] Reset=0x%08X）' % (
        BASE, struct.unpack_from('<I', data, 4)[0]))
    w('  镜像末端: 0x%08X' % (BASE + len(data)))
    chk(len(data) == IMG_SIZE, '固件长度 = %d B' % IMG_SIZE)
    chk(hashlib.sha256(data).hexdigest() == IMG_SHA256, 'sha256 与任务板一致')
    chk(struct.unpack_from('<I', data, 4)[0] == RESET_VECTOR,
        'Reset 向量 = 0x%08X（基址 0x%08X）' % (RESET_VECTOR, BASE))

    # ---- Capstone：整镜像线性反汇编（skipdata 必须开：函数内嵌字面量/位图）
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.skipdata = True
    insns = list(md.disasm(data, BASE))
    by = {i.address: i for i in insns}

    def litval(ins):
        """LDR(literal) -> (字面量地址, 值)；否则 (None, None)。"""
        if ins.mnemonic != 'ldr' or '[pc,' not in ins.op_str:
            return None, None
        m = re.search(r'#(-?0x[0-9a-fA-F]+|-?\d+)', ins.op_str)
        if not m:
            return None, None
        v = m.group(1)
        imm = int(v, 16) if 'x' in v else int(v)
        la = ((ins.address + 4) & ~3) + imm
        off = la - BASE
        if off < 0 or off + 4 > len(data):
            return None, None
        return la, struct.unpack_from('<I', data, off)[0]

    def blt(ins):
        if ins.mnemonic not in ('bl', 'blx'):
            return None
        m = re.search(r'#(0x[0-9a-fA-F]+)', ins.op_str)
        return (int(m.group(1), 16) & ~1) if m else None

    # 函数入口：BL 目标 + 向量表
    entries = set()
    for i in insns:
        t = blt(i)
        if t is not None and BASE <= t < BASE + len(data):
            entries.add(t)
    for off in range(0, 0x50, 4):
        v = struct.unpack_from('<I', data, off)[0]
        if 0x08000000 <= v < 0x09000000 and BASE <= (v & ~1) < BASE + len(data):
            entries.add(v & ~1)
    entries = sorted(entries)

    import bisect

    def fn_bounds(addr):
        k = bisect.bisect_right(entries, addr) - 1
        s = entries[k] if k >= 0 else BASE
        e = entries[k + 1] if k + 1 < len(entries) else BASE + len(data)
        return s, e

    def snippet(lo, hi, label=''):
        w('')
        w('  -- %s  [0x%08X..0x%08X]' % (label, lo, hi))
        a = lo
        while a <= hi:
            ins = by.get(a)
            if ins is None:
                a += 2
                continue
            extra = ''
            la, lv = litval(ins)
            if lv is not None and (0x20000000 <= lv < 0x20002000 or lv >= 0x08010000):
                extra = '   ; lit@0x%08X = 0x%08X' % (la, lv)
            w('    0x%08X  %-12s %-7s %-24s%s' % (
                ins.address, ins.bytes.hex(' '), ins.mnemonic, ins.op_str, extra))
            a += ins.size

    def insns_between(lo, hi):
        return [i for i in insns if lo <= i.address < hi]

    def has_insn(addr, mnemonic=None, frag=None):
        ins = by.get(addr)
        if ins is None:
            return False
        if mnemonic and ins.mnemonic != mnemonic:
            return False
        if frag and frag not in ins.op_str:
            return False
        return True

    def bl_sites(target):
        return [i.address for i in insns if blt(i) == target]

    def lit_sites(value):
        return [i.address for i in insns if litval(i)[1] == value]

    # =====================================================================
    w('')
    w('=' * 78)
    w('0. 结论摘要（详见各节证据）')
    w('=' * 78)
    w('')
    w('  1) C.Fn 列表渲染函数 = 0x0800431C（0x0800431C..0x08004D03，2536 B）；')
    w('     唯一调用点 0x0801219E（画面分派任务 0x08012124 内，[0x200004D4]==1 时）；')
    w('     页/槽位：page = idx>>2、slot = idx&3（0x080043C2 / 0x080043C4）。')
    w('  2) 帧缓冲 0x200007AC（12 条带 x 0xA8 = 2016 B = 168x96）；')
    w('     8x16 字形表 0x080143D6（17B/字形），8x8 表 0x08014088 / 0x0801437C（9B/字形）；')
    w('     列表选中行反显 = render 内联 0x08004B76..0x08004BFC；')
    w('     编辑态值行反显 = 0x08004D50；LCD 刷新 0x08012880（每次 3 条带）。')
    w('  3) 真实执行渲染函数得到：本机型 C.Fn 只有 10 项（idx 0..9）：')
    w('     m/ft, AF, STBY, RX STBY, SCAN, CH, ID, BEEP, LIGHT, LCD。')
    w('     其中 LIGHT(idx8) = 12sec/OFF/ON 三档（max=2，默认 0=12sec，settings 0x20001058+8）。')
    w('     FEB ACL / TX 两项在本机型 UI 中不存在（最大值表里仍留有它们的 2 个字节）。')
    w('  4) 按键：键码 8 切换 C.Fn（0x08004D04）、返回键(1) 0x0800CCB0、SET 0x08011454、')
    w('     +/- 0x080058E4 -> INC 0x0800C528 / DEC 0x080059E4；每次按键保存倒计时')
    w('     0x20000516=200，tick 0x080109A4 -> 标志 0x20000518 -> 任务 0x0800D9C8 ->')
    w('     写 flash 0x0801F000。')

    # =====================================================================
    w('')
    w('=' * 78)
    w('1. 字形表（C.Fn 文字就是这些 8x16 字形）与索引->字符映射')
    w('=' * 78)
    w('')
    w('  8x16 表 0x%08X：17 字节/字形（宽度 1B + 16B 列数据，每列 2B = 16 像素高）。' % FONT16)
    w('  8x8  表 A 0x%08X / 表 B 0x%08X：9 字节/字形（宽度 1B + 8B 列数据）。'
      % (FONT8A, FONT8B))
    w('  字形索引 -> 字符：0-9=数字，10-35=A-Z，36-61=a-z，62+ 符号/图元。')

    glyph_w = {g: data[FONT16 - BASE + 17 * g] for g in range(120)}

    def glyph_art(gi, font=FONT16, per=17, h=16):
        off = font - BASE + per * gi
        n = data[off]
        rows = [''] * h
        for x in range(n):
            if h == 16:
                top = data[off + 1 + 2 * x]
                bot = data[off + 2 + 2 * x]
            else:
                top = data[off + 1 + x]
                bot = 0
            for y in range(8):
                rows[y] += '#' if (top >> (7 - y)) & 1 else '.'
                if h == 16:
                    rows[8 + y] += '#' if (bot >> (7 - y)) & 1 else '.'
        return rows

    def decode_text(items):
        """items = [(col, glyph)]（已按 col 升序）；大间隔补空格。"""
        out = []
        prev_end = None
        for col, g in items:
            if prev_end is not None and col - prev_end >= 4:
                out.append(' ')
            out.append(g2c(g))
            prev_end = col + glyph_w.get(g, 0)
        return ''.join(out)

    # 已知画面字符串自证映射：标题 "C.Fn"、版本 "Ver1.1"、按钮 "CLEAR"
    title = [(0x19, 0x0C), (0x20, 0x40), (0x23, 0x0F), (0x2A, 0x31)]
    ver = [(0x80, 0x1F), (0x87, 0x28), (0x8E, 0x35), (0x97, 0x01),
           (0x9E, 0x40), (0xA1, 0x01)]
    clear = [(0x06, 0x0C), (0x0C, 0x15), (0x12, 0x0E), (0x18, 0x0A), (0x1E, 0x1B)]
    t_title = decode_text(title)
    t_ver = decode_text(ver)
    t_clear = ''.join(g2c(g) for _, g in clear)
    w('  自证 1：顶栏标题字形 0x0C,0x40,0x0F,0x31 解码 = %r' % t_title)
    w('  自证 2：顶栏版字形 0x1F,0x28,0x35,0x01,0x40,0x01 解码 = %r' % t_ver)
    w('  自证 3：底部按钮字形 0x0C,0x15,0x0E,0x0A,0x1B 解码 = %r' % t_clear)
    chk(t_title == 'C.Fn', '字形映射自证：标题 "C.Fn"')
    chk(t_ver == 'Ver1.1', '字形映射自证：版本 "Ver1.1"（本固件 V1.1）')
    chk(t_clear == 'CLEAR', '字形映射自证：按钮 "CLEAR"')
    w('')
    w('  字形样例（8x16）：')
    for gi in (0x0C, 0x0F, 0x01, 0x30, 0x44):
        w('    glyph 0x%02X = %r width=%d' % (gi, g2c(gi), glyph_w[gi]))
        for r in glyph_art(gi):
            w('      ' + r)
    w('')
    w('  8x8 字形样例（表 A 0x%08X）：' % FONT8A)
    for gi in (0x0C, 0x0A, 0x15, 0x0E, 0x1B):
        w('    idx 0x%02X=%r  ' % (gi, g2c(gi)) + ' '.join(glyph_art(gi, FONT8A, 9, 8)))

    # =====================================================================
    w('')
    w('=' * 78)
    w('2. 绘制原语、帧缓冲几何与 LCD 刷新链路')
    w('=' * 78)
    snippet(DRAW16, 0x08006A1E, '8x16 字形绘制 0x080069C0：row*0xA8 + col，第二条带 +0xA8')
    w('')
    w('  语义：0x080069C0(r0=条带行, r1=列, r2=字形索引, r3=移位)')
    w('        写 fb + r0*0xA8 + col 与 fb + r0*0xA8 + 0xA8 + col 两个字节（16 行高）。')
    la16, lv16 = litval(by[0x080069C8])
    lafb, lvfb = litval(by[0x080069DA])
    w('  实测字面量：0x080069C8 -> 0x%08X（=8x16 表）；0x080069DA -> 0x%08X（=帧缓冲）'
      % (lv16, lvfb))
    chk(lv16 == FONT16, 'DRAW16 直接引用 8x16 字形表 0x080143D6')
    chk(lvfb == FB, 'DRAW16 直接引用帧缓冲 0x200007AC')
    chk(has_insn(0x080069D6, 'movs', '#0xa8'), 'DRAW16 条带步长 = 0xA8(168) 字节')
    snippet(INVERT, 0x08004D9A, '编辑态值行反显 0x08004D50（XOR 0x3F/0xFF，[EDIT]==1 时）')
    w('')
    w('  选中行整行反显在渲染函数内联（0x%08X..0x%08X）：' % SELBAR_INLINE)
    w('    r0=[sp,#0x20]（=slot=idx&3）；地址 = FB + (slot+1)*0x150 + x，')
    w('    x=1..0x40，先 XOR 0x7F 再 XOR 0xFF；即整行 y=(slot+1)*16 .. +15。')
    snippet(SELBAR_INLINE[0], SELBAR_INLINE[1], '列表选中行反显（内联）')
    w('')
    w('  逻辑上等价于 V1C 的 0x08005260；V1C 报告把它当独立函数，实际它是渲染函')
    w('  数尾部的内联块（本镜像同样内联在 render 内，且无 BL 调用者）。')
    chk(bl_sites(SELBAR_INLINE[0]) == [] and fn_bounds(SELBAR_INLINE[0])[0] == RENDER,
        '选中行反显是 RENDER 内联代码（无独立函数入口）')
    snippet(LCD_FLUSH, 0x080128F0, 'LCD 刷新 0x08012880(page)：3 条带 x 0xA8 字节')
    w('')
    w('  语义：r0=page(0..3) -> r0*3 得到起始条带；内层 168 次、外层 3 次；')
    w('        每字节调用 0x08010CC4（LCD 数据写）。')
    chk(has_insn(0x08012882, 'lsls', '#1') and
        has_insn(0x08012884, 'adds', 'r0, r0, r1'),
        'LCD_FLUSH 先用 page*3 计算条带基址')
    chk(has_insn(0x080128DC, 'cmp', '#0xa8') and has_insn(0x080128E4, 'cmp', '#3'),
        'LCD_FLUSH 内层 0xA8(168) 字节、外层 3 条带')
    chk(bl_sites(LCD_FLUSH) == [0x08005F68, 0x08008A84, 0x08008A8A,
                                0x08008A90, 0x08008A96],
        'LCD_FLUSH 共 5 个调用点：显示任务 1 处 + 全屏刷新 4 处(page=0..3)')
    chk(has_insn(0x08008A82, 'movs', 'r0, #0') and
        has_insn(0x08008A88, 'movs', 'r0, #1') and
        has_insn(0x08008A8E, 'movs', 'r0, #2') and
        has_insn(0x08008A94, 'movs', 'r0, #3'),
        '全屏刷新依次 flush page=0/1/2/3（=12 条带 = 整屏）')
    snippet(DISP_TASK, 0x08005F78, '显示任务 0x08005F4C：4 页 flush + 每 5 帧重绘')
    w('')
    w('  语义：0x20000114+7 是帧计数；<=4 时 flush(page=计数)，==5 时调画面分派。')
    chk(litval(by[0x08005F4E])[1] == 0x20000114, '显示任务用 0x20000114+7 做帧计数')
    chk(blt(by[0x08005F6E]) == DISPATCH, '显示任务第 5 帧调用画面分派 0x08012124')
    chk(bl_sites(DISP_TASK) == [0x08013DA6], '显示任务由主循环 0x08013DA6 调度')
    w('')
    w('  主循环任务表（0x08013D92 之后按顺序调用，见 0x08013D9E..0x08013DBE）：')
    for a in (0x0800C4D4, 0x08009838, DISP_TASK, KEY_TASK, PM_SCAN,
              0x08003950, 0x080096AC, 0x08003A60, 0x0800C320):
        sites = [s for s in bl_sites(a) if 0x08013D00 <= s < 0x08013E00]
        w('    0x%08X <- 主循环调用点 %s' % (
            a, ['0x%08X' % s for s in sites] if sites else '(非直接 BL)'))
    chk(bl_sites(KEY_TASK) == [0x08013DAA] and bl_sites(PM_SCAN) == [0x08013DAE],
        '按键任务/+- 扫描都由主循环按固定顺序调度')
    w('')
    w('  → 帧缓冲 0x%08X = %d 条带 x 0xA8 = %d 字节 = %d 列 x %d 行单色像素。'
      % (FB, FB_STRIPS, FB_BYTES, FB_W, FB_STRIPS * 8))
    chk(FB + FB_BYTES == 0x20000F8C, '帧缓冲范围 0x200007AC..0x20000F8B（设置在 0x20001058）')
    snippet(FILLFB, 0x08005FE0, '清屏/填充 0x08005F78(r0=填充值)：12 条带 x 0xA8')
    chk(litval(by[0x08005F80])[1] == FB, '清屏函数 0x08005F78 直接写帧缓冲')

    # =====================================================================
    w('')
    w('=' * 78)
    w('3. C.Fn 渲染函数、调用点与页/槽位拆分')
    w('=' * 78)
    rs, re_ = fn_bounds(RENDER)
    w('')
    w('  渲染函数 0x%08X..0x%08X（%d 字节），下一函数从 0x%08X 开始。'
      % (rs, re_ - 1, re_ - rs, re_))
    chk(rs == RENDER and re_ == RENDER_NEXT, '渲染函数边界 = 0x0800431C..0x08004D03')
    snippet(RENDER, 0x08004340, '渲染函数入口：顶栏 + 帧计数')
    snippet(0x080043BC, 0x080043D8, '页/槽位：page=idx>>2, slot=idx&3；page 0/1/2 分派')
    chk(litval(by[0x080043BC])[1] == SEL, '渲染函数读选中项索引 0x200004D6')
    chk(has_insn(0x080043C2, 'lsrs', 'r0, r1, #2'), 'page = idx >> 2（0x080043C2）')
    chk(has_insn(0x080043C4, 'lsls', 'r1, r1, #0x1e') and
        has_insn(0x080043C6, 'lsrs', 'r1, r1, #0x1e'), 'slot = idx & 3（0x080043C4/C6）')
    chk(has_insn(0x080043CA, 'cmp', 'r0, #0') and has_insn(0x080043D2, 'cmp', 'r0, #2'),
        'page 分派只处理 0/1/2 三页（无 page3）')
    w('')
    w('  渲染函数 BL 目标（静态计数）：')
    cnt = {}
    for i in insns_between(rs, re_):
        t = blt(i)
        if t is not None:
            cnt[t] = cnt.get(t, 0) + 1
    for t, n in sorted(cnt.items(), key=lambda x: -x[1]):
        w('    0x%08X  x%d' % (t, n))
    chk(cnt.get(DRAW16) == 114, '渲染函数画 114 个 8x16 字形')
    chk(cnt.get(INVERT) == 8, '渲染函数 8 处调用编辑态反显 0x08004D50')
    chk(cnt.get(BTN) == 4, '渲染函数 4 个底部按钮')
    chk(cnt.get(DRAW8A) == 8, '渲染函数 8 处 8x8 数字（SCAN 进度/结果，运行时循环）')
    w('')
    w('  渲染函数的唯一调用点（画面分派）：')
    for a in bl_sites(RENDER):
        s, e = fn_bounds(a)
        w('    0x%08X  bl 0x%08X   (所在函数 0x%08X..0x%08X)' % (a, RENDER, s, e - 1))
    chk(bl_sites(RENDER) == [0x0801219E],
        'C.Fn 渲染只有 1 个调用点 0x0801219E')
    snippet(DISPATCH, 0x080121B0, '画面分派 0x08012124：[0x200004D4]==1 时画 C.Fn')
    chk(litval(by[0x08012184])[1] == SCREEN, '分派读 C.Fn 画面标志 0x200004D4')
    chk(has_insn(0x0801218A, 'beq', '#0x801219e'), '标志==1 时 beq 到渲染调用')
    chk(has_insn(0x0801219E, 'bl', '#0x800431c'), '调用点 0x0801219E -> 0x0800431C')
    w('')
    w('  分派里还有清屏调用：0x08012154/0x08012166 两处 bl 0x08005F78，')
    w('  传 r0=0xFF（关屏白/黑）或 r0=0。')
    chk(has_insn(0x08012154, 'bl', '#0x8005f78') and
        has_insn(0x08012166, 'bl', '#0x8005f78'), '分派中清屏调用 0x08005F78 x2')

    # =====================================================================
    w('')
    w('=' * 78)
    w('4. 用 Unicorn 真实执行渲染函数，记录每一步绘制 → 还原 10 项')
    w('=' * 78)
    w('')
    w('  harness：映射 0x08000000..0x0801FFFF（镜像@0x08003000）与 0x20000000..0x20003FFF；')
    w('  置 [0x200004D4]=1、[0x200004D5]=edit、[0x200004D6]=idx、settings[0..15]；')
    w('  代码 hook 记录所有绘制 helper 的入参；返回陷阱 0x08002001 时停止。')
    w('  渲染路径全是纯绘制/除法，无需桩。')

    REC_KEYS = (DRAW16, INVERT, BTN, DRAW8A, DRAW8B,
                ICON_MOON, ICON_BEEP, ICON_STATUS, SHAPE1, SHAPE2, FN_3A48)
    REC = {a: [] for a in REC_KEYS}

    class Emu:
        def __init__(self, stubs=(), rec=None):
            self.mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
            self.mu.mem_map(0x08000000, 0x20000)
            img = bytearray(data)
            for s in stubs:
                img[s - BASE:s - BASE + 2] = b'\x70\x47'      # bx lr
            self.mu.mem_write(BASE, bytes(img))
            self.mu.mem_map(0x20000000, 0x4000)
            self.rec = rec
            self.mu.hook_add(UC_HOOK_CODE, self._hook)

        def _hook(self, mu, address, size, ud):
            if 0x08002000 <= address < 0x08003000:
                mu.emu_stop()
                return
            if self.rec is None:
                return
            lst = self.rec.get(address)
            if lst is None:
                return
            args = [mu.reg_read(r) for r in
                    (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3)]
            stack = []
            if address == BTN:
                sp = mu.reg_read(UC_ARM_REG_SP)
                stack = [int.from_bytes(bytes(mu.mem_read(sp + 4 * i, 4)), 'little') & 0xFF
                         for i in range(5)]
            lst.append((args, stack))

        def reset(self):
            self.mu.mem_write(0x20000000, b'\x00' * 0x4000)

        def poke(self, addr, blob):
            self.mu.mem_write(addr, bytes(blob))

        def peek(self, addr, n):
            return bytes(self.mu.mem_read(addr, n))

        def call(self, addr, max_insns=3_000_000):
            self.mu.reg_write(UC_ARM_REG_SP, 0x20001300)
            self.mu.reg_write(UC_ARM_REG_LR, 0x08002001)
            try:
                self.mu.emu_start(addr | 1, 0, count=max_insns)
            except UcError:
                return False
            return True

    def render(idx, edit=0, settings=None, ram=None):
        for k in REC:
            REC[k] = []
        e = Emu(rec=REC)
        e.poke(SCREEN, [1])
        e.poke(EDIT, [edit])
        e.poke(SEL, [idx])
        e.poke(SETTINGS, settings if settings is not None else [0] * 16)
        for addr, blob in (ram or {}).items():
            e.poke(addr, blob)
        good = e.call(RENDER)
        return good, e.peek(FB, FB_BYTES), e

    def label_rows():
        """左栏（x<0x40）标签：{strip_row: [(col, glyph)]}，只取列表 4 行。"""
        rows = {}
        for (args, stack) in REC[DRAW16]:
            row, col, gi, sh = args
            if row in (2, 4, 6, 8) and col < 0x40:
                rows.setdefault(row, []).append((col, gi))
        return rows

    def labels():
        rows = label_rows()
        return [(r, decode_text(sorted(rows[r]))) for r in sorted(rows)]

    def opt_rows():
        """右栏（col>=0x58）文字行 + 箭头行（col=0x50, glyph=0x41）。"""
        rows = {}
        arrow = []
        for (args, stack) in REC[DRAW16]:
            row, col, gi, sh = args
            if row < 4:
                continue
            if col >= 0x58:
                rows.setdefault(row, []).append((col, gi))
            elif 0x48 <= col < 0x58 and gi == 0x41:
                arrow.append(row)
        return [(r, decode_text(sorted(rows[r]))) for r in sorted(rows)], sorted(set(arrow))

    def icons():
        out = {}
        for addr, name in ((ICON_MOON, 'moon'), (ICON_BEEP, 'beep'),
                           (ICON_STATUS, 'status'), (SHAPE1, 'shape1'),
                           (SHAPE2, 'shape2')):
            if REC[addr]:
                out[name] = [(a[0], a[1]) for (a, st) in REC[addr]]
        return out

    # ---------------------------------------------------------------- 4.1
    w('')
    w('  4.1 顶栏（"C.Fn" + "Ver1.1"）——由真实绘制调用解码')
    good, fb, e = render(0)
    chk(good, '渲染 idx=0 成功')
    top = [(args[1], args[2]) for (args, st) in REC[DRAW16] if args[0] == 0]
    w('    顶栏绘制（row=0）：%s' % ' '.join('c0x%02X:g0x%02X' % t for t in top))
    t_ver2 = decode_text([(c, g) for c, g in top if c >= 0x80])
    t_title2 = decode_text([(c, g) for c, g in top if c < 0x80])
    w('    解码：右侧=%r  左侧=%r' % (t_ver2, t_title2))
    chk(t_title2 == 'C.Fn' and t_ver2 == 'Ver1.1',
        '顶栏由真实绘制调用解码 = "C.Fn" + "Ver1.1"（本固件版本号）')

    # ---------------------------------------------------------------- 4.2
    w('')
    w('  4.2 每页 4 行标签（左栏 x<0x40）→ 本机型 10 项')
    page_labels = {}
    for page in range(3):
        good, fb, e = render(page * 4)
        chk(good, '渲染 page=%d 成功' % page)
        labs = labels()
        page_labels[page] = labs
        w('    page %d: %s' % (page, ', '.join('%s(row%d)' % (t, r) for r, t in labs)))
        for r, t in labs:
            gi = [g for c, g in sorted(label_rows().get(r, []))]
            w('      项 %2d  标签 %-8r 字形索引 %s' % (
                page * 4 + (r // 2 - 1), t, ' '.join('0x%02X' % x for x in gi)))
    expected_pages = {
        0: [(2, 'm/ft'), (4, 'AF'), (6, 'STBY'), (8, 'RX STBY')],
        1: [(2, 'SCAN'), (4, 'CH'), (6, 'ID'), (8, 'BEEP')],
        2: [(2, 'LIGHT'), (4, 'LCD')],
    }
    chk(page_labels == expected_pages, '各页标签与真实绘制一致（page2 只有 2 项）')
    n_labels = sum(len(v) for v in page_labels.values())
    chk(n_labels == 10, '全菜单共 10 行标签 → 10 项（不是 12 项）')
    w('')
    w('  → 12 项变 10 项的直接原因：page2 分支代码 0x080049C0..0x08004B74 只画')
    w('    LIGHT(row2) 与 LCD(row4) 两行（row6/row8 空白），没有 FEB ACL/TX。')

    # ---------------------------------------------------------------- 4.3
    w('')
    w('  4.3 每项右栏“值/选项”栏（x=0x5A 起；0x50 的 0x41 字形是箭头）+ 图标')
    item_val = {}
    for idx in range(10):
        good, fb, e = render(idx)
        chk(good, '渲染 idx=%d 成功' % idx)
        opts, arrow = opt_rows()
        item_val[idx] = (opts, arrow)
        w('    idx=%2d 值行: %-46s 箭头行: %s   图标: %s' % (
            idx, '  |  '.join('%d:%s' % o for o in opts) if opts else '(无文字)',
            arrow if arrow else '-', icons() or '-'))
    expected_opts = {
        0: ([(4, 'm'), (6, 'ft')], [4]),
        1: ([(4, 'ON'), (6, 'OFF')], [4]),
        2: ([(4, 'ON'), (6, 'OFF')], [4]),
        3: ([(4, '60min'), (6, '30min')], [4]),
        4: ([(4, 'OFF'), (6, 'START')], [4]),
        5: ([(4, '01')], [4]),
        6: ([(4, 'OFF')], [4]),
        7: ([(4, 'OFF'), (6, 'ON')], [4]),
        8: ([(4, '12sec'), (6, 'OFF'), (8, 'ON')], [4]),
        9: ([(4, '-2'), (6, '-3')], [6]),
    }
    chk(item_val == expected_opts, '10 项右栏取值/箭头与真实绘制一致')
    chk(render(2)[0] and icons().get('moon') == [(2, 70)],
        'idx=2(STBY) 画 16x16 月亮图标（0x08007414, row2,col70）')
    chk(render(7)[0] and icons().get('beep') == [(2, 70)],
        'idx=7(BEEP) 画 16x16 喇叭图标（0x08006CD0, row2,col70）')
    chk(render(0)[0] and icons().get('status') == [(10, 142)],
        '每帧右下画 16x16 状态图标（0x08006EE4, row10,col142）')
    chk(render(2)[0] and icons().get('beep') is None and
        icons().get('moon') is not None, '月亮/喇叭图标只出现在各自项')

    # ---------------------------------------------------------------- 4.4
    w('')
    w('  4.4 取值语义验证（改 settings 后看箭头/文字变化）')
    # CH：显示 settings[5]+1（两位数）
    render(5, settings=[0] * 5 + [0] + [0] * 10)
    ch5_0 = opt_rows()
    render(5, settings=[0] * 5 + [31] + [0] * 10)
    ch5_31 = opt_rows()
    w('    CH   idx=5 settings[5]=0  -> %s' % (ch5_0,))
    w('    CH   idx=5 settings[5]=31 -> %s   (= 显示 32)' % (ch5_31,))
    chk(ch5_0 == ([(4, '01')], [4]) and ch5_31 == ([(4, '32')], [4]),
        'CH 显示 settings[5]+1 的两位十进制（0->01，31->32）')
    # ID：0 -> OFF，否则两位数字
    render(6, settings=[0] * 6 + [0] + [0] * 9)
    id0 = opt_rows()
    render(6, settings=[0] * 6 + [1] + [0] * 9)
    id1 = opt_rows()
    render(6, settings=[0] * 6 + [37] + [0] * 9)
    id37 = opt_rows()
    w('    ID   idx=6 ID=0 -> %s ; ID=1 -> %s ; ID=37 -> %s' % (id0, id1, id37))
    chk(id0 == ([(4, 'OFF')], [4]), 'ID=0 显示 "OFF"（密钥/关闭模式）')
    chk(id1 == ([(4, '01')], [4]) and id37 == ([(4, '37')], [4]),
        'ID=1..99 显示两位十进制')
    # 选项项：值 0/1 -> 箭头行 4/6（0=第一项，1=第二项）
    for idx, opts in ((1, ['ON', 'OFF']), (2, ['ON', 'OFF']),
                      (3, ['60min', '30min']), (4, ['OFF', 'START']),
                      (7, ['OFF', 'ON'])):
        r0 = render(idx, settings=[0] * idx + [0] + [0] * (15 - idx))
        o0, a0 = opt_rows()
        r1 = render(idx, settings=[0] * idx + [1] + [0] * (15 - idx))
        o1, a1 = opt_rows()
        w('    idx=%d 值0 -> 箭头%s 值1 -> 箭头%s' % (idx, a0, a1))
        chk(a0 == [4] and a1 == [6] and [t for _, t in o0] == opts,
            'idx=%d 选项顺序 %s，值 0/1 箭头行 4/6' % (idx, opts))
    # LIGHT：三档 12sec/OFF/ON，箭头行 = 2*value+4
    light_arrows = []
    for v in (0, 1, 2):
        render(8, settings=[0] * 8 + [v] + [0] * 7)
        o, a = opt_rows()
        light_arrows.append(a)
        w('    LIGHT idx=8 value=%d -> 值行 %s 箭头 %s' % (v, o, a))
    chk(light_arrows == [[4], [6], [8]],
        'LIGHT 三档：箭头行 = 2*settings[8]+4（0->row4,1->row6,2->row8）')
    # LIGHT 编辑态反显行
    render(8, edit=1, settings=[0] * 8 + [2] + [0] * 7)
    inv_args = [a[0] for (a, st) in REC[INVERT]]
    w('    LIGHT 编辑态(value=2)：INVERT 入参 %s（0x08004D50）' % inv_args)
    chk(inv_args == [2], 'LIGHT 编辑态反显选中档（INVERT(settings[8])）')
    # LCD：竖排 -3..+3，箭头固定在 row6
    vert = {-3: '-3', -2: '-2', -1: '-1', 0: '0', 1: '+1', 2: '+2', 3: '+3'}
    render(3, settings=[0] * 3 + [0] + [0] * 12)
    chk(opt_rows()[1] == [4], 'AF/STBY/… 值 0 箭头在 row4')
    lcd_ok = True
    for v in range(7):
        render(9, settings=[0] * 9 + [v] + [0] * 6)
        o, a = opt_rows()
        rowtxt = dict(o)
        exp_cur = vert[v - 3]
        exp_next = vert[v - 3 + 1] if v < 6 else None
        exp_prev = vert[v - 3 - 1] if v > 0 else None
        got = (rowtxt.get(6), rowtxt.get(4), rowtxt.get(8))
        exp = (exp_cur, exp_next, exp_prev)
        w('    LCD settings[9]=%d -> 当前 %r 上档 %r 下档 %r 箭头 %s'
          % (v, got[0], got[1], got[2], a))
        lcd_ok = lcd_ok and got == exp and a == [6]
    chk(lcd_ok, 'LCD 竖排刻度 -3..+3（表 0x08015A48），箭头固定 row6 指当前档')
    chk(data[LCD_SCALE_TAB - BASE:LCD_SCALE_TAB - BASE + 14] ==
        bytes.fromhex('4203420242010000430143024303'),
        'LCD 档位表 0x08015A48 = "-3,-2,-1,0,+1,+2,+3"（0x42=- / 0x43=+）')
    # SCAN 状态显示
    render(4, settings=[0] * 4 + [0] + [0] * 11)
    scan0 = (opt_rows(), len(REC[DRAW8A]))
    render(4, settings=[0] * 4 + [1] + [0] * 11)
    scan1 = (opt_rows(), len(REC[DRAW8A]))
    w('    SCAN settings[4]=0 -> %s ; =1 -> %s' % (scan0, scan1))
    chk(scan0[0] == ([(4, 'OFF'), (6, 'START')], [4]) and
        scan1[0] == ([(4, 'OFF'), (6, 'START')], [6]),
        'SCAN 右栏 OFF/START，箭头随 settings[4]')
    for st in (1, 2, 3):
        good, fb, e = render(4, ram={SCAN_STATE: [st], SCAN_PCT: [10],
                                     SCAN_GRID: list(range(1, 9))})
        w('    SCAN state=%d -> 8x8 绘制 %d 个, 形状 %s'
          % (st, len(REC[DRAW8A]), icons()))
        if st in (1, 2):
            chk(good and len(REC[DRAW8A]) >= 3 and 'shape1' in icons(),
                'SCAN state=%d 显示进度数字 + 图形' % st)
        else:
            chk(good and len(REC[DRAW8A]) >= 8 and 'shape2' in icons(),
                'SCAN state=3 显示 2x4 结果网格')

    # ---------------------------------------------------------------- 4.5
    w('')
    w('  4.5 底部按钮栏（y=80..95，x=0/0x2A/0x54/0x7E 四个框）')
    render(0)
    btns = []
    for (args, stack) in REC[BTN]:
        gs = [args[1], args[2], args[3]] + stack
        s = ''.join('' if (g == 0xFF or i >= 5) else g2c(g) for i, g in enumerate(gs))
        btns.append((args[0], s))
    for x, s in btns:
        w('    x=0x%02X 标签=%r' % (x, s))
    chk(len(btns) == 4 and [x for x, s in btns] == [0, 0x2A, 0x54, 0x7E],
        '底部 4 个按钮框 x=0/0x2A/0x54/0x7E')
    chk(btns[0][1] == 'CLEAR', '第一个按钮文字 = "CLEAR"（8x8 表 0x08014088）')
    chk(all(s == '' for x, s in btns[1:]), '其余 3 个按钮为空框')
    chk(litval(by[0x08006730])[1] == FONT8A, '按钮函数 0x08006704 用 8x8 表 A')

    # ---------------------------------------------------------------- 4.6
    w('')
    w('  4.6 关键反汇编摘录（逐条对应上面结论）')
    snippet(0x080049C0, 0x08004A20, 'page2 分支：只画 LIGHT 标签 + 12sec/OFF/ON 三档')
    snippet(0x08004A86, 0x08004A9E, 'LIGHT：箭头行 = 2*settings[8]+4；再 INVERT(settings[8])')
    snippet(0x08004AA0, 0x08004B74, 'page2：LCD 标签 + -3..+3 竖排刻度（表 0x08015A48）')
    snippet(0x080046B8, 0x08004744, 'SCAN 项：OFF/START + 箭头 + INVERT')
    snippet(0x08004852, 0x080048C0, 'CH 项：settings[5]+1 的两位十进制')
    snippet(0x080048B6, 0x08004960, 'ID 项：ID!=0 两位数；ID==0 -> "OFF"（0x08004942 分支）')
    chk(litval(by[0x080048C4])[1] == SETTINGS and has_insn(0x080048C6, 'ldrb', '#6'),
        'ID 项读 settings[6]（0x080048C4/C6）')
    chk(has_insn(0x080048CA, 'beq', '#0x8004942'),
        'ID==0 跳到 0x08004942（画 "OFF" 的分支）')
    chk(has_insn(0x08004944, 'movs', 'r2, #0x18') and
        has_insn(0x08004952, 'movs', 'r2, #0xf') and
        has_insn(0x0800495E, 'movs', 'r2, #0xf'),
        '0x08004942 分支画 0x18,0x0F,0x0F = "OFF"（0x08004944/952/95E）')

    # ---------------------------------------------------------------- 4.7
    w('')
    w('  4.7 反证：FEB ACL / TX 在本机型不存在')
    # 即使强行把索引设到 10/11，page2 分支也只画两行、无取值
    for idx in (10, 11):
        good, fb, e = render(idx)
        o, a = opt_rows()
        w('    强行 idx=%d（不可达）: 标签 %s 值行 %s' % (idx, labels(), o))
        chk(good and labels() == [(2, 'LIGHT'), (4, 'LCD')] and o == [],
            'idx=%d 只复用 page2 前两行（无 FEB ACL/TX 的标签/取值）' % idx)
    # 没有任何 16x16 选项位图（TX 在 V1C 用两个 16x16 图标做两档显示）
    render(11)
    chk(not REC[ICON_MOON] and not REC[ICON_BEEP],
        'idx=11 没有任何 16x16 选项位图（V1C 的 TX 项此处原本是两个 16x16 图标）')
    w('    说明：V1C 的 TX(idx11) 用 0x08017334/0x08017314 两个 16x16 位图做两档；')
    w('    本镜像的 16x16 绘制只剩月亮/喇叭/状态三种（见 4.3），没有 TX 图标。')

    # =====================================================================
    w('')
    w('=' * 78)
    w('5. 导航 / 改值行为（Unicorn 行为级验证）')
    w('=' * 78)
    w('')
    stubs = [SCAN_FN, LCD_APPLY, SYNC_FN]
    calls = {}

    class Emu2(Emu):
        def _hook(self, mu, address, size, ud):
            if address in stubs:
                calls.setdefault(address, []).append(mu.reg_read(UC_ARM_REG_R0))
            Emu._hook(self, mu, address, size, ud)

    nav = Emu2(stubs=stubs, rec=None)

    def nav_reset(idx, edit, settings=None, state=0):
        calls.clear()
        nav.reset()
        nav.poke(SCREEN, [1])
        nav.poke(EDIT, [edit])
        nav.poke(SEL, [idx])
        nav.poke(SCREEN_FLAG, [0])
        nav.poke(SPECIAL_545, [0])
        nav.poke(FMODE, [state])
        nav.poke(SETTINGS, settings if settings is not None else [0] * 16)

    def idx_now():
        return nav.peek(SEL, 1)[0]

    def val_now(i):
        return nav.peek(SETTINGS + i, 1)[0]

    # 5.1 项导航
    nav_reset(0, 0)
    seq = []
    for _ in range(14):
        nav.call(INC)
        seq.append(idx_now())
    w('  5.1 项导航（[0x200004D4]=1、[0x200004D5]=0）')
    w('    + 序列(从0): %s' % seq)
    chk(seq == [1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4],
        '『+』在项 0..9 间循环（0→1→…→9→0）⇒ 只有 10 项')
    nav_reset(0, 0)
    seq = []
    for _ in range(14):
        nav.call(DEC)
        seq.append(idx_now())
    w('    - 序列(从0): %s' % seq)
    chk(seq == [9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 9, 8, 7, 6],
        '『-』在项 0..9 间反向循环（0→9→…）⇒ 只有 10 项')
    chk(nav.peek(KEY_TIMEOUT, 2) == b'\xd0\x07',
        '每次按键把画面超时计数 [0x2000045E+0xC] 设为 0x07D0')
    chk(nav.peek(BACKLIGHT, 1) == b'\x28', '每次按键刷新背光 0x20000542=0x28')
    chk(nav.peek(SAVE_CNT, 2) == b'\xc8\x00',
        '每次按键把保存倒计时 0x20000516 设为 200（0xC8）')
    w('')
    w('  静态证据：INC 内 `cmp r2,#9` 后 >9 归零（0x0800C5A6/0x0800C5AA）；')
    w('  DEC 内 `cmp r1,#9`（0x08005A4E）与 sel==0 时 `movs r0,#9`（0x08005A60）。')
    chk(has_insn(0x0800C5A6, 'cmp', 'r2, #9') and has_insn(0x0800C5AA, 'strb'),
        'INC 项导航上限 9（0x0800C5A6）')
    chk(has_insn(0x08005A4E, 'cmp', 'r1, #9') and
        has_insn(0x08005A60, 'movs', 'r0, #9'), 'DEC 项导航上限 9（0x08005A4E/A60）')
    chk(bl_sites(INC) == [0x08005928] and bl_sites(DEC) == [0x0800592E],
        'INC/DEC 各只有 1 个调用点（0x08005928/0x0800592E，位于 +/- 扫描函数内）')

    # 5.2 改值：按最大值表回绕，LCD 上限钳位
    w('')
    w('  5.2 改值（[0x200004D5]=1）：settings[idx] 按最大值表回绕')
    maxes = [data[MAXTAB - BASE + i] for i in range(12)]
    w('    最大值表 0x%08X = %s' % (MAXTAB, ' '.join('%02X' % b for b in maxes)))
    chk(bytes(maxes) == MAX_BYTES, '最大值表 12 字节与 V1C 完全一致')
    chk(maxes[5] == 0x1F and maxes[6] == 0x63 and maxes[8] == 0x02 and maxes[9] == 0x06,
        'CH max31 / ID max99 / LIGHT max2 / LCD max6 都在表内')
    for idx in range(10):
        mx = maxes[idx]
        nav_reset(idx, 1, [0] * idx + [mx] + [0] * (15 - idx))
        nav.call(INC)
        v_plus = val_now(idx)
        nav_reset(idx, 1, [0] * 16)
        nav.call(DEC)
        v_minus = val_now(idx)
        exp_plus = mx if idx == 9 else 0
        exp_minus = 0 if idx == 9 else mx
        w('    idx=%d max=%d：max 处 + -> %d（期望 %d）；0 处 - -> %d（期望 %d）'
          % (idx, mx, v_plus, exp_plus, v_minus, exp_minus))
        chk(v_plus == exp_plus and v_minus == exp_minus,
            'idx=%d 改值回绕正确（LCD 钳位）' % idx)
    chk(val_now(10) == 0 and val_now(11) == 0, '操作后 settings[10]/[11] 恒为 0（UI 不可达）')

    # 5.3 改值副作用：ID -> 同步字；LCD -> 应用亮度
    w('')
    w('  5.3 改值副作用')
    nav_reset(6, 1, [0] * 6 + [36] + [0] * 9)
    nav.call(INC)
    w('    ID 36->37：settings[6]=%d，同步字函数 0x080115B8 入参=%s'
      % (val_now(6), calls.get(SYNC_FN)))
    chk(val_now(6) == 37 and calls.get(SYNC_FN) == [37],
        'ID 改值调用同步字函数 0x080115B8(新 ID=37)')
    nav_reset(8, 0)
    nav.call(INC)                     # 8 -> 9（LCD）
    w('    导航 8->9：LCD 应用函数 0x080128F0 调用次数=%s'
      % len(calls.get(LCD_APPLY, [])))
    chk(calls.get(LCD_APPLY) is not None, '导航到 LCD(idx9) 调用 0x080128F0（应用亮度）')
    nav_reset(9, 1, [0] * 9 + [0] + [0] * 6)
    nav.call(INC)
    w('    LCD 0->1：0x080128F0 调用次数=%s，settings[9]=%d'
      % (len(calls.get(LCD_APPLY, [])), val_now(9)))
    chk(val_now(9) == 1 and calls.get(LCD_APPLY) is not None,
        'LCD 改值后也调用 0x080128F0（即时生效）')
    chk(litval(by[0x080128F2])[1] == SETTINGS and has_insn(0x080128F6, 'ldrb', '#9'),
        '0x080128F0 内部直接读 settings[9]（字面量 0x20001058 + ldrb #9）')
    setter = Emu(stubs=stubs, rec=None)
    setter.reset()
    setter.poke(SCREEN, [1])
    setter.poke(EDIT, [0])
    setter.poke(SEL, [2])
    setter.call(SET_KEY)
    a = setter.peek(EDIT, 1)[0]
    setter.call(SET_KEY)
    b = setter.peek(EDIT, 1)[0]
    w('    SET 键（0x08011454）编辑态：0 -> %d -> %d' % (a, b))
    chk(a == 1 and b == 0, 'SET 键在 C.Fn 内切换编辑态（0→1→0）')
    setter.reset()
    setter.poke(SCREEN, [1])
    setter.poke(EDIT, [1])
    setter.poke(SEL, [4])
    setter.poke(SETTINGS, [0, 0, 0, 0, 1] + [0] * 11)
    setter.call(SET_KEY)
    w('    SET 在 SCAN(settings[4]=1)：edit=%d' % setter.peek(EDIT, 1)[0])
    chk(setter.peek(EDIT, 1)[0] == 1, 'SET 在 SCAN+settings[4]==1 时触发扫描（不切编辑态）')
    chk(has_insn(0x08011484, 'cmp', 'r3, #4') and
        has_insn(0x0801148C, 'cmp', 'r3, #1'),
        'SET 键代码：idx==4 且 settings[4]==1 时走扫描分支（0x08011484/8C）')

    # =====================================================================
    w('')
    w('=' * 78)
    w('6. 进入/退出 C.Fn 的按键路径')
    w('=' * 78)
    w('')
    w('  主循环 0x08013D92..0x08013DBE 按顺序调用任务（0x08013DA6=显示任务、')
    w('  0x08013DAA=按键扫描/分发、0x08013DAE=+/- 扫描）。')
    snippet(0x0800CF70, 0x0800CFC0, '键码分派 0x0800CF72：键码 1/4/8/0x20/0x40/0x80/0xC/0x10')
    chk(litval(by[0x0800CD88])[1] == KEY_UI,
        '按键任务 0x0800CD7C 使用 UI 结构 0x2000045E（键码在 +0xA = 0x20000468）')
    chk(has_insn(0x0800CF72, 'ldrh', 'r0, [r5, #0xa]'), '0x0800CF72 读键码 [0x2000045E+0xA]')
    chk(has_insn(0x0800CF7E, 'cmp', 'r0, #4') and has_insn(0x0800CF82, 'cmp', 'r0, #8'),
        '键码 4/8 分支（8 = C.Fn 开关键）')
    chk(has_insn(0x0800CF7A, 'cmp', 'r0, #1'), '键码 1 = 返回键分支')
    chk(blt(by[0x0800CFBA]) == CFN_TOGGLE, '键码 8 -> 0x08004D04（C.Fn 开/关）')
    chk(blt(by[0x0800CFAE]) == CFN_BACK, '键码 1 -> 0x0800CCB0（返回/关闭）')
    chk(blt(by[0x0800D00C]) == SET_KEY, 'SET 键 -> 0x08011454')
    w('')
    snippet(CFN_TOGGLE, 0x08004D36, '0x08004D04：切换 [0x200004D4]（C.Fn 开/关）')
    chk(bl_sites(CFN_TOGGLE) == [0x0800CFBA], 'C.Fn 开关只有 1 个调用点（键码 8）')
    chk(has_insn(0x08004D0C, 'ldr') and litval(by[0x08004D0C])[1] == SCREEN and
        has_insn(0x08004D18, 'ldr') and litval(by[0x08004D18])[1] == EDIT,
        '开关函数访问 [SCREEN]/[EDIT]')
    snippet(CFN_BACK, 0x0800CD20, '0x0800CCB0：返回键关闭 C.Fn（清 SCREEN/EDIT）')
    chk(bl_sites(CFN_BACK) == [0x0800CFAE], '返回键函数只有 1 个调用点（键码 1）')
    snippet(PM_SCAN, 0x08005936, '+/- 扫描 0x080058E4：GPIOB 0x48000800 bit7/8')
    chk(litval(by[0x080058E6])[1] == 0x48000800, '+/- 扫描读 GPIOB 0x48000800')
    chk(has_insn(0x080058EA, 'movs', 'r1, #0x80') and
        has_insn(0x080058F8, 'movs', 'r1, #0xff'),
        '+/- 扫描检测 bit7(0x80) 与 bit8(0x100)')
    chk(has_insn(0x0800591E, 'cmp', 'r0, #0xb4') and
        has_insn(0x08005922, 'cmp', 'r0, #0x78'),
        '+/- 历史码：0xB4=+ / 0x78=-')
    # 行为：开关与返回
    tg = Emu(stubs=stubs, rec=None)
    tg.reset()
    tg.poke(SCREEN, [0])
    tg.poke(FMODE, [0])
    tg.call(CFN_TOGGLE)
    on = tg.peek(SCREEN, 1)[0]
    tg.poke(EDIT, [1])
    tg.poke(0x200004C3, [1])
    tg.poke(SETTINGS + 4, [1])
    tg.call(CFN_TOGGLE)
    off = tg.peek(SCREEN, 1)[0]
    w('')
    w('    C.Fn 开关 0x08004D04：0 -> %d -> %d；退出后 edit=%d、settings[4]=%d'
      % (on, off, tg.peek(EDIT, 1)[0], tg.peek(SETTINGS + 4, 1)[0]))
    chk(on == 1 and off == 0, 'C.Fn 开关：同一个键 0→1→0（进/退）')
    chk(tg.peek(EDIT, 1)[0] == 0 and tg.peek(SETTINGS + 4, 1)[0] == 0,
        '退出 C.Fn 时清编辑态与 settings[4]')
    bk = Emu(stubs=stubs, rec=None)
    bk.reset()
    bk.poke(SCREEN, [1])
    bk.poke(EDIT, [1])
    bk.call(CFN_BACK)
    w('    返回键 0x0800CCB0：screen=%d edit=%d'
      % (bk.peek(SCREEN, 1)[0], bk.peek(EDIT, 1)[0]))
    chk(bk.peek(SCREEN, 1)[0] == 0 and bk.peek(EDIT, 1)[0] == 0,
        '返回键关闭 C.Fn 并清编辑态')

    # =====================================================================
    w('')
    w('=' * 78)
    w('7. 默认值 / 改值自动保存链路')
    w('=' * 78)
    w('')
    w('  7.1 出厂默认值函数 0x%08X（写 settings[0..9]，不动 [10]/[11]）' % DEFAULTS_FN)
    df = Emu(stubs=stubs, rec=None)
    df.reset()
    df.poke(SETTINGS, [0xFF] * 16)
    okdf = df.call(DEFAULTS_FN)
    defaults = list(df.peek(SETTINGS, 16))
    w('    执行后 settings[0..15] = %s' % ' '.join('%02X' % b for b in defaults))
    chk(okdf, '默认值函数 0x08005E54 可执行')
    exp_defaults = [0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x01, 0x00, 0x03]
    chk(defaults[:10] == exp_defaults,
        '默认值 = [m/ft=0, AF=1(OFF), STBY=0(ON), RX STBY=0, SCAN=0, CH=0x14(21), '
        'ID=0(OFF), BEEP=1(ON), LIGHT=0(12sec), LCD=3]')
    chk(defaults[10] == 0xFF and defaults[11] == 0xFF,
        '默认值函数不写 settings[10]/[11]（UI 不存在的两项）')
    chk(defaults[8] == 0, 'LIGHT 出厂默认 = 0（12 秒自动熄灭）')
    w('')
    w('  7.2 每次按键把保存倒计时设为 200：0x%08X' % SAVE_SET)
    chk(insns_between(SAVE_SET, SAVE_SET + 12)[0].mnemonic == 'ldr', '0x080109C8 是函数入口')
    sc = Emu(stubs=stubs, rec=None)
    sc.reset()
    sc.call(SAVE_SET)
    w('    执行后 [0x20000516] = %d' % int.from_bytes(sc.peek(SAVE_CNT, 2), 'little'))
    chk(int.from_bytes(sc.peek(SAVE_CNT, 2), 'little') == 200,
        '0x080109C8 把 [0x20000516] 置 200')
    w('')
    w('  7.3 倒计时 tick：0x%08X（到 0 置保存标志）' % SAVE_TICK)
    tk = Emu(stubs=stubs, rec=None)
    tk.reset()
    tk.poke(SAVE_CNT, [2, 0])
    tk.call(SAVE_TICK)
    v1 = int.from_bytes(tk.peek(SAVE_CNT, 2), 'little')
    tk.call(SAVE_TICK)
    v0 = int.from_bytes(tk.peek(SAVE_CNT, 2), 'little')
    flag = tk.peek(SAVE_FLAG, 1)[0]
    w('    倒计时 2 -> %d -> %d；保存标志 0x20000518 = %d' % (v1, v0, flag))
    chk(v1 == 1 and v0 == 0 and flag == 1,
        'tick 递减到 0 时置保存请求标志 0x20000518=1')
    w('')
    w('  7.4 保存任务 0x%08X -> 记录写 flash 0x%08X' % (SAVE_TASK, FLASH_REC))
    snippet(SAVE_TASK, 0x0800DA0C, '保存任务：tick -> 标志 -> 组记录 0x08010D94')
    chk(blt(by[0x0800D9CA]) == SAVE_TICK, '保存任务先调 tick 0x080109A4')
    chk(litval(by[0x0800D9CE])[1] == SAVE_FLAG, '保存任务读标志 0x20000518')
    chk(blt(by[0x0800DA06]) == REC_WRITE, '保存任务调记录写 0x08010D94')
    snippet(REC_WRITE, 0x08010DC2, '记录写 0x08010D94：复制 18 个字 -> flash 0x0801F000')
    chk(litval(by[0x08010DB8])[1] == FLASH_REC, '记录写用 flash 记录区 0x0801F000')
    chk(has_insn(0x08010DB0, 'cmp', 'r1, #0x12'),
        '记录写复制 0x12 个字（72 字节设置记录）')
    snippet(SAVE_REC, 0x0800EED2, '组/写记录 0x0800EEB0：20 槽 x 0x64 字节')
    chk(litval(by[0x0800EECA])[1] == FLASH_REC, '记录区扫描 0x0800EEB0 用 0x0801F000')
    chk(litval(by[0x0800EEB6])[1] == REC_NUM, '记录槽号 0x20000428')
    chk(has_insn(0x0800EEB8, 'movs', 'r0, #0x14'), '记录槽数 20（0x14）')
    chk(FLASH_REC > BASE + len(data),
        'flash 记录区 0x0801F000 在 app 镜像末端 0x%08X 之外' % (BASE + len(data)))
    w('')
    w('  7.5 最大值表的三处引用 + 12 字节全局钳位（含 UI 不存在的最后 2 项）')
    refs = lit_sites(MAXTAB)
    w('    引用点: %s' % ['0x%08X' % a for a in refs])
    chk(refs == [0x08005A6A, 0x0800C586, 0x0800F086],
        '最大值表只有 3 处引用（- handler / + handler / 全局钳位）')
    chk(has_insn(0x0800F09A, 'cmp', 'r0, #0xc'),
        '全局钳位循环跑 settings[0..11] 共 12 字节（0x0800F09A cmp #0xC）')
    chk(has_insn(0x0800F094, 'strb', 'r4'), '超限项被清零（r4=0）')
    w('    ⇒ settings[10]/[11] 是“平台共用 12 字节表”的残留：有 RAM 字节、会被')
    w('      全局钳位，但本机型 C.Fn UI 不展示、不可导航、也没有默认值。')

    # =====================================================================
    w('')
    w('=' * 78)
    w('8. 画面重建（真实执行渲染函数后的帧缓冲，168x96）')
    w('=' * 78)
    w('')
    w('  说明：# = 点亮像素。168 列 x 96 行 = 12 条带 x 8 行。')
    w('  布局：y0..15 顶栏("C.Fn" + “Ver1.1”)；y16..79 左侧 4 行列表(x0..64)；')
    w('        右侧 x≈80..127 为当前项的值/选项栏（0x50 箭头，0x5A 起文字）；')
    w('        y80..95 底部 4 个按钮框（第一个为 CLEAR）。')

    def fb_art(fb):
        out = []
        for y in range(FB_STRIPS * 8):
            strip, bit = divmod(y, 8)
            row = fb[strip * FB_W:(strip + 1) * FB_W]
            out.append(''.join('#' if (row[x] >> (7 - bit)) & 1 else '.'
                               for x in range(FB_W)))
        return out

    shots = [
        ('idx=0  page0 选中 m/ft（未编辑）', 0, 0, [0] * 16, None),
        ('idx=3  page0 选中 RX STBY，settings[3]=1(30min)', 3, 0,
         [0, 0, 0, 1] + [0] * 12, None),
        ('idx=2  page0 选中 STBY，settings[2]=1(OFF) + 月亮图标', 2, 0,
         [0, 0, 1] + [0] * 13, None),
        ('idx=5  page1 选中 CH，settings[5]=31 -> 显示 32', 5, 0,
         [0] * 5 + [31] + [0] * 10, None),
        ('idx=6  page1 选中 ID，ID=0 -> "OFF"', 6, 0, [0] * 16, None),
        ('idx=6 编辑态（ID=37，选中值行反显）', 6, 1, [0] * 6 + [37] + [0] * 9, None),
        ('idx=8  page2 选中 LIGHT，value=2 -> 箭头指 ON', 8, 0,
         [0] * 8 + [2] + [0] * 7, None),
        ('idx=9  page2 选中 LCD，settings[9]=0 -> 当前 -3', 9, 0,
         [0] * 9 + [0] + [0] * 6, None),
    ]
    for title, idx, edit, settings, ram in shots:
        good, fb, e = render(idx, edit, settings, ram)
        w('')
        w('  ┌─ %s   (%d 个非零字节)' % (title, sum(1 for b in fb if b)))
        art = fb_art(fb)
        for y, line in enumerate(art):
            w('  │' + line + '│  y=%d' % y)
        w('  └' + '─' * FB_W)
        chk(good, '画面重建 %s' % title)
        # 结构断言：顶栏 & 选中行反显
        if idx == 0 and edit == 0:
            sel_region = ''.join(art[y][2:66] for y in range(16, 32))
            n_set = sel_region.count('#')
            w('    选中行反显区域 y16..31, x2..65：点亮 %d / 无 %d（反显文字为“洞”）'
              % (n_set, len(sel_region) - n_set))
            chk(n_set >= 200 and (len(sel_region) - n_set) >= 20,
                '选中行整行反显（区域内同时存在点亮与“洞”）')
        if idx == 8:
            opts, arrow = opt_rows()
            w('    LIGHT 绘制记录：值行 %s 箭头 %s' % (opts, arrow))
            chk(arrow == [8], 'LIGHT value=2 箭头画在 row8')
    # 数值页结构断言：ID=0 的 OFF 与 CH 的两位数字出现在右侧值栏
    good, fb, e = render(6, 0, [0] * 16)
    chk(opt_rows() == ([(4, 'OFF')], [4]), 'ID=0 帧里右栏 row4 画 "OFF"')
    good, fb, e = render(5, 0, [0] * 5 + [31] + [0] * 10)
    chk(opt_rows() == ([(4, '32')], [4]), 'CH=31 帧里右栏 row4 画 "32"')

    # =====================================================================
    w('')
    w('=' * 78)
    w('9. 结论')
    w('=' * 78)
    w('')
    tbl = [
        (0, 'm/ft', 'm | ft', '0=m, 1=ft', 1),
        (1, 'AF', 'ON | OFF', '0=ON, 1=OFF', 1),
        (2, 'STBY', '月亮图标 + ON | OFF', '0=ON, 1=OFF', 1),
        (3, 'RX STBY', '60min | 30min', '0=60min, 1=30min', 1),
        (4, 'SCAN', 'OFF | START（state 1/2 进度数字，state 3 2x4 结果网格）',
         '动作项，SET 在 settings[4]==1 时启动扫描', 1),
        (5, 'CH', '两位十进制 = settings[5]+1', '0..31 -> 显示 01..32', 0x1F),
        (6, 'ID', 'OFF (ID=0) | 两位十进制 01..99', '0=OFF，1..99 直接显示', 0x63),
        (7, 'BEEP', '喇叭图标 + OFF | ON', '0=OFF, 1=ON', 1),
        (8, 'LIGHT', '12sec | OFF | ON', '0=12sec, 1=OFF, 2=ON；出厂 0', 2),
        (9, 'LCD', '竖排 -3..+3 刻度（表 0x08015A48，箭头固定 row6 指当前档）',
         '0..6 -> -3..+3，上限钳位', 6),
    ]
    w('  一、C.Fn 列表页 = 渲染函数 0x0800431C（画面分派 0x08012124，')
    w('      [0x200004D4]==1 时在 0x0801219E 调用）：page = idx>>2、slot = idx&3。')
    w('      每页 4 行“名称”（左栏，文字约居中于 x≈30），选中行整行反显（内联'); w('      0x08004B76..0x08004BFC）；右栏 x≈0x50 是箭头、0x5A 起是文字；')
    w('      编辑态再用 0x08004D50 反显选中值行。无平滑滚动，越界即换页。')
    w('      没有 F1..F12 编号，只显示名称。')
    w('')
    w('  二、10 项（index -> 名称 / 右栏 / 取值 / max / settings 字节）：')
    for (i, nm, opt, rule, mx) in tbl:
        w('      %2d  %-8s %-46s %-38s max=%-3d settings=0x%08X'
          % (i, nm, opt, rule, mx, SETTINGS + i))
    w('')
    w('  三、按键路径：')
    w('      MENU 类键（键码 8）-> 0x08004D04 切换 C.Fn 画面（同一个键再按退出）；')
    w('      返回键（键码 1）-> 0x0800CCB0 关闭；')
    w('      +/- 键（GPIOB 0x48000800 bit7/8，0x080058E4）-> 未编辑时移动项（0..9 循环），')
    w('      编辑态时改值（按 0x08016102 最大值表回绕，item9 LCD 钳位）；')
    w('      SET 键（0x08011454）-> 切换编辑态 0x200004D5（SCAN 特例：settings[4]==1 时启动扫描）。')
    w('      改值后保存倒计时 [0x20000516]=200（0x080109C8），tick 0x080109A4；')
    w('      到 0 置 0x20000518=1 -> 任务 0x0800D9C8 -> 记录 0x08010D94 -> flash 0x0801F000。')
    w('')
    w('  四、与 V1C（12 项）的关键差异：')
    w('      · 本机型 C.Fn 只有 10 项（0..9），FEB ACL / TX 不存在；')
    w('        最大值表仍保留 12 字节（平台共用），[10]/[11] 只被全局钳位循环使用。')
    w('      · 顶栏版本号 "Ver1.1"（V1C 为 "Ver2.0"）；渲染函数 0x0800431C（V1C 0x08004914）。')
    w('      · UI 变量整体前移：SCREEN 0x200004D4 / EDIT 0x200004D5 / SEL 0x200004D6')
    w('        （V1C 0x200004F0/F1/F2）；settings 基址 0x20001058（V1C 0x20001086）。')
    w('      · 帧缓冲 0x200007AC（V1C 0x200007C4）；8x16 表 0x080143D6（V1C 0x08016208）。')
    w('      · 最大值表 0x08016102（V1C 0x08018362），字节完全一致。')
    w('      · INC/DEC/SET/开关/返回/保存链路结构同 V1C，地址不同（见下表）。')
    w('')
    w('  五、风险 / 未确认：')
    w('      · 物理按键丝印无法从固件恢复；“MENU/SET/+/-”是按功能与同代 UI 推断。')
    w('      · SCAN 的 state=1/2 进度与 state=3 网格只给出真实绘制证据，未逐像素核对。')
    w('      · 画面重建时 RAM 初值按 0 设置，顶栏状态图标与实机可能不同；')
    w('        C.Fn 列表/取值本身由代码 + settings 决定，可信度高。')
    w('')
    w('  全部检查: %s' % ('PASS' if ok else 'FAIL'))

    text = '\n'.join(lines) + '\n'
    with open(OUT, 'w', encoding='utf-8') as fh:
        fh.write(text)
    print(text)
    print('written: %s (%d lines)' % (OUT, len(lines)))
    return 0 if ok else 1


if __name__ == '__main__':
    try:
        sys.stdout.reconfigure(encoding='utf-8')
    except Exception:
        pass
    sys.exit(main())
