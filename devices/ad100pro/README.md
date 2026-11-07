# 神牛 AD100 Pro 闪光灯固件逆向 + 调试模拟器

> 目标固件：`AD100pro_V1.2.bin`（50248 字节，sha256 `3a3d3b91…`，官方 zip 解出）
> 目标硬件：神牛 AD100 Pro 便携闪光灯（STM32F0 兼容 MCU，Cortex-M0，128 KB Flash / 8 KB SRAM，2.4G 射频，128×64 单色 OLED）
> 本目录内容：静态逆向分析流水线、反汇编清单、伪反编译 C、还原 C、逆向文档、Unicorn 模拟器、网页调试界面。
>
> 本目录位于仓库 `devices/ad100pro/`；下文的相对命令假设已按第 1 节先 `cd` 到本目录，
> 仓库根的便携 Python 为 `tools\python\python.exe`（从本目录即 `..\..\tools\python\python.exe`）。

---

## 1. 一键验证（三种入口）

```powershell
# 从仓库根开始
cd devices\ad100pro

# ① 静态分析（重新生成 out/disasm.txt、out/functions.json、外设映射）
..\..\tools\python\python.exe tools\disasm.py
..\..\tools\python\python.exe tools\periph.py

# ② 命令行模拟器：跑 500 万条指令，打印寄存器/外设/结束状态，导出屏幕 PNG
..\..\tools\python\python.exe emu\main.py --run 5000000 --screen out\lcd_emu.png
..\..\tools\python\python.exe emu\main.py --regs --mem 0x200004CC 1024

# ③ 网页调试界面（先起模拟器后端，再起静态站点）
..\..\tools\python\python.exe emu\server.py                      # 127.0.0.1:8765
..\..\tools\python\python.exe -m http.server 8080 --directory webui
# 浏览器打开 http://127.0.0.1:8080/   （无后端时可 ?mock=1 看假数据）
```

> 注意：Python 用仓库自带的便携版 `..\..\tools\python\python.exe`（3.11.9 + capstone 5.0.7 + unicorn 2.1.4），无需系统安装。

---

## 2. 逆向得到的固件布局（关键结论，全部经过模拟器实跑验证）

| 项目 | 结论 | 证据 |
|---|---|---|
| 镜像类型 | 不是整片 flash，是 **应用镜像**，链接基址 **0x08003000** | 向量表 Reset=0x080030D5 指向 0x080030D4 的 `ldr r0,=SystemInit; blx r0`；镜像末尾 0x0800F448 恰为 init_array 结束指针 |
| 前面 0x3000 字节 | bootloader（不在此 .bin 内） | 镜像基址 + 0x0 处无任何指针引用 |
| Flash / RAM | **128 KB / 8 KB**，初始 SP=0x20000CD0 | 代码访问 0x0801F000、0x0801F800 出厂参数区（超出 64 KB） |
| 出厂标定区 | 0x0801F000 起 20×100 字节记录，首字节 magic=0x5C；0x0801F800 标识字（与 0x1018 比较） | `0x0800AD5E` 处 `ldr r1,=0x0801F000; …; bl 0x08005E8C` |
| 向量表 | 48 项 @0x08003000；main 里复制到 **RAM 0x20000000**，再 `SYSCFG.CFGR1 |= 3` 做 SRAM 向量重映射 | `func_0800CF74` 开头 |
| MCU | STM32F0 兼容（RCC 0x40021000 / GPIO 0x48000000+N×0x400 / FLASH_IF 0x40022000 / EXTI / SYSCFG / ADC / DMA / TIM / RTC / IWDG 全部对得上） | `out/periph_summary.md` |
| 显示 | **128×64 单色 OLED，SH1106/SSD1306 类控制器**，4 线软件串行：SCL=PB6、SI=PA10、A0=PB7、CS=PB9、RESET=PB8；帧缓冲 **RAM 0x200004CC**（8 页×128 列） | 初始化命令序列 `AE 02 10 40 B0 81 FF A1 A6 A8 3F AD 8B 33 C8 D3 00 D5 80 D9 1F DA 12 DB 30 A0 C0 AF`（0x0800C954），实测位流解码与 RAM 帧缓冲一致 |
| 射频 | **TI CC2500/CC1101 寄存器兼容芯片**，4 线软件 SPI：CS=PC6、SCK=PB13、MOSI=PB12、MISO=PB14、GDO0=PA15→EXTI15→IRQ7；32 信道 2413.0–2464.5 MHz；同步字 = 编码 ID（`50024/(ID+1)`，与用户仓库 `godox_encode_id` 完全一致） | docs/03_rf_protocol.md |
| 协议 | 包长 4 字节硬件 payload；`0xD5` 命令帧 / `0xA9` 属性帧 `[A9][0x50 or 9+组][key≥0xB0][value]`；TX 0x0800AA54→0x0800C204；RX 分发 0x0800AA74 | 同上 |
| 时钟 | SYSCLK = **HSI48 = 48 MHz**（CFGR.SW=3，PLL 不用），FLASH 1WS，SysTick 1 ms，TIM1 100 Hz / TIM2 1 Hz / TIM3 ≈37.5 kHz PWM / TIM14 5 kHz | docs/02_boot_and_drivers.md |
| 按键 | 电源键 PB0→EXTI0(IRQ5)、第二键 PA6→EXTI6(IRQ7)；另有 3×5 矩阵扫描（行 PB3/4/5，列 PC10/11/12/PC7/PD4），扫描函数 0x08008D00 | 同上 |
| 电池 | ADC 采样 → 状态机 0x0800A034/0x0800A140，20 ms 调度 0x0800A230 | 同上 |
| 看门狗 | IWDG LSI/256、reload 0xFFF ≈ 26 s，喂狗 0x080074F0 | 同上 |
| 主循环 | 无 RTOS；1 ms SysTick 置 flag（RAM 0x200001E1），main 轮询后跑 16 个任务 + 刷新 IWDG | docs/02、out/decompiled/FINDINGS.md |
| 有效中断 | IRQ2=0x0800ACF8、IRQ5=0x08005A78、IRQ6=0x08005B00、IRQ7=0x08005B0C、IRQ9=0x08004790、IRQ13=0x0800BA7C、IRQ15=0x0800BA8C、IRQ19=0x0800BA44；其余 24 个指向 0x080030E6 的 `b .` | 向量表 + 反汇编 |

启动链：

```
0x080030D4 Reset_Handler → blx SystemInit(0x0800B964) → b 0x080030C0(_start)
0x080030C0 _start        → ldr sp,=0x20000CD0; bl init_array(0x08003548); bx main
0x08003548 init_array    → 遍历表 0x0800F3EC..0x0800F40C（16 B/项：a0,a1,a2,fn）
                           → 0x0800356C(解压 .data→0x20000100,0x13C)
                           → 0x0800C91C(memset .bss→0x2000023C,0xA94)
0x0800CF74 main          → 复制 48 向量→0x20000000；SYSCFG.CFGR1|=3
                         → 时钟/GPIO/ADC/DMA/EXTI/RTC/FLASH/LCD/CC2500 初始化
                         → 主循环（1 ms tick + 16 任务 + LCD 刷新）
```

---

## 3. 目录地图

| 路径 | 内容 | 规模 |
|---|---|---|
| `firmware/AD100pro_V1.2.bin` | 固件原始副本（只读参考） | 50248 B |
| `tools/disasm.py` | capstone/Cortex-M0 递归下降反汇编 + 函数发现 + 字面量解析 | 生成 out/ |
| `tools/periph.py` | 外设访问映射（寄存器级统计） | 862 个访问点 |
| `tools/boottest.py` | **最小可跑模拟器**（Lead 用于验证加载基址/外设桩；含 LCD 位流解码） | 验证工具 |
| `tools/lcd_decode.py` | LCD 位流解码 + PNG 渲染（独立于模拟器） | 验证工具 |
| `tools/hexdump.py`、`chk_vec.py`、`compare.py`、`xrefs_beyond.py`、`find_vectors.py` | 取证/诊断小工具 | — |
| `out/disasm.txt` | 全量注释反汇编清单（函数边界、调用者、字面量=符号/外设名） | 约 2.4 万行 |
| `out/functions.json` | 1199 个函数：地址/大小/基本块/调用者/被调用者 | — |
| `out/periph_summary.md`、`out/periph_map.json` | 外设寄存器访问矩阵 | — |
| `out/decompiled/` | **1199 个函数的伪反编译 C** + `index.tsv` + `CALLGRAPH.md` + `ANNOTATED_KEY_FUNCS.md` + `FINDINGS.md` + `ISSUES.md` + `SELFCHECK.md` | 2.7 MB |
| `out/ghidra/WHY_NOT.md` | 为什么最终没能跑真 Ghidra（zip 损坏 + 需 JDK25），含补跑步骤 | — |
| `out/emu_boot.log`、`out/emu_main_trace.log` | 模拟器启动日志 / main 调用序列 / MMIO 汇总 | 197 KB |
| `out/screen_emu*.png`、`out/fb_*.png`、`out/lcd*.png` | 模拟器解码出的真实屏幕画面 | 多张 |
| `emu/core.py` | **Unicorn 模拟核心**：Cortex-M0 MCLASS、128 KB flash、RAM、MMIO 钩子、外设桩、LCD 位流解码、断点、注入 | 57 KB |
| `emu/main.py` | CLI：`--run/--step/--bp/--regs/--mem/--trace/--dump/--screen/--inject/--calib` | — |
| `emu/server.py` | HTTP JSON 调试服务（127.0.0.1:8765，`/api/state|disasm|mem|cmd|screen|health`） | — |
| `emu/render_screen.py` | 从 RAM 帧缓冲 0x200004CC 直接导出 PNG | — |
| `emu/README.md` | 模拟器架构、外设语义、用法、已知限制 | — |
| `webui/` | 纯静态网页调试器（无反汇编无限滚动、寄存器、断点、内存、外设日志、OLED 画面；`?mock=1` 可脱机演示） | — |
| `src/recovered/boot_gpio.c` | 启动/时钟/GPIO/驱动还原 C（190 处地址注释，未知处 `/*?*/`） | 48 KB |
| `src/recovered/rf_ui.c` | CC2500 驱动/协议/LCD/按键/电池/主循环还原 C | — |
| `docs/00_LEAD_FINDINGS.md` | Lead 验证过的关键事实（加载基址、布局、启动链） | — |
| `docs/02_boot_and_drivers.md` | 启动/时钟/GPIO/驱动逆向（46 KB，15 项待确认） | — |
| `docs/03_rf_protocol.md` | 2.4G CC2500 协议逆向 | — |
| `docs/04_ui_lcd.md` | LCD/UI/按键/电池逆向 | — |
| `docs/INTERFACES.md` | 团队接口契约 | — |

---

## 4. 模拟器验证证据（实测，非推测）

| 项目 | 结果 |
|---|---|
| 启动 | 从 0x080030D4 起跑，25,000,000 条指令无崩溃 |
| 最终状态 | PC 落在 LCD 位操作内层（0x0800A4AC 附近），SP=0x20000C50，循环喂 IWDG（实测 34 次/5 M 指令） |
| 外设 | RCC/FLASH_IF/SYSCFG/EXTI/ADC/DMA/RTC/IWDG/TIM/GPIO/SysTick 全部有真实读写序列 |
| LCD | 解码出 12,544+ 字节显示数据、13+ 整帧刷新；与 RAM 帧缓冲 0x200004CC 内容一致（161 个非零字节），并渲染为 PNG |
| 速度 | 2.0–2.8 M 指令/秒（带完整 MMIO trace） |
| 启动日志 | `out/emu_boot.log` 前 2000 次 MMIO + 全量外设汇总 + 最终寄存器/RAM |

---

## 5. 关于"还原源码"的说明（重要）

- `out/decompiled/` 是**伪反编译 C**：逐条指令翻译 + 结构化 + 原始反汇编注释，**不是原始源码**，变量名/类型是推断的。
- `src/recovered/*.c` 是**人工逆向重建**的等价 C（关键函数为主），可读、可追溯地址，但不等同于厂商源码（编译器优化、内联、库函数、数据结构布局会丢失）。
- 二进制里**没有任何可读 ASCII 字符串**（无 printf/日志），UI 文本以点阵字模形式存在（9 字节/字形：宽度 + 8 字节位图，bank 从 0x0800Dxxx 起）。
- 未包含的部分：0x08000000–0x08002FFF 的 bootloader、0x0801F000 起的出厂标定数据（两者不在升级文件中，模拟器分别用合成桩/空白 0xFF 处理）。

## 6. 已知限制

1. 模拟器用指令计数近似时间（无周期精确），无 NVIC 抢占（固件启动/主循环/LCD 全走轮询，不影响已验证流程）。
2. RF 芯片是外部器件（CC2500 兼容），模拟器只实现 SPI 交互层，不做空口收发；可用 `--inject` 注入"收到一包"。
3. FLASH 擦写只记录不改内容；出厂标定区可用 `--calib synthetic` 生成 magic=0x5C 的合成记录。
4. LCD 画面为控制器 RAM 原样（面板相对控制器旋转 90° 安装），`emu/render_screen.py --rotate` 可切换方向。
5. 真 Ghidra 未跑成：下载的 Ghidra 12.2 包 sha256 不符、核心 jar 损坏且要求 JDK 25+；补跑步骤见 `out/ghidra/WHY_NOT.md`。

---

## 7. 网页模拟器（最终界面）

```powershell
# 后端（模拟器 + HTTP API，默认 127.0.0.1:8765）
..\..\tools\python\python.exe emu\server.py --port 8765

# 前端（静态站点；8080 若被系统保留可换 8088）
..\..\tools\python\python.exe -m http.server 8088 --directory webui
# 浏览器打开 http://127.0.0.1:8088/          ← 连接真实模拟器
# 或          http://127.0.0.1:8088/?mock=1  ← 无需模拟器，离线演示
```

界面功能：寄存器/标志位、PC 所在函数、反汇编（PC 高亮、点击设断点、跳转）、内存 hex+ASCII、外设访问日志（可按寄存器过滤）、调用栈、UART 控制台、LCD 屏幕画布；快捷键 `F5` 运行/暂停、`F10` 单步、`F8` 复位、`Ctrl+G` 跳转。
真实联调截图：`webui/screenshot-live-boot.png`、`webui/screenshot-live-menu.png`、`webui/screenshot-live-rf.png`（PC=0x0800A4BA、指令数 2,000,003、外设日志 200 条均为真实数据）；mock 截图：`webui/screenshot-mock.png`。
自测：`node webui\selftest.js`（26/26 通过）、`node webui\domtest.js`（40 条 DOM 断言通过）。

---

## 8. 交互式模拟（虚拟输入，新增）

预览地址（本机当前已在运行）：
- **live（真实模拟器）**：http://127.0.0.1:8088/?api=http://127.0.0.1:8775
- mock（脱机演示）：http://127.0.0.1:8088/?mock=1

> 端口说明：`8765` 被本机另一个项目（`devices\ad200pro` 的 `sim.server`）同时占用，Windows 下 SO_REUSEADDR 会让请求偶发挂起；本项目模拟器改用 **8775**，网页用 `?api=` 指定后端（页面默认仍是 8765，可用 `?api=` 覆盖）。

"虚拟输入"面板（底部中间）：
| 控件 | 命令 | 说明 |
|---|---|---|
| 电量滑块 2.6–4.2 V | `{"cmd":"battery","mv":N}` | 注入电池电压 → ADC 原始值/百分比回显 |
| 电源/模式/设置/上/下/备用 6 键 | `{"cmd":"key","key":"power","down":true}` / `{"cmd":"key_up",...}` | 按住/松开语义；电源键走 EXTI0→IRQ5，备用键走 EXTI6→IRQ7 |
| 「脉冲 80ms」勾选 | `{"cmd":"key_press","key":"power","ms":80}` | 自动按下+到时释放 |
| LCD 面板「保存画面」 | `{"cmd":"screen_save"}` | 后端把当前 128×64 帧缓冲存 PNG 并返回路径 |

实测（API 直连 8775）：`reset → run 3,000,000` 后 `battery 3000mV` 返回 `battery_pct:25`；`key_press power` 返回 `release_at` 且 `/api/state.inputs` 记录 `key_down/key_up` 事件；`screen_save` 生成 PNG（161 个非零字节）。

已知：当前固件在"无充电/无射频/无真实电池曲线"的状态下，屏幕内容为固定的 `SET`+功率条界面，按键/电量注入会进入固件事件链（RAM 事件状态变化），但暂未触发界面切换——下一步是实现真正的 IRQ 派发与虚拟 CC2500，让固件进入正常工作态。

自测：`node webui\selftest.js`（30 项）、`node webui\domtest.js`（47 条断言）。

### 8.1 进入主界面（交互模式，实测）

自然启动会长时间停留在"启动动画 + 显示刷新"循环（>59M 指令仍未到主调度器），交互演示需要先快进：

```powershell
# 方式 A：HTTP
POST http://127.0.0.1:8775/api/cmd  {"cmd":"reset"}
POST http://127.0.0.1:8775/api/cmd  {"cmd":"jump_main","warmup":1000000}   # 快进到 1ms 主调度器 0x0800D008
POST http://127.0.0.1:8775/api/cmd  {"cmd":"run","count":2000000}
POST http://127.0.0.1:8775/api/cmd  {"cmd":"key_press","key":"set","ms":150}
POST http://127.0.0.1:8775/api/cmd  {"cmd":"run","count":3000000}
POST http://127.0.0.1:8775/api/cmd  {"cmd":"screen_save","path":"out/ui.png"}

# 方式 B：CLI
..\..\tools\python\python.exe emu\main.py --jump-main --run 2000000 --screen out\lcd_main.png
..\..\tools\python\python.exe emu\main.py --jump-main --key set --run 2000000 --screen out\lcd_set.png
```

实测：`jump_main` 后 `run 2M` 的帧缓冲非零像素 **290**，画面即真机主界面（电池图标 / 1/1 / CH1 / M / 底部状态条，见 `out/ui_raw.png`、`out/ui_main.png`）；注入 `set` 键后再跑 3M，帧缓冲变为 **48** 非零像素（界面切换），证明**按键可以驱动真实 UI**。固件键位实测识别 k0→0x001…k9→0x200（k8=PD4 待定）。

### 8.2 默认体验（已实现）

`emu/server.py` 启动时自动执行 **reset → warmup(1M) → jump_main → settle**，所以打开页面即处于主循环：`pc=0x0800D008, taskPhase=scheduler, inMainLoop=true, virtualTicks>0, lcd.nonzero=290`（=`1/1`、`CH1`、`M`、电池图标的真机主界面）。
- `{"cmd":"boot_interactive"}` 可随时一键重新开机（reset→jump_main→settle）
- `/api/state` 新增：`pcName`、`taskPhase`(boot/scheduler/main_loop/fault)、`inMainLoop`、`virtualTicks`、`lcd.nonzero`
- 实测：boot_interactive → run 1.5M → 画面 A（nonzero 290）；注入按键后再跑 → 画面 B（nonzero 48，界面切换）→ **虚拟按键已能驱动固件真实 UI**
- 说明：目前按键触发的界面会进入固件的 "Error 3" 页面（疑似模拟器缺少充电/CC2500 状态导致），属下一步待修；LCD 像素方向（面板 180° 翻转/镜像）仍在最终确认。

---

## 9. 真实中断派发 + 虚拟 2.4G（目标 ②③ 达成，实测）

### 9.1 真实 IRQ 派发（emu/core.py）

Unicorn MCLASS 没有 NVIC 异常模型，emulator 用"手动异常进入 + 返回蹦床"等价实现：
- 蹦床 `0x0800F900 = b .`；进入 handler 时 `LR=0x0800F901`、从 **RAM 向量表 0x20000000** 取地址；`UC_HOOK_CODE` 命中蹦床后恢复 r0-r12/lr/pc/xpsr；同时只允许 1 个 ISR。
- 中断源：SysTick（异常 15，48000 指令）、TIM14 更新（异常 35，9600 指令 ≈5kHz）、EXTI 按键（power→IRQ5/异常21、key2→IRQ7/异常23）、CC2500 GDO0→EXTI15→IRQ7/异常23。
- 实测：`--run 5000000` → 0 fault、`irqCounts={15:104}`、RAM tick 0x20000104=104（真 ISR 里的 HAL_IncTick 递增）；注入电压源后 `irqCounts` 增加 21:1（EXTI0_1 handler 0x08005A78 真的执行）。

### 9.2 虚拟 CC2500（emu/cc2500.py）

按固件位操作 SPI 全量解码：CS=PC6、SCL=PB13、MOSI=PB12、MISO=PB14（下拉）、GDO0=PA15→EXTI15。支持寄存器/命令选通/PATABLE/TX FIFO/RX FIFO/MARCSTATE/状态字节、`inject_rx()` 注入与 GDO0 边沿、`transmit` 抓包。单元测试 `tools/test_cc2500.py` **7/7 通过**。
- `rf_init()`（调用固件 0x08004AF0）：**54 条寄存器写**（0x00=0x29,0x01=0x2E,0x02=0x06,…,0x06=0x04 PKTLEN=4）+ SRES/SCAL/flush/SIDLE/SRX → `state=RX, freq2=0x5E, PATABLE[0]=0xFE`。
- `rf_rx {hex}` 注入后：**IRQ7 计数 +1**、GDO0 回 0、RX FIFO 被固件 ISR 读空、包内容落到 RAM 0x20000214（如 `A9 50 B4 02` → `02 B4 50 A9`）。
- 协议语义实测：`D5 80/81 xx 00` 改信道（LCD 变化 18 字节）、`D5 09 00 00` 使 RAM 0x200001C4=100、`D5 19 00 00` 使 LCD 变化 338 字节（模式切换）、`A9 50 B4 02` 配对帧使 0x200001B0 变化。

### 9.3 网页演示流程

```powershell
..\..\tools\python\python.exe emu\server.py --port 8775     # 自动开机并 rf_init
..\..\tools\python\python.exe -m http.server 8088 --directory webui
# http://127.0.0.1:8088/?api=http://127.0.0.1:8775
# 页面：开机 → 射频初始化 → 注入 A9 50 B4 02 / D5 80 05 00 → Run，观察 LCD 变化与 IRQ/RF 计数
```

---

## 10. 本轮（②③④）验收证据汇总

| 目标 | 证据 |
|---|---|
| ② 真实 IRQ 派发 | `irqCounts={15:112, 35:562, 23:1}`（SysTick / TIM14 5kHz / CC2500 GDO0→EXTI15→IRQ7）；RAM tick 0x20000104 由固件 ISR 递增；power 键 → `21:1` 真进 EXTI0_1 handler |
| ③ 虚拟 CC2500 | `rf_init` 实测 54 条寄存器写 + SRES/SCAL/flush/SIDLE/SRX → `state=RX, freq2=0x5E, PKTLEN=4, PATABLE[0]=0xFE`；注入 `A9 50 B4 02` → IRQ7+1、GDO0 回 0、RX FIFO 被 0x0800AA74 读空、包落 RAM 0x20000214；单元测试 `tools/test_cc2500.py` 7/7 |
| ④ UI 随输入变化 | 主界面（1/1、CH1、M、电池）实测 nonzero=290；矩阵键 → 界面切换（nonzero 48）；RF `D5 80 05 00` → 帧缓冲变化 18 字节；`D5 19 00 00` → 338 字节；`D5 09 00 00` → RAM 0x200001C4=100 |
| ⑤ 预览/文档 | `http://127.0.0.1:8088/?api=http://127.0.0.1:8775`（已打开）；新截图 `webui/screenshot-live-rf.png`（2.4G 面板：state=RX、RX/TX 计数、IRQ 计数、射频初始化/注入按钮）；`docs/05_cc2500_emu_spec.md` |

### 网页演示步骤（30 秒）
1. 打开预览（自动进入主循环，LCD 显示 1/1 / CH1 / M / 电池）。
2. 右下「2.4G 射频」→ 点「射频初始化」（state 变 RX，freq2=0x5E）。
3. 点「注入配对包 A9 50 B4 02」或输入 `D5 80 05 00` 点「注入」→ 点 Run，观察 LCD 变化与 IRQ 计数增长（EXTI4_15/RF +1）。
4. 左「虚拟输入」可注入电量与按键，同样会驱动界面。
