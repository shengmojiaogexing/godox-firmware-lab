/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800ACA8
 * Name      : FUN_0800ACA8
 * End       : 0x0800ACBE
 * Size      : 22 bytes, 11 instructions decoded in body (functions.json insns=11, blocks=4)
 * Class     : hub
 * Callers   : 8  -> 0x08007F68 FUN_08007F68, 0x08007F8A FUN_08007F8A, 0x08007FB4 FUN_08007FB4, 0x08007FBC FUN_08007FBC, 0x08008030 FUN_08008030, 0x08008056 FUN_08008056, 0x08008068 FUN_08008068, 0x08008070 FUN_08008070
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800ACA8(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800ACA8: 0021     movs     r1, #0 */
    r1 = 0;
    /* 0x0800ACAA: 02e0     b        #0x800acb2 */
    goto L_0800ACB2;

    L_0800ACAC:
    /* 0x0800ACAC: 0a38     subs     r0, #0xa */
    r0 = r0 - 0xa;
    /* 0x0800ACAE: c0b2     uxtb     r0, r0 */
    r0 = uxtb(r0);
    /* 0x0800ACB0: 491c     adds     r1, r1, #1 */
    r1 = r1 + 1;

    L_0800ACB2:
    /* 0x0800ACB2: 0a28     cmp      r0, #0xa */
    /* flags = cmp(r0, #0xa) */
    /* 0x0800ACB4: fad2     bhs      #0x800acac */
    if (r0 >= 0xa) goto L_0800ACAC;   /* unsigned */
    /* 0x0800ACB6: 0907     lsls     r1, r1, #0x1c */
    r1 = r1 << 0x1c;
    /* 0x0800ACB8: 090e     lsrs     r1, r1, #0x18 */
    r1 = r1 >> 0x18;
    /* 0x0800ACBA: 0843     orrs     r0, r1 */
    r0 = r0 | r1;
    /* 0x0800ACBC: 7047     bx       lr */
    return;
}
