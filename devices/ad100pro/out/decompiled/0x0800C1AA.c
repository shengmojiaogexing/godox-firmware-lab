/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C1AA
 * Name      : FUN_0800C1AA
 * End       : 0x0800C1AE
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800C080 FUN_0800C080  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080074F0 FUN_080074F0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C1AA(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C1AC: 0d48     ldr      r0, [pc, #0x34]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800C1E4;   /* = RAM 0x20000384 */
}
