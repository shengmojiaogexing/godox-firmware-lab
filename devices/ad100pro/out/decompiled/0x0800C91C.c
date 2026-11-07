/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C91C
 * Name      : FUN_0800C91C
 * End       : 0x0800C92A
 * Size      : 14 bytes, 7 instructions decoded in body (functions.json insns=7, blocks=4)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C91C(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C91C: 0020     movs     r0, #0 */
    r0 = 0;
    /* 0x0800C91E: 01e0     b        #0x800c924 */
    goto L_0800C924;

    L_0800C920:
    /* 0x0800C920: 01c1     stm      r1!, {r0} */
    /* stm r1!, {r0} */
    /* 0x0800C922: 121f     subs     r2, r2, #4 */
    r2 = r2 - 4;

    L_0800C924:
    /* 0x0800C924: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800C926: fbd1     bne      #0x800c920 */
    if (r2 != 0) goto L_0800C920;
    /* 0x0800C928: 7047     bx       lr */
    return;
}
