/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C38C
 * Name      : FUN_0800C38C
 * End       : 0x0800C390
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800C2B8 FUN_0800C2B8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08004FC4 FUN_08004FC4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C38C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C38E: 2648     ldr      r0, [pc, #0x98]  ; =RAM_200001CE */
    r0 = *(uint32_t *)0x0800C428;   /* = RAM 0x200001CE */
}
