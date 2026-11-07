/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006274
 * Name      : FUN_08006274
 * End       : 0x08006278
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800610C FUN_0800610C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080030F0 FUN_080030F0, 0x0800339C FUN_0800339C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006274(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006276: 4ee0     b        #0x8006316 */
    FUN_08006316();   /* tail call 0x08006316 */
}
