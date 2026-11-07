# -*- coding: utf-8 -*-
"""X3-C v1.38 UI 原型补丁（ui-x3c 任务）——「密钥开关 + 广播引闪」注入层。

只做**原位补丁**（输出长度与输入一致），不修改 keypatch/devices/x3c.py。

A. 已验证部分（Unicorn 调用级逐条执行验证，见 verify_ui）
   1) 0x080F0000 注入 ui_key_toggle()：翻转 RAM 密钥标志字节 KEY_ON。
      fw-x3c 的双发注入代码读这个字节：1=密钥模式（先私钥帧再普通帧），0=原厂单发。
   2) 0x080F0020 注入 ui_broadcast_allch()：32 频道顺序广播引闪
      （set_channel → 写 SYNC=KEY → 属性帧 → TEST 帧）。
   3) 0x080F0100 注入 ui_broadcast_allid()：全频道 + 全 ID 扫描（慢，十几秒）。
   4) 0x080F0200 起：自绘汉字点阵（密/钥/广/播/全，24x24 1bpp，来自
      keypatch/dev/lead/glyphs24）+ 0x080F0400 UTF-8 标签字符串。

B. UI 接线（默认 opts["enable_menu_patch"]=False）
   底部页签 2→3 的坐标补丁字节 + 快捷面板磁贴方案。**未在模拟器端到端验证**
   （原因见 FEASIBILITY.md §6），默认不写入，作为"下一步接线方案"输出。

接口（Lead 合并进 keypatch/devices/x3c.py 时调用）：

    from keypatch.dev.x3c_ui.ui_patch import apply_ui, verify_ui, UI_DEFAULTS
    patched, log = apply_ui(data, {"key": 0x6B21})
    results = verify_ui(patched, {"key": 0x6B21})

KEY_ON 标志：默认 0x20001080（选址证据见 FEASIBILITY.md §4），
可用 opts["key_flag"] 覆盖；fw-x3c 双发代码必须读同一地址。
"""
from __future__ import annotations

import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")
BASE = 0x08004800
IMAGE_SIZE = 1021468
SHA256 = "45fdd9522c432f4be60a470741d3e5c12a7ee629eb2b277aed5187639f85ef62"

# ---- 注入区分配（Lead 定：0x080F0000..0x080F6FFF 共 28KB 全 0xFF、无引用） ----
CAVE = 0x080F0000
CAVE_END = 0x080F7000
OFF_KEY_TOGGLE = 0x000
OFF_BROADCAST_ALLCH = 0x020
OFF_BROADCAST_ALLID = 0x100
OFF_GLYPHS = 0x200
OFF_STRINGS = 0x400
OFF_RESERVED_PAGE = 0x800        # 预留：以后放"广播页 / KEY 页"构建函数

# ---- 固件函数 ----
SET_CHANNEL = 0x08014814        # (r0=ch 0..31) 写 FREQ2/1/0
SET_SYNC = 0x08014864           # (r0=id) sync = 50024/(id+1) -> CC2500 reg4/5
WRITE_REG = 0x08023540          # (r0=reg, r1=val)
SEND_PROPERTY = 0x0801184C      # (r0=group, r1=key, r2=val) -> 帧 [A9 g k v]
SEND_SHORT = 0x080118B0         # (r0=cmd) -> 帧 [D5 cmd]

# ---- UI 库（FEASIBILITY.md §2） ----
UI_CREATE_WINDOW = 0x08010ACC   # (parent,type,x,y,w,h)
UI_CREATE_WIDGET = 0x08006220   # (parent,type,x,y,w,h)
UI_CREATE_TEXT = 0x0802553E     # (parent,type,x,y,font,color,fmt_ptr)
UI_SET_TEXT = 0x0802D97A        # (w, str, 0)
UI_SHOW = 0x0802DA22
UI_HIDE = 0x0802E2D2
UI_ATTACH = 0x0802DB44

# ---- 页面分发（FEASIBILITY.md §1） ----
UI_TICK = 0x08014B28            # 100ms 软定时器回调（页面切换/状态刷新）
PAGE_BUILDER = 0x08006F0C       # FUN_08006f0c(win, page)：page 2..13 各一个 case
PAGE_HOME = 0x0800FE78          # 首页（含主菜单/底部页签）
PAGE_MAINMENU = 0x080079E8      # 首页里的主菜单/底部页签构建
RAM_PAGE_REQ = 0x200004C4       # 请求页面 id
RAM_UI_MODE = 0x200003E7        # 当前页面/模式

# ---- KEY_ON 标志 ----
# 设置结构：flash 页 0x080F7800（魔数 0x4B3C2D1E，'Z' 在 RAM 副本 +0x87），
# RAM 副本 = 0x20000D84；FUN_080112E8 上电加载、FUN_080219E4 写回。
# 取其中「默认值初始化/设置项写入都不会碰」的空闲字节：偏移 0x8D（证据见 FEASIBILITY §3/§4.1）。
SETTINGS_RAM_COPY = 0x20000D84
KEY_FLAG_OFFSET_IN_SETTINGS = 0x8D
DEFAULT_KEY_FLAG = 0x20001080   # Lead 拍板：RAM 版 v1（设置扇区空字节留给 v2 持久化）
KEY_FLAG_V2_SETTINGS = SETTINGS_RAM_COPY + KEY_FLAG_OFFSET_IN_SETTINGS  # 0x20000E11
SAVE_SETTINGS = 0x080219E4        # 把 RAM 扇区副本写回 flash（无参）

# ---- 自绘字模（24x24 1bpp，Lead 的 glyph_gen.ps1：每行 u32，MSB 在左，1=亮） ----
GLYPHS24 = os.path.join(ROOT, "keypatch", "dev", "lead", "glyphs24")
GLYPH_CHARS = {
    0x5BC6: "u5BC6",   # 密
    0x94A5: "u94A5",   # 钥
    0x5E7F: "u5E7F",   # 广
    0x64AD: "u64AD",   # 播
    0x5168: "u5168",   # 全
}
GLYPH_SIZE = 24
GLYPH_ROWS = GLYPH_SIZE
GLYPH_BYTES = GLYPH_ROWS * 4

LABELS = {
    "key": "密钥",
    "broadcast": "广播",
    "allch": "全频道",
    "allid": "全ID",
    "on": "KEY ON",
    "off": "KEY OFF",
}


# ---------------------------------------------------------------------------
# Thumb 代码生成小工具
# ---------------------------------------------------------------------------
class Code:
    """够用的 Thumb-1 生成器。"""

    def __init__(self, addr: int):
        self.addr = addr
        self.buf = bytearray()
        self.lits: list[tuple[int, int, int]] = []      # (pos, rt, value)
        self.fix: list[tuple[int, int]] = []            # (pos, target_addr_or_label)
        self.labels: dict[str, int] = {}

    def here(self) -> int:
        return self.addr + len(self.buf)

    def hw(self, value: int):
        self.buf += struct.pack("<H", value & 0xFFFF)
        return self

    def push(self, regs_mask: int):
        return self.hw(0xB400 | (regs_mask & 0x1FF))

    def pop(self, regs_mask: int):
        return self.hw(0xBC00 | (regs_mask & 0x1FF))

    def movs(self, rd: int, imm8: int):
        assert 0 <= rd <= 7 and 0 <= imm8 <= 255
        return self.hw(0x2000 | (rd << 8) | imm8)

    def mov(self, rd: int, rm: int):
        return self.hw(0x4600 | (rm << 3) | rd)

    def adds(self, rd: int, imm8: int):
        return self.hw(0x3000 | (rd << 8) | imm8)

    def lsrs(self, rd: int, rm: int, imm5: int):
        return self.hw(0x0800 | ((imm5 & 0x1F) << 6) | (rm << 3) | rd)

    def uxtb(self, rd: int, rm: int):
        return self.hw(0xB2C0 | (rm << 3) | rd)

    def eors(self, rdn: int, rm: int):
        return self.hw(0x4040 | (rm << 3) | rdn)

    def cmp(self, rn: int, imm8: int):
        return self.hw(0x2800 | (rn << 8) | imm8)

    def ldrb_imm(self, rt: int, rn: int, imm5: int = 0):
        """LDRB (immediate, T1): 0111 1 imm5 Rn Rt"""
        return self.hw(0x7800 | ((imm5 & 0x1F) << 6) | (rn << 3) | rt)

    def str_imm5(self, rt: int, rn: int, imm5: int = 0):
        """STR (immediate, T1): 0110 0 imm5 Rn Rt（Rn 只能是 r0..r7）"""
        return self.hw(0x6000 | ((imm5 & 0x1F) << 6) | (rn << 3) | rt)

    def ldr_imm5(self, rt: int, rn: int, imm5: int = 0):
        """LDR (immediate, T1): 0110 1 imm5 Rn Rt（Rn 只能是 r0..r7）"""
        return self.hw(0x6800 | ((imm5 & 0x1F) << 6) | (rn << 3) | rt)

    def str_sp(self, rt: int, imm8: int = 0):
        """STR (immediate, T2): 1001 0 Rt imm8 -> [sp, #imm8*4]"""
        return self.hw(0x9000 | (rt << 8) | (imm8 & 0xFF))

    def sub_sp(self, nbytes: int):
        return self.hw(0xB080 | ((nbytes // 4) & 0x7F))

    def add_sp(self, nbytes: int):
        return self.hw(0xB000 | ((nbytes // 4) & 0x7F))

    def strb_imm(self, rt: int, rn: int, imm5: int = 0):
        """STRB (immediate, T1): 0111 0 imm5 Rn Rt"""
        return self.hw(0x7000 | ((imm5 & 0x1F) << 6) | (rn << 3) | rt)

    def strh_imm(self, rt: int, rn: int, imm5: int = 0):
        """STRH (immediate, T1): 1000 0000 imm5 Rn Rt"""
        return self.hw(0x8000 | ((imm5 & 0x1F) << 6) | (rn << 3) | rt)

    def ldr_lit(self, rt: int, value: int):
        self.lits.append((len(self.buf), rt, value))
        return self.hw(0x4800 | (rt << 8))

    def bl(self, target: int):
        here = self.here()
        off = target - (here + 4)
        s = 1 if off < 0 else 0
        i1 = (off >> 23) & 1
        i2 = (off >> 22) & 1
        j1 = i1 if s else ((~i1) & 1)
        j2 = i2 if s else ((~i2) & 1)
        return self.buf.extend(struct.pack(
            "<HH", 0xF000 | (s << 10) | ((off >> 12) & 0x3FF),
            0xD000 | (j1 << 13) | (j2 << 11) | ((off >> 1) & 0x7FF)))

    def bcc(self, target_addr: int):
        self.fix.append((len(self.buf), target_addr, 0xD300))
        return self.hw(0xD300)

    def beq_label(self, name: str):
        """前向 BEQ（T1，±256B），目标由 label(name) 给出。"""
        self.fix.append((len(self.buf), name, 0xD000))
        return self.hw(0xD000)

    def bne_label(self, name: str):
        """前向 BNE（T1）"""
        self.fix.append((len(self.buf), name, 0xD100))
        return self.hw(0xD100)

    def bcc_label(self, name: str):
        """前向 BCC（T1，±256B）"""
        self.fix.append((len(self.buf), name, 0xD300))
        return self.hw(0xD300)

    def b_label(self, name: str):
        """前向无条件 B（T2，11 位偏移）"""
        self.fix.append((len(self.buf), name, 0xE000))
        return self.hw(0xE000)

    def ldrh_imm(self, rt: int, rn: int, byte_off: int):
        """LDRH (immediate, T1): 1000 1 imm5 Rn Rt，imm5 = byte_off/2"""
        return self.hw(0x8800 | (((byte_off // 2) & 0x1F) << 6) | (rn << 3) | rt)

    def subs(self, rd: int, rn: int, rm: int):
        """SUBS (register, T1): 0001 101 0 Rm Rn Rd"""
        return self.hw(0x1A00 | ((rm & 7) << 6) | ((rn & 7) << 3) | (rd & 7))

    def orrs(self, rdn: int, rm: int):
        """ORRS (register, T1): 0100 0011 00 Rm Rdn"""
        return self.hw(0x4300 | ((rm & 7) << 3) | (rdn & 7))

    def cmp_reg(self, rn: int, rm: int):
        """CMP (register, T1): 0100 0101 00 Rm Rn"""
        return self.hw(0x4280 | ((rm & 7) << 3) | (rn & 7))

    def ands(self, rdn: int, rm: int):
        """ANDS (register, T1): 0100 0000 00 Rm Rdn"""
        return self.hw(0x4000 | ((rm & 7) << 3) | (rdn & 7))

    def label(self, name: str):
        self.labels[name] = self.here()
        return self

    def b_w(self, target: int):
        """B.W（T4，±16MB）"""
        here = self.here()
        off = target - (here + 4)
        s = 1 if off < 0 else 0
        i1 = (off >> 23) & 1
        i2 = (off >> 22) & 1
        j1 = i1 if s else ((~i1) & 1)
        j2 = i2 if s else ((~i2) & 1)
        return self.buf.extend(struct.pack(
            "<HH", 0xF000 | (s << 10) | ((off >> 12) & 0x3FF),
            0x9000 | (j1 << 13) | (j2 << 11) | ((off >> 1) & 0x7FF)))

    def finish(self) -> bytes:
        while (self.addr + len(self.buf)) % 4:
            self.buf.append(0)
        lit_base = self.addr + len(self.buf)
        for slot, (pos, rt, value) in enumerate(self.lits):
            ins_addr = self.addr + pos
            pc = (ins_addr + 4) & ~3
            imm8 = (lit_base + 4 * slot - pc) // 4
            if not (0 <= imm8 <= 0xFF):
                raise ValueError("ldr 字面量太远: %#x" % (lit_base - pc))
            self.buf[pos:pos + 2] = struct.pack("<H", 0x4800 | (rt << 8) | imm8)
        for slot, (pos, rt, value) in enumerate(self.lits):
            self.buf += struct.pack("<I", value & 0xFFFFFFFF)
        for pos, target, kind in self.fix:
            ins_addr = self.addr + pos
            if isinstance(target, str):
                target = self.labels[target]
            delta = target - (ins_addr + 4)
            if kind == 0xE000:
                val = 0xE000 | ((delta // 2) & 0x7FF)
            else:
                val = kind | ((delta // 2) & 0xFF)
            self.buf[pos:pos + 2] = struct.pack("<H", val)
        return bytes(self.buf)


# ---------------------------------------------------------------------------
# A.1 密钥开关
# ---------------------------------------------------------------------------
def build_key_toggle(va: int, flag: int = DEFAULT_KEY_FLAG,
                     persist: bool = False) -> bytes:
    """u8 ui_key_toggle(void)：翻转 *flag（1=密钥开 / 0=关），返回新值。

        push {r4, r5, lr}
        movs  r4, #1
        ldr   r0, =flag
        ldrb  r1, [r0]
        ands  r1, #1          ; 归一化：0xFF(擦除态)/其它值都当 0
        eors  r1, r1, r4      ; 0 <-> 1
        strb  r1, [r0]
        mov   r5, r1
        [bl SAVE_SETTINGS]    ; persist=True 时写回 flash（0x080219E4）
        mov   r0, r5
        pop   {r4, r5, pc}
        .word flag
    """
    c = Code(va)
    c.push(0x130)            # push {r4, r5, lr}
    c.movs(4, 1)
    c.ldr_lit(0, flag)
    c.ldrb_imm(1, 0)
    c.movs(2, 1)             # r2 = 1（Thumb-1 没有 AND imm，用寄存器版）
    c.hw(0x4011)             # ands r1, r2 → 归一化：只有 1 保持 1，0xFF/其它变 0
    c.eors(1, 4)
    c.strb_imm(1, 0)
    c.mov(5, 1)              # r5 = 新值
    if persist:
        c.bl(SAVE_SETTINGS)  # 写回设置扇区（耗时几十 ms，带擦写）
    c.mov(0, 5)
    c.pop(0x130)             # pop {r4, r5, pc}
    return c.finish()


# ---------------------------------------------------------------------------
# A.2 广播引闪
# ---------------------------------------------------------------------------
def build_broadcast_allch(va: int, key: int = 0x6B21, group: int = 0x50,
                          power_key: int = 0xB1, power_val: int = -1,
                          model_key: int = 0xB2, model_on: int = -1,
                          trigger: int = 0x09, nchan: int = 32,
                          group_idx: int = 0, per_group: bool = False) -> bytes:
    r"""32 频道顺序广播：set_channel / SYNC=KEY / 当前功率 / 当前造型灯 / TEST。

    功率与造型灯取固件当前的 16 组设置表（GROUP_TABLE=0x20000B68，每项 6 字节）：
      * 功率   = 表[group_idx][+4] 的低 7 位  ->  send_property(g, 0xD1, 功率)
        照抄原厂 FUN_080089D0: FUN_0801184c(uVar7, 0xD1, *(iVar10+4) & 0x7f)
      * 造型灯 = 表[group_idx][+0]（0=开,1=关） ->  send_property(g, 0xB1, 1/0)
        照抄原厂 FUN_0800BDDC: FUN_0801184c(uVar5, 0xB1, 1/0)

    g 的取值：
      * per_group=False（默认）：g=0x50（ALL，原厂全局组）
      * per_group=True：g=10,11,12,13,14（A–E 逐组各发一遍）—— 实机若灯不认 ALL 时的备选
    opts 覆盖（测试用）：power_val>=0 固定功率；model_on>=0 固定造型灯值。
    """
    c = Code(va)
    c.push(0x170)                  # push {r4, r5, r6, lr}（16B，8 字节对齐）
    c.movs(4, 0)                   # r4 = ch
    c.ldr_lit(5, key)              # r5 = KEY
    c.ldr_lit(6, GROUP_TABLE + group_idx * 6)     # r6 = 组设置条目
    loop = c.here()
    c.mov(0, 4)
    c.bl(SET_CHANNEL)
    c.movs(0, 4)                   # write_reg(4, KEY>>8)
    c.lsrs(1, 5, 8)
    c.bl(WRITE_REG)
    c.movs(0, 5)                   # write_reg(5, KEY&0xFF)
    c.uxtb(1, 5)
    c.bl(WRITE_REG)

    groups = [10, 11, 12, 13, 14] if per_group else [group]
    for gi, gval in enumerate(groups):
        # ---- 功率 ----
        if power_val >= 0:
            c.movs(2, power_val & 0xFF)
        else:
            c.ldrb_imm(2, 6, 4)
            c.hw(0x0000 | ((25 & 0x1F) << 6) | (2 << 3) | 2)    # lsls r2,r2,#25
            c.lsrs(2, 2, 25)                                    # lsrs r2,r2,#25
        c.movs(0, gval)
        c.movs(1, PROP_KEY_POWER)
        c.bl(SEND_PROPERTY)
        # ---- 造型灯 ----
        if model_on >= 0:
            c.movs(2, model_on & 0xFF)
        else:
            c.ldrb_imm(2, 6, 0)
            c.cmp(2, 0)
            c.beq_label("lamp_on%d" % gi)
            c.movs(2, 0)
            c.b_label("lamp_send%d" % gi)
            c.label("lamp_on%d" % gi)
            c.movs(2, 1)
            c.label("lamp_send%d" % gi)
        c.movs(0, gval)
        c.movs(1, PROP_KEY_LAMP)
        c.bl(SEND_PROPERTY)

    # ---- TEST 帧 ----
    c.movs(0, trigger)
    c.bl(SEND_SHORT)
    c.adds(4, 1)
    c.cmp(4, nchan)
    c.bcc(loop)
    c.pop(0x170)                   # pop {r4, r5, r6, pc}
    return c.finish()


def build_broadcast_allid(va: int, key: int = 0x6B21, nchan: int = 32,
                          id_step: int = 1) -> bytes:
    """全频道 + 全 ID 扫描：每个频道先发一帧私有 KEY，再按 id_step 扫 0..99 普通 ID。"""
    c = Code(va)
    c.push(0x170)                  # push {r4, r5, r6, lr}
    c.movs(4, 0)
    c.ldr_lit(5, key)              # r5 = KEY（字面量池在函数尾）
    loop_ch = c.here()
    c.mov(0, 4)
    c.bl(SET_CHANNEL)
    c.movs(0, 4)                   # SYNC = KEY
    c.lsrs(1, 5, 8)
    c.bl(WRITE_REG)
    c.movs(0, 5)
    c.uxtb(1, 5)
    c.bl(WRITE_REG)
    c.movs(0, 0x50)
    c.movs(1, 0xB1)
    c.movs(2, 0x00)
    c.bl(SEND_PROPERTY)
    c.movs(0, 0x09)
    c.bl(SEND_SHORT)
    c.movs(6, 0)                   # r6 = id
    loop_id = c.here()
    c.mov(0, 6)
    c.bl(SET_SYNC)
    c.movs(0, 0x50)
    c.movs(1, 0xB1)
    c.movs(2, 0x00)
    c.bl(SEND_PROPERTY)
    c.movs(0, 0x09)
    c.bl(SEND_SHORT)
    c.adds(6, id_step)
    c.cmp(6, 100)
    c.bcc(loop_id)
    c.adds(4, 1)
    c.cmp(4, nchan)
    c.bcc(loop_ch)
    c.pop(0x170)                   # pop {r4, r5, r6, pc}
    return c.finish()


# ---------------------------------------------------------------------------
# A.3 快捷面板：新增 KEY / BROADCAST 两个磁贴（"接管尾调用"注入）
# ---------------------------------------------------------------------------
# 挂载点：FUN_080079E8 尾部 `bl 0x08027fe0`（0x08008236，字节 `1f f0 d3 fe`）。
# 该点上下文：
#   r4 = 刚建好的底部指示条控件；r6 = 滑出面板窗口（磁贴的父窗口）；
#   原指令作用：r0 = FUN_08027fe0()（取颜色），随后 `mov r1,r0; str r0,[sp,#0x10];
#   mov r0,r4; bl 0x0803157c`。
# 注入函数做法：先原样调 FUN_08027fe0（结果保存在 r5），然后用 **r6 作 parent**
# 按原厂磁贴完全相同的 API 建 2 个磁贴（FUN_08006220(parent,1,x,y,w,h)），
# 再调 FUN_0802D97A(tile, "KEY"/"BCAST", 0) 设标签，最后返回颜色。
UI_PANEL_EXT = 0x080F0800       # 注入函数入口（0x080F0800..0x080F08FF）
MAINMENU_HOOK_SITE = 0x08008236
MAINMENU_HOOK_BEFORE = "1ff0d3fe"     # bl 0x08027fe0
HANDLE_TABLE = 0x20042740       # FUN_080079E8 存的控件句柄表（见 §1.3）
OFF_TEXT_KEY = 0x500            # ASCII "KEY"
OFF_TEXT_BCAST = 0x510          # ASCII "BCAST"
# --- 点击动作接线（v1.1）---------------------------------------------------
RAM_TILE_KEY = 0x20001084        # ui_quickpanel_ext 存 KEY 磁贴句柄
RAM_TILE_BCAST = 0x20001088      # 存 BCAST 磁贴句柄（0x20001080 = KEY_ON）
UI_PAGE1_HANDLER = 0x0800FE78    # 页 1 处理器（UI tick 每 100ms 调用）
UI_TILES_EXT = 0x080F1200        # 建磁贴注入函数入口（v1.4）
PAGE1_TAIL_HOOK = 0x0800FF50     # 页 1 尾部挂载点（整页建完/已显示之后）
PAGE1_TAIL_BEFORE = "094906200870"   # ldr r1,[pc,#0x24]; movs r0,#6; strb r0,[r1]
RAM_PAGE_READY = 0x200004C2      # 原尾指令写的"页面就绪"标记
# ---- A2 fallback（不新建控件）----
FALLBACK_LONG_TICKS = 5          # 长按阈值：5 tick ≈ 500ms（UI tick 100ms）
FB_LOCK_COUNT = 0x20001084       # 锁磁贴：连续按下计数
FB_LOCK_TRIG = 0x20001085        # 锁磁贴：已触发标志（松开清零）
FB_TAB_COUNT = 0x20001086        # "设置"页签：连续按下计数
FB_TAB_TRIG = 0x20001087         # "设置"页签：已触发标志
RAM_UI_PAGE = 0x200003E7         # 当前页面 id（page0=主界面）
IS_WINDOW = 0x080303B8           # (handle)->1=仍在控件树中（原厂用法见 §19.1/§21）
WIDGET_SHOW = 0x0802DA22         # (widget, 1) = 显示控件（builder 里多处用于显示子控件）
# ---- 触摸链路（见 FEASIBILITY §22，指令级证据）----
TOUCH_COUNTER = 0x20000164       # 触摸事件计数（ISR 在状态位 0x20 时自增）
TOUCH_X = 0x20000168             # u16 X（IRQ18 ISR 写）
TOUCH_Y = 0x2000016A             # u16 Y
TOUCH_LAST_CNT = 0x2000108C      # 我们记录的上次计数（探测用）
TOUCH_LAST_TICK = 0x20001084     # 上次触发时的 UI tick（限速用，4B）
TOUCH_LAST_X = 0x20001088        # 上次 X（u16）
TOUCH_LAST_Y = 0x2000108A        # 上次 Y（u16）
UI_TICK_CNT = 0x20000310         # UI tick 计数（原厂）""
UI_PAGE1_ORIG_INS = "2de9fc41"   # 0x0800FE78 原首指令（必须保持原样）
UI_TICK_CALL_SITE = 0x08014C1E    # UI tick 里 `bl 0x0800FE78` 的调用点
UI_TICK_CALL_BEFORE = "fbf72bf9"  # 原 4 字节（bl 0x0800FE78）
# ---- page 0（主界面快捷面板）----
PAGE0_HANDLER = 0x080345B4        # 页 0 处理器
PAGE0_CALL_SITE = 0x08014C56      # UI tick 里 `bl 0x080345B4` 的调用点
PAGE0_CALL_BEFORE = "1ff0adfc"    # 原 4 字节
PAGE0_MENU_BUILDER = 0x080079E8   # FUN_080345B4 内的快捷面板构建调用
TILES_CALL_SITE = 0x080349A4      # `bl 0x080079E8`
TILES_CALL_BEFORE = "d3f720f8"    # 原 4 字节
WRAPPER0 = 0x080F1200             # page0 处理器 wrapper
WRAPPER1 = 0x080F1300             # 快捷面板 builder wrapper
TILES0_EXT = 0x080F1400           # page0 磁贴创建注入函数
OFF_POLL = 0x1000                # ui_tiles_poll 注入偏移
OFF_TILES = 0x1200               # 磁贴创建函数注入偏移（v1.4）
OFF_TRAMP = 0x1100               # UI tick 调用点 wrapper 注入偏移
RAM_LAST_KEY = 0x2000108C        # 上次按下状态（边沿检测）
RAM_LAST_BCAST = 0x2000108D
GROUP_TABLE = 0x20000B68        # 16 组 x 6B 设置表：[0]=造型灯开关(0=开,1=关) [1]=造型灯亮度 [4]=功率(低7位)
PROP_GROUP_ALL = 0x50
PROP_KEY_POWER = 0xD1           # 原厂 FUN_080089D0 用 (group,0xD1,[+4]&0x7f) 发功率
PROP_KEY_LAMP = 0xB1            # 原厂 FUN_0800BDDC 用 (group,0xB1,1/0) 开关造型灯
IS_PRESSED = 0x080303B8          # 未使用（Lead 更正：IsWindow 成员检查，非按下状态）
CONSUME_EVT = 0x08031FBA         # (handle, val, flags) -> 消费/复位控件状态
TILE_SPECS = (                  # (x, y, w, h, 标签偏移)：锁磁贴下方 44px 空隙
    (40, 288, 128, 44, OFF_TEXT_KEY),
    (176, 288, 128, 44, OFF_TEXT_BCAST),
)


def build_page1_tiles(va: int = UI_TILES_EXT, specs=TILE_SPECS,
                      mode: str = "full", icon: bool = False) -> bytes:
    r"""在页 1 处理器 FUN_0800FE78 尾部（0x0800FF50 被劫持处）创建两个磁贴。

    为什么在这里（v1.4 的关键改动）：
      * 此点整页已建完、动画/显示都已做完，**不在页面构建半途**；
      * 寄存器上下文明确：**r6 = 根窗口句柄**（FUN_0800FE78 里 `bl FUN_08010ACC(0,9,...)` → `mov r6,r0`）；
      * 上一版挂在 0x08008236（页面构建函数尾部），那里 r5/r6 用途混乱，易踩内存 → 实机重启。
    原尾指令（必须复现）：`ldr r1,=0x200004C2; movs r0,#6; strb r0,[r1]`

    mode: "full"=照抄原厂完整属性序列；"minimal"=只 create+设文本；"none"=不建磁贴
    icon: 是否创建图标子控件 FUN_0802553E（默认 False，降风险）
    """
    c = Code(va)
    c.push(0x170)                  # push {r4, r5, r6, lr}
    c.mov(5, 6)                    # r5 = parent（根窗口，来自 r6）
    # ---- 复现原尾指令：*0x200004C2 = 6 ----
    c.ldr_lit(1, RAM_PAGE_READY)
    c.movs(0, 6)
    c.strb_imm(0, 1)

    if mode == "none":
        c.pop(0x170)
        return c.finish()

    def setter(func, v1=None, v2=None, v1_call=False):
        if v1_call:
            c.bl(0x08027FE0)
            c.mov(1, 0)
        elif v1 is not None:
            c.movs(1, v1 & 0xFF)
        if v2 is not None:
            c.movs(2, v2 & 0xFF)
        c.mov(0, 4)
        c.bl(func)

    for (x, y, w, h, label_off) in specs:
        skip = "skip_%d" % (label_off,)
        c.cmp(5, 0)                # 父窗口为空 -> 跳过（安全保护）
        c.beq_label(skip)
        c.sub_sp(8)                # FUN_08006220(r5,1,x,y,w,h)
        c.ldr_lit(0, w)
        c.str_sp(0, 0)
        c.ldr_lit(0, h)
        c.str_sp(0, 1)
        c.mov(0, 5)
        c.movs(1, 1)
        c.ldr_lit(2, x)
        c.ldr_lit(3, y)
        c.bl(0x08006220)
        c.add_sp(8)
        c.mov(4, 0)                # r4 = tile
        c.cmp(4, 0)                # 创建失败 -> 跳过（防 NULL 解引用）
        c.beq_label(skip)
        if mode == "full":
            c.movs(0, 0x27)        # 背景色 = FUN_08027D90(0x27,0x27)
            c.movs(1, 0x27)
            c.movs(2, 0x27)
            c.bl(0x08027D90)
            c.mov(1, 0)
            c.movs(2, 0)
            c.mov(0, 4)
            c.bl(0x0803157C)
            setter(0x08031594, 0xFF, 0)
            setter(0x0803161C, 0xFF, 4)
            setter(0x08031604, v1_call=True, v2=4)
            setter(0x08031630, 2, 4)
            setter(0x08031626, 4, 4)
            setter(0x080315B6, 0, 0)
            setter(0x080315C0, 2, 0)
            setter(0x080315B6, 0, 4)
            setter(0x080315C0, 2, 4)
            setter(0x0803159E, v1_call=True, v2=4)
            setter(0x08031696, 0x6C, 0)
            if icon:
                c.bl(0x08027FE0)
                c.sub_sp(16)
                c.str_sp(0, 1)
                c.ldr_lit(1, 0x0806B090)
                c.str_sp(1, 0)
                c.ldr_lit(1, 0x08007E20)
                c.str_sp(1, 2)
                c.movs(1, 9)
                c.movs(2, 0)
                c.movs(3, 0)
                c.mov(0, 4)
                c.bl(0x0802553E)
                c.add_sp(16)
        # 文本
        c.mov(0, 4)
        c.ldr_lit(1, CAVE + label_off)
        c.movs(2, 0)
        c.bl(0x0802D97A)
        # 句柄存 RAM（poll 用）
        c.ldr_lit(1, RAM_TILE_KEY if label_off == OFF_TEXT_KEY else RAM_TILE_BCAST)
        c.str_imm5(4, 1)
        c.label(skip)
    c.pop(0x170)                   # pop {r4, r5, r6, pc}
    return c.finish()


def _hook_page1_tail(buf: bytearray, log: list[dict]):
    """页 1 尾部 6 字节 -> bl 0x080F1200 + nop。"""
    off = _off(PAGE1_TAIL_HOOK)
    got = bytes(buf[off:off + 6]).hex()
    if got != PAGE1_TAIL_BEFORE:
        raise ValueError("页 1 尾部原始字节不匹配：期望 %s 实际 %s"
                         % (PAGE1_TAIL_BEFORE, got))
    c = Code(PAGE1_TAIL_HOOK)
    c.bl(UI_TILES_EXT)
    after = c.finish()[:4] + b"\x00\xbf"
    buf[off:off + 6] = after
    log.append({"addr": PAGE1_TAIL_HOOK, "offset": off,
                "before": PAGE1_TAIL_BEFORE, "after": after.hex(), "len": 6,
                "desc": "页 1 尾部 -> bl 0x080F1200 + nop（整页建完后建 KEY/BCAST 磁贴）"})


def build_panel_text_blob() -> tuple[bytes, int]:
    """ASCII 标签（KEY / BCAST），返回 (blob, 起始偏移=0x500)。"""
    blob = b"KEY\x00\x00\x00\x00\x00"          # 0x500..0x50F
    blob += b"BCAST\x00\x00\x00"               # 0x510..0x51F
    blob += b"BROADCAST\x00"                   # 0x520..0x529
    return blob, OFF_TEXT_KEY


def _hook_panel_ext(buf: bytearray, log: list[dict]):
    """把 FUN_080079E8 尾部的 bl 0x08027fe0 改成 bl UI_PANEL_EXT。"""
    off = _off(MAINMENU_HOOK_SITE)
    got = bytes(buf[off:off + 4]).hex()
    if got != MAINMENU_HOOK_BEFORE:
        raise ValueError("磁贴挂载点 %#x 原始字节不匹配：期望 %s 实际 %s"
                         % (MAINMENU_HOOK_SITE, MAINMENU_HOOK_BEFORE, got))
    c = Code(MAINMENU_HOOK_SITE)
    c.bl(UI_PANEL_EXT)
    after = c.finish()[:4]
    buf[off:off + 4] = after
    log.append({"addr": MAINMENU_HOOK_SITE, "offset": off,
                "before": MAINMENU_HOOK_BEFORE, "after": after.hex(), "len": 4,
                "desc": "快捷面板：bl 0x08027fe0 -> bl 0x080F0800（加 KEY/BROADCAST 磁贴）"})


def build_touch_probe_poll(va: int = CAVE + OFF_POLL) -> bytes:
    r"""触摸探测：触摸计数变化 -> ui_key_toggle()（并限速 5 个 UI tick ≈500ms）。

    只读 0x20000164/0x20000168/0x2000016A 与 UI tick 计数；不建控件、不写原厂数据。
    """
    c = Code(va)
    c.push(0x170)
    # 当前 touch 计数
    c.ldr_lit(4, TOUCH_COUNTER)
    c.ldr_imm5(5, 4)                    # r5 = *TOUCH_COUNTER (u32)
    c.ldr_lit(1, TOUCH_LAST_CNT)
    c.ldr_imm5(2, 1)                    # r2 = last
    c.cmp_reg(5, 2)
    c.beq_label("probe_end")            # 没变化 -> 返回
    c.mov(0, 5)
    c.str_imm5(0, 1)                    # last = 当前
    # 限速：距上次触发 >= 5 个 UI tick
    c.ldr_lit(1, UI_TICK_CNT)
    c.ldr_imm5(2, 1)                    # r2 = 当前 tick
    c.ldr_lit(1, TOUCH_LAST_TICK)
    c.ldr_imm5(6, 1)                    # r6 = 上次 tick
    c.mov(3, 2)
    c.subs(3, 2, 6)                     # r3 = cur - last
    c.cmp(3, 5)
    c.bcc_label("probe_end")            # 不足 5 tick -> 返回
    c.mov(0, 2)
    c.str_imm5(0, 1)                    # 记录本次 tick
    # 有触摸坐标才动作（两个都为 0 视为噪声）
    c.ldr_lit(1, TOUCH_X)
    c.ldrh_imm(2, 1, 0)                 # r2 = X
    c.ldr_lit(1, TOUCH_Y)
    c.ldrh_imm(3, 1, 0)                 # r3 = Y
    c.mov(0, 2)
    c.orrs(0, 3)
    c.cmp(0, 0)                         # X|Y == 0 -> 无有效坐标
    c.beq_label("probe_end")
    # 坐标变化判定（与上次比较）
    c.ldr_lit(1, TOUCH_LAST_X)
    c.ldrh_imm(6, 1, 0)                 # r6 = last X
    c.cmp_reg(2, 6)
    c.bne_label("probe_fire")
    c.ldr_lit(1, TOUCH_LAST_Y)
    c.ldrh_imm(6, 1, 0)                 # r6 = last Y
    c.cmp_reg(3, 6)
    c.beq_label("probe_end")            # X、Y 都没变 -> 不是新手势
    c.label("probe_fire")
    # 记录新坐标
    c.ldr_lit(1, TOUCH_LAST_X)
    c.strh_imm(2, 1, 0)
    c.ldr_lit(1, TOUCH_LAST_Y)
    c.strh_imm(3, 1, 0)
    c.bl(CAVE + OFF_KEY_TOGGLE)
    c.label("probe_end")
    c.pop(0x170)
    return c.finish()


def build_fallback_poll(va: int = CAVE + OFF_POLL) -> bytes:
    r"""A2-p0b poll：不建控件，只观察原厂控件；**先做控件树校验与页面门**。

    防线（针对实机"短按/长按频闪页签重启"）：
      1) 页面门：`*(u8*)0x200003E7 != 0`（不是主界面）→ 清零状态并直接返回，
         避免页面切换中途动作；
      2) 控件树校验：`0x080303B8(handle) == 0`（句柄已不在当前控件树）→ 跳过并清零状态，
         不再 `ldrh [h,#0x28]`，避免野指针；
      3) 句柄为 0 → 跳过并清零。

    行为：锁磁贴长按 >=5 tick -> ui_key_toggle；设置页签长按 >=5 tick -> ui_broadcast_allch。
    """
    c = Code(va)
    c.push(0x170)                       # push {r4, r5, r6, lr}
    c.ldr_lit(4, HANDLE_TABLE)

    def reset_state(count_va, trig_va):
        c.ldr_lit(1, count_va)
        c.movs(2, 0)
        c.strb_imm(2, 1)
        c.ldr_lit(1, trig_va)
        c.movs(2, 0)
        c.strb_imm(2, 1)

    # ---- 页面门 ----
    c.ldr_lit(1, RAM_UI_PAGE)
    c.ldrb_imm(0, 1)
    c.cmp(0, 0)
    c.beq_label("page_ok")
    reset_state(FB_LOCK_COUNT, FB_LOCK_TRIG)
    reset_state(FB_TAB_COUNT, FB_TAB_TRIG)
    c.b_label("poll_end")
    c.label("page_ok")

    for (idx, count_va, trig_va, action, tag) in (
            (5, FB_LOCK_COUNT, FB_LOCK_TRIG, CAVE + OFF_KEY_TOGGLE, "lock"),
            (7, FB_TAB_COUNT, FB_TAB_TRIG, CAVE + OFF_BROADCAST_ALLCH, "tab")):
        rel, bad, skip = "rel_" + tag, "bad_" + tag, "skip_" + tag
        c.ldr_imm5(5, 4, idx)           # r5 = 句柄表[idx]
        c.cmp(5, 0)
        c.beq_label(bad)
        c.mov(0, 5)                     # 控件树校验：0x080303B8(h)
        c.bl(IS_WINDOW)
        c.cmp(0, 0)
        c.beq_label(bad)
        c.ldrh_imm(1, 5, 0x28)          # 按下位
        c.movs(2, 8)
        c.ands(1, 2)
        c.cmp(1, 0)
        c.beq_label(rel)
        # 按下：已触发则跳过；否则计数
        c.ldr_lit(1, trig_va)
        c.ldrb_imm(2, 1)
        c.cmp(2, 0)
        c.bne_label(skip)
        c.ldr_lit(1, count_va)
        c.ldrb_imm(2, 1)
        c.adds(2, 1)
        c.strb_imm(2, 1)
        c.cmp(2, FALLBACK_LONG_TICKS)
        c.bcc_label(skip)
        c.ldr_lit(1, trig_va)
        c.movs(2, 1)
        c.strb_imm(2, 1)
        c.bl(action)
        c.b_label(skip)
        # 松开：清零
        c.label(rel)
        reset_state(count_va, trig_va)
        c.b_label(skip)
        # 句柄无效/为 0：清零
        c.label(bad)
        reset_state(count_va, trig_va)
        c.label(skip)
    c.label("poll_end")
    c.pop(0x170)
    return c.finish()


def build_tiles_poll(va: int = CAVE + OFF_POLL) -> bytes:
    """ui_tiles_poll()：每 100ms 检查两个新磁贴的"按下"位，做边沿触发。

    按下判定 = (*(u16*)(widget + 0x28) & 8) != 0（与固件 0x0802FCB6(widget,8) 等价）。
    边沿检测用 last-state 字节：KEY 0x2000108C / BCAST 0x2000108D，避免按住重复触发。

        h = *(u32*)TILE;      if (!h) goto end;
        p = (*(u16*)(h+0x28) & 8) != 0;
        if (p) { if (*(u8*)LAST == 0) { *(u8*)LAST = 1; ACTION(); } }
        else   { *(u8*)LAST = 0; }
    """
    c = Code(va)
    c.push(0x170)                       # push {r4, r5, r6, lr}（16B，8 字节对齐）
    c.movs(6, 8)                        # r6 = mask 8（"按下/激活"位）
    for (tile_addr, last_addr, action, tag) in (
            (RAM_TILE_KEY, RAM_LAST_KEY, CAVE + OFF_KEY_TOGGLE, "1"),
            (RAM_TILE_BCAST, RAM_LAST_BCAST, CAVE + OFF_BROADCAST_ALLCH, "2")):
        end = "end" + tag
        np = "np" + tag
        c.ldr_lit(4, tile_addr)         # r4 = &tile 句柄槽
        c.ldr_imm5(0, 4)                # r0 = 句柄
        c.cmp(0, 0)
        c.beq_label(end)
        c.ldrh_imm(1, 0, 0x28)          # r1 = *(u16*)(h + 0x28)
        c.ands(1, 6)                    # r1 &= 8
        c.cmp(1, 0)
        c.beq_label(np)
        # 按下沿：只有上次不是按下才触发
        c.ldr_lit(5, last_addr)         # r5 = &last
        c.ldrb_imm(2, 5)
        c.cmp(2, 0)
        c.bne_label(end)
        c.movs(2, 1)
        c.strb_imm(2, 5)                # last = 1
        c.bl(action)                    # 执行动作
        c.b_label(end)
        c.label(np)
        c.ldr_lit(5, last_addr)
        c.movs(2, 0)
        c.strb_imm(2, 5)                # last = 0（松开）
        c.label(end)
    c.pop(0x170)                        # pop {r4, r5, r6, pc}
    return c.finish()


def build_wrapper0(va: int = WRAPPER0) -> bytes:
    r"""page 0 调用点 wrapper（14B）：
        push {r4, lr}
        mov  r4, r0          ; 保存 param（page0 处理器 r0=0）
        bl   ui_tiles_poll
        mov  r0, r4
        bl   0x080345B4      ; 原厂页 0 处理器（完整执行）
        pop  {r4, pc}
    """
    c = Code(va)
    c.push(0x110)                  # push {r4, lr}
    c.mov(4, 0)
    c.bl(CAVE + OFF_POLL)
    c.mov(0, 4)
    c.bl(PAGE0_HANDLER)
    c.pop(0x110)
    return c.finish()


def build_wrapper1(va: int = WRAPPER1) -> bytes:
    r"""快捷面板 builder 调用点 wrapper（12B）：
        push {r4, lr}        ; 保留 r4（调用者可能使用），并保存返回地址
        bl   0x080079E8      ; 原厂快捷面板构建（r0 入参由调用点 0x080349A2 设好）
        bl   page0_tiles     ; 页面建完后追加两个磁贴（父窗口从 0x20042740 取）
        pop  {r4, pc}
    """
    c = Code(va)
    c.push(0x110)
    c.bl(PAGE0_MENU_BUILDER)
    c.bl(TILES0_EXT)
    c.pop(0x110)
    return c.finish()


def build_page0_tiles(va: int = TILES0_EXT, specs=TILE_SPECS,
                      mode: str = "full") -> bytes:
    r"""page 0 磁贴创建：父窗口 = *(u32*)0x20042740（builder 尾部写的 [0]=根窗口）。

    与 page1 版相同的原厂属性序列（13 个 setter + 文本），**不建图标子控件**；
    父窗口为 0 或创建返回 0 时跳过；句柄存入 0x20001084/88。
    """
    c = Code(va)
    c.push(0x170)                      # push {r4, r5, r6, lr}
    if mode == "none":
        c.pop(0x170)
        return c.finish()
    # ---- 页面门：只在该页为主界面(0)时才建磁贴 ----
    c.ldr_lit(1, RAM_UI_PAGE)
    c.ldrb_imm(0, 1)
    c.cmp(0, 0)
    c.beq_label("pg_ok")
    c.b_label("tiles_end")
    c.label("pg_ok")
    c.ldr_lit(1, HANDLE_TABLE)         # r1 = &句柄表
    c.ldr_imm5(5, 1)                   # r5 = 句柄表[0] = 根窗口

    def setter(func, v1=None, v2=None, v1_call=False):
        if v1_call:
            c.bl(0x08027FE0)
            c.mov(1, 0)
        elif v1 is not None:
            c.movs(1, v1 & 0xFF)
        if v2 is not None:
            c.movs(2, v2 & 0xFF)
        c.mov(0, 4)
        c.bl(func)

    for (x, y, w, h, label_off) in specs:
        skip = "skip0_%d" % (label_off,)
        c.cmp(5, 0)                    # 父窗口 == 0 -> 跳过
        c.beq_label(skip)
        c.sub_sp(8)
        c.ldr_lit(0, w)
        c.str_sp(0, 0)
        c.ldr_lit(0, h)
        c.str_sp(0, 1)
        c.mov(0, 5)
        c.movs(1, 1)
        c.ldr_lit(2, x)
        c.ldr_lit(3, y)
        c.bl(0x08006220)
        c.add_sp(8)
        c.mov(4, 0)                    # r4 = tile
        c.cmp(4, 0)                    # 创建失败 -> 跳过
        c.beq_label(skip)
        if mode == "full":
            c.movs(0, 0x27)            # 背景色
            c.movs(1, 0x27)
            c.movs(2, 0x27)
            c.bl(0x08027D90)
            c.mov(1, 0)
            c.movs(2, 0)
            c.mov(0, 4)
            c.bl(0x0803157C)
            setter(0x08031594, 0xFF, 0)
            setter(0x0803161C, 0xFF, 4)
            setter(0x08031604, v1_call=True, v2=4)
            setter(0x08031630, 2, 4)
            setter(0x08031626, 4, 4)
            setter(0x080315B6, 0, 0)
            setter(0x080315C0, 2, 0)
            setter(0x080315B6, 0, 4)
            setter(0x080315C0, 2, 4)
            setter(0x0803159E, v1_call=True, v2=4)
            setter(0x08031696, 0x6C, 0)
        c.mov(0, 4)                    # 文本
        c.ldr_lit(1, CAVE + label_off)
        c.movs(2, 0)
        c.bl(0x0802D97A)
        c.ldr_lit(1, RAM_TILE_KEY if label_off == OFF_TEXT_KEY else RAM_TILE_BCAST)
        c.str_imm5(4, 1)
        # 显式显示（原厂 builder 用 FUN_0802DA22(w,1) 显示子控件，见 FEASIBILITY §21）
        c.mov(0, 4)
        c.movs(1, 1)
        c.bl(WIDGET_SHOW)
        c.label(skip)
    c.label("tiles_end")
    c.pop(0x170)
    return c.finish()


def build_ui_tick_wrapper(va: int = CAVE + OFF_TRAMP) -> bytes:
    r"""UI tick 调用点 wrapper（12 字节）。

        push {r4, lr}          ; r4 是 callee-saved；同时保存"返回 UI tick"的地址
        bl   ui_tiles_poll     ; 每 100ms 轮询两个磁贴
        bl   0x0800FE78        ; 原厂页 1 处理器（完整执行；它自己保存/恢复 LR）
        pop  {r4, pc}          ; 返回 UI tick

    为什么不能用旧方案：旧跳板用 `pop {r0-r3, pc}`，Thumb 的 POP bit8 是 PC，
    等于把"进入时的 LR"弹进 PC —— 直接返回调用者，页 1 处理器从未执行。
    """
    c = Code(va)
    c.push(0x110)                  # push {r4, lr}
    c.bl(CAVE + OFF_POLL)          # bl ui_tiles_poll
    c.bl(UI_PAGE1_HANDLER)         # bl 0x0800FE78（原厂页面构建/刷新）
    c.pop(0x110)                   # pop {r4, pc}
    return c.finish()


def _hook_page0_call(buf: bytearray, log: list[dict]):
    """UI tick 里 `bl 0x080345B4`（0x08014C56）-> bl wrapper0。"""
    off = _off(PAGE0_CALL_SITE)
    got = bytes(buf[off:off + 4]).hex()
    if got != PAGE0_CALL_BEFORE:
        raise ValueError("page0 调用点 %#x 原始字节不匹配：期望 %s 实际 %s"
                         % (PAGE0_CALL_SITE, PAGE0_CALL_BEFORE, got))
    c = Code(PAGE0_CALL_SITE)
    c.bl(WRAPPER0)
    after = c.finish()[:4]
    buf[off:off + 4] = after
    log.append({"addr": PAGE0_CALL_SITE, "offset": off,
                "before": PAGE0_CALL_BEFORE, "after": after.hex(), "len": 4,
                "desc": "UI tick: bl 0x080345B4 -> bl wrapper0(0x080F1100)"})


def _hook_tiles_call(buf: bytearray, log: list[dict]):
    """FUN_080345B4 内 `bl 0x080079E8`（0x080349A4）-> bl wrapper1。"""
    off = _off(TILES_CALL_SITE)
    got = bytes(buf[off:off + 4]).hex()
    if got != TILES_CALL_BEFORE:
        raise ValueError("builder 调用点 %#x 原始字节不匹配：期望 %s 实际 %s"
                         % (TILES_CALL_SITE, TILES_CALL_BEFORE, got))
    c = Code(TILES_CALL_SITE)
    c.bl(WRAPPER1)
    after = c.finish()[:4]
    buf[off:off + 4] = after
    log.append({"addr": TILES_CALL_SITE, "offset": off,
                "before": TILES_CALL_BEFORE, "after": after.hex(), "len": 4,
                "desc": "FUN_080345B4: bl 0x080079E8 -> bl wrapper1(0x080F1200)"
                        "（build 完快捷面板后追加磁贴）"})


def _hook_ui_tick_call(buf: bytearray, log: list[dict]):
    """把 UI tick 里的 `bl 0x0800FE78`（0x08014C1E）改成 `bl wrapper`。

    同时**不动** 0x0800FE78 本身（保持原厂 2de9fc41）。
    """
    # 先确认 0x0800FE78 仍是原厂字节（绝不能再被改写）
    fn = bytes(buf[_off(UI_PAGE1_HANDLER):_off(UI_PAGE1_HANDLER) + 4]).hex()
    if fn != UI_PAGE1_ORIG_INS:
        raise ValueError("0x0800FE78 首指令不是原厂 %s（实际 %s）"
                         % (UI_PAGE1_ORIG_INS, fn))
    off = _off(UI_TICK_CALL_SITE)
    got = bytes(buf[off:off + 4]).hex()
    if got != UI_TICK_CALL_BEFORE:
        raise ValueError("UI tick 调用点 %#x 原始字节不匹配：期望 %s 实际 %s"
                         % (UI_TICK_CALL_SITE, UI_TICK_CALL_BEFORE, got))
    c = Code(UI_TICK_CALL_SITE)
    c.bl(CAVE + OFF_TRAMP)
    after = c.finish()[:4]
    buf[off:off + 4] = after
    log.append({"addr": UI_TICK_CALL_SITE, "offset": off,
                "before": UI_TICK_CALL_BEFORE, "after": after.hex(), "len": 4,
                "desc": "UI tick: bl 0x0800FE78 -> bl wrapper(0x080F1100)"
                        "（页 1 处理器仍完整执行；0x0800FE78 保持原厂）"})


# ---------------------------------------------------------------------------
# 字模 / 字符串
# ---------------------------------------------------------------------------
def load_glyph(cp: int) -> bytes:
    """读 Lead 的 24x24 hex（每行 u32，MSB 左，1=亮），返回 96 字节小端。"""
    name = GLYPH_CHARS[cp]
    path = os.path.join(GLYPHS24, name + ".hex")
    rows = []
    with open(path, encoding="utf-8-sig") as f:     # Lead 的文件带 BOM
        for line in f:
            line = line.strip().lstrip("\ufeff")
            if line:
                rows.append(int(line, 16))
    if len(rows) != GLYPH_ROWS:
        raise ValueError("%s 行数 %d != %d" % (path, len(rows), GLYPH_ROWS))
    return b"".join(struct.pack("<I", r) for r in rows)


def build_glyph_blob() -> bytes:
    return b"".join(load_glyph(cp) for cp in sorted(GLYPH_CHARS))


def build_strings_blob() -> bytes:
    out = bytearray()
    for name in ("key", "broadcast", "allch", "allid", "on", "off"):
        out += LABELS[name].encode("utf-8") + b"\x00"
    return bytes(out)


# ---------------------------------------------------------------------------
# B. UI 布局接线（默认关闭，未端到端验证）
# ---------------------------------------------------------------------------
# FUN_080079E8 底部页签两个按钮的创建调用与参数：
#   按钮1 @0x08007E8C：h=movs r0,#0x5c @0x08007E7A; w=movs r1,#0xa8 @0x08007E7C
#                     y=movw r3,#0x155 @0x08007E82; x=movs r2,#0xb @0x08007E86
#   按钮2 @0x0800808A：同样 h/w/y；x=movs r2,#0xbb @0x08008084
# 三按钮布局：w 0xA8->0x6E；x2 0xBB->0x81；第三个按钮需注入一次 bl 0x080F0800
# （要接管按钮2的创建块，方案见 FEASIBILITY.md §5.3）。
MENU_LAYOUT_PATCH = [
    (0x08007E7C, "a821", "6e21", "底部页签宽 0xA8 -> 0x6E（三按钮布局）"),
    (0x0800807A, "a821", "6e21", "底部页签宽 0xA8 -> 0x6E（三按钮布局）"),
    (0x08008084, "bb22", "8122", "第二个页签 x 0xBB -> 0x81（给第三个腾位置）"),
]


def _apply_layout_patch(buf: bytearray, log: list[dict]):
    for addr, before, after, desc in MENU_LAYOUT_PATCH:
        off = addr - BASE
        got = bytes(buf[off:off + len(before) // 2])
        if got.hex() != before:
            raise ValueError("布局补丁点 %#x 原始字节不匹配：期望 %s 实际 %s"
                             % (addr, before, got.hex()))
        buf[off:off + len(after) // 2] = bytes.fromhex(after)
        log.append({"addr": addr, "offset": off, "before": before, "after": after,
                    "len": len(after) // 2,
                    "desc": "UI 布局（未端到端验证，见 FEASIBILITY §6）"})


# ---------------------------------------------------------------------------
# apply_ui / verify_ui
# ---------------------------------------------------------------------------
UI_DEFAULTS = {
    "key": 0x6B21,
    "key_flag": DEFAULT_KEY_FLAG,
    "enable_menu_patch": False,     # 未端到端验证，默认关闭
    "broadcast_group": 0x50,
    "broadcast_group_idx": 0,      # 取哪一组的"当前功率/造型灯"（0=A 组）
    "broadcast_per_group": False,  # True = A–E 逐组发（10..14），False = ALL(0x50)
    "three_tab_layout": False,     # True = 把底部 2 页签改窄腾位置(视觉实验项，默认关)
    "tile_mode": "full",           # full | minimal | none（安全阶梯）
    "tile_icon": False,            # True = 额外建图标子控件（默认关，降风险）
    "entry_mode": "tiles",         # "tiles"=新建磁贴入口；"fallback"=复用现有控件(A2)
    "broadcast_power": -1,         # -1 = 用当前设置；>=0 用固定功率值
    "power_key": 0xB1,
    "power_val": 0x00,
    "model_key": 0xB2,
    "model_on": -1,                # -1 = 用当前造型灯设置；>=0 用固定值
    "trigger": 0x09,
    "nchan": 32,
    "allid_step": 1,               # 全 ID 扫描步长：1 = 0..99 全扫（3200 帧，慢）
}


def _off(addr: int) -> int:
    return addr - BASE


def _put(buf: bytearray, off: int, blob: bytes, desc: str, log: list[dict]):
    addr = BASE + off
    if off < 0 or off + len(blob) > IMAGE_SIZE:
        raise ValueError("注入越界: %#x len %d" % (addr, len(blob)))
    before = bytes(buf[off:off + len(blob)])
    if not set(before) <= {0x00, 0xFF}:
        raise ValueError("注入区 %#x 不是空闲区（前 8B %s）" % (addr, before[:8].hex(" ")))
    buf[off:off + len(blob)] = blob
    log.append({
        "addr": addr, "offset": off,
        "before": before[:16].hex(" ") + ("..." if len(before) > 16 else ""),
        "after": blob[:16].hex(" ") + ("..." if len(blob) > 16 else ""),
        "len": len(blob), "desc": desc,
    })


def apply_ui(data: bytes, opts: dict | None = None) -> tuple[bytes, list[dict]]:
    """写入 UI 原型补丁（注入层），返回 (patched, log)。镜像长度不变。"""
    opts = dict(UI_DEFAULTS, **(opts or {}))
    if len(data) != IMAGE_SIZE:
        raise ValueError("X3-C v1.38 固件长度应为 %d，实际 %d（版本不符）"
                         % (IMAGE_SIZE, len(data)))
    key = opts["key"]
    if not (0 <= key <= 0xFFFF):
        raise ValueError("key 必须是 16 位")
    flag = opts["key_flag"]

    buf = bytearray(data)
    log: list[dict] = []
    blobs = [
        (OFF_KEY_TOGGLE, build_key_toggle(CAVE + OFF_KEY_TOGGLE, flag),
         "ui_key_toggle(): 翻转 KEY_ON 标志 %#x（fw-x3c 双发读这个字节）" % flag),
        (OFF_BROADCAST_ALLCH,
         build_broadcast_allch(CAVE + OFF_BROADCAST_ALLCH, key,
                               opts["broadcast_group"], opts["power_key"],
                               opts["broadcast_power"], opts["model_key"],
                               opts["model_on"], opts["trigger"], opts["nchan"],
                               opts["broadcast_group_idx"],
                               bool(opts["broadcast_per_group"])),
         "ui_broadcast_allch(): %d 频道广播（SYNC=KEY）" % opts["nchan"]),
        (OFF_BROADCAST_ALLID, build_broadcast_allid(CAVE + OFF_BROADCAST_ALLID, key,
                                                   opts["nchan"], opts["allid_step"]),
         "ui_broadcast_allid(): 全频道 + 全 ID 扫描（慢）"),
        (OFF_GLYPHS, build_glyph_blob(),
         "自绘字模 24x24：密/钥/广/播/全（%d 字节）" % len(build_glyph_blob())),
        (OFF_STRINGS, build_strings_blob(),
         "UTF-8 标签：密钥 / 广播 / 全频道 / 全ID / KEY ON / KEY OFF"),
    ]
    for off, blob, desc in blobs:
        _put(buf, _off(CAVE) + off, blob, desc, log)
    if opts["enable_menu_patch"]:
        emode = opts.get("entry_mode", "tiles")
        if emode == "touch_p0":
            _put(buf, _off(CAVE + OFF_POLL), build_touch_probe_poll(),
                 "触摸探测 poll：0x20000164 计数变化 + 坐标变化 -> ui_key_toggle", log)
            _put(buf, _off(WRAPPER0), build_wrapper0(),
                 "page0 调用点 wrapper：push{r4,lr}; mov r4,r0; bl poll; mov r0,r4; "
                 "bl 0x080345B4; pop{r4,pc}", log)
            _hook_page0_call(buf, log)
        elif emode in ("fallback_p0", "tiles_p0"):
            # ---- page 0（主界面快捷面板）----
            _put(buf, _off(CAVE + OFF_POLL), build_fallback_poll(),
                 "page0 fallback poll：读句柄表 0x20042740（[5]锁/[7]设置页签），"
                 "长按 >=%d tick -> ui_key_toggle / ui_broadcast_allch" % FALLBACK_LONG_TICKS,
                 log)
            _put(buf, _off(WRAPPER0), build_wrapper0(),
                 "page0 调用点 wrapper：push{r4,lr}; mov r4,r0; bl poll; mov r0,r4; "
                 "bl 0x080345B4; pop{r4,pc}", log)
            _hook_page0_call(buf, log)
            if emode == "tiles_p0":
                _put(buf, _off(WRAPPER1), build_wrapper1(),
                     "builder 调用点 wrapper：push{r4,lr}; bl 0x080079E8; bl tiles; pop{r4,pc}",
                     log)
                _put(buf, _off(TILES0_EXT),
                     build_page0_tiles(TILES0_EXT, TILE_SPECS, opts["tile_mode"]),
                     "page0 磁贴创建：父窗口=*(u32*)0x20042740（判 0 跳过），"
                     "原厂 13 个 setter + 文本，无图标子控件", log)
                _hook_tiles_call(buf, log)
        elif emode == "fallback":
            # ---- A2：完全不新建控件，只装"观察 + 长按"轮询 ----
            _put(buf, _off(CAVE + OFF_POLL), build_fallback_poll(),
                 "A2 fallback poll：读句柄表 0x20042740（[5]锁/[7]设置页签），"
                 "长按 >=%d tick -> ui_key_toggle / ui_broadcast_allch（不建控件/不写 LCD）"
                 % FALLBACK_LONG_TICKS, log)
            _put(buf, _off(CAVE + OFF_TRAMP), build_ui_tick_wrapper(),
                 "UI tick wrapper：push{r4,lr}; bl poll; bl 0x0800FE78; pop{r4,pc}", log)
            _hook_ui_tick_call(buf, log)
        else:
            # 1) 新增磁贴的注入函数 + ASCII 标签
            text_blob, text_off = build_panel_text_blob()
            _put(buf, _off(CAVE) + text_off, text_blob,
                 "快捷面板标签 ASCII: KEY / BCAST / BROADCAST", log)
            _put(buf, _off(CAVE + OFF_TILES),
                 build_page1_tiles(UI_TILES_EXT, TILE_SPECS,
                                   opts["tile_mode"], bool(opts["tile_icon"])),
                 "build_page1_tiles(): 建 KEY / BROADCAST 两个磁贴（页 1 尾部挂载）", log)
            # 2) 挂载点 + 底部页签三按钮布局
            _hook_page1_tail(buf, log)
            if opts["three_tab_layout"]:
                _apply_layout_patch(buf, log)
            # 3) 点击动作接线：轮询函数 + UI tick wrapper
            _put(buf, _off(CAVE + OFF_POLL), build_tiles_poll(),
                 "ui_tiles_poll(): 轮询磁贴按下 -> ui_key_toggle / ui_broadcast_allch", log)
            _put(buf, _off(CAVE + OFF_TRAMP), build_ui_tick_wrapper(),
                 "UI tick wrapper：push{r4,lr}; bl poll; bl 0x0800FE78; pop{r4,pc}", log)
            _hook_ui_tick_call(buf, log)

    patched = bytes(buf)
    assert len(patched) == len(data)
    return patched, log


# ---------------------------------------------------------------------------
# 验证
# ---------------------------------------------------------------------------
def verify_ui(patched: bytes, opts: dict | None = None) -> list[tuple[bool, str]]:
    """验证注入层：静态字节 + Unicorn 调用级执行（KEY 开关 / 32 频道广播）。"""
    opts = dict(UI_DEFAULTS, **(opts or {}))
    key = opts["key"]
    flag = opts["key_flag"]
    res: list[tuple[bool, str]] = []

    res.append((len(patched) == IMAGE_SIZE,
                "镜像长度 %d 字节（应 %d，原位补丁）" % (len(patched), IMAGE_SIZE)))

    with open(IMAGE, "rb") as f:
        orig = f.read()
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    allowed = set(range(_off(CAVE), _off(CAVE_END)))
    if opts["enable_menu_patch"]:
        emode = opts.get("entry_mode", "tiles")
        if emode in ("fallback_p0", "tiles_p0", "touch_p0"):
            allowed.update(range(_off(PAGE0_CALL_SITE), _off(PAGE0_CALL_SITE) + 4))
            if emode == "tiles_p0":
                allowed.update(range(_off(TILES_CALL_SITE), _off(TILES_CALL_SITE) + 4))
        else:
            if emode != "fallback":
                allowed.update(range(_off(PAGE1_TAIL_HOOK), _off(PAGE1_TAIL_HOOK) + 6))
            allowed.update(range(_off(UI_TICK_CALL_SITE), _off(UI_TICK_CALL_SITE) + 4))
        if opts.get("three_tab_layout", False):
            for addr, before, _after, _desc in MENU_LAYOUT_PATCH:
                allowed.update(range(_off(addr), _off(addr) + len(before) // 2))
    outside = [d for d in diffs if d not in allowed]
    res.append((len(outside) == 0,
                "与原厂差异 %d 字节；注入区内 %d，区外（原厂代码）%d（必须 0）"
                % (len(diffs), len(diffs) - len(outside), len(outside))))
    if diffs and not opts["enable_menu_patch"]:
        res.append((min(diffs) >= _off(CAVE),
                    "最小差异偏移 %#x（应 >= 注入区起点 %#x）"
                    % (min(diffs), _off(CAVE))))

    try:
        import capstone
        md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
        for off, name in ((OFF_KEY_TOGGLE, "ui_key_toggle"),
                          (OFF_BROADCAST_ALLCH, "ui_broadcast_allch"),
                          (OFF_BROADCAST_ALLID, "ui_broadcast_allid")):
            addr = CAVE + off
            insns = list(md.disasm(patched[_off(addr):_off(addr) + 96], addr))
            txt = "; ".join("%s %s" % (i.mnemonic, i.op_str) for i in insns[:6])
            res.append((len(insns) >= 4, "%s @0x%08X: %s" % (name, addr, txt)))
    except Exception as exc:      # noqa: BLE001
        res.append((True, "警告: capstone 不可用，跳过反汇编检查（%s）" % exc))

    try:
        ok, msg = _uc_test_key_toggle(patched, flag)
    except Exception as exc:      # noqa: BLE001
        ok, msg = False, "Unicorn KEY 开关测试异常: %s" % exc
    res.append((ok, msg))

    try:
        ok, msg = _uc_test_broadcast(patched, key, opts)
    except Exception as exc:      # noqa: BLE001
        ok, msg = False, "Unicorn 广播测试异常: %s" % exc
    res.append((ok, msg))

    glyph_off = _off(CAVE + OFF_GLYPHS)
    g = patched[glyph_off:glyph_off + GLYPH_BYTES]
    res.append((any(g), "字模区 0x%08X 已写入（%d 字节/字，非零字节 %d）"
                % (CAVE + OFF_GLYPHS, GLYPH_BYTES, sum(1 for b in g if b))))
    str_off = _off(CAVE + OFF_STRINGS)
    labels = patched[str_off:str_off + 128].split(b"\x00")
    res.append((labels[0].decode("utf-8", "replace") == LABELS["key"],
                "标签区 UTF-8 首项 = %r（应为 %r）"
                % (labels[0].decode("utf-8", "replace"), LABELS["key"])))

    if opts["enable_menu_patch"]:
        res.extend(_verify_menu_patch(patched, orig, opts))
    return res


def _verify_fallback(patched: bytes, opts: dict):
    """A2 fallback 的静态 + 桩验证（不建控件、长按触发）。"""
    import capstone
    out: list[tuple[bool, str]] = []
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)

    fn_bytes = patched[_off(UI_PAGE1_HANDLER):_off(UI_PAGE1_HANDLER) + 4]
    out.append((fn_bytes.hex() == UI_PAGE1_ORIG_INS,
                "0x0800FE78 保持原厂首指令 %s（实际 %s）"
                % (UI_PAGE1_ORIG_INS, fn_bytes.hex())))

    got = patched[_off(UI_TICK_CALL_SITE):_off(UI_TICK_CALL_SITE) + 4]
    insn = list(md.disasm(got, UI_TICK_CALL_SITE))[0]
    out.append((insn.mnemonic == "bl" and insn.op_str == "#0x%x" % (CAVE + OFF_TRAMP),
                "UI tick 调用点 0x%08X: %s %s（应为 bl wrapper 0x%X）"
                % (UI_TICK_CALL_SITE, insn.mnemonic, insn.op_str, CAVE + OFF_TRAMP)))

    tramp = list(md.disasm(patched[_off(CAVE + OFF_TRAMP):_off(CAVE + OFF_TRAMP) + 20],
                           CAVE + OFF_TRAMP))
    ttxt = ["%s %s" % (i.mnemonic, i.op_str) for i in tramp]
    bl_targets = [int(i.op_str[1:], 16) for i in tramp
                  if i.mnemonic == "bl" and i.op_str.startswith("#")]
    wok = (len(tramp) >= 4 and tramp[0].mnemonic == "push"
           and bl_targets == [CAVE + OFF_POLL, UI_PAGE1_HANDLER]
           and tramp[-1].mnemonic.startswith("pop"))
    out.append((wok, "wrapper 反汇编: %s" % "; ".join(ttxt)))

    # 页 1 尾部挂载点必须没被动过（A2 不建控件）
    tail = bytes(patched[_off(PAGE1_TAIL_HOOK):_off(PAGE1_TAIL_HOOK) + 6]).hex()
    out.append((tail == PAGE1_TAIL_BEFORE,
                "页 1 尾部 0x%08X 保持原厂 %s（实际 %s）"
                % (PAGE1_TAIL_HOOK, PAGE1_TAIL_BEFORE, tail)))

    # poll 反汇编：句柄表 + 长按计数 + 动作 + 无建控件调用
    code = patched[_off(CAVE + OFF_POLL):_off(CAVE + OFF_POLL) + 220]
    insns = list(md.disasm(code, CAVE + OFF_POLL))
    ptext = "; ".join("%s %s" % (i.mnemonic, i.op_str) for i in insns)
    tg = [int(i.op_str[1:], 16) for i in insns
          if i.mnemonic == "bl" and i.op_str.startswith("#")]
    lit_ok = struct.pack("<I", HANDLE_TABLE) in code
    ok = (lit_ok and ("ldrh r1, [r5, #0x28]" in ptext)
          and ("cmp r2, #%d" % FALLBACK_LONG_TICKS) in ptext
          and tg == [CAVE + OFF_KEY_TOGGLE, CAVE + OFF_BROADCAST_ALLCH]
          and tg.count(UI_CREATE_WIDGET) == 0)
    out.append((ok, "fallback poll: 句柄表 0x%08X 字面量=%s；含 `ldrh r1,[r5,#0x28]` 与 "
                    "`cmp r2,#%d`；动作调用=%s；建控件调用 0 次=%s"
                % (HANDLE_TABLE, lit_ok, FALLBACK_LONG_TICKS,
                   ["0x%08X" % x for x in tg], tg.count(UI_CREATE_WIDGET) == 0)))

    ok, msg = _uc_test_fallback_poll(patched)
    out.append((ok, msg))
    return out


def _uc_test_fallback_poll(patched: bytes) -> tuple[bool, str]:
    """桩执行 fallback poll：锁磁贴长按 5 tick -> KEY 翻转一次；设置页签 -> 广播一次。"""
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_PC, UC_ARM_REG_LR)
    uc = _uc_machine(patched)
    LOCK_W, TAB_W = 0x20043200, 0x20043300        # 假控件结构
    st = {"broadcast": 0, "key_toggle": 0}
    uc.mem_write(HANDLE_TABLE, struct.pack("<IIIIIIII",
                                           0x20043100, 0x20043110, 0x20043120,
                                           0x20043130, 0x20043140, LOCK_W,
                                           0x20043160, TAB_W))
    uc.mem_write(LOCK_W + 0x28, struct.pack("<H", 0))
    uc.mem_write(TAB_W + 0x28, struct.pack("<H", 0))
    uc.mem_write(FB_LOCK_COUNT, bytes(1))
    uc.mem_write(FB_LOCK_TRIG, bytes(1))
    uc.mem_write(FB_TAB_COUNT, bytes(1))
    uc.mem_write(FB_TAB_TRIG, bytes(1))
    uc.mem_write(DEFAULT_KEY_FLAG, bytes(1))
    uc.mem_write(RAM_UI_PAGE, bytes(1))          # 页面门：主界面(0)

    def hook(uc_, address, size, user):
        if address == IS_WINDOW:
            uc_.reg_write(UC_ARM_REG_R0, 1 if uc_.reg_read(UC_ARM_REG_R0) else 0)
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
            return
        if address == CAVE + OFF_BROADCAST_ALLCH:
            st["broadcast"] += 1
            uc_.reg_write(UC_ARM_REG_R0, 0)
        elif address == CAVE + OFF_KEY_TOGGLE:
            st["key_toggle"] += 1
        else:
            return
        if address == CAVE + OFF_BROADCAST_ALLCH:
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)

    def tick():
        _uc_call(uc, CAVE + OFF_POLL, (), count=20000)

    def press(w, on):
        uc.mem_write(w + 0x28, struct.pack("<H", 8 if on else 0))

    # 1) 按住锁磁贴 7 个 tick -> 只触发一次（阈值 5）
    press(LOCK_W, True)
    for _ in range(7):
        tick()
    trig1 = st["key_toggle"]
    # 2) 松开 -> 再按 7 tick -> 再触发一次
    press(LOCK_W, False)
    tick()
    press(LOCK_W, True)
    for _ in range(7):
        tick()
    trig2 = st["key_toggle"]
    press(LOCK_W, False)
    tick()
    # 3) 按住设置页签 7 tick -> 广播只一次
    press(TAB_W, True)
    for _ in range(7):
        tick()
    b = st["broadcast"]
    ok = (trig1 == 1 and trig2 == 2 and b == 1)
    return ok, ("fallback poll 桩执行: 按住锁磁贴 7 tick -> 触发 %d 次（期望 1，阈值 %d）；"
                "松开再按 -> 累计 %d 次（期望 2）；按住设置页签 7 tick -> 广播 %d 次（期望 1）"
                % (trig1, FALLBACK_LONG_TICKS, trig2, b))


def _verify_touch_probe(patched: bytes, opts: dict):
    """触摸探测 bin 的静态验证。"""
    import capstone
    out = []
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
    fn = patched[_off(UI_PAGE1_HANDLER):_off(UI_PAGE1_HANDLER) + 4]
    out.append((fn.hex() == UI_PAGE1_ORIG_INS, "0x0800FE78 保持原厂 %s" % fn.hex()))
    tick = patched[_off(UI_TICK_CALL_SITE):_off(UI_TICK_CALL_SITE) + 4]
    out.append((tick.hex() == UI_TICK_CALL_BEFORE,
                "0x08014C1E 保持原厂 %s" % tick.hex()))
    got = patched[_off(PAGE0_CALL_SITE):_off(PAGE0_CALL_SITE) + 4]
    insn = list(md.disasm(got, PAGE0_CALL_SITE))[0]
    out.append((insn.mnemonic == "bl" and insn.op_str == "#0x%x" % WRAPPER0,
                "page0 调用点 -> %s %s（应为 bl wrapper0）" % (insn.mnemonic,
                                                              insn.op_str)))
    code = patched[_off(CAVE + OFF_POLL):_off(CAVE + OFF_POLL) + 240]
    ins = list(md.disasm(code, CAVE + OFF_POLL))
    tg = [int(i.op_str[1:], 16) for i in ins
          if i.mnemonic == "bl" and i.op_str.startswith("#")]
    need = [struct.pack("<I", v) in code for v in
            (TOUCH_COUNTER, TOUCH_X, TOUCH_Y, TOUCH_LAST_X, TOUCH_LAST_Y)]
    ok = (all(need) and tg == [CAVE + OFF_KEY_TOGGLE]
          and tg.count(UI_CREATE_WIDGET) == 0)
    out.append((ok, "touch poll: 触摸全局字面量=%s；动作调用=%s；建控件 0 次=%s"
                % (need, ["0x%08X" % x for x in tg],
                   tg.count(UI_CREATE_WIDGET) == 0)))
    return out


def _verify_page0(patched: bytes, opts: dict):
    """page 0（主界面）方案的静态 + 桩验证。"""
    import capstone
    out: list[tuple[bool, str]] = []
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)

    # page1 的钩子必须全部保持原厂
    fn = patched[_off(UI_PAGE1_HANDLER):_off(UI_PAGE1_HANDLER) + 4]
    out.append((fn.hex() == UI_PAGE1_ORIG_INS,
                "0x0800FE78 保持原厂首指令 %s（实际 %s）"
                % (UI_PAGE1_ORIG_INS, fn.hex())))
    tick = patched[_off(UI_TICK_CALL_SITE):_off(UI_TICK_CALL_SITE) + 4]
    out.append((tick.hex() == UI_TICK_CALL_BEFORE,
                "0x08014C1E 保持原厂 %s（实际 %s）"
                % (UI_TICK_CALL_BEFORE, tick.hex())))
    tail = patched[_off(PAGE1_TAIL_HOOK):_off(PAGE1_TAIL_HOOK) + 6]
    out.append((tail.hex() == PAGE1_TAIL_BEFORE,
                "0x0800FF50 保持原厂 %s（实际 %s）"
                % (PAGE1_TAIL_BEFORE, tail.hex())))
    # page0 调用点
    got = patched[_off(PAGE0_CALL_SITE):_off(PAGE0_CALL_SITE) + 4]
    insn = list(md.disasm(got, PAGE0_CALL_SITE))[0]
    out.append((insn.mnemonic == "bl" and insn.op_str == "#0x%x" % WRAPPER0,
                "page0 调用点 0x%08X: %s %s（应为 bl wrapper0 0x%X）"
                % (PAGE0_CALL_SITE, insn.mnemonic, insn.op_str, WRAPPER0)))
    w0 = list(md.disasm(patched[_off(WRAPPER0):_off(WRAPPER0) + 20], WRAPPER0))
    w0t = "; ".join("%s %s" % (i.mnemonic, i.op_str) for i in w0)
    bl0 = [int(i.op_str[1:], 16) for i in w0
           if i.mnemonic == "bl" and i.op_str.startswith("#")]
    ok0 = (len(w0) >= 6 and w0[0].mnemonic == "push"
           and bl0 == [CAVE + OFF_POLL, PAGE0_HANDLER]
           and w0[-1].mnemonic.startswith("pop"))
    out.append((ok0, "wrapper0 反汇编: %s（应为 push{r4,lr}; mov r4,r0; bl poll; "
                     "mov r0,r4; bl 0x080345B4; pop{r4,pc}）" % w0t))

    # poll 检查（与 A2 相同）
    code = patched[_off(CAVE + OFF_POLL):_off(CAVE + OFF_POLL) + 220]
    insns = list(md.disasm(code, CAVE + OFF_POLL))
    ptext = "; ".join("%s %s" % (i.mnemonic, i.op_str) for i in insns)
    tg = [int(i.op_str[1:], 16) for i in insns
          if i.mnemonic == "bl" and i.op_str.startswith("#")]
    ok = (struct.pack("<I", HANDLE_TABLE) in code
          and struct.pack("<I", RAM_UI_PAGE) in code
          and "ldrh r1, [r5, #0x28]" in ptext
          and ("cmp r2, #%d" % FALLBACK_LONG_TICKS) in ptext
          and tg == [IS_WINDOW, CAVE + OFF_KEY_TOGGLE, IS_WINDOW,
                     CAVE + OFF_BROADCAST_ALLCH]
          and tg.count(UI_CREATE_WIDGET) == 0)
    out.append((ok, "fallback poll: 句柄表/页面门(0x%08X)/按下位/长按阈值/"
                    "控件树校验(0x%08X)×2/动作调用 均正确=%s"
                % (RAM_UI_PAGE, IS_WINDOW, ok)))
    ok, msg = _uc_test_fallback_poll(patched)
    out.append((ok, msg))

    if opts.get("entry_mode") == "tiles_p0":
        got = patched[_off(TILES_CALL_SITE):_off(TILES_CALL_SITE) + 4]
        insn = list(md.disasm(got, TILES_CALL_SITE))[0]
        out.append((insn.mnemonic == "bl" and insn.op_str == "#0x%x" % WRAPPER1,
                    "builder 调用点 0x%08X: %s %s（应为 bl wrapper1 0x%X）"
                    % (TILES_CALL_SITE, insn.mnemonic, insn.op_str, WRAPPER1)))
        w1 = list(md.disasm(patched[_off(WRAPPER1):_off(WRAPPER1) + 16], WRAPPER1))
        w1t = "; ".join("%s %s" % (i.mnemonic, i.op_str) for i in w1)
        bl1 = [int(i.op_str[1:], 16) for i in w1
               if i.mnemonic == "bl" and i.op_str.startswith("#")]
        ok1 = (len(w1) >= 4 and w1[0].mnemonic == "push"
               and bl1 == [PAGE0_MENU_BUILDER, TILES0_EXT]
               and w1[-1].mnemonic.startswith("pop"))
        out.append((ok1, "wrapper1 反汇编: %s（应为 push{r4,lr}; bl 0x080079E8; "
                         "bl tiles; pop{r4,pc}）" % w1t))
        tcode = patched[_off(TILES0_EXT):_off(TILES0_EXT) + 480]
        tt = list(md.disasm(tcode, TILES0_EXT))
        ttxt = "; ".join("%s %s" % (i.mnemonic, i.op_str) for i in tt)
        ttg = [int(i.op_str[1:], 16) for i in tt
               if i.mnemonic == "bl" and i.op_str.startswith("#")]
        ok = (struct.pack("<I", HANDLE_TABLE) in tcode
              and struct.pack("<I", RAM_UI_PAGE) in tcode
              and ttg.count(UI_CREATE_WIDGET) == 2
              and ttg.count(UI_SET_TEXT) == 2
              and ttg.count(0x0802553E) == 0
              and ttg.count(WIDGET_SHOW) == 2
              and "cmp r5, #0" in ttxt)
        out.append((ok, "tiles0: 页面门(0x%08X)=%s、父窗口取自 0x%08X、2×建磁贴、2×文本、"
                        "2×show(0x%08X)、0 个图标子控件、父窗口判 0=%s"
                    % (RAM_UI_PAGE, struct.pack("<I", RAM_UI_PAGE) in tcode,
                       HANDLE_TABLE, WIDGET_SHOW,
                       ttg.count(WIDGET_SHOW) == 2)))
        ok, msg = _uc_test_tiles0(patched, opts)
        out.append((ok, msg))
    return out


def _uc_test_tiles0(patched: bytes, opts: dict) -> tuple[bool, str]:
    """桩执行 page0 磁贴创建：父窗口必须来自句柄表[0]，句柄非 0 才设属性。"""
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_PC,
                                   UC_ARM_REG_LR)
    uc = _uc_machine(patched)
    PARENT = 0x20042100
    FRAME_COLOR = 0x00332211
    BG = 0x00000027
    created, texts, calls = [], [], []

    shows = []

    def hook(uc_, address, size, user):
        r0, r1, r2 = (uc_.reg_read(UC_ARM_REG_R0), uc_.reg_read(UC_ARM_REG_R1),
                      uc_.reg_read(UC_ARM_REG_R2))
        if address == WIDGET_SHOW:
            shows.append((r0, r1))
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
            return
        if address == 0x08027FE0:
            uc_.reg_write(UC_ARM_REG_R0, FRAME_COLOR)
        elif address == 0x08027D90:
            uc_.reg_write(UC_ARM_REG_R0, BG)
        elif address == UI_CREATE_WIDGET:
            sp = uc_.reg_read(UC_ARM_REG_SP)
            w, h = struct.unpack("<II", bytes(uc_.mem_read(sp, 8)))
            created.append((r0, r1, r2, uc_.reg_read(UC_ARM_REG_R3), w, h))
            uc_.reg_write(UC_ARM_REG_R0, 0x20044000 + len(created) * 0x40)
        elif address == UI_SET_TEXT:
            texts.append((r0, r1))
            uc_.reg_write(UC_ARM_REG_R0, 0)
        elif address in (0x0803157C, 0x08031594, 0x0803161C, 0x08031604,
                         0x08031630, 0x08031626, 0x080315B6, 0x080315C0,
                         0x0803159E, 0x08031696):
            calls.append((address, r0, r1, r2))
            uc_.reg_write(UC_ARM_REG_R0, 0)
        else:
            return
        uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.mem_write(HANDLE_TABLE, struct.pack("<I", PARENT))       # [0] = 根窗口
    uc.mem_write(RAM_UI_PAGE, bytes(1))                              # 主界面(0)
    ret = _uc_call(uc, TILES0_EXT, (), count=200000)
    want = [(PARENT, 1, x, y, w, h) for (x, y, w, h, _l) in TILE_SPECS]
    parent_ok = created == want
    labels_ok = [t[1] == CAVE + s[4] for t, s in zip(texts, TILE_SPECS)]
    setter_ok = len(calls) == 12 * len(TILE_SPECS)
    show_ok = len(shows) == len(TILE_SPECS) and all(s[1] == 1 for s in shows)
    ok = (parent_ok and all(labels_ok) and len(texts) == len(TILE_SPECS)
          and setter_ok and show_ok)
    return ok, ("page0 tiles 桩执行: parent=0x%08X 正确=%s；建磁贴 %d 个；setter %d 次"
                "（期望 %d）；文本指针正确=%s；show(w,1) %d 次（期望 %d）"
                % (PARENT, parent_ok, len(created), len(calls),
                   12 * len(TILE_SPECS), all(labels_ok), len(shows),
                   len(TILE_SPECS)))

    return True, ""


def _verify_menu_patch(patched: bytes, orig: bytes, opts: dict):
    """enable_menu_patch=True 时的静态 + 桩验证（UI 接线部分）。"""
    if opts.get("entry_mode") == "touch_p0":
        return _verify_touch_probe(patched, opts)
    if opts.get("entry_mode", "tiles") in ("fallback_p0", "tiles_p0"):
        return _verify_page0(patched, opts)
    if opts.get("entry_mode", "tiles") == "fallback":
        return _verify_fallback(patched, opts)
    import capstone
    out: list[tuple[bool, str]] = []
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)

    # 1) 挂载点：4 字节换成 bl 0x080F0800
    got = patched[_off(PAGE1_TAIL_HOOK):_off(PAGE1_TAIL_HOOK) + 6]
    insns = list(md.disasm(got, PAGE1_TAIL_HOOK))
    ok = (len(insns) >= 1 and insns[0].mnemonic == "bl"
          and insns[0].op_str == "#0x%x" % UI_TILES_EXT and got[4:6] == b"\x00\xbf")
    out.append((ok, "页 1 尾部挂载点 0x%08X: %s %s + %s（应为 bl 0x%X + nop）"
                % (PAGE1_TAIL_HOOK,
                   insns[0].mnemonic if insns else "?",
                   insns[0].op_str if insns else "?", got[4:6].hex(), UI_TILES_EXT)))

    # 2) FUN_080079E8 其余字节与原厂一致（只该改这 4 字节）
    fn_lo, fn_hi = 0x0800FE78, 0x0800FF5A
    changed = [i for i in range(_off(fn_lo), _off(fn_hi))
               if patched[i] != orig[i]]
    hook_lo = _off(PAGE1_TAIL_HOOK)
    expect = {hook_lo + i for i in range(6)}
    if opts.get("three_tab_layout", False):
        for addr, before, after, _d in MENU_LAYOUT_PATCH:
            if _off(0x080079E8) <= _off(addr) < _off(0x08008250):
                bb, aa = bytes.fromhex(before), bytes.fromhex(after)
                expect.update(_off(addr) + i for i in range(len(bb)) if bb[i] != aa[i])
    out.append((set(changed) == expect,
                "FUN_080079E8 内差异偏移 = %s（应恰好是挂载点 4 字节）"
                % ["0x%05X" % c for c in changed]))

    # 3) 注入函数反汇编：必须出现 2×FUN_08006220、2×FUN_0802D97A、1×FUN_08027FE0
    code = patched[_off(UI_TILES_EXT):_off(UI_TILES_EXT) + 512]
    insns = list(md.disasm(code, UI_TILES_EXT))
    targets = []
    for i in insns:
        if i.mnemonic == "bl" and i.op_str.startswith("#"):
            targets.append(int(i.op_str[1:], 16))
    mode = opts.get("tile_mode", "full")
    icon = bool(opts.get("tile_icon", False))
    need = [0x08006220, 0x0802D97A]
    if mode == "full":
        need += [0x08027D90, 0x0803157C, 0x08031594, 0x0803161C, 0x08031604,
                 0x08031630, 0x08031626, 0x080315B6, 0x080315C0, 0x0803159E,
                 0x08031696]
        if icon:
            need.append(0x0802553E)
    ok = True
    if mode == "none":
        ok = targets.count(0x08006220) == 0
        out.append((ok, "tile_mode=none：不建磁贴（诊断/安全档），无 0x08006220 调用=%s" % ok))
    else:
        ok = (targets.count(UI_CREATE_WIDGET) == 2 and targets.count(UI_SET_TEXT) == 2
              and all(x in targets for x in need)
              and (targets.count(0x0802553E) == (2 if icon else 0)))
        out.append((ok, "tile_mode=%s icon=%s: 共 %d 个 bl；2×建磁贴、2×文本、"
                        "图标调用=%d；原厂 setter 全部出现=%s"
                    % (mode, icon, len(targets), targets.count(0x0802553E), ok)))

    # 4) Unicorn 桩验证：stub 掉 UI API，执行注入函数，检查 2 个磁贴的参数
    ok, msg = _uc_test_panel_ext(patched, opts)
    out.append((ok, msg))

    # 4b) 底部页签布局：默认不改（原厂仍是 2 个页签）
    if not opts.get("three_tab_layout", False):
        same = True
        for addr, before, _after, _desc in MENU_LAYOUT_PATCH:
            cur = patched[_off(addr):_off(addr) + len(before) // 2]
            same = same and (cur.hex() == before)
        out.append((same, "底部页签布局补丁默认未应用（0x08007E7C/0x0800807A/0x08008084 "
                          "与原厂一致 → 仍是原厂 2 个页签）"))
    else:
        out.append((True, "底部页签布局补丁已应用（three_tab_layout=True，实验项）"))

    # 5) 点击动作接线：页 1 入口跳板 + 轮询函数
    # 0x0800FE78 必须是原厂字节（旧方案的致命 bug 就是改写了它）
    fn_bytes = patched[_off(UI_PAGE1_HANDLER):_off(UI_PAGE1_HANDLER) + 4]
    out.append((fn_bytes.hex() == UI_PAGE1_ORIG_INS,
                "0x0800FE78 保持原厂首指令 %s（实际 %s）"
                % (UI_PAGE1_ORIG_INS, fn_bytes.hex())))
    got = patched[_off(UI_TICK_CALL_SITE):_off(UI_TICK_CALL_SITE) + 4]
    insn = list(md.disasm(got, UI_TICK_CALL_SITE))[0]
    out.append((insn.mnemonic == "bl" and insn.op_str == "#0x%x" % (CAVE + OFF_TRAMP),
                "UI tick 调用点 0x%08X: %s %s（应为 bl wrapper 0x%X）"
                % (UI_TICK_CALL_SITE, insn.mnemonic, insn.op_str, CAVE + OFF_TRAMP)))

    tramp = list(md.disasm(patched[_off(CAVE + OFF_TRAMP):_off(CAVE + OFF_TRAMP) + 20],
                           CAVE + OFF_TRAMP))
    ttxt = ["%s %s" % (i.mnemonic, i.op_str) for i in tramp]
    bl_targets = [int(i.op_str[1:], 16) for i in tramp
                  if i.mnemonic == "bl" and i.op_str.startswith("#")]
    wok = (len(tramp) >= 4 and tramp[0].mnemonic == "push"
           and bl_targets == [CAVE + OFF_POLL, UI_PAGE1_HANDLER]
           and tramp[-1].mnemonic.startswith("pop"))
    out.append((wok, "wrapper 反汇编: %s（应为 push {r4,lr}; bl poll; bl 0x0800FE78; pop {r4,pc}）"
                % "; ".join(ttxt)))

    targets = []
    poll_insns = list(md.disasm(patched[_off(CAVE + OFF_POLL):_off(CAVE + OFF_POLL) + 160],
                                CAVE + OFF_POLL))
    for i in poll_insns:
        if i.mnemonic == "bl" and i.op_str.startswith("#"):
            targets.append(int(i.op_str[1:], 16))
    want = [CAVE + OFF_KEY_TOGGLE, CAVE + OFF_BROADCAST_ALLCH]
    ptext = "; ".join("%s %s" % (i.mnemonic, i.op_str) for i in poll_insns)
    has_read = ("ldrh r1, [r0, #0x28]" in ptext) and ("ands r1, r6" in ptext)
    out.append((targets == want and has_read,
                "ui_tiles_poll: 动作调用 %s（期望 %s）；按下判定 `ldrh r1,[r0,#0x28]`+`ands r1,r6(=8)`=%s"
                % (["0x%08X" % t for t in targets],
                   ["0x%08X" % t for t in want], has_read)))

    ok, msg = _uc_test_poll(patched, opts)
    out.append((ok, msg))

    # 5b) 挂载点原 3 条指令必须在注入函数里被原样复现
    if opts.get("tile_mode", "full") != "none":
        tile_code = patched[_off(UI_TILES_EXT):_off(UI_TILES_EXT) + 40]
        ttxt2 = "; ".join("%s %s" % (i.mnemonic, i.op_str)
                          for i in md.disasm(tile_code, UI_TILES_EXT))
        lit_ok = struct.pack("<I", RAM_PAGE_READY) in patched[
            _off(UI_TILES_EXT):_off(UI_TILES_EXT) + 480]
        rep_ok = ("movs r0, #6" in ttxt2) and ("strb r0, [r1]" in ttxt2) and lit_ok
        out.append((rep_ok, "注入函数复现原尾指令 `ldr r1,=0x%08X; movs r0,#6; strb r0,[r1]`=%s"
                    % (RAM_PAGE_READY, rep_ok)))

    # 6) 全 ID 扫描：allid 必须是 step=1、上限 100
    al = list(md.disasm(patched[_off(CAVE + OFF_BROADCAST_ALLID):
                                _off(CAVE + OFF_BROADCAST_ALLID) + 120],
                        CAVE + OFF_BROADCAST_ALLID))
    atext = "; ".join("%s %s" % (i.mnemonic, i.op_str) for i in al)
    step = opts.get("allid_step", 1)
    full_id = (("adds r6, #%d" % step) in atext) and \
              (("cmp r6, #100" in atext) or ("cmp r6, #0x64" in atext))
    out.append((full_id, "ui_broadcast_allid 全 ID 扫描: 步长 %d、上限 100 = %s（期望 True）"
                % (step, full_id)))
    return out


def _uc_test_poll(patched: bytes, opts: dict) -> tuple[bool, str]:
    """桩执行 ui_tiles_poll()：验证"按下 -> 动作"＋边沿检测（按住只触发一次）。"""
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_PC, UC_ARM_REG_LR)
    KEY_H, BCAST_H = 0x20043000, 0x20043040
    uc = _uc_machine(patched)
    st = {"broadcast": 0}

    def hook(uc_, address, size, user):
        if address == CAVE + OFF_BROADCAST_ALLCH:
            st["broadcast"] += 1
            uc_.reg_write(UC_ARM_REG_R0, 0)
            uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))
            return
        if address == CAVE + OFF_KEY_TOGGLE:
            return                        # 真实执行（只翻转 RAM 字节）

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.mem_write(RAM_TILE_KEY, struct.pack("<I", KEY_H))
    uc.mem_write(RAM_TILE_BCAST, struct.pack("<I", BCAST_H))
    uc.mem_write(RAM_LAST_KEY, b"\x00")
    uc.mem_write(RAM_LAST_BCAST, b"\x00")
    uc.mem_write(DEFAULT_KEY_FLAG, b"\x00")

    def tick():
        _uc_call(uc, CAVE + OFF_POLL, (), count=20000)

    def press(h, on):
        uc.mem_write(h + 0x28, struct.pack("<H", 8 if on else 0))

    # 1) 按住 KEY 磁贴，连续 3 个 tick → 只应翻转一次（0->1）
    press(KEY_H, True)
    for _ in range(3):
        tick()
    f1 = uc.mem_read(DEFAULT_KEY_FLAG, 1)[0]
    # 2) 松开 → 再按 → 再翻转一次（1->0）
    press(KEY_H, False)
    tick()
    press(KEY_H, True)
    tick()
    f2 = uc.mem_read(DEFAULT_KEY_FLAG, 1)[0]
    # 3) 按住 BCAST 磁贴 3 个 tick → 只广播一次
    press(BCAST_H, True)
    for _ in range(3):
        tick()
    b = st["broadcast"]
    ok = (f1 == 1 and f2 == 0 and b == 1)
    return ok, ("轮询桩执行: 按住 KEY 3 tick -> KEY_ON=%d（期望 1，边沿只触发一次）；"
                "松开再按 -> %d（期望 0）；按住 BCAST 3 tick -> 广播 %d 次（期望 1）"
                % (f1, f2, b))


def _uc_test_panel_ext(patched: bytes, opts: dict) -> tuple[bool, str]:
    """桩执行 ui_quickpanel_ext：断言照抄了原厂磁贴的完整属性序列 + r5/r6 护栏。"""
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_R5,
                                   UC_ARM_REG_R6, UC_ARM_REG_PC, UC_ARM_REG_LR)
    uc = _uc_machine(patched)
    PARENT = 0x20042100
    RAM_PTR = 0x200004C7                    # 实机上该点 r6 的真实值（RAM 字节指针）
    CANARY = bytes.fromhex("a5" * 32)
    FRAME_COLOR = 0x00332211                # 0x08027FE0 的返回值
    BG_COLOR = 0x00000027                   # 0x08027D90 的返回值

    created, texts = [], []
    calls: list[tuple] = []                 # (func, r0, r1, r2)
    # 原厂"声音"磁贴的属性 setter 顺序（0x08007B16-0x08007BD2）
    mode = opts.get("tile_mode", "full")
    icon = bool(opts.get("tile_icon", False))
    if mode == "none":
        return True, "tile_mode=none（诊断/安全档）：不创建磁贴，跳过磁贴桩测试"
    if mode == "minimal":
        EXPECT_SETTERS = [0x0802D97A]
    else:
        EXPECT_SETTERS = [0x0803157C, 0x08031594, 0x0803161C, 0x08031604,
                          0x08031630, 0x08031626, 0x080315B6, 0x080315C0,
                          0x080315B6, 0x080315C0, 0x0803159E, 0x08031696]
        if icon:
            EXPECT_SETTERS.append(0x0802553E)
        EXPECT_SETTERS.append(0x0802D97A)
    STUBS = {0x08006220, 0x08027FE0, 0x08027D90, 0x0802553E, 0x0802D97A} | set(EXPECT_SETTERS)

    def hook(uc_, address, size, user):
        if address not in STUBS:
            return
        r0 = uc_.reg_read(UC_ARM_REG_R0)
        r1 = uc_.reg_read(UC_ARM_REG_R1)
        r2 = uc_.reg_read(UC_ARM_REG_R2)
        if address == 0x08027FE0:
            uc_.reg_write(UC_ARM_REG_R0, FRAME_COLOR)
        elif address == 0x08027D90:
            uc_.reg_write(UC_ARM_REG_R0, BG_COLOR)
        elif address == 0x08006220:
            sp = uc_.reg_read(UC_ARM_REG_SP)
            w, h = struct.unpack("<II", bytes(uc_.mem_read(sp, 8)))
            created.append((r0, r1, r2, uc_.reg_read(UC_ARM_REG_R3), w, h))
            uc_.reg_write(UC_ARM_REG_R0, 0x20043000 + len(created) * 0x40)
        elif address == 0x0802553E:
            sp = uc_.reg_read(UC_ARM_REG_SP)
            font, color_, fmt = struct.unpack("<3I", bytes(uc_.mem_read(sp, 12)))
            calls.append((address, r0, (r1, r2, font, color_, fmt)))
            uc_.reg_write(UC_ARM_REG_R0, 0)
        elif address == 0x0802D97A:
            texts.append((r0, r1))
            calls.append((address, r0, (r1, r2)))
            uc_.reg_write(UC_ARM_REG_R0, 0)
        else:
            calls.append((address, r0, (r1, r2)))
            uc_.reg_write(UC_ARM_REG_R0, 0)
        uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    # 真实上下文：r5=根窗口、r6=0x200004C7；并在 r6 指向处放 canary 验证未被写
    uc.mem_write(RAM_PTR & ~3, CANARY)
    uc.reg_write(UC_ARM_REG_R5, 0xDEAD0000)     # 该点 r5 无意义（旧版误用它）
    uc.reg_write(UC_ARM_REG_R6, PARENT)         # 真实上下文：r6 = 根窗口
    ret = _uc_call(uc, UI_TILES_EXT, (), count=200000)
    ram_ok = bytes(uc.mem_read(RAM_PTR & ~3, 32)) == CANARY
    ready_ok = uc.mem_read(RAM_PAGE_READY, 1)[0] == 6

    # 1) parent 必须是 r5（不是 0x200004C7），且两次创建参数正确
    want_created = [(PARENT, 1, x, y, w, h) for (x, y, w, h, _l) in TILE_SPECS]
    parent_ok = [c[:6] for c in created] == want_created
    # 2) 每个磁贴的属性 setter 序列 == 原厂序列
    seq = [f for (f, _r0, _a) in calls if f in EXPECT_SETTERS]
    seq_ok = seq == EXPECT_SETTERS * len(TILE_SPECS)
    # 3) 关键参数抽查（顺序与原厂一致）
    arg_ok = True
    detail = []
    per = []
    idx = 0
    for _t in range(len(TILE_SPECS)):
        block = seq[idx:idx + len(EXPECT_SETTERS)]
        args = [args for (f, _r0, args) in calls if f in EXPECT_SETTERS]
        per.append(block)
        idx += len(EXPECT_SETTERS)
    args_all = [a for (f, _r0, a) in calls if f in EXPECT_SETTERS]
    # 每个磁贴 14 个 setter：抽 3157C / 31630 / 31626 / 315B6(第1个) / 31696 / 2553E / 2D97A
    for tt in range(len(TILE_SPECS)):
        base = tt * len(EXPECT_SETTERS)
        a = args_all[base:base + len(EXPECT_SETTERS)]
        idx = {f: i for i, f in enumerate(EXPECT_SETTERS)}

        def arg(f):
            return a[idx[f]] if f in idx and idx[f] < len(a) else None

        if mode == "minimal":
            if arg(0x0802D97A) is None or arg(0x0802D97A)[0] != CAVE + TILE_SPECS[tt][4]:
                arg_ok = False; detail.append("t%d text ptr" % tt)
            continue
        if arg(0x0803157C) != (BG_COLOR, 0):
            arg_ok = False; detail.append("t%d 3157C %r" % (tt, arg(0x0803157C)))
        if arg(0x08031630) != (2, 4):
            arg_ok = False; detail.append("t%d 31630 %r" % (tt, arg(0x08031630)))
        if arg(0x08031626) != (4, 4):
            arg_ok = False; detail.append("t%d 31626 %r" % (tt, arg(0x08031626)))
        if arg(0x080315B6) is None:
            arg_ok = False; detail.append("t%d 315B6 missing" % tt)
        if arg(0x080315C0) is None:
            arg_ok = False; detail.append("t%d 315C0 missing" % tt)
        if arg(0x08031696) != (0x6C, 0):
            arg_ok = False; detail.append("t%d 31696 %r" % (tt, arg(0x08031696)))
        if icon:
            ic = arg(0x0802553E)
            if ic is None or ic[0] != 9 or ic[2] != 0x0806B090 or ic[4] != 0x08007E20:
                arg_ok = False; detail.append("t%d icon %r" % (tt, ic))
        if arg(0x0802D97A) is None or arg(0x0802D97A)[0] != CAVE + TILE_SPECS[tt][4]:
            arg_ok = False; detail.append("t%d text ptr %r" % (tt, arg(0x0802D97A)))
    labels_ok = [txt[1] == CAVE + spec[4] for txt, spec in zip(texts, TILE_SPECS)]
    ok = (parent_ok and seq_ok and arg_ok and all(labels_ok)
          and len(texts) == len(TILE_SPECS) and ram_ok and ready_ok)
    return ok, ("ui_quickpanel_ext 桩执行: parent(r5)=0x%08X 正确=%s；磁贴 %d 个 %s；"
                "属性 setter 序列与原厂(0x08007B16-0x08007BD2)一致=%s（%d 个/片 × %d 片）；"
                "关键参数=%s%s；返回颜色 0x%08X（期望 0x%08X）；r6(0x200004C7) RAM 未被写=%s"
                % (PARENT, parent_ok, len(created),
                   [c[:4] for c in created], seq_ok, len(EXPECT_SETTERS),
                   len(TILE_SPECS), arg_ok,
                   (" " + ",".join(detail)) if detail else "", ret, FRAME_COLOR, ram_ok))


SENTINEL = 0x2000F000


def _uc_machine(patched: bytes):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS
    uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
    uc.mem_map(0x08000000, 0x100000)
    uc.mem_map(0x20000000, 0x50000)
    uc.mem_write(BASE, patched)
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))     # b .
    return uc


def _uc_call(uc, func, args=(), count=400000):
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR)
    for i, a in enumerate(args[:4]):
        uc.reg_write([UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3][i],
                     a & 0xFFFFFFFF)
    uc.reg_write(UC_ARM_REG_SP, 0x20040000)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    uc.emu_start(func | 1, 0, count=count)
    return uc.reg_read(UC_ARM_REG_R0)


def _uc_test_key_toggle(patched: bytes, flag: int) -> tuple[bool, str]:
    uc = _uc_machine(patched)
    uc.mem_write(flag, b"\x00")
    fn = CAVE + OFF_KEY_TOGGLE
    v1 = _uc_call(uc, fn)
    b1 = uc.mem_read(flag, 1)[0]
    v2 = _uc_call(uc, fn)
    b2 = uc.mem_read(flag, 1)[0]
    ok = (b1 == 1 and b2 == 0 and v1 == 1 and v2 == 0)
    return ok, ("ui_key_toggle() 调 2 次: 标志 %#x 0->%d->%d，返回 %d/%d（期望 1->0）"
                % (flag, b1, b2, v1, v2))


def _uc_test_broadcast(patched: bytes, key: int, opts: dict) -> tuple[bool, str]:
    from unicorn import UC_HOOK_CODE
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_PC, UC_ARM_REG_LR)
    uc = _uc_machine(patched)
    uc.mem_write(0x20000146, b"\x00")        # RF busy 标志清 0
    # 组设置表：A 组 -> [0]=0（造型灯开）,[4]=0x1E（功率 30）
    uc.mem_write(GROUP_TABLE, bytes([0, 0x50, 0x1E, 0xC0, 0x1E, 0x00]))
    calls: dict[str, list] = {"chan": [], "reg": [], "prop": [], "short": []}

    def hook(uc_, address, size, user):
        if address == SET_CHANNEL:
            calls["chan"].append(uc_.reg_read(UC_ARM_REG_R0))
        elif address == WRITE_REG:
            calls["reg"].append((uc_.reg_read(UC_ARM_REG_R0),
                                 uc_.reg_read(UC_ARM_REG_R1)))
        elif address == SEND_PROPERTY:
            calls["prop"].append((uc_.reg_read(UC_ARM_REG_R0),
                                  uc_.reg_read(UC_ARM_REG_R1),
                                  uc_.reg_read(UC_ARM_REG_R2)))
        elif address == SEND_SHORT:
            calls["short"].append(uc_.reg_read(UC_ARM_REG_R0))
        else:
            return
        uc_.reg_write(UC_ARM_REG_PC, uc_.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook)
    nchan = opts.get("nchan", 32)
    _uc_call(uc, CAVE + OFF_BROADCAST_ALLCH, (), count=400000)
    syncs = [v for (r, v) in calls["reg"] if r in (4, 5)]
    props = calls["prop"]
    power_frames = [p for p in props if p[1] == PROP_KEY_POWER]
    lamp_frames = [p for p in props if p[1] == PROP_KEY_LAMP]
    want_power = (opts.get("broadcast_power") if opts.get("broadcast_power", -1) >= 0 else 0x1E)
    want_lamp = (opts.get("model_on") if opts.get("model_on", -1) >= 0 else 1)
    per_group = bool(opts.get("broadcast_per_group", False))
    want_groups = [10, 11, 12, 13, 14] if per_group else [PROP_GROUP_ALL]
    want_pf = [(g, PROP_KEY_POWER, want_power) for g in want_groups] * nchan
    want_lf = [(g, PROP_KEY_LAMP, want_lamp) for g in want_groups] * nchan
    power_ok = (len(power_frames) == len(want_pf) and power_frames == want_pf)
    lamp_ok = (len(lamp_frames) == len(want_lf) and lamp_frames == want_lf)
    ok = (calls["chan"] == list(range(nchan))
          and syncs == [(key >> 8), (key & 0xFF)] * nchan
          and len(calls["short"]) == nchan and power_ok and lamp_ok)
    msg = ("ui_broadcast_allch(): 频道 %d；同步字 %d 次（KEY 交替=%s）；"
           "功率帧 %d 个(%s,0x%X,%d)=%s；造型灯帧 %d 个(%s,0x%X,%d)=%s；TEST %d"
           % (len(calls["chan"]), len(syncs), syncs[:2] == [(key >> 8), (key & 0xFF)],
              len(power_frames), want_groups, PROP_KEY_POWER, want_power, power_ok,
              len(lamp_frames), want_groups, PROP_KEY_LAMP, want_lamp, lamp_ok,
              len(calls["short"])))
    return ok, msg


if __name__ == "__main__":
    orig = open(IMAGE, "rb").read()
    patched, log = apply_ui(orig, {})
    print("补丁点 %d 个：" % len(log))
    for e in log:
        print("  %08X  %-62s %4d B" % (e["addr"], e["desc"], e["len"]))
    print("\n验证：")
    allok = True
    for ok, msg in verify_ui(patched, {}):
        print("  [%s] %s" % ("PASS" if ok else "FAIL", msg))
        allok = allok and ok
    print("\n总判定:", "PASS" if allok else "FAIL")
    sys.exit(0 if allok else 1)
