/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D11C
 * Name      : FUN_0800D11C
 * End       : 0x0800D120
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800D108 FUN_0800D108  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : EXTI.IMR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D11C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D11E: 0448     ldr      r0, [pc, #0x10]  ; =EXTI.IMR */
    r0 = *(uint32_t *)0x0800D130;   /* = MMIO EXTI+0x00 */
}
