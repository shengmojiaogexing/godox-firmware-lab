/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080060C2
 * Name      : FUN_080060C2
 * End       : 0x0800610C
 * Size      : 74 bytes, 35 instructions decoded in body (functions.json insns=35, blocks=1)
 * Class     : nested_block
 * Parent    : 0x08006014 FUN_08006014  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080060C2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080060C4: ebe7     b        #0x800609e */
    goto L_0800609E;   /* loop */
    /* 0x080060C6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x080060C8: 3401     lsls     r4, r6, #4 */
    r4 = r6 << 4;
    /* 0x080060CA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060CC: 5e01     lsls     r6, r3, #5 */
    r6 = r3 << 5;
    /* 0x080060CE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060D0: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x080060D2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060D4: 8004     lsls     r0, r0, #0x12 */
    r0 = r0 << 0x12;
    /* 0x080060D6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060D8: ab01     lsls     r3, r5, #6 */
    r3 = r5 << 6;
    /* 0x080060DA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060DC: b301     lsls     r3, r6, #6 */
    r3 = r6 << 6;
    /* 0x080060DE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060E0: b801     lsls     r0, r7, #6 */
    r0 = r7 << 6;
    /* 0x080060E2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060E4: aaf20008 subw     r8, sl, #0 */
    /* TODO: subw r8, sl, #0 */
    /* 0x080060E8: 9801     lsls     r0, r3, #6 */
    r0 = r3 << 6;
    /* 0x080060EA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060EC: 6401     lsls     r4, r4, #5 */
    r4 = r4 << 5;
    /* 0x080060EE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060F0: 7d01     lsls     r5, r7, #5 */
    r5 = r7 << 5;
    /* 0x080060F2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060F4: a201     lsls     r2, r4, #6 */
    r2 = r4 << 6;
    /* 0x080060F6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060F8: cc01     lsls     r4, r1, #7 */
    r4 = r1 << 7;
    /* 0x080060FA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x080060FC: bc01     lsls     r4, r7, #6 */
    r4 = r7 << 6;
    /* 0x080060FE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006100: b201     lsls     r2, r6, #6 */
    r2 = r6 << 6;
    /* 0x08006102: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006104: 8201     lsls     r2, r0, #6 */
    r2 = r0 << 6;
    /* 0x08006106: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006108: 9c01     lsls     r4, r3, #6 */
    r4 = r3 << 6;
    /* 0x0800610A: 0020     movs     r0, #0 */
    r0 = 0;
}
