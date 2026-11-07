"""Extract plain/zlib content from the Qt resource area of the G3 exe.

Strategy: the QML resource data is in .rdata; some resources are zlib compressed.
Scan for zlib streams and decompress; also dump any uncompressed QML-looking text.
"""
import zlib, re, sys, os

p = r'C:\Users\22435\AppData\Local\Temp\g3extract\app\Godox V2.0.exe'
outdir = r'keypatch\dev\g3\qml'
os.makedirs(outdir, exist_ok=True)
d = open(p, 'rb').read()

# zlib streams: try every 0x78 ?? header in the .rdata range and beyond
found = []
for i in range(0x33600, len(d) - 2):
    b0, b1 = d[i], d[i + 1]
    if b0 == 0x78 and b1 in (0x01, 0x5e, 0x9c, 0xda):
        try:
            o = zlib.decompressobj().decompress(d[i:i + 2_000_000])
        except Exception:
            continue
        if len(o) > 100:
            found.append((i, len(o), o))

print('zlib streams:', len(found))
names = set()
for i, n, o in found:
    # try to find a filename in the content (Qt resource files may start with comments)
    m = re.match(rb'\s*(?://\s*)?([A-Za-z0-9_./-]+\.(?:qml|js|json|xml|png|txt))', o[:120])
    nm = m.group(1).decode() if m else None
    base = (nm.replace('/', '_') if nm else f'res_{i:06x}')
    # disambiguate
    k = 1
    stem, ext = os.path.splitext(base)
    while base in names:
        base = f'{stem}_{k}{ext}'
        k += 1
    names.add(base)
    open(os.path.join(outdir, base), 'wb').write(o)
    print(f'  off=0x{i:06x} len={n} name={nm} -> {base}')
    print('    head:', o[:100])

# also scan for uncompressed qml blocks (text starting with // or import)
print('\n=== uncompressed QML-ish blocks ===')
txt = d.decode('latin1')
for m in re.finditer(r'(?:(?://[^\n]{0,80}\n)?\s*import QtQuick[^\n]{0,80}\n)', txt):
    i = m.start()
    # extend to end: take 60KB or until run of binary
    blk = txt[i:i + 60000]
    print(f'  off=0x{i:06x}: {blk[:150]!r}')
