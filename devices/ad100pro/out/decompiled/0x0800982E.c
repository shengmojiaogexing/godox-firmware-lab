/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800982E
 * Name      : FUN_0800982E
 * End       : 0x0800983C
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080097F8 FUN_080097F8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800982E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009830: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
    /* 0x08009832: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08009834: 2001     lsls     r0, r4, #4 */
    r0 = r4 << 4;
    /* 0x08009836: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009838: 5404     lsls     r4, r2, #0x11 */
    r4 = r2 << 0x11;
    /* 0x0800983A: 0020     movs     r0, #0 */
    r0 = 0;
}
