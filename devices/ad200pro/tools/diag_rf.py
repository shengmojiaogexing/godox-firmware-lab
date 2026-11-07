import os, sys, collections
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # devices/ad200pro
from sim.machine import Machine
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_PC

m = Machine()
entries = collections.Counter()
seq = []
def hook(uc, addr, size, ud):
    n = m.fw.symbols.get(addr)
    if n:
        entries[addr] += 1
        if len(seq) < 400:
            seq.append((addr, n))
m.uc.hook_add(UC_HOOK_CODE, hook)
r = m.run(max_insns=600000, stop_on_stall=False)
print('result', r, 'insns', m.insn_count, 'pc', hex(m.stats()['pc']), m.fw.name(m.stats()['pc']))
print('top functions:')
for a, n in entries.most_common(20):
    print('  0x%08x %-30s %d' % (a, m.fw.symbols[a], n))
print('\ncall sequence (first 120):')
for a, n in seq[:120]:
    print('  0x%08x %s' % (a, n))
print('\nrf regs 0x0d/0x0e:', hex(m.rf.regs.get(0x0d, 0)), hex(m.rf.regs.get(0x0e, 0)))
print('rf strobes:', m.rf.log[-20:])
print('spi bytes written count:', len(m.rf.log))
print('device MISO state:', m.device.gpio_inputs)
