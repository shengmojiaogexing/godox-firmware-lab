/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800980E
 * Name      : FUN_0800980E
 * End       : 0x08009816
 * Size      : 8 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080097F8 FUN_080097F8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 3  -> 0x08005DFC FUN_08005DFC, 0x0800B588 FUN_0800B588, 0x0800C92C FUN_0800C92C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800980E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009810: 03f08cf8 bl       #0x800c92c */
    FUN_0800C92C();   /* BL -> 0x0800C92C FUN_0800C92C */
    /* 0x08009814: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
}
