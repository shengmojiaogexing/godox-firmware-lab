/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080053F8
 * Name      : FUN_080053F8
 * End       : 0x08005400
 * Size      : 8 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080053E4 FUN_080053E4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080053F8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080053FA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x080053FC: 8301     lsls     r3, r0, #6 */
    r3 = r0 << 6;
    /* 0x080053FE: 0020     movs     r0, #0 */
    r0 = 0;
}
