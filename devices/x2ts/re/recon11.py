# -*- coding: utf-8 -*-
"""X2T-S 侦察 11（base 0x08003000）：定位设置应用/改值处理函数边界、调用者与结构体地址。"""
import struct
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

IMG = r"D:\神牛\devices\x2ts\firmware\X2T-S_V1.4.bin"
DATA = open(IMG, "rb").read()
BASE = 0x08003000
SIZE = len(DATA)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB | CS_MODE_MCLASS)
md.detail = True


def off(a):
    return a - BASE


def find_func_start(addr, limit=0x200):
    """从 addr 向前找 push 序言（粗粒度：找 push {...} 2/4 字节）并返回最可能的函数起点。"""
    for a in range(addr, addr - limit, -2):
        hw = struct.unpack_from("<H", DATA, off(a))[0]
        if (hw & 0xFF00) == 0xB500 or hw in (0xE92D, 0xB5F0, 0xB570, 0xB538, 0xB510):
            # 确认不是数据：显示一下
            return a
    return None


def dis_range(start, end):
    for ins in md.disasm(DATA[off(start):off(end)], start):
        print("    0x%08X  %-20s %s %s"
              % (ins.address, ins.bytes.hex(" "), ins.mnemonic, ins.op_str))


# 1) 找到包含 0x0800C7F8 的函数并反汇编前半段
target = 0x0800C7F8
print("=== 包含 0x%08X 的函数 ===" % target)
start = find_func_start(target)
print("  推测函数起点 = 0x%08X" % start)
dis_range(start, 0x0800C8B0)
print()

# 2) 扫描整个镜像中所有 BL 到 0x0800C7xx/0x0800C6xx 的调用点
print("=== BL 目标在 0x0800C6C0..0x0800C820 的调用点 ===")
for o in range(0, SIZE - 4, 2):
    for ins in md.disasm(DATA[o:o + 4], BASE + o):
        if ins.mnemonic in ("bl", "bl.w") and ins.operands:
            t = ins.operands[0].imm
            if 0x0800C6C0 <= t <= 0x0800C820:
                print("  BL @0x%08X -> 0x%08X" % (ins.address, t))
        break
print()

# 3) 两个结构体地址的字面量引用
print("=== 字面量 0x200203E0 / 0x20020122 / 0x20020028 的引用点 ===")
want = {0x200203E0, 0x20020122, 0x20020028, 0x20020000}
for o in range(0, SIZE - 4, 2):
    for ins in md.disasm(DATA[o:o + 4], BASE + o):
        if ins.mnemonic in ("ldr", "ldr.w") and len(ins.operands) == 2:
            op1 = ins.operands[1]
            if op1.type == 3 and op1.mem.base in (0, 11):
                lit = ((ins.address + 4) & ~3) + op1.mem.disp
                if 0 <= off(lit) <= SIZE - 4:
                    v = struct.unpack_from("<I", DATA, off(lit))[0]
                    if v in want:
                        print("  0x%08X ldr r%d <- [0x%08X] = 0x%08X"
                              % (ins.address, ins.operands[0].reg, lit, v))
        break
