/*
 * hw_exti.c - EXTI + 引脚输入初始化 + 中断服务
 *
 * 重建自：
 *   0x08006D3C exti_config（157 条，核心）
 *   0x08009758 board_init_inputs（154 条）
 *   0x080053F0 EXTI0_1_IRQHandler
 *   0x08005478 EXTI2_3_IRQHandler
 *   0x08005484 EXTI4_15_IRQHandler
 *   0x0800CEC4 charge_power_on / 0x0800CEE8 charge_power_off
 *   0x0800BC08 exti_enable_group / 0x0800BBE0 exti_disable_group
 */
#include "ad200.h"

/* cfg[] 字段语义（来自 0x08006D42..0x08006E74 的位操作）：
 *   cfg[0] pinmask
 *   cfg[1] bit1:0 = MODER；bit4 = OTYPER(开漏)；
 *          bit16 = IMR；bit17 = EMR；bit20 = RTSR；bit21 = FTSR
 *   cfg[2] PUPDR 2bit 值（按引脚移位）
 *   cfg[3] AFR 值（本固件未使用 AF，恒 0）
 */
void exti_config(uint32_t port, const uint32_t cfg[4])
{
    uint32_t pinmask = cfg[0];
    uint32_t mode    = cfg[1];
    uint32_t pin;

    for (pin = 0; pin < 16; pin++) {
        uint32_t bit = 1u << pin;
        if (!(pinmask & bit))
            continue;

        /* ---- MODER：2 bit/引脚 ---- */
        GPIO_MODER(port) = (GPIO_MODER(port) & ~(3u << (2 * pin)))
                         | ((mode & 3u) << (2 * pin));       /* 0x08006d88 */

        /* ---- OTYPER：开漏（mode 1/2/0x11/0x12 时按 bit4 设置） ---- */
        if ((mode & 3u) == 1 || (mode & 3u) == 2) {
            GPIO_OTYPER(port) = (GPIO_OTYPER(port) & ~bit)
                              | (((mode >> 4) & 1u) << pin); /* 0x08006da6 */
        }

        /* ---- PUPDR ---- */
        GPIO_PUPDR(port) = (GPIO_PUPDR(port) & ~(3u << (2 * pin)))
                         | ((cfg[2] & 3u) << (2 * pin));     /* 0x08006db8 */

        /* ---- AFR（mode 2/0x12 时） ---- */
        if ((mode & 3u) == 2) {
            uint32_t *afr = (uint32_t *)(port + 0x20 + (pin >> 3) * 4);
            *afr = (*afr & ~(0xFu << ((pin & 7) * 4)))
                 | ((cfg[3] & 0xFu) << ((pin & 7) * 4));     /* 0x08006d58..0x08006d70 */
        }

        /* ---- EXTI 选线 + 触发 + 屏蔽 ---- */
        if (mode & 0x10000u) {                              /* bit16 = IMR */
            volatile uint32_t *exticr = (volatile uint32_t *)(SYSCFG_BASE + 8 + (pin >> 2) * 4);
            *exticr = (*exticr & ~(0xFu << ((pin & 3) * 4)))
                    | (gpio_port_index(port) << ((pin & 3) * 4)); /* 0x08006ddc..0x08006e28 */
            EXTI_IMR  = (EXTI_IMR  & ~bit) | ((mode & 0x10000u) ? bit : 0);
            EXTI_EMR  = (EXTI_EMR  & ~bit) | ((mode & 0x20000u) ? bit : 0);
            EXTI_RTSR = (EXTI_RTSR & ~bit) | ((mode & 0x100000u) ? bit : 0);
            EXTI_FTSR = (EXTI_FTSR & ~bit) | ((mode & 0x200000u) ? bit : 0);
        }
    }
}

/* 0x08006D3C 内联的端口序号（0=GPIOA … 5=GPIOF） */
uint32_t gpio_port_index(uint32_t port)
{
    switch (port) {
    case GPIOA_BASE: return 0;
    case GPIOB_BASE: return 1;
    case GPIOC_BASE: return 2;
    case GPIOD_BASE: return 3;
    case 0x48001000u: return 4;
    case GPIOF_BASE: return 5;
    default: return 5;
    }
}

/* ------------------------------------------------------------------ board_init_inputs
 * 0x08009758：开 GPIOA/B/C/F 时钟 → 一批 gpio_write_pin 初值 → 10 组 exti_config
 * → NVIC IRQ5/IRQ7 使能。引脚表见 docs/05-pinmap.md §1/§2。
 */
void board_init_inputs(void)
{
    uint32_t cfg[4];

    /* RCC_AHBENR |= GPIOFEN|GPIOCEN|GPIOAEN|GPIOBEN（bit22/19/17/18，0x0800975e） */
    RCC_AHBENR |= RCC_AHBENR_GPIOFEN | RCC_AHBENR_GPIOCEN |
                  RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

    /* ---- 输出初值（0x0800979a..0x080097e6） ---- */
    gpio_write_pin(GPIOF_BASE, 0x0001, 0);   /* PF1=0 */
    gpio_write_pin(GPIOC_BASE, 0x0048, 1);   /* PC3,PC6 = 1 */
    gpio_write_pin(GPIOA_BASE, 0x0011, 1);   /* PA0,PA4 = 1 */
    gpio_write_pin(GPIOA_BASE, 0x0402, 0);   /* PA1,PA10 = 0 */
    gpio_write_pin(GPIOC_BASE, 0x03A0, 0);   /* PC5,7,8,9 = 0 */
    gpio_write_pin(GPIOB_BASE, 0x3006, 0);   /* PB1,2,12,13 = 0 */
    gpio_write_pin(GPIOB_BASE, 0x0BC0, 1);   /* PB6,7,8,9,11 = 1 */

    /* ---- 10 组 EXTI/引脚配置 ---- */
    cfg[0] = 0x0001; cfg[1] = 0x01; cfg[2] = 0; cfg[3] = 0;     /* PF1 输出 */
    exti_config(GPIOF_BASE, cfg);

    cfg[0] = 0x03E8; cfg[1] = 0x01; cfg[2] = 0; cfg[3] = 0;     /* PC3,5,6,7,8,9 输出 */
    exti_config(GPIOC_BASE, cfg);

    cfg[0] = 0x0413; cfg[1] = 0x01; cfg[2] = 0; cfg[3] = 0;     /* PA0,1,4,10 输出 */
    exti_config(GPIOA_BASE, cfg);

    cfg[0] = 0x8304; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;     /* PA2,8,9,15 输入上拉 */
    exti_config(GPIOA_BASE, cfg);

    cfg[0] = 0x0020; cfg[1] = 0x10210000u; cfg[2] = 1; cfg[3] = 0; /* PA5 EXTI5 双边沿+IMR */
    exti_config(GPIOA_BASE, cfg);

    cfg[0] = 0x1C10; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;     /* PC4,10,11,12 输入上拉 */
    exti_config(GPIOC_BASE, cfg);

    cfg[0] = 0x0001; cfg[1] = 0x10210000u; cfg[2] = 1; cfg[3] = 0; /* PB0 EXTI0 */
    exti_config(GPIOB_BASE, cfg);

    cfg[0] = 0x3BC6; cfg[1] = 0x01; cfg[2] = 0; cfg[3] = 0;     /* PB1,2,6..9,11,12,13 输出 */
    exti_config(GPIOB_BASE, cfg);

    cfg[0] = 0xC000; cfg[1] = 0x00; cfg[2] = 0; cfg[3] = 0;     /* PB14,15 输入 */
    exti_config(GPIOB_BASE, cfg);

    cfg[0] = 0x0038; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;     /* PB3,4,5 输入上拉（按键行） */
    exti_config(GPIOB_BASE, cfg);

    NVIC_SetPriority(5, 0); NVIC_EnableIRQ(5);      /* EXTI0_1，0x08009892 */
    NVIC_SetPriority(7, 0); NVIC_EnableIRQ(7);      /* EXTI4_15 */
}

/* ------------------------------------------------------------------ 中断
 * IRQ5 (0x080053F0)：PB0 触发输入。
 *   PR bit0 → 若 (mode&3)==1 → 按 setting[1] 决定单次/重触发，调 flash_fire_sequence。
 * IRQ7 (0x08005484)：PA5(bit5) 备用触发；EXTI15(bit15) → rf/上报路径 0x0800A954。
 * IRQ6 (0x08005478)：仅清线（未使用）。
 */
void EXTI0_1_IRQHandler(void)
{
    if (!(EXTI_PR & 1u))                            /* 0x080053f4 */
        goto done;
    if (g_shutdown_req)                             /* 0x080053fa */
        goto done;
    if ((g_mode & 3u) != 1)                         /* 0x08005402 */
        goto done;

    switch (g_settings[1]) {                        /* 0x20000419 */
    case 1:                                         /* 单次闪光 */
        g_rf_busy = 1;                              /* 0x2000016B */
        flash_fire_sequence();                      /* 0x08005424 0x08005AE8 */
        g_rf_busy = 0;
        idle_timer_reset();                         /* 0x0800542a */
        break;
    case 2:                                         /* 重触发/连拍 */
        if (g_retrigger_ms == 0)
            g_retrigger_ms = 0x2764;                /* 0x08005456: 0x26AC+0x64=10000+ */
        else if (g_retrigger_ms <= 0x26AC) {
            g_rf_busy = 1;
            flash_fire_sequence();
            g_rf_busy = 0;
            idle_timer_reset();
            g_retrigger_ms = 0;
        }
        break;
    }
done:
    exti_clear_pending(1);                          /* 0x0800544e */
}

void EXTI2_3_IRQHandler(void)
{
    exti_clear_pending(8);                          /* 0x0800547a: 清 line3 */
}

void EXTI4_15_IRQHandler(void)
{
    uint32_t pr = EXTI_PR;                          /* 0x08005488 */

    if (pr & (1u << 15) && !g_shutdown_req) {       /* 0x08005490..0x0800549e */
        if (g_rf_enable == 1)
            rf_exti_task();                         /* 0x0800A954 */
        exti_clear_pending(1u << 15);               /* 0x080054a2 */
    }
    if (pr & (1u << 5) && !g_shutdown_req) {        /* PA5 */
        delay_us(50);                               /* 0x080054b6 0x0800C798(0x32) */
        if (gpio_read_pin(GPIOA_BASE, 0x20) == 0) {
            if (g_charge_req == 1 || g_charge_state_0x168 == 0) {
                g_rf_busy = 1;
                flash_fire_sequence();              /* 0x080054e8 */
                g_rf_busy = 0;
                idle_timer_reset();                 /* 0x080054ee */
            }
        }
    }
    exti_clear_pending((1u << 5) | (1u << 4));      /* 0x080054f2 */
}

/* 0x0800CEC4: 充电使能（EXTI_Config + PB11 输出高） */
void charge_power_on(void)
{
    uint32_t cfg[4] = { 0x0800, 0x01, 0, 0 };       /* 0x0800cec4 */
    exti_config(GPIOB_BASE, cfg);
}

/* 0x0800CEE8: 充电关闭 */
void charge_power_off(void)
{
    uint32_t cfg[4] = { 0x0800, 0x01, 0, 0 };
    exti_config(GPIOB_BASE, cfg);
    EXTI_IMR &= ~0x0800u;                            /* 0x0800cefa */
}

/* 0x0800BC08 / 0x0800BBE0：EXTI 组使能与关闭（PA5 line5） */
void exti_enable_group(void)
{
    uint32_t cfg[4] = { 0x0020, 0x10210000u, 1, 0 };
    exti_config(GPIOA_BASE, cfg);                    /* 0x0800bc1e */
    exti_clear_pending(0x20);                        /* 0x0800bc0c */
}

void exti_disable_group(void)
{
    uint32_t cfg[4] = { 0x0020, 0x10210000u, 1, 0 };
    exti_config(GPIOA_BASE, cfg);
    EXTI_IMR &= ~0x20u;
}

/* 0x0800CC14：另一组 EXTI 重配置（PB 侧） */
void exti_config_group(void)
{
    uint32_t cfg[4] = { 0x0001, 0x10210000u, 1, 0 };
    exti_config(GPIOB_BASE, cfg);                    /* 0x0800cc28 */
}
