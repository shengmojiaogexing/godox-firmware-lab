/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800560E
 * Name      : FUN_0800560E
 * End       : 0x08005614
 * Size      : 6 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=2)
 * Class     : nested_block
 * Parent    : 0x080055FE FUN_080055FE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800560E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800560E: 0627     movs     r7, #6 */
    r7 = 6;
    /* 0x08005610: 0426     movs     r6, #4 */
    r6 = 4;
    /* 0x08005612: 01e0     b        #0x8005618 */
    FUN_08005618();   /* tail call 0x08005618 */
}
