/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080061EA
 * Name      : FUN_080061EA
 * End       : 0x080061EE
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800610C FUN_0800610C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 3  -> 0x080030F0 FUN_080030F0, 0x0800341C FUN_0800341C, 0x080045E4 FUN_080045E4
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080061EA(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080061EC: 019b     ldr      r3, [sp, #4] */
    r3 = *(uint32_t *)(sp + 4);
}
