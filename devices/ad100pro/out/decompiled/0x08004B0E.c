/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004B0E
 * Name      : FUN_08004B0E
 * End       : 0x08004B12
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08004AF0 FUN_08004AF0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800CCA6 FUN_0800CCA6
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004B0E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004B10: 2870     strb     r0, [r5] */
    *(uint8_t *)(r5) = r0;   /* store */
}
