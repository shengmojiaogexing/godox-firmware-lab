import sys, re, struct
p=sys.argv[1]; b=open(p,'rb').read()
# ascii strings
out=[]; cur=bytearray(); start=0
for i,c in enumerate(b):
    if 0x20<=c<0x7f:
        if not cur: start=i
        cur.append(c)
    else:
        if len(cur)>=5: out.append((start,cur.decode('latin1')))
        cur=bytearray()
if len(cur)>=5: out.append((start,cur.decode('latin1')))
print('ascii count',len(out))
for o,s in out: print('%06x %s'%(o,s))
