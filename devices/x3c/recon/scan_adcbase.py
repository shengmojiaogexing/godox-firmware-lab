import struct,collections
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
c=collections.Counter()
for i in range(0,len(b)-3,4):
    v=struct.unpack_from('<I',b,i)[0]
    if 0x40010000<=v<0x40070000: c[v]+=1
print('literals in 0x40010000-0x4006FFFF (unique %d):'%len(c))
for v,n in sorted(c.items()):
    if n>=1: print('  0x%08x x%d'%(v,n))
