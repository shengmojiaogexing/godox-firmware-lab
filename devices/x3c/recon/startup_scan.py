from capstone import *
md=Cs(CS_ARCH_ARM,CS_MODE_THUMB)
for hw in [0x4685,0x468d,0x4680,0x4681,0x468f]:
    for ins in md.disasm(bytes([hw&0xff,hw>>8]),0):
        print('0x%04x -> %s %s'%(hw,ins.mnemonic,ins.op_str))
print()
# scan AD100 & X3 for startup patterns: ldr r0,[pc,#imm]; mov sp,r0 / ldr r0,=..; blx
import re
for path,base in [('devices/ad100pro/firmware/AD100pro_V1.2.bin',0x08000000),(r'devices/x3c/firmware/X3_C_v1.38_app.bin',0x08000000)]:
    b=open(path,'rb').read()
    print('==',path)
    hits=[]
    for i in range(0,len(b)-8,2):
        # ldr r0,[pc,#imm8*4] : 0x48xx ; mov sp,r0 : 0x468d or 0x4685
        if b[i]==0x48 or b[i]==0x4f or b[i]==0x46:
            pass
    # simpler: find 468d/4685 preceded by 48xx
    for i in range(0,len(b)-4,2):
        hw=b[i]|(b[i+1]<<8); hw2=b[i+2]|(b[i+3]<<8)
        if (hw&0xff00)==0x4800 and hw2 in (0x4685,0x468d):
            hits.append(i)
    print(' ldr r0,=...; mov sp,r0 hits at file offsets:',['0x%x'%h for h in hits[:20]])
