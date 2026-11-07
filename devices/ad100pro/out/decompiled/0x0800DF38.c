/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800DF38
 * Name      : FUN_0800DF38
 * End       : 0x0800DF68
 * Size      : 48 bytes, 22 instructions decoded in body (functions.json insns=18, blocks=3)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800DF38(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800DF38: 3c00     movs     r4, r7 */
    r4 = r7;
    /* 0x0800DF3A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF3C: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800DF3E: 0200     movs     r2, r0 */
    r2 = r0;
    /* 0x0800DF40: 0206     lsls     r2, r0, #0x18 */
    r2 = r0 << 0x18;
    /* 0x0800DF42: 021e     subs     r2, r0, #0 */
    r2 = r0 - 0;
    /* 0x0800DF44: 0278     ldrb     r2, [r0] */
    r2 = *(uint8_t *)(r0);
    /* 0x0800DF46: 03e0     b        #0x800df50 */
    goto L_0800DF50;
    /* 0x0800DF48: 0380     strh     r3, [r0] */
    *(uint16_t *)(r0) = r3;   /* store */
    /* 0x0800DF4A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF4C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF4E: 0601     lsls     r6, r0, #4 */
    r6 = r0 << 4;

    L_0800DF50:
    /* 0x0800DF50: dc03     lsls     r4, r3, #0xf */
    r4 = r3 << 0xf;
    /* 0x0800DF52: fe02     lsls     r6, r7, #0xb */
    r6 = r7 << 0xb;
    /* 0x0800DF54: 2202     lsls     r2, r4, #8 */
    r2 = r4 << 8;
    /* 0x0800DF56: 2203     lsls     r2, r4, #0xc */
    r2 = r4 << 0xc;
    /* 0x0800DF58: fe01     lsls     r6, r7, #7 */
    r6 = r7 << 7;
    /* 0x0800DF5A: dc00     lsls     r4, r3, #3 */
    r4 = r3 << 3;
    /* 0x0800DF5C: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF5E: 0006     lsls     r0, r0, #0x18 */
    r0 = r0 << 0x18;
    /* 0x0800DF60: 01e0     b        #0x800df66 */
    goto L_0800DF66;

    L_0800DF66:
    /* 0x0800DF66: 0216     asrs     r2, r0, #0x18 */
    r2 = r0 >> 0x18;
}
