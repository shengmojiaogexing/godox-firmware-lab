/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080084CE
 * Name      : FUN_080084CE
 * End       : 0x080084D2
 * Size      : 4 bytes, 1 instructions decoded in body (functions.json insns=1, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x080083E8 FUN_080083E8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 2  -> 0x0800819C FUN_0800819C, 0x080086DC FUN_080086DC
 * MMIO      : RCC.APB1RSTR, RCC.APB2RSTR, SYS.+0x0, SYSTICK.VAL
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080084CE(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080084D0: 2576     strb     r5, [r4, #0x18] */
    *(uint8_t *)(r4 + 24) = r5;   /* store */  /* MMIO: SYSTICK.VAL(w) */
}
