import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read()
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
B1=0x08004800
vecnames=['SP','Reset','NMI','HardFault','MemManage','BusFault','UsageFault','RSV7','RSV8','RSV9','RSV10','SVC','DebugMon','RSV13','PendSV','SysTick']
print('=== exception vectors under base 0x%08x ==='%B1)
for i in range(16):
    v=struct.unpack_from('<I',b,4*i)[0]
    if i==0: print('SP 0x%08x'%v); continue
    if v==0 or not (0x08000000<=v<0x080F961C): print('%s 0x%08x ???'%(vecnames[i],v)); continue
    off=v-B1
    ins=[]
    if 0<=off<len(b)-8:
        ins=list(md.disasm(b[off:off+8],v&~1))[:2]
    print('%-11s entry 0x%08x -> file 0x%05x vaddr 0x%08x : %s'%(vecnames[i],v,off,v&~1,' ; '.join('%s %s'%(x.mnemonic,x.op_str) for x in ins)))
print()
print('=== distinct IRQ handlers ===')
seen=set()
for irq in range(115):
    v=struct.unpack_from('<I',b,4*(16+irq))[0]
    if v==0 or v==0x08004B2D: continue
    if v in seen: continue
    seen.add(v)
    off=v-B1
    ins=list(md.disasm(b[off:off+10],v&~1))[:3] if 0<=off<len(b)-8 else []
    print('IRQ%3d entry 0x%08x -> file 0x%05x vaddr 0x%08x : %s'%(irq,v,off,v&~1,' ; '.join('%s %s'%(x.mnemonic,x.op_str) for x in ins)))
