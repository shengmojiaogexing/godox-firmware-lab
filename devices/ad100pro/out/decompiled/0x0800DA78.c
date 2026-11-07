/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800DA78
 * Name      : big_isr_candidate
 * End       : 0x0800DF1E
 * Size      : 1190 bytes, 394 instructions decoded in body (functions.json insns=770, blocks=5)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : GPIOB.+0x1D
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void big_isr_candidate(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800DA78: 145a     ldrh     r4, [r2, r0] */
    r4 = *(uint16_t *)(r2 + r0);
    /* 0x0800DA7A: 5a5a     ldrh     r2, [r3, r1] */
    r2 = *(uint16_t *)(r3 + r1);
    /* 0x0800DA7C: 5a5a     ldrh     r2, [r3, r1] */
    r2 = *(uint16_t *)(r3 + r1);
    /* 0x0800DA7E: 5a50     str      r2, [r3, r1] */
    *(uint32_t *)(r3 + r1) = r2;   /* store */
    /* 0x0800DA80: 4646     mov      r6, r8 */
    r6 = r8;
    /* 0x0800DA82: 3c32     adds     r2, #0x3c */
    r2 = r2 + 0x3c;
    /* 0x0800DA84: 2828     cmp      r0, #0x28 */
    /* flags = cmp(r0, #0x28) */
    /* 0x0800DA86: ac03     lsls     r4, r5, #0xe */
    r4 = r5 << 0xe;
    /* 0x0800DA88: de03     lsls     r6, r3, #0xf */
    r6 = r3 << 0xf;
    /* 0x0800DA8A: 2404     lsls     r4, r4, #0x10 */
    r4 = r4 << 0x10;
    /* 0x0800DA8C: 6a04     lsls     r2, r5, #0x11 */
    r2 = r5 << 0x11;
    /* 0x0800DA8E: c404     lsls     r4, r0, #0x13 */
    r4 = r0 << 0x13;
    /* 0x0800DA90: 1405     lsls     r4, r2, #0x14 */
    r4 = r2 << 0x14;
    /* 0x0800DA92: 7805     lsls     r0, r7, #0x15 */
    r0 = r7 << 0x15;
    /* 0x0800DA94: f005     lsls     r0, r6, #0x17 */
    r0 = r6 << 0x17;
    /* 0x0800DA96: 7206     lsls     r2, r6, #0x19 */
    r2 = r6 << 0x19;
    /* 0x0800DA98: 6c07     lsls     r4, r5, #0x1d */
    r4 = r5 << 0x1d;
    /* 0x0800DA9A: 0050     str      r0, [r0, r0] */
    *(uint32_t *)(r0 + r0) = r0;   /* store */
    /* 0x0800DA9C: 44c0     stm      r0!, {r2, r6} */
    /* stm r0!, {r2, r6} */
    /* 0x0800DA9E: 8481     strh     r4, [r0, #0xc] */
    *(uint16_t *)(r0 + 12) = r4;   /* store */
    /* 0x0800DAA0: 4693     str      r3, [sp, #0x118] */
    *(uint32_t *)(sp + 280) = r3;   /* store */
    /* 0x0800DAA2: 558d     ldrh     r5, [r2, #0x2a] */
    r5 = *(uint16_t *)(r2 + 42);
    /* 0x0800DAA4: c669     ldr      r6, [r0, #0x1c] */
    r6 = *(uint32_t *)(r0 + 28);
    /* 0x0800DAA6: 6e7f     ldrb     r6, [r5, #0x1d] */
    r6 = *(uint8_t *)(r5 + 29);  /* MMIO: GPIOB.+0x1D(r) */
    /* 0x0800DAA8: a9bb     cbnz     r1, #0x800db16 */
    if (r1 != 0) goto L_0800DB16;
    /* 0x0800DAAC: 5c5c     ldrb     r4, [r3, r1] */
    r4 = *(uint8_t *)(r3 + r1);
    /* 0x0800DAAE: 5c5d     ldrb     r4, [r3, r5] */
    r4 = *(uint8_t *)(r3 + r5);
    /* 0x0800DAB0: 5d5d     ldrb     r5, [r3, r5] */
    r5 = *(uint8_t *)(r3 + r5);
    /* 0x0800DAB2: 5d5d     ldrb     r5, [r3, r5] */
    r5 = *(uint8_t *)(r3 + r5);
    /* 0x0800DAB4: 5d5d     ldrb     r5, [r3, r5] */
    r5 = *(uint8_t *)(r3 + r5);
    /* 0x0800DAB6: 5d5d     ldrb     r5, [r3, r5] */
    r5 = *(uint8_t *)(r3 + r5);
    /* 0x0800DAB8: 5d5d     ldrb     r5, [r3, r5] */
    r5 = *(uint8_t *)(r3 + r5);
    /* 0x0800DABA: 5d5d     ldrb     r5, [r3, r5] */
    r5 = *(uint8_t *)(r3 + r5);
    /* 0x0800DABC: 5d5d     ldrb     r5, [r3, r5] */
    r5 = *(uint8_t *)(r3 + r5);
    /* 0x0800DABE: 5d5e     ldrsh    r5, [r3, r1] */
    r5 = *(int16_t *)(r3 + r1);
    /* 0x0800DAC0: 5e5e     ldrsh    r6, [r3, r1] */
    r6 = *(int16_t *)(r3 + r1);
    /* 0x0800DAC2: 5e5e     ldrsh    r6, [r3, r1] */
    r6 = *(int16_t *)(r3 + r1);
    /* 0x0800DAC4: 5e5e     ldrsh    r6, [r3, r1] */
    r6 = *(int16_t *)(r3 + r1);
    /* 0x0800DAC6: 5e5e     ldrsh    r6, [r3, r1] */
    r6 = *(int16_t *)(r3 + r1);
    /* 0x0800DAC8: 5e5e     ldrsh    r6, [r3, r1] */
    r6 = *(int16_t *)(r3 + r1);
    /* 0x0800DACA: 5e5e     ldrsh    r6, [r3, r1] */
    r6 = *(int16_t *)(r3 + r1);
    /* 0x0800DACC: cedd     ble      #0x800da6c */
    if (r0 <= 0x28) goto L_0800DA6C;   /* signed */
    /* 0x0800DACE: ec00     lsls     r4, r5, #3 */
    r4 = r5 << 3;
    /* 0x0800DAD0: 0e1d     adds     r6, r1, #4 */
    r6 = r1 + 4;
    /* 0x0800DAD2: 3140     ands     r1, r6 */
    r1 = r1 & r6;
    /* 0x0800DAD4: 5662     str      r6, [r2, #0x24] */
    *(uint32_t *)(r2 + 36) = r6;   /* store */
    /* 0x0800DAD6: 7187     strh     r1, [r6, #0x3a] */
    *(uint16_t *)(r6 + 58) = r1;   /* store */
    /* 0x0800DAD8: 93a2     adr      r2, #0x24c */
    r2 = 0x00000000;   /* address */
    /* 0x0800DADA: b3c4     stm      r4!, {r0, r1, r4, r5, r7} */
    /* stm r4!, {r0, r1, r4, r5, r7} */
    /* 0x0800DADC: d3e9f604 ldrd     r0, r4, [r3, #0x3d8] */
    /* TODO: ldrd r0, r4, [r3, #0x3d8] */
    /* 0x0800DAE0: 1b27     movs     r7, #0x1b */
    r7 = 0x1b;
    /* 0x0800DAE2: 364c     ldr      r4, [pc, #0xd8]  ; =0x00000000 */
    r4 = *(uint32_t *)0x0800DBBC;   /* = 0x00000000 */
    /* 0x0800DAE4: 5867     str      r0, [r3, #0x74] */
    *(uint32_t *)(r3 + 116) = r0;   /* store */
    /* 0x0800DAE6: 7d89     ldrh     r5, [r7, #0xa] */
    r5 = *(uint16_t *)(r7 + 10);
    /* 0x0800DAE8: 98ae     add      r6, sp, #0x260 */
    r6 = sp + 0x260;
    /* 0x0800DAEA: bbc9     ldm      r1, {r0, r1, r3, r4, r5, r7} */
    /* ldm r1, {r0, r1, r3, r4, r5, r7} */
    /* 0x0800DAEC: c489     ldrh     r4, [r0, #0xe] */
    r4 = *(uint16_t *)(r0 + 14);
    /* 0x0800DAEE: 4e00     lsls     r6, r1, #1 */
    r6 = r1 << 1;
    /* 0x0800DAF0: c489     ldrh     r4, [r0, #0xe] */
    r4 = *(uint16_t *)(r0 + 14);
    /* 0x0800DAF2: 3b00     movs     r3, r7 */
    r3 = r7;
    /* 0x0800DAF4: 2776     strb     r7, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r7;   /* store */
    /* 0x0800DAF6: 3b62     str      r3, [r7, #0x20] */
    *(uint32_t *)(r7 + 32) = r3;   /* store */
    /* 0x0800DAF8: b176     strb     r1, [r6, #0x1a] */
    *(uint8_t *)(r6 + 26) = r1;   /* store */
    /* 0x0800DAFA: b1ecb1d8 ldc      p8, c13, [r1], #0x2c4 */
    /* TODO: ldc p8, c13, [r1], #0x2c4 */
    /* 0x0800DAFE: 27ec1362 stc      p2, c6, [r7], #-0x4c */
    /* TODO: stc p2, c6, [r7], #-0x4c */
    /* 0x0800DB02: 274e     ldr      r6, [pc, #0x9c]  ; =0x00005E92 */
    r6 = *(uint32_t *)0x0800DBA0;   /* = 0x00005E92 */
    /* 0x0800DB04: 9d62     str      r5, [r3, #0x28] */
    *(uint32_t *)(r3 + 40) = r5;   /* store */
    /* 0x0800DB06: 89d8     bhi      #0x800da1c */
    if (r0 > 0x28) goto L_0800DA1C;   /* unsigned */
    /* 0x0800DB08: 9dc4     stm      r4!, {r0, r2, r3, r4, r7} */
    /* stm r4!, {r0, r2, r3, r4, r7} */
    /* 0x0800DB0A: 13d8     bhi      #0x800db34 */
    if (r0 > 0x28) goto L_0800DB34;   /* unsigned */
    /* 0x0800DB0C: 04fe8282 cdp2     p2, #0, c8, c4, c2, #4 */
    /* TODO: cdp2 p2, #0, c8, c4, c2, #4 */
    /* 0x0800DB10: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800DB12: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB14: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;

    L_0800DB16:
    /* 0x0800DB16: 40fe0000 cdp2     p0, #4, c0, c0, c0, #0 */
    /* TODO: cdp2 p0, #4, c0, c0, c0, #0 */
    /* 0x0800DB1A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB1C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB1E: 049e     ldr      r6, [sp, #0x10] */
    r6 = *(uint32_t *)(sp + 16);
    /* 0x0800DB20: 9292     str      r2, [sp, #0x248] */
    *(uint32_t *)(sp + 584) = r2;   /* store */
    /* 0x0800DB22: f200     lsls     r2, r6, #3 */
    r2 = r6 << 3;
    /* 0x0800DB24: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB26: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800DB28: 9292     str      r2, [sp, #0x248] */
    *(uint32_t *)(sp + 584) = r2;   /* store */
    /* 0x0800DB2A: 92fe0000 cdp2     p0, #9, c0, c2, c0, #0 */
    /* TODO: cdp2 p0, #9, c0, c2, c0, #0 */
    /* 0x0800DB2E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB30: 04f01010 and      r0, r4, #0x100010 */
    r0 = r4 & 0x100010;

    L_0800DB34:
    /* 0x0800DB34: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800DB36: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB38: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800DB3A: f292     str      r2, [sp, #0x3c8] */
    *(uint32_t *)(sp + 968) = r2;   /* store */
    /* 0x0800DB3C: 929e     ldr      r6, [sp, #0x248] */
    r6 = *(uint32_t *)(sp + 584);
    /* 0x0800DB3E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB40: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB42: 04fe9292 mcr2     p2, #0, sb, c4, c2, #4 */
    /* TODO: mcr2 p2, #0, sb, c4, c2, #4 */
    /* 0x0800DB46: 9e00     lsls     r6, r3, #2 */
    r6 = r3 << 2;
    /* 0x0800DB48: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB4A: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800DB4C: 808e     ldrh     r0, [r0, #0x34] */
    r0 = *(uint16_t *)(r0 + 52);
    /* 0x0800DB4E: b8e0     b        #0x800dcc2 */
    goto L_0800DCC2;

    L_0800DCC2:
    /* 0x0800DCC2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DCC4: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800DCC6: 1e10     asrs     r6, r3, #0x20 */
    r6 = r3 >> 0x20;
    /* 0x0800DCC8: 100e     lsrs     r0, r2, #0x18 */
    r0 = r2 >> 0x18;
    /* 0x0800DCCA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DCCC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DCCE: 041c     adds     r4, r0, #0 */
    r4 = r0 + 0;
    /* 0x0800DCD0: 2222     movs     r2, #0x22 */
    r2 = 0x22;
    /* 0x0800DCD2: 1c00     movs     r4, r3 */
    r4 = r3;
    /* 0x0800DCD4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DCD6: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DCD8: fe90     str      r0, [sp, #0x3f8] */
    *(uint32_t *)(sp + 1016) = r0;   /* store */
    /* 0x0800DCDA: 9090     str      r0, [sp, #0x240] */
    *(uint32_t *)(sp + 576) = r0;   /* store */
    /* 0x0800DCDC: 6000     lsls     r0, r4, #1 */
    r0 = r4 << 1;
    /* 0x0800DCDE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DCE0: 057c     ldrb     r5, [r0, #0x10] */
    r5 = *(uint8_t *)(r0 + 16);
    /* 0x0800DCE2: 828a     ldrh     r2, [r0, #0x14] */
    r2 = *(uint16_t *)(r0 + 20);
    /* 0x0800DCE4: 867e     ldrb     r6, [r0, #0x1a] */
    r6 = *(uint8_t *)(r0 + 26);
    /* 0x0800DCE6: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800DCE8: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DCEA: 3e10     asrs     r6, r7, #0x20 */
    r6 = r7 >> 0x20;
    /* 0x0800DCEC: 2020     movs     r0, #0x20 */
    r0 = 0x20;
    /* 0x0800DCEE: 1000     movs     r0, r2 */
    r0 = r2;
    /* 0x0800DCF0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DCF2: 0412     asrs     r4, r0, #8 */
    r4 = r0 >> 8;
    /* 0x0800DCF4: 2a2a     cmp      r2, #0x2a */
    /* flags = cmp(r2, #0x2a) */
    /* 0x0800DCF6: 2400     movs     r4, r4 */
    r4 = r4;
    /* 0x0800DCF8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DCFA: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DCFC: 8080     strh     r0, [r0, #4] */
    *(uint16_t *)(r0 + 4) = r0;   /* store */
    /* 0x0800DCFE: fe80     strh     r6, [r7, #6] */
    *(uint16_t *)(r7 + 6) = r6;   /* store */
    /* 0x0800DD00: 8000     lsls     r0, r0, #2 */
    r0 = r0 << 2;
    /* 0x0800DD02: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD04: 053c     subs     r4, #5 */
    r4 = r4 - 5;
    /* 0x0800DD06: 0202     lsls     r2, r0, #8 */
    r2 = r0 << 8;
    /* 0x0800DD08: 3c02     lsls     r4, r7, #8 */
    r4 = r7 << 8;
    /* 0x0800DD0A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD0C: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DD0E: e018     adds     r0, r4, r3 */
    r0 = r4 + r3;
    /* 0x0800DD10: 0618     adds     r6, r0, r0 */
    r6 = r0 + r0;
    /* 0x0800DD12: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800DD14: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD16: 07fe0c30 cdp2     p0, #0, c3, c7, c12, #0 */
    /* TODO: cdp2 p0, #0, c3, c7, c12, #0 */
    /* 0x0800DD1A: 4030     adds     r0, #0x40 */
    r0 = r0 + 0x40;
    /* 0x0800DD1C: 0cfe0005 cdp2     p5, #0, c0, c12, c0, #0 */
    /* TODO: cdp2 p5, #0, c0, c12, c0, #0 */
    /* 0x0800DD20: c628     cmp      r0, #0xc6 */
    /* flags = cmp(r0, #0xc6) */
    /* 0x0800DD22: 1028     cmp      r0, #0x10 */
    /* flags = cmp(r0, #0x10) */
    /* 0x0800DD24: c600     lsls     r6, r0, #3 */
    r6 = r0 << 3;
    /* 0x0800DD26: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD28: 05c0     stm      r0!, {r0, r2} */
    /* stm r0!, {r0, r2} */
    /* 0x0800DD2A: 300e     lsrs     r0, r6, #0x18 */
    r0 = r6 >> 0x18;
    /* 0x0800DD2C: 30c0     stm      r0!, {r4, r5} */
    /* stm r0!, {r4, r5} */
    /* 0x0800DD2E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD30: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800DD32: 262a     cmp      r2, #0x26 */
    /* flags = cmp(r2, #0x26) */
    /* 0x0800DD34: 3200     movs     r2, r6 */
    r2 = r6;
    /* 0x0800DD36: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD38: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD3A: 0403     lsls     r4, r0, #0xc */
    r4 = r0 << 0xc;
    /* 0x0800DD3C: 0c30     adds     r0, #0xc */
    r0 = r0 + 0xc;
    /* 0x0800DD3E: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800DD40: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD42: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800DD44: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800DD46: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD48: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD4A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD4C: 0510     asrs     r5, r0, #0x20 */
    r5 = r0 >> 0x20;
    /* 0x0800DD4E: 1010     asrs     r0, r2, #0x20 */
    r0 = r2 >> 0x20;
    /* 0x0800DD50: 1010     asrs     r0, r2, #0x20 */
    r0 = r2 >> 0x20;
    /* 0x0800DD52: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD54: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DD56: 1010     asrs     r0, r2, #0x20 */
    r0 = r2 >> 0x20;
    /* 0x0800DD58: 7c10     asrs     r4, r7, #1 */
    r4 = r7 >> 1;
    /* 0x0800DD5A: 1000     movs     r0, r2 */
    r0 = r2;
    /* 0x0800DD5C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD5E: 0510     asrs     r5, r0, #0x20 */
    r5 = r0 >> 0x20;
    /* 0x0800DD60: 1010     asrs     r0, r2, #0x20 */
    r0 = r2 >> 0x20;
    /* 0x0800DD62: 1010     asrs     r0, r2, #0x20 */
    r0 = r2 >> 0x20;
    /* 0x0800DD64: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD66: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DD68: 2222     movs     r2, #0x22 */
    r2 = 0x22;
    /* 0x0800DD6A: fa22     movs     r2, #0xfa */
    r2 = 0xfa;
    /* 0x0800DD6C: 2200     movs     r2, r4 */
    r2 = r4;
    /* 0x0800DD6E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD70: 0524     movs     r4, #5 */
    r4 = 5;
    /* 0x0800DD72: 66af     add      r7, sp, #0x198 */
    r7 = sp + 0x198;
    /* 0x0800DD74: 3624     movs     r4, #0x36 */
    r4 = 0x36;
    /* 0x0800DD76: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD78: 0001     lsls     r0, r0, #4 */
    r0 = r0 << 4;
    /* 0x0800DD7A: 2800     movs     r0, r5 */
    r0 = r5;
    /* 0x0800DD7C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD7E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD80: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD82: 0520     movs     r0, #5 */
    r0 = 5;
    /* 0x0800DD84: 3038     subs     r0, #0x30 */
    r0 = r0 - 0x30;
    /* 0x0800DD86: 3020     movs     r0, #0x30 */
    r0 = 0x30;
    /* 0x0800DD88: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD8A: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DD8C: 6c00     lsls     r4, r5, #1 */
    r4 = r5 << 1;
    /* 0x0800DD8E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD90: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD92: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD94: 0206     lsls     r2, r0, #0x18 */
    r2 = r0 << 0x18;
    /* 0x0800DD96: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800DD98: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD9A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DD9C: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DD9E: 0e0e     lsrs     r6, r1, #0x18 */
    r6 = r1 >> 0x18;
    /* 0x0800DDA0: 0e0e     lsrs     r6, r1, #0x18 */
    r6 = r1 >> 0x18;
    /* 0x0800DDA2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DDA4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DDA6: 067c     ldrb     r6, [r0, #0x10] */
    r6 = *(uint8_t *)(r0 + 16);
    /* 0x0800DDA8: 7c38     subs     r0, #0x7c */
    r0 = r0 - 0x7c;
    /* 0x0800DDAA: 3810     asrs     r0, r7, #0x20 */
    r0 = r7 >> 0x20;
    /* 0x0800DDAC: 1000     movs     r0, r2 */
    r0 = r2;
    /* 0x0800DDAE: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800DDB0: 1010     asrs     r0, r2, #0x20 */
    r0 = r2 >> 0x20;
    /* 0x0800DDB2: 3838     subs     r0, #0x38 */
    r0 = r0 - 0x38;
    /* 0x0800DDB4: 7c7c     ldrb     r4, [r7, #0x11] */
    r4 = *(uint8_t *)(r7 + 17);
    /* 0x0800DDB6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DDB8: 0610     asrs     r6, r0, #0x20 */
    r6 = r0 >> 0x20;
    /* 0x0800DDBA: 7e50     str      r6, [r7, r1] */
    *(uint32_t *)(r7 + r1) = r6;   /* store */
    /* 0x0800DDBC: 103e     subs     r6, #0x10 */
    r6 = r6 - 0x10;
    /* 0x0800DDBE: 1200     movs     r2, r2 */
    r2 = r2;
    /* 0x0800DDC0: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800DDC2: 1040     ands     r0, r2 */
    r0 = r0 & r2;
    /* 0x0800DDC4: 0c9e     ldr      r6, [sp, #0x30] */
    r6 = *(uint32_t *)(sp + 48);
    /* 0x0800DDC6: 9e0c     lsrs     r6, r3, #0x12 */
    r6 = r3 >> 0x12;
    /* 0x0800DDC8: 4010     asrs     r0, r0, #1 */
    r0 = r0 >> 1;
    /* 0x0800DDCA: 0218     adds     r2, r0, r0 */
    r2 = r0 + r0;
    /* 0x0800DDCC: 1800     movs     r0, r3 */
    r0 = r3;
    /* 0x0800DDCE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DDD0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DDD2: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800DDD4: 3078     ldrb     r0, [r6] */
    r0 = *(uint8_t *)(r6);
    /* 0x0800DDD6: 4884     strh     r0, [r1, #0x22] */
    *(uint16_t *)(r1 + 34) = r0;   /* store */
    /* 0x0800DDD8: 8448     ldr      r0, [pc, #0x210]  ; =0x20002000 */
    r0 = *(uint32_t *)0x0800DFEC;   /* = 0x20002000 */
    /* 0x0800DDDA: 7830     adds     r0, #0x78 */
    r0 = r0 + 0x78;
    /* 0x0800DDDC: 0830     adds     r0, #8 */
    r0 = r0 + 8;
    /* 0x0800DDDE: 7878     ldrb     r0, [r7, #1] */
    r0 = *(uint8_t *)(r7 + 1);
    /* 0x0800DDE0: fcfc7878 ldc2l    p8, c7, [ip], #0x1e0 */
    /* TODO: ldc2l p8, c7, [ip], #0x1e0 */
    /* 0x0800DDE4: 3003     lsls     r0, r6, #0xc */
    r0 = r6 << 0xc;
    /* 0x0800DDE6: 7c38     subs     r0, #0x7c */
    r0 = r0 - 0x7c;
    /* 0x0800DDE8: 1000     movs     r0, r2 */
    r0 = r2;
    /* 0x0800DDEA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DDEC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DDEE: 0512     asrs     r5, r0, #8 */
    r5 = r0 >> 8;
    /* 0x0800DDF0: 347c     ldrb     r4, [r6, #0x10] */
    r4 = *(uint8_t *)(r6 + 16);
    /* 0x0800DDF2: 5890     str      r0, [sp, #0x160] */
    *(uint32_t *)(sp + 352) = r0;   /* store */
    /* 0x0800DDF4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DDF6: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x0800DDF8: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800DDFA: 5224     movs     r4, #0x52 */
    r4 = 0x52;
    /* 0x0800DDFC: 1824     movs     r4, #0x18 */
    r4 = 0x18;
    /* 0x0800DDFE: 4a04     lsls     r2, r1, #0x11 */
    r2 = r1 << 0x11;
    /* 0x0800DE00: 0478     ldrb     r4, [r0] */
    r4 = *(uint8_t *)(r0);
    /* 0x0800DE02: 8484     strh     r4, [r0, #0x24] */
    *(uint16_t *)(r0 + 36) = r4;   /* store */
    /* 0x0800DE04: 8400     lsls     r4, r0, #2 */
    r4 = r0 << 2;
    /* 0x0800DE06: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE08: 0004     lsls     r0, r0, #0x10 */
    r0 = r0 << 0x10;
    /* 0x0800DE0A: fc20     movs     r0, #0xfc */
    r0 = 0xfc;
    /* 0x0800DE0C: 20fc0000 stc2     p0, c0, [r0], #-0 */
    /* TODO: stc2 p0, c0, [r0], #-0 */
    /* 0x0800DE10: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE12: 057c     ldrb     r5, [r0, #0x10] */
    r5 = *(uint8_t *)(r0 + 16);
    /* 0x0800DE14: 8a92     str      r2, [sp, #0x228] */
    *(uint32_t *)(sp + 552) = r2;   /* store */
    /* 0x0800DE16: a27c     ldrb     r2, [r4, #0x12] */
    r2 = *(uint8_t *)(r4 + 18);
    /* 0x0800DE18: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE1A: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DE1C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800DE1E: 40fe0000 cdp2     p0, #4, c0, c0, c0, #0 */
    /* TODO: cdp2 p0, #4, c0, c0, c0, #0 */
    /* 0x0800DE22: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE24: 0546     mov      r5, r0 */
    r5 = r0;
    /* 0x0800DE26: 8a92     str      r2, [sp, #0x228] */
    *(uint32_t *)(sp + 552) = r2;   /* store */
    /* 0x0800DE28: 9262     str      r2, [r2, #0x28] */
    *(uint32_t *)(r2 + 40) = r2;   /* store */
    /* 0x0800DE2A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE2C: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DE2E: 4492     str      r2, [sp, #0x110] */
    *(uint32_t *)(sp + 272) = r2;   /* store */
    /* 0x0800DE30: 9292     str      r2, [sp, #0x248] */
    *(uint32_t *)(sp + 584) = r2;   /* store */
    /* 0x0800DE32: 6c00     lsls     r4, r5, #1 */
    r4 = r5 << 1;
    /* 0x0800DE34: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE36: 0538     subs     r0, #5 */
    r0 = r0 - 5;
    /* 0x0800DE38: 4888     ldrh     r0, [r1, #2] */
    r0 = *(uint16_t *)(r1 + 2);
    /* 0x0800DE3A: fe08     lsrs     r6, r7, #3 */
    r6 = r7 >> 3;
    /* 0x0800DE3C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE3E: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DE40: e4a2     adr      r2, #0x390 */
    r2 = 0x00000000;   /* address */
    /* 0x0800DE42: a2a2     adr      r2, #0x288 */
    r2 = 0x00000000;   /* address */
    /* 0x0800DE44: 9c00     lsls     r4, r3, #2 */
    r4 = r3 << 2;
    /* 0x0800DE46: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE48: 057c     ldrb     r5, [r0, #0x10] */
    r5 = *(uint8_t *)(r0 + 16);
    /* 0x0800DE4A: a2a2     adr      r2, #0x288 */
    r2 = 0x00000000;   /* address */
    /* 0x0800DE4C: a21c     adds     r2, r4, #2 */
    r2 = r4 + 2;
    /* 0x0800DE4E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE50: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DE52: 8080     strh     r0, [r0, #4] */
    *(uint16_t *)(r0 + 4) = r0;   /* store */
    /* 0x0800DE54: 8698     ldr      r0, [sp, #0x218] */
    r0 = *(uint32_t *)(sp + 536);
    /* 0x0800DE56: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800DE58: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE5A: 056c     ldr      r5, [r0, #0x40] */
    r5 = *(uint32_t *)(r0 + 64);
    /* 0x0800DE5C: 9292     str      r2, [sp, #0x248] */
    *(uint32_t *)(sp + 584) = r2;   /* store */
    /* 0x0800DE5E: 926c     ldr      r2, [r2, #0x48] */
    r2 = *(uint32_t *)(r2 + 72);
    /* 0x0800DE60: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE62: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DE64: 708a     ldrh     r0, [r6, #0x12] */
    r0 = *(uint16_t *)(r6 + 18);
    /* 0x0800DE66: 8a8a     ldrh     r2, [r1, #0x14] */
    r2 = *(uint16_t *)(r1 + 20);
    /* 0x0800DE68: 7c00     lsls     r4, r7, #1 */
    r4 = r7 << 1;
    /* 0x0800DE6A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE6C: 057e     ldrb     r5, [r0, #0x18] */
    r5 = *(uint8_t *)(r0 + 24);
    /* 0x0800DE6E: 8181     strh     r1, [r0, #0xc] */
    *(uint16_t *)(r0 + 12) = r1;   /* store */
    /* 0x0800DE70: 817e     ldrb     r1, [r0, #0x1a] */
    r1 = *(uint8_t *)(r0 + 26);
    /* 0x0800DE72: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE74: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DE76: 0040     ands     r0, r0 */
    r0 = r0 & r0;
    /* 0x0800DE78: ff00     lsls     r7, r7, #3 */
    r7 = r7 << 3;
    /* 0x0800DE7A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE7C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE7E: 0543     orrs     r5, r0 */
    r5 = r5 | r0;
    /* 0x0800DE80: 8589     ldrh     r5, [r0, #0xc] */
    r5 = *(uint16_t *)(r0 + 12);
    /* 0x0800DE82: 9161     str      r1, [r2, #0x18] */
    *(uint32_t *)(r2 + 24) = r1;   /* store */
    /* 0x0800DE84: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE86: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DE88: 4291     str      r1, [sp, #0x108] */
    *(uint32_t *)(sp + 264) = r1;   /* store */
    /* 0x0800DE8A: 9191     str      r1, [sp, #0x244] */
    *(uint32_t *)(sp + 580) = r1;   /* store */
    /* 0x0800DE8C: 6e00     lsls     r6, r5, #1 */
    r6 = r5 << 1;
    /* 0x0800DE8E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE90: 0518     adds     r5, r0, r0 */
    r5 = r0 + r0;
    /* 0x0800DE92: 2848     ldr      r0, [pc, #0xa0]  ; =0x007E0242 */
    r0 = *(uint32_t *)0x0800DF34;   /* = 0x007E0242 */
    /* 0x0800DE94: ff08     lsrs     r7, r7, #3 */
    r7 = r7 >> 3;
    /* 0x0800DE96: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DE98: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DE9A: f291     str      r1, [sp, #0x3c8] */
    *(uint32_t *)(sp + 968) = r1;   /* store */
    /* 0x0800DE9C: 9191     str      r1, [sp, #0x244] */
    *(uint32_t *)(sp + 580) = r1;   /* store */
    /* 0x0800DE9E: 8e00     lsls     r6, r1, #2 */
    r6 = r1 << 2;
    /* 0x0800DEA0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEA2: 057e     ldrb     r5, [r0, #0x18] */
    r5 = *(uint8_t *)(r0 + 24);
    /* 0x0800DEA4: 8991     str      r1, [sp, #0x224] */
    *(uint32_t *)(sp + 548) = r1;   /* store */
    /* 0x0800DEA6: 914e     ldr      r6, [pc, #0x244]  ; =0x0070000E */
    r6 = *(uint32_t *)0x0800E0EC;   /* = 0x0070000E */
    /* 0x0800DEA8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEAA: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DEAC: 8087     strh     r0, [r0, #0x3c] */
    *(uint16_t *)(r0 + 60) = r0;   /* store */
    /* 0x0800DEAE: 98a0     adr      r0, #0x260 */
    r0 = 0x00000000;   /* address */
    /* 0x0800DEB0: c000     lsls     r0, r0, #3 */
    r0 = r0 << 3;
    /* 0x0800DEB2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEB4: 056e     ldr      r5, [r0, #0x60] */
    r5 = *(uint32_t *)(r0 + 96);
    /* 0x0800DEB6: 9191     str      r1, [sp, #0x244] */
    *(uint32_t *)(sp + 580) = r1;   /* store */
    /* 0x0800DEB8: 916e     ldr      r1, [r2, #0x68] */
    r1 = *(uint32_t *)(r2 + 104);
    /* 0x0800DEBA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEBC: 0005     lsls     r0, r0, #0x14 */
    r0 = r0 << 0x14;
    /* 0x0800DEBE: 7289     ldrh     r2, [r6, #0xa] */
    r2 = *(uint16_t *)(r6 + 10);
    /* 0x0800DEC0: 8989     ldrh     r1, [r1, #0xc] */
    r1 = *(uint16_t *)(r1 + 12);
    /* 0x0800DEC2: 7e00     lsls     r6, r7, #1 */
    r6 = r7 << 1;
    /* 0x0800DEC4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEC6: 0601     lsls     r6, r0, #4 */
    r6 = r0 << 4;
    /* 0x0800DEC8: fc03     lsls     r4, r7, #0xf */
    r4 = r7 << 0xf;
    /* 0x0800DECA: fe02     lsls     r6, r7, #0xb */
    r6 = r7 << 0xb;
    /* 0x0800DECC: 1202     lsls     r2, r2, #8 */
    r2 = r2 << 8;
    /* 0x0800DECE: 2203     lsls     r2, r4, #0xc */
    r2 = r4 << 0xc;
    /* 0x0800DED0: fe01     lsls     r6, r7, #7 */
    r6 = r7 << 7;
    /* 0x0800DED2: fc00     lsls     r4, r7, #3 */
    r4 = r7 << 3;
    /* 0x0800DED4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DED6: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800DED8: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEDA: 0080     strh     r0, [r0] */
    *(uint16_t *)(r0) = r0;   /* store */
    /* 0x0800DEDC: 0100     movs     r1, r0 */
    r1 = r0;
    /* 0x0800DEDE: 03fe03fe cdp2     p14, #0, c15, c3, c3, #0 */
    /* TODO: cdp2 p14, #0, c15, c3, c3, #0 */
    /* 0x0800DEE2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEE4: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEE6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEE8: 0601     lsls     r6, r0, #4 */
    r6 = r0 << 4;
    /* 0x0800DEEA: 8603     lsls     r6, r0, #0xe */
    r6 = r0 << 0xe;
    /* 0x0800DEEC: 8e02     lsls     r6, r1, #0xa */
    r6 = r1 << 0xa;
    /* 0x0800DEEE: 1a02     lsls     r2, r3, #8 */
    r2 = r3 << 8;
    /* 0x0800DEF0: 3203     lsls     r2, r6, #0xc */
    r2 = r6 << 0xc;
    /* 0x0800DEF2: e201     lsls     r2, r4, #7 */
    r2 = r4 << 7;
    /* 0x0800DEF4: c200     lsls     r2, r0, #3 */
    r2 = r0 << 3;
    /* 0x0800DEF6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DEF8: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800DEFA: 0104     lsls     r1, r0, #0x10 */
    r1 = r0 << 0x10;
    /* 0x0800DEFC: 0322     movs     r2, #3 */
    r2 = 3;
    /* 0x0800DEFE: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x0800DF00: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x0800DF02: 03fe01dc cdp2     p12, #0, c13, c3, c1, #0 */
    /* TODO: cdp2 p12, #0, c13, c3, c1, #0 */
    /* 0x0800DF06: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF08: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF0A: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800DF0C: 7800     lsls     r0, r7, #1 */
    r0 = r7 << 1;
    /* 0x0800DF0E: c801     lsls     r0, r1, #7 */
    r0 = r1 << 7;
    /* 0x0800DF10: 8803     lsls     r0, r1, #0xe */
    r0 = r1 << 0xe;
    /* 0x0800DF12: fe03     lsls     r6, r7, #0xf */
    r6 = r7 << 0xf;
    /* 0x0800DF14: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800DF16: 0800     movs     r0, r1 */
    r0 = r1;
    /* 0x0800DF18: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF1A: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800DF1C: 03e4     b        #0x800d726 */
    goto L_0800D726;   /* loop */
    /* data 0x0800DBA0: .word 0x00005E92 0x00005E92   used by 0800DB02 */
    /* data 0x0800DBBC: .word 0x00000000 0x00000000   used by 0800DAE2 */
    /* data 0x0800DC52: .word 0x0E709070 0x0E709070   used by 0800EA8C */
}
