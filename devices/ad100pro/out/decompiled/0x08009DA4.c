/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009DA4
 * Name      : FUN_08009DA4
 * End       : 0x08009DA8
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08009D84 FUN_08009D84  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x08005C40 FUN_08005C40, 0x080081E8 FUN_080081E8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009DA4(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009DA6: 401e     subs     r0, r0, #1 */
    r0 = r0 - 1;
}
