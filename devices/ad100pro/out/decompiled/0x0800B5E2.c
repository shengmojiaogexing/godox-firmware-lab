/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800B5E2
 * Name      : FUN_0800B5E2
 * End       : 0x0800B5EC
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800B5C0 FUN_0800B5C0  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800B5E2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800B5E4: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x0800B5E6: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800B5E8: cc04     lsls     r4, r1, #0x13 */
    r4 = r1 << 0x13;
    /* 0x0800B5EA: 0020     movs     r0, #0 */
    r0 = 0;
}
