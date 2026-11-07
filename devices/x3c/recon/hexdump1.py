import sys
b=open(sys.argv[1],'rb').read()
for off in (0x2b0,0xef8,0x1d830,0xf3900,0xf9400,0x0):
    print('=== 0x%x ==='%off)
    for i in range(0,64,16):
        c=b[off+i:off+i+16]
        print('%06x '%(off+i)+' '.join('%02x'%x for x in c)+'  '+''.join(chr(x) if 32<=x<127 else '.' for x in c))
