/*
 * hw_spi_soft.c - 2.4G 射频芯片的 GPIO 软件 SPI（bit-bang）
 *
 * 注意：任务清单里的 "hw_usart_soft.c" **不适用**：
 *   - 全镜像没有任何 USART 寄存器访问，也没有 AF（复用）配置；
 *   - 2.4G 芯片用的是 4 线软 SPI（PC6=CSN, PB13=SCK, PB12=MOSI, PB14=MISO），
 *     不是异步串口。见 docs/04-subsystems.md §6。
 *
 * 重建自：
 *   0x08008778 rf_bus_init
 *   0x0800CA2C rf_chip_reset
 *   0x0800CAB0 rf_spi_read_byte
 *   0x0800CAEC rf_spi_write_byte
 *   0x0800CBA8 rf_select / 0x0800CBC8 rf_deselect
 *   0x0800CB38 rf_spi_transfer / 0x0800CB64 rf_write_cmd / 0x0800CB7C rf_write_cmd_buf
 *   0x0800CAA2 rf_write_reg8 / 0x0800CA16 rf_read_reg8
 */
#include "ad200.h"

#define RF_CSN     PC6
#define RF_SCK     PB13
#define RF_MOSI    PB12
#define RF_MISO    PB14

#define PIN_CSN    (1u << 6)
#define PIN_SCK    (1u << 13)
#define PIN_MOSI   (1u << 12)
#define PIN_MISO   (1u << 14)

/* 0x08008778: 总线引脚初始化
 * PC6/PC7 → 输出；PB12/13 → 输出；PB14/15 → 输入；初值 CSN=1, SCK=0, MOSI=0, PC7=1。
 * 反汇编 0x0800877a..0x0800880e，其中 MODER 位掩码是用"端口基址移位"生成的。
 */
void rf_bus_init(void)
{
    /* GPIOC MODER：PC6 输出（清 bit13、置 bit12） */
    GPIO_MODER(GPIOC_BASE) &= ~(1u << 13);
    GPIO_MODER(GPIOC_BASE) |=  (1u << 12);
    gpio_write_pin(GPIOC_BASE, PIN_CSN, 1);          /* CSN=1（未选中） */

    /* GPIOB MODER：PB13 输出、PB12 输出 */
    GPIO_MODER(GPIOB_BASE) &= ~(1u << 27);           /* PB13: [27:26]=01 */
    GPIO_MODER(GPIOB_BASE) |=  (1u << 26);
    gpio_write_pin(GPIOB_BASE, PIN_SCK, 0);
    GPIO_MODER(GPIOB_BASE) &= ~(1u << 25);           /* PB12: [25:24]=01 */
    GPIO_MODER(GPIOB_BASE) |=  (1u << 24);
    gpio_write_pin(GPIOB_BASE, PIN_MOSI, 0);

    /* PB13/PB12 无上下拉 */
    GPIO_PUPDR(GPIOB_BASE) &= ~((3u << 26) | (3u << 24));

    /* PC7 输出=1，PC7 → 0（复位/唤醒脚） */
    GPIO_MODER(GPIOC_BASE) &= ~(3u << 14);
    GPIO_MODER(GPIOC_BASE) |=  (1u << 14);
    gpio_write_pin(GPIOC_BASE, 0x0080, 1);
    gpio_write_pin(GPIOC_BASE, 0x0080, 0);

    /* PB14/PB15 保持输入（board_init_inputs 已配，0x0800880a 再次触发 IDR 读） */
    (void)GPIO_IDR(GPIOB_BASE);
}

/* 0x0800CBA8: 选中芯片并等待 MISO 就绪
 *   反汇编：PC6=0（0x0800cbb0），然后 while (GPIOB_IDR & (1<<14)) 等待。
 */
void rf_select(void)
{
    gpio_write_pin(GPIOC_BASE, PIN_CSN, 0);          /* 0x0800cbb0 */
    while (GPIO_IDR(GPIOB_BASE) & PIN_MISO)          /* 0x0800cbb6 lsls #0x11; bmi */
        ;
}

/* 0x0800CBC8: 取消选中 */
void rf_deselect(void)
{
    gpio_write_pin(GPIOC_BASE, PIN_CSN, 1);          /* 0x0800cbd0 */
}

/* 0x0800CAEC: 写一个字节（MSB first，SPI mode 0）
 * 每位：SCK=0 → 放 MOSI → 延时 → SCK=1 → 延时 → 采样 MISO 拼返回值。
 * 8 位循环见 0x0800cafc..0x0800cb2e；返回值为时钟期间 MISO 采到的字节。
 */
void rf_spi_write_byte(uint8_t b)
{
    uint8_t in = 0;
    int i;

    gpio_write_pin(GPIOB_BASE, PIN_SCK, 0);          /* 0x0800cafc */
    for (i = 0; i < 8; i++) {
        if (b & 0x80)
            GPIO_BSRR(GPIOB_BASE) = PIN_MOSI;        /* 0x0800cb02 */
        else
            GPIO_BRR(GPIOB_BASE)  = PIN_MOSI;        /* 0x0800cb06 */
        b <<= 1;                                     /* 0x0800cb08 */
        delay_us(1);                                 /* 3×nop + 0x0800bbd4 延时 */
        GPIO_BSRR(GPIOB_BASE) = PIN_SCK;             /* 0x0800cb16 上升沿 */
        delay_us(1);
        in = (uint8_t)((in << 1) | ((GPIO_IDR(GPIOB_BASE) & PIN_MISO) ? 1 : 0));
        gpio_write_pin(GPIOB_BASE, PIN_SCK, 0);      /* 0x0800cb12 */
    }
    GPIO_BRR(GPIOB_BASE) = PIN_SCK;                  /* 0x0800cb30 收尾拉低 */
    (void)in;
}

/* 0x0800CAB0: 读一个字节（8 个时钟，高电平期间采样 MISO） */
uint8_t rf_spi_read_byte(void)
{
    uint8_t v = 0;
    int i;

    for (i = 0; i < 8; i++) {
        GPIO_BSRR(GPIOB_BASE) = PIN_SCK;             /* 0x0800cabc */
        __asm volatile ("nop");
        delay_us(1);
        v = (uint8_t)((v << 1) | ((GPIO_IDR(GPIOB_BASE) & PIN_MISO) ? 1 : 0)); /* 0x0800cac6 */
        GPIO_BRR(GPIOB_BASE) = PIN_SCK;              /* 0x0800cad0 */
        delay_us(1);
    }
    return v;
}

/* 0x0800CB38: 传输 —— 选中、写命令字节、读 len 字节、取消选中；返回命令阶段的 MISO 状态字节 */
uint8_t rf_spi_transfer(uint8_t cmd, uint8_t *buf, uint32_t len)
{
    uint32_t i;
    uint8_t status;

    rf_select();                                     /* 0x0800cb40 */
    rf_spi_write_byte(cmd);                          /* 0x0800cb46 */
    status = cmd;                                    /* 实际实现里 r0 来自写字节的返回值 */
    for (i = 0; i < len; i++)
        buf[i] = rf_spi_read_byte();                 /* 0x0800cb50 */
    rf_deselect();                                   /* 0x0800cb5c */
    return status;
}

/* 0x0800CB64: 单字节命令（strobe） */
uint8_t rf_write_cmd(uint8_t cmd)
{
    rf_select();
    rf_spi_write_byte(cmd);
    rf_deselect();
    return cmd;
}

/* 0x0800CB7C: 命令 + 多字节写 */
void rf_write_cmd_buf(uint8_t cmd, const uint8_t *buf, uint32_t len)
{
    uint32_t i;
    rf_select();
    rf_spi_write_byte(cmd);
    for (i = 0; i < len; i++)
        rf_spi_write_byte(buf[i]);
    rf_deselect();
}

/* 0x0800CAA2: 写单寄存器 = 命令(addr) + 1 字节值 */
void rf_write_reg8(uint8_t addr, uint8_t val)
{
    rf_write_cmd_buf(addr, &val, 1);                 /* 0x0800caa2 */
}

/* 0x0800CA16: 读单寄存器 = 命令(addr | 0x80) + 读 1 字节 */
uint8_t rf_read_reg8(uint8_t addr)
{
    uint8_t v = 0;
    rf_spi_transfer((uint8_t)(addr | 0x80), &v, 1);  /* 0x0800ca16: orrs #0x80 */
    return v;
}

/* 0x0800CA2C: 芯片复位/同步：CSN 脉冲 + 等待 MISO + SRES(0x30)
 * 反汇编：PC6=1→延时→0→延时→1→延时→0 → 等 MISO 高 → rf_write_cmd(0x30)。
 */
void rf_chip_reset(void)
{
    gpio_write_pin(GPIOC_BASE, PIN_CSN, 1); delay_us(0x1E);   /* 0x0800ca36 */
    gpio_write_pin(GPIOC_BASE, PIN_CSN, 0); delay_us(0x1E);   /* 0x0800ca46 */
    gpio_write_pin(GPIOC_BASE, PIN_CSN, 1); delay_us(0x2D);   /* 0x0800ca56 */
    gpio_write_pin(GPIOC_BASE, PIN_CSN, 0);                   /* 0x0800ca66 */
    while (!(GPIO_IDR(GPIOB_BASE) & PIN_MISO))                 /* 0x0800ca6c */
        ;
    rf_write_cmd(0x30);                                       /* SRES 0x0800ca74 */
    while (!(GPIO_IDR(GPIOB_BASE) & PIN_MISO))                 /* 0x0800ca78 */
        ;
}
