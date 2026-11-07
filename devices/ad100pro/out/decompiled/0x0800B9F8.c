/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B9F8
 * Name      : FUN_0800B9F8
 * End       : 0x0800B9FC
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800B9C0 FUN_0800B9C0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 4  -> 0x08004AF0 FUN_08004AF0, 0x0800ABE4 FUN_0800ABE4, 0x0800C080 FUN_0800C080, 0x0800D088 FUN_0800D088
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B9F8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B9FA: 0e49     ldr      r1, [pc, #0x38]  ; =RAM_200001DA */
    r1 = *(uint32_t *)0x0800BA34;   /* = RAM 0x200001DA */
}
