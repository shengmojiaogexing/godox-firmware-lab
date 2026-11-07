# 06 启动链与外设最小语义（启动探针结论）

> 本文是 `sim/` 启动实现与验收测试的**证据依据**，来自两处独立探针：
> 1. Unicorn/Cortex-M0 仿真语义探针（本文 §1，见 `tools/t_uc.py` 与临时探针）；
> 2. 独立最小参考实现 `tools/ref_boot.py`（不走 `sim/`，用最小外设模型跑固件）；
> 另附 Lead 的 `tools/boot_probe.py` 卡死枚举结果与 `sim/boot_trace.py` 的当前观测。
>
> 固件：`firmware/AD200pro_V1.60.bin`，镜像基址 `0x08003000`，46380 字节。

---

## 1. Unicorn 2.1.4 在 Cortex-M0 上的实测语义

### 1.1 模式选择：必须 `UC_MODE_THUMB | UC_MODE_MCLASS`，且起始地址一律 `|1`

| 实验 | 结果 | 结论 |
|---|---|---|
| `Uc(UC_ARCH_ARM, UC_MODE_THUMB)` 跑 `msr primask, r0` | `UC_ERR_INSN_INVALID` | 纯 THUMB 模式**不认** M-profile 的 `msr/mrs`；固件 main 0x0800CE0C、0x0800CE48 用它关/开中断 → 无法启动 |
| `Uc(..., UC_MODE_THUMB \| UC_MODE_MCLASS)` + `msr/mrs primask` | OK，`UC_ARM_REG_PRIMASK` 读到 1 | **必须带 MCLASS**，且 PRIMASK 可原生读写 |
| `UC_MODE_THUMB \| UC_MODE_MCLASS`，`emu_start(pc)` 传偶地址 | `UC_ERR_INSN_INVALID`（`tools/t_uc.py` 的现象） | MCLASS 下起始地址必须带 Thumb 位 |
| 同上但 `emu_start(pc \| 1)` | 从 reset 连续执行 2000 条，进入 scatter 拷贝（0x0800C736） | **MCLASS 在 2.1.4 下完全可用**；`tools/t_uc.py` 的「MCLASS 不可用」结论是漏了 `\|1` 的误判 |

> **结论（与 Lead 早期笔记相反，已实测）**：本模拟器必须使用
> `UC_MODE_THUMB | UC_MODE_MCLASS`，并且每次 `emu_start` 的 `begin` 参数
> 一律传 `pc | 1`。`tools/ref_boot.py` 即按此实现并成功跑过启动早期阶段。

### 1.2 异常 / EXC_RETURN

| 实验 | 结果 |
|---|---|
| 手动压 8 字帧、`LR=0xFFFFFFF9`、跳 handler，handler 执行 `bx lr` | 抛 `UC_ERR_EXCEPTION`（MCLASS）/ `UC_ERR_FETCH_UNMAPPED`（THUMB） |
| 在 `UC_HOOK_CODE` 里把 PC 改到 handler 地址 | 生效：hook 返回后从新 PC 执行（重定向可行，无需 emu_stop 重入） |
| `0xFFFFF000` 4 KiB 页 `mem_map` | 可映射；`0xFFFFFF00` 不可（4 KiB 对齐要求） |

**实现要求**：必须在 `bx lr` / `pop {...,pc}` **执行前**判定目标值是否
`EXC_RETURN`（`0xFFFFFFF1/F9/FD`），是则手动弹 8 字帧并跳过该指令
（PC 已在 hook 内改写为恢复地址）。靠「先让 PC 变成 EXC_RETURN 再兜底」不可靠。

异常入口按 ARMv6-M：`SP = (SP - 32) & ~7`，帧顺序 `{R0,R1,R2,R3,R12,LR,PC,xPSR}`，
`LR=0xFFFFFFF9`。**注意先减 32 再对齐**：`SP` 为 4 模 8 时必须落在 `(SP-32)&~7`，
否则会覆盖函数已保存的 `{r4,lr}`（本探针曾因此把返回地址写成 0 而跑飞）。

### 1.3 MMIO 方案（「后备内存 + 读钩子预写」已验证）

把外设区间 map 成普通内存，然后用 hook 做语义：

* `UC_HOOK_MEM_READ` **在 load 之前**触发；在回调里 `mem_write` 会**影响本次读到的值**
  （实测：memory 里 0xAAAAAAAA，读钩子写入 0x55667788，CPU 读到 0x55667788）。
  → 动态位（SWS/COUNTFLAG/IDR/ISR）在每次读时重算并写回即可。
* `UC_HOOK_MEM_WRITE` **在真正落盘之前**触发（回调里读到的是旧值）。
  → 写侧语义要用「地址 + 即将写入的值」，或像 `sim/peripherals.py` 那样把写地址
  入队、下一次读时 flush（flush 时才读得到新值）。

### 1.4 性能（实测，单核本机）

| 场景 | 吞吐 |
|---|---|
| 简单 Python `UC_HOOK_CODE`（只计数） | ≈ **3.0 M insn/s**（1M 条 0.31–0.33 s） |
| 代码钩子内每条指令读一次 unicorn 寄存器（如 PRIMASK） | 掉到 ≈ **0.3 M insn/s**（cProfile：2M 指令 1.95M 次 reg_read，占 5.7 s） |
| 建议 | 用「扫描 flash 得到 `msr primask` 地址集合 + 脏标记」缓存 PRIMASK；只在待注入中断时才读一次 |

**启动预算**：固件启动包含大量 `HAL_Delay`/`delay_us` 软件延时。若 `1 insn = 1 cycle`，
48 MHz 下 1 ms = 48000 条指令，启动前 250 ms 几乎全是延时循环 → 需要 10 M+ 条指令。
建议 `cycles_per_insn ≈ 3–4`（或像 `sim/machine.py` 用 `systick_insn_per_ms=2000`
压缩时基），在「不追求周期精确」的前提下把启动压到百万级指令。

---

## 2. 固件启动事实（静态 + 实测）

### 2.1 向量表（48 项，位于 0x08003000）

| idx | 名称 | 值 | idx | 名称 | 值 |
|---|---|---|---|---|---|
| 0 | initial_sp | 0x200008B0 | 16 | IRQ0 | 0x080030E7 (`b .`) |
| 1 | Reset | 0x080030D5 | 17 | IRQ1 | 0x080030E7 |
| 2 | NMI | 0x0800A25D | 18 | IRQ2 (EXTI2_3?) | 0x0800AC0D |
| 3 | HardFault | 0x08008201 | 21 | IRQ5 EXTI0_1 | 0x080053F1 |
| 11 | SVC | 0x0800B81D | 22 | IRQ6 EXTI2_3 | 0x08005479 |
| 14 | PendSV | 0x0800A90D | 23 | IRQ7 EXTI4_15 | 0x08005485 |
| 15 | SysTick | 0x0800BC8D | 25 | IRQ9 DMA_CH1 | 0x080044D1 |
| | | | 29 | IRQ13 TIM1 | 0x0800BE1D |
| | | | 31 | IRQ15 TIM2 | 0x0800BE59 |
| | | | 35 | IRQ19 TIM14 | 0x0800BDE1 |
| 其余 IRQ | 默认 | 0x080030E7 (`b .`) | | | |

### 2.2 reset → main

```
0x080030D4 reset:
    ldr r0,=0x0800BD0D ; blx r0       ; SystemInit(0x0800BD0C)
    ldr r0,=0x080030C1 ; bx  r0       ; -> 0x080030C0
0x080030C0 startup:
    SP = 0x200008B0
    bl scatter_load(0x08003548)
0x08003548 scatter_load（表在 0x0800E3E4，2 项 × 16 字节）:
    {src=0x0800E404, dst=0x20000100, len=0x128, fn=0x0800C720}  ; .data
    {src=0x0800E52C, dst=0x20000228, len=0x688, fn=0x0800C730}  ; .bss/清 0
    -> 拷完跳 0x080030C8 -> main = 0x0800CDB1
```

注意：`.bss` 结束地址正好 = 0x200008B0 = 初始 SP（栈与 .bss 相邻，栈向低地址增长）。

### 2.3 main（0x0800CDB0）与主循环

* 0x0800CDB0–0x0800CDC4：把 48 个字（向量表）从 0x08003000 拷到 RAM 0x20000000；
* 0x0800CDC6：`SYSCFG_CFGR1 |= 3`（MEM_MODE=SRAM）→ **异常入口从 0x20000000 的 RAM 表取**；
* 初始化调用链（顺序）：
  `0x08007044(FLASH_ACR + SysTick 时基系)`, `0x0800BC98(时钟/外设总 init)`,
  `0x08009758`, `0x08009730`, `0x080099C0`, `0x08009698`, `0x080098E0`, `0x0800993C`,
  `0x08009A4C`, `0x08009AAC`, `0x080099EC`, `0x08007A30`, `0x08006FBC`, `0x08006F68`,
  `0x0800AC24`, `0x0800AC58`, `0x0800BD68`, `0x0800AAF4`；随后 `msr primask,#1`
  → 配置 → `msr primask,#0`；
* 主循环 **0x0800CE46–0x0800CEA2**：
  `msr primask,0` → `ldrb 0x200001CF; cmp #1; bne 0x0800CE46` → 清标志 →
  约 16 个任务函数（0x08006F68, 0x08008818, 0x08004500, 0x08003598, 0x08005A04,
  0x08005F6C, 0x0800389C, 0x08003904, 0x08009228, 0x08006118, 0x08008364,
  0x0800B5B0, 0x0800A2CC, 0x08003BEC, 0x0800C680, 0x0800BA00, 0x08009CF0,
  0x08009B38, 0x0800BA5C）→ `b 0x0800CE46`。

### 2.4 SysTick / 时基（实测值）

* `0x08007C84`: `SysTick_Init(reload)` → `RVR = reload-1`，`CVR=0`，`CSR=7`
  （ENABLE|TICKINT|CLKSOURCE），并用 `0x0800A310` 设置 SysTick 优先级（SHPR3）。
* 调用点 0x0800BCE6：`r0 = SystemCoreClock` → `/1000` → `SysTick_Init()`。
  **实测 RVR = 47999 → SystemCoreClock = 48 MHz，1 ms 节拍**。
* SysTick handler `0x0800BC8C`：`bl 0x08007034`（`*(u32*)0x20000100 += 1`，
  即 HAL tick 计数器）→ `bl 0x08007CB4` → 返回。
* `HAL_GetTick` = `0x08006EAC` → 返回 `*(u32*)0x20000100`；
  `SystemCoreClock` 变量 getter = `0x08007398` → 返回 `*(u32*)0x20000104`。
* 固件里所有 `while(!ready){ if (tick-start > N) return 错误; }` 都依赖
  这两个 RAM 变量 + SysTick 正常推进。

---

## 3. 启动必须的外设最小语义（按实测踩坑顺序）

> 这些是「不满足就永远跑不到主循环」的硬要求；其余寄存器写后读回即可。

### 3.1 RCC —— 时钟就绪位必须「1–3 次读内」就绪

* `RCC_CR (0x40021000)`：`HSION→HSIRDY(bit1)`、`HSEON→HSERDY(bit17)`、
  `PLLON→PLLRDY(bit25)`；
* `RCC_CFGR (0x40021004)`：`SWS[3:2] = SW[1:0]`（`0x08007244` 用它判断当前时钟源）；
* `RCC_CSR (0x40021024)`：`LSION(bit0) → LSIRDY(bit1)`；
* `RCC_BDCR (0x40021020)`：`LSEON → LSERDY`；
* `RCC_CR2 (0x40021034)`：写入的 HSE14ON(bit16) 必须能读回（`0x0800742C` PLL 路径会回读该位）。

**关键**：`0x0800742C`（HAL_RCC_OscConfig 风格）与 `0x08007244`（ClockConfig 风格）
都用 HAL tick 做 100 单位超时；**任何 ready 位不满足 → 函数返回 1/3 → main 初始化
在 `0x0800BCB4` / `0x0800BCCE` 的 `b .` 死循环**。
这不是「等外设」的 stall：自动 unstick 去写 `RCC_CSR` 不会有用（Lead 的
`analysis/boot_probe.json` 里 0x0800BCB4 反复 stall 就是这个原因）。
正确做法是让 `0x0800742C` 返回 0：即所有 ready 位在 1–3 次读内就绪。

### 3.2 RTC —— `RTC_WaitForSynchro` 必须看到 RSF=1

`0x08007C34`：先 `RTC_ISR &= ~(RSF|INITF)`（清 bit5/bit7），然后轮询 **RSF(bit5)=1**，
超时 1000 ms 返回 3。要求：

* 读 `RTC_ISR (0x40002800+0x0C)` 返回 `ALRAWF|ALRBWF|WUTWF (bit0..2)` 恒 1 且
  `RSF(bit5)=1`；
* `INIT(bit6)` 置位时回读 `INITF(bit7)=1`（进初始化模式）；
* `RTC_CR(0x40002808)` 写后读回。

否则启动卡在 `0x08007C4E–0x08007C62`（每 1 s 超时一次，实测读 ISR 200 次）。

### 3.3 ADC —— ADRDY/ADDIS/ADSTART 的握手

* `0x0800356C ADC_Init`：写 `CR/CFGR1/CFGR2/SMPR`、`TR=0x0FFF0000`、`CHSELR`、`CCR`；
* `0x080037B2 ADC_Enable`：`CR.ADEN=1` → `ISR.ADRDY(bit0)=1`；
  之后写 `CR.ADDIS(bit1)` → **必须清 ADEN**，否则 0x0800380C 等 ADEN=0 死等；
* `0x080036D0`：等 `CR.ADSTART(bit2)=0`（转换结束）；
* `0x08003818`：等 `ISR.ADRDY`。

**关键**：单次转换（`CFGR1.CONT=0`）完成后 **ADSTART 必须硬件自清**。
否则实测出现 `ADC_CR` 被读 17.4 万次的死等。
`ADEN→ADRDY`、`ADSTART→转换(EOC/EOS, DR=0x800/通道值)`、`ADSTART` 清零即可。

**本机实测寄存器地址与 DMA 约定（不要照搬 STM32F0 参考手册）**：

| 项 | 地址/约定 | 说明 |
|---|---|---|
| `ADC_DR` | **0x40012440** | 实测数据寄存器；F0 RM 里常写的 0x40012450 是错的 |
| `ADC_CHSELR` | **0x40012428** | 固件 ADC_Init 写 0x28；`TR` 在 0x40012420（写 0x0FFF0000） |
| `ADC_CCR` | 0x40012708 | 时钟/预分频/内部通道使能 |
| ISR 位 | ADRDY=bit0、EOC=bit2、EOS=bit3 | ADEN→ADRDY；ADDIS(bit1) 清 ADEN；单次转换后 ADSTART(bit2) 自清 |
| DMA | `CPAR=0x40012440` 时按 `CNDTR` 批量填样本 | 每样本 2 字节写入 `CMAR`；循环模式重装 CNDTR，非循环则 CNDTR=0、CMAR 前移；置对应通道 TCIF/GIF |

固件 ADC_Init 写 `CR/CFGR1/CFGR2/SMPR/TR/CHSELR/CCR` 后，由 ADC_Enable 完成
`ADEN→ADRDY`、`ADDIS→清 ADEN`；采样序列通过 **DMA（CPAR=ADC_DR）** 搬到 RAM
（实测 DMA 通道 CCR 的 EN/CIRC 与 CPAR/CMAR/CNDTR 均被配置）。

### 3.4 FLASH —— 必须映射到 0x08020000（128 KiB），参数区在 0x0801F000

固件在 **0x0801F000**（128 KiB flash 最后一页）扫描参数区：
`0x0800AC58 → 0x0800590C` 从 `0x0801F000 + 100*i`（i=0x13..0）读 8 字参数结构，
校验首字节 `0x5C`。若 flash 只映射到 0x08010000，会直接
`UC_ERR_READ_UNMAPPED`（实测）→ **flash 大小按 0x20000 映射**，
0x0800E52C 之后到 0x08020000 填 0xFF（参数区未初始化 = 全 FF，走默认值）。

`FLASH_SR(0x4002200C)`：BSY=0、EOP=1；`KEYR` 解锁；`CR.STRT` 完成后置 EOP。

### 3.5 GPIO 软件位操作总线（RF 2.4G，非 SPI 外设，全靠 GPIO 翻转）

由反汇编确认的引脚映射（GPIOB/GPIOC，均为软件位操作）：

| 信号 | 端口/引脚 | 位 | 代码证据 |
|---|---|---|---|
| MOSI | GPIOB pin12 | BSRR/BRR `0x1000` | 0x0800CAEC (`rf_spi_write_byte`) |
| CLK  | GPIOB pin13 | BSRR/BRR `0x2000` | 0x0800CAEC / 0x0800CAB0 |
| MISO | GPIOB pin14 | 读 `IDR` 后 `<<17` 取符号 | 0x0800CAEC/0x0800CAB0/0x0800CBA8 |
| CS   | GPIOC pin6  | `0x40` | 0x0800CBA8/0x0800CBC8 |

函数：`0x0800CAEC`=SPI 全双工交换 1 字节（先送 MOSI，再置 CLK 上升沿并采样 MISO）；
`0x0800CAB0`=读 1 字节；`0x0800CBA8`=片选拉低**并等待 MISO(B14)=0**；
`0x0800CBC8`=片选拉高；`0x0800CB38`=写命令字节+读回 len 字节；
`0x0800CB64`/`0x0800CB7C`=命令封装。

时序（反汇编确认）：CS 低 → 每 bit 先摆 MOSI、再 CLK 上升沿（高电平后采样 MISO）
→ **SPI mode 0（CPOL=0, CPHA=0），MSB first**，与 CC2500 4 线 SPI 一致。

**RF 芯片身份**：初始化命令/寄存器回读序列与 **TI CC2500** 完全吻合
（GODOX 2.4G 常用方案）；Lead 已在 `sim/cc2500.py` 按 CC2500 寄存器/命令集建模
（CSN=PC6、SCK=PB13、MOSI=PB12、MISO=PB14，与上表一致）。

**实测结论（曾 blocking，已解决）**：CC2500 模型接入前，`python -m sim.boot_trace
6000000` 时固件仍在 RF 初始化 SPI 循环里（top PC page `0x0800CBxx`，`main_hits=0`）。
最终并不是靠"补全 CC2500 寄存器协议"解决的，而是 §3.6 定位到的 RF bring-up 死锁，
按 §3.9 的方式（GPIO PUPDR 上拉默认值 → PA5 读 1 → 走 skip 分支）解除。

### 3.6 启动卡死的真正位置：RF bring-up 的 RAM 握手标志（已定位，实测）

单步 / block-hook / RAM 写钩子定位到 **`0x0800AAF4`（main 初始化链最后一个调用，
RF bring-up）**：

```
0x0800AAF4 rf_bring_up():
  *(u8*)0x20000205 = 1;                       ; 0x0800AAFC
  if (read GPIOA pin5 == 0) n++  (PA5=EXTI5/RF status, 10 ms 后再读一次)
  if (n != 2) goto done;                      ; PA5 高 => 跳过整个 RF bring-up
  ...  GPIOB pin2/pin4 置 1、配置 CC2500（SIDLE/SCAL/delay/PWR/SWORRST）...
loop:                                          ; 0x0800AB78
  rf_tx_packet(0x50, 0xB4, 2)                  ; -> 0x0800A934 -> 0x0800C1C4
  iwdg_refresh(handle @0x20000248)             ; -> 0x08006F68, Instance=0x40003000
  if (*(u8*)0x20000205 != 0) goto loop         ; 0x0800AB88
done:
  rf_to_rx(); *(u8*)0x20000205 = 0;            ; 0x0800AB8E / 0x0800AB92
```

每次 loop 的 `0x0800C1C4`：GPIO PC7/PC8 翻转（RF 复位/唤醒）→ 向 CC2500 FIFO
burst 写 4 字节 `{0xA9, k, 0xB4, 0x02}` → `STX(0x35)` → 忙等延时 600 →
`SWORRST(0x3B)`。实测 300k 指令内该 loop 已执行 96 次、1.5M 指令时 2209 次，
永不退出。

**关键实测**：
1. 该 loop 内 `PRIMASK=1`（无中断可运行），RAM 写钩子显示 **没有任何代码把
   `0x20000205` 清零**（只有包缓冲区 `0x20000206..` 被写）；
2. **决定性的验证**：若在 `0x0800AB88` 处把 `0x20000205` 强制清 0（等价于
   "RF 已就绪/跳过 bring-up"），固件 **在约 10 万条指令内到达主循环**，
   随后 120k 指令内主循环执行 2018 次、无 fault —— 说明启动链其余部分完全正常；
3. 若把 GPIOA pin5 置 1（读作高电平），固件按 PA5 判断**自行跳过 RF bring-up**，
   也在约 8 万条指令到达主循环（但随后在 0x20000206 出现一次 fault，
   详见 §3.7 的 exception-return bug，修复后不再复现）。

**修复后的实测结果**（对应 §3.7 的 SP 修复 + §3.9 的 PA5 上拉方案，均已合入 `sim/`）：
主循环到达 insns=**80,000（0.06 s）**；随后稳定阶段采样到 **10 次循环进入、
uptime=**265 ms**、irq_enter=225、无 fault** —— 满足 SIM-SPEC §7.1
（5M 指令内、≥10 循环、uptime≥10ms、<30s）。`tests/test_boot.py` PASS。

### 3.7 machine.py 异常返回 bug（实测，导致主循环后跑飞）

现象：boot 到达主循环后，约 insn=137,500 出现
`Invalid instruction at 0x2000087C`（PC 跑进 RAM 栈区，LR=0x0800CB1D）。

根因（block-ring + 异常日志定位）：异常返回**丢了 4 字节对齐修正**。

* `_exception_entry` 用 `frame_sp = (thread_SP - 32) & ~7`；当线程 SP ≡ 4 (mod 8)
  （很常见：`rf_spi_write_byte` 0x0800CAEC push {r4,r5,r6,r7,lr} 共 5 个寄存器）
  时，frame_sp 会比 `thread_SP - 32` 再低 4 字节；
* `_exception_return` 却用「当前 SP + 32」恢复 SP → 恢复成 `frame_sp + 32`，
  而真实线程 SP 是 `frame_sp + 36` → **返回后 SP 偏低 4 字节**；
* 之后函数 `pop {r4,r5,r6,r7,pc}` 错位弹栈，pc 取到本应属于 r7 的槽
  → PC=0x2000087C（栈上数据）。

**精确修复**：

```python
# 异常入口：
self.exc_thread_sp = uc.reg_read(UC_ARM_REG_SP)          # 保留 4 字节错位
self.exc_frame_sp  = (self.exc_thread_sp - 32) & ~7
# 帧写到 exc_frame_sp，SP 设为 exc_frame_sp
# 异常返回：
frame = struct.unpack('<8I', uc.mem_read(self.exc_frame_sp, 32))   # 不用当前 SP
... PC = frame[6] & ~1 ...
uc.reg_write(UC_ARM_REG_SP, self.exc_thread_sp)          # 恢复原线程 SP
```

另外：handler 用 `pop {...,pc}` 返回时，pop 已执行、SP 已上移 4*n 后才会 fetch
EXC_RETURN；若用当前 SP 读帧会整体偏移（这是另一处曾出现的 PC=0x20000206 的原因）。
用入口记下的 `exc_frame_sp` 读帧，可同时覆盖 `bx lr` 与 `pop {...,pc}` 两种返回方式。
更稳的做法是在 code hook 里对目标 ∈ {0xFFFFFFF1,F9,FD} 做**执行前**拦截
（`tools/ref_boot.py` 即如此实现）。

**验证**：打上上述补丁后，连续运行 **1,000,000 条指令、542 次主循环、uptime=500 ms，
无 fault**（临时补丁脚本实测）。

**最终实现（已合入 `sim/machine.py`）**：`_exception_entry` 记录
`self.exc_thread_sp`（异常前 SP，保留 4 字节错位）与
`self.exc_frame_sp = (thread_sp - 32) & ~7`；`_exception_return` 始终从
`exc_frame_sp` 读 8 字帧、并把 `SP` 恢复为 `exc_thread_sp`。

### 3.8 主循环阶段的实际节拍源是 TIM14

主循环期间 trace 反复出现 `TIM14_SR/TIM14_DIER` 读写，且 `bus.systick.enabled=False`；
超时/任务节拍由 **TIM14（IRQ19，handler 0x0800BDE1）** 驱动，SysTick 只在初始化
早期用于延时。因此 TIM14 的 UIF/DIER 与 NVIC 使能必须工作（已确认当前实现 OK）。

### 3.9 最终解决方式（已实现，2026-11-07）

RF bring-up 死锁的**最终解决方案 = GPIO 输入模型按 PUPDR 返回上拉默认高电平
（不 patch 固件）**：

* `PeripheralBus._refresh_idr()`：对 `MODER=input` 且无外部驱动的引脚，按 `PUPDR`
  返回 `1`（pull-up，01）/ `0`（pull-down 或浮空 10/00）；输出脚仍回读 ODR。
* 固件 `board_init_inputs` 把 **PA5（EXTI5，RF 状态输入）配置为输入+上拉**；
  真机上 CC2500 未响应时该线为上拉高电平。
* PA5 读 1 → `0x0800AAF4` 内两次判读都不为 0 → `n != 2` → 直接走 done 分支
  （**跳过整个 RF bring-up**）→ 启动继续到主循环。
* 实测：boot 到主循环 insns=**80,000（0.06 s）**；无需清 `0x20000205`、
  无需 patch `0x0800AAFC`。这与真机行为一致（RF 模组不应答时 PA5=1）。

**备选方案**（仅当以后需要强制走完整 RF bring-up / 或 PA5 被外部驱动为低时）：
* **A**：`run()` 的 stall 诊断/`device.resolve_stall()` 识别
  `PC∈[0x0800AB78,0x0800AB8C]` 且轮询 `0x20000205`，将该字节清 0 后重试
  （符合 spec §3「卡死诊断 + resolve_stall」）；
* **B**：ROM patch `0x0800AAFC`（`strb r0,[r5,#5]`）跳过置位；
* **C**：补 CC2500 GDO/EXTI5（IRQ7 → 0x08005484 → 0x0800A954）行为；但该 loop 内
  `PRIMASK=1`，中断不可达，除非 loop 中某处会临时开中断，故此路径存疑。

> 结论：**CC2500 寄存器模型本身不是唯一缺口**——真正的死锁是"等待 RAM 标志
> 0x20000205 被外部清 0"。最终用 §3.9 的 **PUPDR 上拉（PA5=1 → skip 分支）**方案
> 跑通，未改动固件；A/B/C 仅作为以后需要强制完整 RF bring-up 时的备选。

---

## 4. 独立参考实现 `tools/ref_boot.py`

为与 `sim/` 解耦地验证启动语义，写了最小参考实现（约 500 行，只用 unicorn + stdlib）：

```powershell
python tools/ref_boot.py --max-insns 8000000 --cycles-per-insn 1
python tools/ref_boot.py --json analysis\ref_boot.json   # 可选输出
```

它实现了 §1 的语义 + §3.1–3.5 的 RCC/RTC/ADC/FLASH/GPIO 最小模型与 SysTick 异常
注入/返回，输出：到达主循环的指令数、循环次数、SysTick RVR/tick、top PC、被轮询的
MMIO 地址（含 PC）、卡死诊断（含 unstick 记录）。

它与 `sim/` 互相印证：RCC/RTC/ADC/FLASH 语义修好后能通过全部启动早期等待，
最终同样停在 **RF bring-up 的 `0x20000205` 等待**（§3.6）；因为该脚本的 GPIO 输入
模型没有实现 PUPDR 上拉默认值，PA5 读作 0，于是进入了 RF bring-up 分支。
这恰好反证了 §3.9 的结论：`sim/` 用 PUPDR 上拉（PA5=1 → skip 分支）是启动成功的
关键，而不是 CC2500 寄存器协议。

> 后续若要用 ref_boot 跑到主循环：给它的 `Device`/GPIO 模型补一行
> “input 脚按 PUPDR 返回上拉默认 1”即可（与 `sim/peripherals.py::_refresh_idr` 一致）。

---

## 5. 复现与判读速查

| 命令 | 用途 |
|---|---|
| `python -m sim.boot_trace 2000000` | 打印每 20k 指令的 PC/状态，看卡在哪个函数 |
| `python tools/ref_boot.py --max-insns 5000000` | 独立参考实现跑启动，输出 hot MMIO/PC |
| `python tests/test_boot.py` | 验收：5M 指令内到主循环、≥10 循环、≥5 SysTick、uptime≥10ms、<30s |
| `python tests/test_debug.py` | 验收：断点/单步/内存/反汇编/trace/backtrace/虚拟整机 |
| `python tests/run_all.py` | 汇总 |

状态判读经验：

* PC 停在 `0x0800BCB4` / `0x0800BCCE` / `0x0800BCE4` → `0x0800742C`/`0x08007244`/`0x08007124`
  返回了错误（ready 位不满足），不是死等；
* PC 在 `0x080038xx` 且反复读 `ADC_CR/ISR` → ADC 握手（ADRDY/ADDIS/ADSTART）不完整；
* PC 在 `0x08007C4E–0x08007C62` 反复读 `RTC_ISR` → 缺 RSF；
* PC 在 `0x0800CAxx–0x0800CBxx` 反复读写 GPIOB/GPIOC → RF 软件 SPI 等待/重试；
* PC 在 `0x0800C7xx` nop 延时 → `delay_us` 类忙等（可安全跳过/压缩）。

## 6. 待办 / 风险

1. RF SPI slave 寄存器协议（nRF24 类命令）未逆向完 —— 当前唯一阻塞主循环的模型缺口；
2. 固件启动含较多软件延时，需要用 cycles/insn 压缩或 `delay` 函数旁路，保证 <30 s；
3. 中断优先级/嵌套暂按「SysTick 优先 + 不嵌套」近似（固件 SysTick 固定 1 ms，够用）；
4. `0x0800E52C–0x08020000` 参数区按全 0xFF（未烧写）处理，固件会走默认参数路径。

## 7. 当前验收状态（2026-11-07，实测）

按本文结论修完后，`sim/` 已能启动到主循环并稳定运行：

| 校验 | 结果 |
|---|---|
| `python -m sim.boot_trace 300000` | `reason=main_loop main_hits=3 insns=120000 uptime=60.0ms` |
| `python tests/test_boot.py` | **PASS**：主循环到达 insns=**80,000**、墙钟 **0.06 s**、稳定阶段循环 **10** 次、uptime=**265 ms**、irq_enter=**225**、无 fault |
| `python tests/test_debug.py` | **PASS**（step/bp/mem/disasm/trace/backtrace/device 全过） |
| `python tests/run_all.py` | **2/2 通过** |

本次实测中修掉的关键点（对应 §3.6/§3.7/§3.5）：
1. 异常返回丢失 `SP` 的 4 字节对齐修正 → 主循环后 `pop {r4,r5,r6,r7,pc}` 错位、
   PC 跑飞（§3.7）；修复后 1M 指令无 fault；
2. RF bring-up 对 `*(u8*)0x20000205` 的死等（§3.6）→ 以"无 RF 从机响应则跳过
   bring-up"的方式解除；
3. RCC/RTC/ADC(ADRDY/ADDIS/ADSTART 自清)/FLASH(128 KiB 映射) 语义（§3.1–§3.4）；
4. CC2500 位操作 SPI（§3.5）与 GPIO PUPDR/IDR、TIM14 节拍（§3.8）。

> 说明：主循环阶段的实际节拍源是 **TIM14(IRQ19)**，不是 SysTick（SysTick 只在初始化
> 早期用）；`test_boot.py` 因此以「irq_enter ≥ 5」判定中断在正常投递（并打印说明）。
> `tools/ref_boot.py` 保留为独立对照实现，便于之后回归时与 `sim/` 交叉验证。

## 8. 复现命令速查（最终）

```powershell
cd devices\ad200pro
..\..\tools\python\python.exe run_sim.py sim.boot_trace 300000      # 启动自检
..\..\tools\python\python.exe tests\test_boot.py            # 验收 1
..\..\tools\python\python.exe tests\test_debug.py           # 验收 2
..\..\tools\python\python.exe tests\run_all.py              # 汇总
..\..\tools\python\python.exe tools\ref_boot.py --max-insns 1000000   # 独立对照
```
