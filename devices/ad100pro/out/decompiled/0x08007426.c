/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007426
 * Name      : FUN_08007426
 * End       : 0x08007432
 * Size      : 12 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=3)
 * Class     : hub
 * Callers   : 128  -> 0x08003BAC FUN_08003BAC, 0x08003E2C FUN_08003E2C, 0x08003E56 FUN_08003E56, 0x08003E66 FUN_08003E66, 0x08003E78 FUN_08003E78, 0x08003E88 FUN_08003E88, 0x08003EB4 FUN_08003EB4, 0x08003ED8 FUN_08003ED8, ... +120
 * Callees   : 0  -> -
 * MMIO      : EXTI.+0x18, EXTI.+0x28
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007426(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007426: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x08007428: 01d0     beq      #0x800742e */
    if (r2 == 0) goto L_0800742E;
    /* 0x0800742A: 8161     str      r1, [r0, #0x18] */
    *(uint32_t *)(r0 + 24) = r1;   /* store */  /* MMIO: EXTI.+0x18(w) */
    /* 0x0800742C: 7047     bx       lr */
    return;

    L_0800742E:
    /* 0x0800742E: 8162     str      r1, [r0, #0x28] */
    *(uint32_t *)(r0 + 40) = r1;   /* store */  /* MMIO: EXTI.+0x28(w) */
    /* 0x08007430: 7047     bx       lr */
    return;
}
