# AD200 Pro 固件 静态分析进展（Lead）

固件：`firmware/AD200pro_V1.60.bin`，46380 字节，SHA256 `fc0f77f1…78bd0`。

## 1. 已确认的关键事实

| 项 | 结论 |
|---|---|
| CPU | ARM Cortex-M0（Thumb/Thumb-2，无 FPU） |
| 镜像基址 | **0x08003000**（不是 0x08000000！）——见下节推导 |
| 镜像范围 | 0x08003000 – 0x0800E52C，文件偏移 = 地址 − 0x08003000 |
| Bootloader | 0x08000000–0x08002FFF 不在本文件中（12 KiB，负责 USB 升级等） |
| Code 区 | 文件 0x0000–0x9FFF（约 40 KiB） |
| 数据区 | 文件 0xA000–0xB52B（约 5.3 KiB：查表/字符串/初始化表） |
| RAM | 0x20000000 起，最大用到 0x200008B0（SP 初值 = 0x200008B0） |
| 向量表 | 48 项（16 系统 + 32 IRQ），位于 0x08003000；运行时被复制到 RAM 0x20000000，且 SYSCFG_CFGR1 MEM_MODE=SRAM 重映射 |
| 系统节拍 | SysTick 中断（0x0800BC8C），主循环等待 0x200001CF 标志 |

### 基址 0x08003000 的推导（关键）
- 向量表[15]=SysTick=0x0800BC8D。若基址 0x08000000，则文件偏移 0xBC8C 落在数据区；
- 若基址 0x08003000，偏移 0x8C8C 恰好是可执行代码，且全部 48 个向量都落在 code 区；
- 向量表[16]=默认异常处理=0x080030E7，基址 0x08003000 时正好指向文件中连续的 `FE E7`（`b .`）桩；
- 复位向量 0x080030D5 → 偏移 0xD4 = `ldr r0,=0x0800BD0D; blx r0; ldr r0,=0x080030C1; bx r0`（先调 SystemInit，再进 startup）。

### 启动链
```
0x080030D4 reset:
    SystemInit(0x0800BD0C)          ; RCC 时钟配置（HSI，清 PLL/HSE）
    goto 0x080030C0
0x080030C0 startup:
    SP = 0x200008B0
    scatter_load(0x08003548)        ; 用 0x0800E3E4 处表初始化 .data / 清 .bss
    goto 0x0800CDB0 (main)
0x0800CDB0 main:
    copy 48 words vector table 0x08003000 -> 0x20000000
    SYSCFG_CFGR1 |= 3               ; MEM_MODE = SRAM（向量表重映射到 SRAM）
    RCC 使能 PWR 等
    init... ; PRIMASK=1
    init... ; PRIMASK=0
    while (1) {
        __enable_irq();
        while (*(u8*)0x200001CF != 1){}   ; 等待中断置位
        *(u8*)0x200001CF = 0;
        ... 主循环任务调用（约 16 个函数）...
    }
```

## 2. 外设使用（由 MMIO 数据流分析得到）

| 外设 | 基址 | 用途/证据 |
|---|---|---|
| RCC | 0x40021000 | 时钟、APB/AHB 时钟门控 |
| GPIOA/B/C/D/F | 0x48000000/+0x400… | 无 AF 配置 → 所有外设均为软件位操作 |
| EXTI | 0x40010400 | 5 条外部中断线（IRQ5/6/7 = EXTI0_1/2_3/4_15） |
| SYSCFG | 0x40010000 | EXTICR、MEM_MODE |
| ADC | 0x40012400 + 0x40012708(CCR) | 电容电压/电池等模拟量 |
| DMA1 | 0x40020000 | 仅 CH1（IRQ9）→ 很可能是 ADC 连续采样 |
| TIM17 | 0x40014800 | 定时 |
| FLASH | 0x40022000 | 内部参数存储（解锁/擦写） |
| PWR | 0x40007000 | 低功耗 |
| SCS/SysTick/NVIC | 0xE000E000 | 系统节拍、中断控制 |
| RTC/BDCR | 0x40002800 | 备份域（可能用于掉电计时/标志） |

**未出现 USART/SPI/I2C/USB**（APP 中）→ 对 PC 的固件升级由 0x08000000 处 bootloader 负责；
APP 与外部 RF 模块/显示之间的通信必须是 **GPIO 软件时序**（待 pin map 确认）。

### 可疑的软件时序/接口
- 0x08009758（154 条）初始化 GPIOF/GPIOC/GPIOA/GPIOB + EXTI（调用 0x8006D3C）+ NVIC(IRQ5/IRQ7)；
  配置参数类似「端口/引脚/上下拉/触发沿」表 → 很可能是 2.4G 接收数据/输入捕获（软件解码）或按键。
- 0x08006D3C = `EXTI_Config(gpio_port, cfg_struct)`：写 GPIO MODER/PUPDR、SYSCFG_EXTICR、EXTI IMR/EMR/RTSR/FTSR。
- 0x08006E9E = `GPIO_WritePin(port, mask, level)`：level?BSRR:BRR。
- 0x0800B888 / 0x08008778 / 0x0800CA2C / 0x0800CAB0 / 0x0800CAEC / 0x0800CBA8 大量 GPIO 位操作 → 候选：LCD 段驱动、RF 软 SPI、按键扫描。
- ADC 初始化 0x0800356C；ADC+DMA 读取 0x0800641C / 0x080068A4。

## 3. 已恢复的函数（323 个）

- `analysis/disasm.txt` — 全量带注释反汇编（含字面量/字符串/MMIO 注解）。
- `analysis/functions.json` — 函数起止、调用关系、MMIO、字面量、字符串。
- `analysis/mmio.json` — 按寄存器聚合的访问点。
- `analysis/summary.txt` — 汇总。
- `analysis/symbols.json` — 地址→符号名（供模拟器/调试器使用，持续更新）。

分析脚本：`tools/analyze.py`（递归 CFG 反汇编 + 常量传播 + MMIO 识别 + 函数指针发现）。
Python（含 capstone/unicorn）：`..\..\tools\python\python.exe`。

## 4. 待办 / 开放问题

1. GPIO pin map：从各端口 MODER/ODR 初值 + 位操作顺序推导（LCD？RF？按键？充电？IGBT？）。
2. 数据区（0xA000–0xB52B）结构：查找表（电源/闪光功率）、初始化表、字符串。
3. 确认 0x0800E3E4 处 scatter 表结构（.data 拷贝源/目标/长度）。
4. SysTick 处理链（0x8007034、0x8007CB4）与 1ms 时基。
5. 0x800BE1C/0x800BE58/0x800BDE0（IRQ13/15/19）的真实用途。
6. 内部 Flash 参数存储布局（FLASH_KEYR/CR 0x080058EC/0x08006C6C 等）。
