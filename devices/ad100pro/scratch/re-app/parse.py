#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Parse out/disasm.txt into a queryable structure (functions, insns, call graph)."""
import sys, os, re, json

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
DIS = os.path.join(ROOT, 'out', 'disasm.txt')

FUNC_RE = re.compile(r'^; =+ (func_[0-9A-F]{8})\s+insns=(\d+)\s+blocks=(\d+)\s+end=0x([0-9A-F]+)')
CALLED_RE = re.compile(r'^;\s+called by:\s*(.+)$')
INSN_RE = re.compile(r'^([0-9A-F]{8})\s+([0-9a-f]+)\s+([a-z][a-z0-9.]*)\s*(.*)$')


def parse():
    funcs = {}
    order = []
    cur = None
    for line in open(DIS, encoding='utf-8', errors='replace'):
        line = line.rstrip('\n')
        m = FUNC_RE.match(line)
        if m:
            name, insns, blocks, end = m.groups()
            addr = int(name[5:], 16)
            cur = {'addr': addr, 'insns': int(insns), 'blocks': int(blocks),
                   'end': int(end, 16), 'called_by': [], 'lines': [], 'code': []}
            funcs[addr] = cur
            order.append(addr)
            continue
        if cur is not None:
            m = CALLED_RE.match(line)
            if m:
                for c in m.group(1).split(','):
                    c = c.strip()
                    if c.startswith('func_'):
                        cur['called_by'].append(int(c[5:13], 16))
                continue
            m = INSN_RE.match(line)
            if m:
                a, b, mn, ops = m.groups()
                cur['code'].append((int(a, 16), b, mn, ops))
                cur['lines'].append(line)
                continue
    # callees
    for a, f in funcs.items():
        f['callees'] = []
    for a, f in funcs.items():
        for (ia, b, mn, ops) in f['code']:
            if mn in ('bl', 'blx', 'b'):
                m = re.search(r'#0x([0-9A-Fa-f]+)', ops)
                if m:
                    t = int(m.group(1), 16)
                    if t in funcs and t != a:
                        f['callees'].append(t)
    for a, f in funcs.items():
        f['callees'] = sorted(set(f['callees']))
    return funcs, order


FUNCS, ORDER = parse()


def show_func(a, maxl=None):
    f = FUNCS.get(a)
    if not f:
        print('no func 0x%08X' % a)
        return
    print('; ========== func_%08X insns=%d blocks=%d end=0x%08X' % (a, f['insns'], f['blocks'], f['end']))
    if f['called_by']:
        print('; called by: ' + ', '.join('%08X' % x for x in f['called_by']))
    for i, ln in enumerate(f['lines']):
        print(ln)
        if maxl and i + 1 >= maxl:
            print('; ... (%d lines total)' % len(f['lines']))
            break


def callers(t):
    return [a for a, f in FUNCS.items() if t in f['callees']]


if __name__ == '__main__':
    cmd = sys.argv[1] if len(sys.argv) > 1 else 'help'
    if cmd == 'show':
        show_func(int(sys.argv[2], 0), int(sys.argv[3]) if len(sys.argv) > 3 else None)
    elif cmd == 'callers':
        for t in sys.argv[2:]:
            tt = int(t, 0)
            print('0x%08X called by: %s' % (tt, ' '.join('%08X' % x for x in callers(tt))))
    elif cmd == 'callees':
        a = int(sys.argv[2], 0)
        f = FUNCS.get(a)
        print(' '.join('%08X' % x for x in (f['callees'] if f else [])))
    elif cmd == 'list':
        for a in ORDER:
            f = FUNCS[a]
            print('%08X insns=%-4d end=0x%08X callers=%d' % (a, f['insns'], f['end'], len(f['called_by'])))
    elif cmd == 'find':
        pat = sys.argv[2]
        rx = re.compile(pat)
        for a in ORDER:
            f = FUNCS[a]
            hits = [ln for ln in f['lines'] if rx.search(ln)]
            if hits:
                print('; --- func_%08X (%d hits)' % (a, len(hits)))
                for h in hits[:12]:
                    print('  ' + h)
    else:
        print('show|callers|callees|list|find')
