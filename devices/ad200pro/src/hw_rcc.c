/*
 * hw_rcc.c - 时钟 / SysTick / 低功耗
 *
 * 重建自：
 *   0x0800BC98 clock_init_task
 *   0x0800742C rcc_clock_config（451 条）
 *   0x08007244 rcc_clock_switch（153 条）
 *   0x08007124 rcc_enable_hse_pll（133 条）
 *   0x080073A4 rcc_get_clock_freq
 *   0x08007C84 systick_init / 0x08007C68 systick_disable
 *   0x080070F0 low_power_sleep
 *   0x08007044 flash_acr_init
 */
#include "ad200.h"

/* 0x0800D00C: AHB 预分频 → 移位量表（16 字节） */
static const uint8_t g_ahb_shift[16] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9
};

/* 0x0800BC98：组装两个配置结构，依次 config → switch → systick_init。
 * 结构布局从栈初始化可见：cfg1 @sp, cfg2 @sp+0x34（0x0800bc9c..0x0800bcc4）。
 */
void clock_init_task(void)
{
    struct {
        uint8_t  flags;    /* +0   bit1: HPRE, bit2: PPRE, bit7: 特殊分支 */
        uint8_t  target;   /* +1   */
        uint16_t pad;
        uint32_t hpre;     /* +8  */
        uint32_t ppre;     /* +0xc */
        uint32_t extra[8];
    } cfg1, cfg2;

    /* ---- 第一段：基本时钟（0x0800bc9c..0x0800bcb4） ---- */
    cfg1.flags  = 0x28;             /* 0x0800bc9c movs r0,#0x28 */
    cfg1.extra[0] = 1;              /* 0x0800bca2 */
    cfg1.extra[1] = 1;              /* 0x0800bca6 */
    cfg1.extra[2] = 0;              /* 0x0800bca8 */
    rcc_clock_config(&cfg1);        /* 0x0800bcac 失败 → Error_Handler (b .) */

    /* ---- 第二段：切换 + SysTick（0x0800bcb6..0x0800bcdc） ---- */
    cfg2.flags  = 7;                /* 0x0800bcb6 */
    cfg2.target = 3;                /* 0x0800bcba */
    cfg2.extra[0] = 0;              /* 0x0800bcbe */
    cfg2.extra[1] = 0;              /* 0x0800bcc2 */
    if (rcc_clock_switch(&cfg2, 1)) /* 0x0800bcc6 */
        Error_Handler();

    rcc_delay_short();              /* 0x0800bce6 0x08007398 */
    delay_us(1000);                 /* 0x0800bcea: r1=0x3E8 -> 0x080030F0?? 实为调用 */
    systick_init(g_systick_reload); /* 0x0800bcf2 0x08007C84 */
    systick_set_clksource(4);       /* 0x0800bcf6 0x08007C68 */
    NVIC_SetPriority(-1, 0);        /* 0x0800bcfc..0x0800bd02 */
}

/* 0x08007244: rcc_clock_switch(cfg, latency)
 * 1) 若 FLASH_ACR 当前延时 < latency → 设置（ACR bit0）
 * 2) 若 cfg->flags.bit1 → 写 CFGR.HPRE
 * 3) 使能目标时钟源并写 CFGR.SW，等待 CFGR.SWS == target（超时 5000 次 tick）
 * 4) 若 cfg->flags.bit2 → 写 CFGR.PPRE
 * 5) rcc_get_clock_freq() → SysTick 重装值 = freq >> ahb_shift[HPRE]
 */
int rcc_clock_switch(const void *cfg_p, uint32_t latency)
{
    const uint8_t *cfg = cfg_p;

    /* --- FLASH 延时 --- */
    if ((FLASH_ACR & 1u) < latency) {               /* 0x0800724c */
        FLASH_ACR = (FLASH_ACR & ~1u) | latency;    /* 0x08007256..0x08007260 */
        if ((FLASH_ACR & 1u) != latency)
            return 1;                               /* 0x0800726a 超时/失败 */
    }

    /* --- HPRE --- */
    if (cfg[0] & 0x02)                              /* 0x08007270 lsls #0x1e */
        RCC_CFGR = (RCC_CFGR & ~0xF0u) | *(const uint32_t *)(cfg + 8);

    /* --- 目标时钟源就绪 --- */
    switch (*(const uint32_t *)(cfg + 4)) {         /* 0x08007286 */
    case 1:                                         /* HSE */
        if (!(RCC_CR & (1u << 17))) return 1;       /* 0x080072bc */
        break;
    case 2:                                         /* PLL */
        if (!(RCC_CR & (1u << 25))) return 1;       /* 0x080072c2 */
        break;
    case 3:                                         /* CFGR3/CR2 侧时钟 */
        if (!(RCC_CR2 & (1u << 16))) return 1;      /* 0x080072c8 */
        break;
    }
    /* --- 切换 SW 并等 SWS --- */
    RCC_CFGR = (RCC_CFGR & ~3u) | *(const uint32_t *)(cfg + 4);  /* 0x0800729c */
    {
        uint32_t t0 = get_tick();                   /* 0x080072a6 */
        while (((RCC_CFGR >> 2) & 3u) != *(const uint32_t *)(cfg + 4)) {
            if (get_tick() - t0 > 5000)             /* 0x080072d4 0x1388 */
                return 3;
        }
    }

    /* --- PPRE --- */
    if (cfg[0] & 0x04)                              /* 0x08007352 lsls #0x1d */
        RCC_CFGR = (RCC_CFGR & ~0x700u) | *(const uint32_t *)(cfg + 0xC);

    /* --- SysTick 重装值 = f_sysclk >> AHB 分频移位 --- */
    {
        uint32_t freq = rcc_get_clock_freq();        /* 0x08007366 */
        uint32_t hpre = (RCC_CFGR >> 4) & 0xF;       /* CFGR[7:4] */
        g_systick_reload = freq >> g_ahb_shift[hpre];/* 0x08007372 -> 0x20000104 */
    }
    flash_set_latency(0);                            /* 0x0800737c */
    return 0;
}

/* 0x080073A4：按 CFGR.SW/SWS 与 PLL 位推导 Hz（细节略） */
uint32_t rcc_get_clock_freq(void)
{
    uint32_t sw = (RCC_CFGR >> 2) & 3u;
    switch (sw) {
    case 0: return 8000000u;                         /* HSI 8 MHz */
    case 1: return 8000000u;                         /* HSE（板载值未知）[推测] */
    case 2: {                                        /* PLL */
        uint32_t mul = ((RCC_CFGR >> 18) & 0xF) + 2;
        return 8000000u * mul;
    }
    default: return 48000000u;                       /* HSI48 */
    }
}

/* 0x08007C84: systick_init(reload) */
int systick_init(uint32_t reload)
{
    if (reload - 1 > 0xFFFFFFu)                      /* 0x08007c88 */
        return 1;
    SYSTICK_RVR = reload - 1;                        /* 0x08007c94 */
    NVIC_SetPriority(15, 3);                         /* 0x08007c9a -> 0x0800A310 */
    SYSTICK_CVR = 0;                                 /* 0x08007ca0 */
    SYSTICK_CSR = 7;                                 /* 0x08007ca4: CLKSOURCE|TICKINT|ENABLE */
    return 0;
}

/* 0x08007C68: 设置 SysTick 时钟源（bit2=CLKSOURCE） */
void systick_set_clksource(uint32_t v)
{
    if (v == 4)
        SYSTICK_CSR |= 4;                            /* 0x08007c78 */
    else
        SYSTICK_CSR &= ~4u;                          /* 0x08007c72 */
}

/* 0x080070F0: low_power_sleep(mode, use_wfi)
 * PWR_CR bit[3:2] = mode & 0x0C，SCB_SCR.SLEEPDEEP=1；use_wfi ? wfi : sev+wfe+wfe。
 * 全镜像唯一的 WFI/WFE（0x0800710a–0x08007118）。
 */
void low_power_sleep(uint32_t mode, uint32_t use_wfi)
{
    PWR_CR = (PWR_CR & ~0x0Cu) | (mode & 0x0C);      /* 0x080070f2..0x080070fa */
    SCB_SCR |= 4;                                    /* 0x080070fe: SLEEPDEEP */
    if (use_wfi) {
        __asm volatile ("wfi");                      /* 0x08007118 */
    } else {
        __asm volatile ("sev");
        __asm volatile ("wfe");
        __asm volatile ("wfe");
    }
    SCB_SCR &= ~4u;                                  /* 0x08007110 */
}

/* 0x08007044: FLASH_ACR |= 0x10（prefetch），再配延时/预取 */
void flash_acr_init(void)
{
    FLASH_ACR |= 0x10;                               /* 0x08007048 */
    flash_set_latency(0);                            /* 0x08007052 */
    flash_prefetch_config();                         /* 0x08007056 */
}

/* 0x08007064 / 0x08007088：延时节 / 预取配置（细节 11/13 条，寄存器级） */
void flash_set_latency(uint32_t v) { (void)v; }      /* 0x08007064 */
void flash_prefetch_config(void) { }                 /* 0x08007088 */

/* 0x080070C0/0x080070D4/0x080070E8：NVIC 原语 */
void NVIC_DisableIRQ(uint32_t irq) { NVIC_ICER = 1u << irq; }        /* 0x080070C0 */
void NVIC_EnableIRQ(uint32_t irq)  { NVIC_ISER = 1u << irq; }        /* 0x080070D4 */
void NVIC_SetPriority(uint32_t irq, uint32_t prio)                   /* 0x080070E8 */
{
    uint32_t shift = (irq & 3) * 8;
    if (irq & 0x80) return;
    if (irq < 4) SCB_SHPR2 = (SCB_SHPR2 & ~(0xFFu << shift)) | (prio << shift);
    else         SCB_SHPR3 = (SCB_SHPR3 & ~(0xFFu << shift)) | (prio << shift);
}

void rcc_delay_short(void) { }                       /* 0x08007398 */
