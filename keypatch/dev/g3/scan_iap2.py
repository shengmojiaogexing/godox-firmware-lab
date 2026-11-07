import sys
import capstone
from capstone import arm as cs_arm

sys.stdout.reconfigure(encoding='utf-8')

files = [
    (r'devices\ad100pro\firmware\AD100pro_V1.2.bin', 0x08003000),
    (r'devices\ad200pro\firmware\AD200pro_V1.60.bin', 0x08003000),
    (r'devices\x3c\firmware\X3_C_v1.38_app.bin', 0x08004800),
]
md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB)
md.detail = True
INTEREST = {0xAA, 0x55, 0xA0, 0xA1, 0x41, 0x3C, 0x400, 0x4000, 0x55AA}

for path, base in files:
    d = open(path, 'rb').read()
    print('=' * 70)
    print(path, len(d), 'base', hex(base))
    hits = []
    for ins in md.disasm(d, base):
        try:
            ops = ins.operands
        except Exception:
            continue
        for op in ops:
            if op.type == cs_arm.ARM_OP_IMM and (op.imm & 0xffffffff) in INTEREST:
                hits.append((ins.address, ins.mnemonic, ins.op_str, op.imm))
                break
    print('  imm hits:', len(hits))
    for a, m, o, v in hits[:80]:
        print('   %08x %s %s (0x%x)' % (a, m, o, v))
