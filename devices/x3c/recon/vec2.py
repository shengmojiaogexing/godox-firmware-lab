import sys,struct
b=open(sys.argv[1],'rb').read()
print('=== candidate embedded vector tables (SP 0x2000_xxxx..0x2007_xxxx, reset 0x0800_xxxx odd) ===')
for i in range(0,len(b)-8,4):
    sp,rst=struct.unpack_from('<II',b,i)
    if 0x20000000<=sp<0x20080000 and (sp&3)==0 and 0x08000000<=rst<0x08100000 and (rst&1):
        print('file 0x%06x  SP=0x%08x reset=0x%08x'%(i,sp,rst))
print('\n=== ASCII density per 1KB (blocks with >8%% printable) ===')
import re
for o in range(0,len(b),1024):
    c=b[o:o+1024]
    if not c: break
    d=sum(1 for x in c if 32<=x<127)/len(c)
    if d>0.08: print('0x%06x  %.2f'%(o,d))
