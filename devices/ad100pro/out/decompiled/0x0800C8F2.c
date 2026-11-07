/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x0800C8F2
 * Name      : FUN_0800C8F2
 * End       : 0x0800C90C
 * Size      : 26 bytes, 13 instructions decoded in body (functions.json insns=13, blocks=3)
 * Class     : function
 * Callers   : 5  -> 0x0800557C FUN_0800557C, 0x080055C0 FUN_080055C0, 0x080057E4 FUN_080057E4, 0x08005806 FUN_08005806, 0x0800AFC4 FUN_0800AFC4
 * Callees   : 0  -> -
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_0800C8F2(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x0800C8F2: 30b4     push     {r4, r5} */
    push({r4, r5});
    /* 0x0800C8F4: 7446     mov      r4, lr */
    r4 = lr;
    /* 0x0800C8F6: 641e     subs     r4, r4, #1 */
    r4 = r4 - 1;
    /* 0x0800C8F8: 2578     ldrb     r5, [r4] */
    r5 = *(uint8_t *)(r4);
    /* 0x0800C8FA: 641c     adds     r4, r4, #1 */
    r4 = r4 + 1;
    /* 0x0800C8FC: ab42     cmp      r3, r5 */
    /* flags = cmp(r3, r5) */
    /* 0x0800C8FE: 00d2     bhs      #0x800c902 */
    if (r3 >= r5) goto L_0800C902;   /* unsigned */
    /* 0x0800C900: 1d46     mov      r5, r3 */
    r5 = r3;

    L_0800C902:
    /* 0x0800C902: 635d     ldrb     r3, [r4, r5] */
    r3 = *(uint8_t *)(r4 + r5);
    /* 0x0800C904: 5b00     lsls     r3, r3, #1 */
    r3 = r3 << 1;
    /* 0x0800C906: e318     adds     r3, r4, r3 */
    r3 = r4 + r3;
    /* 0x0800C908: 30bc     pop      {r4, r5} */
    pop({r4, r5});
    /* 0x0800C90A: 1847     bx       r3 */
    goto *r3;   /* tail branch  */
}
