/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BE2A
 * Name      : FUN_0800BE2A
 * End       : 0x0800BE2E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800BDC4 FUN_0800BDC4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08004C0C FUN_08004C0C, 0x08004EE4 FUN_08004EE4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BE2A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BE2C: 8a4f     ldr      r7, [pc, #0x228]  ; =RAM_200001ED */
    r7 = *(uint32_t *)0x0800C058;   /* = RAM 0x200001ED */
}
