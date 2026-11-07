/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800EC56
 * Name      : FUN_0800EC56
 * End       : 0x0800EC70
 * Size      : 26 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800EC56(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800EC56: 1e00     movs     r6, r3 */
    r6 = r3;
    /* 0x0800EC58: 07e0     b        #0x800ec6a */
    goto L_0800EC6A;

    L_0800EC6A:
    /* 0x0800EC6A: 1800     movs     r0, r3 */
    r0 = r3;
    /* 0x0800EC6C: 1800     movs     r0, r3 */
    r0 = r3;
    /* 0x0800EC6E: 1800     movs     r0, r3 */
    r0 = r3;
}
