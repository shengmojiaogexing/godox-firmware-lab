/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005614
 * Name      : FUN_08005614
 * End       : 0x08005618
 * Size      : 4 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080055FE FUN_080055FE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08004D28 FUN_08004D28, 0x08004D74 FUN_08004D74
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005614(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005614: 0327     movs     r7, #3 */
    r7 = 3;
    /* 0x08005616: 0226     movs     r6, #2 */
    r6 = 2;
}
