/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080075FA
 * Name      : FUN_080075FA
 * End       : 0x080075FE
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080075EC FUN_080075EC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08008164 FUN_08008164
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080075FA(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080075FC: 00f0b2fd bl       #0x8008164 */
    FUN_08008164();   /* BL -> 0x08008164 FUN_08008164 */
}
