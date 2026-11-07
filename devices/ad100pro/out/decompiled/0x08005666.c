/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005666
 * Name      : FUN_08005666
 * End       : 0x0800566A
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080055FE FUN_080055FE  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08004D28 FUN_08004D28
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005666(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005668: 0098     ldr      r0, [sp] */
    r0 = *(uint32_t *)(sp);
}
