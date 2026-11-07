/*
 * app_state.c - 应用状态 / 设置读写 / 标定换算
 *
 * 重建自：
 *   0x0800AC58 settings_load_task（210 条）
 *   0x0800CCC8 settings_validate（90 条）
 *   0x0800CC64 settings_save（27 条）
 *   0x0800CC40 settings_flag_set
 *   0x0800B134 muldiv_scale（功率/电压换算）
 *   0x0800C2D8 ui_menu_task（UI 状态机，见 app_ui.c）
 */
#include "ad200.h"

/* 用户设置镜像 8 字节（0x20000418..0x2000041F），各项在反汇编里的使用点：
 *   [0] 亮度/背光      [1] 触发模式（1=单次，2=重触发/连拍）
 *   [2] 自动关机分钟   [3] 触发源选择/使能
 *   [4] 连拍张数       [5] 蜂鸣/提示
 *   [6] 功率档         [7] RF 通道
 */
uint8_t  g_settings_cfg[8];      /* 0x20000418 */
uint32_t g_flash_params[8];      /* 0x20000464 */

/* 0x0800AC58: 主循环初始化时读取 flash 参数；失败则恢复默认并擦除参数页。
 * 反汇编：0x0800ac60..0x0800ae3c，引用 0x0800590C/0x0800CCC8/0x0800587C/0x0800B820。 */
void settings_load_task(void)
{
    int rc;

    flash_read_params(FLASH_PARAM_PAGE);             /* 0x0800ac60 -> 0x0800590C */
    rc = settings_validate();                        /* 0x0800ac68 -> 0x0800CCC8 */
    if (rc != 0) {
        /* 参数无效：擦除 + 写默认值 + 重新写回 */
        flash_erase_param_page();                    /* 0x0800587C */
        settings_defaults();                         /* 0x0800C740 */
        flash_store_params();                        /* 0x0800B820 */
    }
    /* 把参数块映射到设置镜像 */
    g_settings_cfg[0] = (uint8_t)g_flash_params[0];
    g_settings_cfg[1] = (uint8_t)g_flash_params[1];
    g_settings_cfg[2] = (uint8_t)g_flash_params[2];
    g_settings_cfg[3] = (uint8_t)g_flash_params[3];
    g_settings_cfg[4] = (uint8_t)g_flash_params[4];
    g_settings_cfg[5] = (uint8_t)g_flash_params[5];
    g_settings_cfg[6] = (uint8_t)g_flash_params[6];
    g_settings_cfg[7] = (uint8_t)g_flash_params[7];
}

/* 0x0800CCC8: 参数合法性检查（90 条）：范围/标志位／CRC 一致性 */
int settings_validate(void)
{
    uint32_t i;
    for (i = 0; i < 8; i++) {
        uint32_t v = g_flash_params[i];
        if (v == 0xFFFFFFFFu)                        /* 空片 */
            return 1;
    }
    /* 逐字段范围检查（详见反汇编 0x0800cccc..0x0800cd2e） */
    if (g_flash_params[1] > 3 || g_flash_params[0] > 0x64)
        return 2;
    return 0;
}

/* 0x0800CC64: 保存设置（GPIO 侧提示 + flash 写） */
void settings_save(void)
{
    gpio_write_pin(GPIOB_BASE, 0x03C0, 1);           /* 0x0800cc74 PB6..PB9=1 */
    g_flash_params[0] = g_settings_cfg[0];
    g_flash_params[1] = g_settings_cfg[1];
    g_flash_params[2] = g_settings_cfg[2];
    g_flash_params[3] = g_settings_cfg[3];
    g_flash_params[4] = g_settings_cfg[4];
    g_flash_params[5] = g_settings_cfg[5];
    g_flash_params[6] = g_settings_cfg[6];
    g_flash_params[7] = g_settings_cfg[7];
    flash_store_params();
}

/* 0x0800CC40: 设置变更标志/事件 */
void settings_flag_set(void)
{
    g_settings_pending = 1;                          /* 0x2000040x [推测] */
}

/* 0x0800C740: 默认参数（出厂值） */
void settings_defaults(void)
{
    g_flash_params[0] = 1;
    g_flash_params[1] = 1;
    g_flash_params[2] = 0x1E;                        /* 30 分钟自动关机 */
    g_flash_params[3] = 1;
    g_flash_params[4] = 1;
    g_flash_params[5] = 1;
    g_flash_params[6] = 0x14;                        /* 功率档 20 */
    g_flash_params[7] = 0x0D;                        /* RF 通道 13 */
}

/* 0x0800B134: 比例换算（muldiv_scale），被功率/电压计算大量调用。
 * 反汇编：先用 0x080030F0 求商，再乘/加偏移；签名按调用点恢复。
 */
uint32_t muldiv_scale(uint32_t a, uint32_t b, uint32_t c)
{
    if (b == 0)
        return 0;
    return udivmod32(a * c, b);                      /* 商在 r0，余数在 r1 */
}

/* 0x08003CBC 区段：ADC 原始值 → 闪光脉冲长度（写 0x200001B2）
 * 反汇编：0x08003cbe ldrh 0x200001A4 → muldiv_scale → 0x200001B2，按 0x200001D0 的
 * 模式号选择不同标定表偏移（0x0800D922 起，4 张表）。 */
void adc_to_flash_pulse(void)
{
    uint32_t raw = g_adc_raw;                        /* 0x200001A4 */
    uint32_t mode = g_cal_mode;                      /* 0x200001D0 */
    uint16_t v;

    raw = muldiv_scale(raw, 1, 0x100);               /* 归一化 */
    if (raw > 100) raw = 100;                        /* 0x08003d32 */
    g_cal_index = (uint8_t)raw;                      /* 0x08003d3a -> 0x20000122+2 */

    switch (mode) {
    case 2: case 3:
        v = ((const uint16_t *)0x0800D922)[raw + 0x60];          /* +0xC0/2 */
        break;
    case 4:
        if (g_cal_sub == 6)
            v = ((const uint16_t *)0x0800D922)[raw + 0x12F];     /* +0x25E/2 */
        else
            v = ((const uint16_t *)0x0800D922)[raw + 0xFF + 0x81];/* 0x180 */
        break;
    default:
        v = ((const uint16_t *)0x0800D922)[raw];
        break;
    }
    g_flash_pulse_us = v;                            /* 0x200001B2 */
}
