import re, sys, json
import pefile

p = sys.argv[1] if len(sys.argv) > 1 else r'C:\Users\22435\AppData\Local\Temp\g3extract\app\Godox V2.0.exe'
outp = sys.argv[2] if len(sys.argv) > 2 else r'keypatch\dev\g3'
d = open(p, 'rb').read()
pe = pefile.PE(p, fast_load=True)
secs = []
for s in pe.sections:
    secs.append((s.Name.decode(errors='replace').rstrip('\0'), s.VirtualAddress,
                 s.PointerToRawData, s.SizeOfRawData, s.Misc_VirtualSize))
ib = pe.OPTIONAL_HEADER.ImageBase

def off2va(off):
    for name, va, raw, rsz, vsz in secs:
        if raw <= off < raw + rsz:
            return name, ib + va + (off - raw)
    return None, None

# ASCII / UTF-8-ish strings
asc = []
for m in re.finditer(rb'[\x20-\x7e]{4,}', d):
    sec, va = off2va(m.start())
    asc.append((m.start(), va, sec, m.group().decode('latin1')))
# UTF-16LE strings
u16 = []
for m in re.finditer(rb'(?:[\x20-\x7e]\x00){4,}', d):
    sec, va = off2va(m.start())
    u16.append((m.start(), va, sec, m.group().decode('utf-16le')))

with open(outp + r'\strings_ascii.txt', 'w', encoding='utf-8') as f:
    for off, va, sec, s in asc:
        f.write(f'{off:08x} {va if va is None else format(va,"08x")} {sec}\t{s}\n')
with open(outp + r'\strings_utf16.txt', 'w', encoding='utf-8') as f:
    for off, va, sec, s in u16:
        f.write(f'{off:08x} {va if va is None else format(va,"08x")} {sec}\t{s}\n')

print('ascii strings', len(asc), 'utf16 strings', len(u16))

kw = ['AD100', 'AD200', 'AD300', 'X3C', 'X3', 'Pro', 'device', 'Device', 'DEVICE', 'VID', 'PID',
      'vid', 'pid', 'hid', 'HID', 'CRC', 'crc', 'Check', 'check', 'checksum', 'Checksum', 'md5', 'MD5',
      'sha', 'SHA', 'hash', 'Hash', 'size', 'Size', 'SIZE', 'version', 'Version', 'force', 'Force',
      'upgrade', 'Upgrade', 'update', 'Update', '.bin', '.zip', '.json', 'firmware', 'Firmware',
      'boot', 'Boot', 'iap', 'IAP', 'USB', 'usb', 'IAP', 'Godox', 'godox', 'model', 'Model',
      'Mismatch', 'mismatch', 'not match', 'Open file', 'File size', 'Please', 'Connection', 'packet',
      'Packet', 'write', 'Write', 'read', 'Read', 'erase', 'Erase', 'flash', 'Flash', 'address', 'Address']
print('=== keyword hits (ascii) ===')
seen = set()
for off, va, sec, s in asc:
    for k in kw:
        if k in s:
            key = (s,)
            if key in seen:
                break
            seen.add(key)
            print(f'{off:08x} va={va if va is None else format(va,"08x")} {sec:9} {s[:220]}')
            break
print('=== keyword hits (utf16) ===')
seen = set()
for off, va, sec, s in u16:
    for k in kw:
        if k in s:
            if s in seen:
                break
            seen.add(s)
            print(f'{off:08x} va={va if va is None else format(va,"08x")} {sec:9} {s[:220]}')
            break
