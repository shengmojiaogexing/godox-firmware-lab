/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003C66
 * Name      : FUN_08003C66
 * End       : 0x08003C6A
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08003C50 FUN_08003C50  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080067C0 FUN_080067C0, 0x0800AF88 FUN_0800AF88
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003C66(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003C68: 2449     ldr      r1, [pc, #0x90]  ; =RAM_200001B6 */
    r1 = *(uint32_t *)0x08003CFC;   /* = RAM 0x200001B6 */
}
