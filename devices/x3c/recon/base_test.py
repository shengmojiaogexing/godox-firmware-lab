import sys
from capstone import *
b=open(sys.argv[1],'rb').read()
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
vecs=[0x08004af9,0x08010ac9,0x0800c14d,0x0800f4b9,0x080062e1,0x08017181,0x08013b7d,0x08016669]
for B in [0x08000000,0x08001000,0x08002000,0x08004000,0x08008000,0x08010000,0x08020000]:
    print('===== base 0x%08x'%B)
    for v in vecs[:6]:
        off=(v&~1)-B
        if off<0 or off+16>len(b): print('  vec 0x%08x out of range'%v); continue
        ins=list(md.disasm(b[off:off+12],v&~1))[:3]
        print('  vec 0x%08x off 0x%05x: '%(v,off)+' ; '.join('%s %s'%(i.mnemonic,i.op_str) for i in ins))
