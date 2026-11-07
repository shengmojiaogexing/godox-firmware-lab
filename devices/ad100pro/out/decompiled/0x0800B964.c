/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B964
 * Name      : SystemInit
 * End       : 0x0800B9B0
 * Size      : 76 bytes, 38 instructions decoded in body (functions.json insns=38, blocks=1)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : RCC.CFGR, RCC.CFGR2, RCC.CFGR3, RCC.CIR, RCC.CR, RCC.CR2
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void SystemInit(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B964: 1248     ldr      r0, [pc, #0x48]  ; =RCC.CR */
    r0 = *(uint32_t *)0x0800B9B0;   /* = MMIO RCC+0x00 */
    /* 0x0800B966: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x0800B968: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800B96A: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x0800B96C: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x0800B96E: 4168     ldr      r1, [r0, #4] */
    r1 = *(uint32_t *)(r0 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x0800B970: 104a     ldr      r2, [pc, #0x40]  ; =0x08FFB80C */
    r2 = *(uint32_t *)0x0800B9B4;   /* = 0x08FFB80C */
    /* 0x0800B972: 1140     ands     r1, r2 */
    r1 = r1 & r2;
    /* 0x0800B974: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */  /* MMIO: RCC.CFGR(w) */
    /* 0x0800B976: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x0800B978: 0f4a     ldr      r2, [pc, #0x3c]  ; =0xFEF6FFFF */
    r2 = *(uint32_t *)0x0800B9B8;   /* = 0xFEF6FFFF */
    /* 0x0800B97A: 1140     ands     r1, r2 */
    r1 = r1 & r2;
    /* 0x0800B97C: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x0800B97E: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x0800B980: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800B982: 9204     lsls     r2, r2, #0x12 */
    r2 = r2 << 0x12;
    /* 0x0800B984: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x0800B986: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x0800B988: 4168     ldr      r1, [r0, #4] */
    r1 = *(uint32_t *)(r0 + 4);  /* MMIO: RCC.CFGR(r) */
    /* 0x0800B98A: 3f22     movs     r2, #0x3f */
    r2 = 0x3f;
    /* 0x0800B98C: 1204     lsls     r2, r2, #0x10 */
    r2 = r2 << 0x10;
    /* 0x0800B98E: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x0800B990: 4160     str      r1, [r0, #4] */
    *(uint32_t *)(r0 + 4) = r1;   /* store */  /* MMIO: RCC.CFGR(w) */
    /* 0x0800B992: c16a     ldr      r1, [r0, #0x2c] */
    r1 = *(uint32_t *)(r0 + 44);  /* MMIO: RCC.CFGR2(r) */
    /* 0x0800B994: 0909     lsrs     r1, r1, #4 */
    r1 = r1 >> 4;
    /* 0x0800B996: 0901     lsls     r1, r1, #4 */
    r1 = r1 << 4;
    /* 0x0800B998: c162     str      r1, [r0, #0x2c] */
    *(uint32_t *)(r0 + 44) = r1;   /* store */  /* MMIO: RCC.CFGR2(w) */
    /* 0x0800B99A: 016b     ldr      r1, [r0, #0x30] */
    r1 = *(uint32_t *)(r0 + 48);  /* MMIO: RCC.CFGR3(r) */
    /* 0x0800B99C: 074a     ldr      r2, [pc, #0x1c]  ; =0xFFFCFE2C */
    r2 = *(uint32_t *)0x0800B9BC;   /* = 0xFFFCFE2C */
    /* 0x0800B99E: 1140     ands     r1, r2 */
    r1 = r1 & r2;
    /* 0x0800B9A0: 0163     str      r1, [r0, #0x30] */
    *(uint32_t *)(r0 + 48) = r1;   /* store */  /* MMIO: RCC.CFGR3(w) */
    /* 0x0800B9A2: 416b     ldr      r1, [r0, #0x34] */
    r1 = *(uint32_t *)(r0 + 52);  /* MMIO: RCC.CR2(r) */
    /* 0x0800B9A4: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x0800B9A6: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x0800B9A8: 4163     str      r1, [r0, #0x34] */
    *(uint32_t *)(r0 + 52) = r1;   /* store */  /* MMIO: RCC.CR2(w) */
    /* 0x0800B9AA: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800B9AC: 8160     str      r1, [r0, #8] */
    *(uint32_t *)(r0 + 8) = r1;   /* store */  /* MMIO: RCC.CIR(w) */
    /* 0x0800B9AE: 7047     bx       lr */
    return;
}
