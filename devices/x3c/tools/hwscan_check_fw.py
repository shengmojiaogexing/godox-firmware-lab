#!/usr/bin/env python
"""hwscan_check_fw.py - quick structural check of a Godox firmware (vector table, reset handler)."""
import sys, os, struct, math
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def check(path, base=0x08000000, label=''):
    b = open(path, 'rb').read()
    n = len(b)
    print(f"==== {label} {path} size {n:#x} sha-ish")
    sp = struct.unpack_from('<I', b, 0)[0]
    print(f"SP={sp:#010x}")
    # entropy per 64k
    ent = []
    for blk in range(0, n, 65536):
        chunk = b[blk:blk+65536]
        if not chunk: break
        cnt = [0]*256
        for c in chunk: cnt[c] += 1
        H = 0.0
        for c in cnt:
            if c:
                p = c/len(chunk)
                H -= p*math.log2(p)
        ent.append((blk, H))
    print("entropy/64k:", ' '.join(f"{blk//1024}k:{H:.2f}" for blk, H in ent[:20]))
    # vectors
    vecs = [struct.unpack_from('<I', b, 4*(i+1))[0] for i in range(140)]
    # find last nonzero
    last = max(i for i, v in enumerate(vecs) if v)
    print(f"last nonzero vector index {last} -> {last+1} entries ({last-14} IRQs)")
    for i in range(0, min(last+1, 20)):
        v = vecs[i]
        t = v & ~1
        off = t - base
        ins = []
        if 0 <= off < n:
            for k, x in enumerate(md.disasm(b[off:off+32], t)):
                ins.append(f"{x.mnemonic} {x.op_str}")
                if k >= 4: break
        print(f"  vec{i+1:3d} (IRQ{i-15:4d}) {v:#010x} -> " + ' ; '.join(ins))
    print()

if __name__ == '__main__':
    _repo = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                         '..', '..', '..'))
    check(os.path.join(_repo, 'devices', 'ad200pro', 'firmware',
                       'AD200pro_V1.60.bin'), label='AD200pro')
    check(os.path.join(_repo, 'devices', 'ad100pro', 'firmware',
                       'AD100pro_V1.2.bin'), label='AD100pro')
