/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08009E14
 * Name      : FUN_08009E14
 * End       : 0x08009E2E
 * Size      : 26 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=7)
 * Class     : function
 * Callers   : 1  -> 0x08009FC0 FUN_08009FC0
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08009E14(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08009E14: 3321     movs     r1, #0x33 */
    r1 = 0x33;
    /* 0x08009E16: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08009E18: 04d0     beq      #0x8009e24 */
    if (r0 == 0) goto L_08009E24;
    /* 0x08009E1A: 0928     cmp      r0, #9 */
    /* flags = cmp(r0, #9) */
    /* 0x08009E1C: 05d0     beq      #0x8009e2a */
    if (r0 == 9) goto L_08009E2A;
    /* 0x08009E1E: 1828     cmp      r0, #0x18 */
    /* flags = cmp(r0, #0x18) */
    /* 0x08009E20: 02d0     beq      #0x8009e28 */
    if (r0 == 0x18) goto L_08009E28;
    /* 0x08009E22: 02e0     b        #0x8009e2a */
    goto L_08009E2A;

    L_08009E24:
    /* 0x08009E24: 2921     movs     r1, #0x29 */
    r1 = 0x29;
    /* 0x08009E26: 00e0     b        #0x8009e2a */
    goto L_08009E2A;

    L_08009E28:
    /* 0x08009E28: 5a21     movs     r1, #0x5a */
    r1 = 0x5a;

    L_08009E2A:
    /* 0x08009E2A: 0846     mov      r0, r1 */
    r0 = r1;
    /* 0x08009E2C: 7047     bx       lr */
    return;
}
