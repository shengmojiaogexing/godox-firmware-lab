/*
 * main_loop.c - main / 主循环任务分派 / 空闲与关机状态机
 *
 * 重建自：
 *   0x0800CDB0 main（83 条）
 *   0x0800BA5C system_reinit_task（89 条）
 *   0x0800BB7C autooff_task（32 条）
 *   0x0800BA40 idle_timer_reset（8 条）
 *   0x0800BD68 app_state_init（35 条）
 *   0x0800AC24 boot_reason_check（17 条）
 */
#include "ad200.h"

/* .data / .bss 里的全局（地址逐个来自反汇编 literal 池） */
uint32_t g_tick_ms;             /* 0x20000100 */
uint32_t g_systick_reload;      /* 0x20000104 */
uint8_t  g_irq_latch;           /* 0x20000108 */
uint8_t  g_tim2_flag;           /* 0x20000109 */
uint8_t  g_key_debounce[4];     /* 0x20000122 */
uint8_t  g_ui_state[0x40];      /* 0x20000150 */
uint8_t  g_mode;                /* 0x2000015D */
uint8_t  g_charge_req;          /* 0x20000162 */
uint8_t  g_rf_enable;           /* 0x20000165 */
uint8_t  g_rf_channel;          /* 0x2000016C */
uint8_t  g_rf_rx_active;        /* 0x2000019B */
uint8_t  g_rf_rx_done;          /* 0x200001A1 */
uint16_t g_adc_raw;             /* 0x200001A4 */
uint16_t g_flash_pulse_us;      /* 0x200001B2 */
uint16_t g_retrigger_ms;        /* 0x200001B8 */
uint8_t  g_boot_reason;         /* 0x200001BF */
uint16_t g_idle_ticks;          /* 0x200001C0 */
uint8_t  g_shutdown_req;        /* 0x200001C2 */
uint8_t  g_autooff_cnt;         /* 0x200001C3 */
uint8_t  g_autooff_lim;         /* 0x200001C4 */
uint8_t  g_tim14_div;           /* 0x200001CE */
uint8_t  g_main_wake;           /* 0x200001CF */
uint8_t  g_rf_status[8];        /* 0x20000200 */
uint8_t  g_rtc_flag;            /* 0x2000020D */
uint8_t  g_settings[8];         /* 0x20000418 */
uint32_t g_flash_params[8];     /* 0x20000464 */

/* ------------------------------------------------------------------ main
 * 0x0800CDB0：先复制向量表 + MEM_MODE=SRAM，然后一整串初始化，
 * 最后进入"等 0x200001CF"的调度循环（0x0800CE46–0x0800CEA2）。
 */
int main(void)
{
    int i;

    /* ---- 0x0800cdb2..0x0800cdc4: 48 字向量表 0x08003000 -> 0x20000000 ---- */
    for (i = 0; i < 48; i++)
        ((uint32_t *)RAM_BASE)[i] = ((const uint32_t *)IMG_BASE)[i];

    /* ---- 0x0800cdc6: SYSCFG_CFGR1 |= 3   (MEM_MODE = SRAM) ---- */
    SYSCFG_CFGR1 |= 3;

    flash_acr_init();          /* 0x0800cdd0  0x08007044 FLASH_ACR |= 0x10 */
    clock_init_task();         /* 0x0800cdd4  0x0800BC98 时钟 + SysTick */
    board_init_inputs();       /* 0x0800cdd8  0x08009758 GPIO 输入 + EXTI0/5 + NVIC */
    dma1_clock_init();         /* 0x0800cddc  0x08009730 */
    tim14_init();              /* 0x0800cde0  0x080099C0  主循环心跳定时器 */
    adc_dma_init();            /* 0x0800cde4  0x08009698 */
    iwdg_init_default();       /* 0x0800cde8  0x080098E0  PR=6, RLR=0xFFF */
    rtc_init_task();           /* 0x0800cdec  0x0800993C */
    tim1_init();               /* 0x0800cdf0  0x080099EC */
    tim3_init();               /* 0x0800cdf4  0x08009A4C */
    tim17_init();              /* 0x0800cdf8  0x08009AAC */

    /* ---- 0x0800cdfc: RCC_APB1ENR |= PWREN(bit28) ---- */
    RCC_APB1ENR |= RCC_APB1ENR_PWREN;
    (void)RCC_APB1ENR;

    __disable_irq();           /* 0x0800ce0c  msr primask,#1 */
    rtc_clock_disable();       /* 0x0800ce14  0x08007A30  + 保存 RCC_CSR 值 */
    iwdg_start();              /* 0x0800ce1a  0x08006FBC  KR=0xCCCC,0xAAAA */
    iwdg_refresh();            /* 0x0800ce20  0x08006F68 */
    boot_reason_check();       /* 0x0800ce24  0x0800AC24 */
    settings_load_task();      /* 0x0800ce28  0x0800AC58 */
    app_state_init();          /* 0x0800ce2c  0x0800BD68 */
    rf_tx_task();              /* 0x0800ce30  0x0800AAF4 */

    /* ---- 0x0800ce34: RCC_CSR |= (1<<23)，清复位标志 ---- */
    RCC_CSR |= (1u << 23);

    g_rtc_flag = 0;
    __enable_irq();            /* 0x0800ce48 */

    for (;;) {
        /* 等 TIM14 分频中断置位（0x0800C1A8） */
        while (g_main_wake != 1)
            ;                                       /* 0x0800ce46..0x0800ce50 */
        g_main_wake = 0;                            /* 0x0800ce52 */

        iwdg_refresh();                             /* 0x0800ce56 */

        /* ---- 主循环任务表：地址顺序即调用顺序（0x0800ce5a..0x0800ce9e） ---- */
        key_scan_task();            /* 0x0800ce5a  0x08008818 */
        flash_power_task();         /* 0x0800ce5e  0x08004500 */
        adc_task();                 /* 0x0800ce62  0x08003598 */
        charge_monitor();           /* 0x0800ce66  0x08005A04 */
        measurement_task();         /* 0x0800ce6a  0x08005F6C */
        gpio_output_task();         /* 0x0800ce6e  0x0800389C */
        power_switch_task();        /* 0x0800ce72  0x08003904 */
        display_task();             /* 0x0800ce76  0x08009228 */
        power_level_dispatch();     /* 0x0800ce7a  0x08006118 */
        tick_counter_task();        /* 0x0800ce7e  0x08008364 */
        rf_poll_settings();         /* 0x0800ce82  0x0800B5B0 */
        status_led_task();          /* 0x0800ce86  0x0800A2CC */
        ui_power_switch_task();     /* 0x0800ce8a  0x08003BEC */
        rf_wakeup_sequence();       /* 0x0800ce8e  0x0800C680 */
        charge_enable_task();       /* 0x0800ce92  0x0800BA00 */
        rf_rx_timeout_counter();    /* 0x0800ce96  0x08009CF0 */
        ui_factory_reset_task();    /* 0x0800ce9a  0x08009B38 */
        system_reinit_task();       /* 0x0800ce9e  0x0800BA5C */
    }
}

/* ------------------------------------------------------------------ 空闲/自动关机
 * 0x0800BB7C：每轮主循环调用。setting[2] 是"自动关机分钟数"。
 * 计数 0x200001C0 累加到 0x2EE0(12000) 记 1 分钟，满 setting*30 次 → 请求关机。
 */
void autooff_task(void)
{
    if (g_settings[2] == 0)
        return;                                     /* 0x0800bb82 */

    if (++g_idle_ticks >= 0x2EE0) {                 /* 0x0800bb8a..0x0800bb94 */
        g_idle_ticks = 0;
        g_autooff_cnt++;                            /* 0x200001C3 */
        g_autooff_lim = (uint8_t)(g_settings[2] * 30);
        if (g_autooff_cnt >= g_autooff_lim) {
            g_autooff_cnt = 0;
            g_shutdown_req = 1;                     /* 0x0800bbb6 关键：请求关机 */
        }
    }
}

/* 0x0800BA40：任何用户活动复位空闲计数并清除关机请求 */
void idle_timer_reset(void)
{
    g_idle_ticks   = 0;                             /* 0x0800ba44 */
    g_autooff_cnt  = 0;                             /* 0x0800ba48 */
    g_shutdown_req = 0;                             /* 0x0800ba4c */
}

/* ------------------------------------------------------------------ app_state_init
 * 0x0800BD68：开机应用初始化（PA1=1 上电自锁、延时 100ms、RF 初始化、RTC/TIM 启动）
 */
void app_state_init(void)
{
    gpio_write_pin(GPIOA_BASE, 0x0002, 1);          /* 0x0800bd72  PA1=1 */
    delay_10ms(10);                                 /* 0x0800bd78 0x0800C808(100us×10) */
    reset_machine_state();                          /* 0x0800bd7c 0x08003AF4 */
    g_flash_flag_0x169 = 1;                         /* 0x20000169 */
    g_retrigger_lo = 0;                             /* 0x2000017A */
    g_flag_0x16A = 1;                               /* 0x2000016A */
    g_flag_0x16D = 1;                               /* 0x2000016D */
    gpio_write_pin(GPIOB_BASE, 0x0800, 1);          /* 0x0800bd9a  PB11=1 (充电使能) */
    rf_bus_init();                                  /* 0x0800bd9e 0x080084D4 */
    g_rf_timeout_0x1C8 = 250;                       /* 0x0800bda4 */
    charge_monitor();                               /* 0x0800bda8 0x08005A04 */
    rf_init();                                      /* 0x0800bdac 0x08004860 */
    g_flag_0x16E = 0;                               /* 0x0800bdb2 */
    settings_save();                                /* 0x0800bdb4 0x0800CC64 */
    tim_timebase_start(&g_tim_dev_3cc);             /* 0x0800bdba 0x08007DD4(&0x200003CC) */
}

/* ------------------------------------------------------------------ boot_reason_check
 * 0x0800AC24：RCC_CSR 复位标志 + 0x0801F800 魔数(0x100E) 决定 g_boot_reason。
 * 反汇编：0x0800ac2e ldr RCC_CSR; 0x0800ac32 lsls #5 (测 bit26=引脚复位);
 *         0x0800ac38 flash_read_word(0x0801F800); 0x0800ac40 cmp #0x100E。
 */
void boot_reason_check(void)
{
    uint32_t csr;

    g_boot_reason = 0;
    csr = RCC_CSR;
    if (((csr << 5) & 0x80000000u) == 0) {          /* bit26 == 0 → 非引脚复位 */
        g_boot_reason = 0x7B;
        return;
    }
    if (flash_read_word(FLASH_FLAG_PAGE) == 0x100E)
        g_boot_reason = 0x7B;
}

/* ------------------------------------------------------------------ system_reinit_task
 * 0x0800BA5C：关机 / 重新初始化状态机。只有 g_shutdown_req==1 时才执行。
 * 流程：清中断 → gpio_init_all（所有引脚回安全态）→ 启动 IWDG(3000ms) →
 *       RTC/时钟/EXTI/TIM 重新初始化 → 进入低功耗方向。
 * 反汇编 0x0800ba5e..0x0800bb54。
 */
void system_reinit_task(void)
{
    autooff_task();                                 /* 0x0800ba5e 0x0800BB7C */

    if (g_shutdown_req != 1)                        /* 0x0800ba62 */
        return;

    __disable_irq();                                /* 0x0800ba6a */
    gpio_init_all();                                /* 0x0800ba6e 0x08005544 引脚全部回默认 */
    iwdg_init_timeout(3000);                        /* 0x0800ba74 0x0800990C */
    rtc_clock_enable();                             /* 0x0800ba7a 0x08007A58 */
    tim_oc_setup_0x84D4();                          /* 0x0800ba7e */
    __enable_irq();                                 /* 0x0800ba82 */
    g_rtc_flag = 0;                                 /* 0x0800ba8c */
    exti_enable_group();                            /* 0x0800ba8e 0x0800BC08 */
    charge_power_off();                             /* 0x0800ba92 0x0800CEE8 */
    reset_rf_state();                               /* 0x0800ba96 0x0800608C */
    rf_bus_deinit();                                /* 0x0800ba9a 0x0800CBDC */
    delay_us(10);                                   /* 0x0800baa0 */
    tim_oc_setup_0x7804();                          /* 0x0800baa4 */
    tim_oc_apply();                                 /* 0x0800baaa 0x08007878 */
    __enable_irq();                                 /* 0x0800bab6 */
    sleep_counter_reset();                          /* 0x0800babe 0x08006F68? 实际 0x08006F68=refresh */
    low_power_sleep(1, 1);                          /* 0x0800bac6 0x080070F0 */
    __disable_irq();                                /* 0x0800bacc */
    iwdg_refresh();                                 /* 0x0800bad2 */
    /* 0x0800bad6..0x0800bb08: 读 PB3/PB4/PB5（按键）判断是否要保持关机 */
    if (g_rtc_flag == 1 || /* PB 状态 */ 0) {
        /* 0x0800baea: PC9=1 → delay 10000 → PC9=0（面板电压放电） */
        gpio_write_pin(GPIOC_BASE, 0x0200, 1);
        delay_us(10000);
        gpio_write_pin(GPIOC_BASE, 0x0200, 0);
    }
    /* 0x0800bb22: 重新初始化：时钟、板级、TIM、GPIO，回到正常运行 */
    __disable_irq();
    clock_init_task();
    exti_config_group();
    board_init();
    tim_oc_setup_0x8340();
    reset_machine_state();
    exti_disable_group();
    charge_power_off();
    iwdg_init_default();
    rtc_clock_disable();
    __enable_irq();
}
