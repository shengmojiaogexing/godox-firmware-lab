/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008D00
 * Name      : dispatcher_large
 * End       : 0x08009020
 * Size      : 800 bytes, 358 instructions decoded in body (functions.json insns=358, blocks=79)
 * Class     : hub
 * Callers   : 2  -> 0x0800CF74 main, 0x0800D014 FUN_0800D014
 * Callees   : 19  -> 0x080038AC FUN_080038AC, 0x08004484 FUN_08004484, 0x0800451C FUN_0800451C, 0x08004B74 FUN_08004B74, 0x080067EC FUN_080067EC, 0x080072C4 FUN_080072C4, 0x0800741C FUN_0800741C, 0x08009198 FUN_08009198, ... +11
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void dispatcher_large(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008D00: f0b5     push     {r4, r5, r6, r7, lr} */
    push({r4, r5, r6, r7, lr});
    /* 0x08008D02: 85b0     sub      sp, #0x14 */
    sp = sp - 0x14;
    /* 0x08008D04: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x08008D06: 02f043fd bl       #0x800b790 */
    FUN_0800B790();   /* BL -> 0x0800B790 FUN_0800B790 */
    /* 0x08008D0A: c54d     ldr      r5, [pc, #0x314]  ; =RAM_20000164 */
    r5 = *(uint32_t *)0x08009020;   /* = RAM 0x20000164 */
    /* 0x08008D0C: a87a     ldrb     r0, [r5, #0xa] */
    r0 = *(uint8_t *)(r5 + 10);
    /* 0x08008D0E: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008D10: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08008D12: a872     strb     r0, [r5, #0xa] */
    *(uint8_t *)(r5 + 10) = r0;   /* store */
    /* 0x08008D14: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x08008D16: 7dd9     bls      #0x8008e14 */
    if (r0 <= 4) goto L_08008E14;   /* unsigned */
    /* 0x08008D18: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x08008D1A: ae72     strb     r6, [r5, #0xa] */
    *(uint8_t *)(r5 + 10) = r6;   /* store */
    /* 0x08008D1C: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08008D1E: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08008D20: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08008D22: 0296     str      r6, [sp, #8] */
    *(uint32_t *)(sp + 8) = r6;   /* store */
    /* 0x08008D24: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008D26: 0197     str      r7, [sp, #4] */
    *(uint32_t *)(sp + 4) = r7;   /* store */
    /* 0x08008D28: 0396     str      r6, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r6;   /* store */
    /* 0x08008D2A: be48     ldr      r0, [pc, #0x2f8]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008D2C: fef7cafa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008D30: 1020     movs     r0, #0x10 */
    r0 = 0x10;
    /* 0x08008D32: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08008D34: c1c2     stm      r2!, {r0, r6, r7} */
    /* stm r2!, {r0, r6, r7} */
    /* 0x08008D36: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008D38: ba48     ldr      r0, [pc, #0x2e8]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008D3A: fef7c3fa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008D3E: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x08008D40: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08008D42: c1c2     stm      r2!, {r0, r6, r7} */
    /* stm r2!, {r0, r6, r7} */
    /* 0x08008D44: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008D46: b748     ldr      r0, [pc, #0x2dc]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008D48: fef7bcfa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008D4C: b549     ldr      r1, [pc, #0x2d4]  ; =GPIOB.MODER */
    r1 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008D4E: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08008D50: 8862     str      r0, [r1, #0x28] */
    *(uint32_t *)(r1 + 40) = r0;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x08008D52: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08008D54: 03f070fe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08008D58: b902     lsls     r1, r7, #0xa */
    r1 = r7 << 0xa;
    /* 0x08008D5A: b348     ldr      r0, [pc, #0x2cc]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008D5C: fef75efb bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008D60: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008D62: 00d1     bne      #0x8008d66 */
    if (r0 != 0) goto L_08008D66;
    /* 0x08008D64: 0124     movs     r4, #1 */
    r4 = 1;

    L_08008D66:
    /* 0x08008D66: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08008D68: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x08008D6A: af48     ldr      r0, [pc, #0x2bc]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008D6C: fef756fb bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008D70: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008D72: 01d1     bne      #0x8008d78 */
    if (r0 != 0) goto L_08008D78;
    /* 0x08008D74: 0220     movs     r0, #2 */
    r0 = 2;
    /* 0x08008D76: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008D78:
    /* 0x08008D78: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08008D7A: 0903     lsls     r1, r1, #0xc */
    r1 = r1 << 0xc;
    /* 0x08008D7C: aa48     ldr      r0, [pc, #0x2a8]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008D7E: fef74dfb bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008D82: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008D84: 01d1     bne      #0x8008d8a */
    if (r0 != 0) goto L_08008D8A;
    /* 0x08008D86: 0420     movs     r0, #4 */
    r0 = 4;
    /* 0x08008D88: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008D8A:
    /* 0x08008D8A: 1020     movs     r0, #0x10 */
    r0 = 0x10;
    /* 0x08008D8C: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08008D8E: 0296     str      r6, [sp, #8] */
    *(uint32_t *)(sp + 8) = r6;   /* store */
    /* 0x08008D90: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008D92: 0197     str      r7, [sp, #4] */
    *(uint32_t *)(sp + 4) = r7;   /* store */
    /* 0x08008D94: 0396     str      r6, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r6;   /* store */
    /* 0x08008D96: a348     ldr      r0, [pc, #0x28c]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008D98: fef794fa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008D9C: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08008D9E: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08008DA0: c1c2     stm      r2!, {r0, r6, r7} */
    /* stm r2!, {r0, r6, r7} */
    /* 0x08008DA2: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008DA4: 9f48     ldr      r0, [pc, #0x27c]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008DA6: fef78dfa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008DAA: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x08008DAC: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08008DAE: c1c2     stm      r2!, {r0, r6, r7} */
    /* stm r2!, {r0, r6, r7} */
    /* 0x08008DB0: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008DB2: 9c48     ldr      r0, [pc, #0x270]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008DB4: fef786fa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008DB8: 9a49     ldr      r1, [pc, #0x268]  ; =GPIOB.MODER */
    r1 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008DBA: 1020     movs     r0, #0x10 */
    r0 = 0x10;
    /* 0x08008DBC: 8862     str      r0, [r1, #0x28] */
    *(uint32_t *)(r1 + 40) = r0;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x08008DBE: 0320     movs     r0, #3 */
    r0 = 3;
    /* 0x08008DC0: 03f03afe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08008DC4: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08008DC6: 8902     lsls     r1, r1, #0xa */
    r1 = r1 << 0xa;
    /* 0x08008DC8: 9748     ldr      r0, [pc, #0x25c]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008DCA: fef727fb bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008DCE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008DD0: 01d1     bne      #0x8008dd6 */
    if (r0 != 0) goto L_08008DD6;
    /* 0x08008DD2: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08008DD4: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008DD6:
    /* 0x08008DD6: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08008DD8: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x08008DDA: 9348     ldr      r0, [pc, #0x24c]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008DDC: fef71efb bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008DE0: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008DE2: 01d1     bne      #0x8008de8 */
    if (r0 != 0) goto L_08008DE8;
    /* 0x08008DE4: 1020     movs     r0, #0x10 */
    r0 = 0x10;
    /* 0x08008DE6: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008DE8:
    /* 0x08008DE8: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x08008DEA: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08008DEC: 0296     str      r6, [sp, #8] */
    *(uint32_t *)(sp + 8) = r6;   /* store */
    /* 0x08008DEE: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008DF0: 0197     str      r7, [sp, #4] */
    *(uint32_t *)(sp + 4) = r7;   /* store */
    /* 0x08008DF2: 0396     str      r6, [sp, #0xc] */
    *(uint32_t *)(sp + 12) = r6;   /* store */
    /* 0x08008DF4: 8b48     ldr      r0, [pc, #0x22c]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008DF6: fef765fa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008DFA: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08008DFC: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08008DFE: c1c2     stm      r2!, {r0, r6, r7} */
    /* stm r2!, {r0, r6, r7} */
    /* 0x08008E00: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008E02: 8848     ldr      r0, [pc, #0x220]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008E04: fef75efa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008E08: 1020     movs     r0, #0x10 */
    r0 = 0x10;
    /* 0x08008E0A: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08008E0C: c1c2     stm      r2!, {r0, r6, r7} */
    /* stm r2!, {r0, r6, r7} */
    /* 0x08008E0E: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008E10: 8448     ldr      r0, [pc, #0x210]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008E12: 00e0     b        #0x8008e16 */
    goto L_08008E16;

    L_08008E14:
    /* 0x08008E14: 7ee0     b        #0x8008f14 */
    goto L_08008F14;

    L_08008E16:
    /* 0x08008E16: fef755fa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008E1A: 8249     ldr      r1, [pc, #0x208]  ; =GPIOB.MODER */
    r1 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008E1C: 1020     movs     r0, #0x10 */
    r0 = 0x10;
    /* 0x08008E1E: 8862     str      r0, [r1, #0x28] */
    *(uint32_t *)(r1 + 40) = r0;   /* store */  /* MMIO: GPIOB.BRR(w) */
    /* 0x08008E20: 0a20     movs     r0, #0xa */
    r0 = 0xa;
    /* 0x08008E22: 03f009fe bl       #0x800ca38 */
    FUN_0800CA38();   /* BL -> 0x0800CA38 FUN_0800CA38 */
    /* 0x08008E26: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08008E28: 8902     lsls     r1, r1, #0xa */
    r1 = r1 << 0xa;
    /* 0x08008E2A: 7f48     ldr      r0, [pc, #0x1fc]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008E2C: fef7f6fa bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008E30: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008E32: 01d1     bne      #0x8008e38 */
    if (r0 != 0) goto L_08008E38;
    /* 0x08008E34: 4020     movs     r0, #0x40 */
    r0 = 0x40;
    /* 0x08008E36: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008E38:
    /* 0x08008E38: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08008E3A: c902     lsls     r1, r1, #0xb */
    r1 = r1 << 0xb;
    /* 0x08008E3C: 7a48     ldr      r0, [pc, #0x1e8]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008E3E: fef7edfa bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008E42: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008E44: 01d1     bne      #0x8008e4a */
    if (r0 != 0) goto L_08008E4A;
    /* 0x08008E46: 8020     movs     r0, #0x80 */
    r0 = 0x80;
    /* 0x08008E48: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008E4A:
    /* 0x08008E4A: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08008E4C: 0903     lsls     r1, r1, #0xc */
    r1 = r1 << 0xc;
    /* 0x08008E4E: 7648     ldr      r0, [pc, #0x1d8]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008E50: fef7e4fa bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008E54: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008E56: 02d1     bne      #0x8008e5e */
    if (r0 != 0) goto L_08008E5E;
    /* 0x08008E58: ff20     movs     r0, #0xff */
    r0 = 0xff;
    /* 0x08008E5A: 0130     adds     r0, #1 */
    r0 = r0 + 1;
    /* 0x08008E5C: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008E5E:
    /* 0x08008E5E: 0421     movs     r1, #4 */
    r1 = 4;
    /* 0x08008E60: 7248     ldr      r0, [pc, #0x1c8]  ; =GPIOD.MODER */
    r0 = *(uint32_t *)0x0800902C;   /* = MMIO GPIOD+0x00 */
    /* 0x08008E62: fef7dbfa bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008E66: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008E68: 01d1     bne      #0x8008e6e */
    if (r0 != 0) goto L_08008E6E;
    /* 0x08008E6A: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x08008E6C: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008E6E:
    /* 0x08008E6E: 8021     movs     r1, #0x80 */
    r1 = 0x80;
    /* 0x08008E70: 6d48     ldr      r0, [pc, #0x1b4]  ; =GPIOC.MODER */
    r0 = *(uint32_t *)0x08009028;   /* = MMIO GPIOC+0x00 */
    /* 0x08008E72: fef7d3fa bl       #0x800741c */
    FUN_0800741C();   /* BL -> 0x0800741C FUN_0800741C */
    /* 0x08008E76: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008E78: 02d1     bne      #0x8008e80 */
    if (r0 != 0) goto L_08008E80;
    /* 0x08008E7A: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008E7C: 4002     lsls     r0, r0, #9 */
    r0 = r0 << 9;
    /* 0x08008E7E: 0443     orrs     r4, r0 */
    r4 = r4 | r0;

    L_08008E80:
    /* 0x08008E80: 0820     movs     r0, #8 */
    r0 = 8;
    /* 0x08008E82: 0090     str      r0, [sp] */
    *(uint32_t *)(sp) = r0;   /* store */
    /* 0x08008E84: 0196     str      r6, [sp, #4] */
    *(uint32_t *)(sp + 4) = r6;   /* store */
    /* 0x08008E86: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008E88: 0296     str      r6, [sp, #8] */
    *(uint32_t *)(sp + 8) = r6;   /* store */
    /* 0x08008E8A: 6648     ldr      r0, [pc, #0x198]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008E8C: fef71afa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008E90: 1020     movs     r0, #0x10 */
    r0 = 0x10;
    /* 0x08008E92: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08008E94: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08008E96: c1c2     stm      r2!, {r0, r6, r7} */
    /* stm r2!, {r0, r6, r7} */
    /* 0x08008E98: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008E9A: 6248     ldr      r0, [pc, #0x188]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008E9C: fef712fa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008EA0: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x08008EA2: 6a46     mov      r2, sp */
    r2 = sp;
    /* 0x08008EA4: c1c2     stm      r2!, {r0, r6, r7} */
    /* stm r2!, {r0, r6, r7} */
    /* 0x08008EA6: 6946     mov      r1, sp */
    r1 = sp;
    /* 0x08008EA8: 5e48     ldr      r0, [pc, #0x178]  ; =GPIOB.MODER */
    r0 = *(uint32_t *)0x08009024;   /* = MMIO GPIOB+0x00 */
    /* 0x08008EAA: fef70bfa bl       #0x80072c4 */
    FUN_080072C4();   /* BL -> 0x080072C4 FUN_080072C4 */
    /* 0x08008EAE: 5c4f     ldr      r7, [pc, #0x170]  ; =RAM_20000164 */
    r7 = *(uint32_t *)0x08009020;   /* = RAM 0x20000164 */
    /* 0x08008EB0: 2037     adds     r7, #0x20 */
    r7 = r7 + 0x20;
    /* 0x08008EB2: 002c     cmp      r4, #0 */
    /* flags = cmp(r4, #0) */
    /* 0x08008EB4: 30d0     beq      #0x8008f18 */
    if (r4 == 0) goto L_08008F18;
    /* 0x08008EB6: 5e48     ldr      r0, [pc, #0x178]  ; =RAM_200001E5 */
    r0 = *(uint32_t *)0x08009030;   /* = RAM 0x200001E5 */
    /* 0x08008EB8: 0078     ldrb     r0, [r0] */
    r0 = *(uint8_t *)(r0);
    /* 0x08008EBA: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008EBC: 2ad0     beq      #0x8008f14 */
    if (r0 == 1) goto L_08008F14;
    /* 0x08008EBE: 6007     lsls     r0, r4, #0x1d */
    r0 = r4 << 0x1d;
    /* 0x08008EC0: 00d4     bmi      #0x8008ec4 */
    if (r0 < 1) goto L_08008EC4;   /* result negative */
    /* 0x08008EC2: 6e77     strb     r6, [r5, #0x1d] */
    *(uint8_t *)(r5 + 29) = r6;   /* store */

    L_08008EC4:
    /* 0x08008EC4: faf7f2fc bl       #0x80038ac */
    FUN_080038AC();   /* BL -> 0x080038AC FUN_080038AC */
    /* 0x08008EC8: 7d20     movs     r0, #0x7d */
    r0 = 0x7d;
    /* 0x08008ECA: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x08008ECC: 6886     strh     r0, [r5, #0x32] */
    *(uint16_t *)(r5 + 50) = r0;   /* store */
    /* 0x08008ECE: 02f08ffc bl       #0x800b7f0 */
    FUN_0800B7F0();   /* BL -> 0x0800B7F0 FUN_0800B7F0 */
    /* 0x08008ED2: 02f051fb bl       #0x800b578 */
    FUN_0800B578();   /* BL -> 0x0800B578 FUN_0800B578 */
    /* 0x08008ED6: a87e     ldrb     r0, [r5, #0x1a] */
    r0 = *(uint8_t *)(r5 + 26);
    /* 0x08008ED8: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008EDA: 1bd0     beq      #0x8008f14 */
    if (r0 == 1) goto L_08008F14;
    /* 0x08008EDC: 687a     ldrb     r0, [r5, #9] */
    r0 = *(uint8_t *)(r5 + 9);
    /* 0x08008EDE: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008EE0: 3dd0     beq      #0x8008f5e */
    if (r0 == 0) goto L_08008F5E;
    /* 0x08008EE2: 287b     ldrb     r0, [r5, #0xc] */
    r0 = *(uint8_t *)(r5 + 12);
    /* 0x08008EE4: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008EE6: 15d1     bne      #0x8008f14 */
    if (r0 != 0) goto L_08008F14;
    /* 0x08008EE8: e87a     ldrb     r0, [r5, #0xb] */
    r0 = *(uint8_t *)(r5 + 11);
    /* 0x08008EEA: 401c     adds     r0, r0, #1 */
    r0 = r0 + 1;
    /* 0x08008EEC: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x08008EEE: e872     strb     r0, [r5, #0xb] */
    *(uint8_t *)(r5 + 11) = r0;   /* store */
    /* 0x08008EF0: 3228     cmp      r0, #0x32 */
    /* flags = cmp(r0, #0x32) */
    /* 0x08008EF2: 0fd9     bls      #0x8008f14 */
    if (r0 <= 0x32) goto L_08008F14;   /* unsigned */
    /* 0x08008EF4: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008EF6: 2873     strb     r0, [r5, #0xc] */
    *(uint8_t *)(r5 + 12) = r0;   /* store */
    /* 0x08008EF8: 6e73     strb     r6, [r5, #0xd] */
    *(uint8_t *)(r5 + 13) = r6;   /* store */
    /* 0x08008EFA: 2146     mov      r1, r4 */
    r1 = r4;
    /* 0x08008EFC: ee73     strb     r6, [r5, #0xf] */
    *(uint8_t *)(r5 + 15) = r6;   /* store */
    /* 0x08008EFE: 4d4c     ldr      r4, [pc, #0x134]  ; =RAM_20000480 */
    r4 = *(uint32_t *)0x08009034;   /* = RAM 0x20000480 */
    /* 0x08008F00: 2e74     strb     r6, [r5, #0x10] */
    *(uint8_t *)(r5 + 16) = r6;   /* store */
    /* 0x08008F02: 2129     cmp      r1, #0x21 */
    /* flags = cmp(r1, #0x21) */
    /* 0x08008F04: 76d0     beq      #0x8008ff4 */
    if (r1 == 0x21) goto L_08008FF4;
    /* 0x08008F06: 62dc     bgt      #0x8008fce */
    if (r1 > 0x21) goto L_08008FCE;   /* signed */
    /* 0x08008F08: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08008F0A: 6fd0     beq      #0x8008fec */
    if (r1 == 1) goto L_08008FEC;
    /* 0x08008F0C: 1029     cmp      r1, #0x10 */
    /* flags = cmp(r1, #0x10) */
    /* 0x08008F0E: 01d1     bne      #0x8008f14 */
    if (r1 != 0x10) goto L_08008F14;
    /* 0x08008F10: fbf704fb bl       #0x800451c */
    FUN_0800451C();   /* BL -> 0x0800451C FUN_0800451C */

    L_08008F14:
    /* 0x08008F14: 05b0     add      sp, #0x14 */
    sp = sp + 0x14;
    /* 0x08008F16: f0bd     pop      {r4, r5, r6, r7, pc} */
    pop({r4, r5, r6, r7, pc});   /* return */

    L_08008F18:
    /* 0x08008F18: ae85     strh     r6, [r5, #0x2c] */
    *(uint16_t *)(r5 + 44) = r6;   /* store */
    /* 0x08008F1A: 6e72     strb     r6, [r5, #9] */
    *(uint8_t *)(r5 + 9) = r6;   /* store */
    /* 0x08008F1C: ee72     strb     r6, [r5, #0xb] */
    *(uint8_t *)(r5 + 11) = r6;   /* store */
    /* 0x08008F1E: 2e73     strb     r6, [r5, #0xc] */
    *(uint8_t *)(r5 + 12) = r6;   /* store */
    /* 0x08008F20: ae76     strb     r6, [r5, #0x1a] */
    *(uint8_t *)(r5 + 26) = r6;   /* store */
    /* 0x08008F22: f878     ldrb     r0, [r7, #3] */
    r0 = *(uint8_t *)(r7 + 3);
    /* 0x08008F24: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08008F26: 01d0     beq      #0x8008f2c */
    if (r0 == 0) goto L_08008F2C;
    /* 0x08008F28: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
    /* 0x08008F2A: f870     strb     r0, [r7, #3] */
    *(uint8_t *)(r7 + 3) = r0;   /* store */

    L_08008F2C:
    /* 0x08008F2C: 2e77     strb     r6, [r5, #0x1c] */
    *(uint8_t *)(r5 + 28) = r6;   /* store */
    /* 0x08008F2E: 6e77     strb     r6, [r5, #0x1d] */
    *(uint8_t *)(r5 + 29) = r6;   /* store */
    /* 0x08008F30: 3e70     strb     r6, [r7] */
    *(uint8_t *)(r7) = r6;   /* store */
    /* 0x08008F32: 287c     ldrb     r0, [r5, #0x10] */
    r0 = *(uint8_t *)(r5 + 16);
    /* 0x08008F34: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008F36: 01d1     bne      #0x8008f3c */
    if (r0 != 1) goto L_08008F3C;
    /* 0x08008F38: 03f09efc bl       #0x800c878 */
    FUN_0800C878();   /* BL -> 0x0800C878 FUN_0800C878 */

    L_08008F3C:
    /* 0x08008F3C: 687b     ldrb     r0, [r5, #0xd] */
    r0 = *(uint8_t *)(r5 + 13);
    /* 0x08008F3E: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008F40: 01d1     bne      #0x8008f46 */
    if (r0 != 1) goto L_08008F46;
    /* 0x08008F42: fdf753fc bl       #0x80067ec */
    FUN_080067EC();   /* BL -> 0x080067EC FUN_080067EC */

    L_08008F46:
    /* 0x08008F46: e87b     ldrb     r0, [r5, #0xf] */
    r0 = *(uint8_t *)(r5 + 15);
    /* 0x08008F48: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008F4A: 01d1     bne      #0x8008f50 */
    if (r0 != 1) goto L_08008F50;
    /* 0x08008F4C: 00f024f9 bl       #0x8009198 */
    FUN_08009198();   /* BL -> 0x08009198 FUN_08009198 */

    L_08008F50:
    /* 0x08008F50: 6e73     strb     r6, [r5, #0xd] */
    *(uint8_t *)(r5 + 13) = r6;   /* store */
    /* 0x08008F52: ae73     strb     r6, [r5, #0xe] */
    *(uint8_t *)(r5 + 14) = r6;   /* store */
    /* 0x08008F54: ee73     strb     r6, [r5, #0xf] */
    *(uint8_t *)(r5 + 15) = r6;   /* store */
    /* 0x08008F56: 3648     ldr      r0, [pc, #0xd8]  ; =RAM_200001E5 */
    r0 = *(uint32_t *)0x08009030;   /* = RAM 0x200001E5 */
    /* 0x08008F58: 2e74     strb     r6, [r5, #0x10] */
    *(uint8_t *)(r5 + 16) = r6;   /* store */
    /* 0x08008F5A: 0670     strb     r6, [r0] */
    *(uint8_t *)(r0) = r6;   /* store */
    /* 0x08008F5C: dae7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008F5E:
    /* 0x08008F5E: a88d     ldrh     r0, [r5, #0x2c] */
    r0 = *(uint16_t *)(r5 + 44);
    /* 0x08008F60: 8442     cmp      r4, r0 */
    /* flags = cmp(r4, r0) */
    /* 0x08008F62: 32d1     bne      #0x8008fca */
    if (r4 != r0) goto L_08008FCA;
    /* 0x08008F64: 0120     movs     r0, #1 */
    r0 = 1;
    /* 0x08008F66: 6872     strb     r0, [r5, #9] */
    *(uint8_t *)(r5 + 9) = r0;   /* store */
    /* 0x08008F68: 202c     cmp      r4, #0x20 */
    /* flags = cmp(r4, #0x20) */
    /* 0x08008F6A: 1cd0     beq      #0x8008fa6 */
    if (r4 == 0x20) goto L_08008FA6;
    /* 0x08008F6C: 09dc     bgt      #0x8008f82 */
    if (r4 > 0x20) goto L_08008F82;   /* signed */
    /* 0x08008F6E: 012c     cmp      r4, #1 */
    /* flags = cmp(r4, #1) */
    /* 0x08008F70: 17d0     beq      #0x8008fa2 */
    if (r4 == 1) goto L_08008FA2;
    /* 0x08008F72: 022c     cmp      r4, #2 */
    /* flags = cmp(r4, #2) */
    /* 0x08008F74: 1ad0     beq      #0x8008fac */
    if (r4 == 2) goto L_08008FAC;
    /* 0x08008F76: 042c     cmp      r4, #4 */
    /* flags = cmp(r4, #4) */
    /* 0x08008F78: 10d0     beq      #0x8008f9c */
    if (r4 == 4) goto L_08008F9C;
    /* 0x08008F7A: 102c     cmp      r4, #0x10 */
    /* flags = cmp(r4, #0x10) */
    /* 0x08008F7C: cad1     bne      #0x8008f14 */
    if (r4 != 0x10) goto L_08008F14;
    /* 0x08008F7E: 6873     strb     r0, [r5, #0xd] */
    *(uint8_t *)(r5 + 13) = r0;   /* store */
    /* 0x08008F80: c8e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008F82:
    /* 0x08008F82: 802c     cmp      r4, #0x80 */
    /* flags = cmp(r4, #0x80) */
    /* 0x08008F84: 18d0     beq      #0x8008fb8 */
    if (r4 == 0x80) goto L_08008FB8;
    /* 0x08008F86: ff3c     subs     r4, #0xff */
    r4 = r4 - 0xff;
    /* 0x08008F88: 641e     subs     r4, r4, #1 */
    r4 = r4 - 1;
    /* 0x08008F8A: 12d0     beq      #0x8008fb2 */
    if (r4 == 0x80) goto L_08008FB2;
    /* 0x08008F8C: 012c     cmp      r4, #1 */
    /* flags = cmp(r4, #1) */
    /* 0x08008F8E: 15d0     beq      #0x8008fbc */
    if (r4 == 1) goto L_08008FBC;
    /* 0x08008F90: ff3c     subs     r4, #0xff */
    r4 = r4 - 0xff;
    /* 0x08008F92: 012c     cmp      r4, #1 */
    /* flags = cmp(r4, #1) */
    /* 0x08008F94: bed1     bne      #0x8008f14 */
    if (r4 != 1) goto L_08008F14;
    /* 0x08008F96: 01f0c1fc bl       #0x800a91c */
    FUN_0800A91C();   /* BL -> 0x0800A91C FUN_0800A91C */
    /* 0x08008F9A: bbe7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008F9C:
    /* 0x08008F9C: 00f04efc bl       #0x800983c */
    FUN_0800983C();   /* BL -> 0x0800983C FUN_0800983C */
    /* 0x08008FA0: b8e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FA2:
    /* 0x08008FA2: e873     strb     r0, [r5, #0xf] */
    *(uint8_t *)(r5 + 15) = r0;   /* store */
    /* 0x08008FA4: b6e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FA6:
    /* 0x08008FA6: 02f07bfb bl       #0x800b6a0 */
    FUN_0800B6A0();   /* BL -> 0x0800B6A0 FUN_0800B6A0 */
    /* 0x08008FAA: b3e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FAC:
    /* 0x08008FAC: 03f05cf8 bl       #0x800c068 */
    FUN_0800C068();   /* BL -> 0x0800C068 FUN_0800C068 */
    /* 0x08008FB0: b0e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FB2:
    /* 0x08008FB2: fbf767fa bl       #0x8004484 */
    FUN_08004484();   /* BL -> 0x08004484 FUN_08004484 */
    /* 0x08008FB6: ade7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FB8:
    /* 0x08008FB8: 2874     strb     r0, [r5, #0x10] */
    *(uint8_t *)(r5 + 16) = r0;   /* store */
    /* 0x08008FBA: abe7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FBC:
    /* 0x08008FBC: 287f     ldrb     r0, [r5, #0x1c] */
    r0 = *(uint8_t *)(r5 + 28);
    /* 0x08008FBE: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x08008FC0: a8d1     bne      #0x8008f14 */
    if (r0 != 1) goto L_08008F14;
    /* 0x08008FC2: 3c20     movs     r0, #0x3c */
    r0 = 0x3c;
    /* 0x08008FC4: 2886     strh     r0, [r5, #0x30] */
    *(uint16_t *)(r5 + 48) = r0;   /* store */
    /* 0x08008FC6: 2e77     strb     r6, [r5, #0x1c] */
    *(uint8_t *)(r5 + 28) = r6;   /* store */
    /* 0x08008FC8: a4e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FCA:
    /* 0x08008FCA: ac85     strh     r4, [r5, #0x2c] */
    *(uint16_t *)(r5 + 44) = r4;   /* store */
    /* 0x08008FCC: a2e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FCE:
    /* 0x08008FCE: 8029     cmp      r1, #0x80 */
    /* flags = cmp(r1, #0x80) */
    /* 0x08008FD0: 0dd0     beq      #0x8008fee */
    if (r1 == 0x80) goto L_08008FEE;
    /* 0x08008FD2: ff39     subs     r1, #0xff */
    r1 = r1 - 0xff;
    /* 0x08008FD4: 0229     cmp      r1, #2 */
    /* flags = cmp(r1, #2) */
    /* 0x08008FD6: 9dd1     bne      #0x8008f14 */
    if (r1 != 2) goto L_08008F14;
    /* 0x08008FD8: fbf7ccfd bl       #0x8004b74 */
    FUN_08004B74();   /* BL -> 0x08004B74 FUN_08004B74 */
    /* 0x08008FDC: ee74     strb     r6, [r5, #0x13] */
    *(uint8_t *)(r5 + 19) = r6;   /* store */
    /* 0x08008FDE: 6e75     strb     r6, [r5, #0x15] */
    *(uint8_t *)(r5 + 21) = r6;   /* store */
    /* 0x08008FE0: be70     strb     r6, [r7, #2] */
    *(uint8_t *)(r7 + 2) = r6;   /* store */
    /* 0x08008FE2: 6079     ldrb     r0, [r4, #5] */
    r0 = *(uint8_t *)(r4 + 5);
    /* 0x08008FE4: 02f0b8fb bl       #0x800b758 */
    FUN_0800B758();   /* BL -> 0x0800B758 FUN_0800B758 */
    /* 0x08008FE8: 94e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FEC:
    /* 0x08008FEC: 10e0     b        #0x8009010 */
    goto L_08009010;

    L_08008FEE:
    /* 0x08008FEE: 02f047fc bl       #0x800b880 */
    FUN_0800B880();   /* BL -> 0x0800B880 FUN_0800B880 */
    /* 0x08008FF2: 8fe7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08008FF4:
    /* 0x08008FF4: 697c     ldrb     r1, [r5, #0x11] */
    r1 = *(uint8_t *)(r5 + 17);
    /* 0x08008FF6: 8907     lsls     r1, r1, #0x1e */
    r1 = r1 << 0x1e;
    /* 0x08008FF8: 890f     lsrs     r1, r1, #0x1e */
    r1 = r1 >> 0x1e;
    /* 0x08008FFA: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08008FFC: 8ad1     bne      #0x8008f14 */
    if (r1 != 1) goto L_08008F14;
    /* 0x08008FFE: e978     ldrb     r1, [r5, #3] */
    r1 = *(uint8_t *)(r5 + 3);
    /* 0x08009000: 0129     cmp      r1, #1 */
    /* flags = cmp(r1, #1) */
    /* 0x08009002: 01d0     beq      #0x8009008 */
    if (r1 == 1) goto L_08009008;
    /* 0x08009004: 0121     movs     r1, #1 */
    r1 = 1;
    /* 0x08009006: 00e0     b        #0x800900a */
    goto L_0800900A;

    L_08009008:
    /* 0x08009008: 0021     movs     r1, #0 */
    r1 = 0;

    L_0800900A:
    /* 0x0800900A: e970     strb     r1, [r5, #3] */
    *(uint8_t *)(r5 + 3) = r1;   /* store */
    /* 0x0800900C: a071     strb     r0, [r4, #6] */
    *(uint8_t *)(r4 + 6) = r0;   /* store */
    /* 0x0800900E: 81e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */

    L_08009010:
    /* 0x08009010: b978     ldrb     r1, [r7, #2] */
    r1 = *(uint8_t *)(r7 + 2);
    /* 0x08009012: 4140     eors     r1, r0 */
    r1 = r1 ^ r0;
    /* 0x08009014: b970     strb     r1, [r7, #2] */
    *(uint8_t *)(r7 + 2) = r1;   /* store */
    /* 0x08009016: 0b21     movs     r1, #0xb */
    r1 = 0xb;
    /* 0x08009018: e974     strb     r1, [r5, #0x13] */
    *(uint8_t *)(r5 + 19) = r1;   /* store */
    /* 0x0800901A: 6e75     strb     r6, [r5, #0x15] */
    *(uint8_t *)(r5 + 21) = r6;   /* store */
    /* 0x0800901C: a871     strb     r0, [r5, #6] */
    *(uint8_t *)(r5 + 6) = r0;   /* store */
    /* 0x0800901E: 79e7     b        #0x8008f14 */
    goto L_08008F14;   /* loop */
}
