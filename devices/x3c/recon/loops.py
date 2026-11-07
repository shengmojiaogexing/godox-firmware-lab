import sys,struct,collections
b=open(sys.argv[1],'rb').read(); BASE=0x08000000
# infinite loop patterns
pats={'fe e7':'b .','e7 fe':'b .','00 bf':'nop','fe e7 fe e7':'b .; b .'}
for p,name in pats.items():
    pat=bytes(int(x,16) for x in p.split())
    hits=[]; i=b.find(pat)
    while i!=-1 and len(hits)<20:
        hits.append(i); i=b.find(pat,i+1)
    print('%s (%s): %d printed, first: %s'%(p,name,len(hits),['0x%x'%h for h in hits[:20]]))
# where is 0x08004b2d referenced (if anywhere)
tgt=0x08004b2d
hits=[i for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0]==tgt]
print('words == 0x08004b2d:',['0x%x'%h for h in hits[:20]])
hits=[i for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0]==0x08004af9]
print('words == 0x08004af9:',['0x%x'%h for h in hits[:20]])
# stats: how many 4-aligned words are code ptrs, and their targets' distribution
