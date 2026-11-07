/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003B22
 * Name      : FUN_08003B22
 * End       : 0x08003B26
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08003AC8 FUN_08003AC8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080033F4 FUN_080033F4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003B22(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003B24: 194a     ldr      r2, [pc, #0x64]  ; =0x66666666 */
    r2 = *(uint32_t *)0x08003B8C;   /* = 0x66666666 */
}
