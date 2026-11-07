#!/usr/bin/env python
"""hwscan_irqtable.py - full vector/IRQ table + inspect image2 and USB descriptor area"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load

APP_BASE = 0x08004800
IMG2 = 0x0F3800
IMG2_BASE = 0x08002000
b = load()

sp = struct.unpack_from('<I', b, 0)[0]
print(f"SP={sp:#010x}")
zeros = []
handlers = {}
for k in range(1, 131):
    v = struct.unpack_from('<I', b, 4*k)[0]
    irq = k - 16
    if v == 0:
        zeros.append(irq)
        continue
    handlers.setdefault(v, []).append(irq)
print("zero IRQ slots:", zeros)
print("default handler slots:", len(handlers.get(0x08004B2D, [])))
print("nonzero distinct:", len(handlers))
for v, irqs in sorted(handlers.items()):
    if v == 0x08004B2D:
        continue
    print(f"  {v:#010x} <- IRQ {irqs}")

print()
print("=== image2 vector table at file 0x%x (base %#x)" % (IMG2, IMG2_BASE))
sp2 = struct.unpack_from('<I', b, IMG2)[0]
print(f"SP2={sp2:#010x}")
for k in range(1, 60):
    v = struct.unpack_from('<I', b, IMG2 + 4*k)[0]
    if v == 0:
        continue
    off_in_img = (v & ~1) - IMG2_BASE
    tgt = IMG2 + off_in_img
    hw = struct.unpack_from('<H', b, tgt)[0] if 0 <= tgt < len(b)-1 else 0
    pro = 'push' if (hw & 0xFF80) == 0xB500 or hw == 0xE92D else ('b.' if b[tgt:tgt+2] == b'\xfe\xe7' else '?')
    print(f"  vec{k:2d} IRQ{k-16:3d} {v:#010x} -> file {tgt:#08x} {pro} hw={hw:#06x}")

print()
print("=== USB descriptor area 0x1d7e0..0x1d900")
for o in range(0x1d7e0, 0x1d900, 16):
    chunk = b[o:o+16]
    asc = ''.join(chr(c) if 0x20 <= c < 0x7f else '.' for c in chunk)
    print(f"  {o:08x}  {chunk.hex(' '):<47}  {asc}")
