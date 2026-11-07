/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009D5E
 * Name      : FUN_08009D5E
 * End       : 0x08009D62
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08009D28 FUN_08009D28  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800CA38 FUN_0800CA38
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009D5E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009D60: 0648     ldr      r0, [pc, #0x18]  ; =0x00000BB8 */
    r0 = *(uint32_t *)0x08009D7C;   /* = 0x00000BB8 */
}
