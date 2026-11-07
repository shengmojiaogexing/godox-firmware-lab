/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800ABD8
 * Name      : FUN_0800ABD8
 * End       : 0x0800ABE4
 * Size      : 12 bytes, 5 instructions decoded in body (functions.json insns=5, blocks=1)
 * Class     : nested_block
 * Parent    : 0x0800AB7C FUN_0800AB7C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800ABD8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800ABDA: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x0800ABDC: 7c01     lsls     r4, r7, #5 */
    r4 = r7 << 5;
    /* 0x0800ABDE: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800ABE0: 8301     lsls     r3, r0, #6 */
    r3 = r0 << 6;
    /* 0x0800ABE2: 0020     movs     r0, #0 */
    r0 = 0;
}
