/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C90E
 * Name      : FUN_0800C90E
 * End       : 0x0800C91A
 * Size      : 12 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C90E(void)   /* args in r0-r3, return in r0 unless noted */
{

    L_0800C90E:
    /* 0x0800C90E: 08c8     ldm      r0!, {r3} */
    /* ldm r0!, {r3} */
    /* 0x0800C910: 121f     subs     r2, r2, #4 */
    r2 = r2 - 4;
    /* 0x0800C912: 08c1     stm      r1!, {r3} */
    /* stm r1!, {r3} */
    /* 0x0800C914: 002a     cmp      r2, #0 */
    /* flags = cmp(r2, #0) */
    /* 0x0800C916: fad1     bne      #0x800c90e */
    if (r2 != 0) goto L_0800C90E;
    /* 0x0800C918: 7047     bx       lr */
    return;
}
