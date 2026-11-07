import re
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read()
pats={
 'keil_reset': b'\x04\x48\x80\x47\x04\x48\x00\x47',
 'keil_reset2': b'\x03\x48\x80\x47\x03\x48\x00\x47',
 'ldr_movsp': None,
}
for name,p in pats.items():
    if p is None: continue
    hits=[m.start() for m in re.finditer(re.escape(p),b)]
    print(name, len(hits), ['0x%x'%h for h in hits[:10]])
# all occurrences of pattern ldr r0,[pc,#imm]; blx r0; ldr r0,[pc,#imm]; bx r0 (16 bytes)
import struct
print('\ncandidate keil reset sequences (04 48 80 47):')
for m in re.finditer(b'\x04\x48\x80\x47',b):
    o=m.start()
    print(' file 0x%06x : %s'%(o,' '.join('%02x'%x for x in b[o:o+16])))
