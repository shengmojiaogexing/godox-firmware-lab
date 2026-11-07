import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB); md.detail=True
# find literal words 0x6C000000/0x6C000001 anywhere
hits=[i for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0] in (0x6C000000,0x6C000001)]
print('literal 0x6C000000/1 at file offsets (vaddr):', ['0x%08x'%(B+h) for h in hits])
# find callers of LCD data/command writers
targets={0x08025E94:set(),0x08025E9C:set(),0x08035BC4:set(),0x08035BE8:set()}
off=0
while off<len(b)-4:
    ins=list(md.disasm(b[off:off+4],B+off))
    if not ins: off+=2; continue
    i0=ins[0]
    if i0.mnemonic in ('bl','blx') and i0.operands and i0.operands[0].type==2:
        t=i0.operands[0].imm & ~1
        if t in targets: targets[t].add(i0.address)
    off+=i0.size
for t,cs in targets.items():
    print('callers of 0x%08x: %d %s'%(t,len(cs),['0x%08x'%c for c in sorted(cs)[:20]]))
