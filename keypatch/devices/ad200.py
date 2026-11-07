# -*- coding: utf-8 -*-
"""Godox AD200 Pro V1.60 —— 私有密钥「F8 开关」补丁模块（v4）。

用户可见语义（v4，取代 v2 的 "ID==OFF 当开关"）
------------------------------------------------
灯端 C.Fn 菜单第 8 项 **F8**（原厂为「稳定色温模式 OFF/ON」）被复用为
**私有密钥开关**：开关值就是设置字节 **settings[7] = RAM 0x2000041F**
（C.Fn 项 7，随 flash 参数记录自动加载/保存：记录 0x0801F000+slot*100 的字节 31）。

* F8 = ON  -> CC2500 同步字 = 私有 KEY（只有打了补丁的引闪器能引闪）
* F8 = OFF -> 同步字 = 50024 // (ID + 1)（原厂普通模式）

把原厂「ID -> 16 位同步字」函数 0x0800B9AC 整段改写成：

    flag = *(uint8_t *)0x2000041F;      // C.Fn F8 密钥开关
    if (flag) sync = KEY;               // 密钥模式
    else      sync = 50024 / (id + 1);  // 原厂普通模式
    WriteReg(4, sync >> 8);             // SYNC1
    WriteReg(5, sync & 0xFF);           // SYNC0
    return;                             // 只改同步字：无 SCAL、无延时

实现：原位字节补丁，0x0800B9AC 起 **56 字节**（到下一函数 0x0800B9E4 为止），
输出长度不变。函数体 44 字节 + 3 个 32 位字面量（KEY、开关地址、50024）。

补丁点复核（见 ../dev/ad200/NOTES.md 与 ../dev/ad200_ui/）
----------------------------------------------------------
* 0x0800B9AC 函数占 0x0800B9AC..0x0800B9E3，下一函数 0x0800B9E4；原厂字面量
  50024（0x0000C368）在同一函数尾部 +0x34 = 0x0800B9E0。
* 全镜像只有该函数与 rf_chip_config(0x0800C858) 写 SYNC1/SYNC0；后者由 rf_init
  在调用本函数之前执行，初值 0xC3/0x68 随即被本函数覆盖。
* 本函数 4 个调用点（rf_init 0x080048C6、设置变化 0x080045F2 / 0x08008552、
  按键路径 0x08008AB6）都以 settings[6]（ID）为入参；函数内部决定是否用 KEY，
  因此 ID 菜单逻辑保持不变。
* 与原厂差异：去掉 SCAL(0x33)+1ms 延时（只改 SYNC 数字匹配字，不改频率寄存器，
  不需要重校准）；v4 也不再保留 v2 的 20µs 延时（56 字节空间被开关判断占用，
  详细风险评估见 dev/ad200_ui/NOTES.md）。
"""
from __future__ import annotations

import hashlib
import os
import struct

# ---------------------------------------------------------------- 设备描述
DEVICE = {
    "id": "ad200",
    "name": "Godox AD200 Pro",
    "image": "devices/ad200pro/firmware/AD200pro_V1.60.bin",
    "kind": "flash",                     # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "AD200 Pro",
    "firmware_version": "V1.60",
    "base": 0x08003000,
    "sha256": "fc0f77f1eb1c9f9d6f1f96d15daa5722ae33a57fceffe1da02739530dd798bd0",
    "out_name": "AD200pro_V1.60_key{key:04X}.bin",
    "container_name": "AD200Pro_V1.60_key{key:04X}_container.bin",
    "notes": "v4 F8 密钥开关(settings[7]=0x2000041F)：1 -> SYNC=私有 KEY；"
             "0 -> 原厂 50024/(ID+1)。原位 56 字节整函数改写 + F8 改值即时生效 + "
             "屏蔽 6 处稳定色温读者/阻断 ui_key_80 静默清零（F8=OFF 时与原厂行为一致）",
}

BASE = 0x08003000                 # 镜像链接/加载基址（文件偏移 = 地址 - BASE）
ORIGINAL_SIZE = 46380             # AD200pro_V1.60.bin 长度

# 同步字设置函数（整段改写；0x0800B9AC + 56 = 0x0800B9E4 = 下一函数起点）
SYNC_FUNC = 0x0800B9AC
SYNC_FUNC_SIZE = 56
SYNC_FUNC_END = 0x0800B9E4        # 下一函数 FUNC_0800b9e4 起点（不可越过）
LITERAL_KEY_ADDR = SYNC_FUNC + 0x2C   # 0x0800B9D8：KEY 字面量
LITERAL_FLAG_ADDR = SYNC_FUNC + 0x30  # 0x0800B9DC：开关地址字面量
LITERAL_ORIG_ADDR = SYNC_FUNC + 0x34  # 0x0800B9E0：50024（与原厂同址同值）

# v4：F8 密钥开关 = C.Fn 项 7 的值 = settings[7]；记录字节 31 自动持久化
KEY_FLAG_ADDR = 0x2000041F

# 原厂 0x0800B9AC..0x0800B9E4 的 56 字节（apply 前逐字节校验，防固件版本不对）
ORIGINAL_FUNC = bytes.fromhex(
    "0c4a10b5411c1046f7f79cfb0446010a042001f070f8e1b2052001f06cf8"
    "142000f02cff332001f0c7f87d20c00000f025ff10bd68c30000")
assert len(ORIGINAL_FUNC) == SYNC_FUNC_SIZE

# 函数调用的辅助函数（verify 里替换为记录/跳过桩）
DIV_HELPER = 0x080030F0           # __aeabi_uidiv: r0 = r0 / r1
HELPER_REG_WRITE = 0x0800CAA2     # rf_write_reg8(reg=r0, value=r1)
HELPER_STROBE = 0x0800CB64        # rf_write_cmd(strobe=r0)（v4 不再调用）
HELPER_DELAY = 0x0800C828         # delay_us(r0)（v4 不再调用）

# 原厂公式（证据：dev/ad200/NOTES.md + Unicorn 自检）
ID_NUMERATOR = 50024
TEST_IDS = (0, 1, 37, 99)

# ---------------------------------------------------------------- v4 即时生效 hook
# C.Fn 的 +/- 处理函数只在 item==ID(6) 时重设同步字（cmp #6 + bne return）；
# 把这两处条件判断改成 NOP，使任何 C.Fn 项（含 F8）改值后都重设一次同步字，
# 入参仍是 settings[6]（ID，函数内部再按 F8 开关决定用 KEY）。
#   P5a 0x080045EC/EE：dec 处理函数 0x080045AC 内 `cmp r2,#6; bne return`
#   P5b 0x0800854C/4E：inc 处理函数 0x08008504 内 `cmp r0,#6; bne return`
HOOK_ITEM_GATE = 0x080045EF         # dec 处理函数：bne 的 cond 字节（0xD1 BNE -> 0xD3 BLO）
HOOK_ITEM_GATE2 = 0x0800854F        # inc 处理函数：同上
HOOK_ORIG = b"\xd1"                 # bne（item==6 才重设）
HOOK_ORIG2 = b"\xd1"
HOOK_PATCH = b"\xd3"                # blo：item>=6 时都重设（=ID 或 F8），F1..F6 与原厂一致
assert len(HOOK_ORIG) == len(HOOK_ORIG2) == len(HOOK_PATCH) == 1

# ---------------------------------------------------------------- v4 稳定色温读者屏蔽
# 原厂「稳定色温模式」的 6 处功能读者（settings[7]==1 时切备用闪光时长/功率表、
# 改功率缩放、测量换算、显示、RF 远程设置）。复用 F8 当密钥开关后把这些读取
# 原位改成常数 0（`ldrb rX,[base,#7]` -> `movs rX,#0`，2 字节），等价于原厂 F8=OFF
# 分支；F8=OFF（出厂默认）时与原厂逐指令行为一致。保留 C.Fn 渲染(0x08004230)
# 与记录 builder(0x08004496)。
READER_MASK_PATCHES = (
    (0x08004AFC, b"\xe2\x79", b"\x00\x22", "显示时长/数值表：F8=1 不切稳定色温备用表"),
    (0x08004FF4, b"\xc0\x79", b"\x00\x20", "显示页 1：F8=1 不再清 g_mode bit4/改页面标志"),
    (0x08005C54, b"\xc9\x79", b"\x00\x21", "measurement_task 换算：F8=1 不改闪光时长/能量路径"),
    (0x08006290, b"\xd2\x79", b"\x00\x22", "level_table_lookup_b：F8=1 不切备用时长表（实测 360→210）"),
    (0x0800B144, b"\xdb\x79", b"\x00\x23", "muldiv_scale：F8=1 不改功率/时长缩放"),
    (0x0800B228, b"\xd2\x79", b"\x00\x22", "RF 远程设置处理：F8=1 不改设置分叉"),
)
# ui_key_80 在 g_mode bits4-3=0 -> 1 切换时会静默把 settings[7] 清 0（原厂用于关掉
# 稳定色温）；复用 F8 后这是「密钥开关被悄悄关掉」，NOP 掉该写。
SILENT_CLEAR = (0x0800BC7E, b"\xca\x71", b"\x00\xbf",
                "ui_key_80 模式切换：不再静默清 settings[7]（防密钥开关被关）")


# C.Fn 渲染函数（Unicorn 验证用）
CFN_RENDER = 0x08003FD0
CFN_DEC_HANDLER = 0x080045AC        # F8 值 -1（以及 C.Fn 项导航 -1）
CFN_INC_HANDLER = 0x08008504        # F8 值 +1（以及 C.Fn 项导航 +1）
UI_BASE = 0x20000150                # UI 状态结构
UI_CFN_OPEN = UI_BASE + 0x12        # 0x20000162：C.Fn 打开
UI_EDIT = UI_BASE + 0x13            # 0x20000163：值编辑态
UI_ITEM = UI_BASE + 0x14            # 0x20000164：当前 C.Fn 项 0..7
SETTINGS_BASE = 0x20000418          # settings[0..7]
SAVE_COUNTDOWN = 0x20000218         # 0x0800B810 写的保存倒计时（u16）
SAVE_REQUEST = 0x20000216           # 0x0800B7F4 置的保存请求
STUB_HELPERS = (0x080038F0,         # 提示音/UI 副作用
                0x0800BA40,         # idle 计时复位
                0x0800B810)         # 保存倒计时（200 tick）


def _off(addr: int) -> int:
    return addr - BASE


def _hex(data: bytes) -> str:
    return data.hex(" ")


def _root() -> str:
    """ 工作区根目录（由本文件位置推导，不写死盘符）。"""
    return os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))


def _resolve_image() -> str:
    return os.path.join(_root(), DEVICE["image"])


# ---------------------------------------------------------------- Thumb 编码器
# 编码器沿用 Lead 已验证原型 keypatch/dev/lead/toggle_proto.py / devices/ad100.py。
def _t_ldr_pc(rt: int, imm8: int) -> bytes:
    assert 0 <= imm8 <= 0xFF
    return struct.pack("<H", 0x4800 | (rt << 8) | imm8)


def _t_ldrb_imm(rt: int, rn: int, imm5: int = 0) -> bytes:
    return struct.pack("<H", 0x7800 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rt & 7))


def _t_push_r4lr() -> bytes:
    return struct.pack("<H", 0xB510)


def _t_pop_r4pc() -> bytes:
    return struct.pack("<H", 0xBD10)


def _t_cmp_imm(rn: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x2800 | (rn << 8) | imm8)


def _t_beq(offset_bytes: int) -> bytes:
    assert offset_bytes % 2 == 0
    imm8 = offset_bytes // 2
    assert 0 <= imm8 <= 0xFF
    return struct.pack("<H", 0xD000 | imm8)


def _t_adds_ri(rn: int, rd: int, imm3: int) -> bytes:
    return struct.pack("<H", 0x1C00 | (imm3 << 6) | (rn << 3) | rd)


def _t_movs_imm(rd: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x2000 | (rd << 8) | imm8)


def _t_mov(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0x4600 | (rm << 3) | rd)


def _t_bl(addr: int, target: int) -> bytes:
    """Thumb BL T1 编码（addr = BL 指令地址）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target))
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    # 解码关系: I1 = NOT(J1 XOR S)  ->  J1 = NOT(I1) XOR S
    j1 = (i1 if s else ((~i1) & 1))
    j2 = (i2 if s else ((~i2) & 1))
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    hw1 = 0xF000 | (s << 10) | imm10
    hw2 = 0xD000 | (j1 << 13) | (j2 << 11) | imm11
    return struct.pack("<HH", hw1, hw2)


def _t_lsrs(rd: int, rm: int, imm5: int) -> bytes:
    return struct.pack("<H", 0x0800 | (imm5 << 6) | (rm << 3) | rd)


def _t_uxtb(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0xB2C0 | (rm << 3) | rd)


class _Asm:
    """极简 Thumb 汇编器：只服务本函数（ldr literal / 条件跳转 / BL / 常用算逻）。"""

    def __init__(self, base, lit_key_addr, lit_flag_addr, lit_orig_addr):
        self.base = base
        self.lit_key_addr = lit_key_addr
        self.lit_flag_addr = lit_flag_addr
        self.lit_orig_addr = lit_orig_addr
        self.code = bytearray()
        self.labels = {}
        self.fixups = []

    def here(self):
        return self.base + len(self.code)

    def label(self, name):
        self.labels[name] = self.here()

    def emit(self, b):
        self.code += b

    def branch(self, kind, label):
        self.fixups.append((len(self.code), kind, label))
        self.emit(b"\x00\x00")

    def ldr_pc(self, rt, target):
        # LDR (literal) T1: PC = (addr+4) & ~3, imm8 = (target-PC)/4
        pc = (self.here() + 4) & ~3
        imm8 = (target - pc) // 4
        assert 0 <= imm8 <= 0xFF, (hex(target), hex(pc))
        self.emit(_t_ldr_pc(rt, imm8))

    def bl(self, target):
        self.emit(_t_bl(self.here(), target))

    def finish(self):
        out = bytearray(self.code)
        for off, kind, label in self.fixups:
            delta = self.labels[label] - (self.base + off + 4)
            if kind == "beq":
                imm8 = delta // 2
                assert 0 <= imm8 <= 0xFF
                val = 0xD000 | imm8
            else:
                raise AssertionError("unsupported branch kind %r" % kind)
            out[off:off + 2] = struct.pack("<H", val)
        return bytes(out)


def build_sync_function(key: int) -> bytes:
    """生成 56 字节替换函数体：44 字节代码 + KEY/FLAG/50024 三个字面量。"""
    key_lit = SYNC_FUNC + 0x2C
    flag_lit = SYNC_FUNC + 0x30
    orig_lit = SYNC_FUNC + 0x34        # 50024 保持原厂字面量地址 0x0800B9E0
    assert orig_lit + 4 == SYNC_FUNC_END

    a = _Asm(SYNC_FUNC, key_lit, flag_lit, orig_lit)
    a.emit(_t_push_r4lr())               # +00 push {r4, lr}
    a.ldr_pc(3, flag_lit)                # +02 ldr r3, =0x2000041F
    a.ldr_pc(2, orig_lit)                # +04 ldr r2, =50024（普通模式默认被除数）
    a.emit(_t_ldrb_imm(3, 3))            # +06 ldrb r3, [r3]   ; F8 开关
    a.emit(_t_adds_ri(0, 1, 1))          # +08 adds r1, r0, #1 ; 普通模式除数 ID+1
    a.emit(_t_cmp_imm(3, 0))             # +0A cmp r3, #0
    a.branch("beq", "go")                # +0C beq go          ; 开关关 -> 原厂公式
    a.ldr_pc(2, key_lit)                 # +0E ldr r2, =KEY    ; 密钥模式被除数
    a.emit(_t_movs_imm(1, 1))            # +10 movs r1, #1     ; 除数=1
    a.label("go")                        # +12
    a.emit(_t_mov(0, 2))                 # +12 mov r0, r2
    a.bl(DIV_HELPER)                     # +14 bl __aeabi_uidiv
    a.emit(_t_mov(4, 0))                 # +18 mov r4, r0
    a.emit(_t_lsrs(1, 0, 8))             # +1A lsrs r1, r0, #8
    a.emit(_t_movs_imm(0, 4))            # +1C movs r0, #4     ; SYNC1
    a.bl(HELPER_REG_WRITE)               # +1E bl rf_write_reg8
    a.emit(_t_uxtb(1, 4))                # +22 uxtb r1, r4
    a.emit(_t_movs_imm(0, 5))            # +24 movs r0, #5     ; SYNC0
    a.bl(HELPER_REG_WRITE)               # +26 bl rf_write_reg8
    a.emit(_t_pop_r4pc())                # +2A pop {r4, pc}
    code = a.finish()
    assert len(code) == 0x2C, len(code)
    lits = struct.pack("<III", key, KEY_FLAG_ADDR, ID_NUMERATOR)
    blob = code + lits
    assert len(blob) == SYNC_FUNC_SIZE == SYNC_FUNC_END - SYNC_FUNC
    return blob


# ---------------------------------------------------------------- apply
def apply(data: bytes, key: int, full: bool = True) -> tuple[bytes, list[dict]]:
    """把原厂镜像打上 v4 «F8 密钥开关»补丁，返回 (patched, patch_log)。

    full=True（默认，正式成品）：同步字函数 + 改值即时生效 + 屏蔽稳定色温 6 处读者 +
    阻断 ui_key_80 静默清开关。
    full=False（安全阶梯用 `_core` 变体）：只做同步字函数 + 改值即时生效。

    原位字节补丁，输出长度不变；修改前先校验长度、sha256 与每处原始字节。
    """
    if not isinstance(key, int) or not (0 <= key <= 0xFFFF):
        raise ValueError("key 必须是 16 位整数 (0x0000..0xFFFF)，实际 %r" % (key,))
    if len(data) != ORIGINAL_SIZE:
        raise ValueError("原厂固件长度应为 %d 字节，实际 %d（固件版本不对？）"
                         % (ORIGINAL_SIZE, len(data)))
    got_sha = hashlib.sha256(data).hexdigest()
    if got_sha != DEVICE["sha256"]:
        raise ValueError("原厂固件 sha256 不匹配，拒绝打补丁\n  期望 %s\n  实际 %s"
                         % (DEVICE["sha256"], got_sha))

    off = _off(SYNC_FUNC)
    before = data[off:off + SYNC_FUNC_SIZE]
    if before != ORIGINAL_FUNC:
        raise ValueError("同步字函数 0x%08X..0x%08X 原始字节不匹配（固件版本不对？）\n"
                         "  期望 %s\n  实际 %s"
                         % (SYNC_FUNC, SYNC_FUNC_END - 1,
                            _hex(ORIGINAL_FUNC), _hex(before)))

    blob = build_sync_function(key=key)
    out = bytearray(data)
    out[off:off + SYNC_FUNC_SIZE] = blob

    # ---- 即时生效 hooks（只在 item==6 才重设同步字 -> item>=6 都重设，含 F8）----
    for addr, want, patched_bytes in ((HOOK_ITEM_GATE, HOOK_ORIG, HOOK_PATCH),
                                      (HOOK_ITEM_GATE2, HOOK_ORIG2, HOOK_PATCH)):
        hoff = _off(addr)
        got = bytes(out[hoff:hoff + len(want)])
        if got != want:
            raise ValueError("即时生效补丁点 0x%08X 原始字节不符（固件版本不对？）\n"
                             "  期望 %s\n  实际 %s"
                             % (addr, _hex(want), _hex(got)))
        out[hoff:hoff + len(patched_bytes)] = patched_bytes

    # ---- full：屏蔽稳定色温读者 + 阻断 ui_key_80 静默清开关 ----
    extra_patches = list(READER_MASK_PATCHES) + [SILENT_CLEAR]
    if full:
        for addr, want, patched_bytes, _desc in extra_patches:
            hoff = _off(addr)
            got = bytes(out[hoff:hoff + len(want)])
            if got != want:
                raise ValueError("稳定色温/静默清零补丁点 0x%08X 原始字节不符"
                                 "（固件版本不对？）\n  期望 %s\n  实际 %s"
                                 % (addr, _hex(want), _hex(got)))
            out[hoff:hoff + len(patched_bytes)] = patched_bytes

    log = [
        {"addr": SYNC_FUNC, "offset": off,
         "before": _hex(before[:0x2C]),
         "after": _hex(blob[:0x2C]),
         "desc": "整段改写同步字函数 44B：C.Fn F8 开关 RAM[0x%08X]==1 -> SYNC=KEY，"
                 "==0 -> 50024/(ID+1)；只写 SYNC1/0（无 SCAL、无延时）"
                 % KEY_FLAG_ADDR},
        {"addr": LITERAL_KEY_ADDR, "offset": off + 0x2C,
         "before": _hex(before[0x2C:0x30]),
         "after": _hex(blob[0x2C:0x30]),
         "desc": "字面量池：私有密钥 0x%04X（小端）" % key},
        {"addr": LITERAL_FLAG_ADDR, "offset": off + 0x30,
         "before": _hex(before[0x30:0x34]),
         "after": _hex(blob[0x30:0x34]),
         "desc": "字面量池：密钥开关地址 0x%08X（settings[7]）" % KEY_FLAG_ADDR},
        {"addr": LITERAL_ORIG_ADDR, "offset": off + 0x34,
         "before": _hex(before[0x34:0x38]),
         "after": _hex(blob[0x34:0x38]),
         "desc": "字面量池：50024（与原厂同址同值，普通模式公式用）"},
        {"addr": HOOK_ITEM_GATE, "offset": _off(HOOK_ITEM_GATE),
         "before": _hex(HOOK_ORIG), "after": _hex(HOOK_PATCH),
         "desc": "C.Fn dec 处理函数：bne->blo，item>=6（ID/F8）改值后立即重设同步字"
                 "（F1..F6 与原厂一致）"},
        {"addr": HOOK_ITEM_GATE2, "offset": _off(HOOK_ITEM_GATE2),
         "before": _hex(HOOK_ORIG2), "after": _hex(HOOK_PATCH),
         "desc": "C.Fn inc 处理函数：bne->blo，item>=6（ID/F8）改值后立即重设同步字"
                 "（F1..F6 与原厂一致）"},
    ]
    if full:
        for addr, want, patched_bytes, desc in extra_patches:
            log.append({"addr": addr, "offset": _off(addr),
                        "before": _hex(want), "after": _hex(patched_bytes),
                        "desc": desc})
    return bytes(out), log


# ---------------------------------------------------------------- Unicorn 调用级 harness
def _uc_run_sync_fn(image: bytes, ident: int, flag: int) -> dict:
    """用 Unicorn 执行镜像里 0x0800B9AC 的同步字设置函数。

    先把 RAM[KEY_FLAG_ADDR] 置为 flag（0/1），再把
    rf_write_reg8 / rf_write_cmd / delay_us 入口替换成 `bx lr` 返回桩，
    用代码钩子记录调用参数。
    返回 {"calls": [(kind, a, b), ...], "error": str|None, "insn": int}
    """
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC)

    FLASH_BASE = 0x08000000
    FLASH_SIZE = 0x20000
    RAM_BASE = 0x20000000
    RAM_SIZE = 0x4000
    STACK_TOP = RAM_BASE + 0x1800
    SENTINEL = RAM_BASE + 0x0400

    if len(image) > FLASH_SIZE - (BASE - FLASH_BASE):
        raise ValueError("镜像过大，无法加载")

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)   # Thumb 由入口 bit0 决定
    uc.mem_map(FLASH_BASE, FLASH_SIZE, UC_PROT_ALL)
    uc.mem_map(RAM_BASE, RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    uc.mem_write(KEY_FLAG_ADDR, bytes([flag & 0xFF]))    # F8 密钥开关
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))        # b .
    for addr in (HELPER_REG_WRITE, HELPER_STROBE, HELPER_DELAY):
        uc.mem_write(addr, bytes.fromhex("7047"))        # bx lr

    calls: list[tuple] = []
    state = {"error": None, "insn": 0, "returned": False}

    def hook_code(_uc, address, _size, _user):
        state["insn"] += 1
        if address == SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
            return
        if address == HELPER_REG_WRITE:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        elif address == HELPER_STROBE:
            calls.append(("cmd", _uc.reg_read(UC_ARM_REG_R0), None))
        elif address == HELPER_DELAY:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)   # Thumb 返回地址
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    uc.reg_write(UC_ARM_REG_R1, 0x1234)
    uc.reg_write(UC_ARM_REG_R2, 0x5678)
    uc.reg_write(UC_ARM_REG_R3, 0x9ABC)

    try:
        uc.emu_start(SYNC_FUNC | 1, 0, 0, 20000)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X, lr=0x%08X, sp=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR),
            uc.reg_read(UC_ARM_REG_SP))
    if state["error"] is None and not state["returned"]:
        state["error"] = ("未在 20000 条指令内返回（执行 %d 条，pc=0x%08X），疑似跑飞"
                          % (state["insn"], uc.reg_read(UC_ARM_REG_PC)))
    return {"calls": calls, "error": state["error"], "insn": state["insn"]}


def _format_calls(calls) -> str:
    parts = []
    for kind, a, b in calls:
        if kind == "wreg":
            parts.append("WriteReg(%d,0x%02X)" % (a, b))
        elif kind == "cmd":
            parts.append("WriteCmd(0x%02X)" % a)
        else:
            parts.append("delay(%d)" % a)
    return " -> ".join(parts)


def _check_patched(image: bytes, key: int, ident: int, flag: int) -> tuple[bool, str]:
    """v4 语义：flag==1 -> KEY；flag==0 -> 50024/(ID+1)；只写 SYNC1/0。"""
    res = _uc_run_sync_fn(image, ident, flag)
    if res["error"]:
        return False, "ID=%d flag=%d 补丁版 Unicorn 执行失败: %s" % (ident, flag, res["error"])
    calls = res["calls"]
    expect = key if flag else ID_NUMERATOR // (ident + 1)
    hi, lo = expect >> 8, expect & 0xFF
    want = [("wreg", 4, hi), ("wreg", 5, lo)]
    ok = calls == want
    mode = "密钥模式(F8=ON)" if flag else "原厂公式(F8=OFF)"
    msg = ("ID=%-2d flag=%d %s: sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
           % (ident, flag, mode, expect, hi, lo, _format_calls(calls)))
    if not ok:
        msg += "  期望调用序列 %s" % _format_calls(want)
    if flag:
        public_val = ID_NUMERATOR // (ident + 1)
        ok = ok and expect != public_val
    return ok, msg


def _check_original(image: bytes, ident: int) -> tuple[bool, str]:
    """原厂 harness 自证：应得到 50024/(ID+1)，且包含 SCAL(0x33)+delay(1000)。"""
    res = _uc_run_sync_fn(image, ident, 0)
    if res["error"]:
        return False, "ID=%d 原厂自证 Unicorn 执行失败: %s" % (ident, res["error"])
    calls = res["calls"]
    expect = ID_NUMERATOR // (ident + 1)
    want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF),
            ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
    ok = calls == want
    msg = ("ID=%-2d 原厂(自证): sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
           % (ident, expect, expect >> 8, expect & 0xFF, _format_calls(calls)))
    if not ok:
        msg += "  期望调用序列 %s" % _format_calls(want)
    return ok, msg


# ---------------------------------------------------------------- C.Fn 值修改 harness
def _uc_run_cfn_handler(image: bytes, handler: int, item: int, value: int,
                        ident: int) -> dict:
    """在 Unicorn 里执行 C.Fn 的 +/- 处理函数，模拟"编辑态改值"。

    返回 {"settings": bytes(settings[0..7]), "calls": [...], "error": None}
    """
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC)

    FLASH_BASE = 0x08000000
    FLASH_SIZE = 0x20000
    RAM_BASE = 0x20000000
    RAM_SIZE = 0x4000
    STACK_TOP = RAM_BASE + 0x1800
    SENTINEL = RAM_BASE + 0x0400

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(FLASH_BASE, FLASH_SIZE, UC_PROT_ALL)
    uc.mem_map(RAM_BASE, RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    # UI 状态：C.Fn 打开 + 值编辑态 + 当前项
    uc.mem_write(UI_CFN_OPEN, b"\x01")
    uc.mem_write(UI_EDIT, b"\x01")
    uc.mem_write(UI_ITEM, bytes([item & 0xFF]))
    # settings[]：ID=settings[6]，当前项初值
    uc.mem_write(SETTINGS_BASE, bytes(8))
    uc.mem_write(SETTINGS_BASE + 6, bytes([ident & 0xFF]))
    uc.mem_write(SETTINGS_BASE + (item & 0xFF), bytes([value & 0xFF]))
    uc.mem_write(SENTINEL, bytes.fromhex("fee7"))
    for addr in STUB_HELPERS + (HELPER_REG_WRITE, HELPER_STROBE, HELPER_DELAY):
        uc.mem_write(addr, bytes.fromhex("7047"))     # bx lr

    calls: list[tuple] = []
    stubs = set(STUB_HELPERS)
    state = {"error": None, "returned": False}

    def hook_code(_uc, address, _size, _user):
        if address == SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
            return
        if address == HELPER_REG_WRITE:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))
        elif address == HELPER_STROBE:
            calls.append(("cmd", _uc.reg_read(UC_ARM_REG_R0), None))
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))
        elif address == HELPER_DELAY:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))
        elif address in stubs:
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    for reg, val in ((UC_ARM_REG_R0, 0), (UC_ARM_REG_R1, 0),
                     (UC_ARM_REG_R2, 0), (UC_ARM_REG_R3, 0)):
        uc.reg_write(reg, val)

    try:
        uc.emu_start(handler | 1, 0, 0, 30000)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X, lr=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR))
    if state["error"] is None and not state["returned"]:
        state["error"] = ("处理函数未在 30000 条指令内返回（pc=0x%08X）"
                          % uc.reg_read(UC_ARM_REG_PC))
    try:
        settings = bytes(uc.mem_read(SETTINGS_BASE, 8))
    except Exception:  # noqa: BLE001
        settings = b""
    return {"settings": settings, "calls": calls, "error": state["error"]}


def _expected_sync(flag: int, ident: int, key: int) -> tuple[int, int]:
    sync = key if flag else ID_NUMERATOR // (ident + 1)
    return sync >> 8, sync & 0xFF


def _check_cfn_toggle(image: bytes, key: int, handler: int, item: int, value: int,
                      ident: int, label: str) -> tuple[bool, str]:
    """执行一次 C.Fn 改值，断言 settings[item] 变化 + 立即重设同步字。"""
    res = _uc_run_cfn_handler(image, handler, item, value, ident)
    if res["error"]:
        return False, "%s 执行失败: %s" % (label, res["error"])
    newv = res["settings"][item] if len(res["settings"]) > item else -1
    expect_flag = 1 if newv != 0 else 0
    hi, lo = _expected_sync(expect_flag, ident, key)
    want = [("wreg", 4, hi), ("wreg", 5, lo)]
    ok = (newv == (1 if value == 0 else 0)) and res["calls"] == want
    msg = ("%s: settings[%d] %d -> %d，立即写 SYNC %s（期望 %s）"
           % (label, item, value, newv, _format_calls(res["calls"]),
              _format_calls(want)))
    return ok, msg


def verify(patched: bytes, key: int, full: bool = True) -> list[tuple[bool, str]]:
    """离线验证 v4 补丁：静态字节 + 边界 + 差异范围 + Unicorn 调用级功能测试。

    full=False 时按 `_core` 变体验证（不含稳定色温读者屏蔽/静默清零阻断）。
    """
    results: list[tuple[bool, str]] = []

    # 1) 长度不变
    results.append((len(patched) == ORIGINAL_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁）" % (len(patched), ORIGINAL_SIZE)))
    if len(patched) != ORIGINAL_SIZE:
        return results

    off = _off(SYNC_FUNC)

    # 2) 补丁区 56 字节与期望完全一致
    blob = build_sync_function(key)
    got = patched[off:off + SYNC_FUNC_SIZE]
    results.append((got == blob,
                    "0x%08X..0x%08X 补丁区 %d 字节与期望函数体完全一致"
                    % (SYNC_FUNC, SYNC_FUNC_END - 1, SYNC_FUNC_SIZE) if got == blob else
                    "补丁区字节与期望不符\n  期望 %s\n  实际 %s"
                    % (blob.hex(" "), got.hex(" "))))

    # 3) 不越过下一函数
    results.append((SYNC_FUNC + len(blob) == SYNC_FUNC_END,
                    "函数体结束于 0x%08X == 下一函数起点 0x%08X（不越界）"
                    % (SYNC_FUNC + len(blob), SYNC_FUNC_END)))

    # 4) 字面量落位
    lits = struct.unpack("<III", got[0x2C:0x38])
    lit_ok = (lits[0] == key and lits[1] == KEY_FLAG_ADDR and lits[2] == ID_NUMERATOR)
    results.append((lit_ok,
                    "字面量: KEY=0x%04X @0x%08X, FLAG=0x%08X @0x%08X, 50024 @0x%08X"
                    % (lits[0], SYNC_FUNC + 0x2C, lits[1], SYNC_FUNC + 0x30,
                       SYNC_FUNC + 0x34)))

    # 5) 即时生效 hook 静态字节
    allowed = set(range(off, off + SYNC_FUNC_SIZE))
    for addr, want, pat in ((HOOK_ITEM_GATE, HOOK_ORIG, HOOK_PATCH),
                            (HOOK_ITEM_GATE2, HOOK_ORIG2, HOOK_PATCH)):
        hoff = _off(addr)
        got_h = bytes(patched[hoff:hoff + len(pat)])
        results.append((got_h == pat,
                        "即时生效 hook 0x%08X = %s（期望 %s：bne->blo，item>=6 才重设）"
                        % (addr, _hex(got_h), _hex(pat))))
        allowed |= set(range(hoff, hoff + len(pat)))

    # 5b) full：稳定色温读者屏蔽 + 静默清零阻断 静态字节
    if full:
        for addr, want, patched_bytes, desc in list(READER_MASK_PATCHES) + [SILENT_CLEAR]:
            hoff = _off(addr)
            got_h = bytes(patched[hoff:hoff + len(patched_bytes)])
            results.append((got_h == patched_bytes,
                            "0x%08X = %s（期望 %s）%s"
                            % (addr, _hex(got_h), _hex(patched_bytes), desc)))
            allowed |= set(range(hoff, hoff + len(patched_bytes)))

    # 6) 与原厂差异只落在补丁区
    orig = bytearray(patched)
    orig[off:off + SYNC_FUNC_SIZE] = ORIGINAL_FUNC
    orig[_off(HOOK_ITEM_GATE):_off(HOOK_ITEM_GATE) + len(HOOK_ORIG)] = HOOK_ORIG
    orig[_off(HOOK_ITEM_GATE2):_off(HOOK_ITEM_GATE2) + len(HOOK_ORIG2)] = HOOK_ORIG2
    for addr, want, _pat, _desc in list(READER_MASK_PATCHES) + [SILENT_CLEAR]:
        orig[_off(addr):_off(addr) + len(want)] = want
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    outside = [i for i in diffs if i not in allowed]
    results.append((not outside,
                    "与原厂差异 %d 字节，全部落在补丁区（区外 %d 处）"
                    % (len(diffs), len(outside))))

    # 7) Unicorn 调用级功能测试
    try:
        import unicorn  # noqa: F401
        has_uc = True
    except Exception:  # noqa: BLE001
        has_uc = False
    if not has_uc:
        results.append((True, "警告: unicorn 不可用，跳过 Unicorn 调用级测试（静态检查已通过）"))
        return results

    results.append((True, "Unicorn 调用级测试：从 0x%08X 执行，RAM[0x%08X]=flag，"
                    "桩替换 rf_write_reg8/rf_write_cmd/delay_us"
                    % (SYNC_FUNC | 1, KEY_FLAG_ADDR)))

    # 7a) 密钥模式
    for ident in (0, 5, 99):
        ok, msg = _check_patched(patched, key, ident, flag=1)
        results.append((ok, msg))
    # 7b) 普通模式
    for ident in (0, 1, 5, 37, 99):
        ok, msg = _check_patched(patched, key, ident, flag=0)
        results.append((ok, msg))
    # 7c) 同一 ID 下 flag 切换必须真正改变同步字
    res_on = _uc_run_sync_fn(patched, 5, 1)["calls"]
    res_off = _uc_run_sync_fn(patched, 5, 0)["calls"]
    on_sync = (res_on[0][2] << 8 | res_on[1][2]) if len(res_on) >= 2 else -1
    off_sync = (res_off[0][2] << 8 | res_off[1][2]) if len(res_off) >= 2 else -1
    results.append((on_sync == key and off_sync == ID_NUMERATOR // 6,
                    "同一 ID=5 下 flag=1/0 同步字分别为 0x%04X / 0x%04X（应 0x%04X / 0x%04X）"
                    % (on_sync, off_sync, key, ID_NUMERATOR // 6)))

    # 8) C.Fn F8 改值 -> 立即重设同步字（走真实 +/- 处理函数）
    for handler, label, value, ident in (
            (CFN_INC_HANDLER, "F8 + (OFF->ON)", 0, 5),
            (CFN_INC_HANDLER, "F8 + (ON->OFF, 回绕)", 1, 5),
            (CFN_DEC_HANDLER, "F8 - (ON->OFF)", 1, 5),
            (CFN_DEC_HANDLER, "F8 - (OFF->ON, 回绕)", 0, 37)):
        ok, msg = _check_cfn_toggle(patched, key, handler, 7, value, ident, label)
        results.append((ok, msg))

    # 8b) F7=ID 改值仍然立即重设（不该被 hook 破坏）
    res = _uc_run_cfn_handler(patched, CFN_INC_HANDLER, 6, 5, 5)
    id_ok = False
    msg = "F7(ID) + 改值：%s" % (res["error"] or "ok")
    if not res["error"]:
        new_id = res["settings"][6]
        hi, lo = _expected_sync(0, new_id, key)
        id_ok = (new_id == 6 and res["calls"] == [("wreg", 4, hi), ("wreg", 5, lo)])
        msg = ("F7(ID) + 改值：settings[6] 5 -> %d，立即写 SYNC %s（期望 %s）"
               % (new_id, _format_calls(res["calls"]), _format_calls([("wreg", 4, hi), ("wreg", 5, lo)])))
    results.append((id_ok, msg))

    # 8c) full：稳定色温读者已屏蔽（level_table_lookup_b：F8=1 应返回原厂 F8=OFF 值）
    if full:
        v_on = _uc_level_lookup(patched, flag=1)
        v_off = _uc_level_lookup(patched, flag=0)
        v_orig_on = _uc_level_lookup(_unpatch_all(patched), flag=1)
        ok_reader = (v_on == v_off and v_on != v_orig_on)
        results.append((ok_reader,
                        "稳定色温读者屏蔽：level_table_lookup_b F8=1 -> %s，F8=0 -> %s；"
                        "原厂 F8=1 -> %s（应 补丁两者相等且异于原厂）"
                        % (v_on, v_off, v_orig_on)))

        # 8d) full：ui_key_80 不再静默清 settings[7]
        after_p = _uc_ui_key80(patched)
        after_o = _uc_ui_key80(_unpatch_all(patched))
        results.append((after_p == 1 and after_o == 0,
                        "ui_key_80 模式切换后 settings[7]：补丁=%s（应 1），原厂=%s（应 0）"
                        % (after_p, after_o)))

    # 9) 原厂镜像自证（同一 harness 必须还原原厂公式 + SCAL 序列）
    orig_img = bytearray(patched)
    orig_img[off:off + SYNC_FUNC_SIZE] = ORIGINAL_FUNC
    orig_img[_off(HOOK_ITEM_GATE):_off(HOOK_ITEM_GATE) + len(HOOK_ORIG)] = HOOK_ORIG
    orig_img[_off(HOOK_ITEM_GATE2):_off(HOOK_ITEM_GATE2) + len(HOOK_ORIG2)] = HOOK_ORIG2
    for ident in (0, 1, 5, 37, 99):
        ok, msg = _check_original(bytes(orig_img), ident)
        results.append((ok, msg))

    # 9b) 原厂镜像没有 hook：F8 改值不会重设同步字（自证补丁确实加了立即生效）
    res = _uc_run_cfn_handler(bytes(orig_img), CFN_INC_HANDLER, 7, 0, 5)
    no_hook_ok = (not res["error"]) and res["settings"][7] == 1 and res["calls"] == []
    results.append((no_hook_ok,
                    "原厂(自证)：F8 + 改值 settings[7] 0 -> %s 且不重设同步字（calls=%s）"
                    % (res["settings"][7] if len(res["settings"]) > 7 else "?",
                       _format_calls(res["calls"]))))
    return results


# ---------------------------------------------------------------- 额外 Unicorn 调用级 harness
def _unpatch_all(patched: bytes) -> bytes:
    """把补丁镜像还原成原厂（用于自证对比）：恢复所有补丁点。"""
    buf = bytearray(patched)
    buf[_off(SYNC_FUNC):_off(SYNC_FUNC) + SYNC_FUNC_SIZE] = ORIGINAL_FUNC
    buf[_off(HOOK_ITEM_GATE):_off(HOOK_ITEM_GATE) + len(HOOK_ORIG)] = HOOK_ORIG
    buf[_off(HOOK_ITEM_GATE2):_off(HOOK_ITEM_GATE2) + len(HOOK_ORIG2)] = HOOK_ORIG2
    for addr, want, _pat, _desc in list(READER_MASK_PATCHES) + [SILENT_CLEAR]:
        buf[_off(addr):_off(addr) + len(want)] = want
    return bytes(buf)


def _uc_call(image: bytes, addr: int, pre_ram: dict[int, bytes] | None = None,
             args=()) -> tuple[int, str | None]:
    """极简 Unicorn 调用级 harness：加载镜像，写预置 RAM，调用 addr，返回 r0。

    RET_SENT 处放 `b .`，执行到该地址即停止（用 code hook，避免依赖断点实现）。
    """
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC)

    FLASH_BASE = 0x08000000
    FLASH_SIZE = 0x20000
    RAM_BASE = 0x20000000
    RAM_SIZE = 0x4000
    STACK_TOP = RAM_BASE + 0x1800
    RET_SENT = RAM_BASE + 0x0500

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(FLASH_BASE, FLASH_SIZE, UC_PROT_ALL)
    uc.mem_map(RAM_BASE, RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    if pre_ram:
        for a, b in pre_ram.items():
            uc.mem_write(a, b)
    uc.mem_write(RET_SENT, bytes.fromhex("fee7"))
    state = {"error": None, "returned": False}

    def hook(_uc, address, _size, _user):
        if address == RET_SENT:
            state["returned"] = True
            _uc.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, STACK_TOP)
    uc.reg_write(UC_ARM_REG_LR, RET_SENT | 1)
    for i, v in enumerate(args):
        uc.reg_write(UC_ARM_REG_R0 + i, v)
    try:
        uc.emu_start(addr | 1, 0, 0, 20000)
    except UcError as exc:  # noqa: BLE001
        state["error"] = "%s (pc=0x%08X)" % (exc, uc.reg_read(UC_ARM_REG_PC))
    if state["error"] is None and not state["returned"]:
        state["error"] = "未在 20000 条指令内返回（pc=0x%08X）" % uc.reg_read(UC_ARM_REG_PC)
    return uc.reg_read(UC_ARM_REG_R0), state["error"]


def _uc_level_lookup(image: bytes, flag: int) -> int:
    """调用 level_table_lookup_b(0x0800626C)：返回档位 -> 闪光时长 u16。"""
    pre = {SETTINGS_BASE: bytes(7) + bytes([flag & 0xFF]),
           0x20000166: b"\x00",
           0x20000184: b"\x28",
           0x2000015D: b"\x00"}          # g_mode bit4=0
    r0, err = _uc_call(image, 0x0800626C, pre)
    return r0 if err is None else -1


def _uc_ui_key80(image: bytes) -> int:
    """调用 ui_key_80(0x0800BC34) 触发 g_mode bits4-3 0->1 路径，返回 settings[7]。"""
    from unicorn import Uc  # noqa: F401
    from unicorn.arm_const import UC_ARM_REG_R0  # noqa: F401

    # 复用 _uc_call 后再读 RAM 不方便，这里用一个独立小 harness。
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC)

    FLASH_BASE = 0x08000000
    FLASH_SIZE = 0x20000
    RAM_BASE = 0x20000000
    RAM_SIZE = 0x4000
    RET_SENT = RAM_BASE + 0x0500

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(FLASH_BASE, FLASH_SIZE, UC_PROT_ALL)
    uc.mem_map(RAM_BASE, RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    uc.mem_write(SETTINGS_BASE + 7, b"\x01")     # F8=ON
    uc.mem_write(UI_BASE, bytes(0x30))           # UI 全 0：C.Fn 关、g_mode bits4-3=0
    uc.mem_write(RET_SENT, bytes.fromhex("fee7"))
    returned = {"v": False}

    def hook(_uc, address, _size, _user):
        if address == RET_SENT:
            returned["v"] = True
            _uc.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook)
    uc.reg_write(UC_ARM_REG_SP, RAM_BASE + 0x1800)
    uc.reg_write(UC_ARM_REG_LR, RET_SENT | 1)
    err = None
    try:
        uc.emu_start(0x0800BC34 | 1, 0, 0, 20000)
    except UcError as exc:  # noqa: BLE001
        err = exc
    if err is not None or not returned["v"]:
        return -1
    return uc.mem_read(SETTINGS_BASE + 7, 1)[0]
