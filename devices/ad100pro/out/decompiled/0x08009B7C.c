/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009B7C
 * Name      : FUN_08009B7C
 * End       : 0x08009BC4
 * Size      : 72 bytes, 33 instructions decoded in body (functions.json insns=33, blocks=3)
 * Class     : function
 * Callers   : 1  -> 0x080099B0 FUN_080099B0
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009B7C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009B7C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08009B7E: 114c     ldr      r4, [pc, #0x44]  ; =RAM_20000230 */
    r4 = *(uint32_t *)0x08009BC4;   /* = RAM 0x20000230 */
    /* 0x08009B80: e078     ldrb     r0, [r4, #3] */
    r0 = *(uint8_t *)(r4 + 3);
    /* 0x08009B82: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009B84: 1dd1     bne      #0x8009bc2 */
    if (r0 != 1) goto L_08009BC2;
    /* 0x08009B86: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x08009B88: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x08009B8A: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x08009B8C: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08009B8E: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08009B90: e570     strb     r5, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r5;   /* store */
    /* 0x08009B92: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08009B94: fdf747fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009B98: 4b20     movs     r0, #0x4b */
    r0 = 0x4b;
    /* 0x08009B9A: 2572     strb     r5, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r5;   /* store */
    /* 0x08009B9C: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x08009B9E: 6081     strh     r0, [r4, #0xa] */
    *(uint16_t *)(r4 + 10) = r0;   /* store */
    /* 0x08009BA0: 094e     ldr      r6, [pc, #0x24]  ; =GPIOB.MODER */
    r6 = *(uint32_t *)0x08009BC8;   /* = MMIO GPIOB+0x00 */
    /* 0x08009BA2: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009BA4: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */
    /* 0x08009BA6: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009BA8: 8902     lsls     r1, r1, #0xa */
    r1 = r1 << 0xa;
    /* 0x08009BAA: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08009BAC: fdf73bfc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009BB0: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009BB2: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009BB4: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x08009BB6: 3046     mov      r0, r6 */
    r0 = r6;
    /* 0x08009BB8: fdf735fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009BBC: 6078     ldrb     r0, [r4, #1] */
    r0 = *(uint8_t *)(r4 + 1);
    /* 0x08009BBE: a071     strb     r0, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r0;   /* store */
    /* 0x08009BC0: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */

    L_08009BC2:
    /* 0x08009BC2: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
