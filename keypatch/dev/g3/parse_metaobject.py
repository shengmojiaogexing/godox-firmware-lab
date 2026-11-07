"""Parse Qt5 metaobject structures (stringdata + method table) from the G3 PE."""
import struct, sys
import pefile

p = r'C:\Users\22435\AppData\Local\Temp\g3audit\GodoxV2.exe'
d = open(p, 'rb').read()
pe = pefile.PE(p, fast_load=True)
ib = pe.OPTIONAL_HEADER.ImageBase
secs = [(s.VirtualAddress, s.PointerToRawData, s.SizeOfRawData) for s in pe.sections]


def va2off(va):
    rva = va - ib
    for v, raw, sz in secs:
        if v <= rva < v + sz:
            return raw + (rva - v)
    return None


def rd(va, n):
    o = va2off(va)
    return d[o:o + n]


def cstr(va):
    o = va2off(va)
    e = d.index(b'\0', o)
    return d[o:e].decode('utf-8', 'replace')


for name, mva in (('BackEnd', 0x434080), ('MainTask', 0x4340b0), ('TreeViewModel', 0x434098)):
    print('=' * 60)
    print(name, hex(mva))
    superdata, stringdata, data = struct.unpack('<III', rd(mva, 12))
    print('  superdata=%08x stringdata=%08x data=%08x' % (superdata, stringdata, data))
    if not (0x400000 <= stringdata < 0x500000):
        print('  (no valid stringdata)')
        continue
    hdr = struct.unpack('<14I', rd(data, 14 * 4))
    rev, classname_idx, classinfo_count, methods_count, properties_count = hdr[0], hdr[1], hdr[2], hdr[3], hdr[4]
    print('  rev=%d classname_idx=%d classinfo=%d methods=%d props=%d' % (rev, classname_idx, classinfo_count, methods_count, properties_count))

    def s_at(i):
        # stringdata format: uint32 length followed by chars (Qt5)
        off = va2off(stringdata)
        # compute offset table: each entry length+chars, aligned? Qt5: for i-th string, offset is stored sequentially in data header? Actually header after classinfo has string offsets.
        return None

    # Qt5 method entry: 5 uints per method: name, argc, params, tag, flags
    n = hdr[3]
    mo = data + 14 * 4
    methods = []
    for i in range(n):
        e = struct.unpack('<5I', rd(mo + i * 20, 20))
        methods.append(e)
    # stringdata: uint count, then for each string: uint len + utf16 chars
    o = va2off(stringdata)
    scount = struct.unpack_from('<I', d, o)[0]
    pos = o + 4
    strs = []
    for i in range(scount):
        ln = struct.unpack_from('<I', d, pos)[0]
        pos += 4
        s = d[pos:pos + ln].decode('utf-16le', 'replace')
        pos += ln
        strs.append(s)
    # Qt5 layout: first 2 entries are class name and ... actually strings[0] is class name, strings[1] empty
    print('  stringdata count=%d first=%r' % (scount, strs[:6]))
    # methods: name index points into stringdata; signal indices are 0..signals-1
    for i, (nm, argc, par, tag, flags) in enumerate(methods):
        print('  method[%d] name=%r argc=%d params=%d tag=%d flags=0x%x' % (i, strs[nm] if nm < len(strs) else nm, argc, par, tag, flags))
    # properties: 3 uints per property
    if properties_count:
        po = mo + n * 20
        print('  -- properties --')
        for i in range(properties_count):
            e = struct.unpack('<3I', rd(po + i * 12, 12))
            print('  prop[%d] name=%r type=%d flags=0x%x' % (i, strs[e[0]] if e[0] < len(strs) else e[0], e[1], e[2]))
