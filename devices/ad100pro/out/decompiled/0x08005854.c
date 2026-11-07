/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005854
 * Name      : FUN_08005854
 * End       : 0x0800585A
 * Size      : 6 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=2)
 * Class     : nested_block
 * Parent    : 0x080057E4 FUN_080057E4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005854(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005854: 0626     movs     r6, #6 */
    r6 = 6;
    /* 0x08005856: 0425     movs     r5, #4 */
    r5 = 4;
    /* 0x08005858: 01e0     b        #0x800585e */
    FUN_0800585E();   /* tail call 0x0800585E */
}
