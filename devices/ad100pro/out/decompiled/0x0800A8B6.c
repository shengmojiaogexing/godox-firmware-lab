/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A8B6
 * Name      : FUN_0800A8B6
 * End       : 0x0800A8D0
 * Size      : 26 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800A818 FUN_0800A818  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A8B6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A8B8: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
    /* 0x0800A8BA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800A8BC: b801     lsls     r0, r7, #6 */
    r0 = r7 << 6;
    /* 0x0800A8BE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A8C0: b201     lsls     r2, r6, #6 */
    r2 = r6 << 6;
    /* 0x0800A8C2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A8C4: 7c01     lsls     r4, r7, #5 */
    r4 = r7 << 5;
    /* 0x0800A8C6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A8C8: bc01     lsls     r4, r7, #6 */
    r4 = r7 << 6;
    /* 0x0800A8CA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800A8CC: 3401     lsls     r4, r6, #4 */
    r4 = r6 << 4;
    /* 0x0800A8CE: 0020     movs     r0, #0 */
    r0 = 0;
}
