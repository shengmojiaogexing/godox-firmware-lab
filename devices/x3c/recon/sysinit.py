import sys,struct
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def d(off,n=24):
    v=BASE+off
    print('--- file 0x%x vaddr 0x%08x'%(off,v))
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x  %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
def dump(o,n=32):
    print('--- literals at file 0x%x:'%o)
    for i in range(0,n*4,16):
        print(' 0x%06x: '%(o+i)+' '.join('%08x'%struct.unpack_from('<I',b,o+i+j)[0] for j in range(0,16,4)))
d(0x11e6c,26)
dump(0x11eb8,28)
d(0x31500,36)
dump(0x31540,20)
