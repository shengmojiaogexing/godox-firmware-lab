/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080074B6
 * Name      : FUN_080074B6
 * End       : 0x080074BA
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007440 FUN_08007440  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : EXTI.+0x11, EXTI.FTSR, EXTI.IMR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080074B6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080074B8: 401b     subs     r0, r0, r5 */
    r0 = r0 - r5;
}
