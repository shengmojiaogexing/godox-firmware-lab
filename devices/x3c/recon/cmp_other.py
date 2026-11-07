import sys
from capstone import *
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
a=open('devices/ad100pro/firmware/AD100pro_V1.2.bin','rb').read()
b=open('devices/ad200pro/firmware/AD200pro_V1.60.bin','rb').read()
for name,x in [('AD100',a),('AD200',b)]:
    print('==',name)
    for off in range(0x30c0,0x3100,16):
        c=x[off:off+16]; print('%06x '%off+' '.join('%02x'%v for v in c)+'  '+''.join(chr(v) if 32<=v<127 else '.' for v in c))
print('compare 0x30d0-0x40d0 equal:', a[0x30d0:0x40d0]==b[0x30d0:0x40d0])
print('compare 0x0-0x60:', a[0:0x60]==b[0:0x60])
# try disasm at 0x30d4 with different offsets
for off in (0x30d4,0x30d2,0x30d6,0x30d3):
    print('AD100 disasm at 0x%x:'%off)
    for ins in list(md.disasm(a[off:off+16],0x08000000+off))[:5]:
        print('  0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
