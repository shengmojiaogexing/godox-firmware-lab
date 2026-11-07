import struct
b=open(r'devices/x3c/firmware/X3_C_v1.38_app.bin','rb').read(); B=0x08004800
for t in [0x2000030C,0x20000310,0x20000314,0x20000318]:
    hits=[B+i for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0]==t]
    print('0x%08x literal at: %s'%(t,['0x%08x'%h for h in hits]))
# also search halfword literals (movw)? print nearby pool words
