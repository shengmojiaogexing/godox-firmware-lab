/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800963C
 * Name      : FUN_0800963C
 * End       : 0x08009640
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080095F8 FUN_080095F8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 3  -> 0x08005C40 FUN_08005C40, 0x08007D58 FUN_08007D58, 0x08007F68 FUN_08007F68
 * MMIO      : RTC.+0x15, RTC.+0x16, RTC.+0x17, RTC.WUTR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800963C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800963E: 0120     movs     r0, #1 */
    r0 = 1;
}
