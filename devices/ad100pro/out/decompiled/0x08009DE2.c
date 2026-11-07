/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009DE2
 * Name      : FUN_08009DE2
 * End       : 0x08009DE6
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08009D84 FUN_08009D84  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080082B4 FUN_080082B4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009DE2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009DE4: 0348     ldr      r0, [pc, #0xc]  ; =RAM_200002D0 */
    r0 = *(uint32_t *)0x08009DF4;   /* = RAM 0x200002D0 */
}
