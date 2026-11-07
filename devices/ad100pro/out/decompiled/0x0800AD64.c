/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800AD64
 * Name      : FUN_0800AD64
 * End       : 0x0800AD68
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800AD44 FUN_0800AD44  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 3  -> 0x08004B74 FUN_08004B74, 0x08005DFC FUN_08005DFC, 0x0800CE6C FUN_0800CE6C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800AD64(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800AD66: 7848     ldr      r0, [pc, #0x1e0]  ; =RAM_20000434 */
    r0 = *(uint32_t *)0x0800AF48;   /* = RAM 0x20000434 */
}
