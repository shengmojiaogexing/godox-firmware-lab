# devices/x3c/emu — Godox X3-C firmware debug simulator

Unicorn-based Cortex-M emulator + debugger for
`devices/x3c/firmware/X3_C_v1.38_app.bin` (Godox X3-C TTL trigger, firmware v1.38).
No third-party SDK is needed: the embedded Python at
`tools/python/python.exe` already has unicorn 2.1.4 + capstone 5.0.9.
(Pillow/tkinter are *not* required: PNG output uses a built-in zlib writer,
the GUI is optional/degraded.)

## Quick start

```powershell
# boot the firmware and write a boot log to devices/x3c/emu/logs/boot_<ts>.log
tools/python/python.exe devices/x3c/emu/run.py --max-insn 50000000 --quiet

# boot, then scan SRAM for framebuffer candidates and render them to PNG
tools/python/python.exe devices/x3c/emu/run.py --max-insn 200000000 --scan-fb

# dump one framebuffer region directly
tools/python/python.exe devices/x3c/emu/run.py --max-insn 200000000 ^
    --fb 0x20025000 240 240 rgb565

# MMIO access trace (CSV)
tools/python/python.exe devices/x3c/emu/run.py --trace --max-insn 5000000

# interactive debugger
tools/python/python.exe devices/x3c/emu/debug.py
(emu) boot
(emu) run 2000000
(emu) r
(emu) u 0x08004AF8 20
(emu) b main 0x080176B0
(emu) c
(emu) fb            # scan SRAM for framebuffers -> PNGs in logs/fb_candidates
```

## Image layout (important)

The application is linked at **0x08004800** (`vaddr = 0x08004800 + file_offset`),
*not* at 0x08000000. The first 0x4800 bytes of flash are a bootloader that is
not part of the .bin; the emulator maps that gap and puts a sane vector table
word0/word1 there because the app reset stub loads SP from `*(u32*)0x08000000`.

* vector table at file 0 (131 entries): SP=0x200477C0, Reset=0x08004AF9.
* reset handler at file 0x2F8 = 0x08004AF8 (SP=0x20001000, option-byte check,
  SP from flash[0], SystemInit 0x0801666C, __main 0x08004A0C).
* file 0x00F3800..0x00F961C is a **second MCU firmware blob** (hot-shoe MCU,
  linked at its own 0x08002000 base, ~30 vectors). It is treated as flash data
  at vaddr 0x080F8000.. — the main firmware streams it to the hot-shoe MCU.
* STM32F4/F7-class peripheral map; option bytes at 0x1FFFC000 are preset to the
  "already provisioned" state (`opt02=0x000F`, `opt10=0x0002`) because the
  firmware's reset stub otherwise programs them and issues SYSRESETREQ.
  See `devices/x3c/emu/FINDINGS.md` for the disassembly evidence.

## Files

| file | purpose |
|------|---------|
| `machine.py` | Unicorn Cortex-M4 machine: flash/RAM/PPB map, peripheral registry + models (RCC, FLASH, GPIOA-K, SPI1-3, USART1-6, I2C1-3, ADC1-3, DMA1/2, TIM1-14, RTC, IWDG, EXTI, USB OTG FS, FMC, CRC, RNG, PWR, DBGMCU, SCB/NVIC/SysTick/DWT/MPU), MMIO access trace + hot/unknown reports, breakpoints, watchpoints, exception dispatch/return, fast burst mode |
| `debug.py` | `Debugger` class + interactive CLI / non-interactive script runner |
| `display.py` | framebuffer models (RGB565/BGR565/1bpp/8bpp), pure-python PNG writer, SPI-LCD command sniffer, SRAM framebuffer candidate scanner |
| `input.py` | scriptable input injection (GPIO keys/power/touch, EXTI/NVIC pulses, input scripts) |
| `rf.py` | SPI RF transceiver stub: records TX frames w/ timestamps, RX injection, CSV/JSON export |
| `run.py` | headless boot driver: boot log with PC progress, stop reason, hot/unknown MMIO, hang analysis, framebuffer dump |
| `probe.py` | early throwaway entry-point probe (kept for reference) |
| `logs/` | boot logs, MMIO CSVs, framebuffer PNGs |

## Python API

```python
import sys; sys.path.insert(0, r"devices/x3c/emu")
from machine import Machine
m = Machine()                 # loads the default image at 0x08004800
m.boot()                      # SP/PC from the app vector table
m.set_fine(True)              # per-instruction hook (breakpoints/trace)
m.run(max_insns=2_000_000)
print(m.summary())
m.set_fine(False)             # fast burst mode (~20-50x) for long delays
m.run(max_insns=500_000_000)

from debug import Debugger
d = Debugger()
d.boot()
d.breakpoint("adc_init", 0x08018C18)
d.run(5_000_000)
print(d.registers(), d.disasm(), d.hot if hasattr(d, "hot") else "")
d.save_png(0x20025000, 240, 240, "rgb565", "logs/fb.png")
d.scan_fb()
```

## Peripheral model behaviour

* **RCC** — ready bits are derived from enable bits each read, CFGR SWS mirrors
  SW, BDCR LSERDY when LSEON. Clock init always completes.
* **FLASH** — never busy, no errors; option-byte programming sequence accepted.
* **GPIO** — MODER/OTYPER/OSPEEDR/PUPDR/ODR/BSRR/LCKR/AFR register file with
  input-level injection (`gpio.set_input(pin, level)`); listeners fire on ODR
  changes so SPI CS lines / display D-C lines can be wired up.
* **SPI** — instant transfers: writing DR calls the attached device's
  `on_tx_byte()`; any reply byte is queued in RX and TXE/RXNE/BSY flags are
  provided for polling drivers. `attach_cs()` wires a GPIO pin.
* **UART** — TX bytes recorded, RX injectable, SR flags compatible.
* **ADC** — both F4-style (SR/CR2) and F3/L4-style (ISR/CR) layouts merged;
  ADRDY/EOC/JEOC set, ADSTART/JADSTART self-clear.
* **DMA** — transfers "complete instantly": EN self-clears, TCIF set.
* **TIM** — counters run off the virtual instruction clock (CNT/PSC/ARR, UIF).
* **RTC/IWDG/PWR/USB/FMC** — stubs with ready bits / self-clearing bits so
  init code does not spin.
* **PPB** — CPUID=0x410FC241, VTOR, AIRCR (SYSRESETREQ reboots), SHCSR,
  CFSR/HFSR, MPU readback, CPACR; SysTick with LOAD/VAL/CALIB and IRQ15
  delivery; NVIC ISER/ICER/ISPR/ICPR/IPR/STIR with pending IRQ dispatch into
  the vector table at chunk boundaries; DWT CYCCNT tied to virtual time.
* **Unknown MMIO** — configurable policy (`last` default: reads return the last
  written value or 0); every access is logged with the PC; unmodelled pages can
  be auto-mapped (logged) so boot continues.
* **Exceptions** — HardFaults are taken into vector[3] when possible;
  EXC_RETURN (0xFFFFFFF9/FD) is implemented manually (stack unstacking).

## Fast vs fine mode

`Machine.set_fine(False)` removes the per-instruction Python hook and executes
in 200k-instruction bursts (~20-50x faster). Use it for long software delays
(e.g. the firmware delay loop at 0x0801C160), then `set_fine(True)` for
breakpoints / step / call trace / PC ring. `run.py` runs the first
`--fine-insn` (default 200k) instructions fine, then switches to fast mode
automatically; use `--fine` to disable that.

## Logs and reproducibility

`run.py` writes `logs/boot_<timestamp>.log` with: instruction count, stop
reason, final PC, SP/LR, first/last PCs, hang analysis, hot MMIO, unknown MMIO,
register dump, fault state and the machine summary. With `--trace` it writes
`logs/trace_<ts>.csv` (vtime, pc, dir, addr, size, value, region). Runs are
deterministic given the same options (single-threaded, no wall-clock input).

## Status / roadmap

* Boot: reset -> option-byte provisioning check -> SystemInit -> __main ->
  driver init (GPIO/clock/ADC/SPI/DMA/EXTI) -> long delay loop. See
  `FINDINGS.md` for the latest stop PC and what is missing.
* Next: reach the UI/main loop, identify the LCD interface (FMC vs SPI vs
  parallel FSMC), find the framebuffer in SRAM and capture the first screen.
* Optional: GDB remote stub on port 3333, symbol import from
  `devices/x3c/re/ghidra_exports`.
