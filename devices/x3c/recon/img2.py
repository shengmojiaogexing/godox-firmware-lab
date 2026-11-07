import sys,struct
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
print('=== words at file 0x0f3800 (first 0x120) ===')
for i in range(0,0x120,16):
    off=0xf3800+i
    print('0x%06x '%off+' '.join('%08x'%struct.unpack_from('<I',b,off+j)[0] for j in range(0,16,4)))
def d(v,base,n=20):
    off=(v&~1)-base
    print('--- 0x%08x (file 0x%x)'%(v&~1,off))
    for ins in md.disasm(b[off:off+n*4],v&~1):
        print(' 0x%08x  %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
# second image mapping: assume base 0x08000000, file offset 0xf3800
def d2(v,n=20):
    off=0xf3800+((v&~1)-BASE)
    print('--- img2 0x%08x (file 0x%x)'%(v&~1,off))
    for ins in md.disasm(b[off:off+n*4],v&~1):
        print(' 0x%08x  %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
d2(0x080020cd,24)
print()
d2(0x080020df,10)
