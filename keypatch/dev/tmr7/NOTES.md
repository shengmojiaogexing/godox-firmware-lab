# TMR7 1 ms tick model for the X3-C Unicorn emulator — work notes

Task: teach the existing `devices/x3c/emu` Unicorn emulator the **TMR7 1 ms system tick**
(IRQ55) of the Godox X3-C firmware (`devices/x3c/firmware/X3_C_v1.38_app.bin`, AT32F435,
Cortex-M4, app linked at `0x08004800`), so the firmware leaves the boot/battery
screen and the real UI (home page / quick panel / settings grid) can be reached,
and screenshots / input injection become possible.

Everything here is **emulator-side only**. No firmware byte is patched in the
`.bin`; `devices/x3c/emu/*.py` is **not modified** (the changes are process-local
monkey patches installed by `tmr7_model.py`). This directory
(`keypatch/dev/tmr7/`) is the only place files were written.

---

## 0. TL;DR status

| item | status |
|---|---|
| TMR7 CNT / PSC / ARR modeling, UIF, `pend_irq(55)` | **done** — IRQ55 dispatches into the real handler `0x08016D50` |
| 1 ms software tick at RAM `0x20000310` advances | **done** (continues at ~current instruction rate; see §7) |
| firmware boots past power-management "low battery shutdown" | **done** (ADC IRQ18 model + RCC fixes, see §5) |
| real LCD screenshots (368×448 RGB565 → PNG) | **done** — boot/battery page captured; files in `out/` |
| main UI / home page reached | **not yet** — see §8 for the exact remaining blockers |
| input injection path | GPIO input forcing works (`--gpio PA1=1,PA8=1`); `devices/x3c/emu/input.py` API also usable; touch controller bus not modelled (§9) |

The single most important finding for the tick question: **TMR7 modeling was the
easy part; the firmware then powered itself off ("low battery") because the
emulator never delivered ADC interrupt IRQ18, so the battery measurement RAM
stayed 0.** Fixing that (plus 4 latent `devices/x3c/emu` bugs, §4) made tick advance
without shutdown.

---

## 1. Firmware facts recovered (with evidence)

### 1.1 TMR7 base / IRQ / handler

| item | value | evidence |
|---|---|---|
| TMR7 registers | `0x40001400` (offset map identical to STM32 TIM7: CR1+0, DIER+0xC, SR+0x10, EGR+0x14, CNT+0x24, PSC+0x28, ARR+0x2C) | literal pool `0x08016FA0` = `0x40001400`, also `0x08039C88`, `0x0802029C`, `0x08035A3C` |
| IRQ55 handler | vector[16+55] = `0x08016D51` → `0x08016D50` | `vector_table.csv` / image word at `0x0800491C` |
| init sequence | see §1.2 | MMIO trace, `logs/probe_boot_8m.log` |

### 1.2 TMR7 init registers (MMIO trace, `probe_boot.py`)

```
W 0x4000142C = 0x00000063   ; ARR = 99   -> period 100
W 0x40001428 = 0x0000002F   ; PSC = 47   -> divider 48
W 0x40001414 = 0x00000001   ; EGR UG (load counter)
W 0x40001400 = 0x00000000   ; CR1 (still off)
W 0x4000140C = 0x00000001   ; DIER UIE = 1
NVIC IPR[55] = 0x10         ; 0xE000E437
NVIC ISER1  = 0x00800000    ; 0xE000E104, IRQ55 enable
W 0x40001400 = 0x00000001   ; CR1 = 1  -> timer starts   (vtime 241886)
... re-init at vtime 2576374 (same values, CR1 already 1)
```

`(PSC+1)*(ARR+1) = 48*100 = 4800` TMR7 input clocks per update interrupt).
At a 48 MHz TMR7 clock that is 100 µs ⇒ the ISR runs at 10 kHz and advances the
software 1 ms tick every 10th interrupt. This matches the observed ISR code.

### 1.3 TMR7 ISR `0x08016D50` (decompiled `0x08016d50_FUN_08016d50.c`)

* first checks `TMR7->SR & 1` (UIF) via `0x08039916`; returns if not set;
* increments byte at `0x20000000` (0…9 ms sub-counter);
* every 10th interrupt calls `0x080334EC(1)` which is:
  ```
  *(u8*)0x2000030C = 0;          // "a tick happened" flag, see get_tick
  *(u32*)0x20000310 += 1;        // 1 ms software tick
  ```
* also services GPIO paper/other counters and clears UIF at the end with
  `*(u32*)(TMR7+0x10) = ~1` (rc_w0: writing 0 to bit0 clears UIF);
* runs with the TMR7 SR bit used as a pseudo "mutex" (`0x08039916(TMR7,1)`).

So the **RAM variables that matter**:
`0x20000000` sub-counter, `0x2000030C` tick flag, `0x20000310` ms tick
(read by `get_tick` `0x080334D8`, `tick_advance` `0x080334EC`).

### 1.4 Wait / timing helpers

* `get_tick` `0x080334D8`: reads tick `0x20000310`, writes flag `0x2000030C = 1`,
  spins while flag reads 0. On real hardware the TMR7 ISR clears the flag
  between the store and the load, so each call costs ~1 ms.
* `FUN_0801C118` (µs delay) and `FUN_0801C0FE` (ms delay) are calibrated
  nop-sleds. `devices/x3c/emu/demo_x3.py` already short-circuits them to `bx lr` at
  runtime; this work does the same (`patch_delays`), because the emulator has no
  cycle model and they would otherwise burn 1e8+ instructions.

### 1.5 Battery ADC → power-off decision (why the tick "stopped at ~117 ms")

The UI never appeared after the tick fix; the CPU was found parked in
`FUN_08010C00`'s `WFI` loop (`0x08036068`) — the firmware's **power-off path**.

Call chain: `main 0x08033E20` → `FUN_08010C8C` (power management) → boot
measurement loop → eventually `0x080112C0: bl FUN_08010C00` (WFI forever).

`FUN_08010C8C` loop (disassembly around `0x08010CEE`–`0x08010DB4`):

```
r4 = flags = 0
repeat 70 times:
    if (GPIOA->IDR & 0x100) == 0 for >65 iters:          flags |= 0x40  (PA8 low)
    start ADC conversion (FUN_08018C88 / FUN_08018E46)
    delay; ratio = u16[0x20000168] * 1000 / u16[0x2000016A]
    if ratio > 0x28A (650):  after 66 iters  flags |= 0x10  (battery good)
                             after 66 iters  flags |= 0x80
    if (GPIOA->IDR & 1) == 1 and (GPIOA->IDR & 0x100) == 1 for >4 iters:
                                                    flags |= 0x20  (PA1 high)
...
switch (flags >> 4):
  case 2,6,10,11 : ON-state loop (waits for *0x20000006 UI-tick flag,
                   calls scheduler FUN_080335BC)         <-- our PA1=PA8=1 path
  case 4         : battery-page path (waits 800 ticks then power-off check)
  default/0xD    : return to main (normal continuation)   <-- normal battery boot
...
at the end of the "ON" path:
  if (!(flags & 0x10) || wait_ms <= 200) FUN_08010C00();   // power off!
```

`u16[0x20000168]`/`u16[0x2000016A]` are written **only by the ADC interrupt
handler** IRQ18 `FUN_0800510C`:

```
if (ADC->STS & 0x04) { ADC->STS = ~0x04; ch0 = ADC->DR0; ch1 = ADC->DR1;
                       u16[0x20000168] = ch0; u16[0x2000016A] = ch1; }
```

In stock `devices/x3c/emu`, the demo's ADC patch returns `STS = 0x42` (bits 1 and 6,
enough for the ADRDY poll) and **IRQ18 is never pended**, so the handler never
runs and both RAM values stay 0 ⇒ ratio = 0 ⇒ firmware decides "battery dead" ⇒
power-off. That is exactly the observed `tick` stop at ~117 (≈ the moment the
power-off decision is taken). See `logs/run_20261107_022707.log` (before fix)
vs. `logs/run_20261107_023531.log` (after fix).

`GPIOA` bits used by this logic: `PA1` (bit1) and `PA8` (bit 0x100). Their
real meaning is not identified (likely USB/charger detect + power key); the
emulator default is 0 (GPIO IDR mirrors ODR). Both can be forced with
`--gpio PA1=1,PA8=1` (see §9).

---

## 2. What this patch does (files)

`tmr7_model.py` — the whole model. Public API:

```python
import tmr7_model
m = Machine(image=...)
tmr7_model.patch_real_device(m)      # option bytes / ADC stub / delay short-circuit
model = tmr7_model.attach(m, scale_num=1, scale_den=1)   # patches + TMR7 + TMR5
sp, pc = m.boot()
m.run(...)
```

`run_ui.py` — driver: boot, run, progress log (tick / PC / page vars / LCD
stats), PNG snapshots, optional `--gpio`, `--force-page`.
`lcd_capture.py` — full-screen compositor of the XMC/8080 LCD stream
(CASET/RASET/RAMWR/MADCTL aware), writes PNG (adapted from
`devices/x3c/emu/demo_x3.py`'s `LcdBus`).
`probe_boot.py` — boots with the stock emulator and logs every MMIO access to
TMR7/NVIC/RCC/SCB (used to derive §1.2).
`recon.py`, `resolve_ptrs.py` — static helpers (literal-pool refs, DAT_* →
pointer resolution, disassembly).
`debug_*.py` — throwaway diagnostics used while chasing the power-off/shutdown
and the UI-init faults (kept for reproducibility).

---

## 3. The TMR7 model (minimal intrusion)

Stock `devices/x3c/emu/machine.py` already has a generic `Timer` at `0x40001400`
(named `TIM7`), but:

1. `Timer._tick()` only runs lazily on a register *read*;
2. it never calls `Machine.pend_irq()`;
3. its `SR` write uses `sr &= ~value` (rc_w1) while AT32/STM32 update flags are
   rc_w0.

### 3.1 Where the model runs

`install()` monkey-patches `Machine._check_interrupts`, which the stock run loop
calls at **every 2000-instruction chunk boundary** (both fine and fast mode):

```python
def _fixed_check_interrupts(self):
    _advance_models(self)                       # TMR7 + TMR5 models, vtime delta
    if active_exception: return False
    if not pending_irqs: return False
    if PRIMASK: return False
    cands = [i for i in pending_irqs if i >= 16 and (nvic_enabled >> i) & 1]
    cands.sort(key=(priority, irq))             # Cortex-M priority then number
    take_exception(irq + 16)                    # exception number = IRQ + 16
```

`Tmr7Model.advance_to(vtime)`:

```
if CR1.EN == 0: return
acc_units += dv * scale_num                     # emulated insns -> TMR7 clocks
units = acc_units / scale_den
prescale_phase += units; ticks = phase / (PSC+1); phase %= PSC+1
total = CNT + ticks
periods = total / (ARR+1); CNT = total % (ARR+1)
if periods:
    SR |= UIF
    if DIER.UIE: pend_irq(55)
```

**Approximation**: the emulator has no cycle model, so one instruction is
modelled as one TMR7 input clock (`scale = 1`). With PSC=47, ARR=99 this gives
one update IRQ per **4800 instructions** and one 1 ms software tick per
**48000 instructions** (10 IRQs). A real device runs TMR7 from a 48 MHz clock
and the CPU at 96–288 MHz, so the true instruction/tick ratio is larger; the
firmware only observes "ticks", so the UI timeouts are preserved, but wall-clock
instruction count per virtual millisecond is an approximation.
`scale` (`--scale`) can be raised to make UI timeouts pass faster in exchange
for a coarser interrupt grid (one IRQ per chunk boundary max).

### 3.2 Writes / reads of TMR7

`Timer.write` is patched globally to the correct rc_w0 semantics
(`offset 0x10: sr &= value`), so the ISR's `STR 0xFFFFFFFE, [TMR7+0x10]`
clears UIF. The TMR7 instance gets its lazy `_tick` replaced with a no-op (the
model owns CNT) and its `write` wrapped to reset phase accumulators on
CR1/EGR/CNT/PSC/ARR writes.

---

## 4. Four latent `devices/x3c/emu` bugs fixed by monkey patch (documented, not edited)

These were *not* caused by the tick model; they are stock-model gaps that a
running external interrupt exposes. All are patched at runtime only.

| # | symbol | stock behaviour | bug | patch |
|---|---|---|---|---|
| 1 | `Machine._check_interrupts` | `(nvic_enabled >> (irq-16)) & 1` | `PPB.write` stores ISER bits as **bit == irq**, so IRQ55 enabled bit 55 is tested at bit 39 → never dispatched. Also `take_exception(irq)` indexes the vector table with the IRQ number, landing 16 vectors early (default `b .` handler at `0x08004B2C`) | test `>> irq`; dispatch `take_exception(irq + 16)` |
| 2 | `Timer.write(0x10)` | `sr &= ~value` (rc_w1) | update flags are rc_w0; ISR write of `~1` left UIF set forever | `sr &= value` |
| 3 | `Machine._handle_uc_error` | handles WFI/invalid insn only | Unicorn 2.1.4 raises `UC_ERR_EXCEPTION` when an ISR returns via `EXC_RETURN` (`bx lr` with LR=0xFFFFFFF9) at PC=0xFFFFFFF8; the stock fetch-unmapped EXC_RETURN path never runs, so the first ISR return becomes a HardFault | detect PC in `0xFFFFFF00..` with `active_exception` set and call `Machine._exception_return()` |
| 4 | `RCC.write/read` | write CFGR ignores `SW=0`; CSR never raises LSIRDY | (a) clock-init poll `0x0801BDBC` (`tst CFGR,#0xC / bne`) spins forever after the clock was once switched to PLL; (b) RTC/LSI init `0x080203EC` polls `CSR & 2` (LSIRDY) forever — with interrupts disabled | store CFGR as written, mirror SW→SWS on read; always report LSIRDY |

(Stock `machine.py` also references `UC_ARM_REG_PRIMASK` without importing it —
the original `_check_interrupts` would raise `NameError` as soon as any IRQ was
pending. The patched version imports it from `unicorn.arm_const`.)

---

## 5. Peripherals added for the boot power path

### 5.1 ADC — `X3AdcBlock`

The firmware's battery measurement runs through ADC + IRQ18:

* `FUN_08035280` (ADC init): configures ADC `0x40012000`, enables IRQ18
  (`FUN_08035C78(0x12,1,0)`), enables ADC (`CTRL2+0x08` bit0), polls
  `STS & 0x40` (ADRDY).
* `FUN_08025F30`/`FUN_08010C8C` start conversions by writing
  `CTRL2` bit 22 (ADSTART, `FUN_08018E46`).
* IRQ18 handler `FUN_0800510C` checks `STS & 0x20` and `STS & 0x04`; on the
  latter reads `DR0 (+0x3C)` and `DR1 (+0x40)` and stores them to
  `0x20000168`/`0x2000016A`.

Model:
* `STS = 0x64` (bits 6/5/2 → satisfies the ADRDY poll and the IRQ handler),
* `CTRL2` self-clears ADCAL (bit3), bit2 and ADSTART (bit22) on read,
* data registers `0x3C/0x40/0x44/0x48` return `raw` (default 2600),
* writing `CTRL2.ADSTART` (bit22) **pends IRQ18**.

With `ch0 == ch1 == 2600` the firmware computes
`ratio = 2600*1000/2600 = 1000 > 650` ⇒ battery "good" ⇒ the boot path no
longer powers off. (This mirrors a real cell at a healthy voltage; the exact
raw value is arbitrary — see `--adc-raw`.)

### 5.2 TMR5 capture — `TmrCaptureModel`

The same power path (and other boot code) configures **TMR5 `0x40000C00`** for
input capture on channel 4 (`CCMR2/CCER`, `CR1=1`, `DIER |= 0x10` = CC4IE,
NVIC IRQ50 via `FUN_08035C78(0x32,0)`), then busy-waits for
`RAM[0x200000F4] == 2` (see `FUN_08025F30`). The TMR5 ISR `0x08016C00` reads
`CCR4 (+0x40)`, stores captures at `0x2000001C`, increments `0x200000F4` and
clears CC4IF.

The captured signal comes from an external pin that is not modelled, so the
firmware would wait forever. The model injects a deterministic capture event
when: TMR5 counter enabled, CC4IE armed, CC4IF clear, **and the CPU is actually
spinning in the wait loop `0x08025FA4..0x08025FA8`** (checked via
`Machine.last_pc`, so TMR5 stays armed elsewhere without generating an IRQ
storm). Two injected captures set `0x200000F4 == 2` and the boot code continues.
`CCR4` is filled with a ramp; the boot code only needs the two interrupts and
does not use the captured value for its decision.

Known limitation: no real waveform is modelled; if later UI code measures a
real pulse width it will see the fake ramp.

---

## 6. Screenshots + reproducibility

### 6.1 Commands

```powershell
cd <repo>

# recon: where the firmware touches TMR7/NVIC/RCC (writes logs/probe_boot_8m.log)
& .\tools\python\python.exe keypatch\dev\tmr7\probe_boot.py --max 8000000

# TMR7 model, normal battery boot state, 12M instructions, PNG snapshots
& .\tools\python\python.exe keypatch\dev\tmr7\run_ui.py --max 12000000 --snapshot-every 3000000

# "ON" state (PA1=PA8=1), long run, snapshots every 8M, log every 10M
& .\tools\python\python.exe keypatch\dev\tmr7\run_ui.py --max 160000000 `
    --progress-every 10000000 --snapshot-every 8000000 --gpio PA1=1,PA8=1 --tag long
```

Outputs: `keypatch/dev/tmr7/out/ui_*.png`, logs in `keypatch/dev/tmr7/logs/`.

### 6.2 Results (machine: `tools/python/python.exe`, ~1–1.5M insn/s)

* With TMR7 only (stock ADC stub): boot reaches the power-off WFI loop at
  `PC=0x08036068`, `tick≈117`, no UI (`logs/run_20261107_022707.log`).
* With ADC IRQ18 model + RCC fixes: tick advances continuously; the cold-boot
  power-measurement path works; firmware keeps running
  (`logs/run_20261107_023531.log`).
* With `PA1=PA8=1` (emulated "USB/charger + key" state) the firmware enters the
  ON-state loop (`FUN_08010C8C` big branch): 30M instructions, tick 619,
  no fault, real LCD stream decoded — see
  `out/ui_g11_0016000000_tick00328.png` etc. **But** the UI page variable
  `RAM[0x200003E7]` stays 0 and the screen stays on the battery/charging page.
* Boot-page screenshots (real LCD stream from the firmware):
  * `out/ui_ui_0006400000_tick00128.png` — boot battery page (cold boot path)
  * `out/ui_final.png` — battery page, long run (ON path)
* No `devices/x3c/emu` file is modified; `git status` (if run in a repo) should show
  only `keypatch/dev/tmr7/**`.

### 6.3 Key numbers for the tick model

| run | instructions | TMR7 ticks (`0x20000310`) | PC at snapshot | IRQ55 entries |
|---|---|---|---|---|
| stock demo (no tick) | 8,000,000 | 0 | `0x080335E0` (waits 800 ticks) | – |
| TMR7 only | 12,000,000 | ~0 (power-off at 117) | `0x08036068` WFI | first at vtime 248,272 |
| TMR7 + ADC/RCC | 12,000,000 | 244 | `0x08025FA4` (TMR5 wait) | ~1 per 4.8k insns |
| + PA1/PA8=1 (ON loop) | 30,000,000 | 619 | `0x08010FBC` | 6199 pends |

Tick math: `1 tick = 10 IRQ55 = 10 * 4800 = 48000` instructions at `scale=1`.

---

## 7. Remaining blockers (what is still missing for the home screen)

1. **UI page never leaves 0.** In the ON-state loop the firmware waits for
   `RAM[0x20000006]` (set only by `FUN_08010C8C`'s peers) — no code path we
   reached sets it; the 100 ms UI callback `FUN_08014B28` is only active when
   `RAM[0x200003E7] == 2`, and `0x200003E7` stays 0. Writing the "requested
   page" `RAM[0x200004C4] = 1` (ui-x3c's guess) does not change it.
2. **Cold-boot continuation path (`flags>>4 = 0xD`) hard-faults** at ~3.3–5.4M
   instructions in UI rendering (`FUN_0801DD34` with a NULL destination
   pointer, and scheduler list traversal `FUN_08017F5E` with
   `*list = 0x1C`). `ui-x3c` hit the same class of issue: the "root window"
   pointer `RAM[0x20000470]` is NULL — the UI window subsystem has not been
   initialised on this path (probably an init step skipped because the emulated
   power/charge state differs from a real device).
3. **The ON-state loop is not the normal boot continuation.** It is the
   power/charge state machine; the real home UI is probably reached from the
   `FUN_08010C8C` return path (case 0xD) that currently faults in (2).

Next steps (for whoever continues):
* identify the init that sets `RAM[0x20000470]` (refs listed by
  `recon.literal_refs(0x20000470)`) and `RAM[0x200003E7] = 2`, and find which
  emulated hardware condition gates it (likely USB/charger detect = PA1 or an
  ADC/PMU register);
* alternatively model the remaining power-management peripheral (the one that
  sets `RAM[0x20000006]`, likely DMA/LCD VSYNC or EXTI) to let the ON loop tick;
* once page 1 renders, capture home / quick panel / settings grid and then
  verify the keypatch tile patches.

---

## 8. Input injection

Two levels are available:

1. **GPIO levels** (works today, emulator-side):
   `run_ui.py --gpio PA1=1,PA8=1` forces `GPIOA` input levels via
   `machine.gpio[...].set_input(pin, level, driven=True)` (same API as
   `devices/x3c/emu/input.py`). This is how the "power key/USB present" boot state was
   emulated.
2. **`devices/x3c/emu/input.py`** (`InputController`): `map_key()` + `press/release`
   drive a GPIO + EXTI + NVIC IRQ, `InputScript` replays timed events. It is
   usable with this model (the machine object is a normal `Machine`), but the
   firmware's real key/touch bus is not identified yet, so the key mapping is
   unknown.

**Touch**: no I2C/SPI touch controller has been located on the bus yet
(`devices/x3c/emu/input.py` has a `TouchController` stub but it is not wired to any
firmware path). If the touch controller is later found on I2C, `input.py`'s
`map_touch()` + `i2c_rx()` can inject coordinates. The practical fallback for
screenshots is to drive the UI state machine directly: write the UI mode/page
vars (`0x200003E7`, `0x200004C4`) and let the UI tick run — this is what
`run_ui.py --force-page` does (it currently has no effect because the UI
subsystem is not past its init, see §7).

---

## 9. Performance / how long things take

* fine mode (per-instruction Unicorn hook, needed by the stock machine's
  breakpoints/PC ring): ~1.0–1.5M instructions/s with the tick model.
* 12M instructions ≈ 9–12 s; 30M ≈ 25 s; 160M ≈ 2–3 min.
* Fast burst mode (`Machine.set_fine(False)`) is **not recommended** with the
  tick model: the model only advances at `_check_interrupts`, which in fast mode
  is called once per 200 000-instruction chunk, so interrupt granularity becomes
  very coarse (a whole UI tick would be collapsed).

---

## 10. Files

```
keypatch/dev/tmr7/
  NOTES.md            this file
  tmr7_model.py       the model + all monkey patches (no devices/x3c/emu edits)
  run_ui.py           driver: boot + model + LCD capture + snapshots
  lcd_capture.py      368x448 RGB565 XMC/8080 framebuffer compositor -> PNG
  probe_boot.py       MMIO trace of TMR7/NVIC/RCC (recon evidence)
  recon.py            static analysis helpers (literal refs, BL callers, disasm)
  resolve_ptrs.py     resolve DAT_* literal-pool pointers used by the ISR
  debug_*.py          diagnostics used while chasing shutdown/faults
  out/                PNG screenshots
  logs/               run logs (probe_boot_8m.log, run_*.log)
```

---

## 11. Final status of this session (2026-11-07 ~02:50)

What is proven working:

* TMR7 register model + IRQ55 dispatch into the real ISR `0x08016D50`:
  the 1 ms tick `RAM[0x20000310]` advances reliably (48 000 instructions per
  tick at `scale=1`); logs show `[exc] enter IRQ 71 -> handler 0x08016d50` and
  the ISR running to completion.
* Firmware no longer powers itself off for "low battery": ADC IRQ18 model
  writes `0x20000168/0x2000016A = 2600` so `ratio = 1000 (> 0x28A)`; logs
  confirm `adc_ch0=2600 adc_ch1=2600`.
* Real LCD byte-stream capture works: 368×448 RGB565 → PNG; the boot/battery
  page is reproduced from the firmware's own XMC writes.
* GPIO input injection works (`--gpio PA1=1,PA8=1` changes the boot power state
  and the code path taken by `FUN_08010C8C`).
* Boot flow observed: `FUN_08010C8C` → boot battery measurement → either
  ON-state loop (`uVar12>>4 == 0xB`, when PA1=PA8=1) or return to main
  (`uVar12>>4 == 0xD`, both low). The "ON" path runs indefinitely with tick
  (30M/160M instruction runs, no fault), but stays on the battery/charging
  page.

What is *not* reached: the home page.

Evidence for the remaining blocker (checked with the tools here):

* `RAM[0x200003E7]` ("UI mode/page", read by the UI tick callback
  `FUN_08014B28` at `0x08014B2C`) stays 0 unless forced. `FUN_08014B28`'s
  whole body is gated by `== 2`; it is the only caller of the page switcher
  `FUN_08006F0C` (`0x08014C90`) and of `FUN_08006F0C(page, page_id)`.
* The function that writes `mode = 2` is the **home-page builder** at
  `0x080148A8` (`strb 2, [0x200003E7]` at `0x080148B4`); it builds a
  0x170×0x1C0 window and 16 tiles. It is called from `0x0801579E` (a UI page
  builder in a code region Ghidra did **not** recognise as a function), which
  itself is not reachable by a direct BL — it is a registered page callback.
  So the framework must first switch to page 1, which requires mode==2 and a
  valid window tree: a chicken-and-egg that only the real UI init resolves.
* `RAM[0x20000470]` (UI root window pointer) remains 0 in every run (the same
  NULL root window `ui-x3c` reported). Window creation (`0x08010ACC`) happens
  once (~2.55M) but the tree is never completed.
* On the normal continuation path (`uVar12>>4 = 0xD`) a HardFault happens at
  ~3.3–5.4M instructions in UI rendering:
  * `FUN_0801DD34` writes to a NULL destination buffer (`strb [sl,r5]`,
    `sl=0`) — it is called from `FUN_0802A5C4` (font/image blit);
  * `FUN_08017F5E` (scheduler timer-list traversal) reads `[0x1C]` because the
    list header holds a bad/low pointer; call site `0x08033608` in the
    scheduler `FUN_080335BC`.
  These are consistent with "UI objects not initialised", not with the tick
  itself.
* Forcing `mode=2` + requested page `1` at 0x600k and continuing does not
  produce a page switch: the UI tick callback is never invoked because the
  scheduler `FUN_080335BC` is not called in the ON-state loop in our runs
  (call-trace count 0), even though the loop contains a conditional call to it
  at `0x08010FEA` (gated on `uVar12 & 0x10`, which is set). The loop's
  wake-up flag `RAM[0x20000006]` is observed to become 1 only once (~9M), so
  the loop runs a single iteration and then waits again — the periodic setter
  of this flag is not modelled/identified.

Practical bottom line: the emulator now has a working TMR7 1 ms tick and can
faithfully reproduce the boot screen, and the tick itself is not the blocker
anymore. Reaching the home page needs the UI window/root-window init to be
completed (find what should set `RAM[0x20000470]` / what gates the page
framework) or a way to invoke the registered page builder on a valid window
tree — see §7 for the proposed next steps. `ui-x3c`'s tile-patch work can be
validated as soon as one page renders.

Recommended next actions:
1. Find `RAM[0x20000470]` writer (refs: `0x080059A8, 0x0800D360, 0x0800FE64,
   0x0800FF70, 0x0801083C, 0x080153DC, 0x080159C4, 0x0801FFC4, 0x080267D0,
   0x08036E60, 0x08037CCC`) and which hardware condition gates it.
2. Model the peripheral/IRQ that sets `RAM[0x20000006]` periodically (or find
   the code path setting it — the literal only lives in `FUN_08010C8C`, so it
   is probably written through a RAM base pointer from an ISR/callback).
3. Once mode==2 and a page request work, call the home builder
   `0x080148A8` (via the normal page-switch path) and capture PNGs.

---

## 12. Addendum: the "ON" screen is the *charging* screen (important)

With `--gpio PA1=1,PA8=1` the firmware runs its charge/ON state machine
(`FUN_08010C8C` big branch) and draws a **battery with lightning bolt** —
i.e. the X3-C "charging" page, not a frozen boot logo (see
`out/ui_fc_home_tick00192.png`, 322 296 LCD pixels, nonblack 0.289). The
tick is advancing all along; the page just never leaves charging. Therefore
`PA1=1` must mean "USB/charger present". The home screen needs the
battery-powered state (PA1 = 0), which is the `uVar12 >> 4 == 0xD` path that
returns to main and currently HardFaults in the UI rendering (see §11).

`force_call.py` (new, also in this directory) can call arbitrary firmware
functions from a fully booted machine by planting a tiny Thumb stub in unused
CCM RAM (`0x1000F000`):

```
ldr r4,[pc,#4] ; blx r4 ; nop ; nop ; .word fn   (12 bytes per call)
..., b .
```

Results with `--gpio PA1=1,PA8=1 --after 9500000`:

| calls | result |
|---|---|
| `0x0800BA08` | fault inside (needs prior init) |
| `0x0800FE78` (window init in main's cold-boot path) | faults at `0x0802B79E` reading `[0xC]`; sets `0x200003E7 = 1` first |
| `0x080148A8` (home-page builder) | runs until `0x08030450` reading `[0x4]`; sets `0x200003E7 = 2`, but window create returns NULL (`0x20000470` stays 0) |
| `0x08015740` (registered page framework entry) | same window NULL fault |
| `0x0800FE78, 0x080148A8` | `mode=1` then fault in `0x0802B79E` |

So the home-page builder *is* reachable and does set `mode=2`, but it needs
the UI window manager/root window (`0x20000470`) to be initialised first; the
function that does that (`FUN_0800FE78` / window create `0x08010ACC`) itself
requires earlier UI state that our boot path does not produce. That is the
concrete remaining task.

---

## 13. Late-session breakthrough attempts (UI timer + window init)

### 13.1 Who registers the 100 ms UI tick callback

`FUN_080358FC` registers **three 100 ms soft timers** via
`FUN_08033500(period=100, callback, storage)`:

| callback | address |
|---|---|
| `0x0800E31C` | status/UI helper |
| `0x080103C8` | UI helper |
| `0x08014B28` | **the UI tick callback** (literal `0x08014B29` at pool `0x0803593C`) |

Main calls `FUN_080358FC` at `0x08033FBE` in its cold-boot block (the block our
ON/charging path never returns to). Force-calling it from the stub does
register the UI tick timer; with `RAM[0x2000007F] = 1` forced (this byte gates
the TMR7 ISR's periodic `RAM[0x20000006]=1` wake-up, ISR code at `0x08016E5E`)
the scheduler `FUN_080335BC` then runs repeatedly and the UI tick callback is
executed.

### 13.2 Page switch starts rendering

With:
* boot in ON state (`--gpio PA1=1,PA8=1`) to 9.5M instructions,
* stub call `FUN_080358FC`,
* `RAM[0x2000007F] = 1` poked every chunk,
* `RAM[0x200003E7] = 2`, `RAM[0x200004C4] = 1` forced,

the firmware actually starts switching to page 1: the LCD receives a clear +
new window draws (pixels 322296 → 479225, commands 60 → 87) and the first
screenshot shows the **bottom bar of a new page** rendered at the bottom of the
panel (`out/ui_page1_partial.png`). It then HardFaults:

* `0x080180C2` (`get widget->[8]->[0x1C]`, NULL widget) during the page draw
  chain, and/or
* `0x08022572` reading `[0x4]` after the cold-boot UI init sequence.

### 13.3 Window root finally created

Force-calling the main cold-boot sequence `FUN_0800BA08 → FUN_0800FE78 →
FUN_080358FC` (all from the stub at 9.5M) **does create the root window**:
`RAM[0x20000470]` becomes `0x20048C04` (it is NULL in every normal run).
But the subsequent UI timer/page construction then runs into pointer faults
(`0x0802D8F6` heap loop writing outside RAM / `0x08022572` NULL), i.e. the
forced init is incomplete: some RAM state that main normally sets up before
these calls is missing.

### 13.4 Conclusions for the next session

The shortest remaining path to a home screenshot is probably:

1. replicate the *exact* main cold-boot preconditions around `0x08033F80`–
   `0x08034000` (the `bVar18` block): it runs `FUN_08015B38(0xffff)`,
   `FUN_0801C0FE(...)` delays (patched to `bx lr`), `FUN_0800BA08`,
   `FUN_0800FE78`/`FUN_080345B4`, `FUN_080358FC`, 4× `FUN_080335BC`, then the
   page setup;
2. or let the firmware take the normal (non-charging) path and fix the two
   NULL-pointer faults (`0x0801DD34` blit dest, `0x08017F5E` timer list) — both
   look like "UI object not created", so the missing piece might be exactly the
   window/root init that the force-call in §13.3 partially achieves;
3. after a page renders, run the same flow with the patched image
   `keypatch\out\X3_C_v1.38_app_keyD798.bin` (via `run_ui.py --img`)
   and compare PNGs to verify the quick-panel KEY/BCAST tiles and the 3 bottom
   tabs.

Artifacts of the partial page-1 render: `out/ui_page1_partial.png`,
`out/ui_page1_partial_cleared.png`, `out/ui_reg_ui*_*.png`.

---

## 14. Next round: replaying main's cold-boot block directly (`--jump`)

`force_call.py` gained two modes:

* `--jump ADDR [--calls ...]`: the stub calls run first, then `pop {pc}` returns
  into firmware at `ADDR` (with SP restored to main-entry `0x200477C0`, r7=0,
  sl=0x20000035, as main's cold-boot block expects). This lets us replay main
  from the point where `FUN_08010C8C` would return.
* `--break-at ADDR`: run until a PC, then jump (used to stop right before the
  `bl FUN_08010C8C` at `0x08033E9C` and skip it).

Findings:

1. **`FUN_080345B4(0)` is the real main-UI constructor** (not `FUN_0800FE78`).
   It creates the root window (`FUN_08010ACC(0,0,0,0,0x170,0x1C0)`), top bar,
   tiles, etc. `FUN_0800FE78` is the other branch (USB/charge), taken when
   `RAM[0x20000034] & 0x80`. On our boot `RAM[0x20000034] == 1` ⇒ the normal
   branch is `FUN_080345B4(0)`. The earlier `--jump 0x08033F88` run executed it
   and **created the root window** (`0x20000470 = 0x20048C04`) and drew a
   white page with a black bottom bar (`out/ui_main_init_white.png`,
   `out/ui_main_init_bottom_bar.png`), then panicked in `FUN_08025094`
   (`b .` assert, compare mismatch inside a lock/mutex helper).
2. Calling `FUN_080345B4(0)` from the ON loop (heap already partly used by the
   charging UI) **faults inside the constructor**: `FUN_0802FCB6` reads
   `[NULL+0x28]` — a sub-window handle is NULL, most likely because
   `FUN_08010ACC` returns NULL once the tiny heap (`0x20042760..0x200477C0`,
   ~20 KB) is fragmented/full. => UI init must run on a nearly fresh heap.
3. To get a fresh heap the power-management call `FUN_08010C8C` can be skipped
   at `0x08033E9C` (`--break-at 0x08033E9C --jump 0x08033EA0`), but that call is
   also where TMR7 is initialised (writes ARR/PSC/EGR/DIER/CR1 at
   vt 241806..241886). Re-initialising TMR7 through the stub works:
   ```
   --calls "0x080398BA:0x40001400:99:47,
            0x080398F0:0x40001400:0,
            0x08039A74:0x40001400:1:1,
            0x080398FA:0x40001400:1,
            0x08035C78:55:0:0"
   ```
   (helpers: `FUN_080398BA(arr,psc)`, `FUN_080398F0(CR1 dir bits)`,
   `FUN_08039A74(DIER OR, r2=1)`, `FUN_080398FA(CR1 EN)`,
   `FUN_08035C78(irq=55,prio,sub)`).
   With that, tick advances (426 at 25M insns) and main runs to its UI loop, but
   it then gets stuck in a **huge `memset` (`FUN_0802D8D6`, PC 0x0802D91E)** —
   likely a buffer clear with a garbage length (another uninitialised global),
   so the screen stays mostly at the boot page (`156893` LCD pixels, dark).
4. Current best "new page" evidence: `out/ui_page1_bottom_bar.png`
   (page cleared, bottom tab bar drawn) from the earlier timer-registration
   experiment, and `out/ui_main_init_white.png` / `ui_main_init_bottom_bar.png`
   from the main cold-boot replay.

### Status vs. the goal

The visual home/quick-panel comparison with the patched image is **not yet
possible**: the UI constructor starts and creates the root window, but cannot
finish because of (a) heap exhaustion when called late and (b) uninitialised
globals producing a huge memset when called early; both are "UI object/state
not set up" issues downstream of the emulated power/charge state, not tick
issues. The TMR7 model itself is solid (stable 1 ms tick, all interrupts and
ISR return paths validated over 40M+ instruction runs).
