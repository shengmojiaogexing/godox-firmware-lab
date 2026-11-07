import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB); md.detail=True
# find literal words pointing to 0x20000170 / 0x20000160..0x20000180
for t in [0x20000170,0x2000016c,0x20000174,0x20000178,0x20000168]:
    hits=[(B+i) for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0]==t]
    print('0x%08x literals at vaddr: %s'%(t,['0x%08x'%h for h in hits[:10]]))
# find STR to 0x20000170: search for str/strh with literal base then offset. Look for functions whose literal pools contain it and inspect
