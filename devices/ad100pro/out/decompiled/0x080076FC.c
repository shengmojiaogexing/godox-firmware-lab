/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080076FC
 * Name      : FUN_080076FC
 * End       : 0x08007700
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x08007678 FUN_08007678  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * MMIO      : EXTI.+0x18, EXTI.EMR, EXTI.FTSR, EXTI.IMR, EXTI.PR, EXTI.RTSR, EXTI.SWIER, RCC.APB1ENR, RCC.BDCR, RCC.CFGR3
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080076FC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080076FE: 0099     ldr      r1, [sp] */
    r1 = *(uint32_t *)(sp);
}
