import struct,collections
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
# all literal words in system-memory-ish ranges
for lo,hi,name in [(0x1FFF0000,0x20000000,'0x1FFFxxxx-0x1FFFFFFF'),(0x00000000,0x08000000,'low'),(0x40000000,0x50000000,'periph'),(0x50000000,0x60000000,'periph2'),(0xA0000000,0xA1000000,'FMC'),(0xE0000000,0xE0100000,'PPB'),(0x08004800,0x08100000,'flash-app')]:
    c=collections.Counter()
    for i in range(0,len(b)-3,4):
        v=struct.unpack_from('<I',b,i)[0]
        if lo<=v<hi: c[v]+=1
    print('== %s: %d unique'%(name,len(c)))
    for v,n in c.most_common(40):
        print('   0x%08x x%d'%(v,n))
