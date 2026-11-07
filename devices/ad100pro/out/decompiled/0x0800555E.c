/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800555E
 * Name      : FUN_0800555E
 * End       : 0x0800557C
 * Size      : 30 bytes, 14 instructions decoded in body (functions.json insns=14, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080054E4 FUN_080054E4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800555E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005560: cde7     b        #0x80054fe */
    goto L_080054FE;   /* loop */
    /* 0x08005562: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08005564: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x08005566: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005568: 6701     lsls     r7, r4, #5 */
    r7 = r4 << 5;
    /* 0x0800556A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800556C: 8004     lsls     r0, r0, #0x12 */
    r0 = r0 << 0x12;
    /* 0x0800556E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005570: 9201     lsls     r2, r2, #6 */
    r2 = r2 << 6;
    /* 0x08005572: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005574: 8c01     lsls     r4, r1, #6 */
    r4 = r1 << 6;
    /* 0x08005576: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005578: 8a01     lsls     r2, r1, #6 */
    r2 = r1 << 6;
    /* 0x0800557A: 0020     movs     r0, #0 */
    r0 = 0;
}
