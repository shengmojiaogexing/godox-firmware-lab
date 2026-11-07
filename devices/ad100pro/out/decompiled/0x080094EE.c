/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080094EE
 * Name      : FUN_080094EE
 * End       : 0x080094F2
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800941C FUN_0800941C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080072C4 FUN_080072C4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080094EE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080094F0: 2e48     ldr      r0, [pc, #0xb8]  ; =0x00000602 */
    r0 = *(uint32_t *)0x080095AC;   /* = 0x00000602 */
}
