import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def d(v,n=40):
    off=v-B
    print('--- vaddr 0x%08x'%v)
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
d(0x08016D40,40)
d(0x080354E4,36)
