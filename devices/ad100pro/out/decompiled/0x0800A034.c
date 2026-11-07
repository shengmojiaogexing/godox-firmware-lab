/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A034
 * Name      : FUN_0800A034
 * End       : 0x0800A10E
 * Size      : 218 bytes, 102 instructions decoded in body (functions.json insns=102, blocks=26)
 * Class     : function
 * Callers   : 3  -> 0x0800A230 FUN_0800A230, 0x0800C080 FUN_0800C080, 0x0800C102 FUN_0800C102
 * Callees   : 4  -> 0x0800339C FUN_0800339C, 0x080033F4 FUN_080033F4, 0x08005B9C FUN_08005B9C, 0x08008948 FUN_08008948
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A034(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A034: feb5     push     {r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x0800A036: 3648     ldr      r0, [pc, #0xd8]  ; =RAM_2000020A */
    r0 = *(uint32_t *)0x0800A110;   /* = RAM 0x2000020A */
    /* 0x0800A038: 364a     ldr      r2, [pc, #0xd8]  ; =0x000003B6 */
    r2 = *(uint32_t *)0x0800A114;   /* = 0x000003B6 */
    /* 0x0800A03A: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x0800A03C: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x0800A03E: 3649     ldr      r1, [pc, #0xd8]  ; =RAM_200001ED */
    r1 = *(uint32_t *)0x0800A118;   /* = RAM 0x200001ED */
    /* 0x0800A040: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800A042: 364c     ldr      r4, [pc, #0xd8]  ; =RAM_2000021C */
    r4 = *(uint32_t *)0x0800A11C;   /* = RAM 0x2000021C */
    /* 0x0800A044: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x0800A046: 09d9     bls      #0x800a05c */
    if (r0 <= r2) goto L_0800A05C;   /* unsigned */
    /* 0x0800A048: e078     ldrb     r0, [r4, #3] */
    r0 = *(uint8_t *)(r4 + 3);
    /* 0x0800A04A: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A04C: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A04E: e070     strb     r0, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r0;   /* store */
    /* 0x0800A050: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A052: 02d9     bls      #0x800a05a */
    if (r0 <= 0xa) goto L_0800A05A;   /* unsigned */
    /* 0x0800A054: a672     strb     r6, [r4, #0xa] */
    *(uint8_t *)(r4 + 10) = r6;   /* store */
    /* 0x0800A056: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x0800A058: 0f70     strb     r7, [r1] */
    *(uint8_t *)(r1) = r7;   /* store */

    L_0800A05A:
    /* 0x0800A05A: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A05C:
    /* 0x0800A05C: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800A05E: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x0800A060: 0ad2     bhs      #0x800a078 */
    if (r0 >= 0x14) goto L_0800A078;   /* unsigned */
    /* 0x0800A062: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x0800A064: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A066: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A068: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */
    /* 0x0800A06A: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A06C: f5d9     bls      #0x800a05a */
    if (r0 <= 0xa) goto L_0800A05A;   /* unsigned */
    /* 0x0800A06E: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800A070: a072     strb     r0, [r4, #0xa] */
    *(uint8_t *)(r4 + 10) = r0;   /* store */
    /* 0x0800A072: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */
    /* 0x0800A074: 0e70     strb     r6, [r1] */
    *(uint8_t *)(r1) = r6;   /* store */

    L_0800A076:
    /* 0x0800A076: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A078:
    /* 0x0800A078: e570     strb     r5, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r5;   /* store */
    /* 0x0800A07A: f9f78ff9 bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x0800A07E: 284a     ldr      r2, [pc, #0xa0]  ; =0x69098C81 */
    r2 = *(uint32_t *)0x0800A120;   /* = 0x69098C81 */
    /* 0x0800A080: 284b     ldr      r3, [pc, #0xa0]  ; =0x405CA583 */
    r3 = *(uint32_t *)0x0800A124;   /* = 0x405CA583 */
    /* 0x0800A082: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x0800A084: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800A086: f9f7b5f9 bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x0800A08A: 0dd2     bhs      #0x800a0a8 */
    if (r0 >= 0xa) goto L_0800A0A8;   /* unsigned */
    /* 0x0800A08C: 2079     ldrb     r0, [r4, #4] */
    r0 = *(uint8_t *)(r4 + 4);
    /* 0x0800A08E: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A090: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A092: 2071     strb     r0, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r0;   /* store */
    /* 0x0800A094: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A096: 17d9     bls      #0x800a0c8 */
    if (r0 <= 0xa) goto L_0800A0C8;   /* unsigned */
    /* 0x0800A098: a07a     ldrb     r0, [r4, #0xa] */
    r0 = *(uint8_t *)(r4 + 10);
    /* 0x0800A09A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A09C: 01d0     beq      #0x800a0a2 */
    if (r0 == 1) goto L_0800A0A2;
    /* 0x0800A09E: fbf77dfd bl       #0x8005b9c */
    FUN_08005B9C();   /* BL -> 0x08005B9C FUN_08005B9C */

    L_0800A0A2:
    /* 0x0800A0A2: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */
    /* 0x0800A0A4: a772     strb     r7, [r4, #0xa] */
    *(uint8_t *)(r4 + 10) = r7;   /* store */
    /* 0x0800A0A6: 0fe0     b        #0x800a0c8 */
    goto L_0800A0C8;

    L_0800A0A8:
    /* 0x0800A0A8: 1f4a     ldr      r2, [pc, #0x7c]  ; =0x97AC5FED */
    r2 = *(uint32_t *)0x0800A128;   /* = 0x97AC5FED */
    /* 0x0800A0AA: 204b     ldr      r3, [pc, #0x80]  ; =0x4069849C */
    r3 = *(uint32_t *)0x0800A12C;   /* = 0x4069849C */
    /* 0x0800A0AC: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x0800A0AE: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800A0B0: f9f7a0f9 bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x0800A0B4: 01d2     bhs      #0x800a0ba */
    if (r0 >= 1) goto L_0800A0BA;   /* unsigned */
    /* 0x0800A0B6: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */
    /* 0x0800A0B8: 06e0     b        #0x800a0c8 */
    goto L_0800A0C8;

    L_0800A0BA:
    /* 0x0800A0BA: a07a     ldrb     r0, [r4, #0xa] */
    r0 = *(uint8_t *)(r4 + 10);
    /* 0x0800A0BC: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A0BE: 01d1     bne      #0x800a0c4 */
    if (r0 != 1) goto L_0800A0C4;
    /* 0x0800A0C0: fef742fc bl       #0x8008948 */
    FUN_08008948();   /* BL -> 0x08008948 FUN_08008948 */

    L_0800A0C4:
    /* 0x0800A0C4: 2571     strb     r5, [r4, #4] */
    *(uint8_t *)(r4 + 4) = r5;   /* store */
    /* 0x0800A0C6: a572     strb     r5, [r4, #0xa] */
    *(uint8_t *)(r4 + 10) = r5;   /* store */

    L_0800A0C8:
    /* 0x0800A0C8: 194a     ldr      r2, [pc, #0x64]  ; =0xF0F0F0F1 */
    r2 = *(uint32_t *)0x0800A130;   /* = 0xF0F0F0F1 */
    /* 0x0800A0CA: 1a4b     ldr      r3, [pc, #0x68]  ; =0x4070F0F0 */
    r3 = *(uint32_t *)0x0800A134;   /* = 0x4070F0F0 */
    /* 0x0800A0CC: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x0800A0CE: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800A0D0: f9f790f9 bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x0800A0D4: 08d2     bhs      #0x800a0e8 */
    if (r0 >= 1) goto L_0800A0E8;   /* unsigned */
    /* 0x0800A0D6: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x0800A0D8: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A0DA: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A0DC: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x0800A0DE: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A0E0: c9d9     bls      #0x800a076 */
    if (r0 <= 0xa) goto L_0800A076;   /* unsigned */
    /* 0x0800A0E2: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x0800A0E4: 2670     strb     r6, [r4] */
    *(uint8_t *)(r4) = r6;   /* store */

    L_0800A0E6:
    /* 0x0800A0E6: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A0E8:
    /* 0x0800A0E8: 134a     ldr      r2, [pc, #0x4c]  ; =0x901D6407 */
    r2 = *(uint32_t *)0x0800A138;   /* = 0x901D6407 */
    /* 0x0800A0EA: 144b     ldr      r3, [pc, #0x50]  ; =0x40764075 */
    r3 = *(uint32_t *)0x0800A13C;   /* = 0x40764075 */
    /* 0x0800A0EC: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x0800A0EE: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800A0F0: f9f780f9 bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x0800A0F4: 08d2     bhs      #0x800a108 */
    if (r0 >= 0xa) goto L_0800A108;   /* unsigned */
    /* 0x0800A0F6: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x0800A0F8: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A0FA: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A0FC: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x0800A0FE: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A100: f1d9     bls      #0x800a0e6 */
    if (r0 <= 0xa) goto L_0800A0E6;   /* unsigned */
    /* 0x0800A102: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x0800A104: 2770     strb     r7, [r4] */
    *(uint8_t *)(r4) = r7;   /* store */
    /* 0x0800A106: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A108:
    /* 0x0800A108: 6571     strb     r5, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r5;   /* store */
    /* 0x0800A10A: 2570     strb     r5, [r4] */
    *(uint8_t *)(r4) = r5;   /* store */
    /* 0x0800A10C: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
