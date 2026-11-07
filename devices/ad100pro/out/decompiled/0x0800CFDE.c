/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CFDE
 * Name      : FUN_0800CFDE
 * End       : 0x0800CFE2
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800CF74 main  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 3  -> 0x080074F0 FUN_080074F0, 0x08007544 FUN_08007544, 0x0800AD10 FUN_0800AD10
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CFDE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CFE0: 2648     ldr      r0, [pc, #0x98]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800D07C;   /* = RAM 0x20000384 */
}
