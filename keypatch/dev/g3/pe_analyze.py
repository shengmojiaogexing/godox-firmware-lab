import sys, re, pefile

p = sys.argv[1] if len(sys.argv) > 1 else r'C:\Users\22435\AppData\Local\Temp\g3extract\app\Godox V2.0.exe'
pe = pefile.PE(p, fast_load=False)
print('=== HEADER ===')
print('machine', hex(pe.FILE_HEADER.Machine), 'time', pe.FILE_HEADER.TimeDateStamp, 'subsys', pe.OPTIONAL_HEADER.Subsystem)
print('entry', hex(pe.OPTIONAL_HEADER.AddressOfEntryPoint), 'imagebase', hex(pe.OPTIONAL_HEADER.ImageBase))
print('dll', pe.OPTIONAL_HEADER.DllCharacteristics, 'linker', pe.OPTIONAL_HEADER.MajorLinkerVersion, pe.OPTIONAL_HEADER.MinorLinkerVersion)
print('=== SECTIONS ===')
for s in pe.sections:
    print(f'{s.Name.decode(errors="replace").rstrip(chr(0)):10} va={s.VirtualAddress:08x} vsz={s.Misc_VirtualSize:08x} raw={s.PointerToRawData:08x} rsz={s.SizeOfRawData:08x}')
print('=== IMPORTS ===')
for e in getattr(pe, 'DIRECTORY_ENTRY_IMPORT', []):
    print(f'-- {e.dll.decode()} ({len(e.imports)})')
    print('   ' + ', '.join((i.name.decode() if i.name else f'ord{i.ordinal}') for i in e.imports))
print('=== EXPORTS ===')
if hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
    for e in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        print(e.ordinal, (e.name or b'').decode(), hex(e.address))
print('=== DELAY IMPORTS ===')
for e in getattr(pe, 'DIRECTORY_ENTRY_DELAY_IMPORT', []):
    print('--', e.dll.decode())
    print('   ' + ', '.join((i.name.decode() if i.name else f'ord{i.ordinal}') for i in e.imports))
print('=== RESOURCES ===')
for r in getattr(pe, 'DIRECTORY_ENTRY_RESOURCE', []).entries:
    print('type', r.id, r.name)
    for n in r.directory.entries:
        print('   name', n.id, n.name, 'langs', [l.id for l in n.directory.entries])
print('=== TLS ===', hasattr(pe, 'DIRECTORY_ENTRY_TLS'))
