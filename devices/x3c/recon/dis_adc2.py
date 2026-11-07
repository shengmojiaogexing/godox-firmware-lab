import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def d(v,n=45):
    off=v-B
    print('--- vaddr 0x%08x file 0x%05x'%(v,off))
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
d(0x080352b0,60)
print()
# literal pool dump for that function region
for o in [0x30b00,0x30b40,0x30b80]:
    print('file 0x%x:'%o, ' '.join('0x%08x'%struct.unpack_from('<I',b,o+j*4)[0] for j in range(8)))
