# -*- coding: utf-8 -*-
"""tmr7_model.py -- minimal-intrusion TMR7 1 ms tick model for the X3-C emulator.

Why this module exists
----------------------
``devices/x3c/emu/machine.py`` already models an STM32-style ``Timer`` at
0x40001400 ("TIM7"), but that model has three gaps that keep the X3-C UI
frozen on the boot/battery screen:

1. ``Timer._tick()`` is lazy (only runs on a register read) and it never
   raises the update interrupt, i.e. nothing ever calls ``Machine.pend_irq()``.
2. ``Timer.write(offset=0x10)`` treats TMR SR as *rc_w1* (``sr &= ~value``),
   while AT32/STM32 update flags are *rc_w0* (write 0 clears a flag).  The
   firmware's ISR clears UIF with ``STR 0xFFFFFFFE, [TMR7+0x10]``; under the
   stock model that write leaves UIF set forever.
3. ``Machine._check_interrupts()`` tests ``nvic_enabled`` with
   ``(enabled >> (irq-16)) & 1``, but ``PPB.write`` stores ISER bits as
   ``enabled |= value << ((offset-0xE100)*8)`` i.e. bit *irq*.  For IRQ55 the
   ISER1 bit 23 lands in bit 55 while the dispatcher looks at bit 39, so an
   enabled IRQ55 is never dispatched.

This module fixes the above **without editing devices/x3c/emu**: it monkey-patches
``Machine._check_interrupts`` and ``Timer.write`` in the running process and
drives the TMR7 counter from the emulator's virtual instruction clock.

Firmware facts recovered from the image (see NOTES.md for the raw evidence)
---------------------------------------------------------------------------
TMR7 base 0x40001400 (literal at 0x08016FA0 / 0x08039C88), IRQ55 vector =
0x08016D51 (handler 0x08016D50).  Init (FUN_080398BA etc., called at
vtime~241806 and re-init at ~2576374):

    TMR7+0x2C ARR = 99      (period = 100 ticks)
    TMR7+0x28 PSC = 47      (divide by 48)
    TMR7+0x14 EGR = UG      (load counter)
    TMR7+0x0C DIER = 1      (update interrupt enable)
    TMR7+0x00 CR1  = 1      (start)
    NVIC ISER1 (0xE000E104) = 0x00800000  (IRQ55 enable)
    NVIC IPR[55] = 0x10

=> 48*100 = 4800 timer clocks per update event.  At a 48 MHz TMR7 clock that
is 100 us; the ISR (0x08016D50) increments a 0..9 sub-counter at
0x20000000 and every 10th event calls 0x080334EC which does

    0x2000030C = 0            (clear "tick happened" flag)
    0x20000310 += 1          (1 ms software tick, read by get_tick 0x080334D8)

so one 1 ms software tick per 4800*10 = 48000 TMR7 input clocks.

Virtual-time approximation
--------------------------
The emulator has no cycle model: ``Machine.vtime`` increments once per
instruction.  We map ``scale`` emulated instructions to one TMR7 input clock
(default 1.0).  The update IRQ therefore fires every
``(PSC+1)*(ARR+1)/scale`` instructions (4800 by default => 1 ms software tick
every 48000 instructions).  ``scale`` can be raised to make UI timeouts pass
faster for screenshot runs; the tick/ms relation seen by the firmware is
preserved, only the instruction-per-millisecond calibration changes.

Input-injection API (for the UI verification runs)
--------------------------------------------------
``inject_key()`` / ``inject_touch()`` are thin wrappers over
``devices/x3c/emu/input.py``'s ``InputController``; if the touch bus is not modelled
they can be replaced by direct RAM writes / UI-state calls (see NOTES.md).
"""
from __future__ import annotations

import os
import struct
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, "..", "..", ".."))
EMU = os.path.join(ROOT, "devices", "x3c", "emu")
if EMU not in sys.path:
    sys.path.insert(0, EMU)

import machine as _machine_mod            # noqa: E402
from machine import Machine, Timer, RCC   # noqa: E402
from unicorn.arm_const import (UC_ARM_REG_PRIMASK, UC_ARM_REG_PC,  # noqa: E402
                               UC_ARM_REG_SP, UC_ARM_REG_LR)

IMAGE = os.path.join(ROOT, "devices", "x3c", "firmware", "X3_C_v1.38_app.bin")

# --- firmware data addresses (resolved from the literal pools) -------------
TMR7_BASE = 0x40001400
TMR7_IRQ = 55
TICK_STRUCT = 0x2000030C   # +0: "tick consumed" byte, +4: 1 ms tick counter
TICK_FLAG = 0x2000030C
TICK_COUNTER = 0x20000310
MS_SUBCOUNTER = 0x20000000  # ISR increments this byte, tick every 10th
MAIN_LOOP_FLAG = 0x20000004  # (probe_ui.py used this; kept for reference)

# --- patched once per process ---------------------------------------------
_patched = False
_orig_check_interrupts = None
_orig_timer_write = None
_orig_handle_uc_error = None
_orig_rcc_write = None
_orig_rcc_read = None
_orig_take_exception = None


class Tmr7Model:
    """Drive the existing ``Timer`` instance at 0x40001400 from ``vtime``."""

    def __init__(self, machine, timer_name="TIM7", irq=TMR7_IRQ,
                 scale_num=1, scale_den=1, verbose=False):
        self.m = machine
        self.irq = irq
        self.scale_num = int(scale_num)
        self.scale_den = int(scale_den)
        self.verbose = verbose
        timer = machine.timers.get(timer_name)
        if timer is None:
            raise RuntimeError("no Timer at %s (machine.timers=%r)"
                               % (timer_name, list(machine.timers)))
        self.timer = timer
        # vtime bookkeeping
        self.last_vtime = machine.vtime
        self.acc_units = 0          # accumulated emulated instructions * num
        self.prescale_phase = 0     # phase inside the PSC divider
        # stats
        self.irq_pends = 0
        self.periods = 0
        self.advances = 0
        self.tick_seen = self.read_tick()
        self.ticks_seen = 0
        self.irq_entry_pcs = []
        self.last_irq_vtime = None
        # the stock Timer._tick would fight our CNT bookkeeping: silence it
        self._orig_tick = timer._tick
        timer._tick = lambda: None
        # trace writes so CR1/EGR/CNT changes reset our phase accumulators
        inst_write = timer.write

        def write(offset, size, value):
            inst_write(offset, size, value)
            if offset == 0x00:              # CR1
                # (re)start: resync to the current virtual time
                self.last_vtime = self.m.vtime
                self.acc_units = 0
                self.prescale_phase = 0
                self.timer.start_vtime = self.m.vtime
            elif offset == 0x14 and (value & 1):    # EGR UG
                self.acc_units = 0
                self.prescale_phase = 0
                self.last_vtime = self.m.vtime
            elif offset == 0x24:            # CNT written directly
                self.last_vtime = self.m.vtime
                self.prescale_phase = 0
            elif offset in (0x28, 0x2C):    # PSC / ARR
                self.prescale_phase = 0

        timer.write = write
        machine._tmr7 = self
        if verbose:
            print("[tmr7] attached to %s @ %#x, IRQ%d, scale %d/%d"
                  % (timer_name, timer.base, irq, scale_num, scale_den))

    # -- helpers ------------------------------------------------------
    def read_tick(self):
        try:
            return struct.unpack("<I", bytes(self.m.uc.mem_read(TICK_COUNTER, 4)))[0]
        except Exception:
            return -1

    def enabled(self):
        return bool(self.timer.cr1 & 1)

    def period(self):
        return (self.timer.arr + 1) if self.timer.arr != 0xFFFFFFFF else \
            (1 << self.timer.bits)

    def irq_counts(self):
        """One update event that the firmware will service = one IRQ."""
        return self.irq_pends

    def advance_to(self, vtime):
        """Advance CNT/UIF/pending-IRQ for the vtime delta since last call."""
        dv = vtime - self.last_vtime
        self.last_vtime = vtime
        tick_now = self.read_tick()
        if tick_now != self.tick_seen:
            self.ticks_seen += 1
            self.tick_seen = tick_now
        if dv <= 0:
            return
        if not (self.timer.cr1 & 1):
            return
        self.advances += 1
        # scale emulated instructions -> TMR7 input clocks
        self.acc_units += dv * self.scale_num
        units = self.acc_units // self.scale_den
        self.acc_units -= units * self.scale_den
        if units <= 0:
            return
        # prescaler: CNT advances every (PSC+1) input clocks
        div = (self.timer.psc + 1) & 0xFFFFFFFF
        self.prescale_phase += units
        ticks = self.prescale_phase // div
        self.prescale_phase -= ticks * div
        if ticks <= 0:
            return
        per = self.period()
        if per <= 0:
            return
        total = self.timer.cnt + ticks
        periods = total // per
        self.timer.cnt = total % per
        if periods:
            self.periods += periods
            self.timer.sr |= 1                      # UIF
            if self.timer.dier & 1:                 # update interrupt enable
                self.m.pend_irq(self.irq)
                self.irq_pends += 1
                self.last_irq_vtime = vtime
                if self.verbose and self.irq_pends <= 20:
                    print("[tmr7] pend IRQ%d at vtime=%d cnt=%d"
                          % (self.irq, vtime, self.timer.cnt))


# ==========================================================================
# TMR5 capture model (the boot power-measurement path)
# ==========================================================================
class TmrCaptureModel:
    """Inject TMR5 CH4 input-capture events (IRQ50).

    The boot power-management path (FUN_08010C8C -> FUN_08025F30) configures
    TMR5 (0x40000C00) for input capture on channel 4:

        PSC=0, CCMR2/CCER channel-4 config, CR1 enable, DIER |= 0x10 (CC4IE)
        NVIC IRQ50 enabled (FUN_08035C78(0x32,0))
        clear SR, then busy-wait for RAM[0x200000F4] == 2

    RAM[0x200000F4] is incremented by the TMR5 ISR (0x08016C00), once per
    CC4 capture event (it reads CCR4 at 0x40000C40 and clears SR bit4).  The
    captured signal comes from an external pin that is not modelled, so the
    firmware would wait forever.

    This model injects a capture every time the software has armed CC4IE and
    the previous CC4IF has been cleared by the ISR; CCR4 is filled with a
    deterministic ramp value.  The boot code only needs the two interrupts
    (it does not use the captured values for the decision), and the same
    "signal absent" behaviour is a known emulation limitation, documented in
    NOTES.md.
    """

    def __init__(self, machine, timer_name="TIM5", irq=50, ccif=0x10,
                 gap=1000, wait_pc=(0x08025FA4, 0x08025FA8), verbose=False):
        self.m = machine
        timer = machine.timers.get(timer_name)
        if timer is None:
            raise RuntimeError("no Timer %s" % timer_name)
        self.timer = timer
        self.irq = irq
        self.ccif = ccif
        self.gap = gap
        self.wait_pc = wait_pc
        self.verbose = verbose
        self.last_vtime = machine.vtime
        self.last_capture = machine.vtime - gap
        self.events = 0
        self.pends = 0

    def _cpu_is_waiting(self):
        """Only emulate the external capture signal while the firmware is in
        FUN_08025F30's busy-wait (``ldr [r4,#0x14]; cmp #2; bne`` at
        0x08025FA4..0x08025FA8).  TMR5 stays armed in other code paths too;
        injecting there would be an interrupt storm and corrupt the UI run."""
        if self.wait_pc is None:
            return True
        lo, hi = self.wait_pc
        return lo <= self.m.last_pc <= hi

    def advance_to(self, vtime):
        t = self.timer
        dv = vtime - self.last_vtime
        self.last_vtime = vtime
        if dv <= 0:
            return
        if not (t.cr1 & 1):                 # counter off
            return
        if not (t.dier & 0x10):             # CC4 interrupt not armed
            return
        if not self._cpu_is_waiting():      # only while the firmware waits
            return
        if t.sr & self.ccif:                # previous capture not serviced
            return
        if vtime - self.last_capture < self.gap:   # keep the IRQ rate sane
            return
        self.last_capture = vtime
        self.events += 1
        # deterministic captured pulse: ch4 compare value ramp
        t.ccr[3] = (0x0800 + (self.events & 7) * 0x137) & 0xFFFF
        t.sr |= self.ccif
        self.m.pend_irq(self.irq)
        self.pends += 1
        if self.verbose and self.pends <= 10:
            print("[tmr5cap] capture #%d at vtime=%d CCR4=%#x"
                  % (self.events, vtime, t.ccr[3]))


def _advance_models(machine):
    for tm in getattr(machine, "_timer_models", ()):
        tm.advance_to(machine.vtime)


# ==========================================================================
# monkey patches (process-local only; devices/x3c/emu files stay untouched)
# ==========================================================================
def _fixed_check_interrupts(self):
    """Stock dispatch + (a) advance timer models (b) corrected NVIC mapping."""
    _advance_models(self)
    if self.active_exception is not None:
        return False
    if not self.pending_irqs:
        return False
    primask = 0
    try:
        primask = self.uc.reg_read(UC_ARM_REG_PRIMASK)
    except Exception:
        pass
    if primask:
        return False
    # PPB.write stores ISER/ICER bits as "bit == irq number"; the stock
    # dispatcher wrongly subtracts 16.  Use the stored convention.
    enabled = self.ppb.nvic_enabled
    cands = [i for i in self.pending_irqs
             if i >= 16 and ((enabled >> i) & 1)]
    if not cands:
        return False
    # Cortex-M: lowest priority value first, ties -> lowest IRQ number
    def prio(irq):
        row = self.ppb.nvic_prio.get(irq // 4)
        return row[irq & 3] if row else 0
    cands.sort(key=lambda i: (prio(i), i))
    irq = cands[0]
    self.pending_irqs.discard(irq)
    self.ppb.nvic_pending = self.ppb._pending_mask()
    # Machine.take_exception(exc_num) indexes the vector table with exc_num, so
    # an external IRQ n must be passed as exception number n+16 (vector 16+n).
    # (The stock dispatcher passes the IRQ number directly -> every external
    # IRQ lands 16 vectors early, i.e. on the default "b ." handler.)
    if not self.take_exception(irq + 16):
        return False
    tmr = getattr(self, "_tmr7", None)
    if tmr is not None:
        tmr.irq_entry_pcs.append((self.vtime, irq,
                                  self.uc.reg_read(_machine_mod.UC_ARM_REG_PC)))
    return True


def _fixed_timer_write(self, offset, size, value):
    """AT32/STM32 TMR status flags are rc_w0 (write 0 clears)."""
    if offset == 0x10:
        self.sr &= value
        return
    return _orig_timer_write(self, offset, size, value)


def _fixed_rcc_write(self, offset, size, value):
    """RCC CFGR must honour SW=0 (switch back to HSI).

    The stock model keeps the previous SW field when a zero is written:
        self.cfgr = (self.cfgr & ~3) | (value & 3) if (value & 3) else self.cfgr
    so the clock-init poll at 0x0801BDBC (``tst CFGR,#0xC / bne``) spins for
    ever after the clock has once been switched to PLL.  Store CFGR as-is.
    """
    if offset == 0x08:
        self.cfgr = value & 0xFFFFFFFF
        return
    if offset == 0x74:                 # CSR: keep LSION/RTCSEL/RTCEN
        self.csr = (self.csr & 0xFE000000) | (value & 0x0001FFFF)
        return
    return _orig_rcc_write(self, offset, size, value)


def _fixed_rcc_read(self, offset, size):
    """CFGR SWS (bits 2:3) must mirror SW (bits 1:0), not OR onto the value.

    CSR (0x74): the RTC/LSI init at 0x080203EC polls LSIRDY (bit1) after
    enabling LSI; the stock model never raises it, which leaves the firmware
    spinning with interrupts disabled (cpsid i) forever.
    """
    if offset == 0x08:
        return (self.cfgr & ~0xC) | ((self.cfgr & 0x3) << 2)
    if offset == 0x74:
        v = self.csr | 0x2            # LSIRDY once LSI is involved
        if self.csr & 0x80:           # RTCSEL != 0 -> LSE path
            v |= 0x2
        return v
    return _orig_rcc_read(self, offset, size)


def _fixed_handle_uc_error(self, e):
    """Catch the EXC_RETURN trap that Unicorn reports as UC_ERR_EXCEPTION.

    ``Machine.take_exception`` returns from an ISR by setting LR=0xFFFFFFF9;
    when the handler does ``bx lr`` Unicorn 2.1.4 raises UC_ERR_EXCEPTION at
    PC=0xFFFFFFF8 instead of delivering a fetch-unmapped hook (the stock
    ``_hook_fetch_unmapped`` EXC_RETURN path therefore never runs).  Perform
    the manual unstack here and keep running.
    """
    try:
        pc = self.uc.reg_read(UC_ARM_REG_PC) & 0xFFFFFFFF
    except Exception:
        pc = 0
    if self.active_exception is not None and (pc & 0xFFFFFF00) == 0xFFFFFF00:
        if self._exception_return(pc | 1):
            self.stop_reason = None
            return True
    return _orig_handle_uc_error(self, e)


def install(verbose=False):
    """Install the monkey patches once; safe to call repeatedly."""
    global _patched, _orig_check_interrupts, _orig_timer_write
    global _orig_handle_uc_error, _orig_rcc_write, _orig_rcc_read
    if _patched:
        return
    _orig_check_interrupts = Machine._check_interrupts
    _orig_timer_write = Timer.write
    _orig_handle_uc_error = Machine._handle_uc_error
    _orig_rcc_write = RCC.write
    _orig_rcc_read = RCC.read
    Machine._check_interrupts = _fixed_check_interrupts
    Timer.write = _fixed_timer_write
    Machine._handle_uc_error = _fixed_handle_uc_error
    RCC.write = _fixed_rcc_write
    RCC.read = _fixed_rcc_read
    _patched = True
    if verbose:
        print("[tmr7] patched Machine._check_interrupts + Timer.write "
              "(rc_w0 status, corrected NVIC bit mapping, IRQ vector n+16) "
              "+ Machine._handle_uc_error (EXC_RETURN trap) "
              "+ RCC CFGR (honour SW=0 / SWS mirror)")


def attach(machine, scale_num=1, scale_den=1, verbose=False,
           tmr5_capture=True):
    """Install patches and attach the TMR7 (+ optional TMR5 capture) models."""
    install(verbose=verbose)
    model = Tmr7Model(machine, scale_num=scale_num, scale_den=scale_den,
                      verbose=verbose)
    models = [model]
    if tmr5_capture:
        try:
            models.append(TmrCaptureModel(machine, verbose=verbose))
        except Exception as e:
            if verbose:
                print("[tmr7] TMR5 capture model unavailable: %r" % (e,))
    machine._timer_models = models
    return model


# ==========================================================================
# "real device" accommodations already used by devices/x3c/emu/demo_x3.py
# ==========================================================================
def patch_option_bytes(m):
    """The real device has already been through the one-time option-byte
    provisioning, so 0x1FFFC002=0x000F and 0x1FFFC010=0x0002."""
    try:
        m.uc.mem_write(0x1FFFC000, b"\x00" * 0x1000)
        m.uc.mem_write(0x1FFFC002, struct.pack("<H", 0x000F))
        m.uc.mem_write(0x1FFFC010, struct.pack("<H", 0x0002))
    except Exception:
        pass


class X3AdcBlock:
    """Battery ADC stub for the AT32 ADC in this firmware.

    The stock demo patch returned STS=0x42, which satisfies the ADRDY poll
    (bit 0x40) in the ADC init (FUN_08035280) but leaves the *interrupt*
    handler (IRQ18, FUN_0800510C) doing nothing: it tests STS bits 0x20 and
    0x04, and only on bit 0x04 reads the data registers 0x3C/0x40 and stores
    the two channel values to RAM 0x20000168/0x2000016A - the battery raw
    reading used by the power-management code.  Without it ``ch0*1000/ch1``
    is 0, the firmware decides the battery is dead and powers off
    (FUN_08010C00 WFI loop) instead of continuing to the home screen.

    Model:
      * STS = 0x64 (ADRDY | EOC-ish bits seen by the driver) so both the init
        poll and the IRQ handler branches complete;
      * data registers 0x3C/0x40 return a fixed ADC raw value (default 2600:
        ch0 == ch1 => battery ratio 1000 permille > 0x28A threshold);
      * starting a conversion (CR bit22, FUN_08018E46) pends IRQ18.
    """

    name = "ADC"

    def __init__(self, machine, raw=2600):
        self.m = machine
        self.base = 0x40012000
        self.size = 0x100
        self.raw = raw
        self.regs = {}
        self.conversions = 0

    def read(self, offset, size):
        off = offset & ~3
        if off == 0x00:
            return 0x64
        if off == 0x08:
            # CTRL2: ADCAL(bit3) and bit2 self-clear when the operation is
            # done, ADSTART(bit22) self-clears after the conversion.
            return self.regs.get(off, 0) & ~((1 << 22) | 0x0C)
        if off in (0x3C, 0x40, 0x44, 0x48, 0x4C):
            return self.raw & 0xFFFF
        return self.regs.get(off, 0)

    def write(self, offset, size, value):
        off = offset & ~3
        self.regs[off] = value
        if off == 0x08 and (value & (1 << 22)):     # ADSTART -> EOC interrupt
            self.conversions += 1
            self.m.pend_irq(18)                     # ADC1_2_3_IRQn


def patch_adc(m, raw=2600):
    p = m.find_peripheral(0x40012000)
    dev = X3AdcBlock(m, raw)
    if p is None:
        m.add_peripheral(dev)
        p = dev
    else:
        p.read = dev.read
        p.write = dev.write
        p.name = "ADC"
        p.adc = dev
    m.adc = p
    return dev


def patch_delays(m):
    """Short-circuit the two calibrated nop-sled software delays
    (0x0801C118 us, 0x0801C0FE ms) exactly like devices/x3c/emu/demo_x3.py: on real
    hardware they burn millions of cycles; their duration is not modelled."""
    m.uc.mem_write(0x0801C118, b"\x70\x47")   # bx lr
    m.uc.mem_write(0x0801C0FE, b"\x70\x47")


def patch_real_device(m, adc_raw=2600):
    patch_option_bytes(m)
    patch_adc(m, adc_raw)
    patch_delays(m)


# --------------------------------------------------------------------------
if __name__ == "__main__":
    print(__doc__)
