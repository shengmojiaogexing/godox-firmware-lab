/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005AB4
 * Name      : FUN_08005AB4
 * End       : 0x08005AB8
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08005A78 IRQ5_Handler  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08006014 FUN_08006014, 0x0800B7F0 FUN_0800B7F0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005AB4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005AB6: 0de0     b        #0x8005ad4 */
    FUN_08005AD4();   /* tail call 0x08005AD4 */
}
