import struct
import sys
import pefile

sys.stdout.reconfigure(encoding='utf-8')
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


def parse(name, mva):
    print('=' * 70)
    print(name, hex(mva))
    superdata, stringdata, data = struct.unpack('<III', rd(mva, 12))
    print('  super=%08x stringdata=%08x data=%08x' % (superdata, stringdata, data))
    hdr = struct.unpack('<14I', rd(data, 14 * 4))
    rev = hdr[0]
    method_count, method_data = hdr[4], hdr[5]
    prop_count, prop_data = hdr[6], hdr[7]
    signal_count = hdr[13]

    nstr = 0
    base = va2off(stringdata)
    while True:
        o = base + nstr * 16
        if struct.unpack_from('<i', d, o)[0] != -1 or nstr > 500:
            break
        nstr += 1
    strs = []
    for i in range(nstr):
        o = base + i * 16
        ref, size, alloc, off = struct.unpack_from('<iiIi', d, o)
        ia = ib + (stringdata - ib) + 0  # entry VA
        eva = stringdata + i * 16
        so = va2off(eva + off)
        strs.append(d[so:so + size].decode('utf-8', 'replace') if so is not None else '')

    def s(i):
        return strs[i] if 0 <= i < len(strs) else '<%d>' % i

    md = method_data * 4
    print('  -- methods (signals first %d) --' % signal_count)
    for i in range(method_count):
        nm, argc, params, tag, mflags = struct.unpack('<5I', rd(data + md + i * 20, 20))
        kind = 'signal' if i < signal_count else 'slot/method'
        print('   [%2d] %-32s argc=%d ret=%d flags=0x%03x %s' % (i, s(nm), argc, tag, mflags, kind))
    if prop_count:
        pd = prop_data * 4
        print('  -- properties --')
        for i in range(prop_count):
            nm, typ, pflags = struct.unpack('<3I', rd(data + pd + i * 12, 12))
            print('   prop[%2d] %-24s type=%d flags=0x%x' % (i, s(nm), typ, pflags))


parse('BackEnd', 0x434080)
parse('MainTask', 0x4340b0)
parse('TreeViewModel', 0x434098)
