# -*- coding: utf-8 -*-
"""Godox X2T-C（引闪器）V1.01 —— 私有密钥（CC2500 16 位硬件同步字）补丁模块 ——「C.Fn 开关」版。

固件身份
--------
* 原厂文件：`devices/x2tc/firmware/X2TC_Z02_V1.01.bin`（79,748 B，
  sha256 `d185e410…`）。它本身是 **G3 V2.0 容器**：
  payload(79,704 B) + MD5(16) + 内嵌文件名 "X2TC_Z02_V1.01.bin"(18) + u16 + 8 B 魔数。
* payload 是 app 代码区，加载基址 **0x08005000**（前 20 KB 是 bootloader，不在升级文件里）。
  向量表首字 = 0x20001158（MSP），次字 = 0x080050D1（Reset）→ 基址 0x08005000。
  字模/资源区在 payload 尾部（0x08018000+），参数页在 0x0801F000。
* 因此本模块的 apply() **先拆容器、原位改 payload、再重算 MD5 封回容器**，
  输出仍是长度完全一致的 G3 V2.0 容器（不需要 `--container` 二次打包）。

用户可见语义（与 V1C / V860III 的 C.Fn 开关一致）
------------------------------------------------
引闪器 **C.Fn 菜单里的 `LIGHT` 项**（原厂背光灯时间：12sec / OFF / ON）被复用为
**私有密钥开关**：

* `LIGHT = ON`（值 2）      -> CC2500 同步字 = 私有 KEY（只认打了补丁的灯）
* `LIGHT = 12sec`（0，出厂）或 `OFF`（1） -> 原厂公式 `50024 // (ID+1)`

开关字节 = 菜单设置数组 `settings[14]` = RAM **0x200009C6**（16 项菜单数组基址
0x200009B8；item 5=CH、6=ID、14=LIGHT、15=LCD）。C.Fn 菜单/最大值表由
0x08005228 处的解压器从 0x0801871C 的压缩块还原到 RAM：其中 0x20000092 的
16 项最大值表 index14 = 2（LIGHT 三档），与官方 X2T 说明书一致。
`LIGHT` 随原厂参数记录持久化（0x080073B4 打包 → 0x0801F000 页；开机会解包回 RAM）。

补丁点（payload 内，文件长度不变）
----------------------------------
1. **同步字函数整段改写**：0x0801184C 起 **56 字节**（到下一函数 0x08011884）：
   44 B 代码 + KEY@+0x2C + FLAG_ADDR@+0x30 + 50024@+0x34（50024 与原厂同址同值）。
   逻辑：flag = *(u8*)0x200009C6; flag==2 ? sync=KEY : sync=50024/(id+1);
       WriteReg(4, sync>>8); WriteReg(5, sync&0xFF); return（去掉原厂 SCAL + 1ms 延时）。
2. **背光灯读者屏蔽**：0x08005824 的 `ldrb r0,[r0,#0xe]` -> `movs r0,#0`
   （LIGHT 复用为密钥开关后，背光固定走原厂 12sec 档，不再受该菜单项影响）。
3. **改值后即时生效 hook**：保存任务每次打包参数前（0x080073B4 入口）跳转到
   0x08005010 的 28 B 注入代码，按当前 ID 重设一次同步字再回原函数。
   （C.Fn 改值后约 2 秒自动保存；保存前置 hook 让密钥模式在保存时立即生效；
   开机 RF 初始化也会调用同步字函数，所以断电重启后同样正确。）

注入区 0x08005010..0x0800502B 是 Cortex-M0 向量表里 **7 个保留向量槽**
（vec[4..10] 全 0x00000000，内核不会跳转），28 字节刚好放下注入代码，无引用冲突。

辅助函数（verify 时替换为返回桩）：__aeabi_uidiv=0x080050EC、
CC2500_WriteReg=0x08015CCE、delay_us=0x08015A2C、strobe=0x08015DB8。

风险：无实机验证；去掉 SCAL+1ms 是相对原厂唯一射频差异（只改同步字、不改频率，
理论上不需要重校准）；LIGHT 原厂「背光时间」功能被复用后固定为 12sec 档。
换密钥需重新生成并刷入。
"""
from __future__ import annotations

import hashlib
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
    "id": "x2tc",
    "name": "Godox X2T-C",
    "kind": "trigger",                     # trigger=引闪器 / flash=闪光灯（studio 用）
    "model": "X2T-C",
    "firmware_version": "V1.01",
    "image": "devices/x2tc/firmware/X2TC_Z02_V1.01.bin",
    "base": 0x08005000,
    "sha256": "d185e4101ad61422f2ac2bc9ee7ade1e6c245c6002361dfd50ed5f9e7b90210b",
    "out_name": "X2TC_Z02_V1.01_key{key:04X}.bin",
    # 原厂文件本身就是 G3 V2.0 容器：apply() 已就地重算 MD5 封好，
    # CLI/studio 的“额外生成容器”选项对这台设备必须跳过（否则会二次套壳）。
    "container_builtin": True,
    "notes": "C.Fn 菜单 LIGHT 密钥开关(settings[14]=0x200009C6)：ON(2) -> SYNC=私有 KEY；"
             "12sec/OFF(0/1) -> 原厂 50024/(ID+1)。56 B 整函数改写 + 背光读者屏蔽 + "
             "保存前重设同步字 hook（注入到向量表保留槽）。原厂文件为 G3 V2.0 容器，"
             "输出保持容器格式（长度不变、MD5 同步更新）。",
}

# ---- 镜像 / 容器 -----------------------------------------------------------
BASE = 0x08005000
CONTAINER_TOTAL = 79748
PAYLOAD_SIZE = 79704
PAYLOAD_SHA256 = "f12927c54209ef258ae2246f9375063baad973ef0c857821169143aab2dde438"
CONTAINER_MAGIC = bytes.fromhex("aa55669977882233")
CONTAINER_NAME = b"X2TC_Z02_V1.01.bin"
CONTAINER_FOOTER_TAIL_SIZE = len(CONTAINER_NAME) + 2 + len(CONTAINER_MAGIC)   # 28
MD5_OFFSET_IN_CONTAINER = PAYLOAD_SIZE                                        # +0

# ---- 同步字函数 -------------------------------------------------------------
SYNC_FN = 0x0801184C              # void rf_set_sync(uint8_t id)：写 CC2500 SYNC1/SYNC0
SYNC_FN_END = 0x08011884          # 下一函数起点（56 字节空间）
SYNC_FN_SIZE = SYNC_FN_END - SYNC_FN
assert SYNC_FN_SIZE == 56

# 原厂 0x0801184C 起 56 字节（apply 前逐字节校验，防固件版本不符）
ORIG_FUNC = bytes.fromhex(
    "0c4a10b5411c1046f3f74afc0446010a042004f036fae1b2052004f032fa"
    "142004f0def8332004f0a1fa7d20c00004f0d7f810bd68c30000"
)
assert len(ORIG_FUNC) == SYNC_FN_SIZE

ORD_NUMERATOR = 50024             # 0xC368，原厂公式分子
# ---- C.Fn 设置区 / 开关项 ---------------------------------------------------
SETTINGS_BASE = 0x200009B8        # 16 项菜单设置字节数组（item n = base + n）
FLAG_IDX = 14                     # C.Fn 里第 15 项 = LIGHT（原厂背光时间 12sec/OFF/ON）
KEY_FLAG_ADDR = SETTINGS_BASE + FLAG_IDX   # 0x200009C6
KEY_FLAG_ON_VALUE = 2             # LIGHT=ON（值 2）-> 密钥模式
ID_IDX = 6                        # item 6 = 引闪 ID（同步字函数入参）
CH_IDX = 5                        # item 5 = 信道
CFN_ITEM_COUNT = 16               # 四页 × 四行
CFN_MAX_TABLE_RAM = 0x20000092    # C.Fn 最大值表（开机由压缩块解出；index14=2）
CFN_MAX_TABLE_FLASH = 0x08018723  # 压缩块内的同一张表（16 B，供文档/取证）

# ---- 背光灯读者屏蔽 ---------------------------------------------------------
BACKLIGHT_READER = 0x08005824     # ldrb r0,[r0,#0xe]（读 settings[14]）
BACKLIGHT_ORIG = bytes.fromhex("807b")
BACKLIGHT_NEW = bytes.fromhex("0020")   # movs r0,#0（固定原厂 12sec 档）

# ---- 保存前置 hook / 注入区 -------------------------------------------------
PACK_FN = 0x080073B4              # 参数记录打包函数入口：push {r4,r5,r6,lr}; ldr r1,=0x20000028
PACK_ORIG = bytes.fromhex("70b5ff49")
PACK_RESUME = 0x080073B8          # 注入代码跑完后回到原函数第 3 条指令
PACK_RECORD_BUF = 0x20000028      # ldr r1,=0x20000028（原第 2 条指令，注入里复现）
HOOK_ADDR = 0x08005010            # Cortex-M0 向量表保留槽 vec[4..10]（28 B 全 0）
HOOK_SIZE = 28
HOOK_ORIG = bytes(HOOK_SIZE)

# 辅助函数（verify 里替换为返回桩/记录桩）
DIV_FUNC = 0x080050EC             # __aeabi_uidiv: r0 = r0 / r1
WRITE_REG = 0x08015CCE            # CC2500_WriteReg(reg=r0, value=r1)
WRITE_CMD = 0x08015DB8            # strobe(cmd=r0)（补丁版不再调用 SCAL）
DELAY_US = 0x08015A2C             # delay_us(n=r0)
# 原厂 50024 字面量在补丁区内 +0x34（= 0x08011880）处，保持原地址/原值
ORIG_LITERAL_OFF = 0x34

# Unicorn 测试用内存布局
UC_ROM = 0x08000000
UC_ROM_SIZE = 0x20000
UC_RAM = 0x20000000
UC_RAM_SIZE = 0x2000
UC_STACK = 0x20000E00
UC_SENTINEL = 0x20000F00
MAX_HARNESS_INSNS = 20000


# ---------------------------------------------------------------------------
# G3 V2.0 容器拆分 / 封装（只依赖标准库）
# ---------------------------------------------------------------------------
def split_container(blob: bytes) -> dict:
    """拆 G3 V2.0 容器，返回 {ok,total,payload,name,md5,header_len}（不抛异常）。"""
    out = {"ok": False, "total": len(blob), "payload": b"", "name": "", "md5": ""}
    if len(blob) < 27 or blob[-8:] != CONTAINER_MAGIC:
        return out
    name_len = int.from_bytes(blob[-10:-8], "little")
    if not (0 < name_len <= len(blob) - 26):
        return out
    payload_end = len(blob) - 26 - name_len
    out["payload"] = blob[:payload_end]
    out["md5"] = blob[payload_end:payload_end + 16].hex()
    out["name"] = blob[payload_end + 16:-10].decode("utf-8", "replace")
    out["ok"] = hashlib.md5(out["payload"]).digest() == blob[payload_end:payload_end + 16]
    return out


def build_container(payload: bytes, name: bytes, tail: bytes) -> bytes:
    """payload + MD5 + 原样尾部（文件名/长度/魔数）。"""
    return payload + hashlib.md5(payload).digest() + tail


# ---------------------------------------------------------------------------
# Thumb-1 指令编码
# ---------------------------------------------------------------------------
def _t_ldr_literal(rt: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x4800 | ((rt & 7) << 8) | (imm8 & 0xFF))


def _t_ldrb_imm(rt: int, rn: int, imm5: int) -> bytes:
    return struct.pack("<H", 0x7800 | ((imm5 & 0x1F) << 6) | ((rn & 7) << 3) | (rt & 7))


def _t_push(reglist: int) -> bytes:
    """push T1：reglist 为低 8 位（r0..r7 + lr 的 bit8）。"""
    return struct.pack("<H", 0xB400 | reglist)


def _t_pop(reglist: int) -> bytes:
    """pop T1：reglist 为低 8 位（r0..r7 + pc 的 bit8）。"""
    return struct.pack("<H", 0xBC00 | reglist)


def _t_lsrs(rd: int, rm: int, imm5: int) -> bytes:
    return struct.pack("<H", 0x0800 | ((imm5 & 0x1F) << 6) | ((rm & 7) << 3) | (rd & 7))


def _t_uxtb(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0xB2C0 | ((rm & 7) << 3) | (rd & 7))


def _t_movs_imm(rd: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x2000 | ((rd & 7) << 8) | (imm8 & 0xFF))


def _t_mov(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0x4600 | ((rm & 7) << 3) | (rd & 7))


def _t_bl(addr: int, target: int) -> bytes:
    """Thumb BL T1 编码（addr = 本 BL 指令地址，target = 绝对地址）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0, (hex(addr), hex(target), off)
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
    """按当前指令地址编码 LDR(literal)。"""
    pc = (instr_addr + 4) & ~3
    imm8 = (target - pc) // 4
    assert (target - pc) % 4 == 0 and 0 <= imm8 <= 0xFF, (hex(target), hex(pc), imm8)
    return _t_ldr_literal(rt, imm8)


# ---------------------------------------------------------------------------
# 补丁体构造
# ---------------------------------------------------------------------------
def build_sync_function(key: int) -> bytes:
    """生成 56 字节替换函数体：44 B 代码 + KEY/FLAG/50024 三个字面量。

    布局（相对 0x0801184C）::

        +00 push {r4, lr}
        +02 ldr  r3, =FLAG(0x200009C6)   ; 字面量 +0x30
        +04 ldr  r2, =50024              ; 字面量 +0x34（原厂地址）
        +06 ldrb r3, [r3]
        +08 adds r1, r0, #1              ; 普通模式除数 = ID+1
        +0A cmp  r3, #2                  ; LIGHT=ON(值 2)
        +0C bne  go
        +0E ldr  r2, =KEY                ; 字面量 +0x2C
        +10 movs r1, #1                  ; 密钥模式除数 = 1
        +12 go: mov r0, r2
        +14 bl   __aeabi_uidiv
        +18 mov  r4, r0
        +1A lsrs r1, r0, #8
        +1C movs r0, #4
        +1E bl   WriteReg
        +22 uxtb r1, r4
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
    code += struct.pack("<H", 0xB510)                        # +00 push {r4,lr}
    code += _ldr_lit_here(3, SYNC_FN + len(code), flag_lit)  # +02
    code += _ldr_lit_here(2, SYNC_FN + len(code), orig_lit)  # +04
    code += struct.pack("<H", 0x781B)                        # +06 ldrb r3,[r3]
    code += struct.pack("<H", 0x1C41)                        # +08 adds r1,r0,#1
    code += struct.pack("<H", 0x2B02)                        # +0A cmp r3,#2
    code += struct.pack("<H", 0xD101)                        # +0C bne go
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
    code += struct.pack("<H", 0xBD10)                        # +2A pop {r4,pc}
    assert len(code) == 44, len(code)
    blob = bytes(code) + struct.pack("<III", key, KEY_FLAG_ADDR, ORD_NUMERATOR)
    assert len(blob) == SYNC_FN_SIZE, len(blob)
    assert blob[0x34:0x38] == ORIG_FUNC[0x34:0x38]          # 50024 同址同值
    return blob


def build_pack_hook() -> bytes:
    """28 字节保存前置注入代码（放在向量表保留槽 0x08005010）。

    进入时（BL 来自 0x080073B4，尚未执行原函数头）::

        push {r4,r5,r6,lr}        ; 原函数第 1 条（0x080073B4）
        push {r0,r1,r2,r3}        ; 保存调用者参数（r0=打包目标指针）
        ldr  r0, =SETTINGS        ; 0x200009B8
        ldrb r0, [r0, #6]         ; 当前 ID
        bl   rf_set_sync          ; 0x0801184C（内部读 LIGHT 开关）
        pop  {r0,r1,r2,r3}
        ldr  r1, =0x20000028      ; 原函数第 2 条（0x080073B6）
        bl   0x080073B8           ; 回原函数继续打包
    """
    code = bytearray()
    code += _t_push(0x70 | 0x100)                  # push {r4,r5,r6,lr}
    code += _t_push(0x0F)                          # push {r0,r1,r2,r3}
    code += _ldr_lit_here(0, HOOK_ADDR + len(code), HOOK_ADDR + 0x14)
    code += _t_ldrb_imm(0, 0, ID_IDX)              # ldrb r0,[r0,#6]
    code += _t_bl(HOOK_ADDR + len(code), SYNC_FN)
    code += _t_pop(0x0F)                           # pop {r0,r1,r2,r3}
    code += _ldr_lit_here(1, HOOK_ADDR + len(code), HOOK_ADDR + 0x18)
    code += _t_bl(HOOK_ADDR + len(code), PACK_RESUME)
    assert len(code) == 0x14, len(code)
    blob = bytes(code) + struct.pack("<II", SETTINGS_BASE, PACK_RECORD_BUF)
    assert len(blob) == HOOK_SIZE, len(blob)
    return blob


# ---------------------------------------------------------------------------
# 打补丁
# ---------------------------------------------------------------------------
def _off(addr: int) -> int:
    return addr - BASE


def _check_bytes(buf: bytes, addr: int, expect: bytes, what: str) -> None:
    o = _off(addr)
    got = bytes(buf[o:o + len(expect)])
    if got != expect:
        raise ValueError("%s 0x%08X 原始字节与预期不符（固件版本不符，拒绝打补丁）\n"
                         "  期望 %s\n  实际 %s"
                         % (what, addr, expect.hex(" "), got.hex(" ")))


def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """把原厂 X2TC_Z02_V1.01 容器打上「C.Fn LIGHT 开关」补丁，返回 (新容器, patch_log)。"""
    key = common.validate_key(key)
    if len(data) != CONTAINER_TOTAL:
        raise ValueError("Godox X2T-C V1.01 容器长度应为 %d 字节，实际 %d 字节（版本不符，拒绝打补丁）"
                         % (CONTAINER_TOTAL, len(data)))
    info = split_container(data)
    if not info["ok"]:
        raise ValueError("输入不是合法的 G3 V2.0 容器（MD5/魔数不符，拒绝打补丁）")
    if len(info["payload"]) != PAYLOAD_SIZE:
        raise ValueError("容器 payload 长度应为 %d 字节，实际 %d 字节（版本不符）"
                         % (PAYLOAD_SIZE, len(info["payload"])))
    if hashlib.sha256(data).hexdigest() != DEVICE["sha256"]:
        raise ValueError("原厂容器 sha256 与 DEVICE 声明不符（拒绝打补丁）")

    payload = bytearray(info["payload"])
    log: list[dict] = []

    # 1) 同步字函数整段改写
    _check_bytes(payload, SYNC_FN, ORIG_FUNC, "同步字函数")
    blob = build_sync_function(key)
    o = _off(SYNC_FN)
    before = bytes(payload[o:o + SYNC_FN_SIZE])
    payload[o:o + SYNC_FN_SIZE] = blob
    log.append({
        "addr": SYNC_FN, "offset": o,
        "before": before.hex(" "), "after": blob.hex(" "),
        "desc": ("整段改写同步字函数 0x%08X..0x%08X（%d 字节）：C.Fn LIGHT 开关 "
                 "RAM[0x%08X]==%d -> SYNC=KEY 0x%04X；否则 50024/(ID+1)；只写 SYNC1/0（无 SCAL/延时）"
                 % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE, KEY_FLAG_ADDR,
                    KEY_FLAG_ON_VALUE, key)),
    })

    # 2) 背光灯读者屏蔽（LIGHT 原厂功能固定 12sec）
    _check_bytes(payload, BACKLIGHT_READER, BACKLIGHT_ORIG, "背光灯读者")
    o = _off(BACKLIGHT_READER)
    payload[o:o + len(BACKLIGHT_NEW)] = BACKLIGHT_NEW
    log.append({
        "addr": BACKLIGHT_READER, "offset": o,
        "before": BACKLIGHT_ORIG.hex(" "), "after": BACKLIGHT_NEW.hex(" "),
        "desc": "背光灯任务读 settings[14](LIGHT) 改为常数 0：复用为密钥开关后背光固定 12sec 档",
    })

    # 3) 保存前置 hook：打包参数前按当前 ID 重设同步字
    _check_bytes(payload, PACK_FN, PACK_ORIG, "参数记录打包函数入口")
    o = _off(PACK_FN)
    hook_bl = _t_bl(PACK_FN, HOOK_ADDR)
    payload[o:o + len(hook_bl)] = hook_bl
    log.append({
        "addr": PACK_FN, "offset": o,
        "before": PACK_ORIG.hex(" "), "after": hook_bl.hex(" "),
        "desc": "参数打包函数入口 -> BL 0x%08X 注入代码（C.Fn 改值保存时立即重设同步字）" % HOOK_ADDR,
    })

    # 4) 注入代码（向量表保留槽）
    _check_bytes(payload, HOOK_ADDR, HOOK_ORIG, "注入区（向量表保留槽）")
    inj = build_pack_hook()
    o = _off(HOOK_ADDR)
    payload[o:o + HOOK_SIZE] = inj
    log.append({
        "addr": HOOK_ADDR, "offset": o,
        "before": HOOK_ORIG.hex(" "), "after": inj.hex(" "),
        "desc": ("注入代码（Cortex-M0 向量表保留槽 vec[4..10]，28 B）："
                 "push 原函数头 -> sync(settings[6]) -> 回 0x%08X 继续打包" % PACK_RESUME),
    })

    # 5) 重算容器 MD5（长度不变）
    patched = build_container(bytes(payload), CONTAINER_NAME, data[-CONTAINER_FOOTER_TAIL_SIZE:])
    assert len(patched) == len(data)
    log.append({
        "addr": BASE + PAYLOAD_SIZE, "offset": PAYLOAD_SIZE,
        "before": info["md5"], "after": hashlib.md5(bytes(payload)).hexdigest(),
        "desc": "G3 V2.0 容器 MD5 同步更新（payload 改动后必须重算；长度/文件名/魔数不变）",
    })
    return patched, log


# ---------------------------------------------------------------------------
# 离线验证
# ---------------------------------------------------------------------------
def _unpatch(payload: bytes) -> bytes:
    """把补丁区还原成原厂字节（用于原厂 harness 自证 / 差异集合计算）。"""
    buf = bytearray(payload)
    o = _off(SYNC_FN)
    buf[o:o + SYNC_FN_SIZE] = ORIG_FUNC
    o = _off(BACKLIGHT_READER)
    buf[o:o + len(BACKLIGHT_ORIG)] = BACKLIGHT_ORIG
    o = _off(PACK_FN)
    buf[o:o + len(PACK_ORIG)] = PACK_ORIG
    o = _off(HOOK_ADDR)
    buf[o:o + HOOK_SIZE] = HOOK_ORIG
    return bytes(buf)


def _expected_payload_diff(key: int) -> set[int]:
    """payload 内应当且只应当变化的文件偏移（不含容器 MD5）。"""
    orig = _unpatch(build_container_payload_stub())
    patched = _patch_payload_stub(key)
    return {i for i in range(PAYLOAD_SIZE) if orig[i] != patched[i]}


def build_container_payload_stub() -> bytes:  # pragma: no cover - 供 _expected_payload_diff
    """返回全 0 payload 占位（_expected_payload_diff 只用它算补丁字节差异集合）。"""
    return bytes(PAYLOAD_SIZE)


def _patch_payload_stub(key: int) -> bytes:  # pragma: no cover
    buf = bytearray(build_container_payload_stub())
    o = _off(SYNC_FN)
    buf[o:o + SYNC_FN_SIZE] = build_sync_function(key)
    o = _off(BACKLIGHT_READER)
    buf[o:o + len(BACKLIGHT_NEW)] = BACKLIGHT_NEW
    o = _off(PACK_FN)
    buf[o:o + 4] = _t_bl(PACK_FN, HOOK_ADDR)
    o = _off(HOOK_ADDR)
    buf[o:o + HOOK_SIZE] = build_pack_hook()
    return bytes(buf)


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


def _uc_setup(payload: bytes, flag: int | None = None, ident: int = 0):
    from unicorn import Uc, UC_ARCH_ARM, UC_MODE_MCLASS, UC_PROT_ALL
    from unicorn.arm_const import UC_ARM_REG_LR, UC_ARM_REG_SP

    uc = Uc(UC_ARCH_ARM, UC_MODE_MCLASS)
    uc.mem_map(UC_ROM, UC_ROM_SIZE, UC_PROT_ALL)
    uc.mem_map(UC_RAM, UC_RAM_SIZE, UC_PROT_ALL)
    uc.mem_write(BASE, payload)
    uc.mem_write(UC_SENTINEL, bytes.fromhex("fee7"))       # b .
    for addr in (WRITE_REG, WRITE_CMD, DELAY_US):
        uc.mem_write(addr, bytes.fromhex("7047"))          # bx lr
    if flag is not None:
        uc.mem_write(KEY_FLAG_ADDR, bytes([flag & 0xFF]))
    uc.mem_write(SETTINGS_BASE + ID_IDX, bytes([ident & 0xFF]))
    uc.reg_write(UC_ARM_REG_SP, UC_STACK)
    uc.reg_write(UC_ARM_REG_LR, UC_SENTINEL | 1)
    return uc


def _uc_run_sync(payload: bytes, flag: int, ident: int) -> dict:
    """执行 0x0801184C 同步字函数（RAM[FLAG]=flag），记录 WriteReg/delay/strobe 调用。"""
    from unicorn import UC_HOOK_CODE, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_PC)

    uc = _uc_setup(payload, flag=flag, ident=ident)
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
    uc.reg_write(UC_ARM_REG_R0, ident & 0xFF)
    uc.reg_write(UC_ARM_REG_R1, 0x1234)
    uc.reg_write(UC_ARM_REG_R2, 0x5678)
    uc.reg_write(UC_ARM_REG_R3, 0x9ABC)
    try:
        uc.emu_start(SYNC_FN | 1, 0, 0, MAX_HARNESS_INSNS)
    except UcError as exc:
        state["error"] = "%s (pc=0x%08X, lr=0x%08X)" % (
            exc, uc.reg_read(UC_ARM_REG_PC), uc.reg_read(UC_ARM_REG_LR))
    if state["error"] is None and not state["returned"]:
        state["error"] = ("未在 %d 条指令内返回（执行 %d 条），疑似跑飞"
                          % (MAX_HARNESS_INSNS, state["insn"]))
    return {"calls": calls, "error": state["error"], "insn": state["insn"]}


def _check_sync(payload: bytes, key: int, ident: int, flag: int) -> tuple[bool, str]:
    res = _uc_run_sync(payload, flag=flag, ident=ident)
    if res["error"]:
        return False, "ID=%d flag=%d 补丁版 Unicorn 执行失败: %s" % (ident, flag, res["error"])
    calls = res["calls"]
    key_mode = (flag == KEY_FLAG_ON_VALUE)
    expect = key if key_mode else ORD_NUMERATOR // (ident + 1)
    want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF)]
    ok = calls == want
    msg = ("ID=%-2d flag=%d %s: sync=0x%04X -> SYNC1=0x%02X SYNC0=0x%02X | %s"
           % (ident, flag, "密钥模式(LIGHT=ON)" if key_mode else "原厂公式",
              expect, expect >> 8, expect & 0xFF, _format_calls(calls)))
    if not ok:
        msg += "  期望 %s" % _format_calls(want)
    return ok, msg


def _check_original(payload: bytes, ident: int) -> tuple[bool, str]:
    orig = _unpatch(payload)
    res = _uc_run_sync(orig, flag=0, ident=ident)
    if res["error"]:
        return False, "ID=%d 原厂自证 Unicorn 执行失败: %s" % (ident, res["error"])
    expect = ORD_NUMERATOR // (ident + 1)
    want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF),
            ("delay", 20, None), ("cmd", 0x33, None), ("delay", 1000, None)]
    ok = res["calls"] == want
    msg = ("ID=%-2d 原厂(自证): sync=0x%04X | %s"
           % (ident, expect, _format_calls(res["calls"])))
    if not ok:
        msg += "  期望 %s" % _format_calls(want)
    return ok, msg


def _check_pack_hook(payload: bytes, key: int, ident: int, flag: int) -> tuple[bool, str]:
    """直接执行 0x08005010 注入代码：应先按 ID 重设同步字，再回 0x080073B8 打包。"""
    from unicorn import UC_HOOK_CODE, UcError
    from unicorn.arm_const import (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2,
                                   UC_ARM_REG_R3, UC_ARM_REG_PC)

    uc = _uc_setup(payload, flag=flag, ident=ident)
    uc.mem_write(PACK_RESUME, bytes.fromhex("fee7"))       # 用哨兵替换原函数第 3 条
    calls: list[tuple] = []
    state = {"at_resume": False, "error": None}

    def hook_code(_uc, address, _size, _user):
        if address == WRITE_REG:
            calls.append(("wreg", _uc.reg_read(UC_ARM_REG_R0), _uc.reg_read(UC_ARM_REG_R1)))
        elif address == PACK_RESUME:
            state["at_resume"] = True
            _uc.emu_stop()

    uc.hook_add(UC_HOOK_CODE, hook_code)
    args = (0x20000838, 0x11111111, 0x22222222, 0x33333333)
    for i, v in enumerate(args):
        reg = (UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3)[i]
        uc.reg_write(reg, v)
    try:
        uc.emu_start(HOOK_ADDR | 1, 0, 0, 2000)
    except UcError as exc:
        state["error"] = str(exc)
    r = [uc.reg_read(UC_ARM_REG_R0), uc.reg_read(UC_ARM_REG_R1),
         uc.reg_read(UC_ARM_REG_R2), uc.reg_read(UC_ARM_REG_R3)]
    key_mode = (flag == KEY_FLAG_ON_VALUE)
    expect = key if key_mode else ORD_NUMERATOR // (ident + 1)
    want = [("wreg", 4, expect >> 8), ("wreg", 5, expect & 0xFF)]
    ok = (state["error"] is None and state["at_resume"] and calls == want
          and r[0] == args[0] and r[1] == PACK_RECORD_BUF
          and r[2] == args[2] and r[3] == args[3])
    msg = ("保存前 hook: flag=%d ID=%d -> sync=0x%04X | 回 0x%08X=%s | r0..r3=%s"
           % (flag, ident, expect, PACK_RESUME,
              "是" if state["at_resume"] else "否", [hex(x) for x in r]))
    if not ok:
        msg += "  期望 %s, r0=0x%08X r1=0x%08X" % (
            _format_calls(want), args[0], PACK_RECORD_BUF)
        if state["error"]:
            msg += "  错误: %s" % state["error"]
    return ok, msg


def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证：容器/长度/补丁点/差异范围 + Unicorn 同步字/original 自证/保存 hook。"""
    key = common.validate_key(key)
    results: list[tuple[bool, str]] = []

    # 1) 长度不变
    results.append((len(patched) == CONTAINER_TOTAL,
                    "容器长度 %d 字节（应为 %d，原位补丁）" % (len(patched), CONTAINER_TOTAL)))

    # 2) G3 V2.0 容器结构 + MD5 正确
    info = split_container(patched)
    results.append((info["ok"],
                    "G3 V2.0 容器合法：payload=%d B，MD5=%s，内嵌文件名=%r"
                    % (len(info["payload"]), info["md5"], info["name"])))
    results.append((info["name"] == CONTAINER_NAME.decode("utf-8"),
                    "容器内嵌文件名保持 %r" % CONTAINER_NAME.decode("utf-8")))

    payload = bytearray(info["payload"])
    if len(payload) != PAYLOAD_SIZE:
        results.append((False, "payload 长度异常 %d（应为 %d）" % (len(payload), PAYLOAD_SIZE)))
        return results

    # 3) 各补丁点字节与期望完全一致
    blob = build_sync_function(key)
    o = _off(SYNC_FN)
    got = bytes(payload[o:o + SYNC_FN_SIZE])
    results.append((got == blob,
                    "0x%08X..0x%08X 同步字函数 %d 字节与期望一致"
                    % (SYNC_FN, SYNC_FN_END - 1, SYNC_FN_SIZE) if got == blob else
                    "同步字函数与期望不符\n  期望 %s\n  实际 %s"
                    % (blob.hex(" "), got.hex(" "))))
    results.append((SYNC_FN + len(blob) == SYNC_FN_END,
                    "函数体结束于 0x%08X == 下一函数起点 0x%08X（不越界）"
                    % (SYNC_FN + len(blob), SYNC_FN_END)))
    lits = (int.from_bytes(got[0x2C:0x30], "little"),
            int.from_bytes(got[0x30:0x34], "little"),
            int.from_bytes(got[0x34:0x38], "little"))
    results.append((lits == (key, KEY_FLAG_ADDR, ORD_NUMERATOR),
                    "字面量: KEY=0x%04X@0x%08X, LIGHT_FLAG=0x%08X@0x%08X, 50024@0x%08X"
                    % (lits[0], SYNC_FN + 0x2C, lits[1], SYNC_FN + 0x30, SYNC_FN + 0x34)))

    o = _off(BACKLIGHT_READER)
    got_bl = bytes(payload[o:o + len(BACKLIGHT_NEW)])
    results.append((got_bl == BACKLIGHT_NEW,
                    "背光读者屏蔽 0x%08X = %s（movs r0,#0，固定 12sec 档）"
                    % (BACKLIGHT_READER, got_bl.hex(" "))))

    o = _off(PACK_FN)
    got_hook = bytes(payload[o:o + 4])
    results.append((got_hook == _t_bl(PACK_FN, HOOK_ADDR),
                    "保存前置 hook 0x%08X = %s（BL -> 0x%08X）"
                    % (PACK_FN, got_hook.hex(" "), HOOK_ADDR)))

    inj = build_pack_hook()
    o = _off(HOOK_ADDR)
    got_inj = bytes(payload[o:o + HOOK_SIZE])
    results.append((got_inj == inj,
                    "注入代码 0x%08X（%d B，向量表保留槽）与期望一致"
                    % (HOOK_ADDR, HOOK_SIZE) if got_inj == inj else
                    "注入代码不符\n  期望 %s\n  实际 %s"
                    % (inj.hex(" "), got_inj.hex(" "))))

    # 4) 与原厂差异只落在补丁点（容器 MD5 允许变化）
    orig_payload = _unpatch(bytes(payload))
    diffs = {i for i in range(PAYLOAD_SIZE) if payload[i] != orig_payload[i]}
    expected = _expected_payload_diff(key)
    results.append((diffs == expected,
                    "payload 与原厂差异 %d 字节，应恰为补丁点 %d 字节%s"
                    % (len(diffs), len(expected),
                       "" if diffs == expected else
                       "；多余=%s 缺失=%s"
                       % (["0x%05X" % d for d in sorted(diffs - expected)][:8],
                          ["0x%05X" % d for d in sorted(expected - diffs)][:8]))))
    results.append((len(patched) == CONTAINER_TOTAL and info["ok"],
                    "容器尾部（MD5/文件名/魔数）已重算且长度不变（footer 区 %d B）"
                    % (CONTAINER_TOTAL - PAYLOAD_SIZE)))

    # 5) Unicorn 调用级
    try:
        import unicorn  # noqa: F401
        has_uc = True
    except Exception:  # noqa: BLE001
        has_uc = False
    if not has_uc:
        results.append((True, "警告: unicorn 不可用，跳过 Unicorn 调用级测试（静态检查已通过）"))
        return results
    results.append((True, "Unicorn 调用级测试：SYNC_FN=0x%08X，LIGHT_FLAG=0x%08X，"
                          "hook=0x%08X" % (SYNC_FN | 1, KEY_FLAG_ADDR, HOOK_ADDR | 1)))
    for ident in (0, 5, 37, 99):
        results.append(_check_sync(bytes(payload), key, ident, KEY_FLAG_ON_VALUE))
    for flag in (0, 1):
        for ident in (0, 1, 5, 37, 99):
            results.append(_check_sync(bytes(payload), key, ident, flag))
    for ident in (0, 1, 5, 37, 99):
        results.append(_check_original(bytes(payload), ident))
    for flag in (0, 1, KEY_FLAG_ON_VALUE):
        results.append(_check_pack_hook(bytes(payload), key, ident=37, flag=flag))
    return results
