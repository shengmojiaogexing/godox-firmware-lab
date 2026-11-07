#!/usr/bin/env python3
"""Boot probe v3: walk the firmware to the main loop, discovering every wait.

- Simulates a 1 kHz SysTick tick counter at 0x20000100 (what the real SysTick ISR does).
- Detects stall (PC stays in <=2 4 KiB pages) and un-sticks the polled MMIO register
  using STM32F0 ready-bit semantics; logs what was needed.

Run: python tools/boot_probe.py [max_unsticks]
"""
import os, sys, struct, collections, json
from unicorn import *
from unicorn.arm_const import *

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
data = open(os.path.join(ROOT, 'firmware', 'AD200pro_V1.60.bin'), 'rb').read()
BASE = 0x08003000
MAIN_LO, MAIN_HI = 0x0800CE40, 0x0800CEB0
TICK = 0x20000100

PER, GPIO, SCS = 0x40000000, 0x48000000, 0xE0000000
u = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
for base, size in [(0x08000000, 0x20000), (0x20000000, 0x2000), (PER, 0x40000),
                   (GPIO, 0x2000), (SCS, 0x100000)]:
    u.mem_map(base, size)
u.mem_write(0x08000000, b'\xff' * 0x20000)
u.mem_write(BASE, data)
# fast-forward: stub the busy-wait delay function 0x0800C768 (pure nop loops)
for _a in (0x0800C768, 0x0800C798, 0x0800C7C8, 0x0800C828):
    u.mem_write(_a, bytes.fromhex('7047'))

def in_per(a):
    return PER <= a < PER + 0x40000 or GPIO <= a < GPIO + 0x2000 or SCS <= a < SCS + 0x100000

mmio = collections.deque(maxlen=3000)
def on_read(uc, access, addr, size, value, ud):
    if in_per(addr):
        slice_reads[0] += 1
        if PENDING:
            flush_pending(uc)
        mmio.append(('r', addr, size, uc.reg_read(UC_ARM_REG_PC)))
SIDE = {}
def side_effect(uc, addr, size):
    """apply STM32F0 'ready bit' behaviour right after a register write"""
    try:
        val = struct.unpack('<I', uc.mem_read(addr, 4))[0]
    except Exception:
        return
    nv = val
    if addr == 0x40021000:      # RCC_CR (ready bits sticky)
        nv = val | (1 << 1) | (1 << 17) | (1 << 25)
    elif addr == 0x40021004:    # RCC_CFGR -> SWS
        nv = (val & ~0xC) | ((val & 3) << 2)
    elif addr == 0x40021020:    # BDCR
        nv = val | (1 << 0) | (1 << 1)
    elif addr == 0x40021024:    # CSR (LSI ready sticky)
        nv = val | (1 << 0) | (1 << 1)
    elif addr == 0x40012408:    # ADC_CR
        if val & 1: nv |= 1          # ADEN -> ADRDY in ISR below
        if val & 4: nv &= ~4         # one-shot ADSTART cleared after conv
    elif addr == 0x40012400:    # ADC_ISR write (clear)
        nv = val | (1 << 1) | (1 << 2)
    elif addr == 0x40022010:    # FLASH_CR
        FLASH['pg'] = bool(val & 1)
        if val & (1 << 6):       # STRT -> erase/page program done
            ar = struct.unpack('<I', uc.mem_read(0x40022014, 4))[0]
            if val & 2:          # PER
                flash_erase_page(uc, ar)
            nv &= ~(1 << 6)
            sr = struct.unpack('<I', uc.mem_read(0x4002200C, 4))[0]
            uc.mem_write(0x4002200C, struct.pack('<I', (sr & ~1) | 0x20))
    elif addr == 0x40022004:    # FLASH_KEYR
        if val == 0x45670123:
            FLASH['unlock_stage'] = 1
        elif val == 0xCDEF89AB and FLASH['unlock_stage'] == 1:
            FLASH['unlock_stage'] = 2
            cr = struct.unpack('<I', uc.mem_read(0x40022010, 4))[0]
            uc.mem_write(0x40022010, struct.pack('<I', cr & 0x7FFFFFFF))
    elif addr == 0x4000280C:    # RTC_ISR
        nv = val | 0x77         # ALRAWF|ALRBWF|WUTWF|INITS|RSF|INITF
    elif addr == 0x40002824:    # RTC_WPR
        pass
    if nv != val:
        uc.mem_write(addr, struct.pack('<I', nv & 0xFFFFFFFF))
    # ADC ready flags live in ISR
    if addr == 0x40012408:
        isr = struct.unpack('<I', uc.mem_read(0x40012400, 4))[0]
        uc.mem_write(0x40012400, struct.pack('<I', isr | 0x1 | 0x4 | 0x8))

PENDING = set()
FLASH = {'unlock_stage': 0, 'pg': False, 'erasing': False}

def flash_erase_page(uc, page_addr):
    page = page_addr & 0xFFFFF800
    if 0x08000000 <= page < 0x08020000:
        uc.mem_write(page, b'\xff' * 0x800)

def flush_pending(uc):
    for a in list(PENDING):
        PENDING.discard(a)
        try:
            side_effect(uc, a, 4)
        except Exception:
            pass

def on_write(uc, access, addr, size, value, ud):
    if 0x08000000 <= addr < 0x08020000:
        if FLASH['pg']:
            sr = struct.unpack('<I', uc.mem_read(0x4002200C, 4))[0]
            uc.mem_write(0x4002200C, struct.pack('<I', sr | 0x20))
        return
    if in_per(addr):
        mmio.append(('w', addr, size, uc.reg_read(UC_ARM_REG_PC)))
        PENDING.add(addr)
def on_unmapped(uc, access, addr, size, value, ud):
    print('UNMAPPED access=%d addr=0x%08x size=%d pc=0x%08x' % (access, addr, size, uc.reg_read(UC_ARM_REG_PC)))
    raise SystemExit(0)
u.hook_add(UC_HOOK_MEM_READ_UNMAPPED, on_unmapped)
u.hook_add(UC_HOOK_MEM_WRITE_UNMAPPED, on_unmapped)
u.hook_add(UC_HOOK_MEM_FETCH_UNMAPPED, on_unmapped)
u.hook_add(UC_HOOK_MEM_READ, on_read)
u.hook_add(UC_HOOK_MEM_WRITE, on_write)
u.reg_write(UC_ARM_REG_SP, struct.unpack_from('<I', data, 0)[0])

def tick(n):
    v = struct.unpack('<I', u.mem_read(TICK, 4))[0]
    u.mem_write(TICK, struct.pack('<I', (v + n) & 0xFFFFFFFF))

def unstick(addr):
    old = struct.unpack('<I', u.mem_read(addr, 4))[0]
    if addr == 0x40021000:
        new, how = old | 0x02020003, 'RCC_CR |HSE/PLL/HSIRDY'
    elif addr == 0x40021004:
        new, how = (old & ~0xC) | ((old & 3) << 2), 'RCC_CFGR SWS=SW'
    elif addr == 0x40021024:
        new, how = old | 3, 'RCC_CSR LSIRDY'
    elif addr == 0x40021020:
        new, how = old | 3, 'RCC_BDCR LSERDY'
    elif addr == 0x40012400:
        new, how = old | 0xD, 'ADC_ISR ADRDY/EOC/EOS'
    elif addr == 0x40012408:
        new, how = old | 1, 'ADC_CR ADEN'
    elif addr == 0x40012708:
        new, how = old | 0x01000000, 'ADC_CCR VREFEN'
    elif addr == 0x4002200C:
        new, how = old | 0x24, 'FLASH_SR EOP'
    elif addr == 0x40022000:
        new, how = old | 1, 'FLASH_ACR'
    elif addr == 0xE000E010:
        new, how = old | 0x10000, 'SysTick COUNTFLAG'
    elif 0x40020000 <= addr < 0x40020060:
        new, how = old | 0xFFFFFFFF, 'DMA ISR all'
    elif 0x48000000 <= addr < 0x48002000:
        new, how = 0, 'GPIO IDR = 0'
    elif 0x20000000 <= addr < 0x20002000:
        new, how = 1, 'RAM flag = 1'
    else:
        new, how = old | 0xFFFFFFFF, 'all bits'
    u.mem_write(addr, struct.pack('<I', new & 0xFFFFFFFF))
    return old, new & 0xFFFFFFFF, how

SLICE = 2000
max_unsticks = int(sys.argv[1]) if len(sys.argv) > 1 else 40
stalls = []
pages = collections.deque(maxlen=20)
slice_reads = [0]
pc = struct.unpack_from('<I', data, 4)[0] & ~1
reached = False
for i in range(40000):
    try:
        u.emu_start(pc | 1, 0, count=SLICE)
    except UcError as e:
        print('UC ERROR %s pc=0x%08x' % (e, u.reg_read(UC_ARM_REG_PC)))
        break
    pc = u.reg_read(UC_ARM_REG_PC)
    tick(5)  # fake 1 kHz timebase: 5 "ms" per 2000 insns
    if MAIN_LO <= pc < MAIN_HI:
        print('*** reached MAIN LOOP at slice %d pc=0x%08x ***' % (i, pc))
        reached = True
        break
    if i % 100 == 99: print('  slice %d pc=0x%08x reads=%d' % (i, pc, nreads_pre))
    pages.append(pc >> 12)
    nreads = slice_reads[0]
    slice_reads[0] = 0
    nreads_pre = nreads
    if len(pages) == 20 and len(set(pages)) <= 2 and nreads > 300:
        recent = [e for e in mmio][-1500:]
        polls = collections.Counter(e[1] for e in recent if e[0] == 'r')
        info = {'i': i, 'pc': '0x%08x' % pc, 'pages': sorted(set('0x%x' % p for p in pages)),
                'polls': [('0x%08x' % a, n) for a, n in polls.most_common(5)]}
        print('STALL %2d @0x%08x pages=%s polls=%s' % (len(stalls), pc, info['pages'], info['polls'][:3]))
        if polls:
            target = polls.most_common(1)[0][0]
            old, new, how = unstick(target)
            info['unstick'] = {'addr': '0x%08x' % target, 'old': '0x%08x' % old,
                               'new': '0x%08x' % new, 'how': how}
            print('        unstick 0x%08x: 0x%08x->0x%08x %s' % (target, old, new, how))
            stalls.append(info)
        else:
            print('        no MMIO poll; RAM flag wait? give it one tick and continue')
            stalls.append(info)
        pages.clear()
        if len(stalls) > max_unsticks:
            break
print('\nreached main loop:', reached, 'stalls:', len(stalls))
for s in stalls:
    print('  %-10s %-22s %s' % (s['pc'], s['pages'], s.get('unstick', 'no-mmio')))
json.dump({'reached_main': reached, 'stalls': stalls},
          open(os.path.join(ROOT, 'analysis', 'boot_probe.json'), 'w'), indent=1)
