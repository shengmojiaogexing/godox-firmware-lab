# -*- coding: utf-8 -*-
"""Godox V1C (V1C_V2.0.bin) C.Fn 菜单 / UI 逆向 —— 可重复运行脚本。

    tools\\python\\python.exe devices\\v1c\\re\\cfn_menu.py

产物：
  * 本脚本把全部证据与结论打印到 stdout，并写入
    devices/v1c/re/log_cfn_menu.txt（真实输出，重复运行覆盖）。
  * 退出码 0 = 全部检查 PASS。

覆盖内容：
  1. 固件身份 / 镜像基址
  2. 字体/图元表（0x08016208，17B/字形）与字形索引→字符映射
  3. 绘制原语（0x080070F4 / 0x0800703C / 0x08005440 / 0x08005260 / 0x08006E38）
  4. 帧缓冲几何（0x200007C4，12 strip × 0xA8 = 2016B → 168×96）+ LCD 刷新链路
  5. C.Fn 画面渲染函数 0x08004914、调用者 0x0801472C、页/槽位拆分
  6. 12 个 C.Fn 项：标签、可选项、取值规则、settings 字节、最大值
     （用 Unicorn 真实执行渲染函数并记录每一条绘制调用 → 还原文字）
  7. 导航/改值行为（inc 0x0800D3D8 / dec 0x080061CC）的 Unicorn 行为级验证
  8. SET 键（0x08012058）、C.Fn 开关（0x080053F4）、关闭（0x0800DC14）验证
  9. 按键矩阵 / +/- 键 / 保存持久化链路（静态证据）
 10. 帧缓冲 ASCII 重建（列表页 + 数值页草图）
"""
from __future__ import annotations

import hashlib
import os
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
from unicorn import Uc, UcError, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS, UC_HOOK_CODE
from unicorn.arm_const import (
    UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3,
    UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC,
)

HERE = os.path.dirname(os.path.abspath(__file__))
FW = os.path.join(HERE, '..', 'firmware', 'V1C_V2.0.bin')
OUT = os.path.join(HERE, 'log_cfn_menu.txt')
BASE = 0x08003000

# ---- 关键地址常量（全部由本脚本反汇编自证）
FB = 0x200007C4            # 帧缓冲
FB_W = 0xA8                # 每个 8 行条带 168 字节（=168 列）
FB_STRIPS = 12             # 12 条带 → 96 行
FB_BYTES = FB_W * FB_STRIPS
RENDER = 0x08004914        # C.Fn 画面渲染函数
INC = 0x0800D3D8           # C.Fn 值/项 +
DEC = 0x080061CC           # C.Fn 值/项 -
SEL_IDX = 0x200004F2       # 当前选中项索引 0..0xB
EDIT = 0x200004F1          # 编辑态（0=选项导航，1=改值）
SCREEN = 0x200004F0        # C.Fn 画面是否打开（1=打开）
SETTINGS = 0x20001086      # 设置字节数组基址（item n = +n）
MAXTAB = 0x08018362        # 各项最大值/回绕表（12 字节）
FONT16 = 0x08016208        # 8x16 字体/图元表：17 B/字形
FONT8A = 0x08015EBA        # 8x8 字体（按钮文字）
FONT8B = 0x080161AE        # 8x8 字体（第二套）
DRAW16 = 0x080070F4        # 8x16 字形绘制（row, col, glyph, shift）
DRAW8A = 0x0800703C        # 8x8 字形绘制（row, col, glyph）
DRAW8B = 0x08007070        # 8x8 第二套
ICON_TX_A = 0x08007764     # 16x16 图标（表格 0x08017314）
ICON_TX_B = 0x08007728     # 16x16 图标（表格 0x08017334）
INVERT = 0x08005440        # 编辑态：把选中值行反显（XOR）
SELBAR = 0x08005260        # 列表：把选中项行反显（XOR）
BTN = 0x08006E38           # 底部按钮框 + 8x8 文字
LCD_FLUSH = 0x08014E88     # 刷新 3 个条带到 LCD
DISP_TASK = 0x0800680C     # 显示任务：4 页轮流刷新 + 每 5 帧重绘
DRAW_DISPATCH = 0x0801472C # 画面分派（[SCREEN]==1 → RENDER）
CFN_TOGGLE = 0x080053F4    # C.Fn 画面开/关切换
CFN_CLOSE = 0x0800DC14     # 返回键：关闭 C.Fn
SET_KEY = 0x08012058       # SET 键：切换编辑态（SCAN 特例）
PLUSMINUS_SCAN = 0x080060FC  # +/- 键扫描（GPIOB bit7/8）
KEYSCAN = 0x0800DCDC       # 按键矩阵扫描任务
SAVE_BUILD = 0x08005EF0    # 组设置记录
SAVE_TASK = 0x0800E9A0     # 保存任务（倒计时到 → 写 flash）
SAVE_CNT_SET = 0x080114CC  # 改值后把保存倒计时设为 200
SAVE_CNT_TICK = 0x080114A8 # 倒计时递减，到 0 置保存标志

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


def decode_text(items, widths):
    """items = [(col, glyph)]（已按 col 升序），返回字符串（大间隔补空格）。"""
    out = []
    prev_end = None
    for col, g in items:
        if prev_end is not None and col - prev_end >= 4:
            out.append(' ')
        out.append(g2c(g))
        prev_end = col + widths.get(g, 0)
    return ''.join(out)


def main():
    ok = True

    def chk(cond, msg, extra=''):
        nonlocal ok
        ok = ok and bool(cond)
        w('  [%s] %s%s' % ('PASS' if cond else 'FAIL', msg, ('  ' + extra) if extra else ''))

    data = open(FW, 'rb').read()

    # =====================================================================
    w('=' * 78)
    w('Godox V1C C.Fn 菜单 / UI 逆向  —  cfn_menu.py 输出')
    w('=' * 78)
    w('')
    w('固件: devices/v1c/firmware/V1C_V2.0.bin')
    w('  长度   : %d 字节' % len(data))
    w('  sha256 : %s' % hashlib.sha256(data).hexdigest())
    w('  镜像基址: 0x%08X（vector[1] Reset=0x%08X）' % (
        BASE, int.from_bytes(data[4:8], 'little')))
    chk(len(data) == 88032 and int.from_bytes(data[4:8], 'little') == 0x080030D5,
        '固件身份（88032 B，Reset=0x080030D5，基址 0x08003000）')

    # =====================================================================
    # Capstone（skipdata 必须开：0x08006292..AD 是 switch 跳转表）
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
    md.skipdata = True
    insns = {}
    order = []
    for ins in md.disasm(data, BASE):
        insns[ins.address] = ins
        order.append(ins)

    def snippet(lo, hi, label=''):
        w('')
        w('  -- %s  [0x%08X..0x%08X]' % (label, lo, hi))
        a = lo
        while a <= hi:
            ins = insns.get(a)
            if ins is None:
                a += 2
                continue
            w('    0x%08X  %-14s %-7s %s' % (
                ins.address, ins.bytes.hex(' '), ins.mnemonic, ins.op_str))
            a += ins.size

    # =====================================================================
    w('')
    w('=' * 78)
    w('1. 字体 / 图元表（C.Fn 文字就是这些 8x16 字形）')
    w('=' * 78)
    w('')
    w('  表基址 0x%08X，17 字节/字形：宽度(1B) + 16B 列数据（每列 2B = 16 像素高）'
      % FONT16)
    w('  字形索引 → 字符：0-9=数字，10-35=A-Z，36-61=a-z，62+ 符号/图元。')
    glyph_w = {}
    for gi in range(100):
        glyph_w[gi] = data[FONT16 - BASE + 17 * gi]

    def glyph_art(gi, font=FONT16, per=17):
        off = font - BASE + per * gi
        n = data[off]
        rows = [''] * 16
        for x in range(n):
            top = data[off + 1 + 2 * x]
            bot = data[off + 2 + 2 * x]
            for y in range(8):
                rows[y] += '#' if (top >> (7 - y)) & 1 else '.'
                rows[8 + y] += '#' if (bot >> (7 - y)) & 1 else '.'
        return rows

    # 用已知画面字符串自证映射：标题 "C.Fn"、版本 "Ver2.0"
    title = [(0x19, 0x0C), (0x20, 0x40), (0x23, 0x0F), (0x2A, 0x31)]
    ver = [(0x80, 0x1F), (0x87, 0x28), (0x8E, 0x35), (0x97, 0x02),
           (0x9E, 0x40), (0xA1, 0x00)]
    t_title = decode_text(title, glyph_w)
    t_ver = decode_text(ver, glyph_w)
    w('  自证 1：顶栏标题字形 0x0C,0x40,0x0F,0x31 解码 = %r' % t_title)
    w('  自证 2：顶栏版本字形 0x1F,0x28,0x35,0x02,0x40,0x00 解码 = %r' % t_ver)
    chk(t_title == 'C.Fn' and t_ver == 'Ver2.0',
        '字形索引→字符映射自证（"C.Fn" / "Ver2.0"）', '%s / %s' % (t_title, t_ver))
    w('')
    w('  字形样例（0x0C=C / 0x0F=F / 0x31=n / 0x40=.）：')
    for gi, nm in ((0x0C, 'C'), (0x0F, 'F'), (0x31, 'n'), (0x40, '.')):
        w('    glyph 0x%02X = %r width=%d' % (gi, nm, glyph_w[gi]))
        for r in glyph_art(gi):
            w('      ' + r)
    w('')
    w('  8x8 字体（底部按钮文字，表 0x%08X）：' % FONT8A)
    def glyph8_art(gi):
        off = FONT8A - BASE + 9 * gi
        n = data[off]
        rows = [''] * 8
        for x in range(n):
            b = data[off + 1 + x]
            for y in range(8):
                rows[y] += '#' if (b >> (7 - y)) & 1 else '.'
        return rows
    for gi in (0x0C, 0x0A, 0x15, 0x0E, 0x1B):
        w('    idx 0x%02X=%r  ' % (gi, g2c(gi)) + ' '.join(glyph8_art(gi)))

    # =====================================================================
    w('')
    w('=' * 78)
    w('2. 绘制原语与帧缓冲几何')
    w('=' * 78)
    snippet(DRAW16, 0x0800714E, '8x16 字形绘制：row*0xA8 + col，第二条带 +0xA8 → 16 行')
    w('')
    w('  语义：0x080070F4(r0=条带行, r1=列, r2=字形索引, r3=移位)')
    w('        OR 写 fb + r0*0xA8 + col 与 fb + r0*0xA8 + 0xA8 + col。')
    snippet(DRAW8A, 0x08007066, '8x8 字形绘制：r0=行, r1=列, r2=索引（9B/字形）')
    snippet(INVERT, 0x08005482, '编辑态反显：选中值行 50x16 区域 XOR')
    snippet(SELBAR, 0x08005292, '列表选中行反显：slot 行 XOR 0x7F/0xFF')
    snippet(LCD_FLUSH, 0x08014EF0, 'LCD 刷新：每次 3 个条带(3*0xA8)，调用者传 page=0..3')
    w('')
    # 找出 flush 的调用者
    flow = {}
    for ins in order:
        if ins.mnemonic in ('bl', 'b', 'b.w') and ins.op_str.startswith('#'):
            try:
                flow.setdefault(int(ins.op_str[1:], 16), []).append((ins.address, ins.mnemonic))
            except ValueError:
                pass
    flush_callers = flow.get(LCD_FLUSH, [])
    w('  LCD 刷新调用点: %s' % ', '.join('0x%08X(%s)' % c for c in flush_callers))
    chk(len(flush_callers) == 5 and all(c[1] == 'bl' for c in flush_callers),
        'LCD 刷新 5 个调用点（1 个在初始化、4 个在显示任务 = 4×3 条带 = 12 条带）')
    w('')
    w('  → 帧缓冲 0x%08X = %d 条带 × 0xA8 字节 = %d 字节 = %d×%d 单色像素。'
      % (FB, FB_STRIPS, FB_BYTES, FB_W, FB_STRIPS * 8))
    chk(FB + FB_BYTES == 0x20000FA4, '帧缓冲 0x200007C4..0x20000FA3（168×96）不与设置区重叠')

    # =====================================================================
    w('')
    w('=' * 78)
    w('3. C.Fn 画面渲染函数与页/槽位拆分')
    w('=' * 78)
    snippet(0x08004914, 0x0800492A, '渲染函数入口（0x08004914，无参数）')
    snippet(0x080049B2, 0x080049CE, '页拆分：page = idx>>2, slot = idx&3；page 0/1/2 分派')
    w('')
    w('  → 12 项 = 3 页 × 4 行；page=0 → 项 0..3，page=1 → 4..7，page=2 → 8..0xB。')
    w('  渲染函数调用者:')
    for c in flow.get(RENDER, []):
        w('    0x%08X  %s #0x8004914' % c)
    snippet(0x080147D0, 0x0801480A, '画面分派：[SCREEN]==1 → 画 C.Fn；[SCREEN] 清 0 后回主界面')
    snippet(DISP_TASK, 0x08006832, '显示任务：4 个条带页轮流 flush，每 5 帧重绘一次')
    chk([c[0] for c in flow.get(RENDER, [])] == [0x08014808],
        'C.Fn 渲染只有 1 个调用点（0x08014808），由 [0x200004F0]==1 触发')

    # =====================================================================
    w('')
    w('=' * 78)
    w('4. 用 Unicorn 真实执行渲染函数，记录每一步绘制 → 还原 12 项')
    w('=' * 78)
    w('')
    w('  harness：映射 0x08000000..0x0801FFFF（镜像@0x08003000）与 0x20000000..0x20003FFF；')
    w('  置 [0x200004F2]=idx、[0x200004F1]=edit、settings[0..15]；代码 hook 记录所有')
    w('  绘制 helper 的入参；函数返回到未映射陷阱 0x08002001 时停止。')

    stubs_render = ()   # 渲染路径全是纯绘制/除法，无需桩

    class Emu:
        def __init__(self, stubs=(), rec=None):
            self.mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
            self.mu.mem_map(0x08000000, 0x20000)
            img = bytearray(data)
            for s in stubs:
                off = s - BASE
                img[off:off + 2] = b'\x70\x47'   # bx lr
            self.mu.mem_write(BASE, bytes(img))
            self.mu.mem_map(0x20000000, 0x4000)
            self.rec = rec
            self.calls = {}
            self.mu.hook_add(UC_HOOK_CODE, self._hook)

        def _hook(self, mu, address, size, ud):
            if 0x08002000 <= address < 0x08003000:
                mu.emu_stop()
                return
            if self.rec is None:
                return
            if address in self.rec:
                args = [mu.reg_read(r) for r in
                        (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3)]
                sp = mu.reg_read(UC_ARM_REG_SP)
                stack = []
                if address == BTN:
                    # 底部按钮：r1..r3 + 4 个栈参数 = 8 个 8x8 字形
                    for i in range(4):
                        stack.append(int.from_bytes(
                            bytes(mu.mem_read(sp + 4 * i, 4)), 'little') & 0xFF)
                self.rec[address].append((args, stack))

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

    REC = {DRAW16: [], DRAW8A: [], DRAW8B: [], INVERT: [], BTN: [],
           ICON_TX_A: [], ICON_TX_B: []}

    def render(idx, edit=0, settings=None, state65=0, ram=None, record=True):
        for k in REC:
            REC[k] = []
        e = Emu(rec=(REC if record else None))
        e.poke(0x200004F2, [idx])
        e.poke(0x200004F1, [edit])
        e.poke(0x200004F0, [1])
        e.poke(0x20000565, [state65])
        if settings is None:
            settings = [0] * 16
        e.poke(SETTINGS, settings)
        for addr, blob in (ram or {}).items():
            e.poke(addr, blob)
        good = e.call(RENDER)
        fb = e.peek(FB, FB_BYTES)
        return good, fb, e

    def opt_rows():
        """收集右栏(row>=4)的文字行：(row, text) 与箭头行。"""
        rows = {}
        arrow = []
        for (args, stack) in REC[DRAW16]:
            row, col, gi, sh = args
            if row < 4:
                continue                     # 顶栏（C.Fn / Ver2.0）
            if col >= 0x58:
                rows.setdefault(row, []).append((col, gi))
            elif 0x48 <= col < 0x58 and gi == 0x41:
                arrow.append(row)
        out = []
        for row in sorted(rows):
            out.append((row, decode_text(sorted(rows[row]), glyph_w)))
        return out, sorted(set(arrow))

    # 4.1 每页 4 个标签（把 idx 设为该页第一项，标签都会画出）
    w('')
    w('  4.1 每页 4 行标签（左栏 x<0x40）')
    page_labels = {}
    for page in range(3):
        good, fb, e = render(page * 4)
        chk(good, '渲染 page=%d 成功' % page)
        rows = {}
        for (args, stack) in REC[DRAW16]:
            row, col, gi, sh = args
            if row in (2, 4, 6, 8) and col < 0x40:
                rows.setdefault(row, []).append((col, gi))
        labels = []
        for row in (2, 4, 6, 8):
            items = sorted(rows.get(row, []))
            labels.append(decode_text(items, glyph_w) if items else '?')
        page_labels[page] = labels
        w('    page %d: %s' % (page, labels))
        for i, lab in enumerate(labels):
            gi = [g for c, g in sorted(rows.get((2, 4, 6, 8)[i], []))]
            w('      项 %2d  标签 %-8r 字形索引 %s' % (
                page * 4 + i, lab, ' '.join('0x%02X' % x for x in gi)))
    expected = {0: ['m/ft', 'AF', 'STBY', 'RX STBY'],
                1: ['SCAN', 'CH', 'ID', 'BEEP'],
                2: ['LIGHT', 'LCD', 'FEB ACL', 'TX']}
    chk(page_labels == expected, '12 项标签与证据一致', str(page_labels))
    w('')
    w('  → UI 上不显示 F1..F12 编号，只显示上述名称；左栏宽 64px，文字约居中。')

    # 4.2 每项可选项/取值（右栏 x>=0x48）
    w('')
    w('  4.2 每项右侧“值/选项”栏（x=0x5A 起；0x50 的 0x41 字形是箭头）')
    item_val = {}
    for idx in range(12):
        good, fb, e = render(idx)
        chk(good, '渲染 idx=%d 成功' % idx)
        opts, arrow = opt_rows()
        item_val[idx] = opts
        w('    idx=%2d 值行: %s   箭头行: %s' % (
            idx, '  |  '.join('%d:%s' % o for o in opts) if opts else '(无文字)',
            ['row%d' % r for r in arrow] if arrow else '-'))
        extra = []
        for (args, stack) in REC[DRAW8A]:
            extra.append('8x8@row%d,col0x%02X,g=0x%02X' % (args[0], args[1], args[2]))
        for (args, stack) in REC[DRAW8B]:
            extra.append('8x8b@row%d,col0x%02X,g=0x%02X' % (args[0], args[1], args[2]))
        for name, key in (('icon16A', ICON_TX_A), ('icon16B', ICON_TX_B)):
            for (args, stack) in REC[key]:
                extra.append('%s@row%d,col0x%02X' % (name, args[0], args[1]))
        if extra:
            w('         其它绘制: %s' % '; '.join(extra[:10]))

    # 4.3 数值项 / 特殊项
    w('')
    w('  4.3 数值项与特殊项的实际绘制')
    render(5, settings=[0, 0, 0, 0, 0, 0] + [0] * 10)
    w('    CH  idx=5 settings[5]=0  → %s' % (opt_rows()[0],))
    render(5, settings=[0, 0, 0, 0, 0, 31] + [0] * 10)
    w('    CH  idx=5 settings[5]=31 → %s（= 显示 32）' % (opt_rows()[0],))
    render(6, settings=[0] * 6 + [0] + [0] * 9)
    id0 = opt_rows()[0]
    render(6, settings=[0] * 6 + [37] + [0] * 9)
    id37 = opt_rows()[0]
    w('    ID  idx=6 ID=0  → %s' % (id0,))
    w('    ID  idx=6 ID=37 → %s' % (id37,))
    chk(any(t == 'OFF' for r, t in id0), 'ID=0 的显示分支画出 "OFF"（0x08004F2C 分支）')
    chk(any(t == '37' for r, t in id37), 'ID=37 显示两位十进制 "37"')
    render(6, edit=1, settings=[0] * 6 + [37] + [0] * 9)
    inv = [a[0] for a, st in REC[INVERT]]
    w('    ID  编辑态(ID=37)：反显条 0x08005440 入参 value=%s' % (inv,))
    chk(inv == [0], '数值项(ID/CH)：编辑态反显固定值行（0x08005440 收到 0 → 文本行 2）')
    render(7, edit=1, settings=[0] * 7 + [1] + [0] * 8)
    inv7 = [a[0] for a, st in REC[INVERT]]
    w('    BEEP 编辑态(value=1)：反显条 0x08005440 入参 value=%s' % (inv7,))
    chk(inv7 == [1], '选项项(BEEP=ON)：编辑态反显选中选项行（0x08005440 收到 1 → 行 3）')
    render(8)
    w('    LIGHT idx=8 → %s（3 档，max=2）' % (opt_rows()[0],))
    render(4, state65=0)
    w('    SCAN idx=4 state=0 → %s' % (opt_rows()[0],))
    render(4, state65=1, ram={0x2000056B: [0x0A]})
    w('    SCAN idx=4 state=1（进度数字，0x2000056B=0x0A）→ 8x8 数字: %s'
      % [(a[0], a[1], a[2]) for a, st in REC[DRAW8A]][:6])
    render(4, state65=3, ram={0x20000584: [1, 2, 3, 4, 5, 6, 7, 8]})
    w('    SCAN idx=4 state=3（2×4 网格，0x20000584=1..8）→ 8x8 数字: %s'
      % [(a[0], a[1], a[2]) for a, st in REC[DRAW8A]][:8])
    for v in (0, 3, 6):
        render(9, settings=[0] * 9 + [v] + [0] * 6)
        w('    LCD idx=9 settings[9]=%d → %s   箭头row=%s'
          % (v, opt_rows()[0], opt_rows()[1]))
    w('        LCD 右栏是 -3..+3 的竖排档位（内部 0..6）：表 0x080177C5 每档 2B，')
    w('        0x42=\'-\'、0x43=\'+\'，箭头(0x41)固定在 row6 指向当前档。')
    for v in (0, 1):
        render(11, settings=[0] * 11 + [v])
        w('    TX idx=11 value=%d → 图标 %s   箭头row=%s' % (
            v,
            ['row%d:tabA' % a[0] for a, st in REC[ICON_TX_A]] +
            ['row%d:tabB' % a[0] for a, st in REC[ICON_TX_B]],
            opt_rows()[1]))
    w('        TX 两个 16×16 位图：0x08017314（tab A）/ 0x08017334（tab B），')
    w('        以 XOR 方式叠在 row4/row6；箭头选中其一。')
    # 4.4 底部按钮
    w('')
    w('  4.4 底部按钮栏（y=80..95，x=0/0x2A/0x54/0x7E 四个框）')
    labels_btn = []
    for (args, stack) in REC[BTN]:
        gs = [x for x in [args[1], args[2], args[3]] + stack]
        s = ''.join('' if g == 0xFF else g2c(g) for g in gs)
        labels_btn.append((args[0], s))
    w('    x=0x%02X 标签=%r' % labels_btn[0] if labels_btn else '    (未捕获)')
    for x, s in labels_btn[1:]:
        w('    x=0x%02X 标签=%r（空框）' % (x, s))
    chk(bool(labels_btn) and labels_btn[0][1] == 'CLEAR',
        '底部第一个按钮文字 = "CLEAR"（8x8 字体 0x08015EBA）')

    w('')
    w('  4.5 关键反汇编摘录（逐条对应上面的画面/交互结论）')
    snippet(0x08004E86, 0x08004F4C, 'ID 项：标签 "ID"、ID!=0 两位数、ID==0 → 画 "OFF"')
    snippet(0x0800D42C, 0x0800D45C, 'inc 0x0800D3D8：编辑态改值 / 非编辑态换项 / 越界处理')
    snippet(0x08006220, 0x0800626A, 'dec 0x080061CC：改值/回绕 + 项6(重设同步字) + 项0xB(拷贝 CH)')

    # =====================================================================
    w('')
    w('=' * 78)
    w('5. 导航 / 改值行为（Unicorn 行为级）')
    w('=' * 78)
    stubs = [0x080037CC, 0x08012320, 0x080121BC, 0x08014EF8, 0x0800FB2C]
    nav = Emu(stubs=stubs, rec=None)

    def nav_reset(idx, edit, settings=None, state=0):
        nav.reset()
        nav.poke(0x200004F0, [1])            # C.Fn 画面打开
        nav.poke(0x200004F1, [edit])
        nav.poke(0x200004F2, [idx])
        nav.poke(0x20000550, [0])
        nav.poke(0x2000055D, [0])
        nav.poke(0x20000565, [state])
        nav.poke(SETTINGS, settings if settings is not None else [0] * 16)

    def idx_now():
        return nav.peek(0x200004F2, 1)[0]

    def val_now(i):
        return nav.peek(SETTINGS + i, 1)[0]

    # 5.1 项导航 0..0xB 回绕
    nav_reset(0, 0)
    seq = []
    for _ in range(14):
        nav.call(INC)
        seq.append(idx_now())
    w('')
    w('  5.1 item 导航（[0x200004F0]=1, [0x200004F1]=0）')
    w('    + 序列(从0): %s' % seq)
    chk(seq == [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0, 1, 2],
        '『+』在项 0..0xB 间循环（0→1→…→0xB→0）')
    nav_reset(0, 0)
    seq = []
    for _ in range(14):
        nav.call(DEC)
        seq.append(idx_now())
    w('    - 序列(从0): %s' % seq)
    chk(seq == [11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 11, 10],
        '『-』在项 0..0xB 间反向循环（0→0xB→…）')
    chk(nav.peek(0x20000478 + 0xA, 2) == b'\xd0\x07',
        '每次按键把画面超时计数设为 0x07D0')
    chk(nav.peek(0x20000532, 2) == b'\xc8\x00',
        '每次按键把“保存倒计时”0x20000532 设为 200（0xC8）')
    chk(nav.peek(0x2000055A, 1) == b'\x28', '按键同时刷新 0x2000055A=0x28（背光/活动计时）')

    # 5.2 改值：inc/dec 与最大值表
    w('')
    w('  5.2 改值（[0x200004F1]=1）：settings[idx] 按最大值表回绕')
    maxes = {}
    for i in range(12):
        maxes[i] = data[MAXTAB - BASE + i]
    w('    最大值表 0x%08X = %s' % (
        MAXTAB, ' '.join('%02X' % maxes[i] for i in range(12))))
    chk([maxes[i] for i in range(12)] ==
        [0x01, 0x01, 0x01, 0x01, 0x01, 0x1F, 0x63, 0x01, 0x02, 0x06, 0x01, 0x01],
        '最大值表字节与原厂一致')
    for idx in range(12):
        mx = maxes[idx]
        # inc at max
        settings = [0] * 16
        settings[idx] = mx
        nav_reset(idx, 1, settings)
        nav.call(INC)
        v_after = val_now(idx)
        exp = mx if idx == 9 else 0     # 项 9(LCD) 上限钳位，其余回绕 0
        chk(v_after == exp, 'idx=%d max=%d：『+』后 settings[%d]=%d（期望 %d）'
            % (idx, mx, idx, v_after, exp))
        # dec at 0
        settings = [0] * 16
        nav_reset(idx, 1, settings)
        nav.call(DEC)
        v_after = val_now(idx)
        exp = 0 if idx == 9 else mx
        chk(v_after == exp, 'idx=%d：0 处『-』后 settings[%d]=%d（期望 %d）'
            % (idx, idx, v_after, exp))

    # 5.3 项 6(ID)：改值触发同步字函数；项 0xB：改值拷贝到 0x200004F5
    w('')
    w('  5.3 改值的副作用（静态调用点 + 行为验证）')
    called = {'sync': None}

    class Emu2(Emu):
        def _hook(self, mu, address, size, ud):
            if address == 0x080121BC:      # 桩入口仍在，记录入参
                called['sync'] = mu.reg_read(UC_ARM_REG_R0)
            Emu._hook(self, mu, address, size, ud)

    e2 = Emu2(stubs=stubs, rec=None)
    e2.reset()
    e2.poke(0x200004F0, [1])
    e2.poke(0x200004F1, [1])
    e2.poke(0x200004F2, [6])
    e2.poke(0x20000550, [0])
    e2.poke(0x2000055D, [0])
    e2.poke(0x20000565, [0])
    e2.poke(SETTINGS, [0] * 6 + [36] + [0] * 9)
    e2.call(INC)
    chk(e2.peek(SETTINGS + 6, 1)[0] == 37 and called['sync'] == 37,
        'ID 改 36→37 时调用同步字函数 0x080121BC(37)')
    called['sync'] = None
    e2.poke(0x200004F2, [11])
    e2.poke(SETTINGS + 11, [0])
    e2.poke(0x200004F5, [0xAA])
    e2.call(INC)
    chk(e2.peek(0x200004F5, 1)[0] == 1,
        '项 0xB(TX) 改值后 settings[0xB] 被拷贝到 RAM 0x200004F5')

    # =====================================================================
    w('')
    w('=' * 78)
    w('6. 进入/退出/编辑/保存 的按键链路（静态证据）')
    w('=' * 78)
    snippet(0x0800DF18, 0x0800DF30, '键码分派：键码 1→关闭、4→其它菜单、8→C.Fn 开关(0x080053F4)')
    snippet(0x0800DF5C, 0x0800DF68, '键码 8 分支（[0x20000550]==0 时切换 C.Fn 画面）')
    snippet(CFN_TOGGLE, 0x08005426, '0x080053F4：切换 [0x200004F0]（C.Fn 开/关），清编辑态/settings[4]')
    snippet(CFN_CLOSE, 0x0800DC6A, '0x0800DC14：返回键关闭 C.Fn（清 [0x200004F0]/[0x200004F1]）')
    snippet(SET_KEY, 0x0801209A, 'SET 键 0x08012058：C.Fn 内切换编辑态 [0x200004F1]；SCAN 特例')
    snippet(PLUSMINUS_SCAN, 0x0800614C, '+/- 键扫描：GPIOB(0x48000800) bit7/8，历史码 0xB4→+, 0x78→-')
    snippet(SAVE_CNT_SET, 0x080114D6, '0x080114CC：改值后保存倒计时=200')
    snippet(SAVE_CNT_TICK, 0x080114C0, '0x080114A8：倒计时到 0 → 保存标志 0x20000534=1')
    snippet(SAVE_TASK, 0x0800E9E2, '保存任务 0x0800E9A0 → 组记录 0x08005EF0 → 比较/写 flash')
    snippet(SAVE_BUILD + 0x160, 0x08006020, '记录内容：settings[6](ID) 进入 word')
    w('')
    w('  注：C.Fn 的 12 个 settings 字节全部由 0x08005EF0 组进 flash 记录')
    w('      （settings[0..1]→rec+0x2C、[2..5]→+0x30、[6..9]→+0x34、[10..13]→+0x38），')
    w('      因此复用任一 C.Fn 项不需要新增持久化代码。')

    # SET 键行为验证
    setter = Emu(stubs=stubs, rec=None)
    setter.reset()
    setter.poke(0x200004F0, [1])
    setter.poke(0x200004F1, [0])
    setter.poke(0x200004F2, [2])
    setter.call(SET_KEY)
    a = setter.peek(0x200004F1, 1)[0]
    setter.call(SET_KEY)
    b = setter.peek(0x200004F1, 1)[0]
    chk(a == 1 and b == 0, 'SET 键在 C.Fn 内切换编辑态（0→1→0）')
    toggle = Emu(stubs=stubs, rec=None)
    toggle.reset()
    toggle.poke(0x200004F0, [0])
    toggle.call(CFN_TOGGLE)
    on = toggle.peek(0x200004F0, 1)[0]
    toggle.poke(0x200004F1, [1])
    toggle.call(CFN_TOGGLE)
    off = toggle.peek(0x200004F0, 1)[0]
    chk(on == 1 and off == 0, 'C.Fn 画面开关 0x080053F4：0→1→0（同一个键进出）')

    # =====================================================================
    w('')
    w('=' * 78)
    w('7. 画面重建（真实执行渲染函数后的帧缓冲，168×96）')
    w('=' * 78)
    w('')
    w('  说明：# = 点亮像素。168 列 × 96 行 = 12 条带 × 8 行。')
    w('  布局：y0..15 顶栏("C.Fn"+状态图标+“Ver2.0”)；y16..79 左侧 4 行列表(x0..64)；')
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
        ('idx=0 (page0 选中 m/ft，未进入编辑)', 0, 0, [0] * 16, 0, None),
        ('idx=3 (page0 选中 RX STBY，settings[3]=1=30min)', 3, 0,
         [0, 0, 0, 1] + [0] * 12, 0, None),
        ('idx=6 (page1 选中 ID，ID=0 → "OFF")', 6, 0,
         [0] * 6 + [0] + [0] * 9, 0, None),
        ('idx=6 编辑态 (ID=37，选中值行反显)', 6, 1,
         [0] * 6 + [37] + [0] * 9, 0, None),
        ('idx=8 (page2 选中 LIGHT，三档 12sec/OFF/ON)', 8, 0,
         [0] * 16, 0, None),
        ('idx=11 (page2 选中 TX，value=1，16x16 图标)', 11, 0,
         [0] * 11 + [1] + [0] * 4, 0, None),
    ]
    for title, idx, edit, settings, state65, ram in shots:
        good, fb, e = render(idx, edit, settings, state65, ram)
        w('')
        w('  ┌─ %s   (%d 个非零字节)' % (title, sum(1 for b in fb if b)))
        for y, line in enumerate(fb_art(fb)):
            w('  │' + line + '│  y=%d' % y)
        w('  └' + '─' * FB_W)
        chk(good, '画面重建 %s' % title)

    # =====================================================================
    w('')
    w('=' * 78)
    w('8. 结论')
    w('=' * 78)
    w('')
    w('  一、C.Fn 列表页 = 渲染函数 0x08004914（画面分派 0x0801472C，[0x200004F0]==1 时调用）：')
    w('      12 项 = 3 页 × 4 行；每页画 4 行“名称”（左栏，文字约居中于 x≈32），')
    w('      当前页由 idx>>2 决定，选中行由 idx&3 决定（0x080049B8 "lsrs r1,#2" /')
    w('      0x080049BC "lsls r1,#0x1e;lsrs #0x1e"）；选中行整行反显(0x08005260)。')
    w('      没有 F1..F12 编号，只有名称；没有滚动条，越界即换页。')
    w('  二、12 项（index → 名称 / 右栏可选项 / 取值）：')
    tbl = [
        (0, 'm/ft', 'm | ft', '0=m, 1=ft', 1),
        (1, 'AF', 'ON | OFF', '0=ON, 1=OFF', 1),
        (2, 'STBY', 'ON | OFF', '0=ON, 1=OFF', 1),
        (3, 'RX STBY', '60min | 30min', '0=60min, 1=30min', 1),
        (4, 'SCAN', 'OFF | START（state=1/2 显示数字，state=3 显示 2×4 网格）',
         '动作项：SET 在 settings[4]==1 时触发扫描 0x0800FB2C', 1),
        (5, 'CH', '两位十进制 = settings[5]+1', '0..31 → 显示 01..32', 0x1F),
        (6, 'ID', 'OFF (ID=0) | 两位十进制 01..99', '0=OFF，1..99 直接显示', 0x63),
        (7, 'BEEP', 'OFF | ON', '0=OFF, 1=ON', 1),
        (8, 'LIGHT', '12sec | OFF | ON', '0=12sec, 1=OFF, 2=ON', 2),
        (9, 'LCD', '竖排 -3..+3 刻度（内部 0..6；表 0x080177C5，0x42=-／0x43=+，箭头指当前档）',
         '0..6，上限钳位不回绕', 6),
        (10, 'FEB ACL', 'ON | OFF', '0=ON, 1=OFF', 1),
        (11, 'TX', '两个 16×16 图标（0x08017314 / 0x08017334）', '0/1；改值后拷贝到 0x200004F5', 1),
    ]
    w('      idx  名称      右栏内容                                  取值')
    for (i, nm, opt, rule, mx) in tbl:
        w('      %2d   %-8s  %-40s  %s' % (i, nm, opt, rule))
    w('  三、按键路径：')
    w('      MENU 类键（矩阵键码 8）→ 0x080053F4 切换 C.Fn 画面（同一个键再按退出）；')
    w('      返回键（键码 1）→ 0x0800DC14 关闭；')
    w('      +/- 键（GPIOB 0x48000800 bit7/8，0x080060FC）→ 未编辑时移动项（0..0xB 循环），')
    w('      编辑态时改值（按 0x08018362 最大值表回绕，项 9 钳位）；')
    w('      SET 键（0x08012058）→ 切换编辑态 0x200004F1（值行反显 0x08005440）。')
    w('  四、复用现有 C.Fn 项作“密钥开关”：零 UI 改动的结论 = 是')
    w('      · 12 项全部是真实项、全部可选中（0..0xB 无隐藏/预留项；AD100 那种预留项本机没有）；')
    w('      · 现有 ID 项（index 6，即第 7 项、第 2 页第 3 行）本身就是 C.Fn 项，')
    w('        ID=0 时右栏显示 "OFF"（0x08004F2C 分支），即现方案（ID=0 → KEY）')
    w('        在灯端就是“C.Fn → ID → OFF”，用户可见可操作，无需改 UI；')
    w('      · 若要“ON=密钥模式”的直觉开关，可复用 ON/OFF 项：BEEP(index7, 0=OFF/1=ON)、')
    w('        FEB ACL(index10, 0=ON/1=OFF)、AF(index1, 0=ON/1=OFF)；都不需要改绘制代码。')
    w('      · 改值后 200 tick 自动写 flash（0x080114CC→0x080114A8→0x0800E9A0→0x08005EF0），')
    w('        12 个 settings 字节全在记录里，复用项无需补持久化。')
    w('  五、风险/未确认：')
    w('      · 物理按键丝印无法从固件恢复（矩阵列选 GPIOC 0x48000C00，行读 GPIOC/GPIOB），')
    w('        “MENU/SET/+/-”的名称是按键码功能与同代 UI 推断；')
    w('      · 项 10 名称逐字为 "FEB ACL"，含义未在固件中解释（ON/OFF 两档）；')
    w('      · 项 4(SCAN)/9(LCD)/11(TX) 的右栏是状态/图标显示，未做逐像素人工核对。')
    w('')
    w('  全部检查: %s' % ('PASS' if ok else 'FAIL'))

    text = '\n'.join(lines) + '\n'
    with open(OUT, 'w', encoding='utf-8') as fh:
        fh.write(text)
    print(text)
    print('written: %s (%d lines)' % (OUT, len(lines)))
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
