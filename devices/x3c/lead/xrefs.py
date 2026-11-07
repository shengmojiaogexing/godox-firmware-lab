"""Find functions that reference given peripheral base addresses (literal pools),
and the callers of those functions. Uses capstone-based function discovery.
Usage: python xrefs.py 0x40013000 [0x40012000 ...]
"""
import os
import re
import struct
import sys
from collections import defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
IMAGE = os.path.join(HERE, "..", "firmware", "X3_C_v1.38_app.bin")
BASE = 0x08004800

from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB  # noqa: E402

data = open(IMAGE, "rb").read()
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)
md.detail = True

# ---- discover function starts: vector entries + BL targets + push-prologue scan
funcs = set()


def add(v):
    v &= ~1
    if BASE <= v < BASE + len(data):
        funcs.add(v)


# vector table
nvec = 131
for i in range(1, nvec):
    add(struct.unpack_from("<I", data, 4 * i)[0])

# scan all thumb instructions: collect BL/BLX targets (linear sweep; good enough)
insn_targets = defaultdict(set)   # function-start (approx) -> callers
addr = BASE
end = BASE + len(data)
# build list of code regions from vector entries and known code clusters
for f in sorted(funcs):
    off = f - BASE
    code = list(md.disasm(data[off:off + 0x4000], f))
    for ins in code:
        if ins.mnemonic in ("bl", "blx") and ins.operands and ins.operands[0].type == 2:
            t = ins.operands[0].imm
            add(t)
            insn_targets[t & ~1].add(ins.address & ~1)
        elif ins.mnemonic in ("b", "b.w"):
            pass

# add function starts by scanning for push prologues at 2-byte alignment
for off in range(0, len(data) - 4, 2):
    hw = struct.unpack_from("<H", data, off)[0]
    if hw in (0xB510, 0xB570, 0xB5F0, 0xB580, 0xB5F8, 0xB530):
        funcs.add(BASE + off)
    elif hw == 0xE92D:
        hw2 = struct.unpack_from("<H", data, off + 2)[0]
        if hw2 & 0x4000:      # push with lr
            funcs.add(BASE + off)

funcs = sorted(funcs)

# literal pools: ldr rX, [pc, #imm] -> target word
def literals_in(fstart, max_bytes=0x600):
    off = fstart - BASE
    out = []
    for ins in md.disasm(data[off:off + max_bytes], fstart):
        if ins.mnemonic.startswith("ldr") and "[pc" in ins.op_str:
            m = re.search(r"\[pc, #(-?0x[0-9a-f]+|-?\d+)\]", ins.op_str)
            if not m:
                continue
            imm = int(m.group(1), 0)
            pc = (ins.address + 4) & ~3
            tgt = pc + imm
            if BASE <= tgt < BASE + len(data) - 4:
                val = struct.unpack_from("<I", data, tgt - BASE)[0]
                out.append((ins.address, tgt, val))
    return out


# ownership: which function contains address a (last start <= a)
import bisect
def owner(a):
    i = bisect.bisect_right(funcs, a) - 1
    if i >= 0 and a - funcs[i] < 0x2000:
        return funcs[i]
    return None

want = set()
crit = []
for arg in sys.argv[1:]:
    v = int(arg, 0)
    crit.append(v)
    for f in funcs:
        for insaddr, laddr, val in literals_in(f):
            if val == v or (v <= val < v + 0x400 and v % 0x400 == 0 and v in crit):
                want.add(f)
                break

# collect callers of those functions
callers = defaultdict(set)
for f in funcs:
    off = f - BASE
    for ins in md.disasm(data[off:off + 0x800], f):
        if ins.mnemonic in ("bl", "blx") and ins.operands and ins.operands[0].type == 2:
            t = ins.operands[0].imm & ~1
            callers[t].add(f)

for v in crit:
    print("=== functions referencing 0x%08x ===" % v)
    rows = []
    for f in funcs:
        hits = [x for x in literals_in(f) if x[2] == v or (0x40000000 <= x[2] < 0x60000000 and (x[2] & 0xFFFFFC00) == (v & 0xFFFFFC00))]
        if hits:
            rows.append((f, hits, sorted(callers.get(f, []))[:8]))
    for f, hits, cls in rows[:60]:
        print("  func 0x%08x  lits=%s  callers=%s" % (
            f, ",".join("0x%08x@0x%x" % (h[2], h[0]) for h in hits[:4]),
            ",".join("0x%08x" % c for c in cls)))
    print()
