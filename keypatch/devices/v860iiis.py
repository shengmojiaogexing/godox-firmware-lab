# -*- coding: utf-8 -*-
"""Godox V860III-S V1.1 —— 私有密钥（CC2500 16 位硬件同步字）补丁模块 —— v1「C.Fn 开关」版。

用户可见语义（v1，与 V1C v5 同一套 C.Fn 开关思路）
-------------------------------------------------
灯端 **C.Fn 菜单第 9 项（LIGHT，第 3 页；原厂造型灯模式 12sec/OFF/ON）** 被复用为
**私有密钥开关**：开关值就是设置区字节 **settings[8] = RAM 0x20001060**（C.Fn 项 8，
随原厂设置自动加载/保存到 flash 参数记录）。

* LIGHT = ON（值 2）      -> CC2500 同步字 = 私有 KEY（只有打了补丁的引闪器能引闪）
* LIGHT = 12sec/OFF（0/1）-> 同步字 = 50024 // (ID + 1)（原厂普通模式，ID 菜单恢复原厂含义）

把原厂「ID -> 16 位同步字」函数 0x080115B8 整段改写成：

    flag = *(uint8_t *)0x20001060;     // C.Fn LIGHT 密钥开关（2=ON）
    if (flag == 2) sync = KEY;         // 密钥模式
    else            sync = 50024 / (id + 1); // 原厂普通模式
    WriteReg(4, sync >> 8);            // SYNC1
    WriteReg(5, sync & 0xFF);          // SYNC0
    return;                            // 只改同步字：无 SCAL、无延时

实现（原位字节补丁，文件长度不变 79,300 B；共 6 处补丁点）
----------------------------------------------------------------
1. **同步字函数整段改写**：0x080115B8 起 **56 字节**（到下一函数 0x080115F0），
   布局同 V1C v5：44 B 代码 + KEY@+0x2C + FLAG_ADDR@+0x30 + 50024@+0x34
   （50024 与原厂同址同值，普通模式公式用）。
2. **改值即时生效**：两条 C.Fn 改值 handler 里的 `cmp r1,#6; bne 跳过 sync`
   只把 2 字节 `bne` 改成 `nop`（00 bf），让除 item9(LCD) 外的任意 C.Fn 项
   改值/导航都落入既有 `ldrb r0,[r0,#6]; bl 0x080115B8`（无需代码洞）：
   * 递减 handler 0x080059E4：0x08005A74 (02 d1 -> 00 bf)
   * 递增 handler 0x0800C528：0x0800C5BE (f5 d1 -> 00 bf)
3. **读者屏蔽 / 标志保护**（见 READER_MASK_PATCHES）：
   * 造型灯任务 0x08003950 的两个 settings[8] 读者固定 0（0x08003956/0x0800397E）；
   * RF 设置复位分支不再覆盖 settings[8]（0x0800D0A2 26 72 -> 00 bf）。

补丁点复核（详见 devices/v860iiis/re/、keypatch/dev/v860iiis/）
--------------------------------------------------------------
* 0x080115B8..0x080115EF（52 B 代码 + 0x080115EC 处 4 B 字面量 50024=0xC368），
  下一函数从 0x080115F0 开始；全镜像 0xC368 字面量只出现一次。
* 辅助函数：__aeabi_uidiv=0x080030F0、CC2500_WriteReg=0x08013926、
  strobe=0x080139E8、delay_us=0x08013698。
* C.Fn 最大值表 0x08016102 = 01 01 01 01 01 1F 63 01 02 06 01 01（12 项；
  item 6=ID/最大 99，item 8=LIGHT/最大 2，item 9=LCD/最大 6；与 V1C 完全一致；
  但本机型 UI 只显示 10 项——item10/11 无菜单入口，见 CFN_MENU.md）。
* 持久化：settings[0..13] 打包进 72 B 记录写入 0x0801F000 起 20 槽（含 settings[8]），
  密钥开关随原厂 C.Fn 设置一起掉电保存。

风险：无实机验证；去掉 SCAL+1ms 是相对原厂唯一射频差异（只改同步字不改频率，
理论上不需要重校准；若实机首包异常优先怀疑此处）。LIGHT 原厂「造型灯模式」功能被
复用后固定为出厂默认（造型灯输出跟随原厂 12sec 档；C.Fn 里的 LIGHT 只剩密钥开关语义）。
换密钥需重新生成并刷入。
"""
from __future__ import annotations

import os
import struct
import sys

# 允许 `import common`（keypatch 目录），与 keypatch.py 的加载方式一致。
_HERE = os.path.dirname(os.path.abspath(__file__))
_KEYPATCH_DIR = os.path.dirname(_HERE)
if _KEYPATCH_DIR not in sys.path:
    sys.path.insert(0, _KEYPATCH_DIR)

import common  # noqa: E402

DEVICE = {
    "id": "v860iiis",
    "name": "Godox V860III-S",
    "kind": "flash",                     # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "V860III-S",
    "firmware_version": "V1.1",
    "image": "devices/v860iiis/firmware/V860IIIS_V1.1.bin",
    "base": 0x08003000,
    "sha256": "bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20",
    "out_name": "V860IIIS_V1.1_key{key:04X}.bin",
    "container_name": "V860IIIS_V1.1_key{key:04X}_container.bin",
    "notes": "C.Fn 第 3 页 LIGHT 密钥开关(settings[8]=0x20001060)：ON(2) -> SYNC=私有 KEY；"
             "12sec/OFF(0/1) -> 原厂 50024/(ID+1)。56 B 整函数改写 + 2 处 NOP gate 即时生效 "
             "+ 2 处造型灯读者屏蔽 + 1 处无线设置复位保护",
}

# ---- 镜像与目标函数 ---------------------------------------------------------
BASE = 0x08003000
IMAGE_SIZE = 79300                # V860IIIS_V1.1.bin 长度

SYNC_FN = 0x080115B8              # void rf_set_id(uint8_t id)：写 CC2500 SYNC1/SYNC0
SYNC_FN_END = 0x080115F0          # 下一函数起点；本补丁必须止步于此
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN   # 56 字节
assert SYNC_FN_SIZE == 56

# 原厂 0x080115B8 起 56 字节（apply 前逐字节校验，防固件版本不对）
ORIG_FUNC = bytes.fromhex(
    "0c4a10b5411c1046f1f796fd0446010a042002f0acf9e1b2052002f0a8f9"
    "142002f05ef8332002f003fa7d20c00002f057f810bd68c30000")
assert len(ORIG_FUNC) == SYNC_FN_SIZE

# ---- C.Fn 设置区 / 开关项 ---------------------------------------------------
SETTINGS_BASE = 0x20001058        # C.Fn 设置项数组基址（item n = base + n）
FLAG_IDX = 8                      # C.Fn 第 9 项（LIGHT，原厂造型灯模式：0=12sec/1=OFF/2=ON）
KEY_FLAG_ADDR = SETTINGS_BASE + FLAG_IDX   # 0x20001060
KEY_FLAG_ON_VALUE = 2             # LIGHT=ON（值 2）-> 密钥模式；0/1 -> 原厂普通模式
ID_IDX = 6                        # item 6 = 引闪 ID（原厂既有）

# C.Fn 项最大值/回绕表（0x08016102，用于静态校验本固件的 item 索引/值域）
CFN_MAX_TABLE_ADDR = 0x08016102
CFN_MAX_TABLE = bytes.fromhex("01010101011f630102060101")
CFN_ITEM_COUNT = 12               # item 0..0xB

# ---- 注入区（本版不需要代码洞：改值即时生效用 2 字节 NOP 实现） -----------
# 原厂两条 gate：cmp r1,#6 后 bne 跳过 sync；把 bne 改 00 bf(nop) 让除 item9(LCD)
# 外的任意 C.Fn 项改值/导航都落入既有的 `ldrb r0,[r0,#6]; bl 0x080115B8`。
# 证据：devices/v860iiis/re/cfn_settings.py + keypatch/dev/v860iiis/CFN_SETTINGS.md。
INSTANT_HOOK_PATCHES = (
    (0x08005A74, b"\x02\xd1", b"\x00\xbf",
     "C.Fn 递减 handler 0x080059E4：bne 跳过 sync -> nop（除 item9 外任意项改值都立即重设同步字）"),
    (0x0800C5BE, b"\xf5\xd1", b"\x00\xbf",
     "C.Fn 递增 handler 0x0800C528：bne 跳过 sync -> nop（同上）"),
)

# 辅助函数（verify 里替换为记录/跳过桩）
DIV_FUNC = 0x080030F0             # __aeabi_uidiv: r0 = r0 / r1
WRITE_REG = 0x08013926            # CC2500_WriteReg(reg=r0, value=r1)
WRITE_CMD = 0x080139E8            # strobe(cmd=r0)（补丁版不再调用 SCAL）
DELAY_US = 0x08013698             # delay_us(n=r0)

# C.Fn handler / 菜单索引（供 handler 级 Unicorn 验证使用）
CFN_DEC_HANDLER = 0x080059E4      # 递减 handler 入口（r0 由内部字面量载入 settings 基址）
CFN_INC_HANDLER = 0x0800C528      # 递增 handler 入口
CFN_INDEX_RAM = 0x200004D6        # 当前 C.Fn item 索引（0..9 菜单可达；10/11 直接改 RAM 也可达）
CFN_EDIT_FLAG = 0x200004D4        # ==1 表示处于 C.Fn 画面
CFN_VALUE_EDIT = 0x200004D5       # ==1 值编辑；==0 光标移动
CFN_BUSY_FLAG = 0x20000536        # ==1 时 handler 提前返回（非改动路径）
CFN_MODE_FLAG = 0x20000563        # ==3 时走 0x0800EAE8 分支
# handler 级 harness 需要的最小桩（入口写 bx lr）：保存倒计时 / 两个 UI 辅助 / LCD 应用 / 递增非编辑分支
CFN_HANDLER_STUBS = (0x08003A48, 0x08011790, 0x080109C8, 0x0800EAE8, 0x080128F0, 0x08012A9E)
CFN_HARNESS_SENTINEL = 0x20001300     # 放 `b .`，handler pop {..,pc} 落点
CFN_HARNESS_SP = 0x20001480           # handler push 24B 后用到 0x20001468..0x2000147B

# 原厂 50024 字面量在补丁区内 +0x34（= 0x080115EC）处，保持原地址/原值
ORIG_LITERAL_OFF = 0x34

# 读者屏蔽（settings[FLAG_IDX] 的原厂功能读者 -> 常数 0，等价原厂 LIGHT=12sec）。
# 每项：(addr, 期望原厂字节, 补丁后字节, 说明)。证据：devices/v860iiis/re/cfn_settings.py
READER_MASK_PATCHES = (
    # 造型灯任务 0x08003950：LIGHT 的两个功能读者固定 0（= 12sec 出厂默认，不驱动造型灯 GPIO）
    (0x08003956, b"\x00\x7a", b"\x00\x20",
     "造型灯任务：settings[8](LIGHT) 读者固定 0（密钥开关不影响造型灯输出）"),
    (0x0800397E, b"\x01\x7a", b"\x00\x21",
     "造型灯任务：settings[8](LIGHT) 第二个读者固定 0（同上）"),
    # RF 消息 handler 0x0800CD7C 的「设置复位」分支会把 settings[0..9] 写成默认值（s8=0），
    # NOP 掉对 settings[8] 的写，保护密钥开关不被远程/无线命令静默清 0。
    (0x0800D0A2, b"\x26\x72", b"\x00\xbf",
     "无线/远程设置复位：不再覆盖 settings[8]（保护 LIGHT 密钥开关）"),
)

# Unicorn 测试用内存布局
UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000             # 128 KiB flash（app 位于 0x08003000）
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x4000              # 初始 SP=0x20001548，多映射一点给栈
UC_STACK = 0x20001400             # 栈顶（8 字节对齐）
UC_SENTINEL = 0x20000400          # 函数返回落点，放 `b .`
MAX_HARNESS_INSNS = 20000         # Unicorn 单次调用指令上限


# ---------------------------------------------------------------------------
# Thumb-1 指令编码
# ---------------------------------------------------------------------------
def _t_ldr_literal(rt: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x4800 | ((rt & 7) << 8) | (imm8 & 0xFF))


def _t_push_r4lr() -> bytes:
    return struct.pack("<H", 0xB510)


def _t_pop_r4pc() -> bytes:
    return struct.pack("<H", 0xBD10)


def _t_movs_imm(rd: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x2000 | ((rd & 7) << 8) | (imm8 & 0xFF))


def _t_mov(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0x4600 | ((rm & 7) << 3) | (rd & 7))


def _t_lsrs(rd: int, rm: int, imm5: int) -> bytes:
    return struct.pack("<H", 0x0800 | ((imm5 & 0x1F) << 6) | ((rm & 7) << 3) | (rd & 7))


def _t_uxtb(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0xB2C0 | ((rm & 7) << 3) | (rd & 7))


def _t_bl(addr: int, target: int) -> bytes:
    """Thumb BL T1 编码（addr = 本 BL 指令地址，target = 绝对地址）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (addr, target, off)
    s = 1 if off < 0 else 0
    i1 = (off >> 23) & 1
    i2 = (off >> 22) & 1
    j1 = (i1 if s else ((~i1) & 1))
    j2 = (i2 if s else ((~i2) & 1))
    imm10 = (off >> 12) & 0x3FF
    imm11 = (off >> 1) & 0x7FF
    return struct.pack("<HH", 0xF000 | (s << 10) | imm10,
                        0xD000 | (j1 << 13) | (j2 << 11) | imm11)


def _ldr_lit_here(rt: int, instr_addr: int, target: int) -> bytes:
    """按当前指令地址直接编码 LDR(literal)，避免依赖汇编器状态。"""
    pc = (instr_addr + 4) & ~3
    imm8 = (target - pc) // 4
    assert (target - pc) % 4 == 0 and 0 <= imm8 <= 0xFF, (hex(target), hex(pc), imm8)
    return _t_ldr_literal(rt, imm8)


# ---------------------------------------------------------------------------
# 补丁体构造
# ---------------------------------------------------------------------------
def build_sync_function(key: int) -> bytes:
    """生成 56 字节替换函数体：44 B 代码 + KEY/FLAG/50024 三个字面量。

    布局（相对 0x080115B8）::

        +00 push {r4, lr}
        +02 ldr  r3, =FLAG(0x20001060)   ; 字面量 +0x30
        +04 ldr  r2, =50024              ; 字面量 +0x34（原厂地址）
        +06 ldrb r3, [r3]                ; flag
        +08 adds r1, r0, #1              ; 普通模式除数 = ID+1
        +0A cmp  r3, #2                  ; LIGHT=ON(值 2)
        +0C bne  go                      ; 非 ON -> 原厂公式
        +0E ldr  r2, =KEY                ; 字面量 +0x2C
        +10 movs r1, #1                  ; 密钥模式除数 = 1
        +12 go: mov r0, r2
        +14 bl   __aeabi_uidiv
        +18 mov  r4, r0
        +1A lsrs r1, r0, #8              ; SYNC1
        +1C movs r0, #4
        +1E bl   WriteReg
        +22 uxtb r1, r4                  ; SYNC0
        +24 movs r0, #5
        +26 bl   WriteReg
        +2A pop  {r4, pc}
        +2C .word KEY
        +30 .word FLAG
        +34 .word 50024                  ; 与原厂同址同值
    """
    key = common.validate_key(key)
    key_lit = SYNC_FN + 0x2C
    flag_lit = SYNC_FN + 0x30
    orig_lit = SYNC_FN + ORIG_LITERAL_OFF
    assert orig_lit + 4 == SYNC_FN_END

    code = bytearray()
    code += _t_push_r4lr()                                  # +00
    code += _ldr_lit_here(3, SYNC_FN + len(code), flag_lit)  # +02
    code += _ldr_lit_here(2, SYNC_FN + len(code), orig_lit)  # +04
    code += struct.pack("<H", 0x781B)                        # +06 ldrb r3,[r3]
    code += struct.pack("<H", 0x1C41)                        # +08 adds r1,r0,#1
    code += struct.pack("<H", 0x2B02)                        # +0A cmp r3,#2 (LIGHT=ON)
    code += struct.pack("<H", 0xD101)                        # +0C bne go（非 ON -> 原厂公式）
    code += _ldr_lit_here(2, SYNC_FN + len(code), key_lit)  # +0E
    code += struct.pack("<H", 0x2101)                        # +10 movs r1,#1
    code += _t_mov(0, 2)                                     # +12 go: mov r0,r2
    code += _t_bl(SYNC_FN + len(code), DIV_FUNC)             # +14
    code += _t_mov(4, 0)                                     # +18
    code += _t_lsrs(1, 0, 8)                                 # +1A
    code += _t_movs_imm(0, 4)                                # +1C
    code += _t_bl(SYNC_FN + len(code), WRITE_REG)            # +1E
    code += _t_uxtb(1, 4)                                    # +22
    code += _t_movs_imm(0, 5)                                # +24
    code += _t_bl(SYNC_FN + len(code), WRITE_REG)            # +26
    code += _t_pop_r4pc()                                    # +2A
    assert len(code) == 44, len(code)
    blob = bytes(code) + struct.pack("<III", key, KEY_FLAG_ADDR, common.ID_NUMERATOR)
    assert len(blob) == SYNC_FN_SIZE, len(blob)
    assert blob[0x34:0x38] == ORIG_FUNC[0x34:0x38]          # 50024 同址同值
    return blob


# ---------------------------------------------------------------------------
# 打补丁
# ---------------------------------------------------------------------------
def _off(addr: int) -> int:
    return addr - BASE


def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """把原厂镜像 data 打上「C.Fn 开关」补丁，返回 (patched_bytes, patch_log)。"""
    key = common.validate_key(key)

    if len(data) != IMAGE_SIZE:
        raise ValueError("Godox V860III-S V1.1 固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
                         % (IMAGE_SIZE, len(data)))

    # 1) 同步字函数整段改写
    off = _off(SYNC_FN)
    before = bytes(data[off:off + SYNC_FN_SIZE])
    if before != ORIG_FUNC:
        raise ValueError(
            "补丁点 0x%08X..0x%08X 原始字节与预期不符（固件版本不符，拒绝打补丁）\n"
            "  期望 %s\n  实际 %s"
            % (SYNC_FN, SYNC_FN_END - 1, ORIG_FUNC.hex(" "), before.hex(" ")))
    blob = build_sync_function(key)

    buf = bytearray(data)
    buf[off:off + SYNC_FN_SIZE] = blob
    log: list[dict] = [{
        "addr": SYNC_FN,
        "offset": off,
        "before": before.hex(" "),
        "after": blob.hex(" "),
        "desc": ("整段改写同步字函数 0x%08X..0x%08X（%d 字节）：C.Fn LIGHT 密钥开关"
                 " RAM[0x%08X]==%d -> SYNC=KEY 0x%04X；否则 -> 50024/(ID+1)；"
                 "只写 SYNC1/0（无 SCAL、无延时）"
                 % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE, KEY_FLAG_ADDR,
                    KEY_FLAG_ON_VALUE, key)),
    }]

    # 2) 改值即时生效：两条 gate 的 bne -> nop（无需代码洞）
    for addr, orig, new, desc in INSTANT_HOOK_PATCHES:
        o = _off(addr)
        got = bytes(buf[o:o + len(orig)])
        if got != orig:
            raise ValueError("即时生效 gate 0x%08X 原始字节与预期不符：期望 %s 实际 %s"
                             % (addr, orig.hex(" "), got.hex(" ")))
        buf[o:o + len(new)] = new
        log.append({"addr": addr, "offset": o,
                    "before": orig.hex(" "), "after": new.hex(" "),
                    "desc": desc})

    # 3) 原厂功能读者屏蔽 / 密钥标志保护
    for addr, orig, new, desc in READER_MASK_PATCHES:
        o = _off(addr)
        got = bytes(buf[o:o + len(orig)])
        if got != orig:
            raise ValueError("读者屏蔽点 0x%08X 原始字节与预期不符：期望 %s 实际 %s"
                             % (addr, orig.hex(" "), got.hex(" ")))
        buf[o:o + len(new)] = new
        log.append({"addr": addr, "offset": o,
                    "before": orig.hex(" "), "after": new.hex(" "),
                    "desc": desc})

    patched = bytes(buf)
    assert len(patched) == len(data)
    return patched, log


# ---------------------------------------------------------------------------
# 离线验证
# ---------------------------------------------------------------------------
def _expected_diff_offsets(key: int) -> set[int]:
    """打补丁后相对原厂镜像应当且只应当变化的文件偏移。"""
    diffs = set()
    blob = build_sync_function(key)
    base = _off(SYNC_FN)
    for i in range(SYNC_FN_SIZE):
        if blob[i] != ORIG_FUNC[i]:
            diffs.add(base + i)
    for addr, orig, new, _desc in INSTANT_HOOK_PATCHES:
        base = _off(addr)
        for i in range(len(orig)):
            if new[i] != orig[i]:
                diffs.add(base + i)
    for addr, orig, new, _desc in READER_MASK_PATCHES:
        base = _off(addr)
        for i in range(len(orig)):
            if new[i] != orig[i]:
                diffs.add(base + i)
    return diffs


def _unpatch(patched: bytes) -> bytes:
    """把补丁区还原成原厂字节（用于原厂 harness 自证）。"""
    buf = bytearray(patched)
    off = _off(SYNC_FN)
    buf[off:off + SYNC_FN_SIZE] = ORIG_FUNC
    for addr, orig, _new, _desc in INSTANT_HOOK_PATCHES:
        o = _off(addr)
        buf[o:o + len(orig)] = orig
    for addr, orig, _new, _desc in READER_MASK_PATCHES:
        o = _off(addr)
        buf[o:o + len(orig)] = orig
    return bytes(buf)


def _uc_run_sync_fn(image: bytes, ident: int, flag: int) -> dict:
    """用 Unicorn 执行 0x080115B8 的同步字设置函数（RAM[KEY_FLAG_ADDR]=flag）。"""
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC)

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE, UC_PROT_ALL)
    uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    uc.mem_write(KEY_FLAG_ADDR, bytes([flag & 0xFF]))
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))    # b .
    for addr in (WRITE_REG, WRITE_CMD, DELAY_US):
        uc.mem_write(addr, bytes.fromhex("7047"))        # bx lr

    calls: list[tuple] = []
    state = {"error": None, "insn": 0, "returned": False}

    def hook_code(_uc, address, _size, _user):
        state["insn"] += 1
        if address == UC_SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
            return
        if address == DIV_FUNC:
            r0 = _uc.reg_read(UC_ARM_REG_R0)
            r1 = _uc.reg_read(UC_ARM_REG_R1)
            _uc.reg_write(UC_ARM_REG_R0, (0 if r1 == 0 else r0 // r1) & 0xFFFFFFFF)
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))
            return
        if address == WRITE_REG:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        elif address == WRITE_CMD:
            calls.append(("cmd", _uc.reg_read(UC_ARM_REG_R0), None))
        elif address == DELAY_US:
            calls.append(("delay", _uc.reg_read(UC_ARM_REG_R0), None))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    uc.reg_write(UC_ARM_REG_R1, 0x1234)
    uc.reg_write(UC_ARM_REG_R2, 0x5678)
    uc.reg_write(UC_ARM_REG_R3, 0x9ABC)

    try:
        uc.emu_start(SYNC_FN | 1, 0, 0, MAX_HARNESS_INSNS)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X, lr=0x%08X, sp=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR),
            uc.reg_read(UC_ARM_REG_SP))
    if state["error"] is None and not state["returned"]:
        state["error"] = ("未在 %d 条指令内返回（执行 %d 条，pc=0x%08X），疑似跑飞"
                          % (MAX_HARNESS_INSNS, state["insn"], uc.reg_read(UC_ARM_REG_PC)))
    return {"calls": calls, "error": state["error"], "insn": state["insn"]}


def _format_calls(calls: list[tuple]) -> str:
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
    """v1 语义：flag==2(LIGHT=ON) -> KEY；flag=0/1(12sec/OFF) -> 50024/(ID+1)。"""
    res = _uc_run_sync_fn(image, ident, flag)
    if res["error"]:
        return False, "ID=%d flag=%d 补丁版 Unicorn 执行失败: %s" % (ident, flag, res["error"])
    calls = res["calls"]
    key_mode = (flag == KEY_FLAG_ON_VALUE)
    expect = key if key_mode else common.ID_NUMERATOR // (ident + 1)
    hi, lo = expect >> 8, expect & 0xFF
    want = [("wreg", 4, hi), ("wreg", 5, lo)]
    ok = calls == want
    mode = "密钥模式(LIGHT=ON)" if key_mode else "原厂公式"
    msg = ("ID=%-2d flag=%d %s: sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
           % (ident, flag, mode, expect, hi, lo, _format_calls(calls)))
    if not ok:
        msg += "  期望调用序列 %s" % _format_calls(want)
    if key_mode:
        ok = ok and expect != common.ID_NUMERATOR // (ident + 1)
    return ok, msg


def _check_original(image: bytes, ident: int) -> tuple[bool, str]:
    """原厂 harness 自证：应得到 50024/(ID+1) + delay(20)+SCAL(0x33)+delay(1000)。"""
    res = _uc_run_sync_fn(image, ident, 0)
    if res["error"]:
        return False, "ID=%d 原厂自证 Unicorn 执行失败: %s" % (ident, res["error"])
    calls = res["calls"]
    expect = common.ID_NUMERATOR // (ident + 1)
    want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF),
            ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
    ok = calls == want
    msg = ("ID=%-2d 原厂(自证): sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
           % (ident, expect, expect >> 8, expect & 0xFF, _format_calls(calls)))
    if not ok:
        msg += "  期望调用序列 %s" % _format_calls(want)
    return ok, msg


def _uc_run_cfn_handler(image: bytes, handler: int, item: int, mode: str,
                        value: int) -> dict:
    """用 Unicorn 真实执行 C.Fn 改值 handler（值编辑模式），记录是否调用同步字函数。

    现场由 devices/v860iiis/re/cfn_settings.py 逐项核实（最小桩集合、RAM 预置、恢复断言）：
      * settings 基址 0x20001058 起 12 B：settings[item]=value；item!=6 时 settings[6]=ID=50；
      * 0x200004D4=1(C.Fn 画面)、0x200004D5=1(值编辑)、0x200004D6=item、0x20000536=0、0x20000563=0；
      * 6 个 UI/显示辅助入口写 bx lr；sync 入口 0x080115B8 写 bx lr 并在 code hook 里记 r0；
      * SP=0x20001480、LR=0x20001301（哨兵 `b .`）、r0=settings 基址、r1=item、r3..r7=0x1003..0x1007。
    """
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R3,
                                   UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6,
                                   UC_ARM_REG_R7, UC_ARM_REG_SP, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC, UC_ARM_REG_CPSR)

    ident = 50
    settings = bytearray(CFN_ITEM_COUNT)
    settings[0:CFN_ITEM_COUNT] = bytes(CFN_ITEM_COUNT)
    if item != ID_IDX:
        settings[ID_IDX] = ident
    settings[item] = value & 0xFF

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE, UC_PROT_ALL)
    uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    uc.mem_write(SETTINGS_BASE, bytes(settings))
    uc.mem_write(CFN_HARNESS_SENTINEL, bytes.fromhex("fee7"))   # b .
    for addr in CFN_HANDLER_STUBS:
        uc.mem_write(addr, bytes.fromhex("7047"))               # bx lr
    uc.mem_write(SYNC_FN, bytes.fromhex("7047"))                # bx lr（只记录调用）
    uc.mem_write(CFN_EDIT_FLAG, b"\x01")
    uc.mem_write(CFN_VALUE_EDIT, b"\x01")
    uc.mem_write(CFN_INDEX_RAM, bytes([item & 0xFF]))
    uc.mem_write(CFN_BUSY_FLAG, b"\x00")
    uc.mem_write(CFN_MODE_FLAG, b"\x00")

    sync_calls: list[int] = []
    state = {"returned": False, "error": None, "insn": 0}

    def hook_code(_uc, address, _size, _user):
        state["insn"] += 1
        if address == CFN_HARNESS_SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
        elif address == SYNC_FN:
            sync_calls.append(_uc.reg_read(UC_ARM_REG_R0))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, CFN_HARNESS_SP)
    uc.reg_write(UC_ARM_REG_LR, CFN_HARNESS_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_CPSR, 0x01000000)
    uc.reg_write(UC_ARM_REG_R0, SETTINGS_BASE)
    uc.reg_write(UC_ARM_REG_R1, item)
    uc.reg_write(UC_ARM_REG_R3, 0x1003)
    uc.reg_write(UC_ARM_REG_R4, 0x1004)
    uc.reg_write(UC_ARM_REG_R5, 0x1005)
    uc.reg_write(UC_ARM_REG_R6, 0x1006)
    uc.reg_write(UC_ARM_REG_R7, 0x1007)

    try:
        uc.emu_start(handler | 1, 0, 0, 2000)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X, sp=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_SP))
    after = bytes(uc.mem_read(SETTINGS_BASE, CFN_ITEM_COUNT))
    return {
        "sync": sync_calls,
        "returned": state["returned"],
        "error": state["error"],
        "insn": state["insn"],
        "settings": after,
        "sp": uc.reg_read(UC_ARM_REG_SP),
        "r3": uc.reg_read(UC_ARM_REG_R3),
        "r4": uc.reg_read(UC_ARM_REG_R4),
        "r5": uc.reg_read(UC_ARM_REG_R5),
        "r6": uc.reg_read(UC_ARM_REG_R6),
        "r7": uc.reg_read(UC_ARM_REG_R7),
    }


def _check_cfn_handler(image: bytes, handler: int, item: int, mode: str, value: int,
                       expect_sync: int, expect_value: int | None,
                       expect_before: int | None = None) -> tuple[bool, str]:
    """断言某个 C.Fn 项改值后：sync 调用次数、sync 入参、新值、寄存器/栈恢复。"""
    res = _uc_run_cfn_handler(image, handler, item, mode, value)
    if res["error"]:
        return False, "handler=%s item=%d %s=%d Unicorn 执行失败: %s" % (
            "dec" if handler == CFN_DEC_HANDLER else "inc", item, mode, value, res["error"])
    ok = True
    details = []
    # sync 次数与入参
    want_sync = expect_sync
    ok = ok and (len(res["sync"]) == want_sync)
    if want_sync == 1:
        ident_after = res["settings"][ID_IDX]
        ok = ok and (res["sync"][0] == ident_after)
        details.append("sync(r0=%d)" % res["sync"][0])
    details.append("sync 次数=%d(期望 %d)" % (len(res["sync"]), want_sync))
    # 运行后 settings：目标项新值、其它字节不变
    if expect_value is not None:
        ok = ok and (res["settings"][item] == expect_value)
        details.append("settings[%d]=%d(期望 %d)" % (item, res["settings"][item], expect_value))
        for idx in range(CFN_ITEM_COUNT):
            if idx == item:
                continue
            want = (expect_value if item == ID_IDX else 50) if idx == ID_IDX else 0
            if res["settings"][idx] != want:
                ok = False
                details.append("settings[%d]=%d(期望 %d)" % (idx, res["settings"][idx], want))
                break
    # 返回 / 栈 / r3..r7 恢复
    ok = ok and res["returned"] and res["sp"] == CFN_HARNESS_SP
    regs_ok = (res["r3"], res["r4"], res["r5"], res["r6"], res["r7"]) == (
        0x1003, 0x1004, 0x1005, 0x1006, 0x1007)
    ok = ok and regs_ok
    details.append("返回=%s SP=0x%08X r3-r7=%s" % (
        "是" if res["returned"] else "否", res["sp"], "OK" if regs_ok else "BROKEN"))
    return ok, "C.Fn handler item=%-2d %s %s: %s" % (
        item, mode, ("sync 已触发" if want_sync else "sync 未触发"), "，".join(details))


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证：静态字节/边界/差异范围 + Unicorn 同步字/原厂自证 + C.Fn handler 级。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    # 1) 长度不变
    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁）" % (len(patched), IMAGE_SIZE)))

    # 2) 同步字函数 56 字节与期望完全一致
    blob = build_sync_function(key)
    off = _off(SYNC_FN)
    got = bytes(patched[off:off + SYNC_FN_SIZE])
    results.append((got == blob,
                    "0x%08X..0x%08X 补丁区 %d 字节与期望函数体完全一致"
                    % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE) if got == blob else
                    "补丁区字节与期望不符\n  期望 %s\n  实际 %s"
                    % (blob.hex(" "), got.hex(" "))))

    # 3) 不越过下一函数
    results.append((SYNC_FN + len(blob) == SYNC_FN_END,
                    "函数体结束于 0x%08X == 下一函数起点 0x%08X（不越界）"
                    % (SYNC_FN + len(blob), SYNC_FN_END)))

    # 4) 字面量：+0x2C=KEY、+0x30=FLAG、+0x34=50024（原厂同址同值）
    lits = (int.from_bytes(got[0x2C:0x30], "little"),
            int.from_bytes(got[0x30:0x34], "little"),
            int.from_bytes(got[0x34:0x38], "little"))
    results.append((lits == (key, KEY_FLAG_ADDR, common.ID_NUMERATOR),
                    "字面量: KEY=0x%04X @0x%08X, FLAG=0x%08X @0x%08X, 50024 @0x%08X"
                    % (lits[0], SYNC_FN + 0x2C, lits[1], SYNC_FN + 0x30, SYNC_FN + 0x34)))

    # 5) 改值即时生效 gate（bne -> nop）
    for addr, orig, new, desc in INSTANT_HOOK_PATCHES:
        o = _off(addr)
        gotg = bytes(patched[o:o + len(new)])
        results.append((gotg == new,
                        "即时生效 gate 0x%08X = %s（%s）" % (addr, gotg.hex(" "), desc)
                        if gotg == new else
                        "即时生效 gate 0x%08X 不符：期望 %s 实际 %s"
                        % (addr, new.hex(" "), gotg.hex(" "))))

    # 6) 读者屏蔽 / 开关保护
    for addr, orig, new, desc in READER_MASK_PATCHES:
        o = _off(addr)
        gotr = bytes(patched[o:o + len(new)])
        results.append((gotr == new, "读者屏蔽 0x%08X = %s（%s）" % (addr, gotr.hex(" "), desc)))

    # 7) 与原厂差异必须恰好只落在补丁点集合内
    orig = _unpatch(patched)
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    expected = _expected_diff_offsets(key)
    results.append((set(diffs) == expected,
                    "与原厂差异 %d 字节，offset=%s（应恰为 %s）"
                    % (len(diffs), ["0x%05X" % d for d in diffs],
                       ["0x%05X" % d for d in sorted(expected)])))
    outside = [i for i in diffs if i not in expected]
    results.append((not outside, "差异之外无任何字节变化（%d 处）" % len(outside)))

    # 7) Unicorn 调用级
    try:
        import unicorn
        has_uc = True
    except Exception:  # noqa: BLE001
        has_uc = False
    if not has_uc:
        results.append((True, "警告: unicorn 不可用，跳过 Unicorn 调用级测试（静态检查已通过）"))
        return results

    results.append((True, "Unicorn %s 调用级测试：同步字 0x%08X，RAM[0x%08X]=flag"
                    % (getattr(unicorn, "__version__", "?"), SYNC_FN | 1, KEY_FLAG_ADDR)))
    for ident in (0, 5, 99):
        results.append(_check_patched(patched, key, ident, flag=KEY_FLAG_ON_VALUE))
    for flag in (0, 1):
        for ident in (0, 1, 5, 37, 99):
            results.append(_check_patched(patched, key, ident, flag=flag))
    for ident in (0, 1, 5, 37, 99):
        results.append(_check_original(orig, ident))

    # 8) C.Fn handler 级（值编辑模式，0x200004D5=1）：验证「除 item9 外任意项改值都立即重设同步字」
    results.append((True, "C.Fn handler 级：dec=0x%08X / inc=0x%08X，item 索引 RAM 0x%08X"
                    % (CFN_DEC_HANDLER, CFN_INC_HANDLER, CFN_INDEX_RAM)))
    maxes = CFN_MAX_TABLE
    for item in range(CFN_ITEM_COUNT):
        if item == 9:      # LCD 走专用函数 0x080128F0，原厂就不重设同步字，补丁后也不应触发
            continue
        mx = maxes[item]
        results.append(_check_cfn_handler(patched, CFN_DEC_HANDLER, item, "dec", 0, 1, mx))
        results.append(_check_cfn_handler(patched, CFN_INC_HANDLER, item, "inc", mx, 1, 0))
    for item in (5, 6, 8):       # 非回绕路径：dec(max)->max-1 / inc(0)->1
        mx = maxes[item]
        results.append(_check_cfn_handler(patched, CFN_DEC_HANDLER, item, "dec", mx, 1, mx - 1))
        results.append(_check_cfn_handler(patched, CFN_INC_HANDLER, item, "inc", 0, 1, 1))
    results.append(_check_cfn_handler(patched, CFN_DEC_HANDLER, 9, "dec", 3, 0, None))
    results.append(_check_cfn_handler(patched, CFN_INC_HANDLER, 9, "inc", 3, 0, None))
    # 原厂自证：只有 item6(ID) 改值会重设同步字
    for item in range(CFN_ITEM_COUNT):
        exp = 1 if item == ID_IDX else 0
        results.append(_check_cfn_handler(orig, CFN_DEC_HANDLER, item, "dec", 0, exp, None))
    return results
