# AD100 Pro V1.2 固件逆向 · 启动流程、时钟树、GPIO 引脚映射与底层驱动

> 分析对象：`firmware/AD100pro_V1.2.bin`（50248 字节 = 0xC448，sha256 前缀 3a3d3b91…）
> 加载基址：**0x08003000**（0x08000000–0x08002FFF 为未提供的 0x3000 字节 bootloader）
> 应用镜像：0x08003000–0x0800F448；RAM：0x20000000–0x20002000（8 KB）；Cortex-M0 / STM32F0 类外设映射
> 本文所有地址均指镜像内地址；`★`=反汇编直接证据，`○`=由寄存器值/调用上下文推断，`?`=未能确认。
> 工具：`tools/disasm.py` 产物 `out/disasm.txt`（带函数边界/调用者/字面量解析）+ capstone 复核。
> 本文档中的寄存器基址名称来自 `out/periph_map.json` 的映射（GPIO 0x4800_0000+n*0x400、RCC 0x4002_1000、FLASH_IF 0x4002_2000、SYSCFG 0x4001_0000、EXTI 0x4001_0400、ADC 0x4001_2400、DMA1 0x4002_0000、RTC 0x4000_2800、IWDG 0x4000_3000、PWR 0x4000_7000）。
> 注意：`out/periph_summary.md` 中 “FLASH_IF.ACR” 一行实际是 FLASH 接口基址+0 的命名；代码里 `[FLASH_BASE+0x10]` 是 **FLASH_CR**、`[+0x0C]` 是 **FLASH_SR**、`[+0x00]` 是 **FLASH_ACR**，本文按偏移重新注释。

---

## 0. 结论速览

| 项 | 结论 | 证据 |
|---|---|---|
| 启动链 | 向量表 0x08003000 → Reset 0x080030D5 → SystemInit 0x0800B964 → 0x080030C0 设 SP、调用 init_array 0x08003548 → main 0x0800CF74 | ★ |
| 向量表重定位 | main 把 0x08003000 起 48 个向量（0xC0 字节）复制到 0x20000000，然后 `SYSCFG->CFGR1 |= 3`（MEM_MODE=SRAM） | ★ |
| init_array | 表位于 0x0800F3EC，共 2 项（16 字节/项）：解压 .data（0x0800F40C→0x20000100，0x13C 字节，函数 0x0800356C）；清零 .bss（0x2000023C，0xA94 字节，函数 0x0800C91C） | ★ |
| 时钟 | **SYSCLK/HCLK/PCLK = 48 MHz，时钟源 HSI48（CFGR.SW=3）**，不用 PLL/HSE/HSI；FLASH 1 个等待周期；AHB/APB 不分频 | ★ |
| SysTick | SystemCoreClock/1000 = 48000 → **1 ms 节拍**（`SysTick_Config(48000)`，0x08008164 由 0x0800B8F0 调用） | ★ |
| RTC | 时钟源 LSE（BDCR.RTCSEL=10，0x08007678），异步/同步预分频 0x7F/0xFF（32768/128/256=1 Hz）；RTC 时钟使能位 BDCR[15] | ★(配置)/?（LSE 使能来源） |
| IWDG | LSI 驱动，预分频=6（/256），重装值 0xFFF→ 约 **26.2 s** 超时；句柄 0x20000384；刷新用 0x080074F0（写 KR=0xAAAA） | ★ |
| 定时器 | TIM1 100 Hz、TIM2 1 Hz、TIM3 PSC=1/ARR=639（约 37.5 kHz，PWM）、TIM14 5 kHz；均用 HAL_TIM_Base_Init + Start_IT | ★ |
| GPIO | 板级初始化 0x0800941C；LCD 为 4 线串行（PB6 SCK / PB7 A0 / PB8 RESET / PB9 CS / PA10 SI）；另一路 4 线类 SPI 为射频模块（PC6 CS / PB13 SCK / PB12 MOSI / PB14 MISO / PB15 IRQ），寄存器映射与 CC2500/CC1101 完全吻合（FREQ2/1/0=0x5C/0xCE/0xC4） | ★/○ |
| 按键 | PB0=电源键（EXTI0，下降沿，长按计数 0x200001CA，TIM14 5 kHz 递减）；PA6=第二按键（EXTI6） | ★/○ |
| 固件库 | 与 STM32F0 HAL 高度一致（CMSIS SystemInit 逐条相同；HAL_GPIO_Init/GPIO_Mode 0x10210000、HAL_TIM_Base_Init、IWDG 键值 0xCCCC/0xAAAA、RCC_OscInitTypeDef 含 HSI14/HSI48 字段） | ★ |

---

## 1. 镜像与内存布局

```
0x08000000 ┌──────────────────────────────┐
           │ bootloader（0x3000 字节，未提供）│
0x08003000 ├──────────────────────────────┤ ← 应用镜像 & 向量表（48 项，0x08003000–0x080030BF）
           │ .text / .rodata              │
0x0800F3EC │ init_array（2×16 字节）       │ ★ 0x0800F3EC..0x0800F40B
0x0800F40C │ 压缩的 .data 数据块           │ ★ 0x0800F40C..0x0800F447（60 字节）
0x0800F448 └──────────────────────────────┘ ← 文件结束（文件大小 50248 = 0xC448）
RAM:
0x20000000 ┌──────────────────────────────┐
           │ 向量表副本（48 项 = 192 字节）  │ ★ main 复制
0x20000100 │ .data（SystemCoreClock 等）  │ ★ 0x13C 字节
0x2000023C │ .bss（0xA94 字节，到 0x20000CD0）│ ★ 清零
0x20000CD0 └──────────────────────────────┘ ← 初始 SP（栈向下）
```

- 初始 SP = `0x20000CD0`（向量表首字），与 .bss 末尾一致 ★。
- 代码多处引用 **0x0801F000 / 0x0801F800** 两个地址（参数区读取 0x0800AD44、0x0800AD10），超出 64 KB flash 范围，说明 **实际 Flash 容量可能 ≥128 KB，或这两个常量来自 bootloader 共享区**；参数区按“100 字节/块、最多 20 块”组织（`0x0801F000 + n*100`），另有 0x0801F800 处一个标识字（与 0x1018 比较）。→ 待确认（见 §11）。

---

## 2. 启动流程（逐条）

### 2.1 向量表（0x08003000，48 项）

| idx | 地址 | 值 | 含义 |
|---|---|---|---|
| 0 | 0x08003000 | 0x20000CD0 | 初始 SP |
| 1 | 0x08003004 | 0x080030D5 | Reset_Handler |
| 2 | 0x08003008 | 0x0800A031 | NMI |
| 3 | 0x0800300C | 0x08008735 | HardFault |
| 4–10 | 0x08003010 | 0 | 保留（含 MemManage/BusFault/UsageFault 槽位，M0 无此异常） |
| 11 | 0x0800302C | 0x0800B585 | SVCall（函数体只有 `bx lr`，空实现） |
| 12–13 | 0x08003034 | 0 | 保留 |
| 14 | 0x08003038 | 0x0800A919 | PendSV（空实现） |
| 15 | 0x0800303C | 0x0800B8E5 | SysTick_Handler |
| 16 (IRQ0) | 0x08003040 | 0x080030E7 | 默认死循环（`b .`，0x080030E6 附近） |
| 18 (IRQ2) | 0x08003048 | 0x0800ACF9 | **RTC_IRQHandler** |
| 21 (IRQ5) | 0x08003054 | 0x08005A79 | **EXTI0_1_IRQHandler** |
| 22 (IRQ6) | 0x08003058 | 0x08005B01 | **EXTI2_3_IRQHandler** |
| 23 (IRQ7) | 0x0800305C | 0x08005B0D | **EXTI4_15_IRQHandler** |
| 25 (IRQ9) | 0x08003064 | 0x08004791 | **DMA1_Channel1_IRQHandler** |
| 29 (IRQ13) | 0x08003074 | 0x0800BA7D | TIM1_BRK_UP_TRG_COM_IRQHandler（HAL_TIM_IRQHandler(&htim1)） |
| 31 (IRQ15) | 0x0800307C | 0x0800BA8D | TIM2_IRQHandler（HAL_TIM_IRQHandler(&htim2)） |
| 35 (IRQ19) | 0x0800308C | 0x0800BA45 | TIM14_IRQHandler |
| 其他 | — | 0x080030E7 | 默认处理（0x080030E4 附近无限循环） |

★ IRQ 编号按 STM32F0 RM0091 的中断向量编号（IRQ0=WWDG … IRQ19=TIM14 …），与处理函数内容一致：
- 0x08004790：`HAL_DMA_IRQHandler(&hdma_adc /*0x200003F8*/)`；
- 0x0800BA7C：`HAL_TIM_IRQHandler(&htim1 /*0x20000294*/)`；
- 0x0800BA8C：`HAL_TIM_IRQHandler(&htim2 /*0x200002D0*/)`；
- 0x0800BA44：TIM14 中断内做计数器递减（0x200001CA）等 housekeeping，再调 `HAL_TIM_IRQHandler(&htim14 /*0x20000348*/)`；
- 0x0800ACF8：置位 RAM_2000021A 后调 `HAL_RTC_IRQHandler(&hrtc /*0x20000274*/)`。

### 2.2 Reset_Handler → main

```
0x080030D4  ldr r0, =0x0800B964      ; SystemInit
0x080030D6  blx r0
0x080030D8  ldr r0, =0x080030C0      ; 第二阶段启动
0x080030DA  bx  r0                   ; （不返回）

0x080030C0  ldr r0, =0x20000CD0      ; 重设 SP（与向量表首字相同）
0x080030C2  mov sp, r0
0x080030C4  bl  0x08003548           ; init_array：.data 解压 + .bss 清零
0x080030C8  ldr r0, =0x0800CF74      ; main
0x080030CA  bx  r0
```

### 2.3 init_array（0x08003548，表在 0x0800F3EC）

```
0x08003548  r4 = 0x0800F3EC; r6 = 0x0800F40C; r5 = 1
loop:  r3 = [r4+0xC]; ldm r4!, {r0,r1,r2}; r3 |= 1; r4 -= 0xC
       blx r3;  r4 += 0x10;  while (r4 < r6)
0x08003560  bl 0x080030C8            ; → main
```
★ 表内容（16 字节/项 = {arg0, arg1, arg2, fn|1}）：

| 项 | 地址 | arg0 | arg1 | arg2 | 函数 | 作用 |
|---|---|---|---|---|---|---|
| 1 | 0x0800F3EC | 0x0800F40C | 0x20000100 | 0x0000013C | 0x0800356C | 从 0x0800F40C 解压 316 字节到 0x20000100（.data 初始化） |
| 2 | 0x0800F3FC | 0x0800F448 | 0x2000023C | 0x00000A94 | 0x0800C91C | 把 0x2000023C 起 2708 字节清零（.bss）→ 0x2000023C+0xA94=0x20000CD0=初始 SP |

- 0x0800356C 是 **半字节编码的 RLE/LZ 解压器**（★：读控制字节低 4 位=字面量长度、高 4 位=重复次数，长度/次数为 0 时再取下一字节扩展；0x0800F40C 处压缩流首字节 `04 10 00 12 7a 02 23 …`）。推断为 ARMCC 的 `.data` 压缩（`__scatterload` 风格），未在固件中再出现第二处使用。
- 0x0800C91C 实际是 `memset`（`stm r1!,{r0}`, `subs r2,#4` 循环，r1=dst、r2=len）★。

### 2.4 main（0x0800CF74）

```
0x0800CF74  push {r3,lr}
            ; ① 复制向量表到 RAM，并做 SRAM 重映射
0x0800CF76  r2 = 0x08003000; r3 = 0x20000000
0x0800CF7C  for (i=0; i<0x30; i++) ((u32*)0x20000000)[i] = ((u32*)0x08003000)[i];
0x0800CF8A  SYSCFG->CFGR1 |= 3        ; ★ MEM_MODE = SRAM（向量表重映射到 0x00000000）
            ; ② 初始化序列（见 §2.5）
            ; ③ 事件循环（见 §9.3）
```

### 2.5 main 初始化序列（0x0800CF94–0x0800CFFA）

| 调用 | 目标 | 还原名称/作用 |
|---|---|---|
| 0x0800CF94 | 0x080075CC | **HAL_Init**：`FLASH->ACR |= 0x10`（PRFTBE 预取使能）→ HAL_InitTick → HAL_MspInit |
| 0x0800CF98 | 0x0800B8F0 | **SystemClock_Config**（见 §3） |
| 0x0800CF9C | 0x0800941C | **板级 GPIO 初始化**（见 §5） |
| 0x0800CFA0 | 0x080093F4 | 使能 DMA1 时钟（AHBENR bit0），配置并使能 NVIC IRQ9（DMA1_Ch1） |
| 0x0800CFA4 | 0x0800967C | TIM14 初始化（5 kHz），句柄 0x20000348 |
| 0x0800CFA8 | 0x08009310 | ADC 初始化（通道 10/11/12 等，见 §7） |
| 0x0800CFAC | 0x080095CC | IWDG 初始化（句柄 0x20000384，见 §8.2） |
| 0x0800CFB0 | 0x080095F8 | RTC 初始化（24h、预分频 0x7F/0xFF、使能 RTCEN 与 IRQ2） |
| 0x0800CFB4 | 0x08009708 | TIM2 初始化（1 Hz，句柄 0x200002D0） |
| 0x0800CFB8 | 0x08009768 | TIM3 初始化 + PWM 通道（句柄 0x2000030C，PA7=TIM3_CH2 AF1） |
| 0x0800CFBC | 0x080096A8 | TIM1 初始化（100 Hz，句柄 0x20000294）+ PWM/OC 配置 |
| 0x0800CFC0 | 0x080093A8 | 0x4000_7400（**DAC**，F0 带 DAC 的型号）初始化：HAL_DAC_Init + 两通道 ConfigChannel |
| 0x0800CFC4 | — | `RCC->APB1ENR |= 1<<28`（PWREN），读回校验（★ 0x40021000+0x1C） |
| 0x0800CFD2 | — | `primask = 1`（关总中断） |
| 0x0800CFDC | 0x08007F10 | **HAL_RTC_DeInit**（BDCR.RTCEN=0，禁 IRQ2） |
| 0x0800CFE0 | 0x08007544 | IWDG 启动/重配（写 KR=0xAAAA、0xCCCC，等 SR 更新完成） |
| 0x0800CFE8 | 0x080074F0 | HAL_IWDG_Refresh（KR=0xAAAA） |
| 0x0800CFEC | 0x0800AD10 | 读 `RCC->CSR` bit26（PINRSTF 引脚复位标志），若为 1 则读 0x0801F800 处标识字并与 0x1018 比较，据此设置 RAM_200001D0（作用待确认：升级/掉电恢复标记？） |
| 0x0800CFF0 | 0x0800AD44 | 从 0x0801F000 开始的 20×100 字节参数区读取/挑选，经 0x08005E8C 拷到 RAM_20000434 并解析（0x0800CE6C） |
| 0x0800CFF4 | 0x0800A934(1) | **开机（电源轨/外设上电）序列**（见 §9.1） |
| 0x0800CFFA | 0x0800ABE4 | 检测 PA6 电平后向射频模块发 0x36/0x33 等命令并做初始化（见 §6.2） |
| 0x0800CFFE | — | `RCC->CSR |= 1<<23`（0x00800000；标准 F0 中该位非通用功能位 → 疑为厂商扩展/复位标志操作，**待确认**） |
| 0x0800D008 | — | 清 RAM_2000021B=0；然后进入事件循环 |

### 2.6 SystemInit（0x0800B964）——与 STM32F0 CMSIS 逐条一致 ★

```
0x0800B964  RCC->CR    |= RCC_CR_HSION;
0x0800B96E  RCC->CFGR  &= 0x08FFB80C;   /* 复位 SW/HPRE/PPRE/MCOSEL/MCOPRE */
0x0800B976  RCC->CR    &= 0xFEF6FFFF;   /* 清 HSEON(16)/CSSON(19)/PLLON(24) */
0x0800B97E  RCC->CR    &= ~(1<<18);     /* 清 HSEBYP */
0x0800B988  RCC->CFGR  &= ~(0x3F<<16);  /* 清 PLLSRC/PLLXTPRE/PLLMUL */
0x0800B992  RCC->CFGR2 &= ~0xF;         /* 清 PREDIV */
0x0800B99A  RCC->CFGR3 &= 0xFFFCFE2C;   /* 清 USART/I2C/CEC/USB 时钟源 */
0x0800B9A2  RCC->CR2   &= ~1;           /* 清 HSI48ON */
0x0800B9AA  RCC->CIR    = 0;            /* 关闭所有 RCC 中断 */
```
★ 这些掩码与 `stm32f0xx` CMSIS `SystemInit()` 完全一致（含 CFGR3/CR2 这种只在 F0/F3 存在的寄存器），是**该 MCU 为 STM32F0 系（或 F0 兼容克隆）**的有力证据。

---

## 3. 时钟树

### 3.1 HAL_Init（0x080075CC）/ HAL_InitTick（0x080075EC）/ HAL_MspInit（0x08007610）

```
0x080075CC HAL_Init():
   FLASH->ACR |= 0x10                 ; ★ PRFTBE 预取缓冲使能
   HAL_InitTick(0)                    ; 见下
   HAL_MspInit()

0x080075EC HAL_InitTick(tick_priority=r0):
   ticks = SystemCoreClock /*RAM_20000100，此时=0*/ / 1000;    ; 0x080078EC 取时钟、0x080030F0 为无符号除法
   SysTick_Config(ticks)                                        ; 0x08008164；ticks=0 时返回 1（失败，忽略）
   HAL_NVIC_SetPriority(SysTick_IRQn(-1), tick_priority, 0)     ; 0x08007670 → 0x0800A2C8

0x08007610 HAL_MspInit():
   RCC->APB2ENR |= 1<<0               ; ★ SYSCFGEN
   HAL_NVIC_SetPriority(-5/-2/-1, 0, 0)  ; 0x08007670 ×3（对 SysTick/PendSV 等 SCB 优先级）
```

### 3.2 SystemClock_Config（0x0800B8F0）

三个子调用，参数取自函数内构造的结构体（栈上）：

```
0x0800B8F4  osc.OscillatorType = 0x28;   ; ★ bit3=LSI(0x08) | bit5=HSI48(0x20)
            osc.LSIState       = 1;      ; [sp+0x1C]
            osc.HSI48State     = 1;      ; [sp+0x20]
            osc.PLL.PLLState   = 0;      ; [sp+0x24] → 不配置 PLL
   → HAL_RCC_OscConfig(0x08007980)       ; 其余字段未写（HSE/HSI/LSE/HSI14 不处理）

0x0800B90E  periph.RTCClockSelection = 0x200;  ; BDCR.RTCSEL=10 → LSE
            flags = 0x10000;                    ; bit16 → 使能 PWR 时钟、置 DBP
   → 0x08007678（RTC 时钟源/备份域）    ; BDCR = (BDCR&~0x300)|0x200（必要时先 BDRST），随后关 PWREN

0x0800B90E  clk.SYSCLKSource = 3;         ; ★ RCC_SYSCLKSOURCE_HSI48
            clk.AHBCLKDivider = 0;        ; /1
            clk.APB1CLKDivider = 0;       ; /1
            clk.APB2CLKDivider = 0;       ; /1（同一 PPRE 字段）
            flags = 7;                    ; bit0=切换SYSCLK bit1=HPRE bit2=PPRE
   → HAL_RCC_ClockConfig(0x08007798, FLASH_LATENCY=1)

   随后：
   SystemCoreClock = 0x080078F8();            ; 见 3.3 → 0x02DC6C00 = 48,000,000
   HAL_InitTick(0)                            ; 重新计算 48000 → SysTick_Config(48000) → 1 ms ★
   HAL_SetTickFreq/CTRL 调整(0x08008148(4))   ; SysTick CTRL 位2（CLKSOURCE）按参数设置
   HAL_NVIC_SetPriority(-1, 0, 0)
```

**结论：系统时钟 = HSI48 = 48 MHz，SYSCLK=HCLK=PCLK=48 MHz，FLASH 等待周期 1 WS，SysTick 1 ms。**

- ★ 依据：`RCC->CFGR.SW=3`；频率换算函数 0x080078F8 对 `SWS==3` 直接返回 `0x02DC6C00`（=48,000,000），对 SWS=1（HSE）返回 `0x007A1200`（8 MHz），SWS=0（HSI）返回 8 MHz，SWS=2（PLL）查表计算；`SystemCoreClock`（0x20000100）被写成换算结果。
- ★ HSI48 的使能：0x08007980 的 bit5 分支（0x08007C3A 起）对 `RCC->CR2` 置 **bit16**（0x08007C60/0x08007C8E 分别按 `HSI48State` 置/清），并在切换 SYSCLK 前等待其稳定；0x08007798 的 case 3（HSI48）也用 `RCC->CR2` 的 bit16 作为就绪判断（0x0800781C）。`CR2` 在标准 STM32F0 中为 HSI14/HSI48 控制寄存器（HSI14：bit0 ON/bit1 RDY/bit2 DIS/bit3-7 TRIM；HSI48：bit16 ON/bit17 RDY/bit24-31 CAL）→ 与代码行为一致。`?` 个别位定义（如 bit16 此处被同时用作 ON 与就绪判断）与标准头文件表述略有出入，见 §11。
- ★ PLL 未被使用：[sp+0x24]=0（PLLState=0），0x08007980 中 PLL 配置段被跳过（0x08007CAA 判 0 直接返回）。
- ★ LSI 被使能（`RCC->CSR.LSION`，0x08007AB2），IWDG 依赖它。
- HSE/HSI/LSE 均不在 OscConfig 中处理。**LSE 使能来源未在本镜像中找到**（RTC 却按 LSE 32.768 kHz 配置）→ 推断由 bootloader 预先使能，见 §11。

### 3.3 频率换算（0x080078F8）与 SystemCoreClock（0x20000100）

```
CFGR.SWS:  0(HSI) → 8 MHz 常量;  1(HSE) → 8 MHz 常量;
           2(PLL) → 查表(0x08007954 附近 8 个字节表 + CFGR2.PREDIV + CFGR.PLLMUL 计算);
           3(HSI48) → 0x02DC6C00 (48 MHz)
SystemCoreClock = sysclk / HPRE_table[CFGR.HPRE];   ; 表在 0x0800D164
```
★ SystemCoreClock 初值来自 .data（0x20000100），所以 HAL_Init 阶段为 0（这就是第一次 SysTick_Config 失败、时钟配置后重做的原因）。

### 3.4 RCC 寄存器写序列（核心，按地址）

| 函数 | 寄存器（地址） | 操作 | 值 |
|---|---|---|---|
| 0x0800B964 SystemInit | RCC_CR (0x40021000) | 或/清 | 见 §2.6 |
| 0x0800B8F0 / 0x08007980 | RCC_CR | 读改写 | HSI48/LSI 相关位 |
| 0x08007980 (LSI 段) | RCC_CSR (0x40021024) | 或/清 bit0 | LSION=1（0x08007AB2） |
| 0x08007980 (HSI48 段) | RCC_CR2 (0x40021034) | 或/清 bit16 | HSI48ON |
| 0x08007678 | RCC_APB1ENR (0x4002101C) | 或/清 bit28 | PWREN 临时使能 |
| 0x08007678 | PWR_CR (0x40007000) | 或 bit8 | DBP（备份域写保护解除），带 100 tick 超时 |
| 0x08007678 | RCC_BDCR (0x40021020) | 读改写 | BDRST(bit16) 脉冲 + RTCSEL=0x200 |
| 0x08007798 | FLASH_ACR (0x40022000) | 读改写 bit0-2 | LATENCY=1（若当前不同） |
| 0x08007798 | RCC_CFGR (0x40021004) | 读改写 | HPRE(bit4-7)=0、PPRE(bit8-10)=0、SW(bit0-1)=3 |
| 0x08007798 | RCC_CR / RCC_CR2 | 轮询 | 等 HSERDY/PLLRDY/HSI48 就绪（超时 5000 tick） |
| 0x0800B8F0 | RCC_CFGR / CFGR2 / CFGR3 / CIR | 读 | 计算频率、清中断 |
| 0x0800CFC4/CFC8 | RCC_APB1ENR | 或 bit28 | PWREN（main 里，读回校验） |
| 0x0800CFFE | RCC_CSR | 或 bit23 | 0x00800000（语义待确认） |
| 0x08007F38 (RTC MspInit) | RCC_BDCR | 或 bit15 | RTCEN=1 |
| 0x08007F10 (RTC DeInit) | RCC_BDCR | 清 bit15 | RTCEN=0 |

---

## 4. RCC 使能的外设时钟（AHBENR / APB1ENR / APB2ENR）

位号由代码中的“基址地址移位生成位掩码”手法精确反推（★），外设名按 STM32F0 标准定义对应（★ 与后续 GPIO/外设使用一致）。

| 寄存器 | 位 | 外设 | 使能位置（证据） |
|---|---|---|---|
| AHBENR (0x40021014) | 0 | DMA1 | 0x080093F4（`orrs r1,r2` 且 r2=1，随后 NVIC IRQ9） |
| AHBENR | 17 | GPIOA | 0x0800941C（mask = RCC_BASE<<16 → bit28? 实际序列：1<<19、<<3=1<<22、>>5=1<<17、<<1=1<<18、<<2=1<<20，即 bits 17,18,19,20,22） |
| AHBENR | 18 | GPIOB | 同上 |
| AHBENR | 19 | GPIOC | 同上 |
| AHBENR | 20 | GPIOD | 同上 |
| AHBENR | 22 | GPIOF | 同上（**未使能 GPIOE**，代码中也没有真实 GPIOE 访问） |
| APB1ENR (0x4002101C) | 28 | PWR | 0x0800CFC4、0x0800768E（RCC_BASE<<16 → bit28） |
| APB1ENR | 29 | DAC | 0x08006D18（DAC 基址 0x40007400 的 MspInit；RCC_BASE<<17 → bit29） |
| APB1ENR | 0/1 | TIM2 / TIM3 | TIM2/TIM3 的 HAL_TIM_MspInit（0x0800821C / 0x08008582 / 0x08008652 等，见 §6.3） |
| APB1ENR | 4 | TIM14 | 0x080082F8（TIM14 MspInit，时钟使能） |
| APB2ENR (0x40021018) | 0 | SYSCFG | 0x08007610；另 0x080072C4（HAL_GPIO_Init 配 EXTI 时自动使能） |
| APB2ENR | 9 | ADC1 | 0x08006B20（`r7 = RCC_BASE>>21` → bit9） |
| APB2ENR | 11/17/18 | TIM1 / TIM16 / TIM17 | 各 HAL_TIM_MspInit（0x080083AC / 0x080083E8 等） |

> ★ 说明：TIM 的 MspInit（0x0800821C、0x080082F8、0x080083AC、0x08008582、0x080085D6、0x08008652、0x08008670）分别服务于不同定时器/通道；精确位号可在 `out/periph_map.json` 中按 `RCC.APB2ENR/w`、`RCC.APB1ENR/w` 逐条复查（本文不再逐一展开）。
> ★ 代码中没有 IWDG 时钟使能位（IWDG 由 LSI 直接驱动，无需 RCC 门控）——与 `functions.meta.json` 中无 IWDG 访问记录一致（0x40003000 通过句柄间接访问，未被 periph.py 的地址表识别）。

---

## 5. GPIO 引脚映射

### 5.1 板级 GPIO 初始化（0x0800941C，main 调用）

先使能 GPIOA/B/C/D/F 时钟（§4），随后用 **HAL_GPIO_Init**（0x080072C4）按 F0 的 `GPIO_InitTypeDef{Pin, Mode, Pull, Speed, Alternate}` 配置，并在配置前用 `GPIO_WriteBit`（0x08007426：r2≠0→BSRR 置位，r2=0→BRR 清零）预置初始电平。

**配置前预置电平（★）**

| 时间点 | 操作 | 证据 |
|---|---|---|
| 0x0800946E | GPIOC pins {4,5,8,9,13} ← 0 | `GPIO_WriteBit(GPIOC, 0x2330, 0)` |
| 0x08009478 | GPIOF pin 1 ← 0 | `GPIO_WriteBit(GPIOF, 1, 0)` |
| 0x08009482 | GPIOC pins {3,6} ← 1 | `GPIO_WriteBit(GPIOC, 0x48, 1)` |
| 0x0800948C | GPIOA pin 0 ← 1 | `GPIO_WriteBit(0x48000000, 1, 1)` |
| 0x08009498 | GPIOA pins {0,1,9,10} ← 0 | `GPIO_WriteBit(GPIOA, 0x602, 0)` |
| 0x080094A4 | GPIOB pins {2,10,11,12,13} ← 0 | `GPIO_WriteBit(GPIOB, 0x3C04, 0)` |
| 0x080094AE | GPIOB pins {6,7,8,9} ← 1 | `GPIO_WriteBit(GPIOB, 0x3C0, 1)`（LCD 空闲态：CS/A0/SCL/RESET 高） |

**HAL_GPIO_Init 调用（★，参数直接从栈上结构体解出）**

| # | 地址 | 端口 | Pin 掩码 | Mode | Pull | 说明 |
|---|---|---|---|---|---|---|
| a | 0x080094CC | GPIOC | 0x2378 (PC3,4,5,6,8,9,13) | 1=OUT_PP | 0 | 输出 |
| b | 0x080094DC | GPIOC | 0xDC80 (PC7,10,11,12,14,15) | 0=INPUT | 1=PULLUP | 上拉输入 |
| c | 0x080094EC | GPIOF | 0x0001 (PF1) | 1=OUT_PP | 0 | 输出 |
| d | 0x08009502 | GPIOA | 0x0603 (PA0,1,9,10) | 1=OUT_PP | 0 | 输出 |
| e | 0x08009512 | GPIOA | 0x8004 (PA2,15) | 0=INPUT | 1=PULLUP | 上拉输入 |
| f | 0x08009526 | GPIOA | 0x0040 (PA6) | 0x10210000=IT_FALLING | 1=PULLUP | 下降沿中断（EXTI6） |
| g | 0x08009534 | GPIOB | 0x0001 (PB0) | 0x10210000=IT_FALLING | 1=PULLUP | 下降沿中断（EXTI0） |
| h | 0x08009542 | GPIOB | 0x003A (PB1,3,4,5) | 1=OUT_PP | 0 | 输出 |
| i | 0x08009554 | GPIOB | 0x3FC4 (PB2,6,7,8,9,10,11,12,13) | 1=OUT_PP | 0 | 输出（含 LCD 全部信号与射频 SPI 信号） |
| j | 0x08009566 | GPIOB | 0xC000 (PB14,15) | 0=INPUT | 0 | 浮空输入（PB14 稍后改下拉） |
| k | 0x08009574 | GPIOD | 0x0004 (PD2) | 0=INPUT | 1=PULLUP | 上拉输入 |

随后配置 NVIC：IRQ5（EXTI0_1）优先级 0 使能（0x0800957E/0x08009584）、IRQ7（EXTI4_15）优先级 0 使能（0x0800958E/0x08009594）★。

**引脚汇总表**

| 引脚 | 模式 | 初始电平/上拉 | 推断用途 | 证据地址 |
|---|---|---|---|---|
| PA0 | OUT_PP | 1 | 未知（电源轨使能候选） | 0x08009494 / 0x08009502 |
| PA1 | OUT_PP | 0 | **上电使能**：开机序列置 1、关机序列置 0 | 0x0800949C / 0x0800B9C2(开机) / 0x0800A95C(关机) |
| PA2 | IN 上拉 | — | 未知 | 0x08009512 |
| PA4 | 未在板级初始化中配置 | 关机序列写 0 | 未知（可能为 DAC 输出/模拟脚，见 §7） | 0x0800A97C |
| PA6 | IN 上拉 + EXTI 下降沿 | — | **第二按键/触发输入**（EXTI6 → 0x08005B0C） | 0x08009526 / 0x08005B38 |
| PA7 | OUT_PP（0x08008A2C）；另被配为 **TIM3_CH2 AF1**（0x0800854C） | — | 蜂鸣器/LED/背光 PWM（两种配置冲突，见 §11） | 0x08008A2C / 0x0800854C |
| PA8 | 模拟输入（ANALOG，mode=3） | — | ADC 输入（与标准 F0 ADC 引脚映射不自洽，待确认） | 0x08006B5A |
| PA9 | OUT_PP | 0 | 未知 | 0x0800949C / 0x08009502 |
| PA10 | OUT_PP | 0 | **LCD 串行数据 SI**（0x0800A480 对 GPIOA BSRR/BRR bit10 逐位输出） | 0x0800A480 |
| PA15 | IN 上拉 | — | 未知（同脚 EXTI15 未配置为本脚） | 0x08009512 |
| PB0 | IN 上拉 + EXTI 下降沿 | — | **电源键**（EXTI0 → 0x08005A78，计数器 0x200001CA） | 0x08009534 / 0x08005A78 |
| PB1 | OUT_PP | 0 | 未知；休眠时改为 EXTI 下降沿唤醒源（0x0800D0E4）并切回（0x0800D108） | 0x08009542 / 0x0800D0E4 |
| PB2 | OUT_PP | 0 | 未知（关机序列写 0） | 0x080094A6 / 0x0800A99A |
| PB3 | OUT_PP | — | 未知 | 0x08009542 |
| PB4 | OUT_PP | — | 未知 | 0x08009542 |
| PB5 | OUT_PP | — | 未知 | 0x08009542 |
| PB6 | OUT_PP | 1 | **LCD 时钟 SCL**（0x0800A480 每 bit 先拉低再拉高） | 0x08009554 / 0x0800A480 |
| PB7 | OUT_PP | 1 | **LCD A0/RS（命令/数据选择）**（0=命令，1=数据） | 0x08009554 / 0x0800A480 |
| PB8 | OUT_PP | 1 | **LCD /RESET**（0x0800D134：拉低→延时→拉高，复位脉冲） | 0x08009554 / 0x0800D134 |
| PB9 | OUT_PP | 1 | **LCD /CS**（0=选中；写完后置 1） | 0x08009554 / 0x0800A480 |
| PB10 | OUT_PP | 0 | 未知 | 0x080094A6 / 0x08009554 |
| PB11 | OUT_PP | 0 | 未知（关机序列写 0） | 0x080094A6 / 0x0800A9A6 |
| PB12 | OUT_PP | 0 | **射频模块 MOSI**（0x0800CD7C：bit7→…→bit0，BSRR/BRR bit12） | 0x08009554 / 0x0800CD7C |
| PB13 | OUT_PP | 0 | **射频模块 SCK**（每 bit 产生一个上升沿） | 0x08009554 / 0x0800CD7C |
| PB14 | IN 浮空→**下拉**（0x08008C78） | — | **射频模块 MISO**（0x0800CD7C 读 IDR bit14） | 0x08009566 / 0x08008C78 / 0x0800CD7C |
| PB15 | IN 浮空 + EXTI 上升/下降沿（运行时切换） | — | **射频模块中断（GDO0/IRQ）**（EXTI15 → 0x08005B0C） | 0x08009566 / 0x0800655C / 0x08006584 |
| PC0/PC1/PC2 | 未在板级初始化中配置 | — | ADC 通道 10/11/12（标准 F0 映射，推断，待确认） | 0x08009362 起 |
| PC3 | OUT_PP | 0 | 未知（关机序列写 0） | 0x0800946E / 0x08009554 |
| PC4 | OUT_PP | 0 | 未知 | 0x0800946E / 0x080094CC |
| PC5 | OUT_PP | 0 | **电源保持锁存（推断）**：开机处置 1、关机序列拉低 | 0x0800A9DA / 0x0800A94E |
| PC6 | OUT_PP | 1 | **射频模块 /CS**（0x0800CE38 拉低、0x0800CE58 拉高） | 0x08009482 / 0x08008C78 / 0x0800CE38 |
| PC7 | IN 上拉 | — | **电源/充电检测**（关机序列等 PC7 高/低，0x0800A9C0–0x0800AA36） | 0x080094DC / 0x0800A9C0 |
| PC8 | OUT_PP | 0 | 未知（0x08008C78 再次配置并写 0） | 0x0800946E / 0x08008C78 |
| PC9 | OUT_PP | 0 | 未知 | 0x0800946E / 0x080094CC |
| PC10 | IN 上拉 | — | 未知 | 0x080094DC |
| PC11 | IN 上拉 | — | 未知 | 0x080094DC |
| PC12 | IN 上拉 | — | 上电检测（0x0800C080 连读 3 次做去抖） | 0x0800C082–0x0800C0D8 |
| PC13 | OUT_PP | 0 | 未知（F0 的 PC13 驱动能力有限，常作 LED/使能） | 0x0800946E / 0x080094CC |
| PC14/PC15 | IN 上拉 | — | 未知（F0 上通常与 LSE 复用；此处未用 LSE 晶体） | 0x080094DC |
| PD2 | IN 上拉 | — | 未知 | 0x08009574 |
| PF0 | 未在板级初始化中配置 | 关机序列写 0 | 未知 | 0x0800A986 |
| PF1 | OUT_PP | 1（开机序列置 1） | 未知（使能类信号，推断） | 0x080094EC / 0x0800B9D6 |

> 说明：0x08008C78（射频初始化第一步，由 0x08004AF0 调用）再次显式配置：PC6 输出=1、PB13/PB12 输出=0、PB14 输入+下拉、PC8 输出=0 ★（该函数用“基址移位生成掩码”的紧凑代码，已用位运算逐条核算：0x48000400<<17 恰为 bit27 等）。

### 5.2 LCD：128×64 串行点阵（4 线：SCL/SI/A0/CS + RESET）

| 信号 | 引脚 | 证据 |
|---|---|---|
| SCL | PB6 | 0x0800A480 对 `GPIOB->BSRR/BRR` 写 0x40 |
| SI（数据） | PA10 | 0x0800A480 对 `GPIOA->BSRR/BRR` 写 0x400 |
| A0/RS | PB7 | 0x0800A480 对 `GPIOB->BSRR/BRR` 写 0x80（命令=0，数据=1） |
| /CS | PB9 | 0x0800A480 写 0x200（开始时清零、结束时置 1） |
| /RESET | PB8 | 0x0800D134 GPIOB bit8 低→延时→高；由 0x0800D088 在初始化前调用 |

- **写一个字节**（0x0800A480）：`lcd_write(byte, is_data)`；MSB first，每位：拉低 SCL → 按位设置 SI → 拉高 SCL；结束后 CS=1、A0=1 ★。
- **初始化命令序列**（0x0800C954，共 28 字节）：`AE 02 10 40 B0 81 FF A1 A6 A8 3F AD 8B 33 C8 D3 00 D5 80 D9 1F DA 12 DB 30 A0 C0 AF` ★；与该序列同族命令（0xAE 关显示、0x81 对比度、0xA0/A1 段方向、0xC0/C8 行方向、0xAF 开显示、0xB0 页地址）指向 **ST7565/ST7567/UC1701 类 128×64 单色 LCD 控制器**（○，具体型号待确认）。
- **列地址设置**（0x08004670）：`lcd_write(0x10 | (col>>4))`、`lcd_write(0x02 | (col & 0xF))`（第二字节的 bit1 恒为 1，含义待确认）。
- **整屏刷新**：0x0800A30C 依次对 8 个页（0xB0–0xB7）各写 128 字节；0x0800C82C 按页偏移写 2 页、0x0800C2B8 做其他局部刷新。帧缓冲 **0x200004CC**（1024 字节 = 128×8 页）★。
- 刷新调度：主循环任务 0x0800D098 用计数器 0x20000228[2] 在 0..4 间轮转，4 时调 0x0800C2B8，否则调 0x0800C82C(index) ★。

### 5.3 射频模块：4 线类 SPI（CS/SCK/MOSI/MISO/IRQ）

| 信号 | 引脚 | 证据 |
|---|---|---|
| /CS | PC6 | 0x0800CE38 写 0；0x0800CE58 写 1（GPIOC 0x40） |
| SCK | PB13 | 0x0800CD7C 每 bit 先 `BRR 0x2000` 再 `BSRR 0x2000` |
| MOSI | PB12 | 0x0800CD7C 按 bit 写 `BSRR/BRR 0x1000` |
| MISO | PB14 | 0x0800CD7C 读 `GPIOB->IDR` bit14 |
| IRQ | PB15 | EXTI15（0x0800655C 上升沿 / 0x08006584 下降沿动态切换），中断入口 0x08005B0C → 0x0800AA74 |

- **单字节全双工**（0x0800CD7C）：每次 CS 拉低后，MSB first 收发 8 bit，读回值拼到 r0 ★。
- **写寄存器**（0x0800CD32 → 0x0800CE0C）：`firstByte=reg`，随后写 1 字节数据（共 2 字节）。
- **写多字节**（0x0800CE0C(reg, buf, len)）：首字节 reg + len 字节数据。
- **读寄存器**（0x0800CCA6 → 0x0800CDC8）：`firstByte = reg | 0x80`，读 1 字节 → **R/W 位 = bit7**，与 CC2500/CC1101 的 SPI 约定一致 ★。
- **批量读**（0x0800CC98(buf,len)）：首字节固定 0xFF，读 len 字节；**批量写**（0x0800CD24(buf,len)）：首字节固定 0x7F（= 0x3F|0x40，PATABLE 突发写，推断）。
- HS 复位（0x0800CCBC）：PC6 电平脉冲 + 发命令 **0x30（SRES）**，并等待 PB14 空闲（低）★。
- **初始化寄存器表**（0x0800CAE8，由 0x08004AF0 调用）：40+ 组 (reg, value) 写，例如
  `00=29 01=2E 02=06 03=01 04=C3 05=68 06=04 07=00 08=00 09=00 0A=00 0B=07 0C=00 0D=5C 0E=CE 0F=C4 10=1D 11=3B 12=73 13=22 14=F8 15=00 16=07 17=30 18=08 19=1D 1A=1C 1B=C7 1C=00 1D=B2 1E=6B 1F=87 20=F8 21=B6 22=10 23=EA 24=0A …` ★。
  → 其中 **0x0D/0x0E/0x0F = FREQ2/FREQ1/FREQ0 = 0x5C/0xCE/0xC4**，按 26 MHz 晶振、`f = FREQ×26M/2^16` 算得 ≈ **2.4125 GHz**，落在 2.4 GHz ISM 频段 ★○；0x00–0x02 为 GDO/IOCFG 类配置、0x03 FIFOTHR、0x04/0x05 SYNC、0x06 PKTLEN、0x0B FSCTRL1、0x14/0x20/0x21 为频偏/信道相关。
  → 结论：**射频模块为 CC2500/CC1101 寄存器兼容的 2.4 GHz 收发器**（Godox 2.4G X 系统），具体型号待确认（○）。
- 命令码（0x0800CDF4 单字节发送）：**0x36 = SIDLE、0x33 = SCAL、0x30 = SRES、0x3A = SFRX、0x34 = SRX、0x3B = SFTX**（与 CC2500/CC1101 命令选通字完全对应；0x08004AF0 用 0x36/0x33，0x0800AA74 在收发流程用 0x3A/0x34）★○。
- **发射功率表**：0x0800CD32(0x3E, tbl[0x20000214-6])，0x3E = PATABLE（功率表地址），表数据在 0x0800DA9A（按功率档索引）★。
- 初始化/握手流程（0x08004AF0）：0x08008C78 引脚初始化 → 延时 20000 tick → 最多 **11 次**（retry 0..10）{ 0x0800CCBC 复位+SRES；0x0800CAE8 写配置表；读 reg0x0D/0x0E 直到 0x0D==0x5C } → SIDLE/SCAL → 写 PATABLE → 0x0800B758/0x0800AB7C（进入接收/中断使能）。
- 数据包处理：EXTI15 → 0x08005B0C → 0x0800AA74（读状态、收发数据、命令 0x3A/0x34 等）→ 0x0800AFC4/0x0800B0BE（命令/事件分发，参数 r0=类型、r2=事件 1/2/4/7/9）→ 末尾把 **0x200001E1 置 1**（0x0800B2EE），唤醒主循环 ★。

---

## 6. 定时器 / 时基结构

### 6.1 SysTick
- 1 ms：`SysTick_Config(SystemCoreClock/1000 = 48000)`（0x0800B8F0 → 0x08008164），LOAD=47999、CTRL=7（内核时钟、开中断、使能）★。
- 中断 0x0800B8E5：`HAL_IncTick()`（0x080075BC 递增 0x20000104）→ 0x08008194 ★。

### 6.2 各定时器初始化（HAL_TIM_Base_Init 结构体参数 ★）

| TIM | 实例 | 句柄(RAM) | PSC | ARR | 频率 | 用途（推断） | 证据 |
|---|---|---|---|---|---|---|---|
| TIM1 | 0x40012C00 | 0x20000294 | 0x2F (47) | 0x270F (9999) | 48M/48/10000 = **100 Hz** | 闪光触发/时序基准；IRQ13 | 0x080096A8 |
| TIM2 | 0x40000000 | 0x200002D0 | 0x3BF (959) | 0xC350 (50000) | 48M/960/50001 ≈ **1 Hz** | 秒级计时；IRQ15 | 0x08009708 |
| TIM3 | 0x40000400 | 0x2000030C | 1 | 0x27F (639) | 48M/2/640 = **37.5 kHz** | PWM；PA7=CH2(AF1) | 0x08009768 / 0x0800854C |
| TIM14 | 0x40002000 | 0x20000348 | 0x2F (47) | 0xC7 (199) | 48M/48/200 = **5 kHz** | 系统节拍/按键计时递减；IRQ19 | 0x0800967C / 0x080082B4 |

启动方式：0x080096A8/0x08009708/0x08009768 均调用 0x080082F8（PWM/OC 配置）与 0x080081A0（`HAL_TIM_Base_Start_IT`，写 DIER.UIE + CR1.CEN）；TIM14 由 0x0800B9C0 通过 0x080082B4 启动（同样置 DIER/CR1）。

### 6.3 RTC
- 配置 0x080095F8：`hrtc{Instance=RTC, HourFormat=0(24h), AsynchPrediv=0x7F, SynchPrediv=0xFF, OutPut=0, Polarity=0, Type=0}` → LSE 32768/(128×256)=**1 Hz**；`HAL_RTC_Init` 经 0x08007E70→0x08007F38 的 MspInit 置 **BDCR[15] RTCEN=1** 并使能 NVIC IRQ2 ★。
- 中断 0x0800ACF8：置 RAM_2000021A=1 → `HAL_RTC_IRQHandler(&hrtc)`；主循环把该标志用于时间/唤醒相关处理（○）。
- 首次在 main 中 `HAL_RTC_DeInit`（0x08007F10）清 RTCEN，再重新初始化，属于“重启 RTC 域”的常规做法 ★。

### 6.4 IWDG
- 初始化 0x080095CC：句柄 0x20000384 = { IWDG(0x40003000), Prescaler=6, Reload=0xFFF, Window=0xFFF } → `HAL_IWDG_Init`（0x08007440，写 KR=0x5555 解锁、PR/RLR/WINR、KR=0xAAAA、KR=0xCCCC 启动）★。
  - LSI 约 40 kHz / 256 = 156.25 Hz → 超时 (0xFFF+1)/156.25 ≈ **26.2 s** ○。
- 刷新：0x080074F0（写 KR=0xAAAA，先查状态）；主循环第一件事、各长延时（0x08009038）内部都会刷新 ★。
- 0x08007544 为“启动/重配”序列（KR=0xAAAA 后 KR=0xCCCC，并等待 SR 更新完成），main 在初始化阶段调用一次 ★。

---

## 7. ADC + DMA

- ADC 初始化 0x08009310（`HAL_ADC_Init` 0x080069AC + `HAL_ADC_ConfigChannel` 0x08006878）：
  - 句柄 0x200003AC，Instance=ADC(0x40012400)；`ADC->CR` 各项按结构体位设置（0x080069AC）；
  - 通道配置（结构体 {Channel, Rank, SamplingTime}）：先 **Channel=3**（Rank=0x1000、SamplingTime=3），再依次 **Channel=10、11、12**（沿用同一 Rank/SamplingTime）★；
  - 0x08006878 对 **channel 16/17/18** 有专门分支（0x08006904/0x08006946），与 STM32F0 的“温度传感器/VREFINT/VBAT”通道号一致 ★。
- ADC 的 MSPInit（0x08006B20）：`RCC->APB2ENR |= 1<<9`（ADC1EN）；把 **GPIOC pin7** 与 **GPIOA pin8** 配置为 **模拟输入（mode=3）**；配置 **DMA1 Channel1** 句柄 0x200003F8 并 `HAL_DMA_Init`（0x080070B0）★。
- DMA1_CH1 中断 0x08004790 → `HAL_DMA_IRQHandler(&hdma)` ★；DMA1 时钟在 0x080093F4 使能、NVIC IRQ9 使能 ★。
- **矛盾点（待确认）**：ADC 通道 10/11/12 在标准 F0 上对应 PC0/PC1/PC2，而 MSPInit 把 PC7/PA8 设为模拟脚；两者在标准引脚映射下不自洽（PC7/PA8 不是 ADC 通道）。可能是克隆 MCU 的模拟通道映射不同，或该引脚配置服务于其它用途。见 §11。

---

## 8. FLASH 驱动与参数区

- 0x08005E24：`FLASH->CR |= 4`（MER 全片擦除）→ `FLASH->CR |= 0x40`（STRT）★（调用前清 0x20000240+0x1C）。
- 0x08005E48：`FLASH->CR |= 1`（PG）后按地址写半字（`strh`）★。
- 0x08005E6C：`FLASH->CR |= 1`（PG），`*(u16*)dst = val` ★。
- 0x08005F34：等待 `FLASH->SR` 忙标志结束（超时 3），并检查错误位（bit2/bit4/bit5 类，遇错清标志）★ → 与 HAL_FLASH 的“等待+错误处理”等价。
- 0x08007154/0x08007192/0x080071B6/0x080071F4/0x08007256/0x08007284：FLASH 解锁/加锁/选项字节相关（`FLASH->KEYR=0x45670123/0xCDEF89AB`、`OPTKEYR`）★。
- 参数区：0x0800AD44 以 `0x0801F000 + n*100`（n=0..19，共 20 块、每块 100 字节）读取，`0x08005E8C` 把每块前 **32 字节** 拷到 RAM_20000434，命中特征字节 `0x5C` 则用 0x0800CE6C 解析成一组运行参数；0x0800AD10 另外检查 `0x0801F800` 处一个字与 0x1018 的关系（升级/参数有效标记，○）。
  - → 说明固件具备**参数区自读写**能力（校准/用户设置），并可能在升级流程中使用 FLASH 编程函数。
  - → 地址 0x0801F000/0x0801F800 超出 64 KB，Flash 容量待确认。
- **本镜像内未发现 UART/USB/无线固件升级协议实现**：整个应用只含“参数区读写 + FLASH 擦写原语”，固件升级（IAP）逻辑应位于未提供的 bootloader（0x08000000–0x08002FFF）中；0x0800AD10 对 0x0801F800 标识字的检查可能用于与 bootloader 约定“升级/参数有效”状态（○，待确认）。

---

## 9. 电源、关机与主循环

### 9.1 开机序列（0x0800A934，参数 r0=1）
★ 0x0800B9C0 为“上电初始化”主体（由 0x0800A934 在检测到 PA6 为高/电源状态后调用）：
1. `GPIO_WriteBit(GPIOA, 2, 1)`：PA1=1（0x0800B9C2）；延时 100 ms（0x0800CA98(100)）。
2. `GPIO_WriteBit(GPIOF, 1, 1)`：PF1=1（0x0800B9D6）。
3. 0x08003AC8（其它外设初始化，未展开）。
4. 置若干状态字（0x20000180=1、0x20000181=1、0x20000184=1、0x20000194=0、0x200001DA=0xFA）。
5. 0x08008A2C：PA7=输出推挽高速。
6. **0x08004AF0 射频模块初始化**（§5.3）。
7. 0x0800D088：PB8 复位脉冲 + **0x0800C954 LCD 初始化命令** + 0x0800B5C0。
8. **0x0800ABE4**：读 PA6 两次（间隔 10000 tick）判定，随后向射频模块发 0x36/0x33、0x0800B718 等。
9. **0x0800C080**：硬件检测/自检（PC12 连读 3 次去抖、LCD 局部测试、IWDG 刷新、PC13/PC4/PC7 等电平操作）。
10. 0x080082B4：启动 TIM14 中断（5 kHz）。

### 9.2 关机序列（0x0800A934，r0≠1）
1. `GPIO_WriteBit`：PC5=0、PA1=0、PA9=0、PA0=1、PA4=0、PF0=0、PC3=0、PB2=0、PB11=0（0x0800A94E–0x0800A9B6）★。
2. 轮询 GPIOC pin7：若读回 1 则延时 10 tick 继续等；计数 10 次超时后，否则走另一路（等待 PC7 变低 / 刷新 IWDG）（0x0800A9BE–0x0800AA36）★。
3. 返回后由上层把 PC5 等置 0 → 断开电源锁存（推断，PC5 为电源保持脚 ○）。

### 9.3 主循环（0x0800CF74，0x0800D008 起）

```
for (;;) {
    primask = 0;                                  /* 开中断 */
    while (RAM_200001E1 != 1) ;                   /* 事件等待 */
    RAM_200001E1 = 0;
    HAL_IWDG_Refresh(&hiwdg /*0x20000384*/);      /* 0x080074F0 */
    task_08008D00();   /* 按键/充电状态机（读 GPIOC10/11/12，重配 GPIOB 引脚） */
    task_080047CC();   task_080098F8();  task_080035A8();  task_0800642C();
    task_080038C0();   task_0800D098();  /* 显示刷新调度（0x0800C82C/0x0800C2B8） */
    task_080065CC();   task_0800896C();  task_0800B394();  task_0800A230();
    task_08003BAC();   task_0800C7D4();  task_0800B7B0(); /* 休眠/唤醒管理 */
    task_080097F8();   task_0800B80C();
}
```
- 事件标志 **0x200001E1** 的唯一置位点是 0x0800B2EE（射频包命令处理 0x0800B0BE 末尾）★；即主循环是**射频事件驱动**，其余周期性工作由 TIM14 5 kHz / TIM2 1 Hz / RTC 中断里的计数器推进（○）。
- 0x08008D00 每轮按子状态（0x20000164+0xA，0..4）重配 GPIOB 若干脚并读 GPIOC 10/11/12，属“按键扫描 + 充电检测”类状态机（○）。
- 0x0800B7B0：根据状态把 PB1 在“普通输出”与“EXTI 下降沿唤醒源”之间切换（0x0800D0E4 配置、0x0800D108 撤销），即**休眠/唤醒**管理 ★。

---

## 10. 与 STM32F0 的一致性核对

| 项 | 结论 |
|---|---|
| CMSIS SystemInit（0x0800B964） | 与 `stm32f0xx` 的 `SystemInit()` **逐条、逐掩码相同** ★ → 基本可断定是 STM32F0 系（或 F0 兼容内核/外设克隆） |
| HAL 特征 | `HAL_GPIO_Init`（0x080072C4）结构体 {Pin,Mode,Pull,Speed,Alternate}、`GPIO_Mode` 常量 `0x10210000`（IT_FALLING）/`0x10110000`（IT_RISING）、`HAL_TIM_Base_Init`（0x0800BABC：CR1/PSC/ARR/RCR/EGR=UG）、IWDG KR 键值 0xAAAA/0xCCCC/0x5555、RCC_OscInitTypeDef（含 HSI14/HSI48 字段）均与 STM32F0 HAL 一致 ★ |
| RCC_CFGR3 / RCC_CR2 存在 | 只有 F0/F3 有 CFGR3/CR2；结合 M0 指令集 → **STM32F0 系**（带 HSI48 的型号，如 F042/F072 类）★○ |
| GPIO 时钟位 | AHBENR bits 17–22 = GPIOA–F（F0 定义），与代码一致 ★ |
| 疑点 1：`SYSCFG->CFGR1 |= 3` | 标准 F0 中 CFGR1[1:0]=MEM_MODE（0=主 Flash，1=系统存储，2/3 保留/厂商定义）。此处配“复制向量表到 SRAM + MEM_MODE=3”，代码注释与 Lead 判断为“SRAM 向量表重映射”；**VTOR 始终未写**，说明靠 MEM_MODE 生效（○/待确认 clone 定义） |
| 疑点 2：`RCC->CSR |= 1<<23`（main 0x0800CFFE） | 标准 F0 的 CSR 位 23 不是通用可控位（复位标志区在 24–31，由 RMVF 清除），写 1 的行为在手册中无定义；疑为**厂商扩展位**或我误判（`?`） |
| 疑点 3：ADC 引脚 | 通道 10/11/12 vs PC7/PA8 模拟输入不自洽（§7）→ 可能 clone 模拟通道映射不同（`?`） |
| 疑点 4：Flash 尺寸 | 代码引用 0x0801F000/0x0801F800，需 ≥128 KB（`?`，可能实际为 128 KB 型号或 bootloader 更大区域） |
| 疑点 5：LSE 来源 | RTC 配 LSE 32.768 kHz，但本镜像未使能 LSE；推断由 bootloader 使能（`?`） |

---

## 11. 待确认问题清单

1. **MCU 具体型号**：F0 系哪一款（是否带 HSI48 + DAC + 128 KB Flash）？“STM32F0 兼容国产 MCU”的可能性需硬件丝印/引脚 probing 确认。
2. `SYSCFG->CFGR1 |= 3` 在该芯片上的确切语义（MEM_MODE=11 是否=SRAM；是否另有 bit1 功能）。
3. `RCC->CSR |= 1<<23`（main 0x0800CFFE）的确切硬件含义（复位标志/厂商扩展位）。
4. **LSE 使能来源**：RTC 使用 LSE，但 OscConfig 未使能 LSE；确认是否 bootloader 使能，或 RTC 实际不工作（仅作唤醒/计时占位）。
5. **0x0801F000 / 0x0801F800 参数区**：实际 Flash 容量；参数块格式（100 字节/块、特征字节 0x5C）；0x0801F800 标识字与 0x1018 的含义（升级/参数有效？）。
6. **ADC 通道与引脚**：通道 3/10/11/12 分别接到哪些模拟量（电池电压/温度/充电电流/闪光管电压？）；PC7/PA8 被配成模拟脚的原因。
7. **PA7 双重用途**：0x08008A2C 配为普通输出、0x0800854C 配为 TIM3_CH2(AF1)；确认哪个是最终状态（可能分时复用：蜂鸣器 vs 背光 PWM）。
8. **PC5/PA1/PF1 的确切电源控制角色**（电源锁存、升压使能、充电使能）。
9. **PC7 上电/关机检测逻辑**的物理含义（充电器插入？电池电压比较器输出？）。
10. **射频模块具体型号**：CC2500/CC1101 兼容（寄存器/命令码吻合），但仍有 0x3E(PATABLE)、0x36/0x33 的初始化顺序、PB15 中断沿切换策略需要与协议分析（re-app 任务）对齐。
11. **LCD 控制器型号**：0x0800C954 命令序列与 0xD5/0xD9/0xDA/0xDB 等更像 **ST7567/UC1701 类**；列地址命令的 `0x02|低半字节` 需对照控制器手册确认。
12. TIM14 5 kHz 中断的完整用途（系统节拍/按键计时/蜂鸣器音源？）与主循环任务计数的对应关系。
13. 主循环 16 个任务的语义（0x080047CC、0x080098F8、0x080035A8、0x0800642C、0x080038C0、0x080065CC、0x0800896C、0x0800B394、0x0800A230、0x08003BAC、0x0800C7D4、0x080097F8、0x0800B80C 等）——需交给 UI/应用层分析（re-app 任务）。
14. 0x0800BA44（TIM14 中断）中 `0x0800C1E8`、`0x080099B0` 的具体动作。
15. 0x0800A934 中 PA4 写 0（板级初始化未把 PA4 配成输出）——是 DAC 引脚还是遗漏配置。

---

## 附录 A：关键函数/句柄一览

| 函数 | 还原名称 | 句柄/关键 RAM |
|---|---|---|
| 0x0800B964 | SystemInit（CMSIS） | — |
| 0x08003548 / 0x0800356C / 0x0800C91C | init_array / 解压器 / memset | 0x0800F3EC 表 |
| 0x080030D4 / 0x080030C0 | Reset_Handler / 第二阶段启动 | — |
| 0x0800CF74 | main | — |
| 0x080075CC / 0x080075EC / 0x08007610 | HAL_Init / HAL_InitTick / HAL_MspInit | 0x20000104(uwTick) |
| 0x0800B8F0 / 0x08007980 / 0x08007798 / 0x080078F8 / 0x08007678 | SystemClock_Config 及其子步骤 | 0x20000100(SystemCoreClock) |
| 0x08008164 / 0x08008148 | SysTick_Config / SysTick 时钟源设置 | — |
| 0x080072C4 / 0x0800741C / 0x08007426 | HAL_GPIO_Init / 读引脚 / 写引脚 | — |
| 0x0800941C | 板级 GPIO 初始化 | — |
| 0x0800A480 / 0x0800C954 / 0x08004670 / 0x0800D134 / 0x0800A30C / 0x0800C82C | LCD 写字节/初始化/列地址/复位/全屏/局部刷新 | 帧缓冲 0x200004CC |
| 0x0800CD7C / 0x0800CE38 / 0x0800CE58 / 0x0800CE0C / 0x0800CDC8 / 0x0800CCA6 / 0x0800CD32 / 0x0800CCBC / 0x0800CAE8 / 0x0800CDF4 | 射频 SPI 收发/片选/复位/寄存器读写/配置表/命令 | 0x20000214、0x20000434 |
| 0x08004AF0 | 射频初始化 | — |
| 0x0800AA74 / 0x0800B0BE | 射频数据包/命令处理 | 0x200001E1(主循环事件标志) |
| 0x08009310 / 0x08006878 / 0x080069AC / 0x08006B20 | ADC 初始化/通道配置/底层 | 0x200003AC(hadc)、0x200003F8(hdma) |
| 0x08006CEC / 0x08006CB4 / 0x080093A8 | DAC 初始化/通道配置 | 0x20000260 |
| 0x080096A8 / 0x08009708 / 0x08009768 / 0x0800967C / 0x080082F8 / 0x080082B4 / 0x080081A0 | TIM1/TIM2/TIM3/TIM14 初始化与启动 | 0x20000294/0x200002D0/0x2000030C/0x20000348 |
| 0x080095F8 / 0x08007E70 / 0x08007F38 / 0x08007F10 | RTC 初始化/反初始化 | 0x20000274 |
| 0x080095CC / 0x08007440 / 0x080074F0 / 0x08007544 | IWDG 初始化/刷新 | 0x20000384 |
| 0x08005E24 / 0x08005E48 / 0x08005E6C / 0x08005F34 / 0x08005E8C | FLASH 擦/写/等待/参数读取 | 0x20000240、0x20000434 |
| 0x0800A934 / 0x0800B9C0 | 开机/关机 | — |
| 0x0800B7B0 / 0x0800D0E4 / 0x0800D108 | 休眠/唤醒（PB1 EXTI） | 0x200001E0 |
| 0x0800CA38 / 0x0800CA98 / 0x0800CAB8 / 0x0800CA68 / 0x08009038 | 软件延时（带/不带 IWDG 刷新） | — |

## 附录 B：延时函数换算（经验值）

- 0x0800CA38(n)：内层 2×14 个 `nop` 的忙等 ×n；48 MHz 下约 **n×0.6 µs**（未标定，只用于相对等待）○。
- 0x0800CA98(cnt)：循环 cnt 次 0x0800CA38(990) ≈ **cnt×0.6 ms** ○（0x0800B9C0 用它做 100 ms 延时）。
- 0x0800CAB8/0x0800CA68：与 0x0800CA38 同构，仅内层 NOP 数不同（0x0800CAB8 无 nop 填充）。
- 0x08009038(n)：n 次 { 0x0800CA38(0x3E3) + IWDG 刷新 }，用于长等待且防看门狗复位 ★。
