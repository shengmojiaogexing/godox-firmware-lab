/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080058B6
 * Name      : FUN_080058B6
 * End       : 0x080058BA
 * Size      : 4 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=2)
 * Class     : tiny_fragment
 * Parent    : 0x080058B2 FUN_080058B2  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080058B6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080058B6: 0426     movs     r6, #4 */
    r6 = 4;
    /* 0x080058B8: 02e0     b        #0x80058c0 */
    FUN_080058C0();   /* tail call 0x080058C0 */
}
