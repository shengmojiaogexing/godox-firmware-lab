p = 'sim/machine.py'
s = open(p, encoding='utf-8').read()
start = s.index('class _StallDetector:')
new = '''class _StallDetector:
    """Detect a hard hang: the PC stays in a tiny window *and* instructions inside
    that window keep reading the same MMIO register (a hardware wait the model does
    not satisfy).  A legitimate idle loop (e.g. main waiting for the TIM14 tick)
    runs no such polling code, so it is never flagged."""

    def __init__(self, tracer=None):
        self.pcs = []
        self.tracer = tracer

    def feed(self, pc):
        self.pcs.append(pc)
        if len(self.pcs) > 120:
            self.pcs.pop(0)

    def stalled(self):
        if len(self.pcs) < 120:
            return False
        if (max(self.pcs) - min(self.pcs)) > 0x200:
            return False
        lo, hi = min(self.pcs) - 0x40, max(self.pcs) + 0x40
        counts = {}
        for e in getattr(self.tracer, 'last', lambda n: [])(4000):
            if e['type'] == 'mmio_read' and e['pc'] and lo <= e['pc'] <= hi:
                counts[e['addr']] = counts.get(e['addr'], 0) + 1
        return bool(counts) and max(counts.values()) > 150
'''
s = s[:start] + new
open(p, 'w', encoding='utf-8').write(s)
print('stall detector v3')
