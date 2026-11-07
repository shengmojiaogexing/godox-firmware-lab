/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080053F0
 * Name      : FUN_080053F0
 * End       : 0x080053F4
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080053E4 FUN_080053E4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08004C6C FUN_08004C6C
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080053F0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080053F2: 0248     ldr      r0, [pc, #8]  ; =RAM_20000183 */
    r0 = *(uint32_t *)0x080053FC;   /* = RAM 0x20000183 */
}
