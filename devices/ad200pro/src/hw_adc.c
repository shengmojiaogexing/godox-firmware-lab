/*
 * hw_adc.c - ADC + DMA1 CH1 采样
 *
 * 重建自：
 *   0x0800356C adc_hw_init（16 条）
 *   0x08009698 adc_dma_init（62 条，配置通道 3/10/11/12）
 *   0x0800641C adc_dma_config（140 条）
 *   0x08006550 adc_dma_enable（181 条）
 *   0x080068A4 dma1_ch1_task（320 条）
 *   0x08003598 adc_task（147 条，4 采样中值滤波）
 *   0x080044D0 DMA1_Ch1_IRQHandler
 */
#include "ad200.h"

adc_dev_t g_adc_dev;            /* 0x2000025C */
static uint32_t g_dma_handle;   /* 0x200002A8（DMA 设备结构） */

/* ------------------------------------------------------------------ ADC 寄存器初始化
 * 0x0800356C：只写控制寄存器（通道在 adc_dma_init 里配）。
 * 反汇编 0x0800356e..0x08003596：
 *   CR     = ... (ADEN 等)
 *   CFGR1  = DMAEN | CONT | 对齐
 *   CFGR2  = 采样时钟分频
 *   SMPR   = 采样时间
 *   TR     = 0
 *   ADC_CCR(0x40012708) = ...
 */
void adc_hw_init(void)
{
    ADC_CR    = 0x00000000u;            /* 0x0800356e 先关 */
    ADC_CR    = 0x00000001u;            /* ADEN */
    ADC_CFGR1 = 0x00000000u;            /* 0x08003572 */
    ADC_CFGR2 = 0x00000008u;            /* 0x08003576 */
    ADC_SMPR  = 0x00000000u;            /* 0x0800357a */
    ADC_TR    = 0x0FFF0000u;            /* 0x08003582 */
    ADC_CCR   = 0x00000000u;            /* 0x08003586 0x40012708 */
    ADC_ISR   = 0x00000000u;            /* 清标志 */
}

/* ------------------------------------------------------------------ ADC+DMA 设备初始化
 * 0x08009698：结构 0x2000025C = {base=0x40012400, cfg, ctrl, 结果缓存…}；
 * 依次 adc_dma_config 配置 4 个通道（3, 10, 11, 12），每个失败都跳 Error_Handler。
 * 反汇编：0x0800969a..0x08009726。
 */
void adc_dma_init(void)
{
    uint32_t cfg[3];

    g_adc_dev.base = ADC_BASE;                       /* 0x0800969e */
    g_adc_dev.cfg  = 0x00000008u;                    /* 0x080096a2 */
    g_adc_dev.ctrl = 0x00000008u;                    /* 0x080096a6 */
    /* 0x080096aa..0x080096be：结果缓存/触发配置 */

    dma1_clock_init();                               /* 0x080096bc? 由 DMA 结构初始化 */

    cfg[0] = 3;  cfg[1] = 0x00001000u; cfg[2] = 4;   /* 0x080096ce: 通道 3 */
    if (adc_dma_config(&g_adc_dev, cfg)) Error_Handler();

    cfg[0] = 10;                                     /* 0x080096ea: 通道 10 */
    if (adc_dma_config(&g_adc_dev, cfg)) Error_Handler();

    cfg[0] = 11;                                     /* 0x080096fe: 通道 11 */
    if (adc_dma_config(&g_adc_dev, cfg)) Error_Handler();

    cfg[0] = 12;                                     /* 0x08009712: 通道 12 */
    if (adc_dma_config(&g_adc_dev, cfg)) Error_Handler();
}

/* 0x0800641C: adc_dma_config(dev, {ch, ..., n}) —— 配 ADC_CHSELR + DMA 通道
 * 具体寄存器序列（140 条）以 ADC_CFGR1/CHSELR + DMA_CCR/CNDTR/CPAR/CMAR 为主 [推测]。
 */
int adc_dma_config(adc_dev_t *d, const uint32_t *cfg)
{
    uint32_t ch = cfg[0];

    ADC_CHSELR |= 1u << ch;                          /* 通道选择 */
    DMA_CCR(1)  = 0;                                 /* 先停 */
    DMA_CPAR(1) = ADC_BASE + 0x50;                   /* 源 = ADC_DR */
    DMA_CMAR(1) = (uint32_t)&d->res[ch];             /* 目的 = 结果缓存 */
    DMA_CNDTR(1) = cfg[2];                           /* 样本数 */
    DMA_CCR(1)  = 0x00000101u;                       /* MSIZE/PSIZE/循环/使能 */
    return 0;
}

/* 0x08006550: adc_dma_enable —— 启动转换（CEN/ADSTART 等） */
void adc_dma_enable(void)
{
    ADC_CR |= 0x00000004u;                           /* ADSTART */
}

/* 0x080066C4: DMA 时钟（RCC_APB2ENR 相关位） */
void dma_clock_enable(void)
{
    RCC_APB2ENR |= 1u << 0;                          /* SYSCFGEN 占位，实际位见反汇编 */
}

/* 0x08009730: dma1_clock_init —— RCC_AHBENR |= DMA1EN，NVIC IRQ9 */
void dma1_clock_init(void)
{
    RCC_AHBENR |= RCC_AHBENR_DMA1EN;                 /* 0x08009734 */
    (void)RCC_AHBENR;
    NVIC_SetPriority(9, 0);                          /* 0x08009748 */
    NVIC_EnableIRQ(9);                                /* 0x0800974e */
}

/* 0x080068A4: DMA1 CH1 任务 —— 处理 ISR/IFCR，回填 ADC 结果/状态（320 条） */
void dma1_ch1_task(void *arg)
{
    uint32_t isr = DMA_ISR;                          /* 0x080068a4 */
    (void)arg;
    if (isr & 0x1)  { /* GIF1 */ }
    if (isr & 0x2)  { /* TCIF1 */ }
    if (isr & 0x4)  { /* HTIF1 */ }
    if (isr & 0x8)  { /* TEIF1  */ }
    DMA_IFCR = 0x0F;                                 /* 清 CH1 全标志 */
}

void DMA1_Ch1_IRQHandler(void)
{
    dma1_ch1_task(&g_dma_handle);                    /* 0x080044d4 */
}

/* ------------------------------------------------------------------ adc_task
 * 0x08003598：每个通道取 4 个样本 → 冒泡排序 → 取中间两个平均 → +偏移
 * 结果写回 RAM（0x2000025C 结构的 +0x18/+0x0E/+0x1A/+0x1C 等字段）。
 * 反汇编：4 段完全同构的代码，分别对应 4 个通道（0x080035b4..0x080036be）。
 */
void adc_task(void)
{
    int ch;

    /* 0x0800359c: 先把 16 字节 DMA 缓冲搬到本地（adc_read_block） */
    adc_read_block();

    delay_us(0x12D);                                 /* 0x080035aa 300us 稳定等待 */

    for (ch = 0; ch < 4; ch++) {
        uint32_t *res = &g_adc_dev.res[ch * 4];
        uint32_t s[4];
        int i, j;
        uint16_t med;

        /* 0x080035cc..0x080035e2：4 元素冒泡排序（降序） */
        for (i = 0; i < 4; i++) s[i] = res[i];
        for (i = 0; i < 4; i++)
            for (j = 0; j < 3; j++)
                if (s[j] < s[j + 1]) { uint32_t t = s[j]; s[j] = s[j + 1]; s[j + 1] = t; }

        /* 0x080035ec..0x080035f8：中间两个平均 + 偏移 */
        med = (uint16_t)((s[1] + s[2]) >> 1);
        med += (ch == 0) ? 0x0E : 0x0A;              /* 不同通道偏移不同 */
        store_adc_result(ch, med);
    }
    adc_dma_enable();                                /* 触发下一轮 */
}
