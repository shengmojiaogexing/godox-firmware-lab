/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A140
 * Name      : FUN_0800A140
 * End       : 0x0800A1FE
 * Size      : 190 bytes, 90 instructions decoded in body (functions.json insns=90, blocks=20)
 * Class     : function
 * Callers   : 3  -> 0x0800A230 FUN_0800A230, 0x0800C080 FUN_0800C080, 0x0800C12C FUN_0800C12C
 * Callees   : 2  -> 0x0800339C FUN_0800339C, 0x080033F4 FUN_080033F4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A140(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A140: feb5     push     {r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x0800A142: 2f48     ldr      r0, [pc, #0xbc]  ; =RAM_2000020C */
    r0 = *(uint32_t *)0x0800A200;   /* = RAM 0x2000020C */
    /* 0x0800A144: 2f4a     ldr      r2, [pc, #0xbc]  ; =0x000003B6 */
    r2 = *(uint32_t *)0x0800A204;   /* = 0x000003B6 */
    /* 0x0800A146: 0088     ldrh     r0, [r0] */
    r0 = *(uint16_t *)(r0);
    /* 0x0800A148: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x0800A14A: 2f49     ldr      r1, [pc, #0xbc]  ; =RAM_200001EE */
    r1 = *(uint32_t *)0x0800A208;   /* = RAM 0x200001EE */
    /* 0x0800A14C: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x0800A14E: 2f4c     ldr      r4, [pc, #0xbc]  ; =RAM_2000021C */
    r4 = *(uint32_t *)0x0800A20C;   /* = RAM 0x2000021C */
    /* 0x0800A150: 9042     cmp      r0, r2 */
    /* flags = cmp(r0, r2) */
    /* 0x0800A152: 09d9     bls      #0x800a168 */
    if (r0 <= r2) goto L_0800A168;   /* unsigned */
    /* 0x0800A154: a079     ldrb     r0, [r4, #6] */
    r0 = *(uint8_t *)(r4 + 6);
    /* 0x0800A156: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A158: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A15A: a071     strb     r0, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r0;   /* store */
    /* 0x0800A15C: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A15E: 02d9     bls      #0x800a166 */
    if (r0 <= 0xa) goto L_0800A166;   /* unsigned */
    /* 0x0800A160: e672     strb     r6, [r4, #0xb] */
    *(uint8_t *)(r4 + 11) = r6;   /* store */
    /* 0x0800A162: 6670     strb     r6, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r6;   /* store */
    /* 0x0800A164: 0f70     strb     r7, [r1] */
    *(uint8_t *)(r1) = r7;   /* store */

    L_0800A166:
    /* 0x0800A166: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A168:
    /* 0x0800A168: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x0800A16A: 1428     cmp      r0, #0x14 */
    /* flags = cmp(r0, #0x14) */
    /* 0x0800A16C: 0ad2     bhs      #0x800a184 */
    if (r0 >= 0x14) goto L_0800A184;   /* unsigned */
    /* 0x0800A16E: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x0800A170: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A172: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A174: e071     strb     r0, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r0;   /* store */
    /* 0x0800A176: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A178: f5d9     bls      #0x800a166 */
    if (r0 <= 0xa) goto L_0800A166;   /* unsigned */
    /* 0x0800A17A: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x0800A17C: e072     strb     r0, [r4, #0xb] */
    *(uint8_t *)(r4 + 11) = r0;   /* store */
    /* 0x0800A17E: 6670     strb     r6, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r6;   /* store */
    /* 0x0800A180: 0e70     strb     r6, [r1] */
    *(uint8_t *)(r1) = r6;   /* store */

    L_0800A182:
    /* 0x0800A182: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A184:
    /* 0x0800A184: f9f70af9 bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x0800A188: 214a     ldr      r2, [pc, #0x84]  ; =0xFDAFFC4D */
    r2 = *(uint32_t *)0x0800A210;   /* = 0xFDAFFC4D */
    /* 0x0800A18A: 224b     ldr      r3, [pc, #0x88]  ; =0x4058BE8D */
    r3 = *(uint32_t *)0x0800A214;   /* = 0x4058BE8D */
    /* 0x0800A18C: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x0800A18E: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x0800A190: f9f730f9 bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x0800A194: 08d2     bhs      #0x800a1a8 */
    if (r0 >= 0xa) goto L_0800A1A8;   /* unsigned */
    /* 0x0800A196: a571     strb     r5, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r5;   /* store */
    /* 0x0800A198: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x0800A19A: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A19C: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A19E: e071     strb     r0, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r0;   /* store */
    /* 0x0800A1A0: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A1A2: 09d9     bls      #0x800a1b8 */
    if (r0 <= 0xa) goto L_0800A1B8;   /* unsigned */
    /* 0x0800A1A4: e772     strb     r7, [r4, #0xb] */
    *(uint8_t *)(r4 + 11) = r7;   /* store */
    /* 0x0800A1A6: 07e0     b        #0x800a1b8 */
    goto L_0800A1B8;

    L_0800A1A8:
    /* 0x0800A1A8: 1b4a     ldr      r2, [pc, #0x6c]  ; =0x97AC5FED */
    r2 = *(uint32_t *)0x0800A218;   /* = 0x97AC5FED */
    /* 0x0800A1AA: 1c4b     ldr      r3, [pc, #0x70]  ; =0x4069849C */
    r3 = *(uint32_t *)0x0800A21C;   /* = 0x4069849C */
    /* 0x0800A1AC: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x0800A1AE: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800A1B0: f9f720f9 bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x0800A1B4: a571     strb     r5, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r5;   /* store */
    /* 0x0800A1B6: e571     strb     r5, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r5;   /* store */

    L_0800A1B8:
    /* 0x0800A1B8: 194a     ldr      r2, [pc, #0x64]  ; =0xF0F0F0F1 */
    r2 = *(uint32_t *)0x0800A220;   /* = 0xF0F0F0F1 */
    /* 0x0800A1BA: 1a4b     ldr      r3, [pc, #0x68]  ; =0x4070F0F0 */
    r3 = *(uint32_t *)0x0800A224;   /* = 0x4070F0F0 */
    /* 0x0800A1BC: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x0800A1BE: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800A1C0: f9f718f9 bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x0800A1C4: 08d2     bhs      #0x800a1d8 */
    if (r0 >= 0xa) goto L_0800A1D8;   /* unsigned */
    /* 0x0800A1C6: 207a     ldrb     r0, [r4, #8] */
    r0 = *(uint8_t *)(r4 + 8);
    /* 0x0800A1C8: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A1CA: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A1CC: 2072     strb     r0, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r0;   /* store */
    /* 0x0800A1CE: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A1D0: d7d9     bls      #0x800a182 */
    if (r0 <= 0xa) goto L_0800A182;   /* unsigned */
    /* 0x0800A1D2: 2572     strb     r5, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r5;   /* store */
    /* 0x0800A1D4: 6670     strb     r6, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r6;   /* store */

    L_0800A1D6:
    /* 0x0800A1D6: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A1D8:
    /* 0x0800A1D8: 134a     ldr      r2, [pc, #0x4c]  ; =0x901D6407 */
    r2 = *(uint32_t *)0x0800A228;   /* = 0x901D6407 */
    /* 0x0800A1DA: 144b     ldr      r3, [pc, #0x50]  ; =0x40764075 */
    r3 = *(uint32_t *)0x0800A22C;   /* = 0x40764075 */
    /* 0x0800A1DC: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x0800A1DE: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x0800A1E0: f9f708f9 bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x0800A1E4: 08d2     bhs      #0x800a1f8 */
    if (r0 >= 0xa) goto L_0800A1F8;   /* unsigned */
    /* 0x0800A1E6: 207a     ldrb     r0, [r4, #8] */
    r0 = *(uint8_t *)(r4 + 8);
    /* 0x0800A1E8: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x0800A1EA: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800A1EC: 2072     strb     r0, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r0;   /* store */
    /* 0x0800A1EE: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800A1F0: f1d9     bls      #0x800a1d6 */
    if (r0 <= 0xa) goto L_0800A1D6;   /* unsigned */
    /* 0x0800A1F2: 2572     strb     r5, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r5;   /* store */
    /* 0x0800A1F4: 6770     strb     r7, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r7;   /* store */
    /* 0x0800A1F6: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A1F8:
    /* 0x0800A1F8: 2572     strb     r5, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r5;   /* store */
    /* 0x0800A1FA: 6570     strb     r5, [r4, #1] */
    *(uint8_t *)(r4 + 1) = r5;   /* store */
    /* 0x0800A1FC: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
