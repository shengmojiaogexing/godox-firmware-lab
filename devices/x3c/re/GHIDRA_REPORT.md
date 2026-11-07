# GHIDRA_REPORT.md — X3_C v1.38 firmware: Ghidra decompilation + structural analysis

Append-only log. Owner: re-ghidra. Firmware: `devices/x3c/firmware/X3_C_v1.38_app.bin`
(1,021,468 B = 0x0F961C, sha256 45fdd952...ef62). Tool: Ghidra 12.2 PUBLIC + Temurin JDK 25
(`D:\ghidra_12.2_PUBLIC`, `D:\jdk25`; JDK 21 kept at `tools\jdk\jdk-21.0.13+9`).

Native addr mapping: **vaddr = 0x08004800 + file_offset** (main app image).
Hot-shoe sub-image: file 0x0F3800..0x0F961C (24,092 B), **vaddr = 0x08002000 + (file_off - 0x0F3800)**.

---

## 1. STRUCTURAL PUZZLE — SOLVED (2026-11-07 00:25)

### 1.1 The image is linked at 0x08004800, not 0x08000000

Hard proof (independent of the "b ." handler argument):

* `main()` was located at **0x08033E20** (see §2/§4). Its first statements are
  ```
  08033e20  movs  r4,#0
  08033e2a  mov.w r1,#0x4800
  08033e2e  mov.w r0,#0x08000000
  08033e32  bl    0x08035D38
  ```
  `0x08035D38` is a "set-VTOR" helper: `ldr r2,[pc,#8]; ands r1,r2; orrs r1,r0;
  ldr r0,[pc,#8]=0xE000ED0C; str r1,[r0]; bx lr`
  -> writes **SCB->VTOR = 0x08000000 | 0x4800 = 0x08004800**. The firmware is linked for
  load address 0x08004800 (bootloader occupies 0x08000000..0x08004800 = 18 KiB).
* Counter-test: with base 0x08000000, vector entry 1 (0x08004AF9) maps to file 0x4AF8 which is
  inside a function (`mov r0,r7; bl ...`), i.e. not a reset handler. With base 0x08004800 it maps
  to file 0x2F8 which *is* the reset handler (§1.2).
* Ghidra confirms it structurally: all 106 non-zero flash vector targets are Ghidra function
  starts, and the 86 shared defaults (`fe e7`) land exactly on their `b .` loops (§4.1).
* Peripheral literals are absolute and unaffected (GPIOC 0x40020800 etc.).

### 1.2 Startup chain (all addresses = real image addresses)

```
file 0x000000 (vaddr 0x08004800):  Cortex-M vector table, 131 entries (16 + 115 IRQ)
  [0] SP = 0x200477C0
  [1] Reset_Handler  = 0x08004AF9 (thumb) -> code 0x08004AF8 (file 0x2F8)
  [3] HardFault      = 0x0800C14D -> file 0x794C = `fe e7`  (b .)  default handler
  IRQ default        = 0x08004B2D -> file 0x32C = `fe e7`  (b .)  shared default handler
  vector table ends 0x08004A0C (131*4 = 0x20C), code follows immediately
```

* **Reset_Handler 0x08004AF8** (file 0x2F8):
  `movw r0,#0x1000; movt r0,#0x2000; mov sp,r0` (temp stack 0x20001000)
  `ldr r0,=0x08020A21; blx r0` -> early-boot check **0x08020A20** (reads 0x1FFFC010/0x1FFFC002
   system-memory flags; on failure tail-calls 0x08035D14 = **NVIC_SystemReset**: writes
   AIRCR 0x05FA0004 via 0xE000ED0C).
  `movw/movt r0,#0x08000000; ldr.w sp,[r0]` -> reload SP from bootloader vector table at 0x08000000
  (bootloader's SP; app vector[0] = same value 0x200477C0).
  `ldr r0,=0x0801666D; blx r0` -> **SystemInit 0x0801666C** (file 0x11E6C; RCC/FLASH/PLL setup:
  RW addr 0x40023800 area, wait-state reg 0x40023C00, `mov.w r0,#0x9f0000`).
  `ldr r0,=0x08004A0D; bx r0` -> **__main 0x08004A0C** (file 0x20C).
* **__main 0x08004A0C**: `bl __scatterload (0x08004A14)` then `bl __rt_entry (0x08004AD8)`.
* **__scatterload 0x08004A14**: ARMCC scatter-load / region-init loop (`adr r0,#0x28;
  ldm r0,{sl,fp}; add sl,r0; ... bx r3`).
* **__rt_entry 0x08004AD8**: `bl 0x08050A6` (library init), `bl 0x08033E20` = **main()**,
  then `bl 0x0805098` (semihost bkpt 0xAB / exit).
* **main() 0x08033E20**: sets VTOR=0x08004800, then `bl 0x080393C4, 0x080354E4, 0x0800CFA4,
  0x08022E34 (GPIOB 0x40020400, GPIOC 0x40020800 config), 0x08035280, 0x08035A40, 0x08023B00,
  0x080112E8`, reads a config byte at **0x080047F0** (last byte of bootloader area!) and mirrors
  it into RAM 0x20000060..0x200000B2. 57 callees total (Ghidra).

### 1.3 Vector-table targets are real function starts

Recursive traversal from all 106 non-zero flash vector targets (plus BL targets and literal-pool
function pointers) reaches all of them as instruction starts; every vector target except the
shared defaults is reached by direct BL from other code. Verdict: **vector entries are genuine
function entries (thumb bit set), NOT mid-function or container-transformed data.** The earlier
"mid-function" impression came from assuming base 0x08000000 (all targets looked offset by
+0x4800). Ghidra's independent function detection confirms: **106/106 function starts, 0
mid-function, 86 `b .` defaults** (§4.1).

### 1.4 Second image (hot-shoe MCU) — real and separate

File 0x0F3800..EOF = 24,092-byte standalone Keil/ARMCC Cortex-M image:
vector table at 0x0F3800 (SP=0x200005E0, Reset=0x080020CD, NMI=0x080037DD, HardFault=0x080030BD,
SVC=0x080042D1, PendSV=0x080038A9, SysTick=0x08006E11; 16+30 = 46 vectors).
Reset entry 0x080020CC -> file 0x0F38CC = `ldr r0,=SystemInit; blx r0; ldr r0,=__main; bx r0`
(classic Keil startup). Extracted to `tools\hotshoe_blob.bin`, analyzed as separate Ghidra
program with base 0x08002000: **162 functions, 33/33 vector targets are function starts** (§4.2).
This is a resource inside the main firmware (upgraded via "Hot Shoe Upgrading" UI), not part of
the main image's address space.

---

## 2. UI string referencing (answers CONTEXT.md fact 6)

**Strings are NOT referenced by absolute 32-bit pointers.** A full-image scan finds only 6
coincidental word->string matches (in bitmap data). The code uses **Thumb PC-relative ADR**
(`add rXd, pc, #imm8*4`). 93 such ADR->string references were found in the capstone pass, e.g.:

| code addr (ADR) | string vaddr | text |
|---|---|---|
| 0x080055FC | 0x0800570C | "GR A-E for TTL/M mode" |
| 0x08007018 | 0x08007350 | "Model: " |
| 0x0800714E | 0x0800739C | "Firmware: " |
| 0x080071D0 | 0x080073B0 | "v1.38" |
| 0x08008180 | 0x080082A8 | "Setting" |
| 0x08008644 | 0x08008988 | "Restore Factory" |
| 0x08005A76 | 0x08005D38 | "Auto Off" |

Ghidra's ARM analyzer resolves these to proper references: `strings.csv` now has **109 strings
with xrefs** (e.g. "Model: " <- 0x0800701A in FUN_08006F0C; "GR A-E for TTL/M mode" <-
0x080055FE in FUN_08005264). Consequence: naive literal-pool scans cannot find UI-text xrefs.

The UI code cluster is 0x08005500-0x08008FFF plus later menu/drawing handlers; strings are
interleaved with code (rodata merged into .text) and Chinese strings are UTF-8 (not UTF-16/GBK).

---

## 3. Ghidra pipeline (installed tooling / reproduce)

* JDK 25 (Ghidra 12.2 requirement): `D:\jdk25`; JDK 21 kept at `tools\jdk\jdk-21.0.13+9`.
* Ghidra 12.2 PUBLIC: `D:\ghidra_12.2_PUBLIC` (sha256-verified 569,712,708 B zip).
  **Moved out of the CJK path on purpose**: Ghidra builds `jar:file:/...` URLs unescaped and
  fails on 神牛; cmd .bat launchers also mis-handle CJK paths. Driver/docs stay in devices/x3c/tools.
* Import: raw binary, `ARM:LE:32:Cortex`, base 0x08004800 (main) / 0x08002000 (hot-shoe),
  `-loader BinaryLoader -loader-baseAddr ...`.
* Scripts: `devices/x3c/tools/ghidra_scripts/{PreSetup,ExportAll,VectorCheck,DumpAddr}.java`
  (copied to `D:\ghidra_scripts` by the driver). Driver `devices/x3c/tools/ghidra_run.ps1`;
  ad-hoc decompiler `devices/x3c/tools/decompile.py <addr>...`; doc `tools/GHIDRA_USAGE.md` (+ copy in
  `devices/x3c/tools/`).
* Fallback (capstone, no Ghidra): `devices/x3c/tools/ghidra_fallback.py` — used while downloading and
  for cross-check (1199 functions / 30,927 insns vs Ghidra's 1431 / 86,575).

---

## 4. Ghidra results — function inventory, main/RTOS/UI/LCD/RF (2026-11-07 01:05)

Run: Ghidra 12.2 PUBLIC + Temurin JDK 25; raw binary `ARM:LE:32:Cortex`; base 0x08004800;
`PreSetup.java` (vector pointers + entry functions) -> full auto-analysis (~28 s) ->
`ExportAll.java` (~2.5 min) -> `VectorCheck.java` (~15 s).

### 4.1 MAIN APP (`X3_C_v1.38_app.bin`, base 0x08004800)

| metric | value |
|---|---|
| Ghidra functions | **1431** |
| decompiled OK | **1418** (13 failed: 0x080FB258, 0x080F8138 etc. — data/switch-jump-table regions, see `decomp_failed.csv`) |
| instructions (Ghidra) | **86,575** |
| strings | 1036 (109 with xrefs — ADR refs resolved by Ghidra) |
| memory blocks | 1 (0x08004800-0x080FDE1B) |
| files | `devices/x3c/re/ghidra_exports/main/{functions.csv,decompiled/,strings.csv,datarefs.csv,callgraph.json,memory_map.csv,vector_table.csv,gaps.csv,overview.json}` |

**Vector-table validation (`vector_table.csv` via VectorCheck.java, after Ghidra analysis):**

```
131 vectors -> 106 non-zero flash targets -> function_start=106, mid_function=0, default_b_loops=86
```
=> **Every one of the 106 valid vector targets is a Ghidra function start.** No vector target is
mid-function. The 86 identical targets are the shared `fe e7` (`b .`) default handlers.
This closes the CONTEXT.md §4 structural puzzle with Ghidra's own function detector (matches
the capstone cross-check).

Key functions (Ghidra names; auto-named unless noted):

| addr | name | size | note |
|---|---|---|---|
| 0x08004af8 | `Reset_Handler` | 34 | renamed by PreSetup; 2 callees (0x08020A20, 0x0801666C) |
| 0x08004a0c | `FUN_08004a0c` | — | ARMCC `__main` (calls __scatterload 0x08004A14, __rt_entry 0x08004AD8) |
| 0x08004ad8 | `FUN_08004ad8` | 32 | `__rt_entry`, 6 callees (calls main 0x08033E20) |
| 0x0801666c | `FUN_0801666c` | 80 | `SystemInit` (RCC/FLASH/PLL) |
| 0x08020a20 | `FUN_08020a20` | 78 | early-boot check (system-memory flags 0x1FFFC010); tail->NVIC_SystemReset |
| 0x08033e20 | `FUN_08033e20` | 1136 | **main()** — sets VTOR=0x08004800, 57 callees |
| 0x08006f0c | `FUN_08006f0c` | 9716 | largest function; ADR-refs "Model: " / "Firmware: " (info/settings screen) |
| 0x080079e8 | `FUN_080079e8` | 2102 | ADR-refs "Setting" |
| 0x08005264 | `FUN_08005264` | — | ADR-refs "GR A-E for TTL/M mode" |

Function size distribution (insn_count): 27x 1 insn, 501x 2-10, 538x 11-50, 275x 51-200,
90x >200. Many 2-insn default/return stubs are interrupt handlers.

**RTOS or bare-metal?** No RTOS in this image. Evidence:
* `SVC_Handler` (0x08013B7C) = `bx lr` (2 insns, no context-switch code); `PendSV_Handler`
  (0x08010BFC) = `bx lr`; `SysTick_Handler` (0x08016668) = `bx lr` -> no RTOS kernel vector
  implementations (PendSV/SVC unused).
* The 1 ms scheduler tick is a **hardware timer IRQ**, not SysTick: TMR7 IRQ55 -> 0x08016D50 ->
  tick updater 0x080334EC -> RAM[0x20000310] (emulator-dev/hw-analyst finding; consistent with
  Ghidra's IRQ55 handler).
* Function inventory is a classic super-loop + IRQ bare-metal layout; no task-create tables,
  no PSP usage, no RTOS strings.

**UI architecture.** UI is a state machine driven by the main loop; strings are addressed with
Thumb PC-relative ADR (no absolute pointers — see §2). `strings.csv` now has 109 xref-resolved
strings; highest-value clusters:
* 0x08006F0C (9716 B) — info/about screen ("Model: ", model names "X3 C/N/S/F/O/L",
  "Firmware: ", version strings v0.98/v1.38/v1.25/v1.21/v1.11, "Update").
* 0x080079E8 — menu ("Setting", "Multi", "Home", "Cancel", "Restore Factory").
* 0x08005264 / 0x080055xx — flash help texts ("GR A-E for TTL/M mode", "GR F-9 for M mode only").
* LCD: 368x448 RGB565 panel via XMC/NE4 at 0x6C000000 (command) / 0x6C000001 (data),
  control GPIO PC4 (hw-analyst/emulator-dev findings; Ghidra sees the display/SPI/XMC accessor
  functions in the 0x0800B200-0x0800F800 range — `datarefs.csv` has the 0x6C000000/GPIO literals).

**RF TX.** 2.4 GHz GFSK transceiver side is reachable from the same SPI/GPIO cluster; packet
buffers live in RAM around 0x20000xxx. Exact TX entry point not pinned yet (next step).

### 4.2 HOT-SHOE SUB-IMAGE (`hotshoe_blob.bin`, base 0x08002000)

| metric | value |
|---|---|
| Ghidra functions | **162** |
| decompiled OK | **157** (5 failed) |
| instructions | 6846 |
| strings | 101 |
| vectors | 46 -> 33 non-zero flash targets -> **function_start=33, mid_function=0**, 22 are `b .` default loops |
| files | `devices/x3c/re/ghidra_exports/hotshoe/...` |

Confirms CONTEXT §5: separate MCU image (STM32F0-class, 30 IRQs), a resource inside the main
firmware; it is NOT in the main image's address space.

### 4.3 Pipeline conclusions

1. Base 0x08004800 confirmed both by code (main sets SCB->VTOR=0x08004800 at 0x08033E20/0x08035D38)
   and by Ghidra flow analysis (all vector targets = function starts; reset handler disassembles
   cleanly; 86 `b .` default handlers land exactly on their loops).
2. The file is a container of 2 independent images (main at 0x08004800 + hot-shoe at its own
   0x08002000 space); no relocation/transformation layer.
3. Ghidra decompiled C in `devices/x3c/re/ghidra_exports/main/decompiled/` (1418 files,
   `<0xADDR>_<name>.c`) and `.../hotshoe/decompiled/` (157 files); query arbitrary addresses with
   `devices/x3c/tools/decompile.py <addr>...`.

### 4.4 Not finished / next steps

* Rename well-known library/startup functions (main, SystemInit, __main, __rt_entry, handlers) in
  a Ghidra post-script if the team wants nicer names — currently only Reset_Handler is renamed.
* 13 decompile failures are in high-entropy/table data misidentified as code (0x080FB258,
  0x080F8138); treat as data, or mark undefined and re-export.
* RF TX entry point + packet buffer (0x20000xxx region) not yet pinned from decompiled C.
