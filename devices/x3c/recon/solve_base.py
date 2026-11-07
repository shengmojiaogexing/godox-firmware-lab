import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read()
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
# find aligned fe e7 (b .)
loops=[]
for i in range(0,0xf3800-2,2):
    if b[i]==0xfe and b[i+1]==0xe7: loops.append(i)
print('num b. halfword positions in image1:',len(loops))
print(['0x%x'%x for x in loops[:40]])
print()
# for each loop O: B1 = 0x08004B2C - O ; reset offset = 0x08004AF8-B1 = O-0x34
for O in loops[:60]:
    B1=0x08004B2C-O
    if not (0x08000000<=B1<0x08080000): continue
    ro=0x08004AF8-B1
    if ro<1 or ro>0xf3800-16: continue
    ins=list(md.disasm(b[ro:ro+16],B1+ro))[:4]
    s=' ; '.join('%s %s'%(x.mnemonic,x.op_str) for x in ins)
    print('loop@0x%05x -> B1=0x%08x reset@0x%05x: %s'%(O,B1,ro,s))
