/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08008A52
 * Name      : FUN_08008A52
 * End       : 0x08008A5C
 * Size      : 10 bytes, 4 instructions decoded in body (functions.json insns=4, blocks=1)
 * Class     : nested_block
 * Parent    : 0x08008A2C FUN_08008A2C  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08008A52(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08008A54: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x08008A56: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08008A58: 9803     lsls     r0, r3, #0xe */
    r0 = r3 << 0xe;
    /* 0x08008A5A: 0020     movs     r0, #0 */
    r0 = 0;
}
