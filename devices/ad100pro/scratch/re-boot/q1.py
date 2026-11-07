# -*- coding: utf-8 -*-
import json, os, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # devices/ad100pro
d = json.load(open(os.path.join(ROOT, 'out', 'functions.meta.json')))
which = sys.argv[1] if len(sys.argv) > 1 else 'GPIO'
rows = [(x['addr'], x['periph']) for x in d if any(p.split('.')[0].startswith(which) for p in x['periph'])]
print(len(rows))
for a, pp in rows:
    print(a, ';'.join(p for p in pp if p.split('.')[0].startswith(which)))
