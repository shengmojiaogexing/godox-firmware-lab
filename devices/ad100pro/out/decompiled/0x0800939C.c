/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800939C
 * Name      : FUN_0800939C
 * End       : 0x080093A8
 * Size      : 12 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : nested_block
 * Parent    : 0x08009310 FUN_08009310  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800939C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800939E: 0ebd     pop      {r1, r2, r3, pc} */
    pop({r1, r2, r3, pc});   /* return */
    /* 0x080093A0: 0024     movs     r4, #0 */
    r4 = 0;
    /* 0x080093A2: 0140     ands     r1, r0 */
    r1 = r1 & r0;
    /* 0x080093A4: ac03     lsls     r4, r5, #0xe */
    r4 = r5 << 0xe;
    /* 0x080093A6: 0020     movs     r0, #0 */
    r0 = 0;
}
