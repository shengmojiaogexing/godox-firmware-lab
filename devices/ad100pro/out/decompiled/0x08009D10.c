/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009D10
 * Name      : FUN_08009D10
 * End       : 0x08009D28
 * Size      : 24 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=1)
 * Class     : nested_block
 * Parent    : 0x08009CB4 FUN_08009CB4  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009D10(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009D12: 70bd     pop      {r4, r5, r6, pc} */
    pop({r4, r5, r6, pc});   /* return */
    /* 0x08009D14: c401     lsls     r4, r0, #7 */
    r4 = r0 << 7;
    /* 0x08009D16: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009D18: 6501     lsls     r5, r4, #5 */
    r5 = r4 << 5;
    /* 0x08009D1A: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009D1C: 3401     lsls     r4, r6, #4 */
    r4 = r6 << 4;
    /* 0x08009D1E: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08009D20: b80b     lsrs     r0, r7, #0xe */
    r0 = r7 >> 0xe;
    /* 0x08009D22: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08009D24: bb01     lsls     r3, r7, #6 */
    r3 = r7 << 6;
    /* 0x08009D26: 0020     movs     r0, #0 */
    r0 = 0;
}
