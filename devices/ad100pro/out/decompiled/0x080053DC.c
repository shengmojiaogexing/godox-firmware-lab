/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080053DC
 * Name      : FUN_080053DC
 * End       : 0x080053E4
 * Size      : 8 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080053BC FUN_080053BC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080053DC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080053DE: 00bd     pop      {pc} */
    pop({pc});   /* return */
    /* 0x080053E0: 7501     lsls     r5, r6, #5 */
    r5 = r6 << 5;
    /* 0x080053E2: 0020     movs     r0, #0 */
    r0 = 0;
}
