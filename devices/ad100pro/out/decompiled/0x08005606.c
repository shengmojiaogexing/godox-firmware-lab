/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005606
 * Name      : FUN_08005606
 * End       : 0x0800560E
 * Size      : 8 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=2)
 * Class     : nested_block
 * Parent    : 0x080055FE FUN_080055FE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005606(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005606: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x08005608: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x0800560A: 0825     movs     r5, #8 */
    r5 = 8;
    /* 0x0800560C: 05e0     b        #0x800561a */
    FUN_0800561A();   /* tail call 0x0800561A */
}
