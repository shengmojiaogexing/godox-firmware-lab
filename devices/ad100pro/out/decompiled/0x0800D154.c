/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800D154
 * Name      : FUN_0800D154
 * End       : 0x0800D158
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800D134 FUN_0800D134  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 1  -> 0x08009038 FUN_08009038
 * MMIO      : GPIOB.+0x1, GPIOB.+0x2, GPIOB.MODER, RCC.+0x1, RCC.+0x2, RCC.CR
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800D154(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800D156: 0a20     movs     r0, #0xa */
    r0 = 0xa;
}
