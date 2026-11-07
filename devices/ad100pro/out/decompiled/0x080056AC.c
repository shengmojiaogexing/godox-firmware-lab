/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080056AC
 * Name      : FUN_080056AC
 * End       : 0x080056BE
 * Size      : 18 bytes, 6 instructions decoded in body (functions.json insns=4, blocks=3)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080056AC(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080056AC: 0827     movs     r7, #8 */
    r7 = 8;
    /* 0x080056AE: 04e0     b        #0x80056ba */
    goto L_080056BA;
    /* 0x080056B0: 0427     movs     r7, #4 */
    r7 = 4;
    /* 0x080056B2: 02e0     b        #0x80056ba */
    goto L_080056BA;

    L_080056BA:
    /* 0x080056BA: 0026     movs     r6, #0 */
    r6 = 0;
    /* 0x080056BC: ace7     b        #0x8005618 */
    goto L_08005618;   /* loop */
}
