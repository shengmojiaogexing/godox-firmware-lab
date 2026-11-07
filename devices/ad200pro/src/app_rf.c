/*
 * app_rf.c - 2.4G 收发器（CC2500 兼容寄存器/命令集的）上层协议
 *
 * 芯片判定依据（详见 docs/04-subsystems.md §6）：
 *   - 读寄存器命令 = addr | 0x80；写 = addr；另有 strobe 0x30/0x33/0x34/0x36/0x39/0x3A；
 *   - 寄存器配置 0x00–0x2E 与 CC2500 寄存器映射逐项对应（SYNC=0xC368、FREQ≈2413MHz、MSK）。
 *
 * 重建自：
 *   0x08004860 rf_init          0x0800C858 rf_chip_config
 *   0x0800AA8C rf_apply_settings 0x0800B5B0 rf_poll_settings
 *   0x0800B9AC rf_set_power_level 0x0800B96C rf_write_pair
 *   0x0800B888 rf_rx_timeout_task / 0x08009D2C rf_rx_state_reset
 *   0x0800B4EC charge_pump_burst（PA7 脉冲串，见 app_flash_charge.c）
 *   0x0800E370 发射功率表（16 字节）
 */
#include "ad200.h"

/* 0x0800E370：索引 → PATABLE(0x3E) 值。00 50 44 C0 84 81 46 93 55 8D C6 69 6E 7F A9 BB */
static const uint8_t g_pa_table[16] = {
    0x00, 0x50, 0x44, 0xC0, 0x84, 0x81, 0x46, 0x93,
    0x55, 0x8D, 0xC6, 0x69, 0x6E, 0x7F, 0xA9, 0xBB
};

/* 0x0800C858: rf_chip_config —— 写 0x00..0x2E，然后 strobe + 校准
 * 逐条立即数见反汇编 0x0800c862..0x0800c9fc。
 */
void rf_chip_config(void)
{
    static const uint8_t regs[][2] = {
        {0x00, 0x29}, {0x01, 0x2E}, {0x02, 0x06}, {0x03, 0x01},
        {0x04, 0xC3}, {0x05, 0x68}, {0x06, 0x04}, {0x07, 0x00},
        {0x08, 0x00}, {0x09, 0x00}, {0x0A, 0x00}, {0x0B, 0x07},
        {0x0C, 0x00}, {0x0D, 0x5C}, {0x0E, 0xCE}, {0x0F, 0xC4},
        {0x10, 0x1D}, {0x11, 0x3B}, {0x12, 0x73}, {0x13, 0x22},
        {0x14, 0xF8}, {0x15, 0x00}, {0x16, 0x07}, {0x17, 0x30},
        {0x18, 0x08}, {0x19, 0x1D}, {0x1A, 0x1C}, {0x1B, 0xC7},
        {0x1C, 0x00}, {0x1D, 0xB2}, {0x1E, 0x87}, {0x1F, 0x6B},
        {0x20, 0xF8}, {0x21, 0xB6}, {0x22, 0x10}, {0x23, 0xEA},
        {0x24, 0x0A}, {0x25, 0x00}, {0x26, 0x11}, {0x27, 0x41},
        {0x28, 0x00}, {0x29, 0x59}, {0x2A, 0x7F}, {0x2B, 0x3F},
        {0x2C, 0x88}, {0x2D, 0x31}, {0x2E, 0x0B},
    };
    unsigned i;
    uint8_t one = 1;

    for (i = 0; i < sizeof(regs) / sizeof(regs[0]); i++)
        rf_write_reg8(regs[i][0], regs[i][1]);       /* 0x0800c866..0x0800c9d6 */

    rf_write_cmd_buf(0x7E, &one, 1);                 /* 0x0800c9e0 0x0800CB7C */
    rf_write_cmd(0x33);                              /* SCAL  0x0800c9e6 */
    delay_us(1000);                                  /* 0x0800c9f0 */
    rf_write_cmd(0x39);                              /* SFRX  0x0800c9f6 */
    delay_us(1000);                                  /* 0x0800c9fc */
}

/* 0x08004860: rf_init —— 总线、复位、连通自检（读回 0x0D==0x5C）、配置、PATABLE */
void rf_init(void)
{
    int retry;
    uint8_t r;

    rf_bus_init();                                   /* 0x08004864 -> 0x08008778 */
    delay_us(20000);                                 /* 0x08004868 */

    for (retry = 0; retry <= 10; retry++) {          /* 0x08004894 */
        rf_chip_reset();                             /* 0x08004872 */
        rf_chip_config();                            /* 0x08004876 */
        g_rf_status[6] = rf_read_reg8(0x0D);         /* 0x0800487c -> 0x20000206 */
        g_rf_status[7] = rf_read_reg8(0x0E);         /* 0x08004884 -> 0x20000207 */
        delay_ms(10);                                /* 0x0800488a */
        if (g_rf_status[6] == 0x5C)                  /* 0x0800489a SPI 自检通过 */
            break;
    }

    /* 0x0800489e: 写 PATABLE（寄存器 0x3E），索引取 g_rf_status[0] */
    rf_write_reg8(0x3E, g_pa_table[g_rf_status[0] & 0x0F]);   /* 0x080048a6..0x080048ac */
    rf_write_cmd(0x36);                              /* SIDLE 0x080048b4 */
    rf_write_cmd(0x33);                              /* SCAL  0x080048b6 */
    delay_us(1000);                                  /* 0x080048be */
    rf_set_power_level(g_settings[6]);               /* 0x080048c4 -> 0x0800B9AC */
    rf_apply_settings();                             /* 0x080048ca -> 0x0800AA8C */
}

/* 0x0800AA8C: 按当前 RF 状态做 strobe 序列（SIDLE/SCAL/SFTX/SRX） */
void rf_apply_settings(void)
{
    if (g_rf_enable != 1) {                          /* 0x0800aa96 */
        rf_write_cmd(0x36);                          /* SIDLE */
        rf_write_cmd(0x33);                          /* SCAL */
        delay_us(1000);
        rf_write_cmd(0x39);                          /* SFRX */
        rf_write_cmd(0x34);                          /* SRX */
        rtc_noop();                                  /* 0x0800a5c8 */
        return;
    }
    rf_write_pair(g_rf_channel, 0);                  /* 0x0800aac0 -> 0x0800B96C */
    rf_write_cmd(0x36);                              /* SIDLE */
    rf_write_cmd(0x33);                              /* SCAL */
    delay_us(1000);
    rf_write_cmd(0x3A);                              /* SFTX */
    rf_write_cmd(0x34);                              /* SRX */
    rf_write_pair(0, 0);
}

/* 0x0800B5B0: 主循环检测"待生效参数"是否变化 */
void rf_poll_settings(void)
{
    if (g_rf_enable == 0)                            /* 0x0800b5b6 */
        return;
    if (g_rf_status[3] != g_rf_channel)              /* 0x0800b5c2 */
        rf_apply_settings();
    g_rf_status[3] = g_rf_channel;                   /* 0x0800b5cc */
}

/* 0x0800B9AC: 发射功率档设置 —— udivmod32(50000, level+1) → 写 reg4/reg5 */
void rf_set_power_level(uint8_t level)
{
    uint32_t v = udivmod32(50000u, (uint32_t)level + 1);   /* 0x0800b9b4 */

    rf_write_reg8(4, (uint8_t)(v >> 8));             /* 0x0800b9bc */
    rf_write_reg8(5, (uint8_t)v);                    /* 0x0800b9c6 */
    delay_us(20);                                    /* 0x0800b9ca */
    rf_write_cmd(0x33);                              /* SCAL 0x0800b9d2 */
    delay_us(1000);                                  /* 0x0800b9da */
}

/* 0x0800B96C: 写两个连续寄存器（用于地址/参数对） */
void rf_write_pair(uint8_t a, uint8_t b)
{
    (void)b;
    rf_write_reg8(a, b);                             /* 0x0800b96c 简化 */
}

/* 0x08009D2C: 复位 RF 接收状态机 */
void rf_rx_state_reset(void)
{
    g_rf_rx_active = 0;                              /* 0x2000019B */
    g_rf_rx_done   = 0;                              /* 0x200001A1 */
    rf_clear_pending();                              /* 0x08009F64 */
    g_rf_rx_state  = 0;                              /* 0x2000019C */
    g_rf_rx_bytes  = 0;                              /* 0x20000198 */
}

/* 0x0800B888: TIM2 中断中的接收超时状态机（44 条）
 * 依据 0x2000015D(bits1:0)、0x20000168、0x2000019B/0x200001A6/0x200001A1/0x20000199。
 */
void rf_rx_timeout_task(void)
{
    if ((g_mode & 3u) != 2 && g_charge_state_0x168 == 0)   /* 0x0800b88a */
        rf_rx_state_reset();

    if (g_rf_rx_active == 1) {                       /* 0x0800b8a2 */
        if (g_rf_ready_0x1A6 == 1) {
            if (g_rf_rx_done == 1 || g_key_debounce[3] == 1) {
                flash_trigger_pulse();               /* 0x0800b8c2 -> 0x08009E88 闪光脉冲 */
                if (g_rf_busy_0x151 == 0)
                    g_rf_byte_cnt = g_rf_byte_max;
                g_rf_byte_cnt++;
                if (g_rf_byte_cnt < g_rf_byte_limit)
                    return;
            }
        }
        rf_rx_state_reset();                         /* 0x0800b8e0 */
    }
}
