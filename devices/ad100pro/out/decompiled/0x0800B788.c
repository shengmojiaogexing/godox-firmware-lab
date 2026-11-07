/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B788
 * Name      : FUN_0800B788
 * End       : 0x0800B790
 * Size      : 8 bytes, 3 instructions decoded in body (functions.json insns=3, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800B758 FUN_0800B758  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B788(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B78A: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x0800B78C: 68c3     stm      r3!, {r3, r5, r6} */
    /* stm r3!, {r3, r5, r6} */
    /* 0x0800B78E: 0000     movs     r0, r0 */
    r0 = r0;
}
