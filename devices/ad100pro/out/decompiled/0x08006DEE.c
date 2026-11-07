/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08006DEE
 * Name      : FUN_08006DEE
 * End       : 0x08006DF2
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08006DDE FUN_08006DDE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08007434 delay_or_timing
 * MMIO      : RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08006DEE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08006DF0: 0027     movs     r7, #0 */
    r7 = 0;
}
