import sys,struct
b=open(sys.argv[1],'rb').read()
# look for VTOR refs and SCB
for pat,name in [(0xE000ED08,'VTOR'),(0xE000ED0C,'AIRCR'),(0xE000ED04,'ICSR'),(0xE000ED00,'CPUID'),(0xE000ED88,'CPACR')]:
    hits=[i for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0]==pat]
    print('%s 0x%08x hits at %s'%(name,pat,['0x%x'%h for h in hits[:12]]))
# find ASCII strings in tail blob
import re
def strings(off,end,minlen=5):
    out=[];cur=b'';st=0
    for i in range(off,min(end,len(b))):
        c=b[i]
        if 32<=c<127:
            if not cur: st=i
            cur+=bytes([c])
        else:
            if len(cur)>=minlen: out.append((st,cur.decode()))
            cur=b''
    if len(cur)>=minlen: out.append((st,cur.decode()))
    return out
print('\nstrings in 0x0f3800+:')
for o,s in strings(0xf3800,len(b))[:80]: print(' 0x%06x %s'%(o,s))
