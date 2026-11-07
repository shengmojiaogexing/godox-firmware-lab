/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800EA3E
 * Name      : FUN_0800EA3E
 * End       : 0x0800EAAA
 * Size      : 108 bytes, 35 instructions decoded in body (functions.json insns=30, blocks=4)
 * Class     : nested_block
 * Parent    : 0x0800E800 FUN_0800E800  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800EA3E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800EA3E: fc07     lsls     r4, r7, #0x1f */
    r4 = r7 << 0x1f;
    /* 0x0800EA40: effc0387 stc2l    p7, c8, [pc], #0xc */
    /* TODO: stc2l p7, c8, [pc], #0xc */
    /* 0x0800EA44: f800     lsls     r0, r7, #3 */
    r0 = r7 << 3;
    /* 0x0800EA46: 03e0     b        #0x800ea50 */
    goto L_0800EA50;

    L_0800EA50:
    /* 0x0800EA50: 000d     lsrs     r0, r0, #0x14 */
    r0 = r0 >> 0x14;
    /* 0x0800EA52: 0003     lsls     r0, r0, #0xc */
    r0 = r0 << 0xc;
    /* 0x0800EA54: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800EA56: 0fe0     b        #0x800ea78 */
    goto L_0800EA78;
    /* 0x0800EA58: 001e     subs     r0, r0, #0 */
    r0 = r0 - 0;
    /* 0x0800EA5A: e000     lsls     r0, r4, #3 */
    r0 = r4 << 3;
    /* 0x0800EA5C: 7ce0     b        #0x800eb58 */
    FUN_0800EB58();   /* tail call 0x0800EB58 */
    /* 0x0800EA5E: 00f0e003 and      r3, r0, #0xe0 */
    r3 = r0 & 0xe0;
    /* 0x0800EA62: e0e0     b        #0x800ec26 */
    FUN_0800EC26();   /* tail call 0x0800EC26 */

    L_0800EA78:
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
    FUN_0800EABA();   /* tail call 0x0800EABA */
}
