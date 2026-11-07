/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x080032B0
 * Name      : FUN_080032B0
 * End       : 0x080032BC
 * Size      : 12 bytes, 6 instructions decoded in body (functions.json insns=6, blocks=2)
 * Class     : function
 * Callers   : 0  -> -
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_080032B0(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x080032B0: 30b4     push     {r4, r5} */
    push({r4, r5});
    /* 0x080032B2: 0124     movs     r4, #1 */
    r4 = 1;
    /* 0x080032B4: e407     lsls     r4, r4, #0x1f */
    r4 = r4 << 0x1f;
    /* 0x080032B6: 6340     eors     r3, r4 */
    r3 = r3 ^ r4;
    /* 0x080032B8: 30bc     pop      {r4, r5} */
    pop({r4, r5});
    /* 0x080032BA: 55e7     b        #0x8003168 */
    goto L_08003168;   /* loop */
}
