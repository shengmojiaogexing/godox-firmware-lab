#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
AD100pro_V1.2.bin static analysis (Cortex-M0 / STM32F0).

Produces:
  out/functions.json   - recovered function table + call graph
  out/disasm.txt       - annotated disassembly listing
  out/periph_map.json  - peripheral register access map
  out/gaps.txt         - unclaimed ranges (candidate data)
  out/lits.json        - literal pool words with resolution
"""
import sys, os, json, struct, collections

import capstone
from capstone import arm_const

BASE = 0x08003000   # image load address (0x3000-byte bootloader precedes it)
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FW = os.path.join(ROOT, 'firmware', 'AD100pro_V1.2.bin')
OUT = os.path.join(ROOT, 'out')

ROM_SIZE = 0x10000          # 64 KiB flash
RAM_BASE, RAM_SIZE = 0x20000000, 0x2000
ROM_BASE = 0x08003000   # image load address (0x3000-byte bootloader precedes it)

# ---------------------------------------------------------------- peripherals
PERIPH = {
    0x40000000: 'TIM2', 0x40000400: 'TIM3', 0x40001000: 'TIM6', 0x40001400: 'TIM7',
    0x40002000: 'TIM14', 0x40002800: 'RTC', 0x40002C00: 'WWDG', 0x40003000: 'IWDG',
    0x40003800: 'SPI2', 0x40004400: 'USART2', 0x40005400: 'I2C1', 0x40005800: 'I2C2',
    0x40007000: 'PWR', 0x40010000: 'SYSCFG', 0x40010400: 'EXTI',
    0x40012400: 'ADC', 0x40012C00: 'TIM1', 0x40013000: 'SPI1', 0x40013800: 'USART1',
    0x40014000: 'TIM15', 0x40014400: 'TIM16', 0x40014800: 'TIM17',
    0x40020000: 'DMA1', 0x40021000: 'RCC', 0x40022000: 'FLASH_IF', 0x40023000: 'CRC',
    0x48000000: 'GPIOA', 0x48000400: 'GPIOB', 0x48000800: 'GPIOC', 0x48000C00: 'GPIOD',
    0x48001000: 'GPIOE', 0x48001400: 'GPIOF',
    0xE000E000: 'SYS', 0xE000ED00: 'SCB', 0xE000E010: 'SYSTICK', 0xE000E100: 'NVIC',
    0xE000E200: 'NVIC_ISP', 0xE000E280: 'NVIC_ICP',
}

REGS = {
    'GPIO': {0x00: 'MODER', 0x04: 'OTYPER', 0x08: 'OSPEEDR', 0x0C: 'PUPDR',
             0x10: 'IDR', 0x14: 'ODR', 0x18: 'BSRR', 0x1C: 'LCKR',
             0x20: 'AFRL', 0x24: 'AFRH', 0x28: 'BRR'},
    'RCC': {0x00: 'CR', 0x04: 'CFGR', 0x08: 'CIR', 0x0C: 'APB2RSTR', 0x10: 'APB1RSTR',
            0x14: 'AHBENR', 0x18: 'APB2ENR', 0x1C: 'APB1ENR', 0x20: 'BDCR', 0x24: 'CSR',
            0x28: 'AHBRSTR', 0x2C: 'CFGR2', 0x30: 'CFGR3', 0x34: 'CR2'},
    'FLASH_IF': {0x00: 'ACR', 0x04: 'KEYR', 0x08: 'OPTKEYR', 0x0C: 'SR', 0x10: 'CR',
                 0x14: 'AR', 0x18: 'RESERVED', 0x1C: 'OBR', 0x20: 'WRPR'},
    'EXTI': {0x00: 'IMR', 0x04: 'EMR', 0x08: 'RTSR', 0x0C: 'FTSR', 0x10: 'SWIER', 0x14: 'PR'},
    'SYSCFG': {0x00: 'CFGR1', 0x04: 'CFGR2', 0x08: 'EXTICR1', 0x0C: 'EXTICR2',
               0x10: 'EXTICR3', 0x14: 'EXTICR4', 0x18: 'CFGR3'},
    'TIM': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'SMCR', 0x0C: 'DIER', 0x10: 'SR',
            0x14: 'EGR', 0x18: 'CCMR1', 0x1C: 'CCMR2', 0x20: 'CCER', 0x24: 'CNT',
            0x28: 'PSC', 0x2C: 'ARR', 0x30: 'RCR', 0x34: 'CCR1', 0x38: 'CCR2',
            0x3C: 'CCR3', 0x40: 'CCR4', 0x44: 'BDTR', 0x48: 'DCR', 0x4C: 'DMAR'},
    'ADC': {0x00: 'ISR', 0x04: 'IER', 0x08: 'CR', 0x0C: 'CFGR1', 0x10: 'CFGR2',
            0x14: 'SMPR', 0x40: 'DR', 0x44: 'CCR'},
    'USART': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'CR3', 0x0C: 'BRR', 0x10: 'GTPR',
              0x14: 'RTOR', 0x18: 'RQR', 0x1C: 'ISR', 0x20: 'ICR', 0x24: 'RDR', 0x28: 'TDR'},
    'SPI': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'SR', 0x0C: 'DR', 0x10: 'CRCPR',
            0x14: 'RXCRCR', 0x18: 'TXCRCR', 0x1C: 'I2SCFGR', 0x20: 'I2SPR'},
    'I2C': {0x00: 'CR1', 0x04: 'CR2', 0x08: 'OAR1', 0x0C: 'OAR2', 0x10: 'TIMINGR',
            0x14: 'TIMEOUTR', 0x18: 'ISR', 0x1C: 'ICR', 0x20: 'PECR', 0x24: 'RXDR', 0x28: 'TXDR'},
    'DMA1': {0x00: 'ISR', 0x04: 'IFCR'},
    'RTC': {0x00: 'TR', 0x04: 'DR', 0x08: 'CR', 0x0C: 'ISR', 0x10: 'PRER',
            0x14: 'WUTR', 0x18: 'CALR', 0x1C: 'ALRMAR', 0x20: 'ALRMBR', 0x24: 'WPR',
            0x28: 'SSR', 0x2C: 'SHIFTR', 0x30: 'TSTR', 0x34: 'TSDR', 0x38: 'TSSSR',
            0x3C: 'CALR', 0x40: 'TAFCR', 0x44: 'ALRMASSR', 0x48: 'ALRMBSSR'},
    'PWR': {0x00: 'CR', 0x04: 'CSR'},
    'IWDG': {0x00: 'KR', 0x04: 'PR', 0x08: 'RLR', 0x0C: 'SR', 0x10: 'WINR'},
    'CRC': {0x00: 'DR', 0x04: 'IDR', 0x08: 'CR', 0x0C: 'INIT', 0x10: 'POL'},
    'SCB': {0x00: 'CPUID', 0x04: 'ICSR', 0x08: 'VTOR', 0x0C: 'AIRCR', 0x10: 'SCR',
            0x14: 'CCR', 0x18: 'SHPR1', 0x1C: 'SHPR2', 0x20: 'SHPR3', 0x24: 'SHCSR',
            0x28: 'CFSR', 0x2C: 'HFSR'},
    'SYSTICK': {0x00: 'CTRL', 0x04: 'LOAD', 0x08: 'VAL', 0x0C: 'CALIB'},
    'NVIC': {0x00: 'ISER', 0x80: 'ICER', 0x100: 'ISPR', 0x180: 'ICPR'},
}
GROUP = {}
for b, n in PERIPH.items():
    if n.startswith('GPIO'):
        GROUP[b] = 'GPIO'
    elif n.startswith('TIM'):
        GROUP[b] = 'TIM'
    elif n.startswith('USART'):
        GROUP[b] = 'USART'
    elif n.startswith('SPI'):
        GROUP[b] = 'SPI'
    elif n.startswith('I2C'):
        GROUP[b] = 'I2C'
    elif n.startswith('DMA'):
        GROUP[b] = 'DMA1'
    else:
        GROUP[b] = n


def periph_name(addr):
    """Resolve an absolute address to 'PERIPH.REG' if it is a known register."""
    best = None
    for b in PERIPH:
        if b <= addr < b + 0x400:
            if best is None or b > best:
                best = b
    if best is None:
        return None
    pname = PERIPH[best]
    off = addr - best
    regs = REGS.get(GROUP[best], {})
    if GROUP[best] == 'DMA1':
        ch = off // 0x1C
        r = off % 0x1C
        names = {0x00: 'ISR', 0x04: 'IFCR', 0x08: 'CCR', 0x0C: 'CNDTR',
                 0x10: 'CPAR', 0x14: 'CMAR', 0x18: 'reserved'}
        if off < 0x04:
            return f'{pname}.{names.get(off, hex(off))}'
        return f'{pname}.CH{ch+1}_{names.get(r, hex(r))}'
    return f'{pname}.{regs.get(off, hex(off))}'


def is_periph(addr):
    return periph_name(addr) is not None


def is_flash(v):
    return ROM_BASE <= v < ROM_BASE + ROM_SIZE


def is_ram(v):
    return RAM_BASE <= v < RAM_BASE + RAM_SIZE


# ---------------------------------------------------------------- disassembler
md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB + capstone.CS_MODE_MCLASS)
md.detail = True
md.skipdata = False

COND_B = {'beq', 'bne', 'bcs', 'bhs', 'bcc', 'blo', 'bmi', 'bpl', 'bvs', 'bvc',
          'bhi', 'bls', 'bge', 'blt', 'bgt', 'ble'}


def decode_one(addr, code):
    off = addr - BASE
    if off < 0 or off + 2 > len(code):
        return None
    for insn in md.disasm(code[off:off + 4], addr):
        return insn
    return None


class Analysis:
    def __init__(self, data):
        self.data = data
        self.size = len(data)
        self.insns = {}          # addr -> (size, mnem, opstr, detail)
        self.owner = {}          # addr -> func entry
        self.funcs = {}          # entry -> dict
        self.lits = {}           # addr -> set of (insn_addr, reg)
        self.worklist = []
        self.calls = collections.defaultdict(set)
        self.callers = collections.defaultdict(set)
        self.unk_flows = []

    def next_insn(self, addr):
        insn = decode_one(addr, self.data)
        if insn is None:
            return None
        return insn

    def add_entry(self, addr, why):
        if not is_flash(addr):
            return
        off = addr - BASE
        if off < 0xC0 or off >= self.size:
            return
        if addr not in self.funcs:
            self.funcs[addr] = {'addr': addr, 'entry': addr, 'why': why,
                                'blocks': [], 'insns': 0, 'callees': set(),
                                'lits': set(), 'end': addr}
            self.worklist.append(addr)

    def explore(self):
        while self.worklist:
            entry = self.worklist.pop()
            self.run_func(entry)

    def run_func(self, entry):
        f = self.funcs[entry]
        stack = [entry]
        seen_blocks = set()
        while stack:
            a = stack.pop()
            if a in seen_blocks:
                continue
            seen_blocks.add(a)
            f['blocks'].append(a)
            nb = 0
            while True:
                if a in self.insns:
                    break
                insn = self.next_insn(a)
                if insn is None:
                    break
                nb += 1
                mn, ops = insn.mnemonic, insn.op_str
                self.insns[a] = (insn.size, mn, ops, insn)
                self.owner[a] = entry
                f['insns'] += 1
                f['end'] = max(f['end'], a + insn.size)
                # pc-relative literal
                for op in insn.operands:
                    if op.type == capstone.CS_OP_MEM and op.mem.base == arm_const.ARM_REG_PC:
                        lit = ((a + 4) & ~3) + op.mem.disp
                        self.lits.setdefault(lit, set()).add(a)
                        f['lits'].add(lit)
                # branches
                if mn in ('b', 'bl', 'blx'):
                    tgt = None
                    if insn.operands and insn.operands[0].type == capstone.CS_OP_IMM:
                        tgt = insn.operands[0].imm
                    if mn == 'b':
                        if tgt is not None:
                            stack.append(tgt)
                        break
                    elif mn == 'bl':
                        if tgt is not None:
                            self.add_entry(tgt, f'bl from {a:#x}')
                            self.calls[entry].add(tgt)
                            self.callers[tgt].add(entry)
                        # bl returns, fall through
                    else:  # blx
                        if tgt is not None:
                            self.add_entry(tgt & ~1, f'blx from {a:#x}')
                            self.calls[entry].add(tgt & ~1)
                            self.callers[tgt & ~1].add(entry)
                        break
                elif mn in COND_B:
                    if insn.operands and insn.operands[0].type == capstone.CS_OP_IMM:
                        stack.append(insn.operands[0].imm)
                    a += insn.size
                    nb = 0
                    stack.append(a)
                    break
                elif mn in ('cbz', 'cbnz'):
                    if insn.operands and insn.operands[1].type == capstone.CS_OP_IMM:
                        stack.append(insn.operands[1].imm)
                    a += insn.size
                    stack.append(a)
                    break
                elif mn == 'bx' or mn == 'blx':
                    break
                elif mn == 'pop' and 'pc' in ops:
                    break
                elif mn in ('udf', 'bkpt', 'svc'):
                    if mn == 'svc':
                        a += insn.size
                        continue
                    break
                elif mn in ('nop', 'yield', 'wfe', 'wfi', 'sev'):
                    a += insn.size
                    continue
                elif mn == 'it':
                    a += insn.size
                    continue
                else:
                    # throw-away: detect 'add sp' then 'pop pc' handled by pop rule
                    a += insn.size
                    continue
                a += insn.size
                if nb > 4000:
                    break
        f['blocks'] = sorted(set(f['blocks']))

    # -------------------------------------------------------------- discovery
    def seed_vectors(self):
        for i in range(48):
            v = struct.unpack_from('<I', self.data, i * 4)[0]
            if v & 1 and is_flash(v & ~1):
                self.add_entry(v & ~1, f'vector {i}')

    def scan_gaps(self):
        """Try to find functions that are only reachable indirectly."""
        found = []
        claimed = set(self.insns) | set(self.lits)
        # also mark all literal payload words
        for lit in self.lits:
            for k in range(4):
                claimed.add(lit + k)
        gap = []
        a = BASE + 0xC0
        while a < BASE + self.size:
            if a not in claimed:
                gap.append(a)
            else:
                if gap:
                    found.append((gap[0], gap[-1]))
                    gap = []
            a += 2
        if gap:
            found.append((gap[0], gap[-1]))
        merged = []
        for s, e in found:
            if merged and s <= merged[-1][1] + 2:
                merged[-1] = (merged[-1][0], max(e, merged[-1][1]))
            else:
                merged.append((s, e))
        return merged

    # --------------------------------------------------------- gap recovery
    def validate_entry(self, addr, max_insns=200):
        """Heuristic check that addr starts a plausible Thumb function."""
        if addr in self.insns or addr in self.funcs:
            return False
        off = addr - BASE
        if off < 0xC0 or off >= self.size or (addr & 1):
            return False
        a, n = addr, 0
        while n < max_insns:
            insn = self.next_insn(a)
            if insn is None:
                return False
            mn, ops = insn.mnemonic, insn.op_str
            if mn in ('udf', 'it', 'bkpt'):
                return False
            if mn in COND_B or mn in ('cbz', 'cbnz'):
                return n >= 2
            if mn in ('b', 'bx') or (mn == 'pop' and 'pc' in ops) or \
               (mn == 'add' and 'pc' in ops):
                return n >= 1
            a += insn.size
            n += 1
        return False

    def scan_pointers(self):
        """Any odd flash pointer in the image is strong evidence of a function entry."""
        added = 0
        for i in range(0, self.size - 3, 4):
            v = struct.unpack_from('<I', self.data, i)[0]
            if not (v & 1) or not is_flash(v & ~1):
                continue
            t = v & ~1
            if t in self.funcs or t in self.insns:
                continue
            if self.validate_entry(t):
                self.add_entry(t, f'ptr@{BASE+i:#x}')
                added += 1
        return added

    def sweep_gap_starts(self):
        """Functions are laid out contiguously: decode from each gap start after padding."""
        added = 0
        for s, e in self.scan_gaps():
            a = s
            while a <= e:
                off = a - BASE
                if off + 2 <= self.size and struct.unpack_from('<H', self.data, off)[0] == 0:
                    a += 2
                    continue
                break
            if a <= e:
                if self.validate_entry(a):
                    self.add_entry(a, 'gap start')
                    added += 1
                else:
                    # try later 2-byte offsets too (may follow a pool fragment)
                    b = a + 2
                    while b <= e:
                        if self.validate_entry(b):
                            self.add_entry(b, 'gap probe')
                            added += 1
                            break
                        b += 2
        return added

    # -------------------------------------------------------------- literals
    def resolve(self, v):
        if is_flash(v):
            off = v - BASE
            if v < BASE:
                return f'boot_{v:08X}'
            if v & 1:
                return f'func_{v & ~1:08X}'
            if off in self.insns:
                return f'code_{v:08X}'
            return f'flash_{v:08X}'
        if is_ram(v):
            return f'RAM_{v:08X}'
        p = periph_name(v)
        if p:
            return p
        return f'0x{v:08X}'


def main():
    with open(FW, 'rb') as f:
        data = f.read()
    os.makedirs(OUT, exist_ok=True)
    an = Analysis(data)
    an.seed_vectors()
    an.explore()
    # second pass: literal pool values that look like function pointers
    more = True
    rounds = 0
    while more and rounds < 6:
        more = False
        rounds += 1
        for lit in list(an.lits):
            off = lit - BASE
            if off < 0 or off + 4 > len(data):
                continue
            v = struct.unpack_from('<I', data, off)[0]
            if v & 1 and is_flash(v & ~1):
                before = len(an.funcs)
                an.add_entry(v & ~1, f'lit@{lit:#x}')
                if len(an.funcs) > before:
                    more = True
        an.explore()

    # ---- global recovery: pointer tables + contiguous layout
    for _round in range(5):
        n1 = an.scan_pointers()
        n2 = an.sweep_gap_starts()
        print(f'recovery round {_round}: ptr+{n1} gap+{n2}')
        if n1 + n2 == 0:
            break
        an.explore()

    gaps = an.scan_gaps()

    # ---- output: functions
    funcs_out = []
    for entry, f in sorted(an.funcs.items()):
        funcs_out.append({
            'addr': f'0x{entry:08X}', 'end': f'0x{f["end"]:08X}',
            'size': f['end'] - entry, 'insns': f['insns'],
            'blocks': [f'0x{b:08X}' for b in f['blocks']],
            'callees': [f'0x{c:08X}' for c in sorted(f['callees'])],
            'lits': [f'0x{l:08X}' for l in sorted(f['lits'])],
        })
    with open(os.path.join(OUT, 'functions.json'), 'w', encoding='utf-8') as f:
        json.dump(funcs_out, f, indent=1)

    # ---- output: disassembly listing
    lines = []
    lit_words = set(an.lits)
    func_entries = set(an.funcs)
    covered = set(an.insns)
    for lw in lit_words:
        for k in range(4):
            covered.add(lw + k)
    addr = BASE
    data_start = None
    while addr < BASE + an.size:
        off = addr - BASE
        if addr in func_entries:
            f = an.funcs[addr]
            callers = sorted(an.callers.get(addr, []))
            cs = ', '.join(f'func_{c:08X}' for c in callers[:6])
            more = '' if len(callers) <= 6 else f' (+{len(callers)-6})'
            lines.append('')
            lines.append(f'; ==================== func_{addr:08X}  '
                         f'insns={f["insns"]} blocks={len(f["blocks"])} '
                         f'end=0x{f["end"]:08X}')
            if cs:
                lines.append(f';   called by: {cs}{more}')
        if addr in an.insns:
            sz, mn, ops, insn = an.insns[addr]
            raw = data[off:off + sz].hex()
            cmt = ''
            if mn in ('ldr', 'ldr.w') and '[pc' in ops:
                try:
                    for op in insn.operands:
                        if op.type == capstone.CS_OP_MEM and op.mem.base == arm_const.ARM_REG_PC:
                            lit = ((addr + 4) & ~3) + op.mem.disp
                            lo = lit - BASE
                            if 0 <= lo + 4 <= len(data):
                                v = struct.unpack_from('<I', data, lo)[0]
                                cmt = f'  ; ={an.resolve(v)}'
                except Exception:
                    pass
            lines.append(f'{addr:08X}  {raw:<8} {mn:<8} {ops}{cmt}')
            addr += sz
        elif addr in lit_words:
            lo = off
            if lo + 4 <= len(data):
                v = struct.unpack_from('<I', data, lo)[0]
                used = ', '.join(f'{u:08X}' for u in sorted(an.lits[addr]))
                lines.append(f'{addr:08X}  .word 0x{v:08X}  ; {an.resolve(v)}   used by {used}')
            addr += 4
        else:
            if data_start is None:
                data_start = addr
            lines.append(f'{addr:08X}  .byte 0x{data[off]:02X}')
            addr += 1
    with open(os.path.join(OUT, 'disasm.txt'), 'w', encoding='utf-8') as f:
        f.write('\n'.join(lines))

    # ---- output: peripheral access map
    periph_hits = []
    for addr, (sz, mn, ops, insn) in sorted(an.insns.items()):
        for op in insn.operands:
            base_addr = None
            disp = 0
            if op.type == capstone.CS_OP_MEM and op.mem.base == arm_const.ARM_REG_PC:
                lit = ((addr + 4) & ~3) + op.mem.disp
                lo = lit - BASE
                if 0 <= lo + 4 <= len(data):
                    v = struct.unpack_from('<I', data, lo)[0]
                    if is_periph(v):
                        periph_hits.append({'at': f'0x{addr:08X}', 'func': f'0x{an.owner.get(addr,0):08X}',
                                            'reg': periph_name(v), 'via': 'literal', 'insn': f'{mn} {ops}'})
            break
    with open(os.path.join(OUT, 'periph_map.json'), 'w', encoding='utf-8') as f:
        json.dump(periph_hits, f, indent=1)

    # ---- output: gaps
    with open(os.path.join(OUT, 'gaps.txt'), 'w', encoding='utf-8') as f:
        f.write(f'# {len(gaps)} unclaimed ranges\n')
        for s, e in gaps:
            if e - s < 2:
                continue
            n = min(e - s + 2, 64)
            raw = data[s - BASE:s - BASE + n].hex()
            f.write(f'0x{s:08X} - 0x{e:08X}  len=0x{e-s+2:<4}  {raw}\n')

    print(f'functions: {len(an.funcs)}   instructions: {len(an.insns)}')
    print(f'literal pools: {len(an.lits)}')
    print(f'unclaimed ranges: {len(gaps)}')
    tot = sum(e - s + 2 for s, e in gaps)
    print(f'unclaimed bytes: {tot} of {an.size}')
    print(f'calls: {sum(len(v) for v in an.calls.values())}')
    # top functions
    print('\ntop functions by insn count:')
    for entry, f in sorted(an.funcs.items(), key=lambda kv: -kv[1]['insns'])[:15]:
        print(f'  func_{entry:08X}  insns={f["insns"]:<5} blocks={len(f["blocks"]):<4} '
              f'end=0x{f["end"]:08X}  out={len(an.calls.get(entry,()))}')
    print('\nunclaimed ranges > 8 bytes:')
    for s, e in gaps:
        if e - s + 2 > 8:
            print(f'  0x{s:08X} - 0x{e:08X} len=0x{e-s+2:x}')


if __name__ == '__main__':
    main()
