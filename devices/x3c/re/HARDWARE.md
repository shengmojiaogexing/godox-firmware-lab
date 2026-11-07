# HARDWARE.md — Godox X3-C (X3 C) v1.38 hardware identification & map

Firmware analysed: `devices/x3c/firmware/X3_C_v1.38_app.bin` (1,021,468 bytes, sha256
45fdd952…f85ef62). All addresses below are **virtual addresses** in the app's own link
address space. **App link base = 0x08004800**; `file_offset = vaddr - 0x08004800` for
`0x08004800 ≤ vaddr < 0x080FDE1C`. The second embedded blob maps with base 0x08002000
(see §7). Peripheral addresses are absolute and unaffected by the link base.

Status: MCU, boot chain, memory map, IRQ map, display bus/panel, radio chip and USB are
identified with code evidence. Items marked **(open)** are not yet fully nailed down.

---

## 1. MCU: Artery **AT32F435 / AT32F437** (Cortex-M4F), not STM32

Evidence (each independently discriminating):

| # | Observation | STM32F4 | STM32F7 | **AT32F435/437** | Verdict |
|---|---|---|---|---|---|
| 1 | Vector table = 131 words = SP + 15 system + **115 IRQs** (IRQ0…IRQ114) | 82–91 IRQ | 110 IRQ | **115 IRQ (last `DMA2_Channel7_IRQn = 114`)** | AT32 |
| 2 | 19 zero vector slots at IRQ 70,74,75,79,80,85–90,93,95–101 | no such pattern | no | **exactly AT32's reserved IRQ numbers** | AT32 |
| 3 | IRQ35 (=SPI1 in AT32 enum, =SPI2 in STM32) handler `0x08012985` polls/reads **SPI1 @0x40013000** | mismatch | mismatch | match | AT32 |
| 4 | IRQ18 handler `0x0800510D` accesses **ADC @0x40012000**; AT32 IRQ18 = `ADC1_2_3_IRQn` | (also ADC on STM32) | — | match | consistent |
| 5 | Artery-only registers: `RCC/CRM+0xA0`, `+0xA4` (`0x400238A0/A4`), `FLASH+0x60` (`0x40023C60`), `DMA+0x200` (`0x40026600/0x40026604`) | none | none | Artery extensions | AT32 |
| 6 | IRQ3 = ERTC_WKUP, IRQ40 = EXINT15_10, IRQ41 = ERTCAlarm, IRQ54 = TMR6_DAC, IRQ55 = TMR7, IRQ56 = DMA1_Channel1, IRQ59 = DMA1_Channel4, IRQ67 = **OTGFS1** (USB) — handler 0x08010B19; "Godox_IAP"/"HID IAP Interface" descriptors | partial | partial | exact AT32 numbering | AT32 |
| 7 | Peripherals at STM32F4-compatible addresses (RCC 0x40023800, SPI1 0x40013000, GPIOA–E 0x40020000…, ADC 0x40012000, USB OTG FS 0x50000000, XMC 0x6C000000/0xA0000000, RTC 0x40002800, IWDG 0x40003000, PWR 0x40007000, CRC 0x40023000, FLASH 0x40023C00) | identical | identical | AT32 is a F4-compatible clone | consistent |
| 8 | RAM: stack top `0x200477C0`, heap base `0x20042760` → app uses 0x20000000–0x200477C0 (~286 KB); AT32F435/437 has 384 KB SRAM, 1 MB flash variant fits app region 0x08004800–0x080FDE1C | F4 max 256 KB | F7 320KB | AT32 384KB | consistent |

Cross-reference lists used: Artery AT32F435/437 CMSIS `IRQn` enum
(https://at32.wisemcu.cn/AT32F435_437/group__configuration__section__for__cmsis.html) —
contains `… DMA2_Channel7_IRQn = 114` and the same reserved gaps.

**Conclusion: AT32F435 (or 437, same die family/IRQ map), Cortex-M4F, 1 MB internal flash,
384 KB SRAM. App linked at 0x08004800, leaving 0x08000000–0x08004800 (18 KB) for the
vendor bootloader (not included in this file).**

### Boot chain (verified)

| Address | Content |
|---|---|
| file 0x000–0x20B | app vector table (131 words), SP=0x200477C0 |
| file 0x20C (`0x08004A0C`) | ARMCC `__main` / `__scatterload` (compressed RW initialiser at 0x08004A14…) |
| file 0x2D8 (`0x08004AD8`) | `__rt_entry`: calls `__user_setup_stackheap` (0x080050A6 → heap base **0x20042760**), lib init 0x08033E20, etc. |
| file 0x2F8 (`0x08004AF8`) | **Reset_Handler**: `movw/movt r0,#0x20001000; mov sp,r0` → `blx 0x08020A20` (boot/init hook) → `ldr.w sp,[0x08000000]` (restore MSP from vector table) → `blx 0x0801666C` (SystemInit) → `bx 0x08004A0C` (__main) |
| file 0x11E6C (`0x0801666C`) | **SystemInit**: RCC_CR HSION + wait; CFGR=0; CR &= 0xFEF2FFFF; PLLCFGR=**0x00033002**; RCC+0xA0=0 (Artery reg); RCC_CIR=0x009F0000; **SCB->VTOR = 0x08000000** (computed as `RCC_PLLCFGR_addr << 25`!) |

Notes: the app is linked at 0x08004800 yet its SystemInit points VTOR at 0x08000000 —
i.e. the 18 KB bootloader's vector table at 0x08000000 is the vector table used at run
time (its SP word = 0x200477C0, same as the app's). A `NVIC_SetVectorTable`-style helper
exists at 0x08035C3C..0x08035C56 (`SCB->VTOR = (x & 0x1FFFFF80) | y`) **(open: caller /
final VTOR value under the bootloader)**.

---

## 2. Flash / file layout

| File offset | vaddr | Content |
|---|---|---|
| 0x000000 | 0x08004800 | app vector table (131 words = 0x20C bytes) |
| 0x00020C–~0x042A00 | 0x08004A0C–~0x08047200 | **code** (401 functions recovered by recursive disassembly; ~2 000+ functions in reality) |
| ~0x042A20–0x0E8800 | ~0x08047220–0x080ED000 | **compressed bitmap/font/UI assets** (see §6). e.g. RLE-like stream `42 ff 08 …` from 0x080A7080 onward; image asset pointer 0x080A289C is loaded at 0x08006188 and decoded by 0x0802C400 |
| 0x0E8800–0x0F3800 | 0x080ED000–0x080F8000 | 45 KB zero fill |
| 0x0F3800–0x0F961C | — | **second firmware image** for the hot-shoe MCU (own address space, base 0x08002000; §7) |

RAM (app view): 0x20000000..0x200477C0 (~286 KB): RW/ZI up to ~0x20042760, heap from
0x20042760, MSP starts 0x200477C0. MCU SRAM = 384 KB (0x20000000..0x20060000).

---

## 3. Vector/IRQ map and handler classification

Distinct handlers (from IRQ entries; default `0x08004B2D` = `b .` infinite loop at file
0x32C, used by 82 IRQ slots; 19 slots are 0). `IRQn` = AT32F435/437 enum name.

| IRQn | AT32 name | handler vaddr | file | classification evidence |
|---:|---|---|---|---|
| 3 | ERTC_WKUP | 0x080082EB | 0x3AEB | tiny handler (push {r4,lr}, `bl 0x0802052C/0x08020510` event notifiers) — RTC wakeup → wake/UI event |
| 6 | EXINT0 | 0x080083AD | 0x3BAD | `bl 0x08020984` EXINT dispatcher; EXTI base 0x40013C00, PR 0x40013C14 (seen in 0x08020978/84) |
| 8 | EXINT2 | 0x08008459 | 0x3C59 | EXINT dispatcher; pin/line clear path |
| 18 | ADC1_2_3 | 0x0800510D | 0x90D | **reads ADC1 0x40012000**, `0x08018C16/0x08018C10/0x08018DDA` ADC helpers, stores results to RAM 0x20000164/0x20000168 → **battery/temperature ADC** |
| 23 | EXINT9_5 | 0x080084AD | 0x3CAD | EXINT line[9:5] dispatcher (multiple GPIO sources) |
| 35 | SPI1 | 0x08012985 | 0xE185 | **SPI1 0x40013000**: reads SR/DR via 0x08038216/0x080381DA/0x080381D0, stores byte to RAM 0x20000011 → hot-shoe link RX |
| 40 | EXINT15_10 | 0x080083ED | 0x3BED | EXINT line[15:10] dispatcher |
| 41 | ERTCAlarm | 0x080082C5 | 0x3AC5 | RTC alarm event notifier |
| 50 | TMR5 | 0x08016C01 | 0x12401 | TMR5 0x40000C00 usage in cluster 0x08016Cxx |
| 54 | TMR6_DAC | 0x08016C5D | 0x1245D | TMR6 0x40001000 (DAC not present/used) — periodic tick |
| 55 | TMR7 | 0x08016D51 | 0x12551 | TMR7 0x40001400 |
| 56 | DMA1_Channel1 | 0x08006E25 | 0x2625 | DMA1 (0x40026000) transfer-complete path |
| 59 | DMA1_Channel4 | 0x08006E3D | 0x263D | DMA1 transfer-complete path |
| 67 | OTGFS1 | 0x08010B19 | 0xC319 | **USB OTG FS 0x50000000**; "Godox_IAP" / "HID IAP Interface" USB descriptors at file 0x1D810/0x1D848 → USB HID IAP (firmware update / PC link) |
| — | 82× default | 0x08004B2D | 0x32C | `fe e7` infinite loop |

System exceptions: Reset 0x08004AF8 (file 0x2F8); NMI 0x08010AC8 / DebugMon 0x08006E58 /
PendSV 0x08010BFC / SysTick 0x08016668 are small stubs (`bx lr` / `movs r0,#0; bx lr`);
HardFault 0x0800C14C, MemManage 0x0800F4B8, BusFault 0x080062E0, UsageFault 0x08017180 are
`b .` traps at file 0x794C/0xACB8/0x1AE0/0x12980; SVC 0x08013B7C (file 0xF37C).

Peripheral bases genuinely referenced by code (literal-pool scan with verified `ldr [pc]`
xrefs): GPIOA/B/C/D/E, CRM/RCC (incl. +0xA0/0xA4), FLASH, ERTC, IWDG, PWR, EXINT, SPI1,
SPI2/SPI3 (only in generic SPI init 0x08038238), ADC1, CRC, DMA1/DMA2, USART1, TMR1/3/5/6/7/8/9/10/11,
SCB/SCS, USB OTG FS, XMC (0x6C000000). Unreferenced in reachable code: I2C1-3, SAI, SDIO, DVP.

---

## 4. Display (AMOLED, XMC 8080-style bus)

* **Panel resolution: 448 × 368** (`0x1C0 × 0x170`) — constants at `0x08006138`
  (`mov.w r0,#0x1c0; mov.w r1,#0x170`) passed to the screen/widget factory 0x08010ACC and
  used again at 0x080148B6. The register order is `strd r1, r0, [sp]` → args (w=368, h=448)
  in that call, i.e. **orientation 368 wide × 448 high (portrait)**, while the controller's
  native panel is treated as 448×368 (landscape) — the runtime orientation is decided by the
  `0x36` MADCTL value (not sent in the init function 0x08035A64) and by the `2A/2B` window
  extents; emulator should verify at runtime which extent is used for full-screen frames.
* **Colour format: RGB565** (16 bpp) — panel init writes `0x3A 0x55` (= COLMOD 16-bit).
* **Bus: XMC bank NE4 @ 0x6C000000**, 8-bit parallel:
  * command port **0x6C000000**, data port **0x6C000001** (A0 = XMC address bit 0);
  * raw writers `0x08025E94` (`strb r0,[0x6C000000]`) and `0x08025E9C`
    (`strb r0,[0x6C000001]`);
  * wrapped writers `0x08035BC4` / `0x08035BE8` additionally drive **PC4**
    (`GPIOC bit4`, via `0x40020814`) and the command wrapper first writes the literal
    `0xF9` to 0x6C000000 (panel command-mode preamble), then `delay(1)` then the command.
* **Panel init function: 0x08035A64** (file 0x31264). Command/data pairs (each cmd via
  0x8035BC4, each data byte via 0x8035BE8, `delay(2)` between pairs):
  `FE 20, F4 5A, F5 59, FE 90, AA 00, 4E 1F, FE 00, 3A 55, 35 00, 53 20, 51 FF, 63 FF, 11`
  then `delay(0x14)`. Vendor page-select style `FE` + power commands `F4/F5` and `3A/35/53/51/63`
  → consistent with an **NV3007-class AMOLED controller** (single-chip 448×368 RGB565 AMOLED)
  rather than ST7789/GC9A01. **(open: exact controller part no.)**
* **Panel reset / power-on sequence: 0x08025A04** (file 0x21204): calls board init
  0x0803BABC, resets the panel via a GPIOB output (`0x40020400`, mask `0x400` = PB10) with
  1/10/120 ms delays, calls 0x08035A64, then reads RAM 0x20000170 and computes a battery
  percentage (`udiv by 0xFF`, `+0x37`) for the UI.
* **Window / rectangle:** `2A` + `x0_hi,x0_lo,x1_hi,x1_lo`, `2B` + `y0…,y1…`, then `2C`
  (RAMWR). Sites: raw path 0x0800CE5E–0x0800CEBC; wrapped path 0x080060BE–0x0800612A.
* **Pixel push:** 16-bit pixels sent high byte then low byte to 0x6C000001, e.g. loop at
  0x0801C2E0: `ldrh r0,[r4]; lsrs r0,#8; bl 0x08025E9C; ldrb r0,[r4]; bl 0x08025E9C`.
* **Framebuffer:** no single fixed 448×368 framebuffer was found in RAM. Rendering is
  strip/blit based (source RAM pointer in r4, pixel count in r6, function 0x0801C2C0) plus
  direct draw-to-panel writes; UI bitmaps are **compressed assets in flash** (function
  0x08006188 loads flash asset 0x080A289C and decodes via 0x0802C400). **For the simulator:
  reconstruct VRAM by emulating the LCD port (0x6C000000/1 + 2A/2B/2C window state).**
* **XMC configuration** (bus width, timing, NE4 BCR/BTR): not yet located **(open)** —
  search for writes to 0xA0000018/0xA000001C (BCR4/BTR4) or Artery XMC bank registers.
  PC4 (GPIOC bit4) is a second control line used by the command wrapper.

---

## 5. 2.4 GHz radio: **TI CC2500-class transceiver**, bit-banged SPI

Not a hardware-SPI device: the radio hangs on **bit-banged GPIO** (confirmed from code).

| signal | GPIO | evidence |
|---|---|---|
| SCLK | **GPIOC bit0** (`0x40020800`, mask 0x1) | toggled per bit in byte writer 0x0802377C |
| MOSI | **GPIOC bit1** (mask 0x2) | set/cleared per bit in 0x0802377C |
| CSn | **GPIOC bit14** (mask 0x4000) | 0x08023A7C asserts (low 200×1 ms wait loop), 0x08023AB0 releases |
| MISO/misc | **GPIOC bit15** (mask 0x8000) | read in 0x0802354C (`ldb` via 0x08022EB4) |

Driver functions (`va`):
* `0x0802377C` — bit-bang one byte, **MSB first** (SCLK pulse + MOSI), 9×`nop` per bit.
* `0x08023A52(reg, ptr, len)` — CS assert, write `reg`, write `len` payload bytes, CS release.
  (the only call site: 0x08023444 with reg 0x7E, len 11 → likely a FIFO burst write)
* `0x08023520(cmd)` — CS assert, single command byte, CS release (strobes).
* `0x08023540(reg, val)` — CS assert, reg byte + value byte, CS release.
* `0x0802354C()` — bit-bang read one byte (MISO on PC15).
* `0x080232B6(ms)` — delay (SysTick/us loop calibrated).

Register-map evidence (CC2500 register numbers): writes `0x0D/0x0E/0x0F` (FREQ2/1/0),
`0x04/0x05` (SYNC1/SYNC0), `0x06` PKTLEN, `0x08` PKTCTRL0, `0x12` MDMCFG2, `0x17` MCSM1,
`0x00/0x02` IOCFG2/IOCFG0, PATABLE via `0x3E`, and strobes `0x33` SCAL, `0x35` STX,
`0x36` SIDLE, `0x39` SPWD, `0x3B` SFTX, `0x3D` SNOP. See PROTOCOL.md for values.

Because RF chip is on bit-banged GPIO and not on SPI1, **SPI1 is the hot-shoe MCU link**
(CS = GPIOC bit10 / mask 0x400; command/response protocol in function 0x08013688).

---

## 6. Other hardware blocks

* **Battery ADC**: ADC1 (`0x40012000`) sampled in IRQ18 handler 0x0800510D; results stored
  at RAM 0x20000164/0x20000168; battery % computed in 0x08025A04 (`udiv 0xFF`, +0x37 offset)
  and by 0x0803C344/0x0803C4A4 helpers. **(open: divider values / channels)**
* **USB OTG FS (0x50000000)**: device-mode USB stack; isochronous/interrupt endpoints; HID
  IAP descriptors at file 0x1D7E0–0x1D8C0 (strings "Godox", "HID IAP Interface",
  "Godox_IAP"; descriptor code at 0x0801D7E0…0x0801D8D0 builds them); IRQ67 = OTGFS1
  handler 0x08010B19.
* **RTC/ERTC (0x40002800, +0x0C = RTC_WUTR seen)**: IRQ3 ERTC_WKUP + IRQ41 ERTCAlarm used for
  timers/alarms/wakeup.
* **IWDG (0x40003000)**: preset/ reload helpers 0x0803BA5C (PR bfi bits0-2),
  0x0803BA6C (`KR=0xCCCC` start), 0x0803BA7C (`KR=0x5555` unlock/`0`), 0x0803BA94 (`RLR`).
* **Timers**: TMR5 0x40000C00, TMR6 0x40001000, TMR7 0x40001400 (IRQ50/54/55; tick/timebase);
  TMR1/3/8/9/10/11 referenced in low-level init code 0x0803C4A4.
* **EXINT (0x40013C00, PR 0x40013C14)**: lines 0/2 (IRQ6/8), 5–9 (IRQ23), 10–15 (IRQ40) —
  GPIO IRQ sources: radio GDO, touch, hot-shoe, keys **(open: pin→line mapping)**.
* **DMA1/DMA2**: DMA1 channels used from 0x08006E25/0x08006E3D; DMA2/`0x40026600` helpers at
  0x0801C31C–0x0801C35C (write/read DMA control); used by SPI1 and display/XMC paths.
* **Clock**: SystemInit writes PLLCFGR=0x00033002 (PLLM=2, PLLN=192, PLLP=8, PLLSRC=HSI) and
  RCC+0xA0=0 — exact PLL enable/target clock and HSE use are performed later **(open)**.
* **Keys/touch**: touch panel is present (product spec: touch AMOLED). No I2C and no SPI2/3
  usage found → touch is most likely read via GPIO/EXINT (IRQ23/40 line groups) or the
  hot-shoe MCU **(open)**.

---

## 7. Second embedded image — hot-shoe adapter MCU (file 0x0F3800)

* 24,092 bytes at file 0x0F3800..0x0F961C; its own Cortex-M vector table at file 0xF3800:
  SP=0x200005E0, Reset=0x080020CD (own link base **0x08002000**, code starts file 0xF38B8).
* 16 + 30 vectors → 30 IRQs. Handler set: IRQ2 = 0x08003D1D (push {r4,lr}), IRQ6 = 0x08002879,
  IRQ7 = 0x080029FD, IRQ19 = 0x08006F01, IRQ25 = 0x08003EA1; all other slots point to
  `b .` at 0x080020DF. 30 IRQs + small RAM (SP 0x200005E0) → **STM32F0-class** hot-shoe
  MCU (e.g. STM32F030/F042-class; exact part **open**).
* This blob has **no ASCII/UTF-8 UI strings** (only code) — it is a resource payload that
  the main app flashes into the hot-shoe MCU ("Hot Shoe Upgrading" string, file 0x1F9C0 =
  vaddr 0x080241C0). The transport to that MCU is the **SPI1 link** (CS = GPIOC bit10;
  command/response protocol, function 0x08013688: cmd codes 0xA0, 0xC0–0xC9, response =
  cmd+1, checksum = 0xA1 + Σpayload; frame copied to RAM 0x20000C2A).
* Main app has "Firmware error"/"Update" UI and boot counters (RAM 0x2000006A etc.) — the
  image is only sent in upgrade/maintenance flows.

---

## 8. Device-level summary

| Item | Value | Confidence |
|---|---|---|
| MCU | Artery **AT32F435/437** Cortex-M4F | **high** |
| Flash / SRAM | 1 MB / 384 KB (app uses 0x20000000–0x200477C0) | high |
| App link base / bootloader | 0x08004800 / 0x08000000–0x08004800 (bootloader not in file, VTOR=0x08000000) | high |
| Display | 448×368 AMOLED, RGB565, XMC NE4 8-bit 8080 (0x6C000000/1, PC4, 0xF9 cmd preamble) | high (res/fmt/bus) / NV3007-class controller medium |
| Radio | TI **CC2500-class** 2.4 GHz GFSK transceiver, bit-banged SPI on GPIOC0/1/14/15, ~26 MHz xtal | high |
| Hot-shoe | separate STM32F0-class MCU, SPI1 link (GPIO C10 CS), upgrade payload in app | high |
| USB | OTG FS device, HID IAP ("Godox_IAP"/"HID IAP Interface") | high |
| Battery | Li-ion 3.7 V 850 mAh (product spec), ADC1 + IRQ18 handler | medium |

### Open items for follow-up
1. XMC bank NE4 BCR/BTR config values; PC4 exact role; confirm 8080 8-bit vs 16-bit bus.
2. AMOLED controller exact part (NV3007 vs NV3007A/other); display-on (0x29) + brightness code.
3. Touch controller & keys: bus, IC, EXINT mapping.
4. PLL/clock tree final values and HSE (CC2500 needs 26 MHz; MCU crystal?).
5. Bootloader presence/behaviour at 0x08000000 (not in file) and VTOR handover.
6. Battery divider resistors / ADC channel.
