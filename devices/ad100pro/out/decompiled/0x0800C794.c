/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C794
 * Name      : FUN_0800C794
 * End       : 0x0800C7D4
 * Size      : 64 bytes, 30 instructions decoded in body (functions.json insns=30, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800C6F4 FUN_0800C6F4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C794(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C796: efe7     b        #0x800c778 */
    goto L_0800C778;   /* loop */
    /* 0x0800C798: 5e01     lsls     r6, r3, #5 */
    r6 = r3 << 5;
    /* 0x0800C79A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C79C: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x0800C79E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7A0: 8004     lsls     r0, r0, #0x12 */
    r0 = r0 << 0x12;
    /* 0x0800C7A2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7A4: ab01     lsls     r3, r5, #6 */
    r3 = r5 << 6;
    /* 0x0800C7A6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7A8: b301     lsls     r3, r6, #6 */
    r3 = r6 << 6;
    /* 0x0800C7AA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7AC: b801     lsls     r0, r7, #6 */
    r0 = r7 << 6;
    /* 0x0800C7AE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7B0: aaf20008 subw     r8, sl, #0 */
    /* TODO: subw r8, sl, #0 */
    /* 0x0800C7B4: 9801     lsls     r0, r3, #6 */
    r0 = r3 << 6;
    /* 0x0800C7B6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7B8: 6401     lsls     r4, r4, #5 */
    r4 = r4 << 5;
    /* 0x0800C7BA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7BC: 7d01     lsls     r5, r7, #5 */
    r5 = r7 << 5;
    /* 0x0800C7BE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7C0: a201     lsls     r2, r4, #6 */
    r2 = r4 << 6;
    /* 0x0800C7C2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7C4: cc01     lsls     r4, r1, #7 */
    r4 = r1 << 7;
    /* 0x0800C7C6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7C8: b201     lsls     r2, r6, #6 */
    r2 = r6 << 6;
    /* 0x0800C7CA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7CC: 3401     lsls     r4, r6, #4 */
    r4 = r6 << 4;
    /* 0x0800C7CE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C7D0: 9c01     lsls     r4, r3, #6 */
    r4 = r3 << 6;
    /* 0x0800C7D2: 0020     movs     r0, #0 */
    r0 = 0;
}
