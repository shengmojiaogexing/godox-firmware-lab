import os, sys
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))  # devices/ad200pro
from sim.machine import Machine
from unicorn import UC_HOOK_CODE
from unicorn.arm_const import UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_SP

m = Machine()
log = []
def hook(uc, addr, size, ud):
    if addr == 0x0800CA16:      # rf_read_reg8 entry: r0 = reg
        log.append(('read_reg8', uc.reg_read(UC_ARM_REG_R0)))
    elif addr == 0x0800CA26:    # after rf_spi_transfer: read buf byte
        sp = uc.reg_read(UC_ARM_REG_SP)
        try:
            b = m.read(sp, 1)[0]
        except Exception:
            b = -1
        log.append(('read_result', b))
    elif addr == 0x0800CAA2:    # rf_write_reg8 entry r0=reg
        log.append(('write_reg8', uc.reg_read(UC_ARM_REG_R0)))
    elif addr == 0x0800CAEC:
        pass
m.uc.hook_add(UC_HOOK_CODE, hook)
r = m.run(max_insns=200000, stop_on_stall=False)
print('result', r, 'pc', hex(m.stats()['pc']), m.fw.name(m.stats()['pc']))
print('log entries', len(log))
for e in log[:40]:
    print('  ', e)
print('...')
for e in log[-20:]:
    print('  ', e)
print('rf regs:', {hex(k): hex(v) for k, v in sorted(m.rf.regs.items()) if k in (0x04,0x05,0x0d,0x0e,0x0f)})
print('csn', m.rf.csn, 'sck', m._sck_prev, 'MISO input', m.device.gpio_inputs.get('B', {}).get(14))
print('gpio B ODR', hex(m.read32(0x48000414)), 'gpio C ODR', hex(m.read32(0x48000814)))
