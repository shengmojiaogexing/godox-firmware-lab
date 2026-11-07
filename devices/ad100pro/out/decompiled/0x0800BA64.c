/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800BA64
 * Name      : FUN_0800BA64
 * End       : 0x0800BA68
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800BA44 IRQ19_Handler  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080083E8 FUN_080083E8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800BA64(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800BA66: 0448     ldr      r0, [pc, #0x10]  ; =RAM_20000348 */
    r0 = *(uint32_t *)0x0800BA78;   /* = RAM 0x20000348 */
}
