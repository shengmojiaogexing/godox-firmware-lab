#!/usr/bin/env python
"""hwscan_proto.py - hunt Godox protocol constants (50024=0xC368, 0xAA sync, 0xD5, 0xA9) in code/rodata"""
import sys, os, struct, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE

b = load(); N = len(b)

def u32(off):
    return struct.unpack_from('<I', b, off)[0]

print("=== 32-bit words equal to 50024 (0xC368) or small multiples")
for off in range(0, N-3, 4):
    w = u32(off)
    if w in (50024, 0xC368):
        print(f"  {w:#x} @file {off:#08x} (va {APP_BASE+off:#010x})")

print("=== movw/movt immediates for 0xC368 (movw rX,#0xC368 => bytes 68 f3 68 43?)")
# movw encoding: 11110 i 10 0100 imm4 ; 0 imm3 Rd imm8 ; imm16 = imm4:i:imm3:imm8
def decode_movw(off):
    hw1 = struct.unpack_from('<H', b, off)[0]
    hw2 = struct.unpack_from('<H', b, off+2)[0]
    if (hw1 & 0xFB00) != 0xF200:
        return None
    i = (hw1 >> 10) & 1
    imm4 = hw1 & 0xF
    imm3 = (hw2 >> 12) & 7
    imm8 = hw2 & 0xFF
    imm16 = (imm4 << 12) | (i << 11) | (imm3 << 8) | imm8
    rd = (hw2 >> 8) & 0xF
    return imm16, rd, ('movw' if (hw1 & 0xFBF0) == 0xF240 else 'movt')

for off in range(0, N-4, 2):
    r = decode_movw(off)
    if r and r[0] in (0xC368, 0xC368 & 0xFFFF):
        print(f"  movw/movt imm={r[0]:#06x} rd=r{r[1]} @file {off:#08x} (va {APP_BASE+off:#010x})")

print("=== byte pattern AA AA AA AA in code/data before 0xA0000 with context")
for m in re.finditer(re.escape(b'\xaa\xaa\xaa\xaa'), b[:0xA0000]):
    off = m.start()
    print(f"  @file {off:#08x} (va {APP_BASE+off:#010x}) ctx: {b[off-8:off+16].hex(' ')}")

print("=== D5 as command-ish byte in code region (0..0x40000) followed by 09/11/21")
for off in range(0, 0x40000):
    if b[off] == 0xD5 and b[off+1] in (0x09, 0x11, 0x21):
        print(f"  @file {off:#08x} ctx: {b[off-8:off+8].hex(' ')}")

print("=== A9 group byte patterns (0x50/0x09..0x0E) 3-byte rodata in 0..0x40000")
for off in range(0, 0x40000):
    if b[off] == 0xA9 and b[off+1] in (0x50, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E):
        print(f"  @file {off:#08x} ctx: {b[off-6:off+10].hex(' ')}")
