/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800807A
 * Name      : FUN_0800807A
 * End       : 0x0800807E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08008030 FUN_08008030  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08008114 FUN_08008114, 0x0800ACBE FUN_0800ACBE
 * MMIO      : DMA1.CH2_None, RCC.+0x1, RCC.+0x1D, RCC.+0x2, RCC.+0x3, RCC.APB1ENR, RCC.APB1RSTR, RCC.APB2RSTR, RCC.CR, RTC.CR, RTC.WPR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800807A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800807C: 0643     orrs     r6, r0 */
    r6 = r6 | r0;
}
