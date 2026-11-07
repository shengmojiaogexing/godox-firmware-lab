/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B658
 * Name      : FUN_0800B658
 * End       : 0x0800B65C
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800B61C FUN_0800B61C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08009BCC FUN_08009BCC
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B658(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B65A: 0e48     ldr      r0, [pc, #0x38]  ; =RAM_20000165 */
    r0 = *(uint32_t *)0x0800B694;   /* = RAM 0x20000165 */
}
