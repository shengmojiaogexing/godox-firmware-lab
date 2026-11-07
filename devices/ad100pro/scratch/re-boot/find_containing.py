# -*- coding: utf-8 -*-
"""Find which function block contains an address and print its callers."""
import os, re, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # devices/ad100pro
PATH = os.path.join(ROOT, 'out', 'disasm.txt')
lines = open(PATH, encoding='utf-8', errors='replace').read().splitlines()
blocks = []
cur = None
for i, ln in enumerate(lines):
    m = re.match(r'^;\s*=+\s*func_([0-9A-Fa-f]{8})\s+insns=(\d+).*end=0x([0-9A-Fa-f]+)', ln)
    if m:
        cur = {'addr': int(m.group(1), 16), 'end': int(m.group(3), 16), 'line': i, 'callers': ''}
        nxt = lines[i+1] if i+1 < len(lines) else ''
        mm = re.match(r'^;\s*called by:\s*(.*)', nxt)
        if mm:
            cur['callers'] = mm.group(1)
            cur['cline'] = i+1
        blocks.append(cur)
target = int(sys.argv[1], 16)
for b in blocks:
    cover = b['addr'] <= target < max(b['end'], b['addr']+1)
    if cover:
        print('func_%08X end=0x%08X  callers: %s' % (b['addr'], b['end'], b['callers'] or '(none)'))
        # also print any labels inside
# print candidate blocks containing target by address order
for b in blocks:
    if b['addr'] <= target < b['end']:
        print('  contains: func_%08X' % b['addr'])
