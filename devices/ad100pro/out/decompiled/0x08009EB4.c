/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009EB4
 * Name      : FUN_08009EB4
 * End       : 0x08009EE0
 * Size      : 44 bytes, 21 instructions decoded in body (functions.json insns=21, blocks=3)
 * Class     : nested_block
 * Parent    : 0x08009E30 FUN_08009E30  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009EB4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009EB6: c6e7     b        #0x8009e46 */
    goto L_08009E46;   /* loop */
    /* 0x08009EB8: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x08009EBA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009EBC: b601     lsls     r6, r6, #6 */
    r6 = r6 << 6;
    /* 0x08009EBE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009EC0: c401     lsls     r4, r0, #7 */
    r4 = r0 << 7;
    /* 0x08009EC2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009EC4: b201     lsls     r2, r6, #6 */
    r2 = r6 << 6;
    /* 0x08009EC6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009EC8: 3401     lsls     r4, r6, #4 */
    r4 = r6 << 4;
    /* 0x08009ECA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009ECC: 5ad3     blo      #0x8009f84 */
    if (/* blo */) goto L_08009F84;   /* unsigned */
    /* 0x08009ECE: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x08009ED0: bd01     lsls     r5, r7, #6 */
    r5 = r7 << 6;
    /* 0x08009ED2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009ED4: 7d01     lsls     r5, r7, #5 */
    r5 = r7 << 5;
    /* 0x08009ED6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009ED8: df01     lsls     r7, r3, #7 */
    r7 = r3 << 7;
    /* 0x08009EDA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009EDC: b101     lsls     r1, r6, #6 */
    r1 = r6 << 6;
    /* 0x08009EDE: 0020     movs     r0, #0 */
    r0 = 0;
}
