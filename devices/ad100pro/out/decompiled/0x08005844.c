/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005844
 * Name      : FUN_08005844
 * End       : 0x0800584C
 * Size      : 8 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=2)
 * Class     : nested_block
 * Parent    : 0x080057E4 FUN_080057E4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005844(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005844: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x08005846: 0525     movs     r5, #5 */
    r5 = 5;
    /* 0x08005848: 0624     movs     r4, #6 */
    r4 = 6;
    /* 0x0800584A: 09e0     b        #0x8005860 */
    FUN_08005860();   /* tail call 0x08005860 */
}
