import sys,struct
from capstone import *
b=open(sys.argv[1],'rb').read()
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB); md.detail=False
def dis(off,n=24,base=0x08000000):
    print('--- at file 0x%x (vaddr 0x%x)'%(off,base+off))
    code=b[off:off+n*4]
    for ins in md.disasm(code, base+off):
        print(' 0x%08x  %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
dis(0x20c,30)
dis(0x4af8,30)
dis(0xef0,40)
