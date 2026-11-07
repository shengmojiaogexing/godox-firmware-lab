import sys,struct
from capstone import *
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)

def stats(table_off, base, label):
    n=0; push=0; b_targets=[]
    vals=[struct.unpack_from('<I',b,table_off+4*i)[0] for i in range(16+200) if table_off+4*i+4<=len(b)]
    # stop at first non-code after irq start
    irqs=[]
    for i,v in enumerate(vals[16:],start=16):
        if 0x08000000<=v<0x08100000 and v&1: irqs.append(v)
        else: break
    print('%s: irqs=%d'%(label,len(irqs)))
    for v in irqs:
        off=(v&~1)-base
        ins=list(md.disasm(b[off:off+4],v&~1))
        if not ins: continue
        n+=1
        m=ins[0].mnemonic; op=ins[0].op_str
        if m=='push' or (m=='stmdb' and op.startswith('sp')) or (m=='str' and 'sp,' in op and '!' in op):
            push+=1
        else:
            b_targets.append('%08x:%s %s'%(v&~1,m,op))
    print('  prologue-like: %d/%d'%(push,n))
    print('  non-prologue first instrs (first 40):')
    for s in b_targets[:40]: print('   ',s)

stats(0x0,0x08000000,'image1 table@0')
print()
stats(0xf3800,0x08000000,'image2 table@0xf3800 (base 0x08000000)')
