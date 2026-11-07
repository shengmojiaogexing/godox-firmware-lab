/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08004ADA
 * Name      : FUN_08004ADA
 * End       : 0x08004AF0
 * Size      : 22 bytes, 10 instructions decoded in body (functions.json insns=10, blocks=1)
 * Class     : nested_block
 * Parent    : 0x08004AA8 FUN_08004AA8  (this entry starts inside the parent's range; treat it as a sub-block, not a separate function)
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08004ADA(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08004ADC: f9e7     b        #0x8004ad2 */
    goto L_08004AD2;   /* loop */
    /* 0x08004ADE: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x08004AE0: 9801     lsls     r0, r3, #6 */
    r0 = r3 << 6;
    /* 0x08004AE2: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004AE4: ab01     lsls     r3, r5, #6 */
    r3 = r5 << 6;
    /* 0x08004AE6: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004AE8: b201     lsls     r2, r6, #6 */
    r2 = r6 << 6;
    /* 0x08004AEA: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x08004AEC: 8201     lsls     r2, r0, #6 */
    r2 = r0 << 6;
    /* 0x08004AEE: 0020     movs     r0, #0 */
    r0 = 0;
}
