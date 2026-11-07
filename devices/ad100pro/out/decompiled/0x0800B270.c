/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B270
 * Name      : FUN_0800B270
 * End       : 0x0800B272
 * Size      : 2 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800B26C FUN_0800B26C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B270(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B270: 2c48     ldr      r0, [pc, #0xb0]  ; =RAM_200001A2 */
    r0 = *(uint32_t *)0x0800B324;   /* = RAM 0x200001A2 */
}
