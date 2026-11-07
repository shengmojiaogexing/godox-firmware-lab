#!/usr/bin/env python3
"""ghidra_fallback.py -- capstone-only analysis/export of the X3-C firmware.

This is the Ghidra-independent fallback (and cross-check) exporter. It produces the
same core database as devices/x3c/tools/ghidra_scripts/ExportAll.java:

    functions.csv, decompiled/<addr>_<name>.c (pseudo-C annotated disassembly),
    strings.csv (with literal-pool xrefs), datarefs.csv, callgraph.json,
    vector_table.csv, gaps.csv, overview.json

Main app image:  file offset F -> vaddr 0x08004800 + F
Hot-shoe blob :  tools/hotshoe_blob.bin, file F -> 0x08002000 + F

Usage:
    tools/python/python.exe ghidra_fallback.py [--program main|hotshoe]
                                                     [--out DIR] [--max-funcs N]
"""
import argparse
import json
import os
import sys

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB
from capstone.arm import (ARM_OP_IMM, ARM_OP_MEM, ARM_REG_PC, ARM_GRP_JUMP,
                          ARM_GRP_CALL, ARM_GRP_INT, ARM_INS_LDR, ARM_INS_POP,
                          ARM_INS_BX, ARM_INS_BLX, ARM_INS_B, ARM_INS_BL,
                          ARM_INS_CBZ, ARM_INS_CBNZ)

BASE = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", ".."))
PROGRAMS = {
    "main": dict(file=os.path.join(BASE, "devices", "x3c", "firmware",
                                   "X3_C_v1.38_app.bin"),
                 base=0x08004800, vec=131, reset=0x08004AF9),
    "hotshoe": dict(file=os.path.join(BASE, "tools", "hotshoe_blob.bin"),
                    base=0x08002000, vec=46, reset=0x080020CC),
}

FLASH_LO, FLASH_HI = 0x08000000, 0x08200000
RAM_LO, RAM_HI = 0x20000000, 0x20100000

VECTOR_NAMES = ["SP", "Reset_Handler", "NMI_Handler", "HardFault_Handler",
                "MemManage_Handler", "BusFault_Handler", "UsageFault_Handler",
                "Reserved7", "Reserved8", "Reserved9", "Reserved10", "SVC_Handler",
                "DebugMon_Handler", "Reserved13", "PendSV_Handler", "SysTick_Handler"]

PERIPHERALS = {
    0x40020000: "GPIOA", 0x40020400: "GPIOB", 0x40020800: "GPIOC", 0x40020C00: "GPIOD",
    0x40021000: "GPIOE", 0x40021400: "GPIOF", 0x40021800: "GPIOG", 0x40021C00: "GPIOH",
    0x40022000: "GPIOI", 0x40023800: "RCC", 0x40023C00: "FLASH", 0x40002800: "RTC",
    0x40003000: "IWDG", 0x40007000: "PWR", 0x40001000: "TIM6", 0x40001400: "TIM7",
    0x40010400: "TIM8", 0x40012000: "ADC1", 0x40012100: "ADC2", 0x40012200: "ADC3",
    0x40000C00: "TIM5", 0x40010000: "TIM1", 0x40000400: "TIM3", 0x40000000: "TIM2",
    0x40013000: "SPI1", 0x40003800: "SPI2", 0x40003C00: "SPI3", 0x40011000: "USART1",
    0x40004400: "USART2", 0x40004800: "USART3", 0x40005400: "I2C1", 0x40005800: "I2C2",
    0x40023000: "CRC", 0x40013C00: "EXTI", 0x40026400: "DMA2", 0x40026000: "DMA1",
    0x40006000: "SPI?", 0x50000000: "USB_OTG_FS", 0x50040000: "USB_OTG_HS",
    0xA0000000: "FMC", 0x08000000: "FLASH_BASE", 0xE000E000: "NVIC/SCB",
    0xE000ED00: "SCB", 0xE000E010: "SysTick", 0xE000ED08: "SCB_VTOR",
    0xE000E100: "NVIC_ISER", 0xE000E180: "NVIC_ICER", 0xE000E400: "NVIC_IPR",
}

KNOWN_SYMBOLS = {
    0x08004AF8: "Reset_Handler",
    0x08004A0C: "__scatterload/__main",
    0x0801666C: "SystemInit",
    0x08004800: "__Vectors",
}

PROLOGUE2 = {0xB4, 0xB5, 0xB0}          # push {..}, push {..,lr}, sub sp,#imm
PROLOGUE4 = {0xE92D: "push.w", 0xE9: None}


def is_prologue(buf, off):
    if off + 2 > len(buf):
        return False
    b0, b1 = buf[off], buf[off + 1]
    if b0 in PROLOGUE2:
        return True
    if off + 4 <= len(buf):
        hw = b0 | (b1 << 8)
        if hw == 0xE92D:
            return True
        if b0 == 0xE9 and (b1 & 0x0F) == 0x2D:
            return True
    return False


def classify(v):
    x = v & 0xFFFFFFFF
    if FLASH_LO <= x < FLASH_HI:
        return "flash"
    if RAM_LO <= x < RAM_HI:
        return "ram"
    if 0x40000000 <= x < 0x60000000:
        return "peripheral"
    if 0xE0000000 <= x < 0xE0100000:
        return "scs"
    if 0xA0000000 <= x < 0xA0001000:
        return "fmc"
    return "const"


def hexs(v):
    return "0x%08x" % (v & 0xFFFFFFFF)


class Analyzer:
    def __init__(self, data, base, verbose=True):
        self.data = data
        self.base = base
        self.verbose = verbose
        self.md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
        self.md.detail = True
        self.owner = {}        # insn addr -> function start
        self.blocks = {}       # func start -> set of address covered
        self.calls = []        # (from_func, from_addr, to)
        self.tail_branches = []
        self.literals = {}     # literal_addr -> value  (only ldr literal)
        self.func_starts = []
        self.strings = {}      # addr -> text
        self._string_map_done = False

    def vaddr_to_off(self, va):
        off = va - self.base
        if 0 <= off < len(self.data):
            return off
        return None

    def is_code_range(self, va):
        if not (self.base <= va < self.base + len(self.data)):
            return False
        return True

    def insn_at(self, va):
        off = self.vaddr_to_off(va)
        if off is None:
            return None
        try:
            it = self.md.disasm(self.data[off:off + 4], va)
            return next(it, None)
        except Exception:
            return None

    def add_func(self, va):
        if va in self.owner.values():
            return
        if va in self.func_starts:
            return
        if not self.is_code_range(va):
            return
        self.func_starts.append(va)

    def disasm_block(self, fstart, start_addr):
        """Linear sweep of one basic block until branch/return; returns next block work items."""
        work = []
        a = start_addr
        n = 0
        while True:
            if a in self.owner:
                break
            ins = self.insn_at(a)
            if ins is None:
                break
            self.owner[a] = fstart
            self.blocks.setdefault(fstart, set()).add(a)
            n += 1
            if n > 4000:
                break
            groups = ins.groups
            mnem = ins.mnemonic
            next_addr = a + ins.size
            target = None
            for op in ins.operands:
                if op.type == ARM_OP_IMM:
                    target = op.imm
                    break
            # literal load
            if ins.id == ARM_INS_LDR and len(ins.operands) >= 2:
                op = ins.operands[1]
                if op.type == ARM_OP_MEM and op.mem.base == ARM_REG_PC:
                    lit = ((a + 4) & ~3) + op.mem.disp
                    off = self.vaddr_to_off(lit)
                    if off is not None and off + 4 <= len(self.data):
                        self.literals[lit] = int.from_bytes(self.data[off:off + 4], "little")
            if ARM_GRP_INT in groups:
                break
            if mnem.startswith("pop") and "pc" in ins.op_str:
                break
            if ins.id in (ARM_INS_BX,):
                break
            if ins.id in (ARM_INS_BLX,):
                # indirect call; continue after
                a = next_addr
                continue
            if ins.id == ARM_INS_BL:
                if target:
                    self.calls.append((fstart, a, target))
                    self.add_func(target)
                    work.append((target, target))
                a = next_addr
                continue
            if ins.id == ARM_INS_B and ARM_GRP_JUMP in groups:
                if target:
                    if target in self.func_starts or (target not in self.owner and is_prologue(self.data, self.vaddr_to_off(target) or 0)):
                        self.add_func(target)
                        self.tail_branches.append((fstart, a, target))
                        work.append((target, target))
                    else:
                        work.append((fstart, target))
                break
            if ins.id in (ARM_INS_CBZ, ARM_INS_CBNZ) or (ARM_GRP_JUMP in groups and target):
                if target:
                    work.append((fstart, target))
                a = next_addr
                continue
            if ARM_GRP_JUMP in groups:
                break
            a = next_addr
        return work

    def traverse(self):
        work = []
        for va in list(self.func_starts):
            work.append((va, va))
        while work:
            f, a = work.pop()
            if a in self.owner:
                continue
            work.extend(self.disasm_block(f, a))

    def scan_strings(self):
        data = self.data
        i, n = 0, len(data)
        while i < n:
            b = data[i]
            if 0x20 <= b < 0x7F or b >= 0xC2:
                j = i
                while j < n and (0x20 <= data[j] < 0x7F or data[j] >= 0x80):
                    j += 1
                if j - i >= 4:
                    raw = data[i:j]
                    try:
                        s = raw.decode("utf-8")
                        printable = sum(1 for c in s if ord(c) >= 0x20)
                        if len(s) >= 4 and printable * 10 >= len(s) * 6:
                            self.strings[self.base + i] = s
                    except Exception:
                        pass
                i = j
            else:
                i += 1

    def annotate_value(self, v):
        cls = classify(v)
        if cls == "flash":
            off = self.vaddr_to_off(v)
            if off is not None:
                if v in self.strings:
                    return "= " + hexs(v) + '  "' + self.strings[v].replace("\n", "\\n") + '"'
                if v in self.func_starts or (v & ~1) in self.owner:
                    return "= " + hexs(v) + "  " + self.func_name(v)
                return "= " + hexs(v) + "  [image data]"
            return "= " + hexs(v)
        if cls in ("peripheral", "scs"):
            name = PERIPHERALS.get(v) or PERIPHERALS.get(v & ~0x3FF)
            return "= " + hexs(v) + ("  [" + name + "]" if name else "  [peripheral]")
        if cls == "ram":
            return "= " + hexs(v) + "  [ram]"
        return "= " + hexs(v)

    def func_name(self, va):
        va = va & ~1
        if va in KNOWN_SYMBOLS:
            return KNOWN_SYMBOLS[va]
        return "FUN_%08x" % va

    def discover(self):
        # seeds: vector table targets
        for i in range(self.vec_count):
            off = 4 * i
            if off + 4 > len(self.data):
                break
            v = int.from_bytes(self.data[off:off + 4], "little")
            if i == 0:
                continue
            if FLASH_LO <= v < FLASH_HI:
                self.add_func(v & ~1)
        self.traverse()
        # follow function pointers found in literal pools (odd => Thumb code pointer)
        total_lit_funcs = 0
        for _round in range(6):
            new = 0
            for lit_addr, val in list(self.literals.items()):
                if FLASH_LO <= val < FLASH_HI and (val & 1):
                    t = val & ~1
                    if t not in self.func_starts and self.is_code_range(t):
                        self.add_func(t)
                        new += 1
            if new:
                total_lit_funcs += new
                self.traverse()
            else:
                break
        # prologue scan for leftovers: strong prologues (push {..lr} / push.w) only
        added = 0
        for off in range(0, len(self.data) - 4, 2):
            va = self.base + off
            if va in self.owner:
                continue
            if not is_prologue(self.data, off):
                continue
            b0 = self.data[off]
            strong = (b0 == 0xB5) or (off + 2 <= len(self.data) and self.data[off:off + 2] == b"\x2d\xe9")
            if not strong:
                continue
            # only in a reasonably small uncovered island (avoid scanning data/font blobs)
            island = 0
            while island < 1024 and (off + island) < len(self.data) and (self.base + off + island) not in self.owner:
                island += 2
            if island > 1024:
                continue
            self.add_func(va)
            added += 1
        if added:
            self.traverse()
            for _round in range(4):
                new = 0
                for lit_addr, val in list(self.literals.items()):
                    if FLASH_LO <= val < FLASH_HI and (val & 1):
                        t = val & ~1
                        if t not in self.func_starts and self.is_code_range(t):
                            self.add_func(t)
                            new += 1
                if new:
                    self.traverse()
                else:
                    break
        if self.verbose:
            print("discover: seeds=%d prologue_added=%d funcs=%d insns=%d"
                  % (self.vec_count, added, len(self.func_starts), len(self.owner)))

    def func_inventory(self):
        out = {}
        for f in self.func_starts:
            addrs = sorted(self.blocks.get(f, []))
            if not addrs:
                continue
            out[f] = addrs
        return out

    def run(self):
        self.scan_strings()
        self.discover()
        return self


def export(prog_name, an, outdir):
    os.makedirs(os.path.join(outdir, "decompiled"), exist_ok=True)
    inv = an.func_inventory()
    starts = sorted(inv.keys())
    owner_of = an.owner
    # callers/callees maps
    callers = {f: set() for f in starts}
    callees = {f: set() for f in starts}
    for f, a, t in an.calls:
        t2 = t & ~1
        if t2 in callers:
            callees.setdefault(f, set()).add(t2)
            callers.setdefault(t2, set()).add(f)
    for f, a, t in an.tail_branches:
        t2 = t & ~1
        if t2 in callers:
            callees.setdefault(f, set()).add(t2)

    # functions.csv
    with open(os.path.join(outdir, "functions.csv"), "w", encoding="utf-8", newline="") as fo:
        fo.write("addr,name,size,thunk,external,insn_count,param_count,caller_count,callers,callee_count,callees,comment\n")
        for f in starts:
            addrs = inv[f]
            size = max(addrs) - min(addrs) + 2
            nm = an.func_name(f)
            cs = ";".join(hexs(x) + " " + an.func_name(x) for x in sorted(callers.get(f, [])))
            ce = ";".join(hexs(x) + " " + an.func_name(x) for x in sorted(callees.get(f, [])))
            fo.write('%s,%s,%d,False,False,%d,0,%d,"%s",%d,"%s",\n'
                     % (hexs(f), nm, size, len(addrs), len(callers.get(f, [])), cs,
                        len(callees.get(f, [])), ce))  # noqa: E501

    # decompiled pseudo-C annotated listings
    index = open(os.path.join(outdir, "decompiled", "_index.tsv"), "w", encoding="utf-8")
    index.write("addr\tname\tsize\tfile\n")
    for f in starts:
        addrs = inv[f]
        nm = an.func_name(f)
        fname = "%s_%s.c" % (hexs(f), nm.replace(" ", "_").replace("/", "_"))
        lines = ["// pseudo-C (capstone annotated disassembly) %s %s size=%d" % (hexs(f), nm, max(addrs) - min(addrs) + 2),
                 "// NOTE: generated by devices/x3c/tools/ghidra_fallback.py (Ghidra-independent fallback)"]
        for a in addrs:
            ins = an.insn_at(a)
            if ins is None:
                continue
            txt = "%s  %-7s %s" % (hexs(a), ins.mnemonic, ins.op_str)
            note = ""
            for lit_addr, val in list(an.literals.items()):
                if lit_addr == a:
                    note = "   ; literal @" + hexs(lit_addr) + " " + an.annotate_value(val)
            if ins.id in (ARM_INS_BL, ARM_INS_B, ARM_INS_BLX):
                for op in ins.operands:
                    if op.type == ARM_OP_IMM and an.is_code_range(op.imm):
                        note += "   ; -> " + an.func_name(op.imm)
            if not note:
                # annotate literal loads by looking up operand mem pc disp
                pass
            lines.append(txt + note)
        with open(os.path.join(outdir, "decompiled", fname), "w", encoding="utf-8") as fo:
            fo.write("\n".join(lines) + "\n")
        index.write("%s\t%s\t%d\t%s\n" % (hexs(f), nm, max(addrs) - min(addrs) + 2, fname))
    index.close()

    # strings.csv with literal-pool xrefs
    import csv as _csv
    lit_items = sorted(an.literals.items())
    str_xrefs = {}
    for lit_addr, val in lit_items:
        for cand in (val, val | 1, val & ~1):
            if cand in an.strings:
                str_xrefs.setdefault(cand, []).append(lit_addr)
    with open(os.path.join(outdir, "strings.csv"), "w", encoding="utf-8", newline="") as fo:
        wr = _csv.writer(fo)
        wr.writerow(["addr", "length", "kind", "text", "xref_count", "xrefs"])
        for saddr in sorted(an.strings):
            text = an.strings[saddr]
            xs = str_xrefs.get(saddr, [])
            xr = ";".join(hexs(x) + (" " + an.func_name(owner_of[x]) if x in owner_of else "") for x in xs)
            wr.writerow([hexs(saddr), len(text), "raw", text, len(xs), xr])

    # datarefs.csv
    with open(os.path.join(outdir, "datarefs.csv"), "w", encoding="utf-8", newline="") as fo:
        fo.write("src_addr,kind,value,value_class,target_addr,target_kind,target_symbol\n")
        for lit_addr, val in lit_items:
            cls = classify(val)
            if cls == "const":
                continue
            tk = ""
            if cls == "flash" and val in an.strings:
                tk = "string"
            fo.write("%s,literal,%s,%s,%s,%s,%s\n"
                     % (hexs(lit_addr), hexs(val), cls, hexs(val), tk, an.annotate_value(val)[2:]))

    # callgraph.json
    edges = []
    for f, a, t in an.calls:
        edges.append({"from": hexs(f), "to": hexs(t & ~1), "type": "call", "at": hexs(a)})
    for f, a, t in an.tail_branches:
        edges.append({"from": hexs(f), "to": hexs(t & ~1), "type": "tail_branch", "at": hexs(a)})
    cg = {"program": prog_name,
          "nodes": [{"addr": hexs(f), "name": an.func_name(f),
                     "size": max(inv[f]) - min(inv[f]) + 2} for f in starts],
          "edges": edges}
    with open(os.path.join(outdir, "callgraph.json"), "w", encoding="utf-8") as fo:
        json.dump(cg, fo, indent=1)

    # vector table
    with open(os.path.join(outdir, "vector_table.csv"), "w", encoding="utf-8", newline="") as fo:
        fo.write("index,vector_addr,raw_value,target,valid,function_at_target,insn_at_target,is_default_loop,is_function_start\n")
        for i in range(an.vec_count):
            off = 4 * i
            addr = an.base + off
            v = int.from_bytes(an.data[off:off + 4], "little")
            tgt = v & ~1 if FLASH_LO <= v < FLASH_HI else 0
            fa = an.func_name(tgt) if tgt in inv else ""
            ins = an.insn_at(tgt) if tgt else None
            itxt = (ins.mnemonic + " " + ins.op_str) if ins else ""
            dflt = "yes" if (tgt in an.owner and ins is not None and ins.mnemonic.startswith("b")
                             and ins.op_str.strip() == "#" + hexs(tgt)) else ""
            fo.write("%d,%s,%s,%s,%s,%s,\"%s\",%s,%s\n"
                     % (i, hexs(addr), hexs(v), hexs(tgt) if tgt else "",
                        "yes" if tgt else "no", fa, itxt, dflt, "yes" if tgt in inv else "no"))

    # gaps: uncovered bytes
    with open(os.path.join(outdir, "gaps.csv"), "w", encoding="utf-8", newline="") as fo:
        fo.write("start,end,length\n")
        gap_start = None
        run = 0
        for off in range(0, len(an.data), 2):
            va = an.base + off
            if va not in an.owner:
                if gap_start is None:
                    gap_start = va
                    run = 0
                run += 2
            else:
                if run >= 64:
                    fo.write("%s,%s,%d\n" % (hexs(gap_start), hexs(gap_start + run - 2), run))
                gap_start = None
                run = 0
        if run >= 64:
            fo.write("%s,%s,%d\n" % (hexs(gap_start), hexs(gap_start + run - 2), run))

    # overview
    ov = {"program": prog_name, "base": hexs(an.base), "image_size": len(an.data),
          "functions": len(starts), "instructions": len(an.owner), "strings": len(an.strings),
          "literals": len(an.literals), "decompiled": len(starts)}
    with open(os.path.join(outdir, "overview.json"), "w", encoding="utf-8") as fo:
        json.dump(ov, fo, indent=1)
    print("export: functions=%d insns=%d strings=%d literals=%d -> %s"
          % (len(starts), len(an.owner), len(an.strings), len(an.literals), outdir))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--program", default="main", choices=["main", "hotshoe"])
    ap.add_argument("--out", default=None)
    args = ap.parse_args()
    cfg = PROGRAMS[args.program]
    outdir = args.out or os.path.join(BASE, "work", "re", "ghidra_exports",
                                      "main" if args.program == "main" else "hotshoe")
    data = open(cfg["file"], "rb").read()
    print("loaded %s (%d bytes) base=%s" % (cfg["file"], len(data), hexs(cfg["base"])))
    an = Analyzer(data, cfg["base"])
    an.vec_count = cfg["vec"]
    an.run()
    export(args.program, an, outdir)


if __name__ == "__main__":
    main()
