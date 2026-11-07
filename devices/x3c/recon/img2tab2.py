import sys,struct
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
for i in range(0,46):
    v=struct.unpack_from('<I',b,0xf3800+4*i)[0]
    if i==0: print('  0 SP 0x%08x'%v); continue
    if v==0: print('%3d  0'%i); continue
    if not (0x08000000<=v<0x08100000 and v&1):
        print('%3d  0x%08x  <-- not a code ptr; table ends'%(i,v)); break
    off=0xf3800+((v&~1)-BASE)
    ins=list(md.disasm(b[off:off+8],v&~1))[:2]
    print('%3d  0x%08x  off=0x%06x  %s'%(i,v,off,' ; '.join('%s %s'%(x.mnemonic,x.op_str) for x in ins)))
