import struct
from capstone import *
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB); md.detail=True
targets={0x080334EC:set(),0x080334D8:set(),0x080334C0:set(),0x080354EC:set(),0x080354E4:set(),0x080353A0:set()}
off=0
while off<len(b)-4:
    ins=list(md.disasm(b[off:off+4],B+off))
    if not ins: off+=2; continue
    i0=ins[0]
    if i0.mnemonic in ('bl','blx') and i0.operands and i0.operands[0].type==2:
        t=i0.operands[0].imm&~1
        if t in targets: targets[t].add(i0.address)
    off+=i0.size
for t,cs in targets.items():
    print('callers of 0x%08x: %s'%(t,['0x%08x'%c for c in sorted(cs)[:15]]))
