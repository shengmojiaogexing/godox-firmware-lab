#!/usr/bin/env python3
"""Recursive disassembler + light data-flow analysis for the AD200 Pro image.

Image base: 0x08003000 (file offset 0).
Outputs into analysis/:
  functions.json  disasm.txt  mmio.json  literals.json  summary.txt
"""
import sys, os, json, struct, collections
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS, CS_OP_IMM, CS_OP_REG, CS_OP_MEM
from capstone.arm import ARM_REG_PC, ARM_REG_SP, ARM_REG_LR, ARM_OP_MEM, ARM_OP_IMM

BASE = 0x08003000
HERE = os.path.dirname(os.path.abspath(__file__))
X3C = os.path.dirname(HERE)                                   # devices/x3c
REPO = os.path.abspath(os.path.join(HERE, '..', '..', '..'))  # repo root
BIN = os.path.join(REPO, 'devices', 'ad200pro', 'firmware', 'AD200pro_V1.60.bin')
OUT = os.path.join(X3C, 'analysis')
os.makedirs(OUT, exist_ok=True)

DATA = open(BIN, 'rb').read()
SIZE = len(DATA)
END = BASE + SIZE

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_MCLASS)
md.detail = True
md.skipdata = True

def rd32(a):
    o = a - BASE
    return struct.unpack_from('<I', DATA, o)[0] if 0 <= o <= SIZE - 4 else None
def rd16(a):
    o = a - BASE
    return struct.unpack_from('<H', DATA, o)[0] if 0 <= o <= SIZE - 2 else None
def rd8(a):
    o = a - BASE
    return DATA[o] if 0 <= o < SIZE else None
def in_img(a):
    return BASE <= a < END

# ------------------------------------------------------------------ register maps
REGS = {
    0x40021000: ('RCC', {0x00:'CR',0x04:'CFGR',0x08:'CIR',0x0C:'APB2RSTR',0x10:'APB1RSTR',
        0x14:'AHBENR',0x18:'APB2ENR',0x1C:'APB1ENR',0x20:'BDCR',0x24:'CSR',0x28:'AHBRSTR',
        0x2C:'CFGR2',0x30:'CFGR3',0x34:'CR2'}),
    0x48000000: ('GPIOA', 'gpio'),
    0x48000400: ('GPIOB', 'gpio'),
    0x48000800: ('GPIOC', 'gpio'),
    0x48000C00: ('GPIOD', 'gpio'),
    0x48001000: ('GPIOE', 'gpio'),
    0x48001400: ('GPIOF', 'gpio'),
    0x40010400: ('EXTI', {0x00:'IMR',0x04:'EMR',0x08:'RTSR',0x0C:'FTSR',0x10:'SWIER',0x14:'PR'}),
    0x40010000: ('SYSCFG', {0x00:'CFGR1',0x04:'CFGR2',0x08:'EXTICR1',0x0C:'EXTICR2',0x10:'EXTICR3',0x14:'EXTICR4',0x18:'CFGR3'}),
    0x40022000: ('FLASH', {0x00:'ACR',0x04:'KEYR',0x08:'OPTKEYR',0x0C:'SR',0x10:'CR',0x14:'AR',0x1C:'OBR',0x20:'WRPR'}),
    0x40007000: ('PWR', {0x00:'CR',0x04:'CSR'}),
    0x40003000: ('IWDG', {0x00:'KR',0x04:'PR',0x08:'RLR',0x0C:'SR',0x10:'WINR'}),
    0x40002800: ('RTC', {0x00:'TR',0x04:'DR',0x08:'CR',0x0C:'ISR',0x10:'PRER',0x14:'WUTR',0x18:'CALIBR',0x1C:'ALRMAR',0x20:'ALRMBR',0x24:'WPR',0x28:'SSR',0x2C:'SHIFTR',0x30:'TSTR',0x34:'TSDR',0x38:'TSSSR',0x3C:'CALR',0x40:'TAFCR',0x44:'ALRMASSR',0x48:'ALRMBSSR'}),
    0x40020000: ('DMA1', 'dma'),
    0x40012400: ('ADC', {0x00:'ISR',0x04:'IER',0x08:'CR',0x0C:'CFGR1',0x10:'CFGR2',0x14:'SMPR',0x28:'TR',0x30:'CHSELR',0x50:'DR',0x54:'CCR'}),
    0x40012700: ('ADC_common', {0x00:'CCR',0x04:'CDR'}),
    0x40012C00: ('TIM1', 'tim'),
    0x40000000: ('TIM2', 'tim'),
    0x40000400: ('TIM3', 'tim'),
    0x40001000: ('TIM6', 'tim'),
    0x40001400: ('TIM7', 'tim'),
    0x40002000: ('TIM14', 'tim'),
    0x40014000: ('TIM15', 'tim'),
    0x40014400: ('TIM16', 'tim'),
    0x40014800: ('TIM17', 'tim'),
    0x40013800: ('USART1', {0x00:'CR1',0x04:'CR2',0x08:'CR3',0x0C:'BRR',0x10:'GTPR',0x14:'RTOR',0x18:'RQR',0x1C:'ISR',0x20:'ICR',0x24:'RDR',0x28:'TDR'}),
    0x40004400: ('USART2', {0x00:'CR1',0x04:'CR2',0x08:'CR3',0x0C:'BRR',0x10:'GTPR',0x14:'RTOR',0x18:'RQR',0x1C:'ISR',0x20:'ICR',0x24:'RDR',0x28:'TDR'}),
    0x40013000: ('SPI1', {0x00:'CR1',0x04:'CR2',0x08:'SR',0x0C:'DR',0x10:'CRCPR',0x14:'RXCRCR',0x18:'TXCRCR',0x1C:'I2SCFGR',0x20:'I2SPR'}),
    0x40003800: ('SPI2', {0x00:'CR1',0x04:'CR2',0x08:'SR',0x0C:'DR',0x10:'CRCPR',0x14:'RXCRCR',0x18:'TXCRCR',0x1C:'I2SCFGR',0x20:'I2SPR'}),
    0x40005400: ('I2C1', {0x00:'CR1',0x04:'CR2',0x08:'OAR1',0x0C:'OAR2',0x10:'TIMINGR',0x14:'TIMEOUTR',0x18:'ISR',0x1C:'ICR',0x20:'PECR',0x24:'RXDR',0x28:'TXDR'}),
    0xE000E000: ('SCS', {0x00:'ICTR',0x04:'ICSR',0x08:'VTOR',0x0C:'AIRCR',0x10:'SCR',0x14:'CCR',0x18:'SHPR2',0x1C:'SHPR3',0x100:'ISER',0x180:'ICER',0x200:'ISPR',0x280:'ICPR',0x400:'IPR'}),
    0xE000E010: ('SysTick', {0x00:'CSR',0x04:'RVR',0x08:'CVR',0x0C:'CALIB'}),
    0xE000ED00: ('SCB', {0x00:'CPUID',0x04:'ICSR',0x08:'VTOR',0x0C:'AIRCR',0x10:'SCR',0x14:'CCR',0x18:'SHPR1',0x1C:'SHPR2',0x20:'SHPR3',0x24:'SHCSR',0x28:'CFSR',0x2C:'HFSR',0x34:'MMFAR',0x38:'BFAR',0x3C:'AFSR'}),
}
GPIOREG = {0x00:'MODER',0x04:'OTYPER',0x08:'OSPEEDR',0x0C:'PUPDR',0x10:'IDR',0x14:'ODR',0x18:'BSRR',0x1C:'LCKR',0x20:'AFR0',0x24:'AFR1',0x28:'BRR'}
DMAOFF = {0x00:'ISR',0x04:'IFCR'}
TIMREG = {0x00:'CR1',0x04:'CR2',0x08:'SMCR',0x0C:'DIER',0x10:'SR',0x14:'EGR',0x18:'CCMR1',0x1C:'CCMR2',0x20:'CCER',0x24:'CNT',0x28:'PSC',0x2C:'ARR',0x34:'CCR1',0x38:'CCR2',0x3C:'CCR3',0x40:'CCR4',0x44:'BDTR',0x48:'DCR',0x4C:'DMAR'}

def reg_name(addr):
    for base, ent in REGS.items():
        if base <= addr < base + 0x400:
            name = ent[0]; off = addr - base
            if ent[1] == 'gpio':
                return '%s_%s' % (name, GPIOREG.get(off, '+0x%02x' % off))
            if ent[1] == 'dma':
                if off < 8: return 'DMA_%s' % DMAOFF.get(off, hex(off))
                ch = 1 + (off - 8) // 0x14; r = (off - 8) % 0x14
                rn = {0:'CCR',4:'CNDTR',8:'CPAR',0xC:'CMAR',0x10:'?DR'}.get(r, '+0x%x' % r)
                return 'DMA1_CH%d_%s' % (ch, rn)
            if ent[1] == 'tim':
                return '%s_%s' % (name, TIMREG.get(off, '+0x%02x' % off))
            return '%s_%s' % (name, ent[1].get(off, '+0x%02x' % off))
    if 0x40000000 <= addr < 0x40100000:
        return 'APB+0x%x' % (addr - 0x40000000)
    if 0xE0000000 <= addr < 0xE0100000:
        return 'SCS+0x%x' % (addr - 0xE000E000)
    return None

def lit_target(ins):
    if ins.id == 0 or ins.mnemonic.startswith('.'):
        return None
    if len(ins.operands) == 2 and ins.operands[0].type == CS_OP_REG and ins.operands[1].type == CS_OP_MEM:
        m = ins.operands[1].mem
        if m.base == ARM_REG_PC:
            return ((ins.address + 4) & ~3) + m.disp
    return None

def ascii_at(a, maxlen=80):
    o = a - BASE
    if not (0 <= o < SIZE): return None
    out = bytearray()
    for i in range(o, min(SIZE, o + maxlen)):
        c = DATA[i]
        if 32 <= c < 127: out.append(c)
        else: break
    return out.decode() if len(out) >= 4 else None

def classify(v):
    if v is None: return 'none'
    if v == 0: return 'zero'
    if BASE <= v < END: return 'flash'
    if 0x08000000 <= v < 0x08010000: return 'flash-hi'
    if 0x20000000 <= v < 0x20010000: return 'ram'
    if v >= 0x40000000 and reg_name(v): return 'mmio'
    if v < 0x10000: return 'small'
    if 0xE0000000 <= v < 0xE0100000: return 'scs'
    return 'other'

# ------------------------------------------------------------------ analysis
funcs = {}
work = []
code_addrs = set()
literal_addrs = collections.defaultdict(set)
mmio = collections.defaultdict(list)
literals = {}

def add_func(a):
    if in_img(a) and a not in funcs:
        funcs[a] = {'start': a, 'insns': [], 'end': None, 'calls': set(), 'lits': [],
                    'mmio': [], 'strings': [], 'terminated': False, 'loops': 0}
        work.append(a)

def analyze_func(start):
    f = funcs[start]
    visited = set()
    pending = []          # conditional branch targets inside this function
    pc = start
    regs = {}
    while True:
        if pc in visited:
            if not pending: break
            pc = pending.pop(0); regs = {}; continue
        if len(f['insns']) > 3000: break
        visited.add(pc)
        off = pc - BASE
        if not (0 <= off < SIZE - 1):
            if not pending: break
            pc = pending.pop(0); regs = {}; continue
        il = list(md.disasm(DATA[off:off + 4], pc))
        if not il:
            if not pending: break
            pc = pending.pop(0); regs = {}; continue
        ins = il[0]
        if ins.id == 0 or ins.mnemonic.startswith('.'):
            if not pending: break
            pc = pending.pop(0); regs = {}; continue
        f['insns'].append(ins); code_addrs.add(pc)
        mn = ins.mnemonic; ops = ins.operands
        # literal load
        lt = lit_target(ins)
        if lt is not None:
            v = rd32(lt)
            literal_addrs[lt].add(pc)
            if v is not None:
                rec = literals.setdefault(lt, {'value': v, 'class': classify(v), 'insn': pc, 'refs': [], 'str': None})
                rec['refs'].append(pc)
                f['lits'].append({'addr': lt, 'value': v, 'class': rec['class'], 'insn': pc})
                dst = ops[0].reg
                regs[dst] = ('const', v)
                if in_img(v):
                    s = ascii_at(v)
                    if s:
                        f['strings'].append(s); rec['str'] = s
        # constant building
        def setc(r, v): regs[r] = ('const', v)
        if mn in ('movs', 'mov') and len(ops) == 2 and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_IMM:
            setc(ops[0].reg, ops[1].imm)
        elif mn in ('adds', 'add', 'subs', 'sub') and len(ops) == 3 and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_REG and ops[2].type == CS_OP_IMM:
            b = regs.get(ops[1].reg)
            if b and b[0] == 'const':
                setc(ops[0].reg, b[1] + (ops[2].imm if mn.startswith('add') else -ops[2].imm))
        elif mn == 'mov' and len(ops) == 2 and ops[0].type == CS_OP_REG and ops[1].type == CS_OP_REG and ops[1].reg in regs:
            regs[ops[0].reg] = regs[ops[1].reg]
        # memory accesses with constant base
        for op in ops:
            if op.type == CS_OP_MEM:
                b = regs.get(op.mem.base)
                if b and b[0] == 'const':
                    addr = b[1] + op.mem.disp
                    rn = reg_name(addr)
                    if rn and addr >= 0x40000000:
                        m2 = mn[2:6]
                        sz = 1 if ('b' in m2 and not mn.startswith('ldrs')) else 2 if 'h' in m2 else 4
                        rw = 'r' if mn.startswith('ldr') else 'w' if mn.startswith('str') else '?'
                        rec = {'addr': addr, 'name': rn, 'rw': rw, 'size': sz, 'insn': pc}
                        f['mmio'].append(rec)
                        mmio[addr].append({'func': start, 'rw': rw, 'size': sz, 'insn': pc})
        # control flow
        if mn in ('bl', 'blx'):
            t = None
            if ops and ops[0].type == CS_OP_IMM: t = ops[0].imm & ~1
            elif ops and ops[0].type == CS_OP_REG:
                b = regs.get(ops[0].reg)
                if b and b[0] == 'const': t = b[1] & ~1
            if t is not None and in_img(t):
                f['calls'].add(t); add_func(t)
        # conditional branch target
        if mn.startswith('b') and mn not in ('bl', 'blx', 'bx', 'b') and ops and ops[0].type == CS_OP_IMM:
            t = ops[0].imm & ~1
            if in_img(t) and t not in visited:
                pending.append(t)
        if mn in ('cbz', 'cbnz') and ops and ops[-1].type == CS_OP_IMM:
            t = ops[-1].imm & ~1
            if in_img(t) and t not in visited:
                pending.append(t)
        if mn == 'b' and ops and ops[0].type == CS_OP_IMM:
            t = ops[0].imm & ~1
            if in_img(t) and t not in visited:
                pc = t; regs = {}; continue
            if not pending: break
            pc = pending.pop(0); regs = {}; continue
        if mn == 'bx' and ops and ops[0].type == CS_OP_REG:
            b = regs.get(ops[0].reg)
            if b and b[0] == 'const' and in_img(b[1] & ~1):
                f['tail'] = b[1] & ~1; add_func(b[1] & ~1)
            if not pending: break
            pc = pending.pop(0); regs = {}; continue
        if mn == 'pop' and 'pc' in ins.op_str:
            if not pending: break
            pc = pending.pop(0); regs = {}; continue
        if mn in ('udf', 'bkpt'):
            if not pending: break
            pc = pending.pop(0); regs = {}; continue
        pc += ins.size
    if f['insns']:
        f['end'] = max(i.address + i.size for i in f['insns'])
    else:
        f['end'] = start

def is_prologue(a):
    il = list(md.disasm(DATA[a - BASE: a - BASE + 4], a)) if in_img(a) else []
    if not il: return False
    ins = il[0]
    if ins.id == 0: return False
    return ins.mnemonic == 'push' and 'lr' in ins.op_str

def discover_bl():
    """scan every 2-byte offset for BL/BLX immediate targets and add function-like ones"""
    added = 0
    for i in range(0, SIZE - 4, 2):
        il = list(md.disasm(DATA[i:i + 4], BASE + i))
        if not il: continue
        ins = il[0]
        if ins.id == 0: continue
        if ins.mnemonic in ('bl', 'blx') and ins.operands and ins.operands[0].type == CS_OP_IMM:
            t = ins.operands[0].imm & ~1
            if in_img(t) and is_prologue(t) and t not in funcs:
                add_func(t); added += 1
    return added

def discover_pointers():
    """scan 4-byte aligned words that look like function pointers"""
    added = 0
    for off in range(0, SIZE - 4, 2):
        if off % 2: continue
        w = struct.unpack_from('<I', DATA, off)[0]
        t = w & ~1
        if not in_img(t) or w & 1 == 0: continue
        if t in funcs: continue
        if is_prologue(t):
            add_func(t); added += 1
    return added

def run():
    vecs = struct.unpack_from('<48I', DATA, 0)
    for v in vecs:
        if v and in_img(v & ~1): add_func(v & ~1)
    for a in (BASE + 0xC0, BASE + 0xD4, BASE + 0x100, BASE + 0x3548, BASE + 0xCDB0):
        add_func(a)
    # pass 1 recursive
    while work:
        s = work.pop()
        if s in funcs and not funcs[s]['insns']: analyze_func(s)
    # pass 2+ discover more
    for it in range(4):
        n = discover_bl() + discover_pointers()
        while work:
            s = work.pop()
            if s in funcs and not funcs[s]['insns']: analyze_func(s)
        if n == 0: break
        print('discovery pass %d added %d' % (it + 1, n))
    return vecs

def main():
    vecs = run()
    good = {a: f for a, f in funcs.items() if len(f['insns']) >= 2}
    print('functions:', len(good), ' code bytes:', len(code_addrs), '/', SIZE,
          '(%.1f%%)' % (100.0 * len(code_addrs) / SIZE))
    # coverage gaps
    covered = sorted(code_addrs)
    gaps = []
    prev = BASE
    for a in covered:
        if a - prev >= 8: gaps.append((prev, a))
        prev = max(prev, a + 2)
    if END - prev >= 8: gaps.append((prev, END))
    print('gaps >=8 bytes:', len(gaps))
    # write listing
    lines = []
    for a in sorted(good):
        f = good[a]
        lines.append('\n; ================================================================')
        lines.append('; FUNC_%08x  file+0x%05x  %d insns  calls=%s' % (
            a, a - BASE, len(f['insns']), ','.join('0x%x' % c for c in sorted(f['calls']))[:240]))
        if f['strings']:
            lines.append('; strings: %s' % ' | '.join(repr(s) for s in f['strings'][:12]))
        seen = []
        for m in f['mmio']:
            t = '%s:%s' % (m['rw'], m['name'])
            if t not in seen: seen.append(t)
        if seen: lines.append('; mmio: %s' % ' '.join(seen[:24]))
        for ins in f['insns']:
            lines.append('%08x: %-8s %-26s' % (ins.address, ins.bytes.hex(), ins.mnemonic + ' ' + ins.op_str))
            lt = lit_target(ins)
            if lt is not None:
                v = rd32(lt); s = ascii_at(v) if v and in_img(v) else None
                lines.append('%08x:   ; 0x%08x = 0x%08x (%s)%s' % (ins.address, lt, v, classify(v), (' "%s"' % s) if s else ''))
    open(os.path.join(OUT, 'disasm.txt'), 'w', encoding='utf-8').write('\n'.join(lines))
    export = []
    callers = collections.defaultdict(list)
    for a, f in good.items():
        for c in f['calls']: callers[c].append(a)
    for a in sorted(good):
        f = good[a]
        export.append({'start': a, 'end': f['end'], 'insn_count': len(f['insns']),
                       'calls': sorted(f['calls']), 'callers': sorted(callers.get(a, [])),
                       'strings': f['strings'], 'mmio': f['mmio'],
                       'literals': f['lits'], 'loops': f['loops'],
                       'terminated': f['terminated']})
    json.dump(export, open(os.path.join(OUT, 'functions.json'), 'w', encoding='utf-8'), indent=1)
    mm = [{'addr': a, 'name': reg_name(a), 'accesses': mmio[a]} for a in sorted(mmio)]
    json.dump(mm, open(os.path.join(OUT, 'mmio.json'), 'w', encoding='utf-8'), indent=1)
    lits = []
    for la in sorted(literals):
        r = literals[la]
        lits.append({'addr': la, 'value': r['value'], 'class': r['class'], 'insn': r['insn'],
                     'str': r['str'], 'refs': r['refs']})
    json.dump(lits, open(os.path.join(OUT, 'literals.json'), 'w', encoding='utf-8'), indent=1)
    # summary
    out = []
    out.append('functions: %d' % len(good))
    out.append('code bytes: %d / %d (%.1f%%)' % (len(code_addrs), SIZE, 100.0 * len(code_addrs) / SIZE))
    byclass = collections.Counter(l['class'] for l in lits)
    out.append('literal classes: %s' % dict(byclass))
    per = collections.Counter(reg_name(m['addr']).split('_')[0] for m in mm)
    out.append('peripherals: %s' % dict(per))
    out.append('\nmmio registers:')
    for m in mm:
        kinds = sorted(set('%s%d' % (x['rw'], x['size']) for x in m['accesses']))
        funcs_ = sorted(set(x['func'] for x in m['accesses']))
        out.append('  %-18s 0x%08x  %-6s funcs=%s' % (m['name'], m['addr'], ','.join(kinds),
                   ','.join('0x%x' % f for f in funcs_[:8]) + ('...' if len(funcs_) > 8 else '')))
    out.append('\nall functions:')
    for e in sorted(export, key=lambda x: x['start']):
        out.append('  0x%08x  %4d ins  callers=%-30s %s' % (e['start'], e['insn_count'],
                   ','.join('0x%x' % c for c in e['callers'][:6]), ' | '.join(e['strings'][:3])))
    txt = '\n'.join(out)
    open(os.path.join(OUT, 'summary.txt'), 'w', encoding='utf-8').write(txt)
    print(txt[:6000])

if __name__ == '__main__':
    main()
