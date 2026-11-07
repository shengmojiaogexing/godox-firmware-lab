# -*- coding: utf-8 -*-
"""Lead: 验证"可开关密钥"版灯端补丁（ID==0 -> KEY，否则原厂公式）。

灯端最终语义：
    灯 ID = OFF(0)  ->  CC2500 SYNC = 私有 KEY      (密钥模式开)
    灯 ID = 1..99   ->  SYNC = 50024/(ID+1)         (原厂普通模式)

实现：把 0x0800B758 的同步字设置函数整段改写成带条件的版本（原位，56 字节内）。
"""
from __future__ import annotations

import struct
import sys

sys.path.insert(0, r"keypatch\dev\lead")
from call_harness import run, DEVICES  # noqa: E402

KEY = 0x6B21


def t_ldr_pc(rt: int, imm8: int) -> bytes:
    assert 0 <= imm8 <= 0xFF
    return struct.pack("<H", 0x4800 | (rt << 8) | imm8)


def t_push(r4lr: bool = True) -> bytes:
    return struct.pack("<H", 0xB510)


def t_cmp_imm(rn: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x2800 | (rn << 8) | imm8)


def t_beq(offset_bytes: int) -> bytes:
    assert offset_bytes % 2 == 0
    imm8 = offset_bytes // 2
    assert 0 <= imm8 <= 0xFF
    return struct.pack("<H", 0xD000 | imm8)


def t_b(offset_bytes: int) -> bytes:
    assert offset_bytes % 2 == 0
    imm11 = offset_bytes // 2
    assert -0x800 <= imm11 <= 0x7FF
    return struct.pack("<H", 0xE000 | (imm11 & 0x7FF))


def t_adds_ri(rn: int, rd: int, imm3: int) -> bytes:
    return struct.pack("<H", 0x1C00 | (imm3 << 6) | (rn << 3) | rd)


def t_movs_imm(rd: int, imm8: int) -> bytes:
    return struct.pack("<H", 0x2000 | (rd << 8) | imm8)


def t_mov(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0x4600 | (rm << 3) | rd)  # MOV (register) T1: 0100 0110 D Rm Rd


def t_bl(addr: int, target: int) -> bytes:
    """Thumb BL T1 编码（addr = BL 指令地址）。"""
    off = target - (addr + 4)
    assert -0x400000 <= off <= 0x3FFFFE and off % 2 == 0
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


def t_lsrs(rd: int, rm: int, imm5: int) -> bytes:
    return struct.pack("<H", 0x0800 | (imm5 << 6) | (rm << 3) | rd)


def t_uxtb(rd: int, rm: int) -> bytes:
    return struct.pack("<H", 0xB2C0 | (rm << 3) | rd)


def t_pop(r4pc: bool = True) -> bytes:
    return struct.pack("<H", 0xBD10)


class Asm:
    def __init__(self, base, key, lit_key_addr, lit_orig_addr, div, wreg, delay):
        self.base = base
        self.key = key
        self.lit_key_addr = lit_key_addr
        self.lit_orig_addr = lit_orig_addr
        self.div = div
        self.wreg = wreg
        self.delay = delay
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
        # LDR (literal) T1: 01001 Rt imm8, PC = (addr+4)&~3
        here = self.here()
        pc = (here + 4) & ~3
        imm8 = (target - pc) // 4
        assert 0 <= imm8 <= 0xFF, (target, pc)
        self.emit(t_ldr_pc(rt, imm8))

    def bl(self, target):
        self.emit(t_bl(self.here(), target))

    def finish(self):
        out = bytearray(self.code)
        for off, kind, label in self.fixups:
            here = self.base + off
            target = self.labels[label]
            delta = target - (here + 4)
            if kind == "beq":
                imm8 = delta // 2
                val = 0xD000 | (imm8 & 0xFF)
            else:
                imm11 = delta // 2
                val = 0xE000 | (imm11 & 0x7FF)
            out[off:off + 2] = struct.pack("<H", val)
        return bytes(out)


def build_light_patch(base, div, wreg, delay, key=KEY):
    """生成灯端 56 字节补丁：48 字节代码 + 2 个字面量。"""
    # 先用占位地址计算排版
    a = Asm(base, key, base + 48, base + 52, div, wreg, delay)
    a.ldr_pc(2, a.lit_key_addr)          # 0x00
    a.emit(t_push())                     # 0x02
    a.emit(t_cmp_imm(0, 0))              # 0x04 cmp r0,#0
    a.branch("beq", "keymode")           # 0x06
    a.ldr_pc(2, a.lit_orig_addr)         # 0x08
    a.emit(t_adds_ri(0, 1, 1))           # 0x0A adds r1,r0,#1
    a.branch("b", "go")                  # 0x0C
    a.label("keymode")                   # 0x0E
    a.emit(t_movs_imm(1, 1))             # 0x0E movs r1,#1
    a.label("go")                        # 0x10
    a.emit(t_mov(0, 2))                  # 0x10 mov r0,r2
    a.bl(div)                            # 0x12
    a.emit(t_mov(4, 0))                  # 0x16 mov r4,r0
    a.emit(t_lsrs(1, 0, 8))              # 0x18
    a.emit(t_movs_imm(0, 4))             # 0x1A
    a.bl(wreg)                           # 0x1C
    a.emit(t_uxtb(1, 4))                 # 0x20
    a.emit(t_movs_imm(0, 5))             # 0x22
    a.bl(wreg)                           # 0x24
    a.emit(t_movs_imm(0, 0x14))          # 0x28
    a.bl(delay)                          # 0x2A
    a.emit(t_pop())                      # 0x2E
    code = a.finish()
    assert len(code) == 48, len(code)
    lits = struct.pack("<II", key, 50024)
    return code + lits


def apply_light(image: bytes, base: int, func_addr: int, div, wreg, delay, key=KEY) -> bytes:
    data = bytearray(image)
    off = func_addr - base
    blob = build_light_patch(func_addr, div, wreg, delay, key)
    assert len(blob) == 56
    # 校验原厂字节：函数 50 字节 + 4 字面量
    expect_head = bytes.fromhex("0c4a10b5411c1046")  # 具体依固件，稍后在调用处校验
    data[off:off + 56] = blob
    return bytes(data)


def make_patched(dev: str) -> bytes:
    cfg = DEVICES[dev]
    image = bytearray(open(cfg["image"], "rb").read())
    base = cfg["base"]
    entry = cfg["entry"]
    blob = build_light_patch(entry, cfg["div"], cfg["write_reg"], cfg["delay"], KEY)
    off = entry - base
    image[off:off + 56] = blob
    return bytes(image)


def main() -> int:
    ok_all = True
    for dev in ("ad100", "ad200"):
        cfg = DEVICES[dev]
        orig = bytearray(open(cfg["image"], "rb").read())
        patched = bytearray(make_patched(dev))
        print("=" * 76)
        print("设备", dev, "补丁长度差异检查:", len(orig) == len(patched))
        # 打印补丁后的函数反汇编
        from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
        md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
        off = cfg["entry"] - cfg["base"]
        print("---- 补丁后 0x%08X ----" % cfg["entry"])
        for ins in md.disasm(patched[off:off + 56], cfg["entry"]):
            print("  %08x %-10s %s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
        # 运行 harness：ID=0 应得 KEY，ID=1/37/99 应得原厂公式
        res = run(dev, patched, ids=(0, 1, 37, 99), key=KEY)
        for ident, r in res.items():
            expected = KEY if ident == 0 else 50024 // (ident + 1)
            got = r[0]
            ok = got == expected
            ok_all &= ok
            print("  ID=%-2d sync=0x%04X 期望=0x%04X %s | calls=%r"
                  % (ident, got if isinstance(got, int) else -1, expected,
                     "PASS" if ok else "FAIL", r[1] if isinstance(r[1], dict) else r[1]))
    print("=" * 76)
    print("结论:", "ALL PASS" if ok_all else "FAIL")
    return 0 if ok_all else 1


if __name__ == "__main__":
    sys.exit(main())
