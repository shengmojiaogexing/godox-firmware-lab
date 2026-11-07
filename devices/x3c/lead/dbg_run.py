import sys, os, struct
HERE=os.path.dirname(os.path.abspath("trace_xmc.py")); sys.path.insert(0,os.path.abspath('.')); sys.path.insert(0,'../emu')
from machine import Machine
from boot6 import patch_option_bytes, patch_adc, patch_delay, map_xmc
m=Machine(image='../firmware/X3_C_v1.38_app.bin', log_fn=lambda s: None, trace=True)
patch_option_bytes(m); patch_adc(m); patch_delay(m); map_xmc(m)
m.boot()
trace=[]
def h(uc,addr,size,user):
    if len(trace)<120: trace.append(addr)
m.uc.hook_add(1,h)   # UC_HOOK_CODE
m.run(200000)
print('PC=0x%08x insn=%d'%(m.reg('PC'),m.insn_count))
print('first 120 PCs:', ' '.join('0x%x'%a for a in trace))
