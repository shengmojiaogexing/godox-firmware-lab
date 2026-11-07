# X3_C v1.38 firmware — shared context (Lead notes, 2026-11-06)

Firmware: `devices/x3c/firmware/X3_C_v1.38_app.bin`
size 1,021,468 bytes (0x0F961C), sha256 45fdd9522c432f4be60a470741d3e5c12a7ee629eb2b277aed5187639f85ef62
Target product: Godox **X3-C** TTL wireless flash trigger (Canon hot shoe), firmware v1.38.

Tooling already installed (use `tools/python/python.exe`, Python 3.12 embedded):
- capstone 5.0.9, unicorn 2.1.4, pyelftools 0.33
- Node.js 24 available; network access works (PyPI/GitHub reachable).
- No Java/YET Ghidra installed as of this note.

## Confirmed facts

1. File starts with a plausible Cortex-M vector table at offset 0:
   - SP=0x200477C0, Reset=0x08004AF9 (thumb), NMI=0x08010AC9, HardFault=0x0800C14D,
     MemManage=0x0800F4B9, BusFault=0x080062E1, UsageFault=0x08017181,
     SVC=0x08013B7D, DebugMon=0x08006E59, PendSV=0x08010BFD, SysTick=0x08016669.
   - Gaps 7..10 = 0, 13 = 0. IRQ table keeps going to entry 130 (offset 0x208),
     i.e. 16+115 entries -> 131 vectors, then code begins at file offset 0x20C
     (`bl +4; bl ...; adr r0; ldm r0,{sl,fp}; ...` = ARMCC __scatterload pattern).
   - IRQ entries: ~70 are non-zero/valid; most share value 0x08004B2D (default handler),
     distinct ones at IRQ3(0x080082EB), 6(0x080083AD), 8(0x08008459), 18(0x0800510D),
     23(0x080084AD), 35(0x08012985), 40(0x080083ED), 41(0x080082C5), 50(0x08016C01),
     54(0x08016C5D), 55(0x08016D51), 56(0x08006E25), 59(0x08006E3D), 67(0x08010B19).
   - 19 IRQ slots are 0.
2. Peripheral literals found in the image (aligned 32-bit words):
   GPIOC 0x40020800 (35x), GPIOB 0x40020400 (28), GPIOA 0x40020000 (27), GPIOE 0x40021000 (19),
   SPI1 0x40013000 (17), RTC 0x40002800 (14), IWDG 0x40003000 (13), PWR 0x40007000 (11),
   TIM6 0x800 (8), TIM8 7, ADC1 (6), TIM7 5, GPIOD 4, TIM5 4, FMC 0xA0000000 4, RCC 0x40023800 3,
   CRC 3, TIM1 3, USB 0x50000000 3, EXTI 2, TIM3 2, TIM2 2, DMA2 1, SPI2/3 1 each, etc.
   -> STM32F4/F7-class memory map (AHB1 at 0x40020000, RCC 0x40023800, FLASH 0x40023C00).
   USB OTG FS at 0x50000000 -> likely USB HID IAP ("Godox_IAP", "HID IAP Interface" strings).
3. Code is definitely Thumb-2 (32-bit encodings, cortex-m idioms). Confirmed CMSIS-style code at
   0x08031456 (NVIC ICER: 0xE000E000+0x180) and 0x08031478 (NVIC IPR: 0xE000E000+0x400/0xD14).
   0x08011E6C looks like SystemInit/clock init (RCC 0x40023800, CFGR=0x33002, VTOR 0xE000ED08).
4. **Unsolved structural puzzle**: many vector entries decode to instructions that do not look like
   function prologues (e.g. 0x08004B2C `movs r0,#0x64; bl ...`), same for image2 table entries.
   Need Ghidra/flow analysis to decide whether entries are mid-function, whether a second
   image/relocation is involved, or whether the table uses some container transformation.
5. File appears to be a **container with 2 sub-images**:
   - image1: file 0x000000 .. ~0x0F3800 (fonts/UI assets in 0x0A5700..0x0E8800, 45KB zero fill
     0x0E8800..0x0F3800)
   - image2: file 0x0F3800 .. 0x0F961C (24,092 bytes). At 0x0F3800 there is ANOTHER valid-looking
     Cortex-M vector table: SP=0x200005E0, Reset=0x080020CD, NMI=0x080037DD, HardFault=0x080030BD,
     SVC=0x080042D1, PendSV=0x080038A9, SysTick=0x08006E11, 16+30 vectors, code starts at
     0x0F38B8. 30 IRQs suggests an STM32F0-class MCU (hot-shoe adapter? separate MCU?).
     Strings "Hot Shoe Upgrading" (file 0x1F9C0) suggest the main firmware upgrades the hot-shoe MCU.
6. ASCII/UTF-8 UI strings are present (English + Chinese), e.g. "Model: X3 C" / "Firmware: v1.38"
   at 0x2B50..0x2BB8, plus flash-menu help texts ("GR A-E for TTL/M mode", "Wireless Sync",
   "Turn on the modeling lamp", "Min.Power", "Front Curtain", "Rear Curtain", "One-Shoot"),
   "Restore Factory", "Setting", "Press for 2s to unlock", "HID IAP Interface", "Godox_IAP".
   NO 32-bit literal was found that points at those string addresses -> strings are likely
   referenced through a string-ID/offset table, or the image base assumption is wrong.
7. Existing related project in this workspace: `Godox-S3-Trigger` — a clean-room ESP32-S3
   reimplementation of a Godox trigger (GPL-3.0), with `components/godox_protocol`,
   `godox_waveform`, `rf_transmitter`. It contains prior protocol knowledge (channel/iD/group
   packet formats, channel frequency tables). Read its docs/components for free intel.
   Also `devices/ad100pro/firmware/AD100pro_V1.2.bin` and `devices/ad200pro/firmware/AD200pro_V1.60.bin` (small Godox flash
   firmwares, ~50KB) and recon scripts `devices/x3c/re/recon.mjs`, `devices/x3c/re/scan.mjs` from earlier work.

## Work split (see team task board)
- Lead: resolve image/vector-table structure, MCU identity, memory map; integrate deliverables.
- re-ghidra: install Java+Ghidra, headless decompile, export function/string/xref DB, initial RE report.
- hw-analyst: peripheral/IRQ map, hardware guess, protocol & UI evidence, cross-ref existing project.
- emulator-dev: Unicorn-based debug simulator skeleton (Cortex-M, flash/RAM/NVIC/SysTick, trace,
  breakpoints, virtual display, input injection, RF TX capture).

Shared rules: write only inside your assigned scope; put reports in devices/x3c/re/; never modify
devices/x3c/firmware/X3_C_v1.38_app.bin; prefer small, testable Python tools in devices/x3c/tools/.
