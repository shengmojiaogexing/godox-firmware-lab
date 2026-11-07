#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""decompile.py - capstone-based pseudo-C decompiler for Godox AD100 Pro V1.2.

Ghidra could not be downloaded (network), so this script turns the existing
capstone artifacts into readable pseudo-C:

  inputs   firmware/AD100pro_V1.2.bin
           out/functions.json        (function table: addr/end/size/insns/blocks)
           out/disasm.txt            (annotated linear disassembly, capstone)
           out/periph_map.json       (MMIO register accesses, optional)
           out/functions.meta.json   (periph/callees, optional)

  outputs  out/decompiled/<addr>.c        one pseudo-C file per function
           out/decompiled/index.tsv       address/size/insns/calls/name suggestion
           out/decompiled/CALLGRAPH.md    call graph (main as root + rankings)
           out/decompiled/issues_auto.md  automatically detected data problems
           out/decompiled/SELFCHECK.md    random sample verification vs disasm.txt

The translation is deliberately mechanical: every instruction keeps its
original disassembly comment and gets a pseudo-C statement.  It is not a real
decompiler - it is a structured, searchable representation of the firmware.
"""
import json
import os
import random
import re
import struct
import sys
from collections import Counter, defaultdict

try:
    from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS
except Exception:  # capstone only needed for the rare fallback path
    Cs = None

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # -> devices/ad100pro
BIN = os.path.join(ROOT, "firmware", "AD100pro_V1.2.bin")
FUNCS = os.path.join(ROOT, "out", "functions.json")
META = os.path.join(ROOT, "out", "functions.meta.json")
DISASM = os.path.join(ROOT, "out", "disasm.txt")
PERIPH = os.path.join(ROOT, "out", "periph_map.json")
GAPS = os.path.join(ROOT, "out", "gaps.txt")
OUT = os.path.join(ROOT, "out", "decompiled")

BASE = 0x08003000
IMG_SIZE = 0xC448
END = BASE + IMG_SIZE
RAM_LO, RAM_HI = 0x20000000, 0x20002000

# ----------------------------------------------------------------------------
# known names (vector table + important functions identified by prior analysis)
# ----------------------------------------------------------------------------
SYSTEM_VEC_NAMES = [
    "Initial_SP", "Reset_Handler", "NMI_Handler", "HardFault_Handler",
    "MemManage_Handler", "BusFault_Handler", "UsageFault_Handler",
    "Reserved7", "Reserved8", "Reserved9", "Reserved10", "SVC_Handler",
    "DebugMon_Handler", "Reserved13", "PendSV_Handler", "SysTick_Handler",
]
KEY_NAMES = {
    0x080030C0: "__start",
    0x080030C2: "__start_mov_sp",
    0x080030C8: "__start_call_main",
    0x080030D4: "Reset_Handler",
    0x080030D8: "Reset_Handler_call_start",
    0x080030E6: "padding_b_self",
    0x080030EC: "literal_080030C1",
    0x08003168: "lookup_or_pad",
    0x08003548: "__libc_init_array",
    0x0800548: "SystemInit?",
    0x08007434: "delay_or_timing",
    0x08008C78: "GPIO_init",
    0x08008D00: "dispatcher_large",
    0x0800B964: "SystemInit",
    0x0800CF74: "main",
    0x0800DA78: "big_isr_candidate",
}

# peripheral address map used for literal decoding inside pseudo-C comments
PERIPH_NAMES = [
    (0x40021000, "RCC"), (0x48000000, "GPIOA"), (0x48000400, "GPIOB"),
    (0x48000800, "GPIOC"), (0x48000C00, "GPIOD"), (0x48001000, "GPIOE"),
    (0x48001400, "GPIOF"), (0x40010400, "EXTI"), (0x40010000, "SYSCFG"),
    (0x40022000, "FLASH"), (0x40007000, "PWR"), (0x40003000, "IWDG"),
    (0x40002800, "RTC"), (0x40020000, "DMA1"), (0x40012400, "ADC"),
    (0x40012C00, "TIM1"), (0x40000000, "TIM2"), (0x40000400, "TIM3"),
    (0x40001000, "TIM6"), (0x40001400, "TIM7"), (0x40002000, "TIM14"),
    (0x40014000, "TIM15"), (0x40014400, "TIM16"), (0x40014800, "TIM17"),
    (0x40013800, "USART1"), (0x40004400, "USART2"), (0x40013000, "SPI1"),
    (0x40003800, "SPI2"), (0x40005400, "I2C1"), (0xE000E000, "SCS"),
    (0xE000E010, "SysTick"), (0xE000ED00, "SCB"),
]

# mnemonic -> (C comparison operator, note)   note == None means plain == / !=
COND_MNEM = {
    "beq": ("==", None), "bne": ("!=", None),
    "bcs": (">=", "unsigned"), "bhs": (">=", "unsigned"),
    "bcc": ("<", "unsigned"), "blo": ("<", "unsigned"),
    "bhi": (">", "unsigned"), "bls": ("<=", "unsigned"),
    "bge": (">=", "signed"), "blt": ("<", "signed"),
    "bgt": (">", "signed"), "ble": ("<=", "signed"),
    "bmi": ("<", "result negative"), "bpl": (">=", "result non-negative"),
    "bvs": ("", "overflow set"), "bvc": ("", "overflow clear"),
}
COND_SET = set(COND_MNEM)

DATA_NAMES = {
    0x08003000: "vector_table",
    0x08003001: "vector_table",
    0x20000000: "RAM_base",
    0x20000CD0: "initial_SP",
    0xE000ED08: "SCB_VTOR",
    0x40021014: "RCC_AHBENR",
    0x40021018: "RCC_APB2ENR",
    0x4002101C: "RCC_APB1ENR",
}

REG_RE = re.compile(r"^(r\d+|r1[0-2]|sp|lr|pc|ip|fp|sb|sl)$")


# ----------------------------------------------------------------------------
# data loading
# ----------------------------------------------------------------------------
def load_bin():
    with open(BIN, "rb") as f:
        return f.read()


def rd32(data, addr):
    off = addr - BASE
    if 0 <= off <= len(data) - 4:
        return struct.unpack_from("<I", data, off)[0]
    return None


def rd16(data, addr):
    off = addr - BASE
    if 0 <= off <= len(data) - 2:
        return struct.unpack_from("<H", data, off)[0]
    return None


def load_disasm():
    """Parse out/disasm.txt into instruction/data maps."""
    insns = {}      # addr -> dict(bytes, mnem, ops, cmt, raw)
    words = {}      # addr -> (value, comment)
    sections = {}   # addr -> dict(insns, blocks, end, called_by)
    order = []
    cur = None
    hdr_re = re.compile(
        r"^; =+ func_([0-9A-Fa-f]{8})\s+insns=(\d+) blocks=(\d+) end=0x([0-9A-Fa-f]{8})")
    line_re = re.compile(r"^([0-9A-Fa-f]{8})\s+(\S+)\s*(.*)$")
    with open(DISASM, "r", encoding="utf-8", errors="replace") as f:
        for ln in f:
            ln = ln.rstrip("\n")
            if not ln:
                continue
            if ln.startswith(";"):
                m = hdr_re.match(ln)
                if m:
                    cur = {
                        "addr": int(m.group(1), 16),
                        "insns": int(m.group(2)),
                        "blocks": int(m.group(3)),
                        "end": int(m.group(4), 16),
                        "called_by": [],
                    }
                    sections[cur["addr"]] = cur
                    continue
                if ln.startswith(";   called by:") and cur is not None:
                    cur["called_by"].append(ln.split(":", 1)[1].strip())
                continue
            m = line_re.match(ln)
            if not m:
                continue
            addr = int(m.group(1), 16)
            tok = m.group(2)
            rest = (m.group(3) or "").strip()
            order.append(addr)
            if tok.startswith(".byte"):
                continue
            if tok.startswith(".word"):
                val = None
                cmt = ""
                if ";" in rest:
                    valtxt, cmt = rest.split(";", 1)
                    cmt = cmt.strip()
                else:
                    valtxt = rest
                try:
                    val = int(valtxt.strip(), 16)
                except ValueError:
                    val = None
                words[addr] = (val, cmt)
                continue
            # real instruction: tok is the raw bytes hex
            cmt = ""
            body = rest
            if ";" in body:
                body, cmt = body.split(";", 1)
                cmt = cmt.strip()
            body = body.strip()
            if not body:
                continue
            parts = body.split(None, 1)
            mnem = parts[0]
            ops = parts[1].strip() if len(parts) > 1 else ""
            insns[addr] = {
                "addr": addr,
                "bytes": tok,
                "mnem": mnem,
                "ops": ops,
                "cmt": cmt,
                "raw": ln,
            }
    return insns, words, sections


def load_functions():
    with open(FUNCS, "r", encoding="utf-8") as f:
        funcs = json.load(f)
    out = []
    for f in funcs:
        out.append({
            "addr": int(f["addr"], 16),
            "end": int(f["end"], 16),
            "size": int(f["size"]),
            "insns": int(f["insns"]),
            "blocks": [int(b, 16) for b in f.get("blocks", [])],
        })
    out.sort(key=lambda x: x["addr"])
    return out


def load_meta():
    if not os.path.exists(META):
        return {}
    with open(META, "r", encoding="utf-8") as f:
        data = json.load(f)
    m = {}
    for x in data:
        m[int(x["addr"], 16)] = {
            "callees": [int(c, 16) for c in x.get("callees", [])],
            "periph": x.get("periph", []),
        }
    return m


def load_periph():
    if not os.path.exists(PERIPH):
        return {}
    with open(PERIPH, "r", encoding="utf-8") as f:
        data = json.load(f)
    m = defaultdict(list)
    for x in data:
        at = x.get("at")
        if not at:
            continue
        try:
            addr = int(at, 16)
        except ValueError:
            continue
        m[addr].append(x)
    return m


def load_gaps():
    gaps = []
    if not os.path.exists(GAPS):
        return gaps
    with open(GAPS, "r", encoding="utf-8") as f:
        for ln in f:
            m = re.match(r"0x([0-9A-Fa-f]{8}) - 0x([0-9A-Fa-f]{8})", ln)
            if m:
                gaps.append((int(m.group(1), 16), int(m.group(2), 16)))
    return gaps


# ----------------------------------------------------------------------------
# value classification / naming
# ----------------------------------------------------------------------------
def is_flash(v):
    return BASE <= v < END


def is_ram(v):
    return RAM_LO <= v < RAM_HI


def periph_name(v):
    for base, name in PERIPH_NAMES:
        if base <= v < base + 0x400:
            return "%s+0x%02X" % (name, v - base)
    if 0x40000000 <= v < 0x40100000:
        return "APB+0x%X" % (v - 0x40000000)
    if 0xE0000000 <= v < 0xE0100000:
        return "SCS+0x%X" % (v - 0xE000E000)
    return None


def build_vectors(data):
    """Return {handler_addr: (vector_index, canonical_name)}."""
    vec = {}
    for i in range(48):
        w = rd32(data, BASE + i * 4)
        if w is None or w == 0 or i == 0:
            continue
        t = w & ~1
        if not is_flash(t):
            continue
        # 0x080030E6 is the shared CMSIS "b ." default handler
        if t == 0x080030E6:
            vec.setdefault(t, (i, "Default_Handler"))
            continue
        if i < len(SYSTEM_VEC_NAMES):
            vec.setdefault(t, (i, SYSTEM_VEC_NAMES[i]))
        else:
            vec.setdefault(t, (i, "IRQ%d_Handler" % (i - 16)))
    return vec


def build_names(funcs, vectors):
    names = {}
    for f in funcs:
        names.setdefault(f["addr"], "FUN_%08X" % f["addr"])
    for a, (idx, nm) in vectors.items():
        if a in names:
            names[a] = nm
        else:
            names[a] = nm
    for a, nm in KEY_NAMES.items():
        if a in names:
            names[a] = nm
    return names


def suggested_name(a, names, callers, callees, mmio):
    if a in names and names[a] != "FUN_%08X" % a:
        return names[a]
    hints = []
    if mmio:
        hints.append("mmio:" + "+".join(sorted({m.split("+")[0].split(".")[0] for m in mmio}))[:40])
    if len(callers) >= 8:
        hints.append("hub%d" % len(callers))
    if len(callees) >= 6:
        hints.append("dispatch%d" % len(callees))
    return "FUN_%08X" % a


# ----------------------------------------------------------------------------
# operand helpers
# ----------------------------------------------------------------------------
def split_operands(ops):
    out = []
    depth = 0
    cur = ""
    for ch in ops:
        if ch in "[{(":
            depth += 1
        elif ch in "]})":
            depth -= 1
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


def reg_of(tok):
    tok = tok.strip()
    if REG_RE.match(tok):
        return tok
    return None


def imm_of(tok):
    tok = tok.strip()
    if tok.startswith("#"):
        tok = tok[1:]
    try:
        return int(tok, 0)
    except ValueError:
        return None


MEM_RE = re.compile(r"^\[([^\]]+)\](!?)$")


def parse_mem(tok):
    m = MEM_RE.match(tok.strip())
    if not m:
        return None
    inner = m.group(1)
    wb = m.group(2) == "!"
    parts = [p.strip() for p in inner.split(",")]
    base = reg_of(parts[0])
    if base is None:
        return None
    info = {"base": base, "wb": wb, "index": None, "shift": None, "imm": None}
    if len(parts) >= 2:
        p = parts[1]
        r = reg_of(p)
        if r:
            info["index"] = r
            if len(parts) >= 3:
                info["shift"] = parts[2]
        else:
            iv = imm_of(p)
            info["imm"] = iv
    return info


def mem_expr(info):
    base = info["base"]
    if info["index"]:
        ex = "%s + %s" % (base, info["index"])
        if info["shift"]:
            ex = "(%s << %s)" % (ex, info["shift"].split()[-1].lstrip("#"))
    elif info["imm"] is not None and info["imm"] != 0:
        if info["imm"] < 0:
            ex = "%s - %d" % (base, -info["imm"])
        else:
            ex = "%s + %d" % (base, info["imm"])
    else:
        ex = base
    return ex


def mem_cast(info, ctype):
    return "*(%s *)(%s)" % (ctype, mem_expr(info))


def reg_sub(dst, src, op):
    return "%s = %s %s %s;" % (dst, src, op, dst)


# ----------------------------------------------------------------------------
# pseudo-C translation
# ----------------------------------------------------------------------------
class Translator:
    def __init__(self, data, insns, words, periph, names):
        self.data = data
        self.insns = insns
        self.words = words
        self.periph = periph
        self.names = names

    # -- literal pool resolution -------------------------------------------
    def lit_value(self, addr, insn):
        """For 'ldr rX, [pc, #imm]' return (lit_addr, value)."""
        m = re.search(r"\[pc,\s*(#(?:-?0x[0-9a-f]+|-?\d+))\]", insn["ops"])
        if not m:
            return None, None
        imm = imm_of(m.group(1))
        if imm is None:
            return None, None
        lit = ((addr + 4) & ~3) + imm
        val = rd32(self.data, lit)
        return lit, val

    def resolve_text(self, v):
        if v is None:
            return "?"
        if v in DATA_NAMES:
            return "= %s (0x%08X)" % (DATA_NAMES[v], v)
        if v in self.names:
            return "= %s (0x%08X)" % (self.names[v], v)
        if is_flash(v):
            return "= flash data/const 0x%08X" % v
        if is_ram(v):
            return "= RAM 0x%08X" % v
        p = periph_name(v)
        if p:
            return "= MMIO %s" % p
        return "= 0x%08X" % v

    def comm(self, addr, insn):
        return "/* 0x%08X: %-8s %-8s %s%s */" % (
            addr, insn["bytes"], insn["mnem"], insn["ops"],
            ("  ; " + insn["cmt"]) if insn["cmt"] else "")

    def mmio_comment(self, addr):
        if addr in self.periph:
            xs = self.periph[addr]
            regs = ", ".join("%s(%s)" % (x.get("reg", "?"), x.get("rw", "?")) for x in xs[:4])
            return "  /* MMIO: %s */" % regs
        return ""

    # -- main translation ---------------------------------------------------
    def translate_function(self, body_addrs, end):
        out = []          # list of (kind, text) kind in {'label','stmt','comment'}
        last_flags = None
        targets = set()
        for a in body_addrs:
            insn = self.insns[a]
            mnem = insn["mnem"].replace(".w", "").replace(".n", "")
            ops = insn["ops"]
            if is_branch_target(insn, body_addrs, end):
                targets.add(branch_dest(insn))
        emitted_labels = set()
        for a in body_addrs:
            insn = self.insns[a]
            if a in targets and a in body_addrs:
                out.append(("label", "L_%08X:" % a))
                emitted_labels.add(a)
            out.append(("comment", self.comm(a, insn)))
            mnem = insn["mnem"].replace(".w", "").replace(".n", "")
            ops = insn["ops"]
            oplist = split_operands(ops)
            txt = None
            extra = ""
            comment_mmio = self.mmio_comment(a)
            # ---- literal load
            if mnem.startswith("ldr") and len(oplist) == 2 and oplist[1].startswith("[pc"):
                lit, val = self.lit_value(a, insn)
                dst = oplist[0]
                name = self.resolve_text(val)
                txt = "%s = *(uint32_t *)0x%08X;   /* %s */" % (dst, lit, name)
                if val is not None and is_flash(val) and val in self.names:
                    extra = "   /* function pointer -> %s */" % self.names[val]
            # ---- load
            elif mnem.startswith("ldr") and len(oplist) == 2:
                mem = parse_mem(oplist[1])
                if mem:
                    ctype = {"ldrb": "uint8_t", "ldrh": "uint16_t",
                             "ldrsb": "int8_t", "ldrsh": "int16_t"}.get(mnem, "uint32_t")
                    txt = "%s = %s;" % (oplist[0], mem_cast(mem, ctype))
                else:
                    txt = "%s = %s;   /* unresolved load */" % (oplist[0], oplist[1])
            # ---- store
            elif mnem in ("str", "strb", "strh") and len(oplist) == 2:
                mem = parse_mem(oplist[1])
                if mem:
                    ctype = {"strb": "uint8_t", "strh": "uint16_t"}.get(mnem, "uint32_t")
                    txt = "%s = %s;   /* store */" % (mem_cast(mem, ctype), oplist[0])
                else:
                    txt = "/* store unresolved: %s */" % ops
            # ---- multiple load/store
            elif mnem in ("ldmia", "stmia", "ldm", "stm") and len(oplist) == 2:
                mem = parse_mem(oplist[1])
                if mem:
                    regs = oplist[0].strip("{}")
                    txt = "/* %s %s <- %s */" % (mnem, regs, mem_expr(mem))
                else:
                    txt = "/* %s %s */" % (mnem, ops)
            # ---- pushes / pops
            elif mnem in ("push", "pop") and oplist:
                regs = oplist[0].strip("{}").strip()
                if mnem == "push":
                    txt = "push({%s});" % regs
                else:
                    txt = "pop({%s});" % regs
                    if "pc" in [r.strip() for r in regs.split(",")]:
                        txt += "   /* return */"
            # ---- branches with link
            elif mnem in ("bl", "blx") and oplist:
                if oplist[0].startswith("#"):
                    tgt = imm_of(oplist[0])
                    nm = self.names.get(tgt, "FUN_%08X" % (tgt or 0))
                    txt = "%s();   /* call 0x%08X %s */" % (nm, tgt or 0, "-> " + nm)
                    if tgt:
                        txt = "%s();   /* BL -> 0x%08X %s */" % (nm, tgt, nm)
                elif reg_of(oplist[0]):
                    txt = "((void(*)())%s)();   /* indirect call */" % oplist[0]
                else:
                    txt = "/* call %s */" % ops
            # ---- bx
            elif mnem == "bx" and oplist:
                if oplist[0] == "lr":
                    txt = "return;"
                else:
                    nm = self.names.get(imm_of(oplist[0]), "")
                    txt = "goto *%s;   /* tail branch %s */" % (oplist[0], nm)
            # ---- unconditional branch
            elif mnem == "b" and oplist:
                tgt = imm_of(oplist[0])
                if tgt is not None and BASE <= tgt < end:
                    txt = "goto L_%08X;%s" % (tgt, "   /* loop */" if tgt <= a else "")
                elif tgt is not None and tgt == a:
                    txt = "while (1) { }   /* infinite loop */"
                else:
                    nm = self.names.get(tgt, "FUN_%08X" % (tgt or 0))
                    txt = "%s();   /* tail call 0x%08X */" % (nm, tgt or 0)
            # ---- conditional branch
            elif mnem in COND_SET and oplist:
                tgt = imm_of(oplist[0])
                cond, note = self.cond_expr(mnem, last_flags)
                txt = "if (%s) goto L_%08X;" % (cond, tgt or 0)
                if note:
                    txt += "   /* %s */" % note
            # ---- compare / test (flags)
            elif mnem in ("cmp", "cmn", "tst") and oplist:
                last_flags = (mnem, oplist)
                txt = "/* flags = %s(%s) */" % (mnem, ", ".join(oplist))
            elif mnem.startswith("cb") and oplist:
                tgt = imm_of(oplist[1]) if len(oplist) > 1 else None
                cond = "== 0" if mnem == "cbz" else "!= 0"
                txt = "if (%s %s) goto L_%08X;" % (oplist[0], cond, tgt or 0)
            # ---- move
            elif mnem in ("mov", "movs") and len(oplist) == 2:
                if oplist[1].startswith("#"):
                    txt = "%s = %s;" % (oplist[0], oplist[1].lstrip("#"))
                else:
                    txt = "%s = %s;" % (oplist[0], oplist[1])
            elif mnem in ("mov", "movs") and len(oplist) == 1:
                txt = "/* %s %s */" % (mnem, ops)
            # ---- mvn / rsb
            elif mnem in ("mvn", "mvns") and len(oplist) == 2:
                txt = "%s = ~%s;" % (oplist[0], oplist[1])
            elif mnem in ("rsbs", "rsb") and len(oplist) >= 2:
                if len(oplist) == 3 and oplist[2].startswith("#"):
                    txt = "%s = %s - %s;" % (oplist[0], oplist[2].lstrip("#"), oplist[1])
                else:
                    txt = "%s = 0 - %s;" % (oplist[0], oplist[1])
            # ---- alu two/three operand
            elif mnem in ("adds", "add", "sub", "subs", "and", "ands", "orr", "orrs",
                          "eor", "eors", "adc", "adcs", "sbc", "sbcs",
                          "lsl", "lsls", "lsr", "lsrs", "asr", "asrs", "ror", "rors",
                          "mul", "muls", "bic", "bics") and oplist:
                txt = self.alu(mnem, oplist)
            elif mnem in ("sxtb", "sxth", "uxtb", "uxth", "rev", "rev16", "revsh") and len(oplist) == 2:
                txt = "%s = %s(%s);" % (oplist[0], mnem, oplist[1])
            elif mnem == "adr":
                tgt = imm_of(oplist[0]) if oplist else None
                txt = "%s = 0x%08X;   /* address */" % (oplist[0] if oplist else "?", tgt or 0)
            elif mnem in ("nop",):
                txt = "/* nop */"
            elif mnem in ("wfi", "wfe", "sev"):
                txt = "/* %s (low power wait) */" % mnem
            elif mnem in ("dmb", "dsb", "isb"):
                txt = "/* memory barrier: %s */" % mnem
            elif mnem in ("svc",):
                txt = "svc(%s);   /* system call / supervisor */" % (oplist[0] if oplist else "")
            elif mnem in ("bkpt", "udf"):
                txt = "/* %s - breakpoint/trap */" % mnem
            elif mnem in ("it", "itt", "ite", "ittt", "itee", "itet", "itttt"):
                txt = "/* IT block: %s */" % ops
            elif mnem in ("cpsid", "cpsie"):
                txt = "/* interrupt %s */" % mnem
            else:
                txt = "/* TODO: %s %s */" % (mnem, ops)
            if txt is None:
                txt = "/* TODO: %s %s */" % (mnem, ops)
            if extra:
                txt += extra
            if comment_mmio:
                txt += comment_mmio
            kind = "stmt" if not txt.lstrip().startswith("/*") else "comment"
            out.append((kind, txt))
        return out

    def alu(self, mnem, oplist):
        base = mnem.rstrip("s")
        if len(oplist) == 3:
            dst, a, b = oplist
            b = b.lstrip("#")
            op = {"add": "+", "sub": "-", "and": "&", "orr": "|", "eor": "^",
                  "lsl": "<<", "lsr": ">>", "asr": ">>", "ror": ">>",
                  "adc": "+", "sbc": "-", "mul": "*", "bic": "& ~"}[base]
            if base == "mul":
                return "%s = %s * %s;" % (dst, a, b)
            if base == "bic":
                return "%s = %s & ~%s;" % (dst, a, b)
            return "%s = %s %s %s;" % (dst, a, op, b)
        if len(oplist) == 2:
            dst, b = oplist
            b = b.lstrip("#")
            op = {"add": "+", "sub": "-", "and": "&", "orr": "|", "eor": "^",
                  "lsl": "<<", "lsr": ">>", "asr": ">>", "ror": ">>",
                  "adc": "+", "sbc": "-", "mul": "*", "bic": "& ~"}[base]
            if base == "mul":
                return "%s = %s * %s;" % (dst, dst, b)
            if base == "bic":
                return "%s = %s & ~%s;" % (dst, dst, b)
            return "%s = %s %s %s;" % (dst, dst, op, b)
        return "/* %s %s */" % (mnem, ", ".join(oplist))

    def cond_expr(self, mnem, last_flags):
        op, note = COND_MNEM.get(mnem, (mnem, None))
        if last_flags:
            kind, oplist = last_flags
            if kind == "cmp" and len(oplist) == 2:
                a, b = oplist[0], oplist[1].lstrip("#")
                if op:
                    return "%s %s %s" % (a, op, b), note
                return "/* %s after cmp */" % mnem, note
            if kind == "tst" and len(oplist) == 2:
                return "(%s & %s)" % (oplist[0], oplist[1].lstrip("#")), note or "test bits"
            if kind == "cmn" and len(oplist) == 2:
                return "(%s + %s)" % (oplist[0], oplist[1].lstrip("#")), note
        return "/* %s */" % mnem, note


def is_branch_target(insn, body_addrs, end):
    mnem = insn["mnem"].replace(".w", "").replace(".n", "")
    return mnem in COND_SET or mnem in ("b", "cbz", "cbnz")


def branch_dest(insn):
    mnem = insn["mnem"].replace(".w", "").replace(".n", "")
    oplist = split_operands(insn["ops"])
    if mnem in ("cbz", "cbnz") and len(oplist) >= 2:
        return imm_of(oplist[1])
    if oplist:
        return imm_of(oplist[0])
    return None


# ----------------------------------------------------------------------------
# call graph
# ----------------------------------------------------------------------------
def build_callgraph(funcs, insns, names):
    fmap = {f["addr"]: f for f in funcs}
    callees = {f["addr"]: set() for f in funcs}
    callers = {f["addr"]: set() for f in funcs}
    tail_calls = {f["addr"]: set() for f in funcs}
    for f in funcs:
        a, e = f["addr"], f["end"]
        for i in range(a, e):
            insn = insns.get(i)
            if not insn:
                continue
            mnem = insn["mnem"].replace(".w", "").replace(".n", "")
            if mnem not in ("bl", "blx", "b") or not insn["ops"].startswith("#"):
                continue
            tgt = imm_of(insn["ops"].split(",")[0])
            if tgt is None:
                continue
            tgt &= ~1
            if tgt not in fmap:
                continue
            if mnem == "b":
                tail_calls[f["addr"]].add(tgt)
            else:
                callees[f["addr"]].add(tgt)
    for f in funcs:
        for c in callees[f["addr"]] | tail_calls[f["addr"]]:
            callers.setdefault(c, set()).add(f["addr"])
    return callees, callers, tail_calls


# ----------------------------------------------------------------------------
# body extraction
# ----------------------------------------------------------------------------
def get_body(f, insns):
    addrs = [a for a in range(f["addr"], f["end"]) if a in insns]
    if not addrs:
        # fragment with end==addr (or undecoded body): use decoded instruction at
        # the entry itself if the global listing has one.
        if f["addr"] in insns:
            addrs = [f["addr"]]
    return addrs


# ----------------------------------------------------------------------------
# writers
# ----------------------------------------------------------------------------
def fmt_callers(addrs, names, limit=8):
    parts = []
    for a in sorted(addrs)[:limit]:
        parts.append("0x%08X %s" % (a, names.get(a, "?")))
    if len(addrs) > limit:
        parts.append("... +%d" % (len(addrs) - limit))
    return ", ".join(parts) if parts else "-"


def write_function_file(f, names, insns, words, translator, callers, callees,
                        mmio, body, outdir, gaps, classstr, parent=None):
    a = f["addr"]
    path = os.path.join(outdir, "0x%08X.c" % a)
    lines = []
    body_addrs = body
    mmio_regs = sorted({x.get("reg", "?") for x in mmio})
    lines.append("/*")
    lines.append(" * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)")
    lines.append(" * ---------------------------------------------------------------------")
    lines.append(" * Address   : 0x%08X" % a)
    lines.append(" * Name      : %s" % names.get(a, "FUN_%08X" % a))
    lines.append(" * End       : 0x%08X" % f["end"])
    lines.append(" * Size      : %d bytes, %d instructions decoded in body "
                 "(functions.json insns=%d, blocks=%d)"
                 % (f["size"], len(body_addrs), f["insns"], len(f["blocks"])))
    lines.append(" * Class     : %s" % classstr)
    if parent is not None:
        lines.append(" * Parent    : 0x%08X %s  (this entry starts inside the parent's "
                     "range; treat it as a sub-block, not a separate function)"
                     % (parent, names.get(parent, "FUN_%08X" % parent)))
    lines.append(" * Callers   : %d  -> %s" % (len(callers), fmt_callers(callers, names)))
    lines.append(" * Callees   : %d  -> %s" % (len(callees), fmt_callers(callees, names)))
    if mmio_regs:
        lines.append(" * MMIO      : %s" % ", ".join(mmio_regs[:12]))
    lines.append(" * ---------------------------------------------------------------------")
    lines.append(" * This file is a mechanical translation of the Thumb-1 disassembly, not")
    lines.append(" * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;")
    lines.append(" * every statement is preceded by its original instruction comment.")
    lines.append(" */")
    lines.append("")
    lines.append("void %s(void)   /* args in r0-r3, return in r0 unless noted */"
                 % names.get(a, "FUN_%08X" % a))
    lines.append("{")
    if not body_addrs:
        lines.append("    /* no decoded instructions inside this range "
                     "(fragment / data or entry is &~1 of a neighbouring block) */")
    else:
        for kind, txt in translator.translate_function(body_addrs, f["end"]):
            if kind == "label":
                lines.append("")
                lines.append("    " + txt)
            elif kind == "comment":
                lines.append("    " + txt)
            else:
                lines.append("    " + txt)
    # literal words inside [addr, end) that are data
    for wa in sorted(words):
        if a <= wa < f["end"]:
            val, cmt = words[wa]
            lines.append("    /* data 0x%08X: .word 0x%08X %s */"
                         % (wa, val if val is not None else 0, cmt))
    lines.append("}")
    lines.append("")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines))
    return path


def write_index(funcs, names, callees, callers, mmio_by_func, insns, outdir, vectors):
    path = os.path.join(outdir, "index.tsv")
    rows = []
    for f in funcs:
        a = f["addr"]
        n = len(callees.get(a, ()))
        c = len(callers.get(a, ()))
        body = get_body(f, insns)
        mmio_regs = sorted({x.get("reg", "?") for x in mmio_by_func.get(a, [])})
        if a in vectors:
            cls = "vector_handler"
        elif any(g[0] <= a <= g[1] for g in GAPCACHE):
            cls = "inside_data_range"
        elif len(body) == 0:
            cls = "empty_fragment"
        elif len(body) <= 2:
            cls = "tiny_fragment"
        elif n >= 8 or c >= 8:
            cls = "hub"
        else:
            cls = "function"
        rows.append([a, f["size"], f["insns"], len(body), c, n,
                     suggested_name(a, names, callers.get(a, ()), callees.get(a, ()),
                                    [x.get("reg", "") for x in mmio_by_func.get(a, [])]),
                     cls, ";".join(mmio_regs[:6])])
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("addr\tsize\tinsns_json\tinsns_body\tcallers\tcallees\tname_suggestion\tclass\tmmio\n")
        for r in rows:
            fh.write("0x%08X\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\n" % tuple(r))
    return path, rows


def write_callgraph(funcs, names, callees, callers, tail_calls, vectors, outdir):
    path = os.path.join(outdir, "CALLGRAPH.md")
    lines = ["# AD100 Pro V1.2 call graph (from capstone call instructions)\n"]
    lines.append("- 调用边仅统计已解码指令里的 `bl/blx`（尾部 `b` 单独标记，不进主图）")
    lines.append("- 函数名：向量表处理器/CMSIS 名与已知关键函数用真名，其余为 FUN_<地址>")
    lines.append("- 由于 out/functions.json 存在重叠/碎片条目（见 ISSUES.md），"
                 "同一段指令可能被多个条目统计，调用图仅作导航用。\n")
    total_edges = sum(len(v) for v in callees.values())
    total_tail = sum(len(v) for v in tail_calls.values())
    lines.append("## 统计\n")
    lines.append("| 指标 | 数量 |")
    lines.append("|---|---|")
    lines.append("| 函数条目 | %d |" % len(funcs))
    lines.append("| 调用边 (bl/blx) | %d |" % total_edges)
    lines.append("| 尾调用边 (b, 跨函数) | %d |" % total_tail)
    lines.append("| 无调用者的函数 | %d |" % sum(1 for f in funcs if not callers.get(f["addr"])))
    lines.append("")

    # startup chain
    lines.append("## 启动链（向量表 -> Reset -> __start -> init_array -> main）\n")
    lines.append("```")
    lines.append("0x08003000 vector[0]  = initial SP 0x20000CD0")
    lines.append("0x08003004 vector[1]  = Reset_Handler 0x080030D4")
    lines.append("   Reset_Handler      0x080030D4  -> SystemInit 0x0800B964")
    lines.append("                                    -> __start    0x080030C0 (tail bx)")
    lines.append("   __start            0x080030C0  -> __libc_init_array 0x08003548")
    lines.append("                                    -> FUN_0800CF74 (main, tail bx)")
    lines.append("```")
    lines.append("")
    # trees rooted at main and reset
    visited_global = set()

    def tree(root, depth, maxdepth, prefix):
        if depth > maxdepth:
            return
        cal = sorted(callees.get(root, ()))
        for i, c in enumerate(cal):
            if c in visited_global:
                continue
            visited_global.add(c)
            last = (i == len(cal) - 1)
            branch = "└── " if last else "├── "
            lines.append("%s%s0x%08X %s" % (prefix, branch, c, names.get(c, "FUN_%08X" % c)))
            tree(c, depth + 1, maxdepth, prefix + ("    " if last else "│   "))

    for label, root, md in (("main", 0x0800CF74, 4), ("Reset_Handler", 0x080030D4, 4),
                            ("dispatcher_large", 0x08008D00, 3)):
        if root not in names:
            continue
        lines.append("### 从 %s (0x%08X) 展开（深度 %d）\n" % (names.get(root, "?"), root, md))
        lines.append("```")
        lines.append("0x%08X %s" % (root, names.get(root, "FUN_%08X" % root)))
        visited_global = set([root])
        tree(root, 0, md, "")
        lines.append("```")
        lines.append("")
    # top called functions
    indeg = {a: len(callers.get(a, ())) for a in names}
    lines.append("## 被调用最多的函数（in-degree top 30）\n")
    lines.append("| # | 地址 | 名称 | 被调用次数 | 自身调用数 |")
    lines.append("|---|---|---|---|---|")
    for i, (a, n) in enumerate(sorted(indeg.items(), key=lambda kv: -kv[1])[:30], 1):
        lines.append("| %d | 0x%08X | %s | %d | %d |"
                     % (i, a, names.get(a, "FUN_%08X" % a), n, len(callees.get(a, ()))))
    lines.append("")
    # out-degree
    lines.append("## 调用其他函数最多的函数（out-degree top 20）\n")
    lines.append("| # | 地址 | 名称 | 调用数 |")
    lines.append("|---|---|---|---|")
    for i, (a, c) in enumerate(
            sorted(((a, len(v)) for a, v in callees.items()), key=lambda kv: -kv[1])[:20], 1):
        lines.append("| %d | 0x%08X | %s | %d |" % (i, a, names.get(a, "FUN_%08X" % a), c))
    lines.append("")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines))
    return path


GAPCACHE = []


def write_issues_auto(funcs, insns, words, sections, gaps, outdir):
    path = os.path.join(outdir, "issues_auto.md")
    lines = ["# 自动检测到的数据问题（供 ISSUES.md 引用）\n"]
    # overlapping ranges
    overlaps = []
    starts = [f["addr"] for f in funcs]
    for f in funcs:
        for g in funcs:
            if f is g:
                continue
            if g["addr"] > f["addr"] and g["addr"] < f["end"]:
                overlaps.append((f["addr"], g["addr"]))
    lines.append("## 1. 函数范围重叠\n")
    lines.append("`g.addr` 落在 `f.addr..f.end` 内部的条目共 **%d** 对。" % len(overlaps))
    lines.append("前 20 对（外层函数 -> 内层“函数”）：\n")
    lines.append("| 外层 | 外层结束 | 内层 |")
    lines.append("|---|---|---|")
    fend = {f["addr"]: f["end"] for f in funcs}
    for a, b in overlaps[:20]:
        lines.append("| 0x%08X | 0x%08X | 0x%08X |" % (a, fend[a], b))
    lines.append("")
    # empty bodies
    empties = [f for f in funcs if not get_body(f, insns)]
    lines.append("## 2. 范围内没有已解码指令的“函数”条目\n")
    lines.append("共 **%d** 个（多为 end==addr 的碎片条目）。前 20 个：\n" % len(empties))
    lines.append("| 地址 | end | size | insns(functions.json) |")
    lines.append("|---|---|---|---|")
    for f in empties[:20]:
        lines.append("| 0x%08X | 0x%08X | %d | %d |"
                     % (f["addr"], f["end"], f["size"], f["insns"]))
    lines.append("")
    # tiny
    tiny = [f for f in funcs if 0 < f["size"] <= 4]
    lines.append("## 3. 极小条目（size <= 4 字节）\n")
    lines.append("共 **%d** 个，占全部 %d 条的 %.1f%%。这些大多是跳转目标/尾调用桩，"
                 "不一定是独立函数。\n" % (len(tiny), len(funcs), 100.0 * len(tiny) / len(funcs)))
    lines.append("")
    # sections anomalies
    bad_sec = []
    for a, s in sections.items():
        body = [x for x in range(a, s["end"]) if x in insns]
        if len(body) != s["insns"]:
            bad_sec.append((a, s["insns"], len(body), s["end"]))
    lines.append("## 4. disasm.txt 函数头声明与实际打印指令数不一致\n")
    lines.append("共 **%d** 个 section（disasm 的线性打印按全局指令流走，"
                 "函数头部 insns/end 与打印区间不一定对应）。前 20 个：\n" % len(bad_sec))
    lines.append("| 函数 | 头部声明 insns | 范围内实际已解码 | end |")
    lines.append("|---|---|---|---|")
    for a, n1, n2, e in bad_sec[:20]:
        lines.append("| 0x%08X | %d | %d | 0x%08X |" % (a, n1, n2, e))
    lines.append("")
    # functions inside gaps
    in_gap = [f for f in funcs if any(s <= f["addr"] <= e for s, e in gaps)]
    lines.append("## 5. 起始地址落在未认领数据区（gaps.txt）里的函数\n")
    lines.append("共 **%d** 个（0 表示函数起点都落在已解码代码区）。" % len(in_gap))
    lines.append("")
    # functions size vs insns mismatch
    bad = []
    for f in funcs:
        body = get_body(f, insns)
        if len(body) != f["insns"]:
            bad.append((f["addr"], f["insns"], len(body), f["size"]))
    lines.append("## 6. functions.json 内部 insns 与 [addr,end) 已解码指令数不一致\n")
    lines.append("共 **%d** 个（insns 由递归探索累计，可能重复计数）。前 20 个：\n" % len(bad))
    lines.append("| 地址 | insns(json) | 范围内已解码 | size |")
    lines.append("|---|---|---|---|")
    for a, n1, n2, sz in bad[:20]:
        lines.append("| 0x%08X | %d | %d | %d |" % (a, n1, n2, sz))
    lines.append("")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines))
    return path


def write_selfcheck(funcs, insns, outdir, seed=42):
    """Pick 5 random functions, verify generated C line addresses vs disasm.txt."""
    path = os.path.join(outdir, "SELFCHECK.md")
    rnd = random.Random(seed)
    candidates = [f for f in funcs if len(get_body(f, insns)) >= 5]
    sample = rnd.sample(candidates, 5)
    lines = ["# 自检：随机抽取 5 个函数，核对伪 C 与 disasm.txt\n"]
    lines.append("抽样方法：固定随机种子，只在 body 指令数 >= 5 的函数条目中抽取；")
    lines.append("核对内容：文件里每条指令的 `/* 0x地址: 机器码 助记符 */` 注释必须与")
    lines.append("`out/disasm.txt` 中同一地址的原始行一致（地址、机器码、助记符、操作数）。\n")
    ok = 0
    for f in sample:
        a = f["addr"]
        cfile = os.path.join(outdir, "0x%08X.c" % a)
        if not os.path.exists(cfile):
            lines.append("- 0x%08X: **文件缺失**" % a)
            continue
        with open(cfile, "r", encoding="utf-8") as fh:
            text = fh.read()
        body = sorted(get_body(f, insns))
        bad = []
        for x in body:
            insn = insns[x]
            need = "0x%08X: %s" % (x, insn["bytes"])
            if need not in text:
                bad.append((x, "bytes"))
        # also check the first and last instruction's raw bytes/mnemonic
        first, last = insns[body[0]], insns[body[-1]]
        first_ok = ("0x%08X: %-8s %-8s %s" % (body[0], first["bytes"], first["mnem"], first["ops"])) in text
        last_ok = ("0x%08X: %-8s %-8s %s" % (body[-1], last["bytes"], last["mnem"], last["ops"])) in text
        passed = (not bad) and first_ok and last_ok
        ok += 1 if passed else 0
        lines.append("## 0x%08X %s  (body %d 条指令)\n"
                     % (a, "PASS" if passed else "FAIL", len(body)))
        lines.append("- 首条: `0x%08X  %s %s %s` %s"
                     % (body[0], first["bytes"], first["mnem"], first["ops"],
                        "一致" if first_ok else "**不一致**"))
        lines.append("- 末条: `0x%08X  %s %s %s` %s"
                     % (body[-1], last["bytes"], last["mnem"], last["ops"],
                        "一致" if last_ok else "**不一致**"))
        lines.append("- 全部 %d 条指令的 地址+机器码 在伪 C 中逐条可查，缺失: %d"
                     % (len(body), len(bad)))
        lines.append("")
    lines.append("**结论: %d/5 通过。**" % ok)
    lines.append("")
    with open(path, "w", encoding="utf-8", newline="\n") as fh:
        fh.write("\n".join(lines))
    return ok, [f["addr"] for f in sample]


# ----------------------------------------------------------------------------
def main():
    os.makedirs(OUT, exist_ok=True)
    data = load_bin()
    insns, words, sections = load_disasm()
    funcs = load_functions()
    meta = load_meta()
    periph = load_periph()
    gaps = load_gaps()
    GAPCACHE.extend(gaps)
    vectors = build_vectors(data)
    names = build_names(funcs, vectors)
    callees, callers, tail_calls = build_callgraph(funcs, insns, names)
    # merge Lead's meta callee info (only when we have no decoded edge)
    for f in funcs:
        a = f["addr"]
        for c in meta.get(a, {}).get("callees", []):
            if c in names and c != a:
                callees[a].add(c)
                callers.setdefault(c, set()).add(a)
    mmio_by_func = defaultdict(list)
    for at, hits in periph.items():
        for h in hits:
            try:
                fa = int(h.get("func", "0x0"), 16)
            except ValueError:
                continue
            mmio_by_func[fa].append(h)

    tr = Translator(data, insns, words, periph, names)

    # per function classification for ISSUES/name suggestions
    fmap = {f["addr"]: f for f in funcs}
    class_stats = Counter()
    n_files = 0
    for f in funcs:
        a = f["addr"]
        body = get_body(f, insns)
        inside = [g for g in funcs
                  if g["addr"] < a < g["end"]]
        if a in vectors:
            cls = "vector_handler"
        elif any(s <= a <= e for s, e in gaps):
            cls = "inside_data_range"
        elif not body:
            cls = "empty_fragment"
        elif len(body) <= 2:
            cls = "tiny_fragment"
        elif len(callees.get(a, ())) >= 8 or len(callers.get(a, ())) >= 8:
            cls = "hub"
        elif inside:
            cls = "nested_block"
        else:
            cls = "function"
        class_stats[cls] += 1
        parent = None
        if inside:
            parent = min(inside, key=lambda g: g["size"])["addr"]
        write_function_file(f, names, insns, words, tr, callers.get(a, set()),
                            callees.get(a, set()), mmio_by_func.get(a, []),
                            body, OUT, gaps, cls, parent)
        n_files += 1

    index_path, rows = write_index(funcs, names, callees, callers, mmio_by_func,
                                    insns, OUT, vectors)
    cg_path = write_callgraph(funcs, names, callees, callers, tail_calls, vectors, OUT)
    issues_path = write_issues_auto(funcs, insns, words, sections, gaps, OUT)
    ok, sample = write_selfcheck(funcs, insns, OUT)

    # summary
    print("=" * 70)
    print("pseudo-decompile complete")
    print("  functions             :", len(funcs))
    print("  .c files written      :", n_files, "->", OUT)
    print("  decoded instructions  :", len(insns))
    print("  call edges (bl/blx)   :", sum(len(v) for v in callees.values()))
    print("  tail-call edges       :", sum(len(v) for v in tail_calls.values()))
    print("  class stats           :", dict(class_stats))
    print("  index                 :", index_path)
    print("  callgraph             :", cg_path)
    print("  auto issues           :", issues_path)
    print("  selfcheck             :", ok, "/5", [hex(x) for x in sample])
    print("=" * 70)


if __name__ == "__main__":
    main()
