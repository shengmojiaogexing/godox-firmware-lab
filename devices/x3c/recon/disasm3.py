import sys
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def d(v,n=22):
    v&=~1; off=v-BASE
    print('--- 0x%08x'%v)
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x  %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
for v in [0x08004b2d,0x0800510d,0x080082eb,0x080083ad,0x08008459,0x080084ad,0x08012985,0x08016c01,0x08016c5d,0x08016d51,0x08006e25,0x08006e3d,0x08010b19,0x080083ed,0x080082c5,0x08004af9]:
    d(v,20)
