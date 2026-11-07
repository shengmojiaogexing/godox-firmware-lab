"""Cortex-M0 machine: Unicorn-backed emulation of the AD200 Pro firmware."""
import os
import struct
import time
from unicorn import *
from unicorn.arm_const import *
from capstone import Cs, CS_ARCH_ARM, CS_MODE_THUMB, CS_MODE_MCLASS

from .firmware import Firmware, BASE, FLASH_BASE, FLASH_SIZE, RAM_BASE, RAM_SIZE
from .device import Device
from .peripherals import (PeripheralBus, classify_region, reg_name,
                          PERIPH_BASE, PERIPH_SIZE, GPIO_BASE, GPIO_SIZE,
                          SCS_BASE, SCS_SIZE)
from .tracer import Tracer
from .cc2500 import CC2500

EXC_RETURN = {0xFFFFFFF1, 0xFFFFFFF9, 0xFFFFFFFD}
SCB_VTOR = 0xE000ED08
RAM_VEC = 0x20000000


class StopRun(Exception):
    def __init__(self, reason):
        self.reason = reason


class Machine:
    def __init__(self, bin_path=None, device=None, symbols=None):
        self.fw = Firmware(bin_path)
        self.device = device or Device()
        self.device.cap_voltage = getattr(self.device, 'cap_voltage0', 330.0)
        self.tracer = Tracer()
        self.rf = CC2500(self.tracer)
        self._sck_prev = 1
        self._csn = 1
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
        self._map_memory()
        self.bus = PeripheralBus(self, _Mem(self.uc), self.device, self.tracer)
        self.md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_MCLASS)
        self.md.detail = False
        self.md.skipdata = True

        self.insn_count = 0
        self.cycle_count = 0
        self.state = 'paused'         # paused|running|stalled|fault|reset
        self.stop_reason = None
        self.systick_insn_per_ms = 2000   # emulated "1 ms" budget
        self.exc_active = False
        self.systick_pending = False
        self.handler_exc = None
        self.breakpoints = set()
        self.watchpoints = {}         # addr -> size
        self._code_hook = None
        self._last_pc = None
        self.max_slice_rate = 4000
        self.fast_delays = True

        self._install_hooks()
        self.reset()

    # ------------------------------------------------------------- memory
    def _map_memory(self):
        uc = self.uc
        uc.mem_map(FLASH_BASE, FLASH_SIZE)
        uc.mem_map(RAM_BASE, RAM_SIZE)
        uc.mem_map(PERIPH_BASE, PERIPH_SIZE)
        uc.mem_map(GPIO_BASE, GPIO_SIZE)
        uc.mem_map(SCS_BASE, SCS_SIZE)

    def reset(self):
        uc = self.uc
        # flash image (write through, later writes emulate programming)
        uc.mem_write(FLASH_BASE, bytes(self.fw.image))
        if self.fast_delays:
            self._patch_code()
        uc.mem_write(RAM_BASE, b'\x00' * RAM_SIZE)
        uc.mem_write(PERIPH_BASE, b'\x00' * PERIPH_SIZE)
        uc.mem_write(GPIO_BASE, b'\x00' * GPIO_SIZE)
        uc.mem_write(SCS_BASE, b'\x00' * SCS_SIZE)
        self.bus = PeripheralBus(self, _Mem(uc), self.device, self.tracer)
        self.rf = CC2500(self.tracer)
        self._sck_prev = 1
        self._csn = 1
        self.tracer.clear()
        self.insn_count = 0
        self.cycle_count = 0
        self.exc_active = False
        self.systick_pending = False
        self.handler_exc = None
        self.stop_reason = None
        self.state = 'paused'
        for r in range(13):
            uc.reg_write(UC_ARM_REG_R0 + r, 0)
        uc.reg_write(UC_ARM_REG_SP, self.fw.reset_sp)
        uc.reg_write(UC_ARM_REG_LR, 0xFFFFFFFF)
        uc.reg_write(UC_ARM_REG_PC, self.fw.reset_pc)
        try:
            uc.reg_write(UC_ARM_REG_PRIMASK, 0)
            uc.reg_write(UC_ARM_REG_IPSR, 0)
        except Exception:
            pass
        self._last_pc = self.fw.reset_pc

    def _patch_code(self):
        """WFI would stall the emulated core (no event semaphore) - turn it into NOP."""
        img = bytearray(self.fw.image)
        n = 0
        for i in range(0, min(len(img), 0xA000) - 1, 2):
            if img[i] == 0x30 and img[i + 1] == 0xBF:      # wfi
                img[i] = 0x00
                img[i + 1] = 0xBF
                n += 1
        self.uc.mem_write(FLASH_BASE, bytes(img))
        self.wfi_patched = n
        self._patch_delays()

    def on_gpio_output(self, port, odr):
        """bit-banged CC2500 SPI: CSN=PC6, SCK=PB13, MOSI=PB12, MISO=PB14"""
        if port == 'C':
            csn = (odr >> 6) & 1
            if csn != self._csn:
                self.rf.set_csn(csn)
                self._csn = csn
            return
        if port != 'B':
            return
        sck = (odr >> 13) & 1
        mosi = (odr >> 12) & 1
        if sck != self._sck_prev:
            miso = self.rf.clock(sck, mosi)
            if miso is not None:
                self.device.gpio_inputs.setdefault('B', {})[14] = int(miso)
            self._sck_prev = sck

    def _patch_delays(self):
        """busy-wait delay loops cost millions of emulated instructions; forward them
        by replacing the delay functions with an immediate return."""
        n = 0
        for addr, name in self.fw.symbols.items():
            low = name.lower()
            if name.startswith('delay') or 'delay_' in low or name in ('rcc_delay_short',):
                self.uc.mem_write(addr, struct.pack('<H', 0x4770))   # bx lr
                n += 1
        self.delays_patched = n

    # ------------------------------------------------------------- hooks
    def _install_hooks(self):
        uc = self.uc
        uc.hook_add(UC_HOOK_MEM_WRITE, self._hook_write,
                    begin=PERIPH_BASE, end=GPIO_BASE + 0x2000 - 1)
        uc.hook_add(UC_HOOK_MEM_WRITE, self._hook_write,
                    begin=SCS_BASE, end=SCS_BASE + SCS_SIZE - 1)
        uc.hook_add(UC_HOOK_MEM_WRITE, self._hook_flash_write,
                    begin=FLASH_BASE, end=FLASH_BASE + FLASH_SIZE - 1)
        uc.hook_add(UC_HOOK_MEM_READ, self._hook_read,
                    begin=PERIPH_BASE, end=GPIO_BASE + 0x2000 - 1)
        uc.hook_add(UC_HOOK_MEM_READ, self._hook_read,
                    begin=SCS_BASE, end=SCS_BASE + SCS_SIZE - 1)
        uc.hook_add(UC_HOOK_MEM_FETCH_UNMAPPED, self._hook_fetch_unmapped)

    def _hook_write(self, uc, access, address, size, value, ud):
        if classify_region(address) in ('gpio', 'scs', 'periph'):
            self.tracer.add('mmio_write', pc=uc.reg_read(UC_ARM_REG_PC),
                            addr=address, name=reg_name(address), data=value)
            self.bus.queue(address, size, value)

    def _hook_flash_write(self, uc, access, address, size, value, ud):
        self.bus.flash_data_write(address)
        self.tracer.add('flash', pc=uc.reg_read(UC_ARM_REG_PC), addr=address,
                        name='program', data=value)

    def _hook_read(self, uc, access, address, size, value, ud):
        if self.bus.pending:
            self.bus.flush()
        self.bus.on_read_pre(address)
        self.tracer.add('mmio_read', pc=uc.reg_read(UC_ARM_REG_PC),
                        addr=address, name=reg_name(address))

    def _hook_fetch_unmapped(self, uc, access, address, size, value, ud):
        if address in EXC_RETURN or (address & 0xFFFFFFF0) == 0xFFFFFFF0:
            # handler performed an exception return; unwind the stack frame
            self._exception_return()
            raise StopRun('exc_return')

    # ------------------------------------------------------------- memory API
    def read(self, addr, size):
        return self.uc.mem_read(addr, size)

    def write(self, addr, data):
        self.uc.mem_write(addr, data)

    def read32(self, addr):
        try:
            return struct.unpack('<I', self.uc.mem_read(addr & ~3, 4))[0]
        except Exception:
            return 0

    # ------------------------------------------------------------- registers
    def regs(self):
        uc = self.uc
        out = {}
        for i in range(13):
            out['r%d' % i] = uc.reg_read(UC_ARM_REG_R0 + i)
        out['sp'] = uc.reg_read(UC_ARM_REG_SP)
        out['lr'] = uc.reg_read(UC_ARM_REG_LR)
        out['pc'] = uc.reg_read(UC_ARM_REG_PC) & ~1
        try:
            out['xpsr'] = uc.reg_read(UC_ARM_REG_XPSR)
            out['primask'] = uc.reg_read(UC_ARM_REG_PRIMASK)
            out['ipsr'] = uc.reg_read(UC_ARM_REG_IPSR)
        except Exception:
            out['xpsr'] = out['primask'] = out['ipsr'] = 0
        return out

    def set_reg(self, name, value):
        uc = self.uc
        if name == 'sp':
            uc.reg_write(UC_ARM_REG_SP, value)
        elif name == 'lr':
            uc.reg_write(UC_ARM_REG_LR, value)
        elif name == 'pc':
            uc.reg_write(UC_ARM_REG_PC, value & ~1)
        elif name.startswith('r') and name[1:].isdigit():
            uc.reg_write(UC_ARM_REG_R0 + int(name[1:]), value)

    # ------------------------------------------------------------- exceptions
    def _vector_for(self, exc):
        idx = 15 if exc == -1 else (16 + exc)
        addr = RAM_VEC + 4 * idx
        vec = self.read32(addr)
        if not (vec and vec != 0xFFFFFFFF):
            addr = BASE + 4 * idx
            vec = self.read32(addr)
        return vec & ~1

    def _pending_exception(self):
        if self.exc_active:
            return None
        try:
            if self.uc.reg_read(UC_ARM_REG_PRIMASK) & 1:
                return None
        except Exception:
            pass
        if self.systick_pending:
            return -1
        for irq in range(32):
            if self.bus.irq_pending[irq] and self.bus.irq_enabled(irq):
                return irq
        return None

    def _exception_entry(self, exc):
        uc = self.uc
        handler = self._vector_for(exc)
        if not handler:
            self.bus.raise_irq(exc) if exc != -1 else None
            self.systick_pending = exc == -1
            return False
        self.exc_thread_sp = uc.reg_read(UC_ARM_REG_SP)
        sp = self.exc_frame_sp = (self.exc_thread_sp - 32) & ~7
        frame = (uc.reg_read(UC_ARM_REG_R0), uc.reg_read(UC_ARM_REG_R1),
                 uc.reg_read(UC_ARM_REG_R2), uc.reg_read(UC_ARM_REG_R3),
                 uc.reg_read(UC_ARM_REG_R12), uc.reg_read(UC_ARM_REG_LR),
                 uc.reg_read(UC_ARM_REG_PC) | 1, 0x01000000)
        uc.mem_write(sp, struct.pack('<8I', *frame))
        uc.reg_write(UC_ARM_REG_SP, sp)
        uc.reg_write(UC_ARM_REG_LR, 0xFFFFFFF9)
        uc.reg_write(UC_ARM_REG_PC, handler)
        try:
            uc.reg_write(UC_ARM_REG_IPSR, 0x10 + (1 if exc == -1 else exc + 16))
        except Exception:
            pass
        self.exc_active = True
        self.handler_exc = exc
        if exc == -1:
            self.systick_pending = False
        else:
            self.bus.irq_pending[exc] = False
        self.tracer.add('irq_enter', pc=handler,
                        name='SysTick' if exc == -1 else 'IRQ%d' % exc)
        return True

    def _exception_return(self):
        uc = self.uc
        if not self.exc_active:
            return
        sp = getattr(self, 'exc_frame_sp', None)
        if sp is None:
            sp = (uc.reg_read(UC_ARM_REG_SP) - 32) & ~7
        try:
            frame = struct.unpack('<8I', uc.mem_read(sp, 32))
        except Exception:
            self.exc_active = False
            return
        uc.reg_write(UC_ARM_REG_R0, frame[0])
        uc.reg_write(UC_ARM_REG_R1, frame[1])
        uc.reg_write(UC_ARM_REG_R2, frame[2])
        uc.reg_write(UC_ARM_REG_R3, frame[3])
        uc.reg_write(UC_ARM_REG_R12, frame[4])
        uc.reg_write(UC_ARM_REG_LR, frame[5])
        uc.reg_write(UC_ARM_REG_PC, frame[6] & ~1)
        uc.reg_write(UC_ARM_REG_SP, getattr(self, 'exc_thread_sp', sp + 32))
        try:
            uc.reg_write(UC_ARM_REG_IPSR, 0)
        except Exception:
            pass
        self.exc_active = False
        self.handler_exc = None
        self.tracer.add('irq_exit', pc=frame[6] & ~1)

    # ------------------------------------------------------------- stepping
    def _exec_chunk(self, count, use_hooks=True):
        """execute up to `count` instructions; returns number executed (estimate)"""
        pc = self.uc.reg_read(UC_ARM_REG_PC)
        try:
            self.uc.emu_start(pc | 1, 0, count=count)
        except UcError as e:
            cur = self.uc.reg_read(UC_ARM_REG_PC)
            if (cur & 0xFFFFFFF0) == 0xFFFFFFF0 and self.exc_active:
                self._exception_return()
                return max(1, count // 2)
            self.state = 'fault'
            self.stop_reason = 'fault: %s at 0x%08x' % (e, cur)
            self.tracer.add('fault', pc=cur, name=str(e))
            raise StopRun(self.stop_reason)
        return count

    def step(self, n=1):
        self.state = 'running'
        self.stop_reason = None
        try:
            for _ in range(n):
                exc = self._pending_exception()
                if exc is not None:
                    if self._exception_entry(exc):
                        self.insn_count += 1
                        continue
                self._exec_chunk(1)
                self.bus.flush()
                self.insn_count += 1
                self.bus.advance(1)
        except StopRun as e:
            self.state = 'paused' if 'breakpoint' in str(e.reason) else self.state
            raise
        self.state = 'paused'
        return self.regs()

    # ------------------------------------------------------------- run loop
    def run(self, max_insns=2_000_000, until_pc=None, stop_on_stall=True):
        if self.state == 'fault':
            self.reset()
        self.state = 'running'
        self.stop_reason = None
        start_insns = self.insn_count
        executed = 0
        stall = _StallDetector(self.tracer)
        try:
            while executed < max_insns:
                exc = self._pending_exception()
                if exc is not None and self._exception_entry(exc):
                    executed += 1
                    self.insn_count += 1
                    continue
                # breakpoints / until_pc / watchpoints: exact stepping
                pc = self.uc.reg_read(UC_ARM_REG_PC) & ~1
                if pc in self.breakpoints:
                    self.state = 'paused'
                    self.stop_reason = 'breakpoint @ 0x%08x %s' % (
                        pc, self.fw.name(pc) or '')
                    self.tracer.add('note', pc=pc, name='breakpoint')
                    return 'breakpoint'
                if until_pc is not None and pc == (until_pc & ~1):
                    self.state = 'paused'
                    self.stop_reason = 'until_pc'
                    return 'until_pc'
                chunk = min(self.max_slice_rate, max_insns - executed)
                n = self._exec_chunk(chunk)
                self.bus.flush()
                executed += n
                self.insn_count += n
                self.cycle_count += n
                self.bus.advance(n)
                new_pc = self.uc.reg_read(UC_ARM_REG_PC) & ~1
                if new_pc in EXC_RETURN and self.exc_active:
                    self._exception_return()
                    continue
                stall.feed(new_pc)
                if stop_on_stall and stall.stalled():
                    self.state = 'stalled'
                    diag = stall.diagnose(self)
                    self.stop_reason = 'stall @ 0x%08x' % new_pc
                    self.tracer.add('note', pc=new_pc, name='stall', data=diag)
                    self._write_stall_report(diag)
                    return 'stall'
        except StopRun as e:
            self.state = 'paused' if 'breakpoint' in str(e.reason) or 'until' in str(e.reason) else self.state
            return self.stop_reason or str(e.reason)
        self.state = 'paused'
        self.stop_reason = 'insn_limit'
        return 'insn_limit'

    def _write_stall_report(self, diag):
        import json
        path = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))),
                            'analysis', 'stall_report.json')
        try:
            diag['regs'] = {k: '0x%08x' % v for k, v in self.regs().items()}
            diag['mmio'] = [{'type': e['type'], 'addr': e['addr'], 'name': e['name'],
                             'pc': e['pc']} for e in self.tracer.last(200)]
            json.dump(diag, open(path, 'w', encoding='utf-8'), indent=1)
        except Exception:
            pass

    # ------------------------------------------------------------- debug utils
    def disasm(self, addr, count=16):
        addr &= ~1
        off = addr - BASE
        data = bytes(self.uc.mem_read(addr, min(count * 4 + 8, 0x1000))) if addr >= BASE else b''
        out = []
        for ins in self.md.disasm(bytes(self.uc.mem_read(addr, count * 4 + 8)), addr):
            out.append({'addr': ins.address, 'bytes': ins.bytes.hex(),
                        'text': (ins.mnemonic + ' ' + ins.op_str).strip(),
                        'symbol': self.fw.name(ins.address)})
            if len(out) >= count:
                break
        return out

    def backtrace(self, max_depth=16):
        """heuristic: return addresses on the stack that point into flash code"""
        sp = self.uc.reg_read(UC_ARM_REG_SP)
        out = [{'addr': self.uc.reg_read(UC_ARM_REG_PC) & ~1,
                'name': self.fw.name(self.uc.reg_read(UC_ARM_REG_PC) & ~1), 'current': True}]
        try:
            top = min(sp + 0x400, 0x200008B0)
            data = bytes(self.uc.mem_read(sp, max(0, top - sp)))
        except Exception:
            data = b''
        for i in range(0, len(data) - 4, 4):
            v = struct.unpack_from('<I', data, i)[0]
            if BASE <= (v & ~1) < BASE + self.fw.size and (v & 1):
                out.append({'addr': v & ~1, 'name': self.fw.name(v & ~1),
                            'stack_off': i + 4})
                if len(out) >= max_depth:
                    break
        return out

    def stats(self):
        return {
            'insns': self.insn_count,
            'cycles': self.cycle_count,
            'uptime_ms': round(self.insn_count / self.systick_insn_per_ms, 1),
            'state': self.state,
            'pc': self.uc.reg_read(UC_ARM_REG_PC) & ~1,
            'pc_name': self.fw.name(self.uc.reg_read(UC_ARM_REG_PC) & ~1),
            'exc_active': self.exc_active,
        }

    def status(self):
        regs = self.regs()
        return {
            'state': self.state,
            'stop_reason': self.stop_reason,
            'pc': regs['pc'],
            'pc_name': self.fw.name(regs['pc']),
            'regs': regs,
            'stats': self.stats(),
            'breakpoints': sorted(self.breakpoints),
            'irq': {'pending': self.bus.peripheral_state()['irq_pending'],
                    'enabled': self.bus.peripheral_state()['irq_enabled'],
                    'systick': self.bus.systick,
                    'systick_pending': self.systick_pending},
        }

    # ---------------------------------------------------- debugger API
    def set_breakpoint(self, addr):
        self.breakpoints.add(addr & ~1)

    def clear_breakpoint(self, addr):
        self.breakpoints.discard(addr & ~1)

    def breakpoints_list(self):
        return sorted(self.breakpoints)

    def trace(self, since=0, limit=500):
        return self.tracer.since(since, limit)

    def reset_trace(self):
        self.tracer.clear()

    def device_state(self):
        st = self.device.state()
        st['rf'] = self.rf.state()
        return st

    def device_set(self, **kw):
        d = self.device
        if 'battery_mv' in kw:
            d.battery_mv = float(kw['battery_mv'])
        if 'battery' in kw:
            d.battery_mv = float(kw['battery']) * 1000.0
        if 'cap_voltage' in kw:
            d.cap_voltage = float(kw['cap_voltage'])
        if 'temp_c' in kw:
            d.temp_c = float(kw['temp_c'])
        if 'temp' in kw:
            d.temp_c = float(kw['temp'])
        if 'auto_charge' in kw:
            d.auto_charge = bool(kw['auto_charge'])
        if 'charge_rate' in kw:
            d.charge_rate = float(kw['charge_rate'])
        return self.device_state()

    def device_event(self, kind, payload=None):
        payload = payload or {}
        if kind == 'button':
            self.device.press(payload.get('name') or payload.get('button'), payload.get('pressed', True))
        elif kind == 'rf_rx':
            data = payload.get('bytes') or payload.get('data') or []
            if isinstance(data, str):
                data = bytes.fromhex(data.replace(' ', ''))
            self.device.inject_rf(payload.get('channel', 0), bytes(data))
        elif kind == 'power':
            self.device.battery_mv = float(payload.get('battery_mv', self.device.battery_mv))
        return self.device_state()

    # ---------------------------------------------------- bit-banged RF slave
    def on_spi_clock(self, port, clk, mosi):
        """called from the GPIO model on every CLK edge of the bit-banged bus"""
        if not hasattr(self, '_spi'):
            self._spi = {'bits': 0, 'shift': 0, 'bytes': [], 'resp': 0, 'idx': 0}
        sp = self._spi
        if clk:
            sp['shift'] = ((sp['shift'] << 1) | mosi) & 0xFF
            sp['bits'] += 1
            if sp['bits'] == 8:
                sp['bits'] = 0
                b = sp['shift']
                sp['bytes'].append(b)
                self.tracer.add('rf_tx', addr=None, name='spi', data={'byte': b})
            # MISO bit: default 0 (chip idle / not present)
        self.device.gpio_inputs.setdefault('B', {})[15] = 0


class _Mem:
    def __init__(self, uc):
        self.uc = uc

    def read(self, addr, size):
        return self.uc.mem_read(addr, size)

    def write(self, addr, data):
        self.uc.mem_write(addr, bytes(data))


class _StallDetector:
    """Detect a hard hang: the PC stays in a tiny window *and* instructions inside
    that window keep reading the same MMIO register (a hardware wait the model does
    not satisfy).  A legitimate idle loop (e.g. main waiting for the TIM14 tick)
    runs no such polling code, so it is never flagged."""

    def __init__(self, tracer=None):
        self.pcs = []
        self.tracer = tracer

    def feed(self, pc):
        self.pcs.append(pc)
        if len(self.pcs) > 120:
            self.pcs.pop(0)

    def stalled(self):
        if len(self.pcs) < 120:
            return False
        if (max(self.pcs) - min(self.pcs)) > 0x200:
            return False
        lo, hi = min(self.pcs) - 0x40, max(self.pcs) + 0x40
        counts = {}
        for e in getattr(self.tracer, 'last', lambda n: [])(4000):
            if e['type'] == 'mmio_read' and e['pc'] and lo <= e['pc'] <= hi:
                counts[e['addr']] = counts.get(e['addr'], 0) + 1
        return bool(counts) and max(counts.values()) > 150

    def diagnose(self, machine):
        pc = machine.uc.reg_read(UC_ARM_REG_PC) & ~1
        lo, hi = pc - 0x80, pc + 0x80
        counts = {}
        for e in machine.tracer.last(2000):
            if e['type'].startswith('mmio') and e['pc'] and lo <= e['pc'] <= hi:
                key = (e['addr'], e['type'])
                counts[key] = counts.get(key, 0) + 1
        top = sorted(counts.items(), key=lambda kv: -kv[1])[:5]
        return {'pc': '0x%08x' % pc, 'pc_name': machine.fw.name(pc),
                'hot_mmio': [{'addr': '0x%08x' % a, 'type': t,
                              'name': reg_name(a), 'count': n} for (a, t), n in top]}
