/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800EA58
 * Name      : FUN_0800EA58
 * End       : 0x0800EB62
 * Size      : 266 bytes, 69 instructions decoded in body (functions.json insns=8, blocks=2)
 * Class     : nested_block
 * Parent    : 0x0800EA3E FUN_0800EA3E  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800EA58(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800EA58: 001e     subs     r0, r0, #0 */
    r0 = r0 - 0;
    /* 0x0800EA5A: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800EA5C: 7ce0     b        #0x800eb58 */
    goto L_0800EB58;
    /* 0x0800EA5E: 00f0e003 and      r3, r0, #0xe0 */
    r3 = r0 & 0xe0;
    /* 0x0800EA62: e0e0     b        #0x800ec26 */
    FUN_0800EC26();   /* tail call 0x0800EC26 */
    /* 0x0800EA78: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800EA7A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA7C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA7E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA80: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EA82: 0d00     movs     r5, r1 */
    r5 = r1;
    /* 0x0800EA84: 3830     adds     r0, #0x38 */
    r0 = r0 + 0x38;
    /* 0x0800EA86: 07f8381f strb     r1, [r7, #0x38]! */
    *(uint8_t *)(r7 + 56) = r1;   /* store */
    /* 0x0800EA8A: f83c     subs     r4, #0xf8 */
    r4 = r4 - 0xf8;
    /* 0x0800EA8C: 1ff83e1e ldrb.w   r1, [pc, #-0xe3e] */
    r1 = *(uint32_t *)0x0800DC52;   /* = 0x0E709070 */
    /* 0x0800EA90: 301e     subs     r0, r6, #0 */
    r0 = r6 - 0;
    /* 0x0800EA92: 1c70     strb     r4, [r3] */
    *(uint8_t *)(r3) = r4;   /* store */
    /* 0x0800EA94: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EA96: 700e     lsrs     r0, r6, #0x19 */
    r0 = r6 >> 0x19;
    /* 0x0800EA98: 1c70     strb     r4, [r3] */
    *(uint8_t *)(r3) = r4;   /* store */
    /* 0x0800EA9A: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EA9C: 781e     subs     r0, r7, #1 */
    r0 = r7 - 1;
    /* 0x0800EA9E: 1c7f     ldrb     r4, [r3, #0x1c] */
    r4 = *(uint8_t *)(r3 + 28);
    /* 0x0800EAA0: fc1c     adds     r4, r7, #3 */
    r4 = r7 + 3;
    /* 0x0800EAA2: 3ffc1c1f ldc2     p15, c1, [pc], #-0x70 */
    /* TODO: ldc2 p15, c1, [pc], #-0x70 */
    /* 0x0800EAA6: f000     lsls     r0, r6, #3 */
    r0 = r6 << 3;
    /* 0x0800EAA8: 07e0     b        #0x800eaba */
    goto L_0800EABA;
    /* 0x0800EAB4: 007f     ldrb     r0, [r0, #0x1c] */
    r0 = *(uint8_t *)(r0 + 28);
    /* 0x0800EAB6: c003     lsls     r0, r0, #0xf */
    r0 = r0 << 0xf;
    /* 0x0800EAB8: fff007ff bl       #0x810e8ca */
    FUN_0810E8CA();   /* BL -> 0x0810E8CA FUN_0810E8CA */
    /* 0x0800EABC: f80f     lsrs     r0, r7, #0x1f */
    r0 = r7 >> 0x1f;
    /* 0x0800EABE: fffc0f1c ldc2l    p12, c1, [pc], #0x3c */
    /* TODO: ldc2l p12, c1, [pc], #0x3c */
    /* 0x0800EAC2: 1e1c     adds     r6, r3, #0 */
    r6 = r3 + 0;
    /* 0x0800EAC4: 380e     lsrs     r0, r7, #0x18 */
    r0 = r7 >> 0x18;
    /* 0x0800EAC6: 1c38     subs     r0, #0x1c */
    r0 = r0 - 0x1c;
    /* 0x0800EAC8: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EACA: 380e     lsrs     r0, r7, #0x18 */
    r0 = r7 >> 0x18;
    /* 0x0800EACC: 1e3c     subs     r4, #0x1e */
    r4 = r4 - 0x1e;
    /* 0x0800EACE: 1e1f     subs     r6, r3, #4 */
    r6 = r3 - 4;
    /* 0x0800EAD0: 3ffc0f1f ldc2     p15, c1, [pc], #-0x3c */
    /* TODO: ldc2 p15, c1, [pc], #-0x3c */
    /* 0x0800EAD4: fc07     lsls     r4, r7, #0x1f */
    r4 = r7 << 0x1f;
    /* 0x0800EAD8: 0303     lsls     r3, r0, #0xc */
    r3 = r0 << 0xc;
    /* 0x0800EADA: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800EADC: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EADE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EAE0: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EAE2: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EAE4: 0d1c     adds     r5, r1, #0 */
    r5 = r1 + 0;
    /* 0x0800EAE6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EAE8: 1c00     movs     r4, r3 */
    r4 = r3;
    /* 0x0800EAEA: 001c     adds     r0, r0, #0 */
    r0 = r0 + 0;
    /* 0x0800EAEC: 001e     subs     r0, r0, #0 */
    r0 = r0 - 0;
    /* 0x0800EAEE: 1c01     lsls     r4, r3, #4 */
    r4 = r3 << 4;
    /* 0x0800EAF0: fe1c     adds     r6, r7, #3 */
    r6 = r7 + 3;
    /* 0x0800EAF2: 0ffe1c3f mcr2     p15, #0, r3, c15, c12, #0 */
    /* TODO: mcr2 p15, #0, r3, c15, c12, #0 */
    /* 0x0800EAF6: fe1c     adds     r6, r7, #3 */
    r6 = r7 + 3;
    /* 0x0800EAF8: 7fe0     b        #0x800ebfa */
    FUN_0800EBFA();   /* tail call 0x0800EBFA */
    /* 0x0800EAFA: 1dfe001f cdp2     p15, #1, c1, c13, c0, #0 */
    /* TODO: cdp2 p15, #1, c1, c13, c0, #0 */
    /* 0x0800EAFE: f800     lsls     r0, r7, #3 */
    r0 = r7 << 3;
    /* 0x0800EB00: 1fe0     b        #0x800eb42 */
    goto L_0800EB42;

    L_0800EB42:
    /* 0x0800EB42: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EB44: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EB46: 0d01     lsls     r5, r1, #4 */
    r5 = r1 << 4;
    /* 0x0800EB48: f030     adds     r0, #0xf0 */
    r0 = r0 + 0xf0;

    L_0800EB58:
    /* 0x0800EB58: 0e1c     adds     r6, r1, #0 */
    r6 = r1 + 0;
    /* 0x0800EB5A: 070e     lsrs     r7, r0, #0x18 */
    r7 = r0 >> 0x18;
    /* 0x0800EB5C: 1c07     lsls     r4, r3, #0x1c */
    r4 = r3 << 0x1c;
    /* 0x0800EB5E: 0e1e     subs     r6, r1, #0 */
    r6 = r1 - 0;
    /* 0x0800EB60: 0e3c     subs     r4, #0xe */
    r4 = r4 - 0xe;
}
