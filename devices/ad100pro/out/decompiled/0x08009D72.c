/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009D72
 * Name      : FUN_08009D72
 * End       : 0x08009D84
 * Size      : 18 bytes, 8 instructions decoded in body (functions.json insns=8, blocks=1)
 * Class     : nested_block
 * Parent    : 0x08009D28 FUN_08009D28  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009D72(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009D74: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
    /* 0x08009D76: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08009D78: c401     lsls     r4, r0, #7 */
    r4 = r0 << 7;
    /* 0x08009D7A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009D7C: b80b     lsrs     r0, r7, #0xe */
    r0 = r7 >> 0xe;
    /* 0x08009D7E: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08009D80: bb01     lsls     r3, r7, #6 */
    r3 = r7 << 6;
    /* 0x08009D82: 0020     movs     r0, #0 */
    r0 = 0;
}
