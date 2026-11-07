import sys, struct, collections
p=sys.argv[1]; b=open(p,'rb').read()
# entropy 256-byte blocks -> compress runs
def ent(chunk):
    h=collections.Counter(chunk); n=len(chunk); e=0.0
    for c in h.values():
        pr=c/n; e-=pr*__import__('math').log2(pr)
    return e
rows=[]
for o in range(0,len(b),256):
    rows.append((o,ent(b[o:o+256])))
runs=[]; s=None; last=None
for o,e in rows:
    if last is None or abs(e-last)>0.15:
        if last is not None: runs.append((s,o,last))
        s=o; last=e
    else: last=last*0.7+e*0.3
runs.append((s,len(b),last))
for a,z,e in runs: print('0x%06x-0x%06x H=%.2f len=%d'%(a,z,e,z-a))
