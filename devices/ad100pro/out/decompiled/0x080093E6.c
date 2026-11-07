/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080093E6
 * Name      : FUN_080093E6
 * End       : 0x080093F4
 * Size      : 14 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : nested_block
 * Parent    : 0x080093A8 FUN_080093A8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080093E6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080093E8: 1cbd     pop      {r2, r3, r4, pc} */
    pop({r2, r3, r4, pc});   /* return */
    /* 0x080093EA: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x080093EC: 0074     strb     r0, [r0, #0x10] */
    *(uint8_t *)(r0 + 16) = r0;   /* store */
    /* 0x080093EE: 0040     ands     r0, r0 */
    r0 = r0 & r0;
    /* 0x080093F0: 6002     lsls     r0, r4, #9 */
    r0 = r4 << 9;
    /* 0x080093F2: 0020     movs     r0, #0 */
    r0 = 0;
}
