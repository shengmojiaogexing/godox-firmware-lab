/*
 * startup.c - 启动代码 / scatter / 运行时助手
 *
 * 重建自：
 *   0x08003000 向量表（48 项）
 *   0x080030C0 startup_c
 *   0x080030D4 reset_handler
 *   0x0800BD0C SystemInit
 *   0x08003548 scatter_load
 *   0x0800C71C memcpy（未列入 CFG，手工反汇编）
 *   0x0800C72C memset（同上）
 *   0x080030F0 udivmod32（32 位软件除法）
 *   0x08003144 crc16_block
 */
#include "ad200.h"

/* ------------------------------------------------------------------ 向量表
 * 位置 0x08003000；main 会把整张表复制到 RAM 0x20000000 并置 MEM_MODE=SRAM。
 * 未使用的 26 个向量全部指向 0x080030E6（`b .`，见 default_handler）。
 */
typedef void (*vector_t)(void);

__attribute__((section(".isr_vector")))
const uint32_t g_vectors[48] = {
    0x200008B0,          /*  0 initial SP */
    0x080030D5,          /*  1 Reset        -> reset_handler */
    0x0800A25D,          /*  2 NMI          -> NMI_Handler */
    0x08008201,          /*  3 HardFault    -> HardFault_Handler */
    0, 0, 0, 0, 0, 0, 0, /*  4..10 reserved */
    0x0800B81D,          /* 11 SVCall */
    0, 0,                /* 12..13 reserved */
    0x0800A90D,          /* 14 PendSV */
    0x0800BC8D,          /* 15 SysTick */
    /* ---- IRQ 0..31 ---- */
    0x080030E7, 0x080030E7, 0x0800AC0D, 0x080030E7, 0x080030E7,  /* WWDG PVD RTC FLASH RCC */
    0x080053F1, 0x08005479, 0x08005485, 0x080030E7,              /* EXTI0_1 EXTI2_3 EXTI4_15 TSC */
    0x080044D1, 0x080030E7, 0x080030E7, 0x080030E7,              /* DMA1_Ch1 Ch2_3 Ch4_7 ADC */
    0x0800BE1D, 0x080030E7,                                      /* TIM1_BRK_UP_TRG_COM, TIM1_CC */
    0x0800BE59, 0x080030E7, 0x080030E7, 0x080030E7,              /* TIM2, TIM3, TIM6, TIM7 */
    0x0800BDE1, 0x080030E7, 0x080030E7, 0x080030E7,              /* TIM14, TIM15, TIM16, TIM17 */
    0x080030E7, 0x080030E7, 0x080030E7, 0x080030E7,              /* I2C1 I2C2 SPI1 SPI2 */
    0x080030E7, 0x080030E7, 0x080030E7, 0x080030E7,              /* USART1..CEC USB */
};

/* 0x080030E6: 默认异常处理 = `b .` */
__attribute__((naked, noreturn)) void default_handler(void)
{
    for (;;) { }
}

/* 0x080030C0: 由 reset_handler 跳入。设置 SP 后搬运 .data/.bss 再进入 main */
void startup_c(void)
{
    /* mov sp, 0x200008B0 (0x080030c2)；C 里由启动代码/链接脚本保证 */
    scatter_load();                              /* 0x080030c4 */
    main();                                      /* 0x080030c8 bx 0x0800CDB1 */
    for (;;) { }
}

/* 0x080030D4: 复位向量指向这里 */
void reset_handler(void)
{
    SystemInit();                                /* 0x080030d6 blx 0x0800BD0D */
    startup_c();                                 /* 0x080030da bx 0x080030C1 */
}

/* ------------------------------------------------------------------ scatter
 * 表 0x0800E3E4：2 条 16 字节记录 {src, dst, len, fn}
 *   {0x0800E404, 0x20000100, 0x00000128, 0x0800C720}  memcpy  .data
 *   {0x0800E52C, 0x20000228, 0x00000688, 0x0800C730}  memset  .bss
 * 结构 0x08003548；循环体 0x08003550..0x0800355E。
 */
typedef void (*scatter_fn)(const void *src, void *dst, uint32_t len);
typedef struct {
    const void  *src;
    void        *dst;
    uint32_t     len;
    scatter_fn   fn;    /* Thumb 位在函数内 orr #1 */
} scatter_rec_t;

extern const scatter_rec_t g_scatter_table[];   /* 0x0800E3E4 */

void scatter_load(void)
{
    const scatter_rec_t *r = g_scatter_table;   /* 0x0800E3E4 */
    const scatter_rec_t *end = g_scatter_table + 2; /* 0x0800E404 */
    while (r < end) {
        r->fn(r->src, r->dst, r->len);
        r++;
    }
    memcpy(0, 0, 0); /* 占位，避免空调用被优化掉 */
}

/* 0x0800C71C 附近的 memcpy（4 字节/次） */
void memcpy(void *dst, const void *src, uint32_t len)
{
    uint32_t *d = dst;
    const uint32_t *s = src;
    while (len >= 4) { *d++ = *s++; len -= 4; }
    while (len--) { *(uint8_t *)d = *(uint8_t *)s; d = (uint32_t *)((uint8_t *)d + 1);
                    s = (const uint32_t *)((const uint8_t *)s + 1); }
}

/* 0x0800C72C 附近的 memset（4 字节/次，src 被忽略） */
void memset(void *dst, uint32_t val, uint32_t len)
{
    uint32_t *d = dst;
    while (len >= 4) { *d++ = 0; len -= 4; }
    while (len--) { *(uint8_t *)d = 0; d = (uint32_t *)((uint8_t *)d + 1); }
    (void)val;
}

/* ------------------------------------------------------------------ SystemInit
 * 0x0800BD0C：把时钟树复位到 HSI，关 PLL/HSE/CSS，清中断标志。
 * 证据：RCC_CR/CFGR/CFGR2/CFGR3/CR2 的读改写 0x0800bd0e..0x0800bd54。
 */
void SystemInit(void)
{
    RCC_CR   |= RCC_CR_HSION;                       /* 0x0800bd0e */
    RCC_CFGR &= 0x08FFB80Cu;                        /* 0x0800bd16: 清 SW/HPRE/PPRE/PLL */
    RCC_CR   &= 0xFEF6FFFFu;                        /* 0x0800bd1e: 关 HSE/PLL/CSS */
    RCC_CR   &= ~(1u << 18);                        /* 0x0800bd26: HSEBYP */
    RCC_CFGR &= ~0x003F0000u;                       /* 0x0800bd30: 清 PLLMUL */
    RCC_CFGR2 = (RCC_CFGR2 >> 4) << 4;              /* 0x0800bd3a: PREDIV 清 0 */
    RCC_CFGR3 &= 0xFFFCFE2Cu;                       /* 0x0800bd42 */
    RCC_CR2   &= ~1u;                               /* 0x0800bd4a */
    RCC_CIR    = 0;                                 /* 0x0800bd52 */
}

/* ------------------------------------------------------------------ 32 位除法
 * 0x080030F0：经典移位减法，返回 r0 = 商、r1 = 余数（AAPCS 由调用方读 r1）。
 * 反汇编 0x080030f0..0x0800311a（入口 0x080030F6、循环 0x080030FE）。
 */
uint32_t udivmod32(uint32_t num, uint32_t den)
{
    uint32_t quot = 0;
    int32_t  bit;
    if (den == 0) return 0;                         /* 未显式检查，硬件行为 */
    for (bit = 31; bit >= 0; bit--) {
        if ((num >> bit) >= den) {
            num -= den << bit;
            quot += 1u << bit;
        }
    }
    return quot;                                    /* 余数在 num（r1） */
}

/* ------------------------------------------------------------------ CRC16
 * 0x08003144：对 *(u16*)(p + off) ... 做 16 位校验，用于 flash 参数写保护。
 * 具体多项式未逐位确认，按"块校验和"重建 [推测]。
 */
uint16_t crc16_block(const uint8_t *p, uint32_t len, uint32_t off)
{
    uint16_t crc = 0;
    uint32_t i;
    for (i = 0; i < len; i++)
        crc = (uint16_t)((crc << 1) ^ (uint16_t)(p[off + i] | 0x100u));
    return crc;
}
