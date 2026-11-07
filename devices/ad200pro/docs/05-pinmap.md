# 05 – GPIO 引脚映射（AD200 Pro V1.60 固件）

> 依据：`firmware/AD200pro_V1.60.bin`（基址 0x08003000）反汇编。所有结论后附**证据地址**；
> 无法从代码唯一确定的用途标 **[推测]**。寄存器语义按 STM32F0 风格（MODER/PUPDR/OTYPER/BSRR/BRR/IDR）。
>
> 关键事实（先读这条）：**整个 APP 没有任何 AF（复用功能）配置**（无 AFRL/AFRH 写入、无 USART/SPI/I2C 寄存器访问），
> 因此**所有外设通信都是 GPIO 软件位操作**：2.4G 射频 = 4 线软 SPI，按键 = 3×3 矩阵扫描，
> 其余为电源/充电/触发/检测的直连 GPIO。

## 1. 引脚初始化总表

两个函数决定引脚初始状态，**后者覆盖前者**（执行顺序：`board_init_inputs` 在启动早期，
`gpio_init_all` 在主循环内 `system_reinit_task` 中再次执行，因此以 `gpio_init_all` 为准，
但 `board_init_inputs` 里对输入的 EXTI 配置与"输入模式"覆盖一直有效——见 §2）：

| 函数 | 地址 | 说明 |
|---|---|---|
| `gpio_init_all` | **0x08005544** | 一次性写 5 个端口的 MODER/PUPDR/ODR 常量（无循环、无表） |
| `board_init_inputs` | **0x08009758** | 使能 GPIOA/B/C/F 时钟，逐引脚 `gpio_write_pin`/`exti_config`，配置输入与 EXTI |
| `rf_bus_init` | **0x08008778** | 再配置 RF 软 SPI 的 4 根线（PC6/PC7/PB12/PB13/PB14） |

`gpio_init_all` 写入的常量（**证据 0x0800554e–0x08005598**）：

| 端口 | MODER | PUPDR | ODR |
|---|---|---|---|
| GPIOA | `0x3FD57155` | `0x40000400` | `0x00000001` |
| GPIOB | `0x15755015` | `0x00000540` | `0x00002BC0` |
| GPIOC | `0xFD555455` | `0x00000100` | `0x00000060` |
| GPIOD | `0x00000030` | `0x00000000` | `0x00000000` |
| GPIOF | `0x0000000D` | `0x00000000` | `0x00000000` |

解码（2 bit/引脚，`00=输入 01=输出 10=复用 11=模拟`；PUPDR `00=浮空 01=上拉 10=下拉`）：

| 引脚 | MODER 初值 | PUPDR | ODR | 后续覆盖（证据） | 用途推断 |
|---|---|---|---|---|---|
| PA0 | OUT | 浮空 | **1** | 多处写 1（0x08003f12 / 0x080053a8 / 0x08009e9c / 0x0800c38c…），`adc_task` 读回（0x080039b8） | 电源/触发使能类输出；`flash_trigger_pulse` 先置 1 再发 PA7 脉冲（0x08009e9c）→ **触发/IGBT 相关使能** [推测] |
| PA1 | OUT | 浮空 | 0 | `app_state_init` 置 1（0x0800bd72）、`board_init` 置 1（0x0800c286） | 上电自锁/复位释放 [推测] |
| PA2 | OUT | 浮空 | 0 | `board_init_inputs` 改 **输入+上拉**（cfg `0x8304`，0x0800982e）；`rf_status_update` 读（0x0800c21e） | **2.4G 芯片中断/状态输入** [推测] |
| PA3 | OUT | 浮空 | 0 | — | 保留/输出 |
| PA4 | OUT | 浮空 | 0 | `TIM1 IRQ13` 置 1（0x0800be42）、`board_init_inputs` 置 1（0x080097b8, mask 0x11） | 定时器相关输出（TIM1 中断中驱动）[推测] |
| PA5 | IN | **上拉** | 0 | EXTI 双边沿 + IMR（cfg `0x10210000`，0x08009840）；IRQ7 读（0x080054c2）；`rf_tx_task` 读（0x0800ab06） | **外部中断输入**（RF 或触发输入）[推测] |
| PA6 | AN | — | — | — | 模拟输入（ADC） |
| PA7 | OUT | 浮空 | 0 | `rf_rx_parse`/`rf_rx_handler`/`rf_rx_done` 写（0x0800b50e/0x0800c39e/0x0800c47a），`0x08009560` 写 0 | 射频收发切换/触发脉冲输出 [推测] |
| PA8 | OUT | 浮空 | 0 | `board_init_inputs` 改输入+上拉（0x0982e，mask 0x8304） | 输入 [推测] |
| PA9 | OUT | 浮空 | 0 | 同上 | 输入 [推测] |
| PA10 | OUT | 浮空 | 0 | — | 输出 |
| PA11–PA14 | AN | — | — | — | 模拟输入（ADC/比较器） |
| PA15 | IN | **上拉** | 0 | `board_init_inputs` cfg `0x8304` 输入+上拉（0x0800982e） | 输入/检测 [推测] |
| PB0 | OUT | 浮空 | 0 | `board_init_inputs` 改 **输入+上拉 + EXTI0 双边沿 + IMR**（cfg `0x10210000`，0x0800985c） | **EXTI0 输入**（IRQ5）→ 触发/同步信号 [推测] |
| PB1 | OUT | 浮空 | 0 | `0x0800389c` 写 0/1（0x080038ae）；`rf_tx_task` 写 1（0x0800ab34） | RF 发射使能/PA 控制 [推测] |
| PB2 | OUT | 浮空 | 0 | `board_init_inputs` 初值 0（0x080097d8 mask 0x3006） | 输出 |
| PB3 | **IN+上拉** | 上拉 | 0 | 按键矩阵**行驱动**（`ui_key_scan_task` 用 exti_config 临时改输出，0x08008842–0x08008860、0x0800892e–0x08008940） | **按键矩阵行 0** |
| PB4 | **IN+上拉** | 上拉 | 0 | 按键矩阵行 1（0x080088b0） | **按键矩阵行 1** |
| PB5 | **IN+上拉** | 上拉 | 0 | 按键矩阵行 2（0x08008920） | **按键矩阵行 2** |
| PB6 | OUT | 浮空 | 1 | `board_init_inputs` 置 1（0x080097e4 mask 0xBC0）；`settings_save` 置 1（0x0800cc74 mask 0x3C0） | 电源/充电控制 [推测] |
| PB7 | OUT | 浮空 | 1 | 同上；另有 `0x0800843c` 写 0（0x0800844e） | 同上 [推测] |
| PB8 | OUT | 浮空 | 1 | 同上 | 同上 [推测] |
| PB9 | OUT | 浮空 | 1 | 同上；`gpio_power_latch` 置 1（0x0800849c）；`power_off_sequence` 清 0（0x0800cf24） | **电源保持/关机控制** [推测] |
| PB10 | AN | — | — | — | 模拟 |
| PB11 | OUT | 浮空 | 1 | `board_init_inputs` 置 1（0x080097e4）；`charge_power_on/off` 用 mask **0x800**（0x0800cec4/0x0800cee8）；`power_off_sequence` 清 0 | **充电（高压变换器）使能** |
| PB12 | OUT | 浮空 | 0 | `rf_bus_init` 设输出；`rf_spi_write_byte` 写（BSRR/BRR） | **RF 软 SPI MOSI** |
| PB13 | OUT | 浮空 | 1 | `rf_bus_init` 设输出；`rf_spi_*` 翻转 | **RF 软 SPI SCK** |
| PB14 | IN（`board_init_inputs` 0x08009880 cfg 0xC000/mode0） | 浮空 | 0 | `rf_spi_*` 读 IDR bit14 | **RF 软 SPI MISO** |
| PB15 | IN（同上） | 浮空 | 0 | — | RF 芯片 GDO/中断 [推测] |
| PC0 | OUT | 浮空 | 0 | `measurement_task` 写 1（0x08005fcc） | 测量/充电控制 [推测] |
| PC1,PC2 | OUT | 浮空 | 0 | — | 输出（ADC 通道 11/12 见 §5） |
| PC3 | OUT | 浮空 | 0 | `board_init_inputs` 置 1（0x080097aa）；`status_led_task` 写 1（0x0800a2d4） | 状态 LED [推测] |
| PC4 | **IN+上拉** | 上拉 | 0 | `board_init_inputs` cfg `0x1C10`（0x0800984e） | 输入检测 [推测] |
| PC5 | OUT | 浮空 | **1** | `board_init_inputs` 先清 0（0x080097ce mask 0x3A0）；`0x0800ae40` 写 1（0x0800ae5a） | 输出（显示/充电时序）[推测] |
| PC6 | OUT | 浮空 | **1** | `rf_bus_init` 置 1（0x08008794）；`rf_select` 清 0（0x0800cbb0）/`rf_deselect` 置 1（0x0800cbd0） | **RF 片选 CSN（低有效）** |
| PC7 | OUT | 浮空 | 0 | `board_init_inputs` 清 0（0x080097ce）；`0x0800a5c8/0x0800a5e8` 写 0、`0x0800a608` 写 1 | 输出（RF 复位/唤醒等）[推测] |
| PC8,PC9 | OUT | 浮空 | 0 | 初始化清 0（0x080097ce） | 输出 |
| PC10 | OUT→**IN 读** | 浮空 | 0 | 初始化设输出（0x0800980c cfg 0x3E8/mode1）；按键扫描读 IDR（0x080088e2） | **按键矩阵列 0**（读时靠上拉/外部） |
| PC11 | 同上 | — | 0 | `ui_key_scan_task` 读（0x08008884/0x088f4/0x08968） | **按键矩阵列 1** |
| PC12 | 同上 | — | 0 | `ui_key_scan_task` 读（0x08008896/0x08906/0x0897a） | **按键矩阵列 2** |
| PC13–PC15 | AN | — | — | — | 模拟（ADC/参考） |
| PD2 | AN | — | — | — | 模拟 |
| PF0 | OUT | 浮空 | 0 | `board_init_inputs` 清 0（0x080097a0）；`0x08009c14`（TIM14 中断任务）置 1（0x08009c60） | 充电 PWM/反馈输出 [推测] |
| PF1 | AN（`gpio_init_all`）/OUT（`board_init_inputs` cfg{1,1}） | — | 0 | `board_init_inputs` 0x080097f8 | 触发/充电控制 [推测] |

> 说明：`gpio_init_all` 把 PF1 设为模拟，但 `board_init_inputs` 已把它设成输出（`exti_config(GPIOF,{mask=1,mode=1})`），
> 而 `gpio_init_all` 只在 `system_reinit_task` 的复位恢复路径执行（0x0800ba6e），两者不会互相破坏到影响功能。

## 2. EXTI 中断线配置

`exti_config(port, cfg)` @ **0x08006D3C** 的配置结构（寄存器级证据 0x08006d42–0x08006e74）：

```
struct exti_cfg {          // 4 words
  u32 pinmask;             // [0] 位掩码
  u32 mode;                // [4] bit[1:0]=MODER, bit4=OTYPER(开漏), bit16=IMR, bit17=EMR,
                           //     bit20=RTSR(上升沿), bit21=FTSR(下降沿)
  u32 pull;                // [8] PUPDR 2bit 值（按引脚移位写入）
  u32 af;                  // [0xc] AFR 值（本固件从未使用 AF）
};
```
函数依次写：`MODER`、`OTYPER`、`PUPDR`、`AFR`（仅 mode==2/0x12）、`RCC_APB2ENR.SYSCFGEN`、
`SYSCFG_EXTICR1..4`（端口选择）、`EXTI_IMR/EMR/RTSR/FTSR`。

`board_init_inputs`（**0x08009758**）中的 EXTI 相关配置（顺序即证据地址）：

| # | 端口 | pinmask | mode | pull | 证据 | 解释 |
|---|---|---|---|---|---|---|
| 1 | GPIOF | 0x0001 | 1（输出） | 0 | 0x080097f8 | PF1 输出 |
| 2 | GPIOC | 0x03E8 | 1（输出） | 0 | 0x0800980a | PC3,5,6,7,8,9 输出 |
| 3 | GPIOA | 0x0413 | 1（输出） | 0 | 0x0800981e | PA0,1,4,10 输出 |
| 4 | GPIOA | 0x8304 | 0（输入） | 1 上拉 | 0x0800982e | PA2,8,9,15 输入 |
| 5 | GPIOA | 0x0020 | `0x10210000` | 1 上拉 | 0x08009840 | **PA5 → EXTI5，双边沿+中断** |
| 6 | GPIOC | 0x1C10 | 0（输入） | 1 上拉 | 0x0800984e | PC4,10,11,12 输入 |
| 7 | GPIOB | 0x0001 | `0x10210000` | 1 上拉 | 0x0800985c | **PB0 → EXTI0，双边沿+中断** |
| 8 | GPIOB | 0x3BC6 | 1（输出） | 0 | 0x0800986e | PB1,2,6,7,8,9,11,12,13 输出 |
| 9 | GPIOB | 0xC000 | 0（输入） | 0 | 0x08009880 | PB14,15 输入（RF MISO/GDO） |
| 10 | GPIOB | 0x0038 | 0（输入） | 1 上拉 | 0x0800988e | PB3,4,5 输入（按键行） |

随后 `NVIC_SetPriority(5,0)+NVIC_EnableIRQ(5)`、`NVIC_SetPriority(7,0)+NVIC_EnableIRQ(7)`（0x08009892–0x080098b0）
→ 只用 **IRQ5(EXTI0_1)** 与 **IRQ7(EXTI4_15)**；IRQ6(EXTI2_3) 的向量存在但为空实现（0x08005478 只清挂起）。

其他运行期 EXTI 重配置函数：`charge_power_on` 0x0800CEC4（GPIOB mask 0x800）、`charge_power_off` 0x0800CEE8、
`exti_config_group` 0x0800CC14、`exti_enable_group` 0x0800BC08、`exti_disable_group` 0x0800BBE0、
`ui_key_scan_task` 每次扫描动态切换 PB3/4/5 输入↔输出（0x08008834–0x080089b0，共 12 次 `exti_config`）。

## 3. 按键矩阵（PRIMARY UI）

`ui_key_scan_task` @ **0x08008818**：

```
for row in {PB3, PB4, PB5}:              ; 行驱动，逐行拉低
    exti_config(GPIOB, {row, mode=1})    ; 行脚临时输出
    gpio_write_pin(GPIOB, row, 0)        ; BRR, 0x08008868 / 0x080088d4 / 0x08008946
    delay_us(3 / 3 / 10)                 ; 0x0800c768
    key |= read(PC10)<<0 | read(PC11)<<1 | read(PC12)<<2   ; 0x08008870/0x08008884/0x08008896 ...
```
9 个键位编码（bit = 3*row + col，行=0:PB3 / 1:PB4 / 2:PB5，列=0:PC10 / 1:PC11 / 2:PC12）：
`0x001 0x002 0x004 / 0x008 0x010 0x020 / 0x040 0x080 0x100`
—— 按键分派见 0x08008a40–0x08008ab4：`0x100→0x08004270`、`0x080→0x0800BC34`（置 0x20000150+0xC）、
`0x020→0x0800B90C`、`0x010→0x080042A0`、`0x004→0x08009B7C`、`0x002→0x0800C190`、`0x001→0x08008D08`；
另有组合键 `0x014`（=0x010|0x004，0x08008aaa）与 `0x101`（长按判定 0x08008a70）。
去抖：同一键连续 `0x32`（50）次扫描后才确认（0x080089ee）。

## 4. 2.4G 射频软 SPI（4 线）

| 信号 | 引脚 | 证据 |
|---|---|---|
| CSN（片选，低有效） | **PC6** | `rf_select` 0x0800CBA8：`gpio_write_pin(GPIOC,0x40,0)`；`rf_deselect` 0x0800CBC8：写 1 |
| SCK | **PB13** | `rf_spi_read_byte` 0x0800CAB0：mask 0x2000 → BSRR(0x0800cabc) / BRR(0x0800cad0) 交替 |
| MOSI | **PB12** | `rf_spi_write_byte` 0x0800CAEC：mask 0x1000 → 按数据位写 BSRR/BRR |
| MISO | **PB14** | 两个函数都读 `GPIOB_IDR` 并 `lsls #0x11; bmi`（掩码 bit14，0x0800cac6 / 0x0800cb1e） |
| 复位/唤醒 | PC7、PB15 | PC7 由 0x0800a5c8/0x0800a5e8/0x0800a608 翻转；PB15 为输入 [推测] |

时序（`rf_spi_write_byte`，0x0800cafc–0x0800cb2e）：
`SCK=0 → 放 MOSI(MSB) → delay → SCK=1 → delay → 采样 MISO → 下一位`，
即 **SPI mode 0、MSB first、8 位**；每字节后再采样一次 MISO（返回状态字节）。
`rf_spi_read_byte` 同样 8 次时钟，在 SCK 高电平期间采样 PB14。

## 5. 模拟输入（ADC）

- `adc_hw_init` @ **0x0800356C**：写 ADC_CR/CFGR1/CFGR2/SMPR/TR/CCR（0x0800356e–0x08003596），
  未写 CHSELR；`adc_dma_init` @ **0x08009698** 通过 `0x0800641C(ADC_struct, {ch}, …)` 逐个配置 **通道 3、10、11、12**
  （0x080096f0、0x08009704、0x08009718、0x0800971a）。
- 电池/电容/温度与通道的对应关系在固件里没有字符串可证，`adc_task` 只是把 4 个通道各做 4 点排序取中间平均
  后加偏移（`+0xA/+0xE`，0x08003638/0x080036ba）写入 RAM。**通道→物理量映射为 [推测]**：
  默认按 SIM-SPEC：CH3=电池电压、CH10=电容电压、CH11=温度、CH12=参考/备用
  （模拟引脚候选见 §1：PA6、PA11–PA14、PC13–PC15、PD2、PB10）。

## 6. 复位与电源

- 复位源判断：`boot_reason_check` @ **0x0800AC24** 读 `RCC_CSR` 并测试 bit26（PINRSTF 风格），
  再读 flash `0x0801F800` 魔数 `0x0000100E`（0x0800ac3e）。
- 电源保持：PB9（`gpio_power_latch` 0x0800848C 置 1；`power_off_sequence` 0x0800CF14 清 0）。
- 充电使能：PB11（`charge_power_on` 0x0800CEC4 / `charge_power_off` 0x0800CEE8，mask 0x800）。
- 触发脉冲：`flash_trigger_pulse` @ **0x08009E88**：PA0=1，然后 PA7=1、`delay_us(0x200001B2)`、PA7=0、`delay_us(3000)`，
  最后清 0x200001A9 并调 0x08008204。→ **PA7 为触发脉冲，PA0 为其使能** [推测]。

## 7. 未使用/未确认

- 没有 GPIOE 的任何初始化（0x08005544/0x08009758 均未配置 GPIOE）→ 该封装可能没有 GPIOE 或未用。
- 没有任何 AF（复用）配置 → 无硬件 USART/SPI/I2C；对 PC 的 USB 升级在 bootloader（0x08000000–0x08002FFF，不在本镜像）。
