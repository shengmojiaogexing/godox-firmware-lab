/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B2D6
 * Name      : FUN_0800B2D6
 * End       : 0x0800B2DA
 * Size      : 4 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800B0BE FUN_0800B0BE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B2D6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B2D6: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_20000480 */
    r0 = *(uint32_t *)0x0800B30C;   /* = RAM 0x20000480 */
    /* 0x0800B2D8: 4270     strb     r2, [r0, #1] */
    *(uint8_t *)(r0 + 1) = r2;   /* store */
}
