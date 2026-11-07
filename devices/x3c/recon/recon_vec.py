import sys, re, struct, math, collections
p = sys.argv[1]
b = open(p,'rb').read()
print('size', len(b), hex(len(b)))
# vector table
names=['SP','Reset','NMI','HardFault','MemManage','BusFault','UsageFault','RSV7','RSV8','RSV9','RSV10','SVC','DebugMon','RSV13','PendSV','SysTick']
for i in range(0, 256):
    off=i*4
    if off+4>len(b): break
    v=struct.unpack_from('<I',b,off)[0]
    nm = names[i] if i<len(names) else 'IRQ%d'%(i-16)
    if i>15 and (v==0 or v==0x08017181 or v==0x08017180): continue
    print('%3d %-10s 0x%08x'%(i,nm,v))
