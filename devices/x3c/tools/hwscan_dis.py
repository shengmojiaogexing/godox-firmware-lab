#!/usr/bin/env python
"""hwscan_dis.py - disassemble a range of the X3 firmware."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load, BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

b = load()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
md.detail = True

def dis(va, count=40, arch_bits=True):
    off = va - BASE
    code = b[off:off+count*4]
    out = []
    for ins in md.disasm(code, va):
        out.append(f"{ins.address:08x}: {ins.bytes.hex():<10} {ins.mnemonic:<8} {ins.op_str}")
    return out

if __name__ == '__main__':
    if len(sys.argv) < 2:
        # key spots
        for va, n in [(0x08004AF8, 30), (0x08004B2C, 12), (0x0800200, 40), (0x0800510C, 20),
                      (0x080082EB-1, 30), (0x080082C4, 20), (0x080083AC, 20), (0x08008458, 20),
                      (0x080084AC, 20), (0x080083EC, 20), (0x08012984, 20), (0x08016C00, 20),
                      (0x08016C5C, 20), (0x08016D50, 20), (0x08006E24, 20), (0x08006E3C, 20),
                      (0x08010B18, 20)]:
            print(f"===== {va:08x}")
            print('\n'.join(dis(va, n)))
            print()
    else:
        va = int(sys.argv[1], 16)
        n = int(sys.argv[2]) if len(sys.argv) > 2 else 40
        print('\n'.join(dis(va, n)))
