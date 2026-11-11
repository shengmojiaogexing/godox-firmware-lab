# -*- coding: utf-8 -*-
"""Godox V1S V1.8 —— 私有密钥（CC2500 16 位硬件同步字）补丁模块 —— 「C.Fn LIGHT 开关」版。

V1S = Godox V1 的索尼（Sony）版，与已支持的 V1C（V2.0）属同一固件家族
（Cortex-M0 / Thumb-1、镜像基址 0x08003000、相同的 CC2500 同步字公式与 C.Fn 菜单结构），
但代码/地址整体不同：本模块的所有补丁点均在 `V1S_V1.8.bin`（78,500 B，
sha256 87a06000…）上重新定位并逐字节校验。

用户可见语义
------------
灯端 **C.Fn 菜单第 9 项（LIGHT，第 3 页；原厂造型灯模式）** 被复用为**私有密钥开关**：
开关值就是设置区字节 **settings[8] = RAM 0x2000105C**（item 8，随原厂 C.Fn 设置自动
加载/保存到 flash 参数记录；默认 0）。

* LIGHT = ON（值 2）   -> CC2500 同步字 = 私有 KEY（只有打了补丁的引闪器能引闪）
* LIGHT = 12sec/OFF（0/1）-> 同步字 = 50024 // (ID + 1)（原厂普通模式，ID 菜单恢复原厂含义）

把原厂「ID -> 16 位同步字」函数 0x080112DC 整段改写成：

    flag = *(uint8_t *)0x2000105C;     // C.Fn LIGHT 密钥开关（2=ON）
    if (flag == 2) sync = KEY;         // 密钥模式
    else            sync = 50024 / (id + 1); // 原厂普通模式
    WriteReg(4, sync >> 8);            // SYNC1
    WriteReg(5, sync & 0xFF);          // SYNC0
    return;                            // 只改同步字：无 SCAL、无延时

实现（原位字节补丁，文件长度不变 78,500 B）
------------------------------------------
1. **同步字函数整段改写**：0x080112DC 起 **56 字节**（到下一函数 0x08011314），
   布局同 V1C：44 B 代码 + KEY@+0x2C + FLAG_ADDR@+0x30 + 50024@+0x34
   （50024 与原厂同址同值，普通模式公式用）。
2. **改值即时生效**：原厂只有改 C.Fn item6(ID) 才重设同步字
   （递减处理 0x08005A8A、递增处理 0x0800C3E8 两处 `cmp r1,#6; bne` 门）。
   本补丁把这两处 4 字节门改成 `b <原厂同步块>; nop`（`00 e0 00 bf`）：门后紧邻的就是
   `ldrb r0,[r0,#6]; bl sync_fn`（0x08005A8E / 0x0800C3EC），
   因此**除 item9(LCD) 外的 C.Fn 改值**（含 ID 与 LIGHT）都会立即重设同步字；
   不需要代码洞注入、不需要额外 hook。
3. **读者屏蔽**：造型灯任务（0x08003950）里 settings[8] 的两个功能读者
   （0x08003956 / 0x0800397E）改成常数 0（等价原厂 LIGHT=12sec 出厂默认），
   用户把 LIGHT 设为 1/2 当密钥开关时不再驱动造型灯。见 READER_MASK_PATCHES。

不做的事（有意为之）：**不屏蔽出厂复位/恢复默认路径**
（`0x08005E68` 出厂默认函数写 settings[8]=0、`0x0800CE9C..0x0800CEB4` 按键工厂复位组合）。
V1S 的 C.Fn 设置区**没有** V1C 那种「无线设置同步解包写回 settings[8]」的远程覆盖路径
（V1C 的 0x0800A6FA 在本固件无对应，见 devices/v1s/re/CFN_ANALYSIS.md §7），
所以密钥开关不需要额外的防覆盖补丁；保留「恢复默认 = 回到原厂普通模式」这条重要恢复路径
（忘记密钥时可用出厂复位把灯恢复成原厂行为）。
------------------------------------------------------------------------------
* 同步字函数 0x080112DC..0x08011313（52 B 代码 + 0x08011310 处 4 B 字面量 50024=0xC368），
  下一函数从 0x08011314 开始；全镜像 0xC368 字面量只出现一次。
* 辅助函数：__aeabi_uidiv=0x080030F0（与 V1C 相同）、CC2500_WriteReg=0x0801365A、
  strobe=0x0801371C、delay_us=0x080133CC。
* C.Fn 设置区基址 **0x20001054**（item n = base + n；ID=item6=0x2000105A、
  LIGHT=item8=0x2000105C）；最大值/回绕表 **0x08015DE6** =
  `01 01 01 01 01 1F 63 01 02 06 01 01`（12 项，item6 max=99、item8 max=2）；
  出厂默认函数 0x08005E68（item8 默认 0）；保存记录 builder 0x080058 xx；
  上电 load/展开约 0x080138 xx（record+0x34 的第 3 字节 -> settings[8]）。

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
    "id": "v1s",
    "name": "Godox V1S",
    "kind": "flash",                     # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "V1S",
    "firmware_version": "V1.8",
    "image": "devices/v1s/firmware/V1S_V1.8.bin",
    "base": 0x08003000,
    "sha256": "87a0600046e1b7c2198a7ac8fac15959bb3a5d582edf44d8e0df38706dcb917a",
    "out_name": "V1S_V1.8_key{key:04X}.bin",
    "container_name": "V1S_V1.8_key{key:04X}_container.bin",
    "notes": "C.Fn 第 3 页 LIGHT 密钥开关(settings[8]=0x2000105C)：ON(2) -> SYNC=私有 KEY；"
             "12sec/OFF(0/1) -> 原厂 50024/(ID+1)。56 B 整函数改写 + 2 个 C.Fn 改值门改 `b 同步块`（即时生效）"
             "+ 造型灯读者屏蔽（出厂复位仍回到普通模式）",
}

# ---- 镜像与目标函数 ---------------------------------------------------------
BASE = 0x08003000
IMAGE_SIZE = 78500                # V1S_V1.8.bin 长度

SYNC_FN = 0x080112DC              # void rf_set_id(uint8_t id)：写 CC2500 SYNC1/SYNC0
SYNC_FN_END = 0x08011314          # 下一函数起点；本补丁必须止步于此
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN   # 56 字节
assert SYNC_FN_SIZE == 56

# 原厂 0x080112DC 起 56 字节（apply 前逐字节校验，防固件版本不对）
ORIG_FUNC = bytes.fromhex(
    "0c4a10b5411c1046f1f704ff0446010a042002f0b4f9e1b2052002f0b0f9"
    "142002f066f8332002f00bfa7d20c00002f05ff810bd68c30000")
assert len(ORIG_FUNC) == SYNC_FN_SIZE

# ---- C.Fn 设置区 / 开关项 ---------------------------------------------------
SETTINGS_BASE = 0x20001054        # C.Fn 设置项数组基址（item n = base + n）
FLAG_IDX = 8                      # C.Fn 第 9 项（LIGHT，原厂造型灯模式：0=12sec/1=OFF/2=ON）
KEY_FLAG_ADDR = SETTINGS_BASE + FLAG_IDX   # 0x2000105C
KEY_FLAG_ON_VALUE = 2             # LIGHT=ON（值 2）-> 密钥模式；0/1 -> 原厂普通模式
ID_IDX = 6                        # item 6 = 引闪 ID（原厂既有）

# C.Fn 项最大值/回绕表（0x08015DE6，用于静态校验本固件的 item 索引/值域）
CFN_MAX_TABLE_ADDR = 0x08015DE6
CFN_MAX_TABLE = bytes.fromhex("01010101011f630102060101")
CFN_ITEM_COUNT = 12               # item 0..0xB

# 两个「item==6(ID) 才重设同步字」的 gate（cmp r1,#6; bne <跳过同步字>，各 4 字节）。
# 门后紧邻 sync 调用（ldrb r0,[r0,#6]; bl 0x080112DC），所以把门改成
# `b <同步块>; nop`（00 e0 00 bf）即可让「除 item9(LCD) 外」的所有 C.Fn 改值立即重设同步字。
# （不采用 V1C 式 bl 注入：本固件 gate 的 fall-through 就是同步块，bl hook 会重复调用。）
GATE_DEC = 0x08005A8A             # C.Fn 递减处理（0x080059FC 起）内
GATE_INC = 0x0800C3E8             # C.Fn 递增处理（0x0800C354 起）内
GATE_ORIG_DEC = bytes.fromhex("062902d1")
GATE_ORIG_INC = bytes.fromhex("0629f5d1")
GATE_NOP = bytes.fromhex("00e000bf")  # 0x08005A8A: b 0x08005A8E; nop（无条件进原厂同步块）

# 辅助函数（verify 里替换为记录/跳过桩）
DIV_FUNC = 0x080030F0             # __aeabi_uidiv: r0 = r0 / r1
WRITE_REG = 0x0801365A            # CC2500_WriteReg(reg=r0, value=r1)
WRITE_CMD = 0x0801371C            # strobe(cmd=r0)（补丁版不再调用 SCAL）
DELAY_US = 0x080133CC             # delay_us(n=r0)

# 原厂 50024 字面量在补丁区内 +0x34（= 0x08011310）处，保持原地址/原值
ORIG_LITERAL_OFF = 0x34

# 读者屏蔽，每项：(addr, 期望原厂字节, 补丁后字节, 说明)。
# 证据见 devices/v1s/re/CFN_ANALYSIS.md §4.2/§7 与 keypatch/dev/v1s/NOTES.md
READER_MASK_PATCHES = (
    # 造型灯任务 0x08003950：settings[8] 的第一个读者（cmp #1 分支）固定 0 =
    # 原厂出厂默认（LIGHT=0，12sec 档；密钥开关不再驱动造型灯输出）。
    (0x08003956, b"\x00\x7a", b"\x00\x20",
     "造型灯任务：settings[8](LIGHT) 读者固定 0（ldrb r0,[r0,#8] -> movs r0,#0）"),
    # 同一函数的第二个读者（cmp #2 = ON 分支）。
    (0x0800397E, b"\x01\x7a", b"\x00\x21",
     "造型灯任务：settings[8](LIGHT) 第二个读者固定 0（ldrb r1,[r0,#8] -> movs r1,#0）"),
)

# Unicorn 测试用内存布局
UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000             # 128 KiB flash（app 位于 0x08003000）
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x4000              # V1S 初始 SP=0x20001540，多映射一点给栈
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

    布局（相对 0x080112DC）::

        +00 push {r4, lr}
        +02 ldr  r3, =FLAG(0x2000105C)   ; 字面量 +0x30
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
    """把原厂镜像 data 打上「C.Fn LIGHT 开关」补丁，返回 (patched_bytes, patch_log)。"""
    key = common.validate_key(key)

    if len(data) != IMAGE_SIZE:
        raise ValueError("Godox V1S V1.8 固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
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
                 " RAM[0x%08X]==2 -> SYNC=KEY 0x%04X；==0/1 -> 50024/(ID+1)；"
                 "只写 SYNC1/0（无 SCAL、无延时）"
                 % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE, KEY_FLAG_ADDR, key)),
    }]

    # 2) 两个 C.Fn 改值门 -> `b 同步块; nop`（除 item9(LCD) 外改值都立即重设同步字）
    for site, expect, name, fallthrough in (
            (GATE_DEC, GATE_ORIG_DEC, "C.Fn 递减", 0x08005A8E),
            (GATE_INC, GATE_ORIG_INC, "C.Fn 递增", 0x0800C3EC)):
        o = _off(site)
        got = bytes(buf[o:o + 4])
        if got != expect:
            raise ValueError("gate 0x%08X（%s）原始字节与预期不符：期望 %s 实际 %s"
                             % (site, name, expect.hex(" "), got.hex(" ")))
        after = bytes(buf[o + 4:o + 6])
        if after != b"\x80\x79":   # ldrb r0,[r0,#6]（门后紧邻按 settings[6] 调 sync）
            raise ValueError("gate 0x%08X（%s）门后指令不是 ldrb r0,[r0,#6]：%s"
                             % (site, name, after.hex(" ")))
        buf[o:o + 4] = GATE_NOP
        log.append({
            "addr": site, "offset": o,
            "before": expect.hex(" "), "after": GATE_NOP.hex(" "),
            "desc": ("gate(%s)：cmp r1,#6; bne -> b 0x%08X; nop（无条件进原厂同步块 ldrb r0,[r0,#6]; "
                     "bl sync，除 item9(LCD) 外 C.Fn 改值立即重设同步字）" % (name, fallthrough)),
        })

    # 3) 造型灯读者屏蔽 / 密钥开关保护
    for addr, orig, new, desc in READER_MASK_PATCHES:
        o = _off(addr)
        got = bytes(buf[o:o + len(orig)])
        if got != orig:
            raise ValueError("读者屏蔽点 0x%08X 原始字节与预期不符：期望 %s 实际 %s"
                             % (addr, orig.hex(" "), got.hex(" ")))
        buf[o:o + len(new)] = new
        log.append({
            "addr": addr, "offset": o,
            "before": orig.hex(" "), "after": new.hex(" "),
            "desc": desc,
        })

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
    for site, expect, _name in ((GATE_DEC, GATE_ORIG_DEC, "dec"),
                                (GATE_INC, GATE_ORIG_INC, "inc")):
        base = _off(site)
        for i in range(4):
            if GATE_NOP[i] != expect[i]:
                diffs.add(base + i)
    for addr, orig, new, _desc in READER_MASK_PATCHES:
        base = _off(addr)
        for i in range(len(orig)):
            if new[i] != orig[i]:
                diffs.add(base + i)
    return diffs


def _unpatch(patched: bytes) -> bytes:
    """把补丁区还原成原厂字节（用于原厂 harness 自证 / 差异统计）。"""
    buf = bytearray(patched)
    off = _off(SYNC_FN)
    buf[off:off + SYNC_FN_SIZE] = ORIG_FUNC
    for site, expect, _name in ((GATE_DEC, GATE_ORIG_DEC, "dec"),
                                (GATE_INC, GATE_ORIG_INC, "inc")):
        o = _off(site)
        buf[o:o + 4] = expect
    for addr, orig, _new, _desc in READER_MASK_PATCHES:
        o = _off(addr)
        buf[o:o + len(orig)] = orig
    return bytes(buf)


def _uc_run_sync_fn(image: bytes, ident: int, flag: int) -> dict:
    """用 Unicorn 执行 0x080112DC 的同步字设置函数（RAM[KEY_FLAG_ADDR]=flag）。"""
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


def _uc_run_gate(image: bytes, which: str, item: int, ident: int) -> dict:
    """执行 C.Fn 改值门之后的一小段（门补丁 -> ldrb r0,[r0,#6] -> bl sync），记录 sync 实参。

    模拟现场（dec/inc handler 在 gate 处）：
      * dec handler 0x08005A88: `ldrb r1,[r2]`（r2 = 当前项指针 0x200004D0），之后是门；
      * inc handler 0x0800C3E6: `ldrb r1,[r1]`（r1 = 当前项指针 0x200004D0），之后是门。
    两处 r0 均为 settings 基址。若门补丁正确，无论 item 是否为 6(ID)，都会落到
    `ldrb r0,[r0,#6]; bl 0x080112DC` —— 这正是「C.Fn 改值立即重设同步字」的行为级证据。
    """
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC)

    # 当前选中项指针（原厂 dec/inc handler 都从 0x200004D0 取 item 索引）
    ITEM_PTR = 0x200004D0
    start = (GATE_DEC - 2) if which == "dec" else (GATE_INC - 2)

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE, UC_PROT_ALL)
    uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    uc.mem_write(SETTINGS_BASE + ID_IDX, bytes([ident & 0xFF]))
    uc.mem_write(ITEM_PTR, bytes([item & 0xFF]))
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))    # b .

    state = {"sync": [], "returned": False, "error": None}

    def hook_code(_uc, address, _size, _user):
        if address == UC_SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
        elif address == SYNC_FN:
            state["sync"].append(_uc.reg_read(UC_ARM_REG_R0))
            _uc.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_R0, SETTINGS_BASE)
    uc.reg_write(UC_ARM_REG_R1, ITEM_PTR)
    uc.reg_write(UC_ARM_REG_R2, ITEM_PTR)
    try:
        uc.emu_start(start | 1, 0, 0, 2000)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X)" % (exc, uc.reg_read(UC_ARM_REG_PC))
    return state


def _check_gate(image: bytes, which: str, item: int, ident: int) -> tuple[bool, str]:
    """门补丁行为：无论 item 是否为 6(ID)，都调用一次 sync，且实参 = settings[6] = ID。"""
    r = _uc_run_gate(image, which, item, ident)
    ok = (r["error"] is None and r["sync"] == [ident])
    name = "C.Fn 递减" if which == "dec" else "C.Fn 递增"
    msg = ("%s门(item=%d, ID=%d) -> sync 调用实参 %s（期望 [%d]）%s"
           % (name, item, ident, r["sync"], ident,
              "" if r["error"] is None else "，错误: %s" % r["error"]))
    return ok, msg


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
    """语义：flag==2(LIGHT=ON) -> KEY；flag=0/1(12sec/OFF) -> 50024/(ID+1)。"""
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


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证：静态字节/边界/差异范围 + Unicorn 同步字语义 + 原厂自证。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    # 1) 长度不变
    results.append((len(patched) == IMAGE_SIZE,
                    "固件长度 %d 字节（应为 %d，原位补丁）" % (len(patched), IMAGE_SIZE)))

    # 2) max 表 / item 索引静态确认（防拿到不同版本固件时悄悄放过）
    off_tab = _off(CFN_MAX_TABLE_ADDR)
    got_tab = bytes(patched[off_tab:off_tab + CFN_ITEM_COUNT])
    results.append((got_tab == CFN_MAX_TABLE,
                    "C.Fn 最大值表 0x%08X = %s（12 项；item6=ID max=99, item8=LIGHT max=2）"
                    % (CFN_MAX_TABLE_ADDR, got_tab.hex(" "))))

    # 3) 同步字函数 56 字节与期望完全一致
    blob = build_sync_function(key)
    off = _off(SYNC_FN)
    got = bytes(patched[off:off + SYNC_FN_SIZE])
    results.append((got == blob,
                    "0x%08X..0x%08X 补丁区 %d 字节与期望函数体完全一致"
                    % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE) if got == blob else
                    "补丁区字节与期望不符\n  期望 %s\n  实际 %s"
                    % (blob.hex(" "), got.hex(" "))))

    # 4) 不越过下一函数
    results.append((SYNC_FN + len(blob) == SYNC_FN_END,
                    "函数体结束于 0x%08X == 下一函数起点 0x%08X（不越界）"
                    % (SYNC_FN + len(blob), SYNC_FN_END)))

    # 5) 字面量：+0x2C=KEY、+0x30=FLAG、+0x34=50024（原厂同址同值）
    lits = (int.from_bytes(got[0x2C:0x30], "little"),
            int.from_bytes(got[0x30:0x34], "little"),
            int.from_bytes(got[0x34:0x38], "little"))
    results.append((lits == (key, KEY_FLAG_ADDR, common.ID_NUMERATOR),
                    "字面量: KEY=0x%04X @0x%08X, FLAG=0x%08X @0x%08X, 50024 @0x%08X"
                    % (lits[0], SYNC_FN + 0x2C, lits[1], SYNC_FN + 0x30, SYNC_FN + 0x34)))

    # 6) 两个改值门 = `b 同步块; nop`，且门后仍是 ldrb r0,[r0,#6]（按 ID 调 sync）
    for site, expect, name in ((GATE_DEC, GATE_ORIG_DEC, "C.Fn 递减"),
                               (GATE_INC, GATE_ORIG_INC, "C.Fn 递增")):
        o = _off(site)
        gotg = bytes(patched[o:o + 4])
        after = bytes(patched[o + 4:o + 6])
        ok = gotg == GATE_NOP and after == b"\x80\x79"
        results.append((ok, "%s gate 0x%08X = %s，门后 %s（ldrb r0,[r0,#6]）"
                            % (name, site, gotg.hex(" "), after.hex(" ")) if ok else
                        "%s gate 0x%08X 不符：期望 %s + 80 79，实际 %s + %s"
                        % (name, site, GATE_NOP.hex(" "), gotg.hex(" "), after.hex(" "))))

    # 7) 造型灯读者屏蔽 / 无线命令保护
    for addr, orig, new, desc in READER_MASK_PATCHES:
        o = _off(addr)
        gotr = bytes(patched[o:o + len(new)])
        results.append((gotr == new, "补丁点 0x%08X = %s（%s）" % (addr, gotr.hex(" "), desc)))

    # 8) 与原厂差异必须恰好只落在补丁点集合内
    orig = _unpatch(patched)
    diffs = [i for i in range(len(patched)) if patched[i] != orig[i]]
    expected = _expected_diff_offsets(key)
    results.append((set(diffs) == expected,
                    "与原厂差异 %d 字节，offset=%s（应恰为 %s）"
                    % (len(diffs), ["0x%05X" % d for d in diffs],
                       ["0x%05X" % d for d in sorted(expected)])))
    outside = [i for i in diffs if i not in expected]
    results.append((not outside, "差异之外无任何字节变化（%d 处）" % len(outside)))

    # 9) Unicorn 调用级
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

    # 10) 门补丁行为级：除 item9(LCD) 外的 C.Fn 改值都立即调用同步字函数（实参 = ID）
    for which in ("dec", "inc"):
        for item in (0, 5, 8, 11):
            results.append(_check_gate(patched, which, item, ident=37))
    return results
