/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080099EC
 * Name      : FUN_080099EC
 * End       : 0x08009AC6
 * Size      : 218 bytes, 100 instructions decoded in body (functions.json insns=100, blocks=24)
 * Class     : function
 * Callers   : 1  -> 0x080099B0 FUN_080099B0
 * Callees   : 2  -> 0x0800741C FUN_0800741C, 0x08007426 FUN_08007426
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080099EC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080099EC: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x080099EE: 364c     ldr      r4, [pc, #0xd8]  ; =RAM_20000230 */
    r4 = *(uint32_t *)0x08009AC8;   /* = RAM 0x20000230 */
    /* 0x080099F0: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x080099F2: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x080099F4: 354d     ldr      r5, [pc, #0xd4]  ; =GPIOB.MODER */
    r5 = *(uint32_t *)0x08009ACC;   /* = MMIO GPIOB+0x00 */
    /* 0x080099F6: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x080099F8: 5ed1     bne      #0x8009ab8 */
    if (r0 != 1) goto L_08009AB8;
    /* 0x080099FA: 6089     ldrh     r0, [r4, #0xa] */
    r0 = *(uint16_t *)(r4 + 10);
    /* 0x080099FC: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x080099FE: 5bd0     beq      #0x8009ab8 */
    if (r0 == 0) goto L_08009AB8;
    /* 0x08009A00: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009A02: 0920     movs     r0, #9 */
    r0 = 9;
    /* 0x08009A04: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x08009A06: c006     lsls     r0, r0, #0x1b */
    r0 = r0 << 0x1b;
    /* 0x08009A08: fdf70dfd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009A0C: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x08009A0E: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08009A10: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08009A12: e071     strb     r0, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r0;   /* store */
    /* 0x08009A14: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x08009A16: 00d3     blo      #0x8009a1a */
    if (r0 < 4) goto L_08009A1A;   /* unsigned */
    /* 0x08009A18: e671     strb     r6, [r4, #7] */
    *(uint8_t *)(r4 + 7) = r6;   /* store */

    L_08009A1A:
    /* 0x08009A1A: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009A1C: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x08009A1E: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009A20: fdf701fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009A24: 207a     ldrb     r0, [r4, #8] */
    r0 = *(uint8_t *)(r4 + 8);
    /* 0x08009A26: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08009A28: bf02     lsls     r7, r7, #0xa */
    r7 = r7 << 0xa;
    /* 0x08009A2A: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009A2C: 0dd0     beq      #0x8009a4a */
    if (r0 == 1) goto L_08009A4A;
    /* 0x08009A2E: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x08009A30: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009A32: fdf7f3fc bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08009A36: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009A38: 2dd0     beq      #0x8009a96 */
    if (r0 == 0) goto L_08009A96;
    /* 0x08009A3A: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x08009A3C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009A3E: 35d0     beq      #0x8009aac */
    if (r0 == 0) goto L_08009AAC;
    /* 0x08009A40: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009A42: 09d0     beq      #0x8009a58 */
    if (r0 == 1) goto L_08009A58;
    /* 0x08009A44: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08009A46: 33d0     beq      #0x8009ab0 */
    if (r0 == 2) goto L_08009AB0;
    /* 0x08009A48: 34e0     b        #0x8009ab4 */
    goto L_08009AB4;

    L_08009A4A:
    /* 0x08009A4A: e079     ldrb     r0, [r4, #7] */
    r0 = *(uint8_t *)(r4 + 7);
    /* 0x08009A4C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009A4E: 2dd0     beq      #0x8009aac */
    if (r0 == 0) goto L_08009AAC;
    /* 0x08009A50: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08009A52: 2fd0     beq      #0x8009ab4 */
    if (r0 == 1) goto L_08009AB4;
    /* 0x08009A54: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x08009A56: 2bd0     beq      #0x8009ab0 */
    if (r0 == 2) goto L_08009AB0;

    L_08009A58:
    /* 0x08009A58: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009A5A: 05e0     b        #0x8009a68 */
    goto L_08009A68;

    L_08009A5C:
    /* 0x08009A5C: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x08009A5E: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009A60: fdf7e1fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009A64: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009A66: 04e0     b        #0x8009a72 */
    goto L_08009A72;

    L_08009A68:
    /* 0x08009A68: 3946     mov      r1, r7 */
    r1 = r7;
    /* 0x08009A6A: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009A6C: fdf7dbfc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009A70: 0122     movs     r2, #1 */
    r2 = 1;

    L_08009A72:
    /* 0x08009A72: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009A74: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x08009A76: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009A78: fdf7d5fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009A7C: 6089     ldrh     r0, [r4, #0xa] */
    r0 = *(uint16_t *)(r4 + 10);
    /* 0x08009A7E: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08009A80: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x08009A82: 000c     lsrs     r0, r0, #0x10 */
    r0 = r0 >> 0x10;
    /* 0x08009A84: 6081     strh     r0, [r4, #0xa] */
    *(uint16_t *)(r4 + 10) = r0;   /* store */
    /* 0x08009A86: 05d1     bne      #0x8009a94 */
    if (r0 != 2) goto L_08009A94;
    /* 0x08009A88: 6671     strb     r6, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r6;   /* store */
    /* 0x08009A8A: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009A8C: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x08009A8E: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009A90: fdf7c9fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */

    L_08009A94:
    /* 0x08009A94: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08009A96:
    /* 0x08009A96: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08009A98: a070     strb     r0, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r0;   /* store */
    /* 0x08009A9A: 6681     strh     r6, [r4, #0xa] */
    *(uint16_t *)(r4 + 10) = r6;   /* store */
    /* 0x08009A9C: 0146     mov      r1, r0 */
    r1 = r0;
    /* 0x08009A9E: 6671     strb     r6, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r6;   /* store */
    /* 0x08009AA0: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009AA2: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009AA4: fdf7bffc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009AA8: e670     strb     r6, [r4, #3] */
    *(uint8_t *)(r4 + 3) = r6;   /* store */
    /* 0x08009AAA: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_08009AAC:
    /* 0x08009AAC: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009AAE: d5e7     b        #0x8009a5c */
    goto L_08009A5C;   /* loop */

    L_08009AB0:
    /* 0x08009AB0: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009AB2: d9e7     b        #0x8009a68 */
    goto L_08009A68;   /* loop */

    L_08009AB4:
    /* 0x08009AB4: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x08009AB6: d1e7     b        #0x8009a5c */
    goto L_08009A5C;   /* loop */

    L_08009AB8:
    /* 0x08009AB8: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x08009ABA: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x08009ABC: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08009ABE: fdf7b2fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08009AC2: 6671     strb     r6, [r4, #5] */
    *(uint8_t *)(r4 + 5) = r6;   /* store */
    /* 0x08009AC4: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
}
