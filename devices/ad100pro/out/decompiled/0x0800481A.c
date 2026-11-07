/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800481A
 * Name      : FUN_0800481A
 * End       : 0x0800481E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080047CC FUN_080047CC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08004880 FUN_08004880, 0x08008A5C FUN_08008A5C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800481A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800481C: 01e0     b        #0x8004822 */
    FUN_08004822();   /* tail call 0x08004822 */
}
