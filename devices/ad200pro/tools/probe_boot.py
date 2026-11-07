#!/usr/bin/env python3
"""Fast boot probe: find where the firmware stalls under a bare MMIO (0-read) model.

Usage: python tools/probe_boot.py [max_slices]
Prints the stall loop and the MMIO accesses inside it, to inform the emulator's
peripheral models.
"""
import os, sys, struct, collections
from unicorn import *
from unicorn.arm_const import *

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(ROOT, 'firmware', 'AD200pro_V1.60.bin')
BASE = 0x08003000
data = open(BIN, 'rb').read()

FLASH = 0x08000000
RAM = 0x20000000
u = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
u.mem_map(FLASH, 0x10000)
u.mem_write(FLASH, b'\xff' * 0x3000)
u.mem_write(BASE, data)
u.mem_map(RAM, 0x2000)
PER = 0x40000000
u.mem_map(PER, 0x40000)
GPIO = 0x48000000
u.mem_map(GPIO, 0x2000)
SCS = 0xE0000000
u.mem_map(SCS, 0x100000)

regions = [(PER, 0x40000), (GPIO, 0x2000), (SCS, 0x100000)]
mmio_log = collections.deque(maxlen=4000)

def in_regions(addr):
    for b, s in regions:
        if b <= addr < b + s:
            return True
    return False

def on_read(uc, access, address, size, value, ud):
    if in_regions(address):
        pc = uc.reg_read(UC_ARM_REG_PC)
        mmio_log.append(('r', address, size, pc))

def on_write(uc, access, address, size, value, ud):
    if in_regions(address):
        pc = uc.reg_read(UC_ARM_REG_PC)
        mmio_log.append(('w', address, size, pc))

u.hook_add(UC_HOOK_MEM_READ, on_read)
u.hook_add(UC_HOOK_MEM_WRITE, on_write)

sp = struct.unpack_from('<I', data, 0)[0]
pc = struct.unpack_from('<I', data, 4)[0] & ~1
print('reset sp=0x%08x pc=0x%08x' % (sp, pc))
u.reg_write(UC_ARM_REG_SP, sp)
u.reg_write(UC_ARM_REG_PC, pc)

# run slices, track pc every slice
pcs = []
n_slices = int(sys.argv[1]) if len(sys.argv) > 1 else 200
SLICE = 2000
for i in range(n_slices):
    try:
        u.emu_start(pc | 1, 0, count=SLICE)
    except UcError as e:
        print('UC ERROR at pc=0x%08x: %s' % (u.reg_read(UC_ARM_REG_PC), e))
        break
    pc = u.reg_read(UC_ARM_REG_PC)
    pcs.append(pc)
    if i % 20 == 19:
        print('slice %3d pc=0x%08x sp=0x%08x' % (i, pc, u.reg_read(UC_ARM_REG_SP)))

# detect stall: last 60 slice PCs have <=3 unique values
tail = pcs[-80:]
uniq = collections.Counter(tail)
print('\nlast PCs:', [(hex(a), n) for a, n in uniq.most_common(6)])
if len(uniq) <= 3:
    print('*** STALL DETECTED ***')
    regs = {n: u.reg_read(reg) for n, reg in [('r0', UC_ARM_REG_R0), ('r1', UC_ARM_REG_R1),
            ('r2', UC_ARM_REG_R2), ('r3', UC_ARM_REG_R3), ('r4', UC_ARM_REG_R4),
            ('r5', UC_ARM_REG_R5), ('r6', UC_ARM_REG_R6), ('r7', UC_ARM_REG_R7),
            ('sp', UC_ARM_REG_SP), ('lr', UC_ARM_REG_LR), ('pc', UC_ARM_REG_PC)]}
    print('regs:', {k: hex(v) for k, v in regs.items()})
    print('\nlast MMIO accesses (last 40):')
    for e in list(mmio_log)[-40:]:
        print('  %s 0x%08x size=%d pc=0x%08x' % e)
    print('\nMMIO addresses in last 2000 accesses:')
    c = collections.Counter((a, rw) for rw, a, s, p in list(mmio_log)[-2000:])
    for (a, rw), n in c.most_common(20):
        print('  0x%08x %s x%d' % (a, rw, n))
