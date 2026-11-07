/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A2A4
 * Name      : FUN_0800A2A4
 * End       : 0x0800A2A8
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800A230 FUN_0800A230  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A2A4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A2A6: a570     strb     r5, [r4, #2] */
    *(uint8_t *)(r4 + 2) = r5;   /* store */
}
