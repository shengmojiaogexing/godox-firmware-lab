#!/usr/bin/env python3
"""Second-pass data-flow cross-reference for the AD200 Pro image.

Re-disassembles every recovered function (function list from analysis/functions.json,
instruction bytes from the raw image) and propagates simple constants so that
*every* absolute memory access is recovered:

  * literal loads  (ldr rX, [pc, #imm])  -> constants
  * ldr/str [rBase, #disp] with constant rBase -> target address
  * mov/movs/adds/subs/add/sub of constants

Then prints views:
  funcs    one line per function: size, callees, MMIO, RAM globals touched, data refs
  ram      RAM addresses grouped (which funcs / offsets touch them)
  xref A   who touches a given address (RAM or data/flash)
  data     hex dump of the data region 0x0800d000..0x0800e52c
  names    quick list with tools/an_names.py applied

Author: fw-analyst (task-1).  Read-only; writes nothing.
"""
import os
import re
import sys
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import an_util as U

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_OP_IMM, CS_OP_REG, CS_OP_MEM
from capstone.arm import ARM_REG_PC, ARM_REG_SP, ARM_REG_LR
import capstone.arm as A

MD = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_MCLASS)
MD.detail = True
MD.skipdata = True

# register ids we track
LOW = [A.ARM_REG_R0, A.ARM_REG_R1, A.ARM_REG_R2, A.ARM_REG_R3,
       A.ARM_REG_R4, A.ARM_REG_R5, A.ARM_REG_R6, A.ARM_REG_R7,
       A.ARM_REG_R8, A.ARM_REG_R9, A.ARM_REG_R10, A.ARM_REG_R11, A.ARM_REG_R12]

RAM_LO, RAM_HI = 0x20000000, 0x20001000
DATA_LO, DATA_HI = 0x0800D000, U.END


def insns_of(addr):
    """Re-disassemble a function body (sorted), using the recorded insn bytes."""
    out = []
    for a, raw, text, notes in U.DIS.get(addr, []):
        off = a - U.BASE
        for ins in MD.disasm(U.DATA[off:off + max(2, len(raw) // 2)], a):
            out.append(ins)
            break
    return out


def lit_at(addr):
    """literal value loaded by the ldr [pc,#imm] at addr (from disasm notes)."""
    for a, raw, text, notes in U.DIS.get(CUR_FUNC, []):
        if a != addr:
            continue
        for n in notes:
            m = re.match(r'0x([0-9a-f]{8}) = 0x([0-9a-f]{8})', n)
            if m:
                return int(m.group(2), 16)
    return None


def scan_func(fn):
    """Return list of access records for one function:
    {'insn':a,'addr':t,'rw':'r'/'w','size':n,'kind':'ram'/'data'/'periph'}"""
    acc = []
    regs = {}
    for ins in insns_of(fn):
        ops = ins.operands
        mn = ins.mnemonic
        # --- literal load
        if len(ops) == 2 and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_MEM:
            m = ops[1].mem
            if m.base == ARM_REG_PC:
                v = lit_at(ins.address)
                if v is not None:
                    regs[ops[0].reg] = ('c', v)
            else:
                b = regs.get(m.base)
                if b and b[0] == 'c':
                    t = b[1] + m.disp
                    kind = ('ram' if RAM_LO <= t < RAM_HI else
                            'data' if DATA_LO <= t < DATA_HI else
                            'periph' if t >= 0x40000000 else 'other')
                    if kind != 'other':
                        sz = 4
                        if 'ldrb' in mn or mn.startswith('strb'):
                            sz = 1
                        elif 'ldrh' in mn or mn.startswith('strh'):
                            sz = 2
                        rw = 'r' if mn.startswith('ldr') else 'w' if mn.startswith('str') else '?'
                        acc.append({'insn': ins.address, 'addr': t, 'rw': rw,
                                    'size': sz, 'kind': kind})
                        if mn.startswith('ldr') and not ("ldrb" in mn or "ldrh" in mn):
                            regs[ops[0].reg] = ('?',)
        # --- constant building
        if mn in ('movs', 'mov') and len(ops) == 2 and ops[0].type == CS_OP_REG:
            if ops[1].type == CS_OP_IMM:
                regs[ops[0].reg] = ('c', ops[1].imm)
            elif ops[1].type == CS_OP_REG and ops[1].reg in regs:
                regs[ops[0].reg] = regs[ops[1].reg]
        elif mn in ('adds', 'add', 'subs', 'sub') and len(ops) == 3:
            if ops[0].type == CS_OP_REG and ops[1].type == CS_OP_REG and ops[1].reg in regs:
                b = regs[ops[1].reg]
                if b[0] == 'c':
                    delta = ops[2].imm if ops[2].type == CS_OP_IMM else None
                    if delta is not None:
                        regs[ops[0].reg] = ('c', b[1] + (delta if mn.startswith('add') else -delta))
        # unknown writes clear the reg
        if ops and ops[0].type == CS_OP_REG and ops[0].reg in regs:
            if mn.startswith('ldr') and not mn.startswith('ldrs') and len(ops) > 1 and ops[1].type == CS_OP_MEM:
                pass
            elif mn in ('add', 'adds', 'sub', 'subs', 'mov', 'movs'):
                pass
            elif mn.startswith('ldr') or mn.startswith('adr') or mn in ('lsls', 'lsrs', 'asrs', 'ands', 'orrs', 'eors', 'muls', 'uxth', 'uxtb'):
                if not (len(ops) == 2 and ops[1].type == CS_OP_MEM and ops[1].mem.base == ARM_REG_PC):
                    regs.pop(ops[0].reg, None)
    return acc


CUR_FUNC = 0
ACC = {}


def build():
    for fn in U.FUNCS:
        CUR_FUNC = fn
        globals()['CUR_FUNC'] = fn
        ACC[fn] = scan_func(fn)


# ---------------------------------------------------------------- views
def names():
    try:
        import an_names
        tab = an_names.NAMES
    except Exception:
        tab = {}
    return tab


def nm(a):
    tab = names()
    return tab.get(a) or 'FUNC_%08x' % a


def v_funcs():
    for a in sorted(U.FUNCS):
        rec = U.FUNCS[a]
        acc = ACC.get(a, [])
        rams = sorted(set(x['addr'] for x in acc if x['kind'] == 'ram'))
        datas = sorted(set(x['addr'] for x in acc if x['kind'] == 'data'))
        per = sorted(set(x['addr'] for x in acc if x['kind'] == 'periph'))
        calls = ' '.join(nm(c) for c in rec['calls'])
        print('%08x %-28s %4dins c=%-46s mmio=%-40s ram=[%s] data=[%s] %s' % (
            a, nm(a), rec['insn_count'], calls[:46],
            ' '.join(sorted(set(m['name'] for m in rec['mmio'])))[:40],
            ' '.join('%08x' % x for x in rams)[:120],
            ' '.join('%08x' % x for x in datas)[:60],
            ('str=%r' % rec['strings'][:2]) if rec['strings'] else ''))


def v_ram():
    byaddr = defaultdict(list)
    for fn, acc in ACC.items():
        for x in acc:
            if x['kind'] == 'ram':
                byaddr[x['addr']].append((fn, x['insn'], x['rw'], x['size']))
    for addr in sorted(byaddr):
        recs = byaddr[addr]
        fns = sorted(set(r[0] for r in recs))
        rws = ''.join(sorted(set(r[2] for r in recs)))
        print('0x%08x %s funcs=%s' % (addr, rws, ' '.join(nm(f) for f in fns)))


def v_xref(addr):
    for fn in sorted(U.FUNCS):
        for x in ACC.get(fn, []):
            if x['addr'] == addr:
                print('0x%08x  %-28s insn=%08x %s%d' % (
                    x['addr'], nm(fn), x['insn'], x['rw'], x['size']))


def v_data():
    a = 0x0800D000
    while a < U.END:
        row = []
        for i in range(4):
            w = U.rd32(a + i * 4)
            row.append('%08x' % (w if w is not None else 0))
        asc = ''
        for i in range(16):
            c = U.rd8(a + i)
            asc += chr(c) if c is not None and 32 <= c < 127 else '.'
        # flag words that are flash pointers
        marks = []
        for i in range(4):
            w = U.rd32(a + i * 4)
            if w is not None and U.in_img(w & ~1) and (w & 1):
                marks.append('' if i == 0 else ' ')
                marks.append('>f%03x' % (i * 4))
            if w is not None and 0x20000000 <= w < 0x20001000:
                marks.append('>r%03x' % (i * 4))
        print('0x%08x  %s  |%s| %s' % (a, '  '.join(row), asc, ' '.join(marks)))
        a += 16


def v_names():
    tab = names()
    for a in sorted(tab):
        print('%08x  %s' % (a, tab[a]))


if __name__ == '__main__':
    mode = sys.argv[1] if len(sys.argv) > 1 else 'funcs'
    if mode != 'data':
        build()
    if mode == 'funcs':
        v_funcs()
    elif mode == 'ram':
        v_ram()
    elif mode == 'xref':
        for a in sys.argv[2:]:
            v_xref(int(a, 0))
    elif mode == 'data':
        v_data()
    elif mode == 'names':
        v_names()
