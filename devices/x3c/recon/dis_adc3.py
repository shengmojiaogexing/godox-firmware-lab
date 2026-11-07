import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
off=(0x08018a00-B)
for ins in md.disasm(b[off:off+0x500],0x08018a00):
    print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
