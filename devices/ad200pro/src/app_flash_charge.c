/*
 * app_flash_charge.c - 电容充电 / 闪光触发 / 功率计算
 *
 * 重建自：
 *   0x08004500 flash_power_task        0x080045AC flash_charge_state_machine
 *   0x08008504 flash_trigger_state_machine
 *   0x08005AE8 flash_fire_sequence     0x08009E88 flash_trigger_pulse
 *   0x08009E14 / 0x08009560 两条 arm 路径
 *   0x0800B4EC charge_pump_burst（PA7 脉冲串）
 *   0x08008204 收尾/输出更新
 *   0x08006118 / 0x08006188 功率档分派（7 个模式）
 *   0x0800E000-0x0800E3E0 功率/能量标定表
 */
#include "ad200.h"

/* 0x08004500: 主循环的电源/充电任务：调两个状态机 */
void flash_power_task(void)
{
    if (gpio_read_pin(GPIOA_BASE, 0x0001) == 0)      /* 0x08004502 读 PA0 回读 */
        ;                                            /* [推测] 电源自检 */
    flash_charge_state_machine();                    /* 0x0800450e -> 0x080045AC */
    flash_trigger_state_machine();                   /* 0x08004516 -> 0x08008504 */
    charge_voltage_report();                         /* 0x0800451a -> 0x08009D58 */
}

/* 0x080045AC: 充电状态机（293 条）
 * 引用功率表 0x0800E314（逐字节索引），按 0x2000015D 的模式与 0x200001CE 充电状态
 * 控制 PB11 充电使能、PF0 反馈采样与保护门限。
 */
void flash_charge_state_machine(void)
{
    uint8_t *st = g_ui_state;
    const uint8_t *tbl = (const uint8_t *)0x0800E314;

    switch (st[0x0D] & 3) {
    case 0:                                          /* 空闲 */
        break;
    case 1:                                          /* 充电中 */
        if (tbl[st[0x12]] >= 0x28) {                 /* 门限 [推测] */
            charge_power_on();                       /* PB11 使能 */
            st[0x13] = 1;
        }
        break;
    case 2:                                          /* 已充满/待触发 */
        if (st[0x13] == 1) {
            charge_power_off();
            st[0x13] = 0;
        }
        break;
    default:
        break;
    }
}

/* 0x08008504: 触发状态机（298 条）：触发前后按功率档调整充电时长/脉宽，
 * 引用 0x0800E314 表，调 0x0800B9AC/0x0800B810/0x080038F0 等。 */
void flash_trigger_state_machine(void)
{
    uint8_t *st = g_ui_state;
    switch (st[0x0D] & 3) {
    case 0:
        break;
    case 1:                                          /* 准备触发 */
        rf_set_power_level(g_settings_cfg[6]);       /* 0x0800B9AC */
        st[0x14] = 1;
        break;
    case 2:                                          /* 触发完成/复位 */
        st[0x14] = 0;
        break;
    }
}

/* ------------------------------------------------------------------ 触发核心
 * 0x08005AE8 flash_fire_sequence：由 EXTI0(PB0) / EXTI5(PA5) / 按键 / RF 路径调用。
 * 依据模式位 (0x2000015D & 3)、0x20000418 设置、0x20000197/0x200001A1 标志决定
 * 是否真正发脉冲以及用哪条 arm 路径。反汇编 0x08005ae8..0x08005be2。
 */
void flash_fire_sequence(void)
{
    if (g_retrigger_busy_0x148 != 0) {               /* 0x08005af2 */
        flash_abort_retrigger();                     /* 0x08005A58 */
        return;
    }

    if (g_mode & 3u) {
        /* --- 连拍/多张（setting[4] = 0x2000041C） --- */
        if ((g_mode & 3u) == 1 && g_settings_cfg[4] && g_flag_0x197 == 0) {
            if (++g_burst_cnt > 2) g_burst_cnt = 0;
            return;
        }
        /* --- 常规触发：按时长表计算脉宽（0x0800D05C） --- */
        if (g_settings_cfg[3] && g_flag_0x197 == 0 && g_rf_rx_done == 0) {
            uint16_t v = ((const uint16_t *)0x0800D05C)[g_rf_rx_done];
            g_trigger_duration = (uint32_t)v * 50;   /* 0x20000180 */
            if (v >= 0x32)
                g_ui_state[0] = 1;
            delay_us(1000);                          /* 0x08005b90 */
        }
        /* --- 发脉冲（两条 arm 路径） --- */
        if (g_rf_ready_0x1A6 == 1 && g_flag_0x1FB == 0) {
            if ((g_mode & 3u) == 1)
                flash_arm_b();                       /* 0x08009560  PA0=1, PA7=0 */
            else
                flash_arm_a();                       /* 0x08009E14  PA0=1 */
        }
    }
    g_rf_busy = 0;                                   /* 0x2000016B */
}

/* 0x08009E88 flash_trigger_pulse：真正的灯管驱动脉冲
 *   PA0=1（使能）→ PA7=1 → delay_us(g_flash_pulse_us) → PA7=0 → delay_us(3000)
 *   → g_flag_0x1A9=0 → 0x08008204（输出收尾）。
 * 反汇编 0x08009e88..0x08009ed4。
 */
void flash_trigger_pulse(void)
{
    if (g_flash_pulse_us == 0)                       /* 0x08009e8c */
        return;

    gpio_write_pin(GPIOA_BASE, 0x0001, 1);           /* 0x08009e9c PA0=1 */
    __disable_irq();                                 /* 0x08009ea2 */
    gpio_write_pin(GPIOA_BASE, 0x0080, 1);           /* 0x08009eac PA7=1 触发 */
    delay_us(g_flash_pulse_us);                      /* 0x08009eb2 脉宽=闪光功率 */
    gpio_write_pin(GPIOA_BASE, 0x0080, 0);           /* 0x08009ebc PA7=0 */
    delay_us(3000);                                  /* 0x08009ec2 */
    __enable_irq();
    g_flag_0x1A9 = 0;
    flash_output_update();                           /* 0x08008204 */
}

/* 0x0800B4EC charge_pump_burst：PA7 软件 PWM 脉冲串（充电变换器）
 * 该函数被 EXTI/RF 完成路径 0x0800A954 调用；60 次循环，三档半周期
 * （0x50 / 0x32 / 0x96 us，取决于 0x200001D0 的状态），每轮喂狗。
 * 反汇编 0x0800b4ee..0x0800b5a0。
 */
void charge_pump_burst(void)
{
    uint32_t n;
    uint32_t half;

    switch (g_cal_mode) {                            /* 0x200001D0 */
    case 1:  half = 0x32; break;
    case 4:  half = 0x96; break;
    default: half = 0x50; break;
    }

    __disable_irq();
    for (n = 0; n < 60; n++) {                       /* 0x0800b532 cmp #0x3c */
        gpio_write_pin(GPIOA_BASE, 0x0080, 1);       /* 0x0800b50e PA7=1 */
        delay_us(half);
        gpio_write_pin(GPIOA_BASE, 0x0080, 0);       /* 0x0800b51e PA7=0 */
        delay_us(0x3A98);                            /* 0x0800b524 固定间隔 */
        iwdg_refresh();                              /* 0x0800b52a */
    }
    __enable_irq();
}

/* 0x08006118: 功率档分派（7 个模式），每个模式调用对应的查表计算函数 */
void power_level_dispatch(void)
{
    switch (g_power_mode) {                          /* 0x2000015D 高 2 位/设置 */
    case 1: level_calc_1(); break;                   /* 0x08009FF0 etc. */
    case 2: level_calc_2(); break;
    case 3: level_calc_3(); break;
    case 4: level_calc_4(); break;
    case 5: level_calc_5(); break;
    case 6: level_calc_6(); break;
    case 7: level_calc_7(); break;
    default: break;
    }
}
