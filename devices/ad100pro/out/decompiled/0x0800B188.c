/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B188
 * Name      : FUN_0800B188
 * End       : 0x0800B18C
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800B0BE FUN_0800B0BE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800655C FUN_0800655C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B188(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B18A: 6249     ldr      r1, [pc, #0x188]  ; =RAM_200001DE */
    r1 = *(uint32_t *)0x0800B314;   /* = RAM 0x200001DE */
}
