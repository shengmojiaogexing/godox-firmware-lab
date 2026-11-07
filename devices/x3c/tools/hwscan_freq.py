#!/usr/bin/env python
"""hwscan_freq.py - CC2500 FREQ register writer + frequency table hunt"""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def show(va, n=60, label=''):
    print(f'===== {label} {va:#010x} (file {va-APP_BASE:#08x})')
    off = va - APP_BASE
    for insn in md.disasm(b[off:off+n*4], va):
        ann = ''
        if insn.mnemonic.startswith('ldr') and '[pc' in insn.op_str:
            try:
                disp = int(insn.op_str.split('#')[1].split(']')[0], 0)
            except Exception:
                disp = 0
            tgt = (insn.address + 4) & ~3
            tgt += disp
            tf = tgt - APP_BASE
            w = struct.unpack_from('<I', b, tf)[0] if 0 <= tf < N-3 else None
            ann = f'   ; [{tgt:#010x}] = {w:#010x}'
        print(f'  {insn.address:010x}: {insn.bytes.hex():<10} {insn.mnemonic:<8} {insn.op_str}{ann}')

# CC2500 constants
def cc2500_freq(mhz, xtal=26.0):
    return round(mhz * 65536.0 / xtal)

if __name__ == '__main__':
    show(0x080147A0, 70, 'FREQ writer / channel set')
    print()
    print("CC2500 expected FREQ words for S3 channel plan (26MHz xtal):")
    half = [4826, 4829, 4832, 4836, 4839, 4842, 4846, 4849, 4852, 4856, 4859, 4862, 4866,
            4869, 4872, 4876, 4879, 4882, 4886, 4889, 4892, 4896, 4899, 4902, 4906, 4909,
            4912, 4916, 4919, 4922, 4926, 4929]
    print("  ch  MHz      FREQ24")
    for i, h in enumerate(half):
        mhz = h / 2.0
        f = cc2500_freq(mhz)
        print(f"  {i+1:2d}  {mhz:7.1f}  {f:#08x}  bytes {f>>16:02x} {(f>>8)&0xff:02x} {f&0xff:02x}")
    print()
    # search for 3-byte FREQ sequences (FREQ2,FREQ1,FREQ0) near expected values
    print("search for likely FREQ2 bytes 0x5B-0x5F with pll-ish patterns")
    for off in range(0, N-3, 1):
        if b[off] in (0x5C, 0x5D, 0x5B) and 0x80 <= b[off+1] <= 0xFF:
            pass
