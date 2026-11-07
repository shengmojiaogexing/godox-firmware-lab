/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A934
 * Name      : FUN_0800A934
 * End       : 0x0800AA3A
 * Size      : 262 bytes, 108 instructions decoded in body (functions.json insns=108, blocks=13)
 * Class     : function
 * Callers   : 4  -> 0x0800A91C FUN_0800A91C, 0x0800B814 FUN_0800B814, 0x0800CF74 main, 0x0800CFF2 FUN_0800CFF2
 * Callees   : 6  -> 0x0800741C FUN_0800741C, 0x08007426 FUN_08007426, 0x080074F0 FUN_080074F0, 0x0800B9C0 FUN_0800B9C0, 0x0800C230 FUN_0800C230, 0x0800CA98 FUN_0800CA98
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A934(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A934: f8b5     push     {r3, r4, r5, r6, r7, lr} */
    push({r3, r4, r5, r6, r7, lr});
    /* 0x0800A936: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A938: 81f31088 msr      primask, r1 */
    /* TODO: msr primask, r1 */
    /* 0x0800A93C: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x0800A93E: 3f4f     ldr      r7, [pc, #0xfc]  ; =RAM_200001E0 */
    r7 = *(uint32_t *)0x0800AA3C;   /* = RAM 0x200001E0 */
    /* 0x0800A940: 3f4d     ldr      r5, [pc, #0xfc]  ; =GPIOC.MODER */
    r5 = *(uint32_t *)0x0800AA40;   /* = MMIO GPIOC+0x00 */
    /* 0x0800A942: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A944: 47d0     beq      #0x800a9d6 */
    if (r0 == 1) goto L_0800A9D6;
    /* 0x0800A946: 3e71     strb     r6, [r7, #4] */
    *(uint8_t *)(r7 + 4) = r6;   /* store */

    L_0800A948:
    /* 0x0800A948: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x0800A94A: 80f31088 msr      primask, r0 */
    /* TODO: msr primask, r0 */
    /* 0x0800A94E: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A950: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x0800A952: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A954: fcf767fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A958: 0924     movs     r4, #9 */
    r4 = 9;
    /* 0x0800A95A: e406     lsls     r4, r4, #0x1b */
    r4 = r4 << 0x1b;
    /* 0x0800A95C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A95E: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x0800A960: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A962: fcf760fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A966: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A968: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A96A: 4902     lsls     r1, r1, #9 */
    r1 = r1 << 9;
    /* 0x0800A96C: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A96E: fcf75afd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A972: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A974: 1146     mov      r1, r2 */
    r1 = r2;
    /* 0x0800A976: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A978: fcf755fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A97C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A97E: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x0800A980: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A982: fcf750fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A986: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A988: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A98A: 2e48     ldr      r0, [pc, #0xb8]  ; =GPIOF.MODER */
    r0 = *(uint32_t *)0x0800AA44;   /* = MMIO GPIOF+0x00 */
    /* 0x0800A98C: fcf74bfd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A990: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A992: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x0800A994: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A996: fcf746fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A99A: 2b4c     ldr      r4, [pc, #0xac]  ; =GPIOB.MODER */
    r4 = *(uint32_t *)0x0800AA48;   /* = MMIO GPIOB+0x00 */
    /* 0x0800A99C: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A99E: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x0800A9A0: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A9A2: fcf740fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A9A6: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A9A8: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A9AA: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x0800A9AC: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A9AE: fcf73afd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A9B2: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x0800A9B4: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800A9B6: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x0800A9B8: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A9BA: fcf734fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A9BE: 0a24     movs     r4, #0xa */
    r4 = 0xa;

    L_0800A9C0:
    /* 0x0800A9C0: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800A9C2: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A9C4: fcf72afd bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800A9C8: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800A9CA: 13d0     beq      #0x800a9f4 */
    if (r0 == 1) goto L_0800A9F4;
    /* 0x0800A9CC: 0a24     movs     r4, #0xa */
    r4 = 0xa;
    /* 0x0800A9CE: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800A9D0: 02f062f8 bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800A9D4: f4e7     b        #0x800a9c0 */
    goto L_0800A9C0;   /* loop */

    L_0800A9D6:
    /* 0x0800A9D6: 0122     movs     r2, #1 */
    r2 = 1;
    /* 0x0800A9D8: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x0800A9DA: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800A9DC: fcf723fd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800A9E0: 01f026fc bl       #0x800c230 */
    FUN_0800C230();   /* BL -> 0x0800C230 FUN_0800C230 */
    /* 0x0800A9E4: 3879     ldrb     r0, [r7, #4] */
    r0 = *(uint8_t *)(r7 + 4);
    /* 0x0800A9E6: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800A9E8: aed0     beq      #0x800a948 */
    if (r0 == 0) goto L_0800A948;
    /* 0x0800A9EA: 00f0e9ff bl       #0x800b9c0 */
    FUN_0800B9C0();   /* BL -> 0x0800B9C0 FUN_0800B9C0 */
    /* 0x0800A9EE: 1748     ldr      r0, [pc, #0x5c]  ; =RAM_200001D4 */
    r0 = *(uint32_t *)0x0800AA4C;   /* = RAM 0x200001D4 */
    /* 0x0800A9F0: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */
    /* 0x0800A9F2: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */

    L_0800A9F4:
    /* 0x0800A9F4: 641e     subs     r4, r4, #1 */
    r4 = r4 - 1;
    /* 0x0800A9F6: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x0800A9F8: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800A9FA: 02f04df8 bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800A9FE: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x0800AA00: ded1     bne      #0x800a9c0 */
    if (r4 != 0) goto L_0800A9C0;
    /* 0x0800AA02: 0022     movs     r2, #0 */
    r2 = 0;
    /* 0x0800AA04: 2021     movs     r1, #0x20 */
    r1 = 0x20;
    /* 0x0800AA06: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800AA08: fcf70dfd bl       #0x8007426 */
    FUN_08007426();   /* BL -> 0x08007426 FUN_08007426 */
    /* 0x0800AA0C: 0a24     movs     r4, #0xa */
    r4 = 0xa;

    L_0800AA0E:
    /* 0x0800AA0E: 1048     ldr      r0, [pc, #0x40]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800AA50;   /* = RAM 0x20000384 */
    /* 0x0800AA10: fcf76efd bl       #0x80074f0 */
    FUN_080074F0();   /* BL -> 0x080074F0 FUN_080074F0 */
    /* 0x0800AA14: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x0800AA16: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800AA18: fcf700fd bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x0800AA1C: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x0800AA1E: 04d0     beq      #0x800aa2a */
    if (r0 == 0) goto L_0800AA2A;
    /* 0x0800AA20: 0a24     movs     r4, #0xa */
    r4 = 0xa;
    /* 0x0800AA22: 2046     mov      r0, r4 */
    r0 = r4;
    /* 0x0800AA24: 02f038f8 bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800AA28: f1e7     b        #0x800aa0e */
    goto L_0800AA0E;   /* loop */

    L_0800AA2A:
    /* 0x0800AA2A: 641e     subs     r4, r4, #1 */
    r4 = r4 - 1;
    /* 0x0800AA2C: e4b2     uxtb     r4, r4 */
    r4 = uxtb(r4);
    /* 0x0800AA2E: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x0800AA30: 02f032f8 bl       #0x800ca98 */
    FUN_0800CA98();   /* BL -> 0x0800CA98 FUN_0800CA98 */
    /* 0x0800AA34: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x0800AA36: ead1     bne      #0x800aa0e */
    if (r4 != 0) goto L_0800AA0E;
    /* 0x0800AA38: cde7     b        #0x800a9d6 */
    goto L_0800A9D6;   /* loop */
}
