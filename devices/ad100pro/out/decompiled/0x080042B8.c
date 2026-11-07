/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080042B8
 * Name      : FUN_080042B8
 * End       : 0x080042BC
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08003F0C FUN_08003F0C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08004F30 FUN_08004F30, 0x08005274 FUN_08005274
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080042B8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080042BA: 3248     ldr      r0, [pc, #0xc8]  ; =RAM_2000017B */
    r0 = *(uint32_t *)0x08004384;   /* = RAM 0x2000017B */
}
