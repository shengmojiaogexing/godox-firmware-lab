# -*- coding: utf-8 -*-
"""Godox V1C V2.0 —— 私有密钥（CC2500 16 位硬件同步字）补丁模块 —— v5「C.Fn 开关」版。

用户可见语义（v5，取代 v4 的「ID=OFF 当开关」）
---------------------------------------------
灯端 **C.Fn 菜单第 9 项（LIGHT，第 3 页；原厂造型灯模式 12sec/OFF/ON）** 被复用为
**私有密钥开关**：开关值就是设置区字节 **settings[8] = RAM 0x2000108E**（C.Fn 项 8，
随原厂设置自动加载/保存到 flash 参数记录；原厂该字节的语义与选择依据见
`devices/v1c/README.md` §5 与 `keypatch/dev/v1c/CFN_SETTINGS.md`、`CFN_MENU.md`）。

* LIGHT = ON（值 2）   -> CC2500 同步字 = 私有 KEY（只有打了补丁的引闪器能引闪）
* LIGHT = 12sec/OFF（0/1）-> 同步字 = 50024 // (ID + 1)（原厂普通模式，ID 菜单恢复原厂含义）

把原厂「ID -> 16 位同步字」函数 0x080121BC 整段改写成：

    flag = *(uint8_t *)0x2000108E;     // C.Fn LIGHT 密钥开关（2=ON）
    if (flag == 2) sync = KEY;         // 密钥模式
    else            sync = 50024 / (id + 1); // 原厂普通模式
    WriteReg(4, sync >> 8);            // SYNC1
    WriteReg(5, sync & 0xFF);          // SYNC0
    return;                            // 只改同步字：无 SCAL、无延时

实现（原位字节补丁，文件长度不变 88,032 B）
------------------------------------------
1. **同步字函数整段改写**：0x080121BC 起 **56 字节**（到下一函数 0x080121F4），
   布局同 AD100 v4：44 B 代码 + KEY@+0x2C + FLAG_ADDR@+0x30 + 50024@+0x34
   （50024 与原厂同址同值，普通模式公式用）。
2. **改值即时生效 hook**：把 C.Fn 递减/递增处理函数里「item==6(ID) 才重设同步字」
   的 4 字节门条件各处替换成 `BL 注入代码`；注入代码放在全 0 死区 **0x08017944**
   （234 B 连续 0x00，无 BL/分支/字面量引用），逻辑：
   `sync(settings[6]);` 后返回原 handler 继续跑 —— 除 item9(LCD) 外的 C.Fn 项改值（含 LIGHT、ID）
   都立即重设同步字。
3. **读者屏蔽 / 标志保护**：把 settings[8] 的原厂功能读者（造型灯任务）改成常数 0，
   并 NOP 掉无线设置同步解包对 settings[8] 的覆盖（防密钥开关被静默清 0）；
   见 READER_MASK_PATCHES，证据见 CFN_SETTINGS.md。

补丁点复核（详见 devices/v1c/re/、keypatch/dev/v1c/CFN_*.md、AUDIT.md）
--------------------------------------------------------------------
* 0x080121BC..0x080121F3（52 B 代码 + 0x080121F0 处 4 B 字面量 50024=0xC368），
  下一函数从 0x080121F4 开始；全镜像 0xC368 字面量只出现一次。
* 全镜像只有本函数与 CC2500 初始化表函数 0x08015470 写 SYNC1/SYNC0；
  RF_Init(0x0800664C) 先初始化、随后调用本函数覆盖。
* 本函数 5 个调用点（0x08006274 / 0x080066B2 / 0x0800D484 / 0x0800E0C2 /
  0x0800E138）入参都是 settings[+6]（当前 ID）。
* C.Fn 项 0..0xB 共 12 项（3 页 × 4 行），最大值表 0x08018362；item 6=ID，
  item 8=LIGHT（本补丁开关，值 2=ON）；出厂默认 settings[8]=0（12sec，即普通模式）。
* 辅助函数：__aeabi_uidiv=0x080030F0、CC2500_WriteReg=0x080156CE、
  strobe=0x08015790、delay_us=0x08015440。

风险：无实机验证；去掉 SCAL+1ms 是相对原厂唯一射频差异（只改同步字不改频率，
理论上不需要重校准；若实机首包异常优先怀疑此处）。LIGHT 原厂「造型灯模式」功能被复用后
固定为出厂默认（造型灯输出跟随原厂 12sec 档；C.Fn 里的 LIGHT 只剩密钥开关语义）。
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
    "id": "v1c",
    "name": "Godox V1C",
    "kind": "flash",                     # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "V1C",
    "firmware_version": "V2.0",
    "image": "devices/v1c/firmware/V1C_V2.0.bin",
    "base": 0x08003000,
    "sha256": "4435dda84903cb97cfaf34f7eeb15ecb6168771d60b57da1c346e6a9ca36edfb",
    "out_name": "V1C_V2.0_key{key:04X}.bin",
    "container_name": "V1C_V2.0_key{key:04X}_container.bin",
    "notes": "C.Fn 第 3 页 LIGHT 密钥开关(settings[8]=0x2000108E)：ON(2) -> SYNC=私有 KEY；"
             "12sec/OFF(0/1) -> 原厂 50024/(ID+1)。56 B 整函数改写 + 改值即时生效 hook（代码洞注入）",
}

# ---- 镜像与目标函数 ---------------------------------------------------------
BASE = 0x08003000
IMAGE_SIZE = 88032                # V1C_V2.0.bin 长度

SYNC_FN = 0x080121BC              # void rf_set_id(uint8_t id)：写 CC2500 SYNC1/SYNC0
SYNC_FN_END = 0x080121F4          # 下一函数起点；本补丁必须止步于此
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN   # 56 字节
assert SYNC_FN_SIZE == 56

# 原厂 0x080121BC 起 56 字节（apply 前逐字节校验，防固件版本不对）
ORIG_FUNC = bytes.fromhex(
    "0c4a10b5411c1046f0f794ff0446010a042003f07efae1b2052003f07afa"
    "142003f030f9332003f0d5fa7d20c00003f029f910bd68c30000")
assert len(ORIG_FUNC) == SYNC_FN_SIZE

# ---- C.Fn 设置区 / 开关项 ---------------------------------------------------
SETTINGS_BASE = 0x20001086        # C.Fn 设置项数组基址（item n = base + n）
FLAG_IDX = 8                      # C.Fn 第 9 项（LIGHT，原厂造型灯模式：0=12sec/1=OFF/2=ON）
KEY_FLAG_ADDR = SETTINGS_BASE + FLAG_IDX   # 0x2000108E
KEY_FLAG_ON_VALUE = 2             # LIGHT=ON（值 2）-> 密钥模式；0/1 -> 原厂普通模式
ID_IDX = 6                        # item 6 = 引闪 ID（原厂既有）

# C.Fn 项最大值/回绕表（0x08018362，用于静态校验本固件的 item 索引/值域）
CFN_MAX_TABLE_ADDR = 0x08018362
CFN_MAX_TABLE = bytes.fromhex("01010101011f630102060101")
CFN_ITEM_COUNT = 12               # item 0..0xB

# ---- 注入区：0x08017944 起 234 字节全 0 死区（无 BL/分支/字面量引用） --------
INJ_ADDR = 0x08017944
INJ_MAX_SIZE = 10                 # 本补丁只用前 10 字节
INJ_ORIG = bytes(INJ_MAX_SIZE)    # 原厂这 10 字节全 0x00

# 两个「item==6 才重设同步字」的 gate（cmp r1,#6; beq <sync>，各 4 字节）
GATE_DEC = 0x0800625C             # C.Fn 递减处理函数 0x080061CC 内
GATE_INC = 0x0800D46E             # C.Fn 递增处理函数 0x0800D3D8 内
GATE_ORIG_DEC = bytes.fromhex("062908d0")
GATE_ORIG_INC = bytes.fromhex("062907d0")

# 辅助函数（verify 里替换为记录/跳过桩）
DIV_FUNC = 0x080030F0             # __aeabi_uidiv: r0 = r0 / r1
WRITE_REG = 0x080156CE            # CC2500_WriteReg(reg=r0, value=r1)
WRITE_CMD = 0x08015790            # strobe(cmd=r0)（补丁版不再调用 SCAL）
DELAY_US = 0x08015440             # delay_us(n=r0)

# 原厂 50024 字面量在补丁区内 +0x34（= 0x080121F0）处，保持原地址/原值
ORIG_LITERAL_OFF = 0x34

# 读者屏蔽（settings[FLAG_IDX] 的原厂功能读者 -> 常数 0，等价原厂 LIGHT=12sec）。
# 每项：(addr, 期望原厂字节, 补丁后字节, 说明)。证据见 keypatch/dev/v1c/CFN_SETTINGS.md
READER_MASK_PATCHES = (
    # LIGHT 的功能读者（0x080036D4 造型灯任务控制 GPIO）：固定为出厂默认 0（12sec），
    # 用户把 LIGHT 设为 ON（值 2）当密钥开关时不再驱动造型灯。
    (0x080036DA, b"\x00\x7a", b"\x00\x20",
     "造型灯任务：settings[8](LIGHT) 读者固定 0（密钥开关不影响造型灯输出）"),
    (0x08003702, b"\x01\x7a", b"\x00\x21",
     "造型灯任务：settings[8](LIGHT) 第二个读者固定 0（同上）"),
    # 无线设置同步解包（0x0800A6DC）会把紧凑结构的 c[0x16] 写回 settings[8]；
    # 引闪器侧该值为 0 时会静默关掉密钥开关，这里 NOP 掉该写，保护密钥标志。
    (0x0800A6FA, b"\x08\x72", b"\x00\xbf",
     "无线设置同步解包：不再覆盖 settings[8]（保护 LIGHT 密钥开关不被静默清 0）"),
)

# Unicorn 测试用内存布局
UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000             # 128 KiB flash（app 位于 0x08003000）
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x4000              # V1C 初始 SP=0x20001550，多映射一点给栈
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


# ---------------------------------------------------------------------------
# 补丁体构造
# ---------------------------------------------------------------------------
def build_sync_function(key: int) -> bytes:
    """生成 56 字节替换函数体：44 B 代码 + KEY/FLAG/50024 三个字面量。

    布局（相对 0x080121BC）::

        +00 push {r4, lr}
        +02 ldr  r3, =FLAG(0x2000108E)   ; 字面量 +0x30
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


def _ldr_lit_here(rt: int, instr_addr: int, target: int) -> bytes:
    """按当前指令地址直接编码 LDR(literal)，避免依赖汇编器状态。"""
    pc = (instr_addr + 4) & ~3
    imm8 = (target - pc) // 4
    assert (target - pc) % 4 == 0 and 0 <= imm8 <= 0xFF, (hex(target), hex(pc), imm8)
    return _t_ldr_literal(rt, imm8)


def build_injection() -> bytes:
    """10 字节注入代码：除 item9(LCD) 外的 C.Fn 项改值后立即重设同步字（ID 改值/密钥开关改值都生效）。

    进入时（与两个 handler 现场一致）：r0 = settings 基址(0x20001086)；
    返回时必须原样恢复 r0..r4，并回到 handler 后续代码（0x08006260 / 0x0800D472）。

        push {r0, r1, r2, r3, r4, lr}
        ldrb r0, [r0, #6]        ; ID = settings[6]
        bl   sync_fn             ; 0x080121BC（内部读 settings[2] 密钥开关）
        pop  {r0, r1, r2, r3, r4, pc}
    """
    code = bytearray()
    code += struct.pack("<H", 0xB51F)                        # +00 push {r0-r4,lr}
    code += struct.pack("<H", 0x7980)                        # +02 ldrb r0,[r0,#6]
    code += _t_bl(INJ_ADDR + len(code), SYNC_FN)             # +04 bl sync
    code += struct.pack("<H", 0xBD1F)                        # +08 pop {r0-r4,pc}
    assert len(code) == 10, len(code)
    assert len(code) <= INJ_MAX_SIZE
    return bytes(code)


def gate_patch(site: int) -> bytes:
    """把 4 字节 gate（cmp r1,#6; beq sync）替换成 BL 注入代码。"""
    return _t_bl(site, INJ_ADDR)


# ---------------------------------------------------------------------------
# 打补丁
# ---------------------------------------------------------------------------
def _off(addr: int) -> int:
    return addr - BASE


def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """把原厂镜像 data 打上 v5「C.Fn 开关」补丁，返回 (patched_bytes, patch_log)。"""
    key = common.validate_key(key)

    if len(data) != IMAGE_SIZE:
        raise ValueError("Godox V1C V2.0 固件长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
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
                 " RAM[0x%08X]==1 -> SYNC=KEY 0x%04X；==0 -> 50024/(ID+1)；"
                 "只写 SYNC1/0（无 SCAL、无延时）"
                 % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE, KEY_FLAG_ADDR, key)),
    }]

    # 2) 注入代码（0x08017944 全 0 死区）
    inj = build_injection()
    inj_off = _off(INJ_ADDR)
    inj_before = bytes(buf[inj_off:inj_off + INJ_MAX_SIZE])
    if inj_before != INJ_ORIG:
        raise ValueError("注入区 0x%08X..0x%08X 原厂内容不是 %d 个 0x00（版本不符，拒绝打补丁）"
                         % (INJ_ADDR, INJ_ADDR + INJ_MAX_SIZE - 1, INJ_MAX_SIZE))
    buf[inj_off:inj_off + len(inj)] = inj
    log.append({
        "addr": INJ_ADDR,
        "offset": inj_off,
        "before": inj_before.hex(" "),
        "after": (bytes(inj) + bytes(INJ_MAX_SIZE - len(inj))).hex(" "),
        "desc": ("注入代码（0x08017944 全 0 死区）：除 item9(LCD) 外的 C.Fn 项改值后按 settings[6]"
                 " 立即重设同步字（含 ID 改值与 LIGHT 密钥开关改值）"),
    })

    # 3) 两个 handler gate -> BL 注入
    for site, expect, name in ((GATE_DEC, GATE_ORIG_DEC, "C.Fn 递减"),
                               (GATE_INC, GATE_ORIG_INC, "C.Fn 递增")):
        o = _off(site)
        got = bytes(buf[o:o + 4])
        if got != expect:
            raise ValueError("gate 0x%08X（%s）原始字节与预期不符：期望 %s 实际 %s"
                             % (site, name, expect.hex(" "), got.hex(" ")))
        new = gate_patch(site)
        buf[o:o + 4] = new
        log.append({
            "addr": site, "offset": o,
            "before": expect.hex(" "), "after": new.hex(" "),
            "desc": "gate(%s)：cmp r1,#6; beq sync -> BL 注入（除 item9(LCD) 外的 C.Fn 改值都重设同步字）" % name,
        })

    # 4) 原厂功能读者屏蔽 / 密钥标志保护（settings[2]）
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
    inj = build_injection() + bytes(INJ_MAX_SIZE - len(build_injection()))
    base = _off(INJ_ADDR)
    for i in range(INJ_MAX_SIZE):
        if inj[i] != 0:
            diffs.add(base + i)
    for site, expect, _name in ((GATE_DEC, GATE_ORIG_DEC, "dec"),
                                (GATE_INC, GATE_ORIG_INC, "inc")):
        new = gate_patch(site)
        base = _off(site)
        for i in range(4):
            if new[i] != expect[i]:
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
    off = _off(INJ_ADDR)
    buf[off:off + INJ_MAX_SIZE] = INJ_ORIG
    for site, expect, _name in ((GATE_DEC, GATE_ORIG_DEC, "dec"),
                                (GATE_INC, GATE_ORIG_INC, "inc")):
        o = _off(site)
        buf[o:o + 4] = expect
    for addr, orig, _new, _desc in READER_MASK_PATCHES:
        o = _off(addr)
        buf[o:o + len(orig)] = orig
    return bytes(buf)


def _uc_run_sync_fn(image: bytes, ident: int, flag: int) -> dict:
    """用 Unicorn 执行 0x080121BC 的同步字设置函数（RAM[KEY_FLAG_ADDR]=flag）。"""
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


def _uc_run_injection(image: bytes, item: int, ident: int) -> dict:
    """直接执行注入代码，模拟 handler 现场（r0=settings 基址, r1=C.Fn item）。"""
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_HOOK_CODE, UC_PROT_ALL, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_LR,
                                   UC_ARM_REG_PC, UC_ARM_REG_SP)

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE, UC_PROT_ALL)
    uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, image)
    uc.mem_write(SETTINGS_BASE + ID_IDX, bytes([ident & 0xFF]))
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))
    uc.mem_write(SYNC_FN, bytes.fromhex("7047"))        # sync 入口替换为 bx lr（只记录调用）

    sync_calls: list[int] = []
    state = {"returned": False, "error": None}

    def hook_code(_uc, address, _size, _user):
        if address == UC_SENTINEL:
            state["returned"] = True
            _uc.emu_stop()
        elif address == SYNC_FN:
            sync_calls.append(_uc.reg_read(UC_ARM_REG_R0))
            _uc.reg_write(UC_ARM_REG_PC, _uc.reg_read(UC_ARM_REG_LR))

    uc.hook_add(UC_HOOK_CODE, hook_code)
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    uc.reg_write(UC_ARM_REG_R0, SETTINGS_BASE)
    uc.reg_write(UC_ARM_REG_R1, item)
    try:
        uc.emu_start(INJ_ADDR | 1, 0, 0, 2000)
    except UcError as exc:
        state["error"] = str(exc)
    r0 = uc.reg_read(UC_ARM_REG_R0)
    r1 = uc.reg_read(UC_ARM_REG_R1)
    return {"sync": sync_calls, "r0": r0, "r1": r1,
            "returned": state["returned"], "error": state["error"]}


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
    """v5 语义：flag==2(LIGHT=ON) -> KEY；flag=0/1(12sec/OFF) -> 50024/(ID+1)。"""
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


def _check_injection(image: bytes, ident: int) -> list[tuple[bool, str]]:
    """注入代码行为：除 item9(LCD) 外的 C.Fn 项改值都调用 sync（r0=ID），且 r0..r4 原样恢复。"""
    out: list[tuple[bool, str]] = []
    for item in (ID_IDX, FLAG_IDX, 5, 8, 11):
        r = _uc_run_injection(image, item, ident)
        ok = (r["error"] is None and r["returned"] and
              len(r["sync"]) == 1 and r["sync"][0] == ident and
              r["r0"] == SETTINGS_BASE and r["r1"] == item)
        out.append((ok, "注入 hook: item=%d -> sync 调用 %s（期望 1 次, r0=ID=%d），"
                         "返回 r0=0x%08X r1=%d%s"
                        % (item, r["sync"], ident, r["r0"], r["r1"],
                           "" if r["error"] is None else "，错误: %s" % r["error"])))
    return out


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证 v5 补丁：静态字节/边界/差异范围 + Unicorn 同步字/injection/原厂自证。"""
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

    # 5) 注入代码与两个 gate
    inj = build_injection()
    io = _off(INJ_ADDR)
    got_inj = bytes(patched[io:io + len(inj)])
    results.append((got_inj == inj,
                    "注入代码 @0x%08X（%d 字节，0x08017944 全 0 死区）与期望一致"
                    % (INJ_ADDR, len(inj)) if got_inj == inj else
                    "注入代码不符：期望 %s 实际 %s" % (inj.hex(" "), got_inj.hex(" "))))
    for site, expect, name in ((GATE_DEC, GATE_ORIG_DEC, "C.Fn 递减"),
                               (GATE_INC, GATE_ORIG_INC, "C.Fn 递增")):
        o = _off(site)
        want = gate_patch(site)
        gotg = bytes(patched[o:o + 4])
        results.append((gotg == want,
                        "%s gate 0x%08X = %s（BL 注入）" % (name, site, gotg.hex(" "))
                        if gotg == want else
                        "%s gate 0x%08X 不符：期望 %s 实际 %s"
                        % (name, site, want.hex(" "), gotg.hex(" "))))

    # 6) 读者屏蔽
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

    # 8) Unicorn 调用级
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
    results += _check_injection(patched, ident=37)
    for ident in (0, 1, 5, 37, 99):
        results.append(_check_original(orig, ident))
    return results
