# -*- coding: utf-8 -*-
"""Parse out/disasm.txt into function blocks; query instructions around calls."""
import os, re, sys, json

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # devices/ad100pro
PATH = os.path.join(ROOT, 'out', 'disasm.txt')
lines = open(PATH, encoding='utf-8', errors='replace').read().splitlines()

blocks = []  # list of dict(header, start_line, end_line, lines)
cur = None
for i, ln in enumerate(lines):
    m = re.match(r'^;\s*=+\s*func_([0-9A-Fa-f]{8})', ln)
    if m:
        if cur: blocks.append(cur)
        cur = {'addr': m.group(1).lower(), 'line': i, 'body': []}
    elif cur is not None:
        if ln.startswith('; ===='):
            blocks.append(cur); cur = None
        else:
            cur['body'].append(ln)
if cur: blocks.append(cur)

by_addr = {}
for b in blocks:
    by_addr.setdefault(b['addr'], []).append(b)

def body_text(b):
    return '\n'.join(b['body'])

def find_funcs_with(pat):
    out = []
    r = re.compile(pat)
    for b in blocks:
        if r.search(body_text(b)):
            out.append(b)
    return out

def dump(addr, before=0, after=0, n=None):
    bs = by_addr.get(addr)
    if not bs:
        print('// no block', addr); return
    for b in bs:
        body = b['body']
        if n: body = body[:n]
        print('; ==================== func_%s (line %d)' % (b['addr'], b['line']))
        print('\n'.join(body))

if __name__ == '__main__':
    cmd = sys.argv[1]
    if cmd == 'callers':
        pat = sys.argv[2]
        bs = find_funcs_with(pat)
        for b in bs:
            print(b['addr'])
    elif cmd == 'dump':
        n = int(sys.argv[3]) if len(sys.argv) > 3 else None
        dump(sys.argv[2], n=n)
    elif cmd == 'grep':
        r = re.compile(sys.argv[2])
        for b in blocks:
            if r.search(body_text(b)):
                print(b['addr'])
    elif cmd == 'funcs':
        for b in blocks:
            print(b['addr'], len(b['body']))
