# AD100 Pro 固件模拟器（Unicorn / Cortex-M0）

神牛 AD100 Pro 闪光灯固件 `firmware/AD100pro_V1.2.bin` 的 Cortex-M0 指令级模拟器，附带
CLI 调试器与 HTTP 状态服务。用于逆向验证、运行时行为观察与 LCD 画面还原。

## 1. 文件

| 文件 | 说明 |
|---|---|
| `emu/core.py` | 模拟核心：向量表/复位、内存映射、外设桩、MMIO trace、断点、LCD 解码 |
| `emu/main.py` | CLI 调试器（`--info/--run/--step/--bp/--regs/--mem/--trace/--dump/--screen/--inject/--bootlog`） |
| `emu/server.py` | HTTP JSON 服务（默认 `127.0.0.1:8765`，CORS 全开），供 `webui/` 使用 |
| `emu/screen.json` | LCD 帧缓冲契约（由 server 写出）：`{w,h,addr,format}` |
| `out/emu_boot.log` | 启动日志：前 2000 次 MMIO 访问 + 完整外设汇总 + 最终状态 |
| `out/emu_main_trace.log` | main 流程：函数入口序列、MMIO 汇总、外设/LCD 摘要 |
| `out/lcd_emu.png` | 5M 指令后由 LCD 引脚流解码出的 128×64 画面（2× 放大 PNG） |

## 2. 固件布局（实测结论）

```
flash 0x08000000 .. 0x08020000   128 KiB
  0x08000000 .. 0x08002FFF       bootloader（本镜像文件不含；模拟时用 app 向量表前 0xC0 字节 + 0xFF 填充）
  0x08003000 .. 0x0800F448       app 镜像本体（50248 字节，文件偏移 0 == 0x08003000）
  0x0800F448 .. 0x0801F000       未编程区（0xFF）
  0x0801F000 .. 0x08020000       出厂标定表（不在升级文件中；blank=0xFF，可选 synthetic 假数据）
RAM 0x20000000 .. 0x20002000     8 KiB SRAM
```

* app 向量表在镜像偏移 0（`0x08003000`）：`SP=0x20000CD0`，`Reset_Handler=0x080030D4`。
* `Reset_Handler` → `SystemInit(0x0800B964)` → 启动桩 `0x080030C0`（设 SP、跑 init_array
  `0x08003548`）→ `main(0x0800CF74)`。
* main 把 48 项向量表拷到 RAM `0x20000000`，并置 `SYSCFG.CFGR1 |= 3` 做 SRAM 向量表重映射。
  模拟器同时把 app 向量表写入 `0x08000000`（bootloader 视角）和 `0x00000000`（CPU 异常向量别名），
  MCLASS 模式下异常取向量不会跑飞。

## 3. Unicorn 取舍（重要）

* 优先使用 `Uc(UC_ARCH_ARM, UC_MODE_MCLASS)`（Cortex-M 模式，`CONTROL`/`PRIMASK` 等 M 类语义）。
  在 unicorn 2.1.4 上 MCLASS 可用，但有两个坑，已在 core.py 里打补丁规避：
  1. **`emu_start` 的 begin 地址必须带 Thumb 位**（`pc | 1`），否则 CPU 以 ARM 状态起步并报
     `UC_ERR_INSN_INVALID`。批处理循环每批都传 `pc | 1`。
  2. MCLASS 无完整异常模型（SysTick/EXTI 中断不会真正压栈进 handler）；本模拟器以
     "外设状态机 + COUNTFLAG/Pending 位" 的方式让固件轮询逻辑继续推进，而不是模拟 NVIC 抢占。
     若 `UC_MODE_MCLASS` 创建失败会自动回退 `UC_MODE_ARM | UC_MODE_THUMB`（同样需 `pc|1`）。
* 内存：ROM 64 KiB→已按 128 KiB 映射；RAM 8 KiB；MMIO 分三段按页映射：
  `0x40000000-0x40025000`、`0x48000000-0x48002000`、`0xE0000000-0xE000F000`。
* MMIO 读写用 `UC_HOOK_MEM_READ/WRITE` 精确挂在外设窗口上（不是全内存 hook）：
  读挂钩里把外设当前值写回映射页，CPU 的 load 即取到桩值（unicorn 2.1.4 实测可行）；
  写挂钩里更新外设状态机并记录 trace。
* 指令计数精确：`UC_HOOK_BLOCK` 在基本块边界累计上一块的指令数（capstone 按需解码并缓存块长），
  批处理停止点也用 `emu_stop()` 对齐到块边界，所以 `insnCount` 不会漂移。
* 性能：普通笔记本 5M 指令约 2.0–2.5 s（带完整 MMIO trace）；关 trace 更快。

## 4. 已实现外设语义

| 外设 | 基址 | 语义 |
|---|---|---|
| RCC | 0x40021000 | CR 读回写入值 \| `0x03020003`（HSI/HSE/PLL ready）；CFGR 的 SWS[3:2] 跟随 SW[1:0]；CSR/BDCR 的 LSI/LSE ready 置位；APBxENR 可写可读 |
| FLASH_IF | 0x40022000 | SR：BSY=0、EOP=1；写 SR 清标志；ACR 可写；KEYR 解锁序列记录；CR 写入记录 |
| GPIO A–F | 0x48000000+0x400*i | MODER/OTYPER/OSPEEDR/PUPDR/AFRL/AFRH 有状态；ODR/BSRR/BRR 联动（BSRR 高 16 位复位），IDR = ODR \| 注入输入；每次引脚变化进入 pin 事件（LCD 解码 & 按键注入） |
| SYSCFG | 0x40010000 | CFGR1/CFGR2/EXTICR 可写；CFGR1 写入作为事件记录（向量表重映射） |
| EXTI | 0x40010400 | IMR/EMR/RTSR/FTSR/SWIER/PR；`inject_exti(mask)` 注入 pending |
| TIM1/2/3/6/7/14/15/16/17 | 0x40012C00 等 | CR1.CEN 后 CNT 随指令数/预分频递增、到 ARR 回绕并置 SR.UIF；EGR.UG 重新装载并置 UIF；SR 写 0 清标志；PSC/ARR/CCR 可写 |
| SysTick | 0xE000E010 | CTRL/LOAD 写入生效；VAL 递减（LOAD>0 且 ENABLE），否则返回 `(insn//3)&0xFFFFFF` 自由计数（固件 ms 延时依赖它变化）；COUNTFLAG 在回绕后置位 |
| NVIC/SCB | 0xE000E000 段 | ISER/ICER/ISPR/ICPR 位图、IPR、CPUID(0x410FC241)、VTOR、AIRCR 等可读写不崩溃 |
| ADC | 0x40012400 | ISR 的 EOC/EOS 恒置位；DR 返回按 CFGR1.CHSELR 选中的注入值（默认 0x0C80≈3.2V 电池）；`inject_adc(value[,ch])` |
| USART1/2 | 0x40013800 / 0x40004400 | SR 的 TXE/TC 恒置位、RXNE 由注入队列决定；写 DR → 串口 TX 文本日志；读 DR → 注入字节 |
| SPI1 | 0x40013000 | SR 的 TXE/RXNE 置位；写 DR 记录 MOSI 字节；读 DR 返回注入 MISO（默认 0xFF） |
| I2C1 | 0x40005400 | SR1 的 TXE/BTF/ADDR/SB 等按 CR2 START/STOP 置位；写 DR 记录字节；读 DR 取注入队列 |
| IWDG | 0x40003000 | 写 KR=0xAAAA 记一次喂狗，0xCCCC 启动；永不超时复位（避免误判） |
| DMA1 | 0x40020000 | ISR/IFCR 与 7 通道 CCR/CNDTR/CPAR/CMAR 存储回读 |
| CRC | 0x40023000 | DR/IDR/CR/INIT/POL 存储回读 |
| RTC | 0x40002800 | ISR 读返回 0x7F（INITF/RSF/WUTWF/ALRxWF 就绪，**固件初始化与唤醒定时器轮询依赖此值**）；TR/DR/PRER/WUTR 可写可读 |
| PWR | 0x40007000 | CSR 的 PVDO/VOSF/VREFINTRDYF 恒置位 |

未知地址：读返回 0、写丢弃，trace 里标记为 `MMIO/UNKNOWN`；未映射访问记录 fault（PC/LR/SP/地址）。

## 5. LCD 解码（高价值产物）

从反汇编 `func_0800A480` 得到位操作总线：

| 信号 | 引脚 |
|---|---|
| SCL | GPIOB bit6（上升沿采样） |
| RS/A0 | GPIOB bit7（0=命令，1=数据） |
| CS | GPIOB bit9（低有效，一个字节一次 CS 脉冲） |
| SDA | GPIOA bit10（MSB first） |

控制器为 SH1106/SSD1306 类 128×64 单色。`LCDDecoder` 在 GPIO 写挂钩里按 CS/SCL 边沿还原字节流，
按 `0xB0|page`、`0x10|col_hi`、`0x00|col_lo`、数据字节=纵向 8 像素 的规则写 128×64 帧缓冲。
实测 5M 指令后：12866 字节（322 命令 / 12544 数据）、13 帧、初始化序列
`AE 02 10 40 B0 81 FF A1 A6 A8 3F AD 8B 33 C8 D3 00 D5 80 D9 1F DA 12 DB 30 A0 C0 AF`。

`/api/screen` 返回 `{w,h,format:"1bpp",hex,data,info}`；`emu/screen.json` 给出 `{w,h,addr,format}` 契约。

## 6. CLI 用法

```powershell
# 基本信息（向量表、布局、外设窗口、函数数量）
..\..\tools\python\python.exe emu\main.py --info

# 从复位跑 5,000,000 条指令并打印最终状态 / 外设汇总 / 最后 20 条 trace
..\..\tools\python\python.exe emu\main.py --run 5000000

# 渲染 LCD 画面（PNG，2x 放大）
..\..\tools\python\python.exe emu\main.py --run 5000000 --screen out\lcd_emu.png

# 断点 + 寄存器
..\..\tools\python\python.exe emu\main.py --bp 0x0800CF74 --run 2000000 --regs

# 单步
..\..\tools\python\python.exe emu\main.py --step 20 --regs

# 读内存
..\..\tools\python\python.exe emu\main.py --mem 0x20000000 64

# 只记录 RCC/GPIOB 的 MMIO 并导出
..\..\tools\python\python.exe emu\main.py --run 3000000 --trace RCC,GPIOB --dump out\emu_mmio.log

# 注入：串口接收字节 / 按键按下(PB0=0) / ADC 值 / SPI MISO / EXTI
..\..\tools\python\python.exe emu\main.py --run 1000000 --inject uart=4142 --inject key=PB0:0 --inject adc=3300

# 生成启动日志 + main 流程日志
..\..\tools\python\python.exe emu\main.py --run 5000000 --bootlog

# 可交互模式：快进到 1ms 主调度器（跳过 50M+ 指令的启动动画/电源握手）
..\..\tools\python\python.exe emu\main.py --jump-main --run 2000000 --screen out\lcd_main.png

# 虚拟输入：按住 up 键（矩阵键 k2）、电量 3100 mV
..\..\tools\python\python.exe emu\main.py --jump-main --run 1000000 --key up --battery 3100 --run 2000000
```

`--trace` 过滤项：外设名（`RCC`、`GPIOB`、`USART1`…）、寄存器名（`BSRR`）、
或十六进制地址/范围（`0x40021000`、`0x48000000-0x48002000`）。
`--on-fault stop|continue` 控制未映射数据访问时的行为（默认 stop 并记录）。
`--calib blank|synthetic` 控制 0x0801F000 出厂标定区。

## 7. HTTP 服务（webui 契约）

```powershell
..\..\tools\python\python.exe emu\server.py --port 8765
```

* `GET /api/state` → `{pc, sp, lr, xpsr, regs[16], insnCount, running, breakpoints[], log[], faults[], uart, uartLen, func{addr,name}, callstack[], lcd{w,h,format,page,col,frames}, stats, peripherals}`
* `GET /api/disasm?addr=0x080030D4&n=16` → `{addr, lines:[{addr,bytes,mnem,op}]}`
* `GET /api/mem?addr=0x20000000&len=64` → `{addr, hex, len, ascii}`
* `POST /api/cmd` `{cmd}`：
  * `run`（`count` 可选，缺省一直跑到 pause/断点/异常）
  * `pause`、`step`（`n`）、`step_over`、`reset`
  * `bp_add` / `bp_del`（`addr` 或 `addrs`）
  * `set_pc`（`addr`）、`set_reg`（`name`,`value`）、`set_mem`（`addr`,`hex`）
  * `inject`（`kind`=uart|adc|key|spi|exti）
* 可选扩展：`GET /api/screen`（`{w,h,format,addr,hex,data,info}`，数据即 RAM 0x200004CC 帧缓冲）、
  `GET /api/functions`（`[{addr,end,size,name}]`）、`GET /api/periph`、`GET /api/trace?n=100`、`GET /api/health`
* **虚拟输入命令**（`POST /api/cmd`）：
  * `{"cmd":"key","key":"power|mode|set|up|down|key2|k0..k9","down":true}`
  * `{"cmd":"key_up","key":"power"}`
  * `{"cmd":"key_press","key":"power","ms":80}`（返回 `release_at` 指令数）
  * `{"cmd":"battery","mv":3700}`（2600–4200，返回 `battery_mv`/`battery_pct`/`raw`）
  * `{"cmd":"adc","ch":10,"mv":1000}`
  * `{"cmd":"clear_inputs"}`
  * `{"cmd":"screen_save","path":"out/lcd_live.png"}`（返回 `ok,path,file,saved,bytes,frames,nonzero`）
  * `{"cmd":"jump_main","warmup":1000000}`（快进到主调度器；返回 `{ok,pc,insn,pcName,taskPhase,inputs}`）
  * `{"cmd":"rf_rx","hex":"d5010203"}`（注入 CC2500 收包 → GDO0 → EXTI15 → IRQ7；返回 `rf` 状态与 `irqCounts`）
  * `{"cmd":"rf_status"}` / `{"cmd":"irq_status"}`（RF 模型与真实 IRQ 派发状态）
  * `{"cmd":"boot_interactive","warmup":1000000}`（一步：reset → 快进主循环 → 跑 20 万条稳定 → 返回完整状态；webui「开机」按钮用这个）
  * `{"cmd":"reset"}` 后仍是复位态，需要再调 `boot_interactive`/`jump_main` 回到交互主循环
* `/api/state` 新增：
  * `inputs`：`{keys:{power,key2,mode,set,up,down,matrix,all}, pressed[], battery_mv, battery_pct, adc{}, last{}, events, press_ms_scale}`
  * `pcName`：当前 PC 所在函数名；`taskPhase`：`boot` / `scheduler` / `main_loop` / `fault`；
    `inMainLoop`：是否已进入 1 ms 主调度器；`virtualTicks`：虚拟 SysTick 计数
  * `lcd`：`{w,h,format:"1bpp",addr:0x200004CC,frames,nonzero,bytes_written,...}`
  * `lastScreenSave`：最近一次 `screen_save` 路径
  * 真实 IRQ：`realIrq`(bool)、`irqCounts`(按异常号：15=SysTick、35=TIM14、21=EXTI0_1、23=EXTI4_15…)、
    `irqLog`(最近 40 次进入记录)、`inIsr`、`virtualTicks`、`tim14Count`
  * RF：`rf`（`rf_status()`：state/gdo0/rx_fifo/tx_fifo/rx_count/tx_count/last_rx/last_tx/freq2/pktlen…）
* `server.py` 启动时**默认自动交互开机**（热启动 1M 指令后进入主循环，可用 `--no-auto-boot` 关闭，
  `--warmup N` 调整），因此页面打开即处于可交互状态。端口固定 **8775**（8765 被其他项目占用）。

### 虚拟输入语义

| 逻辑键 | 物理来源 | 说明 |
|---|---|---|
| `power` | PB0 / EXTI0 | EXTI 按键（低有效），按下同时置 EXTI PR 与 NVIC pending |
| `key2` | PA6 / EXTI6 | 备用键，同上 |
| `mode` `set` `up` `down` | 矩阵键 k0 k1 k2 k3 | 行 PB3/PB4/PB5 × 列 PC10/PC11/PC12/PC7/PD4，低有效 |
| `k0..k9` | 原始矩阵位 | 见 `core.VirtualInputs.MATRIX`；`up`==k2、`down`==k3 等为暂定映射 |
| `battery_mv` | ADC ch10/11/12 + RAM 0x2000020A/0x2000020C | 注入后固件电池状态机可读到（raw = mv*950/4200） |

按键注入通过 GPIO IDR 语义实现：行被拉低时对应列读 0；输入脚默认按 PUPDR 上拉/下拉返回电平。
`key_press` 的 ms 按 `MS_SCALE=8000` 指令/ms 折算释放时间（模拟器比真实 48 MHz 慢很多，便于 UI 响应）。

### 可交互模式（--jump-main / jump_main / boot_interactive / rf_init）

> 虚拟 CC2500 全链路（`emu/cc2500.py` + 真实 IRQ 派发）：
> `Emulator.call_function(addr, args=(), max_insns=3_000_000)` 用手动调用方案（保存 r0-r12/lr/pc/xpsr
> → LR=TRAMP → PC=addr|1 → 跑批次直到蹦床或超时 → 恢复上下文）同步调用固件函数；
> `Emulator.rf_init()` = `call_function(0x08004AF0)`（幂等），把 2.4G 驱动完整跑一遍；
> `Emulator.boot_interactive(with_rf=True)` = reset → 主调度器 → 小 settle → rf_init → settle。
> server 的 `{"cmd":"boot_interactive"}` 默认带 RF，也可 `{"cmd":"rf_init"}` 单独补做。
> 实测：rf_init 解码到 **54 条寄存器写**、命令 0x30/0x33/0x39/0x36/0x3A/0x34，状态 **RX**、
> freq2=0x5E、pktlen=4、PATABLE[0]=0xFE；注入 `A9 50 B4 02` 后 `irqCounts[23]` +1（固件
> 0x0800AA74 执行）、GDO0 回 0、RX FIFO 被固件读空；`boot_interactive` 后 insn≈3.38M、
> phase=scheduler、rf.state=RX、irqCounts={35:166,15:33}。


完整启动路径到 59M+ 指令仍在"启动动画 + 显示刷新"循环里（见限制 §9），为了交互演示，模拟器支持快进：
先跑 warmup（默认 1M）指令初始化外设，再直接把 CPU 置入 1 ms 主调度器入口 `0x0800D008`
（预置 r4=0、r5=&事件标志、r0=&UI 标志）。此后虚拟 SysTick 每 48000 指令置一次 tick 与事件标志，
主循环任务表（按键扫描 → UI → 显示刷新 → 电池 → 喂狗）周期性执行，虚拟输入被固件识别。

* 启动即交互：`server.py` 默认在启动时执行等价 `boot_interactive`，`/api/state` 直接显示
  `taskPhase`（scheduler/main_loop）、`inMainLoop=true`、`virtualTicks`。
* 一步开机命令：`POST /api/cmd {"cmd":"boot_interactive"}` → 内部 reset → warmup → 主循环 → settle，
  返回 `{ok, pc, insn, pcName, taskPhase, inputs, lcd}`。
* 只跳主循环：`{"cmd":"jump_main","warmup":1000000}` → 返回同样的状态字段（HTTP 200）。
  （历史 bug：旧版本用 `dict(ok=True, **res)` 造成 duplicate keyword `ok` → HTTP 500，现已修复。）
* CLI：`main.py --jump-main --run 2000000`；实测 `--key up` 使 PNG 从 1630B/675 lit 变为 1891B/831 lit。

实测（8775 最新代码）：
* 启动自动开机：`pc=0x0800D008`、`insn=2,200,028`、`taskPhase=scheduler`、`inMainLoop=true`、
  `virtualTicks=45`、`lcd.nonzero=290`、`inputs` 就绪。
* `jump_main` → **HTTP 200** `{"ok":true,"pc":0x0800D008,"insn":3200031,"pcName":"0x0800D008",
  "taskPhase":"scheduler","inputs":{...}}`
* `boot_interactive` → `{ok:true, pc:0x0800CABE, insn:1200017, phase:main_loop}`；
  `run 2M` → `frames=6, lcd.nonzero=290`；`screen_save` → out/live_8775.png（2920B）；
  `battery 3900` → 81%；`key_press set 80ms` → canonical k1，release_at 正确。



---

## 11. 真实 IRQ 派发（Unicorn MCLASS 无 NVIC 的补丁）

Unicorn 的 MCLASS 模式没有 NVIC 异常模型（写 NVIC ISER/ISPR 不会触发异常），因此 core.py
采用 Lead 在 `tools/boottest.py` 验证过的"手动异常进入/返回"方案：

1. 在 `0x0800F900` 写入 `fe e7`（`b .`）作为返回蹦床 `TRAMP`；进入 ISR 时把 `LR` 设为 `TRAMP|1`。
2. `UC_HOOK_CODE` 命中 `TRAMP` → `emu_stop()`；`run()` 恢复被中断的 r0-r12/lr/pc/xpsr，
   从原 PC 继续（若批处理在 ISR 中途停下，则从当前 PC 继续跑而不是跳回保存 PC）。
3. 同一时刻只允许一个 ISR（`_in_isr` 保护）；handler 从 **RAM 向量表 0x20000000+4*exc** 取，
   槽位为空或指向 `Default_Handler(0x080030E6)` 时跳过（不会卡死在 `b .`）。
4. 中断源与周期（默认开启，可用 `--no-real-irq` / `--no-tim14-irq` 或构造参数关闭）：
   * **SysTick**（异常 15，handler `0x0800B8E4`）：每 48000 指令一次；真实执行 HAL_IncTick，
     RAM `0x20000104` tick 由 handler 自己递增（不再假写）。
   * **TIM14 更新中断**（异常 35，handler `0x0800BA44`）：每 9600 指令一次（真实 48 MHz 下 5 kHz）。
     固件用它作为 UI/按键扫描时间基准；这是主调度器任务能周期运行的关键。
   * **EXTI 按键**：power=EXTI0→IRQ5(异常 21)，key2=EXTI6→IRQ7(异常 23)，按下时置 EXTI PR +
     NVIC pending 并排队派发；handler 读/清 `EXTI.PR`（我们的 EXTI 桩支持写 1 清位）。
   * **CC2500 GDO0=PA15→EXTI15→IRQ7(异常 23)**：`emu/cc2500.py` 的 `inject_rx()` 置 GDO0 后，
     派发器轮询 EXTI PR 位并调用 IRQ7 handler（实测 `0x0800AA74` RF 收包处理被执行）。
5. 队列中的 IRQ 若在 RAM 向量表就绪前到达（如复位即注入按键），会保留重试（上限 10 万次）。
6. 手动函数调用：`call_function()` 走同一蹦床返回机制，用于 `rf_init()` 调起 `0x08004AF0`；
   `boot_interactive()` 在 RF bring-up 前会先 settle 20 万条（让显示/调度状态机稳态），
   否则 RF 驱动会被拉回 IDLE。
7. 去重：`on_gpio_pins()` 中对 `cc2500.on_pins` 只调用一次（保留带 `_poll_cc2500` 的分支）。
8. `/api/state` 同时提供 `irqCounts` 与 `irq_counts`（兼容两种命名）、`rf`、`rfInitialized`、
   `tim14Count`、`inIsr`、`realIrq`、`virtualTicks`。

实测数字（8775 最新代码）：
* `main.py --run 5000000`：insn=5,000,006，**0 fault**，`realIrq=True counts={15:104}`，
  速度 ~2.5 M instr/s，LCD PNG 1484B/lit 698。
* `--jump-main --run 3000000`：`counts={35:416, 15:83}`，virtualTicks=83，tim14=416，帧缓冲 nonzero=290。
* 注入 `power`：`irqCounts` 增加 `21:1`（EXTI0_1 handler 0x08005A78 真的执行）；按键扫描读到 keyval。
* 注入 `rf_rx(d5 01 02 03 04 05)`：`irqCounts` 增加 `23:1`（EXTI4_15 handler 0x08005B0C），
  且 RF 包处理 `0x0800AA74` 执行、`rf.state=RX`。
* 已知限制：主循环 `while(0x200001E1!=1)` 的事件标志由 RF/命令分发路径置位；按键/定时器 ISR
  会跑任务的一部分，但完整 UI 刷新节奏仍以 RF 事件为主，故屏幕刷新帧数较少属正常。
