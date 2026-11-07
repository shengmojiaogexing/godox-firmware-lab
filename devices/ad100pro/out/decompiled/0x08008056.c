/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008056
 * Name      : FUN_08008056
 * End       : 0x0800805A
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08008030 FUN_08008030  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800ACA8 FUN_0800ACA8
 * MMIO      : RCC.+0x3, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008056(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008058: 15e0     b        #0x8008086 */
    FUN_08008086();   /* tail call 0x08008086 */
}
