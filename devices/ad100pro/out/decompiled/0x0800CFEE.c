/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CFEE
 * Name      : FUN_0800CFEE
 * End       : 0x0800CFF2
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800CF74 main  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800AD44 FUN_0800AD44
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CFEE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CFF0: fdf7a8fe bl       #0x800ad44 */
    FUN_0800AD44();   /* BL -> 0x0800AD44 FUN_0800AD44 */
}
