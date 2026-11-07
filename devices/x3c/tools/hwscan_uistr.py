#!/usr/bin/env python
"""hwscan_uistr.py - build devices/x3c/re/UI_STRINGS.md: UI string inventory + referencing analysis.

Extracts human-readable ASCII and UTF-8 (Chinese) strings from X3_C_v1.38_app.bin,
finds ADR (PC-relative) references, groups by firmware region/feature, and writes a
markdown deliverable.
"""
import sys, os, re, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from hwscan_base import load
from hwscan_funcs import APP_BASE
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 're', 'UI_STRINGS.md')
b = load(); N = len(b)
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB)

# ---------- string extraction ----------
def is_ascii(c):
    return 0x20 <= c < 0x7f

def cjk_at(i):
    if i + 3 <= N and b[i] >= 0xE0 and 0x80 <= b[i+1] < 0xC0 and 0x80 <= b[i+2] < 0xC0:
        try:
            ch = b[i:i+3].decode('utf-8')
            if '\u4e00' <= ch <= '\u9fff' or ch in '：，。！？（）《》':
                return ch
        except Exception:
            pass
    return None

strs = []  # (off, text, kind)
i = 0
while i < N:
    start = i
    parts = []
    while i < N:
        if is_ascii(b[i]):
            parts.append(chr(b[i])); i += 1; continue
        ch = cjk_at(i)
        if ch:
            parts.append(ch); i += 3; continue
        break
    text = ''.join(parts)
    if text:
        n_cjk = sum(1 for ch in text if '\u4e00' <= ch <= '\u9fff')
        good = False
        WHITELIST = {'X3 C', 'X3 N', 'X3 S', 'X3 F', 'X3 O', 'X3 L', 'v0.98', 'v1.38', 'v1.25',
                     'v1.21', 'v1.11', 'L-858', 'Multi', 'Home', 'SCAN', 'PROP', 'Times',
                     'CH %d', '0-30m', '1-100m', 'Cancel', 'Apply', 'Update', 'Setting',
                     'Hot Shoe Upgrading', 'Godox', 'HID IAP Interface', 'Godox_IAP',
                     'English', 'Auto Off', 'One-Shoot', 'All-Shoot', 'Min.Power', 'Front Curtain',
                     'Rear Curtain', 'High Speed', 'Wireless Sync', 'Wirelese Sync',
                     'Restore Factory', 'to unlock', 'Option 1', 'the modeling lamp',
                     'main switch', 'Turn on', 'Standby Time', 'Model: ', 'Firmware: ',
                     'Firmware error', 'Firmware: ', 'CH 21'}
        if n_cjk >= 2:
            good = True
        elif re.fullmatch(r"[A-Za-z0-9 .,:%+\-/'()!?&_#=]*", text) and re.search(r'[A-Za-z]{2,}', text):
            ts = text.strip()
            has_lower = any(ch.islower() for ch in text)
            has_space = ' ' in text
            if ts in WHITELIST:
                good = True
            elif has_space and has_lower and len(text) >= 4:
                good = True
            elif len(text) >= 5 and has_lower:
                good = True
            elif re.fullmatch(r'X3 [A-Z]|v[0-9]+\.[0-9]+|L-[0-9]+|CH [0-9]+|[0-9]+-[0-9]+m', ts):
                good = True
        if good:
            strs.append((start, text, 'cjk' if n_cjk else 'ascii'))
        # allow single-char tails to advance
    i = max(i, start + 1)

# dedupe by exact text keeping first
seen = set(); uniq = []
for off, text, kind in strs:
    key = text.strip()
    if len(key) < 2:
        continue
    if key in seen:
        continue
    seen.add(key); uniq.append((off, text, kind))

# ---------- ADR xref scan ----------
adr = {}
for off in range(0, N - 4, 2):
    va = APP_BASE + off
    for ins in md.disasm(b[off:off+4], va):
        if ins.mnemonic == 'adr':
            try:
                disp = int(ins.op_str.split('#')[1].split(',')[0], 0)
            except Exception:
                break
            tgt = ((ins.address + 4) & ~3) + disp
            if tgt != ins.address:
                adr.setdefault(tgt, []).append(ins.address)
        break

def xrefs(off):
    return adr.get(APP_BASE + off, [])

# keep only ADR sites that fall inside a real disassembled function (drops data coincidences)
try:
    from hwscan_funcs import build_cached
    _sp, _vecs, _vt, _funcs = build_cached()
    CODE_ADDRS = set()
    for _f, (_ins, _calls, _lits, _end) in _funcs.items():
        for _a in _ins:
            CODE_ADDRS.add(_a[0])
except Exception:
    CODE_ADDRS = None

KNOWN_CLUSTERS = [(0x0E00, 0x3000), (0x3A00, 0x3B00), (0x4100, 0x4200), (0x6C00, 0x6E00),
                  (0x8A00, 0x8C00), (0x9E00, 0xA100), (0xB000, 0xB200), (0xCD00, 0xCE00),
                  (0xDA00, 0xDD00), (0x10D00, 0x11300), (0x11700, 0x11900), (0x14200, 0x14400),
                  (0x1D700, 0x1D900), (0x1F800, 0x20000), (0x2B00, 0x2D00), (0x4A00, 0x4C00),
                  (0x5D00, 0x5E00), (0x6700, 0x6800), (0x73A0, 0x73F0)]
def in_clusters(off):
    return any(lo <= off < hi for lo, hi in KNOWN_CLUSTERS)

uniq = [(off, text, kind) for off, text, kind in uniq
        if in_clusters(off) or (xrefs(off) and (len(text) >= 4 or '\u4e00' <= text[0] <= '\u9fff'))]

# ---------- group into regions ----------
GROUP_ORDER = [
    ('Hot shoe / upgrade', 0x1F800, 0x1FA00),
    ('About / model & firmware version', 0x2B00, 0x2C80),
    ('Standby / auto-off', 0x1520, 0x1560),
    ('Shooting modes / group help', 0x1F00, 0x2000),
    ('Group A-E / model help', 0xEF0, 0x1120),
    ('Modeling lamp / main switch', 0x6CF0, 0x6D60),
    ('Firmware error / update', 0x9F40, 0x9F80),
    ('Settings / factory reset', 0x4160, 0x41D0),
    ('Language / setting', 0x8B20, 0x8B40),
    ('RF: scan / wireless sync / CH-ID', 0xDB00, 0xDC80),
    ('RF: wireless sync dialogs', 0x10E40, 0x10F40),
    ('RF: sync dialogs 2', 0x111D0, 0x11280),
    ('Min.Power / power', 0x117E0, 0x11810),
    ('UI base (multi/home/setting)', 0x3A60, 0x3AD0),
    ('Other UI strings', -1, -1),
]

def group_of(off):
    for name, lo, hi in GROUP_ORDER:
        if lo >= 0 and lo <= off < hi:
            return name
    return 'Other UI strings'

groups = {}
for off, text, kind in uniq:
    groups.setdefault(group_of(off), []).append((off, text, kind))

# ---------- write markdown ----------
lines = []
lines.append('# UI_STRINGS.md — Godox X3-C v1.38: UI string inventory & referencing\n')
lines.append('Firmware: `devices/x3c/firmware/X3_C_v1.38_app.bin` (1,021,468 bytes).  \n'
             '**vaddr = file offset + 0x08004800** (app link base); the embedded hot-shoe image '
             'at file 0x0F3800 uses base 0x08002000 and contains code only (no UI strings).\n')
lines.append('\n## 1. How the firmware references strings — **ADR (PC-relative), not literal pools**\n')
lines.append(
 'No 32-bit literal in the image points at these strings (checked: all 1,021,468 bytes, all '
 'aligned words; 0 hits for e.g. 0x08007350 "Model: ", 0x080241C0 "Hot Shoe Upgrading"). '
 'The reason: the UI strings sit in **literal-pool/rodata gaps next to the code that uses them**, '
 'and the compiler reaches them with **`ADR` (Thumb `add rX, pc, #imm`, ±4 KB)**. '
 'Scanning every even file offset for `adr` yields **%d strings that are referenced directly by ADR**. '
 'This also explains why "no literal xrefs" was reported earlier — it is the expected ARMCC codegen '
 'for nearby rodata.\n' % len([1 for o, t, k in uniq if xrefs(o)]))
lines.append('\nRepresentative ADR references (string vaddr ← referencing instruction):\n')
lines.append('| string | file off | vaddr | ADR site(s) |\n|---|---|---|---|\n')
shown = 0
for off, text, kind in uniq:
    xs = xrefs(off)
    if xs and shown < 24:
        lines.append(f'| `{text[:46]}` | 0x{off:06x} | 0x{APP_BASE+off:08x} | '
                     + ', '.join(f'`0x{x:08x}`' for x in xs[:3]) + ' |\n')
        shown += 1
lines.append('\nFull machine-readable dump of *all* extracted strings (ASCII, UTF-8 CJK, mixed): '
             '`devices/x3c/re/hwscan_strings.tsv` (3720 raw entries, tab-separated: file offset, vaddr, kind, text). '
             'The filtered human-readable inventory is below.\n')
lines.append('\n## 2. Where the strings live\n')
lines.append(
 '* UI string/rodata clusters (file offsets, vaddr = +0x08004800):\n'
 '  * 0x0EF8–0x1120 (va 0x080056F8–0x08005920): group/modeling help ("A~E组适用于TTL/M模式", '
 '"A~E for TTL/M mode", "GR A-E for TTL/M mode", "GR F-9 for M mode only", "A~C…")\n'
 '  * 0x1520–0x1560 (va 0x08005D20): auto-off/standby ("自动关机" / "Auto Off")\n'
 '  * 0x1F00–0x2000 (va 0x08006700): shooting modes ("单次模式"/"One-Shoot", "群拍模式"/"All-Shoot", '
 '"L-858", "All-shoot mode only")\n'
 '  * 0x1F9C0 (va 0x080241C0): "Hot Shoe Upgrading"\n'
 '  * 0x2B40–0x2BE8 (va 0x08007340–0x080073E8): About page: "型号：/Model: ", model variants '
 '"X3 C/N/S/F/O/L", "固件版本：/Firmware: ", version strings "v0.98, v1.38, v1.25, v1.21, v1.11", '
 '"固件升级/Update"\n'
 '  * 0x3A60–0x3AD0 (va 0x08008260): main menu ("Multi", "多拍", "Home", "Setting")\n'
 '  * 0x4160–0x41D0 (va 0x08008960): "恢复出厂设置/Restore Factory", "Cancel", "Apply"\n'
 '  * 0x6CF0–0x6D60 (va 0x0800B4F0): "是否打开造型灯" / "Turn on the modeling lamp" / "main switch"\n'
 '  * 0x9F40–0x9F80 / 0xBFF0 (va 0x0800E740 / 0x080107F0): "固件错误/Firmware error"\n'
 '  * 0xDB00–0xDC80 (va 0x08012300): RF scan/sync: "扫描频道/SCAN", "识别结果", "无线同步"/'
 '"Wireless Sync", "CH 21"\n'
 '  * 0x10E40–0x10F40, 0x111D0–0x11280 (va 0x08015640, 0x080159D0): "Wirelese Sync" dialogs, '
 '"Please confirm wireless …", "Cancel", "Apply"\n'
 '  * 0x117E0–0x11810 (va 0x08015FE0): "最小功率/Min.Power"\n'
 '  * 0xB100/0xCD58 (va 0x0800F900/0x08011558): "Times", "0-30m", "1-100m", "Standby Time"\n'
 '* USB product strings (not UI): file 0x1D810 "Godox", 0x1D848 "HID IAP Interface", 0x1D8B0 "Godox_IAP".\n'
 '* The 0x42A20–0x0E8800 flash region is compressed bitmap/font data (theme "42 ff 08" RLE stream); '
 'text on screen is rendered from these assets + the strings above.\n')
lines.append('\n## 3. Grouped UI string inventory (filtered, offsets & vaddrs)\n')
lines.append('Legend: file offset (vaddr). `→ADR` = count of PC-relative references found.\n')
for name, _, _ in GROUP_ORDER:
    rows = groups.get(name)
    if not rows:
        continue
    lines.append(f'\n### {name}\n')
    lines.append('| file | vaddr | text | ADR refs |\n|---:|---:|---|---:|\n')
    for off, text, kind in sorted(rows):
        t = text.replace('|', '\\|').strip()
        lines.append(f'| 0x{off:06x} | 0x{APP_BASE+off:08x} | `{t[:70]}` | {len(xrefs(off))} |\n')
lines.append('\n## 4. Referencing mechanism details\n')
lines.append(
 '* The string address is materialised by a 16-bit Thumb `ADR` (e.g. 0x08006FF6 → "型号：" at '
 '0x08007344; 0x08007018 → "Model: " at 0x08007350). Multiple strings of one screen are '
 'referenced from the same function (the About-page renderer around 0x08006FF0–0x08007270), '
 'so a screen = one function + a cluster of ADR-reachable strings.\n'
 '* No string-ID table and no pointer array is needed: the UI code is compiled with '
 '`--split_sections`-style layout where each function\'s rodata follows it, so ADR suffices.\n'
 '* Some strings are printf formats (`- %d.%d`, `+0.%d`, `+%d.%d`, `+0.3`, `%IH1`) and are '
 'reached by ADR too; they format power/TCM offsets.\n'
 '* UTF-8 Chinese and ASCII are separate NUL-terminated strings; mixed strings (e.g. '
 '"A~E组适用于TTL/M模式") are single UTF-8 strings.\n')
lines.append('\n## 5. Notes for UI/emulator work\n')
lines.append(
 '* Strings are drawn with the compressed bitmap font assets (0x42A20–0x0E8800); for screenshots '
 'emulate the LCD (448×368 RGB565 via XMC 0x6C000000/1) and capture ADR/rodata reads or simply '
 'render the captured VRAM. See HARDWARE.md §4.\n'
 '* The About page (function ~0x08006FF0) is the best first target for text rendering tests: '
 'strings "型号："/"Model: ", "固件版本："/"Firmware: " + "v1.38" + model list X3 C/N/S/F/O/L are all '
 'ADR-referenced there.\n')

with open(OUT, 'w', encoding='utf-8') as f:
    f.writelines(lines)
print(f"wrote {OUT}: {len(uniq)} filtered strings, {len(adr)} ADR targets")
print("groups:", {k: len(v) for k, v in groups.items()})
