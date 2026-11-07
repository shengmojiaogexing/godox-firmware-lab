"""Scan Cortex-M images for HID IAP protocol constants / framing code.

Looks for: immediate 0xAA / 0x55 / 0xA0 / 0xA1 / 0x41 / 0x3C / 0x400 / 0x4000
and for the byte pattern AA 55 as literal data.
"""
import struct
import sys
import capstone

sys.stdout.reconfigure(encoding='utf-8')

files = [
    (r'devices\ad100pro\firmware\AD100pro_V1.2.bin', 0x08003000),
    (r'devices\ad200pro\firmware\AD200pro_V1.60.bin', 0x08003000),
    (r'devices\x3c\firmware\X3_C_v1.38_app.bin', 0x08004800),
]
md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
md.detail = True

for path, base in files:
    d = open(path, 'rb').read()
    print('=' * 70)
    print(path, len(d), 'base', hex(base))
    # literal occurrences of AA 55 / 55 AA
    for pat in (b'\xaa\x55', b'\x55\xaa'):
        idx = []
        i = 0
        while True:
            i = d.find(pat, i)
            if i < 0:
                break
            idx.append(i)
            i += 1
        print('  pattern', pat.hex(), 'count', len(idx), 'first', [hex(x) for x in idx[:10]])
    # scan instructions for cmp/mov with interesting immediates
    hits = []
    for ins in md.disasm(d, base):
        try:
            ops = ins.operands
        except Exception:
            continue
        for op in ops:
            if op.type == capstone.arm.ARM_OP_IMM:
                v = op.imm
                if v in (0xAA, 0x55, 0xA0, 0xA1, 0x41, 0x3C, 0x400, 0x4000, 0x00AA, 0x55AA):
                    hits.append((ins.address, ins.mnemonic, ins.op_str, v))
    print('  imm hits:', len(hits))
    for h in hits[:60]:
        print('   %08x %s %s   (0x%x)' % h)
