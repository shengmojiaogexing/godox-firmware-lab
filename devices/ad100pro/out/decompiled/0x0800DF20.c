/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800DF20
 * Name      : FUN_0800DF20
 * End       : 0x0800DF38
 * Size      : 24 bytes, 12 instructions decoded in body (functions.json insns=12, blocks=1)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800DF20(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800DF20: 0242     tst      r2, r0 */
    /* flags = tst(r2, r0) */
    /* 0x0800DF22: 0242     tst      r2, r0 */
    /* flags = tst(r2, r0) */
    /* 0x0800DF24: 027e     ldrb     r2, [r0, #0x18] */
    r2 = *(uint8_t *)(r0 + 24);
    /* 0x0800DF26: 023c     subs     r4, #2 */
    r4 = r4 - 2;
    /* 0x0800DF28: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF2A: 0000     movs     r0, r0 */
    r0 = r0;
    /* 0x0800DF2C: 0600     movs     r6, r0 */
    r6 = r0;
    /* 0x0800DF2E: fc01     lsls     r4, r7, #7 */
    r4 = r7 << 7;
    /* 0x0800DF30: fe03     lsls     r6, r7, #0xf */
    r6 = r7 << 0xf;
    /* 0x0800DF32: 4202     lsls     r2, r0, #9 */
    r2 = r0 << 9;
    /* 0x0800DF34: 4202     lsls     r2, r0, #9 */
    r2 = r0 << 9;
    /* 0x0800DF36: 7e00     lsls     r6, r7, #1 */
    r6 = r7 << 1;
}
