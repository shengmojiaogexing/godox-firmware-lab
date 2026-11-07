# UI_STRINGS.md — Godox X3-C v1.38: UI string inventory & referencing
Firmware: `devices/x3c/firmware/X3_C_v1.38_app.bin` (1,021,468 bytes).  
**vaddr = file offset + 0x08004800** (app link base); the embedded hot-shoe image at file 0x0F3800 uses base 0x08002000 and contains code only (no UI strings).

## 1. How the firmware references strings — **ADR (PC-relative), not literal pools**
No 32-bit literal in the image points at these strings (checked: all 1,021,468 bytes, all aligned words; 0 hits for e.g. 0x08007350 "Model: ", 0x080241C0 "Hot Shoe Upgrading"). The reason: the UI strings sit in **literal-pool/rodata gaps next to the code that uses them**, and the compiler reaches them with **`ADR` (Thumb `add rX, pc, #imm`, ±4 KB)**. Scanning every even file offset for `adr` yields **72 strings that are referenced directly by ADR**. This also explains why "no literal xrefs" was reported earlier — it is the expected ARMCC codegen for nearby rodata.

Representative ADR references (string vaddr ← referencing instruction):
| string | file off | vaddr | ADR site(s) |
|---|---|---|---|
| `A~E组适用于TTL/M模式` | 0x000ed8 | 0x080056d8 | `0x08005582` |
| `F~9组仅用于M模式` | 0x000ef4 | 0x080056f4 | `0x080055aa` |
| `GR A-E for TTL/M mode` | 0x000f0c | 0x0800570c | `0x080055fc` |
| `GR F-9 for M mode only` | 0x000f24 | 0x08005724 | `0x08005624` |
| `A~C组适用于TTL/M模式` | 0x000f3c | 0x0800573c | `0x0800567c` |
| `D~9组仅用于M模式` | 0x001084 | 0x08005884 | `0x08005768` |
| `GR A-C for TTL/M mode` | 0x00109c | 0x0800589c | `0x080057bc` |
| `GR D-9 for M mode only` | 0x0010b4 | 0x080058b4 | `0x080057e4` |
| `自动关机` | 0x001528 | 0x08005d28 | `0x08005a50` |
| `Auto Off` | 0x001538 | 0x08005d38 | `0x08005a76` |
| `单拍模式` | 0x001f18 | 0x08006718 | `0x08006374` |
| `One-Shoot` | 0x001f28 | 0x08006728 | `0x08006398` |
| `群拍模式` | 0x001f44 | 0x08006744 | `0x08006458` |
| `All-Shoot` | 0x001f54 | 0x08006754 | `0x0800647c` |
| `群拍模式只作用于A~E组` | 0x001f74 | 0x08006774 | `0x080065f2` |
| `All-shoot mode only` | 0x001f98 | 0x08006798 | `0x08006644` |
| `型号：` | 0x002b44 | 0x08007344 | `0x08006ff6` |
| `Model: ` | 0x002b50 | 0x08007350 | `0x08007018` |
| `固件版本：` | 0x002b8c | 0x0800738c | `0x0800712c` |
| `Firmware: ` | 0x002b9c | 0x0800739c | `0x0800714e` |
| `固件升级` | 0x002bd0 | 0x080073d0 | `0x0800727a` |
| `Update` | 0x002be0 | 0x080073e0 | `0x0800729c` |
| `频闪` | 0x003a68 | 0x08008268 | `0x08007f78` |
| `Multi` | 0x003a70 | 0x08008270 | `0x08007f9c` |

Full machine-readable dump of *all* extracted strings (ASCII, UTF-8 CJK, mixed): `devices/x3c/re/hwscan_strings.tsv` (3720 raw entries, tab-separated: file offset, vaddr, kind, text). The filtered human-readable inventory is below.

## 2. Where the strings live
* UI string/rodata clusters (file offsets, vaddr = +0x08004800):
  * 0x0EF8–0x1120 (va 0x080056F8–0x08005920): group/modeling help ("A~E组适用于TTL/M模式", "A~E for TTL/M mode", "GR A-E for TTL/M mode", "GR F-9 for M mode only", "A~C…")
  * 0x1520–0x1560 (va 0x08005D20): auto-off/standby ("自动关机" / "Auto Off")
  * 0x1F00–0x2000 (va 0x08006700): shooting modes ("单次模式"/"One-Shoot", "群拍模式"/"All-Shoot", "L-858", "All-shoot mode only")
  * 0x1F9C0 (va 0x080241C0): "Hot Shoe Upgrading"
  * 0x2B40–0x2BE8 (va 0x08007340–0x080073E8): About page: "型号：/Model: ", model variants "X3 C/N/S/F/O/L", "固件版本：/Firmware: ", version strings "v0.98, v1.38, v1.25, v1.21, v1.11", "固件升级/Update"
  * 0x3A60–0x3AD0 (va 0x08008260): main menu ("Multi", "多拍", "Home", "Setting")
  * 0x4160–0x41D0 (va 0x08008960): "恢复出厂设置/Restore Factory", "Cancel", "Apply"
  * 0x6CF0–0x6D60 (va 0x0800B4F0): "是否打开造型灯" / "Turn on the modeling lamp" / "main switch"
  * 0x9F40–0x9F80 / 0xBFF0 (va 0x0800E740 / 0x080107F0): "固件错误/Firmware error"
  * 0xDB00–0xDC80 (va 0x08012300): RF scan/sync: "扫描频道/SCAN", "识别结果", "无线同步"/"Wireless Sync", "CH 21"
  * 0x10E40–0x10F40, 0x111D0–0x11280 (va 0x08015640, 0x080159D0): "Wirelese Sync" dialogs, "Please confirm wireless …", "Cancel", "Apply"
  * 0x117E0–0x11810 (va 0x08015FE0): "最小功率/Min.Power"
  * 0xB100/0xCD58 (va 0x0800F900/0x08011558): "Times", "0-30m", "1-100m", "Standby Time"
* USB product strings (not UI): file 0x1D810 "Godox", 0x1D848 "HID IAP Interface", 0x1D8B0 "Godox_IAP".
* The 0x42A20–0x0E8800 flash region is compressed bitmap/font data (theme "42 ff 08" RLE stream); text on screen is rendered from these assets + the strings above.

## 3. Grouped UI string inventory (filtered, offsets & vaddrs)
Legend: file offset (vaddr). `→ADR` = count of PC-relative references found.

### Hot shoe / upgrade
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x01f9ab | 0x080241ab | `热靴升级中` | 0 |
| 0x01f9c0 | 0x080241c0 | `Hot Shoe Upgrading` | 1 |

### About / model & firmware version
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x002b44 | 0x08007344 | `型号：` | 1 |
| 0x002b50 | 0x08007350 | `Model:` | 1 |
| 0x002b8c | 0x0800738c | `固件版本：` | 1 |
| 0x002b9c | 0x0800739c | `Firmware:` | 1 |
| 0x002bd0 | 0x080073d0 | `固件升级` | 1 |
| 0x002be0 | 0x080073e0 | `Update` | 1 |

### Standby / auto-off
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x001528 | 0x08005d28 | `自动关机` | 1 |
| 0x001538 | 0x08005d38 | `Auto Off` | 1 |
| 0x001557 | 0x08005d57 | `分钟` | 0 |

### Shooting modes / group help
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x001f18 | 0x08006718 | `单拍模式` | 1 |
| 0x001f28 | 0x08006728 | `One-Shoot` | 1 |
| 0x001f44 | 0x08006744 | `群拍模式` | 1 |
| 0x001f54 | 0x08006754 | `All-Shoot` | 1 |
| 0x001f74 | 0x08006774 | `群拍模式只作用于A~E组` | 1 |
| 0x001f98 | 0x08006798 | `All-shoot mode only` | 1 |

### Group A-E / model help
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x000ef4 | 0x080056f4 | `F~9组仅用于M模式` | 1 |
| 0x000f0c | 0x0800570c | `GR A-E for TTL/M mode` | 1 |
| 0x000f24 | 0x08005724 | `GR F-9 for M mode only` | 1 |
| 0x000f3c | 0x0800573c | `A~C组适用于TTL/M模式` | 1 |
| 0x001084 | 0x08005884 | `D~9组仅用于M模式` | 1 |
| 0x00109c | 0x0800589c | `GR A-C for TTL/M mode` | 1 |
| 0x0010b4 | 0x080058b4 | `GR D-9 for M mode only` | 1 |

### Modeling lamp / main switch
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x006cfc | 0x0800b4fc | `是否打开总造型灯开关` | 1 |
| 0x006d1c | 0x0800b51c | `Turn on` | 1 |
| 0x006d24 | 0x0800b524 | `the modeling lamp` | 1 |
| 0x006d38 | 0x0800b538 | `main switch` | 1 |

### Firmware error / update
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x009f47 | 0x0800e747 | `固件错误` | 0 |
| 0x009f5c | 0x0800e75c | `Firmware error` | 1 |

### Settings / factory reset
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x004174 | 0x08008974 | `恢复出厂设置` | 1 |
| 0x004188 | 0x08008988 | `Restore Factory` | 1 |
| 0x0041a4 | 0x080089a4 | `取消` | 1 |
| 0x0041ac | 0x080089ac | `Cancel` | 1 |
| 0x0041b4 | 0x080089b4 | `确定` | 1 |
| 0x0041bc | 0x080089bc | `Apply` | 1 |

### Language / setting
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x008b2c | 0x0800d32c | `English` | 1 |

### RF: scan / wireless sync / CH-ID
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x00db44 | 0x08012344 | `识别号` | 1 |
| 0x00db60 | 0x08012360 | `无线同步` | 1 |
| 0x00db70 | 0x08012370 | `Wireless Sync` | 1 |
| 0x00dc0c | 0x0801240c | `CH 21` | 2 |
| 0x00dc1b | 0x0801241b | `CH %d` | 0 |
| 0x00dc26 | 0x08012426 | `BL h` | 0 |

### RF: wireless sync dialogs
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x010e88 | 0x08015688 | `请查看接收端无线是否同步` | 1 |
| 0x010eb4 | 0x080156b4 | `Wirelese Sync` | 1 |
| 0x010ecc | 0x080156cc | `Please confirm wireless` | 1 |
| 0x010ee5 | 0x080156e5 | `sync on the receiver` | 0 |

### Min.Power / power
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x0117ec | 0x08015fec | `最小功率` | 1 |
| 0x0117fc | 0x08015ffc | `Min.Power` | 1 |
| 0x011808 | 0x08016008 | `档位` | 1 |

### UI base (multi/home/setting)
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x003a68 | 0x08008268 | `频闪` | 1 |
| 0x003a70 | 0x08008270 | `Multi` | 1 |
| 0x003a7c | 0x0800827c | `主界面` | 1 |
| 0x003a88 | 0x08008288 | `Home` | 1 |
| 0x003aa0 | 0x080082a0 | `设置` | 1 |
| 0x003aa8 | 0x080082a8 | `Setting` | 1 |

### Other UI strings
| file | vaddr | text | ADR refs |
|---:|---:|---|---:|
| 0x000ed8 | 0x080056d8 | `A~E组适用于TTL/M模式` | 1 |
| 0x0015cb | 0x08005dcb | `sO 71` | 0 |
| 0x0024ad | 0x08006cad | `xHq?H` | 0 |
| 0x00252d | 0x08006d2d | `xHp+H` | 0 |
| 0x002543 | 0x08006d43 | `xHq)H` | 0 |
| 0x00255b | 0x08006d5b | `xHr'H` | 0 |
| 0x002569 | 0x08006d69 | `xHs%H` | 0 |
| 0x004ac1 | 0x080092c1 | `2xZ` | 0 |
| 0x006ddc | 0x0800b5dc | `ytph'` | 0 |
| 0x008b01 | 0x0800d301 | `!ph` | 0 |
| 0x008b18 | 0x0800d318 | `中文` | 1 |
| 0x008b4b | 0x0800d34b | `M hii` | 0 |
| 0x009768 | 0x0800df68 | `PROP` | 1 |
| 0x009ea6 | 0x0800e6a6 | `h AC` | 0 |
| 0x00b104 | 0x0800f904 | `次数` | 1 |
| 0x00b10c | 0x0800f90c | `Times` | 1 |
| 0x00b548 | 0x0800fd48 | `赫兹` | 1 |
| 0x00d838 | 0x08012038 | `扫描频道` | 1 |
| 0x00d84c | 0x0801204c | `SCAN` | 1 |
| 0x00d864 | 0x08012064 | `频道` | 1 |
| 0x00f6b4 | 0x08013eb4 | `秒钟` | 2 |
| 0x00fb70 | 0x08014370 | `屏幕待机` | 1 |
| 0x00fb80 | 0x08014380 | `Standby Time` | 1 |
| 0x01110f | 0x0801590f | `eP O` | 0 |
| 0x011d9c | 0x0801659c | `前帘同步` | 1 |
| 0x011dac | 0x080165ac | `Front Curtain` | 1 |
| 0x011dcc | 0x080165cc | `高速同步` | 1 |
| 0x011ddc | 0x080165dc | `High Speed` | 1 |
| 0x011dec | 0x080165ec | `后帘同步` | 1 |
| 0x011dfc | 0x080165fc | `Rear Curtain` | 1 |
| 0x01d80f | 0x0802200f | `Godox` | 0 |
| 0x01d847 | 0x08022047 | `HID IAP Interface` | 0 |
| 0x01d8af | 0x080220af | `Godox_IAP` | 0 |
| 0x01d8ea | 0x080220ea | `Diei=` | 0 |
| 0x01fb38 | 0x08024338 | `hh ep` | 0 |
| 0x022094 | 0x08026894 | `to unlock` | 1 |
| 0x0220a0 | 0x080268a0 | `长按2秒解锁` | 1 |
| 0x02d450 | 0x08031c50 | `Option 1` | 1 |
| 0x04dde0 | 0x080525e0 | `ffffbo` | 1 |

## 4. Referencing mechanism details
* The string address is materialised by a 16-bit Thumb `ADR` (e.g. 0x08006FF6 → "型号：" at 0x08007344; 0x08007018 → "Model: " at 0x08007350). Multiple strings of one screen are referenced from the same function (the About-page renderer around 0x08006FF0–0x08007270), so a screen = one function + a cluster of ADR-reachable strings.
* No string-ID table and no pointer array is needed: the UI code is compiled with `--split_sections`-style layout where each function's rodata follows it, so ADR suffices.
* Some strings are printf formats (`- %d.%d`, `+0.%d`, `+%d.%d`, `+0.3`, `%IH1`) and are reached by ADR too; they format power/TCM offsets.
* UTF-8 Chinese and ASCII are separate NUL-terminated strings; mixed strings (e.g. "A~E组适用于TTL/M模式") are single UTF-8 strings.

## 5. Notes for UI/emulator work
* Strings are drawn with the compressed bitmap font assets (0x42A20–0x0E8800); for screenshots emulate the LCD (448×368 RGB565 via XMC 0x6C000000/1) and capture ADR/rodata reads or simply render the captured VRAM. See HARDWARE.md §4.
* The About page (function ~0x08006FF0) is the best first target for text rendering tests: strings "型号："/"Model: ", "固件版本："/"Firmware: " + "v1.38" + model list X3 C/N/S/F/O/L are all ADR-referenced there.
