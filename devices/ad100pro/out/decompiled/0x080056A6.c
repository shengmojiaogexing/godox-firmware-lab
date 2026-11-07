/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080056A6
 * Name      : FUN_080056A6
 * End       : 0x080056AC
 * Size      : 6 bytes, 3 instructions decoded in body (functions.json insns=4, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080056A6(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080056A6: 0127     movs     r7, #1 */
    r7 = 1;
    /* 0x080056A8: 0626     movs     r6, #6 */
    r6 = 6;
    /* 0x080056AA: b5e7     b        #0x8005618 */
    goto L_08005618;   /* loop */
}
