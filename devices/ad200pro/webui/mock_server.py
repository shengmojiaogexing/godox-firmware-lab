#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""AD200 Pro 固件调试器 —— 独立 Mock 后端（仅 Python 标准库）。

用途：在真实模拟器后端 ``sim/server.py`` 尚未就绪（或不可用）时，
为 Web 前端 ``webui/`` 提供与 SIM-SPEC §6 完全一致的 HTTP JSON API，
返回确定性的假数据（假反汇编 / 假 trace / 假设备状态），便于前端自测。

本文件刻意不 import ``sim`` 包，也不依赖 capstone/unicorn；单文件即可运行：

    ..\\..\\tools\\python\\python.exe webui\\mock_server.py --port 8766

然后浏览器打开 http://127.0.0.1:8766 。

已实现的端点（与 docs/SIM-SPEC.md §6 对齐）：
    GET  /api/status          GET/POST /api/breakpoints
    POST /api/reset           GET  /api/mem
    POST /api/run             POST /api/mem
    POST /api/pause           GET  /api/disasm
    POST /api/step            GET  /api/trace
    GET  /api/symbols         GET  /api/backtrace
    GET/POST /api/device      POST /api/device/event
    GET  /api/mmio            GET  /api/console
    GET  /api/health          （附加：mock 自检）
"""

from __future__ import annotations

import argparse
import bisect
import json
import os
import random
import re
import sys
import threading
import time
from http.server import ThreadingHTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs

# ---------------------------------------------------------------------------
# 常量：固件事实（与 docs/01-static-notes.md 一致）
# ---------------------------------------------------------------------------

CODE_START = 0x08003000        # 镜像基址
CODE_END = 0x08003000 + 0xB52C  # 0x0800E52C，文件尾
FLASH_END = 0x08010000
RAM_START = 0x20000000
RAM_END = 0x20002000

RESET_PC = 0x080030D4
RESET_SP = 0x200008B0
MAIN_LOOP = 0x0800CDB0

#: 已知/推测符号（地址 -> 名称）。真后端从 analysis/symbols.json 读取，
#: mock 内嵌一份，保证无外部依赖也能给出像样的符号名。
SYMBOLS = {
    0x080030C0: "startup",
    0x080030D4: "Reset_Handler",
    0x080030E6: "Default_Handler",
    0x08003548: "scatter_load",
    0x0800356C: "ADC_Init",
    0x080058EC: "flash_program_param",
    0x0800641C: "adc_dma_read",
    0x080068A4: "adc_dma_start",
    0x08006D3C: "EXTI_Config",
    0x08006E9E: "GPIO_WritePin",
    0x08007034: "SysTick_Task",
    0x08007CB4: "systick_100us",
    0x08008778: "gpio_bitbang_write",
    0x08009758: "gpio_input_init",
    0x0800B888: "rf_bitbang_send",
    0x0800BC8C: "SysTick_Handler",
    0x0800BD0C: "SystemInit",
    0x0800BE1C: "TIM1_Handler",
    0x0800BE58: "TIM14_Handler",
    0x0800CA2C: "gpio_bank_set",
    0x0800CAB0: "gpio_bank_clear",
    0x0800CAEC: "gpio_pin_toggle",
    0x0800CBA8: "gpio_pin_read",
    0x0800CDB0: "main",
    0x0800E3E4: "scatter_table",
}

#: 向量表（48 项，16 系统 + 32 IRQ），尽量贴近真实固件。
VECTORS = [RESET_SP, 0x080030D5] + [0x080030E7] * 13 + [0x0800BC8D]
for _irq in range(32):
    VECTORS.append(0x080030E7)
VECTORS[16 + 5] = 0x08009759   # EXTI0_1
VECTORS[16 + 6] = 0x08009759   # EXTI2_3
VECTORS[16 + 7] = 0x08009759   # EXTI4_15
VECTORS[16 + 9] = 0x0800641D   # DMA1_CH1
VECTORS[16 + 13] = 0x0800BE1D  # TIM1
VECTORS[16 + 19] = 0x0800BE59  # TIM14
VECTORS[16 + 22] = 0x0800A4B5  # TIM17

IRQ_NAMES = {
    0: "WWDG", 1: "PVD", 2: "RTC", 3: "FLASH", 4: "RCC",
    5: "EXTI0_1", 6: "EXTI2_3", 7: "EXTI4_15", 8: "TSC",
    9: "DMA1_CH1", 10: "DMA1_CH2_3", 11: "DMA1_CH4_7", 12: "ADC_COMP",
    13: "TIM1_BRK_UP", 14: "TIM1_CC", 15: "TIM2", 16: "TIM3",
    17: "TIM6", 18: "TIM7", 19: "TIM14", 20: "TIM15", 21: "TIM16",
    22: "TIM17", 23: "I2C1", 24: "I2C2", 25: "SPI1", 26: "SPI2",
    27: "USART1", 28: "USART2", 29: "USART3_6", 30: "CEC_CAN", 31: "USB",
}

MMIO_NAMES = [
    (0x40021000, "RCC_CR"), (0x40021004, "RCC_CFGR"), (0x40021014, "RCC_AHBENR"),
    (0x40021018, "RCC_APB2ENR"), (0x4002101C, "RCC_APB1ENR"),
    (0x40022000, "FLASH_ACR"), (0x4002200C, "FLASH_SR"), (0x40022010, "FLASH_CR"),
    (0x48000000, "GPIOA_MODER"), (0x48000004, "GPIOA_OTYPER"),
    (0x48000010, "GPIOA_IDR"), (0x48000014, "GPIOA_ODR"),
    (0x48000018, "GPIOA_BSRR"), (0x48000400, "GPIOB_MODER"),
    (0x48000410, "GPIOB_IDR"), (0x48000414, "GPIOB_ODR"),
    (0x48000800, "GPIOC_MODER"), (0x48000814, "GPIOC_ODR"),
    (0x48000C00, "GPIOD_MODER"), (0x48000C14, "GPIOD_ODR"),
    (0x48001400, "GPIOF_MODER"), (0x48001414, "GPIOF_ODR"),
    (0x40010400, "EXTI_IMR"), (0x40010408, "EXTI_RTSR"),
    (0x4001040C, "EXTI_FTSR"), (0x40010414, "EXTI_PR"),
    (0x40010000, "SYSCFG_CFGR1"), (0x40010008, "SYSCFG_EXTICR1"),
    (0x40012400, "ADC_ISR"), (0x40012404, "ADC_IER"), (0x40012408, "ADC_CR"),
    (0x40012428, "ADC_CFGR1"), (0x40012440, "ADC_DR"),
    (0x40020000, "DMA_ISR"), (0x40020008, "DMA_CCR1"),
    (0x4002000C, "DMA_CNDTR1"), (0x40020010, "DMA_CPAR1"), (0x40020014, "DMA_CMAR1"),
    (0x40014800, "TIM17_CR1"), (0x40014824, "TIM17_CNT"), (0x4001482C, "TIM17_ARR"),
    (0x40007000, "PWR_CR"), (0x40007004, "PWR_CSR"),
    (0x40002800, "RTC_TR"), (0xE000E010, "SYST_CSR"), (0xE000E014, "SYST_RVR"),
    (0xE000E018, "SYST_CVR"), (0xE000E100, "NVIC_ISER"), (0xE000E200, "NVIC_ISPR"),
]

TRACE_TYPES = [
    "call", "ret", "mmio_read", "mmio_write", "irq_enter", "irq_exit",
    "exception", "gpio_change", "uart_tx", "uart_rx", "unimpl", "fault", "note",
]


# ---------------------------------------------------------------------------
# 工具函数
# ---------------------------------------------------------------------------

def parse_int(value, default=0):
    """宽容地解析 int / '0x...' / '123' / None。"""
    if value is None:
        return default
    if isinstance(value, bool):
        return int(value)
    if isinstance(value, int):
        return value
    if isinstance(value, float):
        return int(value)
    text = str(value).strip()
    if not text:
        return default
    try:
        if text.lower().startswith(("0x", "-0x")):
            return int(text, 16)
        return int(text, 10)
    except ValueError:
        return default


def hexaddr(value):
    return "0x%08X" % (parse_int(value) & 0xFFFFFFFF)


def symbol_at(addr):
    """返回 (symbol_name, offset)；没有符号时返回 (None, 0)。"""
    exact = SYMBOLS.get(addr)
    if exact:
        return exact, 0
    lower = [a for a in SYMBOLS if a <= addr]
    if lower:
        base = max(lower)
        off = addr - base
        if off < 0x1000:
            return SYMBOLS[base], off
    return None, 0


def irq_name(irq):
    if irq == -1:
        return "SysTick"
    return IRQ_NAMES.get(irq, "IRQ%d" % irq)


# ---------------------------------------------------------------------------
# 假指令流：覆盖 0x08003000..0x0800E52C，保证已知符号地址是函数起点
# ---------------------------------------------------------------------------

def _rand_text_insn(rng, addr):
    """生成一条像样的 Thumb 指令，返回 (size, bytes, text)。"""
    kind = rng.randrange(12)
    rn = "r%d" % rng.randrange(0, 13)
    rm = "r%d" % rng.randrange(0, 13)
    rb = "r%d" % rng.choice([0, 1, 2, 3, 4, 5, 6, 7])
    imm = rng.randrange(0, 0x100)
    if kind == 0:
        return 2, _bytes(rng, 2), "movs %s, #0x%X" % (rn, imm)
    if kind == 1:
        return 2, _bytes(rng, 2), "adds %s, #0x%X" % (rn, imm)
    if kind == 2:
        return 2, _bytes(rng, 2), "subs %s, #0x%X" % (rn, imm)
    if kind == 3:
        return 2, _bytes(rng, 2), "cmp %s, #0x%X" % (rn, imm)
    if kind == 4:
        return 2, _bytes(rng, 2), "mov %s, %s" % (rn, rm)
    if kind == 5:
        return 2, _bytes(rng, 2), "lsls %s, %s, #0x%X" % (rn, rm, rng.randrange(0, 32))
    if kind == 6:
        return 2, _bytes(rng, 2), "ands %s, %s" % (rn, rm)
    if kind == 7:
        return 2, _bytes(rng, 2), "ldr %s, [%s, #0x%X]" % (rn, rb, rng.randrange(0, 0x80))
    if kind == 8:
        return 2, _bytes(rng, 2), "str %s, [%s, #0x%X]" % (rn, rb, rng.randrange(0, 0x80))
    if kind == 9:
        return 2, _bytes(rng, 2), "ldrb %s, [%s, #0x%X]" % (rn, rb, rng.randrange(0, 0x40))
    if kind == 10:
        delta = rng.randrange(-0x100, 0x100) * 2
        target = (addr + 4 + delta) & 0xFFFFFFFF
        return 4, _bytes(rng, 4), "ldr %s, [pc, #0x%X]  ; =0x%08X" % (rn, rng.randrange(0, 0x400), target)
    delta = rng.randrange(1, 0x40) * 2
    target = (addr + 4 + delta) & 0xFFFFFFFF
    return 4, _bytes(rng, 4), "bl 0x%08X" % target


def _bytes(rng, n):
    return [rng.randrange(0, 256) for _ in range(n)]


def _build_code():
    """构造假指令流，返回 (starts, sizes, byte_lists, texts)。"""
    starts = []
    sizes = []
    byte_lists = []
    texts = []
    rng = random.Random(0xAD200)
    entry_addrs = sorted({CODE_START, CODE_END} | {a for a in SYMBOLS if CODE_START <= a < CODE_END})
    for bi in range(len(entry_addrs) - 1):
        start, end = entry_addrs[bi], entry_addrs[bi + 1]
        addr = start
        func_start = True
        remaining = 0
        while addr < end:
            budget = end - addr
            if func_start and budget >= 2:
                size = 2
                data = _bytes(rng, 2)
                text = "push {r4, lr}"
                remaining = rng.randrange(4, 26)
                func_start = False
            elif remaining <= 0 and budget >= 2:
                size = 2
                data = _bytes(rng, 2)
                text = rng.choice(["pop {r4, pc}", "bx lr", "pop {r4, r5, pc}", "bx lr"])
                func_start = True
                remaining = 0
            else:
                size, data, text = _rand_text_insn(rng, addr)
                if size > budget:
                    size = 2
                    data = _bytes(rng, 2)
                    text = "nop"
                remaining -= 1
            starts.append(addr)
            sizes.append(size)
            byte_lists.append(data)
            texts.append(text)
            addr += size
    return starts, sizes, byte_lists, texts


STARTS, SIZES, BYTES, TEXTS = _build_code()
_START_INDEX = {a: i for i, a in enumerate(STARTS)}


def code_byte(addr):
    """取假代码区单字节，addr 不在指令范围内返回 None。"""
    i = bisect.bisect_right(STARTS, addr) - 1
    if i < 0:
        return None
    start = STARTS[i]
    if addr < start + SIZES[i]:
        return BYTES[i][addr - start]
    return None


def disasm_at(addr, count):
    if not STARTS:
        return []
    i = bisect.bisect_right(STARTS, addr) - 1
    if i < 0:
        i = 0
    out = []
    for j in range(i, min(i + count, len(STARTS))):
        out.append({
            "addr": STARTS[j],
            "bytes": list(BYTES[j]),
            "text": TEXTS[j],
        })
    return out


# ---------------------------------------------------------------------------
# 假内存
# ---------------------------------------------------------------------------

class FakeMemory:
    def __init__(self):
        self.overrides = {}

    def _base_byte(self, addr):
        if CODE_START <= addr < CODE_END:
            b = code_byte(addr)
            return 0 if b is None else b
        if CODE_END <= addr < FLASH_END:
            return 0xFF
        if addr < CODE_START:
            return 0xFF
        if addr == 0x20000000 or (0x20000000 <= addr < 0x200000C0):
            # RAM 里的向量表（固件启动后会复制到 0x20000000）
            index = (addr - 0x20000000) // 4
            if 0 <= index < len(VECTORS):
                value = VECTORS[index]
                shift = ((addr - 0x20000000) % 4) * 8
                return (value >> shift) & 0xFF
            return 0
        if RAM_START <= addr < RAM_END:
            off = addr - RAM_START
            return ((off * 37 + 0x5A) ^ (off >> 3)) & 0xFF
        if addr >= 0x40000000:
            return 0
        return 0

    def read(self, addr, length):
        return [self.overrides.get(addr + i, self._base_byte(addr + i)) for i in range(length)]

    def write(self, addr, data):
        for i, b in enumerate(data):
            self.overrides[addr + i] = b & 0xFF


# ---------------------------------------------------------------------------
# Mock 状态机
# ---------------------------------------------------------------------------

BUTTON_PINS = {
    "POWER": ("A", 0), "TEST": ("A", 1), "MENU": ("A", 2), "SET": ("A", 3),
    "UP": ("B", 0), "DOWN": ("B", 1),
}


def default_device():
    return {
        "battery": 3.7,
        "battery_mv": 3700,
        "cap_voltage": 330.0,
        "temp": 25.0,
        "power": True,
        "buttons": {"POWER": False, "TEST": False, "MENU": False,
                    "SET": False, "UP": False, "DOWN": False},
        "rf": {"rx_count": 2, "last_rx": "01 23 45 67",
               "tx_count": 1, "last_tx": "AA 55"},
        "gpio": {
            "A": {"moder": 0xA800, "odr": 0x0800, "idr": 0x0A00},
            "B": {"moder": 0x5555, "odr": 0x00FF, "idr": 0x00F0},
            "C": {"moder": 0xFFFF, "odr": 0x0000, "idr": 0xFFFF},
            "D": {"moder": 0x0000, "odr": 0x0000, "idr": 0x0000},
            "F": {"moder": 0xFFFF, "odr": 0x0003, "idr": 0x0003},
        },
        "adc": {"channels": []},
    }


ADC_DEFS = [
    (0, "电池电压"), (1, "电容电压"), (2, "温度"), (3, "内部参考"),
    (4, "CH4"), (5, "CH5"), (6, "CH6"), (7, "CH7"),
]


class MockState:
    def __init__(self):
        self.lock = threading.RLock()
        self.rng = random.Random(2024)
        self.mem = FakeMemory()
        self.state = "paused"
        self.insn_count = 0
        self.insn_index = _START_INDEX.get(RESET_PC, 0)
        self.uptime_ms = 0
        self.stop_reason = None
        self.breakpoints = {MAIN_LOOP}
        self.irq_enabled = {5, 7, 9, 22}
        self.irq_pending = {22}
        self.trace = []
        self.seq = 0
        self.mmio_history = {}
        self.console_lines = [
            "[mock] AD200 Pro Mock 后端已启动（无 Unicorn，假数据）",
            "[mock] 固件镜像 firmware/AD200pro_V1.60.bin 已加载，基址 0x08003000",
            "[mock] 向量表已复制到 RAM 0x20000000，SYSCFG MEM_MODE=SRAM",
            "[mock] SysTick 配置为 1 ms，NVIC 已使能 EXTI0_1 / EXTI4_15 / DMA1_CH1 / TIM17",
            "[mock] 进入主循环 0x0800CDB0，等待 *(u8*)0x200001CF == 1",
        ]
        self.console_seq = len(self.console_lines)
        self.device = default_device()
        self._refresh_adc()
        self._seed_trace(70)

    # -- 基础 ---------------------------------------------------------------

    @property
    def pc(self):
        if not STARTS:
            return RESET_PC
        return STARTS[self.insn_index % len(STARTS)]

    def regs(self):
        pc = self.pc
        n = self.insn_count
        return {
            "r0": 0x200001CF,
            "r1": 0x00000001,
            "r2": 0x40021000,
            "r3": 0x200008B0,
            "r4": (n * 7 + 0x55) & 0xFFFFFFFF,
            "r5": (n * 13 + 0xAA) & 0xFFFFFFFF,
            "r6": 0x00000000,
            "r7": 0x20000800,
            "r8": 0x00000000,
            "r9": 0x00000000,
            "r10": 0x00000000,
            "r11": 0x00000000,
            "r12": 0x00000000,
            "r13": RESET_SP,
            "sp": RESET_SP,
            "r14": 0xFFFFFFF9,
            "lr": 0xFFFFFFF9,
            "r15": pc,
            "pc": pc,
            "xpsr": 0x01000000,
            "primask": 0,
        }

    def stats(self):
        return {
            "insn_count": self.insn_count,
            "cycle_estimate": self.insn_count + self.insn_count // 3,
            "uptime_ms": self.insn_count // 1000,
        }

    def status(self):
        with self.lock:
            if self.state == "running":
                self._advance(6000 + self.rng.randrange(0, 4000))
            return {
                "state": self.state,
                "pc": self.pc,
                "regs": self.regs(),
                "stats": self.stats(),
                "breakpoints": sorted(self.breakpoints),
                "irq": {"enabled": sorted(self.irq_enabled),
                        "pending": sorted(self.irq_pending)},
                "stop_reason": self.stop_reason,
            }

    def _advance(self, delta):
        if delta <= 0:
            return
        self.insn_count += delta
        self.insn_index = (self.insn_index + delta) % len(STARTS)
        self.uptime_ms = self.insn_count // 1000
        # 动态生成少量 trace，让轮询视图有内容
        for _ in range(self.rng.randrange(0, 3)):
            if self.state == "running" or self.rng.random() < 0.55:
                self._append_fake_event()

    # -- trace --------------------------------------------------------------

    def _append(self, etype, pc=None, addr=None, name=None, data=None):
        self.seq += 1
        ev = {
            "seq": self.seq,
            "type": etype,
            "pc": (pc if pc is not None else self.pc) & 0xFFFFFFFF,
            "addr": None if addr is None else (addr & 0xFFFFFFFF),
            "name": name,
            "data": data,
        }
        self.trace.append(ev)
        if len(self.trace) > 5000:
            del self.trace[:1000]
        if etype in ("mmio_read", "mmio_write") and ev["addr"] is not None:
            self.mmio_history.setdefault(ev["addr"], []).append(ev)
            if len(self.mmio_history[ev["addr"]]) > 30:
                del self.mmio_history[ev["addr"]][:10]
        return ev

    def _append_fake_event(self):
        pc = self.pc
        kind = self.rng.choices(
            ["call", "mmio_read", "mmio_write", "gpio_change", "irq",
             "unimpl", "note"],
            weights=[24, 22, 22, 12, 10, 4, 6], k=1)[0]
        if kind == "call":
            target_addr, target_name = self.rng.choice(list(SYMBOLS.items()))
            self._append("call", name=target_name, addr=target_addr,
                         data={"target": target_addr})
        elif kind in ("mmio_read", "mmio_write"):
            reg_addr, reg_name = self.rng.choice(MMIO_NAMES)
            value = self.rng.choice([0, 1, 0x00000003, 0x0000A000,
                                     0x00008000, 0x00000800, 0x00010000,
                                     self.rng.randrange(0, 0x10000)])
            self._append(kind, addr=reg_addr, name=reg_name,
                         data=value)
        elif kind == "gpio_change":
            port = self.rng.choice(["A", "B", "C", "D", "F"])
            pin = self.rng.randrange(0, 16)
            value = self.rng.randrange(0, 2)
            self._append("gpio_change", addr=0x48000000 + 0x400 * "ABCDF".index(port) + 0x14,
                         name="GPIO%s_ODR" % port,
                         data={"port": port, "pin": pin, "value": value})
        elif kind == "irq":
            irq = self.rng.choice(sorted(self.irq_enabled) or [22])
            if self.rng.random() < 0.5:
                self._append("irq_enter", name=irq_name(irq),
                             data={"irq": irq})
            else:
                self._append("irq_exit", name=irq_name(irq),
                             data={"irq": irq})
        elif kind == "unimpl":
            addr = 0x40010000 + self.rng.randrange(0, 0x400) * 4
            self._append("unimpl", addr=addr,
                         data=self.rng.choice(["read", "write"]))
        else:
            self._append("note", name="main_loop",
                         data="等待 0x200001CF == 1")

    def _seed_trace(self, n):
        for _ in range(n):
            self._append_fake_event()
        # 保证初始 trace 覆盖所有主要类型，方便 UI 过滤自测
        self._append("fault", addr=0x0800B888, name="fault_probe",
                     data="mock 示例故障事件（仅演示 trace 过滤，不代表固件）")
        self._append("irq_enter", name="SysTick_Handler", data={"irq": -1})
        self._append("irq_exit", name="SysTick_Handler", data={"irq": -1})
        self._append("gpio_change", addr=0x48001414, name="GPIOF_ODR",
                     data={"port": "F", "pin": 0, "value": 1})
        self._append("uart_tx", addr=0x48000014, name="soft_uart_tx",
                     data="0x55 0xAA")
        self._append("ret", name="main", data={"from": 0x0800CDB0})

    # -- 操作 ---------------------------------------------------------------

    def reset(self):
        with self.lock:
            self.state = "paused"
            self.insn_count = 0
            self.insn_index = _START_INDEX.get(RESET_PC, 0)
            self.uptime_ms = 0
            self.stop_reason = None
            self.trace = []
            self.seq = 0
            self.mmio_history = {}
            self.irq_pending = {22}
            self._seed_trace(50)
            self._console("固件已复位：PC=0x%08X，SP=0x%08X" % (self.pc, RESET_SP))
            return self.status()

    def step(self, n):
        with self.lock:
            n = max(1, min(int(n), 1_000_000))
            self.state = "paused"
            self.stop_reason = None
            self._advance(n)
            self._console("单步执行 %d 条指令，PC=0x%08X" % (n, self.pc))
            return self.status()

    def pause(self):
        with self.lock:
            if self.state == "running":
                self.state = "paused"
                self.stop_reason = "pause"
                self._console("用户暂停，PC=0x%08X" % self.pc)
            return self.status()

    def run(self, max_insns):
        with self.lock:
            self.state = "running"
            self.stop_reason = None
        max_insns = max(1, min(int(max_insns), 50_000_000))
        deadline = time.time() + 0.45
        done = 0
        while time.time() < deadline and done < max_insns:
            time.sleep(0.03)
            with self.lock:
                delta = min(max_insns - done, 30000)
                self._advance(delta)
                done += delta
                for _ in range(self.rng.randrange(1, 4)):
                    self._append_fake_event()
        with self.lock:
            self.state = "paused"
            if self.breakpoints and done >= max_insns * 0.5:
                bp = sorted(self.breakpoints)[0]
                self.set_pc(bp)
                self.stop_reason = "breakpoint"
            elif done >= max_insns:
                self.stop_reason = "insn_limit"
            else:
                self.stop_reason = "insn_limit"
            self._console("运行结束（%s），共执行约 %d 条指令，PC=0x%08X"
                          % (self.stop_reason, done, self.pc))
            st = self.status()
            st["stop_reason"] = self.stop_reason
            return st

    def set_pc(self, addr):
        addr = parse_int(addr)
        exact = _START_INDEX.get(addr)
        if exact is None:
            i = bisect.bisect_right(STARTS, addr) - 1
            exact = max(0, i)
        self.insn_index = exact

    # -- 设备 ---------------------------------------------------------------

    def _refresh_adc(self):
        dev = self.device
        battery_mv = float(dev.get("battery", 3.7)) * 1000.0
        cap_v = float(dev.get("cap_voltage", 330.0))
        temp = float(dev.get("temp", 25.0))
        values = [
            int(battery_mv / 3300.0 * 4095),
            int(cap_v / 400.0 * 4095),
            int((temp + 40.0) / 120.0 * 4095),
            1489,
            0x800, 0x800, 0x800, 0x800,
        ]
        chans = []
        for (ch, name), value in zip(ADC_DEFS, values):
            value = max(0, min(4095, value))
            chans.append({
                "ch": ch,
                "name": name,
                "value": value,
                "mv": int(value / 4095.0 * 3300.0),
                "volts": round(value / 4095.0 * 3.3, 3),
            })
        dev["adc"] = {"channels": chans}

    def _update_button_pin(self, name, pressed):
        pin = BUTTON_PINS.get(name)
        if not pin:
            return
        port, bit = pin
        gpio = self.device["gpio"][port]
        if pressed:
            gpio["idr"] |= (1 << bit)
        else:
            gpio["idr"] &= ~(1 << bit) & 0xFFFF
        self._append("gpio_change",
                     addr=0x48000000 + 0x400 * "ABCDF".index(port) + 0x10,
                     name="GPIO%s_IDR" % port,
                     data={"port": port, "pin": bit, "value": 1 if pressed else 0})

    def device_state(self):
        with self.lock:
            self._refresh_adc()
            return json.loads(json.dumps(self.device))

    def device_set(self, patch):
        with self.lock:
            for key, value in (patch or {}).items():
                if key in ("battery", "battery_mv"):
                    mv = parse_int(value) if key == "battery_mv" else int(float(value) * 1000)
                    mv = max(0, min(10000, mv))
                    self.device["battery_mv"] = mv
                    self.device["battery"] = round(mv / 1000.0, 3)
                elif key in ("cap_voltage", "cap", "cap_mv"):
                    v = float(value)
                    if key == "cap_mv":
                        v = v / 1000.0
                    self.device["cap_voltage"] = round(max(0.0, min(500.0, v)), 2)
                elif key in ("temp", "temp_c", "temperature"):
                    self.device["temp"] = round(max(-40.0, min(150.0, float(value))), 2)
                elif key == "power":
                    self.device["power"] = bool(value)
                elif key == "button":
                    if isinstance(value, dict):
                        self._update_button_pin(str(value.get("name", "")).upper(),
                                                bool(value.get("pressed")))
                elif key == "buttons" and isinstance(value, dict):
                    for name, pressed in value.items():
                        self._update_button_pin(str(name).upper(), bool(pressed))
                elif key == "rf" and isinstance(value, dict):
                    self.device["rf"].update(value)
                else:
                    self.device[key] = value
            self._refresh_adc()
            return self.device_state()

    def device_event(self, kind, payload):
        payload = payload or {}
        with self.lock:
            if kind == "button":
                name = str(payload.get("name", payload.get("button", ""))).upper()
                pressed = bool(payload.get("pressed", True))
                if name in self.device["buttons"]:
                    self.device["buttons"][name] = pressed
                self._update_button_pin(name, pressed)
                self._console("按键 %s %s" % (name or "?", "按下" if pressed else "释放"))
            elif kind in ("rf_rx", "rf"):
                hexstr = payload.get("hex")
                if not hexstr:
                    raw = payload.get("bytes")
                    if isinstance(raw, list):
                        hexstr = " ".join("%02X" % (parse_int(b) & 0xFF) for b in raw)
                hexstr = str(hexstr or "").strip()
                self.device["rf"]["rx_count"] += 1
                self.device["rf"]["last_rx"] = hexstr
                self._append("note", name="RF_RX", data={"hex": hexstr})
                self._console("RF 接收：%s" % hexstr)
            elif kind == "power":
                if "on" in payload:
                    self.device["power"] = bool(payload.get("on"))
                else:
                    self.device["power"] = not self.device["power"]
                self._console("电源：%s" % ("开" if self.device["power"] else "关"))
            elif kind == "rf_tx":
                hexstr = str(payload.get("hex", "")).strip()
                self.device["rf"]["tx_count"] += 1
                self.device["rf"]["last_tx"] = hexstr
                self._console("RF 发送：%s" % hexstr)
            else:
                self._append("note", name="device_event",
                             data={"kind": kind, "payload": payload})
                self._console("设备事件：%s" % kind)
            return self.device_state()

    def _console(self, line):
        self.console_seq += 1
        self.console_lines.append("[%6.3fs] %s" % (self.insn_count / 1000.0, line))
        if len(self.console_lines) > 500:
            del self.console_lines[:100]

    # -- 其它端点数据 --------------------------------------------------------

    def trace_since(self, since, limit):
        with self.lock:
            if since > self.seq:
                since = 0
            evs = [e for e in self.trace if e["seq"] > since]
            if len(evs) > limit:
                evs = evs[-limit:]
            return {
                "events": json.loads(json.dumps(evs)),
                "seq": self.seq,
            }

    def backtrace(self):
        with self.lock:
            frames = []
            pc = self.pc
            name, off = symbol_at(pc)
            frames.append({"index": 0, "pc": pc, "symbol": name or "??",
                           "offset": off, "addr": pc})
            chain = [MAIN_LOOP, 0x080030C0]
            for i, addr in enumerate(chain, start=1):
                nm, of = symbol_at(addr)
                frames.append({"index": i, "pc": addr, "symbol": nm or "??",
                               "offset": of, "addr": addr})
            return {"frames": frames, "count": len(frames)}

    def mmio(self, addr):
        with self.lock:
            if addr is None:
                evs = [e for e in self.trace if e["type"] in ("mmio_read", "mmio_write")]
                evs = evs[-50:]
                return {"accesses": json.loads(json.dumps(evs)), "count": len(evs)}
            hist = self.mmio_history.get(addr, [])
            return {
                "addr": hexaddr(addr),
                "count": len(hist),
                "accesses": json.loads(json.dumps(hist[-20:])),
                "last": json.loads(json.dumps(hist[-1])) if hist else None,
            }

    def console(self):
        with self.lock:
            return {
                "since": self.console_seq,
                "lines": list(self.console_lines),
                "text": "\n".join(self.console_lines),
            }


# ---------------------------------------------------------------------------
# HTTP 处理
# ---------------------------------------------------------------------------

class MockHandler(BaseHTTPRequestHandler):
    server_version = "AD200ProMock/1.0"
    protocol_version = "HTTP/1.1"

    # 静默/可选日志 ---------------------------------------------------------
    def log_message(self, fmt, *args):  # noqa: A003
        if getattr(self.server, "verbose", False):
            sys.stderr.write("[mock] %s - %s\n" % (self.address_string(), fmt % args))

    def handle_one_request(self):  # noqa: N802
        """客户端提前断开（刷新/关闭页面）时不要抛一整屏 traceback。"""
        try:
            BaseHTTPRequestHandler.handle_one_request(self)
        except (ConnectionResetError, ConnectionAbortedError, BrokenPipeError):
            self.close_connection = True

    @property
    def state(self):
        return self.server.mock_state

    @property
    def webui_dir(self):
        return self.server.webui_dir

    # -- 基础发送 -----------------------------------------------------------

    def _send(self, body, status=200, content_type="application/json; charset=utf-8"):
        if isinstance(body, str):
            body = body.encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Cache-Control", "no-store")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.end_headers()
        try:
            self.wfile.write(body)
        except (BrokenPipeError, ConnectionResetError):
            pass

    def send_json(self, obj, status=200):
        self._send(json.dumps(obj, ensure_ascii=False), status)

    def send_error_json(self, message, status=400):
        self.send_json({"error": message}, status)

    def read_json(self):
        length = parse_int(self.headers.get("Content-Length"), 0)
        if length <= 0:
            return {}
        if length > 4 * 1024 * 1024:
            raise ValueError("请求体过大")
        raw = self.rfile.read(length)
        try:
            return json.loads(raw.decode("utf-8"))
        except Exception:
            raise ValueError("请求体不是合法 JSON")

    # -- 方法分发 -----------------------------------------------------------

    def do_OPTIONS(self):  # noqa: N802
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.send_header("Content-Length", "0")
        self.end_headers()

    def do_GET(self):  # noqa: N802
        self._dispatch("GET")

    def do_POST(self):  # noqa: N802
        self._dispatch("POST")

    def do_HEAD(self):  # noqa: N802
        self._dispatch("HEAD")

    def _dispatch(self, method):
        parsed = urlparse(self.path)
        path = parsed.path.rstrip("/") or "/"
        query = {k: v[-1] for k, v in parse_qs(parsed.query).items()}
        try:
            if path.startswith("/api"):
                self._handle_api(method, path, query)
            elif method in ("GET", "HEAD"):
                self._serve_static(path)
            else:
                self.send_error_json("不支持的静态资源方法", 405)
        except BrokenPipeError:
            pass
        except ValueError as exc:
            # 请求体/参数非法：明确返回 400，而不是 500
            try:
                self.send_error_json(str(exc) or "请求参数不合法", 400)
            except Exception:
                pass
        except Exception as exc:  # noqa: BLE001
            try:
                self.send_error_json("mock 内部错误：%s" % exc, 500)
            except Exception:
                pass

    # -- API ----------------------------------------------------------------

    def _handle_api(self, method, path, query):
        st = self.state
        if path == "/api/health":
            return self.send_json({"ok": True, "mock": True, "engine": "mock",
                                   "version": "1.0", "state": st.state})
        if path == "/api/status":
            if method != "GET":
                return self.send_error_json("请使用 GET", 405)
            return self.send_json(st.status())
        if path == "/api/reset":
            if method != "POST":
                return self.send_error_json("请使用 POST", 405)
            return self.send_json(st.reset())
        if path == "/api/run":
            if method != "POST":
                return self.send_error_json("请使用 POST", 405)
            body = self.read_json() if method == "POST" else {}
            max_insns = parse_int(body.get("max_insns"), 2_000_000)
            return self.send_json(st.run(max_insns))
        if path == "/api/pause":
            if method != "POST":
                return self.send_error_json("请使用 POST", 405)
            return self.send_json(st.pause())
        if path == "/api/step":
            if method != "POST":
                return self.send_error_json("请使用 POST", 405)
            body = self.read_json()
            n = parse_int(body.get("n", body.get("count", 1)), 1)
            return self.send_json(st.step(n))
        if path == "/api/breakpoints":
            if method == "GET":
                return self.send_json({"breakpoints": sorted(st.breakpoints)})
            body = self.read_json()
            added = body.get("add", [])
            removed = body.get("remove", [])
            if isinstance(added, (int, str)):
                added = [added]
            if isinstance(removed, (int, str)):
                removed = [removed]
            for a in added or []:
                st.breakpoints.add(parse_int(a))
            for a in removed or []:
                st.breakpoints.discard(parse_int(a))
            if "addr" in body:
                addr = parse_int(body["addr"])
                if body.get("action", "toggle") == "remove":
                    st.breakpoints.discard(addr)
                else:
                    st.breakpoints.add(addr)
            return self.send_json({"breakpoints": sorted(st.breakpoints)})
        if path == "/api/mem":
            if method == "GET":
                addr = parse_int(query.get("addr"), RAM_START)
                length = max(1, min(parse_int(query.get("len"), 256), 4096))
                data = st.mem.read(addr, length)
                return self.send_json({
                    "addr": hexaddr(addr),
                    "bytes": data,
                    "hex": " ".join("%02X" % b for b in data),
                    "len": len(data),
                })
            body = self.read_json()
            addr = parse_int(body.get("addr"))
            raw = body.get("bytes")
            if isinstance(raw, str):
                txt = raw.replace("0x", "").replace(",", " ").replace("\n", " ")
                try:
                    raw = [int(tok, 16) for tok in txt.split() if tok]
                except ValueError:
                    return self.send_error_json("bytes 字符串必须只包含十六进制字节")
            if not isinstance(raw, list) or not raw:
                return self.send_error_json("bytes 必须是非空数组")
            if len(raw) > 4096:
                return self.send_error_json("单次写入不能超过 4096 字节")
            data = [parse_int(b) & 0xFF for b in raw]
            st.mem.write(addr, data)
            st._append("note", addr=addr, name="mem_write",
                       data={"addr": hexaddr(addr), "len": len(data)})
            return self.send_json({"ok": True, "addr": hexaddr(addr), "len": len(data)})
        if path == "/api/disasm":
            addr = parse_int(query.get("addr"), st.pc)
            n = max(1, min(parse_int(query.get("n"), 20), 200))
            return self.send_json({"addr": hexaddr(addr),
                                   "insns": disasm_at(addr, n)})
        if path == "/api/trace":
            since = parse_int(query.get("since"), 0)
            limit = max(1, min(parse_int(query.get("limit"), 500), 2000))
            return self.send_json(st.trace_since(since, limit))
        if path == "/api/symbols":
            return self.send_json({
                "symbols": {hexaddr(a): name for a, name in sorted(SYMBOLS.items())},
                "vectors": VECTORS,
            })
        if path == "/api/device":
            if method == "GET":
                return self.send_json(st.device_state())
            body = self.read_json()
            try:
                return self.send_json(st.device_set(body))
            except (TypeError, ValueError):
                return self.send_error_json("device 字段类型不正确")
        if path == "/api/device/event":
            if method != "POST":
                return self.send_error_json("请使用 POST", 405)
            body = self.read_json()
            kind = str(body.get("kind", "note"))
            try:
                return self.send_json(st.device_event(kind, body.get("payload") or {}))
            except (TypeError, ValueError):
                return self.send_error_json("device/event payload 类型不正确")
        if path == "/api/mmio":
            addr = parse_int(query.get("addr"), None) if query.get("addr") else None
            return self.send_json(st.mmio(addr))
        if path == "/api/backtrace":
            return self.send_json(st.backtrace())
        if path == "/api/console":
            return self.send_json(st.console())
        return self.send_error_json("未知 API 端点：%s" % path, 404)

    # -- 静态文件 -----------------------------------------------------------

    STATIC_FILES = {
        "/": ("index.html", "text/html; charset=utf-8"),
        "/index.html": ("index.html", "text/html; charset=utf-8"),
        "/app.js": ("app.js", "application/javascript; charset=utf-8"),
        "/style.css": ("style.css", "text/css; charset=utf-8"),
    }

    def _serve_static(self, path):
        if path == "/favicon.ico":
            self.send_response(204)
            self.send_header("Content-Length", "0")
            self.end_headers()
            return
        entry = self.STATIC_FILES.get(path)
        if not entry:
            self._send("404 未找到：%s" % path, 404, "text/plain; charset=utf-8")
            return
        filename, content_type = entry
        full = os.path.join(self.webui_dir, filename)
        try:
            with open(full, "rb") as f:
                data = f.read()
        except OSError:
            self._send("静态文件缺失：%s（请确认 webui 目录完整）" % filename,
                       404, "text/plain; charset=utf-8")
            return
        if self.command == "HEAD":
            self.send_response(200)
            self.send_header("Content-Type", content_type)
            self.send_header("Content-Length", str(len(data)))
            self.send_header("Cache-Control", "no-cache")
            self.end_headers()
            return
        self._send(data, 200, content_type)


class MockHTTPServer(ThreadingHTTPServer):
    daemon_threads = True
    allow_reuse_address = True

    def handle_error(self, request, client_address):
        """忽略浏览器主动断开连接造成的错误，其它异常照常输出。"""
        exc = sys.exc_info()[1]
        if isinstance(exc, (ConnectionResetError, ConnectionAbortedError, BrokenPipeError)):
            return
        ThreadingHTTPServer.handle_error(self, request, client_address)


# ---------------------------------------------------------------------------
# 入口
# ---------------------------------------------------------------------------

def main(argv=None):
    here = os.path.dirname(os.path.abspath(__file__))
    parser = argparse.ArgumentParser(
        description="AD200 Pro 固件调试 Web UI 的独立 Mock 后端（仅标准库）")
    parser.add_argument("--host", default="127.0.0.1", help="监听地址（默认 127.0.0.1）")
    parser.add_argument("--port", type=int, default=8766, help="监听端口（默认 8766）")
    parser.add_argument("--webui", default=here, help="webui 静态文件目录（默认本脚本目录）")
    parser.add_argument("--verbose", action="store_true", help="打印每个 HTTP 请求")
    args = parser.parse_args(argv)

    state = MockState()
    server = MockHTTPServer((args.host, args.port), MockHandler)
    server.mock_state = state
    server.webui_dir = os.path.abspath(args.webui)
    server.verbose = args.verbose

    url = "http://%s:%d" % (args.host, args.port)
    print("=" * 64)
    print(" AD200 Pro 模拟器 Mock 后端（独立，无 Unicorn）")
    print("   浏览器打开: %s" % url)
    print("   API 前缀  : %s/api" % url)
    print("   静态目录  : %s" % server.webui_dir)
    print("   说明: 这是假数据后端，仅用于前端自测；真实后端请用")
    print("         python run_sim.py sim.server firmware/AD200pro_V1.60.bin --port 8765")
    print("   按 Ctrl+C 停止")
    print("=" * 64)
    try:
        server.serve_forever(poll_interval=0.2)
    except KeyboardInterrupt:
        print("\n[mock] 已停止")
    finally:
        server.server_close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
