/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080058E8
 * Name      : FUN_080058E8
 * End       : 0x080058EC
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080058E0 FUN_080058E0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080030F0 FUN_080030F0, 0x08004D74 FUN_08004D74
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080058E8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080058EA: 1e48     ldr      r0, [pc, #0x78]  ; =RAM_2000018E */
    r0 = *(uint32_t *)0x08005964;   /* = RAM 0x2000018E */
}
