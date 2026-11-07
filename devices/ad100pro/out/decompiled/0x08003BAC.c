/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003BAC
 * Name      : FUN_08003BAC
 * End       : 0x08003C1E
 * Size      : 114 bytes, 56 instructions decoded in body (functions.json insns=56, blocks=16)
 * Class     : function
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 1  -> 0x08007426 FUN_08007426
 * MMIO      : GPIOC.MODER
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003BAC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003BAC: 70b5     push     {r4, r5, r6, lr} */
    push({r4, r5, r6, lr});
    /* 0x08003BAE: 1c48     ldr      r0, [pc, #0x70]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08003C20;   /* = MMIO GPIOC+0x00 */
    /* 0x08003BB0: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: GPIOC.MODER(r) */
    /* 0x08003BB2: 0204     lsls     r2, r0, #0x10 */
    r2 = r0 << 0x10;
    /* 0x08003BB4: 9143     bics     r1, r2 */
    r1 = r1 & ~r2;
    /* 0x08003BB6: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: GPIOC.MODER(w) */
    /* 0x08003BB8: 0168     ldr      r1, [r0] */
    r1 = *(uint32_t *)(r0);  /* MMIO: GPIOC.MODER(r) */
    /* 0x08003BBA: c203     lsls     r2, r0, #0xf */
    r2 = r0 << 0xf;
    /* 0x08003BBC: 1143     orrs     r1, r2 */
    r1 = r1 | r2;
    /* 0x08003BBE: 0160     str      r1, [r0] */
    *(uint32_t *)(r0) = r1;   /* store */  /* MMIO: GPIOC.MODER(w) */
    /* 0x08003BC0: 1849     ldr      r1, [pc, #0x60]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x08003C24;   /* = RAM 0x20000480 */
    /* 0x08003BC2: 0023     movs     r3, #0 */
    r3 = 0;
    /* 0x08003BC4: 0c78     ldrb     r4, [r1] */
    r4 = *(uint8_t *)(r1);
    /* 0x08003BC6: 5113     asrs     r1, r2, #0xd */
    r1 = r2 >> 0xd;
    /* 0x08003BC8: 174a     ldr      r2, [pc, #0x5c]  ; =RAM_200001E0 */
    r2 = *(uint32_t *)0x08003C28;   /* = RAM 0x200001E0 */
    /* 0x08003BCA: 012c     cmp      r4, #1 */
    /* flags = cmp(r4, #1) */
    /* 0x08003BCC: 06d0     beq      #0x8003bdc */
    if (r4 == 1) goto L_08003BDC;
    /* 0x08003BCE: 174c     ldr      r4, [pc, #0x5c]  ; =RAM_200001DC */
    r4 = *(uint32_t *)0x08003C2C;   /* = RAM 0x200001DC */
    /* 0x08003BD0: 2488     ldrh     r4, [r4] */
    r4 = *(uint16_t *)(r4);
    /* 0x08003BD2: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x08003BD4: 02d1     bne      #0x8003bdc */
    if (r4 != 0) goto L_08003BDC;
    /* 0x08003BD6: 9478     ldrb     r4, [r2, #2] */
    r4 = *(uint8_t *)(r2 + 2);
    /* 0x08003BD8: 012c     cmp      r4, #1 */
    /* flags = cmp(r4, #1) */
    /* 0x08003BDA: 1ad1     bne      #0x8003c12 */
    if (r4 != 1) goto L_08003C12;

    L_08003BDC:
    /* 0x08003BDC: 947a     ldrb     r4, [r2, #0xa] */
    r4 = *(uint8_t *)(r2 + 10);
    /* 0x08003BDE: 012c     cmp      r4, #1 */
    /* flags = cmp(r4, #1) */
    /* 0x08003BE0: 18d1     bne      #0x8003c14 */
    if (r4 != 1) goto L_08003C14;
    /* 0x08003BE2: 547a     ldrb     r4, [r2, #9] */
    r4 = *(uint8_t *)(r2 + 9);
    /* 0x08003BE4: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x08003BE6: 14d0     beq      #0x8003c12 */
    if (r4 == 0) goto L_08003C12;
    /* 0x08003BE8: 958c     ldrh     r5, [r2, #0x24] */
    r5 = *(uint16_t *)(r2 + 36);
    /* 0x08003BEA: 167a     ldrb     r6, [r2, #8] */
    r6 = *(uint8_t *)(r2 + 8);
    /* 0x08003BEC: b542     cmp      r5, r6 */
    /* flags = cmp(r5, r6) */
    /* 0x08003BEE: 02d8     bhi      #0x8003bf6 */
    if (r5 > r6) goto L_08003BF6;   /* unsigned */
    /* 0x08003BF0: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x08003BF2: d672     strb     r6, [r2, #0xb] */
    *(uint8_t *)(r2 + 11) = r6;   /* store */
    /* 0x08003BF4: 00e0     b        #0x8003bf8 */
    goto L_08003BF8;

    L_08003BF6:
    /* 0x08003BF6: d372     strb     r3, [r2, #0xb] */
    *(uint8_t *)(r2 + 11) = r3;   /* store */

    L_08003BF8:
    /* 0x08003BF8: 6d1c     adds     r5, r5, #1 */
    r5 = r5 + 1;
    /* 0x08003BFA: adb2     uxth     r5, r5 */
    r5 = uxth(r5);
    /* 0x08003BFC: 9584     strh     r5, [r2, #0x24] */
    *(uint16_t *)(r2 + 36) = r5;   /* store */
    /* 0x08003BFE: 568c     ldrh     r6, [r2, #0x22] */
    r6 = *(uint16_t *)(r2 + 34);
    /* 0x08003C00: b542     cmp      r5, r6 */
    /* flags = cmp(r5, r6) */
    /* 0x08003C02: 02d3     blo      #0x8003c0a */
    if (r5 < r6) goto L_08003C0A;   /* unsigned */
    /* 0x08003C04: 9384     strh     r3, [r2, #0x24] */
    *(uint16_t *)(r2 + 36) = r3;   /* store */
    /* 0x08003C06: 641e     subs     r4, r4, #1 */
    r4 = r4 - 1;
    /* 0x08003C08: 5472     strb     r4, [r2, #9] */
    *(uint8_t *)(r2 + 9) = r4;   /* store */

    L_08003C0A:
    /* 0x08003C0A: d27a     ldrb     r2, [r2, #0xb] */
    r2 = *(uint8_t *)(r2 + 11);
    /* 0x08003C0C: 012a     cmp      r2, #1 */
    /* flags = cmp(r2, #1) */
    /* 0x08003C0E: 03d0     beq      #0x8003c18 */
    if (r2 == 1) goto L_08003C18;
    /* 0x08003C10: 01e0     b        #0x8003c16 */
    goto L_08003C16;

    L_08003C12:
    /* 0x08003C12: 9372     strb     r3, [r2, #0xa] */
    *(uint8_t *)(r2 + 10) = r3;   /* store */

    L_08003C14:
    /* 0x08003C14: d372     strb     r3, [r2, #0xb] */
    *(uint8_t *)(r2 + 11) = r3;   /* store */

    L_08003C16:
    /* 0x08003C16: 0022     movs     r2, #0 */
    r2 = 0;

    L_08003C18:
    /* 0x08003C18: 03f005fc bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x08003C1C: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
}
