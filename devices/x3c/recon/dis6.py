import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def dump(o,n=48,note=''):
    print('hexdump file 0x%x %s'%(o,note))
    for i in range(0,n,16):
        c=b[o+i:o+i+16]; print('%06x '%(o+i)+' '.join('%02x'%v for v in c))
def d(v,n=30):
    off=v-B
    print('--- vaddr 0x%08x file 0x%05x'%(v,off))
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
d(0x08004AF8,24)
dump(0x080004AF8-B,64,'(reset stub literals)')
d(0x08020A20,20)
dump(0x08020A20-B,64,'(boot check literals)')
print()
print('literal words at 0x08020A54..0x08020A60:',['0x%08x'%struct.unpack_from('<I',b,0x1C254+j*4)[0] for j in range(4)])
