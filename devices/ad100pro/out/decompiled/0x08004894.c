/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004894
 * Name      : FUN_08004894
 * End       : 0x08004898
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08004880 FUN_08004880  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800B758 FUN_0800B758
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004894(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004896: 8049     ldr      r1, [pc, #0x200]  ; =RAM_200001E3 */
    r1 = *(uint32_t *)0x08004A98;   /* = RAM 0x200001E3 */
}
