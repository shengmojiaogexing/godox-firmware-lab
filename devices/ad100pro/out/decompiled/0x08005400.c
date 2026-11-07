/*
 * Godox AD100 Pro V1.2 - pseudo-decompiled C (capstone fallback)
 * ---------------------------------------------------------------------
 * Address   : 0x08005400
 * Name      : FUN_08005400
 * End       : 0x08005464
 * Size      : 100 bytes, 46 instructions decoded in body (functions.json insns=46, blocks=14)
 * Class     : function
 * Callers   : 2  -> 0x0800C2B8 FUN_0800C2B8, 0x0800C3D4 FUN_0800C3D4
 * Callees   : 2  -> 0x08004E4C FUN_08004E4C, 0x08004E98 FUN_08004E98
 * ---------------------------------------------------------------------
 * This file is a mechanical translation of the Thumb-1 disassembly, not
 * a true decompilation.  Register names r0-r7/sp/lr/pc are kept verbatim;
 * every statement is preceded by its original instruction comment.
 */

void FUN_08005400(void)   /* args in r0-r3, return in r0 unless noted */
{
    /* 0x08005400: 30b5     push     {r4, r5, lr} */
    push({r4, r5, lr});
    /* 0x08005402: 0221     movs     r1, #2 */
    r1 = 2;
    /* 0x08005404: 0028     cmp      r0, #0 */
    /* flags = cmp(r0, #0) */
    /* 0x08005406: 21d0     beq      #0x800544c */
    if (r0 == 0) goto L_0800544C;
    /* 0x08005408: 0128     cmp      r0, #1 */
    /* flags = cmp(r0, #1) */
    /* 0x0800540A: 22d0     beq      #0x8005452 */
    if (r0 == 1) goto L_08005452;
    /* 0x0800540C: 0228     cmp      r0, #2 */
    /* flags = cmp(r0, #2) */
    /* 0x0800540E: 22d0     beq      #0x8005456 */
    if (r0 == 2) goto L_08005456;
    /* 0x08005410: 0328     cmp      r0, #3 */
    /* flags = cmp(r0, #3) */
    /* 0x08005412: 22d0     beq      #0x800545a */
    if (r0 == 3) goto L_0800545A;
    /* 0x08005414: 0428     cmp      r0, #4 */
    /* flags = cmp(r0, #4) */
    /* 0x08005416: 22d0     beq      #0x800545e */
    if (r0 == 4) goto L_0800545E;
    /* 0x08005418: 0022     movs     r2, #0 */
    r2 = 0;

    L_0800541A:
    /* 0x0800541A: 0024     movs     r4, #0 */
    r4 = 0;

    L_0800541C:
    /* 0x0800541C: 0725     movs     r5, #7 */
    r5 = 7;
    /* 0x0800541E: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x08005420: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08005422: fff739fd bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x08005426: 0821     movs     r1, #8 */
    r1 = 8;
    /* 0x08005428: 0223     movs     r3, #2 */
    r3 = 2;
    /* 0x0800542A: 2246     mov      r2, r4 */
    r2 = r4;
    /* 0x0800542C: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800542E: fff733fd bl       #0x8004e98 */
    FUN_08004E98();   /* BL -> 0x08004E98 FUN_08004E98 */
    /* 0x08005432: 1021     movs     r1, #0x10 */
    r1 = 0x10;
    /* 0x08005434: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x08005436: 3022     movs     r2, #0x30 */
    r2 = 0x30;
    /* 0x08005438: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x0800543A: fff707fd bl       #0x8004e4c */
    FUN_08004E4C();   /* BL -> 0x08004E4C FUN_08004E4C */
    /* 0x0800543E: 1621     movs     r1, #0x16 */
    r1 = 0x16;
    /* 0x08005440: 0123     movs     r3, #1 */
    r3 = 1;
    /* 0x08005442: 3022     movs     r2, #0x30 */
    r2 = 0x30;
    /* 0x08005444: 2846     mov      r0, r5 */
    r0 = r5;
    /* 0x08005446: fff701fd bl       #0x8004e4c */
    FUN_08004E4C();   /* BL -> 0x08004E4C FUN_08004E4C */
    /* 0x0800544A: 30bd     pop      {r4, r5, pc} */
    pop({r4, r5, pc});   /* return */

    L_0800544C:
    /* 0x0800544C: 0222     movs     r2, #2 */
    r2 = 2;
    /* 0x0800544E: 0824     movs     r4, #8 */
    r4 = 8;
    /* 0x08005450: e4e7     b        #0x800541c */
    goto L_0800541C;   /* loop */

    L_08005452:
    /* 0x08005452: 0322     movs     r2, #3 */
    r2 = 3;
    /* 0x08005454: 04e0     b        #0x8005460 */
    goto L_08005460;

    L_08005456:
    /* 0x08005456: 0522     movs     r2, #5 */
    r2 = 5;
    /* 0x08005458: dfe7     b        #0x800541a */
    goto L_0800541A;   /* loop */

    L_0800545A:
    /* 0x0800545A: 0722     movs     r2, #7 */
    r2 = 7;
    /* 0x0800545C: dde7     b        #0x800541a */
    goto L_0800541A;   /* loop */

    L_0800545E:
    /* 0x0800545E: 0822     movs     r2, #8 */
    r2 = 8;

    L_08005460:
    /* 0x08005460: 0524     movs     r4, #5 */
    r4 = 5;
    /* 0x08005462: dbe7     b        #0x800541c */
    goto L_0800541C;   /* loop */
}
