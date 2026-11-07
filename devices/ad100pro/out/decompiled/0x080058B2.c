/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080058B2
 * Name      : FUN_080058B2
 * End       : 0x080058C4
 * Size      : 18 bytes, 9 instructions decoded in body (functions.json insns=35, blocks=9)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080058B2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080058B2: 0826     movs     r6, #8 */
    r6 = 8;
    /* 0x080058B4: 04e0     b        #0x80058c0 */
    goto L_080058C0;
    /* 0x080058B6: 0426     movs     r6, #4 */
    r6 = 4;
    /* 0x080058B8: 02e0     b        #0x80058c0 */
    goto L_080058C0;
    /* 0x080058BA: 0226     movs     r6, #2 */
    r6 = 2;
    /* 0x080058BC: 00e0     b        #0x80058c0 */
    goto L_080058C0;
    /* 0x080058BE: 0126     movs     r6, #1 */
    r6 = 1;

    L_080058C0:
    /* 0x080058C0: 0025     movs     r5, #0 */
    r5 = 0;
    /* 0x080058C2: cce7     b        #0x800585e */
    goto L_0800585E;   /* loop */
}
