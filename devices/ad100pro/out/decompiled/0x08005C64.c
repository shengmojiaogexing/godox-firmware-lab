/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005C64
 * Name      : FUN_08005C64
 * End       : 0x08005C68
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08005C44 FUN_08005C44  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080067C0 FUN_080067C0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005C64(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005C66: 1349     ldr      r1, [pc, #0x4c]  ; =RAM_200001C4 */
    r1 = *(uint32_t *)0x08005CB4;   /* = RAM 0x200001C4 */
}
