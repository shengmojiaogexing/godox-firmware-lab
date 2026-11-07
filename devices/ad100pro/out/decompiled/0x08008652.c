/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008652
 * Name      : FUN_08008652
 * End       : 0x08008686
 * Size      : 52 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=6)
 * Class     : function
 * Callers   : 1  -> 0x08009768 FUN_08009768
 * Callees   : 2  -> 0x08008686 FUN_08008686, 0x0800BABC FUN_0800BABC
 * MMIO      : RCC.+0x19, RCC.APB2ENR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008652(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008652: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08008654: 0400     movs     r4, r0 */
    r4 = r0;
    /* 0x08008656: 05d0     beq      #0x8008664 */
    if (/* beq */) goto L_08008664;
    /* 0x08008658: 2546     mov      r5, r4 */
    r5 = r4;
    /* 0x0800865A: 2035     adds     r5, #0x20 */
    r5 = r5 + 0x20;
    /* 0x0800865C: 687e     ldrb     r0, [r5, #0x19] */
    r0 = *(uint8_t *)(r5 + 25);  /* MMIO: RCC.+0x19(r) */
    /* 0x0800865E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008660: 02d0     beq      #0x8008668 */
    if (r0 == 0) goto L_08008668;
    /* 0x08008662: 06e0     b        #0x8008672 */
    goto L_08008672;

    L_08008664:
    /* 0x08008664: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008666: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08008668:
    /* 0x08008668: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800866A: 2876     strb     r0, [r5, #0x18] */
    *(uint8_t *)(r5 + 24) = r0;   /* store */  /* MMIO: RCC.APB2ENR(w) */
    /* 0x0800866C: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800866E: 00f00af8 bl       #0x8008686 */
    FUN_08008686();   /* BL -> 0x08008686 FUN_08008686 */

    L_08008672:
    /* 0x08008672: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08008674: 6876     strb     r0, [r5, #0x19] */
    *(uint8_t *)(r5 + 25) = r0;   /* store */  /* MMIO: RCC.+0x19(w) */
    /* 0x08008676: 211d     adds     r1, r4, #4 */
    r1 = r4 + 4;
    /* 0x08008678: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
    /* 0x0800867A: 03f01ffa bl       #0x800babc */
    FUN_0800BABC();   /* BL -> 0x0800BABC FUN_0800BABC */
    /* 0x0800867E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008680: 6876     strb     r0, [r5, #0x19] */
    *(uint8_t *)(r5 + 25) = r0;   /* store */  /* MMIO: RCC.+0x19(w) */
    /* 0x08008682: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008684: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
