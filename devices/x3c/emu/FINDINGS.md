# devices/x3c/emu — X3-C firmware debug simulator (findings log, append-only)

Append-only notes made while building/running the Unicorn emulator for
`devices/x3c/firmware/X3_C_v1.38_app.bin` (Godox X3-C trigger, v1.38).
Maintained by emulator-dev (task-3).

---

## 2026-11-06 — image layout recon + entry point resolved

### Load address: app is linked at 0x08004800 (file_offset = vaddr - 0x08004800)
This explains the "vector entries look like mid-function code" puzzle: with the
wrong base (0x08000000) every vector target decoded mid-function.

Self-consistency proofs (all with base 0x08004800):
* vector table starts at file 0 (vaddr 0x08004800); SP=0x200477C0,
  Reset entry 0x08004AF9 -> reset handler at 0x08004AF8 = **file 0x2F8**.
* reset handler (file 0x2F8) disassembles exactly as a CMSIS/ARMCC boot stub:
  `movw r0,#0x1000; movt r0,#0x2000; mov sp,r0; ldr r0,[pc]` (=0x08020A21)
  `blx r0`; `movw r0,#0; movt r0,#0x0800; ldr.w sp,[r0]` (SP from flash word 0);
  `ldr r0,[pc]` (=0x0801666D) `blx r0` = SystemInit; `ldr r0,[pc]` (=0x08004A0D)
  `bx r0` = __main.
* file 0x11E6C = SystemInit (matches earlier lead note "0x08011E6C looks like
  SystemInit"); file 0x20C = ARMCC `__main`/scatterload (bl, bl, adr r0,
  ldm sl/fp, region-table loop `bx r3`).
* default IRQ handler 0x08004B2D = file 0x32C = `b .` (infinite loop).
* image size 0xF961C -> ends at vaddr 0x080FDE1C; file 0x00F3800 (2nd blob,
  hot-shoe MCU firmware) therefore lives at vaddr 0x080F8000 in the main flash.

Fake bootloader region: real flash 0x08000000..0x08004800 is NOT in the .bin
(the bootloader lives there). The app's reset stub reads the initial SP from
word 0 of 0x08000000, so the emulator maps that gap and puts
word0=app SP / word1=app reset handler there (configurable; see
`machine.Machine.fake_bootloader`).

### One-time option-byte provisioning at boot (0x1FFFC000)
Reset stub calls **0x08020A20** before SystemInit. That function:
* reads `*(u16*)0x1FFFC010 & 7` and `*(u16*)0x1FFFC002 & 0x60`
* if `(opt10&7)==2 && !(opt02&0x60)`: returns immediately (normal boot)
* else: calls 0x080218E8 / 0x08021908 (FLASH unlock + option programming:
  KEYR=0x40023C04 keys 0x45670123/0xCDEF89AB, OPTCR 0x40023C14 bit4 OPTSTRT,
  program opt02=0x000F, opt10=0x0002 via 0x80218A4/0x8021724), then
  **SYSRESETREQ** (0x08035D24: AIRCR = 0x05FA0004) -> reboot.

=> A real device has already been through this first boot, so the emulator
presets the option-byte page with opt02=0x000F, opt10=0x0002 (see
`Machine._load`). Without that preset the emulator loops in SYSRESETREQ.

### Peripherals identified while walking the boot
* RCC @ 0x40023800 (CR/CFGR/PLLCFGR/BDCR/CSR) — ready bits auto-derived
  (HSION->HSIRDY, HSEON->HSERDY, PLLON->PLLRDY, PLLI2S/PLLSAI, LSEON->LSERDY,
  SW->SWS mirror). Clock init (SystemInit @ file 0x11E6C) completes.
* FLASH interface @ 0x40023C00 (ACR/SR/CR/OPTCR/OPTKEYR): never busy, no errors.
* ADC @ 0x40012000: the driver is a **merged F4/F3-style** ADC: it polls
  ISR/SR bit6 (ADRDY, bit=0x40) after ADEN and polls the self-clearing
  ADSTART/JADSTART bits (CR bits 2,3); CR1/IER @0x04, CR/CR2 @0x08, SQR1 @0x2C,
  ADC_Common @0x40012300. Emulator returns ISR=0x47 (ADRDY/EOSMP/EOC/JEOC) and
  self-clears the start bits. ADC init then completes (first real blocker).
* GPIO A..E init via a generic driver (0x08022E50: MODER/OTYPER/OSPEEDR/PUPDR
  read-modify-write, IDR polled at 0x08022EB8).
* SPI1 @ 0x40013000 touched during init; DMA2 @ 0x40026400 written
  (stream config 0x40026404).
* EXTI @ 0x40013C00 written (27 accesses), GPIOA IDR polled.

### Boot progress
reset -> option-byte check -> SystemInit -> __main/scatterload -> main init ->
ADC/GPIO/clock init -> long **software delay** loop at
0x0801C0FE/0x0801C160 (nop-sled delay_ms with calibration flag; outer loop in
0x0801C0FE calls 0x0801C118(1000) N times). Not a hang: it needs ~1e8+
instructions of budget. Emulator has fast burst mode (no per-instruction hook)
for these.

## TODO / next
* find end of boot delay, then next missing peripheral (likely LCD/FMC, RTC,
  IWDG, USB, interrupts/SysTick).
* identify framebuffer in SRAM once UI init runs; dump first PNG.
* add symbols from devices/x3c/re/ghidra_exports when re-ghidra produces them.
