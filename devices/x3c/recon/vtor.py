import sys
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def d(v,n=26):
    v&=~1; off=v-BASE
    print('--- 0x%08x (file 0x%x)'%(v,off))
    for ins in md.disasm(b[off:off+n*4],v):
        print(' 0x%08x  %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
d(0x11e80,26)
d(0x31500,30)
d(0xf5020,30)
