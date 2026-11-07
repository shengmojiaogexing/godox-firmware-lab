/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B5B2
 * Name      : FUN_0800B5B2
 * End       : 0x0800B5C0
 * Size      : 14 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800B588 FUN_0800B588  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B5B2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B5B4: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
    /* 0x0800B5B6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800B5B8: 5404     lsls     r4, r2, #0x11 */
    r4 = r2 << 0x11;
    /* 0x0800B5BA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800B5BC: 00f00108 and      r8, r0, #1 */
    r8 = r0 & 1;
}
