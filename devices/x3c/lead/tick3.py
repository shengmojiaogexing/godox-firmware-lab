import sys, os, struct
sys.path.insert(0,'.'); sys.path.insert(0,'../emu')
from machine import Machine
from boot6 import patch_option_bytes, patch_adc, patch_delay, map_xmc
m=Machine(image='../firmware/X3_C_v1.38_app.bin', log_fn=lambda s:None, trace=True)
patch_option_bytes(m); patch_adc(m); patch_delay(m); map_xmc(m)
m.boot()
m.run(10000000)
print('PC=0x%08x insns=%d'%(m.reg('PC'),m.insn_count))
d=bytes(m.uc.mem_read(0x200002f0,0x40))
print('RAM 0x200002F0:', ' '.join('%02x'%x for x in d))
print('word 0x2000030c=0x%08x 0x20000310=0x%08x'%(int.from_bytes(d[0x1c:0x20],'little'),int.from_bytes(d[0x20:0x24],'little')))
for a in (0x2000030c,0x20000310,0x20000070,0x20000170):
    print('0x%08x: 0x%08x'%(a,int.from_bytes(bytes(m.uc.mem_read(a,4)),'little')))
