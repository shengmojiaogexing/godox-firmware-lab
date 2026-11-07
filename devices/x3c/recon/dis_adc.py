import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def d(v,n=30):
    off=v-B
    print('--- vaddr 0x%08x file 0x%05x'%(v,off))
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
d(0x08018be0,40)
d(0x08035350,26)
print()
print('literal words near adc funcs:')
for o in [0x1C220,0x14300,0x30B00]:
    print('file 0x%x:'%o, ['0x%08x'%struct.unpack_from('<I',b,o+j*4)[0] for j in range(0,8)])
