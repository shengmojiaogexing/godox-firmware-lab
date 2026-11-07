/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080097F8
 * Name      : FUN_080097F8
 * End       : 0x08009832
 * Size      : 58 bytes, 24 instructions decoded in body (functions.json insns=24, blocks=6)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 5  -> 0x08004690 FUN_08004690, 0x08005DFC FUN_08005DFC, 0x0800B55C FUN_0800B55C, 0x0800B588 FUN_0800B588, 0x0800C92C FUN_0800C92C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080097F8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080097F8: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x080097FA: 01f0affe bl       #0x800b55c */
    FUN_0800B55C();   /* BL -> 0x0800B55C FUN_0800B55C */
    /* 0x080097FE: 0d4c     ldr      r4, [pc, #0x34]  ; =RAM_20000120 */
    r4 = *(uint32_t *)0x08009834;   /* = RAM 0x20000120 */
    /* 0x08009800: a078     ldrb     r0, [r4, #2] */
    r0 = *(uint8_t *)(r4 + 2);
    /* 0x08009802: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009804: 14d1     bne      #0x8009830 */
    if (r0 != 1) goto L_08009830;
    /* 0x08009806: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08009808: a570     strb     r5, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r5;   /* store */
    /* 0x0800980A: 0b48     ldr      r0, [pc, #0x2c]  ; =RAM_20000454 */
    r0 = *(uint32_t *)0x08009838;   /* = RAM 0x20000454 */
    /* 0x0800980C: faf740ff bl       #0x8004690 */
    FUN_08004690();   /* BL -> 0x08004690 FUN_08004690 */
    /* 0x08009810: 03f08cf8 bl       #0x800c92c */
    FUN_0800C92C();   /* BL -> 0x0800C92C FUN_0800C92C */
    /* 0x08009814: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009816: 0bd1     bne      #0x8009830 */
    if (r0 != 1) goto L_08009830;
    /* 0x08009818: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800981A: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800981C: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800981E: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */
    /* 0x08009820: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x08009822: 03d3     blo      #0x800982c */
    if (r0 < 0x14) goto L_0800982C;   /* unsigned */
    /* 0x08009824: fcf7eafa bl       #0x8005dfc */
    FUN_08005DFC();   /* BL -> 0x08005DFC FUN_08005DFC */
    /* 0x08009828: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800982A: 2570     strb     r5, [r4] */
    *(uint8_t *)(r4) = r5;   /* store */

    L_0800982C:
    /* 0x0800982C: 01f0acfe bl       #0x800b588 */
    FUN_0800B588();   /* BL -> 0x0800B588 FUN_0800B588 */

    L_08009830:
    /* 0x08009830: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
