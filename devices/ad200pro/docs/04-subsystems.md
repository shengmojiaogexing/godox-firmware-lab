# 04 – 子系统行为分析（AD200 Pro V1.60 固件）

> 依据：`firmware/AD200pro_V1.60.bin`（Cortex‑M0，基址 **0x08003000**）静态反汇编。
> 每条结论给出证据地址；无法唯一确定处标 **[推测]**。寄存器语义按 STM32F0 风格。
> 符号名见 `analysis/symbols.json`，引脚见 `docs/05-pinmap.md`。

---

## 0. 总体结构一图流

```
复位 0x080030D4
 ├─ SystemInit 0x0800BD0C          清 PLL/HSE、开 HSI、复位 CFGR2/CFGR3/CR2
 └─ 0x080030C0 startup_c
      ├─ SP = 0x200008B0
      ├─ scatter_load 0x08003548   按表 0x0800E3E4 拷贝 .data / 清 .bss
      └─ main 0x0800CDB0
           ├─ 向量表复制 0x08003000 → 0x20000000，SYSCFG_CFGR1=3（MEM_MODE=SRAM）
           ├─ flash_acr_init 0x08007044
           ├─ clock_init_task 0x0800BC98（rcc_clock_config/rcc_clock_switch + systick_init）
           ├─ board_init_inputs 0x08009758   （GPIO 输入 + EXTI + NVIC5/7）
           ├─ dma1_clock_init 0x08009730
           ├─ tim14_init 0x080099C0
           ├─ adc_dma_init 0x08009698
           ├─ iwdg 0x080098E0 / RTC 0x0800993C / TIM1 0x080099EC / TIM3 0x08009A4C / TIM17 0x08009AAC
           ├─ iwdg_start 0x08006FBC（0xCCCC→0xAAAA）
           ├─ boot_reason_check 0x0800AC24
           ├─ settings_load_task 0x0800AC58
           ├─ app_state_init 0x0800BD68
           ├─ rf_tx_task 0x0800AAF4（一次）
           └─ while (1)  ← 0x200001CF 由 TIM14 中断分频置位
                 iwdg_refresh 0x08006F68
                 adc_task / key_scan_task / charge / trigger / display / RF / …
                 system_reinit_task 0x0800BA5C（关机/重初始化状态机）
```

**主循环节拍**：`TIM14_IRQHandler`(0x0800BDE0) → `0x0800C1A8` 把 `0x200001CE` 加 1，
每 **25** 次（0x19，0x0800c1b2）置 `0x200001CF = 1` 唤醒主循环（0x0800CDB0 轮询，0x0800ce46–0x0800ce52）。
SysTick 只提供 1 ms 时基（`0x20000100`），不参与唤醒 —— 证据：`0x200001CF` 的写者只有 `0x0800C1A8`
（`tools/an_dossier.py ram` 交叉引用）与 main 自己清零。

---

## 1. 启动链（startup）

| 步骤 | 地址 | 行为 |
|---|---|---|
| 复位向量 | 0x080030D4 | `ldr r0,=0x0800BD0D; blx r0`（SystemInit），再 `bx 0x080030C1` |
| `SystemInit` | 0x0800BD0C | `RCC_CR |= HSION`；`RCC_CFGR &= 0x08FFB80C`（清 SW/HPRE/PPRE/PLL 位）；`RCC_CR &= 0xFEF6FFFF`（关 HSE/PLL/CSS）；清 `CFGR2/CFGR3`；`RCC_CR2` 清 bit0；`RCC_CIR = 0`（清中断标志） |
| `startup_c` | 0x080030C0 | 从 `0x200008B0` 设置 SP，调用 scatter_load，跳 main |
| `scatter_load` | 0x08003548 | 遍历 **0x0800E3E4** 起的 16 字节记录表 |
| `main` | 0x0800CDB0 | 复制 48 字向量表到 RAM（0x0800cdb2–0x0800cdc4），`SYSCFG_CFGR1 |= 3` |

`scatter_load` 记录格式（**0x08003548** 反汇编）：

```c
struct scatter_rec {            // 4 x u32, 16 bytes
    const void *src;            // [0] flash 源
    void       *dst;            // [4] RAM 目的
    u32         len;            // [8] 字节数
    void      (*fn)(const void*, void*, u32);  // [0xc] 带 Thumb 位（函数内 orr #1）
};
// 0x0800E3E4: {0x0800E404, 0x20000100, 0x00000128, 0x0800C720}   ← memcpy(.data)
// 0x0800E3F4: {0x0800E52C, 0x20000228, 0x00000688, 0x0800C730}   ← memset(0,.bss)
```
因此：**.data = RAM 0x20000100–0x20000227（0x128 B），.bss = 0x20000228–0x200008AF（0x688 B），栈顶 0x200008B0**。
`0x0800C720` / `0x0800C730` 是 memcpy / memset 的极简实现（在 0x0800C71C 区域，未被 CFG 分析收录）。

---

## 2. 时钟（RCC）

| 函数 | 地址 | 作用 |
|---|---|---|
| `SystemInit` | 0x0800BD0C | 复位时钟树到 HSI |
| `rcc_clock_config` | 0x0800742C | 451 条：PWR_CR / FLASH_ACR / RCC_CR / RCC_CFGR 的完整配置（HSE+PLL）[推测] |
| `rcc_enable_hse_pll` | 0x08007124 | RCC_CR/CFGR + PWR_CR 使能 HSE/PLL [推测] |
| `rcc_clock_switch` | **0x08007244** | 真正切换 SYSCLK：先按目标设置 FLASH_ACR 延时，再改 CFGR 的 HPRE/PPRE、写 SW，轮询 SWS（超时 5000 ms = 0x1388，0x080072d4），最后算 SysTick 重装值 |
| `rcc_get_clock_freq` | 0x080073A4 | 由 CFGR 反推 Hz |
| `clock_init_task` | **0x0800BC98** | 组装两个配置结构（sp、sp+0x34）依次调用 `rcc_clock_config`、`rcc_clock_switch`，再 `systick_init` |

`rcc_clock_switch(cfg, latency)` 的 cfg 结构（从 0x08007244 反汇编）：

```c
struct rcc_cfg {
    u8  flags;      // bit1: 设置 HPRE,  bit2: 设置 PPRE,  bit7: 使用 CFGR2/CR2 分支
    u8  target;     // 1/2/3: 切换目标（对应 HSE / PLL / 第三时钟源）
    u16 pad;
    u32 hpre;       // [8]  AHB 预分频值（写 CFGR[7:4]）
    u32 ppre;       // [0xc] APB 预分频值（写 CFGR[10:8]）
    ...
};
```

**SysTick 重装值**：`rcc_clock_switch` 在末尾（0x08007366–0x08007378）：
```
freq = rcc_get_clock_freq();
hpre_shift = table_0x0800D00C[ (RCC_CFGR>>4) & 0xF ];   // AHB 分频的移位量
*(u32*)0x20000104 = freq >> hpre_shift;                  // 1 ms 重装值
```
`0x0800D00C` 表（16 字节）实测为 `00 00 00 00 | 00 00 00 00 | 01 02 03 04 | 06 07 08 09`
→ 对应 HPRE=0..7 → 移 0 位、8→1、9→2、A→3、B→4、C→6、D→7、E→8、F→9（即 AHB /1,/2,/4,/8,/16,/64,/128,/256,/512）。

`systick_init` @ **0x08007C84**：`SYSTICK_RVR = reload-1`；`0x0800A310`(SCB_SHPR2) 设优先级；
`SYSTICK_CVR = 0`；`SYSTICK_CSR = 7`（CLKSOURCE|TICKINT|ENABLE）。若 reload > 0xFFFFFF 返回错误。

**SysTick 中断**：0x0800BC8C → `0x08007034`（`++*(u32*)0x20000100`）+ `0x08007CB4`（调用 0x08007C80 = `bx lr` 空桩）。
即 SysTick 仅做 32 位 ms 计数器；`get_tick()` @ 0x08006EAC 返回它。

---

## 3. ADC 采样 + DMA

| 函数 | 地址 | 作用 |
|---|---|---|
| `adc_hw_init` | 0x0800356C | 写 ADC_CR(0x40012408) / CFGR1 / CFGR2 / SMPR / TR / **CCR(0x40012708)** |
| `adc_dma_init` | **0x08009698** | 构建 ADC 设备结构 `0x2000025C = {base=0x40012400, …}`；调 `adc_dma_config` 配置 **CH3 / CH10 / CH11 / CH12**（0x08009706/0x0800971a 等）；每个失败分支调用 Error_Handler |
| `adc_dma_config` | 0x0800641C | 140 条：ADC 通道 → DMA 通道的寄存器配置 |
| `adc_dma_enable` | 0x08006550 | 启动转换（181 条） |
| `dma1_ch1_task` | 0x080068A4 | DMA1 CH1 传输完成/错误处理（320 条，IRQ9 入口 0x080044D0） |
| `adc_task` | **0x08003598** | 主循环任务：对每个通道取 4 个样本做**排序 + 中间两个平均**（0x080035cc–0x080035e2 的冒泡），再 `+0xE`/`+0xA` 偏移（0x080035f6/0x08003638/0x0800367a/0x080036bc），存回 RAM |

`adc_task` 的 4 组输入取 `ADC 设备结构 + 0xC/+0x1C/+0x2C/+0x3C`（4 个通道的结果缓存），
离线地址 `0x2000025C+…`。结论：**4 通道 × 4 采样中值滤波**，典型用于电池电压、电容电压、温度与参考。
通道→物理量对应关系无字符串/常量可证 **[推测]**（见 docs/05-pinmap.md §5）。

DMA 只有 **CH1** 被初始化（`dma1_clock_init` 0x08009730 只开 DMA 时钟 + IRQ9），
`dma1_ch1_task` 处理 IFCR 标志并回填状态 —— 与 "ADC 连续采样 → 内存缓冲" 一致。

---

## 4. 电容充电（高压变换器）

- 充电使能脚：**PB11**（`charge_power_on` 0x0800CEC4，`exti_config(GPIOB,{mask=0x800…})`；
  `charge_power_off` 0x0800CEE8）。
- 充电控制状态机：`charge_enable_task` @ **0x0800BA00**（主循环）：
  ```
  if ((state_0x2000015D & 3) == 1 && setting_0x20000419 != 0) {
      if (charge_state_0x200001CE+7 == 0) { charge_power_on();  charge_state = 1; }
  } else if (charge_state == 1) { charge_power_off(); charge_state = 0; }
  ```
- `charge_monitor` @ **0x08005A04**（主循环）：以 0x78 / 0xF0 为阈值比较 ADC 值（0x08005a…，
  `cmp #0x78`,`cmp #0xf0`），调用 `charge_voltage_convert` 0x080060AC。→ 电容电压监测。
- `flash_charge_state_machine` @ **0x080045AC**（293 条，主循环 `flash_power_task` 0x08004500 内）：
  引用功率表 `0x0800E314`，按状态机 0x0–0x32 的多个 case 控制充/放电与保护 [推测]。
- `flash_trigger_state_machine` @ **0x08008504**（298 条）：同样引用 0x0800E314 表，
  控制触发前后的功率/脉宽 [推测]。
- `measurement_task` @ 0x08005F6C：把 ADC 结果经 `0x08005C30`（265 条大数学）换算，引用表 0x0800D412。

充电拓扑：软件 PWM —— `rf_rx_done`…实际为 **充电泵驱动函数 0x0800B4EC**：对 **PA7** 输出 60 个
高/低脉冲（半周期 0x50/0x32/0x96 us 三档，0x0800b512/0x0800b54a/0x0800b57c），每次循环喂狗。
该函数被 EXTI/充电路径（0x0800A954）调用，故 PA7 = 高压充电变换器开关 **[推测，基于脉冲模式]**。
（另一条 PA7 用法 `flash_trigger_pulse` 0x08009E88 是单脉冲、长度取自 0x200001B2。）

---

## 5. 闪光触发（IGBT / 灯管）

中断入口：

| 中断 | handler | 触发链 |
|---|---|---|
| IRQ5 EXTI0_1（PB0） | 0x080053F0 | 读 EXTI_PR bit0；若 `(state&3)==1`：按 `setting[1]`（0x20000419）分支：1→`flash_fire_sequence` 0x08005AE8；2→若重触发计时 0x200001B8==0 则置 0x2710（10000）再 `flash_fire_sequence`；最后清 PR |
| IRQ7 EXTI4_15（PA5 / EXTI15） | 0x08005484 | PR bit15 → 0x0800A954（RF 完成路径）；PR bit5(PA5) → 延时 50µs 后按 `setting[3]` + 0x200001A1 调 0x08005AE8；结尾清 PR bit5/bit4 |
| IRQ13 TIM1 | 0x0800BE1C | 若未在关机且 0x20000108 置位 → `0x08007F08` 通用 TIM 处理；否则 `gpio_write_pin(GPIOA,0x10,1)`（PA4）+ `0x08009550` |
| IRQ15 TIM2 | 0x0800BE58 | 若未关机且 0x20000109==0 → `rf_rx_timeout_task` 0x0800B888；再 `0x08007F08`(TIM2 结构 0x20000354) |
| IRQ19 TIM14 | 0x0800BDE0 | `0x0800C1A8`（见 §1）；未关机时 `0x08009C14`(PF0 充电反馈)、`0x08004818`、`0x0800AE40`(PC5)；`0x200001B8` 重触发倒计时 |

`flash_fire_sequence` @ **0x08005AE8**（116 条，核心）：

```c
void flash_fire_sequence(void) {
    state1[0x12][6] = 0;                     // 0x20000122+6
    if (*(u16*)0x20000148) { flash_abort_retrigger(); return; }   // 0x08005A58
    st = *(u8*)0x2000015D;
    if (st & 3) {                            // 正常模式
        if (st & 3) == 1 && setting[4] /*0x2000041C*/ && !flag_0x20000197) {
            burst_cnt++;                     // 0x2000015E
            …
        }
        if (setting[3] /*0x2000041B*/ && !flag_0x20000197 && !rf_done_0x200001A1) {
            u16 v = table_0x0800D05C[ rf_done_flag ];   // 0x0800d05c
            *(u32*)0x20000180 = v * 50;                  // 触发/充电时长
            if (v >= 0x32) state_0x20000150 = 1;
            delay_us(1000);
        }
        if (flag_0x200001A6 == 1 && !flag_0x200001FB) {
            if ((st&3)==1) flash_arm_b();   // 0x08009560  PA0=1, PA7=0
            else           flash_arm_a();   // 0x08009E14  PA0=1
        }
    }
    flag_0x2000016B = 0;
}
```
随后 `flash_trigger_pulse` @ **0x08009E88** 才真正给出脉冲：`PA0=1 → PA7=1 → delay_us(0x200001B2) → PA7=0
→ delay_us(3000) → 0x200001A9=0 → 0x08008204`（收尾）。`0x200001B2` 由 0x08003CBC 区段的
ADC→功率换算得到（`0x0800B134` 比例运算，0x08003cbe）。

**结论**：闪光由 PA0（使能）+ PA7（脉宽可调的驱动脉冲）完成，脉宽来自功率表/ADC 换算，
TIM2 中断负责时序与超时，EXTI0/EXTI5 是触发输入源。 **[触发/IGBT 命名基于此证据；具体灯管/IGBT 拓扑为推测]**

---

## 6. 2.4G 无线（软 SPI + CC2500 兼容收发器）

### 6.1 总线

`rf_bus_init` @ **0x08008778**：PC6（CSN）输出=1，PC7=1，PB13(SCK)=0，PB12(MOSI)=0，
PB14/PB15 输入；`board_init_inputs` 0x08009758 亦已配好（PB12/13 输出、PB14/15 输入）。
详见 `docs/05-pinmap.md` §4。位操作原语见 §6.2。

### 6.2 软 SPI 原语（全部为 GPIO 位操作，无 SPI 外设）

| 函数 | 地址 | 作用 |
|---|---|---|
| `rf_select` | 0x0800CBA8 | PC6=0（CSN 选中），随后轮询 PB14=1（等待芯片就绪） |
| `rf_deselect` | 0x0800CBC8 | PC6=1 |
| `rf_spi_write_byte` | **0x0800CAEC** | 8 次：SCK=0 → 输出 MSB 到 PB12 → SCK=1 → 采样 PB14 → 左移；返回状态字节 |
| `rf_spi_read_byte` | **0x0800CAB0** | 8 次：SCK=1 → 采样 PB14 → SCK=0；拼字节 |
| `rf_spi_transfer` | 0x0800CB38 | select → 写 1 字节命令 → 读 len 字节到 buf → deselect |
| `rf_write_cmd` | 0x0800CB64 | select → 写 1 字节（strobe）→ deselect |
| `rf_write_cmd_buf` | 0x0800CB7C | select → 写命令 → 写 len 字节数据 → deselect |
| `rf_write_reg8` | 0x0800CAA2 | `rf_write_cmd_buf(addr, &value, 1)` |
| `rf_read_reg8` | 0x0800CA16 | 命令 = `addr \| 0x80`，读 1 字节 |

**协议判定**：读地址用 `0x80|addr`、写地址直接用 `addr`、另有 strobe 常量
`0x30/0x33/0x34/0x36/0x39/0x3A` —— 与 **CC2500/CC1101 系列 SPI 命令集完全一致**
（`0x30=SRES, 0x33=SCAL, 0x34=SRX, 0x36=SIDLE, 0x39=SFRX, 0x3A=SFTX`，读/写/突发位 0x80/0x40/0xC0）。
**[推测：芯片为 CC2500 兼容 2.4G 收发器，而非 nRF24：nRF 的读命令是 0x00|addr，与此不符]**

### 6.3 芯片配置

`rf_chip_config` @ **0x0800C858**：逐寄存器写 0x00–0x2E，随后 `rf_write_cmd_buf(0x7E, buf,1)`、
`SFRX(0x39)`？实际为 `0x33(SCAL)` → delay 1000 → `0x39(SFRX)` → delay 1000。写入值（节选）：

| reg | 值 | CC2500 含义 |
|---|---|---|
| 0x00 | 0x29 | IOCFG2 |
| 0x01 | 0x2E | IOCFG1 |
| 0x02 | 0x06 | IOCFG0 |
| 0x03 | 0x01 | FIFOTHR |
| 0x04/0x05 | 0xC3/0x68 | **SYNC1/SYNC0 = 0xC368** |
| 0x06 | 0x04 | PKTLEN = 4 |
| 0x08 | 0x00 | PKTCTRL0（固定长度、无 CRC/白化） |
| 0x0B | 0x07 | FSCTRL1（中频） |
| **0x0D/0x0E/0x0F** | **0x5C/0xCE/0xC4** | **FREQ2/1/0 → 26 MHz 晶振时 ≈ 2413 MHz** |
| 0x10–0x14 | 0x1D/0x3B/0x73/0x22/0xF8 | MDMCFG4..0（0x12=0x73 → MSK, 30/32 sync） |
| 0x17 | 0x30 | MCSM1 |
| 0x19–0x1D | 0x1D/0x1C/0xC7/0x00/0xB2 | FOCCFG/BSCFG/AGCCTRL |
| 0x20–0x26 | 0xF8/0xB6/0x10/0xEA/0x0A/0x00/0x11 | FSCAL / TEST |
| 0x27 | 0x41 | PA_TABLE0 |
| 0x28/0x29/0x2A | 0x00/0x59/0x7F | 扩展/功率条目 |

`rf_init` @ **0x08004860**：`rf_bus_init()` → delay 20000 → 最多 10 次：
`rf_chip_reset()`（0x0800CA2C：CSN 脉冲 + 等 MISO + SRES 0x30）→ `rf_chip_config()` →
读 reg 0x0D/0x0E（0x0800CA16）存入 `0x20000206/07`，要求 reg0x0D 读到 **0x5C**（写回校验值，即 SPI 连通自检）；
成功后写 **PATABLE**：`rf_write_reg8(0x3E, table_0x0800E370[channel])`（0x080048ac）
—— `0x0800E370` 的 16 字节 `00 50 44 C0 84 81 46 93 55 8D C6 69 6E 7F A9 BB` 即 **发射功率表**，
索引取 `0x20000200`（对应 settings[channel]）。随后 strobe `0x36(SIDLE)`、`0x33(SCAL)`、`0x37?`、
delay、`0x33`、`0x3E` 等。

### 6.4 收发与状态

| 函数 | 地址 | 作用 |
|---|---|---|
| `rf_apply_settings` | 0x0800AA8C | 按 `0x20000165`（RF 使能）分支执行 strobe 序列 `0x36/0x33/0x3A/0x34`，写入参数 |
| `rf_poll_settings` | 0x0800B5B0 | 主循环：`0x20000203` 与 `0x2000016C` 不同 → 重新 `rf_apply_settings` |
| `rf_set_power_level` | 0x0800B9AC | `udivmod32(50000, level+1)` → 写 reg4/reg5 的 16 位 → strobe 0x33 → delay |
| `rf_write_pair` | 0x0800B96C | 写两个连续寄存器 |
| `rf_rx_timeout_task` | 0x0800B888 | TIM2 ISR：维护 `0x2000019B/0x200001A1/0x20000199` 等接收状态；超时 → `rf_rx_state_reset` |
| `rf_rx_process` / `rf_rx_parse` / `rf_rx_handler` | 0x0800C554 / 0x0800C354 / 0x0800C4D4 | 收包解析（141 条状态机，含 PA7/PC 输出） |
| `rf_tx_task` | 0x0800AAF4 | 主循环：读 PA5 + `0x20000165`，组包并 `rf_write_cmd_buf` 发送 |
| `rf_wakeup_sequence` | 0x0800C680 | 超时后 strobe `0x36/0x33/0x3A/0x34` 重新唤醒 |
| `flash_trigger_pulse` 路径 | 0x08009E88 | RF 收到触发包后由 TIM2 路径调用（见 §5） |

接收定时：EXTI15/PB15 与 TIM2 中断配合（`0x200001B2`/`0x20000199` 计数），
实际射频接收为 **轮询状态寄存器 + GPIO 中断**，非硬件 SPI-DMA。

---

## 7. UI：按键矩阵 + 状态机

- 3×3 矩阵扫描：`ui_key_scan_task` @ **0x08008818**（每轮主循环一次，行 PB3/4/5 拉低、读 PC10/11/12），
  去抖/长按/组合键与 7 个按键处理函数见 `docs/05-pinmap.md` §3。
- UI 状态结构基址 **0x20000150**（约 0x40 字节）：`+4` 按键按下标志、`+5` 扫描计数、
  `+6/+7` 长按计数、`+0xC/+0xD/+0xE` 模式、`+0x17..+0x1E` 标志位、`+0x26` 上一次按键、`+0x2A` 计时器。
  证据：`ui_key_scan_task` 0x08008824–0x08008ab4、`ui_menu_task` 0x0800C2D8。
- `ui_factory_reset_task` 0x08009B38：调用 `flash_erase_param_page` 0x0800587C + `flash_store_params` 0x0800B820
  （长按组合键恢复出厂）。
- 显示：`display_task` @ **0x08009228**（273 条）驱动 10 个"页/字段"渲染函数
  （0x08008DEC / 0x08008E58 / 0x08008EC4 / 0x08008F34 / 0x08008FA0 / 0x0800900C / 0x08009078 /
  0x080090E4 / 0x08009150 / 0x080091BC，各 44–45 条，均以 `0x0800CCAC` 收尾=提交），
  引用数据表 0x0800D01C/0x0800D03C；`0x08009D58` 引用表 0x0800E35F。
  **显示器的物理接口本固件未用 AF/并口常量配置 → [推测] 段码/IO 扩展或由 RF 芯片侧 MCU 驱动**。
- 提示音/告警：`0x0800B4EC` 以 PA7 输出脉冲串（三档半周期），见 §4。

---

## 8. 内部 Flash 参数存储

### 8.1 存放位置

| 页 | 地址 | 用途 | 证据 |
|---|---|---|---|
| 参数页 | **0x0801F000** | 32 字节参数块（8×u32） | `flash_erase_param_page` 0x0800587C 构造 `{type=0, start=0x0801F000, pages=1}` 调 `flash_erase_range` |
| 标志页 | **0x0801F800** | 魔数 `0x0000100E`（复位原因判定用） | `boot_reason_check` 0x0800AC24 读它并比较 |

### 8.2 参数块

- 写入：`flash_write_params` @ **0x08005974**：解锁 → `state.addr = addr` →
  循环 8 次 `flash_program_record(2, addr, u32, 0)` → 成功则 `addr += 4` → 加锁。
- 读取：`flash_read_params` @ **0x0800590C**：`state.addr = addr`，把 flash 上 8 个字拷到 RAM **0x20000464**。
- RAM 镜像 `0x20000464..0x20000484` 与用户设置镜像 `0x20000418..0x2000041F`（`settings_save` 0x0800CC64 使用）。
- 结构（[推测]，来自使用偏移）：`{magic/校验, 模式, 功率档, 通道, 蜂鸣/自动关机, …}`，
  每 4 字节由 `flash_program_record` 附带 `crc16_block`(0x08003144) 计算出的半字校验一起写入
  （0x08006cba–0x08006cc6：`crc16_block(src, …, i*16)` → `flash_program_halfword(dst+2i, checksum)`）。

### 8.3 FLASH 驱动寄存器层

| 函数 | 地址 | 行为 |
|---|---|---|
| `flash_unlock` | 0x08006CFC | 若 CR bit7（LOCK）→ KEYR=0x45670123, KEYR=0xCDEF89AB |
| `flash_lock` | 0x08006C6C | CR \|= 0x80 |
| `flash_erase_page` | 0x080058C8 | CR \|= 2(PER)；AR=addr；CR \|= 0x40(STRT) |
| `flash_mass_erase` | 0x080058A4 | CR \|= 4；CR \|= 0x40(STRT) |
| `flash_program_halfword` | 0x080058EC | CR \|= 1(PG)；`*(u16*)addr = val` |
| `flash_get_status` | 0x0800593C | 读 SR：bit4=WRPRTERR、bit2=PGERR、bit5=EOP，写 1 清除 |
| `flash_wait_last_operation` | 0x080059B4 | 轮询 SR.BSY，超时 0xC350（50000），清 EOP |
| `flash_erase_range` | 0x08006BCC | 入参 `{type, start, pages}`；type==1→整片擦除，否则按 **0x800 字节/页** 循环擦除 |
| `flash_program_record` | 0x08006C80 | 写 1/2/4 个半字，含 CRC16 计算 |
| `flash_store_params` | 0x0800B820 | 解锁 + 写参数 + 上锁 |

> 注意：**FLASH_CR 布局是 F1 风格**（LOCK=bit7、STRT=bit6、PG=0/PER=1/MER=2），
> 而 RCC/GPIO/SYSCFG/ADC 是 F0 风格（0x48000000 GPIO、0x40012400 ADC）。
> 该 MCU 推断为 **F0 内核 + F1 兼容 FLASH 控制器的国产/兼容型号 [推测]**，或固件直接照搬 F1 驱动。

---

## 9. 看门狗（IWDG）

| 函数 | 地址 | 行为 |
|---|---|---|
| `iwdg_init` | 0x08006EB8 | 结构 `{base=0x40003000, PR, RLR, WINR, state}`；写 KR=0x5555，PR/RLR/WINR，等 SR 的 PVU/RVU/WVU 清除（超时 1000 ms），KR=0xAAAA |
| `iwdg_refresh` | **0x08006F68** | 写 KR = **0xAAAA**（喂狗） |
| `iwdg_start` | **0x08006FBC** | 写 KR = **0xCCCC**（启动）+ 0xAAAA |
| `iwdg_init_default` | 0x080098E0 | PR=6（/256）、RLR=0xFFF（≈26 s @LSI 40 kHz） |
| `iwdg_init_timeout` | 0x0800990C | 同上，RLR 由参数（ms）换算；`system_reinit_task` 用 3000 |

主循环每轮开头 `iwdg_refresh`（0x0800ce56）；长耗时循环内部也喂狗
（如充电脉冲循环 0x0800b52a、0x0800b562、0x0800b594）。**看门狗在启动时由 0x08006FBC 启动**
（main 0x0800ce1a）。

---

## 10. 低功耗 / 关机

- `low_power_sleep` @ **0x080070F0**：`PWR_CR` 的 bit[3:2]（LPDS/PDDS）设置 + `SCB_SCR.SLEEPDEEP`；
  `mode==1` 时执行 `wfi`，否则 `sev; wfe; wfe`（0x0800710a–0x08007118，全镜像唯一的 WFI/WFE）。
- 自动关机：`reinit_prepare` @ **0x0800BB7C**：
  ```
  if (setting_0x2000041A /* auto-off, minutes */ != 0) {
      if (++u16_0x200001C0 >= 0x2EE0 /*12000*/) {   // 12000 × 主循环周期
          u16_0x200001C0 = 0;
          u8_0x200001C3++;
          u8_0x200001C4 = setting * 30;             // 0x1e * setting
          if (u8_0x200001C3 >= u8_0x200001C4) {
              u8_0x200001C3 = 0;
              *(u8*)0x200001C2 = 1;                 // 关机/重初始化请求
          }
      }
  }
  ```
- `system_reinit_task` @ **0x0800BA5C**（主循环末尾）：若 `0x200001C2 == 1` → 关中断 →
  `gpio_init_all`（引脚全部复位到安全态）→ `iwdg_init_timeout(3000)` → RTC → 重新 `clock_init_task`
  → 重新配置 EXTI/TIM → `pwr` 相关 → 最后 `low_power_sleep` 方向进入待机 **[关机流程，推测]**。
- 任何用户活动（`gpio_write_pin`/按键/RF/EXTI）都会调 `0x0800BA40`：清零
  `0x200001C0/0x200001C3/0x200001C2`（复位自动关机计时）。
- `0x200001C2` 同时是 **ISR 让位标志**：所有中断处理里都有
  `if (0x200001C2 != 0) skip 应用层工作`（0x080053fa、0x0800be20、0x0800be5a、0x0800bde6 等）。

---

## 11. 中断向量与处理链（实测）

| 向量 | 地址 | handler | 实际工作 |
|---|---|---|---|
| 1 Reset | 0x080030D4 | reset_handler | → SystemInit → startup |
| 2 NMI | 0x0800A25C | NMI_Handler | 空/复位返回 |
| 3 HardFault | 0x08008200 | HardFault_Handler | 死循环/复位 |
| 11 SVCall | 0x0800B81C | SVCall_Handler | 空桩 |
| 14 PendSV | 0x0800A90C | PendSV_Handler | 空桩 |
| 15 SysTick | 0x0800BC8C | SysTick_Handler | ++ms 计数；调度空桩 |
| 18 IRQ2 RTC | **0x0800AC0C** | RTC_IRQHandler | 置 `0x2000020D=1`，调 `rtc_clear_flags` 0x08007954 |
| 21 IRQ5 EXTI0_1 | **0x080053F0** | EXTI0_1_IRQHandler | PB0 触发 → `flash_fire_sequence` |
| 22 IRQ6 EXTI2_3 | 0x08005478 | EXTI2_3_IRQHandler | 仅清 EXTI line3 |
| 23 IRQ7 EXTI4_15 | **0x08005484** | EXTI4_15_IRQHandler | PA5/EXTI15 → 触发 / RF |
| 25 IRQ9 DMA1_CH1 | **0x080044D0** | DMA1_Ch1_IRQHandler | → `dma1_ch1_task(0x200002A8)` |
| 29 IRQ13 TIM1 | **0x0800BE1C** | TIM1_..._IRQHandler | 0x20000108 latch、PA4、`tim_irq_handler(0x20000318)` |
| 31 IRQ15 TIM2 | **0x0800BE58** | TIM2_IRQHandler | `rf_rx_timeout_task` + `tim_irq_handler(0x20000354)` |
| 35 IRQ19 TIM14 | **0x0800BDE0** | TIM14_IRQHandler | 主循环心跳 + 充电反馈/按键定时 + `tim_irq_handler(0x200003CC)` |
| 其他 | 0x080030E6 | default_handler | `b .`（48 项中 26 项为此桩） |

`tim_irq_handler` @ 0x08007F08（161 条）被三个 TIM 中断共用，入参为各自的 TIM 设备结构
（0x20000318 / 0x20000354 / 0x200003CC），内部处理 SR/CCR/DIER 并按回调表分派。

---

## 12. 数据区（0x0800D000–0x0800E52C，文件 0xA000–0xB52B）

| 地址 | 内容 | 证据 |
|---|---|---|
| 0x0800D000–0x0800D008 | 小常量 / GPIOB 基址备份（0x48000400） | 数据字 0x0800D008 |
| **0x0800D00C** | AHB 预分频移位表 16 字节 `00…01 02 03 04 06 07 08 09` | `rcc_clock_switch` 0x0800736c 索引 |
| 0x0800D01C / 0x0800D03C | 显示/查表偏移表（0x08008DEC…0x08009078 系列读取） | 0x08008d50/0x08008d5c 等 |
| 0x0800D05C | **u16 斜坡表**（0,1,2,3,… 单调递增，用于触发/充电时长） | `flash_fire_sequence` 0x08005b70 索引 |
| 0x0800D18A | **u16 斜坡表 2**（0x41,0x42,0x44,0x45,…,0x14B4） | 功率换算 0x0800626c/0x0800632c |
| 0x0800D412 / 0x0800D922 / 0x0800DC4A / 0x0800DED2 | 各模式功率/能量查表 | `measurement_task`、`level_table_*` 系列 |
| 0x0800E000–0x0800E3E0 | **闪光功率/能量标定表**：u16 单调序列（0x2810…0x28A0，0x282D 为"无效/最大"填充），后段混有 8 位表（0x0800E314 起被 0x080045AC/0x08008504 逐字节索引） | 0x080045ac、0x08008504 引用 0x0800E314..0x0800E325 |
| 0x0800E35F / 0x0800E36C | 段码/功率提示表 | 0x08009d58、0x08009c14 |
| **0x0800E370** | **2.4G 发射功率表（16 字节）** `00 50 44 C0 84 81 46 93 55 8D C6 69 6E 7F A9 BB` | `rf_init` 0x080048a6–0x080048ac 写入 PATABLE(0x3E) |
| **0x0800E3E4** | scatter 表（2×16 字节，见 §1） | `scatter_load` 0x08003548 |
| 0x0800E404–0x0800E52B | .data 初值镜像（0x128 B） | scatter 记录 1 |

> 说明：0x0800E000–0x0800E3E0 的条目并非纯 u16 数组（后段有字节表和看似随机的标定字节），
> 精确条目边界未完全恢复，标 **[推测]**；但"功率/能量标定表"属性有强证据（只有充电/触发状态机引用它）。
