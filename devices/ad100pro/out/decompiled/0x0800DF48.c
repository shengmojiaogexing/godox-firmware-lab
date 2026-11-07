/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800DF48
 * Name      : FUN_0800DF48
 * End       : 0x0800DF50
 * Size      : 8 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800DF38 FUN_0800DF38  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800DF48(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800DF48: 0380     strh     r3, [r0] */
    *(uint16_t *)(r0) = r3;   /* store */
    /* 0x0800DF4A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF4C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF4E: 0601     lsls     r6, r0, #4 */
    r6 = r0 << 4;
}
