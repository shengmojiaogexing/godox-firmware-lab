import struct, sys, re

p = sys.argv[1] if len(sys.argv) > 1 else r'keypatch\dev\g3\installer\Godox_V2.0_setup.exe'
d = open(p, 'rb').read()
print('size', len(d))
print('MZ', d[:2])
pe = struct.unpack_from('<I', d, 0x3c)[0]
print('e_lfanew', hex(pe), d[pe:pe+4])
mach, nsec = struct.unpack_from('<HH', d, pe + 4)
print('machine', hex(mach), 'nsec', nsec)
opt = pe + 24
magic = struct.unpack_from('<H', d, opt)[0]
print('optmagic', hex(magic))
nrva = struct.unpack_from('<I', d, opt + 92)[0] if magic == 0x10b else struct.unpack_from('<I', d, opt + 108)[0]
print('nrva', nrva)
sec = opt + (224 if magic == 0x10b else 240)
for i in range(nsec):
    o = sec + i * 40
    name = d[o:o + 8].split(b'\0')[0].decode('latin1')
    vsz, va, rsz, ra = struct.unpack_from('<IIII', d, o + 8)
    ch = struct.unpack_from('<I', d, o + 36)[0]
    print(f'{name:10} va={va:08x} vsz={vsz:08x} rsz={rsz:08x} raw={ra:08x} ch={ch:08x}')

for pat in [b'Inno Setup', b'Inno', b'JR.Inno', b'nsis', b'Nullsoft', b'7-Zip', b'Delphi', b'Setup Loader', b'MZ']:
    idxs = [m.start() for m in re.finditer(re.escape(pat), d)][:8]
    print(pat, idxs)
