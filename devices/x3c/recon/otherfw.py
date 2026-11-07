import sys,struct
from capstone import *
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
for path in ['devices/ad100pro/firmware/AD100pro_V1.2.bin',
 'devices/ad200pro/firmware/AD200pro_V1.60.bin']:
    b=open(path,'rb').read()
    print('==== %s size 0x%x'%(path,len(b)))
    w=[struct.unpack_from('<I',b,4*i)[0] for i in range(20)]
    print(' first words:', ' '.join('0x%08x'%x for x in w[:12]))
    sp=w[0]; rst=w[1]
    print(' SP=0x%08x reset=0x%08x'%(sp,rst))
    for v,nm in [(rst,'reset'),(w[3],'hardfault')]:
        if not(0x08000000<=v<0x08100000): print('  %s out of range'%nm); continue
        off=(v&~1)-0x08000000
        if off<0 or off>len(b): print('  %s beyond file'%nm); continue
        print('  %s 0x%08x file 0x%x:'%(nm,v&~1,off))
        for ins in list(md.disasm(b[off:off+24],v&~1))[:6]:
            print('   0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
