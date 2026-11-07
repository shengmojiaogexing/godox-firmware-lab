/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CDE2
 * Name      : FUN_0800CDE2
 * End       : 0x0800CDE6
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800CDC8 FUN_0800CDC8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x0800CE58 FUN_0800CE58
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CDE2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CDE4: 3855     strb     r0, [r7, r4] */
    *(uint8_t *)(r7 + r4) = r0;   /* store */
}
