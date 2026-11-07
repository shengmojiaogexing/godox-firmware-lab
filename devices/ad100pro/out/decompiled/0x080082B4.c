/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080082B4
 * Name      : FUN_080082B4
 * End       : 0x080082CA
 * Size      : 22 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : function
 * Callers   : 4  -> 0x08009D84 FUN_08009D84, 0x08009DE2 FUN_08009DE2, 0x0800B9C0 FUN_0800B9C0, 0x0800BA16 FUN_0800BA16
 * Callees   : 0  -> -
 * MMIO      : RCC.CR, TIM14.DIER
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080082B4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080082B4: 0268     ldr      r2, [r0] */
    r2 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x080082B6: d368     ldr      r3, [r2, #0xc] */
    r3 = *(uint32_t *)(r2 + 12);  /* MMIO: TIM14.DIER(r) */
    /* 0x080082B8: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x080082BA: 0b43     orrs     r3, r1 */
    r3 = r3 | r1;
    /* 0x080082BC: d360     str      r3, [r2, #0xc] */
    *(uint32_t *)(r2 + 12) = r3;   /* store */  /* MMIO: TIM14.DIER(w) */
    /* 0x080082BE: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x080082C0: 0268     ldr      r2, [r0] */
    r2 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x080082C2: 0a43     orrs     r2, r1 */
    r2 = r2 | r1;
    /* 0x080082C4: 0260     str      r2, [r0] */
    *(uint32_t *)(r0) = r2;   /* store */  /* MMIO: RCC.CR(w) */
    /* 0x080082C6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080082C8: 7047     bx       lr */
    return;
}
