# AD100 Pro V1.2 固件逆向 —— 关键发现与状态（Lead 维护）

> 本文记录已**验证**的事实，供团队共用。验证方式：静态反汇编（capstone）+ Unicorn 实机执行。
> 最后更新：2026-11-07

## 1. 固件文件与内存布局（已验证）

| 项目 | 值 | 证据 |
|---|---|---|
| 文件 | `firmware/AD100pro_V1.2.bin`，50248 字节，sha256 `3a3d3b91…` | 官方 zip `AD100pro_V1.2.zip` 解出，哈希一致 |
| 应用加载基址 | **0x08003000** | 向量表重置项 0x080030D5→0x080030D4 是 `ldr r0,[pc,#0x10]; blx r0`（SystemInit 桩）；0x080030C0 是 `ldr sp,=0x20000CD0` 启动桩；镜像末地址 0x0800F448 与 init_array 结束指针完全吻合 |
| 镜像范围 | 0x08003000 – 0x0800F448（50248 字节） | 文件长度 |
| bootloader | 0x08000000 – 0x08002FFF（0x3000 字节，**不在固件文件中**） | 镜像基址推算；4 字节对齐扫描无指向该区的指针 |
| Flash | **128 KiB**（0x08000000-0x08020000） | 代码引用 0x0801F000（标定表）、0x0801F800；`func_0800AD44` 读 0x0801F000+(i-1)*100 |
| 出厂标定区 | 0x0801F000 – 0x08020000（20 条 × 100 字节，首字节 magic=0x5C） | `func_0800AD44` @0x0800AD5E：`ldr r1,=0x0801F000; adds r0,r0,r1; bl 0x08005E8C`（拷 8 字）；`ldrb r0,[0x20000434]; cmp r0,#0x5c` |
| RAM | 0x20000000 起（初始 SP=0x20000CD0，至少 3.2KB 栈） | 向量表[0] |
| 向量表 | 48 项 @0x08003000；main 开头把 48 项复制到 **RAM 0x20000000**，随后 `SYSCFG.CFGR1 |= 3`（SRAM 向量重映射） | `func_0800CF74` 开头 16 条指令 |

> 结论：这是"bootloader + 应用"两段式固件。bootloader 被 SysTick 等中断服务代码引用？不——应用自己在 RAM 建立向量表并重映射，因此中断走 RAM 向量表。

## 2. 启动链（已验证）

```
0x080030D4  Reset_Handler        ldr r0,=SystemInit(0x0800B964); blx r0
0x080030D8                       ldr r0,=0x080030C1; bx r0      ; → _start
0x080030C0  _start               ldr r0,=0x20000CD0; mov sp,r0
                                 bl  0x08003548                 ; 静态构造/init_array
                                 ldr r0,=0x0800CF75; bx r0      ; → main
0x08003548  init_array_runner    遍历 0x0800F3EC..0x0800F40C，每条 16 字节 {a0,a1,a2,fn}，调 fn
0x0800CF74  main                 复制 48 向量到 RAM；SYSCFG.CFGR1|=3；一串 init 调用
```

`SystemInit`（0x0800B964）只做 RCC 复位值清理：清 CR 的 HSEON/CSSON/PLLON、CFGR 的 SW/PLL 位、CFGR2/CFGR3/CR2 的保留位，关闭 CIR。**真正的时钟树在 main 调用链里配置**。

## 3. 实测启动过程（Unicorn，3M 指令无崩溃）

```
reset → SystemInit → 0x080030C0 → init_array → main
      → 复制向量表到 RAM + SYSCFG 重映射
      → ADC/RTC(唤醒定时器 LSI)/GPIOA/B/C/D 初始化
      → 读 0x0801F000 标定表（magic 0x5C；空白 0xFF 时走无标定分支）
      → 进入主循环：GPIOB 位操作（LCD）+ GPIOC.IDR（按键/忙）+ IWDG 喂狗
```

外设访问分布（`out/periph_summary.md`）：RCC 最多，其次 GPIOB（LCD/按键）、EXTI（按键）、DMA1、FLASH_IF（参数写入）、TIM1/3/14/17、ADC、RTC、IWDG、PWR、CRC。**没有硬件 SPI/USART/I2C 寄存器访问**——2.4G 射频与 LCD 都是 GPIO 位操作（bit-bang）。

## 4. 已识别的基础 API（可直接命名）

| 地址 | 语义 | 证据 |
|---|---|---|
| 0x0800741C | `gpio_read_pin(base, mask)` → bool | `ldr r0,[r0,#0x10]; ands r0,r1` |
| 0x08007426 | `gpio_write_pin(base, mask, level)` | `cmp r2,#0; str r1,[r0,#0x18]`(BSRR) / `[r0,#0x28]`(BRR) |
| 0x08007434 | `get_tick()` | `ldr r0,=0x20000104; ldr r0,[r0]` |
| 0x0800A4xx | LCD 串行位输出（8 位循环 + BSRR/BRR） | 掩码 0x40/0x80/0x200/0x400 on GPIOB |
| 0x08005E8C | `memcpy_8words(dst=0x20000434, src)` 32 字节 | 循环 8 次 `str r4,[r3,r2]` |

## 5. 未解决问题

1. bootloader（0x0-0x2FFF）内容未知：它是否设置 VTOR/时钟/看门狗？应用已自带 SystemInit，影响有限。
2. 出厂标定区（0x0801F000）真实内容未知 → 模拟器需合成；空白时会走"无标定/产测"路径。
3. MCU 具体型号未定：外设映射 = STM32F0，但 `SYSCFG.CFGR1 |= 3` + RAM 向量表重映射更像是 bootloader 约定；flash 128KB + 8KB RAM 的组合对应 STM32F030CC/F070CB/F072CB 之类。待用 CPUID@0xE000ED00 或 flash 容量寄存器确认（模拟器里可读取）。
4. LCD 控制器型号/分辨率待定（位操作引脚：GPIOB bit6/7/9/10）。

---

## 6. 最终汇总（队友交叉验证后修正）

- **射频**：TI CC2500/CC1101 寄存器兼容芯片（TEST2/1/0=0x88/0x31/0x0B、PATABLE=0x3E 等），4 线软 SPI：CS=PC6、SCK=PB13、MOSI=PB12、MISO=PB14、GDO0=PA15→EXTI15→IRQ7。32 信道 2413.0–2464.5MHz，同步字=编码 ID `50024/(ID+1)`，包 4 字节，`0xD5` 命令帧 / `0xA9` 属性帧。详见 `docs/03_rf_protocol.md`。
- **显示**：128x64 单色 OLED（SH1106/SSD1306 类），4 线串行 SCL=PB6、SI=PA10、A0=PB7、CS=PB9、RESET=PB8；**帧缓冲 RAM 0x200004CC（1024B，8 页×128 列）**；初始化命令表 0x0800C954；字库 9 字节/字形（宽度+8 字节位图），bank 从 0x0800Dxxx 起。刷新函数 0x0800A30C。
- **按键**：电源键 PB0→EXTI0(IRQ5)、第二键 PA6→EXTI6(IRQ7)；矩阵扫描 3 行(PB3/4/5)×5 列(PC10/11/12/PC7/PD4)，扫描函数 0x08008D00，1ms 任务内 5 分频。
- **时钟**：HSI48=48MHz 作 SYSCLK（CFGR.SW=3，PLL 不用），FLASH 1WS，SysTick 1ms(48000)，uwTick=RAM 0x20000104。
- **定时器**：TIM1 100Hz、TIM2 1Hz、TIM3 PSC1/ARR639≈37.5kHz PWM、TIM14 5kHz；对应 IRQ13/15/19 进 HAL_TIM_IRQHandler。
- **RCC 使能**：AHBENR bit0=DMA1、bit17-20/22=GPIOA/B/C/D/F（无 GPIOE）；APB1ENR bit28=PWR、bit29=DAC；APB2ENR bit0=SYSCFG、bit9=ADC1。
- **中继/修正**：0x0800DA78 起是数据（信道/功率表 + 字模），不是 770 条指令的函数（原 capstone 误判，`out/decompiled/ISSUES.md` 有记录）；SVC/PendSV 为空 → 无 RTOS；main 为"1ms tick + 16 任务"轮询架构。
- **模拟器实测**：25M 指令无崩溃；5M 指令 0 fault，2.8M 指令/s；LCD 解码与 RAM 帧缓冲一致；网页调试器 live 模式已联调（见 `webui/screenshot-live.png`）。
