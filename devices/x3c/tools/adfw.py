#!/usr/bin/env python3
"""AD200 Pro firmware static analysis toolkit.

Image layout (derived):
    file offset 0 <=> flash address 0x08003000
    (the update file contains the application image only; a 12 KiB
     bootloader occupies 0x08000000..0x08002FFF on the device)

Usage:
    python adfw.py info
    python adfw.py disasm [--start HEX] [--end HEX] [--out FILE]
    python adfw.py funcs  [--out FILE]
    python adfw.py mmio   [--out FILE]
    python adfw.py refs   ADDR
    python adfw.py sweep
"""
import sys, os, json, struct, argparse, re
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_OP_IMM, CS_OP_MEM, CS_OP_REG
from capstone.arm import ARM_OP_MEM, ARM_OP_IMM, ARM_REG_PC, ARM_REG_SP, ARM_REG_LR

BASE = 0x08003000
HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.abspath(os.path.join(HERE, '..', '..', '..'))  # repo root
BIN = os.path.join(REPO, 'devices', 'ad200pro', 'firmware', 'AD200pro_V1.60.bin')

DATA = open(BIN, 'rb').read()
SIZE = len(DATA)

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_MCLASS)
md.detail = True
md.skipdata = True

def rd32(addr):
    off = addr - BASE
    if 0 <= off <= SIZE - 4:
        return struct.unpack_from('<I', DATA, off)[0]
    return None

def rd16(addr):
    off = addr - BASE
    if 0 <= off <= SIZE - 2:
        return struct.unpack_from('<H', DATA, off)[0]
    return None

def rd8(addr):
    off = addr - BASE
    if 0 <= off < SIZE:
        return DATA[off]
    return None

def in_image(addr):
    return BASE <= addr < BASE + SIZE

# ---------------------------------------------------------------- vectors
def vectors():
    w = struct.unpack_from('<48I', DATA, 0)
    names = {
        0: 'SP', 1: 'Reset', 2: 'NMI', 3: 'HardFault',
        11: 'SVCall', 14: 'PendSV', 15: 'SysTick',
    }
    for i in range(16, 48):
        names[i] = 'IRQ%d' % (i - 16)
    out = []
    for i, v in enumerate(w):
        out.append({'idx': i, 'name': names.get(i, 'reserved'), 'raw': v,
                    'addr': v & ~1, 'thumb': bool(v & 1)})
    return out

def cleanup(v):
    """turn a raw thumb pointer into an even address"""
    return v & ~1 if v else v

# ---------------------------------------------------------------- disasm helpers
def dis1(addr):
    code = DATA[addr - BASE: addr - BASE + 4] if in_image(addr) else b''
    if len(code) < 2:
        return None
    ins = list(md.disasm(code, addr))
    return ins[0] if ins else None

def dis_range(start, end, maxn=100000):
    off = start - BASE
    code = DATA[off: end - BASE]
    return list(md.disasm(code, start))[:maxn]

def lit_target(ins):
    """for ldr rX,[pc,#imm] returns the literal address"""
    if ins.id == 0 or ins.mnemonic.startswith('.'):
        return None
    if len(ins.operands) == 2 and ins.operands[0].type == CS_OP_REG \
       and ins.operands[1].type == CS_OP_MEM:
        m = ins.operands[1].mem
        if m.base == ARM_REG_PC:
            return (ins.address + 4) & ~3 + m.disp
    return None

def literal(ins):
    """if instruction loads a word literal, return (value, lit_addr)"""
    lt = lit_target(ins)
    if lt is None:
        return None
    v = rd32(lt)
    if v is None:
        return None
    return v, lt

def classify(v):
    if v is None:
        return 'none'
    if 0x08000000 <= v < 0x08004000:
        return 'flash:(bootloader?)'
    if BASE <= v < BASE + SIZE:
        return 'flash'
    if 0x08000000 <= v < 0x08010000:
        return 'flash-outside-image'
    if 0x20000000 <= v < 0x20020000:
        return 'ram'
    if 0x40000000 <= v < 0x40100000:
        return 'periph'
    if 0x48000000 <= v < 0x48100000:
        return 'gpio'
    if 0xE0000000 <= v < 0xE0100000:
        return 'scs'
    if v < 0x10000:
        return 'small'
    return 'other'

# ---------------------------------------------------------------- function discovery
def find_bl_targets():
    """linear sweep 2-byte aligned over image for BL/BLX instruction targets"""
    tgt = set()
    off = 0
    data = DATA
    # cheap scan for BL/BLX encodings: 0xF000-0xF7FF followed by 0xD000-0xFFFF-ish
    for i in range(0, SIZE - 4, 2):
        hw1 = data[i] | (data[i+1] << 8)
        if 0xF000 <= hw1 <= 0xF7FF:
            hw2 = data[i+2] | (data[i+3] << 8)
            if hw2 & 0x8000:
                ins = dis1(BASE + i)
                if ins and ins.mnemonic in ('bl', 'blx') and ins.operands:
                    op = ins.operands[0]
                    if op.type == CS_OP_IMM:
                        tgt.add(op.imm & ~1)
    # also scan 32-bit 'bl' only via capstone linear sweep for correctness
    for ins in md.disasm(data, BASE):
        if ins.mnemonic in ('bl', 'blx') and ins.operands and ins.operands[0].type == CS_OP_IMM:
            tgt.add(ins.operands[0].imm & ~1)
    return sorted(t for t in tgt if in_image(t))

def find_literal_refs():
    """scan for ldr rX,[pc,#imm] and return dict lit_addr -> list of insn addr"""
    refs = {}
    for ins in md.disasm(DATA, BASE):
        lt = lit_target(ins)
        if lt is not None and refs.get(lt) is None:
            refs[lt] = ins.address
    return refs

def mmio_candidates():
    """collect literal values that look like peripheral/register addresses with the
    code address that loads them"""
    out = []
    for ins in md.disasm(DATA, BASE):
        lit = literal(ins)
        if not lit:
            continue
        v, la = lit
        c = classify(v)
        if c in ('periph', 'gpio', 'scs'):
            out.append({'insn': ins.address, 'lit_addr': la, 'value': v, 'class': c})
    return out

# ---------------------------------------------------------------- outputs
def dump(x):
    print(json.dumps(x, indent=1, ensure_ascii=False))

def cmd_info(args):
    print('file size : %d (0x%x)' % (SIZE, SIZE))
    print('base      : 0x%08x .. 0x%08x' % (BASE, BASE + SIZE - 1))
    vs = vectors()
    print('\nvector table:')
    for v in vs:
        t = v['addr']
        note = ''
        if t:
            off = t - BASE
            note = ('file+0x%x' % off) if 0 <= off < SIZE else 'OUTSIDE IMAGE'
        print('  %2d %-9s 0x%08x -> 0x%08x %s' % (v['idx'], v['name'], v['raw'], t, note))
    # count unique handlers besides default
    print('\nunique non-default handlers:')
    seen = {}
    for v in vs:
        if v['idx'] < 16 or not v['addr']:
            continue
        seen.setdefault(v['addr'], []).append(v['name'])
    for a, names in sorted(seen.items()):
        print('  0x%08x  %s' % (a, ','.join(names)))

def cmd_sweep(args):
    """estimate code vs data by decoding 2-byte strides and checking BL density"""
    for off in range(0, SIZE, 0x400):
        n = sum(1 for _ in md.disasm(DATA[off:off+0x400], BASE + off))
        unk = DATA[off:off+0x400]
        print('0x%05x  insns=%3d  %s' % (off, n, ''))
    print('\ntotal disassembled:', sum(1 for _ in md.disasm(DATA, BASE)))

def cmd_disasm(args):
    start = int(args.start, 0) if args.start else BASE
    end = int(args.end, 0) if args.end else BASE + SIZE
    out = []
    for ins in md.disasm(DATA[start - BASE:end - BASE], start):
        out.append('%08x: %-8s %-24s' % (ins.address, ins.bytes.hex(), ins.mnemonic + ' ' + ins.op_str))
        lit = literal(ins)
        if lit:
            out.append('%08x:   ; = 0x%08x  (%s)' % (ins.address, lit[0], classify(lit[0])))
    txt = '\n'.join(out)
    if args.out:
        open(args.out, 'w', encoding='utf-8').write(txt)
        print('wrote', args.out, len(out), 'lines')
    else:
        print(txt)

def cmd_refs(args):
    addr = int(args.addr, 0)
    print('refs to 0x%08x:' % addr)
    # literal refs
    for ins in md.disasm(DATA, BASE):
        lit = literal(ins)
        if lit and lit[0] == addr:
            print('  literal @ %08x insn %s %s' % (lit[1], ins.mnemonic, ins.op_str))
        for op in ins.operands:
            if op.type == CS_OP_IMM and (op.imm & 0xfffffffe) == (addr & ~1) and ins.mnemonic in ('bl','blx','b','bx'):
                print('  branch @ %08x' % ins.address)

def funcs_export(args):
    bl = find_bl_targets()
    vecs = [v['addr'] for v in vectors() if v['addr'] and in_image(v['addr'])]
    entry = sorted(set(bl) | set(vecs))
    print('candidate function starts:', len(entry))
    out = []
    for a in entry:
        ins = dis1(a)
        out.append({'addr': a, 'first': (ins.mnemonic + ' ' + ins.op_str) if ins else None})
    if args.out:
        json.dump(out, open(args.out, 'w', encoding='utf-8'), indent=1)
        print('wrote', args.out)
    else:
        for o in out:
            print('%08x  %s' % (o['addr'], o['first']))

def cmd_mmio(args):
    lits = find_literal_refs()
    refs = mmio_candidates()
    print('peripheral literal loads:', len(refs))
    byval = {}
    for r in refs:
        byval.setdefault(r['value'], []).append(r['insn'])
    for v in sorted(byval):
        print('0x%08x %-8s x%-2d  first@%08x' % (v, classify(v), len(byval[v]), byval[v][0]))

def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest='cmd', required=True)
    sub.add_parser('info')
    sub.add_parser('sweep')
    p = sub.add_parser('disasm'); p.add_argument('--start'); p.add_argument('--end'); p.add_argument('--out')
    p = sub.add_parser('refs'); p.add_argument('addr')
    p = sub.add_parser('funcs'); p.add_argument('--out')
    p = sub.add_parser('mmio')
    a = ap.parse_args()
    {'info': cmd_info, 'sweep': cmd_sweep, 'disasm': cmd_disasm, 'refs': cmd_refs,
     'funcs': funcs_export, 'mmio': cmd_mmio}[a.cmd](a)

if __name__ == '__main__':
    main()
