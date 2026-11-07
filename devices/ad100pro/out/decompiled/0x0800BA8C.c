/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BA8C
 * Name      : IRQ15_Handler
 * End       : 0x0800BAAE
 * Size      : 34 bytes, 15 instructions decoded in body (functions.json insns=15, blocks=5)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080083E8 FUN_080083E8, 0x0800B61C FUN_0800B61C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void IRQ15_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BA8C: 10b5     push     {r4, lr} */
    push({r4, lr});
    /* 0x0800BA8E: 0848     ldr      r0, [pc, #0x20]  ; =RAM_200001D4 */
    r0 = *(uint32_t *)0x0800BAB0;   /* = RAM 0x200001D4 */
    /* 0x0800BA90: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x0800BA92: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800BA94: 07d1     bne      #0x800baa6 */
    if (r0 != 0) goto L_0800BAA6;
    /* 0x0800BA96: 074c     ldr      r4, [pc, #0x1c]  ; =RAM_20000109 */
    r4 = *(uint32_t *)0x0800BAB4;   /* = RAM 0x20000109 */
    /* 0x0800BA98: 2078     ldrb     r0, [r4] */
    r0 = *(uint8_t *)(r4);
    /* 0x0800BA9A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800BA9C: 01d1     bne      #0x800baa2 */
    if (r0 != 0) goto L_0800BAA2;
    /* 0x0800BA9E: fff7bdfd bl       #0x800b61c */
    FUN_0800B61C();   /* BL -> 0x0800B61C FUN_0800B61C */

    L_0800BAA2:
    /* 0x0800BAA2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800BAA4: 2070     strb     r0, [r4] */
    *(uint8_t *)(r4) = r0;   /* store */

    L_0800BAA6:
    /* 0x0800BAA6: 0448     ldr      r0, [pc, #0x10]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x0800BAB8;   /* = RAM 0x200002D0 */
    /* 0x0800BAA8: fcf79efc bl       #0x80083e8 */
    FUN_080083E8();   /* BL -> 0x080083E8 FUN_080083E8 */
    /* 0x0800BAAC: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
}
