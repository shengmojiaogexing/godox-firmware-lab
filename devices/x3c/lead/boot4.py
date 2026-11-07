import sys, struct
sys.path.insert(0,'../emu')
from machine import Machine
IMAGE='../firmware/X3_C_v1.38_app.bin'
m=Machine(image=IMAGE, log_fn=lambda s: None, trace=True, trace_max=200000)
try:
    m.uc.mem_map(0x1FFFC000,0x1000)
except Exception as e:
    print('map exists:',e)
try:
    m.uc.mem_write(0x1FFFC010,struct.pack('<H',2))
    print('readback 1FFFC010:',m.uc.mem_read(0x1FFFC010,2))
except Exception as e:
    print('write failed:',e)
p=m.find_peripheral(0x40012000); regs={}
def rd(off,size):
    o=off&~3
    if o==0: return 0x40
    if o==8: return regs.get(o,0)&~0x0C
    return regs.get(o,0)
def wr(off,size,val): regs[off&~3]=val
p.read=rd; p.write=wr
m.boot()
prev=None; same=0
for i in range(60):
    st=m.run(500000)
    pc=m.reg('PC'); sp=m.reg('SP'); lr=m.reg('LR')
    print('[%9d] stop=%-10s PC=0x%08x SP=0x%08x LR=0x%08x'%(m.insn_count,st,pc,sp,lr))
    if prev is not None and abs(pc-prev)<0x2000: same+=1
    else: same=0
    prev=pc
    if same>=8: print('  -> stuck in region'); break
    if st not in (None,'max_insn'): break
print('\nhot mmio:'); print(m.hot_mmio_report(20))
print('\nunknown:'); print(m.unknown_mmio_report(20))
