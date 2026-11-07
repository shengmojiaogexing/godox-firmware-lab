/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007470
 * Name      : FUN_08007470
 * End       : 0x08007474
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007440 FUN_08007440  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : EXTI.+0x11, EXTI.EMR, EXTI.FTSR, EXTI.IMR, EXTI.RTSR, EXTI.SWIER
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007470(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007472: 0220     movs     r0, #2 */
    r0 = 2;
}
