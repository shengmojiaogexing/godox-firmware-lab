/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800486E
 * Name      : FUN_0800486E
 * End       : 0x08004872
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080047CC FUN_080047CC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080064E8 FUN_080064E8
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800486E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004870: 01f03afe bl       #0x80064e8 */
    FUN_080064E8();   /* BL -> 0x080064E8 FUN_080064E8 */
}
