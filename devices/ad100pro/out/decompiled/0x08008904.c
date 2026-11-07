/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008904
 * Name      : FUN_08008904
 * End       : 0x08008948
 * Size      : 68 bytes, 33 instructions decoded in body (functions.json insns=33, blocks=5)
 * Class     : nested_block
 * Parent    : 0x08008738 FUN_08008738  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008904(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008906: f8bd     pop      {r3, r4, r5, r6, r7, pc} */
    pop({r3, r4, r5, r6, r7, pc});   /* return */
    /* 0x08008908: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x0800890A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800890C: 7d01     lsls     r5, r7, #5 */
    r5 = r7 << 5;
    /* 0x0800890E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008910: 90d5     bpl      #0x8008834 */
    if (/* bpl */) goto L_08008834;   /* result non-negative */
    /* 0x08008912: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x08008914: a201     lsls     r2, r4, #6 */
    r2 = r4 << 6;
    /* 0x08008916: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008918: ba01     lsls     r2, r7, #6 */
    r2 = r7 << 6;
    /* 0x0800891A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800891C: 08d8     bhi      #0x8008930 */
    if (/* bhi */) goto L_08008930;   /* unsigned */
    /* 0x0800891E: 0008     lsrs     r0, r0, #0x20 */
    r0 = r0 >> 0x20;
    /* 0x08008920: 3102     lsls     r1, r6, #8 */
    r1 = r6 << 8;
    /* 0x08008922: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008924: bb01     lsls     r3, r7, #6 */
    r3 = r7 << 6;
    /* 0x08008926: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008928: 3401     lsls     r4, r6, #4 */
    r4 = r6 << 4;
    /* 0x0800892A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800892C: b901     lsls     r1, r7, #6 */
    r1 = r7 << 6;
    /* 0x0800892E: 0020     movs     r0, #0 */
    r0 = 0;

    L_08008930:
    /* 0x08008930: b80b     lsrs     r0, r7, #0xe */
    r0 = r7 >> 0xe;
    /* 0x08008932: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08008934: b801     lsls     r0, r7, #6 */
    r0 = r7 << 6;
    /* 0x08008936: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08008938: 9c01     lsls     r4, r3, #6 */
    r4 = r3 << 6;
    /* 0x0800893A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800893C: 7017     asrs     r0, r6, #0x1d */
    r0 = r6 >> 0x1d;
    /* 0x0800893E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08008940: dc05     lsls     r4, r3, #0x17 */
    r4 = r3 << 0x17;
    /* 0x08008942: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08008944: db01     lsls     r3, r3, #7 */
    r3 = r3 << 7;
    /* 0x08008946: 0020     movs     r0, #0 */
    r0 = 0;
}
