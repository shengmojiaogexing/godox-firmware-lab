/*
 * rf_ui.c — AD100 Pro (Godox) 固件 V1.2 逆向还原：2.4G 无线 + LCD/UI + 按键/电池
 *
 * 目标：Cortex-M0 / STM32F0，固件加载基址 0x08003000，应用区间 0x08003000-0x0800F448。
 * main() = 0x0800CF74。RAM 0x20000000-0x20002000（8 KB）。
 *
 * 说明：
 *  - 每个函数前的注释是固件内绝对地址（证据）。
 *  - 寄存器/位域名按 STM32F0 标准外设库风格命名；名称不确定处标 [?] /。
 *  - 本文件是“可读的等价还原”，不是可直接编译的产物：部分结构体布局、常量含义待确认。
 */

#include <stdint.h>
#include <string.h>

/* ===================================================================== */
/* 0. 地址与宏                                                           */
/* ===================================================================== */

#define APP_BASE            0x08003000u

#define GPIOA_BASE          0x48000000u
#define GPIOB_BASE          0x48000400u
#define GPIOC_BASE          0x48000800u
#define GPIOD_BASE          0x48000C00u
#define GPIOE_BASE          0x48001000u
#define GPIOF_BASE          0x48001400u

#define RCC_BASE            0x40021000u
#define EXTI_BASE           0x40010400u
#define SYSCFG_BASE         0x40010000u
#define ADC_BASE            0x40012400u
#define ADC_CCR             (*(volatile uint32_t *)(ADC_BASE + 0x308u)) /* STM32F0 */
#define IWDG_BASE           0x40003000u
#define IWDG_KR             (*(volatile uint32_t *)(IWDG_BASE + 0x00u))
#define IWDG_PR             (*(volatile uint32_t *)(IWDG_BASE + 0x04u))
#define IWDG_RLR            (*(volatile uint32_t *)(IWDG_BASE + 0x08u))
#define IWDG_SR             (*(volatile uint32_t *)(IWDG_BASE + 0x0Cu))

#define GPIO_MODER(g)       (*(volatile uint32_t *)((g) + 0x00u))
#define GPIO_IDR(g)         (*(volatile uint32_t *)((g) + 0x10u))
#define GPIO_BSRR(g)        (*(volatile uint32_t *)((g) + 0x18u))
#define GPIO_BRR(g)         (*(volatile uint32_t *)((g) + 0x28u))

/* 通用 GPIO 读/写（0x0800741C / 0x08007426） */
static inline int  gpio_read(uint32_t port, uint32_t pin)   /* 0x0800741C */
{
    return (GPIO_IDR(port) & pin) != 0;
}
static inline void gpio_write(uint32_t port, uint32_t pin, int v) /* 0x08007426 */
{
    if (v) GPIO_BSRR(port) = pin;
    else   GPIO_BRR(port)  = pin;
}

/* ===================================================================== */
/* 1. 延时（都是 nop 循环；CPU 48 MHz，1 循环≈1 µs 量级）                 */
/* ===================================================================== */

static void delay_us(uint32_t n)        /* 0x0800CA38 */
{
    do {
        for (volatile int i = 0; i < 2; i++)
            __asm__ volatile("nop;nop;nop;nop;nop;nop;nop;nop;"
                             "nop;nop;nop;nop;nop;nop;nop");
    } while (--n);
}
static void delay_us2(uint32_t n)       /* 0x0800CAB8（同形，另一份拷贝） */
{
    do {
        for (volatile int i = 0; i < 2; i++)
            __asm__ volatile("nop;nop;nop;nop;nop;nop;nop;nop;"
                             "nop;nop;nop;nop;nop;nop;nop");
    } while (--n);
}
static void delay_ms(uint32_t ms)       /* 0x0800CA98: ms * delay_us(990) */
{
    for (uint32_t i = 0; i < ms; i++)
        delay_us(990);
}

/* ===================================================================== */
/* 2. 系统节拍 / 看门狗                                                  */
/* ===================================================================== */

/* 0x20000104: SysTick 递增的毫秒 tick（SysTick 0x0800B8E4 → 0x08008194） */
static uint32_t tick_ms(void)           /* 0x08007434 */
{
    return *(volatile uint32_t *)0x20000104u;
}

/* 中断开关（固件直接 msr PRIMASK） */
static inline void irq_disable(void) { __asm__ volatile("msr PRIMASK, %0" :: "r"(1)); }
static inline void irq_enable(void)  { __asm__ volatile("msr PRIMASK, %0" :: "r"(0)); }

/* IWDG 描述结构：0x20000384（0x080095CC 初始化） */
typedef struct {
    uint32_t base;      /* +0x00 = 0x40003000 (IWDG) */
    uint32_t pr;        /* +0x04 = 6   (LSI/256) */
    uint32_t rlr;       /* +0x08 = 0xFFF */
    uint32_t winr;      /* +0x0C = 0xFFF */
    uint8_t  state;     /* +0x10 */
    uint8_t  phase;     /* +0x11 */
} iwdg_desc_t;

/* 0x08007440: IWDG 初始化/启动（先 KR=0xCCCC 由调用者/本函数写 0x5555 开访问） */
static int wdg_init(iwdg_desc_t *d)
{
    if (!d) return 1;
    /* 等待 SR 中的 PVU/RVU/WVU 清零 */
    if (~(*(volatile uint32_t *)(d->base + 0x0C)) & 0x7u) {
        /* fallthrough: 配置 */
    }
    if (d->phase == 0) { d->state = 0; /* 0x080074EC 清状态 [?] */ }
    d->phase = 2;
    *(volatile uint32_t *)d->base = 0x5555u;            /* 允许写 PR/RLR */
    *(volatile uint32_t *)(d->base + 0x04) = d->pr;     /* PR  */
    *(volatile uint32_t *)(d->base + 0x08) = d->rlr;    /* RLR */
    *(volatile uint32_t *)d->base = 0xCCCCu;            /* 启动 IWDG */
    /* 等待 SR==0（更新完成），超时 1 ms 则记录错误 [?] */
    return 0;
}
/* 0x080074F0: 喂狗（KR=0xAAAA），带超时保护 */
static int wdg_feed(iwdg_desc_t *d)
{
    if (d->state != 1) {
        d->state = 1;
        d->phase = 2;
        uint32_t t0 = tick_ms();
        while (!(*(volatile uint32_t *)(d->base + 0x0C) & 0[?]/)) {
            if (tick_ms() - t0 > 1000) { d->phase = 3; d->state = 0; return 3; }
        }
        *(volatile uint32_t *)d->base = 0xAAAAu;        /* 喂狗 */
        d->phase = 1; d->state = 0;
        return 0;
    }
    return 2;
}

/* ===================================================================== */
/* 3. CC2500 软件 SPI（4 线：CS=PC6, SCLK=PB13, MOSI=PB12, MISO=PB14）    */
/* ===================================================================== */

#define RF_CS_PIN       (1u << 6)       /* PC6  */
#define RF_SCLK_PIN     (1u << 13)      /* PB13 */
#define RF_MOSI_PIN     (1u << 12)      /* PB12 */
#define RF_MISO_PIN     (1u << 14)      /* PB14 */
#define RF_GDO0_PIN     (1u << 15)      /* PA15 (EXTI15) */
#define RF_GDO2_PIN     (1u << 6)       /* PA6  (EXTI6, [?]) */

/* 0x08008C78: RF GPIO 初始化 */
static void rf_gpio_init(void)
{
    /* PC6 = 输出（MODER 对应位 = 01） */
    GPIO_MODER(GPIOC_BASE) &= ~(3u << (6 * 2));
    GPIO_MODER(GPIOC_BASE) |=  (1u << (6 * 2));
    gpio_write(GPIOC_BASE, RF_CS_PIN, 0);

    /* PB13(SCLK)=输出，先拉低 */
    GPIO_MODER(GPIOB_BASE) &= ~(3u << (13 * 2));
    GPIO_MODER(GPIOB_BASE) |=  (1u << (13 * 2));
    GPIO_BRR(GPIOB_BASE) = RF_SCLK_PIN;
    /* PB12(MOSI)=输出，先拉低 */
    GPIO_MODER(GPIOB_BASE) &= ~(3u << (12 * 2));
    GPIO_MODER(GPIOB_BASE) |=  (1u << (12 * 2));
    GPIO_BRR(GPIOB_BASE) = RF_MOSI_PIN;
    /* PB14(MISO)=输入 + 上拉 */
    GPIO_MODER(GPIOB_BASE) &= ~(3u << (14 * 2));
    GPIO_MODER(GPIOB_BASE) &= ~(3u << (14 * 2));                  /* PUPDR: 先清零 */
    *(volatile uint32_t *)(GPIOB_BASE + 0x0C) &= ~(3u << (14 * 2));
    *(volatile uint32_t *)(GPIOB_BASE + 0x0C) |=  (1u << (14 * 2)); /* 上拉 */
    /* PC8 = 输出（RF 模块控制/供电，初值 0）[?] */
    *(volatile uint32_t *)(GPIOC_BASE + 0x00) &= ~(3u << (8 * 2));
    *(volatile uint32_t *)(GPIOC_BASE + 0x00) |=  (1u << (8 * 2));
    gpio_write(GPIOC_BASE, (1u << 8), 0);
}

/* 0x0800CE38 / 0x0800CE58 */
static void rf_cs_low(void)
{
    gpio_write(GPIOC_BASE, RF_CS_PIN, 0);
    while (gpio_read(GPIOB_BASE, RF_MISO_PIN)) { }   /* 等 SO 低 */
}
static void rf_cs_high(void)
{
    gpio_write(GPIOC_BASE, RF_CS_PIN, 1);
}

/* 0x0800CD7C: 写 1 字节（MSB first，同时采样 MISO 拼出读回值） */
static uint8_t spi_write_byte(uint8_t b)
{
    uint8_t in = 0;
    for (int i = 0; i < 8; i++) {
        GPIO_BRR(GPIOB_BASE)  = RF_SCLK_PIN;            /* SCLK 低 */
        if (b & 0x80) GPIO_BSRR(GPIOB_BASE) = RF_MOSI_PIN;
        else          GPIO_BRR(GPIOB_BASE)  = RF_MOSI_PIN;
        GPIO_BSRR(GPIOB_BASE) = RF_SCLK_PIN;            /* SCLK 高 */
        __asm__ volatile("nop;nop;nop;nop");
        GPIO_BRR(GPIOB_BASE)  = RF_SCLK_PIN;
        __asm__ volatile("nop;nop");
        if (gpio_read(GPIOB_BASE, RF_MISO_PIN)) in = (uint8_t)((in << 1) | 1);
        else                                    in = (uint8_t)(in << 1);
        b <<= 1;
    }
    return in;
}
/* 0x0800CD40: 读 1 字节（MSB first） */
static uint8_t spi_read_byte(void)
{
    uint8_t out = 0;
    for (int i = 0; i < 8; i++) {
        GPIO_BSRR(GPIOB_BASE) = RF_SCLK_PIN;            /* SCLK 高 */
        __asm__ volatile("nop");
        out = (uint8_t)(out << 1);
        if (gpio_read(GPIOB_BASE, RF_MISO_PIN)) out |= 1u;
        /* 地址字节后回读时使用；与写时序共用一条时钟线 */
        __asm__ volatile("nop;nop");
        GPIO_BRR(GPIOB_BASE) = RF_SCLK_PIN;
        __asm__ volatile("nop");
    }
    return out;
}

/* 0x0800CDF4: 发送命令选通（CC2500: 0x30 SRES, 0x33 SCAL, 0x34 SRX, 0x35 STX,
 *                            0x36 SIDLE, 0x39 SFRX, 0x3A SFTX, 0x3B SWORRST） */
static void cc2500_cmd(uint8_t cmd)
{
    rf_cs_low();
    spi_write_byte(cmd);
    rf_cs_high();
}
/* 0x0800CD32: 写单寄存器 */
static void cc2500_write_reg(uint8_t reg, uint8_t val)
{
    rf_cs_low();
    spi_write_byte(reg);
    spi_write_byte(val);
    rf_cs_high();
}
/* 0x0800CCA6: 读单寄存器 */
static uint8_t cc2500_read_reg(uint8_t reg)
{
    uint8_t v = 0;
    rf_cs_low();
    spi_write_byte((uint8_t)(0x80u | reg));
    v = spi_read_byte();
    rf_cs_high();
    return v;
}
/* 0x0800CDC8: 突发读 */
static void cc2500_burst_read(uint8_t addr, uint8_t *buf, int len)
{
    rf_cs_low();
    spi_write_byte(addr);
    for (int i = 0; i < len; i++) buf[i] = spi_read_byte();
    rf_cs_high();
}
/* 0x0800CE0C: 突发写 */
static void cc2500_burst_write(uint8_t addr, const uint8_t *buf, int len)
{
    rf_cs_low();
    spi_write_byte(addr);
    for (int i = 0; i < len; i++) spi_write_byte(buf[i]);
    rf_cs_high();
}
/* 0x0800CD24: 写 TX FIFO（0x7F = 0x40|0x3F 突发写） */
static void cc2500_write_tx_fifo(const uint8_t *buf, int len)
{
    cc2500_burst_write(0x7Fu, buf, len);
}
/* 0x0800CC98: 读 RX FIFO（0xFF = 0xC0|0x3F 突发读） */
static void cc2500_read_rx_fifo(uint8_t *buf, int len)
{
    cc2500_burst_read(0xFFu, buf, len);
}

/* 0x0800CCBC: 硬复位序列（拉 CS、等 CHIP_RDY、SRES） */
static void cc2500_reset(void)
{
    gpio_write(GPIOC_BASE, RF_CS_PIN, 1);
    delay_us(30);
    gpio_write(GPIOC_BASE, RF_CS_PIN, 0);
    delay_us(30);
    gpio_write(GPIOC_BASE, RF_CS_PIN, 1);
    delay_us(45);
    rf_cs_low();                 /* 等 MISO 低 */
    rf_cs_high();
    cc2500_cmd(0x30);            /* SRES */
    rf_cs_low();
    while (gpio_read(GPIOB_BASE, RF_MISO_PIN)) { }
    rf_cs_high();
}

/* ===================================================================== */
/* 4. CC2500 配置 / 信道 / ID / 功率                                     */
/* ===================================================================== */

/* 0x0800CAE8: 42 个寄存器一次性写入（SmartRF 250 kbps 类设置） */
typedef struct { uint8_t reg, val; } rf_cfg_t;
static const rf_cfg_t rf_cfg[] = {
    {0x00,0x29},{0x01,0x2E},{0x02,0x06},{0x03,0x01},  /* IOCFG2/1/0, FIFOTHR */
    {0x04,0xC3},{0x05,0x68},{0x06,0x04},{0x07,0x00},  /* SYNC1/0, PKTLEN=4, PKTCTRL1 */
    {0x08,0x00},{0x09,0x00},{0x0A,0x00},{0x0B,0x07},  /* PKTCTRL0(无CRC/固定长), ADDR, CHANNR, FSCTRL1 */
    {0x0C,0x00},{0x0D,0x5C},{0x0E,0xCE},{0x0F,0xC4},  /* FREQ2/1/0 = 2413.0 MHz */
    {0x10,0x1D},{0x11,0x3B},{0x12,0x73},{0x13,0x22},  /* MDMCFG4..1 = 250kbps GFSK */
    {0x14,0xF8},{0x15,0x00},{0x16,0x07},{0x17,0x30},  /* MDMCFG0, DEVIATN [?], MCSM2/1 */
    {0x18,0x08},{0x19,0x1D},{0x1A,0x1C},{0x1B,0xC7},  /* MCSM0, FOCCFG, BSCFG, AGCCTRL2 */
    {0x1C,0x00},{0x1D,0xB2},{0x1E,0x6B},{0x1F,0x87},  /* AGCCTRL1/0, WOREVT1/0 */
    {0x20,0xF8},{0x21,0xB6},{0x22,0x10},{0x23,0xEA},  /* WORCTRL, FREND1/0, FSCAL3 */
    {0x24,0x0A},{0x25,0x00},{0x26,0x11},{0x27,0x41},  /* FSCAL2/1/0, RCCTRL1 [?] */
    {0x28,0x00},{0x29,0x59},{0x2A,0x7F},{0x2B,0x3F},  /* 校准/测试组 [?] */
    {0x2C,0x88},{0x2D,0x31},{0x2E,0x0B},              /* TEST2/TEST1/TEST0 魔数 */
};

/* 0x0800CAE8 */
static void cc2500_init(void)
{
    for (unsigned i = 0; i < sizeof(rf_cfg)/sizeof(rf_cfg[0]); i++)
        cc2500_write_reg(rf_cfg[i].reg, rf_cfg[i].val);

    uint8_t pa = 0xFE;                          /* 0x0800DA9A[?] 缺省；实际见 rf_set_power() */
    cc2500_burst_write(0x7Eu, &pa, 1);          /* PATABLE 突发写 */
    cc2500_cmd(0x33);                           /* SCAL */
    delay_us(1000);
    cc2500_cmd(0x39);                           /* SFRX */
    delay_us(1000);
}

/* 0x0800B758: sync word = 50024 / (id + 1) */
static void rf_set_id(uint8_t id)
{
    uint16_t sync = (uint16_t)(50024u / (id + 1u));   /* 0x080030F0 = __aeabi_uidiv */
    cc2500_write_reg(0x04, (uint8_t)(sync >> 8));     /* SYNC1 */
    cc2500_write_reg(0x05, (uint8_t)sync);            /* SYNC0 */
    delay_us(20);
    cc2500_cmd(0x33);                                 /* SCAL */
    delay_us(1000);
}

/* 0x0800B718: 设置 0..31 信道 */
static const uint8_t ch_freq2[32] = {           /* 0x0800DAAC */
    0x5C,0x5C,0x5C,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,0x5D,
    0x5D,0x5D,0x5D,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,0x5E,
};
static const uint8_t ch_freq1[32] = {           /* 0x0800DACC */
    0xCE,0xDD,0xEC,0x00,0x0E,0x1D,0x31,0x40,0x56,0x62,0x71,0x87,0x93,0xA2,0xB3,0xC4,
    0xD3,0xE9,0xF6,0x04,0x1B,0x27,0x36,0x4C,0x58,0x67,0x7D,0x89,0x98,0xAE,0xBB,0xC9,
};
static const uint8_t ch_freq0[32] = {           /* 0x0800DAEC */
    0xC4,0x89,0x4E,0x00,0xC4,0x89,0x3B,0x00,0x27,0x76,0x3B,0x62,0xB1,0x76,0xB1,0xEC,
    0xB1,0xD8,0x27,0xEC,0x13,0x62,0x27,0x4E,0x9D,0x62,0x89,0xD8,0x9D,0xC4,0x13,0xD8,
};
static void rf_set_channel(uint8_t ch)          /* 0x0800B718，ch=0..31 */
{
    if (ch > 31) ch = 31;
    cc2500_write_reg(0x0D, ch_freq2[ch]);
    cc2500_write_reg(0x0E, ch_freq1[ch]);
    cc2500_write_reg(0x0F, ch_freq0[ch]);
    delay_us(20);
    cc2500_cmd(0x33);                            /* SCAL */
    delay_us(1000);
}

/* 0x08004AF0: 发射功率表（18 字节，0x0800DA9A）与设置 */
static const uint8_t rf_pa_table[18] = {         /* 0x0800DA9A */
    0x00,0x50,0x44,0xC0,0x84,0x81,0x46,0x93,0x55,
    0x8D,0xC6,0x69,0x6E,0x7F,0xA9,0xBB,0xFE,0xFF,
};
static void rf_set_power(uint8_t idx)             /* 0x08004B34-0x08004B40 */
{
    if (idx >= 18) idx = 17;
    cc2500_write_reg(0x3E, rf_pa_table[idx]);    /* PATABLE */
}

/* ===================================================================== */
/* 5. 2.4G 协议：收包 / 发包                                             */
/* ===================================================================== */

/* RAM 包缓冲 0x20000214（4 字节）与状态字节 */
#define RF_PKT          ((volatile uint8_t *)0x20000214u)
#define RF_CFG_MODE     (*(volatile uint8_t *)0x20000175u)  /* &3 = 模式 [?] */
#define RF_CFG_GROUP    (*(volatile uint8_t *)0x2000017Du)
#define RF_STATE        (*(volatile uint8_t *)0x20000185u)
#define RF_SCAN_MODE    (*(volatile uint8_t *)0x2000017Cu)
#define RF_EVT          (*(volatile uint8_t *)0x200001B2u)
#define RF_SETTINGS     ((volatile uint8_t *)0x20000480u)   /* [5]=ID, [0]=模式, [6] [?] */

/* 遥测/状态转发（0x0800B7F0），命令处理的一些动作（地址见 docs/03） */
static void ui_notify(void);
static void rf_power_prepare(void);   /* 0x0800A6CC */
static void rf_power_release(void);   /* 0x0800A6A4 */

/* 0x0800C204: 发送一个 4 字节包 */
static void rf_tx_packet(const uint8_t *pkt, int len)
{
    (void)len;
    rf_power_prepare();
    cc2500_write_tx_fifo(pkt, 4);                /* 0x0800CD24 */
    cc2500_cmd(0x35);                            /* STX  */
    delay_us(0x4B << 3);                         /* 600 µs */
    cc2500_cmd(0x3B);                            /* SWORRST [?] */
    rf_power_release();
}

/* 0x0800AA54: 发送属性帧 [A9][group/0x50][key][value] */
static void rf_send_property(uint8_t group, uint8_t key, uint8_t value)
{
    uint8_t pkt[4];
    pkt[0] = 0xA9;
    pkt[1] = (group == 0x50u) ? 0x50u : (uint8_t)(group + 9u);
    pkt[2] = key;
    pkt[3] = value;
    rf_tx_packet(pkt, 4);
}

/* 属性帧处理：0x0800AFC4（跳表分发，键值 = key-0xB0，case 体在 0x0800B0BE 起） */
static void rf_property_handler(uint8_t group, uint8_t key, uint8_t value)
{
    if (group != 0x50u && group != (uint8_t)(RF_CFG_GROUP + 9u))
        return;
    uint8_t mode = (uint8_t)(RF_CFG_MODE & 3u);
    (void)mode;
    /* switch (key - 0xB0) → 0x0800AFEC 处 __ARM_common_switch8 跳表
     * case 体（0x0800B0BE …）主要动作：
     *   - 修改 RAM_2000019C[组]（各组功率/参数）
     *   - 修改 RAM_20000480[0]（模式）
     *   - 调用 0x0800A620 / 0x08005D7C / 0x08003E2C / 0x08003EB4 / 0x08009FC0 等
     * 完整键码表待确认（docs/03 §9）。
     */
    (void)key; (void)value;
    /* 0x0800B028 为 default 分支 [?] */
    ui_notify();
    /* 0x0800B7F0 后由收包路径重发 RX 命令 */
}

/* 0x0800AA74: EXTI15(GDO0 包收到) 中断服务 → 读 4 字节并分发 */
static void rf_rx_isr(void)
{
    if (RF_STATE == 2u || RF_STATE == 4u || RF_STATE == 5u || RF_STATE == 7u) {
        if (*(volatile uint8_t *)0x200001B2u == 0x1Au) {
            /* 特殊路径（升级/工厂模式？[?]）：0x0800C6F4()，复位状态后重新收 */
            irq_enable();
            RF_PKT[0] = 0;
            *(volatile uint8_t *)0x200001DEu = 0;
            cc2500_write_reg(0x06, 4);              /* 0x0800CDF4? 实为写 PKTLEN [?] */
            ui_notify();
        }
    }
    cc2500_read_rx_fifo((uint8_t *)RF_PKT, 4);     /* 0x0800CC98 */
    switch (RF_PKT[0]) {
    case 0xD5: {                                    /* 命令帧 */
        uint8_t cmd = RF_PKT[1];
        switch (cmd) {
        case 0x80: RF_SETTINGS[0] = 0; break;
        case 0x81: RF_SETTINGS[0] = 1; break;
        case 0x88: *(volatile uint8_t *)0x2000016Au = 0; break;
        case 0x89: *(volatile uint8_t *)0x2000016Au = 1; break;
        case 0x09:                                  /* 事件位置位 + 100 定时 */
        case 0x19:
            irq_disable();
            RF_EVT |= 0x0A;
            if (!(RF_CFG_MODE & 0x03)) *(volatile uint16_t *)0x200001C4u = 100;
            /* 0x0800C6F4() [?] */
            RF_EVT = 0;
            irq_enable();
            break;
        case 0x21:                                  /* 0x08009BCC */
            *(volatile uint8_t *)0x200001AFu = 0;
            *(volatile uint8_t *)0x200001B3u = 0;
            /* 0x08009E04() */
            *(volatile uint8_t *)0x200001B0u = 0;
            *(volatile uint8_t *)0x200001ACu = 0;
            break;
        case 0x29:                                  /* 0x0800B344：口线脉冲 60 次（闪光/模拟 [?]） */
            break;
        default: break;
        }
        ui_notify();
        break;
    }
    case 0xA9:                                      /* 属性帧 */
        rf_property_handler(RF_PKT[1], RF_PKT[2], RF_PKT[3]);
        break;
    default:
        break;
    }
    cc2500_cmd(0x3A);                               /* SFTX */
    cc2500_cmd(0x34);                               /* SRX 重新进入接收 */
}

/* 0x0800AB7C: 进入接收（若 RF_SCAN_MODE==1 先设信道） */
static void rf_start_rx(void)
{
    if (RF_SCAN_MODE == 1u) {
        rf_set_channel(*(volatile uint8_t *)0x20000183u);   /* 当前信道 */
        cc2500_cmd(0x36);                           /* SIDLE */
        cc2500_cmd(0x33);                           /* SCAL  */
        delay_us(1000);
        /* 0x0800A6B8() [?] */
        cc2500_cmd(0x3A);                           /* SFTX */
        cc2500_cmd(0x34);                           /* SRX  */
        /* 0x08006584() [?] */
    } else {
        /* 0x080065AC() [?] */
        cc2500_cmd(0x36);                           /* SIDLE */
        cc2500_cmd(0x33);
        delay_us(1000);
        cc2500_cmd(0x39);                           /* SFRX */
        /* 0x0800A6A4() [?] */
    }
}

/* 0x0800ABE4: 扫描/配对循环（发送 A9 50 B4 02，轮询 PA6） */
static void rf_scan_pair(void)
{
    *(volatile uint8_t *)0x20000213u = 1;           /* 0x2000020E+5: 忙标志 */
    delay_us(10);
    int found = 0;
    if (!gpio_read(GPIOA_BASE, (1u << 6))) found++;
    delay_ms(10);
    if (!gpio_read(GPIOA_BASE, (1u << 6))) found++;
    if (found == 2) {
        /* 0x0800BDC4(0)：清扫描状态 [?] */
        while (*(volatile uint8_t *)0x20000213u) {
            rf_send_property(0x50, 0xB4, 0x02);
            delay_us(600 << 3);                     /* 0x08004? 实际为 0x4B<<3 [?] */
            if (!gpio_read(GPIOA_BASE, (1u << 6))) *(volatile uint8_t *)0x20000213u = 0;
            wdg_feed((iwdg_desc_t *)0x20000384u);
        }
    }
    rf_start_rx();
}

/* 0x08004AF0: 射频初始化总入口（由 0x0800B9C0 启动路径调用） */
static void rf_init(void)
{
    rf_gpio_init();
    delay_us(20000);
    cc2500_reset();
    cc2500_init();
    /* 读回 FREQ2/FREQ1 校验 SPI 通路（最多 10 次等 FREQ2==0x5C） */
    for (int i = 0; i <= 10; i++) {
        RF_PKT[0] = cc2500_read_reg(0x0D);
        RF_PKT[1] = cc2500_read_reg(0x0E);
        delay_ms(10);
        if (RF_PKT[0] == 0x5C) break;
    }
    cc2500_cmd(0x36);                               /* SIDLE */
    rf_set_power(*(volatile uint8_t *)0x2000020Eu); /* 功率索引 [?] */
    cc2500_cmd(0x33);                               /* SCAL  */
    delay_us(1000);
    rf_set_id(RF_SETTINGS[5]);                      /* [5] = ID */
    rf_start_rx();
}

/* 0x0800C7D4: 主循环 RF 保活（若 [0x200001DE] 倒计时到 0 则重发 RX 命令） */
static void rf_keepalive(void)
{
    volatile uint8_t *cnt = (volatile uint8_t *)0x200001DEu;
    if (*cnt == 0) return;
    if (RF_SCAN_MODE == 0) { *cnt = 0; return; }
    if (--(*cnt) != 0) return;
    cc2500_cmd(0x36);                               /* SIDLE */
    cc2500_cmd(0x33);                               /* SCAL  */
    delay_us(1000);
    cc2500_cmd(0x3A);                               /* SFTX  */
    cc2500_cmd(0x34);                               /* SRX   */
    cc2500_write_reg(0x06, 4);                      /* 0x0800CDF4? 实为写寄存器 [?] */
    ui_notify();
}

/* ===================================================================== */
/* 6. LCD（SH1106/SSD1306 族，SPI 4 线）                                  */
/* ===================================================================== */

#define LCD_CS_PIN      (1u << 9)       /* PB9 */
#define LCD_DC_PIN      (1u << 7)       /* PB7：1=数据 0=命令 */
#define LCD_SCLK_PIN    (1u << 6)       /* PB6 */
#define LCD_SI_PIN      (1u << 10)      /* PA10 */

#define LCD_FB          ((uint8_t *)0x200004CCu)   /* 8 页 × 128 字节 */

/* 0x0800A480: 写 1 字节（is_data=1 数据，0 命令） */
static void lcd_write_byte(uint8_t b, int is_data)
{
    gpio_write(GPIOB_BASE, LCD_DC_PIN, is_data);
    gpio_write(GPIOB_BASE, LCD_CS_PIN, 0);
    for (int i = 0; i < 8; i++) {
        GPIO_BRR(GPIOB_BASE) = LCD_SCLK_PIN;                    /* SCLK 低 */
        if (b & 0x80) GPIO_BSRR(GPIOA_BASE) = LCD_SI_PIN;       /* SI 高 */
        else          GPIO_BRR(GPIOA_BASE)  = LCD_SI_PIN;       /* SI 低 */
        GPIO_BSRR(GPIOB_BASE) = LCD_SCLK_PIN;                   /* SCLK 高 */
        b <<= 1;
    }
    GPIO_BRR(GPIOB_BASE)  = LCD_SCLK_PIN;
    gpio_write(GPIOB_BASE, LCD_CS_PIN, 1);
    gpio_write(GPIOB_BASE, LCD_DC_PIN, 1);
}
/* 0x080047BE */
static void lcd_cmd(uint8_t c)  { lcd_write_byte(c, 0); }
static void lcd_data(uint8_t d) { lcd_write_byte(d, 1); }

/* 0x0800C954: LCD 初始化命令序列 */
static void lcd_init_seq(void)
{
    static const uint8_t seq[] = {
        0xAE,       /* 显示关 */
        0x02, 0x10, /* 列地址 = 2（132 列控制器） */
        0x40,       /* 起始行 0 */
        0xB0,       /* 页 0 */
        0x81, 0xFF, /* 对比度 */
        0xA1,       /* 段重映射 */
        0xA6,       /* 正常显示 */
        0xA8, 0x3F, /* 复用率 64 */
        0xAD, 0x8B, /* DC-DC 开 */
        0x33,       /* 泵电压 */
        0xC8,       /* COM 反向 */
        0xD3, 0x00, /* 显示偏移 0 */
        0xD5, 0x80, /* 时钟分频 */
        0xD9, 0x1F, /* 预充电 */
        0xDA, 0x12, /* COM 硬件配置 */
        0xDB, 0x30, /* VCOMH */
        0xA0, 0xC0, /* 段/COM 恢复 */
        0xAF,       /* 显示开 */
    };
    for (unsigned i = 0; i < sizeof(seq); i++)
        lcd_cmd(seq[i]);
}

/* 0x0800A30C: 全屏刷新（8 页 × 128 字节） */
static void lcd_flush(void)
{
    for (int page = 0; page < 8; page++) {
        lcd_cmd((uint8_t)(0xB0 + page));            /* 页地址 */
        for (int x = 0; x < 128; x++)
            lcd_data(LCD_FB[page * 128 + x]);
    }
}
/* 0x0800B5C0: 清屏 + 刷新 */
static void lcd_clear(void)
{
    for (int i = 0; i < 1024; i++) LCD_FB[i] = 0;
    lcd_flush();
}
/* 0x0800D088: LCD 初始化链 */
static void lcd_init(void)
{
    /* 0x0800D134()：GPIO/电源准备 [?] */
    lcd_init_seq();
    lcd_clear();
}

/* 字形记录：9 字节 = [宽度][8 字节位图]（0x08004E1C 证明） */
typedef struct { uint8_t width; uint8_t bitmap[8]; } glyph_t;

/* 0x08004E1C: 在 page 行、x 列画一个字形 */
static void lcd_draw_glyph(int page, int x, int idx, const glyph_t *base)
{
    const glyph_t *g = &base[idx];
    uint8_t *dst = LCD_FB + page * 128 + x;
    for (int i = 0; i < g->width && (x + i) < 128; i++)
        dst[i] = g->bitmap[i];
}

/* ===================================================================== */
/* 7. 按键矩阵扫描（0x08008D00）                                          */
/* ===================================================================== */

/* 键值位图（见 docs/04 §3） */
#define KEY_1       0x0001u
#define KEY_2       0x0002u
#define KEY_3       0x0004u
#define KEY_4       0x0008u
#define KEY_5       0x0010u
#define KEY_6       0x0020u
#define KEY_7       0x0040u
#define KEY_8       0x0080u
#define KEY_9       0x0100u
#define KEY_10      0x0200u
#define KEY_11      0x0400u
#define KEY_COMBO   0x0021u

/* UI 状态结构：0x20000164（只列出已确认字段） */
typedef struct {
    uint8_t  pad0[9];       /* +0x00..+0x08 */
    uint8_t  pressed;       /* +0x09 */
    uint8_t  scan_div;      /* +0x0A */
    uint8_t  hold_cnt;      /* +0x0B */
    uint8_t  hold_latch;    /* +0x0C */
    uint8_t  key_flag_d;    /* +0x0D */
    uint8_t  key_flag_e;    /* +0x0E */
    uint8_t  key_flag_f;    /* +0x0F */
    uint8_t  key_flag_10;   /* +0x10 */
    uint8_t  pad1[0x1B];    /* +0x11..+0x2B */
    uint16_t last_key;      /* +0x2C */
    uint16_t long_delay;    /* +0x2E */
    uint16_t repeat_delay;  /* +0x30 */
    uint8_t  pad2[0x04];    /* +0x32..+0x35 */
} ui_state_t;

/* 0x08008D00：矩阵扫描（行 PB3/PB4/PB5，列 PC10/PC11/PC12/PC7/PD4） */
static uint16_t key_scan_matrix(void)
{
    uint16_t k = 0;
    /* 行 PB3 拉低 → 读 PC10/PC11/PC12 */
    gpio_write(GPIOB_BASE, (1u << 3), 0); delay_ms(3);
    if (!gpio_read(GPIOC_BASE, 1u << 10)) k |= KEY_1;
    if (!gpio_read(GPIOC_BASE, 1u << 11)) k |= KEY_2;
    if (!gpio_read(GPIOC_BASE, 1u << 12)) k |= KEY_3;
    /* 行 PB4 → PC10/PC11 */
    gpio_write(GPIOB_BASE, (1u << 4), 0); delay_ms(3);
    if (!gpio_read(GPIOC_BASE, 1u << 10)) k |= KEY_4;
    if (!gpio_read(GPIOC_BASE, 1u << 11)) k |= KEY_5;
    /* 行 PB5 → PC10/PC11 */
    gpio_write(GPIOB_BASE, (1u << 5), 0); delay_ms(3);
    if (!gpio_read(GPIOC_BASE, 1u << 10)) k |= KEY_6;
    if (!gpio_read(GPIOC_BASE, 1u << 11)) k |= KEY_7;
    /* 扩展列：PC7、PD4 等（[?] 行线由 GPIOB 其余位提供） */
    if (!gpio_read(GPIOC_BASE, 1u << 7))  k |= KEY_8;
    if (!gpio_read(GPIOD_BASE, 1u << 4))  k |= KEY_9;
    return k;
}

/* 0x08008D00 事件分发（节选：已确认分支） */
static void key_task(void)
{
    ui_state_t *ui = (ui_state_t *)0x20000164u;

    if (++ui->scan_div <= 4) return;                /* 5 分频 */
    ui->scan_div = 0;

    uint16_t k = key_scan_matrix();
    if (k == 0) {
        ui->pressed = 0;
        ui->hold_cnt = 0;
        ui->hold_latch = 0;
        ui->last_key = 0;
        return;
    }
    if (k != ui->last_key) { ui->last_key = k; ui->pressed = 0; return; }

    if (!ui->pressed) {
        ui->pressed = 1;
        if (k == KEY_1) ui->key_flag_f = 1;
        if (k == KEY_5) ui->key_flag_d = 1;
        if (k == KEY_8) ui->key_flag_10 = 1;
        if (k == KEY_10) {                          /* 0x0200: 改 ID 后重设同步字 */
            /* 0x08004B74() [?] */
            ui->pad0[3] = 0;
            rf_set_id(RF_SETTINGS[5]);
        }
        if (k == KEY_1)  /* 0x0800FEC */ ;
        if (k == KEY_2)  /* 0x0800C068 */ ;
        if (k == KEY_3)  /* 0x0800983C */ ;
        if (k == KEY_6)  /* 0x0800B6A0 */ ;
        if (k == KEY_9)  /* 0x08004484 */ ;
        if (k == KEY_11) /* 0x0800A91C：电源 */ ;
        if (k == KEY_COMBO) ui->pad0[3] ^= 1;       /* 组合键切换 */
    }
    /* 长按检测（>50 次扫描） */
    if (ui->hold_cnt < 50) { ui->hold_cnt++; }
    else if (!ui->hold_latch) { ui->hold_latch = 1; ui->key_flag_f = 0; }
}

/* ===================================================================== */
/* 8. 电池 / 充电（0x0800A034 / 0x0800A140）                              */
/* ===================================================================== */

#define BAT_RAW1        (*(volatile uint16_t *)0x2000020Au)  /* 电池 ADC 原始值 */
#define BAT_RAW2        (*(volatile uint16_t *)0x2000020Cu)  /* 第二路 [?] */
#define BAT_STATE       ((volatile uint8_t *)0x2000021Cu)    /* [0]/[1] 档位, [2] 充电态 */

/* 0x0800A034：电池电压分档（>950 满、<20 极低、其它用 float64 曲线 [?]） */
static void battery_update(void)
{
    uint16_t v = BAT_RAW1;
    if (v > 950) {
        if (++BAT_STATE[3] > 10) { BAT_STATE[0x0A] = 2; BAT_STATE[0] = 2; }
    } else if (v < 20) {
        if (++BAT_STATE[4] > 10) { BAT_STATE[0x0A] = 3; BAT_STATE[0] = 2; }
    } else {
        BAT_STATE[3] = 0;
        /* 0x0800339C/0x080033F4 的 float64 比较：按曲线映射到 0..? 档 [?] */
    }
}
/* 0x0800A140：第二路（充电电流/温度 [?]）分档 */
static void charger_update(void)
{
    uint16_t v = BAT_RAW2;
    if (v > 950) {
        if (++BAT_STATE[6] > 10) { BAT_STATE[0x0B] = 2; BAT_STATE[1] = 2; }
    } else if (v < 20) {
        if (++BAT_STATE[7] > 10) { BAT_STATE[0x0B] = 3; BAT_STATE[1] = 2; }
    } else {
        BAT_STATE[6] = 0;
    }
}

/* 0x0800A230：每 20 ms 调度电池/充电检测 + PC3/PC4 控制 */
static void battery_task(void)
{
    gpio_write(GPIOC_BASE, (1u << 3), 1);            /* PC3 = 1 [?] */
    if (*(volatile uint8_t *)0x200001B2u == 0x1A) return;
    if (*(volatile uint8_t *)0x200001BCu == 0x1A) return;

    if (++BAT_STATE[9] > 20) {
        BAT_STATE[9] = 0;
        charger_update();
        battery_update();
    }
    uint16_t aux = *(volatile uint16_t *)0x20000140u;
    if (BAT_STATE[0] || BAT_STATE[1] || *(volatile uint8_t *)0x2000016Au || aux) {
        if (BAT_STATE[0] == 2 || BAT_STATE[1] == 2 || aux <= (0x4B << 3))
            gpio_write(GPIOC_BASE, (1u << 4), 1);    /* PC4 充电/指示 [?] */
        else
            gpio_write(GPIOC_BASE, (1u << 4), 0);
    } else {
        gpio_write(GPIOC_BASE, (1u << 4), 0);
        BAT_STATE[2] = 0;
    }
}

/* ===================================================================== */
/* 9. 电源序列 / 主循环                                                  */
/* ===================================================================== */

/* 0x0800A934: 电源上/下电序列（参数 1=上电；0x0800A91C 用于关机） */
static void power_sequence(int on);
/* 0x0800B9C0: 启动射频 + 任务（0x0800A934 尾部调用） */
static void system_start(void);

/* 0x0800A91C: 关机/低功耗请求 */
static void power_off_request(void)
{
    /* 若 [0x20000184+3]==0 → power_sequence(0)，然后置标志 [?] */
}

/* main() 0x0800CF74 主循环阶段（节选 0x0800CFE0-0x0800D016） */
static void main_loop(void)
{
    /* 初始化阶段（0x0800CF74 前段）：
     *   memcpy(RAM 0x20000000, vector_table, 48*4) —— 向量表搬到 RAM
     *   SYSCFG_CFGR1 |= 3
     *   SystemInit 类函数：0x080075CC, 0x0800B8F0, 0x0800941C(GPIO/EXTI), 0x080093F4,
     *                     0x0800967C, 0x08009310(ADC), 0x080095CC(IWDG), 0x080095F8,
     *                     0x08009708, 0x08009768, 0x080096A8, 0x080093A8
     *   msr PRIMASK=1
     *   0x08007544/0x080074F0(0x20000384) 看门狗首喂
     *   0x0800AD10 / 0x0800AD44（读 flash 设置 0x0801F000 页）
     *   0x0800A934(1) 电源序列；0x0800ABE4 扫描/配对；...
     * 进入 1 ms 超级循环：
     */
    for (;;) {
        while (*(volatile uint8_t *)0x200001E1u != 1) { }   /* SysTick 置位 */
        *(volatile uint8_t *)0x200001E1u = 0;

        wdg_feed((iwdg_desc_t *)0x20000384u);   /* 0x080074F0 */
        key_task();                              /* 0x08008D00：键盘+UI 事件 */
        /* 0x080047CC：PC14/PC15 状态机 */
        /* 0x080098F8, 0x080035A8, 0x0800642C, 0x080038C0, 0x0800D098,
           0x080065CC, 0x0800896C, 0x0800B394 */
        battery_task();                          /* 0x0800A230 */
        /* 0x08003BAC, 0x0800C7D4(RF 保活), 0x0800B7B0, 0x080097F8, 0x0800B80C */
    }
}

/* 占位：真实实现见固件地址（未逐一还原，标 [?] /） */
static void ui_notify(void)      { /* 0x0800B7F0：UI 事件通知 [?] */ }
static void rf_power_prepare(void) { /* 0x0800A6CC [?] */ }
static void rf_power_release(void) { /* 0x0800A6A4 [?] */ }
static void power_sequence(int on) { (void)on; /* 0x0800A934 [?] */ }
static void system_start(void)     { /* 0x0800B9C0：rf_init()、系统任务、LCD 等 [?] */ }
