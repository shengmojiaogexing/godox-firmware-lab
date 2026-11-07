/*
 * ad200.h - GODOX AD200 Pro 固件逆向重建的公共头（伪 C，非原始工程）
 *
 * 目标 MCU：Cortex-M0（STM32F0 风格外设，FLASH 控制器为 F1 布局 [推测]）
 * 镜像基址：0x08003000   镜像大小：0xB52C
 * RAM：[0x20000100, 0x200008B0)，栈顶 SP = 0x200008B0
 *
 * 本目录所有 .c 都是"可读的伪 C 重建"，逐函数标注固件地址/调用关系；
 * 编译不保证通过，目的是把行为写清楚给模拟器/调试器团队使用。
 */
#ifndef AD200_H
#define AD200_H

#include <stdint.h>

/* ------------------------------------------------------------------ 基础 */
#define REG32(addr)        (*(volatile uint32_t *)(addr))
#define REG16(addr)        (*(volatile uint16_t *)(addr))
#define REG8(addr)         (*(volatile uint8_t  *)(addr))

#define IMG_BASE           0x08003000u
#define RAM_BASE           0x20000000u
#define INITIAL_SP         0x200008B0u

/* .data / .bss（由 0x0800E3E4 scatter 表决定） */
#define DATA_START         0x20000100u
#define BSS_START          0x20000228u

/* ------------------------------------------------------------------ RCC */
#define RCC_BASE           0x40021000u
#define RCC_CR             REG32(RCC_BASE + 0x00)
#define RCC_CFGR           REG32(RCC_BASE + 0x04)
#define RCC_CIR            REG32(RCC_BASE + 0x08)
#define RCC_APB2RSTR       REG32(RCC_BASE + 0x0C)
#define RCC_APB1RSTR       REG32(RCC_BASE + 0x10)
#define RCC_AHBENR         REG32(RCC_BASE + 0x14)
#define RCC_APB2ENR        REG32(RCC_BASE + 0x18)
#define RCC_APB1ENR        REG32(RCC_BASE + 0x1C)
#define RCC_BDCR           REG32(RCC_BASE + 0x20)
#define RCC_CSR            REG32(RCC_BASE + 0x24)
#define RCC_CFGR2          REG32(RCC_BASE + 0x2C)
#define RCC_CFGR3          REG32(RCC_BASE + 0x30)
#define RCC_CR2            REG32(RCC_BASE + 0x34)

#define RCC_CR_HSION       (1u << 0)
#define RCC_AHBENR_DMA1EN  (1u << 0)
#define RCC_AHBENR_GPIOAEN (1u << 17)
#define RCC_AHBENR_GPIOBEN (1u << 18)
#define RCC_AHBENR_GPIOCEN (1u << 19)
#define RCC_AHBENR_GPIODEN (1u << 20)
#define RCC_AHBENR_GPIOFEN (1u << 22)
#define RCC_APB1ENR_PWREN  (1u << 28)
#define RCC_APB2ENR_SYSCFGEN (1u << 0)
#define RCC_APB2ENR_ADCEN  (1u << 9)

/* ------------------------------------------------------------------ GPIO */
#define GPIOA_BASE         0x48000000u
#define GPIOB_BASE         0x48000400u
#define GPIOC_BASE         0x48000800u
#define GPIOD_BASE         0x48000C00u
#define GPIOF_BASE         0x48001400u

#define GPIO_MODER(b)      REG32((b) + 0x00)
#define GPIO_OTYPER(b)     REG32((b) + 0x04)
#define GPIO_OSPEEDR(b)    REG32((b) + 0x08)
#define GPIO_PUPDR(b)      REG32((b) + 0x0C)
#define GPIO_IDR(b)        REG32((b) + 0x10)
#define GPIO_ODR(b)        REG32((b) + 0x14)
#define GPIO_BSRR(b)       REG32((b) + 0x18)
#define GPIO_BRR(b)        REG32((b) + 0x28)

/* ------------------------------------------------------------------ EXTI / SYSCFG / NVIC / SCB */
#define EXTI_BASE          0x40010400u
#define EXTI_IMR           REG32(EXTI_BASE + 0x00)
#define EXTI_EMR           REG32(EXTI_BASE + 0x04)
#define EXTI_RTSR          REG32(EXTI_BASE + 0x08)
#define EXTI_FTSR          REG32(EXTI_BASE + 0x0C)
#define EXTI_SWIER         REG32(EXTI_BASE + 0x10)
#define EXTI_PR            REG32(EXTI_BASE + 0x14)

#define SYSCFG_BASE        0x40010000u
#define SYSCFG_CFGR1       REG32(SYSCFG_BASE + 0x00)
#define SYSCFG_EXTICR1     REG32(SYSCFG_BASE + 0x08)
#define SYSCFG_EXTICR2     REG32(SYSCFG_BASE + 0x0C)
#define SYSCFG_EXTICR3     REG32(SYSCFG_BASE + 0x10)
#define SYSCFG_EXTICR4     REG32(SYSCFG_BASE + 0x14)

#define SCS_BASE           0xE000E000u
#define NVIC_ISER          REG32(SCS_BASE + 0x100)
#define NVIC_ICER          REG32(SCS_BASE + 0x180)
#define NVIC_ISPR          REG32(SCS_BASE + 0x200)
#define NVIC_ICPR          REG32(SCS_BASE + 0x280)
#define NVIC_IPR(n)        REG8(SCS_BASE + 0x400 + (n))
#define SYSTICK_CSR        REG32(SCS_BASE + 0x10)
#define SYSTICK_RVR        REG32(SCS_BASE + 0x14)
#define SYSTICK_CVR        REG32(SCS_BASE + 0x18)
#define SCB_SCR            REG32(SCS_BASE + 0x10)
#define SCB_CCR            REG32(SCS_BASE + 0x14)
#define SCB_SHPR2          REG32(SCS_BASE + 0x18)
#define SCB_SHPR3          REG32(SCS_BASE + 0x1C)

/* ------------------------------------------------------------------ FLASH（F1 风格布局，证据 0x08006CFC/0x08006C6C） */
#define FLASH_BASE         0x40022000u
#define FLASH_ACR          REG32(FLASH_BASE + 0x00)
#define FLASH_KEYR         REG32(FLASH_BASE + 0x04)
#define FLASH_SR           REG32(FLASH_BASE + 0x0C)
#define FLASH_CR           REG32(FLASH_BASE + 0x10)
#define FLASH_AR           REG32(FLASH_BASE + 0x14)

#define FLASH_CR_PG        (1u << 0)
#define FLASH_CR_PER       (1u << 1)
#define FLASH_CR_MER       (1u << 2)
#define FLASH_CR_STRT      (1u << 6)
#define FLASH_CR_LOCK      (1u << 7)
#define FLASH_SR_BSY       (1u << 0)
#define FLASH_SR_PGERR     (1u << 2)
#define FLASH_SR_WRPRTERR  (1u << 4)
#define FLASH_SR_EOP       (1u << 5)

#define FLASH_PARAM_PAGE   0x0801F000u   /* 参数页（0x800 B/页） */
#define FLASH_FLAG_PAGE    0x0801F800u   /* 复位原因魔数页 */

/* ------------------------------------------------------------------ IWDG / PWR / RTC / ADC / DMA / TIM */
#define IWDG_BASE          0x40003000u
#define IWDG_KR            REG32(IWDG_BASE + 0x00)
#define IWDG_PR            REG32(IWDG_BASE + 0x04)
#define IWDG_RLR           REG32(IWDG_BASE + 0x08)
#define IWDG_SR            REG32(IWDG_BASE + 0x0C)
#define IWDG_WINR          REG32(IWDG_BASE + 0x10)

#define PWR_BASE           0x40007000u
#define PWR_CR             REG32(PWR_BASE + 0x00)
#define PWR_CSR            REG32(PWR_BASE + 0x04)

#define RTC_BASE           0x40002800u
#define RTC_TR             REG32(RTC_BASE + 0x00)
#define RTC_DR             REG32(RTC_BASE + 0x04)
#define RTC_CR             REG32(RTC_BASE + 0x08)
#define RTC_ISR            REG32(RTC_BASE + 0x0C)

#define ADC_BASE           0x40012400u
#define ADC_ISR            REG32(ADC_BASE + 0x00)
#define ADC_IER            REG32(ADC_BASE + 0x04)
#define ADC_CR             REG32(ADC_BASE + 0x08)
#define ADC_CFGR1          REG32(ADC_BASE + 0x0C)
#define ADC_CFGR2          REG32(ADC_BASE + 0x10)
#define ADC_SMPR           REG32(ADC_BASE + 0x14)
#define ADC_TR             REG32(ADC_BASE + 0x28)
#define ADC_CHSELR         REG32(ADC_BASE + 0x30)
#define ADC_DR             REG32(ADC_BASE + 0x50)
#define ADC_CCR            REG32(0x40012700u + 0x08)

#define DMA1_BASE          0x40020000u
#define DMA_ISR            REG32(DMA1_BASE + 0x00)
#define DMA_IFCR           REG32(DMA1_BASE + 0x04)
#define DMA_CCR(ch)        REG32(DMA1_BASE + 0x08 + 0x14 * ((ch) - 1) + 0x00)
#define DMA_CNDTR(ch)      REG32(DMA1_BASE + 0x08 + 0x14 * ((ch) - 1) + 0x04)
#define DMA_CPAR(ch)       REG32(DMA1_BASE + 0x08 + 0x14 * ((ch) - 1) + 0x08)
#define DMA_CMAR(ch)       REG32(DMA1_BASE + 0x08 + 0x14 * ((ch) - 1) + 0x0C)

#define TIM14_BASE         0x40002000u   /* 主循环心跳 */
#define TIM2_BASE          0x40000000u   /* RF 接收超时 */
#define TIM1_BASE          0x40012C00u
#define TIM3_BASE          0x40000400u
#define TIM17_BASE         0x40014800u
#define TIM_CR1(b)         REG32((b) + 0x00)
#define TIM_DIER(b)        REG32((b) + 0x0C)
#define TIM_SR(b)          REG32((b) + 0x10)
#define TIM_EGR(b)         REG32((b) + 0x14)
#define TIM_CNT(b)         REG32((b) + 0x24)
#define TIM_PSC(b)         REG32((b) + 0x28)
#define TIM_ARR(b)         REG32((b) + 0x2C)
#define TIM_CCR1(b)        REG32((b) + 0x34)

/* ------------------------------------------------------------------ 全局变量（RAM，来自 0x20000100 起） */
extern uint32_t g_tick_ms;          /* 0x20000100  SysTick 1 ms 计数 */
extern uint32_t g_systick_reload;   /* 0x20000104  SysTick 重装值 */
extern uint8_t  g_irq_latch;        /* 0x20000108 */
extern uint8_t  g_tim2_flag;        /* 0x20000109 */
extern uint8_t  g_key_debounce[4];  /* 0x20000122 */
extern uint8_t  g_ui_state[];       /* 0x20000150  UI/主状态结构（约 0x40 B） */
extern uint8_t  g_mode;             /* 0x2000015D  bit[1:0]: 1=正常闪光 */
extern uint8_t  g_charge_req;       /* 0x20000162 */
extern uint8_t  g_rf_enable;        /* 0x20000165 */
extern uint8_t  g_rf_channel;       /* 0x2000016C */
extern uint8_t  g_rf_rx_active;     /* 0x2000019B */
extern uint8_t  g_rf_rx_done;       /* 0x200001A1 */
extern uint16_t g_adc_raw;          /* 0x200001A4 */
extern uint16_t g_flash_pulse_us;   /* 0x200001B2 */
extern uint16_t g_retrigger_ms;     /* 0x200001B8 */
extern uint8_t  g_boot_reason;      /* 0x200001BF */
extern uint16_t g_idle_ticks;       /* 0x200001C0 */
extern uint8_t  g_shutdown_req;     /* 0x200001C2  关机/重初始化请求 + ISR 让位标志 */
extern uint8_t  g_autooff_cnt;      /* 0x200001C3 */
extern uint8_t  g_autooff_lim;      /* 0x200001C4 */
extern uint8_t  g_tim14_div;        /* 0x200001CE  25 分频计数 */
extern uint8_t  g_main_wake;        /* 0x200001CF  主循环唤醒 */
extern uint8_t  g_rf_status[8];     /* 0x20000200 */
extern uint8_t  g_rtc_flag;         /* 0x2000020D */
extern uint8_t  g_settings[8];      /* 0x20000418  用户设置镜像 */

/* ADC 设备结构（0x2000025C）：+0 外设基址，+0x0C..+0x3C 通道结果 */
typedef struct {
    uint32_t base;          /* +0x00 = 0x40012400 */
    uint32_t cfg;           /* +0x04 */
    uint32_t ctrl;          /* +0x08 */
    uint32_t res[16];       /* +0x0C.. 通道结果（每通道 4 样本缓存） */
} adc_dev_t;
extern adc_dev_t g_adc_dev; /* 0x2000025C */

/* TIM 设备结构（0x200002E4 / 0x20000318 / 0x20000354 / 0x200003CC） */
typedef struct {
    uint32_t base;      /* +0x00 外设基址 */
    uint32_t psc;       /* +0x04 */
    uint32_t ctrl;      /* +0x08 */
    uint32_t arr;       /* +0x0C */
    uint32_t ccr;       /* +0x10 */
} tim_dev_t;

/* ------------------------------------------------------------------ 核心函数原型（地址见注释） */
void     startup_c(void);                       /* 0x080030C0 */
void     reset_handler(void);                   /* 0x080030D4 */
void     SystemInit(void);                      /* 0x0800BD0C */
void     scatter_load(void);                    /* 0x08003548 */
int      main(void);                            /* 0x0800CDB0 */

/* 运行时 */
uint32_t udivmod32(uint32_t num, uint32_t den); /* 0x080030F0 -> quot, rem in r1 */
uint16_t crc16_block(const uint8_t *p, uint32_t len, uint32_t off); /* 0x08003144 */
void     delay_us(uint32_t us);                 /* 0x0800C768 */
void     delay_ms(uint32_t ms);                 /* 0x0800C828 */
uint32_t get_tick(void);                        /* 0x08006EAC */
void     Error_Handler(void);                   /* 0x08005608  b . */

/* 时钟 */
void rcc_clock_config(const void *cfg);         /* 0x0800742C */
int  rcc_clock_switch(const void *cfg, uint32_t latency); /* 0x08007244 */
void clock_init_task(void);                     /* 0x0800BC98 */
int  systick_init(uint32_t reload);             /* 0x08007C84 */
void systick_disable(void);                     /* 0x08007C68 */
void low_power_sleep(uint32_t mode, uint32_t use_wfi); /* 0x080070F0 */

/* GPIO / EXTI */
void gpio_init_all(void);                       /* 0x08005544 */
void gpio_write_pin(uint32_t port, uint32_t mask, uint32_t level); /* 0x08006E9E */
uint32_t gpio_read_pin(uint32_t port, uint32_t mask);              /* 0x08006E94 */
void exti_config(uint32_t port, const uint32_t cfg[4]);            /* 0x08006D3C */
void exti_clear_pending(uint32_t mask);                            /* 0x08006D24 */
void board_init_inputs(void);                   /* 0x08009758 */

/* ADC / DMA */
void adc_hw_init(void);                         /* 0x0800356C */
void adc_dma_init(void);                        /* 0x08009698 */
int  adc_dma_config(adc_dev_t *d, const uint32_t *cfg); /* 0x0800641C */
void adc_task(void);                            /* 0x08003598 */
void dma1_clock_init(void);                     /* 0x08009730 */
void dma1_ch1_task(void *arg);                  /* 0x080068A4 */

/* 定时器 */
int  tim_timebase_init(tim_dev_t *t);           /* 0x08007D08 */
void tim_clock_enable(tim_dev_t *t);            /* 0x08007D3C */
void tim_irq_handler(tim_dev_t *t);             /* 0x08007F08 */
void tim14_init(void);                          /* 0x080099C0 */
void tim1_init(void);                           /* 0x080099EC */
void tim3_init(void);                           /* 0x08009A4C */
void tim17_init(void);                          /* 0x08009AAC */

/* FLASH 存储 */
int  flash_unlock(void);                        /* 0x08006CFC */
void flash_lock(void);                          /* 0x08006C6C */
void flash_erase_page(uint32_t addr);           /* 0x080058C8 */
void flash_mass_erase(void);                    /* 0x080058A4 */
void flash_program_halfword(uint32_t addr, uint16_t v); /* 0x080058EC */
uint32_t flash_get_status(void);                /* 0x0800593C */
uint32_t flash_wait_last_operation(uint32_t timeout); /* 0x080059B4 */
int  flash_erase_range(const uint32_t desc[3], uint32_t *err_addr); /* 0x08006BCC */
int  flash_program_record(uint32_t mode, uint32_t dst, uint32_t src, uint32_t len); /* 0x08006C80 */
void flash_erase_param_page(void);              /* 0x0800587C */
void flash_write_params(uint32_t addr);         /* 0x08005974 */
void flash_read_params(uint32_t addr);          /* 0x0800590C */
uint32_t flash_read_word(uint32_t addr);        /* 0x08005930 */
void flash_store_params(void);                  /* 0x0800B820 */

/* IWDG */
int  iwdg_init(uint32_t base, uint32_t pr, uint32_t rlr, uint32_t winr); /* 0x08006EB8 */
void iwdg_refresh(void);                        /* 0x08006F68 */
void iwdg_start(void);                          /* 0x08006FBC */

/* 2.4G（CC2500 兼容，软 SPI） */
void rf_bus_init(void);                         /* 0x08008778 */
void rf_spi_write_byte(uint8_t b);              /* 0x0800CAEC */
uint8_t rf_spi_read_byte(void);                 /* 0x0800CAB0 */
void rf_select(void);                           /* 0x0800CBA8 */
void rf_deselect(void);                         /* 0x0800CBC8 */
uint8_t rf_spi_transfer(uint8_t cmd, uint8_t *buf, uint32_t len); /* 0x0800CB38 */
uint8_t rf_write_cmd(uint8_t cmd);              /* 0x0800CB64 */
void rf_write_cmd_buf(uint8_t cmd, const uint8_t *buf, uint32_t len); /* 0x0800CB7C */
void rf_write_reg8(uint8_t addr, uint8_t val);  /* 0x0800CAA2 */
uint8_t rf_read_reg8(uint8_t addr);             /* 0x0800CA16 */
void rf_chip_reset(void);                       /* 0x0800CA2C */
void rf_chip_config(void);                      /* 0x0800C858 */
void rf_init(void);                             /* 0x08004860 */
void rf_apply_settings(void);                   /* 0x0800AA8C */
void rf_set_power_level(uint8_t level);         /* 0x0800B9AC */
void rf_poll_settings(void);                    /* 0x0800B5B0 */

/* 应用 */
void adc_task(void);
void key_scan_task(void);                       /* 0x08008818 */
void app_state_init(void);                      /* 0x0800BD68 */
void settings_load_task(void);                  /* 0x0800AC58 */
void boot_reason_check(void);                   /* 0x0800AC24 */
void charge_enable_task(void);                  /* 0x0800BA00 */
void system_reinit_task(void);                  /* 0x0800BA5C */
void flash_power_task(void);                    /* 0x08004500 */
void flash_charge_state_machine(void);          /* 0x080045AC */
void flash_trigger_state_machine(void);         /* 0x08008504 */
void measurement_task(void);                    /* 0x08005F6C */
void display_task(void);                        /* 0x08009228 */
void rf_tx_task(void);                          /* 0x0800AAF4 */
void flash_fire_sequence(void);                 /* 0x08005AE8 */
void flash_trigger_pulse(void);                 /* 0x08009E88 */
void idle_timer_reset(void);                    /* 0x0800BA40 */
void autooff_task(void);                        /* 0x0800BB7C */

/* 中断处理（向量表 0x08003000） */
void SysTick_Handler(void);                     /* 0x0800BC8C */
void EXTI0_1_IRQHandler(void);                  /* 0x080053F0 */
void EXTI2_3_IRQHandler(void);                  /* 0x08005478 */
void EXTI4_15_IRQHandler(void);                 /* 0x08005484 */
void DMA1_Ch1_IRQHandler(void);                 /* 0x080044D0 */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void);      /* 0x0800BE1C */
void TIM2_IRQHandler(void);                     /* 0x0800BE58 */
void TIM14_IRQHandler(void);                    /* 0x0800BDE0 */
void RTC_IRQHandler(void);                      /* 0x0800AC0C */

#endif /* AD200_H */
