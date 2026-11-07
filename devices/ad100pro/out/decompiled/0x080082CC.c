/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080082CC
 * Name      : FUN_080082CC
 * End       : 0x080082F4
 * Size      : 40 bytes, 20 instructions decoded in body (functions.json insns=20, blocks=4)
 * Class     : function
 * Callers   : 1  -> 0x08009E04 FUN_08009E04
 * Callees   : 0  -> -
 * MMIO      : RCC.BDCR, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080082CC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080082CC: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x080082CE: ca68     ldr      r2, [r1, #0xc] */
    r2 = *(uint32_t *)(r1 + 12);
    /* 0x080082D0: 5208     lsrs     r2, r2, #1 */
    r2 = r2 >> 1;
    /* 0x080082D2: 5200     lsls     r2, r2, #1 */
    r2 = r2 << 1;
    /* 0x080082D4: ca60     str      r2, [r1, #0xc] */
    *(uint32_t *)(r1 + 12) = r2;   /* store */
    /* 0x080082D6: 0068     ldr      r0, [r0] */
    r0 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x080082D8: 016a     ldr      r1, [r0, #0x20] */
    r1 = *(uint32_t *)(r0 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x080082DA: 064a     ldr      r2, [pc, #0x18]  ; =0x00001111 */
    r2 = *(uint32_t *)0x080082F4;   /* = 0x00001111 */
    /* 0x080082DC: 1142     tst      r1, r2 */
    /* flags = tst(r1, r2) */
    /* 0x080082DE: 07d1     bne      #0x80082f0 */
    if ((r1 & r2)) goto L_080082F0;   /* test bits */
    /* 0x080082E0: 016a     ldr      r1, [r0, #0x20] */
    r1 = *(uint32_t *)(r0 + 32);  /* MMIO: RCC.BDCR(r) */
    /* 0x080082E2: 9210     asrs     r2, r2, #2 */
    r2 = r2 >> 2;
    /* 0x080082E4: 1142     tst      r1, r2 */
    /* flags = tst(r1, r2) */
    /* 0x080082E6: 03d1     bne      #0x80082f0 */
    if ((r1 & r2)) goto L_080082F0;   /* test bits */
    /* 0x080082E8: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: RCC.CR(r) */
    /* 0x080082EA: 4908     lsrs     r1, r1, #1 */
    r1 = r1 >> 1;
    /* 0x080082EC: 4900     lsls     r1, r1, #1 */
    r1 = r1 << 1;
    /* 0x080082EE: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: RCC.CR(w) */

    L_080082F0:
    /* 0x080082F0: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080082F2: 7047     bx       lr */
    return;
}
