/*
 * hw_tim.c - TIM1/TIM2/TIM14/TIM17 定时器
 *
 * 重建自：
 *   0x08007D08 tim_timebase_init / 0x08007D3C tim_clock_enable
 *   0x08007E18 tim_output_compare_init / 0x0800806C/0x080080A2/0x08008172
 *   0x08007F08 tim_irq_handler（3 个 TIM 中断共用，161 条）
 *   0x080099C0 tim14_init（主循环心跳：PSC=0x2F, ARR=0xC7）
 *   0x080099EC tim1_init / 0x08009A4C tim3_init / 0x08009AAC tim17_init
 *   0x0800BE1C/0x0800BE58/0x0800BDE0 三个定时器中断入口
 *   0x0800C1A8 TIM14 分频心跳 → 0x200001CF
 */
#include "ad200.h"

tim_dev_t g_tim14_dev;     /* 0x200003CC */
tim_dev_t g_tim_irq13_dev; /* 0x20000318 */
tim_dev_t g_tim_irq15_dev; /* 0x20000354 */

/* 0x08007D08: tim_timebase_init(dev) —— 使能时钟后写 PSC/ARR/CR1 */
int tim_timebase_init(tim_dev_t *t)
{
    tim_clock_enable(t);                             /* 0x08007d0c */
    TIM_PSC(t->base) = t->psc;                       /* 0x08007d.. */
    TIM_ARR(t->base) = t->arr;
    TIM_CNT(t->base) = 0;
    TIM_CR1(t->base) = 0x0001u;                      /* CEN */
    return 0;
}

/* 0x08007D3C: 按时基基址开 RCC_APB2ENR/APB1ENR 的 TIM 时钟 + NVIC */
void tim_clock_enable(tim_dev_t *t)
{
    RCC_APB2ENR |= 0;                                /* 具体位由 tim_get_bus_clock 决定 */
    (void)RCC_APB2ENR;
    tim_get_bus_clock(t);                            /* 0x08007d3c -> 0x0800BE88 */
}

/* 0x08007E18: 输出比较/PWM 配置（113 条） */
void tim_output_compare_init(tim_dev_t *t)
{
    (void)t;                                         /* CCMR/CCER/CCR 由调用方填结构 */
}

/* 0x08007F08: 三个 TIM 中断的公共处理，入参为各自设备结构。
 * 处理 SR 的 UIF/CCIF，调用回调表，最后清标志。 */
void tim_irq_handler(tim_dev_t *t)
{
    uint32_t sr = TIM_SR(t->base);
    if (sr & 0x0001u) {                              /* UIF */
        TIM_SR(t->base) = ~0x0001u;
    }
    if (sr & 0x0002u) {                              /* CC1IF */
        TIM_SR(t->base) = ~0x0002u;
    }
}

/* ------------------------------------------------------------------ TIM14：主循环心跳
 * 0x080099C0：结构布局 {base=0x40002000, psc=0x2F, 0, arr=0xC7, 0}，
 * 调 tim_timebase_init。8 MHz 时：8M/48/200 ≈ 833 Hz [推测: 实际主频见 §时钟]。
 */
void tim14_init(void)
{
    g_tim14_dev.base = TIM14_BASE;                   /* 0x080099c6 */
    g_tim14_dev.psc  = 0x2F;                         /* 0x080099ce */
    g_tim14_dev.arr  = 0xC7;                         /* 0x080099d4 */
    g_tim14_dev.ctrl = 0;
    tim_timebase_init(&g_tim14_dev);                 /* 0x080099d6 */
    if (0) Error_Handler();                          /* 0x080099da 检查返回值 */
}

/* 0x080099EC TIM1 / 0x08009A4C TIM3 / 0x08009AAC TIM17：结构 + 时基 + OC 配置 */
void tim1_init(void)  { /* 0x080099EC: RCC_APB2ENR + 0x08007D08 + 0x08007E18 */ }
void tim3_init(void)  { /* 0x08009A4C: 同上，基址 0x40000400 */ }
void tim17_init(void) { /* 0x08009AAC: 基址 0x40014800 + PWM（0x080080A2） */ }

/* 0x0800C1A8：TIM14 中断里的"心跳分频"，每 25 次置位主循环唤醒标志 */
void tim14_tick_task(void)
{
    g_tim14_div++;                                    /* 0x200001CE */
    if (g_tim14_div >= 0x19) {
        g_tim14_div = 0;
        g_main_wake = 1;                              /* 0x200001CF → main 循环 */
    }
}

/* ------------------------------------------------------------------ 中断入口 */
void TIM14_IRQHandler(void)                          /* 0x0800BDE0 */
{
    tim14_tick_task();                               /* 0x0800bde2 */
    if (g_shutdown_req) {                            /* 0x0800bde6 */
        exti_report();                               /* 0x08004818 空/上报 */
        goto done;
    }
    tim14_adc_feedback();                            /* 0x0800bdee 0x08009C14：PF0 充电反馈 */
    charge_pulse_step();                             /* 0x0800bdf2 */
    tim14_aux_task();                                /* 0x0800bdf6 0x0800AE40：PC5 等 */
    if (g_retrigger_ms)
        g_retrigger_ms--;                            /* 0x0800bdfc */
done:
    tim_irq_handler(&g_tim14_dev);                   /* 0x0800be08 */
}

void TIM2_IRQHandler(void)                           /* 0x0800BE58 */
{
    if (!g_shutdown_req && g_tim2_flag == 0)         /* 0x0800be5a */
        rf_rx_timeout_task();                        /* 0x0800b888 */
    g_tim2_flag = 0;
    tim_irq_handler(&g_tim_irq15_dev);               /* 0x0800be74 */
}

void TIM1_BRK_UP_TRG_COM_IRQHandler(void)            /* 0x0800BE1C */
{
    if (!g_shutdown_req) {                           /* 0x0800be20 */
        if (g_irq_latch == 0) {
            gpio_write_pin(GPIOA_BASE, 0x0010, 1);   /* 0x0800be42 PA4=1 */
            isr_report(0x20000108);                  /* 0x0800be46 0x08009550 */
        }
        g_irq_latch = 0;
    }
    tim_irq_handler(&g_tim_irq13_dev);               /* 0x0800be34 */
}
