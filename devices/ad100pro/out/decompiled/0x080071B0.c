/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080071B0
 * Name      : FUN_080071B0
 * End       : 0x080071B4
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007154 FUN_08007154  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08005F34 FUN_08005F34
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080071B0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080071B2: 0e48     ldr      r0, [pc, #0x38]  ; =0x0000C350 */
    r0 = *(uint32_t *)0x080071EC;   /* = 0x0000C350 */
}
