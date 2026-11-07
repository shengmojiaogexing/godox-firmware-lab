/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C398
 * Name      : FUN_0800C398
 * End       : 0x0800C39C
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800C2B8 FUN_0800C2B8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08004C5C FUN_08004C5C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C398(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C39A: 244d     ldr      r5, [pc, #0x90]  ; =RAM_20000480 */
    r5 = *(uint32_t *)0x0800C42C;   /* = RAM 0x20000480 */
}
