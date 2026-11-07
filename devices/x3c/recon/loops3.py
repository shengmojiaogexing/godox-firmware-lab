import sys,struct
b=open(sys.argv[1],'rb').read()
for t in [0x08000301,0x080002d9,0x0800031b,0x080002f1,0x08000401,0x08000501]:
    hits=[i for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0]==t]
    print('word 0x%08x at file offsets: %s'%(t,['0x%x'%h for h in hits[:10]]))
print()
# Check image2's reset handler region similarly: find b. loops and reset-looking code in image2
print('image2 0xf3800 region: dump first 0x100 of code after table (0xf38b8)')
from capstone import *
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
for ins in md.disasm(b[0xf38b8:0xf3900],0x080020b8):
    print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
