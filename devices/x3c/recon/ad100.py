import struct
a=open('devices/ad100pro/firmware/AD100pro_V1.2.bin','rb').read()
b=open(r'devices/ad200pro/firmware/AD200pro_V1.60.bin','rb').read()
print('AD100 size 0x%x, AD200 size 0x%x'%(len(a),len(b)))
print('AD100 first 0x140 words:')
for i in range(0,0x140,16):
    print('%06x '%i+' '.join('%08x'%struct.unpack_from('<I',a,i+j)[0] for j in range(0,16,4)))
print('\nAD100 hexdump 0x90-0x140:')
for off in range(0x90,0x140,16):
    c=a[off:off+16]; print('%06x '%off+' '.join('%02x'%v for v in c)+'  '+''.join(chr(v) if 32<=v<127 else '.' for v in c))
