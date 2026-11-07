import sys,struct
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
print('hexdump 0x280-0x380:')
for off in range(0x280,0x380,16):
    c=b[off:off+16]; print('%06x '%off+' '.join('%02x'%x for x in c)+'  '+''.join(chr(x) if 32<=x<127 else '.' for x in c))
print('\ndisasm 0x300..0x340 (aligned 0x300):')
for ins in md.disasm(b[0x300:0x340],0x08000300):
    print(' 0x%08x %-8s %s'%(ins.address,ins.mnemonic,ins.op_str))
print('\nsearch words 0x0800031x / 0x0800032x:')
for t in [0x0800031b,0x0800031d,0x0800031f,0x08000321,0x0800032d,0x080002d9,0x0800020d,0x080003a1]:
    hits=[i for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0]==t]
    print(' 0x%08x: %s'%(t,['0x%x'%h for h in hits[:12]]))
