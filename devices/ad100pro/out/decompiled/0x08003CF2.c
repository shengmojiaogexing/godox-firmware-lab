/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003CF2
 * Name      : FUN_08003CF2
 * End       : 0x08003D2C
 * Size      : 58 bytes, 28 instructions decoded in body (functions.json insns=28, blocks=3)
 * Class     : nested_block
 * Parent    : 0x08003C50 FUN_08003C50  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003CF2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003CF4: b8e7     b        #0x8003c68 */
    goto L_08003C68;   /* loop */
    /* 0x08003CF6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08003CF8: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x08003CFA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003CFC: b601     lsls     r6, r6, #6 */
    r6 = r6 << 6;
    /* 0x08003CFE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D00: c401     lsls     r4, r0, #7 */
    r4 = r0 << 7;
    /* 0x08003D02: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D04: 6701     lsls     r7, r4, #5 */
    r7 = r4 << 5;
    /* 0x08003D06: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D08: 9201     lsls     r2, r2, #6 */
    r2 = r2 << 6;
    /* 0x08003D0A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D0C: bc01     lsls     r4, r7, #6 */
    r4 = r7 << 6;
    /* 0x08003D0E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D10: b201     lsls     r2, r6, #6 */
    r2 = r6 << 6;
    /* 0x08003D12: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D14: bd01     lsls     r5, r7, #6 */
    r5 = r7 << 6;
    /* 0x08003D16: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D18: d601     lsls     r6, r2, #7 */
    r6 = r2 << 7;
    /* 0x08003D1A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D1C: 7d01     lsls     r5, r7, #5 */
    r5 = r7 << 5;
    /* 0x08003D1E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D20: b101     lsls     r1, r6, #6 */
    r1 = r6 << 6;
    /* 0x08003D22: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D24: 3401     lsls     r4, r6, #4 */
    r4 = r6 << 4;
    /* 0x08003D26: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08003D28: 5ad3     blo      #0x8003de0 */
    if (/* blo */) goto L_08003DE0;   /* unsigned */
    /* 0x08003D2A: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
}
