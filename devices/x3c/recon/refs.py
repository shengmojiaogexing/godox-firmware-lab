import sys,struct
b=open(sys.argv[1],'rb').read()
targets={0x08000f0c:'GR A-E',0x08002b50:'Model: ',0x08002b9c:'Firmware: ',0x08002bb0:'v1.38',0x08002b5b:'X3 C',0x0801d847:'HID IAP'}
for t,name in targets.items():
    hits=[i for i in range(0,len(b)-3,4) if struct.unpack_from('<I',b,i)[0]==t]
    print('0x%08x %-12s -> word found at file offsets: %s'%(t,name,['0x%x'%h for h in hits[:10]]))
# find all words pointing into the rodata string cluster 0x08000ef8-0x08002c10
print('\nwords pointing into 0x08000ef8..0x08002c10:')
for i in range(0,len(b)-3,4):
    v=struct.unpack_from('<I',b,i)[0]
    if 0x08000ef8<=v<0x08002c10:
        print(' file 0x%06x -> vaddr 0x%08x'%(i,v))
