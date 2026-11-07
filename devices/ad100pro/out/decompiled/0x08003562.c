/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08003562
 * Name      : FUN_08003562
 * End       : 0x0800356C
 * Size      : 10 bytes, 1 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08003548 __libc_init_array  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08003562(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08003568: 0cf40008 and      r8, ip, #0x800000 */
    r8 = ip & 0x800000;
    /* data 0x08003564: .word 0x0800F3EC flash_0800F3EC   used by 08003548 */
}
