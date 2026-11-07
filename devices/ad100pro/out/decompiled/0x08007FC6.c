/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007FC6
 * Name      : FUN_08007FC6
 * End       : 0x08007FCA
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007F68 FUN_08007F68  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800ACBE FUN_0800ACBE
 * MMIO      : RCC.CR, RTC.WPR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007FC6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007FC8: 0543     orrs     r5, r0 */
    r5 = r5 | r0;
}
