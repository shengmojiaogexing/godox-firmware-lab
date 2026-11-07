/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005E14
 * Name      : FUN_08005E14
 * End       : 0x08005E24
 * Size      : 16 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=1)
 * Class     : nested_block
 * Parent    : 0x08005DFC FUN_08005DFC  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005E14(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005E16: 10bd     pop      {r4, pc} */
    pop({r4, pc});   /* return */
    /* 0x08005E18: 7404     lsls     r4, r6, #0x11 */
    r4 = r6 << 0x11;
    /* 0x08005E1A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08005E1C: 00f00108 and      r8, r0, #1 */
    r8 = r0 & 1;
    /* 0x08005E20: 2c01     lsls     r4, r5, #4 */
    r4 = r5 << 4;
    /* 0x08005E22: 0020     movs     r0, #0 */
    r0 = 0;
}
