# AD200 Pro V1.60 —— C.Fn 交互路径逆向 + 「F8 切换后同步字立即生效」补丁方案

> task-1 / teammate **cfn-re**，写入范围 `keypatch/dev/ad200_ui/menu/`（未改动其它目录）。
> 依据：`devices/ad200pro/firmware/AD200pro_V1.60.bin`（46380 B，sha256 `fc0f77f1…98bd0`，基址 0x08003000）
> + `devices/ad200pro/analysis/{disasm.txt,functions.json}`；动态验证用 Unicorn 2.1.4 调用级 harness。
> 说明：标 **[Lead已做]** 的是 Lead 已完成/已合入的内容（我只做复核），标 **[本报告]** 的是我独立新增的结论。

---

## 0. 结论先行

| # | 问题 | 结论 |
|---|---|---|
| 1 | C.Fn 怎么进/出？ | **MENU 键 = 矩阵 bit `0x100`（PB5×PC12）** → `ui_key_100` 0x08004270 → `UI+0x12`(0x20000162)=1 打开 / =0 关闭（0x08004290）。另有 5 类退出路径（其它任意键/外部触发/超时），见 §3。 |
| 2 | 当前项导航？ | `UI+0x14`(0x20000164) 全固件**只有 3 个写点**（dec 0x08004600、inc 0x0800855E/0x08008564），读者只有 cfn_render。0..7 回绕 → **F8(item7) 可达**。+/- 不在 3×3 矩阵里，是 **GPIOA PA8/PA9 双相输入** → `flash_power_task` 0x08004500 → `inc 0x08008504` / `dec 0x080045AC`。 |
| 3 | 值编辑？ | **SET 键 = 矩阵 bit `0x020`（PB4×PC12）** → `ui_key_20` 0x0800B90C → C.Fn 内 `UI+0x13 ^= 1`（0x0800B94C..0x0800B954）。编辑态改值写 `settings[item]`（dec 0x080045EA / inc 0x08008542），F8 上限表 `0x0800E314[7]=1` → **F8 值只 0/1**。 |
| 4 | 值改变后保存？ | inc/dec 处理器入口调 `0x0800B810` → `[0x20000218]=200`（倒计时）；main 每轮调 `0x08009B38`：`0x0800B7F4` 递减，到 0 → `param_record_build(0x20000484)` → `flash_store_params 0x0800B820` → `flash_write_params 0x08005974` 写 `0x0801F000+slot*100`。**记录字节 31 = settings[7]**（param_record_build 0x08004496/0x0800449C 写 word7）。 |
| 5 | 立即生效补丁点？ | 原厂 **只有 item==6(ID)** 改值才调同步字函数（dec 0x080045EC、inc 0x0800854C 的 `cmp rX,#6 / bne`），改 F8 不调 → 必须补。**[Lead已做] 方案 A**：两处 4B 改 `00 bf 00 bf`（2×nop）→ 任何项改值都重同步，有效，副作用有限。**[本报告] 方案 B（可选更小）**：只把两处 `bne`(0xD1) 改成 `blo`(0xD3)，**共 2 字节**，条件变 `item>=6` → 仅 ID/F8 重同步，F1..F6 与原厂完全一致。两方案均经 Unicorn 验证。**Lead 的 v4 模块（`keypatch/devices/ad200.py`）已采用方案 A，并额外屏蔽了 6 处 settings[7] 原厂读者 + ui_key_80 的静默清零；这些字节我已逐条复核（§7.7）。** |
| 6 | `0x08008AB4-AB6` 属于哪条路径？ | **不是 F8 切换路径**。它是 `ui_key_scan_task` 长按（重复阶段）里 `0x010|0x004` 组合键路径：`0x08008AAA bl 0x080048E4`（恢复出厂默认，settings[0..7] 清零并置 settings[2]=1）→ 关 C.Fn(0x08008AB2) → `ldrb r0,[settings+6]` → `bl 0x0800B9AC`。即「恢复出厂后重新应用同步字」，可作为固件确有「改设置后重同步」惯例的旁证，但**不能**解决 F8 切换即时生效。 |

**补丁建议（若 Lead 愿意采用最小改动）**：

```
文件偏移 0x15EF：0xD1 -> 0xD3     ; 0x080045EF  dec 处理器：bne -> blo
文件偏移 0x554F：0xD1 -> 0xD3     ; 0x0800854F  inc 处理器：bne -> blo
（只改 2 字节，其余全部保持原厂；两处同步字调用点的条件变为 item>=6）
```

---

## 1. 证据与方法（可复现）

| 脚本 | 作用 | 日志 |
|---|---|---|
| `menu_scan.py` | 解析 `analysis/disasm.txt`+`functions.json`，对全部 323 个函数做一次 worklist 常量传播（含寄存器偏移寻址 / `pop {..,pc}` 终止），输出所有关键 RAM 地址的读写点、被调点、关键函数反汇编 | `report.txt`（UTF-8） |
| `dump_funcs.py` | 用 capstone 直接反汇编 20+ 个关键函数（含 PC 相对字面量解析），写 `disasm_menu.log` | `disasm_menu.log` |
| `dump_region.py` | 任意地址区间反汇编（复核细节用），如 `python dump_region.py 0x080045AC 0x60` | stdout |
| `check_bytes.py` | 21 项字节/常量断言（补丁点、上限表、同步函数、调用点、保存链路、F8 渲染分支、矩阵解码）—**21/21 PASS** | `check_bytes.log` |
| `verify_handlers.py` | **Unicorn 调用级 harness**：真实按键注入（GPIOA PA8/PA9 模拟）、inc/dec 处理函数原厂/补丁 A/补丁 B 对比、端到端 MENU→导航→SET→改值、F8 段码渲染 —**51/51 PASS** | `verify_handlers.log` |

复现：

```powershell
cd keypatch\dev\ad200_ui\menu
devices\ad200pro\python\python.exe menu_scan.py
devices\ad200pro\python\python.exe dump_funcs.py
devices\ad200pro\python\python.exe check_bytes.py
devices\ad200pro\python\python.exe verify_handlers.py
```

> `menu_scan.py` 的关键修正（相比朴素实现）：`pop {..,pc}`/`bx lr` 后不沿线性地址继续走（否则会把返回后的指令当成可达路径，导致 `UI+0x14` 写点漏报）；并支持 `[base, index_reg]` 寄存器偏移寻址（`settings[UI+0x14]` 这类动态索引）。

---

## 2. 键码 ↔ GPIO 矩阵 ↔ 处理函数总表

3×3 矩阵扫描在 `ui_key_scan_task` 0x08008818：行 PB3/PB4/PB5（PB3: `exti_config` mask 0x08 @0x08008834/0x08008ab0，PB4: mask 0x10 @0x080088a2/0x0800891e，PB5: mask 0x20 @0x08008912），列 PC10/PC11/PC12（`gpio_read_pin(GPIOC, 0x400/0x800/0x1000)` @0x08008870/0x08008884/0x08008896 等），bit = 3×row + col。
**去抖**：同一键值需连续 2 次扫描一致（`UI+0x26` 上次键值比较 0x08008A40→0x08008A44，0x08008A46 才分派）；按住 **0x32(50) 次扫描**后进入重复阶段（`UI+0x06` 计数上限 0x080089EE，重复分派 0x080089F8）。

| 键码 | 行×列 | 新按下分派 | 处理器 | C.Fn 内效果 |
|---|---|---|---|---|
| 0x001 | PB3×PC10 | 0x08008A84 | `ui_key_1` 0x08008D08 | 关闭 C.Fn（0x08008D0C→0x08008D34 `strb r2,[UI+0x12]`） |
| 0x002 | PB3×PC11 | 0x08008A90 | `ui_key_2` 0x0800C190 | 无 C.Fn 分支（`[UI+0x18]` 调 0x08005AE8，TEST 类） |
| 0x004 | PB3×PC12 | 0x08008A7E | `ui_key_4` 0x08009B7C | 关闭 C.Fn（0x08009B88→0x08009BD8） |
| 0x008 | PB4×PC10 | —（无分支） | — | 矩阵位未使用 |
| 0x010 | PB4×PC11 | 0x08008A5E：置 `UI+0x08=1`（延迟到无键轮次调 0x080063A0） | `FUNC_080063a0` | 关闭 C.Fn（0x080063A8→0x080063FA） |
| 0x020 | PB4×PC12 | 0x08008A8A | `ui_key_20` 0x0800B90C | **切换编辑态**：`UI+0x13 ^= 1`（0x0800B94C..0x0800B954）→ SET 键 |
| 0x040 | PB5×PC10 | —（无分支） | — | 矩阵位未使用 |
| 0x080 | PB5×PC11 | 0x08008A9C：置 `UI+0x0C=1`（无键轮次调 `ui_menu_task` 0x0800C2D8） | `ui_key_80` 0x0800BC34 | 关闭 C.Fn（0x0800BC3A→0x0800BC54）；松开后 `ui_menu_task` 0x0800C2F8 也会关 |
| **0x100** | **PB5×PC12** | **0x08008A66..0x08008A96** | **`ui_key_100` 0x08004270** | **开关 C.Fn**（见 §3） |
| 组合 0x014（0x010+0x004） | 长按重复阶段 0x080089FC | 0x08008AAA | 0x080048E4 + 0x08008AB2/0x08008AB6 | 恢复出厂默认 → 关 C.Fn → 用 settings[6] 重调同步字 |
| 组合 0x101（0x100+0x001） | 0x08008A70 | — | 置 `UI+0x2A=0x3C`、清 `UI+0x19` [推测：特殊组合/关机] | — |
| **PA8/PA9** | **GPIOA IDR bit8/bit9**（`board_init_inputs` 0x08009758 配为输入+上拉） | `flash_power_task` 0x08004500（main 0x0800CDB0 每轮调用） | **inc 0x08008504（PA9 方向） / dec 0x080045AC（PA8 方向）** | **+/-：项导航 + 值编辑**（见 §4/§5） |

> 物理丝印（MENU/SET/+/-）未在真机核对，按固件语义映射：0x100=MENU(C.Fn) 键、0x020=SET 键、PA8/PA9=± 两键或正交拨轮。**推断依据**：AD100 Pro（同架构固件，`keypatch/dev/lights_ui/FEASIBILITY.md`）记录「C.Fn（MENU 键进入）」且其 item==ID 才调 `rf_set_id` 的模式与本固件一致；PA8/PA9 的状态机支持“两键同按/先后顺序判方向”，更像两个独立 ± 键 **[推测]**。

### 2.1 +/-（PA8/PA9）输入解码（`flash_power_task` 0x08004500）

```
0x08004502 movs r4,#9 ; 0x08004506 lsls r4,#0x1b  -> r4 = 0x48000000 (GPIOA)
0x0800450E bl 0x08006E94            ; gpio_read_pin(GPIOA, 0x100=PA8)
0x08004520 bl 0x08006E94            ; gpio_read_pin(GPIOA, 0x200=PA9)
r5 = bit0(PA8 低=按下) | bit1(PA9 低=按下)   ; 0/1/2/3
switch(r5):
  r5==1 (仅 PA8): [0x20000170] = ([0x20000170]&~3)|1; [0x20000171] = 1
  r5==2 (仅 PA9): [0x20000170] = ([0x20000170]&~3)|2; [0x20000171] = 2
  r5==3 (两键):   [0x20000171]==2 -> bl 0x08008504(inc); ==1 -> bl 0x080045AC(dec)
                  [0x20000171]=0; [0x20000170]=0xC
  r5==0 (全松):   [0x20000171]==0xD -> inc; ==0xE -> dec; 清状态
```
状态变量：`UI+0x20=0x20000170`（相位）、`UI+0x21=0x20000171`（待判定方向，值仅 1/2/0xC/0xD/0xE，全部写点都在 0x08004500 内，见 `report.txt` §[1]）。
→ 该状态机把 PA8/PA9 的按下顺序翻译成 +1/-1，分派到 **inc 0x08008504** 与 **dec 0x080045AC**（唯一调用者都是 0x08004500；0x08004500 唯一调用者是 main）。
**Unicorn 真实按键注入验证**（`verify_handlers.log` §A）：PA9→both 只触发 inc；PA8→both 只触发 dec；两键都不按不触发。

---

## 3. 进入 / 退出 C.Fn（MENU 键码 0x100）

`ui_key_100` 0x08004270 全文（22 条，纯 RAM 操作）：

```
0x08004270 ldr r0,=0x20000150 (UI)
0x08004274 ldrb r2,[UI+0x00]; cmp #1; beq 0x08004280    ; 特殊模式清理
0x0800427A ldrb r2,[UI+0x0E]; cmp #0; beq 0x08004288
0x08004280 strb r1(=0),[UI]; strb r1,[UI+0x0E]; str r1,[UI+0x30]; bx lr
0x08004288 ldrb r2,[UI+0x12]                            ; C.Fn 打开标志
0x0800428A cmp #0
0x0800428C beq 0x08004298                               ; 未开 -> 打开
0x0800428E movs r2,#0                                   ; 已开 -> 关闭
0x08004290 strb r2,[UI+0x12]        ; *** 0x20000162 = 0/1 ***
0x08004292 strb r1,[UI+0x13]        ; C.Fn 编辑态 = 0
0x08004294 strb r1,[UI+0x0F]        ; 标志 = 0
0x08004296 bx lr
0x08004298 movs r2,#1
0x0800429A b 0x08004290                                 ; -> 打开
```

`UI+0x12 == 1` 时 `display_task` 0x08009228 在 0x0800939C 判断、0x080093C4 `bl 0x08003FD0` 渲染 C.Fn 页。

**退出 C.Fn 的全部写入点（`report.txt` §[1] `UI+0x12` 写）**：

| 场景 | 地址 | 说明 |
|---|---|---|
| MENU 再按 | 0x08004290 | `r2=0` |
| 任意其它矩阵键 0x001/0x004/0x010/0x080 | 0x08008D34 / 0x08009BD8 / 0x080063FA / 0x0800BC54 | 各处理器开头「若 C.Fn 打开则关」 |
| 0x080 松开后的 `ui_menu_task` | 0x0800C2F8 | |
| 外部触发 EXTI4_15（PA5 触发输入） | 0x080054DE | |
| 长按组合 0x014 | 0x08008AB2 | 恢复出厂前先关 |
| **超时** | 0x0800B9F8 | `ui_key_scan_task` 入口 `bl 0x0800B9E4`（0x0800881E）每轮递减 `UI+0x2C`（u16，任一按键活动时置 0x7D0，见 0x080089C2..0x080089C6），到 0 自动清 UI+0xF/UI+0x12 |

---

## 4. 当前项导航（UI+0x14）

**全固件读写点**（`report.txt` §[1]）：读 = cfn_render 0x08003FE6/0x08003FF8、dec 0x080045E0/0x080045F8、inc 0x08008538/0x08008558；**写 = 只有 3 处**：dec 0x08004600、inc 0x0800855E、inc 0x08008564。初始 0（RAM 清零）。

**inc 导航**（`UI+0x13==0` 时，inc 0x08008504）：

```
0x08008558 ldrb r0,[UI+0x14]
0x0800855A adds r0,#1 ; uxtb
0x0800855E strb r0,[UI+0x14]
0x08008560 cmp r0,#7
0x08008562 bls return
0x08008564 strb r2(=0),[UI+0x14]        ; >7 回绕到 0
```

**dec 导航**（dec 0x080045AC）：

```
0x080045F8 ldrb r0,[UI+0x14]
0x080045FA cmp r0,#0
0x080045FC beq 0x08004604
0x080045FE subs r0,#1 ; uxtb
0x08004600 strb r0,[UI+0x14]
0x08004602 pop
0x08004604 movs r0,#7
0x08004606 b 0x08004600                 ; 0 回绕到 7
```

→ 上限 **7**（无需改动），F8=item7 可正常到达（Unicorn：item6→item7、item7→0、item0→7 全部 PASS）。

---

## 5. 值编辑（SET 键 0x020 → UI+0x13；写 settings[item]）

**SET 键** `ui_key_20` 0x0800B90C：先做模式清理（`[UI]==1 || [UI+0x0E]!=0`），然后

```
0x0800B926 ldrb r2,[UI+0x12]; cmp #1; beq 0x0800B94C
0x0800B94C ldrb r1,[UI+0x13]
0x0800B94E movs r2,#1
0x0800B950 eors r1,r2
0x0800B952 strb r1,[UI+0x13]            ; UI+0x13 ^= 1（进入/退出值编辑）
```
（C.Fn 未打开时走另外的 `UI+0x0F` 子状态逻辑，与 C.Fn 无关。）

**编辑态判定**：`UI+0x13 != 0` → 改值；`==0` → 只移动 item。渲染时 `cfn_render` 读 `UI+0x13`（0x0800402A/0x0800403A/0x0800405A/0x080041A4/0x080041E2/0x080041FE 等），配合 `[0x2000011F]`（闪烁相位）做值闪烁。

**dec 改值**（0x080045AC）：

```
0x080045D6 ldrb r0,[UI+0x13]; cmp #0; beq 0x080045F8   ; 非编辑态 -> 项导航
0x080045DC strb r1(=1),[UI+0x13]
0x080045DE ldr r0,=0x20000418 (settings)
0x080045E0 ldrb r2,[UI+0x14]            ; item
0x080045E2 ldrb r1,[r0,r2]              ; settings[item]
0x080045E4 cmp r1,#0
0x080045E6 beq 0x08004608               ; ==0 -> 取上限表值（回绕）
0x080045E8 subs r1,#1
0x080045EA strb r1,[r0,r2]              ; *** settings[item]-- ***
0x080045EC cmp r2,#6                    ; <-- Lead 补丁 A 改这里（本报告方案 B 不动这两条）
0x080045EE bne 0x080045F6
0x080045F0 ldrb r0,[r0,#6]              ; ID
0x080045F2 bl 0x0800B9AC                ; 同步字
0x08004608 ldr r1,=0x0800E314; ldrb r1,[r1,r2]; b 0x080045EA   ; 回绕到上限
```

**inc 改值**（0x08008504）：

```
0x0800852C ldrb r0,[UI+0x13]; cmp #0; beq 0x08008558   ; 非编辑态 -> 项导航
0x08008532 movs r0,#1; strb r0,[UI+0x13]
0x08008536 ldr r1,=0x20000418 (settings)
0x08008538 ldrb r0,[UI+0x14]            ; item
0x0800853A ldr r4,=0x0800E314           ; 值上限表
0x0800853C ldrb r3,[r1,r0]
0x0800853E adds r3,#1 ; uxtb
0x08008542 strb r3,[r1,r0]              ; *** settings[item]++ ***
0x08008544 ldrb r4,[r4,r0]              ; limit
0x08008546 cmp r3,r4
0x08008548 bls 0x0800854C
0x0800854A strb r2(=0),[r1,r0]          ; 超上限回绕 0
0x0800854C cmp r0,#6                    ; <-- 同上
0x0800854E bne 0x08008556
0x08008550 ldrb r0,[r1,#6]              ; ID
0x08008552 bl 0x0800B9AC
```

**值上限表 0x0800E314[0..7]**（`check_bytes.log` §[3]）：

| item | 0 | 1 | 2 | 3 | 4 | 5 | **6=ID** | **7=F8** |
|---|---|---|---|---|---|---|---|---|
| 上限 | 1 | 2 | 3 | 150 | 2 | 1 | **99** | **1** |

→ F8 只可能在 **0/1**（inc 到 1 再加 → 回绕 0；dec 到 0 再减 → 回绕 1）。Unicorn 四种方向全部验证 PASS。

**F8 渲染**（cfn_render 0x08003FD0）：
- 画 `F(index+1)`：0x08003FDC `bl 0x08009150`（"F"）+ 0x08003FE6 `[UI+0x14]+1` → 0x08003FEC `bl 0x080091BC`（数字）；
- item==7 分支：0x0800401E `cmp r0,#7; bne ...`，先 0x08004022 点亮 0x1C 位（冒号/单位）；
- 值：**0x08004230 `ldrb r0,[r4,#7]`（settings[7]=0x2000041F）**，`==0` → 0x08004244 按 OFF 渲染（0x08008F34 glyph0 @pos3、0x08008FA0 glyph0xF @pos5、0x0800900C glyph0xF @pos7）；`!=0` → 0x08004236 按 ON 渲染（与 F1 beeper 的 ON/OFF 同一段代码）。
- **Unicorn 段码位图证据**（`verify_handlers.log` §C，段缓冲 RAM 0x20000408）：

```
F8 OFF (settings[7]=0): 40 05 f7 8e 80 10 40 00 77 27 00 00 00 00 40 00
F8 ON  (settings[7]=1): 40 05 f5 e0 00 10 40 00 77 27 00 00 00 00 40 00
差异字节位置 [2,3,4]（值区），字节 8..9（"F8" 编号）两值一致
```

→ 复用 F8 后，灯端仍显示 `F8 OFF/ON`，用户可正常看到开关状态（丝印/文档需说明它已是密钥开关，见 Lead 的 NOTES）。

---

## 6. 值改变后的保存触发（F8 值如何持久化）

1. **倒计时启动**：inc/dec 处理器入口 `bl 0x0800B810`（dec 0x080045BE、inc 0x08008516）；`ui_key_scan_task` 对任意按键也调（0x080089CC）。`0x0800B810` 内容：

```
0x0800B810 ldr r1,=0x20000214
0x0800B812 movs r0,#0xC8
0x0800B814 strh r0,[r1,#4]      ; [0x20000218] = 200（保存倒计时，u16）
0x0800B816 bx lr
```

2. **递减**：main 0x0800CDB0 每轮调用 `0x08009B38`；其首条 `bl 0x0800B7F4` 即倒计时 tick：

```
0x0800B7F4 ldr r0,=0x20000214; ldrh r1,[r0,#4]
0x0800B7F8 cmp r1,#0; beq ret
0x0800B7FC subs r1,#1; ... strh r1,[r0,#4]
0x0800B804 bne ret
0x0800B806 movs r1,#1; strb r1,[r0,#2]    ; [0x20000214+2] = 1（到期待落盘）
```

3. **落盘**（同一函数 0x08009B38，倒计时到 0 的同一轮就执行）：

```
0x08009B40 ldrb r0,[0x20000214+2]; cmp #1; bne ret
0x08009B48 清 [+2]
0x08009B4A ldr r0,=0x20000484 ; bl 0x080043E4   ; param_record_build(record_src)
0x08009B50 bl 0x0800C740                        ; 校验/判断
0x08009B58 [+0x20000214] += 1; cmp #0x14        ; slot 计数
0x08009B64 bl 0x0800587C                        ; slot>=20 -> 擦参数页 0x0801F000
0x08009B6C bl 0x0800B820                        ; flash_store_params(slot)
```

4. **flash_store_params 0x0800B820**：`param_record_build(0x20000484)` → 把 8 words 拷到 `0x20000464` → 地址 `0x0801F000 + slot*100` → `flash_write_params 0x08005974` 编程。
5. **记录布局**：`param_record_build` 0x080043E4 在 0x08004474..0x0800449C 把 `settings[0..7]` 写入记录 **word6（+24..27）与 word7（+28..31）** → **记录字节 31 = settings[7] = F8**；上电时 `settings_load_task 0x0800AC58` → `flash_read_params 0x0800590C` → `settings_expand 0x0800CCC8` 恢复。故 F8 值**持久化**（200 个主循环 tick 后写 flash，上电加载）。
6. 恢复出厂默认：`0x080048E4` 把 settings[0..7]=0（其中 settings[2]=1、channel=0x14、UI+0x15=1），写点 0x08004912..0x08004920 —— 与 BRIEF 一致。
7. `0x0800B578` 全镜像**无调用者**（functions.json），不是保存链路（BRIEF 中提到的该地址可忽略）。

---

## 7. 「切换后立即生效」补丁方案

### 7.1 `0x08008AB4-AB6` 到底属于哪条路径

`ui_key_scan_task` 的重复阶段（按键按住 > 50 次扫描）分派（0x080089F8 起）只处理 `0x010 / 0x014 / 0x080`：

```
0x080089F8 cmp r4,#0x10; beq 0x08008AA4   ; 长按 0x010 -> ui_key_10 0x080042A0
0x080089FC cmp r4,#0x14; beq 0x08008AAA   ; 长按 0x010|0x004 组合
0x08008A00 cmp r4,#0x20; beq ret
0x08008A04 cmp r4,#0x80; bne ret; 0x08008A08 bl 0x0800BC34

0x08008AAA bl 0x080048E4                  ; 恢复出厂默认（settings[0..7] 重置）
0x08008AAE strb r6,[UI+0x0F]              ; UI+0xF = 0
0x08008AB0 ldr r0,=0x20000418 (settings)
0x08008AB2 strb r6,[UI+0x12]              ; 关闭 C.Fn
0x08008AB4 ldrb r0,[r0,#6]                ; settings[6] = ID
0x08008AB6 bl 0x0800B9AC                  ; 重新应用同步字
```

→ **是「0x010+0x004 长按 = 恢复出厂默认 → 重同步」路径，不是 F8 切换路径**，对 F8 切换即时生效没有帮助；但它证明固件本来就有「settings 改变后显式重调 0x0800B9AC」的惯例（只是仅在复位默认时用）。
另：该路径在 v4 语义下也安全（恢复默认后 settings[7]=0 → 普通模式，ID=0 → 50024/1=50024）。

### 7.2 为什么必须动 inc/dec 里的 `item==6` 条件

`0x0800B9AC` 全镜像只有 4 个调用点：rf_init 0x080048C6（上电）、dec 0x080045F2、inc 0x08008552、ui_key_scan_task 0x08008AB6（出厂复位）。改 F8 时**没有任何一处**会调用它（未打补丁时）→ 同步字要等下次上电/ID 改动/出厂复位才更新。因此需要在「F8 值已写入之后」加一次调用。

### 7.3 方案 A（**[Lead已做]**，已合入，我复核）

| 位置 | 原厂 4B | 改后 4B | 含义 |
|---|---|---|---|
| 0x080045EC（dec 内） | `06 2a 02 d1` = `cmp r2,#6` / `bne` | `00 bf 00 bf`（nop;nop） | 删掉条件 → 任何 item 改值都调同步字 |
| 0x0800854C（inc 内） | `06 28 02 d1` = `cmp r0,#6` / `bne` | `00 bf 00 bf` | 同上 |

文件偏移：0x15EC..0x15EF / 0x554C..0x554F；补丁 A 镜像 sha256 前缀 `1be959c9…`（差异恰好 8 字节）。
Unicorn 结果：F8 0↔1 均调用 `0x0800B9AC(settings[6])`，调用时 settings[7] 已是新值（先 mem 写后 sync）；item0..7 全部触发重同步；item/值编辑结果与原厂一致。

### 7.4 方案 B（**[本报告] 建议**，更小更精确）

只改条件码：`bne`(T1 条件分支, 0xD1) → `blo`(0xD3)，**1 字节/处，共 2 字节**：

| 位置 | 原厂 4B | 改后 4B | 条件语义 |
|---|---|---|---|
| 0x080045EC..0x080045EF（dec） | `06 2a 02 d1` | `06 2a 02 **d3**` | `cmp r2,#6; blo skip` → **仅 item≥6 才重同步** |
| 0x0800854C..0x0800854F（inc） | `06 28 02 d1` | `06 28 02 **d3**` | `cmp r0,#6; blo skip` → 同上 |

文件偏移：**0x15EF：0x01→0x03；0x554F：0x01→0x03**（其余字节不变）。
Unicorn 结果：F8 0↔1 均调用同步字（入参=settings[6]）；item6(ID) 20→21 仍调用（入参 21）；item0..5 与原厂完全一致（不调用）；item/值编辑结果与原厂一致；保存倒计时 200 正常。

### 7.5 两方案对比

| 项 | 方案 A（Lead，8B） | 方案 B（建议，2B） |
|---|---|---|
| 改动字节数 | 8 | **2** |
| item 改值触发重同步的范围 | 0..7 全部 | **仅 6(ID)、7(F8)** |
| F1..F6 行为 | 会多调用一次同步字（CC2500 reg4/5 重写 + 校准/延时） | **与原厂一致，不变** |
| 对 v4 同步函数（Lead 重写） | 无害（同步值只依赖 ID/F8） | 无害 |
| 叠加风险 | 极低 | **更低**（不改与 F8 无关的 F1..F6 路径） |
| 是否已合入 | 是（Lead 的 ad200.py 27 项验证） | 需 Lead 决定是否替换 |

> 两个方案都要求 v4 同步字函数**内部读 settings[7]** 决定是 KEY 还是 `50024/(id+1)`（Lead 负责）；补丁只负责「改值后立刻调用一次」。

### 7.6 副作用逐条复核（对 Lead 方案 A 的问题 2）

1. **是否有其它路径改 settings[item] 并依赖被删掉的条件？** settings[0..7] 的全部写点（`report.txt` §[1]）：
   - inc 0x08008542 / dec 0x080045EA（本次补丁所在路径，item 动态）；
   - 出厂默认 0x08004912..0x08004920（后面紧跟 0x08008AB6 显式同步）；
   - 上电加载 settings_expand 0x0800CCC8（随后 rf_init 0x080048C6 同步）；
   - 校验/默认值 FUNC_0800AE40 0x0800AF1A/0x0800AF1E（settings[0] 相关）；
   - cfn_render 0x080041BA 把非法 settings[4] 归一化写 0（渲染副作用，与同步字无关）。
   以上均不经过被删掉的 `cmp rX,#6 / bne`，**没有路径依赖该条件**。
2. **被删指令是否被当作跳转目标？** dec 侧 0x080045EC/0x080045EE 无任何分支指向（只有顺序落入）；inc 侧 0x0800854C 是 `bls 0x08008548` 的目标（0x08008548 `bls 0x0800854C`），但方案 A 后落到 nop;nop;0x08008550，语义等价；方案 B 不动 `cmp`，更安全。
3. **后续指令是否依赖被删/改的寄存器？** dec：0x080045F0 `ldrb r0,[r0,#6]` 用的是 settings 基址 r0（未变）；inc：0x08008550 `ldrb r0,[r1,#6]` 用 r1=settings 基址（未变）。无标志位依赖（之后直接 bl/pop）。
4. **多调用同步字的代价**：每次改值多 2 次 SPI 写 reg4/5 + 20µs 延时；原厂函数还多 SCAL(0x33)+1ms。只在用户改 C.Fn 值时发生，可接受；方案 B 可完全避免。
5. **与 0x08008AB6（出厂复位）路径**：互不影响（不同入口，各自调用一次同步字）。
6. **保存/持久化**：不受补丁影响（仍由 0x0800B810 倒计时 + 0x08009B38 落盘）。

### 7.7 与 Lead v4 模块当前实现的对照（我已逐字节复核）

`keypatch/devices/ad200.py`（v4，Lead 负责）当前包含三类原位补丁，我从 `devices/ad200pro/firmware/AD200pro_V1.60.bin` 独立复核其「原厂字节」全部一致：

| 类别 | 地址 | 原厂字节（我核对） | 补丁 | 我的复核结论 |
|---|---|---|---|---|
| 即时生效 hook（方案 A） | 0x080045EC / 0x0800854C | `06 2a 02 d1` / `06 28 02 d1` | `00 bf 00 bf` | ✅ 与 §7.3/7.6 一致；两处均非跳转目标（dec 侧无分支指向，inc 侧 0x08008548 `bls` 落入亦可等价执行） |
| 屏蔽 settings[7] 原厂读者 | 0x08004AFC | `e2 79`（`ldrb r2,[r4,#7]`） | `00 22`（`movs r2,#0`） | ✅ 与我的扫描 `report.txt` 一致（FUNC_08004ac8） |
| | 0x08004FF4 | `c0 79`（`ldrb r0,[r0,#7]`） | `00 20` | ✅ 一致（FUNC_08004f70） |
| | 0x08005C54 | `c9 79`（`ldrb r1,[r1,#7]`） | `00 21` | ✅ 一致（FUNC_08005c30） |
| | 0x08006290 | `d2 79`（`ldrb r2,[r2,#7]`） | `00 22` | ✅ 一致（FUNC_0800626c） |
| | 0x0800B144 | `db 79`（`ldrb r3,[r3,#7]`） | `00 23` | ✅ 一致（FUNC_0800b134） |
| | 0x0800B228 | `d2 79`（`ldrb r2,[r2,#7]`） | `00 22` | ✅ 原厂字节一致；**此读点我的静态扫描未列出**（基址来自函数入参，属本方法盲区），Lead 的补充正确 |
| 屏蔽 ui_key_80 静默清零 | 0x0800BC7E | `ca 71`（`strb r2,[r1,#7]`） | `00 bf` | ✅ 一致；我扫描也把该地址列为 settings[7] 写点（`report.txt` §[1] settings[7] 写）。路径：`ui_key_80` 在 `[UI+0xD]` bits4-3 从 0 变 1 时（0x0800BC78 `[UI+0xD]|=0x10`）随即把 settings[7] 清 0 → 复用 F8 后会**悄悄关掉密钥开关**，必须屏蔽 |

结论：v4 模块当前补丁集在调用级自洽；方案 B 只是把「即时生效 hook」进一步缩到 2 字节的**可替换**变体，若 Lead 想压缩 diff 可采用，否则保持方案 A 也完全可接受。

**同一批复核的两个小修正/提醒**：
- `SAVE_REQUEST` 的正确地址是 **0x20000216**（0x20000214+2），`SAVE_COUNTDOWN=0x20000218`（+4）——v4 模块中两者定义正确。
- 记录构造 0x080043E4 确实把 settings[0..3] 写记录字节 24..27、settings[4..7] 写 28..31（0x08004474..0x0800449C），**记录字节 31 = settings[7]**，与 BRIEF/v4 一致。

---

## 8. 风险 / 未验证项

1. **物理丝印未验证**：MENU/SET/± 与矩阵 bit、PA8/PA9 的实物对应是固件语义推断；需要真机或照片核对（sim-re 可做整机按键注入回归）。
2. **未在整机模拟器/真机跑完整按键流程**：本文用 Unicorn 调用级验证（处理函数 + GPIO 状态机 + 端到端调用序列）；整机时序（扫描周期、去抖、显示刷新）未覆盖。
3. **v4 同步字函数改写后**（Lead 去掉了原厂 SCAL(0x33)+1ms 的可能性）在「F8 切换即时调用」场景下的 RF 建立时间需 Lead 评估；原厂函数调用点本来也出现在按键上下文（ID 改动），因此上下文安全，但 v4 行为不同需自测。
4. **settings[7] 的其它原厂读者**：Lead 的 v4 已把 6 处功能读者原位改成 `movs rX,#0`、并 NOP 掉 `ui_key_80` 静默清零（见 §7.7，我逐字节复核一致）。剩余风险集中在「是否有本方法/Lead 都未发现的 settings[7] 读者」——由 **flag-re / task-3** 独立复核；本文确认 cfn_render 0x08004230（渲染）与 param_record_build 0x08004496（保存）应保留。
5. **F8 的“ON”语义**：复用后 ON=密钥模式、OFF=普通模式；显示仍为 F8 ON/OFF，用户文档需说明（Lead 的 NOTES/README 负责）。
6. **断电时机**：改值后需 ~200 主循环 tick 才写 flash；若立刻断电可能丢失本次修改（原厂机制，未改变）。

---

## 9. 文件清单 / 复现

```
keypatch/dev/ad200_ui/menu/
├── CFN.md                 ← 本文
├── menu_scan.py           ← 全固件常量传播扫描（RAM 读写点/调用点/反汇编）
├── report.txt             ← menu_scan.py 输出（UTF-8）
├── dump_funcs.py          ← capstone 关键函数反汇编
├── disasm_menu.log        ← dump_funcs.py 输出
├── dump_region.py         ← 任意区间反汇编小工具
├── check_bytes.py         ← 21 项字节/常量断言
├── check_bytes.log        ← 21/21 PASS
├── verify_handlers.py     ← Unicorn 调用级 harness（原厂 vs 补丁 A vs 补丁 B）
└── verify_handlers.log    ← 51/51 PASS
```

Unicorn 验证摘要（`verify_handlers.log`）：

```
D 补丁字节：A=8B/差异 0x15EC..0x15EF,0x554C..0x554F；B=2B/差异 0x15EF,0x554F
A 真实按键路径：PA9->both=inc，PA8->both=dec，无键不触发
B1 导航：item7->0、item0->7、item6->item7；F8 0<->1 四种方向
B2 原厂：仅 item6 调同步字；F8 改值不调（=> 必须补丁）
补丁A：item0..7 全调同步字，入参=settings[6]；F8 两方向均“先写后调”
补丁B：仅 item6/7 调同步字，F1..F6 与原厂一致
E 端到端：MENU 开 C.Fn -> - 导航到 F8 -> SET 进入编辑 -> + 改值 settings[7]=1
         且立即调同步字(ID=5) -> SET 退出编辑 -> MENU 关 C.Fn；原厂同路径不调同步字
C F8 渲染：settings[7]=0/1 段位图不同（差异在值区）
```

---

## 附录 A —— 关键地址速查

| 用途 | 地址 |
|---|---|
| 同步字函数（Lead 整段改写） | 0x0800B9AC..0x0800B9E4（56B），50024 字面量 @0x0800B9E0 |
| 同步字调用点 | 0x080048C6（rf_init）/ 0x080045F2（dec）/ 0x08008552（inc）/ 0x08008AB6（出厂复位） |
| C.Fn 渲染 | 0x08003FD0（仅 display_task 0x080093C4 调用） |
| C.Fn 打开标志 UI+0x12 | 0x20000162（打开/关闭都在 ui_key_100 0x08004290） |
| C.Fn 编辑态 UI+0x13 | 0x20000163（SET ui_key_20 0x0800B952 异或） |
| C.Fn 当前项 UI+0x14 | 0x20000164（写：0x08004600/0x0800855E/0x08008564；读：cfn_render/inc/dec） |
| C.Fn 重复计数 UI+0x15 / 编辑计数 UI+0x16 | 0x20000165 / 0x20000166 |
| UI 基址 / 超时 UI+0x2C | 0x20000150 / 0x2000017C（0x7D0，超时清 C.Fn：0x0800B9F8） |
| settings[0..7] | 0x20000418..0x2000041F（F7=ID=+6，F8=+7） |
| 值上限表 | 0x0800E314[0..7] = 1,2,3,150,2,1,99,1 |
| inc 处理器（+） | 0x08008504 |
| dec 处理器（-） | 0x080045AC |
| +/- 输入轮询任务 | 0x08004500（PA8=bit8 / PA9=bit9，状态 0x20000170/0x20000171） |
| 矩阵键扫描任务 | 0x08008818（MENU=0x100 分派 0x08008A66..0x08008A96） |
| 恢复出厂默认 | 0x080048E4（settings 清零，settings[2]=1、channel=0x14） |
| 保存倒计时置 200 | 0x0800B810 → [0x20000218]（u16） |
| 倒计时递减 / 到期落盘 | 0x0800B7F4 / 0x08009B38（main 每轮调用） |
| 记录构建 / 写 flash | 0x080043E4（记录 word7 含 settings[4..7]）/ 0x0800B820 → 0x08005974（0x0801F000+slot*100） |
| 记录加载 | 0x0800AC58 → 0x0800590C → 0x0800CCC8 |

## 附录 B —— settings[7] 的全部读点（供 flag-re 交叉核对，来自 `report.txt`）

`cfn_render 0x08004230`（F8 渲染）、`param_record_build 0x08004496`（保存记录）、`0x08004AFC`（FUNC_08004ac8）、`0x08004FF4`（FUNC_08004f70）、`0x08005C54`（FUNC_08005c30）、`0x08006290`（FUNC_0800626c）、`0x0800B144`（FUNC_0800b134）。
+ Lead v4 补充的 `0x0800B228`（原厂 `ldrb r2,[r2,#7]`，我复核字节一致；静态扫描因基址来自函数入参未列出）。
写点：`dec 0x080045EA / inc 0x08008542`（item 动态）、`出厂默认 0x08004920`、`上电加载 settings_expand 0x0800CD78`、**`ui_key_80 0x0800BC7E`（模式切换时静默清 0，v4 已 NOP）**。
