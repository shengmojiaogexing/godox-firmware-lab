/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005B28
 * Name      : FUN_08005B28
 * End       : 0x08005B2C
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08005B0C IRQ7_Handler  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080072AC FUN_080072AC, 0x0800CA68 FUN_0800CA68
 * MMIO      : EXTI.PR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005B28(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005B2A: 0120     movs     r0, #1 */
    r0 = 1;
}
