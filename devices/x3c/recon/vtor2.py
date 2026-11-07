import sys
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def d(off,n=26,base=BASE):
    v=base+off
    print('--- file 0x%x vaddr 0x%08x'%(off,v))
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x  %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
d(0x11e20,40)
d(0x31440,50)
d(0x0f4f80,60)
