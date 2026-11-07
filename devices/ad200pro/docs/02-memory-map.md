# AD200 Pro 固件：内存映射 / 外设 / 启动流程（定稿）

本文是静态逆向（`analysis/`、`docs/01`、`docs/03`~`06`）与模拟器实测（`sim/`）的交叉验证结果。
固件：`AD200pro_V1.60.bin`（46380 B，sha256 fc0f77f1…）。

---

## 1. 地址空间

### 1.1 镜像与 Flash
| 区域 | 地址 | 说明 |
|---|---|---|
| Bootloader | `0x08000000 – 0x08002FFF` | 12 KiB，**不在本固件文件中**；负责 USB 升级/跳转 APP。模拟器填 0xFF |
| APP 镜像 | `0x08003000 – 0x0800E52C` | 文件偏移 = 地址 − 0x08003000，共 0xB52C B |
| Flash 参数页 | `0x0801F000 – 0x0801F7FF` | 8×u32 参数块（结构见 §5），页大小 2 KiB |
| Flash 标志页 | `0x0801F800 …` | 魔数 `0x0000100E`（`0x0800AC24` 读取） |
| 芯片 Flash | 128 KiB（0x08000000–0x0801FFFF） | 模拟器必须映射到 0x08020000 才能跑通参数扫描 |

**基址 = 0x08003000 的判定**（三条独立证据）：
1. 向量表 48 项全部落在 `[0x08003000, 0x0800E52C)` 且映射到代码区；
2. `SysTick` 向量 `0x0800BC8D` → 文件偏移 `0x8C8C` 为合法代码（基址取 0x08000000 时落入数据区）；
3. 默认异常入口 `0x080030E7` → 偏移 `0xE6` 恰好是连续 `FE E7`（`b .`）桩。

### 1.2 SRAM（0x20000000 起，实际使用到 0x200008B0）
| 区域 | 地址 | 说明 |
|---|---|---|
| 运行时向量表 | `0x20000000 – 0x200000BF` | main 开头把 0x08003000 的 48 项复制过来；随后 `SYSCFG_CFGR1[1:0]=11`（SRAM 重映射），Cortex-M0 无 VTOR |
| `.data` | `0x20000100 – 0x20000227` | scatter 表第 1 条：src `0x0800E404`，len `0x128` |
| `.bss` | `0x20000228 – 0x200008AF` | scatter 表第 2 条：dst `0x20000228`，len `0x688`（清零） |
| 栈顶 | `0x200008B0` | 向量表[0] 与 startup 均写此值（栈向下生长） |

scatter 表位于 `0x0800E3E4`，两条 16 B 记录：`{src, dst, len, 拷贝函数}`
（`0x0800C720` 为 memcpy，`0x0800C730` 为 memset）。

### 1.3 关键 RAM 变量
| 地址 | 大小 | 语义（证据） |
|---|---|---|
| `0x20000100` | u32 | 1 ms 节拍计数；`SysTick_Handler`(0x0800BC8C)→`0x08007034` 自增；`get_tick()`=0x08006EAC 返回它 |
| `0x20000104` | u32 | SysTick 重装值缓存（RCC 时钟切换 `0x08007244` 按 `0x0800D00C` 表计算后写入） |
| `0x200001CE` | u8 | TIM14 分频计数（0x0800C1A8：+1，到 25 清零） |
| `0x200001CF` | u8 | **主循环唤醒标志**：TIM14 分频到 25 时置 1，main 轮询、消费后清 0 |
| `0x20000200..207` | u8[8] | RF 状态/包缓冲（0x20000206 起 4 B 为 TX 包体，`0xA9 …`） |
| `0x20000205` | u8 | RF bring-up 进行标志（`rf_tx_task` 置 1，就绪路径清 0） |
| `0x20000228` | — | `.bss` 起点（scatter 目标） |
| `0x20000424` | u16[16] | ADC DMA 采样缓冲（4 通道 × 4 次） |
| `0x20000464` | u32[8] | 内部 Flash 参数块 RAM 镜像（读 `0x0800590C` / 写 `0x08005974`） |
| `0x20000248` | struct | 队列/低功耗计数（`main` 用 0x08006FBC 初始化、0x08006F68 入队） |
| `0x20000318/354/3CC` | struct | 三个 TIM 中断的设备结构（`tim_irq_handler` 0x08007F08 的入参） |

---

## 2. 中断向量（Cortex-M0，48 项）

| 向量 | 处理函数 | 用途 |
|---|---|---|
| Reset | `0x080030D4` | 调 `SystemInit` → 0x080030C0（设 SP、scatter、跳 main） |
| NMI / HardFault | `0x0800A25C` / `0x08008200` | 异常处理 |
| SVC | `0x0800B81C` | — |
| PendSV | `0x0800A90C` | — |
| **SysTick** | `0x0800BC8C` | → `0x08007034` 让 0x20000100 +1；→ `0x08007CB4` |
| IRQ2 (RTC) | `0x0800AC0C` | RTC 秒中断 |
| **IRQ5 (EXTI0_1)** | `0x080053F0` | EXTI0 = PB0 触发输入 |
| IRQ6 (EXTI2_3) | `0x08005478` | — |
| **IRQ7 (EXTI4_15)** | `0x08005484` | EXTI5 = PA5（RF/触发同步输入） |
| IRQ9 (DMA1_CH1) | `0x080044D0` | ADC DMA 完成 |
| **IRQ13 (TIM1)** | `0x0800BE1C` | TIM1 超时/脉冲结束 |
| **IRQ15 (TIM2)** | `0x0800BE58` | TIM2 超时 |
| **IRQ19 (TIM14)** | `0x0800BDE0` | 主循环节拍：`0x0800C1A8` 分频 25 → 0x200001CF=1 |

> 注意：主循环唤醒来自 **TIM14**，SysTick 只提供 1 ms 计数。
> 向量表在 main 中复制到 SRAM 并 `MEM_MODE=SRAM`，因此模拟器异常入口从 `0x20000000+4×idx` 取。

---

## 3. 启动流程（实测时钟：80k 指令到主循环）

```
0x080030D4 reset_handler
  └─ 0x0800BD0C SystemInit             ; RCC：开 HSI、关 HSE/PLL、清分频
  └─ 0x080030C0 startup_c
       ├─ SP = 0x200008B0
       ├─ 0x08003548 scatter_load       ; .data 拷贝 + .bss 清零（表 0x0800E3E4）
       └─ 0x0800CDB0 main
0x0800CDB0 main
  ├─ 复制 48 项向量表 → 0x20000000
  ├─ SYSCFG_CFGR1 |= 3                 ; SRAM 重映射
  ├─ flash_acr_init / clock_init_task(0x0800BC98)：RCC 配置到 48 MHz，
  │    RCC_CFGR.SWS 轮询 + RTC/LSI 就绪
  ├─ board_init_inputs(0x08009758)：GPIO 输入/上拉/EXTI0(PB0)+EXTI5(PA5)，NVIC IRQ5/7
  ├─ dma1_clock_init / tim14_init(0x080099C0) / adc_dma_init(0x08009698)
  │    / rtc_init / key_scan / flash 参数加载
  ├─ 复制/校验参数：0x0800590C 读 0x0801F000 页 → 0x20000464；CRC16(0x08003144)
  ├─ 若参数无效：擦 0x0801F000 页并写默认 8×u32（含标志页魔数 0x100E）
  ├─ PRIMASK=1（临界区）
  ├─ rf_init(0x08004860)：CC2500 自检（读 0x0D==0x5C）＋配置
  ├─ rf_tx_task(0x0800AAF4)：PA5=1 时跳过 bring-up（真实上拉默认读 1）
  ├─ PRIMASK=0
  └─ while (1) {
         while (*(u8*)0x200001CF != 1) ;   // 等 TIM14 分频
         *(u8*)0x200001CF = 0;
         ui_key_scan_task / flash_power_task / adc_task / charge_monitor /
         measurement_task / display_task / rf_task / iwdg_refresh … (19 个任务)
     }
```

### 启动期必须正确响应的硬件语义（模拟器踩坑总结）
1. **RCC**：`HSION→HSIRDY`、`HSEON→HSERDY`、`PLLON→PLLRDY`、`LSION→CSR.LSIRDY`、`LSEON→BDCR.LSERDY` 粘连置位；写 `CFGR.SW` 后 `SWS` 立即回读为同值。否则 `0x0800742C`/`0x08007244` 返回错误 → `0x0800BCB4/0x0800BCCE` 的 `b .` 主动挂起。
2. **FLASH**：`KEYR` 解锁序列；`CR.STRT` 完成擦除/编程并置 `SR.EOP`；`SR.BSY=0`；`LOCK` 位按 **F1 布局 bit7**（不是 F0 的 bit31）。
3. **ADC**：`ADEN→ISR.ADRDY`；写 `CR.ADDIS` 必须清 `ADEN`；`CR.ADSTART` 单次转换完成后**自清**；`ADSTART` 触发 DMA 批量搬运。
   **数据寄存器是 `0x40012440`**（DMA `CPAR=0x40012440`，`CHSELR` 在 `0x40012428`，通道 3/10/11/12）。
4. **GPIO**：输入脚无外部驱动时按 `PUPDR` 返回上拉=1/下拉=0（PA5 上拉→1 让 RF bring-up 走 skip 分支，这是能进主循环的关键）。
5. **SysTick/TIM14**：SysTick 提供 1 ms 计数；TIM14（PSC=0x2F，ARR=0xC7）分频 25 次置主循环唤醒标志；NVIC `ISER` 是**写 1 置位**寄存器，需影子位图。
6. **异常返回**：handler `pop {…,pc}`/`bx lr` 返回 `EXC_RETURN` 时须手动弹帧；帧地址用入口保存的 `exc_frame_sp`，线程 SP 用入口保存的 `exc_thread_sp` 恢复（4 字节对齐偏移会破坏栈）。

---

## 4. 外设使用表（本固件只用到这些）

| 外设 | 基址 | 用法 |
|---|---|---|
| RCC | 0x40021000 | 时钟树、门控；`CFGR2/CFGR3/CR2` 也写 |
| FLASH | 0x40022000 | ACR 等待周期、参数页擦写、标志页 |
| GPIO A/B/C/D/F | 0x48000000 + 0x400×n | 无 AF 配置；全部软件位操作 |
| EXTI / SYSCFG | 0x40010400 / 0x40010000 | EXTI0(PB0)、EXTI5(PA5)；SRAM 重映射 |
| ADC | 0x40012400（DR=0x40，CHSELR=0x28） | CH3/CH10/CH11/CH12：电池/电容/温度/基准 |
| DMA1 | 0x40020000 | CH1：ADC→0x20000424（16×u16） |
| TIM1/2/14/17 | 0x40012C00/0x40000000/0x40002000/0x40014800 | 超时、触发脉宽、主循环节拍 |
| PWR | 0x40007000 | 低功耗/备份域 |
| IWDG | 0x40003000 | 喂狗（0xAAAA） |
| RTC | 0x40002800 | 秒计时（BDCR/LSE 或 LSI） |
| SCS | 0xE000E000 | SysTick、NVIC |
| **未使用** | — | 无 USART/SPI/I2C/USB 外设访问 → PC 升级在 bootloader |

### 4.1 2.4 GHz（CC2500 兼容）4 线软 SPI
| 信号 | 引脚 | 说明 |
|---|---|---|
| CSN | **PC6**（低有效） | `rf_select` 0x0800CBA8 / `rf_deselect` 0x0800CBC8 |
| SCK | **PB13** | `rf_spi_read_byte` 0x0800CAB0 |
| MOSI | **PB12** | `rf_spi_write_byte` 0x0800CAEC |
| MISO | **PB14** | 读 `GPIOB_IDR` bit14 |

- SPI mode 0、MSB first、8 位；读命令 = `addr|0x80`，写 = `addr`；burst 位 0x40。
- CC2500 strobe：0x30 SRES / 0x33 SCAL / 0x34 SRX / 0x35 STX / 0x36 SIDLE / 0x39 SFRX / 0x3A SFTX。
- 自检：`rf_init` 读 reg **0x0D** 期望 **0x5C**（FREQ2；FREQ2/1/0=0x5C/0xCE/0xC4 ≈ 2413 MHz）。
- TX 包：先 `0x7F`（burst 写 TX FIFO），再 4 字节 `{0xA9, k, 0xB4, 0x02}`；功率表在 `0x0800E370`（写 PATABLE 0x3E）。
- 模拟器：`sim/cc2500.py` 的行为级模型（寄存器回显 + 读返回），使自检通过。

### 4.2 GPIO 功能（详见 docs/05-pinmap.md）
- 3×3 按键矩阵：行 PB3/PB4/PB5（扫描时改输出拉低），列 PC10/PC11/PC12（输入上拉）；键码 = 3×row+col。
- 充电：**PB11** = 高压变换器使能；**PA7** 触发脉冲，**PA0** 使能。
- 输出：PA0/PA4/PA7/PA8/PC0/PC5/PC6/PF0/PF1；输入：PA2/PA5/PA15/PB0/PB14/PB15/PC4/PC10-12。

---

## 5. 内部 Flash 参数存储
- 参数页基址 `0x0801F000`，页大小 0x800；结构 `{type=0, start, pages=1}`（0x08006BCC 解锁/擦写）。
- 参数块 = **8 × u32**，RAM 镜像 `0x20000464`；读 `0x0800590C`，写 `0x08005974`；每 4 字节附 CRC16（`0x08003144`）。
- 标志页 `0x0801F800` 魔数 `0x0000100E`（0x0800AC24）。
- 模拟器：Flash 为可写后备内存，擦除填 0xFF、编程写半字并置 EOP，因此首次启动的“参数无效 → 擦页写默认值”流程可真实执行。

---

## 6. 数据区（`0x0800D000 – 0x0800E52B`）
| 地址 | 内容 |
|---|---|
| 0x0800D00C | AHB 分频→移位表（时钟切换算 SysTick 重装值） |
| 0x0800D05C / 0x0800D18A | u16 单调斜坡表（触发/充电时长） |
| 0x0800E000–0x0800E3E0 | 闪光功率/能量标定表（u16 0x2810…0x28A0 等） |
| 0x0800E370 | 2.4G 发射功率表（16 B，写 PATABLE） |
| 0x0800E3E4 | scatter 表（2×16 B） |
| 0x0800E404 | `.data` 初始镜像 |

## 7. 模拟器实现要点（`sim/`）
- **CPU**：Unicorn，`UC_MODE_THUMB|UC_MODE_MCLASS`，所有 `emu_start` 入口必须 `pc|1`（MCLASS 要求 Thumb 位）。
- **内存**：flash 0x08000000+128 K（镜像放 0x08003000）、RAM 0x20000000+8 K、外设 0x40000000+256 K、GPIO 0x48000000+8 K、SCS 0xE0000000+1 M。
- **MMIO**：寄存器后备内存 + 写钩子（顺序队列，避免位操作乱序）+ 读前刷新（GPIO IDR 按 MODER/PUPDR/ODR 计算）。
- **中断**：影子 PRIMASK/IPSR、NVIC 写 1 置位模型、异常帧入口/返回（记录 `exc_frame_sp`/`exc_thread_sp`）。
- **时间**：1 ms = 2000 条模拟指令（`systick_insn_per_ms`）；TIMx 周期按 `(PSC+1)(ARR+1)×2000/48000` 折算，保证主循环节拍。
- **加速**：`fast_delays` 把 `delay_us/delay_ms/delay_10ms/rcc_delay_short` 运行时替换为 `bx lr`，把全镜像唯一 `wfi` 改为 `nop`（仅运行时补丁，不修改样本文件）。
- **设备模型**：`sim/device.py` + `sim/cc2500.py`（电池/电容/温度/按键/RF/软 SPI 从机）。

## 8. 仍未确定（文档中已标 [推测]）
- ADC 通道 3/10/11/12 ↔ 电池/电容/温度/基准 的物理对应；
- 显示接口（本固件无并口/AF 常量，display_task 只做页渲染）；
- 充电拓扑细节（PB11 使能 + PA7 PWM 已确认）；
- CC2500 GDO/PA5 在真机上的实际时序。
