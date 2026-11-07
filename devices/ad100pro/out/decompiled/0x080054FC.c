/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080054FC
 * Name      : FUN_080054FC
 * End       : 0x08005500
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080054E4 FUN_080054E4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080030F0 FUN_080030F0, 0x0800596C FUN_0800596C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080054FC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080054FE: 1b49     ldr      r1, [pc, #0x6c]  ; =RAM_20000480 */
    r1 = *(uint32_t *)0x0800556C;   /* = RAM 0x20000480 */
}
