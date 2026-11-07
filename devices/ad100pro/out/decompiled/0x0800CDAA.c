/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CDAA
 * Name      : FUN_0800CDAA
 * End       : 0x0800CDAE
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800CD7C FUN_0800CD7C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : GPIOB.BRR, GPIOB.IDR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CDAA(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CDAC: 4006     lsls     r0, r0, #0x19 */
    r0 = r0 << 0x19;
}
