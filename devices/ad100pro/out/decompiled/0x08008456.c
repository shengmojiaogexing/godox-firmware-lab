/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008456
 * Name      : FUN_08008456
 * End       : 0x0800845A
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080083E8 FUN_080083E8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x080083E6 FUN_080083E6, 0x08008688 FUN_08008688
 * MMIO      : RCC.APB1ENR, RCC.APB1RSTR, RCC.APB2RSTR, SYS.+0x0, SYSTICK.VAL
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008456(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008458: 2046     mov      r0, r4 */
    r0 = r4;
}
