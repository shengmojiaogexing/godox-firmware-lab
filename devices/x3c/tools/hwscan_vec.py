#!/usr/bin/env python
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load, BASE, u32
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

b = load()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def dis_at(va, n=8):
    off = va - BASE
    if off < 0 or off >= len(b):
        return ['<out of range>']
    out = []
    for ins in md.disasm(b[off:off+n*4], va):
        out.append(f"{ins.address:08x}: {ins.bytes.hex():<10} {ins.mnemonic} {ins.op_str}")
        if len(out) >= n: break
    return out

sp = u32(b, 0)
print("SP", hex(sp))
vecs = [u32(b, 4*(i+1)) for i in range(131)]
targets = []
for i, v in enumerate(vecs):
    vec = i+1
    irq = vec - 16
    if v:
        t = v & ~1
        targets.append((vec, irq, v, t))
# count E7FE occurrences
import re
n = b.count(b'\xfe\xe7')
print("count of E7FE (b .) =", n)
print("first E7FE offsets:", [hex(m.start()) for m in re.finditer(b'\xfe\xe7', b)][:20])
print()
for vec, irq, v, t in targets[:30]:
    print(f"--- vec {vec} IRQ {irq} = {v:08x} -> {t:08x}")
    print("   " + " | ".join(dis_at(t, 6)))
