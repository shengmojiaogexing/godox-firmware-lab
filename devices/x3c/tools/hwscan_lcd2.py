#!/usr/bin/env python
"""hwscan_lcd2.py - XMC init, LCD flush loop, command call sites with constants."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from collections import Counter

b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def show(va, n=60, label=''):
    print(f'===== {label} {va:#010x} (file {va-APP_BASE:#08x})')
    off = va - APP_BASE
    if off < 0 or off >= N:
        print('  <out of range>'); return
    for ins in md.disasm(b[off:off+n*4], va):
        ann = ''
        if ins.mnemonic.startswith('ldr') and '[pc' in ins.op_str:
            try:
                disp = int(ins.op_str.split('#')[1].split(']')[0], 0)
            except Exception:
                disp = 0
            tgt = (ins.address + 4) & ~3
            tgt += disp
            tf = tgt - APP_BASE
            w = struct.unpack_from('<I', b, tf)[0] if 0 <= tf < N-3 else None
            ann = f'   ; [{tgt:#010x}] = {w:#010x}'
            if w is not None and 0x40000000 <= w < 0x60000000: ann += ' (MMIO)'
            elif w is not None and 0x20000000 <= w < 0x20060000: ann += ' (RAM)'
            elif w is not None and 0x6C000000 <= w < 0x6D000000: ann += ' (XMC/LCD)'
        print(f'{ins.address:010x}: {ins.bytes.hex():<10} {ins.mnemonic:<8} {ins.op_str}{ann}')

def decode_bl(off):
    if off + 4 > N: return None
    hw1 = struct.unpack_from('<H', b, off)[0]; hw2 = struct.unpack_from('<H', b, off+2)[0]
    if (hw1 & 0xF800) != 0xF000 or (hw2 & 0xD000) != 0xD000: return None
    s = (hw1 >> 10) & 1; imm10 = hw1 & 0x3FF
    j1 = (hw2 >> 13) & 1; j2 = (hw2 >> 11) & 1
    i1 = (~(j1 ^ s)) & 1; i2 = (~(j2 ^ s)) & 1; imm11 = hw2 & 0x7FF
    imm = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | (imm11 << 1)
    if s: imm -= 0x2000000
    return APP_BASE + ((off + 4) & ~3) + imm

def calls_with_const(target, maxshow=40):
    sites = []
    for off in range(0, N-4, 2):
        if decode_bl(off) == target:
            va = APP_BASE + off
            # look back up to 16 bytes for movs r0,#imm
            const = None
            for back in range(2, 18, 2):
                ins = next(md.disasm(b[off-back: off-back+4], va-back), None)
                if ins is None: continue
                if ins.mnemonic in ('movs', 'mov.w') and ins.op_str.startswith('r0, #'):
                    try:
                        const = int(ins.op_str.split('#')[1], 0)
                    except Exception:
                        pass
                    break
            sites.append((va, const))
    print(f"=== calls to {target:#010x}: {len(sites)}")
    c = Counter(x[1] for x in sites if x[1] is not None)
    print("   constants:", {hex(k): v for k, v in c.most_common(30)})
    for va, const in sites[:maxshow]:
        print(f"   {va:#010x} arg={const if const is None else hex(const)}")

if __name__ == '__main__':
    show(0x0803BA5C, 40, 'XMC func A')
    print()
    show(0x0803BA7C, 40, 'XMC func B')
    print()
    show(0x0803BA94, 40, 'XMC func C')
    print()
    show(0x0801C2C0, 60, 'DMA LCD func?')
    print()
    calls_with_const(0x08035BC4)
