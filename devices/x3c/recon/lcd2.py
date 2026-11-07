import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB); md.detail=True
def foff(v): return v-B
print('file bytes around vaddr 0x08035BE0:')
o=foff(0x08035BD0)
for i in range(0,0x40,16):
    print(' vaddr 0x%08x:'%(0x08035BD0+i), ' '.join('0x%08x'%struct.unpack_from('<I',b,o+i+j)[0] for j in range(0,16,4)))
print('\ncallers of 0x08035BC4 / 0x08035BE8 / 0x08035C00:')
targets={0x08035BC4,0x08035BE8,0x08035C00}
from collections import defaultdict
callers=defaultdict(list)
off=0
while off<len(b)-4:
    ins=list(md.disasm(b[off:off+4],B+off))
    if not ins: off+=2; continue
    i0=ins[0]
    if i0.mnemonic in ('bl','blx') and i0.operands and i0.operands[0].type==2:
        t=i0.operands[0].imm & ~1
        if t in targets: callers[t].append(i0.address)
    off+=i0.size
for t in sorted(targets):
    print(' 0x%08x: %s'%(t,['0x%08x'%a for a in callers[t][:20]]))
