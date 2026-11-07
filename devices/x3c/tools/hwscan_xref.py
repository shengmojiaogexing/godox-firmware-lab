#!/usr/bin/env python
"""hwscan_xref.py - literal xref scan with base 0x08004800."""
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_strings import extract

APP_BASE = 0x08004800
b = load()
n = len(b)

def word_of(off):
    return struct.unpack_from('<I', b, off)[0]

# all aligned flash-range literals -> list of storage offsets
flash_refs = {}
for off in range(0, n - 3, 4):
    w = word_of(off)
    if APP_BASE <= w < APP_BASE + n:
        flash_refs.setdefault(w, []).append(off)

strs = {off: s for off, kind, s in extract(b) if kind == 'ascii' and len(s) >= 4}
str_offsets = set(strs)
print(f"{len(flash_refs)} distinct flash pointers, {len(strs)} ascii strings")

refs_to_strings = []
for w, storage in flash_refs.items():
    fo = w - APP_BASE
    if fo in str_offsets:
        refs_to_strings.append((w, fo, strs[fo], storage))
print(f"{len(refs_to_strings)} literals point exactly at an ASCII string start")
for w, fo, s, storage in refs_to_strings[:60]:
    print(f"  {w:#010x} -> file {fo:#08x} {s[:60]!r}  stored_at={[hex(x) for x in storage[:4]]}")

print()
print("=== direct check known strings")
for target in [0x08007350, 0x0800739c, 0x080241c0, 0x080220af, 0x08022047]:
    print(f"  {target:#010x}: {len(flash_refs.get(target,[]))} refs -> {[hex(x) for x in flash_refs.get(target,[])[:8]]}")
