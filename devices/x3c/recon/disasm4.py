import sys
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def d(v,n=8):
    v&=~1; off=v-BASE
    print('--- 0x%08x  bytes: %s'%(v,' '.join('%02x'%x for x in b[off:off+16])))
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x  %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
for v in [0x0800c14d,0x0800f4b9,0x080062e1,0x08017181,0x08010ac9,0x08013b7d,0x08010bfd,0x08016669,0x08006e59,0x08004af9]:
    d(v,10)
