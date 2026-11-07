/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009AD0
 * Name      : FUN_08009AD0
 * End       : 0x08009B70
 * Size      : 160 bytes, 77 instructions decoded in body (functions.json insns=77, blocks=10)
 * Class     : function
 * Callers   : 1  -> 0x080099B0 FUN_080099B0
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009AD0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009AD0: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x08009AD2: 274c     ldr      r4, [pc, #0x9c]  ; =RAM_20000230 */
    r4 = *(uint32_t *)0x08009B70;   /* = RAM 0x20000230 */
    /* 0x08009AD4: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x08009AD6: e178     ldrb     r1, [r4, #3] */
    r1 = *(uint8_t *)(r4 + 3);
    /* 0x08009AD8: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x08009ADA: 48d1     bne      #0x8009b6e */
    if (/* bne */) goto L_08009B6E;
    /* 0x08009ADC: a078     ldrb     r0, [r4, #2] */
    r0 = *(uint8_t *)(r4 + 2);
    /* 0x08009ADE: 0107     lsls     r1, r0, #0x1c */
    r1 = r0 << 0x1c;
    /* 0x08009AE0: 6078     ldrb     r0, [r4, #1] */
    r0 = *(uint8_t *)(r4 + 1);
    /* 0x08009AE2: 090f     lsrs     r1, r1, #0x1c */
    r1 = r1 >> 0x1c;
    /* 0x08009AE4: 0207     lsls     r2, r0, #0x1c */
    r2 = r0 << 0x1c;
    /* 0x08009AE6: 120f     lsrs     r2, r2, #0x1c */
    r2 = r2 >> 0x1c;
    /* 0x08009AE8: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08009AEA: 40d0     beq      #0x8009b6e */
    if (r1 == r2) goto L_08009B6E;
    /* 0x08009AEC: 0027     movs     r7, #0 */
    r7 = 0;
    /* 0x08009AEE: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08009AF0: 12d2     bhs      #0x8009b18 */
    if (r1 >= r2) goto L_08009B18;   /* unsigned */
    /* 0x08009AF2: 2772     strb     r7, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r7;   /* store */
    /* 0x08009AF4: 1f4b     ldr      r3, [pc, #0x7c]  ; =flash_0800F3D8 */
    r3 = *(uint32_t *)0x08009B74;   /* = flash data/const 0x0800F3D8 */
    /* 0x08009AF6: 0207     lsls     r2, r0, #0x1c */
    r2 = r0 << 0x1c;
    /* 0x08009AF8: 6781     strh     r7, [r4, #0xa] */
    *(uint16_t *)(r4 + 10) = r7;   /* store */
    /* 0x08009AFA: 120f     lsrs     r2, r2, #0x1c */
    r2 = r2 >> 0x1c;

    L_08009AFC:
    /* 0x08009AFC: a178     ldrb     r1, [r4, #2] */
    r1 = *(uint8_t *)(r4 + 2);
    /* 0x08009AFE: 6689     ldrh     r6, [r4, #0xa] */
    r6 = *(uint16_t *)(r4 + 10);
    /* 0x08009B00: 0d07     lsls     r5, r1, #0x1c */
    r5 = r1 << 0x1c;
    /* 0x08009B02: ed0e     lsrs     r5, r5, #0x1b */
    r5 = r5 >> 0x1b;
    /* 0x08009B04: 5d5b     ldrh     r5, [r3, r5] */
    r5 = *(uint16_t *)(r3 + r5);
    /* 0x08009B06: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;
    /* 0x08009B08: ad19     adds     r5, r5, r6 */
    r5 = r5 + r6;
    /* 0x08009B0A: 6581     strh     r5, [r4, #0xa] */
    *(uint16_t *)(r4 + 10) = r5;   /* store */
    /* 0x08009B0C: a170     strb     r1, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r1;   /* store */
    /* 0x08009B0E: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08009B10: 090f     lsrs     r1, r1, #0x1c */
    r1 = r1 >> 0x1c;
    /* 0x08009B12: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08009B14: f2d1     bne      #0x8009afc */
    if (r1 != r2) goto L_08009AFC;
    /* 0x08009B16: 13e0     b        #0x8009b40 */
    goto L_08009B40;

    L_08009B18:
    /* 0x08009B18: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009B1A: 0207     lsls     r2, r0, #0x1c */
    r2 = r0 << 0x1c;
    /* 0x08009B1C: 154b     ldr      r3, [pc, #0x54]  ; =flash_0800F3D8 */
    r3 = *(uint32_t *)0x08009B74;   /* = flash data/const 0x0800F3D8 */
    /* 0x08009B1E: 2172     strb     r1, [r4, #8] */
    *(uint8_t *)(r4 + 8) = r1;   /* store */
    /* 0x08009B20: 120f     lsrs     r2, r2, #0x1c */
    r2 = r2 >> 0x1c;
    /* 0x08009B22: 6781     strh     r7, [r4, #0xa] */
    *(uint16_t *)(r4 + 10) = r7;   /* store */
    /* 0x08009B24: 0a33     adds     r3, #0xa */
    r3 = r3 + 0xa;

    L_08009B26:
    /* 0x08009B26: a178     ldrb     r1, [r4, #2] */
    r1 = *(uint8_t *)(r4 + 2);
    /* 0x08009B28: 6689     ldrh     r6, [r4, #0xa] */
    r6 = *(uint16_t *)(r4 + 10);
    /* 0x08009B2A: 0d07     lsls     r5, r1, #0x1c */
    r5 = r1 << 0x1c;
    /* 0x08009B2C: ed0e     lsrs     r5, r5, #0x1b */
    r5 = r5 >> 0x1b;
    /* 0x08009B2E: 5d5b     ldrh     r5, [r3, r5] */
    r5 = *(uint16_t *)(r3 + r5);
    /* 0x08009B30: 491e     subs     r1, r1, #1 */
    r1 = r1 - 1;
    /* 0x08009B32: ad19     adds     r5, r5, r6 */
    r5 = r5 + r6;
    /* 0x08009B34: 6581     strh     r5, [r4, #0xa] */
    *(uint16_t *)(r4 + 10) = r5;   /* store */
    /* 0x08009B36: a170     strb     r1, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r1;   /* store */
    /* 0x08009B38: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x08009B3A: 090f     lsrs     r1, r1, #0x1c */
    r1 = r1 >> 0x1c;
    /* 0x08009B3C: 9142     cmp      r1, r2 */
    /* flags = cmp(r1, r2) */
    /* 0x08009B3E: f2d1     bne      #0x8009b26 */
    if (r1 != r2) goto L_08009B26;

    L_08009B40:
    /* 0x08009B40: a070     strb     r0, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r0;   /* store */
    /* 0x08009B42: 0d4d     ldr      r5, [pc, #0x34]  ; =GPIOB.MODER */
    r5 = *(uint32_t *)0x08009B78;   /* = MMIO GPIOB+0x00 */
    /* 0x08009B44: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009B46: e771     strb     r7, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r7;   /* store */
    /* 0x08009B48: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009B4A: 8902     lsls     r1, r1, #0xa */
    r1 = r1 << 0xa;
    /* 0x08009B4C: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009B4E: fdf76afc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009B52: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009B54: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009B56: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x08009B58: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009B5A: fdf764fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009B5E: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08009B60: 6071     strb     r0, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r0;   /* store */
    /* 0x08009B62: 0246     mov      r2, r0 */
    r2 = r0;
    /* 0x08009B64: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x08009B66: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08009B68: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08009B6A: fdf75cfc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */

    L_08009B6E:
    /* 0x08009B6E: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
