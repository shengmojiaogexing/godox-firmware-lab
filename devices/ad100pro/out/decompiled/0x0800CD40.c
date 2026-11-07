/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CD40
 * Name      : FUN_0800CD40
 * End       : 0x0800CD76
 * Size      : 54 bytes, 25 instructions decoded in body (functions.json insns=25, blocks=5)
 * Class     : function
 * Callers   : 1  -> 0x0800CDC8 FUN_0800CDC8
 * Callees   : 1  -> 0x0800B874 FUN_0800B874
 * MMIO      : GPIOB.BSRR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CD40(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CD40: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x0800CD42: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800CD44: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x0800CD46: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x0800CD48: 0b4b     ldr      r3, [pc, #0x2c]  ; =GPIOB.MODER */
    r3 = *(uint32_t *)0x0800CD78;   /* = MMIO GPIOB+0x00 */
    /* 0x0800CD4A: 8a02     lsls     r2, r1, #0xa */
    r2 = r1 << 0xa;

    L_0800CD4C:
    /* 0x0800CD4C: 9a61     str      r2, [r3, #0x18] */
    *(uint32_t *)(r3 + 24) = r2;   /* store */  /* MMIO: GPIOB.BSRR(w) */
    /* 0x0800CD4E: 00bf     nop       */
    /* nop */
    /* 0x0800CD50: fef790fd bl       #0x800b874 */
    FUN_0800B874();   /* BL -> 0x0800B874 FUN_0800B874 */
    /* 0x0800CD54: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;
    /* 0x0800CD56: 1d69     ldr      r5, [r3, #0x10] */
    r5 = *(uint32_t *)(r3 + 16);  /* MMIO: GPIOB.IDR(r) */
    /* 0x0800CD58: 000e     lsrs     r0, r0, #0x18 */
    r0 = r0 >> 0x18;
    /* 0x0800CD5A: 6d04     lsls     r5, r5, #0x11 */
    r5 = r5 << 0x11;
    /* 0x0800CD5C: 00d5     bpl      #0x800cd60 */
    if (/* bpl */) goto L_0800CD60;   /* result non-negative */
    /* 0x0800CD5E: 2043     orrs     r0, r4 */
    r0 = r0 | r4;

    L_0800CD60:
    /* 0x0800CD60: 9a62     str      r2, [r3, #0x28] */
    *(uint32_t *)(r3 + 40) = r2;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x0800CD62: 00bf     nop       */
    /* nop */
    /* 0x0800CD64: 00bf     nop       */
    /* nop */
    /* 0x0800CD66: 00bf     nop       */
    /* nop */
    /* 0x0800CD68: fef784fd bl       #0x800b874 */
    FUN_0800B874();   /* BL -> 0x0800B874 FUN_0800B874 */
    /* 0x0800CD6C: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x0800CD6E: 0906     lsls     r1, r1, #0x18 */
    r1 = r1 << 0x18;
    /* 0x0800CD70: 090e     lsrs     r1, r1, #0x18 */
    r1 = r1 >> 0x18;
    /* 0x0800CD72: ebd1     bne      #0x800cd4c */
    if (/* bne */) goto L_0800CD4C;
    /* 0x0800CD74: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */
}
