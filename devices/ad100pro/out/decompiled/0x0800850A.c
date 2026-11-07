/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800850A
 * Name      : FUN_0800850A
 * End       : 0x0800850E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080083E8 FUN_080083E8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x080086DE FUN_080086DE
 * MMIO      : RCC.APB1RSTR, RCC.APB2RSTR, SYS.+0x0
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800850A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800850C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: SYS.+0x0(r) */
}
