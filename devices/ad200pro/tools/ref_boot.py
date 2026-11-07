#!/usr/bin/env python3
"""独立最小启动参考实现（只验证「能跑到主循环」）。

本脚本 deliberately 不 import sim/，用于给 sim/ 主实现做交叉验证：
    python tools/ref_boot.py [--max-insns N] [--after N] [--json PATH]

实现要点（与 docs/06-boot-peripherals.md 对应）：
  * Unicorn 2.1.4 / ARM Cortex-M0：必须 UC_MODE_THUMB|UC_MODE_MCLASS 且每次
    emu_start 的 start 地址带 Thumb 位（pc|1）；MCLASS 才解码 `msr primask`。
  * 外设 = 普通内存后备 + 读钩子「预写」canonical 值 + 写钩子副作用。
  * SysTick 1 instr ≈ 1 cycle；异常注入压 8 字帧、LR=0xFFFFFFF9；
    `bx lr` / `pop {..,pc}` 得到 EXC_RETURN 时在代码钩子里弹栈返回。
  * 未建模寄存器：写后原值读回（permissive register file），保证固件不因
    「假的 ready/标志位」死等。
"""
import argparse
import collections
import json
import os
import struct
import sys

from unicorn import (
    Uc, UC_ARCH_ARM, UC_MODE_THUMB, UC_MODE_MCLASS,
    UC_HOOK_CODE, UC_HOOK_MEM_READ, UC_HOOK_MEM_WRITE,
)
from unicorn.arm_const import (
    UC_ARM_REG_R0, UC_ARM_REG_R1, UC_ARM_REG_R2, UC_ARM_REG_R3,
    UC_ARM_REG_R4, UC_ARM_REG_R5, UC_ARM_REG_R6, UC_ARM_REG_R7,
    UC_ARM_REG_R8, UC_ARM_REG_R9, UC_ARM_REG_R10, UC_ARM_REG_R11,
    UC_ARM_REG_R12, UC_ARM_REG_SP, UC_ARM_REG_LR, UC_ARM_REG_PC,
    UC_ARM_REG_XPSR, UC_ARM_REG_PRIMASK,
)

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BIN = os.path.join(ROOT, "firmware", "AD200pro_V1.60.bin")
IMAGE_BASE = 0x08003000
# 注意：固件会在 0x0801F000（128KiB flash 的最后一页）读写参数区，
# 因此 flash 必须映射到 0x08020000，而不是镜像尾 0x08010000。
FLASH_BASE, FLASH_SIZE = 0x08000000, 0x20000
RAM_BASE, RAM_SIZE = 0x20000000, 0x2000
PER_BASE, PER_SIZE = 0x40000000, 0x40000
GPIO_BASE, GPIO_SIZE = 0x48000000, 0x2000
SCS_BASE, SCS_SIZE = 0xE0000000, 0x100000
VECT_RAM = 0x20000000
MAIN_LO, MAIN_HI = 0x0800CE40, 0x0800CEA4     # 主循环（含入口）
MAIN = 0x0800CDB0
EXC_RETURNS = {0xFFFFFFF1, 0xFFFFFFF9, 0xFFFFFFFD}
R14 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]

REG_NAMES = {
    0x40021000: "RCC_CR", 0x40021004: "RCC_CFGR", 0x40021008: "RCC_CIR",
    0x40021014: "RCC_AHBENR", 0x40021018: "RCC_APB2ENR", 0x4002101C: "RCC_APB1ENR",
    0x40021020: "RCC_BDCR", 0x40021024: "RCC_CSR", 0x4002102C: "RCC_CFGR2",
    0x40021030: "RCC_CFGR3", 0x40021034: "RCC_CR2",
    0x40010000: "SYSCFG_CFGR1", 0x40010400: "EXTI_IMR", 0x40010414: "EXTI_PR",
    0x40012400: "ADC_ISR", 0x40012408: "ADC_CR", 0x40012440: "ADC_DR",
    0x40022000: "FLASH_ACR", 0x4002200C: "FLASH_SR", 0x40022010: "FLASH_CR",
    0x40020000: "DMA_ISR", 0x40020004: "DMA_IFCR",
    0xE000E010: "SYST_CSR", 0xE000E014: "SYST_RVR", 0xE000E018: "SYST_CVR",
    0xE000E100: "NVIC_ISER", 0xE000E180: "NVIC_ICER", 0xE000E200: "NVIC_ISPR",
    0xE000E280: "NVIC_ICPR", 0xE000ED04: "SCB_ICSR", 0xE000ED08: "SCB_VTOR",
    0xE000ED10: "SCB_SCR", 0xE000ED1C: "SCB_SHPR2", 0xE000ED20: "SCB_SHPR3",
}


def align4(a):
    return a & ~3


def rw32(uc, addr):
    return struct.unpack("<I", uc.mem_read(align4(addr), 4))[0]


class RefBoot:
    def __init__(self, bin_path=BIN, max_insns=5_000_000, after=300_000,
                 verbose=True, cycles_per_insn=1):
        self.bin_path = bin_path
        self.max_insns = max_insns
        self.after = after
        self.verbose = verbose
        self.cycles_per_insn = max(1, int(cycles_per_insn))
        raw = open(bin_path, "rb").read()
        self.raw = raw
        self.msr_addrs = self._scan_msr_primask()
        self.uc = Uc(UC_ARCH_ARM, UC_MODE_THUMB | UC_MODE_MCLASS)
        self.uc.mem_map(FLASH_BASE, FLASH_SIZE)
        self.uc.mem_map(RAM_BASE, RAM_SIZE)
        self.uc.mem_map(PER_BASE, PER_SIZE)
        self.uc.mem_map(GPIO_BASE, GPIO_SIZE)
        self.uc.mem_map(SCS_BASE, SCS_SIZE)
        self.uc.mem_write(FLASH_BASE, b"\xff" * FLASH_SIZE)
        self.uc.mem_write(IMAGE_BASE, raw)
        self.uc.mem_write(RAM_BASE, b"\x00" * RAM_SIZE)
        self.reg = {}                       # canonical register store
        self.insn = 0
        self.cycles = 0
        self.ticks = 0
        self.irq_serial = 0
        self.ipsr = 0
        self.st_pending = False
        self.pending = {}                   # irq -> True (external)
        self.countflag = 0
        self.mmio = collections.deque(maxlen=200)
        self.read_pc = collections.Counter()      # (addr, pc) -> 次数
        self.pc_ring = collections.deque(maxlen=512)
        self.stalls = []
        self.reached = False
        self.reached_at = None
        self.loop_entries = 0
        self.in_loop_zone = False
        self.stop_reason = ""
        self.last_pc = 0
        self.cand_cache = {}
        self.trace_on = True
        self.auto_unstick = True
        self._install()
        self.reset()

    def _scan_msr_primask(self):
        """扫描 flash 里的 `msr primask, rX`（Thumb-2 32 位指令）地址集合。

        用于缓存 PRIMASK：只在改写 PRIMASK 后重读，避免每条指令都查 unicorn
        寄存器（实测那样会把 ~3M insn/s 拖到 ~0.3M insn/s）。"""
        addrs = set()
        d = self.raw
        for i in range(0, len(d) - 3, 2):
            # MSR 特殊寄存器：0xF380 0x88xx（primask/control/...）
            if d[i] == 0x80 and d[i + 1] == 0xF3 and d[i + 3] == 0x88:
                addrs.add(IMAGE_BASE + i)
        return addrs

    # ---------------------------------------------------------------- setup
    def _install(self):
        self.uc.hook_add(UC_HOOK_CODE, self._on_code)
        fr = self.uc.hook_add(UC_HOOK_MEM_READ, self._on_read,
                              begin=PER_BASE, end=SCS_BASE + SCS_SIZE - 1)
        self._h_read = fr
        self.uc.hook_add(UC_HOOK_MEM_WRITE, self._on_write,
                         begin=PER_BASE, end=SCS_BASE + SCS_SIZE - 1)
        self.reset()

    def reset(self):
        self.uc.reg_write(UC_ARM_REG_PC, struct.unpack_from("<I", self.raw, 4)[0] & ~1)
        self.uc.reg_write(UC_ARM_REG_SP, struct.unpack_from("<I", self.raw, 0)[0])
        for r in R14:
            self.uc.reg_write(UC_ARM_REG_R0 + r, 0)
        self.uc.reg_write(UC_ARM_REG_LR, 0xFFFFFFFF)
        self.uc.reg_write(UC_ARM_REG_PRIMASK, 0)
        self.uc.reg_write(UC_ARM_REG_XPSR, 0x01000000)
        self.reg.clear()
        self.insn = 0
        self.ticks = 0
        self.irq_serial = 0
        self.ipsr = 0
        self.st_pending = False
        self.pending.clear()
        self.countflag = 0
        self.mmio.clear()
        self.read_pc.clear()
        self.pc_ring.clear()
        self.reached = False
        self.reached_at = None
        self.loop_entries = 0
        self.in_loop_zone = False
        self.stop_reason = ""
        self.pm = 0
        self.pm_dirty = True

    # ------------------------------------------------------- MMIO read/write
    def _on_read(self, uc, access, addr, size, value, user):
        base = align4(addr)
        canon = self.canon(base)
        if canon is not None:
            uc.mem_write(base, struct.pack("<I", canon & 0xFFFFFFFF))
        if self.trace_on:
            pc = self.uc.reg_read(UC_ARM_REG_PC)
            self.mmio.append(("r", addr, pc))
            self.read_pc[(base, pc)] += 1

    def _on_write(self, uc, access, addr, size, value, user):
        base = align4(addr)
        self.reg[base] = value & 0xFFFFFFFF
        self.side_write(base, value & 0xFFFFFFFF)
        if self.trace_on:
            self.mmio.append(("w", addr, self.uc.reg_read(UC_ARM_REG_PC)))

    def canon(self, a):
        if SCS_BASE <= a < SCS_BASE + SCS_SIZE:
            return self.canon_scs(a)
        if a == 0x40021000:                      # RCC_CR
            v = self.reg.get(a, 0)
            v |= 1 if v & 1 else 0               # HSION -> HSIRDY
            v |= (1 << 1) if v & 0 else 0
            if v & (1 << 16):                    # HSEON -> HSERDY
                v |= (1 << 17)
            if v & (1 << 24):                    # PLLON -> PLLRDY
                v |= (1 << 25)
            self.reg[a] = v
            return v
        if a == 0x40021004:                      # RCC_CFGR: SWS=SW
            v = self.reg.get(a, 0)
            v = (v & ~0xC) | ((v & 3) << 2)
            return v
        if a == 0x40021024:                      # RCC_CSR: LSION -> LSIRDY
            v = self.reg.get(a, 0)
            if v & 1:
                v |= (1 << 1)
            self.reg[a] = v
            return v
        if a == 0x40021020:                      # RCC_BDCR: LSEON -> LSERDY
            v = self.reg.get(a, 0)
            if v & 1:
                v |= (1 << 1)
            self.reg[a] = v
            return v
        if a == 0x4000280C:                      # RTC_ISR
            # ALRAWF/ALRBWF/WUTWF 常 1；RSF(bit5) 常 1；INIT(bit6)->INITF(bit7)
            v = self.reg.get(a, 0)
            v |= 0x07 | (1 << 5)
            if v & (1 << 6):
                v |= (1 << 7)
            else:
                v &= ~(1 << 7)
            self.reg[a] = v
            return v
        if a == 0x40002808:                      # RTC_CR: 写什么读什么
            return self.reg.get(a, 0)
        if a == 0x40012400:                      # ADC_ISR
            v = self.reg.get(a, 0)
            if self.reg.get(0x40012408, 0) & 1:  # ADEN -> ADRDY(bit0)
                v |= 1
            return v
        return self.reg.get(a, 0)

    def canon_scs(self, a):
        if a == 0xE000E010:                      # SysTick CTRL (+COUNTFLAG)
            v = self.reg.get(a, 0) & 0xFFFF
            if self.countflag:
                v |= (1 << 16)
            self.countflag = 0
            return v
        if a == 0xE000E018:                      # SysTick VAL
            return self.reg.get(a, 0) & 0xFFFFFF
        if a == 0xE000E100:                      # NVIC ISER
            return self.reg.get(a, 0)
        if a == 0xE000E104:
            return 0
        if a == 0xE000ED04:                      # ICSR: VECTACTIVE only
            return self.ipsr & 0x1FF
        return self.reg.get(a, 0)

    def side_write(self, a, v):
        if a == 0xE000E010:                      # SysTick CTRL
            self.reg[a] = v & 0xFFFF
            if not (v & 1):
                self.st_pending = False
        elif a == 0xE000E014:                    # SysTick RVR
            self.reg[a] = v & 0xFFFFFF
        elif a == 0xE000E018:                    # SysTick CVR: write clears
            self.reg[a] = 0
            self.countflag = 0
        elif a == 0xE000E100:                    # NVIC ISER
            self.reg[a] = self.reg.get(a, 0) | v
        elif a == 0xE000E180:                    # NVIC ICER
            self.reg[0xE000E100] = self.reg.get(0xE000E100, 0) & ~v
        elif a == 0xE000E200:                    # NVIC ISPR
            for i in range(32):
                if v & (1 << i):
                    self.pending[i] = True
        elif a == 0xE000E280:                    # NVIC ICPR
            for i in range(32):
                if v & (1 << i):
                    self.pending.pop(i, None)
        elif a == 0x40012400:                    # ADC_ISR: 写 1 清标志
            self.reg[a] = self.reg.get(a, 0) & ~v & ~1
        elif a == 0x40012408:                    # ADC_CR
            self.reg[a] = v
            if v & 1:                            # ADEN -> ADRDY
                self.reg[0x40012400] = self.reg.get(0x40012400, 0) | 1
            if v & 4:                            # ADSTART -> 立即转换
                self._adc_convert()
        elif a == 0x40012440:                    # ADC_DR 读清 EOC（写忽略）
            self.reg[a] = v & 0xFFFF
        elif a == 0x40020004:                    # DMA_IFCR: 写 1 清
            self.reg[0x40020000] = self.reg.get(0x40020000, 0) & ~v

    def _adc_convert(self):
        chsel = self.reg.get(0x40012428, 0)
        ch = 0
        while ch < 19 and not (chsel & (1 << ch)):
            ch += 1
        if ch >= 19:
            ch = 0
        # 通道默认值：CH0 电池、CH1 电容、CH2 温度，其余中点
        vals = {0: 0x7D0, 1: 0xD00, 2: 0x640}
        self.reg[0x40012440] = vals.get(ch, 0x800)
        self.reg[0x40012400] = self.reg.get(0x40012400, 0) | 4 | 8   # EOC|EOS
        # 单次转换完成后 ADSTART 由硬件清零（CONT 模式保持）
        if not (self.reg.get(0x4001240C, 0) & (1 << 13)):
            self.reg[0x40012408] = self.reg.get(0x40012408, 0) & ~4

    # ------------------------------------------------------------ code hook
    def _on_code(self, uc, addr, size, user):
        self.insn += 1
        # SysTick tick (默认 1 instruction == CYCLES_PER_INSN cycles)
        self.cycles += self.cycles_per_insn
        csr = self.reg.get(0xE000E010, 0)
        if csr & 1:
            cvr = self.reg.get(0xE000E018, 0) - self.cycles_per_insn
            if cvr <= 0:
                rvr = self.reg.get(0xE000E014, 0)
                self.reg[0xE000E018] = rvr + cvr
                self.countflag = 1
                self.ticks += 1
                if csr & 2:
                    self.st_pending = True
            else:
                self.reg[0xE000E018] = cvr & 0xFFFFFF
        # 记录 PC 窗口 / 主循环进入
        self.pc_ring.append(addr)
        self.last_pc = addr
        in_zone = MAIN_LO <= addr < MAIN_HI
        if in_zone and not self.in_loop_zone:
            self.loop_entries += 1
            if not self.reached:
                self.reached = True
                self.reached_at = self.insn
                if self.verbose:
                    print("[ref] main loop reached at insn=%d pc=%08X ticks=%d" % (
                        self.insn, addr, self.ticks))
        self.in_loop_zone = in_zone
        # 异常返回（在 handler 内才检查；返回后本 hook 立即结束，
        # 不能在同一次 hook 里再做注入，否则压栈的返回地址会错）
        if self.ipsr:
            self._check_exception_return(addr)
            return
        if addr in self.msr_addrs:               # msr primask：下次决策前重读
            self.pm_dirty = True
        # 异常注入（线程模式边界）
        pend = None
        if self.st_pending:
            pend = 15
        elif self.pending:
            en = self.reg.get(0xE000E100, 0)
            for irq in sorted(self.pending):
                if en & (1 << irq):
                    pend = 16 + irq
                    break
        if pend is not None:
            if self.pm_dirty:
                self.pm = uc.reg_read(UC_ARM_REG_PRIMASK)
                self.pm_dirty = False
            if self.pm == 0:
                self._inject(pend, addr)
                return
        if self.insn >= self.max_insns:
            self.stop_reason = "insn_limit"
            uc.emu_stop()

    # -------------------------------------------------------- exception core
    def _vector_base(self):
        cfgr1 = self.reg.get(0x40010000, 0)
        return VECT_RAM if (cfgr1 & 3) == 3 else FLASH_BASE + 0x3000

    def _vector(self, idx):
        tbl = self._vector_base()
        try:
            v = rw32(self.uc, tbl + idx * 4)
        except Exception:
            v = 0
        if v in (0, 0xFFFFFFFF) or not (FLASH_BASE <= (v & ~1) < 0x08010000):
            try:
                v = rw32(self.uc, IMAGE_BASE + idx * 4)
            except Exception:
                v = 0
        return v

    def _inject(self, exc_num, at_pc):
        if exc_num == 15:
            idx = 15
            name = "SysTick"
            self.st_pending = False
        else:
            irq = exc_num - 16
            idx = 16 + irq
            name = "IRQ%d" % irq
            self.pending.pop(irq, None)
        vector = self._vector(idx)
        if vector in (0, 0xFFFFFFFF):
            vector = 0x080030E7
        uc = self.uc
        # ARMv6-M：SP 先减 32，再对齐到 8 字节（帧压在 SP_new..SP_new+28）
        sp = (uc.reg_read(UC_ARM_REG_SP) - 32) & ~7
        xpsr = uc.reg_read(UC_ARM_REG_XPSR)
        frame = [
            uc.reg_read(UC_ARM_REG_R0), uc.reg_read(UC_ARM_REG_R1),
            uc.reg_read(UC_ARM_REG_R2), uc.reg_read(UC_ARM_REG_R3),
            uc.reg_read(UC_ARM_REG_R12), uc.reg_read(UC_ARM_REG_LR),
            at_pc, xpsr,
        ]
        uc.mem_write(sp, b"".join(struct.pack("<I", w & 0xFFFFFFFF) for w in frame))
        uc.reg_write(UC_ARM_REG_SP, sp)
        uc.reg_write(UC_ARM_REG_LR, 0xFFFFFFF9)
        uc.reg_write(UC_ARM_REG_PC, vector)
        self.ipsr = exc_num + 16
        self.irq_serial += 1

    def _unstack(self, frame_sp):
        uc = self.uc
        words = [rw32(uc, frame_sp + 4 * i) for i in range(8)]
        uc.reg_write(UC_ARM_REG_R0, words[0])
        uc.reg_write(UC_ARM_REG_R1, words[1])
        uc.reg_write(UC_ARM_REG_R2, words[2])
        uc.reg_write(UC_ARM_REG_R3, words[3])
        uc.reg_write(UC_ARM_REG_R12, words[4])
        uc.reg_write(UC_ARM_REG_LR, words[5])
        uc.reg_write(UC_ARM_REG_SP, frame_sp + 32)
        uc.reg_write(UC_ARM_REG_PC, words[6])
        try:
            uc.reg_write(UC_ARM_REG_XPSR, words[7])
        except Exception:
            pass
        self.ipsr = 0

    def _half(self, addr):
        try:
            b = self.uc.mem_read(addr, 2)
            return struct.unpack("<H", b)[0]
        except Exception:
            return 0xFFFF

    def _check_exception_return(self, addr):
        kind = self.cand_cache.get(addr)
        if kind is None:
            h = self._half(addr)
            if h == 0x4770:
                kind = 1                       # bx lr
            elif (h & 0xFF00) == 0xBD00:
                kind = 2                       # pop {..,pc}
            else:
                kind = 0
            self.cand_cache[addr] = kind
        uc = self.uc
        if kind == 1:
            lr = uc.reg_read(UC_ARM_REG_LR)
            if lr in EXC_RETURNS:
                self._unstack(uc.reg_read(UC_ARM_REG_SP))
        elif kind == 2:
            h = self._half(addr)
            regs = [i for i in range(9) if h & (1 << i)]   # r0..r7, pc(bit8)
            sp = uc.reg_read(UC_ARM_REG_SP)
            n = len(regs)
            if n == 0:
                return
            pc_slot = rw32(uc, sp + 4 * (n - 1))
            if pc_slot in EXC_RETURNS:
                for i, r in enumerate(regs[:-1]):
                    uc.reg_write(UC_ARM_REG_R0 + r, rw32(uc, sp + 4 * i))
                self._unstack(sp + 4 * n)

    # ------------------------------------------------------------- run loop
    def run(self):
        slice_insns = 20000
        last_stall_window = 0
        last_progress = 0
        unstick_tries = 0
        while self.insn < self.max_insns:
            if self.reached and self.after and self.insn > self.reached_at + self.after:
                self.stop_reason = "after"
                break
            try:
                pc = self.uc.reg_read(UC_ARM_REG_PC)
                self.uc.emu_start(pc | 1, 0, 0, slice_insns)
            except Exception as e:  # UcError
                self.stop_reason = "fault: %s" % e
                self.fault = {
                    "pc": "0x%08X" % self.uc.reg_read(UC_ARM_REG_PC),
                    "lr": "0x%08X" % self.uc.reg_read(UC_ARM_REG_LR),
                    "sp": "0x%08X" % self.uc.reg_read(UC_ARM_REG_SP),
                    "primask": self.uc.reg_read(UC_ARM_REG_PRIMASK),
                    "shadow_ipsr": self.ipsr,
                    "insn": self.insn,
                    "last_pcs": ["0x%08X" % a for a in list(self.pc_ring)[-24:]],
                }
                if self.verbose:
                    print("[ref] FAULT %s %s" % (e, json.dumps(self.fault, ensure_ascii=False)))
                break
            if self.verbose and self.insn - last_progress >= 1_000_000:
                last_progress = self.insn
                print("[ref] insn=%d pc=%08X ticks=%d reached=%s" % (
                    self.insn, self.last_pc, self.ticks, self.reached))
            if self.insn - last_stall_window >= 20000:
                last_stall_window = self.insn
                st = self._detect_stall()
                if st is not None:
                    self.stalls.append(st)
                    if self.verbose:
                        print("[ref] STALL pc=%08X window=%s polls=%s regs=%s" % (
                            st["pc"], st["window"], st["polls"], st["regs"]))
                    self._resolve(st)
                    unstick_tries += 1
                    if unstick_tries > 20:
                        self.stop_reason = "stall"
                        break
        if not self.stop_reason:
            self.stop_reason = "insn_limit"
        return self.report()

    def _detect_stall(self):
        if len(self.pc_ring) < 256:
            return None
        pcs = list(self.pc_ring)
        distinct = set(pcs)
        if len(distinct) > 4:
            return None
        reads = [(a, p) for (k, a, p) in self.mmio if k == "r"]
        # 最近窗口内是否有异常（说明在等 tick，不是死等）
        if self.irq_serial and self.irq_serial != getattr(self, "_stall_irq", 0):
            self._stall_irq = self.irq_serial
            return None
        lo, hi = min(pcs), max(pcs)
        # 需要窗口内 MMIO 读且读数不变，或 CPU 已长时间在同一小循环
        return {
            "at_insn": self.insn,
            "pc": self.last_pc,
            "window": "0x%08X-0x%08X" % (lo, hi),
            "pcs": ["0x%08X" % a for a in sorted(distinct)],
            "polls": ["0x%08X" % a for a, _ in reads[-8:]],
            "regs": self.snapshot(),
        }

    def _resolve(self, st):
        """通用「置就绪位」兜底：把疑似轮询寄存器的 ready/flag 位强行置位。"""
        pcs = [int(x, 16) for x in st["pcs"]]
        # 找出循环内的 MMIO 读地址（PC 在循环内）
        addrs = []
        for k, a, p in self.mmio:
            if k == "r" and p in pcs:
                addrs.append(a)
        seen = []
        for a in reversed(addrs):
            b = align4(a)
            if b not in seen:
                seen.append(b)
        unstick = []
        for b in seen[:6]:
            old = self.reg.get(b, 0)
            new = old
            if b == 0x40021000:
                new = old | 0x00000000 | (1 << 1) | (1 << 17) | (1 << 25) | 1
            elif b == 0x40021004:
                new = (old & ~0xC) | ((old & 3) << 2)
            elif b == 0x40021024:
                new = old | 3
            elif b == 0x40021020:
                new = old | 3
            elif b == 0x40012400:
                new = old | 0x1F
            elif b == 0x40012408:
                new = old | 1
            elif b == 0x4002200C:
                new = 0x20 | (old & ~0x21)
            elif b == 0x40020000:
                new = old | 0xE000
            elif 0x20000000 <= b < 0x20002000:
                new = 1
            elif b == 0xE000E010:
                new = old | 1
            else:
                new = old | 0xFFFFFFFF
            self.reg[b] = new & 0xFFFFFFFF
            unstick.append("0x%08X:0x%08X->0x%08X" % (b, old, new & 0xFFFFFFFF))
        st["unstick"] = unstick
        if self.verbose and unstick:
            print("        unstick: %s" % ", ".join(unstick))
        self.pc_ring.clear()

    # --------------------------------------------------------------- report
    def snapshot(self):
        u = self.uc
        out = {}
        for name, reg in (("r0", UC_ARM_REG_R0), ("r1", UC_ARM_REG_R1),
                          ("r2", UC_ARM_REG_R2), ("r3", UC_ARM_REG_R3),
                          ("r4", UC_ARM_REG_R4), ("r5", UC_ARM_REG_R5),
                          ("sp", UC_ARM_REG_SP), ("lr", UC_ARM_REG_LR),
                          ("pc", UC_ARM_REG_PC)):
            out[name] = "0x%08X" % u.reg_read(reg)
        out["primask"] = u.reg_read(UC_ARM_REG_PRIMASK)
        return out

    def report(self):
        pcs = list(self.pc_ring)
        top = collections.Counter(pcs).most_common(8)
        reads = collections.Counter(a for k, a, p in self.mmio if k == "r")
        writes = collections.Counter(a for k, a, p in self.mmio if k == "w")
        return {
            "reached_main_loop": self.reached,
            "reached_at_insn": self.reached_at,
            "loop_entries": self.loop_entries,
            "insn_count": self.insn,
            "ticks": self.ticks,
            "uptime_ms": self.ticks,
            "systick_csr": "0x%08X" % self.reg.get(0xE000E010, 0),
            "systick_rvr": self.reg.get(0xE000E014, 0),
            "systick_cvr": self.reg.get(0xE000E018, 0),
            "stop_reason": self.stop_reason,
            "stalls": self.stalls,
            "top_pcs": [["0x%08X" % a, n] for a, n in top],
            "top_mmio_reads": [["0x%08X" % a, n] for a, n in reads.most_common(10)],
            "top_read_pcs": [["%s@%s" % (REG_NAMES.get(a, "0x%08X" % a), "0x%08X" % p), n]
                             for (a, p), n in self.read_pc.most_common(20)],
            "top_mmio_writes": [["0x%08X" % a, n] for a, n in writes.most_common(10)],
            "regs": self.snapshot(),
        }


def main(argv=None):
    ap = argparse.ArgumentParser(description="AD200 Pro firmware minimal boot reference")
    ap.add_argument("--bin", default=BIN)
    ap.add_argument("--max-insns", type=int, default=5_000_000)
    ap.add_argument("--after", type=int, default=300_000,
                    help="到达主循环后继续执行的指令数（0=立即停）")
    ap.add_argument("--json", default=None)
    ap.add_argument("--cycles-per-insn", type=int, default=1,
                    help="每条指令折算的 cycle 数（>1 可压缩 SysTick/HAL_Delay 等待）")
    args = ap.parse_args(argv)
    m = RefBoot(args.bin, args.max_insns, args.after,
                cycles_per_insn=args.cycles_per_insn)
    rep = m.run()
    print(json.dumps(rep, indent=1, ensure_ascii=False))
    if args.json:
        with open(args.json, "w", encoding="utf-8") as f:
            json.dump(rep, f, indent=1, ensure_ascii=False)
    return 0 if rep["reached_main_loop"] else 2


if __name__ == "__main__":
    sys.exit(main())
