/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800DAAC
 * Name      : FUN_0800DAAC
 * End       : 0x0800DB16
 * Size      : 106 bytes, 49 instructions decoded in body (functions.json insns=49, blocks=7)
 * Class     : nested_block
 * Parent    : 0x0800DA78 big_isr_candidate  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800DAAC(void)   /* args in r0-r3, return in r0 unless noted */
{
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
    if (/* ble */) goto L_0800DA6C;   /* signed */
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
    if (/* bhi */) goto L_0800DA1C;   /* unsigned */
    /* 0x0800DB08: 9dc4     stm      r4!, {r0, r2, r3, r4, r7} */
    /* stm r4!, {r0, r2, r3, r4, r7} */
    /* 0x0800DB0A: 13d8     bhi      #0x800db34 */
    if (/* bhi */) goto L_0800DB34;   /* unsigned */
    /* 0x0800DB0C: 04fe8282 cdp2     p2, #0, c8, c4, c2, #4 */
    /* TODO: cdp2 p2, #0, c8, c4, c2, #4 */
    /* 0x0800DB10: fe00     lsls     r6, r7, #3 */
    r6 = r7 << 3;
    /* 0x0800DB12: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DB14: 0002     lsls     r0, r0, #8 */
    r0 = r0 << 8;
}
