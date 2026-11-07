/*
 * hw_gpio.c - GPIO 初始化 / 位操作 / 引脚表
 *
 * 重建自：
 *   0x08005544 gpio_init_all        （写 5 个端口的 MODER/PUPDR/ODR 常量）
 *   0x08006E9E gpio_write_pin
 *   0x08006E94 gpio_read_pin
 *   0x08006D24 exti_clear_pending
 *   0x08008778 rf_bus_init          （RF 软 SPI 引脚，见 hw_spi_soft.c）
 *   0x08009758 board_init_inputs    （见 hw_exti.c）
 */
#include "ad200.h"

/* ------------------------------------------------------------------ 引脚初始化常量
 * 证据：0x0800554e..0x08005598 的立即数写入（无循环/无表，逐端口硬编码）。
 *
 * 索引:  MODER 2bit/pin (00 IN, 01 OUT, 10 AF, 11 ANALOG)
 *        PUPDR 2bit/pin (00 none, 01 pull-up, 10 pull-down)
 */
static const struct {
    uint32_t base;
    uint32_t moder;
    uint32_t pupdr;
    uint32_t odr;
} g_gpio_init[] = {
    { GPIOA_BASE, 0x3FD57155u, 0x40000400u, 0x00000001u },  /* 0x0800554e..0x08005560 */
    { GPIOB_BASE, 0x15755015u, 0x00000540u, 0x00002BC0u },  /* 0x08005562..0x08005572 */
    { GPIOC_BASE, 0xFD555455u, 0x00000100u, 0x00000060u },  /* 0x08005574..0x08005582 */
    { GPIOD_BASE, 0x00000030u, 0x00000000u, 0x00000000u },  /* 0x08005584..0x0800558e */
    { GPIOF_BASE, 0x0000000Du, 0x00000000u, 0x00000000u },  /* 0x08005590..0x08005598 */
};

/* 解码后（见 docs/05-pinmap.md）：
 * PA0 OUT=1   PA1..PA4 OUT  PA5 IN-PU  PA6 AN  PA7..PA10 OUT
 * PA11..PA14 AN  PA15 IN-PU
 * PB0 OUT  PB1,2 OUT  PB3,4,5 IN-PU  PB6..PB9 OUT=1  PB10 AN  PB11..PB14 OUT
 * PC0..PC3 OUT  PC4 IN-PU  PC5,PC6 OUT=1  PC7..PC12 OUT  PC13..PC15 AN
 * PD2 AN ; PF0 OUT ; PF1 AN
 */

/* 0x08005544 gpio_init_all：先调 0x0800A90E（RCC 侧）、ADC 相关、GPIOF 时钟，再逐口写 */
void gpio_init_all(void)
{
    unsigned i;

    gpio_clock_init();                              /* 0x08005546 0x0800A90E */
    adc_hw_init();                                  /* 0x0800554a 0x0800356C */
    exti_power_reset();                             /* 0x0800554e 0x0800CEE8 */

    for (i = 0; i < sizeof(g_gpio_init) / sizeof(g_gpio_init[0]); i++) {
        GPIO_ODR(g_gpio_init[i].base)   = g_gpio_init[i].odr;   /* +0x14 */
        GPIO_MODER(g_gpio_init[i].base) = g_gpio_init[i].moder; /* +0x00 */
        GPIO_PUPDR(g_gpio_init[i].base) = g_gpio_init[i].pupdr; /* +0x0c */
    }
}

/* 0x08006E9E: gpio_write_pin(port, mask, level) */
void gpio_write_pin(uint32_t port, uint32_t mask, uint32_t level)
{
    if (level)
        GPIO_BSRR(port) = mask;                     /* 0x08006ea2 置位 */
    else
        GPIO_BRR(port) = mask;                      /* 0x08006ea6 复位 */
}

/* 0x08006E94: gpio_read_pin(port, mask) -> 0/1 */
uint32_t gpio_read_pin(uint32_t port, uint32_t mask)
{
    return (GPIO_IDR(port) & mask) ? 1u : 0u;       /* 0x08006e94..0x08006e9c */
}

/* 0x08006D24: exti_clear_pending(mask) —— 写 1 清 EXTI_PR */
void exti_clear_pending(uint32_t mask)
{
    EXTI_PR = mask;                                 /* 0x08006d24 */
}

/* 0x08006EAC: get_tick() -> g_tick_ms（SysTick 1 ms 计数） */
uint32_t get_tick(void)
{
    return g_tick_ms;                               /* 0x08006eac */
}

/* 0x0800C768: delay_us(us)。内层 2×12 nop + 计数，1 单位≈1us [推测: 取决于主频] */
void delay_us(uint32_t us)
{
    while (us--) {
        __asm volatile ("nop; nop; nop; nop; nop; nop;"
                        "nop; nop; nop; nop; nop; nop");
        __asm volatile ("nop; nop; nop; nop; nop; nop;"
                        "nop; nop; nop; nop; nop; nop");
    }
}

/* 0x0800C828: delay_ms(ms)，与 delay_us 同构但单位更大 */
void delay_ms(uint32_t ms)
{
    delay_us(ms * 1000u);                           /* 0x0800c828 外层同样 2×12 nop */
}

/* 0x0800BE1C 等中断里用到的"报告/收尾"钩子（见各子系统） */
void isr_report(uint32_t v) { (void)v; }
