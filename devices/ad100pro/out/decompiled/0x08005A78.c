/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005A78
 * Name      : IRQ5_Handler
 * End       : 0x08005AE4
 * Size      : 108 bytes, 49 instructions decoded in body (functions.json insns=49, blocks=12)
 * Class     : vector_handler
 * Callers   : 0  -> -
 * Callees   : 3  -> 0x08006014 FUN_08006014, 0x080072AC FUN_080072AC, 0x0800B7F0 FUN_0800B7F0
 * MMIO      : EXTI.PR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void IRQ5_Handler(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005A78: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08005A7A: 1a48     ldr      r0, [pc, #0x68]  ; =EXTI.IMR */
    r0 = *(uint32_t *)0x08005AE4;   /* = MMIO EXTI+0x00 */
    /* 0x08005A7C: 4069     ldr      r0, [r0, #0x14] */
    r0 = *(uint32_t *)(r0 + 20);  /* MMIO: EXTI.PR(r) */
    /* 0x08005A7E: c007     lsls     r0, r0, #0x1f */
    r0 = r0 << 0x1f;
    /* 0x08005A80: 28d0     beq      #0x8005ad4 */
    if (/* beq */) goto L_08005AD4;
    /* 0x08005A82: 1948     ldr      r0, [pc, #0x64]  ; =RAM_200001D4 */
    r0 = *(uint32_t *)0x08005AE8;   /* = RAM 0x200001D4 */
    /* 0x08005A84: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005A86: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005A88: 24d1     bne      #0x8005ad4 */
    if (r0 != 0) goto L_08005AD4;
    /* 0x08005A8A: 1848     ldr      r0, [pc, #0x60]  ; =RAM_20000175 */
    r0 = *(uint32_t *)0x08005AEC;   /* = RAM 0x20000175 */
    /* 0x08005A8C: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08005A8E: 8007     lsls     r0, r0, #0x1e */
    r0 = r0 << 0x1e;
    /* 0x08005A90: 800f     lsrs     r0, r0, #0x1e */
    r0 = r0 >> 0x1e;
    /* 0x08005A92: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08005A94: 1ed1     bne      #0x8005ad4 */
    if (r0 != 1) goto L_08005AD4;
    /* 0x08005A96: 1648     ldr      r0, [pc, #0x58]  ; =RAM_20000480 */
    r0 = *(uint32_t *)0x08005AF0;   /* = RAM 0x20000480 */
    /* 0x08005A98: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x08005A9A: 4178     ldrb     r1, [r0, #1] */
    r1 = *(uint8_t *)(r0 + 1);
    /* 0x08005A9C: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08005A9E: 154d     ldr      r5, [pc, #0x54]  ; =RAM_20000182 */
    r5 = *(uint32_t *)0x08005AF4;   /* = RAM 0x20000182 */
    /* 0x08005AA0: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08005AA2: 02d0     beq      #0x8005aaa */
    if (r1 == 1) goto L_08005AAA;
    /* 0x08005AA4: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x08005AA6: 07d0     beq      #0x8005ab8 */
    if (r1 == 2) goto L_08005AB8;
    /* 0x08005AA8: 14e0     b        #0x8005ad4 */
    goto L_08005AD4;

    L_08005AAA:
    /* 0x08005AAA: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x08005AAC: 00f0b2fa bl       #0x8006014 */
    FUN_08006014();   /* BL -> 0x08006014 FUN_08006014 */
    /* 0x08005AB0: 2e70     strb     r6, [r5] */
    *(uint8_t *)(r5) = r6;   /* store */
    /* 0x08005AB2: 05f09dfe bl       #0x800b7f0 */
    FUN_0800B7F0();   /* BL -> 0x0800B7F0 FUN_0800B7F0 */
    /* 0x08005AB6: 0de0     b        #0x8005ad4 */
    goto L_08005AD4;

    L_08005AB8:
    /* 0x08005AB8: 0f4c     ldr      r4, [pc, #0x3c]  ; =RAM_200001CA */
    r4 = *(uint32_t *)0x08005AF8;   /* = RAM 0x200001CA */
    /* 0x08005ABA: 2188     ldrh     r1, [r4] */
    r1 = *(uint16_t *)(r4);
    /* 0x08005ABC: 0029     cmp      r1, #0 */
    /* flags = cmp(r1, #0) */
    /* 0x08005ABE: 0dd0     beq      #0x8005adc */
    if (r1 == 0) goto L_08005ADC;
    /* 0x08005AC0: 0e4a     ldr      r2, [pc, #0x38]  ; =0x000026AC */
    r2 = *(uint32_t *)0x08005AFC;   /* = 0x000026AC */
    /* 0x08005AC2: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08005AC4: 06d8     bhi      #0x8005ad4 */
    if (r1 > r2) goto L_08005AD4;   /* unsigned */
    /* 0x08005AC6: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
    /* 0x08005AC8: 00f0a4fa bl       #0x8006014 */
    FUN_08006014();   /* BL -> 0x08006014 FUN_08006014 */
    /* 0x08005ACC: 2e70     strb     r6, [r5] */
    *(uint8_t *)(r5) = r6;   /* store */
    /* 0x08005ACE: 05f08ffe bl       #0x800b7f0 */
    FUN_0800B7F0();   /* BL -> 0x0800B7F0 FUN_0800B7F0 */
    /* 0x08005AD2: 2680     strh     r6, [r4] */
    *(uint16_t *)(r4) = r6;   /* store */

    L_08005AD4:
    /* 0x08005AD4: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08005AD6: 01f0e9fb bl       #0x80072ac */
    FUN_080072AC();   /* BL -> 0x080072AC FUN_080072AC */
    /* 0x08005ADA: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */

    L_08005ADC:
    /* 0x08005ADC: 0748     ldr      r0, [pc, #0x1c]  ; =0x000026AC */
    r0 = *(uint32_t *)0x08005AFC;   /* = 0x000026AC */
    /* 0x08005ADE: 6430     adds     r0, #0x64 */
    r0 = r0 + 0x64;
    /* 0x08005AE0: 2080     strh     r0, [r4] */
    *(uint16_t *)(r4) = r0;   /* store */
    /* 0x08005AE2: f7e7     b        #0x8005ad4 */
    goto L_08005AD4;   /* loop */
}
