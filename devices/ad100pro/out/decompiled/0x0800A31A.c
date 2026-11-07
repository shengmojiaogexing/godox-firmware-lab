/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800A31A
 * Name      : FUN_0800A31A
 * End       : 0x0800A31E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800A30C FUN_0800A30C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800A480 FUN_0800A480
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800A31A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800A31C: 564c     ldr      r4, [pc, #0x158]  ; =RAM_2000022E */
    r4 = *(uint32_t *)0x0800A478;   /* = RAM 0x2000022E */
}
