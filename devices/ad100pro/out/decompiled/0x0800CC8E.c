/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800CC8E
 * Name      : FUN_0800CC8E
 * End       : 0x0800CC94
 * Size      : 6 bytes, 2 instructions decoded in body (functions.json insns=2, blocks=1)
 * Class     : tiny_fragment
 * Parent    : 0x0800CAE8 FUN_0800CAE8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800CC8E(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800CC90: 38bd     pop      {r3, r4, r5, pc} */
    pop({r3, r4, r5, pc});   /* return */
    /* 0x0800CC92: 0000     movs     r0, r0 */
    r0 = r0;
}
