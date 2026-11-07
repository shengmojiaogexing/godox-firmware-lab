/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C60A
 * Name      : FUN_0800C60A
 * End       : 0x0800C60E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800C4E8 FUN_0800C4E8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C60A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C60C: 1248     ldr      r0, [pc, #0x48]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x0800C658;   /* = 0x00000BB8 */
}
