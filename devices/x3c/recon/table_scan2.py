import sys,struct
b=open(sys.argv[1],'rb').read()
def is_ram(v): return 0x20000000<=v<0x20080000 and v%4==0
def is_code(v): return 0x08000000<=v<0x08100000 and v%2==1
print('strict vector-table scan v2:')
for o in range(0,len(b)-64*4,4):
    w=struct.unpack_from('<16I',b,o)
    if not is_ram(w[0]): continue
    if any(w[i]!=0 for i in (7,8,9,10,13)): continue
    if not all(is_code(w[i]) for i in (1,2,3,4,5,6,11,14,15)): continue
    # IRQ entries: allow 0 or code; require at least 4 entries and most are code
    n=0; codecnt=0; ok=True
    while o+4*(16+n)+4<=len(b) and n<400:
        v=struct.unpack_from('<I',b,o+4*(16+n))[0]
        if v==0: n+=1; continue
        if is_code(v): codecnt+=1; n+=1; continue
        ok=False; break
    if not ok and n<16: continue
    print(' table at file 0x%06x SP=0x%08x reset=0x%08x irq_entries=%d code_irq=%d'%(o,w[0],w[1],n,codecnt))
