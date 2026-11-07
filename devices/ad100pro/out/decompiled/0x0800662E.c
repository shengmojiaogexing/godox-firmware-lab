/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800662E
 * Name      : FUN_0800662E
 * End       : 0x0800663C
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080065CC FUN_080065CC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800662E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006630: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x08006632: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08006634: 8501     lsls     r5, r0, #6 */
    r5 = r0 << 6;
    /* 0x08006636: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08006638: 7c01     lsls     r4, r7, #5 */
    r4 = r7 << 5;
    /* 0x0800663A: 0020     movs     r0, #0 */
    r0 = 0;
}
