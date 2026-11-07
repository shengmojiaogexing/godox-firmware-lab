import sys
from unicorn import *
from unicorn.arm_const import *
img=open(sys.argv[1],'rb').read()
def run(pc, sp, n=200000, label=''):
    mu=Uc(UC_ARCH_ARM, UC_MODE_THUMB|UC_MODE_MCLASS)
    mu.mem_map(0x08000000,0x100000)
    mu.mem_write(0x08000000,img)
    mu.mem_map(0x20000000,0x80000)
    mu.mem_map(0xE0000000,0x100000)
    mu.reg_write(UC_ARM_REG_SP,sp); mu.reg_write(UC_ARM_REG_PC,pc)
    trace=[]
    def hook(mu,addr,size,user):
        if len(trace)<40: trace.append(addr)
    mu.hook_add(UC_HOOK_CODE, hook)
    err=None
    try:
        mu.emu_start(pc|1, 0xFFFFFFFF, count=n)
    except UcError as e:
        err=e
    finally:
        pcv=mu.reg_read(UC_ARM_REG_PC); spv=mu.reg_read(UC_ARM_REG_SP); lrv=mu.reg_read(UC_ARM_REG_LR)
        print('%s: pc=0x%08x sp=0x%08x lr=0x%08x err=%s'%(label,pcv,spv,lrv,err))
        print('  trace:', ' '.join('0x%08x'%a for a in trace[:25]))
        # fault status
        try:
            cfsr=mu.mem_read(0xE000ED28,4); hfsr=mu.mem_read(0xE000ED2C,4)
            print('  CFSR=0x%08x HFSR=0x%08x'%(int.from_bytes(cfsr,'little'),int.from_bytes(hfsr,'little')))
        except Exception as e: print('  cfsr read fail',e)
for pc,sp,label in [(0x08004af9,0x200477c0,'reset-from-table'),(0x08000301,0x200477c0,'reset@0x300'),(0x0800020d,0x200477c0,'scatterload@0x20c')]:
    run(pc,sp,label=label)
