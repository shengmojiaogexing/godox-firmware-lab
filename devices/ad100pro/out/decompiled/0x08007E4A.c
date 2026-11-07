/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08007E4A
 * Name      : FUN_08007E4A
 * End       : 0x08007E4E
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007E34 FUN_08007E34  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : EXTI.FTSR, EXTI.PR, RCC.+0x1D, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08007E4A(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08007E4C: 2068     ldr      r0, [r4] */
    r0 = *(uint32_t *)(r4);  /* MMIO: RCC.CR(r) */
}
