import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
def dump(o,n=0x40):
    print('hexdump file 0x%x:'%o)
    for i in range(0,n,16):
        print(' 0x%08x: '%(B+o+i)+' '.join('%08x'%struct.unpack_from('<I',b,o+i+j)[0] for j in range(0,16,4)))
dump(0x080334E0-B,0x30)
dump(0x08033500-B,0x40)
# disassemble 0x08033540..0x080335b0 (scheduler maybe)
print('\n--- 0x08033540 ---')
for ins in md.disasm(b[0x08033540-B:0x080335C0-B],0x08033540):
    print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
