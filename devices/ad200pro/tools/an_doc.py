#!/usr/bin/env python3
"""Generate docs/03-functions.md (function inventory + call graph) from
analysis/functions.json + tools/an_names.py.

Usage: python tools/an_doc.py
"""
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AN = os.path.join(ROOT, 'analysis')
DOCS = os.path.join(ROOT, 'docs')
BASE = 0x08003000

import an_names  # noqa: E402

NAMES = dict(an_names.NAMES)


def nm(a):
    return NAMES.get(a) or 'FUNC_%08x' % a


def main():
    funcs = json.load(open(os.path.join(AN, 'functions.json'), encoding='utf-8'))
    by = {f['start']: f for f in funcs}
    for f in funcs:
        NAMES.setdefault(f['start'], 'FUNC_%08x' % f['start'])

    lines = []
    add = lines.append
    add('# 03 – 函数清单与调用图（AD200 Pro V1.60 固件）')
    add('')
    add('> 本文件由 `tools/an_doc.py` 依据 `analysis/functions.json` + `tools/an_names.py` 生成，'
        '可直接重跑。地址均为**运行地址**（镜像基址 `0x08003000`）。')
    add('> 命名依据与置信度见 `tools/an_names.py` 内每条注释；`FUNC_xxxxxxxx` 为尚未命名的函数。')
    add('> 总数：**%d 个函数**（已命名 %d）。' % (
        len(funcs), sum(1 for f in funcs if not nm(f['start']).startswith('FUNC_'))))
    add('')
    add('## 1. 调用图（自上而下）')
    add('')
    add('```')
    add(_tree(funcs, by, 0x0800CDB0, 0, 2))
    add('```')
    add('')
    add('## 2. 中断处理链')
    add('')
    add('| 向量 | 入口 | 名称 | 直接调用 |')
    add('|---|---|---|---|')
    vec = json.load(open(os.path.join(AN, 'symbols.json'), encoding='utf-8'))['vectors']
    for v in vec:
        t = v.get('target')
        if not t or v['name'] in ('initial_sp', 'Reset'):
            continue
        f = by.get(t)
        callees = ' '.join(nm(c) for c in (f['calls'] if f else []))
        add('| %s | 0x%08x | `%s` | %s |' % (v['name'], t, nm(t), callees))
    add('')
    add('## 3. 全部函数（按地址）')
    add('')
    add('| 地址 | 名称 | 指令数 | 调用者 | 被调用 | 外设(MMIO) | 关键 RAM | 字符串 |')
    add('|---|---|---|---|---|---|---|---|')
    for f in sorted(funcs, key=lambda x: x['start']):
        a = f['start']
        callers = ' '.join(nm(c) for c in f.get('callers', []))
        calls = ' '.join(nm(c) for c in f.get('calls', []))
        mmio = ' '.join(sorted(set(m['name'] for m in f.get('mmio', []))))
        ram = ' '.join(sorted(set('0x%08x' % l['value'] for l in f.get('literals', [])
                                  if 0x20000000 <= l['value'] < 0x20001000))[:120])
        strs = '; '.join(repr(s) for s in (f.get('strings') or [])[:2])
        add('| `0x%08x` | `%s` | %d | %s | %s | %s | %s | %s |' % (
            a, nm(a), f['insn_count'], callers[:110], calls[:170], mmio[:80], ram, strs[:40]))
    add('')
    add('## 4. 调用图综述')
    add('')
    add(_narrative())
    out = os.path.join(DOCS, '03-functions.md')
    open(out, 'w', encoding='utf-8').write('\n'.join(lines) + '\n')
    print('wrote', out, len(lines), 'lines')


def _tree(funcs, by, a, depth, maxdepth, seen=None):
    seen = set() if seen is None else seen
    f = by.get(a)
    pad = '  ' * depth
    if f is None:
        return '%s0x%08x  (no body)' % (pad, a)
    s = '%s0x%08x %-28s %4d ins' % (pad, a, NAMES.get(a, '?'), f['insn_count'])
    if depth >= maxdepth or a in seen:
        return s
    seen.add(a)
    for c in sorted(f['calls']):
        s += '\n' + _tree(funcs, by, c, depth + 1, maxdepth, seen)
    return s


def _narrative():
    return '''### 层次结构

1. **启动层（无调用者）**：`reset_handler` → `SystemInit` → `startup_c` → `scatter_load` → `main`。
2. **运行时助手层**：`udivmod32` / `udiv32` / `crc16_block` / `delay_us` / `delay_ms` / `get_tick` /
   `gpio_write_pin` / `gpio_read_pin` / `exti_config` 等，被几乎所有上层调用。
3. **驱动层**：
   - 时钟：`rcc_clock_config` / `rcc_clock_switch` / `systick_init`
   - GPIO/EXTI：`gpio_init_all` / `board_init_inputs` / `exti_config`
   - ADC/DMA：`adc_hw_init` / `adc_dma_init` / `adc_dma_config` / `adc_task`
   - 定时器：`tim_timebase_init` / `tim_irq_handler` / `tim14_init` …
   - 内部 Flash：`flash_*` 一组（0x0800587C–0x08006D14）
   - IWDG：`iwdg_init` / `iwdg_refresh` / `iwdg_start`
   - 2.4G：`rf_bus_init` / `rf_spi_*` / `rf_chip_config` / `rf_init`
4. **应用层**（主循环 18 个任务 + 中断服务）：`adc_task`、`key_scan_task`、`flash_power_task`、
   `flash_charge_state_machine`、`flash_trigger_state_machine`、`display_task`、`rf_tx_task`、
   `charge_enable_task`、`system_reinit_task` 等。
5. **中断层**：SysTick(ms 计数)、TIM14(主循环心跳/充电反馈)、TIM2(RF 超时)、TIM1(触发)、
   EXTI0_1 / EXTI4_15(触发输入)、DMA1_CH1(ADC)、RTC。

### 关键事实

- 全镜像 **323 个已识别函数**，反汇编覆盖 35% 的映像字节（其余为数据区/字面量池/未解码填充）。
- 主循环 `main` 的 18 个任务由 **TIM14 中断分频（1/25）** 驱动，SysTick 仅做 1 ms 计时。
- 所有外设通信均为 **GPIO 软件时序**：2.4G = 4 线软 SPI，按键 = 3×3 矩阵，
  充电/触发 = 直连 GPIO 脉冲；没有任何硬件 USART/SPI/I2C 使用。
- 内部 Flash 参数页 **0x0801F000**，参数块 8×u32（RAM 镜像 0x20000464）；
  标志页 0x0801F800 存放魔数 0x0000100E。
- 三个"大函数"（`rcc_clock_config` 451 条、`dma1_ch1_task` 320 条、
  `flash_charge/trigger_state_machine` 293/298 条）是理解固件的重点。
'''


if __name__ == '__main__':
    main()
