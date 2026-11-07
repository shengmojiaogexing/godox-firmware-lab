import sys,struct,collections
p=sys.argv[1]; b=open(p,'rb').read()
print('size 0x%x'%len(b))
# peripheral/literal scan
buckets=collections.Counter()
addrs=collections.Counter()
for i in range(0,len(b)-3,4):
    v=struct.unpack_from('<I',b,i)[0]
    if 0x40000000<=v<0x60000000:
        buckets[(v>>16)<<16]+=1; addrs[v]+=1
print('\n=== periph 64K buckets ===')
for k,c in sorted(buckets.items()): print('0x%08x x%d'%(k,c))
print('\n=== top periph addrs ===')
for v,c in addrs.most_common(60): print('0x%08x x%d'%(v,c))
# RAM pointers
ram=collections.Counter()
for i in range(0,len(b)-3,4):
    v=struct.unpack_from('<I',b,i)[0]
    if 0x20000000<=v<0x20080000: ram[(v>>12)<<12]+=1
print('\n=== RAM 4K pages ===')
for k,c in sorted(ram.items()): print('0x%08x x%d'%(k,c))
# tail hexdump
print('\n=== tail 256 bytes ===')
t=b[-256:]
for i in range(0,256,16): print('%06x '%(len(b)-256+i)+' '.join('%02x'%x for x in t[i:i+16])+'  '+''.join(chr(x) if 32<=x<127 else '.' for x in t[i:i+16]))
