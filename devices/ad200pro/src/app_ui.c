/*
 * app_ui.c - 3x3 按键矩阵扫描 / 按键分派 / 显示任务 / UI 状态机
 *
 * 重建自：
 *   0x08008818 ui_key_scan_task（299 条）—— PB3/4/5 行 × PC10/11/12 列
 *   0x08008D08 / 0x0800C190 / 0x08009B7C / 0x0800B90C / 0x0800BC34 /
 *   0x08004270 / 0x080042A0 / 0x080048E4 —— 按键处理函数
 *   0x0800C2D8 ui_menu_task
 *   0x08009228 display_task（273 条）
 *   0x0800A2CC status_led_task
 */
#include "ad200.h"

#define ROW0  (1u << 3)     /* PB3 */
#define ROW1  (1u << 4)     /* PB4 */
#define ROW2  (1u << 5)     /* PB5 */
#define COL0  (1u << 10)    /* PC10 */
#define COL1  (1u << 11)    /* PC11 */
#define COL2  (1u << 12)    /* PC12 */

/* 0x08008818: 按键扫描 + 去抖 + 分派。
 * 每轮主循环调用一次：3 个行各驱动低一次，读 3 列 → 9 位键位图。
 * 键码 = 3*row + col（bit0..bit8），见反汇编 0x08008864..0x080089b0。
 */
void key_scan_task(void)
{
    uint32_t key = 0;
    uint32_t cfg[4];

    /* --- 行 0：PB3 输出低，PB4/PB5 输入上拉（0x08008834..0x08008860） --- */
    cfg[0] = ROW0; cfg[1] = 0x01; cfg[2] = 0; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    cfg[0] = ROW1; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    cfg[0] = ROW2; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    GPIO_BRR(GPIOB_BASE) = ROW0;                    /* 0x08008866 PB3=0 */
    delay_us(3);                                    /* 0x0800886c */
    if (!gpio_read_pin(GPIOC_BASE, COL0)) key |= 0x001;   /* 0x08008870 */
    if (!gpio_read_pin(GPIOC_BASE, COL1)) key |= 0x002;   /* 0x08008884 */
    if (!gpio_read_pin(GPIOC_BASE, COL2)) key |= 0x004;   /* 0x08008896 */

    /* --- 行 1：PB4 输出低（0x080088a2..0x08008912） --- */
    cfg[0] = ROW1; cfg[1] = 0x01; cfg[2] = 0; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    cfg[0] = ROW0; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    cfg[0] = ROW2; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    GPIO_BRR(GPIOB_BASE) = ROW1;                    /* 0x080088d4 */
    delay_us(3);
    if (!gpio_read_pin(GPIOC_BASE, COL0)) key |= 0x008;
    if (!gpio_read_pin(GPIOC_BASE, COL1)) key |= 0x010;
    if (!gpio_read_pin(GPIOC_BASE, COL2)) key |= 0x020;

    /* --- 行 2：PB5 输出低（0x0800892e..0x080089b0） --- */
    cfg[0] = ROW2; cfg[1] = 0x01; cfg[2] = 0; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    cfg[0] = ROW0; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    cfg[0] = ROW1; cfg[1] = 0x00; cfg[2] = 1; cfg[3] = 0;
    exti_config(GPIOB_BASE, cfg);
    GPIO_BRR(GPIOB_BASE) = ROW2;                    /* 0x08008948 */
    delay_us(10);                                   /* 0x0800894c */
    if (!gpio_read_pin(GPIOC_BASE, COL0)) key |= 0x040;
    if (!gpio_read_pin(GPIOC_BASE, COL1)) key |= 0x080;
    if (!gpio_read_pin(GPIOC_BASE, COL2)) key |= 0x100;

    /* --- 去抖：同一键连续 50 次扫描才确认（0x080089e2..0x080089f2） --- */
    if (key != 0 && key == g_ui_last_key) {
        if (++g_ui_stable >= 0x32) {                /* 50 */
            g_ui_last_key = 0;
            g_ui_flags[0] = 1;
            key_dispatch(key);                      /* 分派 */
        }
    } else {
        g_ui_stable = 0;
        g_ui_last_key = key;
    }
    if (key != 0)
        idle_timer_reset();                         /* 0x080089c8 有按键 → 复位空闲计时 */
}

/* 0x08008a40..0x08008ab4 的分派表 */
void key_dispatch(uint32_t key)
{
    switch (key) {
    case 0x001: ui_key_1();  break;                 /* 0x08008D08 */
    case 0x002: ui_key_2();  break;                 /* 0x0800C190 */
    case 0x004: ui_key_4();  break;                 /* 0x08009B7C */
    case 0x010: ui_key_10(); break;                 /* 0x080042A0 */
    case 0x020: ui_key_20(); break;                 /* 0x0800B90C */
    case 0x080: ui_key_80(); break;                 /* 0x0800BC34 */
    case 0x100: ui_key_100(); break;                /* 0x08004270 */
    case 0x014: ui_key_14(); break;                 /* 0x080048E4 */
    default: break;
    }
}

/* ------------------------------------------------------------------ UI 状态机
 * 0x0800C2D8：以 0x20000150 为状态结构，做模式/计时/标志更新与 RF 参数下发。
 * 关键字段：+0x12 模式切换、+0x0D 档位（bit0-1 保留）、+0x15/+0x16 闪烁计数、
 *          +0x34 起 6+6 字节字段数组（0xFF 替换为 0x50）。
 */
void ui_menu_task(void)
{
    uint8_t *s = g_ui_state;                        /* 0x20000150 */

    if (s[0x12] == 1) { s[0x12] = 0; return; }
    if (s[0] == 1 || s[0x0E] != 0) {
        s[0] = 0; s[0x0E] = 0; *(uint32_t *)(s + 0x30) = 0;
        return;
    }
    if (++s[0x15] > 1) {
        s[0x15] = 0;
        s[0x16] = 0;
        if ((s[0x0D] & 3) == 0)
            s[0x0D] = 1;
        else
            s[0x0D] = (uint8_t)((s[0x0D] & ~3u) + 1);
    }
    if (s[0x16] == 0)
        s[0x16] = 1;
    for (int i = 0; i < 6; i++) {
        uint8_t *a = s + 0x34, *b = s + 0x3A;
        if (a[i] == 0xFF) a[i] = 0x50;
        if (b[i] == 0xFF) b[i] = 0x50;
    }
    s[0x1E] = 0;
    rf_apply_settings();                            /* 0x0800c34a */
}

/* 0x08009228: 显示任务（273 条）。调用 10 个页/字段渲染器（各 44/45 条，
 * 收尾统一 0x0800CCAC=提交），由数据表 0x0800D01C/0x0800D03C 驱动。
 * 显示接口本身在本固件里没有 AF/并口常量 → 具体物理接口 [推测]。 */
void display_task(void)
{
    display_page_0();       /* 0x08008DEC */
    display_page_1();       /* 0x08008E58 */
    display_page_2();       /* 0x08008EC4 */
    display_page_3();       /* 0x08008F34 */
    display_page_4();       /* 0x08008FA0 */
    display_page_5();       /* 0x0800900C */
    display_page_6();       /* 0x08009078 */
    display_page_7();       /* 0x080090E4 */
    display_page_8();       /* 0x08009150 */
    display_page_9();       /* 0x080091BC */
    display_commit();       /* 0x0800CCAC */
}

/* 0x0800A2CC: 状态 LED/指示 —— 写 PC3，配合 0x0800A260 的计时 */
void status_led_task(void)
{
    gpio_write_pin(GPIOC_BASE, 0x0008, 1);           /* 0x0800a2d4 PC3=1 */
    led_blink_timer();                               /* 0x0800a260 */
}
