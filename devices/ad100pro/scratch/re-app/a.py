#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""re-app analysis helper: vectors / disasm / literal xrefs / RAM scan"""
import sys, os, struct, json
import capstone

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
FW = os.path.join(ROOT, 'firmware', 'AD100pro_V1.2.bin')
BASE = 0x08003000
DATA = open(FW, 'rb').read()
END = BASE + len(DATA)

md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB + capstone.CS_MODE_MCLASS)
md.detail = True
md.skipdata = False


def off(a):
    return a - BASE


def getw(a):
    o = off(a)
    if 0 <= o <= len(DATA) - 4:
        return struct.unpack_from('<I', DATA, o)[0]
    return None


def is_code_ptr(v):
    return v is not None and BASE <= v < END


def dis(addr, count=200, title=None):
    o = off(addr)
    code = DATA[o:o + count * 4 + 16]
    out = []
    if title:
        out.append('; ---- %s ----' % title)
    out.append('; disasm from 0x%08X' % addr)
    n = 0
    for ins in md.disasm(code, addr):
        if ins.address >= END:
            break
        s = '%08X  %-12s %-8s %s' % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str)
        if '[pc' in ins.op_str:
            try:
                imm = int(ins.op_str.split('#')[1].split(']')[0], 0)
                lit = ((ins.address + 4) & ~3) + imm
                w = getw(lit)
                if w is not None:
                    s += '   ; [0x%08X] = 0x%08X' % (lit, w)
            except Exception:
                pass
        out.append(s)
        n += 1
        if n >= count:
            break
    return '\n'.join(out)


def vec_table():
    names = ['SP', 'Reset', 'NMI', 'HardFault', 'MemManage', 'BusFault', 'UsageFault',
             'RSVD7', 'RSVD8', 'RSVD9', 'RSVD10', 'SVC', 'RSVD12', 'RSVD13', 'PendSV', 'SysTick']
    for i in range(16, 48):
        names.append('IRQ%d' % (i - 16))
    lines = []
    for i, nm in enumerate(names):
        v = getw(BASE + i * 4)
        note = ''
        if v is not None:
            if i > 0 and is_code_ptr(v):
                tgt = v & ~1
                note = 'handler=0x%08X' % tgt
                if tgt in (0x080030E6, 0x080030E4):
                    note += ' (default loop)'
        lines.append('%2d 0x%08X: 0x%08X  %-11s %s' % (i, BASE + i * 4, v if v is not None else 0, nm, note))
    return '\n'.join(lines)


def funcs():
    return json.load(open(os.path.join(ROOT, 'out', 'functions.json')))


def fmap():
    return {int(f['addr'], 16): f for f in funcs()}


def find_func(addr):
    best = None
    for a, f in fmap().items():
        if a <= addr < int(f['end'], 16):
            if best is None or a > best[0]:
                best = (a, f)
    return best


def all_lits():
    res = []
    for ins in md.disasm(DATA, BASE):
        if ins.address >= END:
            break
        if '[pc' in ins.op_str:
            try:
                imm = int(ins.op_str.split('#')[1].split(']')[0], 0)
                lit = ((ins.address + 4) & ~3) + imm
                w = getw(lit)
                res.append((ins.address, ins.mnemonic, lit, w))
            except Exception:
                pass
    return res


def lits_to(lo, hi):
    return [(a, mn, lit, w) for a, mn, lit, w in all_lits() if w is not None and lo <= w < hi]


def callers_of(target):
    res = set()
    for f in funcs():
        for c in f.get('callees', []):
            if c and int(c, 16) == target:
                res.add(int(f['addr'], 16))
    return sorted(res)


def xref_lit(target):
    return [(a, mn, lit) for a, mn, lit, w in all_lits() if w == target]


def list_funcs(minins=20, maxn=100):
    fs = sorted(funcs(), key=lambda f: -f.get('insns', 0))[:maxn]
    for f in fs:
        if f.get('insns', 0) >= minins:
            print('%08X insns=%-4d blocks=%-3d size=%-4d callees=%s' % (
                int(f['addr'], 16), f['insns'], len(f.get('blocks', [])), f['size'],
                ','.join(c[2:] for c in f.get('callees', [])[:8])))


def strings(minlen=4):
    res = []
    cur = b''
    start = 0
    for i in range(len(DATA)):
        c = DATA[i]
        if 0x20 <= c < 0x7f:
            if not cur:
                start = i
            cur += bytes([c])
        else:
            if len(cur) >= minlen:
                res.append((BASE + start, cur.decode('ascii')))
            cur = b''
    return res


CMDS = 'vec|dis|func|top|lits|xlit|callers|str|raw'


if __name__ == '__main__':
    cmd = sys.argv[1] if len(sys.argv) > 1 else 'help'
    if cmd == 'vec':
        print(vec_table())
    elif cmd == 'dis':
        a = int(sys.argv[2], 0)
        n = int(sys.argv[3], 0) if len(sys.argv) > 3 else 120
        print(dis(a, n))
    elif cmd == 'func':
        r = find_func(int(sys.argv[2], 0))
        print(('func 0x%08X: ' % r[0]) + json.dumps(r[1], indent=1)[:4000] if r else 'no func')
    elif cmd == 'top':
        list_funcs(int(sys.argv[2]) if len(sys.argv) > 2 else 20)
    elif cmd == 'lits':
        for a, mn, lit, w in lits_to(int(sys.argv[2], 0), int(sys.argv[3], 0)):
            print('0x%08X %s lit@0x%08X -> 0x%08X' % (a, mn, lit, w))
    elif cmd == 'xlit':
        for a, mn, lit in xref_lit(int(sys.argv[2], 0)):
            print('0x%08X %s -> lit@0x%08X' % (a, mn, lit))
    elif cmd == 'callers':
        print(' '.join('%08X' % x for x in callers_of(int(sys.argv[2], 0))))
    elif cmd == 'str':
        for a, s in strings():
            print('0x%08X %r' % (a, s))
    elif cmd == 'raw':
        a = int(sys.argv[2], 0); n = int(sys.argv[3], 0)
        print(DATA[off(a):off(a) + n].hex())
    else:
        print(CMDS)
