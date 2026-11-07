import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
print('literals around 0x08035BE0 (file 0x31BE0):')
for o in range(0x31BD0,0x31C00,16):
    print(' file 0x%x:'%o, ' '.join('0x%08x'%struct.unpack_from('<I',b,o+j)[0] for j in range(0,16,4)))
# find callers of 0x08035bc4 / 0x08035be8 / 0x08035c00
print('\ncallers of cmd/data writers:')
for t in (0x08035BC4,0x08035BE8):
    hits=[]
    for i in range(0,len(b)-3,2):
        # BL/BLX immediate 32-bit: F000 F800 pattern; decode via capstone scan of likely code? simpler: scan all halfwords for BL with target
        pass
    # brute: disassemble whole code regions in chunks and check bl targets
    off=0
    n=0
    while off<len(b)-4:
        ins=list(md.disasm(b[off:off+4],B+off))
        if ins and ins[0].mnemonic in ('bl','blx') and ins[0].operands and ins[0].operands[0].type==2:
            if (ins[0].operands[0].imm & ~1)==t:
                hits.append(ins[0].address)
                n+=1
        off+=2 if not ins else ins[0].size
    print(' 0x%08x: %d callers %s'%(t,len(hits),['0x%08x'%h for h in hits[:15]]))
