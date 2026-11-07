/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005B0C
 * Name      : IRQ7_Handler
 * End       : 0x08005B82
 * Size      : 118 bytes, 52 instructions decoded in body (functions.json insns=52, blocks=14)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 6  -> 0x08006014 FUN_08006014, 0x080072AC FUN_080072AC, 0x0800741C FUN_0800741C, 0x0800AA74 FUN_0800AA74, 0x0800B7F0 FUN_0800B7F0, 0x0800CA68 FUN_0800CA68
 * MMIO      : EXTI.PR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void IRQ7_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005B0C: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08005B0E: 1d4c     ldr      r4, [pc, #0x74]  ; =EXTI.IMR */
    r4 = *(uint32_t *)0x08005B84;   /* = MMIO EXTI+0x00 */
    /* 0x08005B10: 6069     ldr      r0, [r4, #0x14] */
    r0 = *(uint32_t *)(r4 + 20);  /* MMIO: EXTI.PR(r) */
    /* 0x08005B12: 1d4d     ldr      r5, [pc, #0x74]  ; =RAM_200001D4 */
    r5 = *(uint32_t *)0x08005B88;   /* = RAM 0x200001D4 */
    /* 0x08005B14: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x08005B16: 0cd5     bpl      #0x8005b32 */
    if (/* bpl */) goto L_08005B32;   /* result non-negative */
    /* 0x08005B18: 1c48     ldr      r0, [pc, #0x70]  ; =RAM_2000017C */
    r0 = *(uint32_t *)0x08005B8C;   /* = RAM 0x2000017C */
    /* 0x08005B1A: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005B1C: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005B1E: 04d1     bne      #0x8005b2a */
    if (r0 != 1) goto L_08005B2A;
    /* 0x08005B20: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x08005B22: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005B24: 01d1     bne      #0x8005b2a */
    if (r0 != 0) goto L_08005B2A;
    /* 0x08005B26: 04f0a5ff bl       #0x800aa74 */
    FUN_0800AA74();   /* BL -> 0x0800AA74 FUN_0800AA74 */

    L_08005B2A:
    /* 0x08005B2A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08005B2C: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x08005B2E: 01f0bdfb bl       #0x80072ac */
    FUN_080072AC();   /* BL -> 0x080072AC FUN_080072AC */

    L_08005B32:
    /* 0x08005B32: 6069     ldr      r0, [r4, #0x14] */
    r0 = *(uint32_t *)(r4 + 20);  /* MMIO: EXTI.PR(r) */
    /* 0x08005B34: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;
    /* 0x08005B36: 20d5     bpl      #0x8005b7a */
    if (r0 >= 0) goto L_08005B7A;   /* result non-negative */
    /* 0x08005B38: 2878     ldrb     r0, [r5] */
    r0 = *(uint8_t *)(r5);
    /* 0x08005B3A: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005B3C: 1dd1     bne      #0x8005b7a */
    if (r0 != 0) goto L_08005B7A;
    /* 0x08005B3E: 3220     movs     r0, #0x32 */
    r0 = 0x32;
    /* 0x08005B40: 06f092ff bl       #0x800ca68 */
    FUN_0800CA68();   /* BL -> 0x0800CA68 FUN_0800CA68 */
    /* 0x08005B44: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08005B46: 4021     movs     r1, #0x40 */
    r1 = 0x40;
    /* 0x08005B48: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08005B4A: 01f067fc bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08005B4E: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005B50: 13d1     bne      #0x8005b7a */
    if (r0 != 0) goto L_08005B7A;
    /* 0x08005B52: 0f48     ldr      r0, [pc, #0x3c]  ; =RAM_20000179 */
    r0 = *(uint32_t *)0x08005B90;   /* = RAM 0x20000179 */
    /* 0x08005B54: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08005B56: 0178     ldrb     r1, [r0] */
    r1 = *(uint8_t *)(r0);
    /* 0x08005B58: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08005B5A: 04d0     beq      #0x8005b66 */
    if (r1 == 1) goto L_08005B66;
    /* 0x08005B5C: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_2000017F */
    r0 = *(uint32_t *)0x08005B94;   /* = RAM 0x2000017F */
    /* 0x08005B5E: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005B60: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005B62: 02d0     beq      #0x8005b6a */
    if (r0 == 0) goto L_08005B6A;
    /* 0x08005B64: 09e0     b        #0x8005b7a */
    goto L_08005B7A;

    L_08005B66:
    /* 0x08005B66: 0470     strb     r4, [r0] */
    *(uint8_t *)(r0) = r4;   /* store */
    /* 0x08005B68: 07e0     b        #0x8005b7a */
    goto L_08005B7A;

    L_08005B6A:
    /* 0x08005B6A: 0b4d     ldr      r5, [pc, #0x2c]  ; =RAM_20000182 */
    r5 = *(uint32_t *)0x08005B98;   /* = RAM 0x20000182 */
    /* 0x08005B6C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08005B6E: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x08005B70: 00f050fa bl       #0x8006014 */
    FUN_08006014();   /* BL -> 0x08006014 FUN_08006014 */
    /* 0x08005B74: 2c70     strb     r4, [r5] */
    *(uint8_t *)(r5) = r4;   /* store */
    /* 0x08005B76: 05f03bfe bl       #0x800b7f0 */
    FUN_0800B7F0();   /* BL -> 0x0800B7F0 FUN_0800B7F0 */

    L_08005B7A:
    /* 0x08005B7A: 4020     movs     r0, #0x40 */
    r0 = 0x40;
    /* 0x08005B7C: 01f096fb bl       #0x80072ac */
    FUN_080072AC();   /* BL -> 0x080072AC FUN_080072AC */
    /* 0x08005B80: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
