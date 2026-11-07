/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800EAD8
 * Name      : FUN_0800EAD8
 * End       : 0x0800EC0E
 * Size      : 310 bytes, 37 instructions decoded in body (functions.json insns=25, blocks=2)
 * Class     : nested_block
 * Parent    : 0x0800EA58 FUN_0800EA58  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800EAD8(void)   /* args in r0-r3, return in r0 unless noted */
{
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
    goto L_0800EBFA;
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

    L_0800EBFA:
    /* 0x0800EBFA: 3ffc0000 ldc2     p0, c0, [pc], #-0 */
    /* TODO: ldc2 p0, c0, [pc], #-0 */
    /* 0x0800EBFE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EC00: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EC02: 3e00     movs     r6, r7 */
    r6 = r7;
    /* 0x0800EC04: 4100     lsls     r1, r0, #1 */
    r1 = r0 << 1;
    /* 0x0800EC06: 9c80     strh     r4, [r3, #4] */
    *(uint16_t *)(r3 + 4) = r4;   /* store */
    /* 0x0800EC08: 2200     movs     r2, r4 */
    r2 = r4;
    /* 0x0800EC0A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800EC0C: 1c00     movs     r4, r3 */
    r4 = r3;
}
