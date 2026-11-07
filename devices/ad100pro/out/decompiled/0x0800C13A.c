/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C13A
 * Name      : FUN_0800C13A
 * End       : 0x0800C13E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800C080 FUN_0800C080  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080074F0 FUN_080074F0, 0x0800BDC4 FUN_0800BDC4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C13A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C13C: 2948     ldr      r0, [pc, #0xa4]  ; =RAM_20000384 */
    r0 = *(uint32_t *)0x0800C1E4;   /* = RAM 0x20000384 */
}
