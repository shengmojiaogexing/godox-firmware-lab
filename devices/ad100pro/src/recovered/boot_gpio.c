/*
 * boot_gpio.c — Godox AD100 Pro V1.2 固件启动/时钟/GPIO/底层驱动还原
 * -----------------------------------------------------------------------------
 * 目标固件 : firmware/AD100pro_V1.2.bin (50248 bytes)
 * 加载基址 : 0x08003000 (0x08000000-0x08002FFF 为未提供的 bootloader)
 * MCU      : STM32F0 系 / Cortex-M0 (48 MHz, HSI48 作 SYSCLK, 带 HSI14/HSI48/DAC)
 * 反汇编源 : out/disasm.txt (地址均为镜像内地址)
 *
 * 还原原则:
 *   - 以反汇编可见行为为准; 结构体/枚举按 STM32F0 HAL 的语义命名, 参数为实测值。
 *   - 未能 100% 确认处标注  /*?*/  或  /* 推断 */。
 *   - 本文件是分析产物, 不可直接编译 (缺少 HAL 头文件与完整类型定义);
 *     仅保留与反汇编一一对应的逻辑骨架, 便于人工阅读与移植。
 * -----------------------------------------------------------------------------
 */

#include <stdint.h>

/* ============================================================================
 * 0. 基础地址 / 寄存器宏 (STM32F0 寄存器映射, 见 out/periph_map.json)
 * ==========================================================================*/
#define PERIPH_BASE        0x40000000UL

#define RCC_BASE           0x40021000UL
#define RCC_CR             (*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_CFGR           (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_CIR            (*(volatile uint32_t *)(RCC_BASE + 0x08))
#define RCC_APB2RSTR       (*(volatile uint32_t *)(RCC_BASE + 0x0C))
#define RCC_APB1RSTR       (*(volatile uint32_t *)(RCC_BASE + 0x10))
#define RCC_AHBENR         (*(volatile uint32_t *)(RCC_BASE + 0x14))
#define RCC_APB2ENR        (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR        (*(volatile uint32_t *)(RCC_BASE + 0x1C))
#define RCC_BDCR           (*(volatile uint32_t *)(RCC_BASE + 0x20))
#define RCC_CSR            (*(volatile uint32_t *)(RCC_BASE + 0x24))
#define RCC_AHBRSTR        (*(volatile uint32_t *)(RCC_BASE + 0x28))
#define RCC_CFGR2          (*(volatile uint32_t *)(RCC_BASE + 0x2C))
#define RCC_CFGR3          (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_CR2            (*(volatile uint32_t *)(RCC_BASE + 0x34))

#define PWR_BASE           0x40007000UL
#define PWR_CR             (*(volatile uint32_t *)(PWR_BASE + 0x00))

#define FLASH_R_BASE       0x40022000UL
#define FLASH_ACR          (*(volatile uint32_t *)(FLASH_R_BASE + 0x00))
#define FLASH_KEYR         (*(volatile uint32_t *)(FLASH_R_BASE + 0x04))
#define FLASH_OPTKEYR      (*(volatile uint32_t *)(FLASH_R_BASE + 0x08))
#define FLASH_SR           (*(volatile uint32_t *)(FLASH_R_BASE + 0x0C))
#define FLASH_CR           (*(volatile uint32_t *)(FLASH_R_BASE + 0x10))
#define FLASH_AR           (*(volatile uint32_t *)(FLASH_R_BASE + 0x14))

#define SYSCFG_BASE        0x40010000UL
#define SYSCFG_CFGR1       (*(volatile uint32_t *)(SYSCFG_BASE + 0x00))
#define SYSCFG_EXTICR1     (*(volatile uint32_t *)(SYSCFG_BASE + 0x08))

#define EXTI_BASE          0x40010400UL
#define EXTI_IMR           (*(volatile uint32_t *)(EXTI_BASE + 0x00))
#define EXTI_EMR           (*(volatile uint32_t *)(EXTI_BASE + 0x04))
#define EXTI_RTSR          (*(volatile uint32_t *)(EXTI_BASE + 0x08))
#define EXTI_FTSR          (*(volatile uint32_t *)(EXTI_BASE + 0x0C))
#define EXTI_SWIER         (*(volatile uint32_t *)(EXTI_BASE + 0x10))
#define EXTI_PR            (*(volatile uint32_t *)(EXTI_BASE + 0x14))

#define GPIOA_BASE         0x48000000UL
#define GPIOB_BASE         0x48000400UL
#define GPIOC_BASE         0x48000800UL
#define GPIOD_BASE         0x48000C00UL
#define GPIOF_BASE         0x48001400UL
/* 注意: 本固件未使能 GPIOE 时钟 (AHBENR 未置 GPIOE 位) */

typedef struct {
    volatile uint32_t MODER;    /* 0x00 */
    volatile uint32_t OTYPER;   /* 0x04 */
    volatile uint32_t OSPEEDR;  /* 0x08 */
    volatile uint32_t PUPDR;    /* 0x0C */
    volatile uint32_t IDR;      /* 0x10 */
    volatile uint32_t ODR;      /* 0x14 */
    volatile uint32_t BSRR;     /* 0x18 */
    volatile uint32_t LCKR;     /* 0x1C */
    volatile uint32_t AFR[2];   /* 0x20 AFRL, 0x24 AFRH */
    volatile uint32_t BRR;      /* 0x28 */
} GPIO_TypeDef;

#define GPIOA              ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB              ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC              ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD              ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOF              ((GPIO_TypeDef *)GPIOF_BASE)

#define SYSTICK_BASE       0xE000E010UL
#define SysTick_CTRL       (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define SysTick_LOAD       (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define SysTick_VAL        (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))
#define SysTick_CALIB      (*(volatile uint32_t *)(SYSTICK_BASE + 0x0C))

#define NVIC_ISER          (*(volatile uint32_t *)0xE000E100UL)
#define NVIC_ICER          (*(volatile uint32_t *)0xE000E180UL)
#define NVIC_IPR           ((volatile uint8_t *)0xE000E400UL)
#define SCB_SHPR           ((volatile uint8_t *)0xE000ED18UL)
#define SCB_CPUID          (*(volatile uint32_t *)0xE000ED00UL)

/* HAL 的 GPIO 模式编码 (与 F0 HAL 完全一致) */
#define GPIO_MODE_INPUT             0x00000000U
#define GPIO_MODE_OUTPUT_PP         0x00000001U
#define GPIO_MODE_AF_PP             0x00000002U
#define GPIO_MODE_ANALOG            0x00000003U
#define GPIO_MODE_IT_RISING         0x10110000U
#define GPIO_MODE_IT_FALLING        0x10210000U
#define GPIO_MODE_IT_RISING_FALLING 0x10310000U
#define GPIO_PULL_NONE              0x00000000U
#define GPIO_PULL_UP                0x00000001U
#define GPIO_PULL_DOWN              0x00000002U
#define GPIO_SPEED_LOW              0x00000000U
#define GPIO_SPEED_HIGH             0x00000003U

typedef struct {
    uint32_t Pin;        /* 0x00 pin mask */
    uint32_t Mode;       /* 0x04 见 GPIO_MODE_* */
    uint32_t Pull;       /* 0x08 */
    uint32_t Speed;      /* 0x0C */
    uint32_t Alternate;  /* 0x10 AF 编号 */
} GPIO_InitTypeDef;

/* ============================================================================
 * 1. 启动链
 * ==========================================================================*/

/* 向量表: 0x08003000 (48 项, 0xC0 字节); main 把它整表复制到 0x20000000 */
extern const uint32_t g_vectors_08003000[0x30];

/* Reset_Handler — 0x080030D4
 * 注意: 不设 SP, 直接调 SystemInit 后跳到第二阶段 (SP 由 0x080030C0 重设) */
void Reset_Handler(void)                                   /* 0x080030D4 */
{
    SystemInit();                                          /* 0x0800B964 */
    ((void (*)(void))0x080030C0U)();
}

/* 第二阶段启动 — 0x080030C0 */
void boot_stage2(void)                                     /* 0x080030C0 */
{
    __asm volatile ("ldr r0, =0x20000CD0\n\t"
                    "mov sp, r0");                         /* 重设 SP */
    init_array();                                          /* 0x08003548 */
    main();                                                /* 0x080030C8: bx main */
}

/* init_array — 0x08003548
 * 表: 0x0800F3EC, 2 项 x 16 字节 {arg0, arg1, arg2, fn}
 *   项1: 0x0800356C(0x0800F40C, 0x20000100, 0x13C)  → 解压 .data
 *   项2: 0x0800C91C(0x0800F448, 0x2000023C, 0xA94)  → .bss 清零 (memset)
 */
typedef struct {
    void *arg0;
    void *arg1;
    uint32_t arg2;
    void (*fn)(void *, void *, uint32_t);
} init_array_t;

extern const init_array_t g_init_array_0800F3EC[2];

void init_array(void)                                      /* 0x08003548 */
{
    const init_array_t *p = g_init_array_0800F3EC;          /* 0x0800F3EC */
    for (; p < g_init_array_0800F3EC + 2; p++)
        p->fn(p->arg0, p->arg1, p->arg2);
}

/* 0x0800F40C 处压缩数据解压器 — 0x0800356C
 * 编码: 控制字节低4位=字面量长度(0→再取1字节), 高4位=重复次数(0→再取1字节);
 *       先复制 length 个字面量字节, 再填 count 个 ...(重复字节, 由实现决定) */
void data_decompress_0800356C(uint8_t *src, uint8_t *dst, uint32_t out_len) /* 0x0800356C */
{
    uint8_t *end = dst + out_len;
    while (dst < end) {
        uint8_t ctrl = *src++;
        uint32_t n = ctrl & 0x0F;
        if (n == 0) n = *src++;
        uint32_t v = ctrl >> 4;
        if (v == 0) v = *src++;
        while (n--) *dst++ = *src++;                       /* 字面量 */
        while (v--) *dst++ = 0;                            /* /*?*/ 重复字节值待确认 */
    }
}

/* .bss 清零 = memset(r1=dst, 0, r2=len) — 0x0800C91C */
void memzero_0800C91C(void *dst, uint32_t len)             /* 0x0800C91C */
{
    uint32_t *p = (uint32_t *)dst;
    while (len) { *p++ = 0; len -= 4; }
}

/* ============================================================================
 * 2. SystemInit (CMSIS, STM32F0) — 0x0800B964
 *    与 stm32f0xx 的 SystemInit() 逐条一致
 * ==========================================================================*/
void SystemInit(void)                                      /* 0x0800B964 */
{
    RCC_CR |= 0x00000001U;                                 /* HSION */
    RCC_CFGR &= 0x08FFB80CU;                               /* 复位 SW/HPRE/PPRE/MCO */
    RCC_CR &= 0xFEF6FFFFU;                                 /* 清 HSEON/CSSON/PLLON */
    RCC_CR &= ~(1U << 18);                                 /* 清 HSEBYP */
    RCC_CFGR &= ~(0x3FU << 16);                            /* 清 PLLSRC/PLLMUL */
    RCC_CFGR2 &= ~0x0FU;                                   /* 清 PREDIV */
    RCC_CFGR3 &= 0xFFFCFE2CU;                              /* 清 USART/I2C/CEC/USB 时钟源 */
    RCC_CR2 &= ~0x00000001U;                               /* 清 HSI48ON */
    RCC_CIR = 0;                                           /* 关 RCC 中断 */
}

/* ============================================================================
 * 3. HAL 初始化 / SysTick / 时钟树
 * ==========================================================================*/
static uint32_t uwTick;                                    /* 0x20000104 (in .data) */

void HAL_IncTick(void)                                     /* 0x080075BC */
{
    uwTick++;
}

uint32_t HAL_GetTick(void)                                 /* 0x08007434 */
{
    return uwTick;
}

/* SysTick_Config(ticks) — 0x08008164 */
uint32_t SysTick_Config(uint32_t ticks)
{
    if ((ticks - 1U) > 0x00FFFFFFU) return 1U;             /* 参数越界 */
    SysTick_LOAD = ticks - 1U;
    HAL_NVIC_SetPriority(-1, 3U, 0U);                      /* /*?*/ 优先级参数 */
    SysTick_VAL  = 0U;
    SysTick_CTRL = 0x00000007U;                            /* CLKSOURCE|TICKINT|ENABLE */
    return 0U;
}

/* HAL_NVIC_SetPriority(irq, prio) — 0x0800A2C8 (irq<0 → SCB SHPR, 否则 NVIC IPR) */
void HAL_NVIC_SetPriority(int32_t irq, uint32_t prio, uint32_t sub)
{
    if (irq < 0) {
        uint32_t idx = ((uint32_t)irq & 0xF) - 8U;
        SCB_SHPR[idx >> 2] = (uint8_t)((prio << 4) & 0xF0);
    } else {
        uint32_t idx = ((uint32_t)irq) >> 2;
        uint32_t sh  = (((uint32_t)irq) & 3U) * 8U;
        volatile uint32_t *reg = (volatile uint32_t *)(0xE000E400UL + idx * 4U);
        *reg = (*reg & ~(0xFFUL << sh)) | ((prio << 4) << sh);
    }
}

void HAL_NVIC_EnableIRQ(uint32_t irq)                      /* 0x0800765C */
{
    NVIC_ISER = 1U << (irq & 0x1F);
}

void HAL_NVIC_DisableIRQ(uint32_t irq)                     /* 0x08007648 */
{
    NVIC_ICER = 1U << (irq & 0x1F);
}

/* HAL_InitTick(tick_priority) — 0x080075EC */
void HAL_InitTick(uint32_t tick_priority)
{
    uint32_t ticks = SystemCoreClock / 1000U;              /* 0x080078F8/0x080030F0(除法) */
    SysTick_Config(ticks);                                 /* 首次调用时 SystemCoreClock=0 → 失败 */
    HAL_NVIC_SetPriority(-1, tick_priority, 0U);
}

/* HAL_MspInit — 0x08007610 */
void HAL_MspInit(void)
{
    RCC_APB2ENR |= (1U << 0);                              /* SYSCFGEN */
    HAL_NVIC_SetPriority(-5, 0U, 0U);                      /* /*?*/ 三个异常优先级 */
    HAL_NVIC_SetPriority(-2, 0U, 0U);
    HAL_NVIC_SetPriority(-1, 0U, 0U);
}

/* HAL_Init — 0x080075CC */
void HAL_Init(void)
{
    FLASH_ACR |= 0x10U;                                    /* PRFTBE 预取 */
    HAL_InitTick(0U);
    HAL_MspInit();
}

/* HAL_RCC_OscConfig — 0x08007980
 * 本次仅处理 LSI(bit3)=ON 与 HSI48(bit5)=ON; PLLState=0 → 跳过 PLL。
 * 结构体字段偏移(实测): [0]OscillatorType [4]HSEState [8]LSEState [0xC]HSIState
 *   [0x10]HSICalibrationValue [0x14]HSI14State [0x18]HSI14CalibrationValue
 *   [0x1C]LSIState [0x20]HSI48State [0x24]PLLState [0x28]PLLSource [0x2C]PLLMUL [0x30]PLLDiv
 */
typedef struct {
    uint32_t OscillatorType;
    uint32_t HSEState;
    uint32_t LSEState;
    uint32_t HSIState;
    uint32_t HSICalibrationValue;
    uint32_t HSI14State;
    uint32_t HSI14CalibrationValue;
    uint32_t LSIState;
    uint32_t HSI48State;
    uint32_t PLLState;
    uint32_t PLLSource;
    uint32_t PLLMUL;
    uint32_t PLLDIV;
} RCC_OscInitTypeDef_obs;                                  /* /*?*/ 字段顺序按反汇编反推 */

static void RCC_OscConfig(void)                            /* 0x0800B8F0 内第一次调用 */
{
    /* 反汇编中只有 4 个字段被写: OscType=0x28, LSIState=1, HSI48State=1, PLLState=0 */
    RCC_OscInitTypeDef_obs osc = {0};
    osc.OscillatorType = 0x28U;                            /* LSI(0x08) | HSI48(0x20) */
    osc.LSIState       = 1U;                               /* 0x08007AB2: RCC_CSR |= LSION */
    osc.HSI48State     = 1U;                               /* 0x08007C60: RCC_CR2 |= (1<<16) */
    osc.PLLState       = 0U;                               /* PLL 不配置 */
    HAL_RCC_OscConfig(&osc);                               /* 0x08007980 */
}

/* RTC 时钟源/备份域 — 0x08007678 */
static void RCC_RtcClockConfig(void)
{
    RCC_APB1ENR |= (1U << 28);                             /* PWREN (r5<<16 = bit28) */
    PWR_CR |= (1U << 8);                                   /* DBP 解除备份域写保护 (超时 100 tick) */
    {
        uint32_t bdcr = RCC_BDCR;
        if ((bdcr & 0x300U) == 0U || (bdcr & 0x300U) != 0x200U) {
            RCC_BDCR = (bdcr & ~0x300U) | (1U << 16);      /* BDRST 脉冲 */
            RCC_BDCR = (bdcr & ~0x300U);
            RCC_BDCR = (bdcr & ~0x300U) | 0x200U;          /* RTCSEL = LSE */
        }
    }
    RCC_APB1ENR &= ~(1U << 28);                            /* 关闭 PWREN */
}

/* HAL_RCC_ClockConfig(cfg, FLASH_LATENCY_1) — 0x08007798
 * cfg: flags=7 (bit0=切SYSCLK bit1=HPRE bit2=PPRE), SYSCLKSource=3(HSI48),
 *      AHBCLKDivider=0 (/1), APB1CLKDivider=0 (/1), APB2CLKDivider=0 (/1)
 */
typedef struct {
    uint32_t flags;         /* 0x00 */
    uint32_t SYSCLKSource;  /* 0x04 */
    uint32_t AHBCLKDivider; /* 0x08 */
    uint32_t APB1CLKDivider;/* 0x0C */
    uint32_t APB2CLKDivider;/* 0x10 (与 APB1 共用 PPRE 字段) */
} RCC_ClkInitTypeDef_obs;

static void RCC_ClockConfig(void)                          /* 0x0800B8F0 内第二次调用 */
{
    RCC_ClkInitTypeDef_obs clk = {7U, 3U, 0U, 0U, 0U};
    HAL_RCC_ClockConfig(&clk, 1U);                         /* 0x08007798, FLASH_LATENCY_1 */
    SystemCoreClock = HAL_RCC_GetSysClockFreq();           /* 0x080078F8 → 0x02DC6C00 = 48MHz */
}

/* 频率换算 — 0x080078F8: SWS 0/1→8MHz, 2→查表算PLL, 3→48MHz */
uint32_t HAL_RCC_GetSysClockFreq(void)
{
    uint32_t sws = (RCC_CFGR >> 2) & 0x3U;
    switch (sws) {
        case 3U: return 0x02DC6C00U;                       /* 48 MHz (HSI48) */
        case 2U: return pll_freq_calc_080078F8();          /* /*?*/ 未在本固件路径使用 */
        default: return 0x007A1200U;                       /* 8 MHz (HSI/HSE) */
    }
}

/* SystemClock_Config — 0x0800B8F0 */
void SystemClock_Config(void)
{
    RCC_OscConfig();                                       /* 0x08007980 */
    RCC_RtcClockConfig();                                  /* 0x08007678 */
    RCC_ClockConfig();                                     /* 0x08007798: HSI48, /1, /1, LATENCY=1 */
    HAL_InitTick(0U);                                      /* 0x080078EC(GetCoreClock)/0x080030F0(div) */
                                                           /* → SysTick_Config(48000) = 1 ms */
    /* HAL_SetTickFreq 类调整: 0x08008148(4) → SysTick CTRL.CLKSOURCE */
    SysTick_CTRL |= (1U << 2);
    HAL_NVIC_SetPriority(-1, 0U, 0U);
}

/* FLASH 等待周期设置 — 0x08007798 内 */
static void FLASH_SetLatency(uint32_t latency)
{
    uint32_t acr = FLASH_ACR;
    if ((acr & 0x7U) != latency) {
        FLASH_ACR = (acr & ~0x7U) | latency;               /* LATENCY 位 */
        while ((FLASH_ACR & 0x7U) != latency) { }           /* /*?*/ 实际代码还等待预取/RDY */
    }
}

/* ============================================================================
 * 4. GPIO 底层
 * ==========================================================================*/

/* HAL_GPIO_Init(GPIOx, GPIO_InitTypeDef*) — 0x080072C4
 * 逐位处理 Pin 掩码; 支持 EXTI 模式 (Mode bit28 置位时配置 SYSCFG.EXTICR + EXTI) */
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *init)
{
    uint32_t pinmask = init->Pin;
    for (uint32_t pin = 0; pin < 16U; pin++) {
        uint32_t mask = 1U << pin;
        if ((pinmask & mask) == 0U) continue;

        /* --- 模式 --- */
        uint32_t moder = GPIOx->MODER;
        moder &= ~(0x3U << (pin * 2U));
        moder |= (init->Mode & 0x3U) << (pin * 2U);
        GPIOx->MODER = moder;

        if ((init->Mode & 0x3U) == GPIO_MODE_OUTPUT_PP ||
            (init->Mode & 0x3U) == GPIO_MODE_AF_PP) {
            /* --- 输出类型: Mode bit4 (=0x10) 表示开漏 --- */
            GPIOx->OTYPER &= ~mask;
            GPIOx->OTYPER |= ((init->Mode >> 4) & 1U) << pin;
            /* --- 速度 --- */
            GPIOx->OSPEEDR &= ~(0x3U << (pin * 2U));
            GPIOx->OSPEEDR |= (init->Speed & 0x3U) << (pin * 2U);
        }
        /* --- 上下拉 --- */
        GPIOx->PUPDR &= ~(0x3U << (pin * 2U));
        GPIOx->PUPDR |= (init->Pull & 0x3U) << (pin * 2U);

        /* --- 复用功能编号 (仅 AF 模式有效) --- */
        if ((init->Mode & 0x3U) == GPIO_MODE_AF_PP) {
            uint32_t idx = pin >> 3U;
            uint32_t sh  = (pin & 7U) * 4U;
            GPIOx->AFR[idx] &= ~(0xFU << sh);
            GPIOx->AFR[idx] |= (init->Alternate & 0xFU) << sh;
        }

        /* --- EXTI 模式 (Mode bit28 = 0x10000000 标志) --- */
        if (init->Mode & 0x10000000U) {
            RCC_APB2ENR |= (1U << 0);                      /* SYSCFGEN */
            /* 端口号: A=0,B=1,C=2,D=3,E=4,其他=5 */
            uint32_t port = gpio_to_port_index(GPIOx);
            uint32_t exti_idx = pin >> 2U;
            uint32_t sh = (pin & 3U) * 4U;
            volatile uint32_t *exticr = &SYSCFG_EXTICR1 + exti_idx;
            *exticr = (*exticr & ~(0xFU << sh)) | (port << sh);
            /* 中断/事件/边沿选择: Mode bit16=IMR bit17=EMR bit20=RTSR bit21=FTSR */
            if (init->Mode & (1U << 16)) EXTI_IMR  |= mask; else EXTI_IMR  &= ~mask;
            if (init->Mode & (1U << 17)) EXTI_EMR  |= mask; else EXTI_EMR  &= ~mask;
            if (init->Mode & (1U << 20)) EXTI_RTSR |= mask; else EXTI_RTSR &= ~mask;
            if (init->Mode & (1U << 21)) EXTI_FTSR |= mask; else EXTI_FTSR &= ~mask;
        }
    }
}

/* HAL_GPIO_ReadPin(GPIOx, pin) — 0x0800741C (返回 0/1) */
uint32_t HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t pin)
{
    return (GPIOx->IDR & pin) ? 1U : 0U;
}

/* HAL_GPIO_WritePin(GPIOx, pin, state) — 0x08007426
 * state!=0 → BSRR 置位; state==0 → BRR 清零 (注意: 写的是 BRR 而非 BSRR 高16位) */
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t state)
{
    if (state) GPIOx->BSRR = pin;
    else       GPIOx->BRR  = pin;
}

/* ============================================================================
 * 5. 板级 GPIO 初始化 — 0x0800941C (main 第 3 个初始化)
 *    顺序: 使能 GPIOA/B/C/D/F 时钟 → 预置电平 → HAL_GPIO_Init × 11 → NVIC
 * ==========================================================================*/
void Board_GPIO_Init(void)                                 /* 0x0800941C */
{
    /* --- 时钟: AHBENR bits 17(A),18(B),19(C),20(D),22(F) --- */
    RCC_AHBENR |= (1U << 19);   /* 0x08009424 */
    RCC_AHBENR |= (1U << 22);   /* 0x08009434 */
    RCC_AHBENR |= (1U << 17);   /* 0x08009442 */
    RCC_AHBENR |= (1U << 18);   /* 0x08009450 */
    RCC_AHBENR |= (1U << 20);   /* 0x0800945C */

    /* --- 预置电平 (配置为输出的脚先给默认电平, 避免毛刺) --- */
    HAL_GPIO_WritePin(GPIOC, 0x2330U, 0U);   /* PC4,PC5,PC8,PC9,PC13 = 0 */
    HAL_GPIO_WritePin(GPIOF, 0x0001U, 0U);   /* PF1 = 0 */
    HAL_GPIO_WritePin(GPIOC, 0x0048U, 1U);   /* PC3,PC6 = 1 (PC6=RF_CS 空闲高) */
    HAL_GPIO_WritePin(GPIOA, 0x0001U, 1U);   /* PA0 = 1 */
    HAL_GPIO_WritePin(GPIOA, 0x0602U, 0U);   /* PA1,PA9,PA10 = 0? (0x602=PA1,PA9,PA10) /*?*/ */
    HAL_GPIO_WritePin(GPIOB, 0x3C04U, 0U);   /* PB2,PB10,PB11,PB12,PB13 = 0 */
    HAL_GPIO_WritePin(GPIOB, 0x03C0U, 1U);   /* PB6,PB7,PB8,PB9 = 1 (LCD 空闲) */

    /* --- 端口配置 --- */
    GPIO_InitTypeDef g;

    g.Pin = 0x2378U; g.Mode = GPIO_MODE_OUTPUT_PP; g.Pull = GPIO_PULL_NONE;
    g.Speed = GPIO_SPEED_LOW; g.Alternate = 0U;
    HAL_GPIO_Init(GPIOC, &g);                /* 0x080094CC: PC3,4,5,6,8,9,13 输出 */

    g.Pin = 0xDC80U; g.Mode = GPIO_MODE_INPUT; g.Pull = GPIO_PULL_UP;
    HAL_GPIO_Init(GPIOC, &g);                /* 0x080094DC: PC7,10,11,12,14,15 上拉输入 */

    g.Pin = 0x0001U; g.Mode = GPIO_MODE_OUTPUT_PP; g.Pull = GPIO_PULL_NONE; g.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOF, &g);                /* 0x080094EC: PF1 输出 */

    g.Pin = 0x0603U; g.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &g);                /* 0x08009502: PA0,1,9,10 输出 */

    g.Pin = 0x8004U; g.Mode = GPIO_MODE_INPUT; g.Pull = GPIO_PULL_UP;
    HAL_GPIO_Init(GPIOA, &g);                /* 0x08009512: PA2,PA15 上拉输入 */

    g.Pin = 0x0040U; g.Mode = GPIO_MODE_IT_FALLING; g.Pull = GPIO_PULL_UP;
    HAL_GPIO_Init(GPIOA, &g);                /* 0x08009526: PA6 下降沿中断 (按键2) */

    g.Pin = 0x0001U; g.Mode = GPIO_MODE_IT_FALLING; g.Pull = GPIO_PULL_UP;
    HAL_GPIO_Init(GPIOB, &g);                /* 0x08009534: PB0 下降沿中断 (电源键) */

    g.Pin = 0x003AU; g.Mode = GPIO_MODE_OUTPUT_PP; g.Pull = GPIO_PULL_NONE; g.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOB, &g);                /* 0x08009542: PB1,3,4,5 输出 */

    g.Pin = 0x3FC4U; g.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOB, &g);                /* 0x08009554: PB2,6,7,8,9,10,11,12,13 输出
                                              *   PB6=LCD_SCL PB7=LCD_A0 PB8=LCD_RST PB9=LCD_CS
                                              *   PB12=RF_MOSI PB13=RF_SCK */

    g.Pin = 0xC000U; g.Mode = GPIO_MODE_INPUT; g.Pull = GPIO_PULL_NONE;
    HAL_GPIO_Init(GPIOB, &g);                /* 0x08009566: PB14=RF_MISO, PB15=RF_IRQ 输入 */

    g.Pin = 0x0004U; g.Mode = GPIO_MODE_INPUT; g.Pull = GPIO_PULL_UP;
    HAL_GPIO_Init(GPIOD, &g);                /* 0x08009574: PD2 上拉输入 */

    /* --- NVIC: EXTI0_1(5) 与 EXTI4_15(7) --- */
    HAL_NVIC_SetPriority(5, 0U, 0U);  HAL_NVIC_EnableIRQ(5);   /* 0x0800957E/84 */
    HAL_NVIC_SetPriority(7, 0U, 0U);  HAL_NVIC_EnableIRQ(7);   /* 0x0800958E/94 */
}

/* 射频引脚再初始化 — 0x08008C78 (0x08004AF0 第一步)
 * 反汇编用基址移位生成掩码: 结果 = PC6 输出高, PB13/PB12 输出低,
 * PB14 输入+下拉, PC8 输出低 */
void RF_GPIO_ReInit(void)                                  /* 0x08008C78 */
{
    GPIOC->MODER &= ~(1U << 13);  GPIOC->MODER |= (1U << 12);   /* PC6 = 01 输出 */
    HAL_GPIO_WritePin(GPIOC, 0x0040U, 1U);                      /* PC6 = 1 (CS 空闲) */

    GPIOB->MODER &= ~(1U << 27);  GPIOB->MODER |= (1U << 26);   /* PB13 = 01 输出 */
    HAL_GPIO_WritePin(GPIOB, 0x2000U, 0U);                      /* PB13 = 0 */
    GPIOB->MODER &= ~(1U << 25);  GPIOB->MODER |= (1U << 24);   /* PB12 = 01 输出 */
    HAL_GPIO_WritePin(GPIOB, 0x1000U, 0U);                      /* PB12 = 0 */
    GPIOB->MODER &= ~(0x3U << 28);                              /* PB14 = 输入 */
    GPIOB->PUPDR &= ~(0x3U << 28);
    GPIOB->PUPDR |=  (0x2U << 28);                              /* PB14 下拉 */

    GPIOC->MODER &= ~(1U << 17);  GPIOC->MODER |= (1U << 16);   /* PC8 = 01 输出 */
    HAL_GPIO_WritePin(GPIOC, 0x0100U, 0U);                      /* PC8 = 0 */
}

/* ============================================================================
 * 6. LCD (128x64 串行, 控制器疑为 ST7565/ST7567/UC1701 类)
 *    PB6=SCL  PA10=SI  PB7=A0/RS  PB9=/CS  PB8=/RESET
 *    帧缓冲 0x200004CC (1024 B = 128 x 8 页)
 * ==========================================================================*/
/* 写一个字节 — 0x0800A480 */
void LCD_WriteByte(uint8_t data, uint32_t is_data)         /* 0x0800A480 */
{
    HAL_GPIO_WritePin(GPIOB, 0x0080U, is_data ? 1U : 0U);  /* PB7 = A0/RS */
    HAL_GPIO_WritePin(GPIOB, 0x0200U, 0U);                 /* PB9 = CS 低 (选中) */
    for (int i = 7; i >= 0; i--) {
        HAL_GPIO_WritePin(GPIOB, 0x0040U, 0U);             /* PB6 SCL 低 */
        HAL_GPIO_WritePin(GPIOA, 0x0400U, (data >> i) & 1U); /* PA10 SI = bit */
        HAL_GPIO_WritePin(GPIOB, 0x0040U, 1U);             /* SCL 上升沿 */
    }
    HAL_GPIO_WritePin(GPIOB, 0x0200U, 1U);                 /* CS 高 */
    HAL_GPIO_WritePin(GPIOB, 0x0080U, 1U);                 /* A0 恢复 1 */
}

/* LCD 复位脉冲 — 0x0800D134 (PB8) */
void LCD_Reset(void)                                       /* 0x0800D134 */
{
    HAL_GPIO_WritePin(GPIOB, 0x0100U, 0U);                 /* PB8 = 0 */
    delay_0CA38(10U);
    HAL_GPIO_WritePin(GPIOB, 0x0100U, 1U);                 /* PB8 = 1 */
    delay_0CA38(10U);
}

/* LCD 初始化命令表 — 0x0800C954 (28 字节, 均为命令) */
static const uint8_t lcd_init_cmds[28] = {                 /* 0x0800C954 */
    0xAE, 0x02, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1,
    0xA6, 0xA8, 0x3F, 0xAD, 0x8B, 0x33, 0xC8, 0xD3,
    0x00, 0xD5, 0x80, 0xD9, 0x1F, 0xDA, 0x12, 0xDB,
    0x30, 0xA0, 0xC0, 0xAF
};

void LCD_Init(void)                                        /* 0x0800C954 */
{
    for (unsigned i = 0; i < sizeof(lcd_init_cmds); i++)
        LCD_WriteByte(lcd_init_cmds[i], 0U);
}

/* 列地址设置 — 0x08004670 */
void LCD_SetColumn(uint32_t col)                           /* 0x08004670 */
{
    LCD_WriteByte((uint8_t)(0x10U | ((col >> 4) & 0x0FU)), 0U);
    LCD_WriteByte((uint8_t)(0x02U | (col & 0x0FU)), 0U);   /* /*?*/ 低字节 bit1 恒 1 的原因待确认 */
}

/* 写一页 (128 字节) — 0x0800A30C 的单页循环体 */
void LCD_WritePage(uint8_t page, const uint8_t *buf)
{
    LCD_SetColumn(0U);
    LCD_WriteByte((uint8_t)(0xB0U | page), 0U);
    for (unsigned i = 0; i < 128U; i++)
        LCD_WriteByte(buf[i], 1U);
}

/* 整屏刷新(8页) — 0x0800A30C */
void LCD_RefreshAll(const uint8_t *fb /* 0x200004CC */)    /* 0x0800A30C */
{
    for (uint8_t page = 0; page < 8U; page++)
        LCD_WritePage(page, fb + page * 128U);
}

/* ============================================================================
 * 7. 射频模块 (2.4 GHz, CC2500/CC1101 寄存器兼容)
 *    PC6=/CS  PB13=SCK  PB12=MOSI  PB14=MISO  PB15=IRQ(EXTI15)
 * ==========================================================================*/

/* 全双工交换 1 字节 — 0x0800CD7C */
uint8_t RF_TransferByte(uint8_t data)                      /* 0x0800CD7C */
{
    uint8_t rx = 0;
    HAL_GPIO_WritePin(GPIOB, 0x2000U, 0U);                 /* SCK 低 */
    for (int i = 7; i >= 0; i--) {
        HAL_GPIO_WritePin(GPIOB, 0x1000U, (data >> i) & 1U); /* MOSI */
        HAL_GPIO_WritePin(GPIOB, 0x2000U, 1U);             /* SCK 上升沿 */
        if (HAL_GPIO_ReadPin(GPIOB, 0x4000U))              /* MISO (PB14) */
            rx |= (uint8_t)(1U << 7);                      /* /*?*/ 位序按实现取样 */
        HAL_GPIO_WritePin(GPIOB, 0x2000U, 0U);
        if (i) rx = (uint8_t)((rx << 1) | (rx >> 7));      /* 移位占位, 精确位序见反汇编 */
    }
    return rx;
}

/* 片选控制 — 0x0800CE38 / 0x0800CE58, 并等 PB14 空闲(低) */
void RF_Select(void)                                       /* 0x0800CE38 */
{
    HAL_GPIO_WritePin(GPIOC, 0x0040U, 0U);                 /* PC6 = 0 */
    while (HAL_GPIO_ReadPin(GPIOB, 0x4000U)) { }           /* 等 MISO 空闲低 */
}

void RF_Deselect(void)                                     /* 0x0800CE58 */
{
    HAL_GPIO_WritePin(GPIOC, 0x0040U, 1U);                 /* PC6 = 1 */
}

/* 写寄存器: reg + 1 字节数据 — 0x0800CD32 → 0x0800CE0C */
void RF_WriteReg(uint8_t reg, uint8_t val)                 /* 0x0800CD32 */
{
    RF_WriteBuf(reg, &val, 1U);                            /* 0x0800CE0C(reg, &val, 1) */
}

/* 写缓冲: 首字节 reg + len 字节 — 0x0800CE0C */
void RF_WriteBuf(uint8_t first, const uint8_t *buf, uint32_t len) /* 0x0800CE0C */
{
    RF_Select();
    RF_TransferByte(first);
    for (uint32_t i = 0; i < len; i++)
        RF_TransferByte(buf[i]);
    RF_Deselect();
}

/* 读寄存器: (reg | 0x80) + 读 1 字节 — 0x0800CCA6 → 0x0800CDC8 */
uint8_t RF_ReadReg(uint8_t reg)                            /* 0x0800CCA6 */
{
    uint8_t v = 0;
    RF_ReadBuf((uint8_t)(reg | 0x80U), &v, 1U);
    return v;
}

/* 读缓冲: 首字节 + 读 len 字节 — 0x0800CDC8 */
void RF_ReadBuf(uint8_t first, uint8_t *buf, uint32_t len) /* 0x0800CDC8 */
{
    RF_Select();
    RF_TransferByte(first);
    for (uint32_t i = 0; i < len; i++)
        buf[i] = RF_TransferByte(0xFFU);
    RF_Deselect();
}

/* 单字节命令 (CC1101 命令选通字 0x30-0x3D) — 0x0800CDF4 */
void RF_Strobe(uint8_t cmd)                                /* 0x0800CDF4 */
{
    RF_WriteBuf(cmd, 0, 0U);
}

/* 硬复位: CS 脉冲 + SRES(0x30) — 0x0800CCBC */
void RF_HardReset(void)                                    /* 0x0800CCBC */
{
    HAL_GPIO_WritePin(GPIOC, 0x0040U, 1U);  delay_0CAB8(30U);
    HAL_GPIO_WritePin(GPIOC, 0x0040U, 0U);  delay_0CAB8(30U);
    HAL_GPIO_WritePin(GPIOC, 0x0040U, 1U);  delay_0CAB8(45U);
    HAL_GPIO_WritePin(GPIOC, 0x0040U, 0U);
    while (HAL_GPIO_ReadPin(GPIOB, 0x4000U)) { }
    RF_Strobe(0x30U);                                      /* SRES */
    while (HAL_GPIO_ReadPin(GPIOB, 0x4000U)) { }
    HAL_GPIO_WritePin(GPIOC, 0x0040U, 1U);
}

/* 寄存器配置表 (节选, 完整 40+ 项见 0x0800CAE8) — 0x0800CAE8
 * 0x0D/0x0E/0x0F = FREQ2/FREQ1/FREQ0 = 0x5C/0xCE/0xC4 → 约 2.4125 GHz */
static const uint8_t rf_config_table[][2] = {              /* 0x0800CAE8 */
    {0x00, 0x29}, {0x01, 0x2E}, {0x02, 0x06}, {0x03, 0x01},
    {0x04, 0xC3}, {0x05, 0x68}, {0x06, 0x04}, {0x07, 0x00},
    {0x08, 0x00}, {0x09, 0x00}, {0x0A, 0x00}, {0x0B, 0x07},
    {0x0C, 0x00}, {0x0D, 0x5C}, {0x0E, 0xCE}, {0x0F, 0xC4},
    {0x10, 0x1D}, {0x11, 0x3B}, {0x12, 0x73}, {0x13, 0x22},
    {0x14, 0xF8}, {0x15, 0x00}, {0x16, 0x07}, {0x17, 0x30},
    {0x18, 0x08}, {0x19, 0x1D}, {0x1A, 0x1C}, {0x1B, 0xC7},
    {0x1C, 0x00}, {0x1D, 0xB2}, {0x1E, 0x6B}, {0x1F, 0x87},
    {0x20, 0xF8}, {0x21, 0xB6}, {0x22, 0x10}, {0x23, 0xEA},
    {0x24, 0x0A},
};

/* 射频初始化 — 0x08004AF0 (由 0x0800B9C0 在开机序列中调用) */
void RF_Init(void)                                         /* 0x08004AF0 */
{
    RF_GPIO_ReInit();                                      /* 0x08008C78 */
    delay_0CA38(0x4E20U);                                  /* 20000 */

    for (int retry = 0; retry <= 10; retry++) {            /* 最多 11 次 */
        delay_0CA38(0x2710U);                              /* 10000 */
        RF_HardReset();                                    /* 0x0800CCBC */
        for (unsigned i = 0; i < sizeof(rf_config_table) / 2U; i++)
            RF_WriteReg(rf_config_table[i][0], rf_config_table[i][1]);
        /* 读 FREQ2/FREQ1 作为“配置生效”判断: 期望 0x0D == 0x5C */
        uint8_t f2 = RF_ReadReg(0x0DU);
        (void)RF_ReadReg(0x0EU);
        if (f2 == 0x5CU) break;
    }
    RF_Strobe(0x36U);                                      /* SIDLE */
    /* 写 PATABLE(0x3E) 功率值: 由功率档索引表 0x0800DA9A 查得 */
    RF_WriteReg(0x3EU, rf_pa_table_0800DA9A[rf_power_index /* 0x20000214-6 */]);
    RF_Strobe(0x33U);                                      /* SCAL */
    delay_0CA38(0x3E8U);                                   /* 1000 */
    rf_state_0800B758(/* 0x20000480[5] */);                /* 进入接收/状态初始化 */
    rf_state_0800AB7C();
}

/* ============================================================================
 * 8. 定时器 / IWDG / ADC / DMA / FLASH 驱动骨架
 * ==========================================================================*/
typedef struct {
    volatile uint32_t *Instance;
    uint32_t Init[6];                                      /* Prescaler, CounterMode, Period,
                                                            * ClockDivision, RepetitionCounter, AutoReload */
    /* ... HAL 句柄其余字段 */
} TIM_HandleTypeDef_obs;

/* HAL_TIM_Base_Init 参数化: 0x080096A8(TIM1)/0x08009708(TIM2)/0x08009768(TIM3)/0x0800967C(TIM14) */
static void TIM_Base_Init_obs(TIM_HandleTypeDef_obs *h, uint32_t psc, uint32_t arr)
{
    h->Init[0] = psc;
    h->Init[1] = 0U;                                       /* CounterMode = UP */
    h->Init[2] = arr;
    h->Init[3] = 0U;
    h->Init[4] = 0U;
    HAL_TIM_Base_Init(h);                                  /* 0x0800BABC: CR1/PSC/ARR/RCR/EGR=UG */
    HAL_TIM_Base_Start_IT(h);                              /* 0x080081A0: DIER.UIE=1, CR1.CEN=1 */
}

void MX_TIM1_Init(void)  { TIM_HandleTypeDef_obs h = {&TIM1};  TIM_Base_Init_obs(&h, 0x2F, 0x270F); }  /* 100 Hz  0x080096A8 */
void MX_TIM2_Init(void)  { TIM_HandleTypeDef_obs h = {&TIM2};  TIM_Base_Init_obs(&h, 0x3BF, 0xC350); } /* 1 Hz    0x08009708 */
void MX_TIM3_Init(void)  { TIM_HandleTypeDef_obs h = {&TIM3};  TIM_Base_Init_obs(&h, 1, 0x27F); }      /* 37.5kHz 0x08009768 */
void MX_TIM14_Init(void) { TIM_HandleTypeDef_obs h = {&TIM14}; TIM_Base_Init_obs(&h, 0x2F, 0xC7); }     /* 5 kHz   0x0800967C */

/* IWDG: 句柄 0x20000384 = {IWDG, Prescaler=6, Reload=0xFFF, Window=0xFFF} */
typedef struct {
    volatile uint32_t *Instance;
    uint32_t Prescaler;                                    /* 6 → /256 */
    uint32_t Reload;                                       /* 0xFFF */
    uint32_t Window;                                       /* 0xFFF */
    uint8_t  State, ErrorCode;
} IWDG_HandleTypeDef_obs;

IWDG_HandleTypeDef_obs hiwdg = { (volatile uint32_t *)0x40003000UL, 6U, 0xFFFU, 0xFFFU, 0U, 0U };

void MX_IWDG_Init(void)                                    /* 0x080095CC */
{
    HAL_IWDG_Init(&hiwdg);                                 /* 0x08007440: KR=0x5555, PR/RLR/WINR, KR=0xAAAA, KR=0xCCCC */
}

void HAL_IWDG_Refresh(IWDG_HandleTypeDef_obs *h)           /* 0x080074F0 */
{
    *(volatile uint32_t *)h->Instance = 0x0000AAAAU;       /* KR = 重新装载 */
}

void HAL_IWDG_Start_obs(IWDG_HandleTypeDef_obs *h)         /* 0x08007544 */
{
    *(volatile uint32_t *)h->Instance = 0x0000AAAAU;
    *(volatile uint32_t *)h->Instance = 0x0000CCCCU;       /* 启动 IWDG */
    while (*(volatile uint32_t *)((uint32_t)h->Instance + 0x0C) & 0x7U) { } /* 等 SR 更新 */
}

/* ADC 初始化 — 0x08009310 (HAL_ADC_Init 0x080069AC + HAL_ADC_ConfigChannel 0x08006878) */
typedef struct { uint32_t Channel, Rank, SamplingTime; } ADC_ChannelConfTypeDef_obs;

void MX_ADC_Init(void)                                     /* 0x08009310 */
{
    ADC_HandleTypeDef_obs hadc = { (volatile uint32_t *)0x40012400UL };
    HAL_ADC_Init(&hadc);                                   /* 0x080069AC */
    ADC_ChannelConfTypeDef_obs ch = { 3U, 0x1000U, 3U };   /* 结构体语义 /*?*/ */
    HAL_ADC_ConfigChannel(&hadc, &ch);
    const uint32_t channels[] = { 10U, 11U, 12U };         /* 0x08009362/76/8A */
    for (unsigned i = 0; i < 3U; i++) {
        ch.Channel = channels[i];
        HAL_ADC_ConfigChannel(&hadc, &ch);
    }
    /* MSPInit 0x08006B20: RCC_APB2ENR |= ADC1EN(bit9); PC7=ANALOG, PA8=ANALOG;
     * DMA1_Channel1 句柄 0x200003F8 + HAL_DMA_Init(0x080070B0) */
}

/* FLASH 自编程 — 0x08005E24 / 0x08005E6C / 0x08005F34 */
void FLASH_MassErase_Start(void)                           /* 0x08005E24 */
{
    FLASH_CR |= 0x00000004U;                               /* MER */
    FLASH_CR |= 0x00000040U;                               /* STRT */
}

void FLASH_ProgramHalfWord(uint32_t addr, uint16_t data)   /* 0x08005E6C */
{
    FLASH_CR |= 0x00000001U;                               /* PG */
    *(volatile uint16_t *)addr = data;
}

uint32_t FLASH_WaitForLastOperation(uint32_t timeout)      /* 0x08005F34 */
{
    while (FLASH_SR & 0x00000001U) {                       /* BSY */
        if ((HAL_GetTick() - start) > timeout) return 3U;  /* /*?*/ 超时=3 */
    }
    if (FLASH_SR & 0x00000004U) { FLASH_SR = 0x00000020U; return 1U; } /* PGERR */
    if (FLASH_SR & (0x10U | 0x20U)) { FLASH_SR = FLASH_SR; return 1U; } /* WRPRT/其它 */
    return 0U;
}

/* ============================================================================
 * 9. 中断服务函数
 * ==========================================================================*/

/* SysTick — 0x0800B8E4 (1 ms) */
void SysTick_Handler(void)                                 /* 0x0800B8E4 */
{
    HAL_IncTick();                                         /* 0x080075BC: uwTick++ */
    HAL_SYSTICK_IRQHandler_obs();                          /* 0x08008194 (空实现) */
}

/* EXTI0_1 — 0x08005A78: PB0 电源键 (EXTI0, 下降沿) */
void EXTI0_1_IRQHandler(void)                              /* 0x08005A78 */
{
    if (EXTI_PR & 0x0001U) {                               /* PR bit0 */
        if (key_state_200001D4 == 0 && (sys_state_20000175 & 0x3U) == 1U) {
            if (key_cnt_200001CA == 0) {
                key_cnt_200001CA = 0x26ACU + 0x64U;        /* 9900+100 = 10000 ticks */
            } else if (key_cnt_200001CA <= 0x26ACU) {
                sys_flag_20000182 = 1U;
                power_off_request_08006014();
                sys_flag_20000182 = 0U;
                rf_state_0800B7F0();
                key_cnt_200001CA = 0U;
            }
        }
        HAL_GPIO_EXTI_ClearITPendingBit_obs(0x0001U);      /* 0x080072AC: EXTI_PR = mask */
    }
}

/* EXTI2_3 — 0x08005B00: 仅清 line3 挂起位 (源未在本镜像内配置) /*?*/ */
void EXTI2_3_IRQHandler(void)                              /* 0x08005B00 */
{
    HAL_GPIO_EXTI_ClearITPendingBit_obs(0x0008U);
}

/* EXTI4_15 — 0x08005B0C: PB15=射频中断(15), PA6=按键2(6) */
void EXTI4_15_IRQHandler(void)                             /* 0x08005B0C */
{
    if (EXTI_PR & 0x8000U) {                               /* line15: RF IRQ */
        if (sys_flag_2000017C == 1U && key_state_200001D4 == 0U)
            RF_PacketHandler_0800AA74();
        HAL_GPIO_EXTI_ClearITPendingBit_obs(0x8000U);
    }
    if (EXTI_PR & 0x0040U) {                               /* line6: PA6 按键 */
        if (key_state_200001D4 == 0U) {
            delay_0CA68(0x32U);
            if (HAL_GPIO_ReadPin(GPIOA, 0x0040U) == 0U) {   /* 仍为低 → 有效按键 */
                if (key_long_20000179 == 1U) key_long_20000179 = 0U;
                else if (key_flag_2000017F == 0U) {
                    sys_flag_20000182 = 1U;
                    power_key_action_08006014();
                    sys_flag_20000182 = 0U;
                    rf_state_0800B7F0();
                }
            }
        }
        HAL_GPIO_EXTI_ClearITPendingBit_obs(0x0040U);
    }
}

/* DMA1_Channel1 — 0x08004790: ADC 采样搬运完成 */
void DMA1_Channel1_IRQHandler(void)                        /* 0x08004790 */
{
    HAL_DMA_IRQHandler(&hdma_adc /* 0x200003F8 */);
}

/* RTC — 0x0800ACF8 */
void RTC_IRQHandler(void)                                  /* 0x0800ACF8 */
{
    rtc_flag_2000021A = 1U;
    HAL_RTC_IRQHandler(&hrtc /* 0x20000274 */);            /* 0x08007E34 */
}

/* TIM1/TIM2/TIM14 中断只调用 HAL_TIM_IRQHandler(句柄), 见 §附录 */

/* ============================================================================
 * 10. 电源开/关机 — 0x0800A934
 * ==========================================================================*/

/* 关机序列: r0 != 1 */
void Power_Off_Sequence(void)                              /* 0x0800A946 */
{
    sys_state_200001E0[4] = 0U;
    HAL_GPIO_WritePin(GPIOC, 0x0020U, 0U);                 /* PC5 = 0 */
    HAL_GPIO_WritePin(GPIOA, 0x0002U, 0U);                 /* PA1 = 0 */
    HAL_GPIO_WritePin(GPIOA, 0x0200U, 0U);                 /* PA9 = 0 */
    HAL_GPIO_WritePin(GPIOA, 0x0001U, 1U);                 /* PA0 = 1 */
    HAL_GPIO_WritePin(GPIOA, 0x0010U, 0U);                 /* PA4 = 0 (板级未配置为输出 /*?*/) */
    HAL_GPIO_WritePin(GPIOF, 0x0001U, 0U);                 /* PF1 = 0 */
    HAL_GPIO_WritePin(GPIOC, 0x0008U, 0U);                 /* PC3 = 0 */
    HAL_GPIO_WritePin(GPIOB, 0x0004U, 0U);                 /* PB2 = 0 */
    HAL_GPIO_WritePin(GPIOB, 0x0800U, 0U);                 /* PB11 = 0 */
    HAL_GPIO_WritePin(GPIOB, 0x0800U, 0U);                 /* PB11 = 0 (重复) */

    /* 轮询 PC7: 等待“电源/充电检测”电平变化, 最多 10×10 ms, 期间刷新 IWDG */
    for (int i = 0; i < 10; i++) {
        if (HAL_GPIO_ReadPin(GPIOC, 0x0080U) == 1U) {
            delay_0CA98(10U);
        } else {
            break;
        }
        if (i == 9) {
            /* 超时分支: 继续等 PC7 低并刷新看门狗 (0x0800AA02–0x0800AA36) */
            for (int j = 0; j < 10; j++) {
                HAL_IWDG_Refresh(&hiwdg);
                if (HAL_GPIO_ReadPin(GPIOC, 0x0080U) == 0U) break;
                delay_0CA98(10U);
            }
        }
    }
}

/* 开机序列: r0 == 1 */
void Power_On_Sequence(void)                               /* 0x0800A9D6 */
{
    HAL_GPIO_WritePin(GPIOC, 0x0020U, 1U);                 /* PC5 = 1 (电源锁存, 推断) */
    LCD_Init_And_State_0800C230();                         /* 0x0800C230: LCD/显示 + 定时 */
    if (sys_state_200001E0[4] != 0U) {                     /* 需要完整上电初始化 */
        Power_On_Hardware_Init_0800B9C0();                 /* 0x0800B9C0, 见下 */
        key_state_200001D4 = 0U;
    }
}

/* 上电硬件初始化 — 0x0800B9C0 */
void Power_On_Hardware_Init_0800B9C0(void)                 /* 0x0800B9C0 */
{
    HAL_GPIO_WritePin(GPIOA, 0x0002U, 1U);                 /* PA1 = 1 */
    delay_0CA98(100U);                                     /* 100 ms */
    HAL_GPIO_WritePin(GPIOF, 0x0001U, 1U);                 /* PF1 = 1 */
    other_init_08003AC8();                                 /* 0x08003AC8 (未展开) */

    ram_20000180 = 1U; ram_20000194 = 0U; ram_20000181 = 1U; ram_20000184 = 1U;
    GPIO_InitTypeDef g = { 0x0080U, GPIO_MODE_OUTPUT_PP, GPIO_PULL_NONE, GPIO_SPEED_HIGH, 0U };
    HAL_GPIO_Init(GPIOA, &g);                              /* 0x08008A2C: PA7 输出推挽高速 */
    ram_200001DA = 0xFAU;

    RF_Init();                                             /* 0x08004AF0 */
    ram_20000185 = 0U; ram_20000233 = 1U;

    RF_GPIO_ReInit2_0800D088();                            /* PB8 复位脉冲 + LCD 初始化命令 */
    rf_state_0800ABE4();                                   /* PA6 检测 + 0x36/0x33 + 初始化 */
    hw_selftest_0800C080();                                /* 硬件自检/去抖/看门狗 */
    HAL_TIM_Base_Start_IT(&htim14 /* 0x20000348 */);       /* 0x080082B4: TIM14 5 kHz */
}

/* ============================================================================
 * 11. main — 0x0800CF74
 * ==========================================================================*/
volatile uint8_t  main_event_flag;                         /* 0x200001E1 */
volatile uint32_t SystemCoreClock;                         /* 0x20000100 (in .data) */

int main(void)                                             /* 0x0800CF74 */
{
    /* ① 复制向量表到 0x20000000 (48 项 = 0xC0 字节) 并做 SRAM 重映射 */
    for (uint32_t i = 0; i < 0x30U; i++)
        ((volatile uint32_t *)0x20000000UL)[i] =
            ((const volatile uint32_t *)0x08003000UL)[i];
    SYSCFG_CFGR1 |= 0x00000003U;                           /* MEM_MODE: SRAM 向量表 /*?*/ */

    /* ② 初始化序列 */
    HAL_Init();                                            /* 0x080075CC */
    SystemClock_Config();                                  /* 0x0800B8F0 */
    Board_GPIO_Init();                                     /* 0x0800941C */
    DMA1_Clock_And_NVIC_Init_080093F4();                   /* AHBENR bit0 + NVIC IRQ9 */
    MX_TIM14_Init();                                       /* 5 kHz */
    MX_ADC_Init();                                         /* ADC 通道 3/10/11/12 + DMA1_CH1 */
    MX_IWDG_Init();                                        /* 0x080095CC */
    MX_RTC_Init();                                         /* 0x080095F8 */
    MX_TIM2_Init();                                        /* 1 Hz */
    MX_TIM3_Init();                                        /* PWM 37.5 kHz, PA7=TIM3_CH2 AF1 */
    MX_TIM1_Init();                                        /* 100 Hz */
    DAC_Init_080093A8();                                   /* 0x40007400 DAC */
    RCC_APB1ENR |= (1U << 28);                             /* PWREN */
    (void)RCC_APB1ENR;
    __disable_irq();                                       /* primask = 1 */
    HAL_RTC_DeInit(&hrtc);                                 /* 0x08007F10 */
    HAL_IWDG_Start_obs(&hiwdg);                            /* 0x08007544 */
    HAL_IWDG_Refresh(&hiwdg);                              /* 0x080074F0 */
    reset_flag_check_0800AD10();                           /* 读 RCC_CSR.PINRSTF + 0x0801F800 */
    settings_restore_0800AD44();                           /* 参数区 0x0801F000, 20×100B */
    Power_SetState(1U);                                    /* 0x0800A934: 开机序列 */
    rf_state_0800ABE4();                                   /* 0x0800ABE4 */
    RCC_CSR |= 0x00800000U;                                /* 0x0800CFFE: bit23, 语义待确认 /*?*/ */
    flag_2000021B = 0U;

    /* ③ 事件循环: flag 由射频包处理 0x0800B0BE 末尾置 1 */
    for (;;) {
        __enable_irq();                                    /* primask = 0 */
        while (main_event_flag != 1U) { }
        main_event_flag = 0U;

        HAL_IWDG_Refresh(&hiwdg);                          /* 0x080074F0 */
        task_key_charge_08008D00();                        /* 0x08008D00 */
        task_080047CC(); task_080098F8(); task_080035A8(); task_0800642C();
        task_080038C0();
        display_task_0800D098();                           /* 0x0800D098: LCD 刷新调度 */
        task_080065CC(); task_0800896C(); task_0800B394(); task_0800A230();
        task_08003BAC(); task_0800C7D4();
        sleep_task_0800B7B0();                             /* 0x0800B7B0: 休眠/唤醒管理 */
        task_080097F8(); task_0800B80C();
    }
}

/* ============================================================================
 * 12. 延时函数
 * ==========================================================================*/
/* 0x0800CA38: 内层 ~2×14 nop 的忙等 ×n */
void delay_0CA38(uint32_t n)                               /* 0x0800CA38 */
{
    n &= 0xFFFFU;
    do {
        for (volatile uint32_t i = 0; i < 2U; i++)
            __asm volatile ("nop; nop; nop; nop; nop; nop; nop;"
                            "nop; nop; nop; nop; nop; nop; nop");
    } while (--n);
}

/* 0x0800CA98: cnt × delay_0CA38(990) ≈ 0.6 ms/次 */
void delay_0CA98(uint32_t cnt)                             /* 0x0800CA98 */
{
    for (uint32_t i = 0; i < cnt; i++)
        delay_0CA38(0x3DEU);
}

/* 0x08009038: n 次 { delay_0CA38(0x3E3) + HAL_IWDG_Refresh } */
void wait_with_iwdg_08009038(uint32_t n)                   /* 0x08009038 */
{
    for (uint32_t i = 0; i < n; i++) {
        delay_0CA38(0x3E3U);
        HAL_IWDG_Refresh(&hiwdg);
    }
}

/* 0x0800CAB8 / 0x0800CA68: 无 nop 填充的短忙等 */
void delay_0CAB8(uint32_t n)                               /* 0x0800CAB8 */
{
    n &= 0xFFFFU;
    do { for (volatile uint32_t i = 0; i < 2U; i++) ; } while (--n);
}
void delay_0CA68(uint32_t n)                               /* 0x0800CA68 */
{
    n &= 0xFFFFU;
    do { for (volatile uint32_t i = 0; i < 2U; i++) ; } while (--n);
}
