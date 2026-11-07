"""Event tracer with a bounded ring buffer, consumed by CLI/UI/JSON API."""
import time


class Tracer:
    TYPES = ('call', 'ret', 'mmio_read', 'mmio_write', 'irq_enter', 'irq_exit',
             'exception', 'gpio_change', 'adc', 'dma', 'flash', 'note', 'fault',
             'unimpl', 'rf_tx', 'rf_rx', 'systick')

    def __init__(self, capacity=20000):
        self.capacity = capacity
        self.events = []
        self.seq = 0
        self.enabled = True
        self.filters = None          # set of types or None

    def add(self, type_, pc=0, addr=None, name=None, data=None):
        if not self.enabled:
            return
        if self.filters is not None and type_ not in self.filters:
            return
        self.seq += 1
        ev = {'seq': self.seq, 't': round(time.time(), 3), 'type': type_,
              'pc': pc, 'addr': addr, 'name': name, 'data': data}
        self.events.append(ev)
        if len(self.events) > self.capacity:
            del self.events[:len(self.events) - self.capacity]

    def since(self, seq=0, limit=500):
        out = [e for e in self.events if e['seq'] > seq]
        if limit:
            out = out[-limit:]
        return out

    def clear(self):
        self.events = []
        self.seq = 0

    def last(self, n=200):
        return self.events[-n:]
