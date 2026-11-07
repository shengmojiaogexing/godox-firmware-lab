/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003AC8
 * Name      : FUN_08003AC8
 * End       : 0x08003B80
 * Size      : 184 bytes, 87 instructions decoded in body (functions.json insns=87, blocks=21)
 * Class     : function
 * Callers   : 2  -> 0x0800B9C0 FUN_0800B9C0, 0x0800B9CC FUN_0800B9CC
 * Callees   : 5  -> 0x0800339C FUN_0800339C, 0x080033F4 FUN_080033F4, 0x0800341C FUN_0800341C, 0x080035A8 FUN_080035A8, 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003AC8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003AC8: feb5     push     {r1, r2, r3, r4, r5, r6, r7, lr} */
    push({r1, r2, r3, r4, r5, r6, r7, lr});
    /* 0x08003ACA: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08003ACC: 2c4f     ldr      r7, [pc, #0xb0]  ; =0x00002710 */
    r7 = *(uint32_t *)0x08003B80;   /* = 0x00002710 */
    /* 0x08003ACE: 2d4e     ldr      r6, [pc, #0xb4]  ; =RAM_200001F2 */
    r6 = *(uint32_t *)0x08003B84;   /* = RAM 0x200001F2 */
    /* 0x08003AD0: 2d4c     ldr      r4, [pc, #0xb4]  ; =RAM_20000116 */
    r4 = *(uint32_t *)0x08003B88;   /* = RAM 0x20000116 */

    L_08003AD2:
    /* 0x08003AD2: fff769fd bl       #0x80035a8 */
    FUN_080035A8();   /* BL -> 0x080035A8 FUN_080035A8 */
    /* 0x08003AD6: 3846     mov      r0, r7 */
    r0 = r7;
    /* 0x08003AD8: 08f0aeff bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08003ADC: 6900     lsls     r1, r5, #1 */
    r1 = r5 << 1;
    /* 0x08003ADE: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;
    /* 0x08003AE0: 3088     ldrh     r0, [r6] */
    r0 = *(uint16_t *)(r6);
    /* 0x08003AE2: edb2     uxtb     r5, r5 */
    r5 = uxtb(r5);
    /* 0x08003AE4: 6052     strh     r0, [r4, r1] */
    *(uint16_t *)(r4 + r1) = r0;   /* store */
    /* 0x08003AE6: 042d     cmp      r5, #4 */
    /* flags = cmp(r5, #4) */
    /* 0x08003AE8: f3d3     blo      #0x8003ad2 */
    if (r5 < 4) goto L_08003AD2;   /* unsigned */
    /* 0x08003AEA: 0021     movs     r1, #0 */
    r1 = 0;

    L_08003AEC:
    /* 0x08003AEC: 0020     movs     r0, #0 */
    r0 = 0;

    L_08003AEE:
    /* 0x08003AEE: 4300     lsls     r3, r0, #1 */
    r3 = r0 << 1;
    /* 0x08003AF0: 1d19     adds     r5, r3, r4 */
    r5 = r3 + r4;
    /* 0x08003AF2: e65a     ldrh     r6, [r4, r3] */
    r6 = *(uint16_t *)(r4 + r3);
    /* 0x08003AF4: 6a88     ldrh     r2, [r5, #2] */
    r2 = *(uint16_t *)(r5 + 2);
    /* 0x08003AF6: 9642     cmp      r6, r2 */
    /* flags = cmp(r6, r2) */
    /* 0x08003AF8: 01d9     bls      #0x8003afe */
    if (r6 <= r2) goto L_08003AFE;   /* unsigned */
    /* 0x08003AFA: 6e80     strh     r6, [r5, #2] */
    *(uint16_t *)(r5 + 2) = r6;   /* store */
    /* 0x08003AFC: e252     strh     r2, [r4, r3] */
    *(uint16_t *)(r4 + r3) = r2;   /* store */

    L_08003AFE:
    /* 0x08003AFE: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08003B00: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08003B02: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08003B04: f3d3     blo      #0x8003aee */
    if (r0 < 3) goto L_08003AEE;   /* unsigned */
    /* 0x08003B06: 204e     ldr      r6, [pc, #0x80]  ; =RAM_20000116 */
    r6 = *(uint32_t *)0x08003B88;   /* = RAM 0x20000116 */
    /* 0x08003B08: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08003B0A: c9b2     uxtb     r1, r1 */
    r1 = uxtb(r1);
    /* 0x08003B0C: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08003B0E: 0c3e     subs     r6, #0xc */
    r6 = r6 - 0xc;
    /* 0x08003B10: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x08003B12: 0429     cmp      r1, #4 */
    /* flags = cmp(r1, #4) */
    /* 0x08003B14: ead3     blo      #0x8003aec */
    if (r1 < 4) goto L_08003AEC;   /* unsigned */
    /* 0x08003B16: 6088     ldrh     r0, [r4, #2] */
    r0 = *(uint16_t *)(r4 + 2);
    /* 0x08003B18: a188     ldrh     r1, [r4, #4] */
    r1 = *(uint16_t *)(r4 + 4);
    /* 0x08003B1A: 4018     adds     r0, r0, r1 */
    r0 = r0 + r1;
    /* 0x08003B1C: 4408     lsrs     r4, r0, #1 */
    r4 = r0 >> 1;
    /* 0x08003B1E: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x08003B20: fff73cfc bl       #0x800339c */
    FUN_0800339C();   /* BL -> 0x0800339C FUN_0800339C */
    /* 0x08003B24: 194a     ldr      r2, [pc, #0x64]  ; =0x66666666 */
    r2 = *(uint32_t *)0x08003B8C;   /* = 0x66666666 */
    /* 0x08003B26: 1a4b     ldr      r3, [pc, #0x68]  ; =0x407F8666 */
    r3 = *(uint32_t *)0x08003B90;   /* = 0x407F8666 */
    /* 0x08003B28: 0191     str      r1, [sp, #4] */
    *(uint32_t *)(sp + 4) = r1;   /* store */
    /* 0x08003B2A: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08003B2C: fff762fc bl       #0x80033f4 */
    FUN_080033F4();   /* BL -> 0x080033F4 FUN_080033F4 */
    /* 0x08003B30: 02d2     bhs      #0x8003b38 */
    if (r1 >= 4) goto L_08003B38;   /* unsigned */
    /* 0x08003B32: 3570     strb     r5, [r6] */
    *(uint8_t *)(r6) = r5;   /* store */
    /* 0x08003B34: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_200001CD */
    r0 = *(uint32_t *)0x08003B94;   /* = RAM 0x200001CD */
    /* 0x08003B36: 09e0     b        #0x8003b4c */
    goto L_08003B4C;

    L_08003B38:
    /* 0x08003B38: 1749     ldr      r1, [pc, #0x5c]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003B98;   /* = 0x00000217 */
    /* 0x08003B3A: 1848     ldr      r0, [pc, #0x60]  ; =RAM_200001CE */
    r0 = *(uint32_t *)0x08003B9C;   /* = RAM 0x200001CE */
    /* 0x08003B3C: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x08003B3E: 01d2     bhs      #0x8003b44 */
    if (r4 >= r1) goto L_08003B44;   /* unsigned */
    /* 0x08003B40: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */
    /* 0x08003B42: 0de0     b        #0x8003b60 */
    goto L_08003B60;

    L_08003B44:
    /* 0x08003B44: 1449     ldr      r1, [pc, #0x50]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003B98;   /* = 0x00000217 */
    /* 0x08003B46: 1731     adds     r1, #0x17 */
    r1 = r1 + 0x17;
    /* 0x08003B48: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x08003B4A: 01d2     bhs      #0x8003b50 */
    if (r4 >= r1) goto L_08003B50;   /* unsigned */

    L_08003B4C:
    /* 0x08003B4C: 0770     strb     r7, [r0] */
    *(uint8_t *)(r0) = r7;   /* store */
    /* 0x08003B4E: 07e0     b        #0x8003b60 */
    goto L_08003B60;

    L_08003B50:
    /* 0x08003B50: 1149     ldr      r1, [pc, #0x44]  ; =0x00000217 */
    r1 = *(uint32_t *)0x08003B98;   /* = 0x00000217 */
    /* 0x08003B52: 3631     adds     r1, #0x36 */
    r1 = r1 + 0x36;
    /* 0x08003B54: 8c42     cmp      r4, r1 */
    /* flags = cmp(r4, r1) */
    /* 0x08003B56: 01d2     bhs      #0x8003b5c */
    if (r4 >= r1) goto L_08003B5C;   /* unsigned */
    /* 0x08003B58: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x08003B5A: 00e0     b        #0x8003b5e */
    goto L_08003B5E;

    L_08003B5C:
    /* 0x08003B5C: 0321     movs     r1, #3 */
    r1 = 3;

    L_08003B5E:
    /* 0x08003B5E: 0170     strb     r1, [r0] */
    *(uint8_t *)(r0) = r1;   /* store */

    L_08003B60:
    /* 0x08003B60: 0f4a     ldr      r2, [pc, #0x3c]  ; =0xA3D70A3D */
    r2 = *(uint32_t *)0x08003BA0;   /* = 0xA3D70A3D */
    /* 0x08003B62: 104b     ldr      r3, [pc, #0x40]  ; =0x40807D70 */
    r3 = *(uint32_t *)0x08003BA4;   /* = 0x40807D70 */
    /* 0x08003B64: 0199     ldr      r1, [sp, #4] */
    r1 = *(uint32_t *)(sp + 4);
    /* 0x08003B66: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
    /* 0x08003B68: fff758fc bl       #0x800341c */
    FUN_0800341C();   /* BL -> 0x0800341C FUN_0800341C */
    /* 0x08003B6C: 01d2     bhs      #0x8003b72 */
    if (r4 >= r1) goto L_08003B72;   /* unsigned */
    /* 0x08003B6E: 0948     ldr      r0, [pc, #0x24]  ; =RAM_200001CD */
    r0 = *(uint32_t *)0x08003B94;   /* = RAM 0x200001CD */
    /* 0x08003B70: 0570     strb     r5, [r0] */
    *(uint8_t *)(r0) = r5;   /* store */

    L_08003B72:
    /* 0x08003B72: 3570     strb     r5, [r6] */
    *(uint8_t *)(r6) = r5;   /* store */
    /* 0x08003B74: b570     strb     r5, [r6, #2] */
    *(uint8_t *)(r6 + 2) = r5;   /* store */
    /* 0x08003B76: 0c49     ldr      r1, [pc, #0x30]  ; =RAM_200001CC */
    r1 = *(uint32_t *)0x08003BA8;   /* = RAM 0x200001CC */
    /* 0x08003B78: f570     strb     r5, [r6, #3] */
    *(uint8_t *)(r6 + 3) = r5;   /* store */
    /* 0x08003B7A: 0620     movs     r0, #6 */
    r0 = 6;
    /* 0x08003B7C: 0870     strb     r0, [r1] */
    *(uint8_t *)(r1) = r0;   /* store */
    /* 0x08003B7E: febd     pop      {r1, r2, r3, r4, r5, r6, r7, pc} */
    pop({r1, r2, r3, r4, r5, r6, r7, pc});   /* return */
}
