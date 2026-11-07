/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800584C
 * Name      : FUN_0800584C
 * End       : 0x08005854
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

void FUN_0800584C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800584C: 0126     movs     r6, #1 */
    r6 = 1;
    /* 0x0800584E: 0225     movs     r5, #2 */
    r5 = 2;
    /* 0x08005850: 0824     movs     r4, #8 */
    r4 = 8;
    /* 0x08005852: 05e0     b        #0x8005860 */
    FUN_08005860();   /* tail call 0x08005860 */
}
