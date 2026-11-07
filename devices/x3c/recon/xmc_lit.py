import struct,collections
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
# literals in XMC range
c=collections.Counter()
for i in range(0,len(b)-3,4):
    v=struct.unpack_from('<I',b,i)[0]
    if 0x60000000<=v<0x70000000: c[v]+=1
print('XMC literals:',[(hex(v),n) for v,n in c.most_common(20)])
# disassemble around 0x08035B80
off=0x08035B60-B
print('\n--- 0x08035B60 ---')
for ins in md.disasm(b[off:off+0x100],0x08035B60):
    print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
