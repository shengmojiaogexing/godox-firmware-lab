/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005712
 * Name      : FUN_08005712
 * End       : 0x08005716
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080056E4 FUN_080056E4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 3  -> 0x080030F0 FUN_080030F0, 0x08004E4C FUN_08004E4C, 0x08004E98 FUN_08004E98
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005712(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005714: 304e     ldr      r6, [pc, #0xc0]  ; =RAM_20000177 */
    r6 = *(uint32_t *)0x080057D8;   /* = RAM 0x20000177 */
}
