/*
 * hw_flash.c - 内部 Flash 驱动 + 参数存储 + IWDG
 *
 * 重建自：
 *   0x08006CFC flash_unlock        0x08006C6C flash_lock
 *   0x080058C8 flash_erase_page    0x080058A4 flash_mass_erase
 *   0x080058EC flash_program_halfword
 *   0x0800593C flash_get_status    0x080059B4 flash_wait_last_operation
 *   0x08006BCC flash_erase_range   0x08006C80 flash_program_record
 *   0x0800587C flash_erase_param_page
 *   0x08005974 flash_write_params  0x0800590C flash_read_params
 *   0x08005930 flash_read_word     0x0800B820 flash_store_params
 *   0x08006EB8 iwdg_init / 0x08006F68 iwdg_refresh / 0x08006FBC iwdg_start
 */
#include "ad200.h"

/* FLASH 驱动状态结构 0x20000228：
 *   +0x18 busy；+0x1C 错误标志。擦除结果输出在 0x20000220。 */
static volatile struct {
    uint8_t pad[0x18];
    uint8_t busy;
    uint8_t err;
} g_flash_state;                     /* 0x20000228 */
static uint32_t g_flash_err_out;     /* 0x20000220 */
static uint32_t g_flash_addr;        /* 0x2000021C（state+8） */

/* 0x08006CFC: 解锁（KEYR 双写） */
int flash_unlock(void)
{
    if (!(FLASH_CR & FLASH_CR_LOCK))                 /* 0x08006cfe 测 bit7 */
        return 1;
    FLASH_KEYR = 0x45670123u;                        /* 0x08006d06 */
    FLASH_KEYR = 0xCDEF89ABu;                        /* 0x08006d0a */
    return 0;
}

/* 0x08006C6C: 上锁 */
void flash_lock(void)
{
    FLASH_CR |= FLASH_CR_LOCK;                       /* 0x08006c70 OR 0x80 */
}

/* 0x080058C8: 页擦除 */
void flash_erase_page(uint32_t addr)
{
    g_flash_state.err = 0;                           /* 0x080058cc: state[0x1C]=0 */
    FLASH_CR |= FLASH_CR_PER;                        /* 0x080058d0 */
    FLASH_AR = addr;                                 /* 0x080058d8 */
    FLASH_CR |= FLASH_CR_STRT;                       /* 0x080058da OR 0x40 */
}

/* 0x080058A4: 整片擦除 */
void flash_mass_erase(void)
{
    g_flash_state.err = 0;
    FLASH_CR |= FLASH_CR_MER;                        /* 0x080058ac OR 0x04 */
    FLASH_CR |= FLASH_CR_STRT;
}

/* 0x080058EC: 半字编程 */
void flash_program_halfword(uint32_t addr, uint16_t v)
{
    g_flash_state.err = 0;
    FLASH_CR |= FLASH_CR_PG;                         /* 0x080058f6 OR 0x01 */
    *(volatile uint16_t *)addr = v;                  /* 0x080058fe */
}

/* 0x0800593C: 状态解码 + 清标志 */
uint32_t flash_get_status(void)
{
    uint32_t sr = FLASH_SR;
    uint32_t ret = 0;

    if (sr & FLASH_SR_WRPRTERR) {                    /* bit4 */
        g_flash_state.err |= 2;                      /* 0x0800594a */
        ret = 0x10;
    }
    if (sr & FLASH_SR_PGERR) {                       /* bit2 */
        g_flash_state.err |= 1;
        ret |= 4;
    }
    FLASH_SR = ret;                                  /* 写 1 清除 */
    return ret;
}

/* 0x080059B4: 等待操作完成（超时 0xC350=50000 tick） */
uint32_t flash_wait_last_operation(uint32_t timeout)
{
    uint32_t t0 = get_tick();

    for (;;) {
        uint32_t sr = FLASH_SR;
        if (!(sr & FLASH_SR_BSY))                    /* 0x080059d8 */
            break;
        if (timeout != 0xFFFFFFFFu && (get_tick() - t0) > timeout) {
            if (timeout == 0 || get_tick() - t0 > timeout)
                return 3;                            /* 0x080059d4 超时 */
        }
    }
    if (FLASH_SR & FLASH_SR_EOP)
        FLASH_SR = FLASH_SR_EOP;                     /* 清 EOP（0x080059e4） */
    if ((FLASH_SR & FLASH_SR_WRPRTERR) || (FLASH_SR & FLASH_SR_PGERR)) {
        flash_get_status();
        return 1;
    }
    return 0;
}

/* 0x08006BCC: flash_erase_range(desc{type,start,pages}, &err_addr)
 * type==1 → 整片擦除；否则按 0x800 字节/页循环擦除。
 */
int flash_erase_range(const uint32_t desc[3], uint32_t *err_addr)
{
    uint32_t start = desc[1];
    uint32_t pages = desc[2];
    uint32_t addr;
    uint32_t rc;

    if (g_flash_state.busy)                          /* 0x08006bd4 */
        return 2;
    g_flash_state.busy = 1;

    if (desc[0] != 1) {
        rc = flash_wait_last_operation(0xC350);
        if (rc) goto out;
        *err_addr = 0xFFFFFFFFu;                     /* 0x08006c1c */
        addr = start;
        while (addr < start + pages * 0x800u) {      /* 0x08006c48 */
            flash_erase_page(addr);                  /* 0x08006c26 */
            rc = flash_wait_last_operation(0xC350);
            FLASH_CR &= ~FLASH_CR_PER;               /* 0x08006c38 */
            if (rc) { *err_addr = addr; goto out; }
            addr += 0x800u;
        }
    } else {
        rc = flash_wait_last_operation(0xC350);      /* 0x08006bf8 */
        if (rc) goto out;
        flash_mass_erase();                          /* 0x08006c02 */
        rc = flash_wait_last_operation(0xC350);
        FLASH_CR &= ~FLASH_CR_MER;                   /* 0x08006c14 */
    }
out:
    g_flash_state.busy = 0;
    return (int)rc;
}

/* 0x08006C80: flash_program_record(mode, dst, src, len)
 * 写 mode 个半字（1/2/4），每个半字是 src 上 16 字节块的 crc16_block 值 [推测]。
 */
int flash_program_record(uint32_t mode, uint32_t dst, uint32_t src, uint32_t len)
{
    uint32_t n = (mode == 1 || mode == 2) ? mode : 4;
    uint32_t i;

    if (g_flash_state.busy)
        return 2;
    g_flash_state.busy = 1;
    if (flash_wait_last_operation(0xC350))
        goto out;
    for (i = 0; i < n; i++) {
        uint16_t v = crc16_block((const uint8_t *)src, 16, i * 16); /* 0x08006cba */
        flash_program_halfword(dst + 2 * i, v);                     /* 0x08006cc6 */
        if (flash_wait_last_operation(0xC350))
            goto out;
        FLASH_CR &= ~FLASH_CR_PG;
    }
out:
    g_flash_state.busy = 0;
    return 0;
}

/* ------------------------------------------------------------------ 参数页
 * 参数页 0x0801F000（1 页 0x800B），参数块 = 8 个 u32（RAM 0x20000464）。
 */
void flash_erase_param_page(void)
{
    uint32_t desc[3] = { 0, FLASH_PARAM_PAGE, 1 };   /* 0x08005884..0x0800588e */
    uint32_t err;

    flash_unlock();                                  /* 0x0800587e */
    flash_erase_range(desc, &err);                   /* 0x08005892 -> 0x08006BCC */
}

/* 0x08005974: 保存 8 个 u32 到 addr */
void flash_write_params(uint32_t addr)
{
    uint32_t i;

    flash_unlock();                                  /* 0x08005978 */
    g_flash_addr = addr;                             /* 0x08005980 state+8 */

    for (i = 0; i < 8; i++) {
        uint32_t v = g_flash_params[i];              /* 0x20000464 + i*4 */
        int rc = flash_program_record(2, g_flash_addr, v, 0);  /* 0x0800598e */
        if (rc == 0)
            g_flash_addr += 4;                       /* 0x08005996 */
    }
    flash_lock();                                    /* 0x080059a4 */
}

/* 0x0800590C: 从 addr 读 8 个 u32 到 0x20000464 */
void flash_read_params(uint32_t addr)
{
    uint32_t i;
    g_flash_addr = addr;                             /* 0x08005912 */
    for (i = 0; i < 8; i++)
        g_flash_params[i] = ((const uint32_t *)addr)[i];  /* 0x0800591a */
}

/* 0x08005930: 读一个 32 位字 */
uint32_t flash_read_word(uint32_t addr)
{
    g_flash_addr = addr;
    return *(volatile uint32_t *)addr;
}

/* 0x0800B820: 解锁 + 写参数 + 上锁（工作流封装） */
void flash_store_params(void)
{
    flash_unlock();
    flash_write_params(FLASH_PARAM_PAGE);
    flash_lock();
}

/* ------------------------------------------------------------------ IWDG
 * 0x08006EB8 iwdg_init(base, pr, rlr, winr)：KR=0x5555 → PR/RLR/WINR → 等 SR → KR=0xAAAA
 * 反汇编 0x08006eb8..0x08006f58。
 */
int iwdg_init(uint32_t base, uint32_t pr, uint32_t rlr, uint32_t winr)
{
    volatile uint32_t *iwdg = (volatile uint32_t *)base;
    uint32_t t0;

    iwdg[0] = 0x00005555u;                           /* KR 解锁 0x08006ef0 */
    iwdg[1] = (iwdg[1] & ~7u) | (pr & 7u);           /* PR 0x08006ef4 */
    iwdg[2] = (iwdg[2] & ~0xFFFu) | (rlr & 0xFFFu);  /* RLR 0x08006f02 */
    if (winr != 0xFFF || iwdg[4] != 0xFFF)
        iwdg[4] = winr;                              /* WINR 0x08006f44 */
    t0 = get_tick();
    while (iwdg[3] != 0) {                           /* SR：PVU/RVU/WVU */
        if (get_tick() - t0 > 1000) return 3;        /* 0x08006f36 */
    }
    iwdg[0] = 0x0000AAAAu;                           /* 重装 0x08006f52 区域 */
    return 0;
}

void iwdg_refresh(void)
{
    IWDG_KR = 0x0000AAAAu;                           /* 0x08006F68, literal 0x08006FB8 */
}

void iwdg_start(void)
{
    IWDG_KR = 0x0000CCCCu;                           /* 0x08006FBC, literal 0x08007030 */
    IWDG_KR = 0x0000AAAAu;                           /* literal 0x0800702C */
}

/* 0x080098E0：默认 PR=6（/256），RLR=0xFFF（≈26s @LSI 40kHz） */
void iwdg_init_default(void)
{
    g_iwdg_state.base = IWDG_BASE;                   /* 0x20000248 */
    g_iwdg_state.pr   = 6;
    g_iwdg_state.rlr  = 0xFFF;
    g_iwdg_state.winr = 0xFFF;
    if (iwdg_init(IWDG_BASE, 6, 0xFFF, 0xFFF))
        Error_Handler();
}

/* 0x0800990C：以 ms 计算 RLR（LSI 40kHz / 256 ≈ 156 Hz → RLR ≈ ms*0.156） */
void iwdg_init_timeout(uint32_t ms)
{
    uint32_t rlr = (ms * 40u) / 256u;
    if (rlr > 0xFFF) rlr = 0xFFF;
    if (iwdg_init(IWDG_BASE, 6, rlr, 0xFFF))
        Error_Handler();
}
