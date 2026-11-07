#!/usr/bin/env python
"""hwscan_funcs.py - recursive disassembly of the X3_C app (base 0x08004800)."""
import sys, os, struct, json, pickle
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

APP_BASE = 0x08004800
b = load()
N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

def read_word(va):
    o = va - APP_BASE
    if 0 <= o <= N - 4:
        return struct.unpack_from('<I', b, o)[0]
    return None

def disasm_func(start, maxins=800):
    insns, calls, literals = [], [], []
    va = start
    code = b[va - APP_BASE: va - APP_BASE + maxins*4]
    for ins in md.disasm(code, va):
        insns.append((ins.address, ins.size, ins.mnemonic, ins.op_str, ins.bytes.hex()))
        # PC-relative load
        if '[pc' in ins.op_str and ins.mnemonic.startswith('ldr'):
            try:
                disp = int(ins.op_str.split('#')[1].split(']')[0], 0)
            except Exception:
                disp = 0
            tgt = (ins.address + 4) & ~3
            tgt += disp
            w = read_word(tgt)
            if w is not None:
                literals.append((ins.address, tgt, w))
        # calls
        elif ins.mnemonic in ('bl', 'blx') and ins.op_str.startswith('#'):
            try:
                t = int(ins.op_str[1:], 0)
            except Exception:
                t = None
            if t is not None:
                calls.append((ins.address, t))
        # terminal
        if ins.mnemonic == 'bx' and ins.op_str.strip() == 'lr':
            break
        if ins.mnemonic == 'pop' and 'pc' in ins.op_str:
            break
        if ins.mnemonic in ('b', 'b.w') and ins.op_str.startswith('#'):
            try:
                t = int(ins.op_str[1:], 0)
            except Exception:
                t = 0
            if t == ins.address or t <= start:
                break
        if ins.bytes == b'\xfe\xe7':
            break
        if len(insns) >= maxins:
            break
    end = insns[-1][0] + insns[-1][1] if insns else start
    return insns, calls, literals, end

MMIO_RANGES = [
    (0x40000000, 0x40080000, 'periph'),
    (0x50000000, 0x50080000, 'usb/dcmi'),
    (0xA0000000, 0xA0100000, 'fmc'),
    (0xE0000000, 0xE0100000, 'scs'),
]
def classify(w):
    if w is None:
        return ('none', 0)
    if 0x20000000 <= w < 0x20080000:
        return ('RAM', w)
    if APP_BASE <= w < APP_BASE + N:
        return ('flash', w - APP_BASE)
    for lo, hi, name in MMIO_RANGES:
        if lo <= w < hi:
            return ('MMIO', w)
    if 0 < w < 0x10000:
        return ('small', w)
    return ('other', w)

def build():
    sp = struct.unpack_from('<I', b, 0)[0]
    vecs = [struct.unpack_from('<I', b, 4*(i+1))[0] for i in range(130)]
    starts = set()
    vec_targets = []
    for i, v in enumerate(vecs):
        if v and APP_BASE <= (v & ~1) < APP_BASE + N:
            starts.add(v & ~1)
            vec_targets.append((i, v & ~1))
    work = list(starts)
    funcs = {}
    while work:
        f = work.pop()
        if f in funcs:
            continue
        insns, calls, lits, end = disasm_func(f)
        funcs[f] = (insns, calls, lits, end)
        for at, t in calls:
            tt = t & ~1
            if APP_BASE <= tt < APP_BASE + N and tt not in funcs:
                work.append(tt)
    return sp, vecs, vec_targets, funcs

CACHE = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'hwscan_funcs.pkl')

def build_cached():
    if os.path.exists(CACHE):
        with open(CACHE, 'rb') as fh:
            return pickle.load(fh)
    r = build()
    with open(CACHE, 'wb') as fh:
        pickle.dump(r, fh)
    return r

if __name__ == '__main__':
    sp, vecs, vec_targets, funcs = build_cached()
    from collections import Counter
    print(f"SP={sp:#010x}  IRQ entries={len(vecs)}  functions={len(funcs)}")
    c = Counter(); allmmio = Counter()
    for f, (insns, calls, lits, end) in funcs.items():
        for at, addr, w in lits:
            k, x = classify(w)
            c[k] += 1
            if k == 'MMIO':
                allmmio[w] += 1
    print("literal classes:", dict(c))
    print("MMIO literals (count):")
    for w, cnt in sorted(allmmio.items(), key=lambda kv: (-kv[1], kv[0])):
        print(f"  {w:#010x} x{cnt}")
